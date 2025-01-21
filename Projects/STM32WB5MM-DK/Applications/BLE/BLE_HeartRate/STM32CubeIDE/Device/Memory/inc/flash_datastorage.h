/**
  ******************************************************************************
  * @file    secure_database.h
  * @author  eWBM
  * @version 1.0
  * @date    2018-03-13
  * @brief   Secure Database Header file
  ******************************************************************************
  * @attention
  *
  * Copyright(c) 2015 ~ 2020 eWBM Korea , Ltd.
  * All rights reserved.
  * This software contains confidential information of eWBM Korea Co.,Ltd.
  * and unauthorized distribution of this software, or any portion of it, are
  * prohibited.
  *
  ******************************************************************************
  */

#ifndef __FLASH_DATASTORAGE_H__
#define __FLASH_DATASTORAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
//#include "stm32l4xx_hal_flash.h"

/**
  * @addtogroup MS500_SERVICE MS500 Service
  * @{
  */

/**
  * @addtogroup SECURE_DATABASE Secure Database
  * @{
  */

/**
  * @addtogroup SECURE_DATABASE_EXPORTED_TYPES Secure Database Exported Types
  * @{
  */

#define FDS_MAX_PATH_NUM    (8)         /*!< Secure database max path number */
#define FDS_MAX_PATH_SIZE   (32)        /*!< Secure database max path size */

/**
  * @brief Secure database return value definition
  */
typedef enum
{
    FDS_OK         = 0,     /*!< Secure database return : ok */
    FDS_FAIL       = -1,    /*!< Secure database return : fail */
    FDS_PATH_INDEX = -2,    /*!< Secure database return : path index error */
    FDS_MEMORY     = -4,    /*!< Secure database return : memory error */
    FDS_DUPLICATE  = -5     /*!< Secure database return : duplicate path */
} FDS_Ret;

/**
  * @brief Secure database mode definition
  */
typedef enum
{
    FDS_PLAIN   = 0,    /*!< Secure database plain data mode */
    FDS_ENCRYPT         /*!< Secure database encrypt data mode */
} FDS_Mode;

/**
  * @brief Secure database path information structure definition
  */
typedef struct
{
    uint8_t  data[FDS_MAX_PATH_SIZE];   /*!< Secure database path buffer */
    uint32_t size;                      /*!< Secure database path size */
    uint8_t  used;                      /*!< Secure database path of use */
} FDS_PathInfo_t;

/**
  * @brief Secure database path structure definition
  */
typedef struct
{
    FDS_PathInfo_t info[FDS_MAX_PATH_NUM];  /*!< Secure database path information */
} FDS_Path_t;

/** @} */ /* SECURE_DATABASE_EXPORTED_TYPES */

/**
  * @addtogroup SECURE_DATABASE_EXPORTED_FUNCTIONS Secure Database Exported Functions
  * @{
  */

void FDS_Init(void);
void FDS_DeInit(void);

void ErasePageFlash(uint32_t addr);
void ReadFlash(uint8_t *buf, uint32_t addr, uint32_t size);
void WriteFlash(uint8_t *buf, uint32_t addr, uint32_t size);

int      FDS_MakePath(FDS_Path_t* path, int index, const void* data, int size);
FDS_Ret  FDS_Write(uint8_t* filepath, uint8_t* data, int size, FDS_Mode mode, uint32_t* addr);
FDS_Ret  FDS_Read(uint8_t* filepath, void* data, int* size);
int      FDS_ReadSize(FDS_Path_t* path);
FDS_Ret  FDS_ReadAddr(const FDS_Path_t* path, uint32_t addr, void* data, int size);
FDS_Ret  FDS_SearchPath(FDS_Path_t* path, void* data, int size, uint32_t* pos, uint32_t* cur);
FDS_Ret  FDS_Delete(FDS_Path_t* path);
FDS_Ret  FDS_DeleteAddr(uint32_t addr);
void     FDS_DeleteAll(void);

/** @} */ /* SECURE_DATABASE_EXPORTED_FUNCTIONS */

/** @} */ /* SECURE_DATABASE */

/** @} */ /* MS500_SERVICE */

#ifdef __cplusplus
}
#endif

#endif // __SECURE_DATABASE_H__
