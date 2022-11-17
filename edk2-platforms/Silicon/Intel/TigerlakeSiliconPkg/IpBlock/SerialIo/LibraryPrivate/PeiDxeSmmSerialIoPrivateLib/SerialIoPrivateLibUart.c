/** @file
  Serial IO Private Lib implementation - UART part
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
  Checks if UART is Hidden, and it's Pci Config space available

  @param[in]      UartNumber     Selects Serial IO UART device

  @retval   TRUE             UART is in hidden mode
  @retval   FALSE            UART is not in hidden mode
**/
BOOLEAN
IsSerialIoUartHidden (
  IN UINT8               UartNumber
  )
{
  if (MmioRead16 (GetSerialIoUartFixedPciCfgAddress (UartNumber) + PCI_DEVICE_ID_OFFSET) == GetSerialIoUartDeviceId (UartNumber)) {
    return TRUE;
  }
  return FALSE;
}

/**
  Configures Serial IO Controller before control is passd to the OS

  @param[in] UartNumber         UART Number
  @param[in] UartDeviceConfig   SerialIo UART Config

**/
VOID
SerialIoUartBootHandler (
  IN UINT8                      UartNumber,
  IN SERIAL_IO_UART_CONFIG      *UartDeviceConfig
  )
{
  UINT64   PciCfgBase;
  BOOLEAN  TurnOff;

  TurnOff = FALSE;

  //
  // Even if Uart is Hidden and in D3 SerialIoUartLib is capable of setting D0 during each write/read.
  // In case it is required for Os Debug DBG2 must be set to TRUE.
  //
  if (UartDeviceConfig->Mode == SerialIoUartPci || UartDeviceConfig->Mode == SerialIoUartHidden) {
    TurnOff = TRUE;
  }

  //
  // Uart in Com mode will be placed in D3 depending on PG configuration through ACPI _PS3
  //

  if ((UartDeviceConfig->Mode == SerialIoUartDisabled) && (SerialIoUartFuncNumber (UartNumber) == 0x0)) {
    if (SerialIoHigherFunctionsEnabled (SerialIoUartDevNumber (UartNumber))) {
      TurnOff = TRUE;
    }
  }

  if (UartDeviceConfig->DBG2 == TRUE) {
    TurnOff = FALSE;
  }

  if (TurnOff) {
    PciCfgBase = GetSerialIoUartPciCfg (UartNumber);
    SerialIoSetD3 (PciCfgBase);
    if ((UartDeviceConfig->Mode == SerialIoUartPci) || (UartDeviceConfig->Mode == SerialIoUartDisabled)) {
      SerialIoMmioDisable (PciCfgBase, TRUE);
    }
  }
}

/**
  Sets Pme Control Status and Command register values required for S3 Boot Script

  @param[in]     UartNumber         UART Number
  @param[in]     UartDeviceConfig   SerialIo UART Config
  @param[in/out] S3PciCfgBase       S3 Boot Script Pci Config Base
  @param[in/out] Command            Pci Command register data to save
  @param[in/out] Pme                Pci Pme Control register data to save

**/
VOID
SerialIoUartS3Handler (
  IN     UINT8                  UartNumber,
  IN     SERIAL_IO_UART_CONFIG  *UartDeviceConfig,
  IN OUT UINT64                 *S3PciCfgBase,
  IN OUT UINT32                 *Command,
  IN OUT UINT32                 *Pme
  )
{
  BOOLEAN  TurnOff;
  UINT64   PciCfgBase;

  *S3PciCfgBase = 0;
  TurnOff       = FALSE;

  if (UartDeviceConfig->Mode == SerialIoUartPci) {
    TurnOff = TRUE;
  }

  if ((UartDeviceConfig->Mode == SerialIoUartDisabled) && (SerialIoUartFuncNumber (UartNumber) == 0x0)) {
    if (SerialIoHigherFunctionsEnabled (SerialIoUartDevNumber (UartNumber))) {
      TurnOff = TRUE;
    }
  }

  if (TurnOff) {
    *S3PciCfgBase = GetSerialIoUartS3PciBase (UartNumber);
    PciCfgBase    = GetSerialIoUartPciCfg (UartNumber);
    *Pme          = PciSegmentRead32 ((UINTN) PciCfgBase + R_SERIAL_IO_CFG_PME_CTRL_STS);
    *Pme          = *Pme | BIT0 | BIT1;
    *Command      = PciSegmentRead32 ((UINTN) PciCfgBase + PCI_COMMAND_OFFSET);
    *Command      = *Command & (UINT32)~(EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);
  }
}
