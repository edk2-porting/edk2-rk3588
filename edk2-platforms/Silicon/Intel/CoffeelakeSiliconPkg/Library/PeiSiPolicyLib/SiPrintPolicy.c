/** @file
  This file is PeiSiPolicyLib library for printing Policy settings.

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiSiPolicyLibrary.h"
#include <Library/ConfigBlockLib.h>

/**
  Print whole SI_POLICY_PPI and serial out.

  @param[in] SiPolicyPpi The RC Policy PPI instance
**/
VOID
EFIAPI
SiPrintPolicyPpi (
  IN  SI_POLICY_PPI          *SiPolicyPpi
  )
{
  DEBUG_CODE_BEGIN ();
  SI_CONFIG     *SiConfig;
  EFI_STATUS    Status;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSiConfigGuid, (VOID *) &SiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------------ Silicon Print Policy Start ------------------------\n"));
  DEBUG ((DEBUG_INFO, " CsmFlag= %x\n", SiConfig->CsmFlag));
  DEBUG ((DEBUG_INFO, " TraceHubMemBase = 0x%08x\n", SiConfig->TraceHubMemBase));

  DEBUG ((DEBUG_INFO, "------------------------ Silicon Print Policy End --------------------------\n"));
  DEBUG_CODE_END ();
}

