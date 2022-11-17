/** @file
  This driver will register two callbacks to call fsp's notifies.

  Copyright (c) 2014 - 2016, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>

#include <Protocol/PciEnumerationComplete.h>

#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/BoardInitLib.h>
#include <Library/TestPointCheckLib.h>
#include <Library/PerformanceLib.h>
#include <Library/HobLib.h>

/**
  PciEnumerationComplete Protocol notification event handler.

  @param[in] Event    Event whose notification function is being invoked.
  @param[in] Context  Pointer to the notification function's context.
**/
VOID
EFIAPI
OnPciEnumerationComplete (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS          Status;
  VOID                *Interface;

  //
  // Try to locate it because gEfiPciEnumerationCompleteProtocolGuid will trigger it once when registration.
  // Just return if it is not found.
  //
  Status = gBS->LocateProtocol (
                  &gEfiPciEnumerationCompleteProtocolGuid,
                  NULL,
                  &Interface
                  );
  if (EFI_ERROR (Status)) {
    return ;
  }

  Status = BoardInitAfterPciEnumeration ();
  ASSERT_EFI_ERROR(Status);

  TestPointPciEnumerationDonePciBusMasterDisabled ();

  TestPointPciEnumerationDonePciResourceAllocated ();
}

/**
  This stage is notified when system exists platform manufacture authentication.

  @param  Event         Event whose notification function is being invoked.
  @param  Context       Pointer to the notification function's context, which is
                        always zero in current implementation.

**/
VOID
EFIAPI
OnEndOfDxe (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  gBS->CloseEvent (Event);

  TestPointEndOfDxeNoThirdPartyPciOptionRom ();

  TestPointEndOfDxeDmaAcpiTableFunctional ();

  TestPointEndOfDxeDmaProtectionEnabled ();
}

/**
  SmmReadyToLock Protocol notification event handler.

  @param[in] Event    Event whose notification function is being invoked.
  @param[in] Context  Pointer to the notification function's context.
**/
VOID
EFIAPI
OnDxeSmmReadyToLock (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS          Status;
  VOID                *Interface;

  //
  // Try to locate it because gEfiPciEnumerationCompleteProtocolGuid will trigger it once when registration.
  // Just return if it is not found.
  //
  Status = gBS->LocateProtocol (
                  &gEfiDxeSmmReadyToLockProtocolGuid,
                  NULL,
                  &Interface
                  );
  if (EFI_ERROR (Status)) {
    return ;
  }

  TestPointDxeSmmReadyToLockSmramAligned ();

  TestPointDxeSmmReadyToLockWsmtTableFunctional ();
}

/**
  Notification function of EVT_GROUP_READY_TO_BOOT event group.
  It runs after most ReadyToBoot event signaled.

  This is a notification function registered on EVT_GROUP_READY_TO_BOOT event group.
  When the Boot Manager is about to load and execute a boot option, it reclaims variable
  storage if free size is below the threshold.

  @param[in] Event        Event whose notification function is being invoked.
  @param[in] Context      Pointer to the notification function's context.

**/
VOID
EFIAPI
OnReadyToBootLater (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  DEBUG ((DEBUG_INFO, "OnReadyToBootLater\n"));

  gBS->CloseEvent (Event);

  TestPointReadyToBootMemoryTypeInformationFunctional ();
  TestPointReadyToBootUefiMemoryAttributeTableFunctional ();
  TestPointReadyToBootUefiBootVariableFunctional ();
  TestPointReadyToBootUefiConsoleVariableFunctional ();
  TestPointReadyToBootHstiTableFunctional ();
  TestPointReadyToBootAcpiTableFunctional ();
  TestPointReadyToBootGcdResourceFunctional ();

  TestPointDxeSmmReadyToBootSmiHandlerInstrument ();
  TestPointDxeSmmReadyToBootSmmPageProtection ();

  TestPointReadyToBootUefiSecureBootEnabled ();
  TestPointReadyToBootPiSignedFvBootEnabled ();
  TestPointReadyToBootTcgTrustedBootEnabled ();
  TestPointReadyToBootTcgMorEnabled ();
  TestPointReadyToBootEsrtTableFunctional ();
}

/**
  Notification function of EVT_GROUP_READY_TO_BOOT event group.

  This is a notification function registered on EVT_GROUP_READY_TO_BOOT event group.
  When the Boot Manager is about to load and execute a boot option, it reclaims variable
  storage if free size is below the threshold.

  @param[in] Event        Event whose notification function is being invoked.
  @param[in] Context      Pointer to the notification function's context.

**/
VOID
EFIAPI
OnReadyToBoot (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                        Status;
  EFI_EVENT                         ReadyToBootLaterEvent;

  DEBUG ((DEBUG_INFO, "OnReadyToBoot\n"));

  gBS->CloseEvent (Event);

  Status = BoardInitReadyToBoot ();
  ASSERT_EFI_ERROR(Status);

  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  OnReadyToBootLater,
                  NULL,
                  &ReadyToBootLaterEvent
                  );
  ASSERT_EFI_ERROR (Status);

  gBS->SignalEvent (ReadyToBootLaterEvent);
}

/**
  This stage is notified just before the firmware/Preboot environment transfers
  management of all system resources to the OS or next level execution environment.

  @param  Event         Event whose notification function is being invoked.
  @param  Context       Pointer to the notification function's context, which is
                        always zero in current implementation.

**/
VOID
EFIAPI
OnEndOfFirmware (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                        Status;

  gBS->CloseEvent (Event);

  Status = BoardInitEndOfFirmware ();
  ASSERT_EFI_ERROR(Status);

  TestPointExitBootServices ();
}

/**
  Main entry for the FSP DXE module.

  This routine registers two callbacks to call fsp's notifies.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
BoardNotificationInitEntryPoint (
  VOID
  )
{
  EFI_STATUS Status;
  EFI_EVENT  ReadyToBootEvent;
  VOID       *Registration;
  EFI_EVENT  ProtocolNotifyEvent;
  EFI_EVENT  ExitBootServicesEvent;
  EFI_EVENT  EndOfDxeEvent;
  VOID       *SmmReadyToLockRegistration;
  EFI_EVENT  SmmReadyToLockEvent;

  ProtocolNotifyEvent = EfiCreateProtocolNotifyEvent (
                          &gEfiPciEnumerationCompleteProtocolGuid,
                          TPL_CALLBACK,
                          OnPciEnumerationComplete,
                          NULL,
                          &Registration
                          );
  ASSERT (ProtocolNotifyEvent != NULL);

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  OnEndOfDxe,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  SmmReadyToLockEvent = EfiCreateProtocolNotifyEvent  (
                          &gEfiDxeSmmReadyToLockProtocolGuid,
                          TPL_CALLBACK,
                          OnDxeSmmReadyToLock,
                          NULL,
                          &SmmReadyToLockRegistration
                          );
  ASSERT (SmmReadyToLockEvent != NULL);

  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             OnReadyToBoot,
             NULL,
             &ReadyToBootEvent
             );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  OnEndOfFirmware,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &ExitBootServicesEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
  Initialize  DXE Platform.

  @param[in] ImageHandle       Image handle of this driver.
  @param[in] SystemTable       Global system service table.

  @retval EFI_SUCCESS           Initialization complete.
  @exception EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
PlatformInitDxeEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  BoardNotificationInitEntryPoint ();

  return EFI_SUCCESS;
}