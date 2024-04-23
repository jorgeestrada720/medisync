
#include "helper.h"
#include "CONFIG.h"
#include "lcd.h"

int16_t ADC_Read(int8_t channel)
{
    int16_t digital;
    ADCON0 =(ADCON0 & 0b11000011)|((channel<<2) & 0b00111100);
    ADCON0 |= ((1<<ADON)|(1<<GO)); 
    while(ADCON0bits.GO_nDONE==1);
    digital = (ADRESH*256) | (ADRESL); 
    return(digital);
}

void ADC_init(void){
    //ANALOG READING PIN CONFIGURATION
    TRISA = 0xff; /*set as input port*/
    ADCON1 = 0x0e; /*ref vtg is VDD and Configure pin as analog pin*/
    ADCON2 = 0x92; /*Right Justified, 4Tad and Fosc/32. */
    ADRESH = 0; /*Flush ADC output Register*/
    ADRESL = 0;
}

void usePeripheral(int peripheral, int action){

    if(peripheral == green_led && action == on){
        LATA2 = on;
    }else if(peripheral == green_led && action == off){
        LATA2 = off;
    }else if(peripheral == orange_led && action == on){
        LATC6 = on;
    }else if(peripheral == orange_led && action == off){
        LATC6 = off;
    }
    else if(peripheral == red_led && action == on){
        LATC5 = on;
    }else if(peripheral == red_led && action == off){
        LATC5 = off;
    }else if(peripheral == buzzer && action == on){
        LATC4 = on;
    }else if(peripheral == buzzer && action == off){
        LATC4 = off;
    }

}

void input_value(char* inputType, char* initial, char* limit, char* units){
    Lcd_Clear();;
    Lcd_Set_Cursor(position_row_1,position_column_1 );
    Lcd_Write_String("INPUT ");
    Lcd_Write_String(inputType);
    Lcd_Set_Cursor(position_row_2,position_column_1 );
    Lcd_Write_String(initial);
    Lcd_Write_String(" BETWEEN 0-");
    Lcd_Write_String(limit);
    Lcd_Write_String(" ");
    Lcd_Write_String(units);
    Lcd_Set_Cursor(position_row_3, position_column_1);
    Lcd_Write_String(initial);
    Lcd_Write_String(": ");
    Lcd_Set_Cursor(position_row_4,position_column_17 );
    Lcd_Write_String("BACK");
}

void syringe_not_pushed_screen(){

   Lcd_Clear();;
    Lcd_Set_Cursor(position_row_1, position_column_1);
    Lcd_Write_String("SYRINGE IS NOT");
    Lcd_Set_Cursor(position_row_2, position_column_1);
    Lcd_Write_String("PUSHED IN");
    __delay_ms(2000);
   Lcd_Clear();;
    Lcd_Set_Cursor(position_row_1,position_column_1 );
    Lcd_Write_String("PUSHING");
}

void input_valve_screen(char action){
    //INTERFACE
   Lcd_Clear();;
    Lcd_Set_Cursor( position_row_1,position_column_1);
    Lcd_Write_String("DO YOU WANT TO");
    Lcd_Set_Cursor(position_row_2, position_column_1);
    if(action == 'o'){
        Lcd_Write_String("OPEN VALVE");
    }else if(action == 'c'){
        Lcd_Write_String("CLOSE VALVE");
    }
    
    Lcd_Set_Cursor( position_row_3,position_column_1);
    Lcd_Write_String("1. YES");
    Lcd_Set_Cursor(position_row_4, position_column_1);
    Lcd_Write_String("2. NO");
    Lcd_Set_Cursor( position_row_4,position_column_17);
    Lcd_Write_String("BACK");

}


void start_process_screen(char* word){
    
    Lcd_Clear();
    Lcd_Set_Cursor(position_row_1, position_column_1);
    Lcd_Write_String("DO YOU WANT TO");
    Lcd_Set_Cursor( position_row_2,position_column_1);
    Lcd_Write_String("START ");
    Lcd_Write_String(word);
    Lcd_Set_Cursor( position_row_3,position_column_1);
    Lcd_Write_String("1. YES");
    Lcd_Set_Cursor(position_row_4, position_column_1);
    Lcd_Write_String("2. NO");
    Lcd_Set_Cursor(position_row_4,position_column_17 );
    Lcd_Write_String("BACK");



}

void menu(){
    Lcd_Clear(); 
    Lcd_Set_Cursor(position_row_1,position_column_1 );
    Lcd_Write_String("MediSync Menu");
    Lcd_Set_Cursor(position_row_2,position_column_1);
    Lcd_Write_String("1.Start New Infusion");
    Lcd_Set_Cursor(position_row_3,position_column_1);
    Lcd_Write_String("2.Manual");
}

void manual(void){
    Lcd_Clear();
    Lcd_Set_Cursor( position_row_1,position_column_1);
    Lcd_Write_String("For more information");
    Lcd_Set_Cursor( position_row_2,position_column_2);
    Lcd_Write_String("Visit: hardng.ed");
    Lcd_Set_Cursor( position_row_4,position_column_1 );
    Lcd_Write_String("BACK");

}




