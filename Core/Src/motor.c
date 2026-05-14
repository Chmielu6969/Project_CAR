#include "motor.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim11;

/* Right-side motors face the opposite direction from left-side motors.
   Set to 1 to invert that bridge's direction signals.
   Adjust if a bridge still spins the wrong way after flashing. */
#define BRIDGE1_INVERT  0
#define BRIDGE2_INVERT  1

static GPIO_PinState invert_pin(GPIO_PinState s)
{
    return (s == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET;
}

static void set_bridge1(GPIO_PinState ain1, GPIO_PinState ain2,
                        GPIO_PinState bin1, GPIO_PinState bin2,
                        uint16_t speed)
{
#if BRIDGE1_INVERT
    ain1 = invert_pin(ain1); ain2 = invert_pin(ain2);
    bin1 = invert_pin(bin1); bin2 = invert_pin(bin2);
#endif
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
#if BRIDGE2_INVERT
    ain1 = invert_pin(ain1); ain2 = invert_pin(ain2);
    bin1 = invert_pin(bin1); bin2 = invert_pin(bin2);
#endif
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
    /* TIM1 is an advanced-control timer: MOE bit must be set explicitly
       so the PWM output appears on PA8 (M2_PWMA). HAL_TIM_PWM_Start
       should do this, but calling it again is harmless and ensures it. */
    __HAL_TIM_MOE_ENABLE(&htim1);

    /* Enable both bridges: STDBY HIGH = active */
    HAL_GPIO_WritePin(M1_STDBY_GPIO_Port, M1_STDBY_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M2_STDBY_GPIO_Port, M2_STDBY_Pin, GPIO_PIN_SET);

    /* Drive Bridge 2 STOP as pure COAST (all LOW), not BRAKE.
       With BRIDGE2_INVERT=1 the old Motor_SetAll(STOP) set all direction
       pins HIGH (BRAKE mode on TB6612). Explicit LOW here avoids that. */
    HAL_GPIO_WritePin(M2_AIN1_GPIO_Port, M2_AIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_AIN2_GPIO_Port, M2_AIN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_BIN1_GPIO_Port, M2_BIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_BIN2_GPIO_Port, M2_BIN2_Pin, GPIO_PIN_RESET);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);

    Motor_SetAll(MOTOR_STOP, 0);
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
