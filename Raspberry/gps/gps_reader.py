#!/usr/bin/env python3
"""
gps_reader.py - reads NMEA sentences from GY-GPS6MV2 and exposes speed in km/h.

Hardware: GY-GPS6MV2 TXD → GPIO5 (UART3 RXD, /dev/ttyAMA1) @ 9600 baud.
Requires: pynmea2  (pip install pynmea2)
"""

import threading
import serial
import pynmea2

GPS_PORT     = '/dev/ttyAMA1'
GPS_BAUDRATE = 9600
KNOTS_TO_KMH = 1.852


class GPSReader:
    """Background thread that reads NMEA sentences and exposes current speed."""

    def __init__(self, port=GPS_PORT, baudrate=GPS_BAUDRATE):
        self._port      = port
        self._baudrate  = baudrate
        self._speed_kmh = 0.0
        self._lock      = threading.Lock()
        self._running   = False
        self._thread    = None

    def start(self):
        """Start background reading thread."""
        self._running = True
        self._thread  = threading.Thread(target=self._run, daemon=True)
        self._thread.start()
        print(f"GPS reader started on {self._port} @ {self._baudrate}")

    def stop(self):
        """Signal background thread to stop."""
        self._running = False

    def get_speed_kmh(self):
        """Return last known speed in km/h (0.0 when no GPS fix)."""
        with self._lock:
            return self._speed_kmh

    # ------------------------------------------------------------------

    def _run(self):
        ser = None
        while self._running:
            if ser is None:
                ser = self._open_serial()
                if ser is None:
                    import time; time.sleep(5)
                    continue

            try:
                line = ser.readline().decode('ascii', errors='replace').strip()
                self._parse_line(line)
            except serial.SerialException as e:
                print(f"GPS serial error: {e}")
                ser = None
            except Exception:
                pass  # ignore malformed lines silently

        if ser is not None:
            ser.close()

    def _open_serial(self):
        try:
            s = serial.Serial(self._port, self._baudrate, timeout=2)
            print(f"GPS port opened: {self._port}")
            return s
        except serial.SerialException as e:
            print(f"GPS port unavailable: {e}")
            return None

    def _parse_line(self, line):
        # Only interested in $GPRMC (or $GNRMC) – contains speed over ground in knots
        if not (line.startswith('$GPRMC') or line.startswith('$GNRMC')):
            return
        try:
            msg = pynmea2.parse(line)
            if msg.status == 'A' and msg.spd_over_grnd is not None:
                kmh = float(msg.spd_over_grnd) * KNOTS_TO_KMH
                with self._lock:
                    self._speed_kmh = kmh
            else:
                # No valid fix – reset speed to 0
                with self._lock:
                    self._speed_kmh = 0.0
        except (pynmea2.ParseError, AttributeError, ValueError):
            pass
