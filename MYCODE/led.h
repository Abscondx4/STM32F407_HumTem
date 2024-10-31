#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"


//寄存器的地址  0x4002 1400
//       int *p = (volatile int *)(0x40021400+0x00)
//        *p --->变量名
#define  GPIOF_MODER   		*((volatile int *)(0x40021400+0x00)) //端口模式寄存器的地址
#define  GPIOF_OTYPER   	*((volatile int *)(0x40021400+0x04)) //输出类型寄存器的地址
#define  GPIOF_OSPEEDR      *((volatile int *)(0x40021400+0x08))  //速度寄存器的地址
#define  GPIOF_PUPDR    	*((volatile int *)(0x40021400+0x0C)) //上拉/下拉寄存器的地址
#define  GPIOF_ODR     		*((volatile int *)(0x40021400+0x14))  //输出数据寄存器的地址
  

#define  GPIOE_MODER   		*((volatile int *)(0x40021000+0x00)) //端口模式寄存器的地址
#define  GPIOE_OTYPER   	*((volatile int *)(0x40021000+0x04)) //输出类型寄存器的地址
#define  GPIOE_OSPEEDR      *((volatile int *)(0x40021000+0x08))  //速度寄存器的地址
#define  GPIOE_PUPDR    	*((volatile int *)(0x40021000+0x0C)) //上拉/下拉寄存器的地址
#define  GPIOE_ODR     		*((volatile int *)(0x40021000+0x14))  //输出数据寄存器的地址

//
#define RCC_AHB1ENR			*((volatile int *)(0x40023800+0x30)) //AHB1外设时钟是能寄存器



void Led_Init(void);

#endif 
