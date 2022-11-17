/** @file
  This file include all platform action which can be customized by IBV/OEM.

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "BdsPlatform.h"

extern UINTN                                   mBootMenuOptionNumber;

VOID
ExitPmAuth (
  VOID
  )
{
  EFI_HANDLE                 Handle;
  EFI_STATUS                 Status;
  EFI_EVENT                  EndOfDxeEvent;

  DEBUG((DEBUG_INFO,"ExitPmAuth ()- Start\n"));
  //
  // Prepare S3 information, this MUST be done before ExitPmAuth/EndOfDxe
  //
  //
  // Since PI1.2.1, we need signal EndOfDxe as ExitPmAuth
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  EfiEventEmptyFunction,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);
  gBS->SignalEvent (EndOfDxeEvent);
  gBS->CloseEvent (EndOfDxeEvent);
  DEBUG((DEBUG_INFO,"All EndOfDxe callbacks have returned successfully\n"));

  //
  // NOTE: We need install DxeSmmReadyToLock directly here because many boot script is added via ExitPmAuth/EndOfDxe callback.
  // If we install them at same callback, these boot script will be rejected because BootScript Driver runs first to lock them done.
  // So we seperate them to be 2 different events, ExitPmAuth is last chance to let platform add boot script. DxeSmmReadyToLock will
  // make boot script save driver lock down the interface.
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEfiDxeSmmReadyToLockProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
  DEBUG((DEBUG_INFO,"ExitPmAuth ()- End\n"));
}


/**
  Creates an EFI event in the BDS Event Group.

  @param  NotifyTpl         The task priority level of the event.
  @param  gEfiEventGuid     The GUID of the event group to signal.
  @param  BdsConsoleEvent   Returns the EFI event returned from gBS->CreateEvent(Ex).

  @retval EFI_SUCCESS       Event was created.
  @retval Other             Event was not created.

**/
EFI_STATUS
EFIAPI
CreateBdsEvent (
  IN  EFI_TPL           NotifyTpl,
  IN  EFI_GUID          *gEfiEventGuid,
  OUT EFI_EVENT         *BdsConsoleEvent
  )
{
  EFI_STATUS        Status;

  ASSERT (BdsConsoleEvent != NULL);

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  NotifyTpl,
                  EfiEventEmptyFunction,
                  NULL,
                  gEfiEventGuid,
                  BdsConsoleEvent
                  );

  return Status;
}

/**
  Create, Signal, and Close the Bds Event Before Console After
  Trusted Console event using CreateBdsEvent().

**/
VOID
EFIAPI
BdsSignalEventBeforeConsoleAfterTrustedConsole (
  VOID
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     BdsConsoleEvent;

  DEBUG ((DEBUG_INFO, "%a \n", __FUNCTION__));

  Status = CreateBdsEvent (
             TPL_CALLBACK,
             &gBdsEventBeforeConsoleAfterTrustedConsoleGuid,
             &BdsConsoleEvent
             );

  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {
    gBS->SignalEvent (BdsConsoleEvent);
    gBS->CloseEvent (BdsConsoleEvent);
    DEBUG ((DEBUG_INFO,"All EventBeforeConsoleAfterTrustedConsole callbacks have returned successfully\n"));
  }
}


/**
  Create, Signal, and Close the Bds Before Console Before End Of Dxe
  event using CreateBdsEvent().
**/
VOID
EFIAPI
BdsSignalEventBeforeConsoleBeforeEndOfDxe (
  VOID
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     BdsConsoleEvent;

  DEBUG ((DEBUG_INFO, "%a \n", __FUNCTION__));

  Status = CreateBdsEvent (
             TPL_CALLBACK,
             &gBdsEventBeforeConsoleBeforeEndOfDxeGuid,
             &BdsConsoleEvent
             );

   ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {
    gBS->SignalEvent (BdsConsoleEvent);
    gBS->CloseEvent (BdsConsoleEvent);
    DEBUG ((DEBUG_INFO,"All BeforeConsoleBeforeEndOfDxe callbacks have returned successfully\n"));
  }
}

/**
  Create, Signal, and Close the Bds After Console Ready Before Boot Option
  using CreateBdsEvent().
**/
VOID
EFIAPI
BdsSignalEventAfterConsoleReadyBeforeBootOption (
  VOID
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     BdsConsoleEvent;

  DEBUG ((DEBUG_INFO, "%a \n", __FUNCTION__));

  Status = CreateBdsEvent (
             TPL_CALLBACK,
             &gBdsEventAfterConsoleReadyBeforeBootOptionGuid,
             &BdsConsoleEvent
             );

  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {
    gBS->SignalEvent (BdsConsoleEvent);
    gBS->CloseEvent (BdsConsoleEvent);
    DEBUG ((DEBUG_INFO,"All AfterConsoleReadyBeforeBootOption callbacks have returned successfully\n"));
  }
}


/**
  Platform Bds init. Incude the platform firmware vendor, revision
  and so crc check.
**/
VOID
EFIAPI
PlatformBootManagerBeforeConsole (
  VOID
  )
{

  DEBUG ((EFI_D_INFO, "PlatformBootManagerBeforeConsole\n"));

  //
  // Trusted console can be added in a PciEnumComplete callback
  //

  //
  // Signal Before Console, after Trusted console Event
  //
  BdsSignalEventBeforeConsoleAfterTrustedConsole ();

  //
  // Signal Before Console, before End of Dxe
  //
  BdsSignalEventBeforeConsoleBeforeEndOfDxe ();

  //
  // Signal End Of Dxe Event
  //
  PERF_START_EX(NULL,"EventRec", NULL, AsmReadTsc(), 0x7020);
  ExitPmAuth ();
  PERF_END_EX(NULL,"EventRec", NULL, AsmReadTsc(), 0x7021);

  //
  // Deferred 3rd party images can be dispatched in
  // an SmmReadyToLock callback
  //
}


/**
  The function will excute with as the platform policy, current policy
  is driven by boot mode. IBV/OEM can customize this code for their specific
  policy action.

  @param DriverOptionList - The header of the driver option link list
  @param BootOptionList   - The header of the boot option link list
  @param ProcessCapsules  - A pointer to ProcessCapsules()
  @param BaseMemoryTest   - A pointer to BaseMemoryTest()
**/
VOID
EFIAPI
PlatformBootManagerAfterConsole (
  VOID
  )
{
  DEBUG ((EFI_D_INFO, "PlatformBootManagerAfterConsole\n"));

  BdsSignalEventAfterConsoleReadyBeforeBootOption ();
}

/**
  The function is called when no boot option could be launched,
  including platform recovery options and options pointing to applications
  built into firmware volumes.

  If this function returns, BDS attempts to enter an infinite loop.
**/
VOID
EFIAPI
PlatformBootManagerUnableToBoot (
  VOID
  )
{
  BoardBootManagerUnableToBoot ();
}

/**
  This function is called each second during the boot manager waits the timeout.

  @param TimeoutRemain  The remaining timeout.
**/
VOID
EFIAPI
PlatformBootManagerWaitCallback (
  UINT16          TimeoutRemain
  )
{
  BoardBootManagerWaitCallback (TimeoutRemain);
}
