# Projekt CAR – Samochód RC na STM32

Zdalnie sterowany samochód zbudowany na płytce **STM32 Nucleo F401RE** (ARM Cortex-M4). Pojazd obsługuje sterowanie przez pad PlayStation 5 (Bluetooth), autonomiczną jazdę po linii oraz omijanie przeszkód za pomocą czujnika ultradźwiękowego.

---

## Funkcje

- Sterowanie przez pad PS5 (Bluetooth)
- Jazda po linii – autonomiczne śledzenie czarnej linii na białym tle
- Omijanie przeszkód – czujnik ultradźwiękowy HC-SR04
- Sterowanie serwomechanizmem SG90 (skręt kół przednich)
- Wyświetlanie stanu na LCD 16×2 (HD44780, tryb 4-bit)
- Symulacja świateł samochodowych (LED czerwony, niebieski, żółty)

---

## Użyte komponenty

| Komponent                              | Ilość |
|----------------------------------------|-------|
| STM32 Nucleo F401RE (ARM Cortex-M4)    | 1×    |
| TB6612FNG – sterownik silników 13,5V/1A| 2×    |
| Silnik N20-BT03 micro 10:1 3000RPM 12V | 4×    |
| Micro servo 9g SG90                    | 1×    |
| Wyświetlacz LCD 16×2 (HD44780)         | 1×    |
| Czujnik HC-SR04                        | 1×    |
| LED czerwony                           | 2×    |
| LED niebieski                          | 4×    |
| LED żółty                              | 2×    |

---

## Postęp realizacji

| Moduł | Opis                                        | Status     |
|-------|---------------------------------------------|------------|
| 1     | Mechanika – servo + 4 silniki N20           | W toku     |
| 2     | Jazda zaprogramowaną sekwencją 10 kroków    | Oczekuje   |
| 3     | Jazda po linii (line follower)              | Oczekuje   |
| 4     | Sterowanie przez pad PS5 (Bluetooth)        | Oczekuje   |

**Aktualnie zaimplementowane:**
- Sterownik silników N20 przez dwa mostki TB6612FNG (`motor.c / motor.h`)
- Sterownik serwa SG90 przez PWM (`servo.c / servo.h`)
- Sterownik LCD HD44780 w trybie 4-bit (`lcd.c / lcd.h`)
- Obsługa joysticka analogowego HW-504 z kalibracją (`joystick.c / joystick.h`)
- Pętla główna łącząca joystick → silniki + servo z feedbackiem na LCD

---

## Szybki start

1. Sklonuj repozytorium:
   ```bash
   git clone https://github.com/Chmielu6969/Project_CAR.git
   ```
2. Otwórz STM32CubeIDE: **File → Open Projects from File System** → wskaż katalog `Project_CAR`.
3. Zbuduj projekt: **Project → Build All** (`Ctrl+B`).
4. Podłącz Nucleo przez USB i wgraj firmware: **Run → Run** (`F11`).

Szczegółowe informacje o podłączeniu, timerach i strukturze plików znajdziesz w [dokumentacji technicznej](DOCS.md).

---

## Licencja

Ten projekt jest dostępny na licencji [MIT](LICENSE).
