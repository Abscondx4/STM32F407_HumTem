#include "pwm_steering.h"

#include "delay.h"

void Pwm_PB10_TIM2_CH3_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	//2.使能定时器2和相关IO口时钟
	//使能定时器2时钟：  84MHZ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//使能GPIOA时钟：
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	//3.初始化IO口为复用功能输出
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//复用功能输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP ;//推挽
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//4.GPIOA2复用映射到定时器2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2); 
	
	//5.初始化定时器：ARR,PSC等  84 000 000 / 840 = 10000 0HZ
	TIM_TimeBaseInitStruct.TIM_Prescaler = 840-1;//预分频系数  100000 HZ   1000ms计数100000  那么1ms 需要计数100个
	TIM_TimeBaseInitStruct.TIM_Period = 2000-1;//重装载值  计数值   20ms
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//计数方式 递增  
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频因子
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	//6.初始化输出比较参数  输出通道1初始化
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; //PWM模式1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//输出通道使能
	TIM_OCInitStruct.TIM_Pulse = 0;//CCR1的值 捕获比较寄存器的值 初值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性 高电平
	
	TIM_OC3Init(TIM2, &TIM_OCInitStruct);
	
	//7.使能预装载寄存器
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable); 
	
	//8.使能自动重装载的预装载寄存器允许位    
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	
	//9.使能定时器
	TIM_Cmd(TIM2,ENABLE);
}

/*以下函数为0-180度舵机角度控制测试函数
	*	PWM 信号与0-180舵机的关系：
	    时间           计数个数
        20ms		    2000
         1ms            100
	*	0.5ms			50		---------------- 0度
	*	1ms   			100		---------------- 45度
	*	1.5ms 			150		---------------- 90度
	*	2ms   			200		---------------- 135度
	*	2.5ms 			250		---------------- 180度
 
	*	舵机频率与占空比的计算：
	*	设舵机的频率为50HZ，则PWM周期为20ms，0度对应的占空比为2.5%，即0.05ms的高电平输出。
 */
 
//0-180度舵机 --> 0-90-180度控制函数
void Server_180_Angle(void)
{
	TIM_SetCompare3(TIM2,50);//0度  0.5ms			50
	delay_s(2);
	
	TIM_SetCompare3(TIM2,150);//90度  1.5ms 			150
	delay_s(2);
	
	TIM_SetCompare3(TIM2,250);//180度  2.5ms 			250
	delay_s(2);
}