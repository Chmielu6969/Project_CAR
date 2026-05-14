#include "uart_cmd.h"
#include <string.h>
#include <stdlib.h>

#define LINE_BUF_SIZE 64U

static UART_HandleTypeDef *uart_handle;
static uint8_t rx_byte;
static char    line_buf[LINE_BUF_SIZE];
static uint8_t line_pos = 0;

static volatile uint8_t  cross_state    = 0;
static volatile float    lsx_value      = 0.0f;
static volatile uint32_t lsx_last_tick  = 0;    /* timestamp of last LSX command */

#define LSX_TIMEOUT_MS  300U  /* return 0.0 if no LSX received within this time */

static void parse_line(void)
{
    char *sep = strchr(line_buf, ':');
    if (sep == NULL) return;

    *sep = '\0';
    const char *cmd = line_buf;
    const char *val = sep + 1;

    if (strcmp(cmd, "CROSS") == 0)
    {
        cross_state = (val[0] == '1') ? 1U : 0U;
    }
    else if (strcmp(cmd, "LSX") == 0)
    {
        lsx_value     = strtof(val, NULL);
        lsx_last_tick = HAL_GetTick();
    }
}

/* Called from HAL UART interrupt when one byte is received */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance != USART1) return;

    char c = (char)rx_byte;

    if (c == '\n' || c == '\r')
    {
        if (line_pos > 0)
        {
            line_buf[line_pos] = '\0';
            parse_line();
            line_pos = 0;
        }
    }
    else if (line_pos < (LINE_BUF_SIZE - 1U))
    {
        line_buf[line_pos++] = c;
    }
    else
    {
        line_pos = 0;
    }

    /* Re-arm interrupt for next byte */
    HAL_UART_Receive_IT(uart_handle, &rx_byte, 1);
}

void UartCmd_Init(UART_HandleTypeDef *huart)
{
    uart_handle = huart;
    HAL_UART_Receive_IT(uart_handle, &rx_byte, 1);
}

uint8_t UartCmd_GetCross(void)
{
    return cross_state;
}

float UartCmd_GetLSX(void)
{
    if ((HAL_GetTick() - lsx_last_tick) > LSX_TIMEOUT_MS)
        return 0.0f;
    return lsx_value;
}
