# Projekt CAR_123 – Samochód RC na STM32

## Cel projektu

Budowa zdalnie sterowanego samochodu z użyciem pada PlayStation 5, działającego na płytce STM32 Nucleo F401RE. Pojazd ma jeździć autonomicznie po linii oraz omijać przeszkody za pomocą czujnika ultradźwiękowego HC-SR04.

---

## Główne funkcje

- Sterowanie przez pad PS5 (Bluetooth)
- Jazda po linii (line follower)
- Omijanie przeszkód – czujnik HC-SR04
- Symulacja świateł samochodowych (LED czerwony, niebieski, żółty)
- Wyświetlanie informacji na LCD
- Sterowanie serwomechanizmem (skręt)

---

## Użyte komponenty

| Komponent | Ilość |
|---|---|
| STM32 Nucleo F401RE (ARM Cortex M4) | 1x |
| TB6612FNG – sterownik silników 13,5V/1A | 2x |
| Silnik N20-BT03 micro 10:1 3000RPM 12V | 4x |
| Micro servo 9g SG90 | 1x |
| Wyświetlacz LCD (16-pin, tryb 4-bit) | 1x |
| Czujnik HC-SR04 | 1x |
| LED czerwony | 2x |
| LED niebieski | 4x |
| LED żółty | 2x |

---

## Schemat podłączenia

### Wyświetlacz LCD (tryb 4-bit, HD44780)

| Pin LCD | Funkcja    | Pin Nucleo | Uwagi              |
|---------|------------|------------|--------------------|
| 1 (GND) | VSS        | GND        |                    |
| 2 (VDD) | VCC        | +5V        |                    |
| 3 (VO)  | Kontrast   | GND / pot. | pot. 10k zalecany  |
| 4 (RS)  | RS         | PC10       | GPIO Output        |
| 5 (RW)  | R/W        | GND        | na stałe           |
| 6 (E)   | Enable     | PC12       | GPIO Output        |
| 11 (D4) | DB4        | PC0        | GPIO Output        |
| 12 (D5) | DB5        | PC1        | GPIO Output        |
| 13 (D6) | DB6        | PC2        | GPIO Output        |
| 14 (D7) | DB7        | PC3        | GPIO Output        |
| 15 (BLA)| Backlight+ | +5V        |                    |
| 16 (BLK)| Backlight- | GND        |                    |

### Micro servo 9g SG90

| Przewód       | Pin Nucleo | Funkcja            |
|---------------|------------|--------------------|
| Żółty (sygnał)| PB6        | TIM4 CH1, AF2, PWM |
| Czerwony (VCC)| +5V        | CN7 pin 18         |
| Brązowy (GND) | GND        |                    |

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

### Podsumowanie zajętych timerów

| Timer    | Kanał | Pin | Zastosowanie      |
|----------|-------|-----|-------------------|
| TIM1     | CH1   | PA8 | Mostek2 PWMA      |
| TIM3     | CH1   | PB4 | Mostek1 PWMA      |
| TIM3     | CH2   | PC7 | Mostek2 PWMB      |
| TIM4     | CH1   | PB6 | Servo SG90        |
| TIM4     | CH4   | PB9 | Mostek1 PWMB      |

---

## Platforma

**STM32 Nucleo F401RE** – mikrokontroler STM32F401RE, rdzeń ARM Cortex-M4, zasilanie 3,3V / 5V przez złącza CN7/CN10.

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
