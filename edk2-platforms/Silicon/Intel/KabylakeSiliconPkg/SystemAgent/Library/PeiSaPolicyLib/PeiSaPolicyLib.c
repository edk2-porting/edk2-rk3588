/** @file
  This file provides services for PEI policy default initialization

Copyright (c) 2017 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "PeiSaPolicyLibrary.h"
#include <Library/GpioLib.h>
#include <GpioPinsSklH.h>
#include <Library/CpuPlatformLib.h>
#include <MrcTypes.h>
#include "MrcInterface.h"


extern EFI_GUID gSaMiscPeiPreMemConfigGuid;
extern EFI_GUID gMemoryConfigGuid;
extern EFI_GUID gMemoryConfigNoCrcGuid;
extern EFI_GUID gGraphicsPeiConfigGuid;
extern EFI_GUID gVtdConfigGuid;

#define DEFAULT_OPTION_ROM_TEMP_BAR            0x80000000
#define DEFAULT_OPTION_ROM_TEMP_MEM_LIMIT      0xC0000000

//
// Function call to Load defaults for Individial IP Blocks
//
VOID
LoadSaMiscPeiPreMemDefault (
  IN   VOID          *ConfigBlockPointer
  )
{
  SA_MISC_PEI_PREMEM_CONFIG            *MiscPeiPreMemConfig;

  MiscPeiPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "MiscPeiPreMemConfig->Header.GuidHob.Name = %g\n", &MiscPeiPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "MiscPeiPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", MiscPeiPreMemConfig->Header.GuidHob.Header.HobLength));

  //
  // Policy initialization commented out here is because it's the same with default 0 and no need to re-do again.
  //
  MiscPeiPreMemConfig->LockPTMregs                      = 1;

  //
  // Initialize the Platform Configuration
  //
  MiscPeiPreMemConfig->MchBar              = (UINT32) PcdGet64 (PcdMchBaseAddress);
  MiscPeiPreMemConfig->DmiBar              = 0xFED18000;
  MiscPeiPreMemConfig->EpBar               = 0xFED19000;
  MiscPeiPreMemConfig->EdramBar            = 0xFED80000;
  MiscPeiPreMemConfig->SmbusBar            = PcdGet16 (PcdSmbusBaseAddress);
  MiscPeiPreMemConfig->TsegSize            = PcdGet32 (PcdTsegSize);
  MiscPeiPreMemConfig->GdxcBar             = 0xFED84000;

  //
  // Initialize the Switchable Graphics Default Configuration
  //
  MiscPeiPreMemConfig->SgDelayAfterHoldReset  = 100; //100ms
  MiscPeiPreMemConfig->SgDelayAfterPwrEn      = 300; //300ms
  MiscPeiPreMemConfig->SgDelayAfterOffMethod  = 0;
  MiscPeiPreMemConfig->SgDelayAfterLinkEnable = 0;
  MiscPeiPreMemConfig->SgGenSpeedChangeEnable = 0;

  ///
  /// Initialize the DataPtr for S3 resume
  ///
  MiscPeiPreMemConfig->S3DataPtr = NULL;
  MiscPeiPreMemConfig->OpRomScanTempMmioBar      = DEFAULT_OPTION_ROM_TEMP_BAR;
  MiscPeiPreMemConfig->OpRomScanTempMmioLimit    = DEFAULT_OPTION_ROM_TEMP_MEM_LIMIT;
}

VOID
LoadVtdDefault (
  IN VOID   *ConfigBlockPointer
  )
{
  VTD_CONFIG   *Vtd;

  Vtd = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "Vtd->Header.GuidHob.Name = %g\n", &Vtd->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "Vtd->Header.GuidHob.Header.HobLength = 0x%x\n", Vtd->Header.GuidHob.Header.HobLength));

  //
  // Initialize the Vtd Configuration
  //
  Vtd->BaseAddress[0]  = 0xFED90000;
  Vtd->BaseAddress[1]  = 0xFED91000;
}

VOID
LoadGraphichsPeiDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  GRAPHICS_PEI_CONFIG                         *GtConfig;

  GtConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "GtConfig->Header.GuidHob.Name = %g\n", &GtConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "GtConfig->Header.GuidHob.Header.HobLength = 0x%x\n", GtConfig->Header.GuidHob.Header.HobLength));
}

VOID
LoadMemConfigNoCrcDefault (
  IN VOID    *ConfigBlockPointer
  )
{

  MEMORY_CONFIG_NO_CRC                    *MemConfigNoCrc;

  MemConfigNoCrc = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "MemConfigNoCrc->Header.GuidHob.Name = %g\n", &MemConfigNoCrc->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "MemConfigNoCrc->Header.GuidHob.Header.HobLength = 0x%x\n", MemConfigNoCrc->Header.GuidHob.Header.HobLength));
  //
  // Allocating memory space for pointer structures inside MemConfigNoCrc
  //
  MemConfigNoCrc->SpdData = (SPD_DATA_BUFFER *) AllocateZeroPool (sizeof (SPD_DATA_BUFFER));
  ASSERT (MemConfigNoCrc->SpdData != NULL);
  if (MemConfigNoCrc->SpdData == NULL) {
    return;
  }

  MemConfigNoCrc->DqByteMap = (SA_MEMORY_DQ_MAPPING *) AllocateZeroPool (sizeof (SA_MEMORY_DQ_MAPPING));
  ASSERT (MemConfigNoCrc->DqByteMap != NULL);
  if (MemConfigNoCrc->DqByteMap == NULL) {
    return;
  }

  MemConfigNoCrc->DqsMap = (SA_MEMORY_DQS_MAPPING *) AllocateZeroPool (sizeof (SA_MEMORY_DQS_MAPPING));
  ASSERT (MemConfigNoCrc->DqsMap != NULL);
  if (MemConfigNoCrc->DqsMap == NULL) {
    return;
  }

  MemConfigNoCrc->RcompData = (SA_MEMORY_RCOMP *) AllocateZeroPool (sizeof (SA_MEMORY_RCOMP));
  ASSERT (MemConfigNoCrc->RcompData != NULL);
  if (MemConfigNoCrc->RcompData == NULL) {
    return;
  }

  //
  // Set PlatformMemory Size
  //
  MemConfigNoCrc->PlatformMemorySize = PcdGet32 (PcdPeiMinMemorySize);

  MemConfigNoCrc->SerialDebugLevel  = 3;  //< Enable MRC debug message

}


VOID
LoadMemConfigDefault (
  IN VOID *ConfigBlockPointer
  )
{
  MEMORY_CONFIGURATION                    *MemConfig;
  CPU_SKU                                 CpuSku;
  UINT16                                  DeviceId;
  BOOLEAN                                 HasEdram;

  CpuSku = GetCpuSku ();
  DeviceId    = MmioRead16 (MmPciBase (SA_MC_BUS, 0, 0) + R_SA_MC_DEVICE_ID);
  HasEdram    = ((AsmReadMsr64 (MSR_PLATFORM_INFO) & B_PLATFORM_INFO_EDRAM_EN) != 0);


  MemConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "MemConfig->Header.GuidHob.Name = %g\n", &MemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "MemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", MemConfig->Header.GuidHob.Header.HobLength));
  //
  // Initialize the Memory Configuration
  //
  MemConfig->EccSupport         = 1;
  MemConfig->ScramblerSupport   = 1;
  MemConfig->PowerDownMode      = 0xFF;
  MemConfig->RankInterleave     = TRUE;
  MemConfig->EnhancedInterleave = TRUE;
  MemConfig->CmdTriStateDis     = 0;
  MemConfig->EnCmdRate          = 3;
  MemConfig->AutoSelfRefreshSupport = TRUE;
  MemConfig->ExtTemperatureSupport  = TRUE;

  //
  // Thermal Management Configuration
  //
  MemConfig->ThermalManagement  = 1;
  //
  // Channel Hash Configuration
  //
  MemConfig->ChHashEnable         = TRUE;
  MemConfig->ChHashMask           = ((CpuSku == EnumCpuUlt) && (HasEdram)) ? 0x30D0 : 0x30C8;
  MemConfig->ChHashInterleaveBit  = 2;
  //
  // Options for Thermal settings
  //
  MemConfig->EnablePwrDn            = 1;
  MemConfig->EnablePwrDnLpddr       = 1;
  MemConfig->DdrThermalSensor     = 1;

  MemConfig->EnergyScaleFact        = 3;
  MemConfig->WarmThresholdCh0Dimm0  = 0xFF;
  MemConfig->WarmThresholdCh0Dimm1  = 0xFF;
  MemConfig->WarmThresholdCh1Dimm0  = 0xFF;
  MemConfig->WarmThresholdCh1Dimm1  = 0xFF;
  MemConfig->HotThresholdCh0Dimm0   = 0xFF;
  MemConfig->HotThresholdCh0Dimm1   = 0xFF;
  MemConfig->HotThresholdCh1Dimm0   = 0xFF;
  MemConfig->HotThresholdCh1Dimm1   = 0xFF;
  MemConfig->WarmBudgetCh0Dimm0     = 0xFF;
  MemConfig->WarmBudgetCh0Dimm1     = 0xFF;
  MemConfig->WarmBudgetCh1Dimm0     = 0xFF;
  MemConfig->WarmBudgetCh1Dimm1     = 0xFF;
  MemConfig->HotBudgetCh0Dimm0      = 0xFF;
  MemConfig->HotBudgetCh0Dimm1      = 0xFF;
  MemConfig->HotBudgetCh1Dimm0      = 0xFF;
  MemConfig->HotBudgetCh1Dimm1      = 0xFF;

  MemConfig->SrefCfgEna             = 1;
  MemConfig->SrefCfgIdleTmr         = 0x200;
  MemConfig->ThrtCkeMinTmr          = 0x30;
  MemConfig->ThrtCkeMinDefeatLpddr = 1;
  MemConfig->ThrtCkeMinTmrLpddr    = 0x40;

  //
  // CA Vref routing: board-dependent
  // 0 - VREF_CA goes to both CH_A and CH_B (LPDDR3/DDR3L)
  // 1 - VREF_CA to CH_A, VREF_DQ_A to CH_B (should not be used)
  // 2 - VREF_CA to CH_A, VREF_DQ_B to CH_B (DDR4)
  //
  //MemConfig->CaVrefConfig = 0;

  MemConfig->VttTermination     = ((CpuSku == EnumCpuUlx) || (CpuSku == EnumCpuUlt));
  MemConfig->VttCompForVsshi    = 1;

  MemConfig->McLock               = TRUE;

  MemConfig->GdxcIotSize          = 4;
  MemConfig->GdxcMotSize          = 12;

  //
  // MRC training steps
  //
  MemConfig->ERDMPRTC2D           = 1;
  MemConfig->SOT                  = 1;
  MemConfig->RDMPRT               = 1;
  MemConfig->RCVET                = 1;
  MemConfig->JWRL                 = 1;
  MemConfig->EWRTC2D              = 1;
  MemConfig->ERDTC2D              = 1;
  MemConfig->WRTC1D               = 1;
  MemConfig->WRVC1D               = 1;
  MemConfig->RDTC1D               = 1;
  MemConfig->DIMMODTT             = 1;
  MemConfig->DIMMRONT             = 1;
  MemConfig->WRSRT                = 1;
  MemConfig->RDODTT               = 1;
  MemConfig->RDEQT                = 1;
  MemConfig->RDAPT                = 1;
  MemConfig->WRTC2D               = 1;
  MemConfig->RDTC2D               = 1;
  MemConfig->CMDVC                = 1;
  MemConfig->WRVC2D               = 1;
  MemConfig->RDVC2D               = 1;
  MemConfig->LCT                  = 1;
  MemConfig->RTL                  = 1;
  MemConfig->TAT                  = 1;
  MemConfig->ALIASCHK             = 1;
  MemConfig->RCVENC1D             = 1;
  MemConfig->RMC                  = 1;
  MemConfig->CMDSR                = 1;
  MemConfig->CMDDSEQ              = 1;
  MemConfig->CMDNORM              = 1;
  MemConfig->EWRDSEQ              = 1;

  // MrcFastBoot enabled by default
  MemConfig->MrcFastBoot          = TRUE;
  MemConfig->RemapEnable          = TRUE;
  MemConfig->BClkFrequency        = 100 * 1000 * 1000;

  MemConfig->Vc1ReadMeter           = TRUE;
  MemConfig->Vc1ReadMeterTimeWindow = 0x320;
  MemConfig->Vc1ReadMeterThreshold  = 0x118;
  MemConfig->StrongWkLeaker         = 7;

  MemConfig->MobilePlatform     = (IS_SA_DEVICE_ID_MOBILE (DeviceId)) ? TRUE : FALSE;
  MemConfig->PciIndex           = 0xCF8;
  MemConfig->PciData            = 0xCFC;
  MemConfig->CkeRankMapping     = 0xAA;
  //
  // SA GV: 0 - Disabled, 1 - FixedLow, 2 - FixedHigh, 3 - Enabled
  //
  //
  // Current Simics will fail in MRC training when SAGV enabled so need to by default disable SAGV.
  //
  MemConfig->SaGv               = 3;  // This only affects ULX/ULT and CPU steppings C0/J0 and above; otherwise SA GV is disabled.

  MemConfig->Idd3n              = 26;
  MemConfig->Idd3p              = 11;

  MemConfig->RhPrevention       = TRUE;         // Row Hammer prevention.
  MemConfig->RhSolution         = HardwareRhp;  // Type of solution to be used for RHP - 0/1 = HardwareRhp/Refresh2x
  MemConfig->RhActProbability   = OneIn2To11;    // Activation probability for Hardware RHP

  MemConfig->LpddrMemWriteLatencySet = 1;  // Enable LPDDR3 WL Set B if supported by DRAM
  MemConfig->EvLoaderDelay           = 1;

  MemConfig->DllBwEn1 = 1;
  MemConfig->DllBwEn2 = 2;
  MemConfig->DllBwEn3 = 2;

  MemConfig->RetrainOnFastFail = 1; //  Restart MRC in Cold mode if SW MemTest fails during Fast flow. 0 = Disabled, 1 = Enabled
}


static COMPONENT_BLOCK_ENTRY  mSaIpBlocksPreMem[] = {
  { &gSaMiscPeiPreMemConfigGuid, sizeof(SA_MISC_PEI_PREMEM_CONFIG), SA_MISC_PEI_PREMEM_CONFIG_REVISION, LoadSaMiscPeiPreMemDefault },
  { &gMemoryConfigGuid, sizeof(MEMORY_CONFIGURATION), MEMORY_CONFIG_REVISION, LoadMemConfigDefault },
  { &gMemoryConfigNoCrcGuid, sizeof(MEMORY_CONFIG_NO_CRC), MEMORY_CONFIG_REVISION, LoadMemConfigNoCrcDefault }
};

static COMPONENT_BLOCK_ENTRY  mSaIpBlocks [] = {
  {&gGraphicsPeiConfigGuid,     sizeof (GRAPHICS_PEI_CONFIG),  GRAPHICS_PEI_CONFIG_REVISION,     LoadGraphichsPeiDefault},
  {&gVtdConfigGuid,             sizeof (VTD_CONFIG),           VTD_CONFIG_REVISION,              LoadVtdDefault}
};

/**
  Get SA config block table total size.

  @retval     Size of SA config block table
**/
UINT16
EFIAPI
SaGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mSaIpBlocks[0], sizeof (mSaIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  Get SA config block table total size.

  @retval      Size of SA config block table
**/
UINT16
EFIAPI
SaGetConfigBlockTotalSizePreMem (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mSaIpBlocksPreMem[0], sizeof (mSaIpBlocksPreMem) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  SaAddConfigBlocksPreMem add all SA config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add SA config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SaAddConfigBlocksPreMem (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "SA AddConfigBlocks. TotalBlockCount = 0x%x\n",  sizeof (mSaIpBlocksPreMem) / sizeof (COMPONENT_BLOCK_ENTRY)));
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mSaIpBlocksPreMem[0], sizeof (mSaIpBlocksPreMem) / sizeof (COMPONENT_BLOCK_ENTRY));
  if (Status == EFI_SUCCESS) {
    SaLoadSamplePolicyPreMem (ConfigBlockTableAddress);
  }
  return Status;
}

/**
  SaAddConfigBlocks add all SA config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add SA config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SaAddConfigBlocks (
  IN VOID           *ConfigBlockTableAddress
  )
{
  DEBUG ((DEBUG_INFO, "SA AddConfigBlocks. TotalBlockCount = 0x%x\n",  sizeof (mSaIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY)));

  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mSaIpBlocks[0], sizeof (mSaIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}
