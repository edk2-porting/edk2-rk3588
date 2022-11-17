/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>
#include <Library/BoardInitLib.h>
#include <Library/PcdLib.h>

EFI_STATUS
EFIAPI
BoardAfterTempRamInit (
  VOID
  )
{
  return EFI_SUCCESS;
}
