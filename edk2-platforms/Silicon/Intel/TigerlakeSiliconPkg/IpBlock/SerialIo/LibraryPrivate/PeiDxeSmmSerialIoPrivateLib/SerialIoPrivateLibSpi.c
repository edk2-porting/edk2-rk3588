/** @file
  Common Serial IO Private Lib implementation - SPI part
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
  Checks if SPI is Hidden, and it's Pci Config space available

  @param[in]      SpiNumber     Selects Serial IO SPI device

  @retval   TRUE             SPI is in hidden mode
  @retval   FALSE            SPI is not in hidden mode
**/
BOOLEAN
IsSerialIoSpiHidden (
  IN UINT8               SpiNumber
  )
{
  if (MmioRead16 (GetSerialIoSpiFixedPciCfgAddress (SpiNumber) + PCI_DEVICE_ID_OFFSET) == GetSerialIoSpiDeviceId (SpiNumber)) {
    return TRUE;
  }
  return FALSE;
}

/**
  Configures Serial IO Controller before control is passd to the OS

  @param[in] SpiNumber         SPI Number
  @param[in] SpiDeviceConfig   SerialIo SPI Config

**/
VOID
SerialIoSpiBootHandler (
  IN UINT8                      SpiNumber,
  IN SERIAL_IO_SPI_CONFIG       *SpiDeviceConfig
  )
{
  UINT64   PciCfgBase;
  BOOLEAN  TurnOff;

  TurnOff = FALSE;

  if (SpiDeviceConfig->Mode == SerialIoSpiPci) {
    TurnOff = TRUE;
  }

  if ((SpiDeviceConfig->Mode == SerialIoSpiDisabled) && (SerialIoSpiFuncNumber (SpiNumber) == 0x0)) {
    if (SerialIoHigherFunctionsEnabled (SerialIoSpiDevNumber (SpiNumber))) {
      TurnOff = TRUE;
    }
  }

  if (TurnOff) {
    PciCfgBase = GetSerialIoSpiPciCfg (SpiNumber);
    SerialIoSetD3 (PciCfgBase);
    SerialIoMmioDisable (PciCfgBase, TRUE);
  }
}

/**
  Sets Pme Control Status and Command register values required for S3 Boot Script

  @param[in]     SpiNumber         SPI Number
  @param[in]     SpiDeviceConfig   SerialIo SPI Config
  @param[in/out] S3PciCfgBase      S3 Boot Script Pci Config Base
  @param[in/out] Command           Pci Command register data to save
  @param[in/out] Pme               Pci Pme Control register data to save

**/
VOID
SerialIoSpiS3Handler (
  IN     UINT8                 SpiNumber,
  IN     SERIAL_IO_SPI_CONFIG  *SpiDeviceConfig,
  IN OUT UINT64                *S3PciCfgBase,
  IN OUT UINT32                *Command,
  IN OUT UINT32                *Pme
  )
{
  BOOLEAN  TurnOff;
  UINT64   PciCfgBase;

  *S3PciCfgBase = 0;
  TurnOff       = FALSE;

  if (SpiDeviceConfig->Mode == SerialIoSpiPci) {
    TurnOff = TRUE;
  }

  if ((SpiDeviceConfig->Mode == SerialIoSpiDisabled) && (SerialIoSpiFuncNumber (SpiNumber) == 0x0)) {
    if (SerialIoHigherFunctionsEnabled (SerialIoSpiDevNumber (SpiNumber))) {
      TurnOff = TRUE;
    }
  }

  if (TurnOff) {
    *S3PciCfgBase = GetSerialIoSpiS3PciBase (SpiNumber);
    PciCfgBase    = GetSerialIoSpiPciCfg (SpiNumber);
    *Pme          = PciSegmentRead32 ((UINTN) PciCfgBase + R_SERIAL_IO_CFG_PME_CTRL_STS);
    *Pme          = *Pme | BIT0 | BIT1;
    *Command      = PciSegmentRead32 ((UINTN) PciCfgBase + PCI_COMMAND_OFFSET);
    *Command      = *Command & (UINT32)~(EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);
  }
}
