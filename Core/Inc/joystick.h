#ifndef INC_JOYSTICK_H_
#define INC_JOYSTICK_H_

#include "main.h"

/* ADC 12-bit: center ~2048, deadzone avoids jitter around neutral */
#define JOY_DEADZONE_LOW   1500U
#define JOY_DEADZONE_HIGH  2500U

typedef struct {
    uint16_t x;          /* 0–4095, left=0, right=4095  */
    uint16_t y;          /* 0–4095, up=0,   down=4095   */
    uint8_t  btn_pressed; /* 1 when SW button held down  */
} JoystickData_t;

void Joystick_Read(JoystickData_t *data);

#endif /* INC_JOYSTICK_H_ */
