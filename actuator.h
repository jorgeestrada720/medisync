#ifndef ACTUATOR_H
#define ACTUATOR_H
#include "CONFIG.h"

// Input to h driver
#define H_DRIVER_IN_1 RD1
#define H_DRIVER_IN_2 RD0

void init_actuator(void);

// set PWM1 duty cycle function
void set_pwm_duty(uint16_t pwm_duty);

void change_direction_motion(int hDriverIn1, int hDriverIn2);

void move_actuator(int direction , uint16_t speed);

#endif

