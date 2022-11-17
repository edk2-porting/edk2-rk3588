/** @file
  Serial IO Private Uart Lib implementation TigerLake specific.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SerialIoAccessLib.h>
#include <Register/SerialIoRegsVer2.h>
#include <IndustryStandard/Pci30.h>
#include <SerialIoPrivateLibInternal.h>
#include <PchReservedResources.h>
#include <PchLimits.h>

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPchLpSerialIoUartDevId [] = {
  V_VER2_PCH_LP_SERIAL_IO_CFG_UART0_DEVICE_ID,
  V_VER2_PCH_LP_SERIAL_IO_CFG_UART1_DEVICE_ID,
  V_VER2_PCH_LP_SERIAL_IO_CFG_UART2_DEVICE_ID,
  V_VER2_PCH_LP_SERIAL_IO_CFG_UART3_DEVICE_ID
};

GLOBAL_REMOVE_IF_UNREFERENCED SERIAL_IO_CONTROLLER_DESCRIPTOR mSerialIoUartFixedAddress [] = {
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x1E000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x1F000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x20000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x21000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x22000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x23000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x24000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x25000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x26000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x27000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x28000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x29000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x2A000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x2B000}
};

/**
  Gets Fixed Base Address used for BAR0

  @param[in] UartNumber              Serial IO device UART number

  @retval                            Config control offset
**/
UINT32
GetSerialIoUartFixedMmioAddress (
  IN UINT8       UartNumber
  )
{
  return mSerialIoUartFixedAddress[UartNumber].Bar0;
}

/**
  Gets Fixed Address used for Pci Config Space manipulation

  @param[in] UartNumber              Serial IO device UART number

  @retval                            Pci Config Address
**/
UINT32
GetSerialIoUartFixedPciCfgAddress (
  IN UINT8       UartNumber
  )
{
  return mSerialIoUartFixedAddress[UartNumber].Bar1;
}

/**
  Gets Uarts Device Id

  @param[in] UartNumbe               Serial IO device UART number

  @retval                            Device Id
**/
UINT16
GetSerialIoUartDeviceId (
  IN UINT8       UartNumber
  )
{
  return mPchLpSerialIoUartDevId[UartNumber];
}
