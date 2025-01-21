/**
  ******************************************************************************
  * @file    Flash_datastorage.c
  * @author  eWBM
  * @version 1.0
  * @date    2018-03-13
  * @brief   Flash Data storage Source file
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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
//#include "stm32l4xx_hal_flash.h"
#include "w25q_mem.h"
#include "flash_datastorage.h"

/**
  * @addtogroup MS500_SERVICE MS500 Service
  * @{
  */

/**
  * @addtogroup SECURE_DATABASE Secure Database
  * @{
  */

#define	FDS_PAGE_SIZE					256

#define FDS_BLOCK_SIZE                  (0x1000)		//Sector size
#define FDS_BLOCK_SIZE_MASK             (0xFFF)
#define FDS_STATUS_EMPTY                (0xFFFF)
#define FDS_STATUS_FULL                 (0xAAAA)
#define FDS_STATUS_NONE                 (0x0000)
#define FDS_BLOCK_MAX_COUNT             (8)

#define FDS_START_ADDR					0
#define FDS_END_ADDR					0x80FFFF


static const uint8_t iv[] __ALIGNED(8) = {0xB2, 0xD3, 0xE9, 0x0F, 0x65, 0x0C, 0xB4, 0x8D, 0x06, 0x3D, 0x9D, 0x60};
uint8_t	read_buf[400];

extern uint8_t testbuf[];

typedef struct
{
    FDS_Path_t path;
    uint16_t   status;
    uint16_t   secure;
    uint32_t   blkCnt;
    uint32_t   size;
} FDS_Header_t;

void ErasePageFlash(uint32_t addr){
	
	uint32_t SectAddr;

	if(addr < FDS_BLOCK_SIZE)
	{
		SectAddr = 0;
	}
	else{
		SectAddr = addr / FDS_BLOCK_SIZE;
	}
	
	W25Q_EraseSector(SectAddr);

}

void ReadFlash(uint8_t *buf, uint32_t addr, uint32_t size){
	uint32_t offset = 0;
	uint16_t len = 0;

	while(offset < size){
		if(size - offset > (FDS_PAGE_SIZE-1)){
			len = 256;
		}
		else {
			len = size - offset;
		}
		
		W25Q_ReadRaw((uint8_t*)&buf[offset], len, addr + offset);
//		W25Q_ReadRaw((uint8_t*)&read_buf[offset], len, addr + offset);
		offset += len;
	};
}

uint8_t w25qRead_buf[400];

void WriteFlash(uint8_t *buf, uint32_t addr, uint32_t size){
	uint32_t offset = 0;
	uint16_t len = 0;

	
	W25Q_WakeUP();

	while(offset < size){
		if(size - offset > (FDS_PAGE_SIZE-1)){
			len = 256;
		}
		else {
			len = size - offset;
		}
		W25Q_ProgramRaw((uint8_t*)&buf[offset], len, addr + offset);
		
//		W25Q_ProgramRaw((uint8_t*)testbuf, 160, 0);
//		W25Q_ReadRaw((uint8_t*)w25qRead_buf, 160, 0);
		
//		DbgTrace_mem_print_bin("-- write W25Q ---", w25qRead_buf, 400);

		offset += len;
	};
}

static int FDS_makeFilePathInfo(uint8_t* filepath, FDS_Path_t* path){

}

static int findEmptyBlockAddr(uint32_t* addr, int size)
{
    int      found    = 0;
    int      blkCnt   = 0;
    uint32_t nodeAddr = (*addr != 0 ? *addr : FDS_START_ADDR);
	FDS_Header_t nodeHeader;
    FDS_Header_t* header = NULL;

    blkCnt  = size / FDS_BLOCK_SIZE;
    blkCnt += ((size & FDS_BLOCK_SIZE_MASK) != 0 ? 1 : 0);

    while ( nodeAddr < FDS_END_ADDR )
    {
		ReadFlash((uint8_t *)&nodeHeader, nodeAddr, sizeof(FDS_Header_t));
        header = &nodeHeader;

        if ( header->status == FDS_STATUS_FULL )
        {
            found = 0;
            if (( header->blkCnt == 0 ) || ( header->blkCnt >= FDS_BLOCK_MAX_COUNT ))
                nodeAddr += FDS_BLOCK_SIZE;
            else
                nodeAddr += (FDS_BLOCK_SIZE * header->blkCnt);
        }
        else
        {
            ErasePageFlash(nodeAddr);

            if ( header->status == FDS_STATUS_EMPTY )
            {
                found++;
                if ( found == blkCnt )
                {
                    *addr = nodeAddr - (FDS_BLOCK_SIZE * (found-1));
                    return blkCnt;
                }
            }

            nodeAddr += FDS_BLOCK_SIZE;
        }
    }

    *addr = 0;
    return 0;
}

static FDS_Ret findBlockAddr(FDS_Path_t* path, uint32_t* addr)
{
    int i =0;
    uint32_t nodeAddr = FDS_START_ADDR;
	FDS_Header_t nodeHeader;
    FDS_Header_t* header = NULL;

    while ( nodeAddr < FDS_END_ADDR )
    {
		ReadFlash((uint8_t *)&nodeHeader, nodeAddr, sizeof(FDS_Header_t));
        header = &nodeHeader;

        if ( header->status == FDS_STATUS_FULL )
        {
            int compare = 0;

            for ( i=0; i<FDS_MAX_PATH_NUM; i++ )
            {
                if ( path->info[i].used == 1 )
                {
                    compare |= memcmp(header->path.info[i].data, path->info[i].data, path->info[i].size);
                }
            }

            if ( compare == 0 )
            {
				printf("file find: success, address : 0x%x\r\n", nodeAddr);
                *addr = nodeAddr;
                return FDS_OK;
            }

            nodeAddr += (FDS_BLOCK_SIZE * header->blkCnt);
        }
        else
        {
            nodeAddr += FDS_BLOCK_SIZE;
        }
    }

    *addr = FDS_START_ADDR;

	printf("file not found. start addr : 0x%0x\r\n", *addr);
	return FDS_FAIL;
}

/**
  * @addtogroup SECURE_DATABASE_EXPORTED_FUNCTIONS Secure Database Exported Functions
  * @{
  */


/**
  * @brief      The function adds the path.
  * @param[in]  path    Path structure
  * @param[in]  index   Path index to add
  * @param[in]  data    Path data to add
  * @param[in]  size    Path size to add
  * @return     Secure database result value
  */
int FDS_MakePath(FDS_Path_t* path, int index, const void* data, int size)
{
    if ( index > FDS_MAX_PATH_NUM )
        return FDS_PATH_INDEX;

    if ( size > FDS_MAX_PATH_SIZE )
        return FDS_MEMORY;

    memcpy(path->info[index].data, data, size);
    path->info[index].size = size;
    path->info[index].used = 1;

    return FDS_OK;
}

/**
  * @brief      The function searchs the file address.
  * @param[in]  filePaht    Path + filename
  * @param[out] addr    Data address
  * @return     Secure database result value
  */

FDS_Ret FDS_GetFileAddress(uint8_t* filepath, FDS_Path_t* path,  uint32_t* addr){
	FDS_Ret ret;

	FDS_MakePath(path, 0, filepath, strlen(filepath));

	printf("get file path : %s\r\n", path->info[0].data);
	ret = findBlockAddr(path, addr);
	return ret;
}

/**
  * @brief      The function searchs the path.
  * @param[in]  path    Path structure
  * @param[out] data    Data from the searched path
  * @param[in]  size    Data buffer size to read
  * @param[out] pos     Next path position
  * @return     Secure database result value
  */
FDS_Ret FDS_SearchPath(FDS_Path_t* path, void* data, int size, uint32_t* pos, uint32_t* cur)
{
    uint32_t nodeAddr = 0;
    uint32_t dataAddr = 0;
	FDS_Header_t nodeHeader;
    FDS_Header_t* header = NULL;

    nodeAddr = *pos;
    findBlockAddr(path, &nodeAddr);
    if ( nodeAddr == 0 )
        return FDS_FAIL;

	ReadFlash((uint8_t *)&nodeHeader, nodeAddr, sizeof(FDS_Header_t));
	header = &nodeHeader;
//    header = (FDS_Header_t*)nodeAddr;
    dataAddr = nodeAddr + sizeof(FDS_Header_t);

    if ( data == NULL )
    {
        *cur = nodeAddr;
        *pos = nodeAddr + (FDS_BLOCK_SIZE * header->blkCnt);
        return FDS_OK;
    }

    if ( header->size > size )
        return FDS_MEMORY;

    if ( header->secure == FDS_ENCRYPT )
    {
//        readEncryptData(dataAddr, data, header->size);
    }
    else
    {
		ReadFlash((uint8_t *)data, dataAddr, header->size);
//        memcpy(data, (uint8_t*)dataAddr, header->size);		//del timothy
    }

    *cur = nodeAddr;
    *pos = nodeAddr + (FDS_BLOCK_SIZE * header->blkCnt);

    return FDS_OK;
}

/**
  * @brief      The function writes the data.
  * @param[in]  path    Path structure
  * @param[in]  data    Data buffer to write
  * @param[in]  size    Data size to write
  * @param[in]  mode    Secure database mode
  * @param[out] addr    Data stored address
  * @return     Secure database result value
  */
FDS_Ret FDS_Write(uint8_t* filepath, uint8_t* data, int size, FDS_Mode mode, uint32_t* addr)
{
    int      blkCnt   = 0;
    int      reqSize  = 0;
    uint32_t nodeAddr = 0;
    FDS_Header_t header;
	FDS_Path_t path = {0,};
	FDS_Ret ret;
	uint16_t	dataStart;

	ret = FDS_GetFileAddress(filepath, &path, &nodeAddr);
	
//	printf("Write address : 0x%x\r\n", nodeAddr);
	if(ret == FDS_OK){
		FDS_Delete(&path);
	}

/*
	FDS_MakePath(&path, 0, filepath, strlen(filepath));


	if(FDS_ReadSize(&path) > 0){
		FDS_Delete(&path);
	}

    if ( mode == FDS_ENCRYPT )
        reqSize = sizeof(FDS_Header_t) + size + FDS_AES_GCM_AUTHTAG_SIZE + 128;
    else

    findBlockAddr(&path, &nodeAddr);
    if ( nodeAddr != 0 )
    {
        return FDS_DUPLICATE;
    }
*/
	dataStart = 256*2;
//	reqSize = sizeof(FDS_Header_t) + size;
	reqSize = dataStart + size;

    blkCnt = findEmptyBlockAddr(&nodeAddr, reqSize);
//    if (( nodeAddr == 0 ) || ( blkCnt == 0 )){
	
	if (blkCnt == 0){
		printf("File write failed. Not found Empty Block\r\n");
		return FDS_FAIL;
    }

	printf("Write address : 0x%x\r\n", nodeAddr);

	header.status = FDS_STATUS_FULL;
    header.secure = mode;
    header.blkCnt = blkCnt;
    header.size   = size;
	
    memcpy(&header.path, &path, sizeof(FDS_Path_t));

    if ( addr != NULL )
        memcpy(addr, &nodeAddr, 4);

    WriteFlash((uint8_t*)&header, nodeAddr,  sizeof(FDS_Header_t));
 //   nodeAddr += sizeof(FDS_Header_t);
    nodeAddr += dataStart;

	
	if(dataStart % 8){
		nodeAddr  += 8 - (dataStart % 8);
	}

    if ( header.secure == FDS_ENCRYPT )
    {
//        writeEncryptData(nodeAddr, data, size);
    }
    else
    {
		
       	WriteFlash(data, nodeAddr, size);
    }
	printf("Write success, address: 0x%x. \r\n\n", nodeAddr);

    return FDS_OK;
}

/**
  * @brief      The function reads the data.
  * @param[in]  path    Path structure
  * @param[out] data    Data buffer to read
  * @param[in]  size    Data buffer size to read
  * @return     Secure database result value
  */
FDS_Ret FDS_Read(uint8_t* filepath, void* data, int* size)
{
    uint32_t nodeAddr = 0;
	uint16_t	dataStart;

	FDS_Header_t nodeHeader;
    FDS_Header_t* header = NULL;
	FDS_Path_t path = {0,};
	FDS_Ret ret;

	memset(read_buf, 0, 400);


	printf("FDS Read!\r\n");

	ret = FDS_GetFileAddress(filepath, &path, &nodeAddr);
	
	if(ret != FDS_OK){
		printf("file not found!\r\n");
		return FDS_FAIL;
	}
	
	dataStart = 256*2;
/*
    findBlockAddr(path, &nodeAddr);
    if ( nodeAddr == 0 )
        return FDS_FAIL;
*/
    if ( data == NULL )
        return FDS_OK;

	printf("File read address : 0x%x\r\n", nodeAddr);

	ReadFlash((uint8_t *)&nodeHeader, nodeAddr, sizeof(FDS_Header_t));
	header = &nodeHeader;
//    header = (FDS_Header_t*)nodeAddr;		//del timothy
    nodeAddr += dataStart;
	
	if(dataStart % 8){
		nodeAddr  += 8 - (dataStart % 8);
	}

	printf("file size : 0x%x\r\n", header->size);
/*
    if ( header->size > size )
        return FDS_MEMORY;
*/
    if ( header->secure == FDS_ENCRYPT )
    {
//        readEncryptData(nodeAddr, data, header->size);
    }
    else
    {
		
		printf("File read data address : 0x%x\r\n", nodeAddr);

		ReadFlash((uint8_t *)data, nodeAddr, header->size);
//        memcpy(data, (uint8_t*)nodeAddr, header->size);		//del timothy
    }

	*size = header->size;

	printf("Read success, address: 0x%x. \r\n\n", nodeAddr);

    return FDS_OK;
}

/**
  * @brief      The function gets the data size.
  * @param[in]  path    Path structure
  * @return     Data size
  */
int FDS_ReadSize(FDS_Path_t* path)
{
    uint32_t nodeAddr = 0;
	FDS_Header_t nodeHeader;
    FDS_Header_t* header = NULL;

    findBlockAddr(path, &nodeAddr);
    if ( nodeAddr == 0 )
        return 0;

	ReadFlash((uint8_t *)&nodeHeader, nodeAddr, sizeof(FDS_Header_t));
	header = &nodeHeader;
//    header = (FDS_Header_t*)nodeAddr;		//del timothy

    return header->size;
}

/**
  * @brief      The function reads the data.
  * @param[in]  addr    Data address
  * @param[out] data    Data buffer to read
  * @param[in]  size    Data buffer size to read
  * @return     Secure database result value
  */
FDS_Ret FDS_ReadAddr(const FDS_Path_t* path, uint32_t addr, void* data, int size)
{
    int i = 0;
    int compare = 0;
	FDS_Header_t nodeHeader;
    FDS_Header_t* header = NULL;

	ReadFlash((uint8_t *)&nodeHeader, addr, sizeof(FDS_Header_t));
	header = &nodeHeader;

//    header = (FDS_Header_t*)addr;
    addr += sizeof(FDS_Header_t);

    if ( path != NULL )
    {
        for ( i=0; i<FDS_MAX_PATH_NUM; i++ )
        {
            if ( path->info[i].used == 1 )
            {
                compare |= memcmp(header->path.info[i].data, path->info[i].data, path->info[i].size);
            }
        }

        if ( compare != 0 )
            return FDS_FAIL;
    }

    if ( header->size > size )
        return FDS_MEMORY;

    if ( header->secure == FDS_ENCRYPT )
    {
//        readEncryptData(addr, data, header->size);
    }
    else
    {
		ReadFlash((uint8_t *)data, addr, header->size);
        //memcpy(data, (uint8_t*)addr, header->size);		//del timothy
    }

    return FDS_OK;
}

/**
  * @brief      The function deletes the data of path.
  * @param[in]  path    Path structure
  * @return     Secure database result value
  */
FDS_Ret FDS_Delete(FDS_Path_t* path)
{
    int i = 0;
    int      blkCnt   = 0;
    uint32_t nodeAddr = 0;
	FDS_Header_t nodeHeader;
    FDS_Header_t* header = NULL;

	FDS_Ret ret = 0;

    ret = findBlockAddr(path, &nodeAddr);
    if ( (nodeAddr == 0) && (ret != FDS_OK)){
		printf("Flash Delete fail.\r\n");
		return FDS_FAIL;
    }

	ReadFlash((uint8_t *)&nodeHeader, nodeAddr, sizeof(FDS_Header_t));
	header = &nodeHeader;	
//    header = (FDS_Header_t*)nodeAddr;		//del timothy
    blkCnt = header->blkCnt;

    for ( i=0; i<blkCnt; i++ )
    {
        ErasePageFlash(nodeAddr);
        nodeAddr += FDS_BLOCK_SIZE;
    }

    return FDS_OK;
}

/**
  * @brief      The function deletes the data of path.
  * @param[in]  addr    Data address
  * @return     Secure database result value
  */
FDS_Ret FDS_DeleteAddr(uint32_t addr)
{
    int i = 0;
    int blkCnt = 0;
	FDS_Header_t nodeHeader;
    FDS_Header_t* header = NULL;

	ReadFlash((uint8_t *)&nodeHeader, addr, sizeof(FDS_Header_t));
	header = &nodeHeader;	
//    header = (FDS_Header_t*)addr;		//del timothy
    blkCnt = header->blkCnt;

    for ( i=0; i<blkCnt; i++ )
    {
        ErasePageFlash(addr);
        addr += FDS_BLOCK_SIZE;
    }

    return FDS_OK;
}

/**
  * @brief      The function deletes the all data.
  * @return     None
  */
void FDS_DeleteAll(void)
{
    uint32_t nodeAddr;

    for ( nodeAddr=FDS_START_ADDR; nodeAddr<FDS_END_ADDR; nodeAddr+=FDS_BLOCK_SIZE )
    {
        ErasePageFlash(nodeAddr);
    }
}



/** @} */ /* SECURE_DATABASE_EXPORTED_FUNCTIONS */

/** @} */ /* SECURE_DATABASE */

/** @} */ /* MS500_SERVICE */
