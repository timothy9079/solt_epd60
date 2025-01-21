/**
  ******************************************************************************
  * @file    dbg_trace.c
  * @author  MCD Application Team
  * @brief   This file contains the Interface with BLE Drivers functions.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"

#include "utilities_common.h"
#include "stm_queue.h"
#include "dbg_trace.h"

#define LINE_LEN 16

extern UART_HandleTypeDef hlpuart1;


/* Definition of the function */
#if !defined(__GNUC__)  /* SW4STM32 */
size_t __write(int handle, const unsigned char * buf, size_t bufSize);
#endif

/** @addtogroup TRACE
 * @{
 */


/** @defgroup TRACE_LOG
 * @brief TRACE Logging functions
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/** @defgroup TRACE Log private typedef 
 * @{
 */

/**
 * @}
 */

/* Private defines -----------------------------------------------------------*/
/** @defgroup TRACE Log private defines 
 * @{
 */

/**
 * @}
 */

/* Private macros ------------------------------------------------------------*/
/** @defgroup TRACE Log private macros 
 * @{
 */
/**
 * @}
 */

/* Private variables ---------------------------------------------------------*/
/** @defgroup TRACE Log private variables 
 * @{
 */
#if (( CFG_DEBUG_TRACE_FULL != 0 ) || ( CFG_DEBUG_TRACE_LIGHT != 0 ))
#if (DBG_TRACE_USE_CIRCULAR_QUEUE != 0)
static queue_t MsgDbgTraceQueue;
static uint8_t MsgDbgTraceQueueBuff[DBG_TRACE_MSG_QUEUE_SIZE];
#endif
__IO ITStatus DbgTracePeripheralReady = SET;
#endif
/**
 * @}
 */

/* Global variables ----------------------------------------------------------*/
/** @defgroup TRACE Log Global variable
 * @{
 */
/**
 * @}
 */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup TRACE Log private function prototypes 
 * @{
 */
#if (( CFG_DEBUG_TRACE_FULL != 0 ) || ( CFG_DEBUG_TRACE_LIGHT != 0 ))
static void DbgTrace_TxCpltCallback(void);
#endif


/**
 * @}
 */

void DbgTrace_mem_print_bin(
        const uint8_t *title /**< [in] display name strings */, 
        const uint8_t* buffer /**< [in] print buffer  */, 
        uint32_t length /**< [in] length of buffer */
        )
{
    uint32_t i, sz;
    
    if(title) {
        printf("\t%s, %d \r\r\n", title, length) ;
    }

    if (!buffer) {
        printf("\tNULL\r\n");
        return;
    }

    while (length > 0) {
        sz = length;
        if (sz > LINE_LEN)
            sz = LINE_LEN;

        printf("\t");
        for (i = 0; i < LINE_LEN; i++) {
            if (i < length)
                printf("%02x ", buffer[i]);
            else
                printf("   ");
        }
/*        printf("| ");
        for (i = 0; i < sz; i++) {
            if (buffer[i] > 31 && buffer[i] < 127)
                printf("%c", buffer[i]);
            else
                printf(".");
        }*/
        printf("\r\r\n");


        buffer += sz;
        length -= sz;
    }
}



/* Private Functions Definition ------------------------------------------------------*/
/** @defgroup TRACE Log Private function 
 * @{
 */


/* Functions Definition ------------------------------------------------------*/
/** @defgroup TRACE Log APIs 
 * @{
 */

/**
 * @brief  DbgTraceGetFileName: Return filename string extracted from full path information
 * @param  *fullPath Fullpath string (path + filename)
 * @retval char* Pointer on filename string
 */

const char *DbgTraceGetFileName(const char *fullpath)
{
  const char *ret = fullpath;

  if (strrchr(fullpath, '\\') != NULL)
  {
    ret = strrchr(fullpath, '\\') + 1;
  }
  else if (strrchr(fullpath, '/') != NULL)
  {
    ret = strrchr(fullpath, '/') + 1;
  }

  return ret;
}

/**
 * @brief  DbgTraceBuffer: Output buffer content information to output Stream
 * @param  *pBuffer  Pointer on buffer to be output
 * @param  u32Length buffer Size
 * @paramt strFormat string as expected by "printf" function. Used to desrcibe buffer content information.
 * @param  ...       Parameters to be "formatted" in strFormat string (if any)
 * @retval None
 */

void DbgTraceBuffer(const void *pBuffer, uint32_t u32Length, const char *strFormat, ...)
{
  va_list vaArgs;
  uint32_t u32Index;
  va_start(vaArgs, strFormat);
  vprintf(strFormat, vaArgs);
  va_end(vaArgs);
  for (u32Index = 0; u32Index < u32Length; u32Index ++)
  {
    printf(" %02X", ((const uint8_t *) pBuffer)[u32Index]);
  }
}

#if (( CFG_DEBUG_TRACE_FULL != 0 ) || ( CFG_DEBUG_TRACE_LIGHT != 0 ))
/**
 * @brief  DBG_TRACE USART Tx Transfer completed callback
 * @param  UartHandle: UART handle.
 * @note   Indicate the end of the transmission of a DBG_TRACE trace buffer to DBG_TRACE USART. If queue
 *         contains new trace data to transmit, start a new transmission.
 * @retval None
 */
static void DbgTrace_TxCpltCallback(void)
{
#if (DBG_TRACE_USE_CIRCULAR_QUEUE != 0)
  uint8_t* buf;
  uint16_t bufSize;

  BACKUP_PRIMASK();

  DISABLE_IRQ();			/**< Disable all interrupts by setting PRIMASK bit on Cortex*/
  /* Remove element just sent to UART */
  CircularQueue_Remove(&MsgDbgTraceQueue,&bufSize);

  /* Sense if new data to be sent */
  buf=CircularQueue_Sense(&MsgDbgTraceQueue,&bufSize);


  if ( buf != NULL) 
  {
    RESTORE_PRIMASK();
    DbgOutputTraces((uint8_t*)buf, bufSize, DbgTrace_TxCpltCallback);
  } 
  else
  {
    DbgTracePeripheralReady = SET;
    RESTORE_PRIMASK();
  }

#else
  BACKUP_PRIMASK();

  DISABLE_IRQ();      /**< Disable all interrupts by setting PRIMASK bit on Cortex*/

  DbgTracePeripheralReady = SET;

  RESTORE_PRIMASK();
#endif
}
#endif

void DbgTraceInit( void )
{
#if (( CFG_DEBUG_TRACE_FULL != 0 ) || ( CFG_DEBUG_TRACE_LIGHT != 0 ))
  DbgOutputInit();
#if (DBG_TRACE_USE_CIRCULAR_QUEUE != 0)
  CircularQueue_Init(&MsgDbgTraceQueue, MsgDbgTraceQueueBuff, DBG_TRACE_MSG_QUEUE_SIZE, 0, CIRCULAR_QUEUE_SPLIT_IF_WRAPPING_FLAG);
#endif 
#endif
  return;
}


#if (( CFG_DEBUG_TRACE_FULL != 0 ) || ( CFG_DEBUG_TRACE_LIGHT != 0 ))
#if defined(__GNUC__)  /* SW4STM32 (GCC) */
/**
 * @brief	_write: override the __write standard lib function to redirect printf to USART.
 * @param	handle output handle (STDIO, STDERR...)
 * @param	buf buffer to write
 * @param	bufsize buffer size
 * @param	...: arguments to be formatted in format string
 * @retval none
 */
/*
size_t _write(int handle, const unsigned char * buf, size_t bufSize)
{
  return ( DbgTraceWrite(handle, buf, bufSize) );
}
*/
int _write(int fd, char *ptr, int len)
{
	HAL_UART_Transmit(&hlpuart1, (unsigned char*)ptr, len, HAL_MAX_DELAY);
	return len;
}
#else
/**
 * @brief __write: override the _write standard lib function to redirect printf to USART.
 * @param handle output handle (STDIO, STDERR...)
 * @param buf buffer to write
 * @param bufsize buffer size
 * @param ...: arguments to be formatted in format string
 * @retval none
 */
/* 
size_t __write(int handle, const unsigned char * buf, size_t bufSize)
{
  return ( DbgTraceWrite(handle, buf, bufSize) );
}
*/


int _write(int fd, char *ptr, int len)
{
	HAL_UART_Transmit(&hlpuart1, (unsigned char*)ptr, len, HAL_MAX_DELAY);
	return len;
}



#endif /* #if defined(__GNUC__)  */

/**
 * @brief Override the standard lib function to redirect printf to USART.
 * @param handle output handle (STDIO, STDERR...)
 * @param buf buffer to write
 * @param bufsize buffer size
 * @retval Number of elements written
 */
size_t DbgTraceWrite(int handle, const unsigned char * buf, size_t bufSize)
{
  size_t chars_written = 0;
  uint8_t* buffer;

  BACKUP_PRIMASK();

  /* Ignore flushes */
  if ( handle == -1 )
  {
    chars_written = ( size_t ) 0;
  }
  /* Only allow stdout/stderr output */
  else if ( ( handle != 1 ) && ( handle != 2 ) )
  {
    chars_written = ( size_t ) - 1;
  }
  /* Parameters OK, call the low-level character output routine */
  else if (bufSize != 0)
  {
    chars_written = bufSize;
    /* If queue emepty and TX free, send directly */
    /* CS Start */

#if (DBG_TRACE_USE_CIRCULAR_QUEUE != 0)
    DISABLE_IRQ();      /**< Disable all interrupts by setting PRIMASK bit on Cortex*/
    buffer=CircularQueue_Add(&MsgDbgTraceQueue,(uint8_t*)buf, bufSize,1);
    if (buffer && DbgTracePeripheralReady)
    {
      DbgTracePeripheralReady = RESET;
      RESTORE_PRIMASK();
      DbgOutputTraces((uint8_t*)buffer, bufSize, DbgTrace_TxCpltCallback);
    }
    else
    {
      RESTORE_PRIMASK();
    }
#else
    DISABLE_IRQ();      /**< Disable all interrupts by setting PRIMASK bit on Cortex*/
    DbgTracePeripheralReady = RESET;
    RESTORE_PRIMASK();

    DbgOutputTraces((uint8_t*)buf, bufSize, DbgTrace_TxCpltCallback);
    while (!DbgTracePeripheralReady);
#endif
    /* CS END */
  }
  return ( chars_written );
}

#if defined ( __CC_ARM ) || defined (__ARMCC_VERSION) /* Keil */

/**
  Called from assert() and prints a message on stderr and calls abort().

  \param[in] expr  assert expression that was not TRUE
  \param[in] file  source file of the assertion
  \param[in] line  source line of the assertion
*/
__attribute__((weak,noreturn))
void __aeabi_assert (const char *expr, const char *file, int line) {
  char str[12], *p;

  fputs("*** assertion failed: ", stderr);
  fputs(expr, stderr);
  fputs(", file ", stderr);
  fputs(file, stderr);
  fputs(", line ", stderr);

  p = str + sizeof(str);
  *--p = '\0';
  *--p = '\n';
  while (line > 0) {
    *--p = '0' + (line % 10);
    line /= 10;
  }
  fputs(p, stderr);

  abort();
}

/* For KEIL re-implement our own version of fputc */
int fputc(int ch, FILE *f)
{
  /* temp char avoids endianness issue */
  char tempch = ch;
  /* Write one character to Debug Circular Queue */
  DbgTraceWrite(1U, (const unsigned char *) &tempch, 1);
  return ch;
}

#endif /* #if   defined ( __CC_ARM )  */

#endif /* #if (( CFG_DEBUG_TRACE_FULL != 0 ) || ( CFG_DEBUG_TRACE_LIGHT != 0 )) */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

