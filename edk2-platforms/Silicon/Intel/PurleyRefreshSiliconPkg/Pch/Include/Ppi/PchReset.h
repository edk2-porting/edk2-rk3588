/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PCH_RESET_PPI_H_
#define _PCH_RESET_PPI_H_

//
// Extern the GUID for PPI users.
//
extern EFI_GUID gPchResetPpiGuid;
extern EFI_GUID gPchResetCallbackPpiGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _PCH_RESET_PPI          PCH_RESET_PPI;
typedef struct _PCH_RESET_CALLBACK_PPI PCH_RESET_CALLBACK_PPI;

//
// Related Definitions
//
//
// PCH Reset Types
//
typedef enum {
  ColdReset,
  WarmReset,
  ShutdownReset,
  PowerCycleReset,
  GlobalReset,
  GlobalResetWithEc,
  PchResetTypeMax
} PCH_RESET_TYPE;

//
// Member functions
//
/**
  Execute Pch Reset from the host controller.

  @param[in] This                 Pointer to the PCH_RESET_PPI instance.
  @param[in] PchResetType         Pch Reset Types which includes ColdReset, WarmReset, ShutdownReset,
                                  PowerCycleReset, GlobalReset, GlobalResetWithEc

  @retval EFI_SUCCESS             Successfully completed.
  @retval EFI_INVALID_PARAMETER   If ResetType is invalid.
**/
typedef
EFI_STATUS
(EFIAPI *PCH_RESET_PPI_API) (
  IN  PCH_RESET_PPI  *This,
  IN  PCH_RESET_TYPE PchResetType
  );

/**
  Execute call back function for Pch Reset.

  @param[in] PchResetType         Pch Reset Types which includes PowerCycle, Globalreset.

  @retval EFI_SUCCESS             The callback function has been done successfully
  @retval EFI_NOT_FOUND           Failed to find Pch Reset Callback ppi. Or, none of
                                  callback ppi is installed.
  @retval Others                  Do not do any reset from PCH
**/
typedef
EFI_STATUS
(EFIAPI *PCH_RESET_CALLBACK) (
  IN     PCH_RESET_TYPE           PchResetType
  );

/**
  Interface structure to execute Pch Reset from the host controller.
**/
struct _PCH_RESET_PPI {
  PCH_RESET_PPI_API Reset;
};

/**
  This ppi is used to execute PCH Reset from the host controller.
  The PCH Reset protocol and PCH Reset PPI implement the Intel (R) PCH Reset Interface
  for DXE and PEI environments, respectively. If other drivers need to run their
  callback function right before issuing the reset, they can install PCH Reset
  Callback Protocol/PPI before PCH Reset DXE/PEI driver to achieve that.
**/
struct _PCH_RESET_CALLBACK_PPI {
  PCH_RESET_CALLBACK  ResetCallback;
};

#endif
