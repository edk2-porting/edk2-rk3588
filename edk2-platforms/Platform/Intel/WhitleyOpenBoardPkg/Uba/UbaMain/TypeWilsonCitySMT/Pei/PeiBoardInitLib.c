/** @file

 @copyright
  Copyright 2018 - 2021 Intel Corporation.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/


/**
  The constructor function for Board Init Libray.

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval  EFI_SUCCESS            Table initialization successfully.
  @retval  EFI_OUT_OF_RESOURCES   No enough memory to initialize table.
**/

#include "PeiBoardInit.h"
#include <UncoreCommonIncludes.h>
#include <Library/PchMultiPchBase.h>
#include <Ppi/DynamicSiLibraryPpi.h>

EFI_STATUS
EFIAPI
TypeWilsonCitySMTPeiBoardInitLibConstructor (
  IN EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS                      Status = EFI_SUCCESS;
  UBA_CONFIG_DATABASE_PPI         *UbaConfigPpi;
  EFI_HOB_GUID_TYPE               *GuidHob;
  EFI_PLATFORM_INFO               *PlatformInfo;
  UINT8                           SocketIndex;

  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  PlatformInfo  = GET_GUID_HOB_DATA(GuidHob);

  if (PlatformInfo->BoardId == TypeWilsonCitySMT) {

    DEBUG ((DEBUG_INFO, "PEI UBA init BoardId 0x%X: WilsonCitySMT\n", PlatformInfo->BoardId));

    Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = UbaConfigPpi->InitSku (
                       UbaConfigPpi,
                       PlatformInfo->BoardId,
                       NULL,
                       NULL
                       );
    ASSERT_EFI_ERROR (Status);

    Status = TypeWilsonCitySMTInstallGpioData (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = TypeWilsonCitySMTInstallPcdData (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = TypeWilsonCitySMTInstallSoftStrapData (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = TypeWilsonCitySMTPchEarlyUpdate (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = TypeWilsonCitySMTPlatformUpdateUsbOcMappings (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = TypeWilsonCitySMTInstallSlotTableData (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = TypeWilsonCitySMTInstallKtiEparamData (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    //
    // Set default memory type connector to DimmConnectorSmt
    //
    (*PeiServices)->SetMem (&PlatformInfo->MemoryConnectorType, sizeof (PlatformInfo->MemoryConnectorType), DimmConnectorSmt);

    //
    // Initialize InterposerType to InterposerUnknown
    //
    for (SocketIndex = 0; SocketIndex < MAX_SOCKET; ++SocketIndex) {
      PlatformInfo->InterposerType[SocketIndex] = InterposerUnknown;
    }

    //
    //  TypeWilsonCitySMTIioPortBifurcationInit will use PlatformInfo->InterposerType for PPO.
    //
    Status = TypeWilsonCitySMTIioPortBifurcationInit (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }
  return Status;
}
