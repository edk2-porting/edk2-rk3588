/** @file
  Board Init PEIM.

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "BoardInitPei.h"

EFI_PEI_PPI_DESCRIPTOR     mPpiListBoardInit = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gBoardInitGuid,
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
BoardInitPeimEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
)
{
  EFI_STATUS                    Status;
  DEBUG ((DEBUG_INFO, "UBA :UbaMainPeimEntry!\n"));

  //
  // Inform board init ready, then peims which will use board init data such as
  // pcd data in uba database can take gBoardInitGuid as dependency.
  //
  Status = PeiServicesInstallPpi (&mPpiListBoardInit);
  if (Status != EFI_SUCCESS) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return EFI_SUCCESS;
}
