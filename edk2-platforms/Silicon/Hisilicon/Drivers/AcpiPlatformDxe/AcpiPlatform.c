/** @file

  Copyright (c) 2014, Applied Micro Curcuit Corporation. All rights reserved.<BR>
  Copyright (c) 2015 - 2020, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2015, Linaro Limited. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <PiDxe.h>
#include <Guid/EventGroup.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/FirmwareVolume2.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/AcpiAml.h>
#include "UpdateDsdt.h"

EFI_EVENT       mUpdateAcpiDsdtTableEvent;

VOID
EFIAPI
UpdateAcpiDsdt (
  IN EFI_EVENT         Event,
  IN VOID              *Context
  )
{
  EFI_ACPI_TABLE_PROTOCOL *AcpiTableProtocol;
  EFI_STATUS              Status;

  Status = gBS->LocateProtocol (
                  &gEfiAcpiTableProtocolGuid,
                  NULL,
                  (VOID**)&AcpiTableProtocol
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, " Unable to locate ACPI table protocol\n"));
    return;
  }

  Status = UpdateAcpiDsdtTable ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, " UpdateAcpiDsdtTable Failed, Status = %r\n", Status));
  }

  gBS->CloseEvent (Event);
  return;
}

EFI_STATUS
EFIAPI
AcpiPlatformEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS Status;

  //
  // Register notify function
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  UpdateAcpiDsdt,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &mUpdateAcpiDsdtTableEvent
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Create ReadyToBoot event for UpdateAcpiDsdt failed.\n"));
  } else {
    DEBUG ((DEBUG_INFO, "Create ReadyToBoot event for UpdateAcpiDsdt success.\n"));
  }

  return Status;
}
