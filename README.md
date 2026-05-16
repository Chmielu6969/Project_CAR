# RC_CAR Ferrari SF90 XX Stradale

Zdalnie sterowany samochód RC w karoserii Ferrari SF90 XX Stradale, zbudowany na płytce **STM32 Nucleo F401RE** (ARM Cortex-M4). Sterowanie odbywa się przez kontroler PlayStation 5 via Bluetooth (Raspberry Pi Zero 2W jako most UART). Pojazd obsługuje autonomiczną jazdę po linii (czujnik IR) oraz omijanie przeszkód (HC-SR04).

---

## Funkcje

- Sterowanie przez pad PS5 (Bluetooth via Raspberry Pi Zero 2W)
- Jazda po linii – 8-kanałowy czujnik IR
- Omijanie przeszkód – czujnik ultradźwiękowy HC-SR04
- Sterowanie serwomechanizmem Digital Servo 21G S007M (skręt kół przednich)
- Wyświetlacz trybu jazdy – lewy okrągły TFT 1.28" GC9A01
- Wyświetlacz prędkości – prawy okrągły TFT 1.28" GC9A01
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
| 8-kanałowy moduł czujnika śledzenia IR | 1x |
| Wyświetlacz 1.28 TFT (240x240, IC:GC9A01) | 2x |
| Wyświetlacz 2.0 TFT SPI (GMT020-02-7P) | 1x |
| Moduł GPS GY-GPS6MV2 (u-blox NEO-6M) | 1x |
| Koszyk na akumulatory 3x18650 | 1x |
| Wskaźnik poziomu baterii DC7-40V | 1x |
| Złącze żeńskie USB-C (Port ładowania 3A) | 1x |

---

## Model 3D karoserii

Karoseria pojazdu została wydrukowana w 3D na podstawie gotowego modelu dostępnego na platformie Cults3D:

**[2025 Ferrari SF90 XX Stradale – Scale Hollow Shell Body (RC Car Body)](https://cults3d.com/en/3d-model/game/2025-ferrari-sf90-xx-stradale-scale-hollow-shell-body-detailed-rc-car-body-s)**

Dziękujemy autorowi modelu za udostępnienie szczegółowej karoserii przeznaczonej do samochodów RC.

---

## Szybki start

1. Sklonuj repozytorium:
   ```bash
   git clone https://github.com/Chmielu6969/Project_CAR.git
   ```
2. Otwórz STM32CubeIDE: **File → Open Projects from File System** → wskaż katalog `Project_CAR`.
3. Zbuduj projekt: **Project → Build All** (`Ctrl+B`).
4. Podłącz Nucleo przez USB i wgraj firmware: **Run → Run** (`F11`).

---

## Dokumentacja techniczna

| Plik | Zawartość |
|------|-----------|
| [Docs/HARDWARE.md](Docs/HARDWARE.md) | Schematy podłączeń i instrukcje konfiguracji STM32CubeMX per urządzenie |
| [Docs/RASPBERRY_CONFIG.md](Docs/RASPBERRY_CONFIG.md) | Konfiguracja Raspberry Pi: most BT/UART, PS5, GPS |

---

## Licencja

Ten projekt jest dostępny na licencji [MIT](LICENSE).
