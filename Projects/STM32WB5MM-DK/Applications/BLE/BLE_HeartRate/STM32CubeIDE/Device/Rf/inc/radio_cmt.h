/** 
 ***********************************************************************************
 *
 * @file			radio_cmt.h
 * 
 * @author		Jaewoon Kim
 * 
 * @date			2024-05-17
 * 
 * @brief			radio
 * 
 ***********************************************************************************
 * 
 * @attention
 * 
 ***********************************************************************************
 * 
 ***********************************************************************************
 */

#ifndef __RADIO_CMT_H
#define __RADIO_CMT_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ----------------------------------------------------------------------*/
#include "main.h"

/* Exported defines --------------------------------------------------------------*/

/* Exported types ----------------------------------------------------------------*/
/* Exported macro ----------------------------------------------------------------*/

/* Exported functions ----------------------------------------------------------1--*/
void vRadioTxInit( void );
void vRadioRxInit( void );
void vRadioClearInterrupt( void );
void vRadioReadAllStatus( void );
void vRadioGoTxInit( void );
void vRadioGoRxInit( void );
void vRadioSend( uint8_t * buf, uint8_t length );

void vRadioEnPreamIrq( uint8_t en );
void vRadioEnSyncIrq( uint8_t en );
void vRadioEnDclkIrq( uint8_t en );

void vRadioStopDCGoRx( void );
void vRadioStartDC( void );
#ifdef __cplusplus
}
#endif

#endif /* __RADIO_CMT_H */

