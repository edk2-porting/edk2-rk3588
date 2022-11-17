/** @file
  This file is SampleCode for PCH DXE Policy initialization.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PchPolicyInitDxe.h"

//
// Function implementations
//

/**
  Initialize PCH DXE Policy

  @param[in] ImageHandle          Image handle of this driver.

  @retval EFI_SUCCESS             Initialization complete.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
PchPolicyInitDxe (
  IN EFI_HANDLE                   ImageHandle
  )
{
  EFI_STATUS               Status;
  PCH_POLICY_PROTOCOL      *PchPolicy;

  //
  // Call CreatePchDxeConfigBlocks to create & initialize platform policy structure
  // and get all Intel default policy settings.
  //
  Status = CreatePchDxeConfigBlocks (&PchPolicy);
  DEBUG((DEBUG_INFO, "PchPolicy->TableHeader.NumberOfBlocks = 0x%x\n", PchPolicy->TableHeader.NumberOfBlocks));
  ASSERT_EFI_ERROR (Status);

  if (mFirmwareConfiguration != FwConfigDefault) {
    UpdateDxePchPolicy (PchPolicy);
  }

  //
  // Install PchInstallPolicyProtocol.
  // While installed, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  Status = PchInstallPolicyProtocol (ImageHandle, PchPolicy);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

