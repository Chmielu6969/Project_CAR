#ifndef SPEEDOMETER_H
#define SPEEDOMETER_H

#include <stdint.h>

/* Inicjalizacja – czyści TFT_RIGHT i rysuje etykietę KM/H */
void Speedometer_Init(void);

/* Aktualizacja cyfr prędkości; rysuje tylko gdy wartość się zmieniła */
void Speedometer_Update(uint16_t speed_kmh);

#endif /* SPEEDOMETER_H */
