/** @file
  UbaPcdUpdateLib implementation.

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Uefi/UefiSpec.h>

#include <Ppi/UbaCfgDb.h>
#include <Library/DebugLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/UbaPcdUpdateLib.h>

/**
  Function updates Platform Configuration Data (PCD) in Unified Board Abstraction (UBA)

  @param FileHandle      Handle of the file being invoked.
  @param PeiServices     Describes the list of possible PEI Services.

  @return EFI_SUCCESS    PCDs successfuly intialized
  @return EFI_ERROR      An error ocurs during PCDs initialization

**/
EFI_STATUS
PlatformUpdatePcds (
  VOID
  )
{
  EFI_STATUS                            Status;
  UBA_CONFIG_DATABASE_PPI               *UbaConfigPpi = NULL;
  PLATFORM_PCD_UPDATE_TABLE             PcdUpdateTable;
  UINTN                                 Size;

  Status = PeiServicesLocatePpi (
                                 &gUbaConfigDatabasePpiGuid,
                                 0,
                                 NULL,
                                 &UbaConfigPpi
                                 );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Size = sizeof(PcdUpdateTable);
  Status = UbaConfigPpi->GetData (
                                  UbaConfigPpi,
                                  &gPlatformPcdConfigDataGuid,
                                  &PcdUpdateTable,
                                  &Size
                                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ASSERT (PcdUpdateTable.Signature == PLATFORM_PCD_UPDATE_SIGNATURE);
  ASSERT (PcdUpdateTable.Version == PLATFORM_PCD_UPDATE_VERSION);

  Status = PcdUpdateTable.CallUpdate ();
  ASSERT_EFI_ERROR (Status);

  return Status;
}

