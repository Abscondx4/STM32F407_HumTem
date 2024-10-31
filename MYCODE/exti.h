#ifndef __EXTI_H
#define __EXTI_H

#include "stdbool.h"
#include "stm32f4xx.h"
#include "rtc.h"



// 控制速度
extern int speed;
// 控制方向
extern bool direct;

extern int speed_flag;
extern int menu_flag;
extern int ret;
extern bool tem_flag;
extern bool setting_flag;
extern int History_Flag;
extern int History_Page;

void Exti_PA0_Init(void);
void Exti_PE234_Init(void);

#endif
