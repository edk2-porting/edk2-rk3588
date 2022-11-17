/** @file
  Instance of Fsp Policy Initialization Library.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PeiFspPolicyInitLib.h>
#include <Library/FspWrapperApiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Ppi/FspmArchConfigPpi.h>

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
  DEBUG ((DEBUG_INFO, "FspmUpd - 0x%x\n", FspmUpd));
  FspPolicyInitPreMem ((FSPM_UPD *) FspmUpd);
  return FspmUpd;
}

/**
  SiPreMemInstallPolicyReadyPpi installs SiPreMemPolicyReadyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiPreMemInstallPolicyReadyPpi (
  VOID
  )
{
  EFI_STATUS             Status;
  EFI_PEI_PPI_DESCRIPTOR *SiPolicyReadyPreMemPpiDesc;

  SiPolicyReadyPreMemPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (SiPolicyReadyPreMemPpiDesc == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  SiPolicyReadyPreMemPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  SiPolicyReadyPreMemPpiDesc->Guid  = &gSiPreMemPolicyReadyPpiGuid;
  SiPolicyReadyPreMemPpiDesc->Ppi   = NULL;

  //
  // Install PreMem Silicon Policy Ready PPI
  //
  Status = PeiServicesInstallPpi (SiPolicyReadyPreMemPpiDesc);
  ASSERT_EFI_ERROR (Status);
  return Status;
}

RETURN_STATUS
EFIAPI
SiliconPolicyDonePreMem(
  IN VOID *FspmUpd
)
{
  EFI_STATUS                   Status;
#if FixedPcdGet8(PcdFspModeSelection) == 0
  FSPM_ARCH_CONFIG_PPI                  *FspmArchConfigPpi;
  EFI_PEI_PPI_DESCRIPTOR                *FspmArchConfigPpiDesc;

  FspmArchConfigPpi = (FSPM_ARCH_CONFIG_PPI *) AllocateZeroPool (sizeof (FSPM_ARCH_CONFIG_PPI));
  if (FspmArchConfigPpi == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }
  FspmArchConfigPpi->Revision            = 1;
  FspmArchConfigPpi->NvsBufferPtr        = NULL;
  FspmArchConfigPpi->BootLoaderTolumSize = 0;

  FspmArchConfigPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (FspmArchConfigPpiDesc == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }
  FspmArchConfigPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  FspmArchConfigPpiDesc->Guid  = &gFspmArchConfigPpiGuid;
  FspmArchConfigPpiDesc->Ppi   = FspmArchConfigPpi;
  //
  // Install FSP-M Arch Config PPI
  //
  Status = PeiServicesInstallPpi (FspmArchConfigPpiDesc);
  ASSERT_EFI_ERROR (Status);
#endif

  //
  // Install Policy Ready PPI
  // While installed, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  Status = SiPreMemInstallPolicyReadyPpi ();
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "Silicon PEI Policy Initialization Done in Pre-Memory\n"));

  return Status;
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
  DEBUG ((DEBUG_INFO, "FspsUpd - 0x%x\n", FspsUpd));
  FspPolicyInit ((FSPS_UPD *) FspsUpd);
  return FspsUpd;
}

/**
  SiInstallPolicyReadyPpi installs SiPolicyReadyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiInstallPolicyReadyPpi (
  VOID
  )
{
  EFI_STATUS             Status;
  EFI_PEI_PPI_DESCRIPTOR *SiPolicyReadyPpiDesc;

  SiPolicyReadyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (SiPolicyReadyPpiDesc == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  SiPolicyReadyPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  SiPolicyReadyPpiDesc->Guid  = &gSiPolicyReadyPpiGuid;
  SiPolicyReadyPpiDesc->Ppi   = NULL;

  //
  // Install Silicon Policy Ready PPI
  //
  Status = PeiServicesInstallPpi (SiPolicyReadyPpiDesc);
  ASSERT_EFI_ERROR (Status);
  return Status;
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
  SiInstallPolicyReadyPpi();
  return EFI_SUCCESS;
}

