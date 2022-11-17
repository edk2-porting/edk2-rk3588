/** @file
  Reset scheduling library services

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Uefi/UefiBaseType.h>
#include <Uefi.h>
#include <Pi/PiMultiPhase.h>
#include <Library/HobLib.h>
#include <Private/SiScheduleResetHob.h>

/**
  This function returns SiScheduleResetHob for library use
**/
SI_SCHEDULE_RESET_HOB *
SiScheduleGetResetData (
  VOID
  )
{
  STATIC SI_SCHEDULE_RESET_HOB *SiScheduleResetHob = NULL;
  SI_SCHEDULE_RESET_HOB        *SiScheduleResetHobTemp;
  VOID                         *HobPtr;

  if (SiScheduleResetHob != NULL) {
    return SiScheduleResetHob;
  }

  HobPtr = GetFirstGuidHob (&gSiScheduleResetHobGuid);
  if (HobPtr == NULL) {
    SiScheduleResetHobTemp = BuildGuidHob (&gSiScheduleResetHobGuid, sizeof (SI_SCHEDULE_RESET_HOB));
    if (SiScheduleResetHobTemp == NULL) {
      ASSERT (FALSE);
      return SiScheduleResetHobTemp;
    }
    SiScheduleResetHobTemp->ResetType = 0xFF;
    DEBUG ((DEBUG_INFO, "SiScheduleResetSetType : Init SiScheduleResetHob\n"));
  } else {
    SiScheduleResetHobTemp = (SI_SCHEDULE_RESET_HOB*) GET_GUID_HOB_DATA (HobPtr);
  }
  SiScheduleResetHob = SiScheduleResetHobTemp;
  return SiScheduleResetHobTemp;
}

/**
  This function updates the reset information in SiScheduleResetHob
  @param[in] ResetType        UEFI defined reset type.
  @param[in] ResetData        Optional element used to introduce a platform specific reset.
                               The exact type of the reset is defined by the EFI_GUID that follows
                               the Null-terminated Unicode string.
**/
VOID
SiScheduleResetSetType (
  IN EFI_RESET_TYPE     ResetType,
  IN PCH_RESET_DATA     *ResetData OPTIONAL
  )
{
  SI_SCHEDULE_RESET_HOB *SiScheduleResetHob;
  if (ResetType > EfiResetPlatformSpecific) {
    DEBUG ((DEBUG_INFO, "Unsupported Reset Type Requested\n"));
    return;
  }
  SiScheduleResetHob = SiScheduleGetResetData ();
  if (SiScheduleResetHob == NULL) {
    return;
  }
  DEBUG ((DEBUG_INFO, "SiScheduleResetSetType : Current Reset Type = 0x%x\n", SiScheduleResetHob->ResetType));
  if (SiScheduleResetHob->ResetType == ResetType) {
    DEBUG ((DEBUG_INFO, "Current Reset Type is same as requested Reset Type\n"));
    return;
  }
  if (SiScheduleResetHob->ResetType == 0xFF) {
    //
    // Init Reset Type to lowest ResetType
  //
    SiScheduleResetHob->ResetType = EfiResetWarm;
  }
  //
  // ResetType Priority set as : ResetPlatformSpecific(3) > ResetShutdown(2) > ResetCold(0) > ResetWarm(1)
  //
  switch (ResetType) {
    case EfiResetWarm:
      break;

    case EfiResetCold:
      if (SiScheduleResetHob->ResetType == EfiResetWarm) {
        SiScheduleResetHob->ResetType = ResetType;
      }
      break;

    case EfiResetShutdown:
      if (SiScheduleResetHob->ResetType < ResetType)
      SiScheduleResetHob->ResetType = ResetType;
      break;

    case EfiResetPlatformSpecific:
      SiScheduleResetHob->ResetType = ResetType;
      SiScheduleResetHob->ResetData = *ResetData;
      break;
  }
  DEBUG ((DEBUG_INFO, "SiScheduleResetSetType : New Reset Type = 0x%x\n", SiScheduleResetHob->ResetType));
}

/**
  This function returns TRUE or FALSE depending on whether a reset is required based on SiScheduleResetHob

  @retval     BOOLEAN       The function returns FALSE if no reset is required
**/
BOOLEAN
SiScheduleResetIsRequired (
  VOID
  )
{
  VOID                  *HobPtr;

  HobPtr = NULL;
  HobPtr = GetFirstGuidHob (&gSiScheduleResetHobGuid);
  if (HobPtr == NULL) {
    return FALSE;
  }
  return TRUE;
}
