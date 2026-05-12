#!/usr/bin/env python3
"""
uart_sender.py - reads PS5 controller state and sends commands to STM32 via UART
Command format: <CMD:VALUE>\n  e.g. "CROSS:1\n", "LSX:0.75\n", "GPS_SPEED:42.3\n"
Handles UART disconnection and controller disconnection gracefully.
"""

import sys
import time
import serial
from pathlib import Path

_ROOT = Path(__file__).resolve().parent.parent
sys.path.append(str(_ROOT / 'ps5'))
sys.path.append(str(_ROOT))

from ps5_controller import PS5Controller
from gps.gps_reader import GPSReader

# UART config
UART_PORT        = '/dev/ttyAMA0'
UART_BAUDRATE    = 115200
UART_RECONNECT   = 5.0  # seconds between UART reconnection attempts

# How often to send state [seconds]
SEND_INTERVAL = 0.05


def format_commands(state):
    """Convert controller state to list of UART command strings."""
    commands = []

    for stick, key in (("LSX", "left_stick_x"), ("LSY", "left_stick_y"),
                       ("RSX", "right_stick_x"), ("RSY", "right_stick_y")):
        if state[key] != 0.0:
            commands.append(f"{stick}:{state[key]:.2f}")

    for trigger, key in (("L2", "l2"), ("R2", "r2")):
        if state[key] > 0.0:
            commands.append(f"{trigger}:{state[key]:.2f}")

    for btn in ('cross', 'circle', 'triangle', 'square',
                'l1', 'r1', 'l3', 'r3', 'share', 'options', 'ps'):
        if state[btn]:
            commands.append(f"{btn.upper()}:1")

    for dpad in ('dpad_up', 'dpad_down', 'dpad_left', 'dpad_right'):
        if state[dpad]:
            commands.append(f"{dpad.upper()}:1")

    return commands


def open_uart():
    """Try to open UART port. Returns serial object or None on failure."""
    try:
        ser = serial.Serial(UART_PORT, UART_BAUDRATE, timeout=1)
        print(f"UART connected: {UART_PORT} @ {UART_BAUDRATE}")
        return ser
    except serial.SerialException as e:
        print(f"UART connection failed: {e}")
        return None


def main():
    controller = PS5Controller()
    controller.connect()

    gps = GPSReader()
    gps.start()

    ser = open_uart()
    last_uart_attempt = time.monotonic()

    print("Sending controller commands + GPS speed to STM32. Press Ctrl+C to exit.\n")

    try:
        while True:
            # Reconnect UART if disconnected
            if ser is None:
                now = time.monotonic()
                if now - last_uart_attempt >= UART_RECONNECT:
                    last_uart_attempt = now
                    print("Attempting to reconnect UART...")
                    ser = open_uart()

            controller.update()

            # Only send if both controller and UART are connected
            if controller.connected and ser is not None:
                commands = format_commands(controller.state)
                commands.append(f"GPS_SPEED:{gps.get_speed_kmh():.1f}")

                for cmd in commands:
                    try:
                        ser.write(f"{cmd}\n".encode())
                        print(f"[SENT] {cmd}")
                    except serial.SerialException as e:
                        print(f"UART write error: {e}")
                        ser = None
                        last_uart_attempt = time.monotonic()
                        break

            time.sleep(SEND_INTERVAL)

    except KeyboardInterrupt:
        print("\nExiting.")
    finally:
        gps.stop()
        if ser is not None:
            ser.close()
        controller.disconnect()


if __name__ == "__main__":
    main()
