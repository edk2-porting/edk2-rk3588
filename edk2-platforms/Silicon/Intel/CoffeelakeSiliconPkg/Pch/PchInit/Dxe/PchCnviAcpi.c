/** @file
  Initializes PCH CNVi device ACPI data.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include "PchInit.h"
#include <Library/DxeSaPolicyLib.h>

/**
  Update ASL definitions for CNVi device.

  @retval EFI_SUCCESS         The function completed successfully
**/
EFI_STATUS
UpdateCnviAcpiData (
  VOID
  )
{

  DEBUG ((DEBUG_INFO, "UpdateCnviAcpiData() Start\n"));

  mPchNvsAreaProtocol.Area->CnviMode = (UINT8) mPchConfigHob->Cnvi.Mode;

  DEBUG ((DEBUG_INFO, "UpdateCnviAcpiData() End\n"));

  return EFI_SUCCESS;
}


