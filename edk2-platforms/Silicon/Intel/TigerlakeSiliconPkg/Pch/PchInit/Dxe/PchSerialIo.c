/** @file
  Initializes Serial IO Controllers.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include "PchInit.h"
#include <Library/SerialIoPrivateLib.h>
#include <Library/PchInfoLib.h>

/**
  Calls Boot Os Hand off routine for each Serial IO Controller
**/
VOID
ConfigureSerialIoAtBoot (
  VOID
  )
{
  UINT8  Index;

  for (Index = 0; Index < GetPchMaxSerialIoSpiControllersNum (); Index++) {
    SerialIoSpiBootHandler (Index, &mPchConfigHob->SerialIo.SpiDeviceConfig[Index]);
  }

  for (Index = 0; Index < GetPchMaxSerialIoI2cControllersNum (); Index++) {
    SerialIoI2cBootHandler (Index, &mPchConfigHob->SerialIo.I2cDeviceConfig[Index]);
  }

  for (Index = 0; Index < GetPchMaxSerialIoUartControllersNum (); Index++) {
    SerialIoUartBootHandler (Index, &mPchConfigHob->SerialIo.UartDeviceConfig[Index]);
  }
}

