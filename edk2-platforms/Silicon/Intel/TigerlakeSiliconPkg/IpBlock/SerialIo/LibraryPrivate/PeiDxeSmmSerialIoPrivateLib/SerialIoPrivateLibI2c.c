/** @file
  Common Serial IO Private Lib implementation - I2C part
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
#include <Library/PciSegmentLib.h>
#include <Library/SerialIoAccessLib.h>
#include <Library/SerialIoPrivateLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Register/SerialIoRegs.h>
#include <SerialIoPrivateLibInternal.h>
#include <PchLimits.h>
#include <Library/PchPciBdfLib.h>

/**
  Checks if I2C is Hidden, and it's Pci Config space available

  @param[in] I2cNumber     Selects Serial IO I2C device

  @retval   TRUE             I2C is in hidden mode
  @retval   FALSE            I2C is not in hidden mode
**/
BOOLEAN
IsSerialIoI2cHidden (
  IN UINT8               I2cNumber
  )
{
  if (MmioRead16 (GetSerialIoI2cFixedPciCfgAddress (I2cNumber) + PCI_DEVICE_ID_OFFSET) == GetSerialIoI2cDeviceId (I2cNumber)) {
    return TRUE;
  }
  return FALSE;
}

/**
  Configures Serial IO Controller before control is passd to the OS

  @param[in] I2cNumber         I2C Number
  @param[in] I2cDeviceConfig   SerialIo I2C Config

**/
VOID
SerialIoI2cBootHandler (
  IN UINT8                      I2cNumber,
  IN SERIAL_IO_I2C_CONFIG       *I2cDeviceConfig
  )
{
  UINT64   PciCfgBase;
  BOOLEAN  TurnOff;

  TurnOff = FALSE;

  if (I2cDeviceConfig->Mode == SerialIoI2cPci) {
    TurnOff = TRUE;
  }

  if ((I2cDeviceConfig->Mode == SerialIoI2cDisabled) && (SerialIoI2cFuncNumber (I2cNumber) == 0x0)) {
    if (SerialIoHigherFunctionsEnabled (SerialIoI2cDevNumber (I2cNumber))) {
      TurnOff = TRUE;
    }
  }

  if (TurnOff) {
    PciCfgBase = GetSerialIoI2cPciCfg (I2cNumber);
    SerialIoSetD3 (PciCfgBase);
    SerialIoMmioDisable (PciCfgBase, TRUE);
  }
}

/**
  Sets Pme Control Status and Command register values required for S3 Boot Script

  @param[in]     I2cNumber         I2C Number
  @param[in]     I2cDeviceConfig   SerialIo I2C Config
  @param[in/out] S3PciCfgBase      S3 Boot Script Pci Config Base
  @param[in/out] Command           Pci Command register data to save
  @param[in/out] Pme               Pci Pme Control register data to save

**/
VOID
SerialIoI2cS3Handler (
  IN     UINT8                 I2cNumber,
  IN     SERIAL_IO_I2C_CONFIG  *I2cDeviceConfig,
  IN OUT UINT64                *S3PciCfgBase,
  IN OUT UINT32                *Command,
  IN OUT UINT32                *Pme
  )
{
  BOOLEAN  TurnOff;
  UINT64   PciCfgBase;

  *S3PciCfgBase = 0;
  TurnOff       = FALSE;

  if (I2cDeviceConfig->Mode == SerialIoI2cPci) {
    TurnOff = TRUE;
  }

  if ((I2cDeviceConfig->Mode == SerialIoI2cDisabled) && (SerialIoI2cFuncNumber (I2cNumber) == 0x0)) {
    if (SerialIoHigherFunctionsEnabled (SerialIoI2cDevNumber (I2cNumber))) {
      TurnOff = TRUE;
    }
  }

  if (TurnOff) {
    *S3PciCfgBase = GetSerialIoI2cS3PciBase (I2cNumber);
    PciCfgBase    = GetSerialIoI2cPciCfg (I2cNumber);
    *Pme          = PciSegmentRead32 ((UINTN) PciCfgBase + R_SERIAL_IO_CFG_PME_CTRL_STS);
    *Pme          = *Pme | BIT0 | BIT1;
    *Command      = PciSegmentRead32 ((UINTN) PciCfgBase + PCI_COMMAND_OFFSET);
    *Command      = *Command & (UINT32)~(EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);
  }
}
