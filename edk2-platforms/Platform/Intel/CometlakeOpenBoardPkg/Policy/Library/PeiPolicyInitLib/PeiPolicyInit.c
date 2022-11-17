/** @file
  This file is SampleCode for Intel PEI Platform Policy initialization.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiPolicyInit.h"

/**
  Initialize Intel PEI Platform Policy

  @param[in] PeiServices            General purpose services available to every PEIM.
  @param[in] FirmwareConfiguration  It uses to skip specific policy init that depends
                                    on the 'FirmwareConfiguration' varaible.
**/
VOID
EFIAPI
PeiPolicyInit (
  IN UINT8                     FirmwareConfiguration
  )
{
  EFI_STATUS                   Status;
  SI_POLICY_PPI                *SiPolicyPpi;

  //
  // Call SiCreateConfigBlocks to initialize Silicon Policy structure
  // and get all Intel default policy settings.
  //
  Status = SiCreateConfigBlocks (&SiPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return;
  }

  if (PcdGetBool (PcdDumpDefaultSiliconPolicy)) {
    DEBUG ((DEBUG_INFO, "Dump Default Silicon Policy...\n"));
    DumpSiPolicy (SiPolicyPpi);
  }

  //
  // Update policy by board configuration
  //
  UpdatePeiSiPolicyBoardConfig (SiPolicyPpi);
  UpdatePeiPchPolicyBoardConfig (SiPolicyPpi);
  UpdatePeiSaPolicyBoardConfig (SiPolicyPpi);
  UpdatePeiCpuPolicyBoardConfig (SiPolicyPpi);
  UpdatePeiMePolicyBoardConfig (SiPolicyPpi);

  UpdatePeiSiPolicy(SiPolicyPpi);
  UpdatePeiPchPolicy(SiPolicyPpi);
  UpdatePeiSaPolicy(SiPolicyPpi);
  UpdatePeiCpuPolicy(SiPolicyPpi);
  UpdatePeiMePolicy(SiPolicyPpi);

  //
  // Install SiPolicyPpi.
  // While installed, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  Status = SiInstallPolicyPpi (SiPolicyPpi);
  ASSERT_EFI_ERROR (Status);
}

