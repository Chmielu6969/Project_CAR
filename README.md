# RC_CAR Ferrari SF90 XX Stradale

Zdalnie sterowany samochód RC w karoserii Ferrari SF90 XX Stradale, zbudowany na płytce **STM32 Nucleo F401RE** (ARM Cortex-M4). Sterowanie odbywa się przez kontroler PlayStation 5 via Bluetooth (Raspberry Pi Zero 2W jako most UART). Pojazd obsługuje autonomiczną jazdę po linii (czujniki IR) oraz omijanie przeszkód (HC-SR04).

**Autorzy:** Jakub Chmielewski 21432 · Kacper Siemieniako 21560 · Hot Dog 6767

---

## Funkcje

- Sterowanie przez pad PS5 (Bluetooth via Raspberry Pi Zero 2W)
- Jazda po linii – 8-kanałowy czujnik IR (2 moduły, 16 czujników łącznie)
- Omijanie przeszkód – czujnik ultradźwiękowy HC-SR04
- Sterowanie serwomechanizmem Digital Servo 21G S007M (skręt kół przednich)
- Wyświetlacz trybu jazdy – lewy okrągły TFT 1.28" GC9A01
- Wyświetlacz prędkości – prawy okrągły TFT 1.28" GC9A01 (animacja prędkościomierza)
- Wyświetlacz stanu PS5 – centralny prostokątny TFT 2.0" GMT020-02-7P

---

## Użyte komponenty

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

---

## Postęp realizacji

| Moduł | Opis                                        | Status     |
|-------|---------------------------------------------|------------|
| 1     | Mechanika – servo + 4 silniki N20           | W toku     |
| 2     | Jazda zaprogramowaną sekwencją 10 kroków    | Oczekuje   |
| 3     | Jazda po linii (line follower, czujnik IR)  | Oczekuje   |
| 4     | Sterowanie przez pad PS5 (Bluetooth)        | Oczekuje   |

**Aktualnie zaimplementowane:**
- Sterownik silników N20 przez dwa mostki TB6612FNG (`motor.c / motor.h`)
- Sterownik serwa Digital Servo 21G S007M przez PWM (`servo.c / servo.h`)
- Most PS5 → STM32 przez Raspberry Pi Zero 2W + UART (`Raspberry/`)

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
Konfiguracja Raspberry Pi i instrukcja parowania PS5 – w [Raspberry/README.md](Raspberry/README.md).

---

## Licencja

Ten projekt jest dostępny na licencji [MIT](LICENSE).
