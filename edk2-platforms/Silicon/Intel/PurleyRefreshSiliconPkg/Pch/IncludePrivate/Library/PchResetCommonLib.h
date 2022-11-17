/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PCH_RESET_COMMON_LIB_H_
#define _PCH_RESET_COMMON_LIB_H_
#include <Uefi/UefiSpec.h>
#include <Protocol/PchReset.h>
///
/// Private data structure definitions for the driver
///
#define PCH_RESET_SIGNATURE SIGNATURE_32 ('I', 'E', 'R', 'S')

typedef struct {
  UINT32              Signature;
  EFI_HANDLE          Handle;
  union {
    PCH_RESET_PPI       PchResetPpi;
    PCH_RESET_PROTOCOL  PchResetProtocol;
  }PchResetInterface;
  UINT32              PchPwrmBase;
  UINT16              PchAcpiBase;
  UINTN               PchPmcBase;
} PCH_RESET_INSTANCE;

//
// Function prototypes used by the Pch Reset ppi/protocol.
//
/**
  Initialize an Pch Reset ppi/protocol instance.

  @param[in] PchResetInstance     Pointer to PchResetInstance to initialize

  @retval EFI_SUCCESS             The protocol instance was properly initialized
  @exception EFI_UNSUPPORTED      The PCH is not supported by this module
**/
EFI_STATUS
PchResetConstructor (
  PCH_RESET_INSTANCE          *PchResetInstance
  );

/**
  Execute Pch Reset from the host controller.
  @param[in] PchResetInstance     Pointer to PchResetInstance to initialize
  @param[in] PchResetType         Pch Reset Types which includes ColdReset, WarmReset, ShutdownReset,
                                  PowerCycleReset, GlobalReset, GlobalResetWithEc

  @retval EFI_SUCCESS             Successfully completed.
  @retval EFI_INVALID_PARAMETER   If ResetType is invalid.
**/
EFI_STATUS
PchReset (
  IN PCH_RESET_INSTANCE *PchResetInstance,
  IN PCH_RESET_TYPE     PchResetType
  );
#endif
