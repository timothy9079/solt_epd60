#ifndef	__FLASH_H__
#define  __FLASH_H__

#define FLASH_PAGE_PER_BANK		256
#define ATHW_FDS_START_PAGE			160
#define ATHW_FDS_PAGE_SIZE			60			//BANK2
//#define ATHW_FDS_START_ADDR			FLASH_BASE + ((FLASH_PAGE_PER_BANK + ATHW_FDS_START_PAGE) * FLASH_PAGE_SIZE) 

#define ATHW_FDS_AREA_SIZE       (ATHW_FDS_PAGE_SIZE * 4096)                    /*!< Secure database area size */
#define ATHW_FDS_START_ADDR      (FLASH_BASE+(ATHW_FDS_START_PAGE) * 4096)         /*!< Secure database area start address (flash address) */
#define ATHW_FDS_END_ADDR        (ATHW_FDS_START_ADDR+ATHW_FDS_AREA_SIZE)  /*!< Secure database area end address (flash address) */




HAL_StatusTypeDef ATHW_ErasePageFlash(uint32_t addr);
HAL_StatusTypeDef ATHW_WriteFlash(uint32_t addr, uint8_t* data, uint32_t len);

uint32_t ATHW_GetPageNum(uint32_t Address);


#endif
