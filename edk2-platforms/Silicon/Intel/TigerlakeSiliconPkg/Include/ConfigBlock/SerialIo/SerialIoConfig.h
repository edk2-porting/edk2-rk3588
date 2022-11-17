/** @file
  Serial IO policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SERIAL_IO_CONFIG_H_
#define _SERIAL_IO_CONFIG_H_

#define SERIAL_IO_CONFIG_REVISION 1
extern EFI_GUID gSerialIoConfigGuid;

#include <SerialIoDevices.h>

#pragma pack (push,1)

/**
  The SERIAL_IO_CONFIG block provides the configurations to set the Serial IO controllers

  <b>Revision 1:</b>
  - Inital version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                                              ///< Config Block Header
  SERIAL_IO_SPI_CONFIG  SpiDeviceConfig[PCH_MAX_SERIALIO_SPI_CONTROLLERS];   ///< SPI Configuration
  SERIAL_IO_I2C_CONFIG  I2cDeviceConfig[PCH_MAX_SERIALIO_I2C_CONTROLLERS];   ///< I2C Configuration
  SERIAL_IO_UART_CONFIG UartDeviceConfig[PCH_MAX_SERIALIO_UART_CONTROLLERS]; ///< UART Configuration
} SERIAL_IO_CONFIG;

#pragma pack (pop)

#endif // _SERIAL_IO_CONFIG_H_
