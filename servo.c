//https://www.electronicwings.com/pic/servo-motor-interfacing-with-pic18f4550

#include "servo.h"
//default frequency, might be needed to change when using the servo
uint16_t mapServo(uint32_t x, uint32_t eMin, uint32_t eMax, uint32_t sMin, uint32_t sMax){
   return(uint16_t)((x-eMin)*(sMax-sMin)/(eMax-eMin)+sMin);
}

void signalServo(uint8_t pinServo, uint16_t pulseServo){
    if (pinServo){ 
        TMR0_ini= (uint16_t)pulseHigh(pulseServo);
    }
    else {
        TMR0_ini= pulseLow(pulseServo); 
        
        if(servo_active == N_SERVO) servo_active=1;    
        else servo_active++;  
    }
}

uint16_t ProbarServo(uint8_t pinDecrementar, uint8_t pinIncrementar){ //con resistencia pull-down 
    
    if (pinDecrementar==1){
        __delay_ms(2);
        pulse[0]--;         
        if (pulse[0]<50)pulse[0]=50;//probar servo desde un pulso de 50us
    } 
    if (pinIncrementar==1){       
        __delay_ms(2);
        pulse[0]++;         
        if (pulse[0]>2500)pulse[0]=2500;//probar servo hasta un pulso de 2500us
    }  
    return pulse[0];
}

void servo1_write(uint8_t angle){pulse[0] = mapServo(angle,0,180,pulseMin[0], pulseMax[0]);}
void servo2_write(uint8_t angle){pulse[1] = mapServo(angle,0,180,pulseMin[1], pulseMax[1]);}
void servo3_write(uint8_t angle){pulse[2] = mapServo(angle,0,180,pulseMin[2], pulseMax[2]);}
void servo4_write(uint8_t angle){pulse[3] = mapServo(angle,0,180,pulseMin[3], pulseMax[3]);}
void servo5_write(uint8_t angle){pulse[4] = mapServo(angle,0,180,pulseMin[4], pulseMax[4]);}
void servo6_write(uint8_t angle){pulse[5] = mapServo(angle,0,180,pulseMin[5], pulseMax[5]);}
