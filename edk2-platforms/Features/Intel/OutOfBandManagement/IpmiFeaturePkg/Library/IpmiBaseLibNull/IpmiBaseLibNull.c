/** @file
  A Null Library to support all BMC access via IPMI command.

  @copyright
  Copyright 2011 - 2021 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/IpmiBaseLib.h>
#include <Ppi/IpmiTransportPpi.h>

/**

  Initialize the global varible with the pointer of IpmiTransport Protocol

  @retval EFI_SUCCESS Always return success.

**/
EFI_STATUS
InitializeIpmiBase (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  Routine to send commands to BMC.
  @param [in]   NetFunction        Net function of the command
  @param [in]   Command            IPMI Command
  @param [in]   CommandData        Command Data
  @param [in]   CommandDataSize    Size of CommandData
  @param [out]  ResponseData       Response Data
  @param [out]  ResponseDataSize   Response Data Size

  @retval EFI_SUCCESS            Restart Successly.
  @retval EFI_NOT_AVAILABLE_YET  IpmiTransport Protocol is not installed yet.
  @retval Other                  Failure.

**/
EFI_STATUS
IpmiSubmitCommand (
  IN UINT8     NetFunction,
  IN UINT8     Command,
  IN UINT8     *CommandData,
  IN UINT32    CommandDataSize,
  OUT UINT8    *ResponseData,
  OUT UINT32   *ResponseDataSize
  )
{
  return EFI_SUCCESS;
}

/**
  Routine to send commands to BMC.
  @param [out] BmcStatus   A pointer to BMC_STATUS.
  @param [out] ComAddress  A pointer to SM_COM_ADDRESS.

  @retval EFI_SUCCESS  Restart Successly.
  @retval EFI_NOT_AVAILABLE_YET - IpmiTransport Protocol is not installed yet.
  @retval Other        Failure.

**/
EFI_STATUS
GetBmcStatus (
  OUT BMC_STATUS                         *BmcStatus,
  OUT SM_COM_ADDRESS                     *ComAddress
  )
{
  return EFI_SUCCESS;
}

