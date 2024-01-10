/** @file
  UART Serial Port library functions

  Copyright (c) 2006 - 2009, Intel Corporation
  Copyright (c) 2015 - 2016, Hisilicon Limited. All rights reserved.
  Copyright (c) 2015 - 2016, Linaro Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

  Based on the files under ArmPlatformPkg/Library/PL011SerialPortLib/
**/
#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>
#include <Library/IoLib.h>
#include <Protocol/SerialIo.h>

#include "Dw8250SerialPortLib.h"

/**
  Initialize the serial device hardware.

  If no initialization is required, then return RETURN_SUCCESS.
  If the serial device was successfuly initialized, then return RETURN_SUCCESS.
  If the serial device could not be initialized, then return RETURN_DEVICE_ERROR.

  @retval RETURN_SUCCESS        The serial device was initialized.
  @retval RETURN_DEVICE_ERROR   The serail device could not be initialized.

**/
RETURN_STATUS
EFIAPI
SerialPortInitialize (
  VOID
  )
{
  //
  // Assume already initialized.
  //
  return RETURN_SUCCESS;
}
