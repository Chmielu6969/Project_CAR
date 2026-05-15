#include "mode_display.h"
#include "tft.h"
#include <stdint.h>

#define MD_BG_COLOR   TFT_BLACK
#define MD_TEXT_COLOR TFT_WHITE

/* Czcionka bitmapowa 5×7 (row-major, bit4 = lewy skrajny piksel).
   Każdy piksel renderowany jako kwadrat FONT_SCALE×FONT_SCALE. */
#define FONT_W     5u
#define FONT_H     7u
#define FONT_SCALE 10u   /* → znak 50×70 px, czytelny na GC9A01 240×240 */
#define FONT_GAP   10u   /* odstęp między „S" a „+" */

static const uint8_t font_sp[2][FONT_H] = {
    /* S */ {0x0E, 0x11, 0x10, 0x0E, 0x01, 0x11, 0x0E},
    /* + */ {0x00, 0x04, 0x04, 0x1F, 0x04, 0x04, 0x00},
};

static void draw_char(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t color)
{
    for (uint8_t row = 0u; row < FONT_H; row++) {
        uint8_t data = bitmap[row];
        for (uint8_t col = 0u; col < FONT_W; col++) {
            if (data & (0x10u >> col)) {
                uint16_t px = x + (uint16_t)col * FONT_SCALE;
                uint16_t py = y + (uint16_t)row * FONT_SCALE;
                TFT_FillRect(TFT_LEFT, px, py,
                             px + FONT_SCALE - 1u,
                             py + FONT_SCALE - 1u, color);
            }
        }
    }
}

void ModeDisplay_Init(void)
{
    TFT_FillColor(TFT_LEFT, MD_BG_COLOR);

    /* Wyśrodkuj „S+" na 240×240 */
    uint16_t total_w = 2u * FONT_W * FONT_SCALE + FONT_GAP;   /* 110 px */
    uint16_t total_h = FONT_H * FONT_SCALE;                    /*  70 px */
    uint16_t x_start = (GC9A01_W - total_w) / 2u;             /*  65 px */
    uint16_t y_start = (GC9A01_H - total_h) / 2u;             /*  85 px */

    for (uint8_t i = 0u; i < 2u; i++) {
        uint16_t x = x_start + (uint16_t)i * (FONT_W * FONT_SCALE + FONT_GAP);
        draw_char(x, y_start, font_sp[i], MD_TEXT_COLOR);
    }
}
