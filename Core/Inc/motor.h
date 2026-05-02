#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "main.h"

/* PWM period is 1000 ticks, so speed range is 0 (0%) to 1000 (100%) */
#define MOTOR_MAX_SPEED  1000U

typedef enum {
    MOTOR_STOP = 0,
    MOTOR_FORWARD,
    MOTOR_BACKWARD
} MotorDir_t;

void Motor_Init(void);
void Motor_SetAll(MotorDir_t dir, uint16_t speed);

#endif /* INC_MOTOR_H_ */
