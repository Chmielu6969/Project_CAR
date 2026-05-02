#include "joystick.h"

extern ADC_HandleTypeDef hadc1;

/* Reads both axes via ADC scan (rank1=VRX, rank2=VRY) and the SW button */
void Joystick_Read(JoystickData_t *data)
{
    HAL_ADC_Start(&hadc1);

    HAL_ADC_PollForConversion(&hadc1, 10);
    data->x = (uint16_t)HAL_ADC_GetValue(&hadc1);

    HAL_ADC_PollForConversion(&hadc1, 10);
    data->y = (uint16_t)HAL_ADC_GetValue(&hadc1);

    HAL_ADC_Stop(&hadc1);

    data->btn_pressed = (HAL_GPIO_ReadPin(JOY_SW_GPIO_Port, JOY_SW_Pin) == GPIO_PIN_RESET) ? 1U : 0U;
}
