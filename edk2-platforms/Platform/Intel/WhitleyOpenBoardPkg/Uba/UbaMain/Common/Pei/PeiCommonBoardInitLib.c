/** @file
  Common Board Init Lib.

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/


/**
  The constructor function for Board Init Libray.

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval  EFI_SUCCESS            Table initialization successfully.
  @retval  EFI_OUT_OF_RESOURCES   No enough memory to initialize table.
**/

#include "PeiCommonBoardInitLib.h"

EFI_STATUS
EFIAPI
CommonPeiBoardInitLibConstructor (
  IN EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS                      Status;
  UBA_CONFIG_DATABASE_PPI         *UbaConfigPpi;


  Status = PeiServicesLocatePpi (
            &gUbaConfigDatabasePpiGuid,
            0,
            NULL,
            &UbaConfigPpi
            );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = InstallClockgenData (UbaConfigPpi);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = InstallGpioPlatformData (UbaConfigPpi);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = InstallBoardInfoData (UbaConfigPpi);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = InstallPlatformClocksConfigData (UbaConfigPpi);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = InstallPlatformHsioPtssTableData (UbaConfigPpi);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = InstallIioPortBifurcationInitData (UbaConfigPpi);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return Status;
}
