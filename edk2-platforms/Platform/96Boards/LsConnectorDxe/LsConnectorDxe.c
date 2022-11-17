/** @file

  Copyright (c) 2018, Linaro, Ltd. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HiiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/LsConnector.h>
#include <Protocol/Mezzanine.h>

#include "LsConnectorDxe.h"

extern  UINT8                     LsConnectorHiiBin[];
extern  UINT8                     LsConnectorDxeStrings[];

typedef struct {
  VENDOR_DEVICE_PATH              VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL        End;
} HII_VENDOR_DEVICE_PATH;

STATIC HII_VENDOR_DEVICE_PATH     m96BoardsDxeVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    NINETY_SIX_BOARDS_FORMSET_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),
      (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

STATIC LS_CONNECTOR_PROTOCOL  mLsConnector;
STATIC EFI_EVENT              EndOfDxeEvent;

STATIC
EFI_STATUS
InstallHiiPages (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_HII_HANDLE                  HiiHandle;
  EFI_HANDLE                      DriverHandle;

  DriverHandle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (&DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &m96BoardsDxeVendorDevicePath,
                  NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HiiHandle = HiiAddPackages (&g96BoardsFormsetGuid,
                              DriverHandle,
                              LsConnectorDxeStrings,
                              LsConnectorHiiBin,
                              NULL);

  if (HiiHandle == NULL) {
    gBS->UninstallMultipleProtocolInterfaces (DriverHandle,
           &gEfiDevicePathProtocolGuid,
           &m96BoardsDxeVendorDevicePath,
           NULL);
    return EFI_OUT_OF_RESOURCES;
  }
  return EFI_SUCCESS;
}

STATIC
VOID
EFIAPI
PublishOsDescription (
  EFI_EVENT           Event,
  VOID                *Context
  )
{
  VOID                    *Dtb;
  MEZZANINE_PROTOCOL      *Mezzanine;
  EFI_STATUS              Status;
  EFI_ACPI_TABLE_PROTOCOL *AcpiProtocol;

  Status = gBS->LocateProtocol (&g96BoardsMezzanineProtocolGuid, NULL,
                  (VOID **)&Mezzanine);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: no mezzanine driver active\n", __FUNCTION__));
    return;
  }

  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL,
                  (VOID **)&AcpiProtocol);
  if (!EFI_ERROR (Status)) {
    Status = Mezzanine->InstallSsdtTable (Mezzanine, AcpiProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a: failed to install SSDT table - %r\n",
        __FUNCTION__, Status));
    }
    return;
  }

  //
  // Find the DTB in the configuration table array. If it isn't there, just
  // bail without an error: the system may be able to proceed even without
  // ACPI or DT description, so it isn't up to us to complain about this.
  //
  Status = EfiGetSystemConfigurationTable (&gFdtTableGuid, &Dtb);
  if (Status == EFI_NOT_FOUND) {
    return;
  }
  ASSERT_EFI_ERROR (Status);

  Status = Mezzanine->ApplyDeviceTreeOverlay (Mezzanine, Dtb);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a: failed to apply DT overlay - %r\n", __FUNCTION__,
      Status));
  }
}

/**
  The entry point for 96BoardsDxe driver.

  @param[in] ImageHandle     The image handle of the driver.
  @param[in] SystemTable     The system table.

  @retval EFI_ALREADY_STARTED     The driver already exists in system.
  @retval EFI_OUT_OF_RESOURCES    Fail to execute entry point due to lack of
                                  resources.
  @retval EFI_SUCCES              All the related protocols are installed on
                                  the driver.

**/
EFI_STATUS
EFIAPI
EntryPoint (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS                      Status;
  NINETY_SIX_BOARDS_CONFIG_DATA   ConfigData;
  UINTN                           BufferSize;

  //
  // Get the current config settings from the EFI variable.
  //
  BufferSize = sizeof (ConfigData);
  Status = gRT->GetVariable (NINETY_SIX_BOARDS_CONFIG_VARIABLE_NAME,
                  &g96BoardsFormsetGuid, NULL, &BufferSize, &ConfigData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: no config data found\n", __FUNCTION__));
    ConfigData.MezzanineType = MEZZANINE_NONE;
  }

  if (!EFI_ERROR (Status) &&
      ConfigData.MezzanineType >= MEZZANINE_MAX) {
    DEBUG ((DEBUG_WARN,
      "%a: invalid value for %s, defaulting to MEZZANINE_NONE\n",
      __FUNCTION__, NINETY_SIX_BOARDS_CONFIG_VARIABLE_NAME));
    ConfigData.MezzanineType = MEZZANINE_NONE;
    Status = EFI_INVALID_PARAMETER; // trigger setvar below
  }

  //
  // Write the newly selected value back to the variable store.
  //
  if (EFI_ERROR (Status)) {
    ZeroMem (&ConfigData.Reserved, sizeof (ConfigData.Reserved));
    Status = gRT->SetVariable (NINETY_SIX_BOARDS_CONFIG_VARIABLE_NAME,
                    &g96BoardsFormsetGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof (ConfigData), &ConfigData);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: gRT->SetVariable () failed - %r\n",
        __FUNCTION__, Status));
      return Status;
    }
  }

  switch (ConfigData.MezzanineType) {
  case MEZZANINE_SECURE96:
    mLsConnector.MezzanineType = MezzanineSecure96;
    break;
  default:
    mLsConnector.MezzanineType = MezzanineUnknown;
  }

  Status = gBS->InstallProtocolInterface (&ImageHandle,
                  &g96BoardsLsConnectorProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mLsConnector);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  PublishOsDescription,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent);
  ASSERT_EFI_ERROR (Status);

  return InstallHiiPages ();
}
