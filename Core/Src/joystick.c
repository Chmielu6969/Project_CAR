#include "joystick.h"

extern ADC_HandleTypeDef hadc1;

static int16_t center_x = 2048;
static int16_t center_y = 2048;

static void read_raw(uint16_t *rx, uint16_t *ry)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    *rx = (uint16_t)HAL_ADC_GetValue(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    *ry = (uint16_t)HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
}

/* Average 16 readings at rest to find true center of this joystick unit */
void Joystick_Calibrate(void)
{
    uint32_t sx = 0, sy = 0;
    uint16_t rx, ry;

    for (uint8_t i = 0; i < 16; i++)
    {
        read_raw(&rx, &ry);
        sx += rx;
        sy += ry;
        HAL_Delay(5);
    }

    center_x = (int16_t)(sx / 16);
    center_y = (int16_t)(sy / 16);
}

/* Returns signed offset from calibrated center position */
void Joystick_Read(JoystickData_t *data)
{
    uint16_t rx, ry;
    read_raw(&rx, &ry);

    data->x = (int16_t)rx - center_x;
    data->y = (int16_t)ry - center_y;
    data->btn_pressed = (HAL_GPIO_ReadPin(JOY_SW_GPIO_Port, JOY_SW_Pin) == GPIO_PIN_RESET) ? 1U : 0U;
}
