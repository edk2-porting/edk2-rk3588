/** @file
  This file is SampleCode for Intel PEI Platform Policy initialization.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiPolicyInit.h"

/**
  Initialize Intel PEI Platform Policy

  @param[in]  FirmwareConfiguration  It uses to skip specific policy init that depends
                                     on the 'FirmwareConfiguration' varaible.
**/
VOID
EFIAPI
PeiPolicyInitPreMem (
  IN UINT8                     FirmwareConfiguration
  )
{
  EFI_STATUS                   Status;
  SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi;

  DEBUG ((DEBUG_INFO, "Silicon PEI Policy Initialization Start in Pre-Memory...\n"));
  //
  // Call SiCreatePreMemConfigBlocks to initialize platform policy structure
  // and get all intel default policy settings.
  //
  Status = SiCreatePreMemConfigBlocks (&SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  //
  // Update policy by board configuration
  //
  UpdatePeiPchPolicyBoardConfigPreMem (SiPreMemPolicyPpi);
  UpdatePeiMePolicyBoardConfigPreMem (SiPreMemPolicyPpi);
  UpdatePeiSaPolicyBoardConfigPreMem (SiPreMemPolicyPpi);
  UpdatePeiCpuPolicyBoardConfigPreMem (SiPreMemPolicyPpi);

  //
  // Update and override all platform related and customized settings below.
  //
  UpdatePeiPchPolicyPreMem (SiPreMemPolicyPpi);
  UpdatePeiMePolicyPreMem (SiPreMemPolicyPpi);
  UpdatePeiSaPolicyPreMem (SiPreMemPolicyPpi);
  UpdatePeiCpuPolicyPreMem (SiPreMemPolicyPpi);

  //
  // Install SiPreMemPolicyPpi.
  // While installed, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  Status = SiPreMemInstallPolicyPpi (SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "Silicon PEI Policy Initialization Done in Pre-Memory\n"));
}

