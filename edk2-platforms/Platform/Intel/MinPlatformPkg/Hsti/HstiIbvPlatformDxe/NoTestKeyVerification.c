/** @file
  This file contains sample for setting no test key bit
  in sample IBV HSTI structure. Actual test implementation is not present
  in this sample driver.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "HstiIbvPlatformDxe.h"

/**
  Sets the verified bit for NoTestKeyVerification.
  Actual test implementation is not present in this sample function.
**/
VOID
CheckNoTestKeyVerification (
  IN UINT32                   Role
  )
{
  EFI_STATUS      Status;
  BOOLEAN         Result;

  if ((mFeatureImplemented[2] & HSTI_BYTE2_NO_TEST_KEY_VERIFICATION) == 0) {
    return ;
  }

  Result = TRUE;
  DEBUG ((EFI_D_INFO, "  No Test Key Verification \n"));

  //
  // Get db and check Microsoft Test Key
  //

  //
  // ALL PASS
  //
  if (Result) {
    Status = HstiLibSetFeaturesVerified (
               Role,
               NULL,
               2,
               HSTI_BYTE2_NO_TEST_KEY_VERIFICATION
               );
    ASSERT_EFI_ERROR (Status);
  }

}
