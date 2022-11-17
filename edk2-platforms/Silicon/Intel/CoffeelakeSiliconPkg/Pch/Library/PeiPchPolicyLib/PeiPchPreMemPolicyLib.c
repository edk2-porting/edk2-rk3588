/** @file
  This file is PeiPchPreMemPolicy library.

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiPchPolicyLibrary.h"
#include <Library/CpuPlatformLib.h>
#include <Library/ConfigBlockLib.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPchGeneralPreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_GENERAL_PREMEM_CONFIG  *PchGeneralPreMemConfig;
  PchGeneralPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "PchGeneralPreMemConfig->Header.GuidHob.Name = %g\n", &PchGeneralPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PchGeneralPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PchGeneralPreMemConfig->Header.GuidHob.Header.HobLength));

  /********************************
    PCH general premem configuration
  ********************************/
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadDciPreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_DCI_PREMEM_CONFIG  *DciPreMemConfig;
  DciPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "DciPreMemConfig->Header.GuidHob.Name = %g\n", &DciPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "DciPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", DciPreMemConfig->Header.GuidHob.Header.HobLength));

  /********************************
    DCI Configuration
  ********************************/
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadWatchDogPreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_WDT_PREMEM_CONFIG       *WdtPreMemConfig;
  WdtPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "WdtPreMemConfig->Header.GuidHob.Name = %g\n", &WdtPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "WdtPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", WdtPreMemConfig->Header.GuidHob.Header.HobLength));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPchTraceHubPreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_TRACE_HUB_PREMEM_CONFIG  *PchTraceHubPreMemConfig;
  PchTraceHubPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "PchTraceHubPreMemConfig->Header.GuidHob.Name = %g\n", &PchTraceHubPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PchTraceHubPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PchTraceHubPreMemConfig->Header.GuidHob.Header.HobLength));
}

GLOBAL_REMOVE_IF_UNREFERENCED UINT8 mSmbusRsvdAddresses[] = {
  0xA0,
  0xA2,
  0xA4,
  0xA6
};

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadSmbusPreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_SMBUS_PREMEM_CONFIG  *SmbusPreMemConfig;
  SmbusPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "SmbusPreMemConfig->Header.GuidHob.Name = %g\n", &SmbusPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "SmbusPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", SmbusPreMemConfig->Header.GuidHob.Header.HobLength));

  /********************************
    SMBus configuration
  ********************************/
  SmbusPreMemConfig->Enable                = TRUE;
  SmbusPreMemConfig->DynamicPowerGating    = TRUE;
  SmbusPreMemConfig->SpdWriteDisable       = TRUE;
  SmbusPreMemConfig->SmbusIoBase           = PcdGet16 (PcdSmbusBaseAddress);
  ASSERT (sizeof (mSmbusRsvdAddresses) <= PCH_MAX_SMBUS_RESERVED_ADDRESS);
  SmbusPreMemConfig->NumRsvdSmbusAddresses = sizeof (mSmbusRsvdAddresses);
  CopyMem (
    SmbusPreMemConfig->RsvdSmbusAddressTable,
    mSmbusRsvdAddresses,
    sizeof (mSmbusRsvdAddresses)
    );
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadLpcPreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_LPC_PREMEM_CONFIG  *LpcPreMemConfig;
  LpcPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "LpcPreMemConfig->Header.GuidHob.Name = %g\n", &LpcPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "LpcPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", LpcPreMemConfig->Header.GuidHob.Header.HobLength));

  /********************************
    LPC Configuration
  ********************************/
  LpcPreMemConfig->EnhancePort8xhDecoding     = TRUE;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadHsioPciePreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_HSIO_PCIE_PREMEM_CONFIG  *HsioPciePreMemConfig;
  HsioPciePreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "HsioPciePreMemConfig->Header.GuidHob.Name = %g\n", &HsioPciePreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "HsioPciePreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", HsioPciePreMemConfig->Header.GuidHob.Header.HobLength));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadHsioSataPreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_HSIO_SATA_PREMEM_CONFIG  *HsioSataPreMemConfig;
  HsioSataPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "HsioSataPreMemConfig->Header.GuidHob.Name = %g\n", &HsioSataPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "HsioSataPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", HsioSataPreMemConfig->Header.GuidHob.Header.HobLength));
}

/**
  Get Hsio Sata Pre Mem Config Policy

  @param[in]  SiPolicy            The RC Policy PPI instance
  @param[in]  SataCtrlIndex       SATA controller index

  @retval     Pointer to Hsio Sata Pre Mem Config Policy
**/
PCH_HSIO_SATA_PREMEM_CONFIG *
GetPchHsioSataPreMemConfig (
  IN SI_PREMEM_POLICY_PPI *SiPreMemPolicy,
  IN UINT32               SataCtrlIndex
  )
{
  PCH_HSIO_SATA_PREMEM_CONFIG     *HsioSataPreMemConfig;
  EFI_STATUS                      Status;

  ASSERT (SataCtrlIndex < GetPchMaxSataControllerNum ());

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHsioSataPreMemConfigGuid, (VOID *) &HsioSataPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  HsioSataPreMemConfig += SataCtrlIndex;

  return HsioSataPreMemConfig;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPcieRpPreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_PCIE_RP_PREMEM_CONFIG  *PcieRpPreMemConfig;
  UINT32                     RpIndex;

  PcieRpPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "PcieRpPreMemConfig->Header.GuidHob.Name = %g\n", &PcieRpPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PcieRpPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PcieRpPreMemConfig->Header.GuidHob.Header.HobLength));

  for (RpIndex = 0; RpIndex < GetPchMaxPciePortNum (); RpIndex ++) {
    PcieRpPreMemConfig->RpEnabledMask |= (UINT32) (1 << RpIndex);
  }
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadHdAudioPreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_HDAUDIO_PREMEM_CONFIG  *HdaPreMemConfig;
  HdaPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "PcieRpPreMemConfig->Header.GuidHob.Name = %g\n", &HdaPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PcieRpPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", HdaPreMemConfig->Header.GuidHob.Header.HobLength));
  HdaPreMemConfig->Enable = 1;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadIshPreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_ISH_PREMEM_CONFIG  *IshPreMemConfig;
  IshPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "PcieRpPreMemConfig->Header.GuidHob.Name = %g\n", &IshPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PcieRpPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", IshPreMemConfig->Header.GuidHob.Header.HobLength));
  //Enable ISH controller for Non-Desktop sku platforms
  if (GetCpuSku () != EnumCpuTrad) {
    IshPreMemConfig->Enable = TRUE;
  }
}


GLOBAL_REMOVE_IF_UNREFERENCED COMPONENT_BLOCK_ENTRY  mPchIpBlocksPreMem [] = {
  {&gPchGeneralPreMemConfigGuid,     sizeof (PCH_GENERAL_PREMEM_CONFIG),    PCH_GENERAL_PREMEM_CONFIG_REVISION,        LoadPchGeneralPreMemConfigDefault},
  {&gDciPreMemConfigGuid,            sizeof (PCH_DCI_PREMEM_CONFIG),        DCI_PREMEM_CONFIG_REVISION,                LoadDciPreMemConfigDefault},
  {&gWatchDogPreMemConfigGuid,       sizeof (PCH_WDT_PREMEM_CONFIG),        WATCH_DOG_PREMEM_CONFIG_REVISION,          LoadWatchDogPreMemConfigDefault},
  {&gPchTraceHubPreMemConfigGuid,    sizeof (PCH_TRACE_HUB_PREMEM_CONFIG),  PCH_TRACEHUB_PREMEM_CONFIG_REVISION,       LoadPchTraceHubPreMemConfigDefault},
  {&gSmbusPreMemConfigGuid,          sizeof (PCH_SMBUS_PREMEM_CONFIG),      SMBUS_PREMEM_CONFIG_REVISION,              LoadSmbusPreMemConfigDefault},
  {&gLpcPreMemConfigGuid,            sizeof (PCH_LPC_PREMEM_CONFIG),        LPC_PREMEM_CONFIG_REVISION,                LoadLpcPreMemConfigDefault},
  {&gHsioPciePreMemConfigGuid,       sizeof (PCH_HSIO_PCIE_PREMEM_CONFIG),  HSIO_PCIE_PREMEM_CONFIG_REVISION,          LoadHsioPciePreMemConfigDefault},
  {&gHsioSataPreMemConfigGuid,       sizeof (PCH_HSIO_SATA_PREMEM_CONFIG),  HSIO_SATA_PREMEM_CONFIG_REVISION,          LoadHsioSataPreMemConfigDefault},
  {&gPcieRpPreMemConfigGuid,         sizeof (PCH_PCIE_RP_PREMEM_CONFIG),    PCIE_RP_PREMEM_CONFIG_REVISION,            LoadPcieRpPreMemConfigDefault},
  {&gHdAudioPreMemConfigGuid,        sizeof (PCH_HDAUDIO_PREMEM_CONFIG),    HDAUDIO_PREMEM_CONFIG_REVISION,            LoadHdAudioPreMemConfigDefault},
  {&gIshPreMemConfigGuid,            sizeof (PCH_ISH_PREMEM_CONFIG),        ISH_PREMEM_CONFIG_REVISION,                LoadIshPreMemConfigDefault},
};

/**
  Get PCH PREMEM config block table total size.

  @retval                               Size of PCH PREMEM config block table
**/
UINT16
EFIAPI
PchGetPreMemConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mPchIpBlocksPreMem[0], sizeof (mPchIpBlocksPreMem) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  PchAddPreMemConfigBlocks add all PCH PREMEM config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add PCH PREMEM config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PchAddPreMemConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  )
{
  DEBUG ((DEBUG_INFO, "PCH AddPreMemConfigBlocks\n"));

  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mPchIpBlocksPreMem[0], sizeof (mPchIpBlocksPreMem) / sizeof (COMPONENT_BLOCK_ENTRY));
}
