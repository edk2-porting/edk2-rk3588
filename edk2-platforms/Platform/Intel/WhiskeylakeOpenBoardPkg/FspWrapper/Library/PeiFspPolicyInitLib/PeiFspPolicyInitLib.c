/** @file
  Instance of Fsp Policy Initialization Library.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PeiFspPolicyInitLib.h>
#include <Library/SpiLib.h>

VOID
EFIAPI
FspPolicyInitPreMem(
  IN FSPM_UPD           *FspmUpdDataPtr
);

VOID *
EFIAPI
SiliconPolicyInitPreMem(
  IN OUT VOID    *FspmUpd
)
{
  FspPolicyInitPreMem((FSPM_UPD *)FspmUpd);
  return FspmUpd;
}

RETURN_STATUS
EFIAPI
SiliconPolicyDonePreMem(
  IN VOID *FspmUpd
)
{
  EFI_STATUS         Status;

  Status = SpiServiceInit();
  ASSERT_EFI_ERROR(Status);

  return RETURN_SUCCESS;
}

/**
  Performs FSP PEI Policy Pre-memory initialization.

  @param[in] FspmUpdDataPtr       Pointer to FSPM UPD data.
**/
VOID
EFIAPI
FspPolicyInitPreMem (
  IN FSPM_UPD           *FspmUpdDataPtr
  )
{
  EFI_STATUS            Status;

  //
  // SI Pei Fsp Policy Initialization
  //
  Status = PeiFspSiPolicyInitPreMem (FspmUpdDataPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR - SI Pei Fsp Policy in Pre-Memory Initialization fail, Status = %r\n", Status));
  }

  //
  // PCH Pei Fsp Policy Initialization
  //
  Status = PeiFspPchPolicyInitPreMem (FspmUpdDataPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR - PCH Pei Fsp Policy in Pre-Memory Initialization fail, Status = %r\n", Status));
  }

  //
  // Cpu Pei Fsp Policy Initialization
  //
  Status = PeiFspCpuPolicyInitPreMem (FspmUpdDataPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR - CPU Pei Fsp Policy in Pre-Memory Initialization fail, Status = %r\n", Status));
  }

  //
  // Security Pei Fsp Policy Initialization
  //
  Status = PeiFspSecurityPolicyInitPreMem (FspmUpdDataPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR - Security Pei Fsp Policy in Pre-Memory Initialization fail, Status = %r\n", Status));
  }

  //
  // ME Pei Fsp Policy Initialization
  //
  Status = PeiFspMePolicyInitPreMem (FspmUpdDataPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR - ME Pei Fsp Policy in Pre-Memory Initialization fail, Status = %r\n", Status));
  }

  //
  // SystemAgent Pei Fsp Policy Initialization
  //
  Status = PeiFspSaPolicyInitPreMem (FspmUpdDataPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR - SystemAgent Pei Fsp Policy in Pre-Memory Initialization fail, Status = %r\n", Status));
  }

  //
  // Other Upd Initialization
  //
  Status = PeiFspMiscUpdInitPreMem (FspmUpdDataPtr);

}

/**
  Performs FSP PEI Policy initialization.

  @param[in][out] FspsUpd  Pointer UPD data region

**/
VOID
EFIAPI
FspPolicyInit (
  IN OUT FSPS_UPD    *FspsUpd
  )
{
  EFI_STATUS            Status;

  //
  // SI Pei Fsp Policy Initialization
  //
  Status = PeiFspSiPolicyInit (FspsUpd);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR - SI Pei Fsp Policy iInitialization fail, Status = %r\n", Status));
  }

  //
  // PCH Pei Fsp Policy Initialization
  //
  Status = PeiFspPchPolicyInit (FspsUpd);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR - PCH Pei Fsp Policy iInitialization fail, Status = %r\n", Status));
  }

  //
  // ME Pei Fsp Policy Initialization
  //
  Status = PeiFspMePolicyInit (FspsUpd);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR - ME Pei Fsp Policy Initialization fail, Status = %r\n", Status));
  }

  //
  // SystemAgent Pei Fsp Policy Initialization
  //
  Status = PeiFspSaPolicyInit (FspsUpd);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR - SystemAgent Pei Fsp Policy Initialization fail, Status = %r\n", Status));
  }

  //
  // Cpu Pei Fsp Policy Initialization
  //
  Status = PeiFspCpuPolicyInit (FspsUpd);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR - CPU Pei Fsp Policy Initialization fail, Status = %r\n", Status));
  }

  //
  // Security Pei Fsp Policy Initialization
  //
  Status = PeiFspSecurityPolicyInit(FspsUpd);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "ERROR - Security Pei Fsp Policy Initialization fail, Status = %r\n", Status));
  }

}

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
SiliconPolicyInitPostMem(
  IN OUT VOID    *FspsUpd
)
{
  FspPolicyInit((FSPS_UPD *)FspsUpd);
  return FspsUpd;
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
SiliconPolicyDonePostMem(
  IN OUT VOID    *FspsUpd
)
{
  return EFI_SUCCESS;
}

