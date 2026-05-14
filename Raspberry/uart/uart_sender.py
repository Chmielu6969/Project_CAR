#!/usr/bin/env python3
"""
uart_sender.py - reads PS5 controller state and sends commands to STM32 via UART
Command format: <CMD:VALUE>\n  e.g. "CROSS:1\n", "LSX:0.75\n"
Buttons are edge-based: CROSS:1 on press, CROSS:0 on release.
Axes/triggers are sent only when non-zero.
Handles UART disconnection and controller disconnection gracefully.
"""

import sys
import time
import serial
from pathlib import Path

# Add ps5 directory to path using absolute path based on this file's location
sys.path.append(str(Path(__file__).resolve().parent.parent / 'ps5'))
from ps5_controller import PS5Controller

# UART config
UART_PORT        = '/dev/ttyAMA0'
UART_BAUDRATE    = 115200
UART_RECONNECT   = 5.0  # seconds between UART reconnection attempts

# How often to poll controller state [seconds]
SEND_INTERVAL = 0.05

_BUTTONS = ('cross', 'circle', 'triangle', 'square',
            'l1', 'r1', 'l3', 'r3', 'share', 'options', 'ps')
_DPADS   = ('dpad_up', 'dpad_down', 'dpad_left', 'dpad_right')


def format_commands(state, prev_state):
    """
    Convert controller state to list of UART command strings.
    Axes/triggers: sent when non-zero.
    Buttons/D-pad: edge-based – sent only when state changes (press=1, release=0).
    """
    commands = []

    for stick, key in (("LSX", "left_stick_x"), ("LSY", "left_stick_y"),
                       ("RSX", "right_stick_x"), ("RSY", "right_stick_y")):
        if state[key] != 0.0:
            commands.append(f"{stick}:{state[key]:.2f}")

    for trigger, key in (("L2", "l2"), ("R2", "r2")):
        if state[key] > 0.0:
            commands.append(f"{trigger}:{state[key]:.2f}")

    for btn in _BUTTONS:
        cur  = state[btn]
        prev = prev_state.get(btn, False)
        if cur != prev:
            commands.append(f"{btn.upper()}:{'1' if cur else '0'}")

    for dpad in _DPADS:
        cur  = state[dpad]
        prev = prev_state.get(dpad, False)
        if cur != prev:
            commands.append(f"{dpad.upper()}:{'1' if cur else '0'}")

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

    ser = open_uart()
    last_uart_attempt = time.monotonic()
    prev_state = {}  # empty → first press will always generate :1 edge

    print("Sending controller commands to STM32. Press Ctrl+C to exit.\n")

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
                commands = format_commands(controller.state, prev_state)
                for cmd in commands:
                    try:
                        ser.write(f"{cmd}\n".encode())
                        print(f"[SENT] {cmd}")
                    except serial.SerialException as e:
                        print(f"UART write error: {e}")
                        ser = None
                        last_uart_attempt = time.monotonic()
                        break

            # Always track state so release events fire correctly after reconnect
            prev_state = dict(controller.state)
            time.sleep(SEND_INTERVAL)

    except KeyboardInterrupt:
        print("\nExiting.")
    finally:
        if ser is not None:
            ser.close()
        controller.disconnect()


if __name__ == "__main__":
    main()
