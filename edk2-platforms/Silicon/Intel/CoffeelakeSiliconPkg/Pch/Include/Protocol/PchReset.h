/** @file
  PCH Reset Protocol

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_RESET_PROTOCOL_H_
#define _PCH_RESET_PROTOCOL_H_

//
// Member functions
//
/**
  Execute call back function for Pch Reset.

  @param[in] ResetType            Reset Types which includes GlobalReset.
  @param[in] ResetTypeGuid        Pointer to an EFI_GUID, which is the Reset Type Guid.

  @retval EFI_SUCCESS             The callback function has been done successfully
  @retval EFI_NOT_FOUND           Failed to find Pch Reset Callback protocol. Or, none of
                                  callback protocol is installed.
  @retval Others                  Do not do any reset from PCH
**/
typedef
EFI_STATUS
(EFIAPI *PCH_RESET_CALLBACK) (
  IN  EFI_RESET_TYPE    ResetType,
  IN  EFI_GUID          *ResetTypeGuid
  );

/**
  This protocol is used to execute PCH Reset from the host controller.
  If drivers need to run their callback function right before issuing the PCH Reset,
  they can install PCH Reset Callback Protocol before PCH Reset DXE driver to achieve that.
**/
typedef struct {
  PCH_RESET_CALLBACK  ResetCallback;
} PCH_RESET_CALLBACK_PROTOCOL;

#endif
