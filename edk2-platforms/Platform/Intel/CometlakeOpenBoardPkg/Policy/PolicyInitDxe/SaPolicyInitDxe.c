/** @file
  This file is SampleCode for SA DXE Policy initialization.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "SaPolicyInitDxe.h"


//
// Function implementations
//

/**
  Initialize SA DXE Policy

  @param[in] ImageHandle          Image handle of this driver.

  @retval EFI_SUCCESS             Initialization complete.
  @exception EFI_UNSUPPORTED      The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
SaPolicyInitDxe (
  IN EFI_HANDLE                   ImageHandle
  )
{
  EFI_STATUS               Status;
  SA_POLICY_PROTOCOL       *SaPolicy;

  //
  // Call CreateSaDxeConfigBlocks to create & initialize platform policy structure
  // and get all Intel default policy settings.
  //
  Status = CreateSaDxeConfigBlocks(&SaPolicy);
  DEBUG((DEBUG_INFO, "SaPolicy->TableHeader.NumberOfBlocks = 0x%x\n ", SaPolicy->TableHeader.NumberOfBlocks));
  ASSERT_EFI_ERROR(Status);

  UpdateDxeSaPolicyBoardConfig (SaPolicy);

  if (mFirmwareConfiguration != FwConfigDefault) {

    UpdateDxeSaPolicy (SaPolicy);
  }

  //
  // Install SaInstallPolicyProtocol.
  // While installed, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  Status = SaInstallPolicyProtocol (ImageHandle, SaPolicy);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

