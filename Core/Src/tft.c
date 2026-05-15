#include "tft.h"

extern SPI_HandleTypeDef hspi2;

/* Jeden wiersz pikseli jako bufor do masowego wysyłania (240 × 2 B = 480 B) */
static uint8_t line_buf[GC9A01_W * 2u];

/* ─────────────────────────── CS / DC ─────────────────────────── */

static void cs_low(TFT_Display_t d)
{
    switch (d) {
    case TFT_LEFT:   HAL_GPIO_WritePin(TFT_LEFT_CS_GPIO_Port,   TFT_LEFT_CS_Pin,   GPIO_PIN_RESET); break;
    case TFT_RIGHT:  HAL_GPIO_WritePin(TFT_RIGHT_CS_GPIO_Port,  TFT_RIGHT_CS_Pin,  GPIO_PIN_RESET); break;
    case TFT_CENTER: HAL_GPIO_WritePin(TFT_CENTER_CS_GPIO_Port, TFT_CENTER_CS_Pin, GPIO_PIN_RESET); break;
    }
}

static void cs_high(TFT_Display_t d)
{
    switch (d) {
    case TFT_LEFT:   HAL_GPIO_WritePin(TFT_LEFT_CS_GPIO_Port,   TFT_LEFT_CS_Pin,   GPIO_PIN_SET); break;
    case TFT_RIGHT:  HAL_GPIO_WritePin(TFT_RIGHT_CS_GPIO_Port,  TFT_RIGHT_CS_Pin,  GPIO_PIN_SET); break;
    case TFT_CENTER: HAL_GPIO_WritePin(TFT_CENTER_CS_GPIO_Port, TFT_CENTER_CS_Pin, GPIO_PIN_SET); break;
    }
}

static void dc_cmd(TFT_Display_t d)
{
    switch (d) {
    case TFT_LEFT:   HAL_GPIO_WritePin(TFT_LEFT_DC_GPIO_Port,   TFT_LEFT_DC_Pin,   GPIO_PIN_RESET); break;
    case TFT_RIGHT:  HAL_GPIO_WritePin(TFT_RIGHT_DC_GPIO_Port,  TFT_RIGHT_DC_Pin,  GPIO_PIN_RESET); break;
    case TFT_CENTER: HAL_GPIO_WritePin(TFT_CENTER_DC_GPIO_Port, TFT_CENTER_DC_Pin, GPIO_PIN_RESET); break;
    }
}

static void dc_data(TFT_Display_t d)
{
    switch (d) {
    case TFT_LEFT:   HAL_GPIO_WritePin(TFT_LEFT_DC_GPIO_Port,   TFT_LEFT_DC_Pin,   GPIO_PIN_SET); break;
    case TFT_RIGHT:  HAL_GPIO_WritePin(TFT_RIGHT_DC_GPIO_Port,  TFT_RIGHT_DC_Pin,  GPIO_PIN_SET); break;
    case TFT_CENTER: HAL_GPIO_WritePin(TFT_CENTER_DC_GPIO_Port, TFT_CENTER_DC_Pin, GPIO_PIN_SET); break;
    }
}

/* ─────────────────────────── SPI ─────────────────────────── */

/* Wysyła komendę, a następnie opcjonalnie bajty danych (CS trzymane przez cały czas) */
static void write_cmd_data(TFT_Display_t d, uint8_t cmd, const uint8_t *data, uint16_t len)
{
    cs_low(d);
    dc_cmd(d);
    HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
    if (len > 0u) {
        dc_data(d);
        HAL_SPI_Transmit(&hspi2, (uint8_t *)data, len, HAL_MAX_DELAY);
    }
    cs_high(d);
}

static void write_cmd(TFT_Display_t d, uint8_t cmd)
{
    write_cmd_data(d, cmd, NULL, 0);
}

static void write_cmd1(TFT_Display_t d, uint8_t cmd, uint8_t b0)
{
    write_cmd_data(d, cmd, &b0, 1);
}

/* ─────────────────────────── Reset ─────────────────────────── */

static void hw_reset(void)
{
    HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(120);
}

/* ─────────────────────────── GC9A01 ─────────────────────────── */

static void gc9a01_init(TFT_Display_t d)
{
    /* Odblokowanie rejestrów wewnętrznych */
    write_cmd(d, 0xEF);
    write_cmd1(d, 0xEB, 0x14);
    write_cmd(d, 0xFE);
    write_cmd(d, 0xEF);
    write_cmd1(d, 0xEB, 0x14);

    write_cmd1(d, 0x84, 0x40);
    write_cmd1(d, 0x85, 0xFF);
    write_cmd1(d, 0x86, 0xFF);
    write_cmd1(d, 0x87, 0xFF);
    write_cmd1(d, 0x88, 0x0A);
    write_cmd1(d, 0x89, 0x21);
    write_cmd1(d, 0x8A, 0x00);
    write_cmd1(d, 0x8B, 0x80);
    write_cmd1(d, 0x8C, 0x01);
    write_cmd1(d, 0x8D, 0x01);
    write_cmd1(d, 0x8E, 0xFF);
    write_cmd1(d, 0x8F, 0xFF);

    { uint8_t b[] = {0x00, 0x00};                                           write_cmd_data(d, 0xB6, b, 2); }
    write_cmd1(d, 0x36, 0x18); /* MADCTL: ML=1, BGR=1, portrait */
    write_cmd1(d, 0x3A, 0x05); /* COLMOD: 16-bit RGB565 */
    { uint8_t b[] = {0x08, 0x08, 0x08, 0x08};                              write_cmd_data(d, 0x90, b, 4); }
    write_cmd1(d, 0xBD, 0x06);
    write_cmd1(d, 0xBC, 0x00);
    { uint8_t b[] = {0x60, 0x01, 0x04};                                    write_cmd_data(d, 0xFF, b, 3); }
    write_cmd1(d, 0xC3, 0x13);
    write_cmd1(d, 0xC4, 0x13);
    write_cmd1(d, 0xC9, 0x22);
    write_cmd1(d, 0xBE, 0x11);
    { uint8_t b[] = {0x10, 0x0E};                                          write_cmd_data(d, 0xE1, b, 2); }
    { uint8_t b[] = {0x21, 0x0C, 0x02};                                    write_cmd_data(d, 0xDF, b, 3); }
    { uint8_t b[] = {0x45, 0x09, 0x08, 0x08, 0x26, 0x2A};                 write_cmd_data(d, 0xF0, b, 6); }
    { uint8_t b[] = {0x43, 0x70, 0x72, 0x36, 0x37, 0x6F};                 write_cmd_data(d, 0xF1, b, 6); }
    { uint8_t b[] = {0x45, 0x09, 0x08, 0x08, 0x26, 0x2A};                 write_cmd_data(d, 0xF2, b, 6); }
    { uint8_t b[] = {0x43, 0x70, 0x72, 0x36, 0x37, 0x6F};                 write_cmd_data(d, 0xF3, b, 6); }
    { uint8_t b[] = {0x1B, 0x0B};                                          write_cmd_data(d, 0xED, b, 2); }
    write_cmd1(d, 0xAE, 0x77);
    write_cmd1(d, 0xCD, 0x63);
    { uint8_t b[] = {0x07, 0x07, 0x04, 0x0E, 0x0F, 0x09, 0x07, 0x08, 0x03}; write_cmd_data(d, 0x70, b, 9); }
    write_cmd1(d, 0xE8, 0x34);
    { uint8_t b[] = {0x18, 0x0D, 0x71, 0xED, 0x70, 0x70, 0x18, 0x0F, 0x71, 0xEF, 0x70, 0x70}; write_cmd_data(d, 0x62, b, 12); }
    { uint8_t b[] = {0x18, 0x11, 0x71, 0xF1, 0x70, 0x70, 0x18, 0x13, 0x71, 0xF3, 0x70, 0x70}; write_cmd_data(d, 0x63, b, 12); }
    { uint8_t b[] = {0x28, 0x29, 0xF1, 0x01, 0xF1, 0x00, 0x07};            write_cmd_data(d, 0x64, b, 7); }
    { uint8_t b[] = {0x3C, 0x00, 0xCD, 0x67, 0x45, 0x45, 0x10, 0x00, 0x00, 0x00}; write_cmd_data(d, 0x66, b, 10); }
    { uint8_t b[] = {0x00, 0x3C, 0x00, 0x00, 0x00, 0x01, 0x54, 0x10, 0x32, 0x98}; write_cmd_data(d, 0x67, b, 10); }
    { uint8_t b[] = {0x10, 0x85, 0x80, 0x00, 0x00, 0x4E, 0x00};            write_cmd_data(d, 0x74, b, 7); }
    { uint8_t b[] = {0x3E, 0x07};                                          write_cmd_data(d, 0x98, b, 2); }

    write_cmd(d, 0x35); /* Tearing effect line ON */
    write_cmd(d, 0x21); /* Display inversion ON – wymagane przez GC9A01 */

    write_cmd(d, 0x11); /* Sleep Out */
    HAL_Delay(120);
    write_cmd(d, 0x13); /* Normal Display Mode ON */
    write_cmd(d, 0x29); /* Display On */
    HAL_Delay(20);
}

/* ─────────────────────────── GMT020-02-7P (ST7789) ─────────────────────────── */
static void gmt020_init(void)
{
    TFT_Display_t d = TFT_CENTER;

    write_cmd(d, 0x01); /* Software Reset */
    HAL_Delay(150);

    write_cmd(d, 0x11); /* Sleep Out */
    HAL_Delay(120);

    write_cmd1(d, 0x36, 0x00); /* MADCTL: normal portrait, RGB */
    write_cmd1(d, 0x3A, 0x05); /* COLMOD: 16-bit RGB565 */
    write_cmd(d, 0x21);        /* Display Inversion ON – wymagane przez ST7789 */
    write_cmd(d, 0x13);        /* Normal Display Mode ON */

    write_cmd(d, 0x29); /* Display On */
    HAL_Delay(20);
}

/* ─────────────────────────── Okno adresowe i wypełnienie ─────────────────────────── */

/* CS musi być trzymane LOW przez wywołującego – nie zarządza tu CS */
static void set_window(TFT_Display_t d, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint8_t cmd;
    uint8_t buf[4];

    cmd = 0x2A;
    dc_cmd(d);  HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
    buf[0] = (uint8_t)(x0 >> 8); buf[1] = (uint8_t)x0;
    buf[2] = (uint8_t)(x1 >> 8); buf[3] = (uint8_t)x1;
    dc_data(d); HAL_SPI_Transmit(&hspi2, buf, 4, HAL_MAX_DELAY);

    cmd = 0x2B;
    dc_cmd(d);  HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
    buf[0] = (uint8_t)(y0 >> 8); buf[1] = (uint8_t)y0;
    buf[2] = (uint8_t)(y1 >> 8); buf[3] = (uint8_t)y1;
    dc_data(d); HAL_SPI_Transmit(&hspi2, buf, 4, HAL_MAX_DELAY);

    cmd = 0x2C; /* Memory Write */
    dc_cmd(d);  HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
}

void TFT_FillColor(TFT_Display_t disp, uint16_t color)
{
    uint16_t w = (disp == TFT_CENTER) ? GMT020_W : GC9A01_W;
    uint16_t h = (disp == TFT_CENTER) ? GMT020_H : GC9A01_H;

    /* Wypełnij bufor jednym wierszem */
    for (uint16_t i = 0u; i < w; i++) {
        line_buf[i * 2u]      = (uint8_t)(color >> 8);
        line_buf[i * 2u + 1u] = (uint8_t)(color);
    }

    /* CS trzymane przez cały czas: od ustawienia okna aż do ostatniego bajtu danych */
    cs_low(disp);
    set_window(disp, 0, 0, w - 1u, h - 1u);
    dc_data(disp);
    for (uint16_t row = 0u; row < h; row++) {
        HAL_SPI_Transmit(&hspi2, line_buf, w * 2u, HAL_MAX_DELAY);
    }
    cs_high(disp);
}

/* ─────────────────────────── Inicjalizacja wszystkich ─────────────────────────── */

void TFT_Init(void)
{
    /* Odznacz wszystkie wyświetlacze */
    cs_high(TFT_LEFT);
    cs_high(TFT_RIGHT);
    cs_high(TFT_CENTER);

    hw_reset();

    gc9a01_init(TFT_LEFT);
    gc9a01_init(TFT_RIGHT);
    gmt020_init();
}
