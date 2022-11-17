/** @file
  A Library to support all BMC access via IPMI command during SMM Phase.

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiDxe.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Library/IpmiBaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/DebugLib.h>

STATIC IPMI_TRANSPORT     *mIpmiTransport = NULL;
VOID                      *mEfiIpmiProtocolNotifyReg = NULL;
EFI_EVENT                 mEfiIpmiProtocolEvent;

/**
  Callback function for locating the IpmiTransport protocol.

  @param Protocol     A pointer to EFI_GUID
  @param Interface    A pointer to Interface
  @param Handle       Handle

  @retval EFI_SUCCESS:               Callback successfully

**/
EFI_STATUS
NotifyIpmiTransportCallback (
  IN CONST EFI_GUID                *Protocol,
  IN VOID                          *Interface,
  IN EFI_HANDLE                    Handle
  )
{
  EFI_STATUS  Status;
  Status = EFI_SUCCESS;
  if (mIpmiTransport == NULL) {
    Status = gSmst->SmmLocateProtocol (
                      &gSmmIpmiTransportProtocolGuid,
                      NULL,
                      (VOID **) &mIpmiTransport
                      );
  }

  return Status;
}

/**
  Routine to send commands to BMC.

  @retval EFI_SUCCESS:        Always return success

**/
EFI_STATUS
InitializeIpmiBase (
  VOID
  )
{
  EFI_STATUS  Status;
  if (mIpmiTransport == NULL) {
    Status = gSmst->SmmLocateProtocol (
                      &gSmmIpmiTransportProtocolGuid,
                      NULL,
                      (VOID **) &mIpmiTransport
                      );
    if (EFI_ERROR (Status)) {
      Status = gSmst->SmmRegisterProtocolNotify (
                        &gSmmIpmiTransportProtocolGuid,
                        (EFI_SMM_NOTIFY_FN) NotifyIpmiTransportCallback,
                        &mEfiIpmiProtocolNotifyReg
                        );
    }
    ASSERT_EFI_ERROR (Status);
    if (Status != EFI_SUCCESS) {
      return Status;
    }
  }

  return EFI_SUCCESS;
}

/**
  Routine to send commands to BMC.

  @param NetFunction       - Net function of the command
  @param Command           - IPMI Command
  @param CommandData       - Command Data
  @param CommandDataSize   - Size of CommandData
  @param ResponseData      - Response Data
  @param ResponseDataSize  - Response Data Size

  @retval EFI_SUCCESS:               Get successfully
  @retval EFI_NOT_AVAILABLE_YET

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
/*++

Routine Description:

  Routine to send commands to BMC

Arguments:

  NetFunction       - Net function of the command
  Command           - IPMI Command
  CommandData       - Command Data
  CommandDataSize   - Size of CommandData
  ResponseData      - Response Data
  ResponseDataSize  - Response Data Size

Returns:

  EFI_NOT_AVAILABLE_YET - IpmiTransport Protocol is not installed yet

--*/
{
  EFI_STATUS  Status;

  Status = gSmst->SmmLocateProtocol (&gSmmIpmiTransportProtocolGuid, NULL, (VOID **) &mIpmiTransport);
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

  @param BmcStatus                   The ConnectAllComplete EFI Event.
  @param ComAddress                  Event context pass to create function

  @retval EFI_SUCCESS:               Get successfully
  @retval EFI_NOT_AVAILABLE_YET

**/
EFI_STATUS
GetBmcStatus (
  OUT BMC_STATUS                       *BmcStatus,
  OUT SM_COM_ADDRESS                   *ComAddress
  )
{
  EFI_STATUS  Status;

  Status = gSmst->SmmLocateProtocol (&gSmmIpmiTransportProtocolGuid, NULL, (VOID **) &mIpmiTransport);
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

