# Raspberry Pi – Most dla kontrolera PS5

Ten katalog zawiera skrypty Python działające na **Raspberry Pi Zero 2 W**.
Ich zadaniem jest odczytywanie sygnałów z kontrolera **PS5 DualSense** przez Bluetooth
i przesyłanie poleceń do **STM32 Nucleo F401RE** przez UART.

---

## Wymagania

- Raspberry Pi Zero 2 W z włączonym Bluetooth
- Kontroler PS5 DualSense sparowany przez Bluetooth
- Python 3 z `virtualenv`

---

## Konfiguracja

### 1. Włącz Bluetooth

Upewnij się, że plik `/boot/firmware/config.txt` zawiera:

```ini
[all]
enable_uart=1
dtoverlay=miniuart-bt
```

### 2. Sparuj kontroler PS5

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

> **Uwaga:** Po pierwszym sparowaniu kontroler łączy się automatycznie przy każdym uruchomieniu Raspberry Pi.

### 3. Utwórz wirtualne środowisko

```bash
cd ~/Desktop/Project_CAR/Raspberry
python3 -m venv venv
venv/bin/pip install pygame pyserial pynmea2 pigpio
```

### 4. Podłączenie UART (Raspberry Pi → STM32)

| Raspberry Pi Zero 2 W   | STM32 Nucleo | Opis              |
|-------------------------|--------------|-------------------|
| GPIO14 / TXD (pin 8)    | PA10 (RX)    | RPi TX → STM RX   |
| GPIO15 / RXD (pin 10)   | PA9 (TX)     | RPi RX → STM TX   |
| GND (pin 6)             | GND          | Wspólna masa      |

> **Napięcia:** Raspberry Pi i STM32 pracują na 3.3 V – można łączyć bezpośrednio bez konwertera poziomów.

### 5. Podłącz moduł GPS GY-GPS6MV2

RPi Zero 2 W ma tylko 2 sprzętowe UART-y (oba zajęte przez STM32 i Bluetooth), dlatego GPS używa **software serial przez pigpio** na GPIO23.

#### Schemat podłączenia

| Pin GPS (GY-GPS6MV2)  | Pin Raspberry Pi Zero 2 W | Fizyczny pin | Opis                   |
|-----------------------|---------------------------|--------------|------------------------|
| VCC                   | 5V                        | Pin 2 lub 4  | Zasilanie (moduł ma własny regulator 3,3 V) |
| GND                   | GND                       | Pin 6        | Masa                   |
| TXD                   | GPIO23                    | Pin 16       | GPS TX → RPi RX (software serial) |
| RXD                   | —                         | —            | Niepotrzebne           |

#### Instalacja i autostart pigpiod

```bash
sudo apt install pigpio
sudo systemctl enable pigpiod
sudo systemctl start pigpiod
```

Sprawdź czy działa:

```bash
sudo systemctl status pigpiod
```

#### Usunięcie zbędnego overlay z `/boot/firmware/config.txt`

Jeśli wcześniej dodałeś `dtoverlay=uart3`, usuń tę linię — na RPi Zero 2 W nie ma sprzętowego UART3 i overlay nie działa. Sekcja `[all]` powinna wyglądać:

```ini
[all]
enable_uart=1
dtoverlay=miniuart-bt
```

Weryfikacja pigpio:

```bash
python3 -c "import pigpio; pi=pigpio.pi(); print('pigpio OK:', pi.connected); pi.stop()"
```

---

### 6. Zainstaluj serwis systemd (autostart)

```bash
sudo cp controller_uart.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable controller_uart
sudo systemctl start controller_uart
```

Sprawdź status:

```bash
sudo systemctl status controller_uart
```

---

## Uruchamianie ręczne

```bash
# Test kontrolera – wypisuje wejścia w konsoli
venv/bin/python ps5/ps5_test.py

# Wysyłanie komend przez UART do STM32
venv/bin/python uart/uart_sender.py
```

---

## Struktura plików

```
Raspberry/
├── gps/
│   ├── gps_reader.py           # Klasa GPSReader – odczyt NMEA, prędkość w km/h
│   └── __init__.py
├── ps5/
│   ├── ps5_controller.py       # Klasa PS5Controller – przechowuje stan kontrolera
│   └── ps5_test.py             # Skrypt testowy – wypisuje aktywne wejścia w konsoli
├── uart/
│   ├── uart_sender.py          # Wysyła komendy z pada + prędkość GPS przez UART do STM32
│   └── __init__.py
├── controller_uart.service     # Plik serwisu systemd (autostart po restarcie)
└── README.md                   # Ten plik
```

---

## Format komend UART

Komendy wysyłane przez UART do STM32 mają format `NAZWA:WARTOŚĆ\n`, np.:

| Komenda           | Opis                                        |
|-------------------|---------------------------------------------|
| `LSX:0.75\n`      | Lewy drążek poziomo, wartość `0.75`         |
| `LSY:-0.50\n`     | Lewy drążek pionowo, wartość `-0.50`        |
| `CROSS:1\n`       | Wciśnięty krzyżyk                          |
| `R2:0.85\n`       | Spust R2, wartość `0.85`                    |
| `DPAD_UP:1\n`     | Krzyżak góra                               |
| `GPS_SPEED:42.3\n`| Prędkość z GPS w km/h (co 50 ms)           |

---

## Mapowanie kontrolera

### Osie

| Klucz           | Opis                    | Zakres                          |
|-----------------|-------------------------|---------------------------------|
| `left_stick_x`  | Lewy drążek poziomo     | `-1.0` (lewo) do `1.0` (prawo) |
| `left_stick_y`  | Lewy drążek pionowo     | `-1.0` (góra) do `1.0` (dół)   |
| `right_stick_x` | Prawy drążek poziomo    | `-1.0` (lewo) do `1.0` (prawo) |
| `right_stick_y` | Prawy drążek pionowo    | `-1.0` (góra) do `1.0` (dół)   |
| `l2`            | Spust L2                | `0.0` (zwolniony) do `1.0` (pełne wciśnięcie) |
| `r2`            | Spust R2                | `0.0` (zwolniony) do `1.0` (pełne wciśnięcie) |

### Przyciski

| Klucz        | Opis                          |
|--------------|-------------------------------|
| `cross`      | Krzyżyk (×)                   |
| `circle`     | Kółko (○)                     |
| `triangle`   | Trójkąt (△)                   |
| `square`     | Kwadrat (□)                   |
| `l1`         | L1                            |
| `r1`         | R1                            |
| `l2_digital` | L2 cyfrowy (dowolne wciśnięcie) |
| `r2_digital` | R2 cyfrowy (dowolne wciśnięcie) |
| `l3`         | Wciśnięcie lewego drążka      |
| `r3`         | Wciśnięcie prawego drążka     |
| `share`      | Share                         |
| `options`    | Options                       |
| `ps`         | Przycisk PS                   |

### Krzyżak

| Klucz        | Opis          |
|--------------|---------------|
| `dpad_up`    | Krzyżak góra  |
| `dpad_down`  | Krzyżak dół   |
| `dpad_left`  | Krzyżak lewo  |
| `dpad_right` | Krzyżak prawo |

