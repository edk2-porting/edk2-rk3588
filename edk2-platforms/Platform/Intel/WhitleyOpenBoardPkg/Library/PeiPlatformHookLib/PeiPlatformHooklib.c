/** @file
  PEI Library Functions. Initialize GPIOs

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Guid/PlatformInfo.h>
#include <Library/DebugLib.h>
#include <Library/UbaGpioInitLib.h>
#include <Library/PeiPlatformHooklib.h>

/**
  Configure GPIO

  @param[in]  PlatformInfo
**/
VOID
GpioInit (
)
{
  EFI_STATUS                  Status;
  Status = PlatformInitGpios();
}


/**
  Configure GPIO and SIO

  @retval  EFI_SUCCESS   Operation success.
**/
EFI_STATUS
BoardInit (
  )
{

  GpioInit();

  return EFI_SUCCESS;
}
