/** @file
  System Congfig Update.

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "SystemConfigUpdateDxe.h"
#include <PlatformInfoTypes.h>

EFI_PLATFORM_INFO        *mPlatformInfo = NULL;

/**

    Update the IioDefaults

    @param *SYSTEM_CONFIGURATION   Pointer to the SystemConfiguration structure

    @retval None

**/
VOID
IioDefaultConfigUpdateCallback (
  IN  SYSTEM_CONFIGURATION       *Default
  )
{
  UINT8                    BoardId;

  BoardId = mPlatformInfo->BoardId;
  Default->PlatformOCSupport = 0;
  if (BoardId == TypeHedtCRB) {
    Default->PlatformOCSupport = 1;
  }
}

SYSTEM_CONFIG_UPDATE_DATA SystemConfigUpdateTable = {
    SYSTEM_CONFIG_UPDATE_SIGNATURE,
    SYSTEM_CONFIG_UPDATE_VERSION,
    IioDefaultConfigUpdateCallback
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
SystemConfigUpdateEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
)
{
  EFI_STATUS                                Status;
  UBA_CONFIG_DATABASE_PROTOCOL              *UbaConfigProtocol = NULL;
  EFI_HOB_GUID_TYPE                         *GuidHob;

  GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT (GuidHob != NULL);
  mPlatformInfo  = GET_GUID_HOB_DATA(GuidHob);

  DEBUG((DEBUG_INFO, "UBA:System Config Update Table.\n"));
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
                                     &gSystemConfigUpdateDataGuid,
                                     &SystemConfigUpdateTable,
                                     sizeof(SystemConfigUpdateTable)
                                     );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return Status;
}