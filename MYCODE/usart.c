#include "usart.h"

#include "stdio.h"

#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
    int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
int _sys_exit(int x) 
{ 
    x = x; 
} 
//�ض���fputc����   printf ��һ����
int fputc(int ch, FILE *f)
{     
    USART_SendData(USART1,ch);  //ͨ�����ڷ�������
    //�ȴ����ݷ������
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);      
    return ch;
}


/*
�Զ�������ͨ��Э��

֡ͷ	����		Ӳ������(������)	���ݳ���		����		У����		֡β

H		C			L									 01						:	


֡ͷ��������һ֡���ݰ��Ŀ�ͷ  ռ1���ֽ�    'H'
��� ������Ҫ����     ռ1���ֽ�   
			��������		C
			��ȡ��Ϣ���� 	G
			....
Ӳ�����ͣ��������������������ټ�������
			���������µ������� 
						L		�������LED��
						B		�������BEEP
						D		������Ƶ��
						U		������ƶ��
			��ȡ��Ϣ�����������
						��������
���ݳ��ȣ����� ��Ҫ����/�������ݵ� �ֽ���			 ���������Ȳ�����   ռ0���ֽ�
���ݣ� ��Ҫ���ͻ��߽��յ�Դ����
У���룺 �Դ�������� ����  ���      CRC  ��У��  żУ��       ����Ҳ������ ռ0���ֽ�
֡β�� ��һ֡���ݵ�β��   ռ1���ֽ�     Ĭ��Ϊ ':'


*/





/*
PA9  USART1_TX  ���Ͷ�
PA10 USART1_RX  ���ն�
*/
void Usart1_Init(uint32_t baudRate)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;
	USART_InitTypeDef 	USART_InitStruct;
	NVIC_InitTypeDef 	NVIC_InitStruct;
	
	//3. ����ʱ��ʹ�ܣ�GPIO ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//5.GPIO ��ʼ�����ã�Ҫ����ģʽΪ���ù���
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//���ù������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP ;//����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct); 
	
	//4.�������Ÿ�����ӳ�䣺���� GPIO_PinAFConfig ����
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10,GPIO_AF_USART1);
	
	//6.���ڲ�����ʼ�������ò����ʣ��ֳ�����żУ��Ȳ���
	USART_InitStruct.USART_BaudRate = baudRate;//������
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//����λ  8
	USART_InitStruct.USART_StopBits = USART_StopBits_2;//ֹͣλ  2
	USART_InitStruct.USART_Parity = USART_Parity_No;//У��λ  ��У��
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;// �����ͺͽ��� --ȫ˫��
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//���� ��
	USART_Init(USART1, &USART_InitStruct);
	
	
	//7.�����жϲ��ҳ�ʼ�� NVIC��ʹ���жϣ������Ҫ���������жϲ���Ҫ������裩
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;//�ж�ͨ��  �ж�Դ
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;//��Ӧ���ȼ�

	NVIC_Init(&NVIC_InitStruct);
	
	//8.����Ϊ�����жϣ���ʾ�����ݹ�����CPUҪ�жϽ��н��գ�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  

	//9.ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);	
}

//10.��д�жϴ���������������ʽΪ USARTx_IRQHandler(x ��Ӧ���ں�)��
//�жϷ������в���д��ʱ  Ҳ����д��ѭ��
//void USART1_IRQHandler(void)
//{
//    //�жϽ��ձ�־λ�Ƿ���1
//    if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//    {
//        //��������
//		u8 r_byte = USART_ReceiveData(USART1);
//	
//		//��ÿ�ν��յ�����ƴ�ӵ�������rbuffer
//		rbuffer[r_count++] = r_byte;
//		
//		//������յ�һ֡����
//		if(r_byte == ':')
//		{
//			r_flag = true;
//			printf("rbuffer:%s\r\n",rbuffer); //fputc

//		}
//		
//		//��ת�� --����
//		//USART_SendData(USART1, r_byte);
//    }
//	
//	//������ձ�־λ
//    USART_ClearITPendingBit(USART1, USART_IT_RXNE);   
//}