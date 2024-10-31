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
	
	//1.GPIOAʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	//2.GPIO�ڳ�ʼ��  PA0  ���� ����/����  �ٶ�25MHZ  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;//����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;//�ٶ�
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
		
	//1.GPIOEʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//2.GPIO�ڳ�ʼ��  PE2 PE3 PE4  ���� ����/����  �ٶ�25MHZ  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7|GPIO_Pin_9;//����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;//�ٶ�
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}
