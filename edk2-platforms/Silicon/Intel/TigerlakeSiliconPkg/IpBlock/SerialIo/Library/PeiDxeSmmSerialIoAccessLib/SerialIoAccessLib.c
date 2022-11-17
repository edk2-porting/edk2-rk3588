/** @file
  Serial Io Common Lib implementation.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Include/PcieRegs.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/SerialIoAccessLib.h>
#include <Library/SerialIoPrivateLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/DebugLib.h>
#include <PchLimits.h>
#include <PchReservedResources.h>
#include <Library/PchPciBdfLib.h>

/**
  Returns BAR0

  @param[in] PciCfgBase         Pci Config Base

  @retval    64bit MMIO BAR Address
**/
UINT64
GetSerialIoBar (
  IN UINT64        PciCfgBase
  )
{
  if (PciCfgBase < PCH_SERIAL_IO_BASE_ADDRESS) {
    return (UINT64) ((PciSegmentRead32 ((UINTN) (PciCfgBase + PCI_BASE_ADDRESSREG_OFFSET)) & 0xFFFFF000) + LShiftU64 (PciSegmentRead32 ((UINTN) (PciCfgBase + PCI_BASE_ADDRESSREG_OFFSET + 4)), 32));
  }
  return (UINT64) ((MmioRead32 ((UINTN) (PciCfgBase + PCI_BASE_ADDRESSREG_OFFSET)) & 0xFFFFF000) + LShiftU64 (MmioRead32 ((UINTN) (PciCfgBase + PCI_BASE_ADDRESSREG_OFFSET + 4)), 32));
}

/**
  Returns I2C Pci Config Space

  @param[in] I2cNumber         I2C Number

  @retval    I2C Pci Config Space Address
**/
UINT64
GetSerialIoI2cPciCfg (
  IN UINT8        I2cNumber
  )
{
  if (IsSerialIoI2cHidden (I2cNumber)) {
    return (UINTN) GetSerialIoI2cFixedPciCfgAddress (I2cNumber);
  }
  return SerialIoI2cPciCfgBase (I2cNumber);
}

/**
  Returns SPI Pci Config Space

  @param[in] SpiNumber         SPI Number

  @retval    SPI Pci Config Space Address
**/
UINT64
GetSerialIoSpiPciCfg (
  IN UINT8        SpiNumber
  )
{
  if (IsSerialIoSpiHidden (SpiNumber)) {
    return (UINTN) GetSerialIoSpiFixedPciCfgAddress (SpiNumber);
  }
  return SerialIoSpiPciCfgBase (SpiNumber);
}

/**
  Returns UART Pci Config Space

  @param[in] UartNumber         UART Number

  @retval    UART Pci Config Space Address
**/
UINT64
GetSerialIoUartPciCfg (
  IN UINT8        UartNumber
  )
{
  if (IsSerialIoUartHidden (UartNumber)) {
    return GetSerialIoUartFixedPciCfgAddress (UartNumber);
  }
  return SerialIoUartPciCfgBase (UartNumber);
}

/**
  Returns SPI S3 boot script PCI address

  @param[in] UartNumber         UART Number

  @retval    SPI S3 boot script PCI address
**/
UINT64
GetSerialIoSpiS3PciBase (
  IN UINT8        SpiNumber
  )
{
  if (IsSerialIoSpiHidden (SpiNumber)) {
  //
  // It's not expected to return Spi S3 Boot Script PCI address for non PCI mode.
  //
    ASSERT (TRUE);
  }
  return S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (
           DEFAULT_PCI_BUS_NUMBER_PCH,
           SerialIoSpiDevNumber (SpiNumber),
           SerialIoSpiFuncNumber (SpiNumber),
           0
           );
}

/**
  Returns UART S3 boot script PCI address

  @param[in] UartNumber         UART Number

  @retval    UART  S3 boot script PCI address
**/
UINT64
GetSerialIoUartS3PciBase (
  IN UINT8        UartNumber
  )
{
  if (IsSerialIoUartHidden (UartNumber)) {
  //
  // It's not expected to return Uart S3 Boot Script PCI address for non PCI mode.
  //
    ASSERT (TRUE);
  }
  return S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (
           DEFAULT_PCI_BUS_NUMBER_PCH,
           SerialIoUartDevNumber (UartNumber),
           SerialIoUartFuncNumber (UartNumber),
           0
           );
}

/**
  Returns I2C S3 boot script PCI address

  @param[in] I2cNumber         I2C Number

  @retval    I2C  S3 boot script PCI address
**/
UINT64
GetSerialIoI2cS3PciBase (
  IN UINT8        I2cNumber
  )
{
  if (IsSerialIoI2cHidden (I2cNumber)) {
  //
  // It's not expected to return I2c S3 Boot Script PCI address for non PCI mode.
  //
    ASSERT (TRUE);
  }
  return S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (
           DEFAULT_PCI_BUS_NUMBER_PCH,
           SerialIoI2cDevNumber (I2cNumber),
           SerialIoI2cFuncNumber (I2cNumber),
           0
           );
}

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
  )
{
  UINT8 Index;

  for (Index = 0; Index < GetPchMaxSerialIoI2cControllersNum (); Index++) {
    if (PciDevId == GetSerialIoI2cDeviceId (Index)) {
      *I2cNumber = Index;
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Checks if I2c is Function 0 Enabled

  @param[in] I2cIndex                   Number of the SerialIo I2C controller

  @retval TRUE                          Enabled
  @retval FALSE                         Disabled
**/
BOOLEAN
IsSerialIoI2cFunction0Enabled (
  IN UINT8    I2cIndex
  )
{
  if (SerialIoI2cFuncNumber (I2cIndex) == 0) {
    if (SerialIoHigherFunctionsEnabled (SerialIoI2cDevNumber (I2cIndex))) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Checks if Uart is Function 0 Enabled

  @param[in] UartIndex                   Number of the SerialIo Uart controller

  @retval TRUE                          Enabled
  @retval FALSE                         Disabled
**/
BOOLEAN
IsSerialIoUartFunction0Enabled (
  IN UINT8    UartIndex
  )
{
  if (SerialIoUartFuncNumber (UartIndex) == 0) {
    if (SerialIoHigherFunctionsEnabled (SerialIoUartDevNumber (UartIndex))) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Checks if Spi is Function 0 Enabled

  @param[in] SpiIndex                   Number of the SerialIo Spi controller

  @retval TRUE                          Enabled
  @retval FALSE                         Disabled
**/
BOOLEAN
IsSerialIoSpiFunction0Enabled (
  IN UINT8    SpiIndex
  )
{
  if (SerialIoSpiFuncNumber (SpiIndex) == 0) {
    if (SerialIoHigherFunctionsEnabled (SerialIoSpiDevNumber (SpiIndex))) {
      return TRUE;
    }
  }
  return FALSE;
}

