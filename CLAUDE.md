# Projekt RC_CAR Ferrari SF90 XX Stradale

## Autorzy

- Jakub Chmielewski 21432
- Kacper Siemieniako 21560
- Bartek Mikołajczyk 21513

## Cel projektu

Budowa samochodu zdalnie sterowanego RC w karoserii Ferrari SF90 XX Stradale, sterowanego za pomocą kontrolera PlayStation 5. Projekt oparty na mikrokontrolerze STM32 Nucleo-F401RE z modułem Raspberry Pi Zero 2W jako mostem Bluetooth. Pojazd porusza się autonomicznie po linii (8-kanałowy czujnik IR) oraz omija przeszkody za pomocą czujnika ultradźwiękowego HC-SR04.

---

## Główne funkcje

- Sterowanie przez pad PS5 (Bluetooth via Raspberry Pi Zero 2W)
- Jazda po linii (line follower) – 8-kanałowy czujnik IR (2 moduły)
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
| 8-kanałowy moduł czujnika śledzenia IR (detektor podczerwieni) | 2x |
| Wyświetlacz 1.28 TFT (240x240, IC:GC9A01) | 2x |
| Wyświetlacz 2.0 TFT SPI (GMT020-02-7P) | 1x |
| Koszyk na akumulatory 3x18650 | 1x |
| Wskaźnik poziomu baterii DC7-40V (Lipo/Acid) | 1x |
| Złącze żeńskie typu C (Port ładowania 3A) | 1x |
| RC Car Metal Magnet Body Shell | 4x |

---

## Schemat podłączenia

### Digital Servo 21G Model S007M

| Przewód        | Pin Nucleo / Złącze | Funkcja     |
|----------------|---------------------|-------------|
| Żółty (sygnał) | PB6 [TIM4 CH1, AF2] | Sygnał PWM  |
| Czerwony (VCC) | +5V (CN7 pin 18)    | VCC         |
| Brązowy (GND)  | GND                 | Masa        |

> Zworka: PA6–PA5

### Silniki N20 – Mostek 1 (TB6612FNG)

| Pin mostka | Pin Nucleo | Złącze | Funkcja         |
|------------|------------|--------|-----------------|
| STDBY      | PB0        | CN7    | GPIO Output     |
| AIN1       | PB1        | CN7    | GPIO Output     |
| AIN2       | PB3        | CN7    | GPIO Output     |
| PWMA       | PB4        | CN7    | TIM3 CH1 AF2    |
| BIN1       | PB5        | CN7    | GPIO Output     |
| BIN2       | PB8        | CN7    | GPIO Output     |
| PWMB       | PB9        | CN7    | TIM4 CH4 AF2    |

### Silniki N20 – Mostek 2 (TB6612FNG)

| Pin mostka | Pin Nucleo | Złącze | Funkcja         |
|------------|------------|--------|-----------------|
| STDBY      | PC8        | CN10   | GPIO Output     |
| AIN1       | PC9        | CN10   | GPIO Output     |
| AIN2       | PA4        | CN10   | GPIO Output     |
| PWMA       | PA8        | CN10   | TIM1 CH1 AF1    |
| BIN1       | PC4        | CN10   | GPIO Output     |
| BIN2       | PC5        | CN10   | GPIO Output     |
| PWMB       | PC7        | CN10   | TIM3 CH2 AF2    |

### Przycisk użytkownika

| Komponent | Pin Nucleo | Funkcja               |
|-----------|------------|-----------------------|
| B1 (USER) | PC13       | GPIO Input, Pull-up   |

### Czujnik HC-SR04

> Piny zostaną przypisane przy implementacji modułu omijania przeszkód.

### Czujniki IR (2× moduł 8-kanałowy)

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

| Pin  | Kanał   | Uwaga                                             |
|------|---------|---------------------------------------------------|
| PB12 | IR2_CH1 |                                                   |
| PB14 | IR2_CH2 |                                                   |
| PC12 | IR2_CH3 |                                                   |
| PD2  | IR2_CH4 |                                                   |
| PA11 | IR2_CH5 | USB_DM – brak złącza USB mikrokontrolera na Nucleo |
| PA12 | IR2_CH6 | USB_DP – j.w.                                     |
| PC14 | IR2_CH7 | OSC32_IN – GPIO gdy LSE wyłączone w CubeMX        |
| PC15 | IR2_CH8 | OSC32_OUT – j.w.                                  |

> Konfiguracja CubeMX: GPIO Input, Pull-up. Dla PA11/PA12: USB nie włączone. Dla PC14/PC15: RCC → LSE = Disable.

### Wyświetlacze TFT (SPI2, na tyle pojazdu)

Wspólna magistrala SPI2, różnicowane przez osobne piny CS:

| Pin  | Sygnał        | Opis                                      |
|------|---------------|-------------------------------------------|
| PB13 | SPI2_SCK      | Zegar SPI (AF5)                           |
| PB15 | SPI2_MOSI     | Dane do wyświetlaczy (AF5)                |
| PC0  | TFT_LEFT_CS   | Chip select – lewy GC9A01 (tryb jazdy)    |
| PC1  | TFT_LEFT_DC   | Data/Command – lewy GC9A01               |
| PC2  | TFT_RIGHT_CS  | Chip select – prawy GC9A01 (prędkość)    |
| PC3  | TFT_RIGHT_DC  | Data/Command – prawy GC9A01              |
| PC6  | TFT_CENTER_CS | Chip select – środkowy GMT020-02-7P (PS5) |
| PC10 | TFT_CENTER_DC | Data/Command – środkowy GMT020-02-7P     |
| PC11 | TFT_RST       | Reset wspólny (wszystkie 3)               |

> Konfiguracja CubeMX: SPI2 Transmit Only Master, Prescaler /4 (~21 MHz). PC0/PC1/PC2/PC3/PC6/PC10/PC11: GPIO Output push-pull.

### Podsumowanie zajętych timerów

| Timer    | Kanał | Pin | Zastosowanie      |
|----------|-------|-----|-------------------|
| TIM1     | CH1   | PA8 | Mostek2 PWMA      |
| TIM3     | CH1   | PB4 | Mostek1 PWMA      |
| TIM3     | CH2   | PC7 | Mostek2 PWMB      |
| TIM4     | CH1   | PB6 | Servo S007M       |
| TIM4     | CH4   | PB9 | Mostek1 PWMB      |

---

## Platforma

**STM32 Nucleo F401RE** – mikrokontroler STM32F401RE, rdzeń ARM Cortex-M4, zasilanie 3,3V / 5V przez złącza CN7/CN10.

**Raspberry Pi Zero 2W** – most Bluetooth/UART między kontrolerem PS5 a STM32. Odbiera dane z pada przez Bluetooth i przesyła komendy do STM32 przez UART.

**Zasilanie:** Koszyk na 3 ogniwa 18650, wskaźnik poziomu baterii DC7-40V, ładowanie przez złącze USB-C 3A.

---

## Plan realizacji projektu

Moduły są realizowane **w podanej kolejności** – każdy następny zaczyna się dopiero po zatwierdzeniu i zmergowaniu poprzedniego do `main`. Po zatwierdzeniu przez użytkownika Claude oznacza moduł jako ukończony (✅).

---

### Moduł 1 – Mechanika robota

- [ ] **Cel:** Robot jest złożony i gotowy do programowania – jedzie prosto, skręca, zatrzymuje się na komendę. Serwo obraca się do zadanej pozycji.

  **Kryteria ukończenia:**
  - Serwo SG90 obraca się płynnie do skrajnych pozycji (lewo/prawo/środek).
  - Wszystkie 4 silniki N20 kręcą się w dobrym kierunku przy komendach przód/tył.
  - Sterowniki TB6612FNG nie grzeją się ponad normę, zasilanie stabilne.
  - Robot fizycznie jedzie prosto bez wyraźnego znoszenia.

  > Claude sam decyduje od czego zacząć (np. najpierw samo obrócenie serwa jedną funkcją, potem jeden silnik, potem para, potem wszystkie cztery) i przedstawia plan kroków na początku pracy nad modułem.

---

### Moduł 2 – Jazda zaprogramowaną sekwencją

- [ ] **Cel:** Robot wykonuje z góry zaprogramowaną sekwencję 10 rozkazów (przód, tył, lewo, prawo) bez ingerencji użytkownika.

  **Kryteria ukończenia:**
  - Zdefiniowane co najmniej 4 typy rozkazów: `FORWARD`, `BACKWARD`, `LEFT`, `RIGHT`.
  - Każdy rozkaz ma parametr czasu trwania [ms].
  - Sekwencja 10 kroków wykonuje się jednorazowo po starcie i zatrzymuje po ostatnim kroku.
  - Robot powtarzalnie przejeżdża tę samą trasę przy każdym resecie.

  > Claude sam decyduje o kolejności kroków implementacji i przedstawia plan na początku pracy nad modułem.

---

### Moduł 3 – Jazda po linii (line follower)

- [ ] **Cel:** Robot autonomicznie śledzi czarną linię na białym tle, radząc sobie z zakrętem prostym (90°) oraz łukiem o promieniu 15 cm.

  **Kryteria ukończenia:**
  - Robot nie gubi linii na prostym odcinku.
  - Robot poprawnie pokonuje zakręt złamany pod kątem 90°.
  - Robot poprawnie pokonuje łuk o promieniu 15 cm.
  - Po zgubieniu linii robot zatrzymuje się (nie jedzie w nieskończoność).

  > Claude sam decyduje o kolejności kroków implementacji (np. najpierw odczyt jednego czujnika, potem logika korekcji, potem obsługa zakrętu 90°) i przedstawia plan na początku pracy nad modułem.

---

### Moduł 4 – Sterowanie przez pad PS5 (Bluetooth) *(moduł końcowy)*

- [ ] **Cel:** Użytkownik steruje robotem w czasie rzeczywistym za pomocą pada PS5 przez Bluetooth. Możliwe jest przełączanie między trybem ręcznym a autonomicznym (line follower).

  **Kryteria ukończenia:**
  - Pad PS5 paruje się z robotem i połączenie jest stabilne.
  - Joystick / D-pad steruje kierunkiem i prędkością jazdy.
  - Jeden z przycisków zatrzymuje robota awaryjnie.
  - Możliwe przełączenie między trybem ręcznym (pad) a autonomicznym (line follower).

  > Claude sam decyduje o kolejności kroków implementacji i przedstawia plan na początku pracy nad modułem.

---

> **Instrukcja dla Claude:** Przed rozpoczęciem pracy nad modułem przedstaw użytkownikowi swój plan kroków implementacji (od najprostszego do najbardziej złożonego) i poczekaj na akceptację. Po zatwierdzeniu modułu przez użytkownika zaktualizuj listę – zmień `- [ ]` na `- [x]` oraz dodaj ✅ przed nagłówkiem modułu (np. `### ✅ Moduł 1`). Następnie przejdź do kolejnego modułu.

---

## Zasady pracy z Claude

### Konfiguracja przez plik .ioc (STM32CubeMX)

- Claude **nigdy nie edytuje pliku `.ioc` samodzielnie** – wszelkie zmiany konfiguracji peryferiów (timery, GPIO, UART, PWM, DMA, zegary itp.) muszą być wykonane ręcznie przez użytkownika w STM32CubeMX.
- Gdy dana funkcjonalność wymaga zmian w `.ioc`, Claude **zatrzymuje się** i przed napisaniem jakiegokolwiek kodu przedstawia użytkownikowi dokładną instrukcję konfiguracji, np.:

```
⚙️ Wymagana konfiguracja w STM32CubeMX (.ioc):

1. Otwórz plik .ioc w STM32CubeMX.
2. Przejdź do zakładki „Pinout & Configuration".
3. W sekcji „Timers" wybierz TIM3.
4. Ustaw „Clock Source" na „Internal Clock".
5. W „Channel 1" wybierz „PWM Generation CH1".
6. Przejdź do „Parameter Settings" i ustaw:
   - Prescaler: 83
   - Counter Period: 999
   - Pulse: 0
7. W zakładce „Pinout" upewnij się, że PA6 jest przypisany do TIM3_CH1.
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
4. **Oczekiwanie na zatwierdzenie** – Claude czeka na potwierdzenie od użytkownika, że wszystko działa poprawnie. Nie wykonuje żadnych dalszych działań bez tej zgody.
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
