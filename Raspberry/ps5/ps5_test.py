#!/usr/bin/env python3
"""
ps5_test.py - PS5 DualSense controller test using PS5Controller state manager
"""

import time
from ps5_controller import PS5Controller

def main():
    controller = PS5Controller()
    controller.connect()
    print("Reading controller state. Press Ctrl+C to exit.\n")

    try:
        while True:
            controller.update()
            s = controller.state

            # Print only active inputs to keep output clean
            active = []

            for stick in ("left_stick_x", "left_stick_y", "right_stick_x", "right_stick_y"):
                if s[stick] != 0.0:
                    active.append(f"{stick}: {s[stick]:.3f}")

            for trigger in ("l2", "r2"):
                if s[trigger] > 0.0:
                    active.append(f"{trigger}: {s[trigger]:.3f}")

            for btn in ("cross", "circle", "triangle", "square",
                        "l1", "r1", "l2_digital", "r2_digital",
                        "l3", "r3", "share", "options", "ps"):
                if s[btn]:
                    active.append(f"{btn}: PRESSED")

            for dpad in ("dpad_up", "dpad_down", "dpad_left", "dpad_right"):
                if s[dpad]:
                    active.append(f"{dpad}: PRESSED")

            if active:
                print(" | ".join(active))

            time.sleep(0.05)

    except KeyboardInterrupt:
        print("\nExiting.")
    finally:
        controller.disconnect()

if __name__ == "__main__":
    main()
