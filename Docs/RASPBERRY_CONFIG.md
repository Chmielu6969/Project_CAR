# Raspberry Pi Zero 2W – Konfiguracja

Most między kontrolerem **PS5 DualSense** (Bluetooth) a **STM32 Nucleo F401RE** (UART).  
Obsługuje też odczyt prędkości z modułu GPS GY-GPS6MV2.

---

## 1. Wymagania

- Raspberry Pi Zero 2W z włączonym Bluetooth
- Kontroler PS5 DualSense sparowany przez Bluetooth
- Python 3 z `virtualenv`
- Daemon `pigpiod` (dla GPS)

---

## 2. Konfiguracja systemu

### `/boot/firmware/config.txt`

Wymagana zawartość sekcji `[all]`:

```ini
[all]
enable_uart=1
dtoverlay=miniuart-bt
```

> Nie dodawaj `dtoverlay=uart3` – nie istnieje na Zero 2W (tylko RPi 4/5).

### Wirtualne środowisko Python

```bash
cd ~/Desktop/Project_CAR/Raspberry
python3 -m venv venv
venv/bin/pip install pygame pyserial pynmea2 pigpio
```

### Parowanie kontrolera PS5

```bash
bluetoothctl
power on
agent on
default-agent
scan on
# Przytrzymaj PS + Create na padzie, aż zacznie migać
pair XX:XX:XX:XX:XX:XX
trust XX:XX:XX:XX:XX:XX
connect XX:XX:XX:XX:XX:XX
scan off
exit
```

> Po pierwszym sparowaniu kontroler łączy się automatycznie przy każdym starcie RPi.

---

## 3. Połączenie UART z STM32

| Raspberry Pi Zero 2W    | Pin fizyczny | STM32 Nucleo | Opis              |
|-------------------------|--------------|--------------|-------------------|
| GPIO14 / TXD            | Pin 8        | PA10 (RX)    | RPi TX → STM RX   |
| GPIO15 / RXD            | Pin 10       | PA9 (TX)     | RPi RX ← STM TX   |
| GND                     | Pin 6        | GND          | Wspólna masa      |

> Oba urządzenia pracują na 3.3V – bezpośrednie połączenie bez konwertera poziomów.  
> Baud rate: **115200**, 8N1.

---

## 4. Autostart (systemd)

```bash
sudo cp controller_uart.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable controller_uart
sudo systemctl start controller_uart
```

Sprawdzenie statusu:

```bash
sudo systemctl status controller_uart
```

Uruchamianie ręczne (bez serwisu):

```bash
venv/bin/python uart/uart_sender.py
```

---

## 5. Format komend UART

Komendy wysyłane przez UART do STM32 mają format `NAZWA:WARTOŚĆ\n`:

| Komenda              | Opis                                  |
|----------------------|---------------------------------------|
| `LSX:0.75\n`         | Lewy drążek poziomo, wartość `0.75`   |
| `LSY:-0.50\n`        | Lewy drążek pionowo, wartość `-0.50`  |
| `CROSS:1\n`          | Wciśnięty krzyżyk                     |
| `R2:0.85\n`          | Spust R2, wartość `0.85`              |
| `DPAD_UP:1\n`        | Krzyżak góra                          |
| `GPS_SPEED_MS:1.23\n`| Prędkość z GPS w m/s (co ~50 ms)     |

---

## 6. Mapowanie kontrolera PS5

### Osie

| Klucz           | Opis                 | Zakres                          |
|-----------------|----------------------|---------------------------------|
| `left_stick_x`  | Lewy drążek poziomo  | `-1.0` (lewo) do `1.0` (prawo) |
| `left_stick_y`  | Lewy drążek pionowo  | `-1.0` (góra) do `1.0` (dół)   |
| `right_stick_x` | Prawy drążek poziomo | `-1.0` (lewo) do `1.0` (prawo) |
| `right_stick_y` | Prawy drążek pionowo | `-1.0` (góra) do `1.0` (dół)   |
| `l2`            | Spust L2             | `0.0` – `1.0`                  |
| `r2`            | Spust R2             | `0.0` – `1.0`                  |

### Przyciski

| Klucz        | Opis                            |
|--------------|---------------------------------|
| `cross`      | Krzyżyk (×)                     |
| `circle`     | Kółko (○)                       |
| `triangle`   | Trójkąt (△)                     |
| `square`     | Kwadrat (□)                     |
| `l1`         | L1                              |
| `r1`         | R1                              |
| `l2_digital` | L2 cyfrowy (dowolne wciśnięcie) |
| `r2_digital` | R2 cyfrowy (dowolne wciśnięcie) |
| `l3`         | Wciśnięcie lewego drążka        |
| `r3`         | Wciśnięcie prawego drążka       |
| `share`      | Share                           |
| `options`    | Options                         |
| `ps`         | Przycisk PS                     |

### Krzyżak

| Klucz        | Opis          |
|--------------|---------------|
| `dpad_up`    | Krzyżak góra  |
| `dpad_down`  | Krzyżak dół   |
| `dpad_left`  | Krzyżak lewo  |
| `dpad_right` | Krzyżak prawo |

---

## 7. Moduł GPS GY-GPS6MV2

### Dlaczego software serial

RPi Zero 2W ma tylko 2 sprzętowe UART-y – oba zajęte:

| Port            | Użycie w projekcie                         |
|-----------------|--------------------------------------------|
| `/dev/ttyAMA0`  | STM32 (GPIO14 TX, GPIO15 RX)               |
| `/dev/ttyS0`    | Bluetooth / PS5 (`dtoverlay=miniuart-bt`)  |

Rozwiązanie: **pigpio software serial** (bit-banging) na GPIO23.

### Fizyczne połączenie

| Pin GPS (GY-GPS6MV2) | Pin Raspberry Pi Zero 2W | Fizyczny pin | Opis                              |
|----------------------|--------------------------|--------------|-----------------------------------|
| VCC                  | 5V                       | Pin 2 lub 4  | Zasilanie (moduł ma własny LDO 3.3V) |
| GND                  | GND                      | Pin 6        | Masa                              |
| TXD                  | GPIO23                   | Pin 16       | GPS TX → RPi software RX          |
| RXD                  | —                        | —            | Nieużywane                        |

> GPS TXD pracuje na 3.3V – nie potrzeba dzielnika napięcia.  
> GPIO23 musi być wejściem – nie podłączać do innych pinów.

### Instalacja pigpiod

```bash
sudo apt update
sudo apt install pigpio
sudo systemctl enable pigpiod
sudo systemctl start pigpiod
```

Weryfikacja:

```bash
sudo systemctl status pigpiod
# Powinno być: active (running)

python3 -c "import pigpio; pi=pigpio.pi(); print('pigpio OK:', pi.connected); pi.stop()"
```

### API klasy GPSReader

```python
from gps.gps_reader import GPSReader

gps = GPSReader()          # GPIO23, 9600 baud
gps.start()                # uruchamia wątek daemon w tle
speed = gps.get_speed_ms() # aktualna prędkość w m/s (0.0 przy braku fixu)
gps.stop()                 # zatrzymuje wątek i zwalnia GPIO
```

| Parametr      | Wartość domyślna | Opis                         |
|---------------|------------------|------------------------------|
| `gpio`        | `23`             | GPIO z podłączonym GPS TXD   |
| `baudrate`    | `9600`           | Prędkość transmisji NEO-6M   |
| `KNOTS_TO_MS` | `0.51444`        | Przelicznik węzły → m/s      |

Komenda UART wysyłana do STM32: `GPS_SPEED_MS:1.23\n` (m/s, 2 miejsca po przecinku).

### Diagnostyka

**Test surowych danych NMEA z GPIO23:**

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

**Czas do pierwszego fixu (cold start):**

| Stan        | Dioda PPS na module | `status` w GNRMC |
|-------------|---------------------|-------------------|
| Brak fixu   | miga co ~1 s        | `V` (void)        |
| Fix uzyskany| miga co ~5 s        | `A` (active)      |

Cold start może trwać **15–30 minut** (wymagana dobra widoczność nieba, min. 4 satelity).

**Częste błędy:**

| Błąd | Przyczyna | Rozwiązanie |
|------|-----------|-------------|
| `GPIO already in use` | Poprzednia sesja nie zwolniła GPIO23 | Naprawione w kodzie przez `bb_serial_read_close` przed `open` |
| Śmieciowe dane zamiast NMEA | GPS TXD podłączony do złego pinu | Przenieść kabel na **pin 16 (GPIO23)** |
| `pigpio daemon not running` | Daemon zatrzymany | `sudo systemctl start pigpiod` |
| Prędkość zawsze 0.0 | Brak fixu GPS | Normalny stan – czekać na fix |
