# Projekt CAR – Samochód RC na STM32

Zdalnie sterowany samochód zbudowany na płytce **STM32 Nucleo F401RE**. Pojazd obsługuje sterowanie przez pad PlayStation 3 (Bluetooth), jazdę po linii (line follower) oraz autonomiczne omijanie przeszkód za pomocą czujnika HC-SR04.

---

## Funkcje

- Sterowanie przez pad PS3 (Bluetooth)
- Jazda po linii – autonomiczne śledzenie czarnej linii na białym tle
- Omijanie przeszkód – czujnik ultradźwiękowy HC-SR04
- Sterowanie serwomechanizmem SG90 (skręt kół)
- Wyświetlanie stanu na LCD 16x2 (HD44780, tryb 4-bit)
- Symulacja świateł samochodowych (LED czerwony, niebieski, żółty)

---

## Sprzęt

| Komponent                             | Ilość |
|---------------------------------------|-------|
| STM32 Nucleo F401RE (ARM Cortex-M4)   | 1×    |
| TB6612FNG – sterownik silników        | 2×    |
| Silnik N20-BT03 micro 10:1 3000RPM 12V| 4×    |
| Micro servo 9g SG90                   | 1×    |
| Wyświetlacz LCD 16x2 (HD44780)        | 1×    |
| Czujnik HC-SR04                       | 1×    |
| LED czerwony                          | 2×    |
| LED niebieski                         | 4×    |
| LED żółty                             | 2×    |

---

## Schemat podłączenia

### Wyświetlacz LCD (tryb 4-bit)

| Pin LCD   | Funkcja  | Pin Nucleo | Uwagi             |
|-----------|----------|------------|-------------------|
| 1 (GND)   | VSS      | GND        |                   |
| 2 (VDD)   | VCC      | +5V        |                   |
| 3 (VO)    | Kontrast | GND / pot. | pot. 10k zalecany |
| 4 (RS)    | RS       | PC10       | GPIO Output       |
| 5 (RW)    | R/W      | GND        | na stałe          |
| 6 (E)     | Enable   | PC12       | GPIO Output       |
| 11 (D4)   | DB4      | PC0        | GPIO Output       |
| 12 (D5)   | DB5      | PC1        | GPIO Output       |
| 13 (D6)   | DB6      | PC2        | GPIO Output       |
| 14 (D7)   | DB7      | PC3        | GPIO Output       |
| 15 (BLA)  | Backlight+| +5V       |                   |
| 16 (BLK)  | Backlight-| GND       |                   |

### Micro servo SG90

| Przewód        | Pin Nucleo | Funkcja            |
|----------------|------------|--------------------|
| Żółty (sygnał) | PB6        | TIM4 CH1, AF2, PWM |
| Czerwony (VCC) | +5V        | CN7 pin 18         |
| Brązowy (GND)  | GND        |                    |

> Zworka: PB6–PA7

### Mostek 1 (TB6612FNG) – silniki lewe

| Pin mostka | Pin Nucleo | Funkcja      |
|------------|------------|--------------|
| STDBY      | PB0        | GPIO Output  |
| AIN1       | PB1        | GPIO Output  |
| AIN2       | PB3        | GPIO Output  |
| PWMA       | PB4        | TIM3 CH1 AF2 |
| BIN1       | PB5        | GPIO Output  |
| BIN2       | PB8        | GPIO Output  |
| PWMB       | PB9        | TIM4 CH4 AF2 |

### Mostek 2 (TB6612FNG) – silniki prawe

| Pin mostka | Pin Nucleo | Funkcja      |
|------------|------------|--------------|
| STDBY      | PC8        | GPIO Output  |
| AIN1       | PC9        | GPIO Output  |
| AIN2       | PA4        | GPIO Output  |
| PWMA       | PA8        | TIM1 CH1 AF1 |
| BIN1       | PC4        | GPIO Output  |
| BIN2       | PC5        | GPIO Output  |
| PWMB       | PC7        | TIM3 CH2 AF2 |

### Przycisk użytkownika

| Komponent | Pin Nucleo | Funkcja             |
|-----------|------------|---------------------|
| B1 (USER) | PC13       | GPIO Input, Pull-up |

### Podsumowanie timerów

| Timer | Kanał | Pin | Zastosowanie  |
|-------|-------|-----|---------------|
| TIM1  | CH1   | PA8 | Mostek2 PWMA  |
| TIM3  | CH1   | PB4 | Mostek1 PWMA  |
| TIM3  | CH2   | PC7 | Mostek2 PWMB  |
| TIM4  | CH1   | PB6 | Servo SG90    |
| TIM4  | CH4   | PB9 | Mostek1 PWMB  |

---

## Struktura projektu

```
Project_CAR/
├── Core/
│   ├── Inc/
│   │   ├── lcd.h          # Sterownik LCD HD44780 (4-bit)
│   │   ├── servo.h        # Sterownik serwomechanizmu SG90
│   │   └── main.h
│   └── Src/
│       ├── lcd.c          # Implementacja LCD
│       ├── servo.c        # Implementacja servo (TIM4 CH1 PWM)
│       └── main.c         # Główna pętla aplikacji
├── Drivers/               # STM32 HAL + CMSIS (generowane przez CubeMX)
├── Project_CAR.ioc        # Konfiguracja STM32CubeMX
└── STM32F401RETX_FLASH.ld # Skrypt linkera
```

---

## Postęp realizacji

| Moduł | Opis | Status |
|-------|------|--------|
| 1 | Mechanika – servo + 4 silniki N20 | 🔄 W toku |
| 2 | Jazda zaprogramowaną sekwencją 10 kroków | ⏳ Oczekuje |
| 3 | Jazda po linii (line follower) | ⏳ Oczekuje |
| 4 | Sterowanie przez pad PS3 (Bluetooth) | ⏳ Oczekuje |

**Aktualnie zaimplementowane:**
- Sterownik LCD HD44780 w trybie 4-bit (`lcd.c` / `lcd.h`)
- Sterownik serwa SG90 przez PWM TIM4 CH1 (`servo.c` / `servo.h`)
- Obsługa przycisku użytkownika (PC13) z debouncingiem – togglowanie pozycji serwa lewo/prawo

---

## Budowanie i wgrywanie

### Wymagania

- [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) (zalecana wersja 1.14+)
- Kabel micro-USB do programowania przez ST-Link wbudowany w Nucleo

### Kroki

1. Sklonuj repozytorium:
   ```bash
   git clone https://github.com/kacper7011/Project_CAR.git
   ```
2. Otwórz STM32CubeIDE i zaimportuj projekt: **File → Open Projects from File System** → wskaż katalog `Project_CAR`.
3. Zbuduj projekt: **Project → Build All** (`Ctrl+B`).
4. Podłącz Nucleo przez USB i wgraj firmware: **Run → Run** (`F11`).

### Modyfikacja konfiguracji peryferiów

Wszelkie zmiany pinów, timerów lub innych peryferiów wykonuj w **STM32CubeMX** przez plik `Project_CAR.ioc`, a następnie wygeneruj kod ponownie (**Project → Generate Code**). Nie edytuj sekcji oznaczonych `/* USER CODE BEGIN/END */` – są nadpisywane przez generator.

---

## Platforma

**STM32 Nucleo F401RE**
- Mikrokontroler: STM32F401RE
- Rdzeń: ARM Cortex-M4 @ 84 MHz
- Flash: 512 KB, RAM: 96 KB
- Zasilanie: 3,3 V / 5 V przez złącza CN7/CN10

---

## Licencja

Ten projekt jest dostępny na licencji [MIT](LICENSE).
