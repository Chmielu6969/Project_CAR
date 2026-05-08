#!/usr/bin/env python3
"""
ps5_test.py - raw input reader for PS5 DualSense controller
Step 2: deadzone filtering to suppress joystick noise
"""

import pygame
import sys

# Ignore axis movements smaller than this value
DEADZONE = 0.15

def apply_deadzone(value, threshold):
    """Return 0.0 if value is within deadzone, otherwise return value as-is."""
    if abs(value) < threshold:
        return 0.0
    return value

def main():
    pygame.init()
    pygame.joystick.init()

    if pygame.joystick.get_count() == 0:
        print("No controller detected. Make sure the PS5 pad is connected via Bluetooth.")
        sys.exit(1)

    pad = pygame.joystick.Joystick(0)
    pad.init()
    print(f"Controller detected: {pad.get_name()}")
    print(f"Axes: {pad.get_numaxes()}")
    print(f"Buttons: {pad.get_numbuttons()}")
    print(f"Hats (D-pad): {pad.get_numhats()}")
    print("\nPress Ctrl+C to exit.\n")

    try:
        while True:
            for event in pygame.event.get():
                if event.type == pygame.JOYAXISMOTION:
                    filtered = apply_deadzone(event.value, DEADZONE)
                    if filtered != 0.0:
                        print(f"Axis {event.axis}: {filtered:.3f}")
                elif event.type == pygame.JOYBUTTONDOWN:
                    print(f"Button {event.button}: PRESSED")
                elif event.type == pygame.JOYBUTTONUP:
                    print(f"Button {event.button}: RELEASED")
                elif event.type == pygame.JOYHATMOTION:
                    print(f"D-pad: {event.value}")
    except KeyboardInterrupt:
        print("\nExiting.")
    finally:
        pygame.quit()

if __name__ == "__main__":
    main()
