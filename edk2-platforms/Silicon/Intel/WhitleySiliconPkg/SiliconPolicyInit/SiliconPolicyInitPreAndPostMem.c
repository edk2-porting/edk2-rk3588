/** @file
  SiliconPolicyInit PEIM.

  This PEIM initializes silicon policy with the defaults from the silicon provider.
  We publish a PPI that is consumed by a shim library instance that provides the functions used by the
  common MinPlatformPkg PolicyInit pre and post memory code.

  @copyright
  Copyright 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/SiliconPolicyInitLib.h>
#include <Guid/SiliconPolicyInitLibInterface.h>

STATIC SILICON_POLICY_INIT_LIB_PPI mSiliconPolicyInitLibPpi = {
  SiliconPolicyInitPreMem,
  SiliconPolicyDonePreMem,
  SiliconPolicyInitPostMem,
  SiliconPolicyDonePostMem
};

STATIC EFI_PEI_PPI_DESCRIPTOR mSiliconPolicyInitLibPpiDescriptor = {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gSiliconPolicyInitLibInterfaceGuid,
    &mSiliconPolicyInitLibPpi
  };

/**
  Entry point function for the PEIM

  @param FileHandle      Handle of the file being invoked.
  @param PeiServices     Describes the list of possible PEI Services.

  @return EFI_SUCCESS    If we installed our PPI
**/
EFI_STATUS
EFIAPI
SiliconPolicyInitPreAndPostMemPeimEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                      Status = EFI_SUCCESS;

  //
  // Just produce our PPI
  //
  Status = PeiServicesInstallPpi (&mSiliconPolicyInitLibPpiDescriptor);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "EntryPoint: failed to register PPI!\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return Status;
}
