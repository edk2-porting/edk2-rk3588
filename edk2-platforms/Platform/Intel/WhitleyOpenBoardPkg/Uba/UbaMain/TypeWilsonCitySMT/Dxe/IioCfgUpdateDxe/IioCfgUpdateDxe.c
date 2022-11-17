/** @file
  IIO Config Update.

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "IioCfgUpdateDxe.h"

EFI_STATUS
UpdateWilsonCitySMTIioConfig (
  IN  IIO_GLOBALS             *IioGlobalData
  )
{
  return EFI_SUCCESS;
}

PLATFORM_IIO_CONFIG_UPDATE_TABLE  TypeWilsonCitySMTIioConfigTable =
{
  PLATFORM_IIO_CONFIG_UPDATE_SIGNATURE,
  PLATFORM_IIO_CONFIG_UPDATE_VERSION,

  IioBifurcationTable,
  sizeof(IioBifurcationTable),
  UpdateWilsonCitySMTIioConfig,
  IioSlotTable,
  sizeof(IioSlotTable)

};

/**
  The Driver Entry Point.

  The function is the driver Entry point.

  @param ImageHandle   A handle for the image that is initializing this driver
  @param SystemTable   A pointer to the EFI system table

  @retval EFI_SUCCESS:              Driver initialized successfully
  @retval EFI_LOAD_ERROR:           Failed to Initialize or has been loaded
  @retval EFI_OUT_OF_RESOURCES      Could not allocate needed resources

**/
EFI_STATUS
EFIAPI
IioCfgUpdateEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
)
{
  EFI_STATUS                               Status;
  UBA_CONFIG_DATABASE_PROTOCOL             *UbaConfigProtocol = NULL;

  DEBUG((DEBUG_INFO, "UBA:IioCfgUpdate-TypeWilsonCitySMT\n"));
  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = UbaConfigProtocol->AddData (
                                     UbaConfigProtocol,
                                     &gPlatformIioConfigDataDxeGuid,
                                     &TypeWilsonCitySMTIioConfigTable,
                                     sizeof(TypeWilsonCitySMTIioConfigTable)
                                     );

  Status = UbaConfigProtocol->AddData (
                                     UbaConfigProtocol,
                                     &gPlatformIioConfigDataDxeGuid_1,
                                     &TypeWilsonCitySMTIioConfigTable,
                                     sizeof(TypeWilsonCitySMTIioConfigTable)
                                     );

  Status = UbaConfigProtocol->AddData (
                                     UbaConfigProtocol,
                                     &gPlatformIioConfigDataDxeGuid_2,
                                     &TypeWilsonCitySMTIioConfigTable,
                                     sizeof(TypeWilsonCitySMTIioConfigTable)
                                     );

  Status = UbaConfigProtocol->AddData (
                                     UbaConfigProtocol,
                                     &gPlatformIioConfigDataDxeGuid_3,
                                     &TypeWilsonCitySMTIioConfigTable,
                                     sizeof(TypeWilsonCitySMTIioConfigTable)
                                     );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  return Status;
}
