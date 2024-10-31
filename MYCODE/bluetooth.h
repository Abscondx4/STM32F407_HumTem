#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "stm32f4xx.h"
#include "stdbool.h"


extern u8 rbuffer[100];
extern int r_count ;
extern u8  r_flag;

void BlueTooth_Init(int baudRate);


#endif 
