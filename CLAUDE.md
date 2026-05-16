## Cel projektu

Budowa samochodu zdalnie sterowanego RC w karoserii Ferrari SF90 XX Stradale, sterowanego za pomocą kontrolera PlayStation 5. Projekt oparty na mikrokontrolerze STM32 Nucleo-F401RE z modułem Raspberry Pi Zero 2W jako mostem Bluetooth. Pojazd porusza się autonomicznie po linii (8-kanałowy czujnik IR) oraz omija przeszkody za pomocą czujnika ultradźwiękowego HC-SR04.

---

## Główne funkcje

- Sterowanie przez pad PS5 (Bluetooth via Raspberry Pi Zero 2W)
- Jazda po linii (line follower) – 8-kanałowy czujnik IR (1 moduł)
- Omijanie przeszkód – czujnik HC-SR04
- Wyświetlacz trybu jazdy – lewy okrągły TFT 1.28" GC9A01
- Wyświetlacz prędkości – prawy okrągły TFT 1.28" GC9A01
- Wyświetlacz stanu PS5 – centralny prostokątny TFT 2.0" GMT020-02-7P
- Sterowanie serwomechanizmem Digital Servo 21G S007M (skręt)

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
| 8-kanałowy moduł czujnika śledzenia IR (detektor podczerwieni) | 1x |
| Wyświetlacz 1.28 TFT (240x240, IC:GC9A01) | 2x |
| Wyświetlacz 2.0 TFT SPI (GMT020-02-7P) | 1x |
| Koszyk na akumulatory 3x18650 | 1x |
| Wskaźnik poziomu baterii DC7-40V (Lipo/Acid) | 1x |
| Złącze żeńskie typu C (Port ładowania 3A) | 1x |
| RC Car Metal Magnet Body Shell | 4x |
| Moduł GPS GY-GPS6MV2 (u-blox NEO-6M) | 1x |

---

## Platforma

Mikrokontroler **STM32 Nucleo F401RE** (ARM Cortex-M4, 16 MHz HSI) + **Raspberry Pi Zero 2W** jako most Bluetooth/UART. Szczegółowe parametry obu płytek: [`Docs/HARDWARE.md`](Docs/HARDWARE.md).

---

## Dokumentacja techniczna

Przed rozpoczęciem pracy nad jakimkolwiek modułem sprzętowym Claude **zawsze** czyta odpowiednie pliki dokumentacji:

| Plik | Kiedy czytać |
|------|--------------|
| [`Docs/HARDWARE.md`](Docs/HARDWARE.md) | Przed pisaniem kodu dla serwa, silników, TFT, HC-SR04, IR, UART |
| [`Docs/RASPBERRY_CONFIG.md`](Docs/RASPBERRY_CONFIG.md) | Przed pracą z Raspberry Pi, GPS, PS5 |
| `Project_CAR.ioc` | Jako ostateczne źródło prawdy dla pinów i timerów |

**Reguła weryfikacji pinów:** Claude **nigdy nie przyjmuje pinów ani parametrów timerów z pamięci** — każda wartość musi być zweryfikowana w `Project_CAR.ioc` lub `Docs/HARDWARE.md` przed użyciem w kodzie.

---

## Zasady pracy z Claude

### Konfiguracja przez plik .ioc (STM32CubeMX)

- Claude **nigdy nie edytuje pliku `.ioc` samodzielnie** – wszelkie zmiany konfiguracji peryferiów (timery, GPIO, UART, PWM, DMA, zegary itp.) muszą być wykonane ręcznie przez użytkownika w STM32CubeMX.
- Gdy dana funkcjonalność wymaga zmian w `.ioc`, Claude **zatrzymuje się** i przed napisaniem jakiegokolwiek kodu przedstawia użytkownikowi dokładną instrukcję konfiguracji, np.:

```
⚙️ Wymagana konfiguracja w STM32CubeMX (.ioc):

1. Otwórz plik .ioc w STM32CubeMX.
2. Przejdź do zakładki „Pinout & Configuration".
3. W sekcji „Timers" wybierz <TIMER>.
4. Ustaw „Clock Source" na „Internal Clock".
5. W „Channel N" wybierz „PWM Generation CHN".
6. Przejdź do „Parameter Settings" i ustaw:
   - Prescaler: <wartość>
   - Counter Period: <wartość>
   - Pulse: <wartość>
7. W zakładce „Pinout" upewnij się, że <PIN> jest przypisany do <TIMER_CHN>.
8. Kliknij „Generate Code".

Daj znać gdy gotowe – wtedy napiszę kod korzystający z tej konfiguracji.
```

- Claude czeka na potwierdzenie przez użytkownika że konfiguracja została zastosowana i kod wygenerowany, zanim przejdzie do implementacji.

---

### Styl pisania kodu

- Kod jest pisany w małych, czytelnych fragmentach – każda funkcja robi jedną rzecz.
- Nazwy zmiennych, funkcji i plików są opisowe i jednoznaczne.
- Każdy fragment kodu zawiera komentarze wyjaśniające cel i działanie.
- Unika się „magicznych liczb" – stałe są definiowane przez `#define` lub `const` z nazwą.
- Kod jest dzielony na logiczne moduły (pliki `.c` / `.h`) odpowiadające komponentom sprzętowym.

### Praca z gałęziami Git

- Cały kod projektu jest przechowywany na GitHubie.
- Każdy nowy feature (funkcjonalność) dostaje **osobny branch** o nazwie odpowiadającej tej funkcjonalności, np.:
  - `feature/lcd-display`
  - `feature/hc-sr04-obstacle`
  - `feature/ps5-bluetooth`
  - `feature/line-follower`
- Claude pracuje **wyłącznie na branchu danego feature'a** – nigdy bezpośrednio na `main`.

### Przepływ pracy (workflow)

1. **Rozpoczęcie feature'a** – Claude tworzy nowy branch i informuje użytkownika o jego nazwie.
2. **Implementacja** – kod jest pisany i commitowany stopniowo, w małych logicznych krokach.
3. **Zakończenie pracy** – gdy implementacja jest gotowa, Claude przedstawia użytkownikowi podsumowanie w trzech punktach:
   - **Co zostało zrobione** – lista zmian i dodanych plików.
   - **Jak to ręcznie przetestować** – konkretne kroki weryfikacji na sprzęcie lub w debuggerze.
   - **Jak to łączy się z resztą projektu** – zależności od innych modułów, piny, peryferia.
4. **Oczekiwanie na zatwierdzenie** – Claude czeka na potwierdzenie od użytkownika, że wszystko działa poprawnie. Nie wykonuje żadnych dalszych działań bez tej zgody. Dopiero gdy otrzyma zatwierdzenie robi claude robi commit.
5. **Merge i push** – po otrzymaniu zatwierdzenia Claude merguje branch do `main` i wypycha zmiany do GitHuba (`git push origin main`).

### Przykładowy komunikat po zakończeniu feature'a

```
✅ Feature gotowy: feature/hc-sr04-obstacle

Co zrobiłem:
- Dodałem plik hc_sr04.c / hc_sr04.h z funkcją pomiaru odległości przez GPIO.
- Trigger wysyłany jest przez PA0 (10 µs impuls), Echo odczytywane na PA1.
- Zmierzona odległość zwracana jest w centymetrach jako uint16_t.

Jak przetestować:
1. Wgraj firmware na Nucleo.
2. Otwórz terminal szeregowy (115200 baud).
3. Ustaw przeszkodę w odległości ~20 cm od czujnika.
4. Sprawdź, czy wyświetlana wartość to ok. 20 cm (±2 cm).

Jak to łączy się z projektem:
- Moduł będzie wywoływany w pętli głównej co 100 ms.
- Wynik trafia do logiki sterowania silnikami (motor_control.c).
- Nie ma konfliktu z innymi timerami – HC-SR04 używa opóźnień przez DWT.

Czy mogę zmergować do main?
```
