#ifndef UART_CMD_H
#define UART_CMD_H

#include "stm32f4xx_hal.h"

/* Start receiving UART commands via interrupt mode */
void UartCmd_Init(UART_HandleTypeDef *huart);

/* Returns 1 if CROSS button is currently pressed, 0 otherwise */
uint8_t UartCmd_GetCross(void);

/* Returns left stick X axis value: -1.0 (left) … 0.0 (center) … 1.0 (right) */
float UartCmd_GetLSX(void);

#endif /* UART_CMD_H */
