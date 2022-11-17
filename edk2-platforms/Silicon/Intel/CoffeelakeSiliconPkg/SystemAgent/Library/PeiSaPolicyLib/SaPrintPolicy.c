/** @file
  This file provides service for PEI phase policy printing

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiSaPolicyLibrary.h"
#include <Library/GpioNativeLib.h>
#include <Library/ConfigBlockLib.h>

/**
  This function prints the PEI phase PreMem policy.

  @param[in] SiPolicyPreMemPpi - Instance of SI_PREMEM_POLICY_PPI
**/
VOID
EFIAPI
SaPrintPolicyPpiPreMem (
  IN  SI_PREMEM_POLICY_PPI *SiPolicyPreMemPpi
  )
{
  DEBUG_CODE_BEGIN ();
  INTN                                  Index;
  INTN                                  Index2;
  EFI_STATUS                            Status;
  SA_MISC_PEI_PREMEM_CONFIG             *MiscPeiPreMemConfig;
  GRAPHICS_PEI_PREMEM_CONFIG            *GtPreMemConfig;
  MEMORY_CONFIGURATION                  *MemConfig;
  PCIE_PEI_PREMEM_CONFIG                *PciePeiPreMemConfig;
  SWITCHABLE_GRAPHICS_CONFIG            *SgConfig;
  MEMORY_CONFIG_NO_CRC                  *MemConfigNoCrc;
  VTD_CONFIG                            *Vtd;
  OVERCLOCKING_PREMEM_CONFIG            *OcPreMemConfig;
  IPU_PREMEM_CONFIG                     *IpuPreMemPolicy;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *)SiPolicyPreMemPpi, &gSaMiscPeiPreMemConfigGuid, (VOID *) &MiscPeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *)SiPolicyPreMemPpi, &gGraphicsPeiPreMemConfigGuid, (VOID *) &GtPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPreMemPpi, &gVtdConfigGuid, (VOID *) &Vtd);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *)SiPolicyPreMemPpi, &gMemoryConfigGuid, (VOID *) &MemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *)SiPolicyPreMemPpi, &gSaPciePeiPreMemConfigGuid, (VOID *) &PciePeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *)SiPolicyPreMemPpi, &gSwitchableGraphicsConfigGuid, (VOID *) &SgConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *)SiPolicyPreMemPpi, &gMemoryConfigNoCrcGuid, (VOID *) &MemConfigNoCrc);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPreMemPpi, &gSaOverclockingPreMemConfigGuid, (VOID *) &OcPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPreMemPpi, &gIpuPreMemConfigGuid, (VOID *) &IpuPreMemPolicy);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "\n------------------------ SA Policy (PEI PreMem) Print BEGIN -----------------\n"));
  DEBUG ((DEBUG_INFO, "Revision : 0x%x\n", SiPolicyPreMemPpi->TableHeader.Header.Revision));
  ASSERT (SiPolicyPreMemPpi->TableHeader.Header.Revision == SI_PREMEM_POLICY_REVISION);

  DEBUG ((DEBUG_INFO, "------------------------ SA_MISC_PEI_PREMEM_CONFIG  -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", MiscPeiPreMemConfig->Header.Revision));
  ASSERT (MiscPeiPreMemConfig->Header.Revision == SA_MISC_PEI_PREMEM_CONFIG_REVISION);
  DEBUG ((DEBUG_INFO, " SpdAddressTable[%d] :", SA_MC_MAX_SOCKETS));
  for (Index = 0; Index < SA_MC_MAX_SOCKETS; Index++) {
    DEBUG ((DEBUG_INFO, " 0x%x", MiscPeiPreMemConfig->SpdAddressTable[Index]));
  }

  DEBUG ((DEBUG_INFO, "\n MchBar : 0x%x\n", MiscPeiPreMemConfig->MchBar));
  DEBUG ((DEBUG_INFO, " DmiBar : 0x%x\n", MiscPeiPreMemConfig->DmiBar));
  DEBUG ((DEBUG_INFO, " EpBar : 0x%x\n", MiscPeiPreMemConfig->EpBar));
  DEBUG ((DEBUG_INFO, " SmbusBar : 0x%x\n", MiscPeiPreMemConfig->SmbusBar));
  DEBUG ((DEBUG_INFO, " GdxcBar : 0x%x\n", MiscPeiPreMemConfig->GdxcBar));
  DEBUG ((DEBUG_INFO, " TsegSize : 0x%x\n", MiscPeiPreMemConfig->TsegSize));
  DEBUG ((DEBUG_INFO, " UserBd : 0x%x\n", MiscPeiPreMemConfig->UserBd));
  DEBUG ((DEBUG_INFO, " EdramBar : 0x%x\n", MiscPeiPreMemConfig->EdramBar));
  DEBUG ((DEBUG_INFO, " MmioSize : %d MB\n", MiscPeiPreMemConfig->MmioSize));
  DEBUG ((DEBUG_INFO, " MmioSizeAdjustment : %d MB\n", MiscPeiPreMemConfig->MmioSizeAdjustment));
  DEBUG ((DEBUG_INFO, " SkipExtGfxScan: 0x%x\n", MiscPeiPreMemConfig->SkipExtGfxScan));
  DEBUG ((DEBUG_INFO, " S3DataPtr : 0x%x\n", MiscPeiPreMemConfig->S3DataPtr));
  DEBUG ((DEBUG_INFO, "------------------------ SG_DELAY_OPTIMIZATION_DATA -----------------\n"));
  DEBUG ((DEBUG_INFO, " SaRtd3.SgDelayAfterHoldReset : 0x%x\n", MiscPeiPreMemConfig->SgDelayAfterHoldReset));
  DEBUG ((DEBUG_INFO, " SaRtd3.SgDelayAfterPwrEn     : 0x%x\n", MiscPeiPreMemConfig->SgDelayAfterPwrEn));

  DEBUG ((DEBUG_INFO, " ScanExtGfxForLegacyOpRom : 0x%x\n", MiscPeiPreMemConfig->ScanExtGfxForLegacyOpRom));
  DEBUG ((DEBUG_INFO, " AcpiReservedMemoryBase : 0x%x\n", MiscPeiPreMemConfig->AcpiReservedMemoryBase));
  DEBUG ((DEBUG_INFO, " AcpiReservedMemorySize : 0x%x\n", MiscPeiPreMemConfig->AcpiReservedMemorySize));
  DEBUG ((DEBUG_INFO, " SystemMemoryLength : 0x%x\n", MiscPeiPreMemConfig->SystemMemoryLength));
  DEBUG ((DEBUG_INFO, " OpRomScanTempMmioBar : 0x%x\n", MiscPeiPreMemConfig->OpRomScanTempMmioBar));
  DEBUG ((DEBUG_INFO, " OpRomScanTempMmioLimit : 0x%x\n", MiscPeiPreMemConfig->OpRomScanTempMmioLimit));

  DEBUG ((DEBUG_INFO, "------------------------ GRAPHICS_PEI_PREMEM_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", GtPreMemConfig->Header.Revision));
  ASSERT (GtPreMemConfig->Header.Revision == GRAPHICS_PEI_PREMEM_CONFIG_REVISION);
  DEBUG ((DEBUG_INFO, " PanelPowerEnable : 0x%x\n", GtPreMemConfig->PanelPowerEnable));
  DEBUG ((DEBUG_INFO, " GttSize : %d MB\n", GtPreMemConfig->GttSize));
  DEBUG ((DEBUG_INFO, " IgdDvmt50PreAlloc : 0x%x\n", GtPreMemConfig->IgdDvmt50PreAlloc));
  DEBUG ((DEBUG_INFO, " InternalGraphics : 0x%x\n", GtPreMemConfig->InternalGraphics));
  DEBUG ((DEBUG_INFO, " PrimaryDisplay : 0x%x\n", GtPreMemConfig->PrimaryDisplay));
  DEBUG ((DEBUG_INFO, " ApertureSize : 0x%x\n", GtPreMemConfig->ApertureSize));
  DEBUG ((DEBUG_INFO, " GtPsmiSupport : 0x%x\n", GtPreMemConfig->GtPsmiSupport));
  DEBUG ((DEBUG_INFO, " PsmiRegionSize : 0x%x\n", GtPreMemConfig->PsmiRegionSize));
  DEBUG ((DEBUG_INFO, " GttMmAdr : 0x%x\n", GtPreMemConfig->GttMmAdr));
  DEBUG ((DEBUG_INFO, " GmAdr : 0x%x\n", GtPreMemConfig->GmAdr));
  DEBUG ((DEBUG_INFO, " DeltaT12PowerCycleDelayPreMem : 0x%x\n", GtPreMemConfig->DeltaT12PowerCycleDelayPreMem));

  DEBUG ((DEBUG_INFO, "------------------------ PCIE_PEI_PREMEM_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", PciePeiPreMemConfig->Header.Revision));
  ASSERT (PciePeiPreMemConfig->Header.Revision == SA_PCIE_PEI_PREMEM_CONFIG_REVISION);
  DEBUG ((DEBUG_INFO, " DmiMaxLinkSpeed : 0x%x\n", PciePeiPreMemConfig->DmiMaxLinkSpeed));
  DEBUG ((DEBUG_INFO, " DmiGen3EqPh2Enable : 0x%x\n", PciePeiPreMemConfig->DmiGen3EqPh2Enable));
  DEBUG ((DEBUG_INFO, " DmiGen3EqPh3Method : 0x%x\n", PciePeiPreMemConfig->DmiGen3EqPh3Method));
  DEBUG ((DEBUG_INFO, " DmiGen3ProgramStaticEq : 0x%x\n", PciePeiPreMemConfig->DmiGen3ProgramStaticEq));
  DEBUG ((DEBUG_INFO, " Peg0Enable : 0x%x\n", PciePeiPreMemConfig->Peg0Enable));
  DEBUG ((DEBUG_INFO, " Peg1Enable : 0x%x\n", PciePeiPreMemConfig->Peg1Enable));
  DEBUG ((DEBUG_INFO, " Peg2Enable : 0x%x\n", PciePeiPreMemConfig->Peg2Enable));
  DEBUG ((DEBUG_INFO, " Peg3Enable : 0x%x\n", PciePeiPreMemConfig->Peg3Enable));
  DEBUG ((DEBUG_INFO, " Peg0MaxLinkSpeed : 0x%x\n", PciePeiPreMemConfig->Peg0MaxLinkSpeed));
  DEBUG ((DEBUG_INFO, " Peg1MaxLinkSpeed : 0x%x\n", PciePeiPreMemConfig->Peg1MaxLinkSpeed));
  DEBUG ((DEBUG_INFO, " Peg2MaxLinkSpeed : 0x%x\n", PciePeiPreMemConfig->Peg2MaxLinkSpeed));
  DEBUG ((DEBUG_INFO, " Peg3MaxLinkSpeed : 0x%x\n", PciePeiPreMemConfig->Peg3MaxLinkSpeed));
  DEBUG ((DEBUG_INFO, " Peg0MaxLinkWidth : 0x%x\n", PciePeiPreMemConfig->Peg0MaxLinkWidth));
  DEBUG ((DEBUG_INFO, " Peg1MaxLinkWidth : 0x%x\n", PciePeiPreMemConfig->Peg1MaxLinkWidth));
  DEBUG ((DEBUG_INFO, " Peg2MaxLinkWidth : 0x%x\n", PciePeiPreMemConfig->Peg2MaxLinkWidth));
  DEBUG ((DEBUG_INFO, " Peg3MaxLinkWidth : 0x%x\n", PciePeiPreMemConfig->Peg3MaxLinkWidth));
  DEBUG ((DEBUG_INFO, " Peg0PowerDownUnusedLanes : 0x%x\n", PciePeiPreMemConfig->Peg0PowerDownUnusedLanes));
  DEBUG ((DEBUG_INFO, " Peg1PowerDownUnusedLanes : 0x%x\n", PciePeiPreMemConfig->Peg1PowerDownUnusedLanes));
  DEBUG ((DEBUG_INFO, " Peg2PowerDownUnusedLanes : 0x%x\n", PciePeiPreMemConfig->Peg2PowerDownUnusedLanes));
  DEBUG ((DEBUG_INFO, " Peg3PowerDownUnusedLanes : 0x%x\n", PciePeiPreMemConfig->Peg3PowerDownUnusedLanes));
  DEBUG ((DEBUG_INFO, " Peg0Gen3EqPh2Enable : 0x%x\n", PciePeiPreMemConfig->Peg0Gen3EqPh2Enable));
  DEBUG ((DEBUG_INFO, " Peg1Gen3EqPh2Enable : 0x%x\n", PciePeiPreMemConfig->Peg1Gen3EqPh2Enable));
  DEBUG ((DEBUG_INFO, " Peg2Gen3EqPh2Enable : 0x%x\n", PciePeiPreMemConfig->Peg2Gen3EqPh2Enable));
  DEBUG ((DEBUG_INFO, " Peg3Gen3EqPh2Enable : 0x%x\n", PciePeiPreMemConfig->Peg3Gen3EqPh2Enable));
  DEBUG ((DEBUG_INFO, " Peg0Gen3EqPh3Method : 0x%x\n", PciePeiPreMemConfig->Peg0Gen3EqPh3Method));
  DEBUG ((DEBUG_INFO, " Peg1Gen3EqPh3Method : 0x%x\n", PciePeiPreMemConfig->Peg1Gen3EqPh3Method));
  DEBUG ((DEBUG_INFO, " Peg2Gen3EqPh3Method : 0x%x\n", PciePeiPreMemConfig->Peg2Gen3EqPh3Method));
  DEBUG ((DEBUG_INFO, " Peg3Gen3EqPh3Method : 0x%x\n", PciePeiPreMemConfig->Peg3Gen3EqPh3Method));
  DEBUG ((DEBUG_INFO, " PegGen3ProgramStaticEq : 0x%x\n", PciePeiPreMemConfig->PegGen3ProgramStaticEq));
  DEBUG ((DEBUG_INFO, " Gen3SwEqAlwaysAttempt : 0x%x\n", PciePeiPreMemConfig->Gen3SwEqAlwaysAttempt));
  DEBUG ((DEBUG_INFO, " Gen3SwEqNumberOfPresets : 0x%x\n", PciePeiPreMemConfig->Gen3SwEqNumberOfPresets));
  DEBUG ((DEBUG_INFO, " Gen3SwEqEnableVocTest : 0x%x\n", PciePeiPreMemConfig->Gen3SwEqEnableVocTest));
  DEBUG ((DEBUG_INFO, " InitPcieAspmAfterOprom : 0x%x\n", PciePeiPreMemConfig->InitPcieAspmAfterOprom));
  DEBUG ((DEBUG_INFO, " PegRxCemTestingMode : 0x%x\n", PciePeiPreMemConfig->PegRxCemTestingMode));
  DEBUG ((DEBUG_INFO, " PegRxCemLoopbackLane : 0x%x\n", PciePeiPreMemConfig->PegRxCemLoopbackLane));
  DEBUG ((DEBUG_INFO, " PegRxCemNonProtocolAwareness : 0x%x\n", PciePeiPreMemConfig->PegRxCemNonProtocolAwareness));
  DEBUG ((DEBUG_INFO, " PegDisableSpreadSpectrumClocking : 0x%x\n", PciePeiPreMemConfig->PegDisableSpreadSpectrumClocking));
  DEBUG ((DEBUG_INFO, " PegGenerateBdatMarginTable : 0x%x\n", PciePeiPreMemConfig->PegGenerateBdatMarginTable));
  DEBUG ((DEBUG_INFO, " DmiGen3RootPortPreset[%d] :", SA_DMI_MAX_LANE));
  for (Index = 0; Index < SA_DMI_MAX_LANE; Index++) {
    DEBUG ((DEBUG_INFO, " 0x%x", PciePeiPreMemConfig->DmiGen3RootPortPreset[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n DmiGen3EndPointPreset[%d] :", SA_DMI_MAX_LANE));
  for (Index = 0; Index < SA_DMI_MAX_LANE; Index++) {
    DEBUG ((DEBUG_INFO, " 0x%x", PciePeiPreMemConfig->DmiGen3EndPointPreset[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n DmiGen3EndPointHint[%d] :", SA_DMI_MAX_LANE));
  for (Index = 0; Index < SA_DMI_MAX_LANE; Index++) {
    DEBUG ((DEBUG_INFO, " 0x%x", PciePeiPreMemConfig->DmiGen3EndPointHint[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n DmiGen3RxCtlePeaking[%d] :", SA_DMI_MAX_BUNDLE));
  for (Index = 0; Index < SA_DMI_MAX_BUNDLE; Index++) {
    DEBUG ((DEBUG_INFO, " 0x%x", PciePeiPreMemConfig->DmiGen3RxCtlePeaking[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n PegGen3RootPortPreset[%d] :", SA_PEG_MAX_LANE));
  for (Index = 0; Index < SA_PEG_MAX_LANE; Index++) {
    DEBUG ((DEBUG_INFO, " 0x%x", PciePeiPreMemConfig->PegGen3RootPortPreset[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n PegRootPortHPE[%d] :", SA_PEG_MAX_FUN));
  for (Index = 0; Index < SA_PEG_MAX_FUN; Index++) {
    DEBUG ((DEBUG_INFO, " 0x%x", PciePeiPreMemConfig->PegRootPortHPE[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n PegGen3EndPointPreset[%d] :", SA_PEG_MAX_LANE));
  for (Index = 0; Index < SA_PEG_MAX_LANE; Index++) {
    DEBUG ((DEBUG_INFO, " 0x%x", PciePeiPreMemConfig->PegGen3EndPointPreset[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n PegGen3EndPointHint[%d] :", SA_PEG_MAX_LANE));
  for (Index = 0; Index < SA_PEG_MAX_LANE; Index++) {
    DEBUG ((DEBUG_INFO, " 0x%x", PciePeiPreMemConfig->PegGen3EndPointHint[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n PegGen3RxCtlePeaking[%d] :", SA_PEG_MAX_BUNDLE));
  for (Index = 0; Index < SA_PEG_MAX_BUNDLE; Index++) {
    DEBUG ((DEBUG_INFO, " 0x%x", PciePeiPreMemConfig->PegGen3RxCtlePeaking[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n PegGen3RxCtleOverride : 0x%x\n", PciePeiPreMemConfig->PegGen3RxCtleOverride));
  DEBUG ((DEBUG_INFO, " DmiDeEmphasis : 0x%x\n", PciePeiPreMemConfig->DmiDeEmphasis));
  DEBUG ((DEBUG_INFO, "\n Gen3SwEqJitterDwellTime : 0x%x\n", PciePeiPreMemConfig->Gen3SwEqJitterDwellTime));
  DEBUG ((DEBUG_INFO, " Gen3SwEqJitterErrorTarget : 0x%x\n", PciePeiPreMemConfig->Gen3SwEqJitterErrorTarget));
  DEBUG ((DEBUG_INFO, " Gen3SwEqVocDwellTime : 0x%x\n", PciePeiPreMemConfig->Gen3SwEqVocDwellTime));
  DEBUG ((DEBUG_INFO, " Gen3SwEqVocErrorTarget : 0x%x\n", PciePeiPreMemConfig->Gen3SwEqVocErrorTarget));
  DEBUG ((DEBUG_INFO, " PegDataPtr : %p\n", PciePeiPreMemConfig->PegDataPtr));
  DEBUG ((DEBUG_INFO, " PegGpioData->GpioSupport : 0x%x\n", PciePeiPreMemConfig->PegGpioData.GpioSupport));
  DEBUG ((DEBUG_INFO, " PegGpioData.SaPeg0ResetGpio.GpioPad Group:%d, PadNumber:%d\n", GpioGetGroupIndexFromGpioPad (PciePeiPreMemConfig->PegGpioData.SaPeg0ResetGpio.GpioPad), GpioGetPadNumberFromGpioPad (PciePeiPreMemConfig->PegGpioData.SaPeg0ResetGpio.GpioPad)));
  DEBUG ((DEBUG_INFO, " PegGpioData.SaPeg0ResetGpio.Active : 0x%x\n", PciePeiPreMemConfig->PegGpioData.SaPeg0ResetGpio.Active));
  DEBUG ((DEBUG_INFO, " PegGpioData.SaPeg3ResetGpio.GpioPad Group:%d, PadNumber:%d\n", GpioGetGroupIndexFromGpioPad (PciePeiPreMemConfig->PegGpioData.SaPeg3ResetGpio.GpioPad), GpioGetPadNumberFromGpioPad (PciePeiPreMemConfig->PegGpioData.SaPeg3ResetGpio.GpioPad)));
  DEBUG ((DEBUG_INFO, " PegGpioData.SaPeg3ResetGpio.Active : 0x%x\n", PciePeiPreMemConfig->PegGpioData.SaPeg3ResetGpio.Active));

  DEBUG ((DEBUG_INFO, "------------------------ VTD_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", Vtd->Header.Revision));
  ASSERT (Vtd->Header.Revision == VTD_CONFIG_REVISION);
  DEBUG ((DEBUG_INFO, " VtdDisable : 0x%x\n", Vtd->VtdDisable));
  DEBUG ((DEBUG_INFO, " X2ApicOptOut : 0x%x\n", Vtd->X2ApicOptOut));
  DEBUG ((DEBUG_INFO, " VtdBaseAddress[%d] :", SA_VTD_ENGINE_NUMBER));
  for (Index = 0; Index < SA_VTD_ENGINE_NUMBER; Index++) {
    DEBUG ((DEBUG_INFO, " 0x%x", Vtd->BaseAddress[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DEBUG ((DEBUG_INFO, "------------------------ SWITCHABLE_GRAPHICS_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", SgConfig->Header.Revision));
  ASSERT (SgConfig->Header.Revision == SWITCHABLE_GRAPHICS_CONFIG_REVISION);
  DEBUG ((DEBUG_INFO, " SgConfig->SaRtd3Pcie0Gpio.GpioSupport : 0x%x\n", SgConfig->SaRtd3Pcie0Gpio.GpioSupport));

  DEBUG ((DEBUG_INFO, "------------------------ IPU_PREMEM_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", IpuPreMemPolicy->Header.Revision));
  ASSERT (IpuPreMemPolicy->Header.Revision == IPU_PREMEM_CONFIG_REVISION);
  DEBUG ((DEBUG_INFO, " SaIpuEnable : 0x%x\n", IpuPreMemPolicy->SaIpuEnable));
  DEBUG ((DEBUG_INFO, " SaIpuImrConfiguration : 0x%x\n", IpuPreMemPolicy->SaIpuImrConfiguration));

  DEBUG ((DEBUG_INFO, "------------------------ MEMORY_CONFIG ------------------------------\n"));
  DEBUG ((DEBUG_INFO, " Guid                   : %g\n", &MemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, " Revision               : %d\n",   MemConfig->Header.Revision));
  ASSERT (MemConfig->Header.Revision == MEMORY_CONFIG_REVISION);
  DEBUG ((DEBUG_INFO, " Size                   : 0x%x\n", MemConfig->Header.GuidHob.Header.HobLength));
  DEBUG ((DEBUG_INFO, " HobBufferSize          : 0x%x\n", MemConfig->HobBufferSize));
  DEBUG ((DEBUG_INFO, " EccSupport             : 0x%x\n", MemConfig->EccSupport));
  DEBUG ((DEBUG_INFO, " DdrFreqLimit           : %d\n",   MemConfig->DdrFreqLimit));
  DEBUG ((DEBUG_INFO, " SpdProfileSelected     : 0x%x\n", MemConfig->SpdProfileSelected));
  DEBUG ((DEBUG_INFO, " tCL                    : 0x%x\n", MemConfig->tCL));
  DEBUG ((DEBUG_INFO, " tRCDtRP                : 0x%x\n", MemConfig->tRCDtRP));
  DEBUG ((DEBUG_INFO, " tRAS                   : 0x%x\n", MemConfig->tRAS));
  DEBUG ((DEBUG_INFO, " tWR                    : 0x%x\n", MemConfig->tWR));
  DEBUG ((DEBUG_INFO, " tRFC                   : 0x%x\n", MemConfig->tRFC));
  DEBUG ((DEBUG_INFO, " tRRD                   : 0x%x\n", MemConfig->tRRD));
  DEBUG ((DEBUG_INFO, " tWTR                   : 0x%x\n", MemConfig->tWTR));
  DEBUG ((DEBUG_INFO, " tRTP                   : 0x%x\n", MemConfig->tRTP));
  DEBUG ((DEBUG_INFO, " tFAW                   : 0x%x\n", MemConfig->tFAW));
  DEBUG ((DEBUG_INFO, " tCWL                   : 0x%x\n", MemConfig->tCWL));
  DEBUG ((DEBUG_INFO, " tREFI                  : 0x%x\n", MemConfig->tREFI));
  DEBUG ((DEBUG_INFO, " NModeSupport           : 0x%x\n", MemConfig->NModeSupport));
  DEBUG ((DEBUG_INFO, " VddVoltage             : %d\n", MemConfig->VddVoltage));

  DEBUG ((DEBUG_INFO, " DisableDimmChannel[%d] :", SA_MC_MAX_CHANNELS));
  for (Index = 0; Index < SA_MC_MAX_CHANNELS; Index++) {
    DEBUG ((DEBUG_INFO, " 0x%x", MemConfig->DisableDimmChannel[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n RemapEnable            : 0x%x\n", MemConfig->RemapEnable));
  DEBUG ((DEBUG_INFO, " ScramblerSupport       : 0x%x\n", MemConfig->ScramblerSupport));
  DEBUG ((DEBUG_INFO, " SerialDebug            : 0x%x\n", MemConfigNoCrc->SerialDebugLevel));
  DEBUG ((DEBUG_INFO, " ProbelessTrace      : 0x%x\n", MemConfig->ProbelessTrace));
  DEBUG ((DEBUG_INFO, " ECT                    : 0x%x\n", MemConfig->ECT));
  DEBUG ((DEBUG_INFO, " SOT                    : 0x%x\n", MemConfig->SOT));
  DEBUG ((DEBUG_INFO, " ERDMPRTC2D             : 0x%x\n", MemConfig->ERDMPRTC2D));
  DEBUG ((DEBUG_INFO, " RDMPRT                 : 0x%x\n", MemConfig->RDMPRT));
  DEBUG ((DEBUG_INFO, " RCVET                  : 0x%x\n", MemConfig->RCVET));
  DEBUG ((DEBUG_INFO, " JWRL                   : 0x%x\n", MemConfig->JWRL));
  DEBUG ((DEBUG_INFO, " EWRTC2D                : 0x%x\n", MemConfig->EWRTC2D));
  DEBUG ((DEBUG_INFO, " ERDTC2D                : 0x%x\n", MemConfig->ERDTC2D));
  DEBUG ((DEBUG_INFO, " WRTC1D                 : 0x%x\n", MemConfig->WRTC1D));
  DEBUG ((DEBUG_INFO, " WRVC1D                 : 0x%x\n", MemConfig->WRVC1D));
  DEBUG ((DEBUG_INFO, " RDTC1D                 : 0x%x\n", MemConfig->RDTC1D));
  DEBUG ((DEBUG_INFO, " DIMMODTT               : 0x%x\n", MemConfig->DIMMODTT));
  DEBUG ((DEBUG_INFO, " DIMMRONT               : 0x%x\n", MemConfig->DIMMRONT));
  DEBUG ((DEBUG_INFO, " WRDSEQT                : 0x%x\n", MemConfig->WRDSEQT));
  DEBUG ((DEBUG_INFO, " WRSRT                  : 0x%x\n", MemConfig->WRSRT));
  DEBUG ((DEBUG_INFO, " RDODTT                 : 0x%x\n", MemConfig->RDODTT));
  DEBUG ((DEBUG_INFO, " RDEQT                  : 0x%x\n", MemConfig->RDEQT));
  DEBUG ((DEBUG_INFO, " RDAPT                  : 0x%x\n", MemConfig->RDAPT));
  DEBUG ((DEBUG_INFO, " WRTC2D                 : 0x%x\n", MemConfig->WRTC2D));
  DEBUG ((DEBUG_INFO, " RDTC2D                 : 0x%x\n", MemConfig->RDTC2D));
  DEBUG ((DEBUG_INFO, " WRVC2D                 : 0x%x\n", MemConfig->WRVC2D));
  DEBUG ((DEBUG_INFO, " RDVC2D                 : 0x%x\n", MemConfig->RDVC2D));
  DEBUG ((DEBUG_INFO, " CMDVC                  : 0x%x\n", MemConfig->CMDVC));
  DEBUG ((DEBUG_INFO, " LCT                    : 0x%x\n", MemConfig->LCT));
  DEBUG ((DEBUG_INFO, " RTL                    : 0x%x\n", MemConfig->RTL));
  DEBUG ((DEBUG_INFO, " TAT                    : 0x%x\n", MemConfig->TAT));
  DEBUG ((DEBUG_INFO, " RMT                    : 0x%x\n", MemConfig->RMT));
  DEBUG ((DEBUG_INFO, " MEMTST                 : 0x%x\n", MemConfig->MEMTST));
  DEBUG ((DEBUG_INFO, " ALIASCHK               : 0x%x\n", MemConfig->ALIASCHK));
  DEBUG ((DEBUG_INFO, " RCVENC1D               : 0x%x\n", MemConfig->RCVENC1D));
  DEBUG ((DEBUG_INFO, " RMC                    : 0x%x\n", MemConfig->RMC));
  DEBUG ((DEBUG_INFO, " WRDSUDT                : 0x%x\n", MemConfig->WRDSUDT));

  DEBUG ((DEBUG_INFO, " VddSettleWaitTime      : 0x%x\n", MemConfig->VddSettleWaitTime));
  DEBUG ((DEBUG_INFO, " RefClk                 : 0x%x\n", MemConfig->RefClk));
  DEBUG ((DEBUG_INFO, " Ratio                  : 0x%x\n", MemConfig->Ratio));
  DEBUG ((DEBUG_INFO, " OddRatioMode           : 0x%x\n", MemConfig->OddRatioMode));
  DEBUG ((DEBUG_INFO, " MrcTimeMeasure         : 0x%x\n", MemConfig->MrcTimeMeasure));
  DEBUG ((DEBUG_INFO, " MrcFastBoot            : 0x%x\n", MemConfig->MrcFastBoot));
  DEBUG ((DEBUG_INFO, " DqPinsInterleaved      : 0x%x\n", MemConfig->DqPinsInterleaved));
  DEBUG ((DEBUG_INFO, " MrcSafeConfig          : 0x%x\n", MemConfig->MrcSafeConfig));
  DEBUG ((DEBUG_INFO, " SafeMode               : 0x%x\n", MemConfig->SafeMode));
  DEBUG ((DEBUG_INFO, " Lp4DqsOscEn            : 0x%x\n", MemConfig->Lp4DqsOscEn));
  DEBUG ((DEBUG_INFO, " EnBER                  : 0x%x\n", MemConfig->EnBER));
  DEBUG ((DEBUG_INFO, " Ddr4MixedUDimm2DpcLimit: 0x%x\n", MemConfig->Ddr4MixedUDimm2DpcLimit));
  DEBUG ((DEBUG_INFO, " PowerDownMode          : 0x%x\n", MemConfig->PowerDownMode));
  DEBUG ((DEBUG_INFO, " PwdwnIdleCounter       : 0x%x\n", MemConfig->PwdwnIdleCounter));
  DEBUG ((DEBUG_INFO, " RankInterleave         : 0x%x\n", MemConfig->RankInterleave));
  DEBUG ((DEBUG_INFO, " EnhancedInterleave     : 0x%x\n", MemConfig->EnhancedInterleave));
  DEBUG ((DEBUG_INFO, " WeaklockEn             : 0x%x\n", MemConfig->WeaklockEn));
  DEBUG ((DEBUG_INFO, " EnCmdRate              : 0x%x\n", MemConfig->EnCmdRate));
  DEBUG ((DEBUG_INFO, " CmdTriStateDis         : 0x%x\n", MemConfig->CmdTriStateDis));
  DEBUG ((DEBUG_INFO, " BClkFrequency          : 0x%x\n", MemConfig->BClkFrequency));
  DEBUG ((DEBUG_INFO, " MemoryTrace            : 0x%x\n", MemConfig->MemoryTrace));
  DEBUG ((DEBUG_INFO, " ChHashEnable           : 0x%x\n", MemConfig->ChHashEnable));
  DEBUG ((DEBUG_INFO, " ChHashMask             : 0x%x\n", MemConfig->ChHashMask));
  DEBUG ((DEBUG_INFO, " ChHashInterleaveBit    : 0x%x\n", MemConfig->ChHashInterleaveBit));
  DEBUG ((DEBUG_INFO, " PerBankRefresh         : 0x%x\n", MemConfig->PerBankRefresh));
  DEBUG ((DEBUG_INFO, " EnableExtts            : 0x%x\n", MemConfig->EnableExtts));
  DEBUG ((DEBUG_INFO, " EnableCltm             : 0x%x\n", MemConfig->EnableCltm));
  DEBUG ((DEBUG_INFO, " EnableOltm             : 0x%x\n", MemConfig->EnableOltm));
  DEBUG ((DEBUG_INFO, " EnablePwrDn            : 0x%x\n", MemConfig->EnablePwrDn));
  DEBUG ((DEBUG_INFO, " EnablePwrDnLpddr       : 0x%x\n", MemConfig->EnablePwrDnLpddr));
  DEBUG ((DEBUG_INFO, " Refresh2X              : 0x%x\n", MemConfig->Refresh2X));
  DEBUG ((DEBUG_INFO, " DdrThermalSensor       : 0x%x\n", MemConfig->DdrThermalSensor));
  DEBUG ((DEBUG_INFO, " LockPTMregs            : 0x%x\n", MemConfig->LockPTMregs));
  DEBUG ((DEBUG_INFO, " UserPowerWeightsEn     : 0x%x\n", MemConfig->UserPowerWeightsEn));
  DEBUG ((DEBUG_INFO, " EnergyScaleFact        : 0x%x\n", MemConfig->EnergyScaleFact));
  DEBUG ((DEBUG_INFO, " RaplPwrFlCh1           : 0x%x\n", MemConfig->RaplPwrFlCh1));
  DEBUG ((DEBUG_INFO, " RaplPwrFlCh0           : 0x%x\n", MemConfig->RaplPwrFlCh0));
  DEBUG ((DEBUG_INFO, " RaplLim2Lock           : 0x%x\n", MemConfig->RaplLim2Lock));
  DEBUG ((DEBUG_INFO, " RaplLim2WindX          : 0x%x\n", MemConfig->RaplLim2WindX));
  DEBUG ((DEBUG_INFO, " RaplLim2WindY          : 0x%x\n", MemConfig->RaplLim2WindY));
  DEBUG ((DEBUG_INFO, " RaplLim2Ena            : 0x%x\n", MemConfig->RaplLim2Ena));
  DEBUG ((DEBUG_INFO, " RaplLim2Pwr            : 0x%x\n", MemConfig->RaplLim2Pwr));
  DEBUG ((DEBUG_INFO, " RaplLim1WindX          : 0x%x\n", MemConfig->RaplLim1WindX));
  DEBUG ((DEBUG_INFO, " RaplLim1WindY          : 0x%x\n", MemConfig->RaplLim1WindY));
  DEBUG ((DEBUG_INFO, " RaplLim1Ena            : 0x%x\n", MemConfig->RaplLim1Ena));
  DEBUG ((DEBUG_INFO, " RaplLim1Pwr            : 0x%x\n", MemConfig->RaplLim1Pwr));
  DEBUG ((DEBUG_INFO, " WarmThresholdCh0Dimm0  : 0x%x\n", MemConfig->WarmThresholdCh0Dimm0));
  DEBUG ((DEBUG_INFO, " WarmThresholdCh0Dimm1  : 0x%x\n", MemConfig->WarmThresholdCh0Dimm1));
  DEBUG ((DEBUG_INFO, " WarmThresholdCh1Dimm0  : 0x%x\n", MemConfig->WarmThresholdCh1Dimm0));
  DEBUG ((DEBUG_INFO, " WarmThresholdCh1Dimm1  : 0x%x\n", MemConfig->WarmThresholdCh1Dimm1));
  DEBUG ((DEBUG_INFO, " HotThresholdCh0Dimm0   : 0x%x\n", MemConfig->HotThresholdCh0Dimm0));
  DEBUG ((DEBUG_INFO, " HotThresholdCh0Dimm1   : 0x%x\n", MemConfig->HotThresholdCh0Dimm1));
  DEBUG ((DEBUG_INFO, " HotThresholdCh1Dimm0   : 0x%x\n", MemConfig->HotThresholdCh1Dimm0));
  DEBUG ((DEBUG_INFO, " HotThresholdCh1Dimm1   : 0x%x\n", MemConfig->HotThresholdCh1Dimm1));
  DEBUG ((DEBUG_INFO, " WarmBudgetCh0Dimm0     : 0x%x\n", MemConfig->WarmBudgetCh0Dimm0));
  DEBUG ((DEBUG_INFO, " WarmBudgetCh0Dimm1     : 0x%x\n", MemConfig->WarmBudgetCh0Dimm1));
  DEBUG ((DEBUG_INFO, " WarmBudgetCh1Dimm0     : 0x%x\n", MemConfig->WarmBudgetCh1Dimm0));
  DEBUG ((DEBUG_INFO, " WarmBudgetCh1Dimm1     : 0x%x\n", MemConfig->WarmBudgetCh1Dimm1));
  DEBUG ((DEBUG_INFO, " HotBudgetCh0Dimm0      : 0x%x\n", MemConfig->HotBudgetCh0Dimm0));
  DEBUG ((DEBUG_INFO, " HotBudgetCh0Dimm1      : 0x%x\n", MemConfig->HotBudgetCh0Dimm1));
  DEBUG ((DEBUG_INFO, " HotBudgetCh1Dimm0      : 0x%x\n", MemConfig->HotBudgetCh1Dimm0));
  DEBUG ((DEBUG_INFO, " HotBudgetCh1Dimm1      : 0x%x\n", MemConfig->HotBudgetCh1Dimm1));
  DEBUG ((DEBUG_INFO, " IdleEnergyCh0Dimm0     : 0x%x\n", MemConfig->IdleEnergyCh0Dimm0));
  DEBUG ((DEBUG_INFO, " IdleEnergyCh0Dimm1     : 0x%x\n", MemConfig->IdleEnergyCh0Dimm1));
  DEBUG ((DEBUG_INFO, " IdleEnergyCh1Dimm0     : 0x%x\n", MemConfig->IdleEnergyCh1Dimm0));
  DEBUG ((DEBUG_INFO, " IdleEnergyCh1Dimm1     : 0x%x\n", MemConfig->IdleEnergyCh1Dimm1));
  DEBUG ((DEBUG_INFO, " PdEnergyCh0Dimm0       : 0x%x\n", MemConfig->PdEnergyCh0Dimm0));
  DEBUG ((DEBUG_INFO, " PdEnergyCh0Dimm1       : 0x%x\n", MemConfig->PdEnergyCh0Dimm1));
  DEBUG ((DEBUG_INFO, " PdEnergyCh1Dimm0       : 0x%x\n", MemConfig->PdEnergyCh1Dimm0));
  DEBUG ((DEBUG_INFO, " PdEnergyCh1Dimm1       : 0x%x\n", MemConfig->PdEnergyCh1Dimm1));
  DEBUG ((DEBUG_INFO, " ActEnergyCh0Dimm0      : 0x%x\n", MemConfig->ActEnergyCh0Dimm0));
  DEBUG ((DEBUG_INFO, " ActEnergyCh0Dimm1      : 0x%x\n", MemConfig->ActEnergyCh0Dimm1));
  DEBUG ((DEBUG_INFO, " ActEnergyCh1Dimm0      : 0x%x\n", MemConfig->ActEnergyCh1Dimm0));
  DEBUG ((DEBUG_INFO, " ActEnergyCh1Dimm1      : 0x%x\n", MemConfig->ActEnergyCh1Dimm1));
  DEBUG ((DEBUG_INFO, " RdEnergyCh0Dimm0       : 0x%x\n", MemConfig->RdEnergyCh0Dimm0));
  DEBUG ((DEBUG_INFO, " RdEnergyCh0Dimm1       : 0x%x\n", MemConfig->RdEnergyCh0Dimm1));
  DEBUG ((DEBUG_INFO, " RdEnergyCh1Dimm0       : 0x%x\n", MemConfig->RdEnergyCh1Dimm0));
  DEBUG ((DEBUG_INFO, " RdEnergyCh1Dimm1       : 0x%x\n", MemConfig->RdEnergyCh1Dimm1));
  DEBUG ((DEBUG_INFO, " WrEnergyCh0Dimm0       : 0x%x\n", MemConfig->WrEnergyCh0Dimm0));
  DEBUG ((DEBUG_INFO, " WrEnergyCh0Dimm1       : 0x%x\n", MemConfig->WrEnergyCh0Dimm1));
  DEBUG ((DEBUG_INFO, " WrEnergyCh1Dimm0       : 0x%x\n", MemConfig->WrEnergyCh1Dimm0));
  DEBUG ((DEBUG_INFO, " WrEnergyCh1Dimm1       : 0x%x\n", MemConfig->WrEnergyCh1Dimm1));
  DEBUG ((DEBUG_INFO, " SrefCfgEna             : 0x%x\n", MemConfig->SrefCfgEna));
  DEBUG ((DEBUG_INFO, " SrefCfgIdleTmr         : 0x%x\n", MemConfig->SrefCfgIdleTmr));
  DEBUG ((DEBUG_INFO, " ThrtCkeMinDefeat       : 0x%x\n", MemConfig->ThrtCkeMinDefeat));
  DEBUG ((DEBUG_INFO, " ThrtCkeMinTmr          : 0x%x\n", MemConfig->ThrtCkeMinTmr));
  DEBUG ((DEBUG_INFO, " ThrtCkeMinDefeatLpddr  : 0x%x\n", MemConfig->ThrtCkeMinDefeatLpddr));
  DEBUG ((DEBUG_INFO, " ThrtCkeMinTmrLpddr     : 0x%x\n", MemConfig->ThrtCkeMinTmrLpddr));
  DEBUG ((DEBUG_INFO, " AutoSelfRefreshSupport : 0x%x\n", MemConfig->AutoSelfRefreshSupport));
  DEBUG ((DEBUG_INFO, " ExtTemperatureSupport  : 0x%x\n", MemConfig->ExtTemperatureSupport));
  DEBUG ((DEBUG_INFO, " MaxRttWr               : 0x%x\n", MemConfig->MaxRttWr));
  DEBUG ((DEBUG_INFO, " MobilePlatform         : 0x%x\n", MemConfig->MobilePlatform));
  DEBUG ((DEBUG_INFO, " Force1Dpc              : 0x%x\n", MemConfig->Force1Dpc));


  DEBUG ((DEBUG_INFO, " ForceSingleRank        : 0x%x\n", MemConfig->ForceSingleRank));
  DEBUG ((DEBUG_INFO, " PciIndex               : 0x%x\n", MemConfig->PciIndex));
  DEBUG ((DEBUG_INFO, " PciData                : 0x%x\n", MemConfig->PciData));
  DEBUG ((DEBUG_INFO, " CkeRankMapping         : 0x%x\n", MemConfig->CkeRankMapping));
  DEBUG ((DEBUG_INFO, " RhPrevention           : 0x%x\n", MemConfig->RhPrevention));
  DEBUG ((DEBUG_INFO, " RhSolution             : 0x%x\n", MemConfig->RhSolution));
  DEBUG ((DEBUG_INFO, " RhActProbability       : 0x%x\n", MemConfig->RhActProbability));
  DEBUG ((DEBUG_INFO, " VttTermination         : 0x%x\n", MemConfig->VttTermination));
  DEBUG ((DEBUG_INFO, " VttCompForVsshi        : 0x%x\n", MemConfig->VttCompForVsshi));
  DEBUG ((DEBUG_INFO, " BerEnable              : 0x%x\n", MemConfig->BerEnable));
  for (Index = 0; Index < 4; Index++) {
    DEBUG ((DEBUG_INFO, " BerAddress[%d]      : 0x%x\n",Index , MemConfig->BerAddress[Index]));
  }
  DEBUG ((DEBUG_INFO, " CleanMemory            : 0x%x\n", MemConfigNoCrc->CleanMemory));
  DEBUG ((DEBUG_INFO, " MemTestOnWarmBoot      : 0x%x\n", MemConfigNoCrc->MemTestOnWarmBoot));
  DEBUG ((DEBUG_INFO, " ExitOnFailure          : 0x%x\n", MemConfig->ExitOnFailure));
  DEBUG ((DEBUG_INFO, " Vc1ReadMeter           : 0x%x\n", MemConfig->Vc1ReadMeter));
  DEBUG ((DEBUG_INFO, " SaGv                   : 0x%x\n", MemConfig->SaGv));
  DEBUG ((DEBUG_INFO, " FreqSaGvLow            : 0x%x\n FreqSaGvMid            : 0x%x\n",
          MemConfig->FreqSaGvLow, MemConfig->FreqSaGvMid));
  DEBUG ((DEBUG_INFO, " StrongWkLeaker         : 0x%x\n", MemConfig->StrongWkLeaker));
  DEBUG ((DEBUG_INFO, " CaVrefConfig           : 0x%x\n", MemConfig->CaVrefConfig));
  DEBUG ((DEBUG_INFO, " SimicsFlag             : 0x%x\n", MemConfig->SimicsFlag));
  DEBUG ((DEBUG_INFO, " PlatformMemorySize     : 0x%x\n", MemConfigNoCrc->PlatformMemorySize));
  DEBUG ((DEBUG_INFO, " SmramMask              : 0x%x\n", MemConfig->SmramMask));
  DEBUG ((DEBUG_INFO, " DllBwEn0: %d\n DllBwEn1: %d\n DllBwEn2: %d\n DllBwEn3: %d\n",
          MemConfig->DllBwEn0, MemConfig->DllBwEn1, MemConfig->DllBwEn2, MemConfig->DllBwEn3));
  DEBUG ((DEBUG_INFO, " RetrainOnFastFail: %d\n ForceOltmOrRefresh2x: %d\n",
          MemConfig->RetrainOnFastFail, MemConfig->ForceOltmOrRefresh2x));
  DEBUG ((DEBUG_INFO, " RmtPerTask: %u\n TrainTrace: %u\n", MemConfig->RmtPerTask, MemConfig->TrainTrace));
  DEBUG ((DEBUG_INFO, " tRd2RdSG               : 0x%x\n tRd2RdDG               : 0x%x\n", MemConfig->tRd2RdSG, MemConfig->tRd2RdDG));
  DEBUG ((DEBUG_INFO, " tRd2RdDR               : 0x%x\n tRd2RdDD               : 0x%x\n", MemConfig->tRd2RdDR, MemConfig->tRd2RdDD));
  DEBUG ((DEBUG_INFO, " tRd2WrSG               : 0x%x\n tRd2WrDG               : 0x%x\n", MemConfig->tRd2WrSG, MemConfig->tRd2WrDG));
  DEBUG ((DEBUG_INFO, " tRd2WrDR               : 0x%x\n tRd2WrDD               : 0x%x\n", MemConfig->tRd2WrDR, MemConfig->tRd2WrDD));
  DEBUG ((DEBUG_INFO, " tWr2RdSG               : 0x%x\n tWr2RdDG               : 0x%x\n", MemConfig->tWr2RdSG, MemConfig->tWr2RdDG));
  DEBUG ((DEBUG_INFO, " tWr2RdDR               : 0x%x\n tWr2RdDD               : 0x%x\n", MemConfig->tWr2RdDR, MemConfig->tWr2RdDD));
  DEBUG ((DEBUG_INFO, " tWr2WrSG               : 0x%x\n tWr2WrDG               : 0x%x\n", MemConfig->tWr2WrSG, MemConfig->tWr2WrDG));
  DEBUG ((DEBUG_INFO, " tWr2WrDR               : 0x%x\n tWr2WrDD               : 0x%x\n", MemConfig->tWr2WrDR, MemConfig->tWr2WrDD));
  DEBUG ((DEBUG_INFO, "------------------------ OVERCLOCKING_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", OcPreMemConfig->Header.Revision));
  ASSERT (OcPreMemConfig->Header.Revision == SA_OVERCLOCKING_CONFIG_REVISION);
  DEBUG ((DEBUG_INFO, " OcSupport : 0x%x\n", OcPreMemConfig->OcSupport));
  DEBUG ((DEBUG_INFO, " GtMaxOcRatio : 0x%x\n", OcPreMemConfig->GtMaxOcRatio));
  DEBUG ((DEBUG_INFO, " GtVoltageMode : 0x%x\n", OcPreMemConfig->GtVoltageMode));
  DEBUG ((DEBUG_INFO, " GtVoltageOffset : 0x%x\n", OcPreMemConfig->GtVoltageOffset));
  DEBUG ((DEBUG_INFO, " GtVoltageOverride : 0x%x\n", OcPreMemConfig->GtVoltageOverride));
  DEBUG ((DEBUG_INFO, " GtExtraTurboVoltage : 0x%x\n", OcPreMemConfig->GtExtraTurboVoltage));
  DEBUG ((DEBUG_INFO, " SaVoltageOffset : 0x%x\n", OcPreMemConfig->SaVoltageOffset));
  DEBUG ((DEBUG_INFO, " GtusMaxOcRatio : 0x%x\n", OcPreMemConfig->GtusMaxOcRatio));
  DEBUG ((DEBUG_INFO, " GtusVoltageMode : 0x%x\n", OcPreMemConfig->GtusVoltageMode));
  DEBUG ((DEBUG_INFO, " GtusVoltageOffset : 0x%x\n", OcPreMemConfig->GtusVoltageOffset));
  DEBUG ((DEBUG_INFO, " GtusVoltageOverride : 0x%x\n", OcPreMemConfig->GtusVoltageOverride));
  DEBUG ((DEBUG_INFO, " GtusExtraTurboVoltage : 0x%x\n", OcPreMemConfig->GtusExtraTurboVoltage));
  for (Index = 0; Index < SA_MC_MAX_CHANNELS; Index++) {
    DEBUG ((DEBUG_INFO, " DqByteMapCh%d        : ", Index));
    for (Index2 = 0; Index2 < SA_MRC_ITERATION_MAX; Index2++) {
      DEBUG ((DEBUG_INFO, "0x%02x ", MemConfigNoCrc->DqByteMap->DqByteMap[Index][Index2][0]));
      DEBUG ((DEBUG_INFO, "0x%02x ", MemConfigNoCrc->DqByteMap->DqByteMap[Index][Index2][1]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
  }
  for (Index = 0; Index < SA_MC_MAX_CHANNELS; Index++) {
    DEBUG ((DEBUG_INFO, " DqsMapCpu2DramCh%d   : ", Index));
    for (Index2 = 0; Index2 < SA_MC_MAX_BYTES_NO_ECC; Index2++) {
      DEBUG ((DEBUG_INFO, "%d ", MemConfigNoCrc->DqsMap->DqsMapCpu2Dram[Index][Index2]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
  }
  DEBUG ((DEBUG_INFO, "\n------------------------ SA Policy (PEI PreMem) Print END -----------------\n"));
  DEBUG_CODE_END ();
  return;
}

/**
  This function prints the PEI phase policy.

  @param[in] SiPolicyPpi - Instance of SI_POLICY_PPI
**/
VOID
EFIAPI
SaPrintPolicyPpi (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  )
{
  DEBUG_CODE_BEGIN ();
  INTN                                  Index;
  EFI_STATUS                            Status;
  GRAPHICS_PEI_CONFIG                   *GtConfig;
  PCIE_PEI_CONFIG                       *PciePeiConfig;
  SA_MISC_PEI_CONFIG                    *MiscPeiConfig;
  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gGraphicsPeiConfigGuid, (VOID *) &GtConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSaPciePeiConfigGuid, (VOID *) &PciePeiConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSaMiscPeiConfigGuid, (VOID *) &MiscPeiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "\n------------------------ SA Policy (PEI) Print BEGIN -----------------\n"));
  DEBUG ((DEBUG_INFO, "Revision : 0x%x\n", SiPolicyPpi->TableHeader.Header.Revision));
  ASSERT (SiPolicyPpi->TableHeader.Header.Revision == SI_POLICY_REVISION);

  DEBUG ((DEBUG_INFO, "------------------------ SA_MISC_PEI_CONFIG  -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", MiscPeiConfig->Header.Revision));
  ASSERT (MiscPeiConfig->Header.Revision == SA_MISC_PEI_CONFIG_REVISION);
  DEBUG ((DEBUG_INFO, " ChapDeviceEnable : 0x%x\n", MiscPeiConfig->ChapDeviceEnable));
  DEBUG ((DEBUG_INFO, " Device4Enable : 0x%x\n", MiscPeiConfig->Device4Enable));
  DEBUG ((DEBUG_INFO, " CridEnable : 0x%x\n", MiscPeiConfig->CridEnable));
  DEBUG ((DEBUG_INFO, " SkipPamLock : 0x%x\n", MiscPeiConfig->SkipPamLock));
  DEBUG ((DEBUG_INFO, " EdramTestMode : 0x%x\n", MiscPeiConfig->EdramTestMode));

  DEBUG ((DEBUG_INFO, "------------------------ GRAPHICS_PEI_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", GtConfig->Header.Revision));
  ASSERT (GtConfig->Header.Revision == GRAPHICS_PEI_CONFIG_REVISION);
  DEBUG ((DEBUG_INFO, " RenderStandby : 0x%x\n", GtConfig->RenderStandby));
  DEBUG ((DEBUG_INFO, " PmSupport : 0x%x\n", GtConfig->PmSupport));
  DEBUG ((DEBUG_INFO, " PavpEnable : 0x%x\n", GtConfig->PavpEnable));
  DEBUG ((DEBUG_INFO, " CdClock : 0x%x\n", GtConfig->CdClock));
  DEBUG ((DEBUG_INFO, " PeiGraphicsPeimInit : 0x%x\n", GtConfig->PeiGraphicsPeimInit));
  DEBUG ((DEBUG_INFO, " LogoPtr : 0x%x\n", GtConfig->LogoPtr));
  DEBUG ((DEBUG_INFO, " LogoSize : 0x%x\n", GtConfig->LogoSize));
  DEBUG ((DEBUG_INFO, " BltBufferAddress : 0x%x\n", GtConfig->BltBufferAddress));
  DEBUG ((DEBUG_INFO, " BltBufferSize : 0x%x\n", GtConfig->BltBufferSize));
  DEBUG ((DEBUG_INFO, " GraphicsConfigPtr : 0x%x\n", GtConfig->GraphicsConfigPtr));
  DEBUG ((DEBUG_INFO, " CdynmaxClampEnable : 0x%x\n", GtConfig->CdynmaxClampEnable));
  DEBUG ((DEBUG_INFO, " GtFreqMax : 0x%x\n", GtConfig->GtFreqMax));
  DEBUG ((DEBUG_INFO, " DisableTurboGt : 0x%x\n", GtConfig->DisableTurboGt));
  DEBUG ((DEBUG_INFO, " DdiPortEdp  : 0x%x\n", GtConfig->DdiConfiguration.DdiPortEdp));
  DEBUG ((DEBUG_INFO, " DdiPortBHpd : 0x%x\n", GtConfig->DdiConfiguration.DdiPortBHpd));
  DEBUG ((DEBUG_INFO, " DdiPortCHpd : 0x%x\n", GtConfig->DdiConfiguration.DdiPortCHpd));
  DEBUG ((DEBUG_INFO, " DdiPortDHpd : 0x%x\n", GtConfig->DdiConfiguration.DdiPortDHpd));
  DEBUG ((DEBUG_INFO, " DdiPortFHpd : 0x%x\n", GtConfig->DdiConfiguration.DdiPortFHpd));
  DEBUG ((DEBUG_INFO, " DdiPortBDdc : 0x%x\n", GtConfig->DdiConfiguration.DdiPortBDdc));
  DEBUG ((DEBUG_INFO, " DdiPortCDdc : 0x%x\n", GtConfig->DdiConfiguration.DdiPortCDdc));
  DEBUG ((DEBUG_INFO, " DdiPortDDdc : 0x%x\n", GtConfig->DdiConfiguration.DdiPortDDdc));
  DEBUG ((DEBUG_INFO, " DdiPortFDdc : 0x%x\n", GtConfig->DdiConfiguration.DdiPortFDdc));
  DEBUG ((DEBUG_INFO, " SkipS3CdClockInit : 0x%x\n", GtConfig->SkipS3CdClockInit));

  DEBUG ((DEBUG_INFO, "------------------------ PCIE_PEI_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", PciePeiConfig->Header.Revision));
  ASSERT (PciePeiConfig->Header.Revision == SA_PCIE_PEI_CONFIG_REVISION);
  DEBUG ((DEBUG_INFO, " DmiExtSync : 0x%x\n", PciePeiConfig->DmiExtSync));
  DEBUG ((DEBUG_INFO, " DmiIot : 0x%x\n", PciePeiConfig->DmiIot));
  DEBUG ((DEBUG_INFO, " DmiAspm : 0x%x\n", PciePeiConfig->DmiAspm));
  DEBUG ((DEBUG_INFO, "\n PegSlotPowerLimitValue[%d] :", SA_PEG_MAX_FUN));
  for (Index = 0; Index < SA_PEG_MAX_FUN; Index++) {
    DEBUG ((DEBUG_INFO, " 0x%x", PciePeiConfig->PegSlotPowerLimitValue[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n PegSlotPowerLimitScale[%d] :", SA_PEG_MAX_FUN));
  for (Index = 0; Index < SA_PEG_MAX_FUN; Index++) {
    DEBUG ((DEBUG_INFO, " 0x%x", PciePeiConfig->PegSlotPowerLimitScale[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n PegPhysicalSlotNumber[%d] :", SA_PEG_MAX_FUN));
  for (Index = 0; Index < SA_PEG_MAX_FUN; Index++) {
    DEBUG ((DEBUG_INFO, " 0x%x", PciePeiConfig->PegPhysicalSlotNumber[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n PegDeEmphasis[%d] :", SA_PEG_MAX_FUN));
  for (Index = 0; Index < SA_PEG_MAX_FUN; Index++) {
    DEBUG ((DEBUG_INFO, " 0x%x", PciePeiConfig->PegDeEmphasis[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n PegMaxPayload[%d] :", SA_PEG_MAX_FUN));
  for (Index = 0; Index < SA_PEG_MAX_FUN; Index++) {
    DEBUG ((DEBUG_INFO, " 0x%x", PciePeiConfig->PegMaxPayload[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  DEBUG ((DEBUG_INFO, "\n------------------------ SA Policy (PEI) Print END -----------------\n"));
  DEBUG_CODE_END ();
  return;
}
