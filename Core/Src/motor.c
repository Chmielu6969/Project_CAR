#include "motor.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim11;

static void set_bridge1(GPIO_PinState ain1, GPIO_PinState ain2,
                        GPIO_PinState bin1, GPIO_PinState bin2,
                        uint16_t speed)
{
    HAL_GPIO_WritePin(M1_AIN1_GPIO_Port, M1_AIN1_Pin, ain1);
    HAL_GPIO_WritePin(M1_AIN2_GPIO_Port, M1_AIN2_Pin, ain2);
    HAL_GPIO_WritePin(M1_BIN1_GPIO_Port, M1_BIN1_Pin, bin1);
    HAL_GPIO_WritePin(M1_BIN2_GPIO_Port, M1_BIN2_Pin, bin2);
    __HAL_TIM_SET_COMPARE(&htim3,  TIM_CHANNEL_1, speed);  /* M1_PWMA */
    __HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, speed);  /* M1_PWMB */
}

static void set_bridge2(GPIO_PinState ain1, GPIO_PinState ain2,
                        GPIO_PinState bin1, GPIO_PinState bin2,
                        uint16_t speed)
{
    HAL_GPIO_WritePin(M2_AIN1_GPIO_Port, M2_AIN1_Pin, ain1);
    HAL_GPIO_WritePin(M2_AIN2_GPIO_Port, M2_AIN2_Pin, ain2);
    HAL_GPIO_WritePin(M2_BIN1_GPIO_Port, M2_BIN1_Pin, bin1);
    HAL_GPIO_WritePin(M2_BIN2_GPIO_Port, M2_BIN2_Pin, bin2);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, speed);   /* M2_PWMA */
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed);   /* M2_PWMB */
}

void Motor_Init(void)
{
    HAL_TIM_PWM_Start(&htim3,  TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3,  TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim11, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1,  TIM_CHANNEL_1);

    /* Enable both bridges: STDBY HIGH = active */
    HAL_GPIO_WritePin(M1_STDBY_GPIO_Port, M1_STDBY_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M2_STDBY_GPIO_Port, M2_STDBY_Pin, GPIO_PIN_SET);

    Motor_SetAll(MOTOR_STOP, 0);
}

/* Diagnostic: run ONLY B channels at full speed for 'ms' milliseconds.
   Bypasses all joystick logic. Used to isolate B-channel hardware issues. */
void Motor_TestB(uint16_t ms)
{
    HAL_GPIO_WritePin(M1_BIN1_GPIO_Port, M1_BIN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M1_BIN2_GPIO_Port, M1_BIN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_BIN1_GPIO_Port, M2_BIN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M2_BIN2_GPIO_Port, M2_BIN2_Pin, GPIO_PIN_RESET);
    __HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, 700U);  /* M1_PWMB */
    __HAL_TIM_SET_COMPARE(&htim3,  TIM_CHANNEL_2, 700U);  /* M2_PWMB */
    HAL_Delay(ms);
    HAL_GPIO_WritePin(M1_BIN1_GPIO_Port, M1_BIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_BIN1_GPIO_Port, M2_BIN1_Pin, GPIO_PIN_RESET);
    __HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, 0U);
    __HAL_TIM_SET_COMPARE(&htim3,  TIM_CHANNEL_2, 0U);
}

void Motor_SetAll(MotorDir_t dir, uint16_t speed)
{
    GPIO_PinState a1, a2;

    switch (dir)
    {
        case MOTOR_FORWARD:
            a1 = GPIO_PIN_SET;   a2 = GPIO_PIN_RESET;
            break;
        case MOTOR_BACKWARD:
            a1 = GPIO_PIN_RESET; a2 = GPIO_PIN_SET;
            break;
        case MOTOR_STOP:
        default:
            a1 = GPIO_PIN_RESET; a2 = GPIO_PIN_RESET;
            speed = 0;
            break;
    }

    set_bridge1(a1, a2, a1, a2, speed);
    set_bridge2(a1, a2, a1, a2, speed);
}
