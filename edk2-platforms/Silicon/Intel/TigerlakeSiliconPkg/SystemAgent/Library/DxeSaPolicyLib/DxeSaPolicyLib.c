/** @file
  This file provide services for DXE phase policy default initialization

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "DxeSaPolicyLibrary.h"
#include <Library/DxeGraphicsPolicyLib.h>
#include <Library/DxeVtdPolicyLib.h>
#include <Register/SaRegsHostBridge.h>

extern EFI_GUID gMemoryDxeConfigGuid;
extern EFI_GUID gPcieDxeConfigGuid;

/**
  This function prints the SA DXE phase policy.

  @param[in] SaPolicy - SA DXE Policy protocol
**/
VOID
SaPrintPolicyProtocol (
  IN  SA_POLICY_PROTOCOL      *SaPolicy
  )
{
  EFI_STATUS                  Status;
  PCIE_DXE_CONFIG             *PcieDxeConfig;
  MEMORY_DXE_CONFIG           *MemoryDxeConfig;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *) SaPolicy, &gPcieDxeConfigGuid, (VOID *)&PcieDxeConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SaPolicy, &gMemoryDxeConfigGuid, (VOID *)&MemoryDxeConfig);
  ASSERT_EFI_ERROR (Status);


  DEBUG_CODE_BEGIN ();
  INTN  i;

  DEBUG ((DEBUG_INFO, "\n------------------------ SA Policy (DXE) print BEGIN -----------------\n"));
  DEBUG ((DEBUG_INFO, "Revision : %x\n", SaPolicy->TableHeader.Header.Revision));
  ASSERT (SaPolicy->TableHeader.Header.Revision == SA_POLICY_PROTOCOL_REVISION);

  DEBUG ((DEBUG_INFO, "------------------------ SA_MEMORY_CONFIGURATION -----------------\n"));

  DEBUG ((DEBUG_INFO, " SpdAddressTable[%d] :", 4));
  for (i = 0; i < 4; i++) {
    DEBUG ((DEBUG_INFO, " %x", MemoryDxeConfig->SpdAddressTable[i]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  DEBUG ((DEBUG_INFO, " ChannelASlotMap : %x\n", MemoryDxeConfig->ChannelASlotMap));
  DEBUG ((DEBUG_INFO, " ChannelBSlotMap : %x\n", MemoryDxeConfig->ChannelBSlotMap));
  DEBUG ((DEBUG_INFO, " MrcTimeMeasure  : %x\n", MemoryDxeConfig->MrcTimeMeasure));
  DEBUG ((DEBUG_INFO, " MrcFastBoot     : %x\n", MemoryDxeConfig->MrcFastBoot));

  DEBUG ((DEBUG_INFO, "------------------------ CPU_PCIE_CONFIGURATION -----------------\n"));
  DEBUG ((DEBUG_INFO, " PegAspm[%d] :", SA_PEG_MAX_FUN));
  for (i = 0; i < SA_PEG_MAX_FUN; i++) {
    DEBUG ((DEBUG_INFO, " %x", PcieDxeConfig->PegAspm[i]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  DEBUG ((DEBUG_INFO, " PegRootPortHPE[%d] :", SA_PEG_MAX_FUN));
  for (i = 0; i < SA_PEG_MAX_FUN; i++) {
    DEBUG ((DEBUG_INFO, " %x", PcieDxeConfig->PegRootPortHPE[i]));
  }
  DEBUG ((DEBUG_INFO, "\n"));


  DEBUG ((DEBUG_INFO, "\n------------------------ SA Policy (DXE) print END -----------------\n"));
  DEBUG_CODE_END ();

  return;
}

/**
  Load DXE Config block default for PCIe

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPcieDxeDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  UINT8                  Index;
  PCIE_DXE_CONFIG        *PcieDxeConfig;

  PcieDxeConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "PcieDxeConfig->Header.GuidHob.Name = %g\n", &PcieDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PcieDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PcieDxeConfig->Header.GuidHob.Header.HobLength));
  ///
  /// Initialize the PCIE Configuration
  /// PEG ASPM per port configuration. 4 PEG controllers i.e. 0,1,2,3
  ///
  for (Index = 0; Index < SA_PEG_MAX_FUN; Index++) {
    PcieDxeConfig->PegAspm[Index]       = CpuPcieAspmAutoConfig;
  }

  for (Index = 0; Index < SA_PEG_MAX_FUN; Index++) {
    PcieDxeConfig->PegPwrOpt[Index].LtrEnable            = 1;
    PcieDxeConfig->PegPwrOpt[Index].LtrMaxSnoopLatency   = V_SA_LTR_MAX_SNOOP_LATENCY_VALUE;
    PcieDxeConfig->PegPwrOpt[Index].LtrMaxNoSnoopLatency = V_SA_LTR_MAX_NON_SNOOP_LATENCY_VALUE;
    PcieDxeConfig->PegPwrOpt[Index].ObffEnable           = 1;
  }
}


/**
  Load DXE Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadMemoryDxeDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  MEMORY_DXE_CONFIG        *MemoryDxeConfig;

  MemoryDxeConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "MemoryDxeConfig->Header.GuidHob.Name = %g\n", &MemoryDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "MemoryDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", MemoryDxeConfig->Header.GuidHob.Header.HobLength));
  ///
  /// Initialize the Memory Configuration
  ///
  ///
  /// DIMM SMBus addresses info
  /// Refer to the SpdAddressTable[] mapping rule in DxeSaPolicyLibrary.h
  ///
  MemoryDxeConfig->SpdAddressTable = AllocateZeroPool (sizeof (UINT8) * 4);
  ASSERT (MemoryDxeConfig->SpdAddressTable != NULL);
  if (MemoryDxeConfig->SpdAddressTable != NULL) {
    MemoryDxeConfig->SpdAddressTable[0] = DIMM_SMB_SPD_P0C0D0;
    MemoryDxeConfig->SpdAddressTable[1] = DIMM_SMB_SPD_P0C0D1;
    MemoryDxeConfig->SpdAddressTable[2] = DIMM_SMB_SPD_P0C1D0;
    MemoryDxeConfig->SpdAddressTable[3] = DIMM_SMB_SPD_P0C1D1;
  }
  MemoryDxeConfig->ChannelASlotMap = 0x01;
  MemoryDxeConfig->ChannelBSlotMap = 0x01;
}

GLOBAL_REMOVE_IF_UNREFERENCED COMPONENT_BLOCK_ENTRY  mSaDxeIpBlocks [] = {
  {&gPcieDxeConfigGuid,     sizeof (PCIE_DXE_CONFIG),     PCIE_DXE_CONFIG_REVISION,      LoadPcieDxeDefault},
  {&gMemoryDxeConfigGuid,   sizeof (MEMORY_DXE_CONFIG),   MEMORY_DXE_CONFIG_REVISION,    LoadMemoryDxeDefault}
};


/**
  CreateSaDxeConfigBlocks generates the config blocks of SA DXE Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] SaPolicy               The pointer to get SA  DXE Protocol instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CreateSaDxeConfigBlocks (
  IN OUT  SA_POLICY_PROTOCOL      **SaPolicy
  )
{
  UINT16              TotalBlockSize;
  EFI_STATUS          Status;
  SA_POLICY_PROTOCOL  *SaInitPolicy;
  UINT16              RequiredSize;

  DEBUG ((DEBUG_INFO, "SA Create Dxe Config Blocks\n"));

  SaInitPolicy = NULL;

  TotalBlockSize = GetComponentConfigBlockTotalSize (&mSaDxeIpBlocks[0], sizeof (mSaDxeIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
  TotalBlockSize += VtdGetConfigBlockTotalSizeDxe ();
  TotalBlockSize += GraphicsGetConfigBlockTotalSizeDxe ();
  DEBUG ((DEBUG_INFO, "TotalBlockSize = 0x%x\n", TotalBlockSize));

  RequiredSize = sizeof (CONFIG_BLOCK_TABLE_HEADER) + TotalBlockSize;

  Status = CreateConfigBlockTable (RequiredSize, (VOID *) &SaInitPolicy);
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize Policy Revision
  //
  SaInitPolicy->TableHeader.Header.Revision = SA_POLICY_PROTOCOL_REVISION;
  //
  // Add config blocks.
  //
  Status =  AddComponentConfigBlocks ((VOID *) SaInitPolicy, &mSaDxeIpBlocks[0], sizeof (mSaDxeIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
  ASSERT_EFI_ERROR (Status);

  // Vtd
  Status = VtdAddConfigBlocksDxe((VOID *) SaInitPolicy);
  ASSERT_EFI_ERROR (Status);

  // Gfx
  Status = GraphicsAddConfigBlocksDxe ((VOID *) SaInitPolicy);
  ASSERT_EFI_ERROR (Status);

  //
  // Assignment for returning SaInitPolicy config block base address
  //
  *SaPolicy = SaInitPolicy;
  return Status;
}


/**
  SaInstallPolicyProtocol installs SA Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.
  @param[in] SaPolicy                   The pointer to SA Policy Protocol instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
SaInstallPolicyProtocol (
  IN  EFI_HANDLE                  ImageHandle,
  IN  SA_POLICY_PROTOCOL         *SaPolicy
  )
{
  EFI_STATUS            Status;

  ///
  /// Print SA DXE Policy
  ///
  SaPrintPolicyProtocol (SaPolicy);
  GraphicsDxePolicyPrint (SaPolicy);
  VtdPrintPolicyDxe (SaPolicy);

  ///
  /// Install protocol to to allow access to this Policy.
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gSaPolicyProtocolGuid,
                  SaPolicy,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

