/** @file
 *
 *  Copyright (c) 2017, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <IndustryStandard/Bcm2836.h>

#ifndef __BCM2836_SDHOST_H__
#define __BCM2836_SDHOST_H__

#define SDHOST_OFFSET               0x00202000
#define SDHOST_BASE_ADDRESS         (BCM2836_SOC_REGISTERS + SDHOST_OFFSET)
#define SDHOST_LENGTH               0x00000100
#define SDHOST_REG(X)               (SDHOST_BASE_ADDRESS + (X))
#define SDHOST_CMD                  SDHOST_REG(0x0)
#define SDHOST_ARG                  SDHOST_REG(0x4)
#define SDHOST_TOUT                 SDHOST_REG(0x8)
#define SDHOST_CDIV                 SDHOST_REG(0xC)
#define SDHOST_RSP0                 SDHOST_REG(0x10) // [31:0]
#define SDHOST_RSP1                 SDHOST_REG(0x14) // [63:32]
#define SDHOST_RSP2                 SDHOST_REG(0x18) // [95:64]
#define SDHOST_RSP3                 SDHOST_REG(0x1C) // [127:96]
#define SDHOST_HSTS                 SDHOST_REG(0x20)
#define SDHOST_VDD                  SDHOST_REG(0x30)
#define SDHOST_EDM                  SDHOST_REG(0x34)
#define SDHOST_HCFG                 SDHOST_REG(0x38)
#define SDHOST_HBCT                 SDHOST_REG(0x3C)
#define SDHOST_DATA                 SDHOST_REG(0x40)
#define SDHOST_HBLC                 SDHOST_REG(0x50)

//
// CMD
//
#define SDHOST_CMD_READ_CMD                     BIT6
#define SDHOST_CMD_WRITE_CMD                    BIT7
#define SDHOST_CMD_RESPONSE_CMD_LONG_RESP       BIT9
#define SDHOST_CMD_RESPONSE_CMD_NO_RESP         BIT10
#define SDHOST_CMD_BUSY_CMD                     BIT11
#define SDHOST_CMD_FAIL_FLAG                    BIT14
#define SDHOST_CMD_NEW_FLAG                     BIT15

//
// VDD
//
#define SDHOST_VDD_POWER_ON         BIT0

//
// HSTS
//
#define SDHOST_HSTS_CLEAR           0x7F8
#define SDHOST_HSTS_BLOCK_IRPT      BIT9
#define SDHOST_HSTS_REW_TIME_OUT    BIT7
#define SDHOST_HSTS_CMD_TIME_OUT    BIT6
#define SDHOST_HSTS_CRC16_ERROR     BIT5
#define SDHOST_HSTS_CRC7_ERROR      BIT4
#define SDHOST_HSTS_FIFO_ERROR      BIT3
#define SDHOST_HSTS_DATA_FLAG       BIT0

#define SDHOST_HSTS_TIMOUT_ERROR    (SDHOST_HSTS_CMD_TIME_OUT | SDHOST_HSTS_REW_TIME_OUT)
#define SDHOST_HSTS_TRANSFER_ERROR  (SDHOST_HSTS_FIFO_ERROR | SDHOST_HSTS_CRC7_ERROR | SDHOST_HSTS_CRC16_ERROR)
#define SDHOST_HSTS_ERROR           (SDHOST_HSTS_TIMOUT_ERROR | SDHOST_HSTS_TRANSFER_ERROR)

//
// HCFG
//
#define SDHOST_HCFG_SLOW_CARD       BIT3
#define SDHOST_HCFG_WIDE_EXT_BUS    BIT2
#define SDHOST_HCFG_WIDE_INT_BUS    BIT1
#define SDHOST_HCFG_DATA_IRPT_EN    BIT4
#define SDHOST_HCFG_BLOCK_IRPT_EN   BIT8
#define SDHOST_HCFG_BUSY_IRPT_EN    BIT10

//
// EDM
//
#define SDHOST_EDM_FIFO_CLEAR               BIT21
#define SDHOST_EDM_WRITE_THRESHOLD_SHIFT    9
#define SDHOST_EDM_READ_THRESHOLD_SHIFT     14
#define SDHOST_EDM_THRESHOLD_MASK           0x1F
#define SDHOST_EDM_READ_THRESHOLD(X)        ((X) << SDHOST_EDM_READ_THRESHOLD_SHIFT)
#define SDHOST_EDM_WRITE_THRESHOLD(X)       ((X) << SDHOST_EDM_WRITE_THRESHOLD_SHIFT)

#define CMD8_SD_ARG       (0x0UL << 12 | BIT8 | 0xCEUL << 0)
#define CMD8_MMC_ARG      (0)

#endif /*__BCM2836_SDHOST_H__ */
