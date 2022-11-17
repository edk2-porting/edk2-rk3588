/** @file
  This file initialises and Installs GopPolicy Protocol.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Library/DebugLib.h>

EFI_STATUS
EFIAPI
CreatePchDxeConfigBlocks (
  IN OUT  VOID      **SaPolicy
  );

EFI_STATUS
EFIAPI
PchInstallPolicyProtocol (
  IN  EFI_HANDLE                  ImageHandle,
  IN  VOID                        *PchPolicy
  );

/**
  Initialize PCH DXE Policy

  @param[in] ImageHandle          Image handle of this driver.

  @retval EFI_SUCCESS             Initialization complete.
  @retval EFI_UNSUPPORTED         The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
PchPolicyInitDxe (
  IN EFI_HANDLE           ImageHandle
  )
{
  EFI_STATUS    Status;
  EFI_HANDLE    PchHandle;
  VOID          *PchPolicy;

  //
  // Call CreatePchDxeConfigBlocks to create & initialize platform policy structure
  // and get all Intel default policy settings.
  //
  Status = CreatePchDxeConfigBlocks (&PchPolicy);
  ASSERT_EFI_ERROR (Status);

  //
  // Install PchInstallPolicyProtocol.
  // While installed, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  PchHandle = NULL;
  Status = PchInstallPolicyProtocol (PchHandle, PchPolicy);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
