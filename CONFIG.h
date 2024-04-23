#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H
// PIC18F4321 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = INTIO2     // Oscillator (Internal oscillator block, CLKO function on RA6, port function on RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum Setting)

// CONFIG2H
#pragma config WDT = OFF       // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = RC1     // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = DIG     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config BBSIZ = BB256    // Boot Block Size Select bits ( 256 Word)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bitProtect Boot (Boot block not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>


//MY EDITION STARTS HERE

#define _XTAL_FREQ 8000000L


//used for servo
#define MINTHR              8000
#define RESOLUTION          488

enum output_peripherals{
    green_led= 0,
    orange_led = 1,
    red_led = 2,
    buzzer=3
};
enum direction{
    forward=1,
    backwards=0
};

enum states{
    on=1,
    off=0,
};

enum rows{
    position_row_1=1,
    position_row_2,
    position_row_3,
    position_row_4
};

enum columns {
    position_column_1 = 1,
    position_column_2,
    position_column_3,
    position_column_4,
    position_column_5,
    position_column_6,
    position_column_7,
    position_column_8,
    position_column_9,
    position_column_10,
    position_column_11,
    position_column_12,
    position_column_13,
    position_column_14,
    position_column_15,
    position_column_16,
    position_column_17,
    position_column_18,
    position_column_19,
    position_column_20
};

enum units{
    mm=1,
    cm3
};


typedef enum {
    WELCOME_SCREEN,
    MAIN_MENU,
    SET_UP_INFUSION,  // This represents the entire infusion setup process
    MANUAL_SCREEN,
    SETTINGS,
    ABOUT_PAGE,
    ERROR_SCREEN,
    INPUT_VOLUME,
    INPUT_TIME,
    CONFIRM_START,
    START_INFUSION,
    BACK_SCREEN // General navigation back
} ScreenState;


typedef enum {
    START_INFUSION_SETUP,
    INPUT_VOLUME_STATE,
    INPUT_TIME_STATE,
    CONFIRM_START_INFUSION,
    ACTUATOR_PUSH_STATE,
    VALVE_OPEN_STATE,
    SYRINGE_FILL_STATE,
    VALVE_CLOSE_STATE,
    CONFIRM_INFUSION_START,
    INFUSION_STATE,
    FINISH_INFUSION_SETUP
} InfusionSetupState;

#include <stdint.h>        // include stdint header
#include <stdio.h>
#include <string.h>
#include <math.h>



#endif	/* XC_HEADER_TEMPLATE_H�*/