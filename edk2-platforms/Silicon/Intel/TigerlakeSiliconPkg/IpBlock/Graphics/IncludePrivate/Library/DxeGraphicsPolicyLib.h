/** @file
  Header file for the DXE Graphics Policy Init library.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _DXE_GRAPHICS_POLICY_LIB_H_
#define _DXE_GRAPHICS_POLICY_LIB_H_

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/SaPolicy.h>
#include <ConfigBlock.h>
#include <GraphicsConfig.h>
#include <Library/SiConfigBlockLib.h>

#define WORD_FIELD_VALID_BIT  BIT15

extern EFI_GUID gGraphicsDxeConfigGuid;

/**
  This function prints the Graphics DXE phase policy.

  @param[in] SaPolicy - SA DXE Policy protocol
**/
VOID
GraphicsDxePolicyPrint (
  IN  SA_POLICY_PROTOCOL      *SaPolicy
  );

/**
  This function Load default Graphics DXE policy.

  @param[in] ConfigBlockPointer    The pointer to add Graphics config block
**/
VOID
LoadIgdDxeDefault (
  IN VOID    *ConfigBlockPointer
  );


/**
  Get DXE Graphics config block table total size.

  @retval     Size of DXE Graphics config block table
**/
UINT16
EFIAPI
GraphicsGetConfigBlockTotalSizeDxe (
  VOID
  );

/**
  GraphicsAddConfigBlocksDxe add all DXE Graphics config block.

  @param[in] ConfigBlockTableAddress    The pointer to add SA config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
GraphicsAddConfigBlocksDxe (
  IN VOID           *ConfigBlockTableAddress
  );

#endif // _DXE_GRAPHICs_POLICY_LIBRARY_H_
