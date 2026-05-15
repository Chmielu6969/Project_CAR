#include "lcd.h"

void LCD_Init(void)                         {}
void LCD_Clear(void)                        {}
void LCD_SetCursor(uint8_t row, uint8_t col) { (void)row; (void)col; }
void LCD_Print(const char *text)            { (void)text; }
void LCD_SendCommand(uint8_t cmd)           { (void)cmd; }
void LCD_SendData(uint8_t data)             { (void)data; }
