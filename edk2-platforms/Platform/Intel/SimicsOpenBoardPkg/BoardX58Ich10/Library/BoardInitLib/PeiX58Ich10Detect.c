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
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BoardInitLib.h>

EFI_STATUS
EFIAPI
X58Ich10BoardDetect (
  VOID
  )
{
  DEBUG ((EFI_D_INFO, "X58Ich10BoardDetect\n"));
  return EFI_SUCCESS;
}
