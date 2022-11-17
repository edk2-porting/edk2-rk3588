/** @file
  This file provide services for DXE phase policy default initialization

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "DxeSaPolicyLibrary.h"
#include <Library/ConfigBlockLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED PCIE_ASPM_OVERRIDE_LIST mPcieAspmDevsOverride[] = {
  {0x8086, 0x108b, 0xff, 2, 2},           ///< Tekoa w/o iAMT
  {0x8086, 0x108c, 0x00, 0, 0},           ///< Tekoa A2
  {0x8086, 0x108c, 0xff, 2, 2},           ///< Tekoa others
  {0x8086, 0x109a, 0xff, 2, 2},           ///< Vidalia
  {0x8086, 0x4222, 0xff, 2, 3},           ///< 3945ABG
  {0x8086, 0x4227, 0xff, 2, 3},           ///< 3945ABG
  {0x8086, 0x4228, 0xff, 2, 3},           ///< 3945ABG
  ///
  /// Place structures for known bad OEM/IHV devices here
  ///
  {SA_PCIE_DEV_END_OF_TABLE, 0, 0, 0, 0}  ///< End of table
};

GLOBAL_REMOVE_IF_UNREFERENCED PCIE_LTR_DEV_INFO mPcieLtrDevsOverride[] = {
  ///
  /// Place holder for PCIe devices with correct LTR requirements
  ///
  {SA_PCIE_DEV_END_OF_TABLE, 0, 0, 0, 0}  ///< End of table
};

extern EFI_GUID gGraphicsDxeConfigGuid;
extern EFI_GUID gMemoryDxeConfigGuid;
extern EFI_GUID gMiscDxeConfigGuid;
extern EFI_GUID gPcieDxeConfigGuid;
extern EFI_GUID gVbiosDxeConfigGuid;

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
  GRAPHICS_DXE_CONFIG         *GraphicsDxeConfig;
  PCIE_DXE_CONFIG             *PcieDxeConfig;
  MISC_DXE_CONFIG             *MiscDxeConfig;
  MEMORY_DXE_CONFIG           *MemoryDxeConfig;
  VBIOS_DXE_CONFIG            *VbiosDxeConfig;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *) SaPolicy, &gGraphicsDxeConfigGuid, (VOID *)&GraphicsDxeConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SaPolicy, &gMiscDxeConfigGuid, (VOID *)&MiscDxeConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SaPolicy, &gPcieDxeConfigGuid, (VOID *)&PcieDxeConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SaPolicy, &gMemoryDxeConfigGuid, (VOID *)&MemoryDxeConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SaPolicy, &gVbiosDxeConfigGuid, (VOID *)&VbiosDxeConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG_CODE_BEGIN ();
  INTN  i;

  DEBUG ((DEBUG_INFO, "\n------------------------ SA Policy (DXE) print BEGIN -----------------\n"));
  DEBUG ((DEBUG_INFO, "Revision : %x\n", SaPolicy->TableHeader.Header.Revision));
  ASSERT (SaPolicy->TableHeader.Header.Revision == SA_POLICY_PROTOCOL_REVISION);

  DEBUG ((DEBUG_INFO, "------------------------ SA_MEMORY_CONFIGURATION -----------------\n"));
  DEBUG ((DEBUG_INFO, " SpdAddressTable[%d] :", SA_MC_MAX_SOCKETS));
  for (i = 0; i < SA_MC_MAX_SOCKETS; i++) {
    DEBUG ((DEBUG_INFO, " %x", MemoryDxeConfig->SpdAddressTable[i]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  DEBUG ((DEBUG_INFO, " ChannelASlotMap : %x\n", MemoryDxeConfig->ChannelASlotMap));
  DEBUG ((DEBUG_INFO, " ChannelBSlotMap : %x\n", MemoryDxeConfig->ChannelBSlotMap));
  DEBUG ((DEBUG_INFO, " MrcTimeMeasure  : %x\n", MemoryDxeConfig->MrcTimeMeasure));
  DEBUG ((DEBUG_INFO, " MrcFastBoot     : %x\n", MemoryDxeConfig->MrcFastBoot));

  DEBUG ((DEBUG_INFO, "------------------------ SA_PCIE_CONFIGURATION -----------------\n"));
  DEBUG ((DEBUG_INFO, " PegAspm[%d] :", SA_PEG_MAX_FUN));
  for (i = 0; i < SA_PEG_MAX_FUN; i++) {
    DEBUG ((DEBUG_INFO, " %x", PcieDxeConfig->PegAspm[i]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  DEBUG ((DEBUG_INFO, " PegAspmL0s[%d] :", SA_PEG_MAX_FUN));
  for (i = 0; i < SA_PEG_MAX_FUN; i++) {
    DEBUG ((DEBUG_INFO, " %x", PcieDxeConfig->PegAspmL0s[i]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  DEBUG ((DEBUG_INFO, " PegRootPortHPE[%d] :", SA_PEG_MAX_FUN));
  for (i = 0; i < SA_PEG_MAX_FUN; i++) {
    DEBUG ((DEBUG_INFO, " %x", PcieDxeConfig->PegRootPortHPE[i]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  if (PcieDxeConfig->PcieAspmDevsOverride != NULL) {
    DEBUG ((DEBUG_INFO, "------------------------ PCIE_ASPM_OVERRIDE_LIST -----------------\n"));
    DEBUG ((DEBUG_INFO, " VendorId DeviceId RevId RootApmcMask EndpointApmcMask\n"));
    i = 0;
    while ((PcieDxeConfig->PcieAspmDevsOverride[i].VendorId != SA_PCIE_DEV_END_OF_TABLE) &&
           (i < MAX_PCIE_ASPM_OVERRIDE)) {
      DEBUG ((DEBUG_INFO, " %04x     %04x     %02x    %01x            %01x\n",
              PcieDxeConfig->PcieAspmDevsOverride[i].VendorId,
              PcieDxeConfig->PcieAspmDevsOverride[i].DeviceId,
              PcieDxeConfig->PcieAspmDevsOverride[i].RevId,
              PcieDxeConfig->PcieAspmDevsOverride[i].RootApmcMask,
              PcieDxeConfig->PcieAspmDevsOverride[i].EndpointApmcMask));
      i++;
    }
    DEBUG ((DEBUG_INFO, "------------------------ END_OF_TABLE -----------------------\n"));
  }
  if (PcieDxeConfig->PcieLtrDevsOverride != NULL) {
    DEBUG ((DEBUG_INFO, "------------------------ PCIE_LTR_DEV_INFO -----------------\n"));
    DEBUG ((DEBUG_INFO, " VendorId DeviceId RevId SnoopLatency NonSnoopLatency\n"));
    i = 0;
    while ((PcieDxeConfig->PcieLtrDevsOverride[i].VendorId != SA_PCIE_DEV_END_OF_TABLE) &&
           (i < MAX_PCIE_LTR_OVERRIDE)) {
      DEBUG ((DEBUG_INFO, " %04x     %04x     %02x    %01x            %01x\n",
              PcieDxeConfig->PcieLtrDevsOverride[i].VendorId,
              PcieDxeConfig->PcieLtrDevsOverride[i].DeviceId,
              PcieDxeConfig->PcieLtrDevsOverride[i].RevId,
              PcieDxeConfig->PcieLtrDevsOverride[i].SnoopLatency,
              PcieDxeConfig->PcieLtrDevsOverride[i].NonSnoopLatency));
      i++;
    }
    DEBUG ((DEBUG_INFO, "------------------------ END_OF_TABLE ----------------------\n"));
  }

  for (i = 0; i < SA_PEG_MAX_FUN; i++) {
    DEBUG ((DEBUG_INFO, " PegPwrOpt[%d].LtrEnable            : %x\n", i, PcieDxeConfig->PegPwrOpt[i].LtrEnable));
    DEBUG ((DEBUG_INFO, " PegPwrOpt[%d].LtrMaxSnoopLatency   : %x\n", i, PcieDxeConfig->PegPwrOpt[i].LtrMaxSnoopLatency));
    DEBUG ((DEBUG_INFO, " PegPwrOpt[%d].ObffEnable           : %x\n", i, PcieDxeConfig->PegPwrOpt[i].ObffEnable));
    DEBUG ((DEBUG_INFO, " PegPwrOpt[%d].LtrMaxNoSnoopLatency : %x\n", i, PcieDxeConfig->PegPwrOpt[i].LtrMaxNoSnoopLatency));
  }


  if (VbiosDxeConfig != NULL) {
    DEBUG ((DEBUG_INFO, "------------------------ SA_SG_VBIOS_CONFIGURATION -----------------\n"));
    DEBUG ((DEBUG_INFO, " LoadVbios    : %x\n", VbiosDxeConfig->LoadVbios));
    DEBUG ((DEBUG_INFO, " ExecuteVbios : %x\n", VbiosDxeConfig->ExecuteVbios));
    DEBUG ((DEBUG_INFO, " VbiosSource  : %x\n", VbiosDxeConfig->VbiosSource));
  }


  DEBUG ((DEBUG_INFO, "------------------------ SA_MISC_CONFIGURATION -----------------\n"));
  DEBUG ((DEBUG_INFO, " EnableAbove4GBMmio : %x\n", MiscDxeConfig->EnableAbove4GBMmio));
  DEBUG ((DEBUG_INFO, "\n------------------------ SA Policy (DXE) print END -----------------\n"));
  DEBUG_CODE_END ();

  return;
}

EFI_STATUS
EFIAPI
LoadIgdDxeDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  GRAPHICS_DXE_CONFIG        *GraphicsDxeConfig;

  GraphicsDxeConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "GraphicsDxeConfig->Header.GuidHob.Name = %g\n", &GraphicsDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "GraphicsDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", GraphicsDxeConfig->Header.GuidHob.Header.HobLength));
  ///
  /// Initialize the Graphics configuration
  ///
  GraphicsDxeConfig->PlatformConfig = 1;
  GraphicsDxeConfig->AlsEnable = 2;
  GraphicsDxeConfig->BacklightControlSupport = 2;
  GraphicsDxeConfig->IgdBlcConfig = 2;
  GraphicsDxeConfig->IgdDvmtMemSize = 1;
  GraphicsDxeConfig->GfxTurboIMON = 31;
  ///
  /// <EXAMPLE> Create a static Backlight Brightness Level Duty cycle Mapping Table
  /// Possible 20 entries (example used 11), each 16 bits as follows:
  /// [15] = Field Valid bit, [14:08] = Level in Percentage (0-64h), [07:00] = Desired duty cycle (0 - FFh).
  ///
  GraphicsDxeConfig->BCLM[0] = (0x0000 + WORD_FIELD_VALID_BIT);  ///< 0%
  GraphicsDxeConfig->BCLM[1] = (0x0A19 + WORD_FIELD_VALID_BIT);  ///< 10%
  GraphicsDxeConfig->BCLM[2] = (0x1433 + WORD_FIELD_VALID_BIT);  ///< 20%
  GraphicsDxeConfig->BCLM[3] = (0x1E4C + WORD_FIELD_VALID_BIT);  ///< 30%
  GraphicsDxeConfig->BCLM[4] = (0x2866 + WORD_FIELD_VALID_BIT);  ///< 40%
  GraphicsDxeConfig->BCLM[5] = (0x327F + WORD_FIELD_VALID_BIT);  ///< 50%
  GraphicsDxeConfig->BCLM[6] = (0x3C99 + WORD_FIELD_VALID_BIT);  ///< 60%
  GraphicsDxeConfig->BCLM[7] = (0x46B2 + WORD_FIELD_VALID_BIT);  ///< 70%
  GraphicsDxeConfig->BCLM[8] = (0x50CC + WORD_FIELD_VALID_BIT);  ///< 80%
  GraphicsDxeConfig->BCLM[9] = (0x5AE5 + WORD_FIELD_VALID_BIT);  ///< 90%
  GraphicsDxeConfig->BCLM[10] = (0x64FF + WORD_FIELD_VALID_BIT);  ///< 100%

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
LoadPcieDxeDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  UINT8                  pegFn;
  UINT8                  Index;
  PCIE_DXE_CONFIG        *PcieDxeConfig;

  PcieDxeConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "PcieDxeConfig->Header.GuidHob.Name = %g\n", &PcieDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PcieDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PcieDxeConfig->Header.GuidHob.Header.HobLength));
  ///
  /// Initialize the PCIE Configuration
  /// PEG ASPM per port configuration. 4 PEG controllers i.e. 0,1,2,3
  ///
  for (pegFn = 0; pegFn < SA_PEG_MAX_FUN; pegFn++) {
    PcieDxeConfig->PegAspm[pegFn]       = PcieAspmAutoConfig;
    PcieDxeConfig->PegAspmL0s[pegFn]    = 0;
  }

  for (Index = 0; Index < SA_PEG_MAX_FUN; Index++) {
    PcieDxeConfig->PegPwrOpt[Index].LtrEnable            = 1;
    PcieDxeConfig->PegPwrOpt[Index].LtrMaxSnoopLatency   = V_SA_LTR_MAX_SNOOP_LATENCY_VALUE;
    PcieDxeConfig->PegPwrOpt[Index].LtrMaxNoSnoopLatency = V_SA_LTR_MAX_NON_SNOOP_LATENCY_VALUE;
    PcieDxeConfig->PegPwrOpt[Index].ObffEnable           = 1;
  }

  PcieDxeConfig->PcieAspmDevsOverride = mPcieAspmDevsOverride;
  PcieDxeConfig->PcieLtrDevsOverride = mPcieLtrDevsOverride;
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
LoadMiscDxeDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  MISC_DXE_CONFIG        *MiscDxeConfig;

  MiscDxeConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "MiscDxeConfig->Header.GuidHob.Name = %g\n", &MiscDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "MiscDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", MiscDxeConfig->Header.GuidHob.Header.HobLength));
  ///
  /// RMRR Base and Limit Address for USB
  ///
  MiscDxeConfig->RmrrUsbBaseAddress = AllocateZeroPool (sizeof (EFI_PHYSICAL_ADDRESS) * 2);
  ASSERT (MiscDxeConfig->RmrrUsbBaseAddress != NULL);
  if (MiscDxeConfig->RmrrUsbBaseAddress != NULL) {
    ///
    /// BIOS must update USB RMRR base address
    ///
    MiscDxeConfig->RmrrUsbBaseAddress[0] = 0;
    MiscDxeConfig->RmrrUsbBaseAddress[1] = 0;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
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
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
LoadVbiosDxeDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  VBIOS_DXE_CONFIG        *VbiosDxeConfig;

  VbiosDxeConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "VbiosDxeConfig->Header.GuidHob.Name = %g\n", &VbiosDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "VbiosDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", VbiosDxeConfig->Header.GuidHob.Header.HobLength));
  ///
  /// Initialize the SG VBIOS DXE Policies
  ///
  ///
  /// 1 = secondary display device VBIOS Source is PCI Card
  /// 0 = secondary display device VBIOS Source is FW Volume
  ///
  VbiosDxeConfig->VbiosSource = 1;
  return EFI_SUCCESS;
}

/**
  LoadSaDxeConfigBlockDefault - Initialize default settings for each SA Config block

  @param[in] ConfigBlockPointer         The buffer pointer that will be initialized as specific config block
  @param[in] BlockId                    Request to initialize defaults of specified config block by given Block ID

  @retval EFI_SUCCESS                   The given buffer has contained the defaults of requested config block
  @retval EFI_NOT_FOUND                 Block ID is not defined so no default Config block will be initialized
**/
EFI_STATUS
EFIAPI
LoadSaDxeConfigBlockDefault (
  IN   VOID          *ConfigBlockPointer,
  IN   EFI_GUID      BlockGuid
  )
{
  if (CompareGuid (&BlockGuid, &gGraphicsDxeConfigGuid)) {
    LoadIgdDxeDefault (ConfigBlockPointer);
  } else if (CompareGuid (&BlockGuid, &gMiscDxeConfigGuid)) {
    LoadMiscDxeDefault (ConfigBlockPointer);
  } else if (CompareGuid (&BlockGuid, &gPcieDxeConfigGuid)) {
    LoadPcieDxeDefault (ConfigBlockPointer);
  } else if (CompareGuid (&BlockGuid, &gMemoryDxeConfigGuid)) {
    LoadMemoryDxeDefault (ConfigBlockPointer);
  } else if (CompareGuid (&BlockGuid, &gVbiosDxeConfigGuid)) {
    LoadVbiosDxeDefault (ConfigBlockPointer);
  } else {
    return EFI_NOT_FOUND;
  }
  return EFI_SUCCESS;
}


/**
  CreateSaDxeConfigBlocks generates the config blocksg of SA DXE Policy.
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
  UINT16                            TotalBlockSize;
  UINT16                            TotalBlockCount;
  UINT16                            BlockCount;
  VOID                              *ConfigBlockPointer;
  EFI_STATUS                        Status;
  SA_POLICY_PROTOCOL                *SaInitPolicy;
  UINT16                            RequiredSize;
  STATIC CONFIG_BLOCK_HEADER        SaDxeIpBlocks [] = {
       {{{0, sizeof (GRAPHICS_DXE_CONFIG),    0},  {0}},     GRAPHICS_DXE_CONFIG_REVISION,           0, {0, 0}},
       {{{0, sizeof (MEMORY_DXE_CONFIG), 0},  {0}},     MEMORY_DXE_CONFIG_REVISION,        0, {0, 0}},
       {{{0, sizeof (MISC_DXE_CONFIG),   0},  {0}},     MISC_DXE_CONFIG_REVISION,          0, {0, 0}},
       {{{0, sizeof (PCIE_DXE_CONFIG),   0},  {0}},     PCIE_DXE_CONFIG_REVISION,          0, {0, 0}},
       {{{0, sizeof (VBIOS_DXE_CONFIG),  0},  {0}},     VBIOS_DXE_CONFIG_REVISION,         0, {0, 0}}
  };

  SaInitPolicy = NULL;
  TotalBlockCount = sizeof (SaDxeIpBlocks) / sizeof (CONFIG_BLOCK_HEADER);
  DEBUG ((DEBUG_INFO, "TotalBlockCount = 0x%x\n", TotalBlockCount));

  TotalBlockSize = 0;
  for (BlockCount = 0 ; BlockCount < TotalBlockCount; BlockCount++) {
    TotalBlockSize += (UINT32) SaDxeIpBlocks[BlockCount].GuidHob.Header.HobLength;
    DEBUG ((DEBUG_INFO, "TotalBlockSize after adding  Block[0x%x]= 0x%x\n", BlockCount, TotalBlockSize));
  }

  RequiredSize = sizeof (CONFIG_BLOCK_TABLE_HEADER) + TotalBlockSize;

  Status = CreateConfigBlockTable (RequiredSize, (VOID *)&SaInitPolicy);
  ASSERT_EFI_ERROR (Status);
  //
  // Initalize SklSaIpBlocks table GUID
  //
  CopyMem (&SaDxeIpBlocks[0].GuidHob.Name,  &gGraphicsDxeConfigGuid, sizeof (EFI_GUID));
  CopyMem (&SaDxeIpBlocks[1].GuidHob.Name,  &gMemoryDxeConfigGuid,   sizeof (EFI_GUID));
  CopyMem (&SaDxeIpBlocks[2].GuidHob.Name,  &gMiscDxeConfigGuid,     sizeof (EFI_GUID));
  CopyMem (&SaDxeIpBlocks[3].GuidHob.Name,  &gPcieDxeConfigGuid,     sizeof (EFI_GUID));
  CopyMem (&SaDxeIpBlocks[4].GuidHob.Name,  &gVbiosDxeConfigGuid,    sizeof (EFI_GUID));

  //
  // Initialize Policy Revision
  //
  SaInitPolicy->TableHeader.Header.Revision = SA_POLICY_PROTOCOL_REVISION;
  //
  // Initialize ConfigBlockPointer to NULL
  //
  ConfigBlockPointer = NULL;
  //
  // Loop to identify each config block from SaDxeIpBlocks[] Table and add each of them
  //
  for (BlockCount = 0 ; BlockCount < TotalBlockCount; BlockCount++) {
    ConfigBlockPointer = (VOID *)&SaDxeIpBlocks[BlockCount];
    Status = AddConfigBlock ((VOID *) SaInitPolicy, (VOID *)&ConfigBlockPointer);
    ASSERT_EFI_ERROR (Status);
    LoadSaDxeConfigBlockDefault ((VOID *) ConfigBlockPointer, SaDxeIpBlocks[BlockCount].GuidHob.Name);
  }
  //
  // Assignment for returning SaPolicy config block base address
  //
  *SaPolicy = SaInitPolicy;
  return EFI_SUCCESS;
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

