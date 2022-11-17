/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef OEM_APEI_HI1620_H_H
#define OEM_APEI_HI1620_H_H
#define GPIO0_BASE             0x94100000
#define GPIO1_BASE             0x94110000
#define GPIO_INT_MASK          0x34
#define GPIO_INT_EN            0x30
#define GPIO_SWPORT_DDR        0x04
#define GPIO_INT_TYPE          0x38
#define GPIO_INT_POLARITY      0x3c
#define GPIO_LS_SYNC           0x60
#define GPIO_INT_COMB          0xffc
#define IOMUX_REG_BASE         0x94190000
#define IOMG051                0xCC
#define IOMG052                0xD0
#define PAD_EX_INT1            0x4
#define CPLD_GPIO10_INT_OFFSET 0xfc
#define CPLD_BASE_ADDR         0x80000000
#define CPLD_MASK              0x01030000
#define CPLD_VALUE             0x01020000

#define MAX_GHES                          3
#define GENERIC_HARDWARE_ERROR_BLOCK_SIZE 0x1000
#define HEST_TABLE_SIZE                   0x2000
#define BOOT_ERROR_REGION_SIZE            0x1000
#define GPIO_HEST_NOTIFIED_PIN            BIT8

#define ERST_DATASTORE_SIZE               0x2000
#endif
