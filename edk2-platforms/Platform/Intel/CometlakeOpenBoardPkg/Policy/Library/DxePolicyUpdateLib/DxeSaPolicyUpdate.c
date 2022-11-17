/** @file
  This file is the library for SA DXE Policy initialization.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <DxeSaPolicyUpdate.h>
#include <Library/ConfigBlockLib.h>

/**
  Get data for platform policy from setup options.

  @param[in] SaPolicy                  The pointer to get SA Policy protocol instance

  @retval EFI_SUCCESS                  Operation success.

**/
EFI_STATUS
EFIAPI
UpdateDxeSaPolicy (
  IN OUT  SA_POLICY_PROTOCOL    *SaPolicy
  )
{
  EFI_STATUS                Status;
  GRAPHICS_DXE_CONFIG       *GraphicsDxeConfig;
  PCIE_DXE_CONFIG           *PcieDxeConfig;
  MISC_DXE_CONFIG           *MiscDxeConfig;
  MEMORY_DXE_CONFIG         *MemoryDxeConfig;

  GraphicsDxeConfig = NULL;
  PcieDxeConfig = NULL;
  MiscDxeConfig = NULL;
  MemoryDxeConfig = NULL;
  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *)SaPolicy, &gGraphicsDxeConfigGuid, (VOID *)&GraphicsDxeConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *)SaPolicy, &gMiscDxeConfigGuid, (VOID *)&MiscDxeConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *)SaPolicy, &gPcieDxeConfigGuid, (VOID *)&PcieDxeConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *)SaPolicy, &gMemoryDxeConfigGuid, (VOID *)&MemoryDxeConfig);
  ASSERT_EFI_ERROR (Status);

  PcieDxeConfig->PegAspmL0s[0] = 3;
  PcieDxeConfig->PegAspmL0s[1] = 3;
  PcieDxeConfig->PegAspmL0s[2] = 3;

  return EFI_SUCCESS;
}


