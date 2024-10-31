#include "key.h"

/*
S1  KEY0	PA0
S2  KEY1	PE2
S3  KEY2	PE3
S4  KEY3	PE4
*/
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.GPIOA时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	//2.GPIO口初始化  PA0  输入 浮空/上拉  速度25MHZ  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;//引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;//速度
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
		
	//1.GPIOE时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//2.GPIO口初始化  PE2 PE3 PE4  输入 浮空/上拉  速度25MHZ  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7|GPIO_Pin_9;//引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;//速度
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}
