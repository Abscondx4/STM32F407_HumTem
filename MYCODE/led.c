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
	
	//1.ʹ��GPIOF��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//2.GPIO�ڳ�ʼ��  ���  ����  ���� �ٶ�25MHZ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;//PF9����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������Ϊ����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;//�ٶ�
	
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	

	//1.ʹ��GPIOE��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//2.GPIO�ڳ�ʼ��  ���  ����  ���� �ٶ�25MHZ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;//����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������Ϊ����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;//�ٶ�
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	

	//LED����
	GPIO_SetBits(GPIOF,GPIO_Pin_9);
	GPIO_SetBits(GPIOF,GPIO_Pin_10);
	GPIO_SetBits(GPIOE,GPIO_Pin_13);
	GPIO_SetBits(GPIOE,GPIO_Pin_14);
}
