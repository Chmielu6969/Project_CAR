#include "speedometer.h"
#include "tft.h"
#include <stdint.h>

/* ─── Kolory ─── */
#define SPD_BG_COLOR    TFT_BLACK
#define SPD_DIGIT_COLOR TFT_RED
#define SPD_LABEL_COLOR TFT_WHITE

/* ─── Geometria cyfry 7-segmentowej ─── */
#define DIG_W   50u   /* szerokość jednej cyfry [px] */
#define DIG_H   80u   /* wysokość jednej cyfry [px]  */
#define DIG_T    8u   /* grubość segmentu [px]       */
#define DIG_GAP  6u   /* odstęp między cyframi [px]  */

/* Pozycja Y cyfr (wyśrodkowana z marginesem dla okrągłej ramki GC9A01) */
#define DIG_Y   35u

/* Pozycja Y etykiety „KM/H" */
#define LBL_Y  127u   /* DIG_Y + DIG_H + 12 */

/* ─── Bitmaska segmentów ─── */
#define SEG_TOP (1u << 0)  /* górna pozioma       */
#define SEG_TL  (1u << 1)  /* lewa pionowa górna  */
#define SEG_TR  (1u << 2)  /* prawa pionowa górna */
#define SEG_MID (1u << 3)  /* środkowa pozioma    */
#define SEG_BL  (1u << 4)  /* lewa pionowa dolna  */
#define SEG_BR  (1u << 5)  /* prawa pionowa dolna */
#define SEG_BOT (1u << 6)  /* dolna pozioma       */

static const uint8_t digit_seg[10] = {
    /* 0 */ SEG_TOP | SEG_TL | SEG_TR | SEG_BL | SEG_BR | SEG_BOT,
    /* 1 */ SEG_TR  | SEG_BR,
    /* 2 */ SEG_TOP | SEG_TR | SEG_MID | SEG_BL | SEG_BOT,
    /* 3 */ SEG_TOP | SEG_TR | SEG_MID | SEG_BR | SEG_BOT,
    /* 4 */ SEG_TL  | SEG_TR | SEG_MID | SEG_BR,
    /* 5 */ SEG_TOP | SEG_TL | SEG_MID | SEG_BR | SEG_BOT,
    /* 6 */ SEG_TOP | SEG_TL | SEG_MID | SEG_BL | SEG_BR | SEG_BOT,
    /* 7 */ SEG_TOP | SEG_TR | SEG_BR,
    /* 8 */ SEG_TOP | SEG_TL | SEG_TR | SEG_MID | SEG_BL | SEG_BR | SEG_BOT,
    /* 9 */ SEG_TOP | SEG_TL | SEG_TR | SEG_MID | SEG_BR | SEG_BOT,
};

/* ─── Czcionka 5×7 (row-major, bit4 = lewy skrajny piksel) ─── */
/* Znaki: K, M, /, H – do etykiety „KM/H"                       */
#define FONT_W     5u
#define FONT_H     7u
#define FONT_SCALE 3u   /* każdy piksel → kwadrat 3×3 px */
#define FONT_GAP   4u   /* odstęp między znakami [px]    */

static const uint8_t font_kmslash_h[4][FONT_H] = {
    /* K */ {0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11},
    /* M */ {0x11, 0x1B, 0x15, 0x11, 0x11, 0x11, 0x11},
    /* / */ {0x01, 0x02, 0x04, 0x08, 0x10, 0x00, 0x00},
    /* H */ {0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11},
};

/* ─── Rysowanie jednego znaku z czcionki (skalowanej) ─── */
static void draw_char(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t color)
{
    for (uint8_t row = 0u; row < FONT_H; row++) {
        uint8_t data = bitmap[row];
        for (uint8_t col = 0u; col < FONT_W; col++) {
            if (data & (0x10u >> col)) {
                uint16_t px = x + (uint16_t)col * FONT_SCALE;
                uint16_t py = y + (uint16_t)row * FONT_SCALE;
                TFT_FillRect(TFT_RIGHT, px, py,
                             px + FONT_SCALE - 1u,
                             py + FONT_SCALE - 1u, color);
            }
        }
    }
}

/* ─── Rysowanie jednej cyfry 7-segmentowej w pozycji (dx, dy) ─── */
static void draw_digit(uint16_t dx, uint16_t dy, uint8_t d, uint16_t color)
{
    uint8_t  segs = digit_seg[d];
    uint16_t half = DIG_H / 2u;      /* 40 px */
    uint16_t mt   = DIG_T / 2u;      /* 4 px  */

    /* Każdy segment rysowany jako wypełniony prostokąt.
       Odstępy 1-px między segmentami sprawiają, że cyfry wyglądają czytelnie. */

    if (segs & SEG_TOP)
        TFT_FillRect(TFT_RIGHT,
                     dx + DIG_T,          dy,
                     dx + DIG_W - DIG_T - 1u, dy + DIG_T - 1u,
                     color);

    if (segs & SEG_TL)
        TFT_FillRect(TFT_RIGHT,
                     dx,                  dy + DIG_T + 1u,
                     dx + DIG_T - 1u,    dy + half - mt - 2u,
                     color);

    if (segs & SEG_TR)
        TFT_FillRect(TFT_RIGHT,
                     dx + DIG_W - DIG_T, dy + DIG_T + 1u,
                     dx + DIG_W - 1u,    dy + half - mt - 2u,
                     color);

    if (segs & SEG_MID)
        TFT_FillRect(TFT_RIGHT,
                     dx + DIG_T,          dy + half - mt,
                     dx + DIG_W - DIG_T - 1u, dy + half + mt - 1u,
                     color);

    if (segs & SEG_BL)
        TFT_FillRect(TFT_RIGHT,
                     dx,                  dy + half + mt + 1u,
                     dx + DIG_T - 1u,    dy + DIG_H - DIG_T - 2u,
                     color);

    if (segs & SEG_BR)
        TFT_FillRect(TFT_RIGHT,
                     dx + DIG_W - DIG_T, dy + half + mt + 1u,
                     dx + DIG_W - 1u,    dy + DIG_H - DIG_T - 2u,
                     color);

    if (segs & SEG_BOT)
        TFT_FillRect(TFT_RIGHT,
                     dx + DIG_T,          dy + DIG_H - DIG_T,
                     dx + DIG_W - DIG_T - 1u, dy + DIG_H - 1u,
                     color);
}

/* ─── Stan poprzedniej prędkości – rysuj tylko przy zmianie ─── */
static uint16_t spd_prev = 0xFFFFu;

/* ─── Rysowanie obszaru cyfr ─── */
static void draw_speed_digits(uint16_t speed)
{
    if (speed > 999u) speed = 999u;
    if (speed == spd_prev) return;
    spd_prev = speed;

    uint8_t d[3];
    d[0] = (uint8_t)(speed / 100u);
    d[1] = (uint8_t)((speed / 10u) % 10u);
    d[2] = (uint8_t)(speed % 10u);

    uint8_t  ndigits = (speed >= 100u) ? 3u : (speed >= 10u) ? 2u : 1u;
    uint16_t total_w = (uint16_t)ndigits * DIG_W + (uint16_t)(ndigits - 1u) * DIG_GAP;
    uint16_t x_start = (GC9A01_W - total_w) / 2u;

    /* Wyczyść cały pas cyfr */
    TFT_FillRect(TFT_RIGHT,
                 0u, DIG_Y,
                 GC9A01_W - 1u, DIG_Y + DIG_H - 1u,
                 SPD_BG_COLOR);

    /* Rysuj cyfry (bez wiodących zer) */
    uint8_t first = 3u - ndigits;
    for (uint8_t i = 0u; i < ndigits; i++) {
        uint16_t dx = x_start + (uint16_t)i * (DIG_W + DIG_GAP);
        draw_digit(dx, DIG_Y, d[first + i], SPD_DIGIT_COLOR);
    }
}

/* ─── Rysowanie etykiety „KM/H" (jednorazowo w Init) ─── */
static void draw_kmh_label(void)
{
    uint16_t label_w = 4u * FONT_W * FONT_SCALE + 3u * FONT_GAP;
    uint16_t x_start = (GC9A01_W - label_w) / 2u;

    for (uint8_t i = 0u; i < 4u; i++) {
        uint16_t x = x_start + (uint16_t)i * (FONT_W * FONT_SCALE + FONT_GAP);
        draw_char(x, LBL_Y, font_kmslash_h[i], SPD_LABEL_COLOR);
    }
}

/* ─── API ─── */

void Speedometer_Init(void)
{
    TFT_FillColor(TFT_RIGHT, SPD_BG_COLOR);
    draw_kmh_label();
    draw_speed_digits(0u);
}

void Speedometer_Update(uint16_t speed_kmh)
{
    draw_speed_digits(speed_kmh);
}
