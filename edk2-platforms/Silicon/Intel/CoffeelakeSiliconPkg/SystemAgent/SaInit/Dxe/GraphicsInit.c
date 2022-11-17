/** @file
  DXE driver for Initializing SystemAgent Graphics ACPI table initialization.

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "GraphicsInit.h"
#include "SaInit.h"
#include <Protocol/LegacyBios.h>
#include <Protocol/GopComponentName2.h>
#include <SiConfigHob.h>
#include <Library/ConfigBlockLib.h>

typedef union {
  struct {
    UINT32  Low;
    UINT32  High;
  } Data32;
  UINT64 Data;
} UINT64_STRUCT;

extern SYSTEM_AGENT_NVS_AREA_PROTOCOL  mSaNvsAreaProtocol;

GLOBAL_REMOVE_IF_UNREFERENCED UINT64                        mGttMmAdr;
GLOBAL_REMOVE_IF_UNREFERENCED UINT64_STRUCT                 mMchBarBase;
GLOBAL_REMOVE_IF_UNREFERENCED GOP_COMPONENT_NAME2_PROTOCOL  *GopComponentName2Protocol = NULL;

/**
    Do Post GT PM Init Steps after VBIOS Initialization.

  @retval EFI_SUCCESS          Succeed.
**/
EFI_STATUS
PostPmInitEndOfDxe (
  VOID
  )
{
  CHAR16                    *DriverVersion;
  UINTN                     Index;
  EFI_LEGACY_BIOS_PROTOCOL  *LegacyBios;
  EFI_STATUS                Status;
  GRAPHICS_DXE_CONFIG       *GraphicsDxeConfig;
  EFI_PEI_HOB_POINTERS      HobPtr;
  SI_CONFIG_HOB_DATA        *SiConfigHobData;

  ///
  /// Get the platform setup policy.
  ///
  DriverVersion = NULL;
  LegacyBios = NULL;
  Status = gBS->LocateProtocol (&gSaPolicyProtocolGuid, NULL, (VOID **) &mSaPolicy);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) mSaPolicy, &gGraphicsDxeConfigGuid, (VOID *)&GraphicsDxeConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Get Silicon Config data HOB
  //
  HobPtr.Guid   = GetFirstGuidHob (&gSiConfigHobGuid);
  SiConfigHobData = (SI_CONFIG_HOB_DATA *)GET_GUID_HOB_DATA (HobPtr.Guid);

  if (SiConfigHobData->CsmFlag == 1) {
    Status = gBS->LocateProtocol (
                    &gEfiLegacyBiosProtocolGuid,
                    NULL,
                    (VOID **) &LegacyBios
                    );
  }

  if (LegacyBios == NULL) {
    Status = gBS->LocateProtocol (&gGopComponentName2ProtocolGuid, NULL, (VOID **)&GopComponentName2Protocol);
    if (!EFI_ERROR (Status)) {
      Status = GopComponentName2Protocol->GetDriverVersion (
                                            GopComponentName2Protocol,
                                            "en-US",
                                            &DriverVersion
                                            );
      if (!EFI_ERROR (Status)) {
        for (Index = 0; (DriverVersion[Index] != '\0'); Index++) {
        }
        Index = (Index+1)*2;
        CopyMem (GraphicsDxeConfig->GopVersion, DriverVersion, Index);
      }
    }
  }

  ///
  /// Return final status
  ///
  return EFI_SUCCESS;
}


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

  mGttMmAdr               = 0;
  Status                  = EFI_SUCCESS;
  mMchBarBase.Data32.High = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, 0, 0, R_SA_MCHBAR + 4));
  mMchBarBase.Data32.Low  = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, 0, 0, R_SA_MCHBAR));
  mMchBarBase.Data       &= (UINT64) ~BIT0;

  Status = GetConfigBlock ((VOID *) SaPolicy, &gGraphicsDxeConfigGuid, (VOID *)&GraphicsDxeConfig);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Update IGD SA Global NVS
  ///
  DEBUG ((DEBUG_INFO, " Update Igd SA Global NVS Area.\n"));

  mSaNvsAreaProtocol.Area->AlsEnable                    = GraphicsDxeConfig->AlsEnable;
  ///
  /// Initialize IGD state by checking if IGD Device 2 Function 0 is enabled in the chipset
  ///
  if (PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, 0, 0, R_SA_DEVEN)) & B_SA_DEVEN_D2EN_MASK) {
    mSaNvsAreaProtocol.Area->IgdState = 1;
  } else {
    mSaNvsAreaProtocol.Area->IgdState = 0;
  }

  mSaNvsAreaProtocol.Area->BrightnessPercentage         = 100;
  mSaNvsAreaProtocol.Area->IgdBootType                  = GraphicsDxeConfig->IgdBootType;
  mSaNvsAreaProtocol.Area->IgdPanelType                 = GraphicsDxeConfig->IgdPanelType;
  mSaNvsAreaProtocol.Area->IgdPanelScaling              = GraphicsDxeConfig->IgdPanelScaling;
  ///
  /// Get SFF power mode platform data for the IGD driver.  Flip the bit (bitwise xor)
  /// since Setup value is opposite of NVS and IGD OpRegion value.
  ///
  mSaNvsAreaProtocol.Area->IgdDvmtMemSize               = GraphicsDxeConfig->IgdDvmtMemSize;
  mSaNvsAreaProtocol.Area->IgdFunc1Enable               = 0;
  mSaNvsAreaProtocol.Area->IgdHpllVco                   = MmioRead8 (mMchBarBase.Data + 0xC0F) & 0x07;
  mSaNvsAreaProtocol.Area->IgdSciSmiMode                = 0;
  mSaNvsAreaProtocol.Area->GfxTurboIMON                 = GraphicsDxeConfig->GfxTurboIMON;

  mSaNvsAreaProtocol.Area->EdpValid                     = 0;

  return EFI_SUCCESS;
}

