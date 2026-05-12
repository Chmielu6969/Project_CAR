# Dokumentacja techniczna – Projekt CAR

## Platforma

**STM32 Nucleo F401RE**
- Mikrokontroler: STM32F401RE
- Rdzeń: ARM Cortex-M4 @ 16 MHz (HSI, bez PLL)
- Flash: 512 KB, RAM: 96 KB
- Zasilanie: 3,3 V / 5 V przez złącza CN7 / CN10

**Raspberry Pi Zero 2 W**
- Most między kontrolerem PS5 (Bluetooth) a STM32 (UART)
- UART0/ttyAMA0 (GPIO14/GPIO15) → STM32 @ 115200 baud
- UART3/ttyAMA1 (GPIO4/GPIO5) → GPS GY-GPS6MV2 @ 9600 baud
- Bluetooth (dtoverlay=miniuart-bt) → Kontroler PS5 DualSense

---

## Schemat podłączenia

### Wyświetlacz LCD (HD44780, tryb 4-bit)

| Pin LCD    | Funkcja    | Pin Nucleo | Uwagi              |
|------------|------------|------------|--------------------|
| 1 (GND)    | VSS        | GND        |                    |
| 2 (VDD)    | VCC        | +5V        |                    |
| 3 (VO)     | Kontrast   | GND / pot. | pot. 10k zalecany  |
| 4 (RS)     | RS         | PC10       | GPIO Output        |
| 5 (RW)     | R/W        | GND        | na stałe           |
| 6 (E)      | Enable     | PC12       | GPIO Output        |
| 11 (D4)    | DB4        | PC0        | GPIO Output        |
| 12 (D5)    | DB5        | PC1        | GPIO Output        |
| 13 (D6)    | DB6        | PC2        | GPIO Output        |
| 14 (D7)    | DB7        | PC3        | GPIO Output        |
| 15 (BLA)   | Backlight+ | +5V        |                    |
| 16 (BLK)   | Backlight- | GND        |                    |

### Micro servo SG90

| Przewód        | Pin Nucleo | Funkcja            |
|----------------|------------|--------------------|
| Żółty (sygnał) | PB6        | TIM4 CH1, AF2, PWM |
| Czerwony (VCC) | +5V        | CN7 pin 18         |
| Brązowy (GND)  | GND        |                    |

> Zworka: PA6–PA5

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

### Moduł GPS GY-GPS6MV2 (Raspberry Pi)

| Pin GPS (GY-GPS6MV2)  | Pin Raspberry Pi Zero 2 W | Fizyczny pin | Opis                             |
|-----------------------|---------------------------|--------------|----------------------------------|
| VCC                   | 5V                        | Pin 2 lub 4  | Zasilanie (regulator 3,3 V na module) |
| GND                   | GND                       | Pin 6        | Masa                             |
| TXD                   | GPIO5 (UART3 RXD)         | Pin 29       | GPS TX → RPi RX (dane NMEA)      |
| RXD                   | GPIO4 (UART3 TXD)         | Pin 7        | GPS RX ← RPi TX (opcjonalne)     |

Wymagany overlay w `/boot/firmware/config.txt`: `dtoverlay=uart3` → port `/dev/ttyAMA1`.

---

## Przypisanie timerów

| Timer | Kanał | Pin | Zastosowanie   |
|-------|-------|-----|----------------|
| TIM1  | CH1   | PA8 | Mostek 2 PWMA  |
| TIM3  | CH1   | PB4 | Mostek 1 PWMA  |
| TIM3  | CH2   | PC7 | Mostek 2 PWMB  |
| TIM4  | CH1   | PB6 | Servo SG90     |
| TIM4  | CH4   | PB9 | Mostek 1 PWMB  |

TIM4 jest skonfigurowany z okresem 20 000 µs (50 Hz) dla sygnału PWM serwa.
TIM1, TIM3 są skonfigurowane z okresem 1000 taktów dla PWM silników.

---

## Struktura plików projektu

```
Project_CAR/
├── Core/
│   ├── Inc/
│   │   ├── joystick.h       # Joystick analogowy HW-504 (ADC)
│   │   ├── lcd.h            # Sterownik LCD HD44780 (4-bit)
│   │   ├── motor.h          # Sterownik silników N20 (TB6612FNG)
│   │   ├── servo.h          # Sterownik serwomechanizmu SG90
│   │   └── main.h           # Definicje pinów GPIO (generowane przez CubeMX)
│   └── Src/
│       ├── joystick.c       # Odczyt osi X/Y przez ADC, kalibracja, przycisk SW
│       ├── lcd.c            # Komunikacja z LCD w trybie 4-bit
│       ├── motor.c          # Sterowanie dwoma mostkami TB6612FNG
│       ├── servo.c          # Sterowanie PWM przez TIM4 CH1
│       └── main.c           # Inicjalizacja peryferiów, główna pętla sterowania
├── Raspberry/
│   ├── gps/
│   │   └── gps_reader.py    # GPSReader – odczyt NMEA z GY-GPS6MV2, prędkość w km/h
│   ├── ps5/
│   │   ├── ps5_controller.py # PS5Controller – stan kontrolera DualSense przez Bluetooth
│   │   └── ps5_test.py       # Skrypt testowy kontrolera
│   ├── uart/
│   │   └── uart_sender.py    # Wysyła komendy PS5 + GPS_SPEED do STM32 przez UART
│   └── controller_uart.service # Serwis systemd – autostart po restarcie RPi
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

### `servo.c` – sterownik serwa

Generuje sygnał PWM 50 Hz przez TIM4 CH1. Funkcja `Servo_SetPulse(us)` przyjmuje szerokość impulsu w mikrosekundach. Stałe `SERVO_LEFT_US`, `SERVO_CENTER_US`, `SERVO_RIGHT_US` definiują skrajne pozycje.

### `lcd.c` – sterownik LCD

Implementacja protokołu HD44780 w trybie 4-bit. Udostępnia funkcje `LCD_Init()`, `LCD_Print(str)`, `LCD_SetCursor(row, col)` i `LCD_Clear()`. Komunikacja odbywa się przez GPIO bez użycia sprzętowego interfejsu szeregowego.

### `joystick.c` – odczyt joysticka

Odczytuje dwie osie (X, Y) przez ADC1 (kanały 0 i 1, rozdzielczość 12-bit). Funkcja `Joystick_Calibrate()` uśrednia 16 próbek przy starcie i zapamiętuje punkt zerowy. `Joystick_Read()` zwraca wartości ze znakiem względem skalibrowanego środka oraz stan przycisku SW.

### `gps_reader.py` – odczyt GPS (Raspberry Pi)

Klasa `GPSReader` uruchamia wątek tła, który czyta zdania NMEA z `/dev/ttyAMA1` (@ 9600 baud) i parsuje `$GPRMC`/`$GNRMC` przy użyciu biblioteki `pynmea2`. Prędkość nad ziemią (w węzłach) jest przeliczana na km/h i udostępniana przez `get_speed_kmh() -> float`. Brak sygnału GPS lub utrata połączenia zwraca `0.0`. Wynik jest co 50 ms dołączany do strumienia komend UART jako `GPS_SPEED:XX.X\n`.
