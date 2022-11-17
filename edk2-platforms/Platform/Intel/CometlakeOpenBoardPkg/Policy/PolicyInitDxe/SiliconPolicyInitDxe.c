/** @file
  This file is SampleCode for Intel Silicon DXE Platform Policy initialzation.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <SiliconPolicyInitDxe.h>
#include <Library/BaseLib.h>

DXE_SI_POLICY_PROTOCOL mSiPolicyData  = { 0 };

/**
  Initilize Intel Cpu DXE Platform Policy

  @param[in] ImageHandle        Image handle of this driver.

  @retval EFI_SUCCESS           Initialization complete.
  @exception EFI_UNSUPPORTED    The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
SiliconPolicyInitDxe (
  IN EFI_HANDLE       ImageHandle
  )
{
  EFI_STATUS Status;

  mSiPolicyData.Revision                         = DXE_SI_POLICY_PROTOCOL_REVISION;

  ///
  /// Install the DXE_SI_POLICY_PROTOCOL interface
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gDxeSiPolicyProtocolGuid,
                  &mSiPolicyData,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

