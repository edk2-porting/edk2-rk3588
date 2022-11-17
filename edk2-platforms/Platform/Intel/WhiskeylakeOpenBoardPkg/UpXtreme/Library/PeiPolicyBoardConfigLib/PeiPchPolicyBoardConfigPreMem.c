/** @file
 Intel PEI PCH Pre-Memory Policy update by board configuration


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiPolicyBoardConfig.h"
#include <Library/ConfigBlockLib.h>

/**
  This function performs PEI PCH Pre-Memory Policy update by board configuration.

  @param[in, out] SiPolicy        The SI PreMem Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyBoardConfigPreMem (
  IN OUT   SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                         Status;
  PCH_GENERAL_PREMEM_CONFIG          *PchGeneralPreMemConfig;

  DEBUG((DEBUG_INFO, "Updating PCH Policy by board config in Pre Mem\n"));

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  return Status;
}


