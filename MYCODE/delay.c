#include "delay.h"

//��ʱ1us ��Ҫ�ƶ��ٸ�
int my_us = 21;
//��ʱ1ms ��Ҫ�������ٸ�
int my_ms = 21000;

void Delay_Init(void)
{
	//ʱ��Դ��ѡ��  ѡ��ο�ʱ��Դ 168MHZ / 8 = 21MHZ
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}
//5us  21*5 = 105
void delay_us(int nus)
{
	int temp;
	//������װ��ֵ  --��Ҫ�������ٸ�
	SysTick->LOAD = my_us *nus - 1;
	//��ǰֵ�Ĵ��� ����Ϊ0
	//������ʹ�ܶ�ʱ����ʱ��Ӳ�����Զ�����װ��ֵ����ֵ ���뵽  ��������
	SysTick->VAL = 0;
	
	//ʹ�ܶ�ʱ��
	SysTick->CTRL |= (0x01<<0);
	
	while(1)
	{
		temp = SysTick->CTRL;
		//�ж�����0 ���˳�   bit16�����1����˵��������������0��
		if((temp & (0x01<<16)) != 0)
		{
			break;
		}
		//�����ʱ�� ͻȻ�ر��ˣ�Ҳ�˳�
		if((temp & 0x01) == 0)
		{
			break;
		}
	}
	//�رն�ʱ��
	SysTick->CTRL &= (~(0x01<<0));
}

/*
����������  0-798ms 
*/
void delay_ms(int nms)
{
	if(nms>798)
		return;
	
	int temp;
	//������װ��ֵ  --��Ҫ�������ٸ�
	SysTick->LOAD = my_ms *nms - 1;
	//��ǰֵ�Ĵ��� ����Ϊ0
	//������ʹ�ܶ�ʱ����ʱ��Ӳ�����Զ�����װ��ֵ����ֵ ���뵽  ��������
	SysTick->VAL = 0;
	
	//ʹ�ܶ�ʱ��
	SysTick->CTRL |= (0x01<<0);
	
//	while(1)
//	{
//		temp = SysTick->CTRL;
//		//�ж�����0 ���˳�   bit16�����1����˵��������������0��
//		if((temp & (0x01<<16)) != 0)
//		{
//			break;
//		}
//		//�����ʱ�� ͻȻ�ر��ˣ�Ҳ�˳�
//		if((temp & 0x01) == 0)
//		{
//			break;
//		}
//	}
	do{
		temp = SysTick->CTRL; 
	}while((temp & (0x01<<0)) && !(temp & (0x01<<16)));
	
	//�رն�ʱ��
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
