# Raspberry Pi – Most dla kontrolera PS5

Ten katalog zawiera skrypty Python działające na Raspberry Pi Zero 2 W.
Ich zadaniem jest odczytywanie sygnałów z kontrolera PS5 DualSense przez Bluetooth
i przesyłanie poleceń do STM32 Nucleo F401RE przez UART.

---

## Wymagania

- Raspberry Pi Zero 2 W z włączonym Bluetooth
- Kontroler PS5 DualSense sparowany przez Bluetooth
- Python 3 z virtualenv

---

## Konfiguracja

### 1. Włącz Bluetooth

Upewnij się, że plik `/boot/firmware/config.txt` zawiera:

```
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
# Przytrzymaj PS + Create na padzie aż zacznie migać
pair XX:XX:XX:XX:XX:XX
trust XX:XX:XX:XX:XX:XX
connect XX:XX:XX:XX:XX:XX
scan off
exit
```

### 3. Utwórz wirtualne środowisko

```bash
python3 -m venv raspberry/venv
raspberry/venv/bin/pip install pygame
```

---

## Uruchamianie

```bash
raspberry/venv/bin/python raspberry/ps5/ps5_test.py
```

---

## Struktura plików

```
raspberry/
├── ps5/
│   ├── ps5_controller.py   # Klasa PS5Controller – przechowuje stan kontrolera
│   └── ps5_test.py         # Skrypt testowy – wypisuje aktywne wejścia w konsoli
└── README.md               # Ten plik
```

---

## Mapowanie kontrolera

### Osie

| Klucz | Opis | Zakres |
|-------|------|--------|
| `left_stick_x` | Lewy drążek poziomo | -1.0 (lewo) do 1.0 (prawo) |
| `left_stick_y` | Lewy drążek pionowo | -1.0 (góra) do 1.0 (dół) |
| `right_stick_x` | Prawy drążek poziomo | -1.0 (lewo) do 1.0 (prawo) |
| `right_stick_y` | Prawy drążek pionowo | -1.0 (góra) do 1.0 (dół) |
| `l2` | Spust L2 | 0.0 (zwolniony) do 1.0 (pełne wciśnięcie) |
| `r2` | Spust R2 | 0.0 (zwolniony) do 1.0 (pełne wciśnięcie) |

### Przyciski

| Klucz | Opis |
|-------|------|
| `cross` | Krzyżyk (×) |
| `circle` | Kółko (○) |
| `triangle` | Trójkąt (△) |
| `square` | Kwadrat (□) |
| `l1` | L1 |
| `r1` | R1 |
| `l2_digital` | L2 cyfrowy (dowolne wciśnięcie) |
| `r2_digital` | R2 cyfrowy (dowolne wciśnięcie) |
| `l3` | Wciśnięcie lewego drążka |
| `r3` | Wciśnięcie prawego drążka |
| `share` | Share |
| `options` | Options |
| `ps` | Przycisk PS |

### Krzyżak

| Klucz | Opis |
|-------|------|
| `dpad_up` | Krzyżak góra |
| `dpad_down` | Krzyżak dół |
| `dpad_left` | Krzyżak lewo |
| `dpad_right` | Krzyżak prawo |

