/** @file
  SiliconPolicyInitLib DXE.

  This driver initializes silicon policy with the defaults from the silicon provider.
  It publishes a protocol that is consumed by a shim library instance that provides the functions used by the
  common MinPlatformPkg PolicyInit late policy initialization code.

  @copyright
  Copyright 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DriverEntryPoint.h>
#include <Library/SiliconPolicyInitLib.h>
#include <Guid/SiliconPolicyInitLibInterface.h>

STATIC SILICON_POLICY_INIT_LIB_PROTOCOL mSiliconPolicyInitLibProtocol = {
  SiliconPolicyInitLate,
  SiliconPolicyDoneLate
};

/**
  Entry point function

  @param ImageHandle     - Handle for the image of this driver.
  @param SystemTable     - Pointer to the EFI System Table.

  @retval EFI_SUCCESS     - Protocol installed sucessfully.
**/
EFI_STATUS
EFIAPI
SiliconPolicyInitLateDxeEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS                      Status = EFI_SUCCESS;
  EFI_HANDLE                      Handle;

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (&Handle, &gSiliconPolicyInitLibInterfaceGuid, EFI_NATIVE_INTERFACE, &mSiliconPolicyInitLibProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return Status;
}
