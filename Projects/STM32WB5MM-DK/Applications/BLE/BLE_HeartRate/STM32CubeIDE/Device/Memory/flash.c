/**
  ******************************************************************************
  * @file    flash.c
  * @author  Trustkey
  * @version 1.0
  * @date    2024-07-02
  * @brief   stm32l475 internal flash control source file
  ******************************************************************************
  * @attention
  *
  * Copyright(c) 2015 ~ 2020 Trustkey Korea , Ltd.
  * All rights reserved.
  * This software contains confidential information of Trustkey Korea Co.,Ltd.
  * and unauthorized distribution of this software, or any portion of it, are
  * prohibited.
  *
  ******************************************************************************
  */

#include <stdio.h>
#include <string.h>
#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_flash.h"
#include "flash.h"

	/* USER CODE BEGIN 4 */
HAL_StatusTypeDef ATHW_ErasePageFlash(uint32_t addr)
{
	uint32_t SectorError = 0;

	/* Unlock to control */
	HAL_FLASH_Unlock();

	/* Calculate sector index */
	uint32_t EraseStartPage = ATHW_GetPageNum(addr);
	uint32_t NbOfPage = 1;

	/* Erase sectors */
	FLASH_EraseInitTypeDef EraseInitStruct;

	EraseInitStruct.TypeErase =FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Page = 160 + EraseStartPage;
	EraseInitStruct.NbPages = NbOfPage;

	if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
	{ 
		uint32_t errorcode = HAL_FLASH_GetError();            
		return HAL_ERROR;
	}

	/* Clear cache for flash */
	__HAL_FLASH_DATA_CACHE_DISABLE();
	__HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

	__HAL_FLASH_DATA_CACHE_RESET();
	__HAL_FLASH_INSTRUCTION_CACHE_RESET();

	__HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
	__HAL_FLASH_DATA_CACHE_ENABLE();

	/* Lock flash control register */
	HAL_FLASH_Lock();

	return HAL_OK;  
}


HAL_StatusTypeDef ATHW_WriteFlash(uint32_t addr, uint8_t* data, uint32_t len)
{
	uint32_t bytecnt = 0;
	uint64_t wdata;
	uint32_t	offset = 0;
	/* Unlock to control */
	HAL_FLASH_Unlock();


	while(len > 0)
	{
		if(len < 8){
			wdata = 0;
			memcpy((uint8_t *)&wdata, (const void *)((uint32_t)data + offset), len);
			len = 0;
		}
		else {
			memcpy((uint8_t *)&wdata, (const void *)((uint32_t)data + offset), 8);
			len -= 8;
			offset += 8;
		}

		/* Writing data to flash memory */
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, wdata) == HAL_OK)
		{
			addr = addr + 8;
		}
		else
		{
			uint32_t errorcode = HAL_FLASH_GetError();
			return HAL_ERROR;
		}
	}

	/* Lock flash control register */
	HAL_FLASH_Lock();  

	return HAL_OK;
}

/*
HAL_StatusTypeDef ReadFlash()
{
	__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;
	uint32_t Address = FLASH_USER_START_ADDR;

	while(Address < FLASH_USER_END_ADDR)
	{
		data32 = *(__IO uint32_t*)Address;

		if(data32 != DATA_32)
			MemoryProgramStatus++;

		Address = Address + 4;
	}

	if(MemoryProgramStatus > 0)
		return HAL_ERROR;

	return HAL_OK;
}

uint32_t GetWriteProtect()
{
	FLASH_OBProgramInitTypeDef OBInit; 

	HAL_FLASHEx_OBGetConfig(&OBInit);
	uint32_t SectorsWRPStatus = OBInit.WRPSector & FLASH_WRP_SECTORS;

	return SectorsWRPStatus;
}

HAL_StatusTypeDef EnableWriteProtect()
{
	FLASH_OBProgramInitTypeDef OBInit; 

	HAL_FLASH_OB_Unlock();
	HAL_FLASH_Unlock();

	OBInit.OptionType = OPTIONBYTE_WRP;
	OBInit.WRPState   = OB_WRPSTATE_ENABLE;
	OBInit.Banks      = FLASH_BANK_1;
	OBInit.WRPSector  = FLASH_WRP_SECTORS;
	HAL_FLASHEx_OBProgram(&OBInit);   

	if (HAL_FLASH_OB_Launch() != HAL_OK)
	{
		return HAL_ERROR;
	}

	HAL_FLASH_OB_Lock();  
	HAL_FLASH_Lock();  

	return HAL_OK;
}

HAL_StatusTypeDef DisableWriteProtect()
{
	FLASH_OBProgramInitTypeDef OBInit; 

	HAL_FLASH_OB_Unlock();
	HAL_FLASH_Unlock();

	OBInit.OptionType = OPTIONBYTE_WRP;
	OBInit.WRPState   = OB_WRPSTATE_DISABLE;
	OBInit.Banks      = FLASH_BANK_1;
	OBInit.WRPSector  = FLASH_WRP_SECTORS;
	HAL_FLASHEx_OBProgram(&OBInit); 

	if (HAL_FLASH_OB_Launch() != HAL_OK)
	{
		return HAL_ERROR;
	}

	HAL_FLASH_OB_Lock();  
	HAL_FLASH_Lock();

	return HAL_OK;
}
*/


uint32_t ATHW_GetPageNum(uint32_t Address)
{
	uint32_t pagenum = 0;
	uint32_t offset;

	pagenum = (Address - (ATHW_FDS_START_ADDR-1))/FLASH_PAGE_SIZE;
	pagenum += ((Address - (ATHW_FDS_START_ADDR-1)) % FLASH_PAGE_SIZE) ? 1:0;

	return pagenum;
}

