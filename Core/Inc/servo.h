#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#include "main.h"

/* NB-S007M digital servo – pulse widths in microseconds (1 tick = 1 us at 1 MHz timer clock) */
#define SERVO_LEFT_US    500U   /* 0 degrees   */
#define SERVO_CENTER_US  1500U  /* 90 degrees  */
#define SERVO_RIGHT_US   2500U  /* 180 degrees */

void Servo_Init(void);
void Servo_SetPulse(uint16_t pulse_us);

#endif /* INC_SERVO_H_ */
