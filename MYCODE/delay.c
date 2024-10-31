#include "delay.h"

//延时1us 需要计多少个
int my_us = 21;
//延时1ms 需要计数多少个
int my_ms = 21000;

void Delay_Init(void)
{
	//时钟源的选择  选择参考时钟源 168MHZ / 8 = 21MHZ
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}
//5us  21*5 = 105
void delay_us(int nus)
{
	int temp;
	//设置重装载值  --需要计数多少个
	SysTick->LOAD = my_us *nus - 1;
	//当前值寄存器 设置为0
	//当我们使能定时器的时候，硬件会自动将重装载值的数值 传入到  计数器中
	SysTick->VAL = 0;
	
	//使能定时器
	SysTick->CTRL |= (0x01<<0);
	
	while(1)
	{
		temp = SysTick->CTRL;
		//判断数到0 就退出   bit16如果是1，则说明计数器计数到0了
		if((temp & (0x01<<16)) != 0)
		{
			break;
		}
		//如果定时器 突然关闭了，也退出
		if((temp & 0x01) == 0)
		{
			break;
		}
	}
	//关闭定时器
	SysTick->CTRL &= (~(0x01<<0));
}

/*
毫秒限制在  0-798ms 
*/
void delay_ms(int nms)
{
	if(nms>798)
		return;
	
	int temp;
	//设置重装载值  --需要计数多少个
	SysTick->LOAD = my_ms *nms - 1;
	//当前值寄存器 设置为0
	//当我们使能定时器的时候，硬件会自动将重装载值的数值 传入到  计数器中
	SysTick->VAL = 0;
	
	//使能定时器
	SysTick->CTRL |= (0x01<<0);
	
//	while(1)
//	{
//		temp = SysTick->CTRL;
//		//判断数到0 就退出   bit16如果是1，则说明计数器计数到0了
//		if((temp & (0x01<<16)) != 0)
//		{
//			break;
//		}
//		//如果定时器 突然关闭了，也退出
//		if((temp & 0x01) == 0)
//		{
//			break;
//		}
//	}
	do{
		temp = SysTick->CTRL; 
	}while((temp & (0x01<<0)) && !(temp & (0x01<<16)));
	
	//关闭定时器
	SysTick->CTRL &= (~(0x01<<0));
}

void delay_s(int ns)
{
	for(int i=0; i<ns; i++)
	{
		delay_ms(500);
		delay_ms(500);	
	}
}
