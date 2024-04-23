#ifndef LCD_H
#define LCD_H
#include "CONFIG.h"
// https://simple-circuit.com/pic-microcontroller-mplab-xc8-lcd/

void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Clear();
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init();
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Shift_Right();
void Lcd_Shift_Left();
void Lcd_Shift_Left();
#endif
