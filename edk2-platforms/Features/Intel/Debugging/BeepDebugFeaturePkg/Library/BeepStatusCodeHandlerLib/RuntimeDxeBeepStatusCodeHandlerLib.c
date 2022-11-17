/** @file
  Beep status code implementation.

  Copyright (c) 2010 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Guid/EventGroup.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Protocol/ReportStatusCodeHandler.h>

#include <Library/BeepMapLib.h>
#include <Library/BeepLib.h>

EFI_RSC_HANDLER_PROTOCOL  *mBeepRscHandlerProtocol       = NULL;
EFI_EVENT                 mBeepExitBootServicesEvent     = NULL;
BOOLEAN                   mBeepRegistered                = FALSE;

/**
  Convert status code value to the times of beep.

  @param  CodeType         Indicates the type of status code being reported.
  @param  Value            Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.
  @param  Instance         The enumeration of a hardware or software entity within
                           the system. A system may contain multiple entities that match a class/subclass
                           pairing. The instance differentiates between them. An instance of 0 indicates
                           that instance information is unavailable, not meaningful, or not relevant.
                           Valid instance numbers start with 1.
  @param  CallerId         This optional parameter may be used to identify the caller.
                           This parameter allows the status code driver to apply different rules to
                           different callers.
  @param  Data             This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS      Status code reported to beep successfully.

**/
EFI_STATUS
EFIAPI
BeepStatusCodeReportWorker (
  IN EFI_STATUS_CODE_TYPE           CodeType,
  IN EFI_STATUS_CODE_VALUE          Value,
  IN UINT32                         Instance,
  IN EFI_GUID                       *CallerId,
  IN EFI_STATUS_CODE_DATA           *Data OPTIONAL
  )
{
  UINT32 BeepValue;

  BeepValue = GetBeepValueFromStatusCode (CodeType, Value);
  if (BeepValue != 0) {
    Beep (BeepValue);
  }

  return EFI_SUCCESS;
}

/**
  Unregister status code callback functions only available at boot time from
  report status code router when exiting boot services.

  @param  Event         Event whose notification function is being invoked.
  @param  Context       Pointer to the notification function's context, which is
                        always zero in current implementation.

**/
VOID
EFIAPI
UnregisterBeepBootTimeHandlers (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  if (mBeepRegistered) {
    mBeepRscHandlerProtocol->Unregister (BeepStatusCodeReportWorker);
  }
}

/**
  Register status code callback function only when Report Status Code protocol
  is installed.

  @param  Event         Event whose notification function is being invoked.
  @param  Context       Pointer to the notification function's context, which is
                        always zero in current implementation.

**/
VOID
EFIAPI
RegisterBeepBootTimeHandlers (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_STATUS                Status;

  Status = gBS->LocateProtocol (
                  &gEfiRscHandlerProtocolGuid,
                  NULL,
                  (VOID **) &mBeepRscHandlerProtocol
                  );
  ASSERT_EFI_ERROR (Status);

  mBeepRscHandlerProtocol->Register (BeepStatusCodeReportWorker, TPL_HIGH_LEVEL);
  ASSERT_EFI_ERROR (Status);
  mBeepRegistered = TRUE;

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  UnregisterBeepBootTimeHandlers,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &mBeepExitBootServicesEvent
                  );
  ASSERT_EFI_ERROR (Status);
}

/**
  Constructor function of RuntimeDxeBeepStatusCodeHandlerLib.

  This function allocates memory for extended status code data, caches
  the report status code service, and registers events.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
RuntimeDxeBeepStatusCodeHandlerLibConstructor (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                Status;
  EFI_EVENT                 RegisterStatusCodeHandlerEvent;
  VOID                      *Registration;

  if (!PcdGetBool (PcdStatusCodeUseBeep)) {
    return EFI_SUCCESS;
  }

  Status = gBS->LocateProtocol (
                  &gEfiRscHandlerProtocolGuid,
                  NULL,
                  (VOID **) &mBeepRscHandlerProtocol
                  );

  if (!EFI_ERROR (Status)) {
    RegisterBeepBootTimeHandlers (NULL, NULL);
  } else {
    Status = gBS->CreateEvent (
                    EVT_NOTIFY_SIGNAL,
                    TPL_NOTIFY,
                    RegisterBeepBootTimeHandlers,
                    NULL,
                    &RegisterStatusCodeHandlerEvent
                    );
    ASSERT_EFI_ERROR (Status);

    //
    // Register for protocol notifications on this event
    //
    Status = gBS->RegisterProtocolNotify (
                    &gEfiRscHandlerProtocolGuid,
                    RegisterStatusCodeHandlerEvent,
                    &Registration
                    );
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
