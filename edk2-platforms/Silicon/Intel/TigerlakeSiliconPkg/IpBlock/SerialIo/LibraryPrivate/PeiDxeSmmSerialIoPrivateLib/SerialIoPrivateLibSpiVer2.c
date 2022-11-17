/** @file
  Serial IO Spi Private Lib implementation TigerLake specific.
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
#include <Register/SerialIoRegsVer2.h>
#include <IndustryStandard/Pci30.h>
#include <SerialIoPrivateLibInternal.h>
#include <PchReservedResources.h>
#include <PchLimits.h>

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPchLpSerialIoSpiDevId [] = {
  V_VER2_PCH_LP_SERIAL_IO_CFG_SPI0_DEVICE_ID,
  V_VER2_PCH_LP_SERIAL_IO_CFG_SPI1_DEVICE_ID,
  V_VER2_PCH_LP_SERIAL_IO_CFG_SPI2_DEVICE_ID,
  V_VER2_PCH_LP_SERIAL_IO_CFG_SPI3_DEVICE_ID
};

GLOBAL_REMOVE_IF_UNREFERENCED SERIAL_IO_CONTROLLER_DESCRIPTOR mSerialIoSpiFixedAddress [] = {
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x10000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x11000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x12000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x13000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x14000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x15000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x16000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x17000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x18000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x19000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x1A000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x1B000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x1C000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x1D000}
};

/**
  Gets Fixed Base Address used for BAR0

  @param[in] SpiNumber               Serial IO device SPI number

  @retval                            Config control offset
**/
UINT32
GetSerialIoSpiFixedMmioAddress (
  IN UINT8       SpiNumber
  )
{
  return mSerialIoSpiFixedAddress[SpiNumber].Bar0;
}

/**
  Gets Fixed Address used for Pci Config Space manipulation

  @param[in] SpiNumber               Serial IO device SPI number

  @retval                            Pci Config Address
**/
UINT32
GetSerialIoSpiFixedPciCfgAddress (
  IN UINT8       SpiNumber
  )
{
  return mSerialIoSpiFixedAddress[SpiNumber].Bar1;
}

/**
  Gets Spi Device Id

  @param[in] SpiNumber               Serial IO device SPI number

  @retval                            Device Id
**/
UINT16
GetSerialIoSpiDeviceId (
  IN UINT8       SpiNumber
  )
{
  return mPchLpSerialIoSpiDevId[SpiNumber];
}

