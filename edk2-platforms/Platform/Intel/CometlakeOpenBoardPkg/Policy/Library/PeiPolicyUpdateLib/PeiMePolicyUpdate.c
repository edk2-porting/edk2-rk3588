/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiMePolicyUpdate.h"
#include <ConfigBlock/MePeiConfig.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PmcLib.h>

/**
  Update the ME Policy Library

  @param[in, out] SiPolicyPpi     The pointer to SiPolicyPpi

  @retval EFI_SUCCESS             Update complete.
**/
EFI_STATUS
UpdatePeiMePolicy (
  IN OUT SI_POLICY_PPI            *SiPolicyPpi
  )
{
  EFI_STATUS                      Status;
  ME_PEI_CONFIG                   *MePeiConfig;

  DEBUG ((DEBUG_INFO, "UpdatePeiMePolicy\n"));

  Status = EFI_SUCCESS;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gMePeiConfigGuid, (VOID *) &MePeiConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (!PmcIsRtcBatteryGood ()) {
    //
    // For non coin battery design, this can be skipped.
    //
    MePeiConfig->MeUnconfigOnRtcClear   = 2;
  }

  return Status;
}


