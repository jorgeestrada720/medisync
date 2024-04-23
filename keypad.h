#ifndef KEYPAD_H
#define KEYPAD_H
#include "CONFIG.h"

#define X_1    RB0
#define X_2    RB1
#define X_3    RB2
#define X_4    RB3
#define Y_1    RB4
#define Y_2    RC7
#define Y_3    RB6
#define Y_4    RB7
#define Keypad_PORT          PORTB
#define Keypad_PORT_Direction     TRISB   

void InitKeypad(void);
char switch_press_scan(void);

#endif