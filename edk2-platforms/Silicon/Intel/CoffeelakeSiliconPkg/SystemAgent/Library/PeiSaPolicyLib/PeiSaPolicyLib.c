/** @file
  This file provides services for PEI policy default initialization

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiSaPolicyLibrary.h"
#include <Library/GpioLib.h>
#include <Library/CpuPlatformLib.h>
#include "MrcInterface.h"
#include <Library/PchInfoLib.h>

#define DEFAULT_OPTION_ROM_TEMP_BAR            0x80000000
#define DEFAULT_OPTION_ROM_TEMP_MEM_LIMIT      0xC0000000
//
// Need minimum of 48MB during PEI phase for IAG and some buffer for boot.
//
#define  PEI_MIN_MEMORY_SIZE               (10 * 0x800000 + 0x10000000)    // 80MB + 256MB

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
  MiscPeiPreMemConfig->MchBar              = 0xFED10000;
  MiscPeiPreMemConfig->DmiBar              = 0xFED18000;
  MiscPeiPreMemConfig->EpBar               = 0xFED19000;
  MiscPeiPreMemConfig->EdramBar            = 0xFED80000;
  MiscPeiPreMemConfig->SmbusBar            = 0xEFA0;
  MiscPeiPreMemConfig->TsegSize            = PcdGet32 (PcdTsegSize);
  MiscPeiPreMemConfig->GdxcBar             = 0xFED84000;

  //
  // Initialize the Switchable Graphics Default Configuration
  //
  MiscPeiPreMemConfig->SgDelayAfterHoldReset = 100; //100ms
  MiscPeiPreMemConfig->SgDelayAfterPwrEn     = 300; //300ms

  ///
  /// Initialize the DataPtr for S3 resume
  ///
  MiscPeiPreMemConfig->S3DataPtr = NULL;
  MiscPeiPreMemConfig->OpRomScanTempMmioBar      = DEFAULT_OPTION_ROM_TEMP_BAR;
  MiscPeiPreMemConfig->OpRomScanTempMmioLimit    = DEFAULT_OPTION_ROM_TEMP_MEM_LIMIT;
}

VOID
LoadSaMiscPeiDefault (
  IN   VOID          *ConfigBlockPointer
  )
{
  SA_MISC_PEI_CONFIG        *MiscPeiConfig;

  MiscPeiConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "MiscPeiConfig->Header.GuidHob.Name = %g\n", &MiscPeiConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "MiscPeiConfig->Header.GuidHob.Header.HobLength = 0x%x\n", MiscPeiConfig->Header.GuidHob.Header.HobLength));

  ///
  /// EDRAM H/W Mode by default (0- EDRAM SW Disable, 1- EDRAM SW Enable, 2- EDRAM HW Mode)
  ///
  MiscPeiConfig->EdramTestMode = 2;

  if (IsWhlCpu()) {
    MiscPeiConfig->Device4Enable = 1;
  }
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
  Vtd->VtdDisable      = 0;
  Vtd->BaseAddress[0]  = 0xFED90000;
  Vtd->BaseAddress[1]  = 0xFED92000;
  Vtd->BaseAddress[2]  = 0xFED91000;
}

VOID
LoadIpuPreMemDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  IPU_PREMEM_CONFIG      *IpuPreMemPolicy;

  IpuPreMemPolicy = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "IpuPreMemPolicy->Header.GuidHob.Name = %g\n", &IpuPreMemPolicy->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "IpuPreMemPolicy->Header.GuidHob.Header.HobLength = 0x%x\n", IpuPreMemPolicy->Header.GuidHob.Header.HobLength));

}

VOID
LoadPciePeiPreMemDefault (
  IN   VOID          *ConfigBlockPointer
  )
{
  UINT8                  Index;
  PCIE_PEI_PREMEM_CONFIG *PciePeiPreMemConfig;

  PciePeiPreMemConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "PciePeiPreMemConfig->Header.GuidHob.Name = %g\n", &PciePeiPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PciePeiPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PciePeiPreMemConfig->Header.GuidHob.Header.HobLength));
  //
  // Initialize the PciExpress Configuration
  //
  PciePeiPreMemConfig->DmiGen3EqPh2Enable             = 2;
  PciePeiPreMemConfig->DmiGen3ProgramStaticEq         = 1;
  PciePeiPreMemConfig->Peg0Enable                     = 2;
  PciePeiPreMemConfig->Peg1Enable                     = 2;
  PciePeiPreMemConfig->Peg2Enable                     = 2;
  PciePeiPreMemConfig->Peg3Enable                     = 2;
  PciePeiPreMemConfig->Peg0PowerDownUnusedLanes       = 1;
  PciePeiPreMemConfig->Peg1PowerDownUnusedLanes       = 1;
  PciePeiPreMemConfig->Peg2PowerDownUnusedLanes       = 1;
  PciePeiPreMemConfig->Peg3PowerDownUnusedLanes       = 1;
  PciePeiPreMemConfig->Peg0Gen3EqPh2Enable            = 2;
  PciePeiPreMemConfig->Peg1Gen3EqPh2Enable            = 2;
  PciePeiPreMemConfig->Peg2Gen3EqPh2Enable            = 2;
  PciePeiPreMemConfig->Peg3Gen3EqPh2Enable            = 2;
  PciePeiPreMemConfig->PegGen3ProgramStaticEq         = 1;
  PciePeiPreMemConfig->Gen3SwEqNumberOfPresets        = 2;
  PciePeiPreMemConfig->Gen3SwEqEnableVocTest          = 2;

  if (IsCnlPch() && IsPchH() && (PchStepping() == PCH_A0)) {
    PciePeiPreMemConfig->DmiMaxLinkSpeed = 1;
  }

  for (Index = 0; Index < SA_DMI_MAX_LANE; Index++) {
    PciePeiPreMemConfig->DmiGen3RootPortPreset[Index] = 4;
    PciePeiPreMemConfig->DmiGen3EndPointPreset[Index] = 7;
    PciePeiPreMemConfig->DmiGen3EndPointHint[Index]   = 2;
  }
  for (Index = 0; Index < SA_DMI_MAX_BUNDLE; Index++) {
    ///
    /// Gen3 RxCTLE peaking default is 0 for DMI
    ///
    PciePeiPreMemConfig->DmiGen3RxCtlePeaking[Index]  = 0;
  }
  for (Index = 0; Index < SA_PEG_MAX_LANE; Index++) {
    PciePeiPreMemConfig->PegGen3RootPortPreset[Index] = 7;
    PciePeiPreMemConfig->PegGen3EndPointPreset[Index] = 7;
    PciePeiPreMemConfig->PegGen3EndPointHint[Index]   = 2;
  }
  PciePeiPreMemConfig->DmiDeEmphasis = 1;
  ///
  /// Gen3 Software Equalization Jitter Dwell Time:               1 msec
  /// Gen3 Software Equalization Jitter Error Target:             1
  /// Gen3 Software Equalization VOC    Dwell Time:               10 msec
  /// Gen3 Software Equalization VOC    Error Target:             2
  ///
  PciePeiPreMemConfig->Gen3SwEqJitterDwellTime        = 3 * STALL_ONE_MILLI_SECOND;
  PciePeiPreMemConfig->Gen3SwEqJitterErrorTarget      = 2;
  PciePeiPreMemConfig->Gen3SwEqVocDwellTime           = 10 * STALL_ONE_MILLI_SECOND;
  PciePeiPreMemConfig->Gen3SwEqVocErrorTarget         = 2;

  /**
  Parameters for PCIe Gen3 device reset
  @note Refer to the Platform Design Guide (PDG) for additional information about this GPIO.
  **/
  PciePeiPreMemConfig->PegGpioData.GpioSupport        = FALSE;
  PciePeiPreMemConfig->PegGpioData.SaPeg0ResetGpio.GpioPad = 0;
  PciePeiPreMemConfig->PegGpioData.SaPeg0ResetGpio.Active  = FALSE;
  PciePeiPreMemConfig->PegGpioData.SaPeg3ResetGpio.GpioPad = 0;
  PciePeiPreMemConfig->PegGpioData.SaPeg3ResetGpio.Active  = FALSE;
}

VOID
LoadPciePeiDefault (
  IN   VOID          *ConfigBlockPointer
  )
{
  UINT8                Index;
  PCIE_PEI_CONFIG      *PciePeiConfig;

  PciePeiConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "PciePeiConfig->Header.GuidHob.Name = %g\n", &PciePeiConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PciePeiConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PciePeiConfig->Header.GuidHob.Header.HobLength));
  //
  // Initialize the PciExpress Configuration
  //
  for (Index = 0; Index < SA_PEG_MAX_FUN; Index++) {
    PciePeiConfig->PegDeEmphasis[Index] = 1;
    PciePeiConfig->PegMaxPayload[Index] = 0xFF;
  }
  ///
  /// Slot Power Limit Value:   75 W
  /// Slot Power Limit Scale:   1.0x
  /// Physical Slot Number:     Peg Index + 1 (1,2,3)
  ///
  for (Index = 0; Index < SA_PEG_MAX_FUN; Index++) {
    PciePeiConfig->PegSlotPowerLimitValue[Index] = 75;
    PciePeiConfig->PegPhysicalSlotNumber[Index] =  Index + 1;
  }

}

VOID
LoadGraphichsPeiPreMemDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  GRAPHICS_PEI_PREMEM_CONFIG                         *GtPreMemConfig;

  GtPreMemConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "GtPreMemConfig->Header.GuidHob.Name = %g\n", &GtPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "GtPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", GtPreMemConfig->Header.GuidHob.Header.HobLength));

  ///
  /// Initialize Graphics Pre-Mem Configurations.
  ///
  GtPreMemConfig->GmAdr               = 0xD0000000;
  GtPreMemConfig->GttMmAdr            = 0xCF000000;
  GtPreMemConfig->GttSize             = 3;
  GtPreMemConfig->IgdDvmt50PreAlloc   = 1;
  GtPreMemConfig->InternalGraphics    = 2;
  GtPreMemConfig->PrimaryDisplay      = 3;
  GtPreMemConfig->ApertureSize        = SA_GT_APERTURE_SIZE_256MB;
  GtPreMemConfig->PanelPowerEnable    = 1;
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

  //
  // Initialize the Graphics configuration
  //
  GtConfig->RenderStandby       = 1;
  GtConfig->PavpEnable          = 1;
  GtConfig->PmSupport           = 1;
  GtConfig->CdynmaxClampEnable  = 1;
  GtConfig->GtFreqMax           = 0xFF;
  //
  // Initialize the default VBT settings
  //
  GtConfig->DdiConfiguration.DdiPortEdp  = 1;
  GtConfig->DdiConfiguration.DdiPortBHpd = 1;
  GtConfig->DdiConfiguration.DdiPortCHpd = 1;
  GtConfig->DdiConfiguration.DdiPortDHpd = 1;
  GtConfig->DdiConfiguration.DdiPortFHpd = 0;
  GtConfig->DdiConfiguration.DdiPortBDdc = 1;
  GtConfig->DdiConfiguration.DdiPortCDdc = 1;
  GtConfig->DdiConfiguration.DdiPortDDdc = 1;
  GtConfig->DdiConfiguration.DdiPortFDdc = 0;

  ///
  /// Initialize the CdClock to 675 Mhz
  ///
  GtConfig->CdClock             = 3;
}

VOID
LoadSwitchableGraphichsDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  SWITCHABLE_GRAPHICS_CONFIG        *SgConfig;
  SgConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "SgConfig->Header.GuidHob.Name = %g\n", &SgConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "SgConfig->Header.GuidHob.Header.HobLength = 0x%x\n", SgConfig->Header.GuidHob.Header.HobLength));
  SgConfig->SaRtd3Pcie0Gpio.GpioSupport    = NotSupported;
  SgConfig->SaRtd3Pcie1Gpio.GpioSupport    = NotSupported;
  SgConfig->SaRtd3Pcie2Gpio.GpioSupport    = NotSupported;
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

  MemConfigNoCrc->PlatformMemorySize = PEI_MIN_MEMORY_SIZE;

  MemConfigNoCrc->SerialDebugLevel  = 3;  //< Enable MRC debug message

  MemConfigNoCrc->MemTestOnWarmBoot = 1;  //< Enable to run BaseMemoryTest on warm boot by default

}

VOID
LoadGnaDefault (
  IN VOID *ConfigBlockPointer
  )
{
  GNA_CONFIG                        *GnaConfig;
  GnaConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "GnaConfig->Header.GuidHob.Name = %g\n", &GnaConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "GnaConfig->Header.GuidHob.Header.HobLength = 0x%x\n", GnaConfig->Header.GuidHob.Header.HobLength));
  GnaConfig->GnaEnable    = TRUE;
}

VOID
LoadMemConfigDefault (
  IN VOID *ConfigBlockPointer
  )
{
  MEMORY_CONFIGURATION                    *MemConfig;
  CPU_FAMILY                              CpuFamily;
  UINT16                                  DeviceId;

  CPU_SKU                                 CpuSku;
  CpuSku = GetCpuSku ();
  CpuFamily   = GetCpuFamily ();
  DeviceId    = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, 0, 0, R_SA_MC_DEVICE_ID));

  MemConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "MemConfig->Header.GuidHob.Name = %g\n", &MemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "MemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", MemConfig->Header.GuidHob.Header.HobLength));
  //
  // Initialize the Memory Configuration
  //
  MemConfig->EnBER              = 1;
  MemConfig->EccSupport         = 1;
  MemConfig->ScramblerSupport   = 1;
  MemConfig->PowerDownMode      = 0xFF;
  MemConfig->RankInterleave     = TRUE;
  MemConfig->EnhancedInterleave = TRUE;
  MemConfig->EnCmdRate          = 3;
  MemConfig->AutoSelfRefreshSupport = TRUE;
  MemConfig->ExtTemperatureSupport  = TRUE;
  MemConfig->WeaklockEn = 1;
  MemConfig->Ddr4MixedUDimm2DpcLimit = 1;

  MemConfig->DualDimmPerChannelBoardType = (CpuFamily == EnumCpuCflDtHalo) ? TRUE : FALSE;

  //
  // Channel Hash Configuration
  //
  MemConfig->ChHashEnable         = TRUE;
  MemConfig->ChHashMask           = 0;
  MemConfig->ChHashInterleaveBit  = 2;
  MemConfig->PerBankRefresh       = TRUE;
  //
  // Options for Thermal settings
  //
  MemConfig->EnablePwrDn            = 1;
  MemConfig->EnablePwrDnLpddr       = 1;
  MemConfig->DdrThermalSensor       = 1;

  MemConfig->EnergyScaleFact        = 4;
  MemConfig->IdleEnergyCh0Dimm0     = 0xA;
  MemConfig->IdleEnergyCh0Dimm1     = 0xA;
  MemConfig->IdleEnergyCh1Dimm0     = 0xA;
  MemConfig->IdleEnergyCh1Dimm1     = 0xA;
  MemConfig->PdEnergyCh0Dimm0       = 0x6;
  MemConfig->PdEnergyCh0Dimm1       = 0x6;
  MemConfig->PdEnergyCh1Dimm0       = 0x6;
  MemConfig->PdEnergyCh1Dimm1       = 0x6;
  MemConfig->ActEnergyCh0Dimm0      = 0xAC;
  MemConfig->ActEnergyCh0Dimm1      = 0xAC;
  MemConfig->ActEnergyCh1Dimm0      = 0xAC;
  MemConfig->ActEnergyCh1Dimm1      = 0xAC;
  MemConfig->RdEnergyCh0Dimm0       = 0xD4;
  MemConfig->RdEnergyCh0Dimm1       = 0xD4;
  MemConfig->RdEnergyCh1Dimm0       = 0xD4;
  MemConfig->RdEnergyCh1Dimm1       = 0xD4;
  MemConfig->WrEnergyCh0Dimm0       = 0xDD;
  MemConfig->WrEnergyCh0Dimm1       = 0xDD;
  MemConfig->WrEnergyCh1Dimm0       = 0xDD;
  MemConfig->WrEnergyCh1Dimm1       = 0xDD;
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
  MemConfig->ThrtCkeMinDefeatLpddr  = 1;
  MemConfig->ThrtCkeMinTmrLpddr     = 0x40;

  MemConfig->RaplLim2WindX = 1;
  MemConfig->RaplLim2WindY = 1;
  MemConfig->RaplLim2Pwr   = 0xDE;

  //
  // CA Vref routing: board-dependent
  // 0 - VREF_CA goes to both CH_A and CH_B (LPDDR3/DDR3L)
  // 1 - VREF_CA to CH_A, VREF_DQ_A to CH_B (should not be used)
  // 2 - VREF_CA to CH_A, VREF_DQ_B to CH_B (DDR4)
  //
  //MemConfig->CaVrefConfig = 0;
  MemConfig->VttTermination     = ((CpuSku == EnumCpuUlx) || (CpuSku == EnumCpuUlt));
  MemConfig->VttCompForVsshi    = 0;

#ifdef UP_SERVER_FLAG
  MemConfig->TsodTcritmax = 0x69;
  MemConfig->TsodThigMax  = 0x5D;
#endif


  //
  // MRC training steps
  //
  MemConfig->ECT                  = 1;
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
  MemConfig->McLock               = TRUE;
  MemConfig->GdxcIotSize          = 4;
  MemConfig->GdxcMotSize          = 12;
  MemConfig->RDEQT                = 1;

  MemConfig->MrcFastBoot          = TRUE;
  MemConfig->MrcTrainOnWarm       = FALSE;
  MemConfig->RemapEnable          = TRUE;
  MemConfig->BClkFrequency        = 100 * 1000 * 1000;

#ifdef EMBEDDED_FLAG
  MemConfig->Force1Dpc = TRUE;
#endif
  MemConfig->Vc1ReadMeter           = TRUE;
  MemConfig->Vc1ReadMeterTimeWindow = 0x320;
  MemConfig->Vc1ReadMeterThreshold  = 0x118;
  MemConfig->StrongWkLeaker         = 7;

  MemConfig->MobilePlatform     = (IS_SA_DEVICE_ID_MOBILE (DeviceId)) ? TRUE : FALSE;
  MemConfig->PciIndex           = 0xCF8;
  MemConfig->PciData            = 0xCFC;
  MemConfig->CkeRankMapping     = 0xAA;

  // This only affects ULX/ULT; otherwise SA GV is disabled.
  // CFL SA GV: 0 - Disabled, 1 - FixedLow, 2 - FixedHigh, 3 - Enabled
  MemConfig->SaGv             = 3;
  MemConfig->SimicsFlag       = 0;

  MemConfig->Idd3n              = 26;
  MemConfig->Idd3p              = 11;

  MemConfig->RhPrevention       = TRUE;         // Row Hammer prevention.
  MemConfig->RhSolution         = HardwareRhp;  // Type of solution to be used for RHP - 0/1 = HardwareRhp/Refresh2x
  MemConfig->RhActProbability   = OneIn2To11;    // Activation probability for Hardware RHP

  MemConfig->LpddrMemWriteLatencySet = 1;  // Enable LPDDR3 WL Set B if supported by DRAM

  MemConfig->DllBwEn1 = 1;
  MemConfig->DllBwEn2 = 2;
  MemConfig->DllBwEn3 = 2;

  MemConfig->RetrainOnFastFail  = 1; //  Restart MRC in Cold mode if SW MemTest fails during Fast flow. 0 = Disabled, 1 = Enabled
  MemConfig->Lp4DqsOscEn        = 1;
  MemConfig->IsvtIoPort         = 0x99;
}


VOID
LoadOverClockConfigDefault (
  IN VOID   *ConfigBlockPointer
  )
{
  OVERCLOCKING_PREMEM_CONFIG    *OcPreMemConfig;
  OcPreMemConfig = (OVERCLOCKING_PREMEM_CONFIG *)ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "OcPreMemConfig->Header.GuidHob.Name = %g\n", &OcPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "OcPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", OcPreMemConfig->Header.GuidHob.Header.HobLength));
}

static COMPONENT_BLOCK_ENTRY  mSaIpBlocksPreMem [] = {
  {&gSaMiscPeiPreMemConfigGuid,       sizeof (SA_MISC_PEI_PREMEM_CONFIG),  SA_MISC_PEI_PREMEM_CONFIG_REVISION,  LoadSaMiscPeiPreMemDefault},
  {&gSaPciePeiPreMemConfigGuid,       sizeof (PCIE_PEI_PREMEM_CONFIG),     SA_PCIE_PEI_PREMEM_CONFIG_REVISION,  LoadPciePeiPreMemDefault},
  {&gGraphicsPeiPreMemConfigGuid,     sizeof (GRAPHICS_PEI_PREMEM_CONFIG), GRAPHICS_PEI_PREMEM_CONFIG_REVISION, LoadGraphichsPeiPreMemDefault},
  {&gSwitchableGraphicsConfigGuid,    sizeof (SWITCHABLE_GRAPHICS_CONFIG), SWITCHABLE_GRAPHICS_CONFIG_REVISION, LoadSwitchableGraphichsDefault},
  {&gMemoryConfigGuid,                sizeof (MEMORY_CONFIGURATION),       MEMORY_CONFIG_REVISION,              LoadMemConfigDefault},
  {&gMemoryConfigNoCrcGuid,           sizeof (MEMORY_CONFIG_NO_CRC),       MEMORY_CONFIG_REVISION,              LoadMemConfigNoCrcDefault},
  {&gSaOverclockingPreMemConfigGuid,  sizeof (OVERCLOCKING_PREMEM_CONFIG), SA_OVERCLOCKING_CONFIG_REVISION,     LoadOverClockConfigDefault},
  {&gVtdConfigGuid,                   sizeof (VTD_CONFIG),                 VTD_CONFIG_REVISION,                 LoadVtdDefault},
  {&gIpuPreMemConfigGuid,             sizeof (IPU_PREMEM_CONFIG),          IPU_PREMEM_CONFIG_REVISION,          LoadIpuPreMemDefault}
};

static COMPONENT_BLOCK_ENTRY  mSaIpBlocks [] = {
  {&gSaMiscPeiConfigGuid,       sizeof (SA_MISC_PEI_CONFIG),   SA_MISC_PEI_CONFIG_REVISION,      LoadSaMiscPeiDefault},
  {&gSaPciePeiConfigGuid,       sizeof (PCIE_PEI_CONFIG),      SA_PCIE_PEI_CONFIG_REVISION,      LoadPciePeiDefault},
  {&gGraphicsPeiConfigGuid,     sizeof (GRAPHICS_PEI_CONFIG),  GRAPHICS_PEI_CONFIG_REVISION,     LoadGraphichsPeiDefault},
  {&gGnaConfigGuid,             sizeof (GNA_CONFIG),           GNA_CONFIG_REVISION,              LoadGnaDefault}
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
