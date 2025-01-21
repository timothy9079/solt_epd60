/**
 * @file rf_cmd.h
 * @author Jaewoon Kim (jwkim@solt.co.kr)
 * @brief 
 * @version 0.1
 * @date 2024-09-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __RF_CMD_H
#define __RF_CMD_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ----------------------------------------------------------------------*/

/* Exported defines --------------------------------------------------------------*/
// SOLT RF Keycodes
#define RF_KEYCODE_1	0x14
#define RF_KEYCODE_2	0x24
#define RF_KEYCODE_3	0x28
#define RF_KEYCODE_4	0x34
#define RF_KEYCODE_5	0x38
#define RF_KEYCODE_6	0x44
#define RF_KEYCODE_7	0x48
#define RF_KEYCODE_8	0x54
#define RF_KEYCODE_9	0x58
#define RF_KEYCODE_10	0x64
#define RF_KEYCODE_11	0x68
#define RF_KEYCODE_12	0x74
#define RF_KEYCODE_13	0x78
#define RF_KEYCODE_14	0x84

#define RF_KEYCODE_17	0x98
#define RF_KEYCODE_18	0xA4
#define RF_KEYCODE_19	0xA8
#define RF_KEYCODE_20	0xB4
#define RF_KEYCODE_21	0xB8
#define RF_KEYCODE_22	0xC4
#define RF_KEYCODE_23	0xC8
#define RF_KEYCODE_24	0xD4

#define RF_KEYCODE_MGRC	0x84
#define RF_KEYCODE_GRPC	0xE4
#define RF_KEYCODE_ALLC	0xF4

#define RF_KEYCODE_1_1	0x10
#define RF_KEYCODE_2_1	0x2C
#define RF_KEYCODE_3_1	0x20
#define RF_KEYCODE_4_1	0x3C
#define RF_KEYCODE_5_1	0x30
#define RF_KEYCODE_6_1	0x4C
#define RF_KEYCODE_7_1	0x40
#define RF_KEYCODE_8_1	0x5C
#define RF_KEYCODE_9_1	0x50
#define RF_KEYCODE_10_1	0x6C
#define RF_KEYCODE_11_1	0x60
#define RF_KEYCODE_12_1	0x7C
#define RF_KEYCODE_13_1	0x70
#define RF_KEYCODE_14_1	0x8C
#define RF_KEYCODE_15_1	0xE0
#define RF_KEYCODE_16_1	0xF0

#define RF_KEYCODE_MSK	0xfc
#define RF_LOWBATT_MSK	0x02

// SOLT RF Special ID
#define RF_TEST_BELLID	( 0x3f232900 )
#define RF_CERTIFY_BELLID	( 0xC7B8A900 )

/* Exported types ----------------------------------------------------------------*/
/* Exported macro ----------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif	/* __RF_CMD_H */

