/** @file
  Beep status code implementation.

  Copyright (c) 2010 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/UefiDriverEntryPoint.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Protocol/SmmReportStatusCodeHandler.h>

#include <Library/BeepMapLib.h>
#include <Library/BeepLib.h>

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
  Register status code callback function only when Report Status Code protocol
  is installed.

  @param Protocol       Points to the protocol's unique identifier.
  @param Interface      Points to the interface instance.
  @param Handle         The handle on which the interface was installed.

  @retval EFI_SUCCESS   Notification runs successfully.

**/
EFI_STATUS
EFIAPI
RegisterBeepBootTimeHandlers (
  IN CONST EFI_GUID        *Protocol,
  IN VOID                  *Interface,
  IN EFI_HANDLE            Handle
  )
{
  EFI_STATUS                   Status;
  EFI_SMM_RSC_HANDLER_PROTOCOL *RscHandlerProtocol;

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmRscHandlerProtocolGuid,
                    NULL,
                    (VOID **) &RscHandlerProtocol
                    );
  ASSERT_EFI_ERROR (Status);

  RscHandlerProtocol->Register (BeepStatusCodeReportWorker);

  return EFI_SUCCESS;
}

/**
  Constructor function of SmmBeepStatusCodeHandlerLib.

  This function allocates memory for extended status code data, caches
  the report status code service, and registers events.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
SmmBeepStatusCodeHandlerLibConstructor (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                   Status;
  VOID                         *Registration;
  EFI_SMM_RSC_HANDLER_PROTOCOL *RscHandlerProtocol;

  if (!PcdGetBool (PcdStatusCodeUseBeep)) {
    return EFI_SUCCESS;
  }

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmRscHandlerProtocolGuid,
                    NULL,
                    (VOID **) &RscHandlerProtocol
                    );
  if (!EFI_ERROR (Status)) {
    RegisterBeepBootTimeHandlers (NULL, NULL, NULL);
  } else {
    gSmst->SmmRegisterProtocolNotify (
             &gEfiSmmRscHandlerProtocolGuid,
             RegisterBeepBootTimeHandlers,
             &Registration
             );
  }

  return EFI_SUCCESS;
}
