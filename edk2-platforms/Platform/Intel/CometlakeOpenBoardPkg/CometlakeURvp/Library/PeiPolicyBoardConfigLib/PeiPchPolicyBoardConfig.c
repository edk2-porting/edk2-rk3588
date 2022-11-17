/** @file
 Intel PEI PCH Policy update by board configuration


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiPolicyBoardConfig.h"
#include <Library/ConfigBlockLib.h>

/**
  This function performs PEI PCH Policy update by board configuration.

  @param[in, out] SiPolicy        The SI Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyBoardConfig (
  IN OUT  SI_POLICY_PPI              *SiPolicyPpi
  )
{
  EFI_STATUS                         Status;
  PCH_GENERAL_CONFIG                 *PchGeneralConfig;

  DEBUG((DEBUG_INFO, "Updating PCH Policy by board config in Post Mem\n"));

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

