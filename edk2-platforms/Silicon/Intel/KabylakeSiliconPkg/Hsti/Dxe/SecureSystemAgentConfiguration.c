/** @file
  This file contains the tests for the SecureSystemAgentConfiguration bit

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "HstiSiliconDxe.h"

/**
  Run tests for SecureSystemAgentConfiguration bit
**/
VOID
CheckSecureSystemAgentConfiguration (
  VOID
  )
{
  EFI_STATUS      Status;
  BOOLEAN         Result;
  UINT32          MchBar;

  if ((mFeatureImplemented[0] & HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION) == 0) {
    return;
  }

  Result = TRUE;
  MchBar = (UINT32) MmioRead64 (MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,SA_MC_DEV,SA_MC_FUN) + R_SA_MCHBAR) & B_SA_MCHBAR_MCHBAR_MASK;


  //
  // ALL PASS
  //
  if (Result) {
    Status = HstiLibSetFeaturesVerified (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               0,
               HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION
               );
    ASSERT_EFI_ERROR (Status);
  }

  return;
}
