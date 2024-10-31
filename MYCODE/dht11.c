#include "dht11.h"

/*
��ʪ�ȴ����� DATA   PG9
*/
void Dht11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.ʹ��GPIO��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	//2.GPIO�ڳ�ʼ��  ���  ����  ���� �ٶ�25MHZ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 ;//����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������Ϊ����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;//�ٶ�
	
	GPIO_Init(GPIOG, &GPIO_InitStruct);
}
/*
���ŵ�ģʽ���
*/
void Dht11_Pin_Mode(GPIOMode_TypeDef GPIO_Mode)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//2.GPIO�ڳ�ʼ��  ���  ����  ���� �ٶ�25MHZ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 ;//����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode;//ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������Ϊ����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;//�ٶ�
	
	GPIO_Init(GPIOG, &GPIO_InitStruct);
}

//����DHT11��������0  ���������ط�0
int	Dht11_Start(void)
{

	int t=0;

	//���ģʽ
	Dht11_Pin_Mode(GPIO_Mode_OUT);
	//�տ�ʼ�Ǹߵ�ƽ
	GPIO_SetBits(GPIOG,GPIO_Pin_9);
	delay_ms(2);
	//��ʼ�ź� ���͵�ƽ ����18ms
	GPIO_ResetBits(GPIOG,GPIO_Pin_9);
	delay_ms(25);
	
	//���������� 20-40us
	GPIO_SetBits(GPIOG,GPIO_Pin_9);
	delay_us(30);

	
	
	//���Ϊ ����ģʽ
	Dht11_Pin_Mode(GPIO_Mode_IN);

	delay_us(40);
	
	//DHT11 ��Ӧ�źŵĵ͵�ƽ ����ʱ��
	while(1)
	{
		uint8_t ret = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9);
		if(ret == SET)
		{
			//ֱ���ߵ�ƽ���˳�
			break;
		}
		delay_us(10);
		t++;
		if(t>5) //���һֱ���ǵ͵�ƽ ������80us֮��һֱ���ǵ͵�ƽ����ô˵�����ģ��������ˣ����˳�
		{
			return -1;
		}
	}
	
	t=0;
	//DHT ���� 80us
	delay_us(40);
	//��ʪ�ȴ����� ������ ��Ƭ�� ���͸ߵ�ƽ
	while(1)
	{
		uint8_t ret = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9);
		if(ret == RESET)
		{
			//ֱ���͵�ƽ���˳�
			break;
		}

		delay_us(10);
		t++;
		if(t>5) 
		{
			return -2;
		}
	}
	
	return 0;
}

/*
��ȡ1���ֽڵ�����  1001 0111
*/
uint8_t Dht11_Read_Byte()
{

	uint8_t data=0;
	int t,i=0;
	//printf("data[] = %d\r\n",data);
	for(i=0; i<8; i++)
	{
		t=0;
		//�ѵ͵�ƽ��50usʱ����˵�
		while(1)
		{
			uint8_t ret = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9);
			if(ret == SET)
			{
				//ֱ���ߵ�ƽ���˳�
				break;
			}
			delay_us(10);
			t++;
			if(t>6)  //���һֱ���ǵ͵�ƽ��˵��ģ���������
			{
				//printf("data[3] = %d\r\n",data);
				return 0;
			}
		}
		
		//�϶��ߵ�ƽ����
		delay_us(40);
		//printf("data[3]= %d\r\n",data);
		//�����ʱ���Ż��Ǹߵ�ƽ��˵�������������1
		uint8_t ret = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9);
		if(ret == SET)
		{
			//˵�������������1
			data |= (0x01<<(7-i)); // 1000 0000
			
			//printf("data[] = %d\r\n",data);
			
			//ʣ�µľ���Ҫ�ߵ�ƽ���˵�
			t=0;
			while(1)
			{
				uint8_t ret = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9);
				if(ret == RESET)
				{
					break;
				}

				delay_us(10);
				t++;
				if(t>8)
				{
					return 0;
				}
			}
			
		}
	}
	return data;
}
/*
��ȡһ������������
����ֵ��
		�ɹ�����0
		ʧ�ܷ��� ��0
*/
int  Dht11_Data(u8*data)
{
	int i,temp;
	//data[0] ʪ������  
	//data[1] ʪ��С��
	//data[2] �¶�����
	//data[3] �¶�С��
	//data[4] У���
	//u8 data[5] = {0};
	
	for(i=0; i<5; i++)
	{
		data[i] = Dht11_Read_Byte();
		
		//printf("data[%d] = %d\r\n",i,data[i]);
	}
	
	//����У��
	if(data[4] != data[0] + data[1] + data[2] + data[3])
	{
		return -1;
	}
	
	
	return 0;
}

