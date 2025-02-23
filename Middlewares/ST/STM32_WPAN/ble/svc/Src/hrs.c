/**
  ******************************************************************************
  * @file    hrs.c
  * @author  MCD Application Team
  * @brief   Heart Rate Service
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
#include "common_blesvc.h"
#include "flash_datastorage.h"

#include "w25q_libs.h"
#include "w25q_mem.h"


/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint16_t	HeartRateSvcHdle;				/**< Service handle */
  uint16_t	HeartRatemeasurementCharHdle;	/**< Characteristic handle */
#if (BLE_CFG_HRS_BODY_SENSOR_LOCATION_CHAR != 0)
  uint16_t	BodySensorLocationCharHdle;		/**< Characteristic handle */
#endif
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
  uint16_t	ControlPointCharHdle;			/**< Characteristic handle */
#endif
#if (BLE_CFG_OTA_REBOOT_CHAR != 0)  
uint16_t        RebootReqCharHdle;                      /**< Characteristic handle */
#endif
}HRS_Context_t;


/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define HRS_CNTL_POINT_RESET_ENERGY_EXPENDED      (0x01)
#define HRS_CNTL_POINT_VALUE_IS_SUPPORTED         (0x00)
#define HRS_CNTL_POINT_VALUE_NOT_SUPPORTED        (0x80)

#define HRS_MAX_NBR_RR_INTERVAL_VALUES            9

#define BM_REQ_CHAR_SIZE    (3)


#define BLE_RX_DATA_BUFFER_SIZE		1024*5
#define BLE_DATA_HEADER_START		0x55

/* Private variables ---------------------------------------------------------*/
/**
 * Reboot Characteristic UUID
 * 0000fe11-8e22-4541-9d4c-21edae82ed19
 */
#if(BLE_CFG_OTA_REBOOT_CHAR != 0)
static const uint8_t BM_REQ_CHAR_UUID[16] = {0x19, 0xed, 0x82, 0xae,
                                       0xed, 0x21, 0x4c, 0x9d,
                                       0x41, 0x45, 0x22, 0x8e,
                                       0x11, 0xFE, 0x00, 0x00};
#endif

typedef struct {
	uint8_t		start;
	uint8_t 	obj;
	uint8_t 	type;
	uint8_t		len_h;
	uint8_t		len_l;
} Bl_PacketHeader_t;

typedef enum {
	BL_RX_STAT_OK = 0,
	BL_RX_STAT_START,
	BL_RX_STAT_CONTINUE,
	BL_RX_STAT_EMPTY
} Bl_Rx_Status;


uint8_t 	blRxDataBuffer[BLE_RX_DATA_BUFFER_SIZE];
uint16_t 	blRxDataBufferIndex = 0;
uint16_t	blRxDataLen = 0;
Bl_PacketHeader_t blDataHeader;	

/**
 * START of Section BLE_DRIVER_CONTEXT
 */

#if 1
__attribute__((section ("BLE_DRIVER_CONTEXT"))) static HRS_Context_t HRS_Context;

#else
PLACE_IN_SECTION("BLE_DRIVER_CONTEXT") static HRS_Context_t HRS_Context;
#endif
/**
 * END of Section BLE_DRIVER_CONTEXT
 */


/* Private function prototypes -----------------------------------------------*/
#if (BLE_CFG_HRS_BODY_SENSOR_LOCATION_CHAR != 0)
static tBleStatus Update_Char_BodySensorLocation( HRS_BodySensorLocation_t *pBodySensorLocationValue );
#endif
static tBleStatus Update_Char_Measurement(HRS_MeasVal_t *pMeasurement );
static SVCCTL_EvtAckStatus_t HeartRate_Event_Handler(void *Event);


void printArrtoHex(uint8_t* txt, uint8_t* buf, uint16_t len){
	if(txt == NULL){
    	printf("Array contents in hex:\r\n");
	}
	else {
		printf("%s:\r\n", txt);
	}
	
    for (int i = 0; i < len; i++) {
        printf("0x%02X ", buf[i]);
        if ((i + 1) % 16 == 0 || i == len - 1) {
            printf("\r\n");
        }
    }

}

static Bl_Rx_Status BlRxData(uint8_t * data, uint16_t len){
	Bl_Rx_Status ret = BL_RX_STAT_CONTINUE;
	
	if(blDataHeader.start == BLE_DATA_HEADER_START) {
		Osal_MemCpy((uint8_t *)(blRxDataBuffer + blRxDataBufferIndex), (uint8_t *)(data + sizeof(Bl_PacketHeader_t)), len - sizeof(Bl_PacketHeader_t));
		blRxDataBufferIndex += len;
	}
	else {
		Osal_MemCpy((uint8_t *)&blDataHeader, data, sizeof(Bl_PacketHeader_t));
		blRxDataLen = (blDataHeader.len_h << 8) + blDataHeader.len_l;
		printf("Bl_PacketHeader_t size : %d\r\n", sizeof(Bl_PacketHeader_t));
		printf("start : 0x%x\r\n obj : 0x%x\r\n type : 0x%x\r\n len : 0x%x \r\n", blDataHeader.start,blDataHeader.obj,blDataHeader.type, blRxDataLen);
		if((blDataHeader.start != BLE_DATA_HEADER_START) ||  (len < sizeof(Bl_PacketHeader_t))){
			Osal_MemSet((uint8_t *)&blDataHeader, 0, sizeof(Bl_PacketHeader_t));
			ret = BL_RX_STAT_EMPTY;
		}
		else {
			Osal_MemSet(blRxDataBuffer, 0, BLE_RX_DATA_BUFFER_SIZE);
			Osal_MemCpy(blRxDataBuffer, (uint8_t *)(data + sizeof(Bl_PacketHeader_t)), len - sizeof(Bl_PacketHeader_t));
			blRxDataBufferIndex = len - sizeof(Bl_PacketHeader_t);
			ret = BL_RX_STAT_START;
		}
	}

	if(blRxDataBufferIndex && (blRxDataBufferIndex >= blRxDataLen)) {
		ret = BL_RX_STAT_OK;
	}

	return ret;
}

/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/

/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t HeartRate_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;
  aci_gatt_attribute_modified_event_rp0    * attribute_modified;
  HRS_App_Notification_evt_t Notification;

  Bl_Rx_Status ret;
  
  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);

  switch(event_pckt->evt)
  {
    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
    {
      blecore_evt = (evt_blecore_aci*)event_pckt->data;
      switch(blecore_evt->ecode)
      {
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
        case ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE:
        {
          aci_gatt_write_permit_req_event_rp0 * write_perm_req;

          BLE_DBG_HRS_MSG("ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE\r\n");
          write_perm_req = (aci_gatt_write_permit_req_event_rp0*)blecore_evt->data;

		  ret = BlRxData(write_perm_req->Data, write_perm_req->Data_Length);
		  if(ret == BL_RX_STAT_START){
			  printArrtoHex("Rx started. Data header", write_perm_req->Data, write_perm_req->Data_Length);
		  }
		  else if (ret == BL_RX_STAT_OK){
			  blDataHeader.start = 0;
			  printf("Rx data size : %d of %d.\r\n", blRxDataBufferIndex, blRxDataLen);
		  
			  FDS_Write((uint8_t *)("flash/image1"), blRxDataBuffer, blRxDataLen, FDS_PLAIN, NULL);
			  printf("Rx Data received completed!\r\n");
		  }

		  if(blDataHeader.start == BLE_DATA_HEADER_START){
		  	printf("Rx data size : %d of %d.\r\n", blRxDataBufferIndex, blRxDataLen);
		  }

//		  Osal_MemCpy((uint8_t *)(blRxDataBuffer + blRxDataBufferIndex), (uint8_t *)(write_perm_req->Data), write_perm_req->Data_Length);
//		  blRxDataBufferIndex += write_perm_req->Data_Length;

//		  printArrtoHex(NULL, write_perm_req->Data, write_perm_req->Data_Length);

          if(write_perm_req->Attribute_Handle == (HRS_Context.ControlPointCharHdle + 1))
          {
            return_value = SVCCTL_EvtAckFlowEnable;

//            if (write_perm_req->Data[0] == HRS_CNTL_POINT_RESET_ENERGY_EXPENDED)
            {
              /* received a correct value for HRM control point char */
              aci_gatt_write_resp(write_perm_req->Connection_Handle,
                                      write_perm_req->Attribute_Handle,
                                      0x00, /* write_status = 0 (no error))*/
                                      (uint8_t)HRS_CNTL_POINT_VALUE_IS_SUPPORTED, /* err_code */
                                      write_perm_req->Data_Length,
                                      (uint8_t *)&write_perm_req->Data[0]);

              /**
               * Notify the application to Reset The Energy Expended Value
               */
              Notification.HRS_Evt_Opcode = HRS_RESET_ENERGY_EXPENDED_EVT;
              HRS_Notification(&Notification);
            }
#if 0
            else
            {
              /* received value of HRM control point char is incorrect */
              aci_gatt_write_resp(write_perm_req->Connection_Handle,
                                      write_perm_req->Attribute_Handle,
                                      0x1, /* write_status = 1 (error))*/
                                      (uint8_t)HRS_CNTL_POINT_VALUE_NOT_SUPPORTED, /* err_code */
                                      write_perm_req->Data_Length,
                                      (uint8_t *)&write_perm_req->Data[0]);
            }
#endif
          }
        }
        break;
#endif

        case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
        {
          attribute_modified = (aci_gatt_attribute_modified_event_rp0*)blecore_evt->data;
          if(attribute_modified->Attr_Handle == (HRS_Context.HeartRatemeasurementCharHdle + 2))
          {
            return_value = SVCCTL_EvtAckFlowEnable;

            /**
             * Notify the application to start measurement
             */
            if(attribute_modified->Attr_Data[0] & COMSVC_Notification)
            {
              BLE_DBG_HRS_MSG("ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE HRS_NOTIFICATION_ENABLED\r\n");
              Notification.HRS_Evt_Opcode =HRS_NOTIFICATION_ENABLED;
              HRS_Notification(&Notification);
            }
            else
            {
              BLE_DBG_HRS_MSG("ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE HRS_NOTIFICATION_DISABLED\r\n");
              Notification.HRS_Evt_Opcode =HRS_NOTIFICATION_DISABLED;
              HRS_Notification(&Notification);
            }
          }
#if (BLE_CFG_OTA_REBOOT_CHAR != 0)          
          else if(attribute_modified->Attr_Handle == (HRS_Context.RebootReqCharHdle + 1))
            {
              BLE_DBG_HRS_MSG("ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE HRS_STM_BOOT_REQUEST_EVT\r\n");
              Notification.HRS_Evt_Opcode = HRS_STM_BOOT_REQUEST_EVT;
              Notification.DataTransfered.Length=attribute_modified->Attr_Data_Length;
              Notification.DataTransfered.pPayload=attribute_modified->Attr_Data;
              HRS_Notification(&Notification);
            }
#endif         
        }
        break;

        default:
          break;
      }
    }
    break; /* HCI_HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE_SPECIFIC */

    default:
      break;
  }

  return(return_value);
}/* end HeartRate_Event_Handler */

#if (BLE_CFG_HRS_BODY_SENSOR_LOCATION_CHAR != 0)
/**
 * @brief  Body Sensor Location Characteristic update
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 * @param  pBodySensorLocationValue: The address of the new value to be written
 * @retval None
 */
static tBleStatus Update_Char_BodySensorLocation(HRS_BodySensorLocation_t *pBodySensorLocationValue)
{
  tBleStatus return_value = BLE_STATUS_SUCCESS;

  return_value = aci_gatt_update_char_value(HRS_Context.HeartRateSvcHdle,
                                            HRS_Context.BodySensorLocationCharHdle,
                                            0, /* charValOffset */
                                            1, /* charValueLen */
                                            (uint8_t *) pBodySensorLocationValue);
  return return_value;
}/* end Update_Char_BodySensorLocation() */
#endif

/**
 * @brief  Heart rate Measurement Characteristic update
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 * @param  pBodySensorLocationValue: The address of the new value to be written
 * @retval None
 */
static tBleStatus Update_Char_Measurement (HRS_MeasVal_t *pMeasurement )
{
  tBleStatus return_value=BLE_STATUS_SUCCESS;
  uint8_t ahrm_value[
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
                     2                                     /** Energy Expended Info */
#endif
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG != 0)
                     +(2*BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG)  /**< RR Interval */
#endif
                     +3
                     ];

  uint8_t hrm_char_length;

  /**
   * Flags update
   */
  ahrm_value[0] = (uint8_t)pMeasurement->Flags;
  hrm_char_length = 1;

  /**
   *  Heart Rate Measurement Value
   */
  if ( (pMeasurement->Flags) &  HRS_HRM_VALUE_FORMAT_UINT16 )
  {
    ahrm_value[hrm_char_length] = (uint8_t)(pMeasurement->MeasurementValue & 0xFF);
    hrm_char_length++;
    ahrm_value[hrm_char_length] = (uint8_t)(pMeasurement->MeasurementValue >> 8);
    hrm_char_length++;
  }
  else
  {
    ahrm_value[hrm_char_length] = (uint8_t)pMeasurement->MeasurementValue;
    hrm_char_length++;
  }

#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
  /**
   *  Energy Expended
   */
  if ((pMeasurement->Flags) &  HRS_HRM_ENERGY_EXPENDED_PRESENT)
  {
    ahrm_value[hrm_char_length] = (uint8_t)(pMeasurement->EnergyExpended & 0xFF);
    hrm_char_length++;
    ahrm_value[hrm_char_length] = (uint8_t)(pMeasurement->EnergyExpended >> 8);
    hrm_char_length++;
  }
#endif

#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG != 0)
  /**
   *  RR Interval Values
   */
  if ((pMeasurement->Flags) &  HRS_HRM_RR_INTERVAL_PRESENT)
  {
    uint8_t index;
    uint8_t rr_interval_number;

    if((pMeasurement->Flags) &  HRS_HRM_VALUE_FORMAT_UINT16)
    {
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
      if ((pMeasurement->Flags) &  HRS_HRM_ENERGY_EXPENDED_PRESENT)
      {
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG > (HRS_MAX_NBR_RR_INTERVAL_VALUES-2))
        /**
         * When the HRM is on 16bits and the Energy expended info is present
         * The maximum RR values is 7
         */
        if(pMeasurement->NbreOfValidRRIntervalValues > (HRS_MAX_NBR_RR_INTERVAL_VALUES-2))
        {
          rr_interval_number = (HRS_MAX_NBR_RR_INTERVAL_VALUES-2);
        }
        else
#endif
        {
          rr_interval_number = pMeasurement->NbreOfValidRRIntervalValues;
        }
      }
      else
      {
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG > (HRS_MAX_NBR_RR_INTERVAL_VALUES-1))
        /**
         * When the HRM is on 16bits and the Energy expended info is not present
         * The maximum RR values is 8
         */
        if(pMeasurement->NbreOfValidRRIntervalValues > (HRS_MAX_NBR_RR_INTERVAL_VALUES-1))
        {
          rr_interval_number = (HRS_MAX_NBR_RR_INTERVAL_VALUES-1);
        }
        else
#endif
        {
          rr_interval_number = pMeasurement->NbreOfValidRRIntervalValues;
        }
      }
#else /**< (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0) */
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG > (HRS_MAX_NBR_RR_INTERVAL_VALUES-1))
      /**
       * When the HRM is on 16bits and the Energy expended info is not present
       * The maximum RR values is 8
       */
      if(pMeasurement->NbreOfValidRRIntervalValues > (HRS_MAX_NBR_RR_INTERVAL_VALUES-1))
      {
        rr_interval_number = (HRS_MAX_NBR_RR_INTERVAL_VALUES-1);
      }
      else
#endif
      {
        rr_interval_number = pMeasurement->NbreOfValidRRIntervalValues;
      }
#endif
    }
    else
    {
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
      if ((pMeasurement->Flags) &  HRS_HRM_ENERGY_EXPENDED_PRESENT)
      {
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG > (HRS_MAX_NBR_RR_INTERVAL_VALUES-1))
        /**
         * When the HRM is on 8bits and the Energy expended info is present
         * The maximum RR values is 8
         */
        if(pMeasurement->NbreOfValidRRIntervalValues > (HRS_MAX_NBR_RR_INTERVAL_VALUES-1))
        {
          rr_interval_number = (HRS_MAX_NBR_RR_INTERVAL_VALUES-1);
        }
        else
#endif
        {
          rr_interval_number = pMeasurement->NbreOfValidRRIntervalValues;
        }
      }
      else
      {
        rr_interval_number = pMeasurement->NbreOfValidRRIntervalValues;
      }
#else /**< (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0) */
      rr_interval_number = pMeasurement->NbreOfValidRRIntervalValues;
#endif
    }

    for ( index = 0 ; index < rr_interval_number ; index++ )
    {
      ahrm_value[hrm_char_length] = (uint8_t)(pMeasurement->aRRIntervalValues[index] & 0xFF);
      hrm_char_length++;
      ahrm_value[hrm_char_length] = (uint8_t)(pMeasurement->aRRIntervalValues[index] >> 8);
      hrm_char_length++;
    }
  }

#endif  /**< (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG != 0) */

  return_value = aci_gatt_update_char_value(HRS_Context.HeartRateSvcHdle,
                                            HRS_Context.HeartRatemeasurementCharHdle,
                                            0, /* charValOffset */
                                            hrm_char_length, /* charValueLen */
                                            (uint8_t *) &ahrm_value[0]);

  return return_value;
}/* end Update_Char_Measurement() */


/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void HRS_Init(void)
{
  uint16_t uuid;
  tBleStatus hciCmdResult = BLE_STATUS_SUCCESS;

  /**
   *	Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(HeartRate_Event_Handler);

  Osal_MemSet(blRxDataBuffer, 0, BLE_RX_DATA_BUFFER_SIZE);
  Osal_MemSet(&blDataHeader, 0, sizeof(Bl_PacketHeader_t));
  blRxDataBufferIndex = 0;

  /**
   *  Add Heart Rate Service
   *
   * Max_Attribute_Records = 2*no_of_char + 1
   * service_max_attribute_record = 1 for heart rate service +
   *                                2 for hear rate measurement characteristic +
   *                                1 for client char configuration descriptor +
   *                                2 for body sensor location characteristic +
   *                                2 for control point characteristic
   */
  uuid = HEART_RATE_SERVICE_UUID;
  hciCmdResult = aci_gatt_add_service(UUID_TYPE_16,
                                   (Service_UUID_t *) &uuid,
                                   PRIMARY_SERVICE,
#if (BLE_CFG_HRS_BODY_SENSOR_LOCATION_CHAR != 0)
                                   2+
#endif
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
                                   2+
#endif
#if (BLE_CFG_OTA_REBOOT_CHAR != 0)
                                   2+
#endif
                                   4,
                                   &(HRS_Context.HeartRateSvcHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    BLE_DBG_HRS_MSG ("Heart Rate Service (HRS) is added Successfully %04X\r\n",
                        HRS_Context.HeartRateSvcHdle);
  }
  else
  {
    BLE_DBG_HRS_MSG ("FAILED to add Heart Rate Service (HRS), Error: %02X !!\r\n",
                        hciCmdResult);
  }

  /**
   *  Add Heart Rate Measurement Characteristic
   */
  uuid = HEART_RATE_MEASURMENT_UUID;
  hciCmdResult = aci_gatt_add_char(HRS_Context.HeartRateSvcHdle,
                                   UUID_TYPE_16,
                                   (Char_UUID_t *) &uuid ,
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
                                   2										                  /** Energy Expended Info */
#endif
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG != 0)
                                   +(2*BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG)	/**< RR Interval */
#endif
                                   +1                                    /** Flags */
                                   +2,                                   /** Measure */
                                   CHAR_PROP_NOTIFY,
                                   ATTR_PERMISSION_NONE,
                                   GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   1, /* isVariable */
                                   &(HRS_Context.HeartRatemeasurementCharHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    BLE_DBG_HRS_MSG ("Heart Rate Measurement Characteristic Added Successfully  %04X \r\n",
                        HRS_Context.HeartRatemeasurementCharHdle);
  }
  else
  {
    BLE_DBG_HRS_MSG ("FAILED to add Heart Rate Measurement Characteristic, Error: %02X !!\r\n",
                        hciCmdResult);
  }

#if (BLE_CFG_HRS_BODY_SENSOR_LOCATION_CHAR != 0)
  /**
   *  Add Body Sensor Location Characteristic
   */
  uuid = SENSOR_LOCATION_UUID;
  hciCmdResult = aci_gatt_add_char(HRS_Context.HeartRateSvcHdle,
                                   UUID_TYPE_16,
                                   (Char_UUID_t *) &uuid ,
                                   1, /* bytes */
                                   CHAR_PROP_READ,
                                   ATTR_PERMISSION_NONE,
                                   GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   0, /* isVariable: 0 */
                                   &(HRS_Context.BodySensorLocationCharHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    BLE_DBG_HRS_MSG ("Sensor Location Characteristic Added Successfully  %04X \r\n",
                        HRS_Context.BodySensorLocationCharHdle);
  }
  else
  {
    BLE_DBG_HRS_MSG ("FAILED to add Sensor Location Characteristic, Error: %02X !!\r\n",
                        hciCmdResult);
  }

#endif

#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
  uuid = CONTROL_POINT_UUID;
  hciCmdResult = aci_gatt_add_char(HRS_Context.HeartRateSvcHdle,
                                   UUID_TYPE_16,
                                   (Char_UUID_t *) &uuid ,
                                   240, /* bytes */
                                   CHAR_PROP_WRITE,
                                   ATTR_PERMISSION_NONE,
                                   GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   0, /* isVariable: 0*/
                                   &(HRS_Context.ControlPointCharHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    BLE_DBG_HRS_MSG ("Control Point Characteristic Added Successfully  %04X \r\n",
                        HRS_Context.ControlPointCharHdle);
  }
  else
  {
    BLE_DBG_HRS_MSG ("FAILED to add Control Point Characteristic, Error: %02X !!\r\n",
                        hciCmdResult);
  }
#endif

#if (BLE_CFG_OTA_REBOOT_CHAR != 0)
   /**
     *  Add Boot Request Characteristic
     */
     hciCmdResult =   aci_gatt_add_char(HRS_Context.HeartRateSvcHdle,
                      UUID_TYPE_128,
                      (Char_UUID_t *)BM_REQ_CHAR_UUID,
                      BM_REQ_CHAR_SIZE,
                      CHAR_PROP_WRITE_WITHOUT_RESP,
                      ATTR_PERMISSION_NONE,
                      GATT_NOTIFY_ATTRIBUTE_WRITE,
                      10,
                      0,
                      &(HRS_Context.RebootReqCharHdle));
  

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    BLE_DBG_HRS_MSG ("OTA Reboot Characteristic Added Successfully  %04X \r\n",
                        HRS_Context.RebootReqCharHdle);
  }
  else
  {
    BLE_DBG_HRS_MSG ("FAILED to add OTA Reboot Characteristic, Error: %02X !!\r\n",
                        hciCmdResult);
  }
#endif
  
  
  
  return;
}

/**
 * @brief  Characteristic update
 * @param  UUID: UUID of the characteristic
 * @retval BodySensorLocationValue: The new value to be written
 */
tBleStatus HRS_UpdateChar(uint16_t UUID, uint8_t *pPayload)
{
  tBleStatus return_value=0;
  switch(UUID)
  {
#if (BLE_CFG_HRS_BODY_SENSOR_LOCATION_CHAR != 0)
    case SENSOR_LOCATION_UUID:
      return_value = Update_Char_BodySensorLocation((HRS_BodySensorLocation_t*)pPayload);
      break;
#endif
    case HEART_RATE_MEASURMENT_UUID:
      return_value = Update_Char_Measurement((HRS_MeasVal_t*)pPayload);
      break;

    default:
      break;
  }

  return return_value;
}/* end HRS_UpdateChar() */


