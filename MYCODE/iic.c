#include "iic.h"

void Iic_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.ʹ��GPIOB��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	//2.GPIO�ڳ�ʼ��  ���  ����  ���� �ٶ�25MHZ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;//����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������Ϊ����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;//�ٶ�
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//����ģʽ
	SCL = 1;
	SDA_OUT = 1;
}

void Iic_Sda_Mode(GPIOMode_TypeDef GPIO_Mode)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//2.GPIO�ڳ�ʼ��  ���  ����  ���� �ٶ�25MHZ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;//����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode;//ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������Ϊ����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;//�ٶ�
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}
//��ʼ�ź�
void Iic_Start(void)
{
	//����������Ϊ���ģʽ
	Iic_Sda_Mode(GPIO_Mode_OUT);
	
	SCL = 1;
	SDA_OUT = 1;
	delay_us(5);
	
	SDA_OUT = 0; //������Ҫ����
	delay_us(5);
	
	SCL = 0;
}

void Iic_Stop(void)
{
	//����������Ϊ���ģʽ
	Iic_Sda_Mode(GPIO_Mode_OUT);
	
	SCL = 0;
	SDA_OUT = 0;
	delay_us(5);
	
	SCL = 1;
	delay_us(5); 
	
	SDA_OUT = 1; //������Ҳ��1
}
//����1bit����
void Iic_Send_Ack(u8 ack)
{
	//����������Ϊ���ģʽ
	Iic_Sda_Mode(GPIO_Mode_OUT);
	
	//�������֮ǰ Ҫ��ʱ��������
	SCL = 0;
	
	if(ack == 1)
	{
		SDA_OUT = 1;
	}
	else{
		SDA_OUT = 0;
	}
	//����Ҫ��ʱ����ʱ��������֮ǰ��Ҫ������׼���ã���ʱ�����ݱ����ȶ�һ��ʱ��
	delay_us(5);

	SCL = 1;
	delay_us(5); //Ϊ���ôӻ� ���㹻��ʱ��ȥ������� ��ȡ����
		
	//SCL = 0;  //���������֤�����д�в���
}

//����һ���ֽ�   ack =  1001 0011
void Iic_Send_Byte(u8 txdata)
{

	//����������Ϊ���ģʽ
	Iic_Sda_Mode(GPIO_Mode_OUT);
	
	//�������֮ǰ Ҫ��ʱ��������
	SCL = 0;
	
	for(int i=0; i<8; i++)
	{
		//׼������
		if(txdata & (0x01 << (7-i)) )
		{
			SDA_OUT = 1;
		}
		else{
			SDA_OUT = 0;
		}
		
		delay_us(5);
		
		SCL = 1;
		delay_us(5);
		
		SCL = 0;
	}
}

//����1bit����
u8 Iic_Recv_Ack()
{
	u8 ack = 0;
	
	//����������Ϊ����ģʽ
	Iic_Sda_Mode(GPIO_Mode_IN);
	
	//������ʱ����SCL����
	SCL = 0;
	delay_us(5);
	
	SCL = 1;
	delay_us(5); //�ôӻ������ݱ���һ��ʱ��
	
	if(SDA_IN == 1)
	{
		ack = 1;
	}
	else{
		ack = 0;
	}
	
	SCL = 0; //�����ʵ����Ҳ��
	return ack;
}

//����1���ֽ�����
u8 Iic_Recv_Byte()
{
	u8 data = 0;
	
	//����������Ϊ����ģʽ
	Iic_Sda_Mode(GPIO_Mode_IN);
	
	SCL = 0;
	
	for(int i=0; i<8; i++)
	{
		delay_us(5);
		SCL = 1;
		delay_us(5); //�ôӻ������ݱ���һ��ʱ��
		
		//�ж����ŵ�ƽ ��ȡ����
		if(SDA_IN == 1)
		{
			data |= (0x01<<(7-i));
		}
		SCL = 0;
	}
	
	return data;
}
/*
AT24C02 һ�� 256���ֽ� ����Ϊ 32ҳ ÿһҳ8���ֽ�  

addr : ��Ҫ������д��AT24C02�е���һ���ַ
write_buff����Ҫд�������
len��д�����ݵĴ�С

*/
void AT24C02_Write_Page(u8 addr, u8 *write_buff, u8 len)
{
	u8 ack;
	//��ʼ�ź�
	Iic_Start();
	
	//���� AT24C02��������ַ  0xA0
	Iic_Send_Byte(0xA0);
	//Ӧ��1bit
	ack = Iic_Recv_Ack();
	if(ack == 1)  //Ӧ�� �͵�ƽ ����ЧӦ��  �ߵ�ƽ˵��������
	{
		printf("ack1 error\r\n");
		return;
	}
	
	//��Ҫ������д�뵽�洢��ָ����λ�ã���ַ������
	//Ҳ����˵����Ҫд��AT24C02�洢������һ��λ���أ���Ҫ������
	//����д������ʼ��ַ
	Iic_Send_Byte(addr);
	//Ӧ��1bit
	ack = Iic_Recv_Ack();
	if(ack == 1)  
	{
		printf("ack2 error\r\n");
		return;
	}
	
	//��������
	for(int i=0; i<len; i++)
	{
		Iic_Send_Byte(write_buff[i]);
		//Ӧ��1bit
		ack = Iic_Recv_Ack();
		if(ack == 1)  
		{
			printf("ack3 error\r\n");
			return;
		}
	}
	//ֹͣ�ź�
	Iic_Stop();
}
/*
�����������
addr:��ʾ��Ҫ����һ���ַ��ʼ��
read_buff:��ȡ������ͨ�����ָ�봫��ȥ
len :��Ҫ��ȡ�����ݵ��ֽ���
*/
void AT24C02_Read(u8 addr, u8 *read_buff, u8 len)
{
	u8 ack;
	//��ʼ�ź�
	Iic_Start();
	
	//���� AT24C02��������ַ  0xA0
	Iic_Send_Byte(0xA0);
	//Ӧ��1bit
	ack = Iic_Recv_Ack();
	if(ack == 1)  //Ӧ�� �͵�ƽ ����ЧӦ��  �ߵ�ƽ˵��������
	{
		printf("ack1 error\r\n");
		return;
	}
	
	//��Ҫ����һ���ַ��ȡ���ݣ��ѵ�ַ����ȥ
	//���Ͷ�������ʼ��ַ
	Iic_Send_Byte(addr);
	//Ӧ��1bit
	ack = Iic_Recv_Ack();
	if(ack == 1)  
	{
		printf("ack2 error\r\n");
		return;
	}
	
	
	//��ʼ�ź�
	Iic_Start();
	//���� AT24C02��������ַ  0xA1
	Iic_Send_Byte(0xA1);
	//Ӧ��1bit
	ack = Iic_Recv_Ack();
	if(ack == 1)  //Ӧ�� �͵�ƽ ����ЧӦ��  �ߵ�ƽ˵��������
	{
		printf("ack1 error\r\n");
		return;
	}
	
	//������
	for(int i=0; i<len; i++)
	{
		read_buff[i] = Iic_Recv_Byte();
		if(i<len-1)
		{
			Iic_Send_Ack(0);
		}
	}
	
	Iic_Send_Ack(1);
	
	//ֹͣ�ź�
	Iic_Stop();
}