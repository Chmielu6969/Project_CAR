#!/usr/bin/env python3
"""
uart_sender.py - reads PS5 controller state and sends commands to STM32 via UART
Command format: <CMD:VALUE>\n  e.g. "CROSS:1\n", "LSX:0.75\n"
"""

import sys
import time
import serial

sys.path.append('/home/admin/Desktop/Project_CAR/Raspberry/ps5')
from ps5_controller import PS5Controller

# UART config
UART_PORT     = '/dev/ttyAMA0'
UART_BAUDRATE = 115200

# How often to send state [seconds]
SEND_INTERVAL = 0.05


def format_commands(state):
    """Convert controller state to list of UART command strings."""
    commands = []

    # Sticks – send only if non-zero
    if state['left_stick_x'] != 0.0:
        commands.append(f"LSX:{state['left_stick_x']:.2f}")
    if state['left_stick_y'] != 0.0:
        commands.append(f"LSY:{state['left_stick_y']:.2f}")
    if state['right_stick_x'] != 0.0:
        commands.append(f"RSX:{state['right_stick_x']:.2f}")
    if state['right_stick_y'] != 0.0:
        commands.append(f"RSY:{state['right_stick_y']:.2f}")

    # Triggers
    if state['l2'] > 0.0:
        commands.append(f"L2:{state['l2']:.2f}")
    if state['r2'] > 0.0:
        commands.append(f"R2:{state['r2']:.2f}")

    # Buttons – send only on change (True = pressed)
    for btn in ('cross', 'circle', 'triangle', 'square',
                'l1', 'r1', 'l3', 'r3',
                'share', 'options', 'ps'):
        if state[btn]:
            commands.append(f"{btn.upper()}:1")

    # D-pad
    for dpad in ('dpad_up', 'dpad_down', 'dpad_left', 'dpad_right'):
        if state[dpad]:
            commands.append(f"{dpad.upper()}:1")

    return commands


def main():
    # Connect controller
    controller = PS5Controller()
    controller.connect()

    # Connect UART
    try:
        ser = serial.Serial(UART_PORT, UART_BAUDRATE, timeout=1)
        print(f"UART connected: {UART_PORT} @ {UART_BAUDRATE}")
    except serial.SerialException as e:
        print(f"UART error: {e}")
        controller.disconnect()
        sys.exit(1)

    print("Sending controller commands to STM32. Press Ctrl+C to exit.\n")

    try:
        while True:
            controller.update()
            commands = format_commands(controller.state)

            for cmd in commands:
                msg = f"{cmd}\n"
                ser.write(msg.encode())
                print(f"[SENT] {cmd}")

            time.sleep(SEND_INTERVAL)

    except KeyboardInterrupt:
        print("\nExiting.")
    finally:
        ser.close()
        controller.disconnect()


if __name__ == "__main__":
    main()
