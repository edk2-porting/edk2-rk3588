/** @file
  This file contains sample for setting MOR support bit
  in sample IBV HSTI structure. Actual test implementation is not present
  in this sample driver.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "HstiIbvPlatformDxe.h"

/**
  Sets the verified bit for MorSupport.
  Actual test implementation is not present in this sample function.
**/
VOID
CheckMorSupport (
  IN UINT32                   Role
  )
{
  EFI_STATUS        Status;
  BOOLEAN           Result;

  if ((mFeatureImplemented[2] & HSTI_BYTE2_MOR_SUPPORT) == 0) {
    return ;
  }

  Result = TRUE;

  DEBUG ((EFI_D_INFO, "  MOR Strength\n"));

  //
  // ALL PASS
  //
  if (Result) {
    Status = HstiLibSetFeaturesVerified (
               Role,
               NULL,
               2,
               HSTI_BYTE2_MOR_SUPPORT
               );
    ASSERT_EFI_ERROR (Status);
  }


  return ;
}
