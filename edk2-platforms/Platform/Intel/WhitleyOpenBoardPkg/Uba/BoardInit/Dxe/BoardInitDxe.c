/** @file
  BOARD INIT DXE Driver.

  @copyright
  Copyright 2014 - 2021 Intel Corporation.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "BoardInitDxe.h"
#include <PlatformInfoTypes.h>

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
BoardInitDxeDriverEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
)
{
  EFI_STATUS                              Status = EFI_SUCCESS;
  UBA_CONFIG_DATABASE_PROTOCOL           *UbaConfigProtocol = NULL;
  UINT32                                  PlatformType = 0;
  EFI_HANDLE                              Handle = NULL;

  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = UbaConfigProtocol->GetSku(
                         UbaConfigProtocol,
                         &PlatformType,
                         NULL,
                         NULL
                         );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "Uba init Dxe driver:PlatformType=%d\n", PlatformType));

  //according to the platform type to install different dummy maker.
  //later, the PEIM will be loaded by the dependency.
  switch(PlatformType)
  {
    case TypeWilsonCityRP:
      Status = gBS->InstallProtocolInterface (
            &Handle,
            &gEfiPlatformTypeWilsonCityRPProtocolGuid,
            EFI_NATIVE_INTERFACE,
            NULL
            );
      ASSERT_EFI_ERROR (Status);
      break;

    case TypeCooperCityRP:
      Status = gBS->InstallProtocolInterface (
        &Handle,
        &gEfiPlatformTypeCooperCityRPProtocolGuid,
        EFI_NATIVE_INTERFACE,
        NULL
        );
      ASSERT_EFI_ERROR (Status);
      break;

    default:
      // CAN'T GO TO HERE.
      ASSERT_EFI_ERROR (FALSE);
  }

  return Status;
}
