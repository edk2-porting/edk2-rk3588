/** @file
  This file contains sample for setting secureboot bypass bit
  in sample IBV HSTI structure. Actual test implementation is not present
  in this sample driver.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "HstiIbvPlatformDxe.h"

/**
  Sets verified bit for SecureBootBypass bit
  Actual test implementation is not present in this sample function.
**/
VOID
CheckSecureBootBypass (
  IN UINT32                   Role
  )
{
  EFI_STATUS  Status;
  BOOLEAN     Result;

  if ((mFeatureImplemented[2] & HSTI_BYTE2_SECUREBOOT_BYPASS_CHECKING) == 0) {
    return ;
  }

  Result = TRUE;
  DEBUG ((EFI_D_INFO, "  Secure Boot Bypass\n"));

  //
  // ALL PASS
  //
  if (Result) {
    Status = HstiLibSetFeaturesVerified (
               Role,
               NULL,
               2,
               HSTI_BYTE2_SECUREBOOT_BYPASS_CHECKING
               );
    ASSERT_EFI_ERROR (Status);
  }

}
