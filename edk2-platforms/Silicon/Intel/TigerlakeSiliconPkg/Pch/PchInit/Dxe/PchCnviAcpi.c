/** @file
  Initializes PCH CNVi device ACPI data.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <CnviConfigHob.h>
#include "PchInit.h"
#include <Register/PchPcrRegs.h>

/**
  Update ASL definitions for CNVi device.

  @retval EFI_SUCCESS         The function completed successfully
**/
EFI_STATUS
UpdateCnviAcpiData (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS HobPtr;
  CNVI_CONFIG_HOB      *CnviConfigHob;

  DEBUG ((DEBUG_INFO, "UpdateCnviAcpiData() Start\n"));

  // Get CNVi Config HOB.
  HobPtr.Guid = GetFirstGuidHob (&gCnviConfigHobGuid);
  if (HobPtr.Guid != NULL) {
    CnviConfigHob = (CNVI_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr.Guid);
    mPchNvsAreaProtocol.Area->CnviMode           = (UINT8)CnviConfigHob->Mode;
    mPchNvsAreaProtocol.Area->CnviBtCore         = (UINT8)CnviConfigHob->BtCore;
    mPchNvsAreaProtocol.Area->CnviBtAudioOffload = (UINT8)CnviConfigHob->BtAudioOffload;
  }
  mPchNvsAreaProtocol.Area->CnviPortId = PID_CNVI;
  DEBUG ((DEBUG_INFO, "UpdateCnviAcpiData() End\n"));

  return EFI_SUCCESS;
}
