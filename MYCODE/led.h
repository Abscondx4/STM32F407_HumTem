#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"


//�Ĵ����ĵ�ַ  0x4002 1400
//       int *p = (volatile int *)(0x40021400+0x00)
//        *p --->������
#define  GPIOF_MODER   		*((volatile int *)(0x40021400+0x00)) //�˿�ģʽ�Ĵ����ĵ�ַ
#define  GPIOF_OTYPER   	*((volatile int *)(0x40021400+0x04)) //������ͼĴ����ĵ�ַ
#define  GPIOF_OSPEEDR      *((volatile int *)(0x40021400+0x08))  //�ٶȼĴ����ĵ�ַ
#define  GPIOF_PUPDR    	*((volatile int *)(0x40021400+0x0C)) //����/�����Ĵ����ĵ�ַ
#define  GPIOF_ODR     		*((volatile int *)(0x40021400+0x14))  //������ݼĴ����ĵ�ַ
  

#define  GPIOE_MODER   		*((volatile int *)(0x40021000+0x00)) //�˿�ģʽ�Ĵ����ĵ�ַ
#define  GPIOE_OTYPER   	*((volatile int *)(0x40021000+0x04)) //������ͼĴ����ĵ�ַ
#define  GPIOE_OSPEEDR      *((volatile int *)(0x40021000+0x08))  //�ٶȼĴ����ĵ�ַ
#define  GPIOE_PUPDR    	*((volatile int *)(0x40021000+0x0C)) //����/�����Ĵ����ĵ�ַ
#define  GPIOE_ODR     		*((volatile int *)(0x40021000+0x14))  //������ݼĴ����ĵ�ַ

//
#define RCC_AHB1ENR			*((volatile int *)(0x40023800+0x30)) //AHB1����ʱ�����ܼĴ���



void Led_Init(void);

#endif 
