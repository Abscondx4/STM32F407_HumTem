#include "flash.h"
#include "stdio.h"

static uint32_t GetSector(uint32_t Address);

/*
addr: ��Ҫ������д��FLASH����һ���ڴ棬������Ǹ��ڴ�ĵ�ַ
write_buff�� ��Ҫд�������   char buf[1024] = "abcdef"; �����write_buff������Ҫд�����ݵ���ʼ��ַbuf
len: ��д�����ݵ��ֽ���  ����
*/
void Flash_Write(u32 addr, u8 *write_buff, u32 len)
{
	u32 FLASH_USER_START_ADDR,FLASH_USER_END_ADDR;
	uint32_t uwStartSector = 0;
	uint32_t uwEndSector = 0;
	uint32_t uwAddress = 0;
	uint32_t uwSectorCounter = 0;

	/* Unlock the Flash *********************************************************/
	/* �������FLASH�Ĵ�����д����  ʹ�� */
	//Ĭ��FLASH�Ĵ����ǲ���д�����ݵģ���ô��������Щ�Ĵ�����д����
	FLASH_Unlock();
	  
	/* Erase the user Flash area ************************************************/
	/* area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR */

	/* ���FLASH��ر�־λ */  
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
					FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	
	//��Ҫ������FLASH����ʼ��ַ  �� ������ַ
	FLASH_USER_START_ADDR = addr ;
	FLASH_USER_END_ADDR = addr +len;
	
	/* �õ���Ҫ�����Ŀ�ʼ�����ͽ�������*/
	uwStartSector = GetSector(FLASH_USER_START_ADDR);
	uwEndSector = GetSector(FLASH_USER_END_ADDR);

	/* Ҫ�����������Щ�����Ȳ���*/
	uwSectorCounter = uwStartSector;
	while (uwSectorCounter <= uwEndSector) 
	{
		/* ���������������ж������ͷŲ������ */ 
		if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE)
		{ 
			printf("����%d ����ʧ��\r\n",uwSectorCounter);
			break;
		}

		/* ������һ��Ҫ���������� */
		uwSectorCounter += 8;
	}

	/* Program the user Flash area word by word ********************************/
	/* area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR */
	
	uwAddress = FLASH_USER_START_ADDR;

	while (uwAddress < FLASH_USER_END_ADDR)
	{
		//һ���ֽ�һ���ֽڵ�д������
		FLASH_Status ret = FLASH_ProgramByte(uwAddress,*write_buff);\
		if(ret !=FLASH_COMPLETE )
		{
			printf("write byte error\r\n");
			break;
		}
		write_buff++;
		uwAddress++;
	}

	/* д��֮������ д����*/
	FLASH_Lock(); 
}
/*
�ж�ĳ����ַ ������ �ĸ�������
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

