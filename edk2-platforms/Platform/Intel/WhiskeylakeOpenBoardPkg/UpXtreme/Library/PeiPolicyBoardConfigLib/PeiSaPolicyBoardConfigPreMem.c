/** @file
 Intel PEI SA Pre-Memory Policy update by board configuration


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiPolicyBoardConfig.h"
#include <ConfigBlock/SaMiscPeiPreMemConfig.h>
#include <Library/ConfigBlockLib.h>

/**
  This function performs PEI SA Pre-Memory Policy update by board configuration.

  @param[in, out] SiPolicy        The SI PreMem Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiSaPolicyBoardConfigPreMem (
  IN OUT   SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi
  )
{
  return EFI_SUCCESS;
}


