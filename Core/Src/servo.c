#include "servo.h"

extern TIM_HandleTypeDef htim4;

void Servo_Init(void)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    Servo_SetPulse(SERVO_CENTER_US);
}

/* pulse_us: microseconds, valid range SERVO_LEFT_US to SERVO_RIGHT_US */
void Servo_SetPulse(uint16_t pulse_us)
{
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, pulse_us);
}
