/** @file
  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PciLib.h>

#include "PeiX58Ich10InitLib.h"
EFI_STATUS
EFIAPI
X58Ich10BoardInitBeforeSiliconInit (
  VOID
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
X58Ich10BoardInitAfterSiliconInit (
  VOID
  )
{

  DEBUG((EFI_D_ERROR, "X58Ich10BoardInitAfterSiliconInit\n"));
  return EFI_SUCCESS;
}
