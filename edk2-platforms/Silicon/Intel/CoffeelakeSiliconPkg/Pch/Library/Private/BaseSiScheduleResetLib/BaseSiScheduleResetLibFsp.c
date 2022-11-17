/** @file
  Reset scheduling library services

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Uefi/UefiBaseType.h>
#include <Uefi.h>
#include <Pi/PiMultiPhase.h>
#include <Pi/PiPeiCis.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/HobLib.h>
#include <Private/Library/SiScheduleResetLib.h>
#include <Private/SiScheduleResetHob.h>

/**
  This function returns SiScheduleResetHob for library use
**/
SI_SCHEDULE_RESET_HOB *
SiScheduleGetResetData (
  VOID
  );

/**
  This function performs reset based on SiScheduleResetHob

  @retval     BOOLEAN       The function returns FALSE if no reset is required
**/
BOOLEAN
SiScheduleResetPerformReset (
  VOID
  )
{
  UINTN                 DataSize;
  SI_SCHEDULE_RESET_HOB *SiScheduleResetHob;

  if (!SiScheduleResetIsRequired ()) {
    return FALSE;
  }
  SiScheduleResetHob = SiScheduleGetResetData ();

  if (SiScheduleResetHob == NULL) {
    return TRUE;
  }

  DEBUG ((DEBUG_INFO, "SiScheduleResetPerformReset : Reset Type = 0x%x\n", SiScheduleResetHob->ResetType));
  if (SiScheduleResetHob->ResetType == EfiResetPlatformSpecific) {
    DataSize = sizeof (PCH_RESET_DATA);
    (*GetPeiServicesTablePointer ())->ResetSystem2 (SiScheduleResetHob->ResetType, EFI_SUCCESS, DataSize, &SiScheduleResetHob->ResetData);
  } else {
    (*GetPeiServicesTablePointer ())->ResetSystem2 (SiScheduleResetHob->ResetType, EFI_SUCCESS, 0, NULL);
  }
  //
  // Code should never reach here
  //
  ASSERT (FALSE);
  return TRUE;
}
