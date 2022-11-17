/** @file
  This file contains the tests for the DebugModeDisabled Bit

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "HstiSiliconDxe.h"

/**
  Run tests for DebugModeDisabled bit
**/
VOID
CheckDebugModeDisabled (
  VOID
  )
{
  EFI_STATUS      Status;
  BOOLEAN         Result;
  UINT32          RegEcx;
  UINT64          DebugInterface;
  CHAR16          *HstiErrorString;

  if ((mFeatureImplemented[0] & HSTI_BYTE0_DEBUG_MODE_DISABLED_VERIFICATION) == 0) {
    return;
  }

  Result = TRUE;

  DEBUG ((DEBUG_INFO, "  Debug Interface check\n"));

  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &RegEcx, NULL);

  if ((RegEcx & BIT11) != 0) {

    DebugInterface =  AsmReadMsr64 (MSR_IA32_DEBUG_INTERFACE);

    DEBUG ((DEBUG_INFO, "    1. Disabled Check\n"));

    if ((DebugInterface & B_DEBUG_INTERFACE_ENABLE) != 0) {
      DEBUG ((DEBUG_INFO, "Fail: Debug Interface Enabled\n"));


      HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_DEBUG_MODE_DISABLED_VERIFICATION_ERROR_CODE_1 ,HSTI_CPU_SECURITY_CONFIGURATION, HSTI_BYTE0_DEBUG_MODE_DISABLED_VERIFICATION_ERROR_STRING_1);
      Status = HstiLibAppendErrorString (
                 PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
                 NULL,
                 HstiErrorString
                 );
      ASSERT_EFI_ERROR (Status);
      Result = FALSE;
      FreePool (HstiErrorString);
    }

    DEBUG ((DEBUG_INFO, "    2. Locked\n"));

    if ((DebugInterface & B_DEBUG_INTERFACE_LOCK) == 0) {
      DEBUG ((DEBUG_INFO, "Fail: Debug interface not locked\n"));


      HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_DEBUG_MODE_DISABLED_VERIFICATION_ERROR_CODE_2 ,HSTI_CPU_SECURITY_CONFIGURATION, HSTI_BYTE0_DEBUG_MODE_DISABLED_VERIFICATION_ERROR_STRING_2);
      Status = HstiLibAppendErrorString (
                 PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
                 NULL,
                 HstiErrorString
                 );
      ASSERT_EFI_ERROR (Status);
      Result = FALSE;
      FreePool (HstiErrorString);
    }
  }

  //
  // ALL PASS
  //
  if (Result) {
    Status = HstiLibSetFeaturesVerified (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               0,
               HSTI_BYTE0_DEBUG_MODE_DISABLED_VERIFICATION
               );
    ASSERT_EFI_ERROR (Status);
  }

  return;
}
