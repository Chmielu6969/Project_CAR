#!/usr/bin/env python3
"""
ps5_controller.py - PS5 DualSense controller state manager
Maintains current state of all axes, buttons and D-pad.
"""

import pygame

DEADZONE = 0.15

# Axes that return -1.0 at rest, normalized to [0.0, 1.0]
TRIGGER_AXES = {2, 5}

AXIS_MAP = {
    0: "left_stick_x",
    1: "left_stick_y",
    2: "l2",
    3: "right_stick_x",
    4: "right_stick_y",
    5: "r2",
}

BUTTON_MAP = {
    0:  "cross",
    1:  "circle",
    2:  "triangle",
    3:  "square",
    4:  "l1",
    5:  "r1",
    6:  "l2_digital",
    7:  "r2_digital",
    8:  "share",
    9:  "options",
    10: "ps",
    11: "l3",
    12: "r3",
}

DPAD_MAP = {
    (0,  1): "dpad_up",
    (0, -1): "dpad_down",
    (-1, 0): "dpad_left",
    (1,  0): "dpad_right",
}


def _apply_deadzone(value, threshold):
    """Return 0.0 if value is within deadzone, otherwise return value as-is."""
    if abs(value) < threshold:
        return 0.0
    return value


def _normalize_trigger(value):
    """Convert trigger range [-1.0, 1.0] to [0.0, 1.0]."""
    return (value + 1.0) / 2.0


class PS5Controller:
    """Maintains the current state of a connected PS5 DualSense controller."""

    def __init__(self):
        self.state = {
            # Sticks
            "left_stick_x":  0.0,
            "left_stick_y":  0.0,
            "right_stick_x": 0.0,
            "right_stick_y": 0.0,
            # Triggers
            "l2": 0.0,
            "r2": 0.0,
            # Buttons
            "cross":      False,
            "circle":     False,
            "triangle":   False,
            "square":     False,
            "l1":         False,
            "r1":         False,
            "l2_digital": False,
            "r2_digital": False,
            "l3":         False,
            "r3":         False,
            "share":      False,
            "options":    False,
            "ps":         False,
            # D-pad
            "dpad_up":    False,
            "dpad_down":  False,
            "dpad_left":  False,
            "dpad_right": False,
        }
        self._pad = None

    def connect(self):
        """Initialize pygame and connect to the first available controller."""
        pygame.init()
        pygame.joystick.init()

        if pygame.joystick.get_count() == 0:
            raise RuntimeError("No controller detected. Make sure the PS5 pad is connected via Bluetooth.")

        self._pad = pygame.joystick.Joystick(0)
        self._pad.init()
        print(f"Controller connected: {self._pad.get_name()}")

    def update(self):
        """Process all pending pygame events and update internal state."""
        for event in pygame.event.get():
            if event.type == pygame.JOYAXISMOTION:
                self._handle_axis(event)
            elif event.type == pygame.JOYBUTTONDOWN:
                self._handle_button(event.button, pressed=True)
            elif event.type == pygame.JOYBUTTONUP:
                self._handle_button(event.button, pressed=False)
            elif event.type == pygame.JOYHATMOTION:
                self._handle_dpad(event.value)

    def _handle_axis(self, event):
        """Update axis state with deadzone and trigger normalization."""
        name = AXIS_MAP.get(event.axis)
        if name is None:
            return
        if event.axis in TRIGGER_AXES:
            self.state[name] = _normalize_trigger(event.value)
        else:
            self.state[name] = _apply_deadzone(event.value, DEADZONE)

    def _handle_button(self, button_index, pressed):
        """Update button state."""
        name = BUTTON_MAP.get(button_index)
        if name is not None:
            self.state[name] = pressed

    def _handle_dpad(self, value):
        """Update D-pad state – reset all directions then set active one."""
        for key in ("dpad_up", "dpad_down", "dpad_left", "dpad_right"):
            self.state[key] = False
        direction = DPAD_MAP.get(value)
        if direction:
            self.state[direction] = True

    def disconnect(self):
        """Clean up pygame resources."""
        pygame.quit()
