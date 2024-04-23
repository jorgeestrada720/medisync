
#include "CONFIG.h"
#include "lcd.h"
#include "actuator.h"
#include "helper.h"
#include "servo.h"
#include "keypad.h"
#include <stdio.h>
#include <time.h>


long volume = 10;
long timeInfusion = 100;

//For timing function
#define Pulse LATB
long mil;

//PID LOOP

long c = 1000;
long dT = 100;


long Kp = 7500;   // Proportional gain (scaled by 100)
long Ki = 2000;    // Integral gain (scaled by 100)
long Kd = 2000;    // Derivative gain (scaled by 100)

// Variables for PID control (scaled to integers)
long errorSum = 0;
long lastError = 0;



// Function definition
void delay_ms(unsigned int milliseconds) {
    while(milliseconds > 0) {
        __delay_ms(1); // __delay_ms can only accept a constant expression in XC8
        milliseconds--;
       
    }
}

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

char* input_numbers(char separator, int number_of_digits){
    char userInput = 'n';
    char inputBuffer[5];
    inputBuffer[2]= separator;
    
    int counter = 0;

    Lcd_Set_Cursor(position_row_3, position_column_8);
    //LCD_Cmd(LCD_BLINK_CURSOR_ON);

    while(userInput == 'n' && counter <= number_of_digits){
        char outputBuffer[2];  
        userInput = switch_press_scan(); 
        
        if (userInput == 'b') {  // Implement your own condition based on actual input
            return 'b';  // Return 'b' to indicate back action
        }else{
            
            if(userInput >= 48 && userInput <= 57 ){
                __delay_ms(500);
                inputBuffer[counter]= userInput;
                outputBuffer[0] = inputBuffer[counter];
                outputBuffer[1] = '\0';
                Lcd_Write_String(outputBuffer);
                counter++;
                Lcd_Set_Cursor( position_row_3,position_column_8 + counter);
                userInput = 'n';
                if(inputBuffer[counter] == separator){
                    outputBuffer[0] = separator;
                    outputBuffer[1] = '\0';
                    Lcd_Write_String(outputBuffer);
                    counter++;
                    Lcd_Set_Cursor( position_row_3,position_column_8 + counter);
                    userInput = 'n';
                }         
            }else{
                userInput = 'n';
            }
        
        
        }
        
        
    }
    __delay_ms(2000);
    
    return inputBuffer;
}

void TMR0_Init(){ 
    T0CON  = 0x80; //configuro el registro con preescaler 1:2             
    INTCONbits.GIE    = 1; //Habilito las interrupciones gloabales
    //INTCONbits.PEIE   = 1; //Habilito  interrupciones internas o de perifericos (ADC,USB,USART,ETC)
    INTCONbits.TMR0IE = 1; //habilito la interrupcion por TMR0  
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
    usePeripheral(green_led, on);
    usePeripheral(orange_led, off);
    usePeripheral(red_led, off);
    
    
    //SERVO
    TRISCbits.RC1 = 0; //pin como salida servo1

};


void turn_valve(int direction){
    
    if(direction == forward){ 
       
        servo1_write(140);    __delay_ms(2000);
        //servo1_write(90);  __delay_ms(2000);
        servo1_write(45);__delay_ms(2000);      
    }else if(direction == backwards){
         
        servo1_write(45);    __delay_ms(2000);
       // servo1_write(90);  __delay_ms(2000);
        servo1_write(140);__delay_ms(2000);
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


long string_long_vol(const char* str) {
    long result = ((long)(str[1] - '0') * 10) + (long)(str[3] - '0');  
    if(((long)(str[1] - '0')) == 0){
        result = 100;
    }
    return result;
}

int string_long_time(const char* str) {
    int decMin = ((int)(str[0] - '0') * 1000 );
    int uniMin = ((int)(str[1] - '0') * 100 );
    int decSec = ((int)(str[3] - '0')* 10);
    int cenSec = (int)(str[4] - '0');
    
    
    int result = decMin + uniMin + decSec + cenSec;
    
    
    return result;
}


int handle_actuator_push(int act_pos){
    int decision;
    start_process_screen("PUSHING");
    decision = input_option();
    
    if(decision == 1){
        Lcd_Clear();
        Lcd_Set_Cursor( position_row_1,position_column_1);
        Lcd_Write_String("PUSHING...");
        
        while(act_pos > 20){
            act_pos  = ADC_Read(0);
            usePeripheral(orange_led, on);
            __delay_ms(25);
            move_actuator(backwards, 1023);
            __delay_ms(25);
           
        }
        move_actuator(forward, 0); 
        usePeripheral(orange_led, off);
    }
    
    return decision;

}

int handle_syringe_fill(int act_pos){
    int decision;
    start_process_screen("FILLING");
    decision = input_option();
    
    if(decision == 1){
        
        Lcd_Clear();
        Lcd_Set_Cursor( position_row_1,position_column_1);
        Lcd_Write_String("PULLING...");
        
        while(act_pos < 1015){
            act_pos  = ADC_Read(0);
            usePeripheral(orange_led, on);
            __delay_ms(25);
            move_actuator(forward, 1023);
            __delay_ms(25);
        }
        move_actuator(forward, 0); 
        usePeripheral(orange_led, off);
    }
    
    return decision;

}

int handle_valve_open(){
   
    int decision;
    input_valve_screen('o');
    decision = input_option();
    
    if(decision == 1){//YES
        __delay_ms(1000);
        Lcd_Clear();
        
        TMR0_Init();
        turn_valve(forward);
        INTCONbits.TMR0IE = 0;
    }    
    return decision;
    
}

int handle_valve_close(){
   
    int decision;
    input_valve_screen('c');
    decision = input_option();

    if(decision == 1){//YES
        __delay_ms(1000);
        Lcd_Clear();
        
        TMR0_Init();
        turn_valve(backwards);
        INTCONbits.TMR0IE = 0;
    }    
    return decision;
    
}

int confirm_start(){
    
    int decision;
    
    start_process_screen("INFUSION");
    decision = input_option();

    if(decision == 1){//YES
        __delay_ms(1000);
    }    
    return decision;


}
int processTimeToSeconds(int timeInput) {
    // Extract minutes and seconds from the integer input
    int minutes = timeInput / 100;  // Get minutes by dividing by 100
    int seconds = timeInput % 100;  // Get seconds by taking modulus 100

    // Calculate total seconds
    int totalSeconds = (minutes * 60) + seconds;
    // Return the total seconds calculated
    return totalSeconds;
}

long calculateBitRatePer10Sec(long volume_mL, long totalSeconds) {
    // Convert time in seconds to minutes for compatibility with original scaling
    //long time_min = totalSeconds / 60;  // Convert seconds to minutes correctly

    // Calculate length in mm for the given volume, scaled to maintain precision
    long length_mm_scaled = (volume_mL * 10000) / 158;  // Constant factor to convert volume to length

    // Calculate length per minute in mm, using actual minutes
    long length_per_min_mm_scaled = length_mm_scaled / totalSeconds;

    // Adjust the bit rate calculation to reduce scaling by a factor of 10
    long bit_rate_per_min_scaled = (length_per_min_mm_scaled * 100) / (635/60);  // Reduced scale factor from 1000 to 100

    // Convert bit rate per minute to bit rate per 10 seconds
    long bit_rate_per_10sec = bit_rate_per_min_scaled / 6;

    return bit_rate_per_10sec;
}

long getExpected(void){
    long ms = mil;
    long expected = 1023 - (((ms/1000UL) * (calculateBitRatePer10Sec(volume, processTimeToSeconds(timeInfusion)))/10)) ;
    return expected;
}




void infusion(){
    
    Timer1_start();
    Pulse = 0xff;  	/* Send high on PortB */
    int act_pos = ADC_Read(0);
    
    char buff[5];
    long expectedX, analogReading, error, dError, controlSignal;
    
    long stopTime = processTimeToSeconds(timeInfusion);
    Lcd_Clear();
    usePeripheral(orange_led, on);
    
    
    while( (mil) < stopTime*1000){
        
        expectedX = getExpected();
        analogReading = ADC_Read(0);
        error = (analogReading - expectedX);

        dError = (error - lastError)/(dT/1000);
        errorSum += (error * (dT / 1000));

        controlSignal = (Kp * error + Ki * errorSum + Kd * dError) / 100; // Scale back by 100

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
        Lcd_Write_String("Error:");
        sprintf(buff, "%ld", error);
        Lcd_Write_String(buff);
        
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("Pos:");
        sprintf(buff, "%ld", analogReading);
        Lcd_Write_String(buff);
        
        
        Lcd_Set_Cursor(3,1);
        Lcd_Write_String("Exp:");
        sprintf(buff, "%ld", expectedX);
        Lcd_Write_String(buff);


        Lcd_Set_Cursor(4,10);
        Lcd_Write_String("T: ");

        sprintf(buff, "%d", stopTime - (mil/1000));
        Lcd_Write_String(buff);  
        
       
        delay_ms(dT);
        Lcd_Clear();
        //mil+= dT;
        act_pos = ADC_Read(0);
    }
    
    usePeripheral(orange_led, off);
    move_actuator(forward, 0);
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("INFUSION COMPLETED");
    
    for(int8_t i = 0; i < 5; i++){
        usePeripheral(buzzer, on);
        usePeripheral(red_led, on);
        __delay_ms(1000);
        usePeripheral(buzzer, off);
        usePeripheral(red_led, off);
        __delay_ms(1000);
    }
    
    handle_actuator_push(act_pos);
    
    
   
}

void handle_infusion_setup() {
    InfusionSetupState currentState = START_INFUSION_SETUP;
    int decision;
    int actuator_position = ADC_Read(0);
    const char* v;
    const char* t;
    while (currentState != FINISH_INFUSION_SETUP) {
        switch (currentState) {
            case START_INFUSION_SETUP:
                infusion();
                input_value("VOLUME", "V", "10", "ml");
                
                currentState = INPUT_VOLUME_STATE;
                break; 
             
            case INPUT_VOLUME_STATE:
                v = input_numbers('.', 3);
                volume = string_long_vol(v);     
                
                if (v == 'b') {
                    currentState = FINISH_INFUSION_SETUP;  // Go back to volume input
                }else {
                    currentState = INPUT_TIME_STATE;
                }
                break;
            /*
            case INPUT_TIME_STATE:
                input_value("TIME", "T", "15", "min");
                
                t = input_numbers(':', 4);
                timeInfusion = string_long_time(t);
             
                if (t == 'b') {
                    currentState = START_INFUSION_SETUP;  // Go back to volume input
                    
                }else {
                    currentState = ACTUATOR_PUSH_STATE;
                }
                break;
                                
            case ACTUATOR_PUSH_STATE:
                actuator_position = ADC_Read(0);
                delay_ms(3000);
                
                if(actuator_position  > 0){                           
                    Lcd_Clear();
                    Lcd_Set_Cursor( position_row_1,position_column_1);
                    Lcd_Write_String("SYRINGE NOT PUSHED");
                    __delay_ms(2000);
                    decision = handle_actuator_push(actuator_position);
        
                    if(decision == 1){
                        currentState = VALVE_OPEN_STATE;
                    }
                    else{//NO
                        currentState = INPUT_TIME_STATE;
                    }
                    
                }else{   
                    Lcd_Clear();
                    Lcd_Set_Cursor( position_row_1,position_column_1);
                    Lcd_Write_String("SYRINGE IS PUSHED IN");
                    __delay_ms(3000);     
                    currentState = VALVE_OPEN_STATE;
                }
                
                break;  
              
            case VALVE_OPEN_STATE:
                decision = handle_valve_open();
                
                if(decision == 1){
                    currentState = SYRINGE_FILL_STATE;
                }
                else{//NO
                    currentState = INPUT_TIME_STATE;
                }
                break;
                
            case SYRINGE_FILL_STATE:
                
                actuator_position = ADC_Read(0);
                                                  
                decision = handle_syringe_fill(actuator_position);

                if(decision == 1){
                    currentState = VALVE_CLOSE_STATE;
                }
                else{//NO
                    currentState = VALVE_OPEN_STATE;
                }
                break;
                
            case VALVE_CLOSE_STATE:
                decision = handle_valve_close();
                
                if(decision == 1){
                    currentState = CONFIRM_START_INFUSION;
                }
                else{//NO
                    currentState = SYRINGE_FILL_STATE;
                }
                break;
            
             case CONFIRM_START_INFUSION:
                decision = confirm_start();  // This should implement the yes/no/back logic
                if(decision == 1){
                    currentState = INFUSION_STATE;
                    
                }
                else{//NO
                    currentState = VALVE_CLOSE_STATE;
                }
                break;
             case INFUSION_STATE:
                
                
                currentState = FINISH_INFUSION_SETUP;
                break;   */ 
        }
    }
}


ScreenState GetNextState(ScreenState currentState, char input) {
    switch (currentState) {
        case WELCOME_SCREEN:
            if (input == '1') return MAIN_MENU;
            break;
        case MANUAL_SCREEN:
            if (input == 'b') return MAIN_MENU;
            break;
        case MAIN_MENU:
            if (input == '1') return SET_UP_INFUSION;
            if (input == '2') return MANUAL_SCREEN;
            if (input == '3') return SETTINGS;
            if (input == '4') return ABOUT_PAGE;
            if (input == 'b') return WELCOME_SCREEN;
            break;
        case SET_UP_INFUSION:
            return MAIN_MENU;  
        
    }
    return currentState; 
}




void main(void) {
    system_init();
    ScreenState currentState = MAIN_MENU;
    char userInput = 'n';

    while (1) {  // or some condition to end the application
        switch (currentState) {   
            case MAIN_MENU:
                menu();
                break;
            case SET_UP_INFUSION:
                handle_infusion_setup();  // This function encapsulates all the infusion setup logic
                currentState = MAIN_MENU;  // Return to the main menu after setting up the infusion
                break;
            case MANUAL_SCREEN:
                manual();
                currentState = MANUAL_SCREEN;
                break;
            // Add other cases as needed
        }
        
        
        userInput = switch_press_scan(); // Scan for user input
        
       
        currentState = GetNextState(currentState, userInput); // Determine the next state based on the user input */
    } 
}