#include "led.h"

/*
D1   LED0   PF9
D2   LED1   PF10
D3   LED2   PE13
D4   LED4   PE14
*/
void Led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.使能GPIOF组时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//2.GPIO口初始化  输出  推挽  上拉 速度25MHZ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;//PF9引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//输出类型为推挽
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;//速度
	
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	

	//1.使能GPIOE组时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//2.GPIO口初始化  输出  推挽  上拉 速度25MHZ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;//引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//输出类型为推挽
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;//速度
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	

	//LED灯灭
	GPIO_SetBits(GPIOF,GPIO_Pin_9);
	GPIO_SetBits(GPIOF,GPIO_Pin_10);
	GPIO_SetBits(GPIOE,GPIO_Pin_13);
	GPIO_SetBits(GPIOE,GPIO_Pin_14);
}
