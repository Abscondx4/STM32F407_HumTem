#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"
#include "stdbool.h"


extern u8 rbuffer[100];
extern int r_count ;
extern u8  r_flag;

void Usart1_Init(uint32_t baudRate);

#endif 
