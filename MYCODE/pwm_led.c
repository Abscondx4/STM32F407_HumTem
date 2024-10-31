#include "pwm_led.h"

/*
LED0   PF9   TIM14  CH1
*/
void Pwm_PF9_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	//2.使能定时器14和相关IO口时钟
	//使能定时器14时钟：  84MHZ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
	//使能GPIOF时钟：
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//3.初始化IO口为复用功能输出
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//复用功能输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP ;//推挽
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	//4.GPIOF9复用映射到定时器14
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); 
	
	//5.初始化定时器：ARR,PSC等
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84-1;//预分频系数  1MHZ
	TIM_TimeBaseInitStruct.TIM_Period = 1000-1;//重装载值  计数值   1ms
	
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//计数方式 递增  
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频因子
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStruct);
	
	//6.初始化输出比较参数  输出通道1初始化
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; //PWM模式1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//输出通道使能
	TIM_OCInitStruct.TIM_Pulse = 0;//CCR1的值 捕获比较寄存器的值 初值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;//输出极性 低电平
	
	TIM_OC1Init(TIM14, &TIM_OCInitStruct);
	
	//7.使能预装载寄存器
	TIM_OC1PreloadConfig(TIM14,TIM_OCPreload_Enable); 
	
	//8.使能自动重装载的预装载寄存器允许位    
	TIM_ARRPreloadConfig(TIM14,ENABLE);
	
	//9.使能定时器
	TIM_Cmd(TIM14,ENABLE);

}
