#include "lcd.h"

/* Pulse the Enable pin to latch data into the LCD */
static void LCD_Pulse_E(void)
{
    HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
}

/* Send the upper 4 bits of 'byte' on D4-D7 */
static void LCD_SendNibble(uint8_t nibble)
{
    HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (nibble >> 0) & 0x01);
    HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (nibble >> 1) & 0x01);
    HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (nibble >> 2) & 0x01);
    HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (nibble >> 3) & 0x01);
    LCD_Pulse_E();
}

/* Send a full byte as two nibbles, high nibble first */
static void LCD_SendByte(uint8_t byte, uint8_t rs)
{
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, rs);
    LCD_SendNibble(byte >> 4);
    LCD_SendNibble(byte & 0x0F);
}

void LCD_SendCommand(uint8_t cmd)
{
    LCD_SendByte(cmd, GPIO_PIN_RESET);  /* RS = 0 */
    HAL_Delay(2);
}

void LCD_SendData(uint8_t data)
{
    LCD_SendByte(data, GPIO_PIN_SET);   /* RS = 1 */
    HAL_Delay(1);
}

/* HD44780 4-bit initialization sequence (per datasheet) */
void LCD_Init(void)
{
    HAL_Delay(50);  /* Wait for LCD power-on */

    /* Force 4-bit mode using special 3-step sequence */
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
    LCD_SendNibble(0x03);
    HAL_Delay(5);
    LCD_SendNibble(0x03);
    HAL_Delay(1);
    LCD_SendNibble(0x03);
    HAL_Delay(1);
    LCD_SendNibble(0x02);  /* Switch to 4-bit mode */
    HAL_Delay(1);

    /* Configure display */
    LCD_SendCommand(LCD_CMD_FUNCTION_SET);   /* 4-bit, 2 lines, 5x8 */
    LCD_SendCommand(LCD_CMD_DISPLAY_ON);     /* Display on, cursor off */
    LCD_SendCommand(LCD_CMD_CLEAR);          /* Clear display */
    HAL_Delay(2);
    LCD_SendCommand(LCD_CMD_ENTRY_MODE);     /* Cursor moves right */
}

void LCD_Clear(void)
{
    LCD_SendCommand(LCD_CMD_CLEAR);
    HAL_Delay(2);
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t addr = (row == 0 ? LCD_ROW0_ADDR : LCD_ROW1_ADDR) + col;
    LCD_SendCommand(LCD_CMD_DDRAM_ADDR | addr);
}

void LCD_Print(const char *text)
{
    while (*text)
    {
        LCD_SendData((uint8_t)*text++);
    }
}
