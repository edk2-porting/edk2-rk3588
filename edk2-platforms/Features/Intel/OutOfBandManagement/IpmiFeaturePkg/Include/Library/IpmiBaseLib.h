/** @file
  IPMI Command Library Header File.

  @copyright
  Copyright 2011 - 2021 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IPMI_BASE_LIB_H_
#define _IPMI_BASE_LIB_H_

//
// Prototype definitions for IPMI Library
//
/**
  Initialize the global varible with the pointer of IpmiTransport Protocol.

  @retval EFI_SUCCESS - Always return success

**/
EFI_STATUS
InitializeIpmiBase (
  VOID
  );

/**
  Routine to send commands to BMC.

  @param NetFunction       - Net function of the command
  @param Command           - IPMI Command
  @param CommandData       - Command Data
  @param CommandDataSize   - Size of CommandData
  @param ResponseData      - Response Data
  @param ResponseDataSize  - Response Data Size

  @retval EFI_NOT_AVAILABLE_YET - IpmiTransport Protocol is not installed yet

**/
EFI_STATUS
IpmiSubmitCommand (
  IN UINT8     NetFunction,
  IN UINT8     Command,
  IN UINT8     *CommandData,
  IN UINT32    CommandDataSize,
  OUT UINT8    *ResponseData,
  OUT UINT32   *ResponseDataSize
  );

#endif

