/** @file
  Instance of Fsp Policy Initialization Library.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PeiFspPolicyInitLib.h>
#include <Library/FspWrapperApiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
/**
  Performs silicon pre-mem policy initialization.

  The meaning of Policy is defined by silicon code.
  It could be the raw data, a handle, a PPI, etc.

  The returned data must be used as input data for SiliconPolicyDonePreMem(),
  and SiliconPolicyUpdateLib.SiliconPolicyUpdatePreMem().

  1) In FSP path, the input Policy should be FspmUpd.
  Value of FspmUpd has been initialized by FSP binary default value.
  Only a subset of FspmUpd needs to be updated for different silicon sku.
  The return data is same FspmUpd.

  2) In non-FSP path, the input policy could be NULL.
  The return data is the initialized policy.

  @param[in, out] Policy       Pointer to policy.

  @return the initialized policy.
**/
VOID *
EFIAPI
SiliconPolicyInitPreMem (
  IN OUT VOID    *FspmUpd
  )
{
  FSPM_UPD              *FspmUpdDataPtr;
  EFI_STATUS            Status;
  UINTN                 *SourceData;
  FSP_INFO_HEADER       *FspmHeaderPtr;

  if (FspmUpd == NULL) {
    //
    // Allocate and initialize UPD buffer, copy default FSP-M UPD data
    //
    FspmHeaderPtr = (FSP_INFO_HEADER *) FspFindFspHeader (PcdGet32 (PcdFspmBaseAddress));
    ASSERT (FspmHeaderPtr != NULL);
    if (FspmHeaderPtr != NULL) {
      DEBUG ((DEBUG_INFO, "FspmHeaderPtr - 0x%x\n", FspmHeaderPtr));
      FspmUpdDataPtr = (FSPM_UPD *) AllocateZeroPool ((UINTN) FspmHeaderPtr->CfgRegionSize);
      ASSERT (FspmUpdDataPtr != NULL);
      if (FspmUpdDataPtr != NULL) {
        SourceData = (UINTN *) ((UINTN) FspmHeaderPtr->ImageBase + (UINTN) FspmHeaderPtr->CfgRegionOffset);
        CopyMem (FspmUpdDataPtr, SourceData, (UINTN) FspmHeaderPtr->CfgRegionSize);
        PcdSet32S (PcdFspmUpdDataAddress, (UINT32) FspmUpdDataPtr);
      }
    }
  } else {
    FspmUpdDataPtr = FspmUpd;
  }
  //
  // Return NULL pointer as error occurried and do not continue rest of the steps.
  //
  if (FspmUpdDataPtr == NULL) {
    return NULL;
  }
  DEBUG ((DEBUG_INFO, "FspmUpdDataPtr - 0x%x\n", FspmUpdDataPtr));
  //
  // Initialize Intel PEI Platform Policy
  //
  PeiPolicyInitPreMem ();

  //
  // SI Pei Fsp Policy Initialization
  //
  Status = PeiFspSiPolicyInitPreMem (FspmUpdDataPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ERROR - SI Pei Fsp Policy in Pre-Memory Initialization fail, Status = %r\n", Status));
  }

  //
  // PCH Pei Fsp Policy Initialization
  //
  Status = PeiFspPchPolicyInitPreMem (FspmUpdDataPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ERROR - PCH Pei Fsp Policy in Pre-Memory Initialization fail, Status = %r\n", Status));
  }

  //
  // Cpu Pei Fsp Policy Initialization
  //
  Status = PeiFspCpuPolicyInitPreMem (FspmUpdDataPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ERROR - CPU Pei Fsp Policy in Pre-Memory Initialization fail, Status = %r\n", Status));
  }


  //
  // ME Pei Fsp Policy Initialization
  //
  Status = PeiFspMePolicyInitPreMem (FspmUpdDataPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ERROR - ME Pei Fsp Policy in Pre-Memory Initialization fail, Status = %r\n", Status));
  }

  //
  // SystemAgent Pei Fsp Policy Initialization
  //
  Status = PeiFspSaPolicyInitPreMem (FspmUpdDataPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ERROR - SystemAgent Pei Fsp Policy in Pre-Memory Initialization fail, Status = %r\n", Status));
  }

  //
  // Other Upd Initialization
  //
  Status = PeiFspMiscUpdInitPreMem (FspmUpdDataPtr);

  return FspmUpdDataPtr;
}

/*
  The silicon pre-mem policy is finalized.
  Silicon code can do initialization based upon the policy data.

  The input Policy must be returned by SiliconPolicyInitPreMem().
  
  @param[in] Policy       Pointer to policy.

  @retval EFI_SUCCESS The policy is handled consumed by silicon code.
*/
EFI_STATUS
EFIAPI
SiliconPolicyDonePreMem (
  IN OUT VOID    *FspmUpd
  )
{
  return EFI_SUCCESS;
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
SiliconPolicyInitPostMem (
  IN OUT VOID    *FspsUpd
  )
{
  FSPS_UPD              *FspsUpdDataPtr;
  EFI_STATUS            Status;
  FSP_INFO_HEADER       *FspsHeaderPtr;
  UINTN                 *SourceData;

  if (FspsUpd == NULL) {
    //
    // Allocate and initialize UPD buffer, copy default FSP-S UPD data
    //
    FspsHeaderPtr = (FSP_INFO_HEADER *) FspFindFspHeader (PcdGet32 (PcdFspsBaseAddress));
    ASSERT (FspsHeaderPtr != NULL);
    if (FspsHeaderPtr != NULL) {
      DEBUG ((DEBUG_INFO, "FspsHeaderPtr - 0x%x\n", FspsHeaderPtr));
      FspsUpdDataPtr = (FSPS_UPD *) AllocateZeroPool ((UINTN) FspsHeaderPtr->CfgRegionSize);
      ASSERT (FspsUpdDataPtr != NULL);
      if (FspsUpdDataPtr != NULL) {
        SourceData = (UINTN *) ((UINTN) FspsHeaderPtr->ImageBase + (UINTN) FspsHeaderPtr->CfgRegionOffset);
        CopyMem (FspsUpdDataPtr, SourceData, (UINTN) FspsHeaderPtr->CfgRegionSize);
        PcdSet32S (PcdFspsUpdDataAddress, (UINT32) FspsUpdDataPtr);
      }
    }
  } else {
    FspsUpdDataPtr = FspsUpd;
  }
  //
  // Return NULL pointer as error occurried and do not continue rest of the steps.
  //
  if (FspsUpdDataPtr == NULL) {
    return NULL;
  }
  DEBUG ((DEBUG_INFO, "FspsUpdDataPtr - 0x%x\n", FspsUpdDataPtr));
  //
  // Initialize Intel PEI Platform Policy
  //
  PeiPolicyInit ();

  //
  // PCH Pei Fsp Policy Initialization
  //
  Status = PeiFspPchPolicyInit (FspsUpdDataPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ERROR - PCH Pei Fsp Policy iInitialization fail, Status = %r\n", Status));
  }

  //
  // ME Pei Fsp Policy Initialization
  //
  Status = PeiFspMePolicyInit (FspsUpdDataPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ERROR - ME Pei Fsp Policy Initialization fail, Status = %r\n", Status));
  }

  //
  // SystemAgent Pei Fsp Policy Initialization
  //
  Status = PeiFspSaPolicyInit (FspsUpdDataPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ERROR - SystemAgent Pei Fsp Policy Initialization fail, Status = %r\n", Status));
  }


  //
  // Cpu Pei Fsp Policy Initialization
  //
  Status = PeiFspCpuPolicyInit (FspsUpdDataPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ERROR - CPU Pei Fsp Policy Initialization fail, Status = %r\n", Status));
  }

  return FspsUpdDataPtr;
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
  IN OUT VOID    *FspsUpd
  )
{
  return EFI_SUCCESS;
}
