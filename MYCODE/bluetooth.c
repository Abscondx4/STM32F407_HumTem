#include "bluetooth.h"

/*
PB10  USART3_TX  ���Ͷ�
PB11  USART3_RX  ���ն�
*/
void BlueTooth_Init(int baudRate)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;
	USART_InitTypeDef 	USART_InitStruct;
	NVIC_InitTypeDef 	NVIC_InitStruct;
	
	//3. ����ʱ��ʹ�ܣ�GPIO ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	//5.GPIO ��ʼ�����ã�Ҫ����ģʽΪ���ù���
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//���ù������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP ;//����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct); 
	
	//4.�������Ÿ�����ӳ�䣺���� GPIO_PinAFConfig ����
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11,GPIO_AF_USART3);
	
	//6.���ڲ�����ʼ�������ò����ʣ��ֳ�����żУ��Ȳ���
	USART_InitStruct.USART_BaudRate = baudRate;//������
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//����λ  8
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//ֹͣλ  1
	USART_InitStruct.USART_Parity = USART_Parity_No;//У��λ  ��У��
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;// �����ͺͽ��� --ȫ˫��
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//���� ��
	USART_Init(USART3, &USART_InitStruct);

	//7.�����жϲ��ҳ�ʼ�� NVIC��ʹ���жϣ������Ҫ���������жϲ���Ҫ������裩
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;//�ж�ͨ��  �ж�Դ
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;//��Ӧ���ȼ�

	NVIC_Init(&NVIC_InitStruct);
	
	//8.����Ϊ�����жϣ���ʾ�����ݹ�����CPUҪ�жϽ��н��գ�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  

	//9.ʹ�ܴ���
	USART_Cmd(USART3, ENABLE);	
}


//�жϷ�����
//void USART3_IRQHandler(void)
//{
//    //�жϽ��ձ�־λ�Ƿ���1
//    if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
//    {
//        //��������  -����������������
//		u8 r_byte = USART_ReceiveData(USART3);
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
//		//��ת�� --����
//		USART_SendData(USART3, r_byte);
//    }
//	
//	//������ձ�־λ
//    USART_ClearITPendingBit(USART3, USART_IT_RXNE);   
//}