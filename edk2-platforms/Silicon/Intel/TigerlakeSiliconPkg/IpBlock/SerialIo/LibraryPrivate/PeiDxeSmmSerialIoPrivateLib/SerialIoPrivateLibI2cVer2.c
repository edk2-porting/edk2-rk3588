/** @file
  Serial IO I2C Private Lib implementation TigerLake specific.
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

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPchLpSerialIoI2cDevId [] = {
  V_VER2_PCH_LP_SERIAL_IO_CFG_I2C0_DEVICE_ID,
  V_VER2_PCH_LP_SERIAL_IO_CFG_I2C1_DEVICE_ID,
  V_VER2_PCH_LP_SERIAL_IO_CFG_I2C2_DEVICE_ID,
  V_VER2_PCH_LP_SERIAL_IO_CFG_I2C3_DEVICE_ID,
  V_VER2_PCH_LP_SERIAL_IO_CFG_I2C4_DEVICE_ID,
  V_VER2_PCH_LP_SERIAL_IO_CFG_I2C5_DEVICE_ID
};

GLOBAL_REMOVE_IF_UNREFERENCED SERIAL_IO_CONTROLLER_DESCRIPTOR mSerialIoI2cFixedAddress [] = {
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x0000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x1000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x2000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x3000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x4000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x5000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x6000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x7000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x8000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x9000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0xA000,  PCH_SERIAL_IO_BASE_ADDRESS + 0xB000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0xC000,  PCH_SERIAL_IO_BASE_ADDRESS + 0xD000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0xE000,  PCH_SERIAL_IO_BASE_ADDRESS + 0xF000}
};

/**
  Gets Fixed Address used for Pci Config Space manipulation

  @param[in] I2cNumber               Serial IO device I2C number

  @retval                            Pci Config Address
**/
UINT32
GetSerialIoI2cFixedPciCfgAddress (
  IN UINT8       I2cNumber
  )
{
  return mSerialIoI2cFixedAddress[I2cNumber].Bar1;
}


/**
  Gets I2C Device Id

  @param[in] I2cNumber               Serial IO device I2C number

  @retval                            Device Id
**/
UINT16
GetSerialIoI2cDeviceId (
  IN UINT8       I2cNumber
  )
{
  return mPchLpSerialIoI2cDevId[I2cNumber];
}
