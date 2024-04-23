#ifndef SERVO_H
#define SERVO_H
#include "CONFIG.h"


//Debido a los retrasos introducidos en la llamada a la interrupci�n y a los c�lculos previos 
//he comprobado que es m�s exacto inicializar el contador un poco m�s adelante de su valor
#define tc 30 //tiempo de correcion a agregar
#define set_TMR0(x)      {TMR0H=(x>>8); TMR0L=(x&0x00FF);} //Ejem:0xFFCA en 0xFF y 0xCA
#define slot             (20000/N_SERVO)      //Espacios de tiempo asignada a cada servo
#define pulseHigh(pulse) (65536-pulse+tc)     //(65536-pulse)+tc)
#define pulseLow(pulse)  (65536-slot+pulse+tc)//(65536-(slot-pulse))+tc

#define N_SERVO 6
#define SERVO_1 LATCbits.LATC1  
#define SERVO_2 LATBbits.LATB1
#define SERVO_3 LATBbits.LATB2
#define SERVO_4 LATBbits.LATB3
#define SERVO_5 LATBbits.LATB4
#define SERVO_6 LATBbits.LATB5

//Anchos de pulsos minimo y maximo para cada servomotor a usar
const uint16_t pulseMin[N_SERVO] = {500 ,500 ,500 ,500 ,500 ,500 };
const uint16_t pulseMax[N_SERVO] = {2500,2500,2500,2500,2500,2500};
//Pulsos a generar(50-2500us) para obtener los grados deseados 
uint16_t pulse[N_SERVO]={50,50,50,50,50,50};//inician con un pulso de 50us(apagado) ya que con 0us no funciona bien    
uint16_t TMR0_ini;      //valor a cargar al TMR0 despues de que se produce la interrupcion
uint8_t servo_active=1; //inicio en 1 para el case:1 de la funcion switch de la interrupcion

uint16_t mapServo(uint32_t x, uint32_t eMin, uint32_t eMax, uint32_t sMin, uint32_t sMax); //funcion para convertir un rango de valores enteros(0,180) a otro rango de valores enteros (0us-2500us)
void signalServo(uint8_t pinServo, uint16_t pulseServo); //funcion que genera los pulsos con periodo de 20ms, en alto "pulseServo" y en bajo lo que le falta para llegar a 20ms
uint16_t ProbarServo(uint8_t pinDecrementar, uint8_t pinIncrementar); //funcion para probar los anchos de pulsos minimio y maximo de un servo, solo funciona para el SERVO_1,ya que probaremos un servo ala vez
//Funciones para controlar los grados de 0-180 de los servos
void servo1_write(uint8_t angle);
void servo2_write(uint8_t angle);
void servo3_write(uint8_t angle);
void servo4_write(uint8_t angle);
void servo5_write(uint8_t angle);
void servo6_write(uint8_t angle);


#endif