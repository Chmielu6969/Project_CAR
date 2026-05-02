#ifndef INC_JOYSTICK_H_
#define INC_JOYSTICK_H_

#include "main.h"

/* Deadzone: joystick must move this many counts from calibrated center
   to register movement. Increase if motors twitch at rest. */
#define JOY_DEADZONE  500

typedef struct {
    int16_t x;           /* signed offset from calibrated center, left<0, right>0 */
    int16_t y;           /* signed offset from calibrated center, up<0,   down>0  */
    uint8_t btn_pressed; /* 1 when SW button held down                            */
} JoystickData_t;

/* Call once at startup with joystick at rest to measure center position */
void Joystick_Calibrate(void);

void Joystick_Read(JoystickData_t *data);

#endif /* INC_JOYSTICK_H_ */
