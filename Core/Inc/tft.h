#ifndef TFT_H
#define TFT_H

#include "main.h"
#include <stdint.h>

/* RGB565 colors */
#define TFT_BLACK   0x0000u
#define TFT_WHITE   0xFFFFu
#define TFT_RED     0xF800u
#define TFT_GREEN   0x07E0u
#define TFT_BLUE    0x001Fu
#define TFT_YELLOW  0xFFE0u
#define TFT_CYAN    0x07FFu
#define TFT_MAGENTA 0xF81Fu

/* Resolution */
#define GC9A01_W 240u
#define GC9A01_H 240u
#define GMT020_W  240u
#define GMT020_H  320u

typedef enum {
    TFT_LEFT   = 0,   /* lewy GC9A01 240×240    – CS=PB12, DC=PB14 */
    TFT_RIGHT  = 1,   /* prawy GC9A01 240×240   – CS=PB10, DC=PB7  */
    TFT_CENTER = 2,   /* środkowy GMT020 240×320 – CS=PB2,  DC=PA15 */
} TFT_Display_t;

void TFT_Init(void);
void TFT_FillColor(TFT_Display_t disp, uint16_t color);
void TFT_FillRect(TFT_Display_t disp, uint16_t x0, uint16_t y0,
                  uint16_t x1, uint16_t y1, uint16_t color);

#endif /* TFT_H */
