#include "tim2.h"

//定时1秒钟
void Tim2_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//2.使能定时器时钟 42MHZ X 2 = 84MHZ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//3.初始化定时器，配置ARR,PSC。
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;//预分频系数  84000 000HZ / 8400 =   1000 0HZ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//计数方式  递增/向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 2000 - 1 ;//自动重装载寄存器的值  0~65535  定时200MS
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频因子
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	//4.启动定时器中断，配置NVIC。
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn; //定时器2的中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_Init(&NVIC_InitStruct);
	
	//5.设置允许更新中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	//6.使能定时器。
	TIM_Cmd(TIM2,ENABLE);
}
//7.编写中断服务函数。
void TIM2_IRQHandler(void)
{
	//判断是否触发了中断请求
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		//业务的处理
		//翻转LED0的状态
		GPIO_ToggleBits(GPIOF,GPIO_Pin_10);	
	}
	
	//清除中断标志位
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}


