/** @file
  A Library to support all BMC access via IPMI command during Dxe Phase.

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiDxe.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

STATIC IPMI_TRANSPORT     *mIpmiTransport = NULL;
VOID                      *mEfiIpmiProtocolNotifyReg;
EFI_EVENT                 mEfiIpmiProtocolEvent;

/**
  Callback function for locating the IpmiTransport protocol.
  @param[in]  Event    Event on which the callback is called.
  @param[in]  Context  The context of the Callback.

  @retval EFI_SUCCESS  Return from EfiLocateProtocolInterface function.
  @retval Other        Failure.

**/
EFI_STATUS
NotifyIpmiTransportCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;
  if (mIpmiTransport == NULL) {
    Status = gBS->LocateProtocol (
                    &gIpmiTransportProtocolGuid,
                    NULL,
                    (VOID **) &mIpmiTransport
                    );
  }

  return Status;
}

/**
  Initialize the global varible with the pointer of IpmiTransport Protocol.

  @retval EFI_SUCCESS  Always return success.

**/
EFI_STATUS
InitializeIpmiBase (
  VOID
  )
{
  EFI_STATUS  Status;

  if (mIpmiTransport == NULL) {
    Status = gBS->CreateEvent (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, (EFI_EVENT_NOTIFY) NotifyIpmiTransportCallback, NULL, &mEfiIpmiProtocolEvent);
    ASSERT_EFI_ERROR (Status);
    if (Status != EFI_SUCCESS) {
      return Status;
    }

    Status = gBS->RegisterProtocolNotify (&gIpmiTransportProtocolGuid, mEfiIpmiProtocolEvent, &mEfiIpmiProtocolNotifyReg);
    ASSERT_EFI_ERROR (Status);
    if (Status != EFI_SUCCESS) {
      return Status;
    }

    gBS->SignalEvent (mEfiIpmiProtocolEvent);
  }

  return EFI_SUCCESS;
}

/**
  Routine to send commands to BMC.
  @param[in]       NetFunction        Net function of the command
  @param[in]       Command            IPMI Command
  @param[in]       CommandData        Command Data
  @param[in]       CommandDataSize    Size of CommandData
  @param[out]  ResponseData       Response Data
  @param[in, out]  ResponseDataSize   Response Data Size

  @retval EFI_SUCCESS            Return Successly.
  @retval EFI_NOT_AVAILABLE_YET  IpmiTransport Protocol is not installed yet
  @retval Other                  Failure.

**/
EFI_STATUS
IpmiSubmitCommand (
  IN UINT8        NetFunction,
  IN UINT8        Command,
  IN UINT8        *CommandData,
  IN UINT32       CommandDataSize,
  OUT UINT8       *ResponseData,
  IN OUT UINT32   *ResponseDataSize
  )
{
  EFI_STATUS  Status;

  Status = gBS->LocateProtocol (&gIpmiTransportProtocolGuid, NULL, (VOID **) &mIpmiTransport);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = mIpmiTransport->IpmiSubmitCommand (
                             mIpmiTransport,
                             NetFunction,
                             0,
                             Command,
                             CommandData,
                             CommandDataSize,
                             ResponseData,
                             ResponseDataSize
                             );
  return Status;
}

/**
  Routine to send commands to BMC.
  @param[out]  BmcStatus    A pointer to the BMC_STATUS.
  @param[out]  ComAddress   Pointer to the SM_COM_ADDRESS.

  @retval EFI_SUCCESS            Restart Successly.
  @retval EFI_NOT_AVAILABLE_YET  IpmiTransport Protocol is not installed yet.
  @retval Other                  Failure.

**/
EFI_STATUS
IpmiGetBmcStatus (
  OUT BMC_STATUS                         *BmcStatus,
  OUT SM_COM_ADDRESS                     *ComAddress
  )
{
  EFI_STATUS  Status;

  Status = gBS->LocateProtocol (&gIpmiTransportProtocolGuid, NULL, (VOID **) &mIpmiTransport);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = mIpmiTransport->GetBmcStatus (
                             mIpmiTransport,
                             BmcStatus,
                             ComAddress
                             );
  return Status;
}

