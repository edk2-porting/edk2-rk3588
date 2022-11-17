/** @file
  Implementation of Fsp CPU Policy Initialization.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PeiFspPolicyInitLib.h>

#include <Ppi/SiPolicy.h>
#include <Ppi/SecPlatformInformation2.h>

#include <CpuAccess.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PcdLib.h>
#include <FspEas.h>
#include <PolicyUpdateMacro.h>

/**
  Performs FSP CPU PEI Policy initialization.

  @param[in][out]  FspmUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspCpuPolicyInitPreMem (
  IN OUT FSPM_UPD    *FspmUpd
  )
{
  CPU_CONFIG_LIB_PREMEM_CONFIG    *CpuConfigLibPreMemConfig;
  CPU_SECURITY_PREMEM_CONFIG      *CpuSecurityPreMemConfig;
#if FixedPcdGet8(PcdFspModeSelection) == 0
  EFI_STATUS                      Status;
  SI_PREMEM_POLICY_PPI            *SiPreMemPolicyPpi;
  SiPreMemPolicyPpi = NULL;
#endif

  CpuConfigLibPreMemConfig = NULL;
  CpuSecurityPreMemConfig = NULL;
  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP Update SiCpuPolicy Pre-Mem Start\n"));

#if FixedPcdGet8(PcdFspModeSelection) == 0
  //
  // Locate SiPreMemPolicyPpi
  //
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuConfigLibPreMemConfigGuid, (VOID *) &CpuConfigLibPreMemConfig);
  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP Update SiCpuPolicy Pre-Mem End\n"));
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuSecurityPreMemConfigGuid, (VOID *) &CpuSecurityPreMemConfig);
  ASSERT_EFI_ERROR(Status);
#endif
  //
  //  Cpu Config Lib policies
  //
  UPDATE_POLICY (FspmUpd->FspmConfig.CpuRatio, CpuConfigLibPreMemConfig->CpuRatio, 0);
  DEBUG ((DEBUG_INFO, "BIOS Guard PCD and Policy are disabled\n"));
  UPDATE_POLICY (FspmUpd->FspmConfig.BiosGuard, CpuSecurityPreMemConfig->BiosGuard, 0);
  UPDATE_POLICY (FspmUpd->FspmConfig.PrmrrSize, CpuSecurityPreMemConfig->PrmrrSize, SIZE_1MB);
  UPDATE_POLICY (FspmUpd->FspmConfig.EnableC6Dram, CpuSecurityPreMemConfig->EnableC6Dram, 1);

  return EFI_SUCCESS;
}
