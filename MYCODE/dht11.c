#include "dht11.h"

/*
温湿度传感器 DATA   PG9
*/
void Dht11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.使能GPIO组时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	//2.GPIO口初始化  输出  推挽  上拉 速度25MHZ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 ;//引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//输出类型为推挽
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;//速度
	
	GPIO_Init(GPIOG, &GPIO_InitStruct);
}
/*
引脚的模式变更
*/
void Dht11_Pin_Mode(GPIOMode_TypeDef GPIO_Mode)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//2.GPIO口初始化  输出  推挽  上拉 速度25MHZ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 ;//引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode;//模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//输出类型为推挽
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;//速度
	
	GPIO_Init(GPIOG, &GPIO_InitStruct);
}

//启动DHT11正常返回0  不正常返回非0
int	Dht11_Start(void)
{

	int t=0;

	//输出模式
	Dht11_Pin_Mode(GPIO_Mode_OUT);
	//刚开始是高电平
	GPIO_SetBits(GPIOG,GPIO_Pin_9);
	delay_ms(2);
	//起始信号 拉低电平 至少18ms
	GPIO_ResetBits(GPIOG,GPIO_Pin_9);
	delay_ms(25);
	
	//主机再拉高 20-40us
	GPIO_SetBits(GPIOG,GPIO_Pin_9);
	delay_us(30);

	
	
	//变更为 输入模式
	Dht11_Pin_Mode(GPIO_Mode_IN);

	delay_us(40);
	
	//DHT11 响应信号的低电平 持续时间
	while(1)
	{
		uint8_t ret = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9);
		if(ret == SET)
		{
			//直到高电平才退出
			break;
		}
		delay_us(10);
		t++;
		if(t>5) //如果一直都是低电平 超过了80us之后还一直都是低电平，那么说明这个模块出问题了，则退出
		{
			return -1;
		}
	}
	
	t=0;
	//DHT 拉高 80us
	delay_us(40);
	//温湿度传感器 持续给 单片机 发送高电平
	while(1)
	{
		uint8_t ret = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9);
		if(ret == RESET)
		{
			//直到低电平才退出
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
读取1个字节的数据  1001 0111
*/
uint8_t Dht11_Read_Byte()
{

	uint8_t data=0;
	int t,i=0;
	//printf("data[] = %d\r\n",data);
	for(i=0; i<8; i++)
	{
		t=0;
		//把低电平的50us时间过滤掉
		while(1)
		{
			uint8_t ret = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9);
			if(ret == SET)
			{
				//直到高电平才退出
				break;
			}
			delay_us(10);
			t++;
			if(t>6)  //如果一直都是低电平，说明模块出问题了
			{
				//printf("data[3] = %d\r\n",data);
				return 0;
			}
		}
		
		//肯定高电平来了
		delay_us(40);
		//printf("data[3]= %d\r\n",data);
		//如果此时引脚还是高电平，说明输入的数据是1
		uint8_t ret = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9);
		if(ret == SET)
		{
			//说明传入的数据是1
			data |= (0x01<<(7-i)); // 1000 0000
			
			//printf("data[] = %d\r\n",data);
			
			//剩下的就是要高电平过滤掉
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
获取一次完整的数据
返回值：
		成功返回0
		失败返回 非0
*/
int  Dht11_Data(u8*data)
{
	int i,temp;
	//data[0] 湿度整数  
	//data[1] 湿度小数
	//data[2] 温度整数
	//data[3] 温度小数
	//data[4] 校验和
	//u8 data[5] = {0};
	
	for(i=0; i<5; i++)
	{
		data[i] = Dht11_Read_Byte();
		
		//printf("data[%d] = %d\r\n",i,data[i]);
	}
	
	//进行校验
	if(data[4] != data[0] + data[1] + data[2] + data[3])
	{
		return -1;
	}
	
	
	return 0;
}

