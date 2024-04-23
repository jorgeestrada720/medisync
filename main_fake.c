#include "CONFIG.h"
#include "lcd.h"
#include "actuator.h"
#include "helper.h"
#include "servo.h"
#include "keypad.h"
#include <stdio.h>
#include <time.h>


uint16_t actuator_speed = 0;
#define  ADC_CHANNEL 0

long infusion_volume;
long infusion_time;

void MSDelay(uint8_t n)
{
    for (; n > 0; n--)
    {
        __delay_ms(10);
    }
}


void TMR0_Init(){ 
    T0CON  = 0x80; //configuro el registro con preescaler 1:2             
    INTCONbits.GIE    = 1; //Habilito las interrupciones gloabales
    //INTCONbits.PEIE   = 1; //Habilito  interrupciones internas o de perifericos (ADC,USB,USART,ETC)
    INTCONbits.TMR0IE = 1; //habilito la interrupcion por TMR0  
}
 


void system_init(){ 
    // set internal oscillator to 8MHz and using internal oscillator
    OSCCON = 0x72;
    // PORTD initial state
    PORTD  = 0x00;   
    // configure all PORTD pins as outputs
    TRISD  = 0x00; 
    // configure all PORTC pins as outputs
    TRISC  = 0x00; 
    //initialize LCD16x2 in 4-bit mode 
    Lcd_Init();
    //initialize 4x4keypad
    InitKeypad();   
    //set TRISA2 as output
    
    ADC_init();
    
    
    init_actuator();
    
    TRISA2 = 0;  
    TRISC6 = 0;
    TRISC5 = 0;
    TRISC4 = 0;
    //Turn on Power LED
    
    
    //SERVO
    TRISCbits.RC1 = 0; //pin como salida servo1

};




void welcome_screen(void){
    Lcd_Clear();
    Lcd_Set_Cursor(position_column_1, position_row_1);
    Lcd_Write_String("Welcome to MediSync");
    Lcd_Set_Cursor(position_column_2, position_row_2);
    Lcd_Write_String("Precision Syringe");
    __delay_ms(2000);
    Lcd_Clear();
}

void manual(void){
    char userInput = 'n';

    Lcd_Clear();
    Lcd_Set_Cursor( position_row_1,position_column_1);
    Lcd_Write_String("For more information");
    Lcd_Set_Cursor( position_row_2,position_column_2);
    Lcd_Write_String("Visit:");
    Lcd_Set_Cursor( position_row_3,position_column_2);
    Lcd_Write_String("httpshola");
    Lcd_Set_Cursor( position_row_4,position_column_17 );
    Lcd_Write_String("BACK");

    
    while(userInput == 'n'){  
        userInput = switch_press_scan();
        if(userInput == 'b'){
            __delay_ms(1000);
            Lcd_Clear();
            
            
        }else{
            userInput = 'n';
       
        }
    }
}

int input_option(){
    char userInput = 'n';
    int option;

    while(userInput == 'n'){  
        userInput = switch_press_scan(); 
        if(userInput == '1'){
            __delay_ms(1000);
            option = 1;
        }else if(userInput == '2'){
            __delay_ms(1000);
            option = 2;
        }else if(userInput == 'b'){
            __delay_ms(1000);
            option = 'b';
        }else{
            userInput = 'n';
        }
    }
    return option;
}

char* input_numbers(char separator, int number_of_digits){
    char userInput = 'n';
    char inputBuffer[10];
    inputBuffer[2]= separator;
    
    int counter = 0;

    Lcd_Set_Cursor(position_row_3, position_column_6);
    //LCD_Cmd(LCD_BLINK_CURSOR_ON);

    while(userInput == 'n' && counter <= number_of_digits){
        char outputBuffer[2];  
        userInput = switch_press_scan(); 
        if(userInput >= 48 && userInput <= 57 ){
                __delay_ms(500);
                inputBuffer[counter]= userInput;
                outputBuffer[0] = inputBuffer[counter];
                outputBuffer[1] = '\0';
                Lcd_Write_String(outputBuffer);
                counter++;
                Lcd_Set_Cursor( position_row_3,position_column_6 + counter);
                userInput = 'n';
            if(inputBuffer[counter] == separator){
                outputBuffer[0] = separator;
                outputBuffer[1] = '\0';
                Lcd_Write_String(outputBuffer);
                counter++;
                Lcd_Set_Cursor( position_row_3,position_column_6 + counter);
                userInput = 'n';
            }         
        }else{
            userInput = 'n';
        }
    }
    __delay_ms(2000);
    //LCD_Cmd(LCD_CURSOR_OFF);
    //eliminate null
    return inputBuffer;
}

void infusion(int volume, int minutes, int seconds){
    char data[10];
    int status = 1;
     //INTERFACE VOLUME
    Lcd_Clear();
    Lcd_Set_Cursor(position_column_17, position_row_4);
    Lcd_Write_String("BACK");

    Lcd_Set_Cursor(position_column_1, position_row_1);
    Lcd_Write_String("T LEFT :");
    Lcd_Write_String("4.45");
    
    Lcd_Set_Cursor(position_column_1, position_row_2);
    Lcd_Write_String("T PASS :");
    Lcd_Write_String("9.93");

    Lcd_Set_Cursor(position_column_1, position_row_3);
    Lcd_Write_String("V DELIV:");
    Lcd_Write_String("4.45");

    Lcd_Set_Cursor(position_column_1, position_row_4);
    Lcd_Write_String("V LEFT :");
    Lcd_Write_String("4.45");
    

    ADC_init();
    int actuator_position  = ADC_Read(0);
    __delay_ms(25);
    usePeripheral(orange_led, on);
    while(actuator_position > 20){
        actuator_position  = ADC_Read(0);
        __delay_ms(25);
            move_actuator(backwards, 1023);
        __delay_ms(25);
    }
    //STOP ACTUATOR WHEN IT REACHES 1023
    move_actuator(forward, 0);
    Lcd_Clear();
    Lcd_Set_Cursor(position_column_1, position_row_1);
    usePeripheral(orange_led, off);
    Lcd_Write_String("INFUSION COMPLETED");
    for(int i = 0; i < 10; i++){
        usePeripheral(red_led, on);
        usePeripheral(buzzer, on);
        __delay_ms(500);
        usePeripheral(red_led, off);
        usePeripheral(buzzer, off);
        __delay_ms(500);
    } 

    __delay_ms(3000);  
    

};



void turn_valve(int direction){
    
    if(direction == forward){ 
       
        servo1_write(0);    __delay_ms(3000);
        //servo1_write(90);  __delay_ms(1000);
        servo1_write(180);__delay_ms(3000);      
    }else if(direction == backwards){
         
        servo1_write(180);    __delay_ms(3000);
        //servo1_write(90);  __delay_ms(1000);
        servo1_write(0);__delay_ms(3000);
    }
}



void set_up_infusion(void){
    
    
    input_volume_screen();
    //volume
    const char* volume;
    volume = input_numbers('.', 3);

    //TIME
    input_time_screen();
    const char* time;
    time = input_numbers(':', 4);

    Lcd_Clear();
    
  
    int actuator_position  = ADC_Read(0);
    Lcd_Clear();;
    Lcd_Set_Cursor( position_row_1,position_column_1);
    Lcd_Write_String("DO YOU WANT TO");
    Lcd_Set_Cursor( position_row_2,position_column_1);
    Lcd_Write_String("START PUSHING");
    Lcd_Set_Cursor( position_row_3,position_column_1);
    Lcd_Write_String("1. YES");
    Lcd_Set_Cursor( position_row_4,position_column_1);
    Lcd_Write_String("2. NO");
    Lcd_Set_Cursor( position_row_4,position_column_17);
    Lcd_Write_String("BACK");
    int decision = input_option();
    if(decision == 1){//YES
        Lcd_Clear();
        actuator_position  = ADC_Read(0);
        __delay_ms(25);
        Lcd_Set_Cursor(position_column_1, position_row_1);
        Lcd_Write_String("PUSHING");

        while(actuator_position > 10){
            actuator_position  = ADC_Read(0);
            __delay_ms(25);
            move_actuator(backwards, 1023);
            __delay_ms(25);
        }

        //STOP ACTUATOR
        move_actuator(forward, 0);
        Lcd_Clear();
        Lcd_Set_Cursor(position_row_1,position_column_1 );
        Lcd_Write_String("PUSHED IN");
        __delay_ms(3000);
        
    }else if(decision == 2){//NO
        __delay_ms(1000);
        Lcd_Clear();
        //LOGIC TO BE IMPLEMENTED
    }else if(decision == 'b'){
        __delay_ms(1000);
        Lcd_Clear();
        //STILL NEED TO IMPLEMENT LOGIC FOR THIS
    }


    //OPEN VALVE PROCESS
    input_valve_screen('o');
    decision =input_option();

    if(decision == 1){//YES
        __delay_ms(1000);
        Lcd_Clear();
        TMR0_Init();
        turn_valve(forward);
        INTCONbits.TMR0IE = 0;
    }else if(decision == 2){//NO
        __delay_ms(1000);
        Lcd_Clear();
    }else if(decision == 'b'){
        __delay_ms(1000);
        Lcd_Clear();
        //STILL NEED TO IMPLEMENT LOGIC FOR THIS
    }
    
    //FILLING
    syringe_filling_screen();
    decision = input_option();
    if(decision == 1){//YES
        Lcd_Clear();
        actuator_position  = ADC_Read(0);
        __delay_ms(25);
        Lcd_Set_Cursor(position_column_1, position_row_1);
        Lcd_Write_String("PULLING");

        while(actuator_position < 1000){
            actuator_position  = ADC_Read(0);
            __delay_ms(25);
            move_actuator(forward, 1023);
            __delay_ms(25);
        }

        //STOP ACTUATOR
        move_actuator(forward, 0);
        Lcd_Clear();
        Lcd_Set_Cursor(position_row_1,position_column_1 );
        Lcd_Write_String("PULLED OUT");
        __delay_ms(3000);
        
    }else if(decision == 2){//NO
        __delay_ms(1000);
        Lcd_Clear();
        //LOGIC TO BE IMPLEMENTED
    }else if(decision == 'b'){
        __delay_ms(1000);
        Lcd_Clear();
        //STILL NEED TO IMPLEMENT LOGIC FOR THIS
    }

    input_valve_screen('c');

    decision = input_option();
    if(decision == 1){//YES
        __delay_ms(1000);
        Lcd_Clear();
        TMR0_Init();
        turn_valve(backwards);
        INTCONbits.TMR0IE = 0;
    }else if(decision == 2){//NO
        __delay_ms(1000);
        Lcd_Clear();
    }else if(decision == 'b'){
        __delay_ms(1000);
        Lcd_Clear();
        //STILL NEED TO IMPLEMENT LOGIC FOR THIS
    }
    //MIGHT BE BETTER IN INPUT UNITS FUNCTIONS
    start_infusion_screen();
    decision = input_option();
    if(decision == 1){//YES
        __delay_ms(1000);
        Lcd_Clear();

        infusion(1,1,1);

    }else if(decision == 2){//NO
        __delay_ms(1000);
        Lcd_Clear();
    }else if(decision == 'b'){
        __delay_ms(1000);
        Lcd_Clear();
        //STILL NEED TO IMPLEMENT LOGIC FOR THIS
    }
    //MIGHT BE BETTER IN INPUT UNITS FUNCTIONS
}

void menu_screen(void){
    char userInput = 'n';
    __delay_ms(1000);
    Lcd_Clear(); 
    Lcd_Set_Cursor(position_row_1,position_column_1 );
    Lcd_Write_String("Selection Menu");
    Lcd_Set_Cursor(position_row_2,position_column_1);
    Lcd_Write_String("1.Start New Infusion");
    Lcd_Set_Cursor(position_row_3,position_column_1);
    Lcd_Write_String("2.Manual");
    
    user_menu:
    
    while(userInput == 'n'){  
        userInput = switch_press_scan(); 
    }

    if(userInput == '1'){
       
        set_up_infusion(); 
    }
    if(userInput == '2'){
        manual(); 
        
    }
    
    
}

#define MM_TO_ML_CONVERSION_FACTOR 0.158
#define POTENTIOMETER_RANGE_MM 65
#define ADC_RESOLUTION 1023

long calculateBitRatePer10Sec(long volume_mL, long time_min) {
    // Calculate length in mm for the given volume, scaled to maintain precision
    long length_mm_scaled = (volume_mL * 10000) / 158;  // Using 10000 as scale factor

    // Calculate length per minute in mm, still using scaled values
    long length_per_min_mm_scaled = length_mm_scaled / (time_min/100);

    // Calculate bit rate per minute, using scaled values
    // Here we adjust the scaling by dividing by an additional 10 to correct the final result
    long bit_rate_per_min_scaled = (length_per_min_mm_scaled * 1000) / 635;  // Adjusted scale here

    // Convert bit rate per minute to bit rate per 10 seconds, correctly scaled
    long bit_rate_per_10sec = bit_rate_per_min_scaled / 6;

    return bit_rate_per_10sec;  // Return the bit rate per 10 seconds
}



void bitrateF() {
    system_init();
    char buf[40];
    welcome_screen();

    while(1) {
        long bitRate = calculateBitRatePer10Sec(10, 10);
        sprintf(buf, "%ld", bitRate);
        Lcd_Write_String(buf);
        __delay_ms(1000);
        Lcd_Clear();
    }
}



long mil;
long c = 1000;
long dT = 100;
long expectedX;

long Kp = 6000;   // Proportional gain (scaled by 100)
long Ki = 1000;    // Integral gain (scaled by 100)
long Kd = 1000;    // Derivative gain (scaled by 100)

// Variables for PID control (scaled to integers)
long errorSum = 0;
long lastError = 0;

long getExpected(void){
    long ms = mil;
    long expected = 1023 - (((ms/1000UL) * 17)/10) ;
    return expected;
}


// Function definition
void delay_ms(unsigned int milliseconds) {
    while(milliseconds > 0) {
        __delay_ms(1); // __delay_ms can only accept a constant expression in XC8
        milliseconds--;
       
    }
}

void pullBack(){
    
    char buff[40];
    long expectedX = getExpected();
    long analogReading = ADC_Read(0);


    long error = (analogReading - expectedX);
    
    long dError = (error - lastError)/(dT/1000);
    errorSum += (error * (dT / 1000));

    long controlSignal = (Kp * error + Ki * errorSum + Kd * dError) / 100; // Scale back by 100

// Ensure control signal is always positive
    if (controlSignal < 0) {
        controlSignal = 0; // Set control signal to 0 if negative
    }
    if (controlSignal > 510) {
        controlSignal = 510;// Set control signal to 0 if negative
    }

    move_actuator(backwards, controlSignal);
    lastError = error;

    Lcd_Set_Cursor(1,1);
    sprintf(buff, "%ld", error);
    Lcd_Write_String(buff);

    Lcd_Set_Cursor(2,1);
    sprintf(buff, "%ld", controlSignal);
    Lcd_Write_String(buff);
    
    
    Lcd_Set_Cursor(4,1);
    Lcd_Write_String("An: ");
    sprintf(buff, "%ld", analogReading);
    Lcd_Write_String(buff);
    
    Lcd_Set_Cursor(4,10);
    Lcd_Write_String("T: ");
    sprintf(buff, "%ld", mil/1000);
    Lcd_Write_String(buff);

    
}

void Timer1_start();

#define Pulse LATB

void __interrupt() Timer1_ISR()
{
    if (TMR1IF == 1) {
        TMR1=0xF856;
        Pulse = ~Pulse;  	/* Toggle PortB at of 500 Hz */   
        PIR1bits.TMR1IF=0;  /* Make Timer1 Overflow Flag to '0' */
        mil++;
    }
    else if(TMR0IF==1){           //si se desborda el TMR0 y genera la interrupcion      
        INTCONbits.TMR0IF=0; //clareo la bandera del TMR0
        //aqui entra el codigo
        switch (servo_active){
          case 1: SERVO_1=!SERVO_1;signalServo(SERVO_1,pulse[0]);break;
          case 2: SERVO_2=!SERVO_2;signalServo(SERVO_2,pulse[1]);break;
          case 3: SERVO_3=!SERVO_3;signalServo(SERVO_3,pulse[2]);break;
          case 4: SERVO_4=!SERVO_4;signalServo(SERVO_4,pulse[3]);break; 
          case 5: SERVO_5=!SERVO_5;signalServo(SERVO_5,pulse[4]);break; 
          case 6: SERVO_6=!SERVO_6;signalServo(SERVO_6,pulse[5]);break;             
         }
        set_TMR0(TMR0_ini);                
    }
    
}

void Timer1_start()
{
    GIE=1;		/* Enable Global Interrupt */
    PEIE=1;  		/* Enable Peripheral Interrupt */
    TMR1IE=1;		/* Enable Timer1 Overflow Interrupt */
    TMR1IF=0;
    
    /* Enable 16-bit TMR1 register,no pre-scale,internal clock, timer OFF */
    T1CON=0x80;		
    TMR1=0xF856;	/* Load Count for generating delay of 1ms */
    TMR1ON=1;		/* Turn ON Timer1 */
}


int speed(){

    system_init();
    Timer1_start();
    
    ADC_init();
    init_actuator();
    
    OSCCON=0x72;	/* Configure oscillator frequency to 8MHz */
    TRISB = 0;  	/* Set as output Port */
    Pulse = 0xff;  	/* Send high on PortB */
    
    PORTD  = 0x00;   
    // configure all PORTD pins as outputs
    TRISD  = 0x00; 
    
                                                                                                                                                                                                                                                                                                                                                                                                                                         
    char buff[10];
    
    while(1){
       
        pullBack();   
        Lcd_Set_Cursor(3,1);
        Lcd_Write_String("Exp:");
        sprintf(buff, "%ld", getExpected());
        Lcd_Set_Cursor(3,6);
        Lcd_Write_String(buff);
       
        delay_ms(dT);
        Lcd_Clear();
        //mil+= dT;
    }
     
}


void processTimeInput(const char* buffer) {
    char buff[10];
    char minutesStr[3] = {buffer[0], buffer[1], '\0'}; // Extract minutes part
    char secondsStr[3] = {buffer[2], buffer[3], '\0'}; // Extract seconds part

    int minutes = atoi(minutesStr); // Convert minutes to integer
    int seconds = atoi(secondsStr); // Convert seconds to integer

    // Convert seconds to a fraction of a minute, scaled by 100 (to avoid floating points)
    // Since 60 seconds make a full minute, we can scale the seconds by 100/60
    // To avoid floating-point operations, we can scale the division by 60 up by 100
    int scaledSeconds = (seconds * 100) / 60;

    // Now, minutes are also scaled by 100 to be consistent with scaledSeconds
    int totalScaledMinutes = (minutes * 100) + scaledSeconds;
    Lcd_Set_Cursor(2,1);

        sprintf(buff, "%d",totalScaledMinutes );
        Lcd_Write_String(buff);
        __delay_ms(5000);
}
int main_fake(){
    system_init();
    const char* buff;
    while(1){
        //welcome_screen();
        //menu_screen();
        buff = input_numbers('.', 4);
        processTimeInput(buff);
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String(buff);
        __delay_ms(5000);
        Lcd_Clear();
        
         
    }
      
}




