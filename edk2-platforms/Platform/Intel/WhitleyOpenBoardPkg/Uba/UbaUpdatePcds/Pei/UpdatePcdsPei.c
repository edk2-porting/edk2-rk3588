/** @file
  UBA PCDs update PEIM.

  @copyright
  Copyright 2014 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "UpdatePcdsPei.h"

EFI_PEI_PPI_DESCRIPTOR     mPpiListUpdatePcds = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gUpdatePcdGuid,
  NULL
};

/**
  Entry point function for the PEIM

  @param FileHandle      Handle of the file being invoked.
  @param PeiServices     Describes the list of possible PEI Services.

  @return EFI_SUCCESS    If we installed our PPI

**/
EFI_STATUS
EFIAPI
UpdatePcdPeimEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
)
{
  EFI_STATUS                            Status;

  Status = PlatformUpdatePcds();
  ASSERT_EFI_ERROR (Status);

  Status = PeiServicesInstallPpi (&mPpiListUpdatePcds);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
