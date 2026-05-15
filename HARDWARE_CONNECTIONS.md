# Schemat podłączenia – HC-SR04 i wyświetlacze TFT

## Czujnik ultradźwiękowy HC-SR04

> Wymagane w CubeMX: **System Core → RCC → LSE: Disable** (zwalnia PC14/PC15).

| Pin czujnika | Pin Nucleo | Złącze       | Uwaga                          |
|-------------|------------|--------------|--------------------------------|
| VCC         | 5V         | CN10 U5V     | HC-SR04 wymaga 5V              |
| GND         | GND        | CN10 GND     |                                |
| TRIG        | PC14       | CN7 pin 25   | GPIO Output                    |
| ECHO        | PC15       | CN7 pin 27   | **Dzielnik napięcia 1kΩ/2kΩ** |

ECHO zwraca logikę 5V, a STM32 toleruje max 3.3V – obowiązkowy dzielnik napięcia:

```
HC_ECHO (5V) ──[1 kΩ]──┬── PC15 (STM32)
                        │
                      [2 kΩ]
                        │
                       GND
```

---

## Wyświetlacze TFT – wspólna magistrala SPI2

Trzy wyświetlacze współdzielą SPI2 (SCK + MOSI). Wybór aktywnego wyświetlacza przez osobne linie CS. Wspólna linia RST resetuje wszystkie jednocześnie.

### Sygnały wspólne SPI2

| Sygnał     | Pin Nucleo | Złącze       |
|------------|------------|--------------|
| SPI2_SCK   | PB13       | CN10 pin 30  |
| SPI2_MOSI  | PB15       | CN10 pin 26  |
| TFT_RST    | PC11       | CN7 (morpho) |

### Lewy GC9A01 1.28" – tryb jazdy

| Pin wyświetlacza | Pin Nucleo | Złącze       |
|-----------------|------------|--------------|
| CS              | PB12       | CN10 pin 29  |
| DC (RS)         | PB14       | CN10 pin 28  |
| SCL             | PB13       | —            |
| SDA             | PB15       | —            |
| RST             | PC11       | —            |
| VCC             | 3.3V       | —            |
| GND             | GND        | —            |

### Prawy GC9A01 1.28" – prędkościomierz

| Pin wyświetlacza | Pin Nucleo | Złącze   |
|-----------------|------------|----------|
| CS              | PB10       | CN9 D6   |
| DC (RS)         | PB7        | CN7 pin 21 |
| SCL             | PB13       | —        |
| SDA             | PB15       | —        |
| RST             | PC11       | —        |
| VCC             | 3.3V       | —        |
| GND             | GND        | —        |

### Środkowy GMT020-02-7P 2.0" – stan PS5

| Pin wyświetlacza | Pin Nucleo | Złącze     |
|-----------------|------------|------------|
| CS              | PB2        | CN10 pin 22|
| DC (RS)         | PA15       | CN7 pin 17 |
| SCL             | PB13       | —          |
| SDA             | PB15       | —          |
| RST             | PC11       | —          |
| VCC             | 3.3V       | —          |
| GND             | GND        | —          |

---

## Konfiguracja CubeMX – wyświetlacze TFT

### SPI2

1. **Connectivity → SPI2**
2. Mode: **Transmit Only Master**
3. Hardware NSS Signal: **Disable**
4. Parameter Settings:
   - Prescaler: `/4` (~21 MHz przy APB1 42 MHz)
   - CPOL: Low, CPHA: 1 Edge, First Bit: MSB First, Data Size: 8 Bits
5. Piny: PB13 → SPI2_SCK (AF5), PB15 → SPI2_MOSI (AF5)

### GPIO Output – CS / DC / RST

Dla pinów: **PB2, PB7, PB10, PB12, PB14, PA15, PC11**

- GPIO output level: High
- GPIO mode: Output Push Pull
- GPIO Pull-up/Pull-down: No pull
- Maximum output speed: High

Etykiety (User Label):

| Pin  | Etykieta      |
|------|---------------|
| PB12 | TFT_LEFT_CS   |
| PB14 | TFT_LEFT_DC   |
| PB10 | TFT_RIGHT_CS  |
| PB7  | TFT_RIGHT_DC  |
| PB2  | TFT_CENTER_CS |
| PA15 | TFT_CENTER_DC |
| PC11 | TFT_RST       |

---

## Konfiguracja CubeMX – HC-SR04

1. **System Core → RCC → Low Speed Clock (LSE): Disable**
2. PC14 → GPIO_Output, User Label: `HC_TRIG`, Level: Low
3. PC15 → GPIO_Input, User Label: `HC_ECHO`, No pull
4. Podłącz fizyczny dzielnik 1kΩ/2kΩ na linii ECHO (patrz schemat powyżej)
