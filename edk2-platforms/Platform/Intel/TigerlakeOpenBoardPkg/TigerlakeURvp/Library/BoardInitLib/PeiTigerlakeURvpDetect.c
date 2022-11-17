/** @file

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

#include <PlatformBoardId.h>

BOOLEAN
TigerlakeURvp(
  VOID
  )
{
  return TRUE;
}

EFI_STATUS
EFIAPI
TigerlakeURvpBoardDetect (
  VOID
  )
{
  if (LibPcdGetSku () != 0) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "TigerLakeU Board Detection Callback\n"));

  if (TigerlakeURvp ()) {
    LibPcdSetSku (SkuIdTglU);
    DEBUG ((DEBUG_INFO, "SKU_ID: 0x%x\n", LibPcdGetSku()));
    ASSERT (LibPcdGetSku() == SkuIdTglU);
  }
  return EFI_SUCCESS;
}
