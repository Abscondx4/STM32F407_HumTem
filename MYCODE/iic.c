#include "iic.h"

void Iic_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.使能GPIOB组时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	//2.GPIO口初始化  输出  推挽  上拉 速度25MHZ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;//引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//输出类型为推挽
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;//速度
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//空闲模式
	SCL = 1;
	SDA_OUT = 1;
}

void Iic_Sda_Mode(GPIOMode_TypeDef GPIO_Mode)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//2.GPIO口初始化  输出  推挽  上拉 速度25MHZ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;//引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode;//模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//输出类型为推挽
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;//速度
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}
//起始信号
void Iic_Start(void)
{
	//设置数据线为输出模式
	Iic_Sda_Mode(GPIO_Mode_OUT);
	
	SCL = 1;
	SDA_OUT = 1;
	delay_us(5);
	
	SDA_OUT = 0; //数据线要拉低
	delay_us(5);
	
	SCL = 0;
}

void Iic_Stop(void)
{
	//设置数据线为输出模式
	Iic_Sda_Mode(GPIO_Mode_OUT);
	
	SCL = 0;
	SDA_OUT = 0;
	delay_us(5);
	
	SCL = 1;
	delay_us(5); 
	
	SDA_OUT = 1; //数据线也置1
}
//发送1bit数据
void Iic_Send_Ack(u8 ack)
{
	//设置数据线为输出模式
	Iic_Sda_Mode(GPIO_Mode_OUT);
	
	//变更数据之前 要把时钟线拉低
	SCL = 0;
	
	if(ack == 1)
	{
		SDA_OUT = 1;
	}
	else{
		SDA_OUT = 0;
	}
	//必须要延时，在时钟线拉高之前就要把数据准备好，延时让数据保持稳定一段时间
	delay_us(5);

	SCL = 1;
	delay_us(5); //为了让从机 有足够的时间去检测引脚 获取数据
		
	//SCL = 0;  //后面可以验证这个不写行不行
}

//发送一个字节   ack =  1001 0011
void Iic_Send_Byte(u8 txdata)
{

	//设置数据线为输出模式
	Iic_Sda_Mode(GPIO_Mode_OUT);
	
	//变更数据之前 要把时钟线拉低
	SCL = 0;
	
	for(int i=0; i<8; i++)
	{
		//准备数据
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

//接收1bit数据
u8 Iic_Recv_Ack()
{
	u8 ack = 0;
	
	//设置数据线为输入模式
	Iic_Sda_Mode(GPIO_Mode_IN);
	
	//主动把时钟线SCL拉低
	SCL = 0;
	delay_us(5);
	
	SCL = 1;
	delay_us(5); //让从机的数据保持一段时间
	
	if(SDA_IN == 1)
	{
		ack = 1;
	}
	else{
		ack = 0;
	}
	
	SCL = 0; //这个其实不用也行
	return ack;
}

//接收1个字节数据
u8 Iic_Recv_Byte()
{
	u8 data = 0;
	
	//设置数据线为输入模式
	Iic_Sda_Mode(GPIO_Mode_IN);
	
	SCL = 0;
	
	for(int i=0; i<8; i++)
	{
		delay_us(5);
		SCL = 1;
		delay_us(5); //让从机的数据保持一段时间
		
		//判断引脚电平 获取数据
		if(SDA_IN == 1)
		{
			data |= (0x01<<(7-i));
		}
		SCL = 0;
	}
	
	return data;
}
/*
AT24C02 一共 256个字节 ，分为 32页 每一页8个字节  

addr : 你要将数据写到AT24C02中的哪一块地址
write_buff：你要写入的数据
len：写入数据的大小

*/
void AT24C02_Write_Page(u8 addr, u8 *write_buff, u8 len)
{
	u8 ack;
	//起始信号
	Iic_Start();
	
	//发送 AT24C02的器件地址  0xA0
	Iic_Send_Byte(0xA0);
	//应答1bit
	ack = Iic_Recv_Ack();
	if(ack == 1)  //应答 低电平 是有效应答  高电平说明有问题
	{
		printf("ack1 error\r\n");
		return;
	}
	
	//你要将数据写入到存储器指定的位置（地址）里面
	//也就是说，你要写到AT24C02存储器的哪一个位置呢，你要告诉他
	//发送写数据起始地址
	Iic_Send_Byte(addr);
	//应答1bit
	ack = Iic_Recv_Ack();
	if(ack == 1)  
	{
		printf("ack2 error\r\n");
		return;
	}
	
	//发送数据
	for(int i=0; i<len; i++)
	{
		Iic_Send_Byte(write_buff[i]);
		//应答1bit
		ack = Iic_Recv_Ack();
		if(ack == 1)  
		{
			printf("ack3 error\r\n");
			return;
		}
	}
	//停止信号
	Iic_Stop();
}
/*
函数：随机读
addr:表示你要从哪一块地址开始读
read_buff:读取的数据通过这个指针传出去
len :你要读取的数据的字节数
*/
void AT24C02_Read(u8 addr, u8 *read_buff, u8 len)
{
	u8 ack;
	//起始信号
	Iic_Start();
	
	//发送 AT24C02的器件地址  0xA0
	Iic_Send_Byte(0xA0);
	//应答1bit
	ack = Iic_Recv_Ack();
	if(ack == 1)  //应答 低电平 是有效应答  高电平说明有问题
	{
		printf("ack1 error\r\n");
		return;
	}
	
	//你要从哪一块地址读取数据，把地址发过去
	//发送读数据起始地址
	Iic_Send_Byte(addr);
	//应答1bit
	ack = Iic_Recv_Ack();
	if(ack == 1)  
	{
		printf("ack2 error\r\n");
		return;
	}
	
	
	//起始信号
	Iic_Start();
	//发送 AT24C02的器件地址  0xA1
	Iic_Send_Byte(0xA1);
	//应答1bit
	ack = Iic_Recv_Ack();
	if(ack == 1)  //应答 低电平 是有效应答  高电平说明有问题
	{
		printf("ack1 error\r\n");
		return;
	}
	
	//读数据
	for(int i=0; i<len; i++)
	{
		read_buff[i] = Iic_Recv_Byte();
		if(i<len-1)
		{
			Iic_Send_Ack(0);
		}
	}
	
	Iic_Send_Ack(1);
	
	//停止信号
	Iic_Stop();
}