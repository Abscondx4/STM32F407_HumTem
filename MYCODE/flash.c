#include "flash.h"
#include "stdio.h"

static uint32_t GetSector(uint32_t Address);

/*
addr: 你要将数据写到FLASH中哪一块内存，这个就是该内存的地址
write_buff： 你要写入的数据   char buf[1024] = "abcdef"; 那这个write_buff就是你要写入数据的起始地址buf
len: 你写入数据的字节数  长度
*/
void Flash_Write(u32 addr, u8 *write_buff, u32 len)
{
	u32 FLASH_USER_START_ADDR,FLASH_USER_END_ADDR;
	uint32_t uwStartSector = 0;
	uint32_t uwEndSector = 0;
	uint32_t uwAddress = 0;
	uint32_t uwSectorCounter = 0;

	/* Unlock the Flash *********************************************************/
	/* 解除操作FLASH寄存器的写保护  使能 */
	//默认FLASH寄存器是不能写入数据的，那么必须解除这些寄存器的写保护
	FLASH_Unlock();
	  
	/* Erase the user Flash area ************************************************/
	/* area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR */

	/* 清除FLASH相关标志位 */  
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
					FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	
	//你要操作的FLASH的起始地址  和 结束地址
	FLASH_USER_START_ADDR = addr ;
	FLASH_USER_END_ADDR = addr +len;
	
	/* 得到你要操作的开始扇区和结束扇区*/
	uwStartSector = GetSector(FLASH_USER_START_ADDR);
	uwEndSector = GetSector(FLASH_USER_END_ADDR);

	/* 要把你操作的那些扇区先擦除*/
	uwSectorCounter = uwStartSector;
	while (uwSectorCounter <= uwEndSector) 
	{
		/* 擦除扇区，而且判断扇区释放擦除完成 */ 
		if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE)
		{ 
			printf("扇区%d 擦除失败\r\n",uwSectorCounter);
			break;
		}

		/* 跳到下一个要擦除的扇区 */
		uwSectorCounter += 8;
	}

	/* Program the user Flash area word by word ********************************/
	/* area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR */
	
	uwAddress = FLASH_USER_START_ADDR;

	while (uwAddress < FLASH_USER_END_ADDR)
	{
		//一个字节一个字节的写入数据
		FLASH_Status ret = FLASH_ProgramByte(uwAddress,*write_buff);\
		if(ret !=FLASH_COMPLETE )
		{
			printf("write byte error\r\n");
			break;
		}
		write_buff++;
		uwAddress++;
	}

	/* 写完之后，重启 写保护*/
	FLASH_Lock(); 
}
/*
判断某个地址 是属于 哪个扇区的
*/
static uint32_t GetSector(uint32_t Address)
{
	uint32_t sector = 0;

	if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
	{
		sector = FLASH_Sector_0;  
	}
	else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
	{
		sector = FLASH_Sector_1;  
	}
	else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
	{
		sector = FLASH_Sector_2;  
	}
	else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
	{
		sector = FLASH_Sector_3;  
	}
	else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
	{
		sector = FLASH_Sector_4;  
	}
	else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
	{
		sector = FLASH_Sector_5;  
	}
	else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
	{
		sector = FLASH_Sector_6;  
	}
	else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
	{
		sector = FLASH_Sector_7;  
	}
	

	return sector;
}


void Flash_Read(u32 addr,u8*read_buff,u32 len)
{
	for(int i=0; i<len; i++)
	{
		read_buff[i] = *((__IO uint8_t*)(addr+i));
	}
}

