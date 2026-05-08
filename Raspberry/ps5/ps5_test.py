#!/usr/bin/env python3
"""
ps5_test.py - raw input reader for PS5 DualSense controller
Step 4: final corrected axis and button mapping
"""

import pygame
import sys

DEADZONE = 0.15

AXIS_NAMES = {
    0: "Left Stick X",
    1: "Left Stick Y",
    2: "L2",
    3: "Right Stick X",
    4: "Right Stick Y",
    5: "R2",
}

BUTTON_NAMES = {
    0:  "Cross",
    1:  "Circle",
    2:  "Triangle",
    3:  "Square",
    4:  "L1",
    5:  "R1",
    6:  "L2 Digital",
    7:  "R2 Digital",
    8:  "Share",
    9:  "Options",
    10: "PS",
    11: "L3",
    12: "R3",
}

DPAD_NAMES = {
    (0, 1):  "Up",
    (0, -1): "Down",
    (-1, 0): "Left",
    (1, 0):  "Right",
    (0, 0):  "Released",
}

# Axes that return -1.0 at rest and 1.0 at full press
INVERTED_AXES = {2, 5}

def apply_deadzone(value, threshold):
    """Return 0.0 if value is within deadzone, otherwise return value as-is."""
    if abs(value) < threshold:
        return 0.0
    return value

def normalize_trigger(value):
    """Convert trigger range from [-1.0, 1.0] to [0.0, 1.0]."""
    return (value + 1.0) / 2.0

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
                    if event.axis in INVERTED_AXES:
                        # Normalize triggers to [0.0, 1.0]
                        value = normalize_trigger(event.value)
                        if value > DEADZONE:
                            name = AXIS_NAMES.get(event.axis, f"Axis {event.axis}")
                            print(f"[TRIGGER] {name}: {value:.3f}")
                    else:
                        filtered = apply_deadzone(event.value, DEADZONE)
                        if filtered != 0.0:
                            name = AXIS_NAMES.get(event.axis, f"Axis {event.axis}")
                            print(f"[AXIS]    {name}: {filtered:.3f}")
                elif event.type == pygame.JOYBUTTONDOWN:
                    name = BUTTON_NAMES.get(event.button, f"Button {event.button}")
                    print(f"[BTN]     {name}: PRESSED")
                elif event.type == pygame.JOYBUTTONUP:
                    name = BUTTON_NAMES.get(event.button, f"Button {event.button}")
                    print(f"[BTN]     {name}: RELEASED")
                elif event.type == pygame.JOYHATMOTION:
                    direction = DPAD_NAMES.get(event.value, str(event.value))
                    if direction != "Released":
                        print(f"[D-PAD]   {direction}")
    except KeyboardInterrupt:
        print("\nExiting.")
    finally:
        pygame.quit()

if __name__ == "__main__":
    main()
