/** @file
  Prototype of the DXE VTD Policy Init library.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _DXE_VTD_POLICY_INIT_LIB_H_
#define _DXE_VTD_POLICY_INIT_LIB_H_

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/SaPolicy.h>
#include <ConfigBlock.h>
#include <VtdConfig.h>
#include <Library/SiConfigBlockLib.h>

extern EFI_GUID gVtdDxeConfigGuid;

/**
  This function Load default Vtd DXE policy.

  @param[in] ConfigBlockPointer    The pointer to add VTD config block
**/
VOID
VtdLoadDefaultDxe (
  IN VOID    *ConfigBlockPointer
  );

/**
  This function prints the DXE phase VTD policy.

  @param[in] SaPolicy - Instance of SA_POLICY_PROTOCOL
**/
VOID
VtdPrintPolicyDxe (
  IN  SA_POLICY_PROTOCOL      *SaPolicy
  );

/**
  This function is used to add VTD Config Block.

  @param[in] ConfigBlockTableAddress    The pointer to add VTD config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
VtdAddConfigBlocksDxe (
  IN VOID           *ConfigBlockTableAddress
  );

/**
  Get VTD DXE config block table total size.

  @retval     Size of VTD DXE config block table
**/
UINT16
EFIAPI
VtdGetConfigBlockTotalSizeDxe (
  VOID
  );
#endif // _DXE_VTD_POLICY_INIT_LIB_H_
