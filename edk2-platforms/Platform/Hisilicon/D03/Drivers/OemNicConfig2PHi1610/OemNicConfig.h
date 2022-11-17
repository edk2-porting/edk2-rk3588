/** @file
*
*  Copyright (c) 2016, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2016, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __OEM_NIC_CONFIG_H__
#define __OEM_NIC_CONFIG_H__

#define I2C_SLAVEADDR_EEPROM     (0x52)

#define I2C_OFFSET_EEPROM_ETH0   (0xc00)
#define I2C_OFFSET_EEPROM_ETH1   (I2C_OFFSET_EEPROM_ETH0 + 6)
#define I2C_OFFSET_EEPROM_ETH2   (I2C_OFFSET_EEPROM_ETH1 + 6)
#define I2C_OFFSET_EEPROM_ETH3   (I2C_OFFSET_EEPROM_ETH2 + 6)
#define I2C_OFFSET_EEPROM_ETH4   (I2C_OFFSET_EEPROM_ETH3 + 6)
#define I2C_OFFSET_EEPROM_ETH5   (I2C_OFFSET_EEPROM_ETH4 + 6)
#define I2C_OFFSET_EEPROM_ETH6   (I2C_OFFSET_EEPROM_ETH5 + 6)
#define I2C_OFFSET_EEPROM_ETH7   (I2C_OFFSET_EEPROM_ETH6 + 6)

#define MAC_ADDR_LEN  6

#pragma pack(1)
typedef struct {
  UINT16 Crc16;
  UINT16 MacLen;
  UINT8  Mac[MAC_ADDR_LEN];
} NIC_MAC_ADDRESS;
#pragma pack()

#endif
