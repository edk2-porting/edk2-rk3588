/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
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
MtOlympusBoardDetect (
  VOID
  )
{
  DEBUG ((EFI_D_INFO, "MtOlympusBoardDetect\n"));
  return EFI_SUCCESS;
}
