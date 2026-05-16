# Konfiguracja modułu GPS GY-GPS6MV2 na Raspberry Pi Zero 2W

## Kontekst i ograniczenie sprzętowe

Raspberry Pi Zero 2W (chip BCM2710A1 / BCM2837) posiada tylko **dwa sprzętowe UART-y**:

| Port         | Opis                        | Użycie w projekcie       |
|--------------|-----------------------------|--------------------------|
| `/dev/ttyAMA0` | UART0 – PL011 (pełny)     | STM32 → GPIO14 (TX), GPIO15 (RX) |
| `/dev/ttyS0`   | UART1 – mini UART          | Bluetooth / kontroler PS5 (dtoverlay=miniuart-bt) |

`dtoverlay=uart3` **nie działa** na Zero 2W – istnieje tylko na RPi 4/5 (BCM2711).

**Rozwiązanie:** pigpio software serial (bit-banging) na dowolnym wolnym GPIO.

---

## Schemat podłączenia

### GY-GPS6MV2 → Raspberry Pi Zero 2W

| Pin GPS     | Pin Raspberry Pi Zero 2W | Fizyczny pin płytki | Opis                        |
|-------------|--------------------------|---------------------|-----------------------------|
| VCC         | 5V                       | Pin 2 lub 4         | Zasilanie (moduł ma własny LDO 3.3V) |
| GND         | GND                      | Pin 6               | Masa                        |
| TXD (GPS→RPi) | GPIO23                 | Pin 16              | GPS TX → RPi software RX    |
| RXD (GPS←RPi) | nieużywane             | —                   | Opcjonalne – tylko konfiguracja modułu |

> **Ważne:** GPIO23 (pin 16) musi być **wejściem** – nie podłączać do żadnego innego pinu.
> GPS TXD pracuje na 3.3V, więc nie potrzeba dzielnika napięcia.

---

## Konfiguracja systemu

### 1. `/boot/firmware/config.txt`

Wymagana zawartość (bez `dtoverlay=uart3`):

```ini
[all]
enable_uart=1
dtoverlay=miniuart-bt
```

Nie dodawaj żadnych dodatkowych overlayów UART – pigpio nie wymaga zmian w config.txt.

### 2. Instalacja pigpiod

```bash
sudo apt update
sudo apt install pigpio
sudo systemctl enable pigpiod
sudo systemctl start pigpiod
```

Sprawdzenie statusu:

```bash
sudo systemctl status pigpiod
# Powinno być: active (running)
```

### 3. Instalacja zależności Python

```bash
cd ~/Desktop/Project_CAR/Raspberry
venv/bin/pip install pygame pyserial pynmea2 pigpio
```

---

## Jak to działa

`pigpio` uruchamia demona (`pigpiod`) z uprawnieniami roota, który obsługuje GPIO na poziomie sprzętowym. Biblioteka Python `pigpio` komunikuje się z demonem przez socket lokalny.

Wywołanie `bb_serial_read_open(gpio, baud, bits)` konfiguruje wybrany GPIO jako wejście software serial – pigpiod próbkuje pin z odpowiednią częstotliwością i rekonstruuje bajty NMEA.

---

## Pliki projektu

```
Raspberry/
└── gps/
    ├── __init__.py        # plik inicjalizacyjny modułu Python
    └── gps_reader.py      # klasa GPSReader (pigpio software serial)
```

### Klasa `GPSReader` – najważniejsze parametry

| Parametr       | Domyślna wartość | Opis                       |
|----------------|------------------|----------------------------|
| `gpio`         | `23`             | GPIO, na którym podłączony GPS TXD |
| `baudrate`     | `9600`           | Prędkość transmisji NEO-6M |
| `KNOTS_TO_MS`  | `0.51444`        | Przelicznik węzły → m/s   |

### API

```python
from gps.gps_reader import GPSReader

gps = GPSReader()     # GPIO23, 9600 baud
gps.start()           # uruchamia wątek daemon w tle
speed = gps.get_speed_ms()   # aktualna prędkość w m/s (0.0 gdy brak fixu)
gps.stop()            # zatrzymuje wątek i zwalnia GPIO
```

---

## Integracja z `uart_sender.py`

Skrypt `uart/uart_sender.py` importuje `GPSReader` i dołącza komendę UART w każdej iteracji pętli:

```
GPS_SPEED_MS:1.23\n
```

Format: prędkość w m/s z dokładnością 2 miejsc po przecinku.

---

## Diagnostyka

### Sprawdzenie czy pigpiod działa

```bash
sudo systemctl status pigpiod
```

### Test surowych danych NMEA z GPIO23

Zapisz poniższy skrypt jako `test_raw.py` i uruchom:

```python
import pigpio, time

GPIO = 23
pi = pigpio.pi()
try:
    pi.bb_serial_read_close(GPIO)
except:
    pass

pi.bb_serial_read_open(GPIO, 9600, 8)
print("Czytam NMEA przez 10 sekund...")

start = time.time()
buf = b''
while time.time() - start < 10:
    count, data = pi.bb_serial_read(GPIO)
    if count > 0:
        buf += bytes(data)
        while b'\n' in buf:
            line, buf = buf.split(b'\n', 1)
            print(line.decode('ascii', errors='replace').strip())
    else:
        time.sleep(0.05)

pi.bb_serial_read_close(GPIO)
pi.stop()
```

```bash
venv/bin/python test_raw.py
```

Prawidłowe wyjście (zdania NMEA):

```
$GNRMC,123456.00,V,,,,,,,160526,,,N*5E
$GPGSV,3,1,09,01,45,120,30,...
$GLGSV,3,1,09,...
```

### Interpretacja zdania `$GNRMC`

```
$GNRMC,HHMMSS.ss,Status,Lat,N,Lon,E,Speed,Course,DDMMYY,...
                  ^^^^^^
                  A = fix aktywny (prędkość prawidłowa)
                  V = void (brak fixu)
```

Pole `Speed` = prędkość w węzłach × 0.51444 = m/s.

### Czas do pierwszego fixu (cold start)

| Stan        | Dioda PPS na module GPS | `status` w GNRMC |
|-------------|-------------------------|-------------------|
| Brak fixu   | miga co ~1 s            | `V` (void)        |
| Fix uzyskany| miga co ~5 s            | `A` (active)      |

Cold start (pierwsza włączona na świeżo) może trwać **15–30 minut**.  
Wymagania dla fixu:
- Antena skierowana ku niebu (outdoor, brak zabudowy nad modułem)
- PDOP < 5–6 (wskaźnik jakości geometrii satelitów)
- Minimum 4 satelity z dobrym sygnałem

---

## Częste błędy

### `GPS pigpio init error: 'GPIO already in use'`

Poprzednia sesja nie zwolniła GPIO23. Naprawione w kodzie przez:

```python
try:
    pi.bb_serial_read_close(self._gpio)
except Exception:
    pass
# a dopiero potem:
pi.bb_serial_read_open(self._gpio, self._baudrate, 8)
```

### Śmieciowe dane zamiast NMEA w terminalu

GPS TXD był podłączony do GPIO15 (UART0, używany przez STM32) zamiast GPIO23.  
Rozwiązanie: przenieść kabel GPS TXD na **pin 16 (GPIO23)**.

### `pigpio daemon not running`

```bash
sudo systemctl start pigpiod
```

### Prędkość zawsze 0.0 przy statusie `V`

Normalny stan do uzyskania fixu – patrz sekcja "Czas do pierwszego fixu".
