/** @file
  This file is SampleCode for Intel PEI Platform Policy initialization.

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "PeiPolicyInit.h"

/**
  Performs silicon post-mem policy initialization.

  The meaning of Policy is defined by silicon code.
  It could be the raw data, a handle, a PPI, etc.

  The returned data must be used as input data for SiliconPolicyDonePostMem(),
  and SiliconPolicyUpdateLib.SiliconPolicyUpdatePostMem().

  1) In FSP path, the input Policy should be FspsUpd.
  Value of FspsUpd has been initialized by FSP binary default value.
  Only a subset of FspsUpd needs to be updated for different silicon sku.
  The return data is same FspsUpd.

  2) In non-FSP path, the input policy could be NULL.
  The return data is the initialized policy.

  @param[in, out] Policy       Pointer to policy.

  @return the initialized policy.
**/
VOID *
EFIAPI
SiliconPolicyInitPostMem (
  IN OUT VOID *Policy
  )
{
  EFI_STATUS                            Status;
  SI_POLICY_PPI                         *SiPolicyPpi;
  PEI_SI_DEFAULT_POLICY_INIT_PPI        *PeiSiDefaultPolicyInitPpi;

  DEBUG ((DEBUG_INFO, "Silicon PEI Policy Initialization Start in Post-Memory...\n"));

  ASSERT (Policy == NULL);

  //
  // Locate Policy init PPI to install default silicon policy
  //
  Status = PeiServicesLocatePpi (
             &gSiDefaultPolicyInitPpiGuid,
             0,
             NULL,
             (VOID **) &PeiSiDefaultPolicyInitPpi
             );
  ASSERT_EFI_ERROR (Status);
  if (PeiSiDefaultPolicyInitPpi != NULL) {
    Status = PeiSiDefaultPolicyInitPpi->PeiPolicyInit ();
    ASSERT_EFI_ERROR (Status);
    if (Status == EFI_SUCCESS) {
      Status = PeiServicesLocatePpi (
                 &gSiPolicyPpiGuid,
                 0,
                 NULL,
                 (VOID **) &SiPolicyPpi
                 );
      ASSERT_EFI_ERROR (Status);
    }
  }

  return SiPolicyPpi;
}

/*
  The silicon post-mem policy is finalized.
  Silicon code can do initialization based upon the policy data.

  The input Policy must be returned by SiliconPolicyInitPostMem().
  
  @param[in] Policy       Pointer to policy.

  @retval EFI_SUCCESS The policy is handled consumed by silicon code.
*/
EFI_STATUS
EFIAPI
SiliconPolicyDonePostMem (
  IN VOID *Policy
  )
{
  EFI_STATUS                   Status;
  SI_POLICY_PPI                *SiPolicyPpi;

  SiPolicyPpi = Policy;

  //
  // Install SiPolicyPpi.
  // While installed, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  Status = SiInstallPolicyReadyPpi ();
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "Silicon PEI Policy Initialization Done in Post-Memory\n"));

  return Status;
}
