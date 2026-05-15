# Dokumentacja techniczna – RC_CAR Ferrari SF90 XX Stradale

## Platforma

**STM32 Nucleo F401RE**
- Mikrokontroler: STM32F401RE
- Rdzeń: ARM Cortex-M4 @ 16 MHz (HSI, bez PLL)
- Flash: 512 KB, RAM: 96 KB
- Zasilanie: 3,3 V / 5 V przez złącza CN7 / CN10

**Raspberry Pi Zero 2W**
- Most Bluetooth/UART między kontrolerem PS5 a STM32
- Skrypty Python odbierają dane z pada DualSense przez Bluetooth i przesyłają komendy przez UART (GPIO14 TX → PA10 RX STM32)

**Zasilanie**
- Koszyk na 3 ogniwa 18650
- Wskaźnik poziomu baterii DC7-40V (Lipo/Acid)
- Ładowanie przez złącze USB-C 3A

---

## Lista komponentów

| Komponent | Ilość |
|---|---|
| STM32 NUCLEO-F401RE (ARM Cortex M4) | 1x |
| Raspberry Pi Zero 2W | 1x |
| Silnik N20-BT03 micro 10:1 3000RPM - 12V | 4x |
| TB6612FNG – dwukanałowy sterownik silników (Pololu 713) | 2x |
| Digital Servo 21G Model S007M | 1x |
| Karoseria Ferrari SF90 XX Stradale (hollow shell body) | 1x |
| Koła 1,9-calowe (65 mm, opony 12 mm Hex) | 4x |
| Sześciokątny adapter do kół (12mm/3mm – Pololu 2682) | 4x |
| Czujnik HC-SR04 | 1x |
| 8-kanałowy moduł czujnika śledzenia IR (detektor podczerwieni) | 2x |
| Wyświetlacz 1.28 TFT (240x240, IC:GC9A01) | 2x |
| Wyświetlacz 2.0 TFT SPI (GMT020-02-7P) | 1x |
| Koszyk na akumulatory 3x18650 | 1x |
| Wskaźnik poziomu baterii DC7-40V (Lipo/Acid) | 1x |
| Złącze żeńskie typu C (Port ładowania 3A) | 1x |
| RC Car Metal Magnet Body Shell | 4x |
| Moduł GPS GY-GPS6MV2 (NEO-6M, UART 9600 baud) | 1x |

---

## Schemat podłączenia

### Digital Servo 21G Model S007M

| Przewód        | Pin Nucleo / Złącze | Funkcja     |
|----------------|---------------------|-------------|
| Żółty (sygnał) | PB6 [TIM4 CH1, AF2] | Sygnał PWM  |
| Czerwony (VCC) | +5V (CN7 pin 18)    | VCC         |
| Brązowy (GND)  | GND                 | Masa        |

> Zworka: PA6–PA5

> Servo cyfrowe – zakres impulsu 500–2500 µs, PWM 50 Hz. Reaguje precyzyjniej niż analogowe; zastosowano deadband 3 µs i wygładzanie wykładnicze aby zapobiec buzzowaniu.

### Silniki N20 – Mostek 1 (TB6612FNG)

| Pin mostka | Pin Nucleo | Złącze | Funkcja      |
|------------|------------|--------|--------------|
| STDBY      | PB0        | CN7    | GPIO Output  |
| AIN1       | PB1        | CN7    | GPIO Output  |
| AIN2       | PB3        | CN7    | GPIO Output  |
| PWMA       | PB4        | CN7    | TIM3 CH1 AF2 |
| BIN1       | PB5        | CN7    | GPIO Output  |
| BIN2       | PB8        | CN7    | GPIO Output  |
| PWMB       | PB9        | CN7    | TIM4 CH4 AF2 |

### Silniki N20 – Mostek 2 (TB6612FNG)

| Pin mostka | Pin Nucleo | Złącze | Funkcja      |
|------------|------------|--------|--------------|
| STDBY      | PC8        | CN10   | GPIO Output  |
| AIN1       | PC9        | CN10   | GPIO Output  |
| AIN2       | PA4        | CN10   | GPIO Output  |
| PWMA       | PA8        | CN10   | TIM1 CH1 AF1 |
| BIN1       | PC4        | CN10   | GPIO Output  |
| BIN2       | PC5        | CN10   | GPIO Output  |
| PWMB       | PC7        | CN10   | TIM3 CH2 AF2 |

### Przycisk użytkownika

| Komponent | Pin Nucleo | Funkcja             |
|-----------|------------|---------------------|
| B1 (USER) | PC13       | GPIO Input, Pull-up |

### Czujnik HC-SR04

Piny zostaną przypisane przy implementacji Modułu 3 (omijanie przeszkód).

### Czujniki IR (2× moduł 8-kanałowy, łącznie 16 wejść GPIO)

Moduł 1:

| Pin  | Kanał   |
|------|---------|
| PA0  | IR1_CH1 |
| PA1  | IR1_CH2 |
| PA7  | IR1_CH3 |
| PA15 | IR1_CH4 |
| PB2  | IR1_CH5 |
| PB7  | IR1_CH6 |
| PB10 | IR1_CH7 |
| PB11 | IR1_CH8 |

Moduł 2:

| Pin  | Kanał   | Uwaga                                              |
|------|---------|-----------------------------------------------------|
| PB12 | IR2_CH1 |                                                     |
| PB14 | IR2_CH2 |                                                     |
| PC12 | IR2_CH3 |                                                     |
| PD2  | IR2_CH4 |                                                     |
| PA11 | IR2_CH5 | USB_DM – brak złącza USB mikrokontrolera na Nucleo  |
| PA12 | IR2_CH6 | USB_DP – j.w.                                       |
| PC14 | IR2_CH7 | OSC32_IN – GPIO gdy LSE wyłączone w CubeMX          |
| PC15 | IR2_CH8 | OSC32_OUT – j.w.                                    |

Konfiguracja CubeMX: GPIO Input, Pull-up. Dla PA11/PA12: USB nie włączone. Dla PC14/PC15: RCC → LSE = Disable.

### Moduł GPS GY-GPS6MV2 (NEO-6M) → Raspberry Pi Zero 2W

Moduł GPS podłączony jest do Raspberry Pi Zero 2W przez osobny port UART
(inny niż port komunikacji ze STM32). Raspberry odczytuje zdania NMEA ($GPRMC / $GPVTG)
i przesyła prędkość do STM32 komendą `SPEED:<km/h>\n`.

| Pin GPS       | Raspberry Pi Zero 2W          | Opis                        |
|---------------|-------------------------------|-----------------------------|
| VCC           | 3.3 V (pin 1)                 | Zasilanie                   |
| GND           | GND (pin 6)                   | Masa                        |
| TX (GPS→RPi)  | RX – port zależny od dtoverlay | Odbiór NMEA, 9600 baud     |
| RX (GPS←RPi)  | TX – port zależny od dtoverlay | Opcjonalnie (konfiguracja) |

> Port GPS na Raspberry Pi ustawiany przez `dtoverlay` lub adapter USB-UART
> (domyślny `GPS_PORT` w skrypcie: `/dev/ttyUSB0`).
> Skrypt odczytu: `Raspberry/gps/gps_reader.py`.

### Wyświetlacze TFT (SPI2, na tyle pojazdu)

Wspólna magistrala SPI2, różnicowane przez osobne piny CS:

| Pin  | Sygnał        | Opis                                      |
|------|---------------|-------------------------------------------|
| PB13 | SPI2_SCK      | Zegar SPI (AF5)                           |
| PB15 | SPI2_MOSI     | Dane do wyświetlaczy (AF5)                |
| PB12 | TFT_LEFT_CS   | Chip select – lewy GC9A01 (tryb jazdy)    |
| PB14 | TFT_LEFT_DC   | Data/Command – lewy GC9A01               |
| PB10 | TFT_RIGHT_CS  | Chip select – prawy GC9A01 (prędkość)    |
| PB7  | TFT_RIGHT_DC  | Data/Command – prawy GC9A01              |
| PB2  | TFT_CENTER_CS | Chip select – środkowy GMT020-02-7P (PS5) |
| PA15 | TFT_CENTER_DC | Data/Command – środkowy GMT020-02-7P     |
| PC11 | TFT_RST       | Reset wspólny (wszystkie 3)               |

Konfiguracja CubeMX: SPI2 Transmit Only Master, Prescaler /4 (~21 MHz). PB2/PB7/PB10/PB12/PB14/PA15/PC11: GPIO Output push-pull.

---

## Przypisanie timerów

| Timer | Kanał | Pin | Zastosowanie   |
|-------|-------|-----|----------------|
| TIM1  | CH1   | PA8 | Mostek 2 PWMA  |
| TIM3  | CH1   | PB4 | Mostek 1 PWMA  |
| TIM3  | CH2   | PC7 | Mostek 2 PWMB  |
| TIM4  | CH1   | PB6 | Servo S007M    |
| TIM4  | CH4   | PB9 | Mostek 1 PWMB  |

TIM4 jest skonfigurowany z okresem 20 000 µs (50 Hz) dla sygnału PWM serwa.
TIM1, TIM3 są skonfigurowane z okresem 1000 taktów dla PWM silników.

---

## Struktura plików projektu

```
Project_CAR/
├── Core/
│   ├── Inc/
│   │   ├── motor.h          # Sterownik silników N20 (TB6612FNG)
│   │   ├── servo.h          # Sterownik serwomechanizmu Digital Servo 21G S007M
│   │   ├── tft.h            # Sterownik wyświetlaczy TFT (SPI2) – GC9A01, GMT020
│   │   ├── speedometer.h    # Licznik prędkości na TFT_RIGHT (GC9A01)
│   │   ├── uart_cmd.h       # Parser komend UART (LSX, R2, L2, CROSS, SPEED…)
│   │   └── main.h           # Definicje pinów GPIO (generowane przez CubeMX)
│   └── Src/
│       ├── motor.c          # Sterowanie dwoma mostkami TB6612FNG
│       ├── servo.c          # Sterowanie PWM przez TIM4 CH1
│       ├── tft.c            # Inicjalizacja GC9A01 / GMT020, FillColor, FillRect
│       ├── speedometer.c    # Wyświetlanie prędkości (7-segmentowe cyfry + „KM/H")
│       ├── uart_cmd.c       # Odbiór UART przez przerwanie, parsowanie linii
│       └── main.c           # Inicjalizacja peryferiów, główna pętla sterowania
├── Raspberry/               # Skrypty Python – most PS5 Bluetooth → STM32 UART
│   ├── ps5/
│   │   ├── ps5_controller.py
│   │   └── ps5_test.py
│   ├── uart/
│   │   └── uart_sender.py
│   ├── gps/
│   │   └── gps_reader.py    # Odczyt GY-GPS6MV2, wysyłanie SPEED: do STM32
│   └── README.md
├── Drivers/                 # STM32 HAL + CMSIS (generowane przez CubeMX)
├── Project_CAR.ioc          # Konfiguracja STM32CubeMX
└── STM32F401RETX_FLASH.ld   # Skrypt linkera
```

---

## Konfiguracja STM32CubeMX

Wszelkie zmiany pinów, timerów i innych peryferiów należy wykonywać wyłącznie w **STM32CubeMX** przez plik `Project_CAR.ioc`, a następnie wygenerować kod (**Project → Generate Code**). Sekcje `/* USER CODE BEGIN/END */` są zachowywane przez generator i to w nich należy umieszczać własny kod.

### Wymagania narzędziowe

- [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) w wersji 1.14 lub nowszej

### Budowanie i wgrywanie firmware

1. Zaimportuj projekt: **File → Open Projects from File System** → wskaż katalog `Project_CAR`.
2. Zbuduj: **Project → Build All** (`Ctrl+B`).
3. Podłącz płytkę Nucleo przez USB.
4. Wgraj firmware: **Run → Run** (`F11`).

---

## Opis modułów oprogramowania

### `motor.c` – sterownik silników

Obsługuje dwa mostki TB6612FNG sterujące czterema silnikami N20. Eksponuje funkcję `Motor_SetAll(dir, speed)` przyjmującą kierunek (`MOTOR_FORWARD`, `MOTOR_BACKWARD`, `MOTOR_STOP`) i wartość wypełnienia PWM (0–1000). Oba mostki są inicjalizowane z aktywnym STDBY.

### `tft.c` – sterownik wyświetlaczy TFT

Obsługuje trzy wyświetlacze na magistrali SPI2. Eksponuje:
- `TFT_Init()` – inicjalizuje GC9A01 (lewy, prawy) i GMT020-02-7P (środkowy).
- `TFT_FillColor(disp, color)` – wypełnia cały wyświetlacz kolorem RGB565.
- `TFT_FillRect(disp, x0, y0, x1, y1, color)` – wypełnia prostokąt kolorem (używane przez speedometer).

### `speedometer.c` – licznik prędkości

Wyświetla prędkość GPS na prawym wyświetlaczu GC9A01 (240×240) jako duże cyfry
7-segmentowe (kolor czerwony) z etykietą „KM/H" (kolor biały, poniżej cyfr).
- `Speedometer_Init()` – czyści ekran i rysuje etykietę jednorazowo.
- `Speedometer_Update(speed_kmh)` – przerysowuje obszar cyfr tylko gdy wartość się zmieni.
Prędkość pochodzi z komendy `SPEED:<wartość>` odbieranej przez UART z Raspberry Pi.

### `uart_cmd.c` – parser komend UART

Odbiera bajty przez przerwanie USART1, składa linie formatu `KOMENDA:WARTOŚĆ\n`
i parsuje je. Obsługiwane komendy: `LSX`, `LSY`, `RSX`, `RSY`, `L2`, `R2`,
`CROSS` oraz `SPEED` (prędkość GPS w km/h, timeout 2 s).

### `servo.c` – sterownik serwa

Generuje sygnał PWM 50 Hz przez TIM4 CH1 dla serwomechanizmu cyfrowego Digital Servo 21G S007M. Funkcja `Servo_SetPulse(us)` przyjmuje szerokość impulsu w mikrosekundach (500–2500 µs). Stałe `SERVO_LEFT_US`, `SERVO_CENTER_US`, `SERVO_RIGHT_US` definiują skrajne pozycje. Sterowanie z padu PS5 (oś LSX) realizuje wygładzanie wykładnicze z deadbandem 3 µs – zapobiega to buzzowaniu charakterystycznemu dla serw cyfrowych.
