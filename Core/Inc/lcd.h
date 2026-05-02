#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "main.h"

/* Display geometry */
#define LCD_COLS  16
#define LCD_ROWS   2

/* HD44780 commands */
#define LCD_CMD_CLEAR           0x01
#define LCD_CMD_HOME            0x02
#define LCD_CMD_ENTRY_MODE      0x06  /* increment, no shift */
#define LCD_CMD_DISPLAY_ON      0x0C  /* display on, cursor off, blink off */
#define LCD_CMD_FUNCTION_SET    0x28  /* 4-bit, 2 lines, 5x8 font */
#define LCD_CMD_DDRAM_ADDR      0x80

/* Row DDRAM base addresses */
#define LCD_ROW0_ADDR  0x00
#define LCD_ROW1_ADDR  0x40

void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Print(const char *text);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);

#endif /* INC_LCD_H_ */
