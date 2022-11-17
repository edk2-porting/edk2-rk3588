/** @file
  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <PiPei.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PciLib.h>
#include <Library/BoardInitLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>

#include "PeiX58Ich10InitLib.h"
#include <Register/X58Ich10.h>

EFI_STATUS
EFIAPI
X58Ich10BoardInitBeforeMemoryInit (
  VOID
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
X58Ich10BoardInitAfterMemoryInit (
  VOID
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
X58Ich10BoardDebugInit (
  VOID
  )
{
  return EFI_SUCCESS;
}

EFI_BOOT_MODE
EFIAPI
X58Ich10BoardBootModeDetect (
  VOID
  )
{
  EFI_BOOT_MODE BootMode = BOOT_WITH_FULL_CONFIGURATION;

  DEBUG((EFI_D_INFO, "modeValue = %x\n", IoBitFieldRead16(ICH10_PMBASE_IO + 4, 10, 12)));
  if (IoBitFieldRead16(ICH10_PMBASE_IO + 4, 10, 12) == 0x5) {
    BootMode = BOOT_ON_S3_RESUME;
  }

  return BootMode;
}
