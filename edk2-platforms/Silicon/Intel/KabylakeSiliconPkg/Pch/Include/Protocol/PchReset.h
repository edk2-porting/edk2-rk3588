/** @file
  PCH Reset Protocol

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PCH_RESET_PROTOCOL_H_
#define _PCH_RESET_PROTOCOL_H_

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                             gPchResetProtocolGuid;
extern EFI_GUID                             gPchResetCallbackProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _PCH_RESET_PROTOCOL           PCH_RESET_PROTOCOL;
typedef struct _PCH_RESET_CALLBACK_PROTOCOL  PCH_RESET_CALLBACK_PROTOCOL;

//
// Related Definitions
//
///
/// PCH Reset Types
///
typedef enum {
  PchColdReset     = 0,
  PchWarmReset     = 1,
  PchShutdownReset = 2,
  PchGlobalReset   = 4,
  PchResetTypeMax
} PCH_RESET_TYPE;

//
// Member functions
//
/**
  Execute Pch Reset from the host controller.

  @param[in] This                 Pointer to the PCH_RESET_PROTOCOL instance.
  @param[in] ResetType            UEFI defined reset type.
  @param[in] DataSize             The size of ResetData in bytes.
  @param[in] ResetData            Optional element used to introduce a platform specific reset.
                                  The exact type of the reset is defined by the EFI_GUID that follows
                                  the Null-terminated Unicode string.

  @retval EFI_SUCCESS             Successfully completed.
  @retval EFI_INVALID_PARAMETER   If ResetType is invalid.
**/
typedef
EFI_STATUS
(EFIAPI *PCH_RESET) (
  IN PCH_RESET_PROTOCOL *This,
  IN EFI_RESET_TYPE     ResetType,
  IN UINTN              DataSize,
  IN VOID               *ResetData OPTIONAL
  );

/**
  Retrieve PCH platform specific ResetData

  @param[in]  Guid      PCH platform specific reset GUID.
  @param[out] DataSize  The size of ResetData in bytes.

  @retval ResetData     A platform specific reset that the exact type of
                        the reset is defined by the EFI_GUID that follows
                        the Null-terminated Unicode string.
  @retval NULL          If Guid is not defined in PCH platform specific reset.
**/
typedef
VOID *
(EFIAPI *PCH_RESET_GET_RESET_DATA) (
  IN  EFI_GUID  *Guid,
  OUT UINTN     *DataSize
  );

/**
  Interface structure to execute Pch Reset from the host controller.
**/
struct _PCH_RESET_PROTOCOL {
  PCH_RESET                Reset;
  PCH_RESET_GET_RESET_DATA GetResetData;
};

/**
  Execute call back function for Pch Reset.

  @param[in] ResetType            Reset Types which includes PowerCycle, GlobalReset.

  @retval EFI_SUCCESS             The callback function has been done successfully
  @retval EFI_NOT_FOUND           Failed to find Pch Reset Callback protocol. Or, none of
                                  callback protocol is installed.
  @retval Others                  Do not do any reset from PCH
**/
typedef
EFI_STATUS
(EFIAPI *PCH_RESET_CALLBACK) (
  IN  PCH_RESET_TYPE    ResetType
  );

/**
  This protocol is used to execute PCH Reset from the host controller.
  The PCH Reset protocol and PCH Reset PPI implement the Intel (R) PCH Reset Interface
  for DXE and PEI environments, respectively. If other drivers need to run their
  callback function right before issuing the reset, they can install PCH Reset
  Callback Protocol/PPI before PCH Reset DXE/PEI driver to achieve that.
**/
struct _PCH_RESET_CALLBACK_PROTOCOL {
  PCH_RESET_CALLBACK  ResetCallback;
};

#endif
