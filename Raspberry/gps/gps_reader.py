#!/usr/bin/env python3
"""
gps_reader.py – odczytuje prędkość z GY-GPS6MV2 (NEO-6M) i wysyła komendę
SPEED:<km/h> do STM32 przez UART.

Uruchomienie samodzielne (bez kontrolera PS5):
    venv/bin/python gps/gps_reader.py

Uwaga: STM_PORT (/dev/ttyAMA0) jest dzielony z uart_sender.py.
       Nie uruchamiaj obu skryptów jednocześnie – otwierają ten sam port.
       Jeśli chcesz korzystać z GPS równolegle z padem PS5, zintegruj
       odczyt GPS jako wątek w uart_sender.py.
"""

import serial
import time
import sys

# ─── Konfiguracja portów ───────────────────────────────────────────────────────
# GPS_PORT – port, na którym podłączony jest GY-GPS6MV2.
# Dostosuj do swojego ustawienia (dtoverlay / USB-serial adapter):
#   /dev/ttyUSB0   – adapter USB-UART
#   /dev/serial1   – sprzętowy UART Raspberry Pi (AMA0 lub ttyS0)
#   /dev/ttyAMA1   – dodatkowy UART przez dtoverlay (uart3 / uart4 / uart5)
GPS_PORT = "/dev/ttyUSB0"
GPS_BAUD = 9600

# STM32 UART – ten sam co uart_sender.py
STM_PORT = "/dev/ttyAMA0"
STM_BAUD = 115200

KNOTS_TO_KMH = 1.852
SEND_INTERVAL = 1.0  # GPS aktualizuje się z częstotliwością 1 Hz


def parse_nmea(line: str) -> float | None:
    """
    Parsuje zdanie GPRMC lub GPVTG; zwraca prędkość w km/h lub None.
    Nie weryfikuje sumy kontrolnej NMEA – zakłada poprawny sygnał sprzętowy.
    """
    if not line.startswith('$'):
        return None

    # Odrzuć sumę kontrolną (*XX)
    if '*' in line:
        line = line[:line.index('*')]

    parts = line.split(',')
    tag = parts[0][1:]  # usuń '$'

    if tag in ('GPRMC', 'GNRMC'):
        # Pole [2] = status: A = aktywny fix, V = nieważny
        if len(parts) < 8 or parts[2] != 'A':
            return None
        try:
            return float(parts[7]) * KNOTS_TO_KMH
        except ValueError:
            return None

    if tag in ('GPVTG', 'GNVTG'):
        # Pole [9] = prędkość w km/h (oznaczone 'K' w polu [10])
        if len(parts) < 10 or not parts[9]:
            return None
        try:
            return float(parts[9])
        except ValueError:
            return None

    return None


def open_port(path: str, baud: int) -> serial.Serial | None:
    try:
        return serial.Serial(path, baud, timeout=2)
    except serial.SerialException as e:
        print(f"Błąd otwarcia {path}: {e}")
        return None


def main() -> None:
    print(f"GPS reader: {GPS_PORT} @ {GPS_BAUD} baud")
    print(f"STM32 UART: {STM_PORT} @ {STM_BAUD} baud\n")

    gps = open_port(GPS_PORT, GPS_BAUD)
    if gps is None:
        sys.exit(1)

    stm = open_port(STM_PORT, STM_BAUD)
    if stm is None:
        gps.close()
        sys.exit(1)

    speed_kmh: float = 0.0
    last_send: float = time.monotonic() - SEND_INTERVAL  # wyślij od razu na starcie

    print("Działam. Ctrl+C aby zakończyć.\n")

    try:
        while True:
            # ── Odczyt zdania NMEA ──────────────────────────────────────────────
            try:
                raw = gps.readline()
                line = raw.decode('ascii', errors='ignore').strip()
            except serial.SerialException as e:
                print(f"Błąd odczytu GPS: {e}")
                time.sleep(1)
                continue

            result = parse_nmea(line)
            if result is not None:
                speed_kmh = result
                print(f"GPS: {speed_kmh:.1f} km/h")

            # ── Wysyłanie komendy SPEED co SEND_INTERVAL sekund ────────────────
            now = time.monotonic()
            if now - last_send >= SEND_INTERVAL:
                cmd = f"SPEED:{speed_kmh:.1f}\n"
                try:
                    stm.write(cmd.encode('ascii'))
                    print(f"[SENT] {cmd.strip()}")
                except serial.SerialException as e:
                    print(f"Błąd zapisu UART: {e}")
                last_send = now

    except KeyboardInterrupt:
        print("\nZakończono.")
    finally:
        gps.close()
        stm.close()


if __name__ == "__main__":
    main()
