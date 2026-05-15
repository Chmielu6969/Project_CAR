#include "joystick.h"

void Joystick_Calibrate(void) {}

void Joystick_Read(JoystickData_t *data)
{
    data->x           = 0;
    data->y           = 0;
    data->btn_pressed = 0;
}
