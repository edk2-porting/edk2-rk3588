/** @file
 Intel PEI CPU Policy update by board configuration


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiPolicyBoardConfig.h"
#include <Library/ConfigBlockLib.h>

/**
  This function performs PEI CPU Policy update by board configuration.

  @param[in, out] SiPolicy        The SI Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicyBoardConfig (
  IN OUT  SI_POLICY_PPI              *SiPolicyPpi
  )
{
  EFI_STATUS                  Status;
  SA_MISC_PEI_PREMEM_CONFIG   *MiscPeiPreMemConfig;
  SI_PREMEM_POLICY_PPI        *SiPreMemPolicyPpi;
  CPU_CONFIG                  *CpuConfig;

  DEBUG((DEBUG_INFO, "Updating CPU Policy by board config in Post Mem\n"));

  Status = PeiServicesLocatePpi(
      &gSiPreMemPolicyPpiGuid,
      0,
      NULL,
      (VOID **)&SiPreMemPolicyPpi
      );
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gSaMiscPeiPreMemConfigGuid, (VOID *) &MiscPeiPreMemConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuConfigGuid, (VOID *) &CpuConfig);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

