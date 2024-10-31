#ifndef _DELAY_H
#define _DELAY_H

#include "stm32f4xx.h"

void Delay_Init(void);
void delay_us(int nus);
void delay_ms(int nms);//ºÁÃëÏŞÖÆÔÚ  0-798ms 
void delay_s(int ns);

#endif 
