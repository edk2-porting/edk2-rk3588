/** @file
  Implementation of Fsp PCH Policy Initialization.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PeiFspPolicyInitLib.h>

#include <Ppi/SiPolicy.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PchInfoLib.h>
#include <Library/ConfigBlockLib.h>

/**
  Performs FSP PCH PEI Policy pre mem initialization.

  @param[in][out]  FspmUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspPchPolicyInitPreMem (
  IN OUT FSPM_UPD    *FspmUpd
  )
{
  EFI_STATUS                       Status;
  UINTN                            Index;
  UINTN                            MaxPcieRootPorts;
  SI_PREMEM_POLICY_PPI             *SiPreMemPolicy;
  PCH_TRACE_HUB_PREMEM_CONFIG      *PchTraceHubPreMemConfig;
  PCH_SMBUS_PREMEM_CONFIG          *SmbusPreMemConfig;
  PCH_DCI_PREMEM_CONFIG            *DciPreMemConfig;
  PCH_HSIO_PCIE_PREMEM_CONFIG      *HsioPciePreMemConfig;
  PCH_HSIO_SATA_PREMEM_CONFIG      *HsioSataPreMemConfig;
  PCH_PCIE_RP_PREMEM_CONFIG        *PcieRpPreMemConfig;
  PCH_LPC_PREMEM_CONFIG            *LpcPreMemConfig;
  PCH_GENERAL_PREMEM_CONFIG        *PchGeneralPreMemConfig;
  PCH_WDT_PREMEM_CONFIG            *WdtPreMemConfig;
  PCH_HDAUDIO_PREMEM_CONFIG        *HdaPreMemConfig;
  PCH_ISH_PREMEM_CONFIG            *IshPreMemConfig;
  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP UpdatePeiPchPolicyPreMem\n"));
  DEBUG((DEBUG_INFO | DEBUG_INIT, "FspmUpd = 0x%x\n", FspmUpd));
  //
  // Locate PchPreMemPolicyPpi
  //
  SiPreMemPolicy = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPchTraceHubPreMemConfigGuid, (VOID *) &PchTraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gSmbusPreMemConfigGuid, (VOID *) &SmbusPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gDciPreMemConfigGuid, (VOID *) &DciPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHsioPciePreMemConfigGuid, (VOID *) &HsioPciePreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHsioSataPreMemConfigGuid, (VOID *) &HsioSataPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gLpcPreMemConfigGuid, (VOID *) &LpcPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gWatchDogPreMemConfigGuid, (VOID *) &WdtPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPcieRpPreMemConfigGuid, (VOID *) &PcieRpPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHdAudioPreMemConfigGuid, (VOID *) &HdaPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gIshPreMemConfigGuid, (VOID *) &IshPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  DEBUG((DEBUG_INFO | DEBUG_INIT, "WYQ UpdatePeiPchPolicyPreMem\n"));
  //
  // Update PCIE RP policies
  //
//  MaxPcieRootPorts = 16;

  MaxPcieRootPorts = GetPchMaxPciePortNum ();
//  MaxPcieRootPorts = 16;
  FspmUpd->FspmConfig.PcieRpEnableMask = PcieRpPreMemConfig->RpEnabledMask & ((1 << MaxPcieRootPorts) - 1);
  FspmUpd->FspmConfig.PcieImrEnabled = PcieRpPreMemConfig->PcieImrEnabled;
  FspmUpd->FspmConfig.PcieImrSize = PcieRpPreMemConfig->PcieImrSize;
  FspmUpd->FspmConfig.ImrRpSelection = PcieRpPreMemConfig->ImrRpSelection;
  //
  // Update TraceHub policies
  //
  FspmUpd->FspmConfig.PchTraceHubMode = (UINT8) PchTraceHubPreMemConfig->EnableMode;
  FspmUpd->FspmConfig.PchTraceHubMemReg0Size = (UINT8) PchTraceHubPreMemConfig->MemReg0Size;
  FspmUpd->FspmConfig.PchTraceHubMemReg1Size = (UINT8) PchTraceHubPreMemConfig->MemReg1Size;

  //
  // Update Smbus policies
  //
  FspmUpd->FspmConfig.SmbusEnable = (UINT8)SmbusPreMemConfig->Enable;
  FspmUpd->FspmConfig.SmbusArpEnable = (UINT8)SmbusPreMemConfig->ArpEnable;
  FspmUpd->FspmTestConfig.SmbusDynamicPowerGating = (UINT8)SmbusPreMemConfig->DynamicPowerGating;
  FspmUpd->FspmTestConfig.SmbusSpdWriteDisable = (UINT8)SmbusPreMemConfig->SpdWriteDisable;
  FspmUpd->FspmConfig.PchSmbAlertEnable = (UINT8)SmbusPreMemConfig->SmbAlertEnable;
  FspmUpd->FspmConfig.PchSmbusIoBase = (UINT16)SmbusPreMemConfig->SmbusIoBase;
  FspmUpd->FspmConfig.PchNumRsvdSmbusAddresses = (UINT8)SmbusPreMemConfig->NumRsvdSmbusAddresses;
  FspmUpd->FspmConfig.RsvdSmbusAddressTablePtr = (UINT32)SmbusPreMemConfig->RsvdSmbusAddressTable;

  DEBUG((DEBUG_INFO | DEBUG_INIT, "WYQ1 UpdatePeiPchPolicyPreMem\n"));
  //
  // Update Dci policies
  //
  FspmUpd->FspmConfig.PlatformDebugConsent = (UINT8)DciPreMemConfig->PlatformDebugConsent;
  DEBUG((DEBUG_INFO | DEBUG_INIT, "WYQ11 UpdatePeiPchPolicyPreMem\n"));
  FspmUpd->FspmConfig.DciUsb3TypecUfpDbg = (UINT8)DciPreMemConfig->DciUsb3TypecUfpDbg;
  //
  // Update HSIO PCIE policies
  //
  for (Index = 0; Index < MaxPcieRootPorts; Index ++) {
    FspmUpd->FspmConfig.PchPcieHsioRxSetCtleEnable[Index]           = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioRxSetCtleEnable;
    FspmUpd->FspmConfig.PchPcieHsioRxSetCtle[Index]                 = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioRxSetCtle;
    FspmUpd->FspmConfig.PchPcieHsioTxGen1DownscaleAmpEnable[Index]  = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen1DownscaleAmpEnable;
    FspmUpd->FspmConfig.PchPcieHsioTxGen1DownscaleAmp[Index]        = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen1DownscaleAmp;
    FspmUpd->FspmConfig.PchPcieHsioTxGen2DownscaleAmpEnable[Index]  = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen2DownscaleAmpEnable;
    FspmUpd->FspmConfig.PchPcieHsioTxGen2DownscaleAmp[Index]        = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen2DownscaleAmp;
    FspmUpd->FspmConfig.PchPcieHsioTxGen3DownscaleAmpEnable[Index]  = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen3DownscaleAmpEnable;
    FspmUpd->FspmConfig.PchPcieHsioTxGen3DownscaleAmp[Index]        = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen3DownscaleAmp;
    FspmUpd->FspmConfig.PchPcieHsioTxGen1DeEmphEnable[Index]        = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen1DeEmphEnable;
    FspmUpd->FspmConfig.PchPcieHsioTxGen1DeEmph[Index]              = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen1DeEmph;
    FspmUpd->FspmConfig.PchPcieHsioTxGen2DeEmph3p5Enable[Index]     = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph3p5Enable;
    FspmUpd->FspmConfig.PchPcieHsioTxGen2DeEmph3p5[Index]           = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph3p5;
    FspmUpd->FspmConfig.PchPcieHsioTxGen2DeEmph6p0Enable[Index]     = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph6p0Enable;
    FspmUpd->FspmConfig.PchPcieHsioTxGen2DeEmph6p0[Index]           = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph6p0;
  }

  //
  // Update HSIO SATA policies
  //
  for (Index = 0; Index < PCH_MAX_SATA_PORTS; Index ++) {
    FspmUpd->FspmConfig.PchSataHsioRxGen1EqBoostMagEnable[Index]    = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioRxGen1EqBoostMagEnable;
    FspmUpd->FspmConfig.PchSataHsioRxGen1EqBoostMag[Index]          = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioRxGen1EqBoostMag;
    FspmUpd->FspmConfig.PchSataHsioRxGen2EqBoostMagEnable[Index]    = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioRxGen2EqBoostMagEnable;
    FspmUpd->FspmConfig.PchSataHsioRxGen2EqBoostMag[Index]          = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioRxGen2EqBoostMag;
    FspmUpd->FspmConfig.PchSataHsioRxGen3EqBoostMagEnable[Index]    = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioRxGen3EqBoostMagEnable;
    FspmUpd->FspmConfig.PchSataHsioRxGen3EqBoostMag[Index]          = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioRxGen3EqBoostMag;
    FspmUpd->FspmConfig.PchSataHsioTxGen1DownscaleAmpEnable[Index]  = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DownscaleAmpEnable;
    FspmUpd->FspmConfig.PchSataHsioTxGen1DownscaleAmp[Index]        = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DownscaleAmp;
    FspmUpd->FspmConfig.PchSataHsioTxGen2DownscaleAmpEnable[Index]  = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DownscaleAmpEnable;
    FspmUpd->FspmConfig.PchSataHsioTxGen2DownscaleAmp[Index]        = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DownscaleAmp;
    FspmUpd->FspmConfig.PchSataHsioTxGen3DownscaleAmpEnable[Index]  = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen3DownscaleAmpEnable;
    FspmUpd->FspmConfig.PchSataHsioTxGen3DownscaleAmp[Index]        = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen3DownscaleAmp;
    FspmUpd->FspmConfig.PchSataHsioTxGen1DeEmphEnable[Index]        = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DeEmphEnable;
    FspmUpd->FspmConfig.PchSataHsioTxGen1DeEmph[Index]              = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DeEmph;
    FspmUpd->FspmConfig.PchSataHsioTxGen2DeEmphEnable[Index]        = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DeEmphEnable;
    FspmUpd->FspmConfig.PchSataHsioTxGen2DeEmph[Index]              = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DeEmph;
    FspmUpd->FspmConfig.PchSataHsioTxGen3DeEmphEnable[Index]        = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen3DeEmphEnable;
    FspmUpd->FspmConfig.PchSataHsioTxGen3DeEmph[Index]              = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen3DeEmph;
  }
  DEBUG((DEBUG_INFO | DEBUG_INIT, "WYQ2 UpdatePeiPchPolicyPreMem\n"));
  // Update LPC policies
  //
  FspmUpd->FspmConfig.PchLpcEnhancePort8xhDecoding = (UINT8)LpcPreMemConfig->EnhancePort8xhDecoding;

  //
  // Update Pch General Premem policies
  //
  FspmUpd->FspmConfig.PchPort80Route = (UINT8)PchGeneralPreMemConfig->Port80Route;

  //
  // Update Wdt policies
  //
  FspmUpd->FspmTestConfig.WdtDisableAndLock = (UINT8)WdtPreMemConfig->DisableAndLock;

  //
  // HdAudioConfig
  //
  FspmUpd->FspmConfig.PchHdaEnable = (UINT8)HdaPreMemConfig->Enable;

  //
  // IshConfig
  //
  FspmUpd->FspmConfig.PchIshEnable = (UINT8)IshPreMemConfig->Enable;

  DEBUG((DEBUG_INFO | DEBUG_INIT, "WYQ3 UpdatePeiPchPolicyPreMem\n"));
  return EFI_SUCCESS;
}

/**
  Performs FSP PCH PEI Policy initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspPchPolicyInit (
  IN OUT FSPS_UPD    *FspsUpd
  )
{
  EFI_STATUS                   Status;
  UINTN                        Index;
  UINTN                        MaxPcieRootPorts;
  UINT8                        Data8;
  SI_POLICY_PPI                *SiPolicy;
  PCH_LAN_CONFIG               *LanConfig;
  PCH_HDAUDIO_CONFIG           *HdAudioConfig;
  PCH_SCS_CONFIG               *ScsConfig;
  PCH_ISH_CONFIG               *IshConfig;
  PCH_SATA_CONFIG              *SataConfig;
  USB_CONFIG                   *UsbConfig;
  PCH_SERIAL_IO_CONFIG         *SerialIoConfig;
  PCH_INTERRUPT_CONFIG         *InterruptConfig;
  PCH_LOCK_DOWN_CONFIG         *LockDownConfig;
  PCH_CNVI_CONFIG              *CnviConfig;
  PCH_HSIO_CONFIG              *HsioConfig;
  PCH_ESPI_CONFIG              *EspiConfig;
  PCH_PCIE_CONFIG              *PcieRpConfig;
  PCH_DMI_CONFIG               *DmiConfig;
  PCH_FLASH_PROTECTION_CONFIG  *FlashProtectionConfig;
  PCH_IOAPIC_CONFIG            *IoApicConfig;
  PCH_P2SB_CONFIG              *P2sbConfig;
  PCH_GENERAL_CONFIG           *PchGeneralConfig;
  PCH_PM_CONFIG                *PmConfig;
  PCH_LPC_SIRQ_CONFIG          *PchSerialIrqConfig;
  PCH_THERMAL_CONFIG           *PchThermalConfig;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP UpdatePeiPchPolicy\n"));
  //
  // Locate SiPolicyPpi
  //
  SiPolicy = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicy
             );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLanConfigGuid, (VOID *) &LanConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gHdAudioConfigGuid, (VOID *) &HdAudioConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gScsConfigGuid, (VOID *) &ScsConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gIshConfigGuid, (VOID *) &IshConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gSataConfigGuid, (VOID *) &SataConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsbConfigGuid, (VOID *) &UsbConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gSerialIoConfigGuid, (VOID *) &SerialIoConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gInterruptConfigGuid, (VOID *) &InterruptConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gLockDownConfigGuid, (VOID *) &LockDownConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPcieRpConfigGuid, (VOID *) &PcieRpConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gDmiConfigGuid, (VOID *) &DmiConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gFlashProtectionConfigGuid, (VOID *) &FlashProtectionConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gIoApicConfigGuid, (VOID *) &IoApicConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gP2sbConfigGuid, (VOID *) &P2sbConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gSerialIrqConfigGuid, (VOID *) &PchSerialIrqConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gThermalConfigGuid, (VOID *) &PchThermalConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gCnviConfigGuid, (VOID *) &CnviConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gHsioConfigGuid, (VOID *) &HsioConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gEspiConfigGuid, (VOID *) &EspiConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Update LAN policies
  //
  FspsUpd->FspsConfig.PchLanEnable          = (UINT8)LanConfig->Enable;
  FspsUpd->FspsConfig.PchLanLtrEnable       = (UINT8)LanConfig->LtrEnable;

  //
  // Update HDA policies
  //
  FspsUpd->FspsConfig.PchHdaDspEnable             = (UINT8)HdAudioConfig->DspEnable;
  FspsUpd->FspsConfig.PchHdaPme                   = (UINT8)HdAudioConfig->Pme;
  FspsUpd->FspsConfig.PchHdaVcType                = (UINT8)HdAudioConfig->VcType;
  FspsUpd->FspsConfig.PchHdaLinkFrequency         = (UINT8)HdAudioConfig->HdAudioLinkFrequency;
  FspsUpd->FspsConfig.PchHdaIDispLinkFrequency    = (UINT8)HdAudioConfig->IDispLinkFrequency;
  FspsUpd->FspsConfig.PchHdaIDispLinkTmode        = (UINT8)HdAudioConfig->IDispLinkTmode;
  FspsUpd->FspsConfig.PchHdaDspUaaCompliance      = (UINT8)HdAudioConfig->DspUaaCompliance;
  FspsUpd->FspsConfig.PchHdaIDispCodecDisconnect  = (UINT8)HdAudioConfig->IDispCodecDisconnect;
  FspsUpd->FspsConfig.PchHdaCodecSxWakeCapability = (UINT8)HdAudioConfig->CodecSxWakeCapability;
  FspsUpd->FspsTestConfig.PchHdaResetWaitTimer    = (UINT16)HdAudioConfig->ResetWaitTimer;
  FspsUpd->FspsConfig.PchHdaVerbTableEntryNum     = HdAudioConfig->VerbTableEntryNum;
  FspsUpd->FspsConfig.PchHdaVerbTablePtr          = HdAudioConfig->VerbTablePtr;
  FspsUpd->FspsConfig.PchHdaAudioLinkHda          = (UINT8)HdAudioConfig->AudioLinkHda;
  FspsUpd->FspsConfig.PchHdaAudioLinkDmic0        = (UINT8)HdAudioConfig->AudioLinkDmic0;
  FspsUpd->FspsConfig.PchHdaAudioLinkDmic1        = (UINT8)HdAudioConfig->AudioLinkDmic1;
  FspsUpd->FspsConfig.PchHdaAudioLinkSsp0         = (UINT8)HdAudioConfig->AudioLinkSsp0;
  FspsUpd->FspsConfig.PchHdaAudioLinkSsp1         = (UINT8)HdAudioConfig->AudioLinkSsp1;
  FspsUpd->FspsConfig.PchHdaAudioLinkSsp2         = (UINT8)HdAudioConfig->AudioLinkSsp2;
  FspsUpd->FspsConfig.PchHdaAudioLinkSndw1        = (UINT8)HdAudioConfig->AudioLinkSndw1;
  FspsUpd->FspsConfig.PchHdaAudioLinkSndw2        = (UINT8)HdAudioConfig->AudioLinkSndw2;
  FspsUpd->FspsConfig.PchHdaAudioLinkSndw3        = (UINT8)HdAudioConfig->AudioLinkSndw3;
  FspsUpd->FspsConfig.PchHdaAudioLinkSndw4        = (UINT8)HdAudioConfig->AudioLinkSndw4;
  FspsUpd->FspsConfig.PchHdaSndwBufferRcomp       = (UINT8)HdAudioConfig->SndwBufferRcomp;

  //
  // Update SCS policies
  //
  FspsUpd->FspsConfig.ScsEmmcEnabled = (UINT8)ScsConfig->ScsEmmcEnabled;
  FspsUpd->FspsConfig.ScsEmmcHs400Enabled = (UINT8)ScsConfig->ScsEmmcHs400Enabled;
  FspsUpd->FspsConfig.ScsSdCardEnabled = (UINT8)ScsConfig->ScsSdcardEnabled;
  FspsUpd->FspsConfig.SdCardPowerEnableActiveHigh = (UINT8)ScsConfig->SdCardPowerEnableActiveHigh;
#ifdef CFL_SIMICS
  FspsUpd->FspsConfig.ScsUfsEnabled    = 0;
#else
  FspsUpd->FspsConfig.ScsUfsEnabled    = (UINT8)ScsConfig->ScsUfsEnabled;
#endif
  FspsUpd->FspsConfig.PchScsEmmcHs400TuningRequired = (UINT8)ScsConfig->ScsEmmcHs400TuningRequired;
  FspsUpd->FspsConfig.PchScsEmmcHs400DllDataValid   = (UINT8)ScsConfig->ScsEmmcHs400DllDataValid;
  FspsUpd->FspsConfig.PchScsEmmcHs400RxStrobeDll1   = (UINT8)ScsConfig->ScsEmmcHs400RxStrobeDll1;
  FspsUpd->FspsConfig.PchScsEmmcHs400TxDataDll      = (UINT8)ScsConfig->ScsEmmcHs400TxDataDll;
  FspsUpd->FspsConfig.PchScsEmmcHs400DriverStrength = (UINT8)ScsConfig->ScsEmmcHs400DriverStrength;

  //
  // Update ISH policies
  //
  FspsUpd->FspsConfig.PchIshSpiGpioAssign   = (UINT8)IshConfig->SpiGpioAssign;
  FspsUpd->FspsConfig.PchIshUart0GpioAssign = (UINT8)IshConfig->Uart0GpioAssign;
  FspsUpd->FspsConfig.PchIshUart1GpioAssign = (UINT8)IshConfig->Uart1GpioAssign;
  FspsUpd->FspsConfig.PchIshI2c0GpioAssign  = (UINT8)IshConfig->I2c0GpioAssign;
  FspsUpd->FspsConfig.PchIshI2c1GpioAssign  = (UINT8)IshConfig->I2c1GpioAssign;
  FspsUpd->FspsConfig.PchIshI2c2GpioAssign  = (UINT8)IshConfig->I2c2GpioAssign;
  FspsUpd->FspsConfig.PchIshGp0GpioAssign   = (UINT8)IshConfig->Gp0GpioAssign;
  FspsUpd->FspsConfig.PchIshGp1GpioAssign   = (UINT8)IshConfig->Gp1GpioAssign;
  FspsUpd->FspsConfig.PchIshGp2GpioAssign   = (UINT8)IshConfig->Gp2GpioAssign;
  FspsUpd->FspsConfig.PchIshGp3GpioAssign   = (UINT8)IshConfig->Gp3GpioAssign;
  FspsUpd->FspsConfig.PchIshGp4GpioAssign   = (UINT8)IshConfig->Gp4GpioAssign;
  FspsUpd->FspsConfig.PchIshGp5GpioAssign   = (UINT8)IshConfig->Gp5GpioAssign;
  FspsUpd->FspsConfig.PchIshGp6GpioAssign   = (UINT8)IshConfig->Gp6GpioAssign;
  FspsUpd->FspsConfig.PchIshGp7GpioAssign   = (UINT8)IshConfig->Gp7GpioAssign;
  FspsUpd->FspsConfig.PchIshPdtUnlock       = (UINT8)IshConfig->PdtUnlock;

  //
  // Update PCIE RP RootPort policies
  //
  MaxPcieRootPorts = GetPchMaxPciePortNum ();
  FspsUpd->FspsConfig.PcieRpDpcMask = 0;
  FspsUpd->FspsConfig.PcieRpDpcExtensionsMask = 0;
  FspsUpd->FspsConfig.PcieRpPtmMask = 0;
  for (Index = 0; Index < MaxPcieRootPorts; Index ++) {
    FspsUpd->FspsConfig.PcieRpHotPlug[Index] = (UINT8)PcieRpConfig->RootPort[Index].HotPlug;
    FspsUpd->FspsConfig.PcieRpSlotImplemented[Index] = (UINT8)PcieRpConfig->RootPort[Index].SlotImplemented;
    FspsUpd->FspsConfig.PcieRpPmSci[Index] = (UINT8)PcieRpConfig->RootPort[Index].PmSci;
    FspsUpd->FspsConfig.PcieRpExtSync[Index] = (UINT8)PcieRpConfig->RootPort[Index].ExtSync;
    FspsUpd->FspsConfig.PcieRpTransmitterHalfSwing[Index] = (UINT8)PcieRpConfig->RootPort[Index].TransmitterHalfSwing;
    FspsUpd->FspsConfig.PcieRpClkReqDetect[Index] = (UINT8)PcieRpConfig->RootPort[Index].ClkReqDetect;
    FspsUpd->FspsConfig.PcieRpAdvancedErrorReporting[Index] = (UINT8)PcieRpConfig->RootPort[Index].AdvancedErrorReporting;
    FspsUpd->FspsConfig.PcieRpUnsupportedRequestReport[Index] = (UINT8)PcieRpConfig->RootPort[Index].UnsupportedRequestReport;
    FspsUpd->FspsConfig.PcieRpFatalErrorReport[Index] = (UINT8)PcieRpConfig->RootPort[Index].FatalErrorReport;
    FspsUpd->FspsConfig.PcieRpNoFatalErrorReport[Index] = (UINT8)PcieRpConfig->RootPort[Index].NoFatalErrorReport;
    FspsUpd->FspsConfig.PcieRpCorrectableErrorReport[Index] = (UINT8)PcieRpConfig->RootPort[Index].CorrectableErrorReport;
    FspsUpd->FspsConfig.PcieRpSystemErrorOnFatalError[Index] = (UINT8)PcieRpConfig->RootPort[Index].SystemErrorOnFatalError;
    FspsUpd->FspsConfig.PcieRpSystemErrorOnNonFatalError[Index] = (UINT8)PcieRpConfig->RootPort[Index].SystemErrorOnNonFatalError;
    FspsUpd->FspsConfig.PcieRpSystemErrorOnCorrectableError[Index] = (UINT8)PcieRpConfig->RootPort[Index].SystemErrorOnCorrectableError;
    FspsUpd->FspsConfig.PcieRpMaxPayload[Index] = (UINT8)PcieRpConfig->RootPort[Index].MaxPayload;
    if (PcieRpConfig->RootPort[Index].DpcEnabled) {
      FspsUpd->FspsConfig.PcieRpDpcMask |= (BIT0<<Index);
    }
    if (PcieRpConfig->RootPort[Index].RpDpcExtensionsEnabled) {
      FspsUpd->FspsConfig.PcieRpDpcExtensionsMask |= (BIT0<<Index);
    }
    if (PcieRpConfig->RootPort[Index].PtmEnabled) {
      FspsUpd->FspsConfig.PcieRpPtmMask |= (BIT0<<Index);
    }
    FspsUpd->FspsConfig.PcieRpPcieSpeed[Index] = (UINT8)PcieRpConfig->RootPort[Index].PcieSpeed;
    FspsUpd->FspsConfig.PcieRpGen3EqPh3Method[Index] = (UINT8)PcieRpConfig->RootPort[Index].Gen3EqPh3Method;
    FspsUpd->FspsConfig.PcieRpPhysicalSlotNumber[Index] = (UINT8)PcieRpConfig->RootPort[Index].PhysicalSlotNumber;
    FspsUpd->FspsConfig.PcieRpCompletionTimeout[Index] = (UINT8)PcieRpConfig->RootPort[Index].CompletionTimeout;
    FspsUpd->FspsConfig.PcieRpAspm[Index] = (UINT8)PcieRpConfig->RootPort[Index].Aspm;
    FspsUpd->FspsConfig.PcieRpL1Substates[Index] = (UINT8)PcieRpConfig->RootPort[Index].L1Substates;
    FspsUpd->FspsConfig.PcieRpLtrEnable[Index] = (UINT8)PcieRpConfig->RootPort[Index].LtrEnable;
    FspsUpd->FspsConfig.PcieRpLtrConfigLock[Index] = (UINT8)PcieRpConfig->RootPort[Index].LtrConfigLock;
    FspsUpd->FspsConfig.PcieRpAcsEnabled[Index] = (UINT8)PcieRpConfig->RootPort[Index].AcsEnabled;
    FspsUpd->FspsConfig.PcieRpDetectTimeoutMs[Index] = (UINT16)PcieRpConfig->RootPort[Index].DetectTimeoutMs;
    FspsUpd->FspsConfig.PcieRootPortGen2PllL1CgDisable[Index] = (UINT8)PcieRpConfig->RootPort[Index].PcieRootPortGen2PllL1CgDisable;

    FspsUpd->FspsTestConfig.PcieRpLtrMaxSnoopLatency[Index] = (UINT16)PcieRpConfig->RootPort[Index].LtrMaxSnoopLatency;
    FspsUpd->FspsTestConfig.PcieRpLtrMaxNoSnoopLatency[Index] = (UINT16)PcieRpConfig->RootPort[Index].LtrMaxNoSnoopLatency;

    FspsUpd->FspsTestConfig.PcieRpSnoopLatencyOverrideMode[Index] = (UINT8)PcieRpConfig->RootPort[Index].SnoopLatencyOverrideMode;
    FspsUpd->FspsTestConfig.PcieRpSnoopLatencyOverrideMultiplier[Index] = (UINT8)PcieRpConfig->RootPort[Index].SnoopLatencyOverrideMultiplier;
    FspsUpd->FspsTestConfig.PcieRpSnoopLatencyOverrideValue[Index] = (UINT16)PcieRpConfig->RootPort[Index].SnoopLatencyOverrideValue;

    FspsUpd->FspsTestConfig.PcieRpNonSnoopLatencyOverrideMode[Index] = (UINT8)PcieRpConfig->RootPort[Index].NonSnoopLatencyOverrideMode;
    FspsUpd->FspsTestConfig.PcieRpNonSnoopLatencyOverrideMultiplier[Index] = (UINT8)PcieRpConfig->RootPort[Index].NonSnoopLatencyOverrideMultiplier;
    FspsUpd->FspsTestConfig.PcieRpNonSnoopLatencyOverrideValue[Index] = (UINT16)PcieRpConfig->RootPort[Index].NonSnoopLatencyOverrideValue;

    FspsUpd->FspsTestConfig.PcieRpSlotPowerLimitScale[Index] = (UINT8)PcieRpConfig->RootPort[Index].SlotPowerLimitScale;
    FspsUpd->FspsTestConfig.PcieRpSlotPowerLimitValue[Index] = (UINT16)PcieRpConfig->RootPort[Index].SlotPowerLimitValue;
    FspsUpd->FspsTestConfig.PcieRpUptp[Index] = (UINT8)PcieRpConfig->RootPort[Index].Uptp;
    FspsUpd->FspsTestConfig.PcieRpDptp[Index] = (UINT8)PcieRpConfig->RootPort[Index].Dptp;

  }
  for (Index = 0; Index < GetPchMaxPcieClockNum (); Index ++) {
    FspsUpd->FspsConfig.PcieClkSrcUsage[Index] = PcieRpConfig->PcieClock[Index].Usage;
    FspsUpd->FspsConfig.PcieClkSrcClkReq[Index] = PcieRpConfig->PcieClock[Index].ClkReq;
  }

  //
  // Update PCIE RP EqPh3LaneParam policies
  //
  for (Index = 0; Index < MaxPcieRootPorts; Index ++) {
    FspsUpd->FspsConfig.PcieEqPh3LaneParamCm[Index] = (UINT8)PcieRpConfig->EqPh3LaneParam[Index].Cm;
    FspsUpd->FspsConfig.PcieEqPh3LaneParamCp[Index] = (UINT8)PcieRpConfig->EqPh3LaneParam[Index].Cp;
  }

  //
  // Update PCIE RP SwEqCoeffList policies
  //
  for (Index = 0; Index < PCH_PCIE_SWEQ_COEFFS_MAX; Index ++) {
    FspsUpd->FspsConfig.PcieSwEqCoeffListCm[Index] = (UINT8)PcieRpConfig->SwEqCoeffList[Index].Cm;
    FspsUpd->FspsConfig.PcieSwEqCoeffListCp[Index] = (UINT8)PcieRpConfig->SwEqCoeffList[Index].Cp;
  }

  //
  // Update PCIE RP policies
  //
  FspsUpd->FspsTestConfig.PcieEnablePort8xhDecode        = (UINT8)PcieRpConfig->EnablePort8xhDecode;
  FspsUpd->FspsTestConfig.PchPciePort8xhDecodePortIndex  = (UINT8)PcieRpConfig->PchPciePort8xhDecodePortIndex;
  FspsUpd->FspsConfig.PcieDisableRootPortClockGating = (UINT8)PcieRpConfig->DisableRootPortClockGating;
  FspsUpd->FspsConfig.PcieEnablePeerMemoryWrite      = (UINT8)PcieRpConfig->EnablePeerMemoryWrite;
  FspsUpd->FspsConfig.PcieComplianceTestMode         = (UINT8)PcieRpConfig->ComplianceTestMode;
  FspsUpd->FspsConfig.PcieRpFunctionSwap             = (UINT8)PcieRpConfig->RpFunctionSwap;
  FspsUpd->FspsConfig.PchPcieDeviceOverrideTablePtr  = PcieRpConfig->PcieDeviceOverrideTablePtr;

  //
  // Update Sata Policies
  //
  FspsUpd->FspsConfig.SataEnable                     = (UINT8)SataConfig->Enable;
  FspsUpd->FspsTestConfig.SataTestMode               = (UINT8)SataConfig->TestMode;
  FspsUpd->FspsConfig.SataSalpSupport                = (UINT8)SataConfig->SalpSupport;
  FspsUpd->FspsConfig.SataPwrOptEnable = (UINT8)SataConfig->PwrOptEnable;
  FspsUpd->FspsConfig.EsataSpeedLimit  = (UINT8)SataConfig->EsataSpeedLimit;
  FspsUpd->FspsConfig.SataLedEnable    = (UINT8)SataConfig->LedEnable;
  FspsUpd->FspsConfig.SataMode         = (UINT8)SataConfig->SataMode;
  FspsUpd->FspsConfig.SataSpeedLimit   = (UINT8)SataConfig->SpeedLimit;

  for (Index = 0; Index < PCH_MAX_SATA_PORTS; Index++) {
    FspsUpd->FspsConfig.SataPortsEnable[Index] = (UINT8)SataConfig->PortSettings[Index].Enable;
    FspsUpd->FspsConfig.SataPortsHotPlug[Index]     = (UINT8)SataConfig->PortSettings[Index].HotPlug;
    FspsUpd->FspsConfig.SataPortsInterlockSw[Index] = (UINT8)SataConfig->PortSettings[Index].InterlockSw;
    FspsUpd->FspsConfig.SataPortsExternal[Index]    = (UINT8)SataConfig->PortSettings[Index].External;
    FspsUpd->FspsConfig.SataPortsSpinUp[Index]      = (UINT8)SataConfig->PortSettings[Index].SpinUp;
    FspsUpd->FspsConfig.SataPortsSolidStateDrive[Index]  = (UINT8)SataConfig->PortSettings[Index].SolidStateDrive;
    FspsUpd->FspsConfig.SataPortsDevSlp[Index] = (UINT8)SataConfig->PortSettings[Index].DevSlp;
    FspsUpd->FspsConfig.SataPortsEnableDitoConfig[Index] = (UINT8)SataConfig->PortSettings[Index].EnableDitoConfig;
    FspsUpd->FspsConfig.SataPortsDmVal[Index]       = (UINT8)SataConfig->PortSettings[Index].DmVal;
    FspsUpd->FspsConfig.SataPortsDitoVal[Index]     = (UINT16)SataConfig->PortSettings[Index].DitoVal;
    FspsUpd->FspsConfig.SataPortsZpOdd[Index]       = (UINT8)SataConfig->PortSettings[Index].ZpOdd;
  }

  FspsUpd->FspsConfig.SataRstRaidDeviceId            = (UINT8)SataConfig->Rst.RaidDeviceId;
  FspsUpd->FspsConfig.SataRstInterrupt               = (UINT8)SataConfig->Rst.SataRstInterrupt;
  FspsUpd->FspsConfig.SataRstRaid0                   = (UINT8)SataConfig->Rst.Raid0;
  FspsUpd->FspsConfig.SataRstRaid1                   = (UINT8)SataConfig->Rst.Raid1;
  FspsUpd->FspsConfig.SataRstRaid10                  = (UINT8)SataConfig->Rst.Raid10;
  FspsUpd->FspsConfig.SataRstRaid5                   = (UINT8)SataConfig->Rst.Raid5;
  FspsUpd->FspsConfig.SataRstIrrt                    = (UINT8)SataConfig->Rst.Irrt;
  FspsUpd->FspsConfig.SataRstOromUiBanner            = (UINT8)SataConfig->Rst.OromUiBanner;
  FspsUpd->FspsConfig.SataRstOromUiDelay             = (UINT8)SataConfig->Rst.OromUiDelay;
  FspsUpd->FspsConfig.SataRstHddUnlock               = (UINT8)SataConfig->Rst.HddUnlock;
  FspsUpd->FspsConfig.SataRstLedLocate               = (UINT8)SataConfig->Rst.LedLocate;
  FspsUpd->FspsConfig.SataRstIrrtOnly                = (UINT8)SataConfig->Rst.IrrtOnly;
  FspsUpd->FspsConfig.SataRstSmartStorage            = (UINT8)SataConfig->Rst.SmartStorage;
  FspsUpd->FspsConfig.SataRstOptaneMemory            = (UINT8)SataConfig->Rst.OptaneMemory;
  FspsUpd->FspsConfig.SataRstLegacyOrom              = (UINT8)SataConfig->Rst.LegacyOrom;
  FspsUpd->FspsConfig.SataRstCpuAttachedStorage      = (UINT8)SataConfig->Rst.CpuAttachedStorage;

  for (Index = 0; Index < PCH_MAX_RST_PCIE_STORAGE_CR; Index++) {
    FspsUpd->FspsConfig.SataRstPcieEnable[Index]           = (UINT8)SataConfig->RstPcieStorageRemap[Index].Enable;
    FspsUpd->FspsConfig.SataRstPcieStoragePort[Index]      = (UINT8)SataConfig->RstPcieStorageRemap[Index].RstPcieStoragePort;
    FspsUpd->FspsConfig.SataRstPcieDeviceResetDelay[Index] = (UINT8)SataConfig->RstPcieStorageRemap[Index].DeviceResetDelay;
  }

  FspsUpd->FspsConfig.SataP0T1M            = (UINT8)SataConfig->ThermalThrottling.P0T1M;
  FspsUpd->FspsConfig.SataP0T2M            = (UINT8)SataConfig->ThermalThrottling.P0T2M;
  FspsUpd->FspsConfig.SataP0T3M            = (UINT8)SataConfig->ThermalThrottling.P0T3M;
  FspsUpd->FspsConfig.SataP0TDisp          = (UINT8)SataConfig->ThermalThrottling.P0TDisp;
  FspsUpd->FspsConfig.SataP1T1M            = (UINT8)SataConfig->ThermalThrottling.P1T1M;
  FspsUpd->FspsConfig.SataP1T2M            = (UINT8)SataConfig->ThermalThrottling.P1T2M;
  FspsUpd->FspsConfig.SataP1T3M            = (UINT8)SataConfig->ThermalThrottling.P1T3M;
  FspsUpd->FspsConfig.SataP1TDisp          = (UINT8)SataConfig->ThermalThrottling.P1TDisp;
  FspsUpd->FspsConfig.SataP0Tinact         = (UINT8)SataConfig->ThermalThrottling.P0Tinact;
  FspsUpd->FspsConfig.SataP0TDispFinit     = (UINT8)SataConfig->ThermalThrottling.P0TDispFinit;
  FspsUpd->FspsConfig.SataP1Tinact         = (UINT8)SataConfig->ThermalThrottling.P1Tinact;
  FspsUpd->FspsConfig.SataP1TDispFinit     = (UINT8)SataConfig->ThermalThrottling.P1TDispFinit;
  FspsUpd->FspsConfig.SataThermalSuggestedSetting = (UINT8)SataConfig->ThermalThrottling.SuggestedSetting;

  //
  // Update USB policies
  //
  FspsUpd->FspsConfig.PchEnableComplianceMode           = (UINT8)UsbConfig->EnableComplianceMode;
  FspsUpd->FspsConfig.UsbPdoProgramming                 = (UINT8)UsbConfig->PdoProgramming;
  FspsUpd->FspsConfig.PchUsbOverCurrentEnable           = (UINT8)UsbConfig->OverCurrentEnable;
  FspsUpd->FspsConfig.PchUsb2PhySusPgEnable             = (UINT8)UsbConfig->Usb2PhySusPgEnable;
  FspsUpd->FspsTestConfig.PchXhciOcLock                 = (UINT8)UsbConfig->XhciOcLock;
  for (Index = 0; Index < PCH_MAX_USB2_PORTS; Index++) {
    FspsUpd->FspsConfig.PortUsb20Enable[Index]  = (UINT8)UsbConfig->PortUsb20[Index].Enable;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[Index] = (UINT8)UsbConfig->PortUsb20[Index].OverCurrentPin;
    FspsUpd->FspsConfig.Usb2AfePetxiset[Index]  = (UINT8)UsbConfig->PortUsb20[Index].Afe.Petxiset;
    FspsUpd->FspsConfig.Usb2AfeTxiset[Index]    = (UINT8)UsbConfig->PortUsb20[Index].Afe.Txiset;
    FspsUpd->FspsConfig.Usb2AfePredeemp[Index]  = (UINT8)UsbConfig->PortUsb20[Index].Afe.Predeemp;
    FspsUpd->FspsConfig.Usb2AfePehalfbit[Index] = (UINT8)UsbConfig->PortUsb20[Index].Afe.Pehalfbit;
  }
  for (Index = 0; Index < PCH_MAX_USB3_PORTS; Index++) {
    FspsUpd->FspsConfig.PortUsb30Enable[Index]              = (UINT8)UsbConfig->PortUsb30[Index].Enable;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[Index]           = (UINT8)UsbConfig->PortUsb30[Index].OverCurrentPin;
    FspsUpd->FspsConfig.Usb3HsioTxDeEmphEnable[Index]       = (UINT8)UsbConfig->PortUsb30[Index].HsioTxDeEmphEnable;
    FspsUpd->FspsConfig.Usb3HsioTxDeEmph[Index]             = (UINT8)UsbConfig->PortUsb30[Index].HsioTxDeEmph;
    FspsUpd->FspsConfig.Usb3HsioTxDownscaleAmpEnable[Index] = (UINT8)UsbConfig->PortUsb30[Index].HsioTxDownscaleAmpEnable;
    FspsUpd->FspsConfig.Usb3HsioTxDownscaleAmp[Index]       = (UINT8)UsbConfig->PortUsb30[Index].HsioTxDownscaleAmp;

    Data8 = 0;
    Data8 |= UsbConfig->PortUsb30HsioRx[Index].HsioCtrlAdaptOffsetCfgEnable ? B_XHCI_HSIO_CTRL_ADAPT_OFFSET_CFG_EN : 0;
    Data8 |= UsbConfig->PortUsb30HsioRx[Index].HsioFilterSelNEnable ? B_XHCI_HSIO_FILTER_SELECT_N_EN : 0;
    Data8 |= UsbConfig->PortUsb30HsioRx[Index].HsioFilterSelPEnable ? B_XHCI_HSIO_FILTER_SELECT_P_EN : 0;
    Data8 |= UsbConfig->PortUsb30HsioRx[Index].HsioOlfpsCfgPullUpDwnResEnable ? B_XHCI_HSIO_LFPS_CFG_PULLUP_DWN_RES_EN : 0;
    FspsUpd->FspsConfig.PchUsbHsioRxTuningEnable[Index] = Data8;

    Data8 = ((UsbConfig->PortUsb30HsioRx[Index].HsioCtrlAdaptOffsetCfg & 0x1F) << N_XHCI_UPD_HSIO_CTRL_ADAPT_OFFSET_CFG) |
            ((UsbConfig->PortUsb30HsioRx[Index].HsioOlfpsCfgPullUpDwnRes & 0x7) << N_XHCI_UPD_HSIO_LFPS_CFG_PULLUP_DWN_RES);
    FspsUpd->FspsConfig.PchUsbHsioRxTuningParameters[Index] = Data8;

    Data8 = ((UsbConfig->PortUsb30HsioRx[Index].HsioFilterSelN & 0x7) << N_XHCI_UPD_HSIO_FILTER_SELECT_N) |
            ((UsbConfig->PortUsb30HsioRx[Index].HsioFilterSelP & 0x7) << N_XHCI_UPD_HSIO_FILTER_SELECT_P);
    FspsUpd->FspsConfig.PchUsbHsioFilterSel[Index] = Data8;
  }

  FspsUpd->FspsConfig.XdciEnable     = (UINT8)UsbConfig->XdciConfig.Enable;

  //
  // Update SerialIo policies
  //
  for (Index = 0; Index < GetPchMaxSerialIoSpiControllersNum (); Index++) {
    FspsUpd->FspsConfig.SerialIoSpiMode[Index] = SerialIoConfig->DevMode[Index];
  }

  //
  // SPI CS Polarity
  //
  FspsUpd->FspsConfig.SerialIoSpi0CsPolarity[0] = 1;
  FspsUpd->FspsConfig.SerialIoSpi0CsPolarity[1] = 0;
  FspsUpd->FspsConfig.SerialIoSpi1CsPolarity[0] = 0;
  FspsUpd->FspsConfig.SerialIoSpi1CsPolarity[1] = 0;
  FspsUpd->FspsConfig.SerialIoSpi2CsPolarity[0] = 1;
  FspsUpd->FspsConfig.SerialIoSpi2CsPolarity[1] = 0;
  for (Index = 0; Index < GetPchMaxSerialIoUartControllersNum (); Index++) {
    FspsUpd->FspsConfig.SerialIoUartAutoFlow[Index] = SerialIoConfig->UartHwFlowCtrl[Index];
  }
  for (Index = 0; Index < GetPchMaxSerialIoI2cControllersNum (); Index++) {
    FspsUpd->FspsConfig.SerialIoI2cMode[Index]               = SerialIoConfig->DevMode[Index];
    FspsUpd->FspsConfig.PchSerialIoI2cPadsTermination[Index] = SerialIoConfig->I2cPadsTermination[Index];
  }

  FspsUpd->FspsConfig.SerialIoDebugUartNumber          = (UINT8)SerialIoConfig->DebugUartNumber;
  FspsUpd->FspsConfig.SerialIoUartPinMux[0]            = (UINT8)SerialIoConfig->Uart0PinMuxing;

  //
  // Update Interrupt policies
  //
  FspsUpd->FspsConfig.DevIntConfigPtr = (UINT32)InterruptConfig->DevIntConfig;
  FspsUpd->FspsConfig.NumOfDevIntConfig = InterruptConfig->NumOfDevIntConfig;
  for (Index = 0; Index < PCH_MAX_PXRC_CONFIG; Index ++) {
    FspsUpd->FspsConfig.PxRcConfig[Index] = (UINT8)InterruptConfig->PxRcConfig[Index];
  }
  FspsUpd->FspsConfig.GpioIrqRoute = (UINT8)InterruptConfig->GpioIrqRoute;
  FspsUpd->FspsConfig.SciIrqSelect = (UINT8)InterruptConfig->SciIrqSelect;
  FspsUpd->FspsConfig.TcoIrqSelect = (UINT8)InterruptConfig->TcoIrqSelect;
  FspsUpd->FspsConfig.TcoIrqEnable = (UINT8)InterruptConfig->TcoIrqEnable;

  //
  // Update LockDown policies
  //
  FspsUpd->FspsTestConfig.PchLockDownGlobalSmi     = (UINT8)LockDownConfig->GlobalSmi;
  FspsUpd->FspsTestConfig.PchLockDownBiosInterface = (UINT8)LockDownConfig->BiosInterface;
  FspsUpd->FspsConfig.PchLockDownBiosLock          = (UINT8)LockDownConfig->BiosLock;
  FspsUpd->FspsConfig.PchLockDownRtcMemoryLock     = (UINT8)LockDownConfig->RtcMemoryLock;
  FspsUpd->FspsTestConfig.PchUnlockGpioPads        = (UINT8)LockDownConfig->UnlockGpioPads;

  //
  // Update Dmi policies
  //
  FspsUpd->FspsConfig.PchPwrOptEnable = (UINT8)DmiConfig->PwrOptEnable;
  FspsUpd->FspsConfig.PchDmiAspmCtrl = (UINT8)DmiConfig->DmiAspmCtrl;

  //
  // Update Flash Protection policies
  //
  for (Index = 0; Index < PCH_FLASH_PROTECTED_RANGES; Index ++) {
    FspsUpd->FspsConfig.PchWriteProtectionEnable[Index] = (UINT8)FlashProtectionConfig->ProtectRange[Index].WriteProtectionEnable;
    FspsUpd->FspsConfig.PchReadProtectionEnable[Index]  = (UINT8)FlashProtectionConfig->ProtectRange[Index].ReadProtectionEnable;
    FspsUpd->FspsConfig.PchProtectedRangeLimit[Index]  = (UINT16)FlashProtectionConfig->ProtectRange[Index].ProtectedRangeLimit;
    FspsUpd->FspsConfig.PchProtectedRangeBase[Index]   = (UINT16)FlashProtectionConfig->ProtectRange[Index].ProtectedRangeBase;
  }

  //
  // Update IO Apic policies
  //
  FspsUpd->FspsConfig.PchIoApicEntry24_119       = (UINT8)IoApicConfig->IoApicEntry24_119;
  FspsUpd->FspsConfig.Enable8254ClockGating      = (UINT8)IoApicConfig->Enable8254ClockGating;
  FspsUpd->FspsConfig.Enable8254ClockGatingOnS3  = (UINT8)IoApicConfig->Enable8254ClockGatingOnS3;
  FspsUpd->FspsConfig.PchIoApicId                = (UINT8)IoApicConfig->IoApicId;

  //
  // Update P2sb policies
  //
  FspsUpd->FspsTestConfig.PchSbAccessUnlock  = (UINT8)P2sbConfig->SbAccessUnlock;

  //
  // Update Pch General policies
  //
  FspsUpd->FspsConfig.PchCrid               = (UINT8)PchGeneralConfig->Crid;
  FspsUpd->FspsConfig.PchLegacyIoLowLatency = (UINT8)PchGeneralConfig->LegacyIoLowLatency;

  //
  // Update Pm policies
  //
  FspsUpd->FspsConfig.PchPmPmeB0S5Dis         = (UINT8)PmConfig->WakeConfig.PmeB0S5Dis;
  FspsUpd->FspsConfig.PchPmWolEnableOverride  = (UINT8)PmConfig->WakeConfig.WolEnableOverride;
  FspsUpd->FspsConfig.PchPmPcieWakeFromDeepSx = (UINT8)PmConfig->WakeConfig.PcieWakeFromDeepSx;
  FspsUpd->FspsConfig.PchPmWoWlanEnable       = (UINT8)PmConfig->WakeConfig.WoWlanEnable;
  FspsUpd->FspsConfig.PchPmWoWlanDeepSxEnable = (UINT8)PmConfig->WakeConfig.WoWlanDeepSxEnable;
  FspsUpd->FspsConfig.PchPmLanWakeFromDeepSx  = (UINT8)PmConfig->WakeConfig.LanWakeFromDeepSx;

  FspsUpd->FspsConfig.PchPmDeepSxPol          = (UINT8)PmConfig->PchDeepSxPol;
  FspsUpd->FspsConfig.PchPmSlpS3MinAssert     = (UINT8)PmConfig->PchSlpS3MinAssert;
  FspsUpd->FspsConfig.PchPmSlpS4MinAssert     = (UINT8)PmConfig->PchSlpS4MinAssert;
  FspsUpd->FspsConfig.PchPmSlpSusMinAssert    = (UINT8)PmConfig->PchSlpSusMinAssert;
  FspsUpd->FspsConfig.PchPmSlpAMinAssert      = (UINT8)PmConfig->PchSlpAMinAssert;
  FspsUpd->FspsConfig.PchPmLpcClockRun        = (UINT8)PmConfig->LpcClockRun;
  FspsUpd->FspsConfig.PchPmSlpStrchSusUp      = (UINT8)PmConfig->SlpStrchSusUp;
  FspsUpd->FspsConfig.PchPmSlpLanLowDc        = (UINT8)PmConfig->SlpLanLowDc;
  FspsUpd->FspsConfig.PchPmPwrBtnOverridePeriod = (UINT8)PmConfig->PwrBtnOverridePeriod;
  FspsUpd->FspsTestConfig.PchPmDisableEnergyReport  = (UINT8)PmConfig->DisableEnergyReport;
  FspsUpd->FspsConfig.PchPmDisableDsxAcPresentPulldown = (UINT8)PmConfig->DisableDsxAcPresentPulldown;
  FspsUpd->FspsConfig.PchPmDisableNativePowerButton    = (UINT8)PmConfig->DisableNativePowerButton;
  FspsUpd->FspsConfig.PmcPowerButtonDebounce  = PmConfig->PowerButtonDebounce;
  FspsUpd->FspsConfig.PchPmSlpS0Enable        = (UINT8)PmConfig->SlpS0Enable;
  FspsUpd->FspsConfig.PchPmMeWakeSts          = (UINT8)PmConfig->MeWakeSts;
  FspsUpd->FspsConfig.PchPmWolOvrWkSts        = (UINT8)PmConfig->WolOvrWkSts;
  FspsUpd->FspsConfig.EnableTcoTimer          = (UINT8)PmConfig->EnableTcoTimer;
  FspsUpd->FspsConfig.PchPmVrAlert            = (UINT8)PmConfig->VrAlert;
  FspsUpd->FspsConfig.PchPmPwrCycDur          = (UINT8)PmConfig->PchPwrCycDur;
  FspsUpd->FspsConfig.PchPmPciePllSsc         = (UINT8)PmConfig->PciePllSsc;
  FspsUpd->FspsConfig.PchPmSlpS0VmRuntimeControl = (UINT8)PmConfig->SlpS0VmRuntimeControl;
  FspsUpd->FspsConfig.PchPmSlpS0Vm070VSupport   = (UINT8)PmConfig->SlpS0Vm070VSupport;
  FspsUpd->FspsConfig.PchPmSlpS0Vm075VSupport   = (UINT8)PmConfig->SlpS0Vm075VSupport;
  FspsUpd->FspsConfig.SlpS0Override             = (UINT8)PmConfig->SlpS0Override;
  FspsUpd->FspsConfig.SlpS0DisQForDebug         = (UINT8)PmConfig->SlpS0DisQForDebug;
  FspsUpd->FspsConfig.PmcDbgMsgEn               = (UINT8)PmConfig->PmcDbgMsgEn;
  FspsUpd->FspsConfig.PsOnEnable                = (UINT8)PmConfig->PsOnEnable;
  FspsUpd->FspsConfig.PmcCpuC10GatePinEnable    = (UINT8)PmConfig->CpuC10GatePinEnable;
  FspsUpd->FspsConfig.PmcModPhySusPgEnable      = (UINT8)PmConfig->ModPhySusPgEnable;
  FspsUpd->FspsConfig.SlpS0WithGbeSupport       = (UINT8)PmConfig->SlpS0WithGbeSupport;
  //
  // Update Pch Serial IRQ policies
  //
  FspsUpd->FspsConfig.PchSirqEnable       = (UINT8)PchSerialIrqConfig->SirqEnable;
  FspsUpd->FspsConfig.PchSirqMode         = (UINT8)PchSerialIrqConfig->SirqMode;
  FspsUpd->FspsConfig.PchStartFramePulse  = (UINT8)PchSerialIrqConfig->StartFramePulse;
  //
  // Update Pch Thermal policies
  //
  FspsUpd->FspsConfig.PchTsmicLock        = (UINT8)PchThermalConfig->TsmicLock;
  FspsUpd->FspsConfig.PchHotEnable        = (UINT8)PchThermalConfig->PchHotEnable;

  FspsUpd->FspsConfig.PchT0Level          = (UINT16)PchThermalConfig->TTLevels.T0Level;
  FspsUpd->FspsConfig.PchT1Level          = (UINT16)PchThermalConfig->TTLevels.T1Level;
  FspsUpd->FspsConfig.PchT2Level          = (UINT16)PchThermalConfig->TTLevels.T2Level;
  FspsUpd->FspsConfig.PchTTEnable         = (UINT8)PchThermalConfig->TTLevels.TTEnable;
  FspsUpd->FspsConfig.PchTTState13Enable  = (UINT8)PchThermalConfig->TTLevels.TTState13Enable;
  FspsUpd->FspsConfig.PchTTLock           = (UINT8)PchThermalConfig->TTLevels.TTLock;
  FspsUpd->FspsConfig.TTSuggestedSetting  = (UINT8)PchThermalConfig->TTLevels.SuggestedSetting;
  FspsUpd->FspsConfig.TTCrossThrottling   = (UINT8)PchThermalConfig->TTLevels.PchCrossThrottling;

  FspsUpd->FspsConfig.PchDmiTsawEn        = (UINT8)PchThermalConfig->DmiHaAWC.DmiTsawEn;
  FspsUpd->FspsConfig.DmiSuggestedSetting = (UINT8)PchThermalConfig->DmiHaAWC.SuggestedSetting;
  FspsUpd->FspsConfig.DmiTS0TW            = (UINT8)PchThermalConfig->DmiHaAWC.TS0TW;
  FspsUpd->FspsConfig.DmiTS1TW            = (UINT8)PchThermalConfig->DmiHaAWC.TS1TW;
  FspsUpd->FspsConfig.DmiTS2TW            = (UINT8)PchThermalConfig->DmiHaAWC.TS2TW;
  FspsUpd->FspsConfig.DmiTS3TW            = (UINT8)PchThermalConfig->DmiHaAWC.TS3TW;

  FspsUpd->FspsConfig.PchMemoryThrottlingEnable    = (UINT8)PchThermalConfig->MemoryThrottling.Enable;
  FspsUpd->FspsConfig.PchMemoryPmsyncEnable[0]     = (UINT8)PchThermalConfig->MemoryThrottling.TsGpioPinSetting[0].PmsyncEnable;
  FspsUpd->FspsConfig.PchMemoryPmsyncEnable[1]     = (UINT8)PchThermalConfig->MemoryThrottling.TsGpioPinSetting[1].PmsyncEnable;
  FspsUpd->FspsConfig.PchMemoryC0TransmitEnable[0] = (UINT8)PchThermalConfig->MemoryThrottling.TsGpioPinSetting[0].C0TransmitEnable;
  FspsUpd->FspsConfig.PchMemoryC0TransmitEnable[1] = (UINT8)PchThermalConfig->MemoryThrottling.TsGpioPinSetting[1].C0TransmitEnable;
  FspsUpd->FspsConfig.PchMemoryPinSelection[0]     = (UINT8)PchThermalConfig->MemoryThrottling.TsGpioPinSetting[0].PinSelection;
  FspsUpd->FspsConfig.PchMemoryPinSelection[1]     = (UINT8)PchThermalConfig->MemoryThrottling.TsGpioPinSetting[1].PinSelection;

  FspsUpd->FspsConfig.PchTemperatureHotLevel = (UINT16)PchThermalConfig->PchHotLevel;

  //
  // Update Pch CNVi policies
  //
  FspsUpd->FspsConfig.CnviMode = (UINT8)CnviConfig->Mode;

  //
  // Update Pch HSIO policies
  //
  FspsUpd->FspsConfig.ChipsetInitBinPtr = HsioConfig->ChipsetInitBinPtr;
  FspsUpd->FspsConfig.ChipsetInitBinLen = HsioConfig->ChipsetInitBinLen;

  //
  // Update Pch Espi policies
  //
  FspsUpd->FspsConfig.PchEspiLgmrEnable = (UINT8)EspiConfig->LgmrEnable;

  return EFI_SUCCESS;
}

