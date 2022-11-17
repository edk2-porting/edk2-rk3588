/** @file
  Bds Hook Point callbacks DXE driver

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/BoardBdsHookLib.h>

#include <Protocol/PciEnumerationComplete.h>

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
BdsHookDxeEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_EVENT   BeforeConsoleAfterTrustedConsoleEvent;
  EFI_EVENT   BeforeConsoleBeforeEndOfDxeEvent;
  EFI_EVENT   AfterConsoleReadyBeforeBootOptionEvent;
  EFI_EVENT   ReadyToBootEvent;
  EFI_EVENT   PciEnumCompleteEvent;
  EFI_EVENT   SmmReadyToLockEvent;
  EFI_STATUS  Status;
  VOID        *Registration;

  DEBUG ((DEBUG_INFO, "%a starts\n", __FUNCTION__ ));

  //
  // Create event to set proper video resolution and text mode for internal shell.
  //
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             BdsReadyToBootCallback,
             NULL,
             &ReadyToBootEvent
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Create PCI Enumeration Completed callback for BDS
  //
  PciEnumCompleteEvent = EfiCreateProtocolNotifyEvent (
                           &gEfiPciEnumerationCompleteProtocolGuid,
                           TPL_CALLBACK,
                           BdsPciEnumCompleteCallback,
                           NULL,
                           &Registration
                           );
  ASSERT (PciEnumCompleteEvent != NULL);

  //
  // Create PCI Enumeration Completed callback for BDS
  //
  SmmReadyToLockEvent = EfiCreateProtocolNotifyEvent (
                          &gEfiDxeSmmReadyToLockProtocolGuid,
                          TPL_CALLBACK,
                          BdsSmmReadyToLockCallback,
                          NULL,
                          &Registration
                          );
  ASSERT (SmmReadyToLockEvent != NULL);

  //
  // Create BeforeConsoleAfterTrustedConsole event callback
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  BdsBeforeConsoleAfterTrustedConsoleCallback,
                  NULL,
                  &gBdsEventBeforeConsoleAfterTrustedConsoleGuid,
                  &BeforeConsoleAfterTrustedConsoleEvent
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Create BeforeConsoleBeforeEndOfDxeGuid event callback
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  BdsBeforeConsoleBeforeEndOfDxeGuidCallback,
                  NULL,
                  &gBdsEventBeforeConsoleBeforeEndOfDxeGuid,
                  &BeforeConsoleBeforeEndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Create AfterConsoleReadyBeforeBootOption event callback
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  BdsAfterConsoleReadyBeforeBootOptionCallback,
                  NULL,
                  &gBdsEventAfterConsoleReadyBeforeBootOptionGuid,
                  &AfterConsoleReadyBeforeBootOptionEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
