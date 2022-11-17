/** @file
  Initializes Serial IO Controllers.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include "PchInit.h"
#include <Library/PchSerialIoLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegsSerialIo.h>

/**
  Puts all SerialIo controllers (except UARTs in debug mode) in D3
  Clears MemoryEnable for all PCI-mode controllers
**/
EFI_STATUS
ConfigureSerialIoAtBoot (
  VOID
  )
{
  PCH_SERIAL_IO_CONTROLLER Index;
  UINTN                    PciCfgBase;

  for (Index = 0; Index < PchSerialIoIndexMax; Index++) {
    if (mPchConfigHob->SerialIo.DevMode[Index] == PchSerialIoDisabled) {
      if (IsSerialIoFunctionZero (Index)) {
        if (IsSerialIoDeviceEnabled (GetSerialIoDeviceNumber (Index), GetSerialIoFunctionNumber (Index))) {
          PciCfgBase = FindSerialIoBar (Index,1);
          MmioOr32 (PciCfgBase + R_SERIAL_IO_CFG_PME_CTRL_STS, B_SERIAL_IO_CFG_PME_CTRL_STS_PWR_ST);
        }
      }
      continue;
    }
    if ((Index >= PchSerialIoIndexUart0) &&
        (mPchConfigHob->SerialIo.EnableDebugUartAfterPost) &&
        (mPchConfigHob->SerialIo.DebugUartNumber == (UINT32) (Index - PchSerialIoIndexUart0))) {
      continue;
    }
    PciCfgBase = FindSerialIoBar (Index,1);
    MmioOr32 (PciCfgBase + R_SERIAL_IO_CFG_PME_CTRL_STS, B_SERIAL_IO_CFG_PME_CTRL_STS_PWR_ST);
    MmioRead32 (PciCfgBase + R_SERIAL_IO_CFG_PME_CTRL_STS);
    if (mPchConfigHob->SerialIo.DevMode[Index] == PchSerialIoPci) {
      MmioAnd32 (PciCfgBase + PCI_COMMAND_OFFSET, (UINT32)~(EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER) );
      if (mPchConfigHob->SerialIo.DebugUartNumber == (UINT32) (Index - PchSerialIoIndexUart0)) {
        continue;
      }
      MmioWrite32 (PciCfgBase + R_SERIAL_IO_CFG_BAR0_LOW, 0);
      MmioWrite32 (PciCfgBase + R_SERIAL_IO_CFG_BAR0_HIGH, 0);
    }
  }
  return EFI_SUCCESS;
}

