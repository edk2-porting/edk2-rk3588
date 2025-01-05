/** @file
 *
 *  Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/ExitBootServicesOsNotify.h>

STATIC
EFI_STATUS
EFIAPI
GetTime (
  OUT EFI_TIME               *Time,
  OUT EFI_TIME_CAPABILITIES  *Capabilities
  )
{
  return EFI_UNSUPPORTED;
}

STATIC
EFI_STATUS
EFIAPI
SetTime (
  IN EFI_TIME  *Time
  )
{
  return EFI_UNSUPPORTED;
}

STATIC
EFI_STATUS
EFIAPI
GetWakeupTime (
  OUT BOOLEAN   *Enabled,
  OUT BOOLEAN   *Pending,
  OUT EFI_TIME  *Time
  )
{
  return EFI_UNSUPPORTED;
}

STATIC
EFI_STATUS
EFIAPI
SetWakeupTime (
  IN BOOLEAN    Enabled,
  OUT EFI_TIME  *Time
  )
{
  return EFI_UNSUPPORTED;
}

STATIC
VOID
EFIAPI
ExitBootServicesOsHandler (
  IN EXIT_BOOT_SERVICES_OS_CONTEXT  *Context
  )
{
  EFI_STATUS  Status;
  VOID        *Dtb;

  Status = EfiGetSystemConfigurationTable (&gFdtTableGuid, &Dtb);
  if (EFI_ERROR (Status) || (Dtb == NULL)) {
    return;
  }

  if (Context->OsType != ExitBootServicesOsLinux) {
    return;
  }

  //
  // The OS (i.e. Linux with FDT) most likely has a native driver
  // for the RTC peripheral. In this case, we must prevent UEFI
  // from accessing the hardware at runtime by disabling RTC services.
  //
  DEBUG ((DEBUG_ERROR, "RuntimeServicesManager: Disabling EFI RTC services\n"));
  gRT->GetTime       = GetTime;
  gRT->SetTime       = SetTime;
  gRT->GetWakeupTime = GetWakeupTime;
  gRT->SetWakeupTime = SetWakeupTime;
}

EFI_STATUS
EFIAPI
RuntimeServicesManagerDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                             Status;
  EXIT_BOOT_SERVICES_OS_NOTIFY_PROTOCOL  *ExitBootServicesOsNotify;

  Status = gBS->LocateProtocol (
                  &gExitBootServicesOsNotifyProtocolGuid,
                  NULL,
                  (VOID **)&ExitBootServicesOsNotify
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = ExitBootServicesOsNotify->RegisterHandler (
                                       ExitBootServicesOsNotify,
                                       ExitBootServicesOsHandler
                                       );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return EFI_SUCCESS;
}
