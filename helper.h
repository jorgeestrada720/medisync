#ifndef HELPER_H
#define HELPER_H
#include "CONFIG.h"

int16_t ADC_Read(int8_t channel);
void ADC_init(void);

void usePeripheral(int peripheral, int action);
void start_process_screen(char* word);
void input_volume_screen();
void input_time_screen();
void input_valve_screen(char action);
void menu();
void manual();
void input_value(char* inputType, char* initial, char* limit, char* units);



#endif