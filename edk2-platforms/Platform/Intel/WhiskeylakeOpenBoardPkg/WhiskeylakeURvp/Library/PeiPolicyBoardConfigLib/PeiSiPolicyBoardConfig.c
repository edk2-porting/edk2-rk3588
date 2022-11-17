/** @file
 Intel PEI SA Policy update by board configuration


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiPolicyBoardConfig.h"

/**
  This function performs PEI SI Policy update by board configuration.

  @param[in, out] SiPolicy        The SI Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiSiPolicyBoardConfig (
  IN OUT  SI_POLICY_PPI              *SiPolicyPpi
  )
{
  return EFI_SUCCESS;
}

