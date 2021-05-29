/*********************************************************************
*						   Flash���������ļ�
*						(c)copyright 2015,jdh
*						  All Right Reserved
*�½�����:2015/3/17 by jdh
*�޸�����:2015/4/29 by jdh
**********************************************************************/

#include "drv_flash.h"

/*********************************************************************
*							��̬����
**********************************************************************/

/*********************************************************************
*							�õ���ַ��Ӧ������
*����:address:��ַ
*����:����
**********************************************************************/

static uint32_t GetSector(uint32_t address);

/*********************************************************************
*							ι��
**********************************************************************/

static void feed_dog(void);

/*********************************************************************
*							����
**********************************************************************/

/*********************************************************************
*							��ʼ��flash
**********************************************************************/

void drv_flash_init(void)
{	
	//����flash
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                    FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
}

/*********************************************************************
*							����flash
*˵��:����������Ϊ��λ
*����:addr:д���ַ
*     size:�����ĵ�ַ���С,��λ�ֽ�
*����:�ɹ�����1,ʧ�ܷ���0
**********************************************************************/

void drv_flash_erase(uint32_t addr,uint32_t size) 
{
	uint32_t start_sector = 0;
	uint32_t end_sector = 0;
	uint16_t i = 0;
	
	//�õ�������
	start_sector = GetSector(addr);
	end_sector = GetSector(addr + size);
  
	//����FLASH
	for (i = start_sector;i < end_sector;i += 8)
	{
		feed_dog();
		//�豸��ѹ:2.7V-3.6V
		if (FLASH_EraseSector(i, VoltageRange_3) != FLASH_COMPLETE)
		{ 
			while (1){}
		}
	}
}

/*********************************************************************
*							д���ֽ�
*����:addr:д���ַ
*     data:д�������
**********************************************************************/

void drv_flash_write_byte(uint32_t addr,uint8_t data) 
{
	FLASH_ProgramByte(addr,data);
}

/*********************************************************************
*							��ȡ�ֽ�
*����:addr:д���ַ
*����:����
**********************************************************************/

uint8_t drv_flash_read_byte(uint32_t addr) 
{
	return *(__IO uint8_t*)addr;
}

/*********************************************************************
*							д����ֳ�����
*����:addr:д���ַ
*     data:д�������
**********************************************************************/

void drv_flash_write_half_word(uint32_t addr,uint16_t data) 
{
	FLASH_ProgramHalfWord(addr,data);
}

/*********************************************************************
*							��ȡ���ֳ�����
*����:addr:д���ַ
*����:����
**********************************************************************/

uint16_t drv_flash_read_half_word(uint32_t addr) 
{
	return *(__IO uint16_t*)addr;
}

/*********************************************************************
*							д���ֳ�����
*����:addr:д���ַ
*     data:д�������
**********************************************************************/

void drv_flash_write_word(uint32_t addr,uint32_t data) 
{
	FLASH_ProgramWord(addr,data);
}

/*********************************************************************
*							��ȡ�ֳ�����
*����:addr:д���ַ
*����:����
**********************************************************************/

uint32_t drv_flash_read_word(uint32_t addr) 
{
	return *(__IO uint32_t*)addr;
}

/*********************************************************************
*							�õ���ַ��Ӧ������
*����:address:��ַ
*����:����
**********************************************************************/

static uint32_t GetSector(uint32_t address)
{
	uint32_t sector = 0;

	if((address < ADDR_FLASH_SECTOR_1) && (address >= ADDR_FLASH_SECTOR_0))
	{
		sector = FLASH_Sector_0;  
	}
	else if((address < ADDR_FLASH_SECTOR_2) && (address >= ADDR_FLASH_SECTOR_1))
	{
		sector = FLASH_Sector_1;  
	}
	else if((address < ADDR_FLASH_SECTOR_3) && (address >= ADDR_FLASH_SECTOR_2))
	{
		sector = FLASH_Sector_2;  
	}
	else if((address < ADDR_FLASH_SECTOR_4) && (address >= ADDR_FLASH_SECTOR_3))
	{
		sector = FLASH_Sector_3;  
	}
	else if((address < ADDR_FLASH_SECTOR_5) && (address >= ADDR_FLASH_SECTOR_4))
	{
		sector = FLASH_Sector_4;  
	}
	else if((address < ADDR_FLASH_SECTOR_6) && (address >= ADDR_FLASH_SECTOR_5))
	{
		sector = FLASH_Sector_5;  
	}
	else if((address < ADDR_FLASH_SECTOR_7) && (address >= ADDR_FLASH_SECTOR_6))
	{
		sector = FLASH_Sector_6;  
	}
	else if((address < ADDR_FLASH_SECTOR_8) && (address >= ADDR_FLASH_SECTOR_7))
	{
		sector = FLASH_Sector_7;  
	}
	else if((address < ADDR_FLASH_SECTOR_9) && (address >= ADDR_FLASH_SECTOR_8))
	{
		sector = FLASH_Sector_8;  
	}
	else if((address < ADDR_FLASH_SECTOR_10) && (address >= ADDR_FLASH_SECTOR_9))
	{
		sector = FLASH_Sector_9;  
	}
	else if((address < ADDR_FLASH_SECTOR_11) && (address >= ADDR_FLASH_SECTOR_10))
	{
		sector = FLASH_Sector_10;  
	}
	else /* (address < FLASH_END_ADDR) && (address >= ADDR_FLASH_SECTOR_11) */
	{
		sector = FLASH_Sector_11;
	}

	return sector;
}

/*********************************************************************
*							д������
*����:addr:д���ַ
*     data:д�������
*     len:���ݳ���
**********************************************************************/

void drv_flash_write(uint32_t addr,uint8_t *data,uint16_t len)
{
	uint16_t i = 0;
	
	for (i = 0;i < len;i++)
	{
		drv_flash_write_byte(addr + i,data[i]);
		
		if (i % 20 == 0 && i != 0)
		{
			feed_dog();
		}
	}
}

/*********************************************************************
*							��ȡ����
*����:addr:����ȡ�ĵ�ַ
*     data:��ȡ���ݻ���
*     len:��ȡ���ݳ���
**********************************************************************/

void drv_flash_read(uint32_t addr,uint8_t *data,uint16_t len)
{
	uint16_t i = 0;
	
	for (i = 0;i < len;i++)
	{
		data[i] = drv_flash_read_byte(addr + i);
		
		if (i % 20 == 0 && i != 0)
		{
			feed_dog();
		}
	}
}

/*********************************************************************
*							ι��
**********************************************************************/

static void feed_dog(void)
{
//    // todo
//	static uint8_t level = 0;
	
	//ι�ڲ���
	//drv_iwdg_feed();
//	//ι�ⲿ��
//	drv_wdg_feed(level);
//	if (level)
//	{
//		level = 0;
//	}
//	else
//	{
//		level = 1;
//	}
}

