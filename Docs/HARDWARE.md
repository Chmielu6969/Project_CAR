# HARDWARE – Połączenia i konfiguracja STM32CubeMX

Źródło prawdy dla pinów i parametrów timerów: `Project_CAR.ioc`.  
Taktowanie zegara: **16 MHz HSI**.

---

## 1. Serwo Digital Servo 21G S007M

### Fizyczne połączenie

| Przewód        | Pin Nucleo          | Złącze      | Funkcja        |
|----------------|---------------------|-------------|----------------|
| Żółty (sygnał) | PB6 (TIM4 CH1 AF2)  | CN10        | Sygnał PWM     |
| Czerwony (VCC) | +5V (CN7 pin 18)    | CN7         | Zasilanie      |
| Brązowy (GND)  | GND                 | —           | Masa           |

### Konfiguracja CubeMX (skonfigurowane w .ioc)

1. Przejdź do: **Timers → TIM4**.
2. Clock Source: **Internal Clock**.
3. Channel 1: **PWM Generation CH1**.
4. Parameter Settings:
   - Prescaler: `15`
   - Counter Period: `20000`
   - Pulse (CH1): `1500` (pozycja środkowa)
5. Pin PB6 → `TIM4_CH1` (AF2) – przypisywany automatycznie.

> Wynikowa częstotliwość: 16 MHz / 16 / 20001 ≈ **50 Hz** (standard serwo RC).  
> Zakres impulsu: 1000 µs (pełne lewo) – 2000 µs (pełne prawo), środek 1500 µs.

---

## 2. Silniki N20 – Mostek 1 (TB6612FNG)

### Fizyczne połączenie

| Pin mostka | Pin Nucleo | Złącze     | Funkcja      |
|------------|------------|------------|--------------|
| STDBY      | PB0        | CN7        | GPIO Output  |
| AIN1       | PB1        | CN7        | GPIO Output  |
| AIN2       | PB3        | CN7        | GPIO Output  |
| PWMA       | PB4        | CN7        | TIM3 CH1 AF2 |
| BIN1       | PB5        | CN7        | GPIO Output  |
| BIN2       | PB8        | CN7        | GPIO Output  |
| PWMB       | PB9        | CN7        | TIM11 CH1 AF3|

### Konfiguracja CubeMX (skonfigurowane w .ioc)

**TIM3 CH1 (PWMA – PB4):**
1. Przejdź do: **Timers → TIM3**.
2. Clock Source: **Internal Clock**.
3. Channel 1: **PWM Generation CH1**.
4. Parameter Settings: Prescaler `15`, Counter Period `1000`.

**TIM11 CH1 (PWMB – PB9):**
1. Przejdź do: **Timers → TIM11**.
2. Clock Source: **Internal Clock**.
3. Channel 1: **PWM Generation CH1**.
4. Parameter Settings: Prescaler `15`, Counter Period `1000`.

**GPIO Output (PB0, PB1, PB3, PB5, PB8):**
- GPIO output level: Low, Push Pull, No pull, Speed: Low.

> Wynikowa częstotliwość PWM: 16 MHz / 16 / 1001 ≈ **998 Hz**.

---

## 3. Silniki N20 – Mostek 2 (TB6612FNG)

### Fizyczne połączenie

| Pin mostka | Pin Nucleo | Złącze     | Funkcja      |
|------------|------------|------------|--------------|
| STDBY      | PC8        | CN10       | GPIO Output  |
| AIN1       | PC9        | CN10       | GPIO Output  |
| AIN2       | PA4        | CN10       | GPIO Output  |
| PWMA       | PA8        | CN10       | TIM1 CH1 AF1 |
| BIN1       | PC4        | CN10       | GPIO Output  |
| BIN2       | PC5        | CN10       | GPIO Output  |
| PWMB       | PC7        | CN10       | TIM3 CH2 AF2 |

### Konfiguracja CubeMX (skonfigurowane w .ioc)

**TIM1 CH1 (PWMA – PA8):**
1. Przejdź do: **Timers → TIM1**.
2. Clock Source: **Internal Clock**.
3. Channel 1: **PWM Generation CH1**.
4. Parameter Settings: Prescaler `15`, Counter Period `1000`.

**TIM3 CH2 (PWMB – PC7):**
1. W TIM3 (już skonfigurowanym powyżej dla Mostka 1):
2. Channel 2: **PWM Generation CH2**.
3. (Prescaler i Period wspólne z CH1: `15` / `1000`).

**GPIO Output (PC8, PC9, PA4, PC4, PC5):**
- GPIO output level: Low, Push Pull, No pull, Speed: Low.

---

## 4. Czujnik HC-SR04

### Fizyczne połączenie

| Pin czujnika | Pin Nucleo | Złącze       | Uwaga                          |
|--------------|------------|--------------|--------------------------------|
| VCC          | 5V         | CN10 U5V     | HC-SR04 wymaga 5V              |
| GND          | GND        | CN10 GND     |                                |
| TRIG         | PC14       | CN7 pin 25   | GPIO Output                    |
| ECHO         | PC15       | CN7 pin 27   | GPIO Input + dzielnik 1kΩ/2kΩ |


### Konfiguracja CubeMX (skonfigurowane w .ioc)

- PC14 → `GPIO_Output`, User Label: `HC_TRIG` (LSE wyłączone, pin zwolniony).
- PC15 → `GPIO_Input`, User Label: `HC_ECHO`, No pull (LSE wyłączone, pin zwolniony).

---

## 5. Czujnik IR – Moduł 1 (8 kanałów)

### Fizyczne połączenie

Zasilanie: **5V**

| Pin Nucleo | Kanał  | Złącze     |
|------------|--------|------------|
| PC0        | IR_CH1 | CN8 A5     |
| PC1        | IR_CH2 | CN8 A4     |
| PC2        | IR_CH3 | CN7 pin 35 |
| PC3        | IR_CH4 | CN7 pin 37 |
| PC6        | IR_CH5 | CN10 pin 4 |
| PC10       | IR_CH6 | CN7 pin 1  |
| PC12       | IR_CH7 | CN7 pin 3  |
| PD2        | IR_CH8 | CN7 pin 4  |

### Konfiguracja CubeMX (skonfigurowane w .ioc)

Wszystkie 8 pinów skonfigurowane jako `GPIO_Input`, Pull-up, etykiety `IR_CH1` … `IR_CH8`.

---

## 6. Wyświetlacze TFT (SPI2)

Trzy wyświetlacze na wspólnej magistrali SPI2, różnicowane osobnymi pinami CS:

- **Lewy** – GC9A01 1.28" 240×240 (tryb jazdy)
- **Prawy** – GC9A01 1.28" 240×240 (prędkościomierz)
- **Środkowy** – GMT020-02-7P 2.0" (status kontrolera PS5 i Raspberry)

### Fizyczne połączenie

| Pin Nucleo | Sygnał         | Opis                          |
|------------|----------------|-------------------------------|
| PB13       | SPI2_SCK       | Zegar SPI (AF5)               |
| PB15       | SPI2_MOSI      | Dane do wyświetlaczy (AF5)    |
| PB12       | TFT_LEFT_CS    | Chip select – lewy GC9A01     |
| PB14       | TFT_LEFT_DC    | Data/Command – lewy GC9A01    |
| PB10       | TFT_RIGHT_CS   | Chip select – prawy GC9A01    |
| PB7        | TFT_RIGHT_DC   | Data/Command – prawy GC9A01   |
| PB2        | TFT_CENTER_CS  | Chip select – środkowy GMT020 |
| PA15       | TFT_CENTER_DC  | Data/Command – środkowy GMT020|
| PC11       | TFT_RST        | Reset wspólny (wszystkie 3)   |

### Konfiguracja CubeMX (skonfigurowane w .ioc)

**SPI2:**
1. Przejdź do: **Connectivity → SPI2**.
2. Mode: **Transmit Only Master**.
3. Hardware NSS Signal: **Disable**.
4. Parameter Settings:
   - Prescaler: `/4` (→ 4 MHz przy 16 MHz APB1)
   - CPOL: Low, CPHA: 1 Edge, First Bit: MSB, Data Size: 8 Bits.
5. PB13 → `SPI2_SCK` (AF5), PB15 → `SPI2_MOSI` (AF5) – przypisywane automatycznie.

**GPIO Output – CS / DC / RST:**

Dla każdego z: PB2, PB7, PB10, PB12, PB14, PA15, PC11:
1. Kliknij pin → `GPIO_Output`.
2. GPIO output level: **High** (CS/RST domyślnie nieaktywne).
3. GPIO mode: Output Push Pull, No pull, Speed: **High**.

| Pin  | User Label     |
|------|----------------|
| PB12 | TFT_LEFT_CS    |
| PB14 | TFT_LEFT_DC    |
| PB10 | TFT_RIGHT_CS   |
| PB7  | TFT_RIGHT_DC   |
| PB2  | TFT_CENTER_CS  |
| PA15 | TFT_CENTER_DC  |
| PC11 | TFT_RST        |

---

## 7. Przycisk użytkownika

### Fizyczne połączenie

| Komponent | Pin Nucleo | Złącze | Funkcja             |
|-----------|------------|--------|---------------------|
| B1 (USER) | PC13       | CN7    | GPIO Input, Pull-up |

### Konfiguracja CubeMX (skonfigurowane w .ioc)

- PC13 → `GPIO_Input`, Pull-up, User Label: `BTN_USER`.

---

## 8. USART1 – Komunikacja z Raspberry Pi

### Fizyczne połączenie

| Pin Nucleo  | Pin Raspberry Pi Zero 2W | Opis              |
|-------------|--------------------------|-------------------|
| PA9 (TX)    | GPIO15 / RXD (pin 10)    | STM TX → RPi RX   |
| PA10 (RX)   | GPIO14 / TXD (pin 8)     | STM RX ← RPi TX   |
| GND         | GND (pin 6)              | Wspólna masa      |

> Oba urządzenia pracują na 3.3V – można łączyć bezpośrednio bez konwertera poziomów.

### Konfiguracja CubeMX (skonfigurowane w .ioc)

1. Przejdź do: **Connectivity → USART1**.
2. Mode: **Asynchronous**.
3. Parameter Settings: Baud Rate `115200`, Word Length `8 Bits`, Parity `None`, Stop Bits `1`.
4. PA9 → `USART1_TX`, PA10 → `USART1_RX` – przypisywane automatycznie.

---

## 9. Podsumowanie zajętych timerów

| Timer | Kanał | Pin | Label     | Zastosowanie       | Prescaler | Period |
|-------|-------|-----|-----------|--------------------|-----------|--------|
| TIM1  | CH1   | PA8 | M2_PWMA   | Mostek 2 silnik A  | 15        | 1000   |
| TIM3  | CH1   | PB4 | M1_PWMA   | Mostek 1 silnik A  | 15        | 1000   |
| TIM3  | CH2   | PC7 | M2_PWMB   | Mostek 2 silnik B  | 15        | 1000   |
| TIM4  | CH1   | PB6 | SERVO_PWM | Serwo S007M        | 15        | 20000  |
| TIM11 | CH1   | PB9 | M1_PWMB   | Mostek 1 silnik B  | 15        | 1000   |
