#!/usr/bin/env python3
"""
gps_reader.py - reads NMEA sentences from GY-GPS6MV2 via software serial (pigpio).

Hardware: GY-GPS6MV2 TXD → GPIO23 (physical pin 16) @ 9600 baud.
Requires: pigpio daemon (sudo systemctl start pigpiod) + pynmea2
"""

import threading
import time
import pigpio
import pynmea2

GPS_GPIO     = 23
GPS_BAUDRATE = 9600
KNOTS_TO_KMH = 1.852


class GPSReader:
    """Background thread that reads NMEA sentences and exposes current speed."""

    def __init__(self, gpio=GPS_GPIO, baudrate=GPS_BAUDRATE):
        self._gpio      = gpio
        self._baudrate  = baudrate
        self._speed_kmh = 0.0
        self._lock      = threading.Lock()
        self._running   = False
        self._thread    = None
        self._pi        = None
        self._buf       = b''

    def start(self):
        """Start background reading thread."""
        self._running = True
        self._thread  = threading.Thread(target=self._run, daemon=True)
        self._thread.start()
        print(f"GPS reader started on GPIO{self._gpio} @ {self._baudrate} baud (pigpio)")

    def stop(self):
        """Signal background thread to stop."""
        self._running = False

    def get_speed_kmh(self):
        """Return last known speed in km/h (0.0 when no GPS fix)."""
        with self._lock:
            return self._speed_kmh

    # ------------------------------------------------------------------

    def _run(self):
        while self._running:
            if self._pi is None:
                self._pi = self._connect()
                if self._pi is None:
                    time.sleep(5)
                    continue

            try:
                count, data = self._pi.bb_serial_read(self._gpio)
                if count > 0:
                    self._buf += bytes(data)
                    self._process_buffer()
                else:
                    time.sleep(0.05)
            except Exception as e:
                print(f"GPS read error: {e}")
                self._disconnect()
                time.sleep(5)

        self._disconnect()

    def _connect(self):
        try:
            pi = pigpio.pi()
            if not pi.connected:
                print("GPS: pigpio daemon not running – start with: sudo systemctl start pigpiod")
                return None
            pi.bb_serial_read_open(self._gpio, self._baudrate, 8)
            print(f"GPS software serial opened on GPIO{self._gpio}")
            return pi
        except Exception as e:
            print(f"GPS pigpio init error: {e}")
            return None

    def _disconnect(self):
        if self._pi is not None:
            try:
                self._pi.bb_serial_read_close(self._gpio)
                self._pi.stop()
            except Exception:
                pass
            self._pi = None

    def _process_buffer(self):
        while b'\n' in self._buf:
            line, self._buf = self._buf.split(b'\n', 1)
            sentence = line.decode('ascii', errors='replace').strip()
            self._parse_sentence(sentence)

    def _parse_sentence(self, sentence):
        if not (sentence.startswith('$GPRMC') or sentence.startswith('$GNRMC')):
            return
        try:
            msg = pynmea2.parse(sentence)
            if msg.status == 'A' and msg.spd_over_grnd is not None:
                kmh = float(msg.spd_over_grnd) * KNOTS_TO_KMH
                with self._lock:
                    self._speed_kmh = kmh
            else:
                with self._lock:
                    self._speed_kmh = 0.0
        except (pynmea2.ParseError, AttributeError, ValueError):
            pass
