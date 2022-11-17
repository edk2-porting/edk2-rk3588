/** @file
  This file provides services for DXE VTD policy default initialization

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DxeVtdPolicyLib.h>

/**
  This function prints the DXE phase VTD policy.

  @param[in] SaPolicy - Instance of SA_POLICY_PROTOCOL
**/
VOID
VtdPrintPolicyDxe (
  IN  SA_POLICY_PROTOCOL      *SaPolicy
  )
{
  EFI_STATUS                  Status;
  VTD_DXE_CONFIG             *VtdDxeConfig;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *) SaPolicy, &gVtdDxeConfigGuid, (VOID *)&VtdDxeConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INFO, "\n------------------------ Vtd Policy (DXE) print BEGIN -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", VtdDxeConfig->Header.Revision));
  ASSERT (VtdDxeConfig->Header.Revision == VTD_DXE_CONFIG_REVISION);
  DEBUG ((DEBUG_INFO, "\n------------------------ Vtd Policy (DXE) print END -----------------\n"));
  DEBUG_CODE_END ();
  return;
}

/**
  This function Load default Vtd DXE policy.

  @param[in] ConfigBlockPointer    The pointer to add VTD config block
**/
VOID
VtdLoadDefaultDxe (
  IN VOID    *ConfigBlockPointer
  )
{
  VTD_DXE_CONFIG        *VtdDxeConfig;

  VtdDxeConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "VtdDxeConfig->Header.GuidHob.Name = %g\n", &VtdDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "VtdDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", VtdDxeConfig->Header.GuidHob.Header.HobLength));
}

static COMPONENT_BLOCK_ENTRY  mVtdDxeIpBlock = {
  &gVtdDxeConfigGuid, sizeof (VTD_DXE_CONFIG), VTD_DXE_CONFIG_REVISION, VtdLoadDefaultDxe
};

/**
  Get VTD DXE config block table total size.

  @retval     Size of VTD DXE config block table
**/
UINT16
EFIAPI
VtdGetConfigBlockTotalSizeDxe (
  VOID
  )
{
  return mVtdDxeIpBlock.Size;
}

/**
  VtdAddConfigBlocks add VTD DXE config block.

  @param[in] ConfigBlockTableAddress    The pointer to add VTD config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
VtdAddConfigBlocksDxe (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mVtdDxeIpBlock, 1);
  return Status;
}
