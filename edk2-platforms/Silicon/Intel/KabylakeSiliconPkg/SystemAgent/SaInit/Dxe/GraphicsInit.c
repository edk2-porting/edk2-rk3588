/** @file
  DXE driver for Initializing SystemAgent Graphics ACPI table initialization.

Copyright (c) 2017 - 2020 Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "GraphicsInit.h"
#include <Library/ConfigBlockLib.h>

extern SYSTEM_AGENT_GLOBAL_NVS_AREA_PROTOCOL  mSaGlobalNvsAreaProtocol;


/**
Initialize GT ACPI tables

  @param[in] ImageHandle -     Handle for the image of this driver
  @param[in] SaPolicy -        SA DXE Policy protocol

  @retval EFI_SUCCESS          - GT ACPI initialization complete
  @retval EFI_NOT_FOUND        - Dxe System Table not found.
  @retval EFI_OUT_OF_RESOURCES - Mmio not allocated successfully.
**/
EFI_STATUS
GraphicsInit (
  IN EFI_HANDLE              ImageHandle,
  IN SA_POLICY_PROTOCOL      *SaPolicy
  )
{
  EFI_STATUS            Status;
  GRAPHICS_DXE_CONFIG   *GraphicsDxeConfig;

  Status = GetConfigBlock ((VOID *) SaPolicy, &gGraphicsDxeConfigGuid, (VOID *)&GraphicsDxeConfig);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Update IGD SA Global NVS
  ///
  DEBUG ((DEBUG_INFO, " Update Igd SA Global NVS Area.\n"));

  mSaGlobalNvsAreaProtocol.Area->AlsEnable                    = GraphicsDxeConfig->AlsEnable;
  ///
  /// Initialize IGD state by checking if IGD Device 2 Function 0 is enabled in the chipset
  ///
  if (MmioRead16 (MmPciBase (SA_MC_BUS, 0, 0) + R_SA_DEVEN) & B_SA_DEVEN_D2EN_MASK) {
    mSaGlobalNvsAreaProtocol.Area->IgdState = 1;
  } else {
    mSaGlobalNvsAreaProtocol.Area->IgdState = 0;
  }

  mSaGlobalNvsAreaProtocol.Area->BrightnessPercentage         = 100;
  mSaGlobalNvsAreaProtocol.Area->IgdBootType                  = GraphicsDxeConfig->IgdBootType;
  mSaGlobalNvsAreaProtocol.Area->IgdPanelType                 = GraphicsDxeConfig->IgdPanelType;
  mSaGlobalNvsAreaProtocol.Area->IgdPanelScaling              = GraphicsDxeConfig->IgdPanelScaling;
  ///
  /// Get SFF power mode platform data for the IGD driver.  Flip the bit (bitwise xor)
  /// since Setup value is opposite of NVS and IGD OpRegion value.
  ///
  mSaGlobalNvsAreaProtocol.Area->IgdDvmtMemSize               = GraphicsDxeConfig->IgdDvmtMemSize;
  mSaGlobalNvsAreaProtocol.Area->IgdFunc1Enable               = 0;
  mSaGlobalNvsAreaProtocol.Area->IgdSciSmiMode                = 0;
  mSaGlobalNvsAreaProtocol.Area->GfxTurboIMON                 = GraphicsDxeConfig->GfxTurboIMON;

  mSaGlobalNvsAreaProtocol.Area->EdpValid                     = 0;

  return EFI_SUCCESS;
}

