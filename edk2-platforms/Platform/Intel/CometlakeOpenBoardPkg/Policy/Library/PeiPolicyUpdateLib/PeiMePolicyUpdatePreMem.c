/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiMePolicyUpdate.h"
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>

/**
  Update the ME Policy Library

  @param[in] SiPreMemPolicyPpi  The pointer to SiPreMemPolicyPpi

  @retval EFI_SUCCESS           Update complete.
**/
EFI_STATUS
UpdatePeiMePolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                      Status;

  DEBUG ((DEBUG_INFO, "UpdatePeiMePolicyPreMem\n"));

  Status = EFI_SUCCESS;

  return Status;
}

