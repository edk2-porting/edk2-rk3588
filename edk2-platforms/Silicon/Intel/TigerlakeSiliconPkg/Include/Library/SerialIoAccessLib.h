/** @file
  Header file for Serial Io Common Lib

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SERIAL_IO_ACCESS_LIB_H_
#define _SERIAL_IO_ACCESS_LIB_H_

/**
  Returns BAR0

  @param[in] PciCfgBase         Pci Config Base

  @retval    64bit MMIO BAR Address
**/
UINT64
GetSerialIoBar (
  IN UINT64        PciCfgBase
  );

/**
  Returns I2C Pci Config Space

  @param[in] I2cNumber         I2C Number

  @retval    I2C Pci Config Space Address
**/
UINT64
GetSerialIoI2cPciCfg (
  IN UINT8        I2cNumber
  );

/**
  Returns SPI Pci Config Space

  @param[in] SpiNumber         SPI Number

  @retval    SPI Pci Config Space Address
**/
UINT64
GetSerialIoSpiPciCfg (
  IN UINT8        SpiNumber
  );

/**
  Returns UART Pci Config Space

  @param[in] UartNumber         UART Number

  @retval    UART Pci Config Space Address
**/
UINT64
GetSerialIoUartPciCfg (
  IN UINT8        UartNumber
  );

/**
  Checks if Device with given PciDeviceId is one of SerialIo I2C controllers
  If yes, its number is returned through I2cIndex parameter, otherwise I2cIndex is not updated

  @param[in]  PciDevId                  Device ID
  @param[out] I2cNumber                 Number of SerialIo I2C controller

  @retval TRUE                          yes it is a SerialIo I2C controller
  @retval FALSE                         no it isn't a SerialIo I2C controller
**/
BOOLEAN
IsSerialIoI2cDeviceId (
  IN  UINT16    PciDevId,
  OUT UINT8     *I2cNumber
  );

/**
  Checks if I2c is Function 0 Enabled

  @param[in] I2cIndex                   Number of the SerialIo I2C controller

  @retval TRUE                          Enabled
  @retval FALSE                         Disabled
**/
BOOLEAN
IsSerialIoI2cFunction0Enabled (
  IN UINT8    I2cIndex
  );

/**
  Checks if Uart is Function 0 Enabled

  @param[in] UartIndex                   Number of the SerialIo Uart controller

  @retval TRUE                          Enabled
  @retval FALSE                         Disabled
**/
BOOLEAN
IsSerialIoUartFunction0Enabled (
  IN UINT8    UartIndex
  );

/**
  Checks if Spi is Function 0 Enabled

  @param[in] SpiIndex                   Number of the SerialIo Spi controller

  @retval TRUE                          Enabled
  @retval FALSE                         Disabled
**/
BOOLEAN
IsSerialIoSpiFunction0Enabled (
  IN UINT8    SpiIndex
  );

#endif // _SERIAL_IO_ACCESS_LIB_H_
