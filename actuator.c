#include "actuator.h"
 // https://simple-circuit.com/mplab-xc8-dc-motor-control-l293d/
void init_actuator(void){

    // PWM configuration
    RC2     = 0;     // RC2 output low
    TRISC2  = 0;     // configure RC2 pin as output (PWM signal output)
    CCP1CON = 0x0C;  // configure CCP1 module as PWM with single output & clear duty cycle 2 LSBs
    CCPR1L  = 0;     // clear duty cycle 8 MSBs

    // Timer2 module configuration for PWM frequency of 1.953kHz & 10-bit resolution
    TMR2IF = 0;      // clear Timer2 interrupt flag bit
    T2CON  = 0x05;   // enable Timer2 module with presacler = 4
    PR2    = 0xFF;   // Timer2 preload value = 255
};

// set PWM1 duty cycle function
void set_pwm_duty(uint16_t pwm_duty)
{
  CCP1CON = ((pwm_duty << 4) & 0x30) | 0x0C;
  CCPR1L  =   pwm_duty>>2;
}

void change_direction_motion(int hDriverIn1, int hDriverIn2){
        H_DRIVER_IN_1 = hDriverIn1;
        H_DRIVER_IN_2= hDriverIn2;
}

void move_actuator(int direction, uint16_t speed){ 
    
    if(speed > 0) {
        if(direction == forward){
            change_direction_motion(0,1);
        }else if(direction == backwards){
            change_direction_motion(1,0);
        }
    }
    set_pwm_duty(speed);
};
