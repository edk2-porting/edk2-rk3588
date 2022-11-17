/** @file
  Kaby Lake RVP 3 Multi-Board Initialization Post-Memory library

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/BoardInitLib.h>
#include <Library/MultiBoardInitSupportLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

#include <PlatformBoardId.h>

EFI_STATUS
EFIAPI
KabylakeRvp3BoardInitBeforeSiliconInit (
  VOID
  );

BOARD_POST_MEM_INIT_FUNC  mKabylakeRvp3BoardInitFunc = {
  KabylakeRvp3BoardInitBeforeSiliconInit,
  NULL, // BoardInitAfterSiliconInit
};

EFI_STATUS
EFIAPI
PeiKabylakeRvp3MultiBoardInitLibConstructor (
  VOID
  )
{
  if ((LibPcdGetSku () == BoardIdKabyLakeYLpddr3Rvp3) || (LibPcdGetSku () == BoardIdSkylakeRvp3)) {
    return RegisterBoardPostMemInit (&mKabylakeRvp3BoardInitFunc);
  }
  return EFI_SUCCESS;
}
