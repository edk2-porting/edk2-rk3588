/** @file
  Print whole PCH_POLICY_PPI

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiPchPolicyLibrary.h"
#include <Private/PchHsio.h>
#include <Library/ConfigBlockLib.h>

/**
  Print USB_CONFIG and serial out.

  @param[in] UsbConfig         Pointer to a USB_CONFIG that provides the platform setting

**/
VOID
PchPrintUsbConfig (
  IN CONST USB_CONFIG     *UsbConfig
  )
{
  UINT32 Index;

  DEBUG ((DEBUG_INFO, "------------------ PCH USB Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " EnableComplianceMode           = %x\n", UsbConfig->EnableComplianceMode));
  DEBUG ((DEBUG_INFO, " PdoProgramming                 = %x\n", UsbConfig->PdoProgramming));
  DEBUG ((DEBUG_INFO, " OverCurrentEnable              = %x\n", UsbConfig->OverCurrentEnable));
  DEBUG ((DEBUG_INFO, " XhciOcLock                     = %x\n", UsbConfig->XhciOcLock));
  DEBUG ((DEBUG_INFO, " Usb2PhySusPgEnable             = %x\n", UsbConfig->Usb2PhySusPgEnable));

  for (Index = 0; Index < GetPchUsb2MaxPhysicalPortNum (); Index++) {
    DEBUG ((DEBUG_INFO, " PortUsb20[%d].Enabled        = %x\n", Index, UsbConfig->PortUsb20[Index].Enable));
    DEBUG ((DEBUG_INFO, " PortUsb20[%d].OverCurrentPin = OC%x\n", Index, UsbConfig->PortUsb20[Index].OverCurrentPin));
    DEBUG ((DEBUG_INFO, " PortUsb20[%d].Afe.Petxiset   = %x\n", Index, UsbConfig->PortUsb20[Index].Afe.Petxiset));
    DEBUG ((DEBUG_INFO, " PortUsb20[%d].Afe.Txiset     = %x\n", Index, UsbConfig->PortUsb20[Index].Afe.Txiset));
    DEBUG ((DEBUG_INFO, " PortUsb20[%d].Afe.Predeemp   = %x\n", Index, UsbConfig->PortUsb20[Index].Afe.Predeemp));
    DEBUG ((DEBUG_INFO, " PortUsb20[%d].Afe.Pehalfbit  = %x\n", Index, UsbConfig->PortUsb20[Index].Afe.Pehalfbit));
  }

  for (Index = 0; Index < GetPchXhciMaxUsb3PortNum (); Index++) {
    DEBUG ((DEBUG_INFO, " PortUsb30[%d] Enabled                  = %x\n", Index, UsbConfig->PortUsb30[Index].Enable));
    DEBUG ((DEBUG_INFO, " PortUsb30[%d].OverCurrentPin           = OC%x\n", Index, UsbConfig->PortUsb30[Index].OverCurrentPin));
    DEBUG ((DEBUG_INFO, " PortUsb30[%d].HsioTxDeEmphEnable       = %x\n", Index, UsbConfig->PortUsb30[Index].HsioTxDeEmphEnable));
    DEBUG ((DEBUG_INFO, " PortUsb30[%d].HsioTxDeEmph             = %x\n", Index, UsbConfig->PortUsb30[Index].HsioTxDeEmph));
    DEBUG ((DEBUG_INFO, " PortUsb30[%d].HsioTxDownscaleAmpEnable = %x\n", Index, UsbConfig->PortUsb30[Index].HsioTxDownscaleAmpEnable));
    DEBUG ((DEBUG_INFO, " PortUsb30[%d].HsioTxDownscaleAmp       = %x\n", Index, UsbConfig->PortUsb30[Index].HsioTxDownscaleAmp));

    DEBUG ((DEBUG_INFO, " PortUsb30HsioRx[%d].HsioCtrlAdaptOffsetCfgEnable    = %x\n", Index, UsbConfig->PortUsb30HsioRx[Index].HsioCtrlAdaptOffsetCfgEnable));
    DEBUG ((DEBUG_INFO, " PortUsb30HsioRx[%d].HsioCtrlAdaptOffsetCfg          = %x\n", Index, UsbConfig->PortUsb30HsioRx[Index].HsioCtrlAdaptOffsetCfg));
    DEBUG ((DEBUG_INFO, " PortUsb30HsioRx[%d].HsioFilterSelNEnable            = %x\n", Index, UsbConfig->PortUsb30HsioRx[Index].HsioFilterSelNEnable));
    DEBUG ((DEBUG_INFO, " PortUsb30HsioRx[%d].HsioFilterSelN                  = %x\n", Index, UsbConfig->PortUsb30HsioRx[Index].HsioFilterSelN));
    DEBUG ((DEBUG_INFO, " PortUsb30HsioRx[%d].HsioFilterSelPEnable            = %x\n", Index, UsbConfig->PortUsb30HsioRx[Index].HsioFilterSelPEnable));
    DEBUG ((DEBUG_INFO, " PortUsb30HsioRx[%d].HsioFilterSelP                  = %x\n", Index, UsbConfig->PortUsb30HsioRx[Index].HsioFilterSelP));
    DEBUG ((DEBUG_INFO, " PortUsb30HsioRx[%d].HsioOlfpsCfgPullUpDwnResEnable  = %x\n", Index, UsbConfig->PortUsb30HsioRx[Index].HsioOlfpsCfgPullUpDwnResEnable));
    DEBUG ((DEBUG_INFO, " PortUsb30HsioRx[%d].HsioOlfpsCfgPullUpDwnRes        = %x\n", Index, UsbConfig->PortUsb30HsioRx[Index].HsioOlfpsCfgPullUpDwnRes));
  }

  DEBUG ((DEBUG_INFO, " XdciConfig.Enable= %x\n", UsbConfig->XdciConfig.Enable));

}

/**
  Print PCH_PCIE_CONFIG and serial out.

  @param[in] PcieConfig         Pointer to a PCH_PCIE_CONFIG that provides the platform setting

**/
VOID
PchPrintPcieConfig (
  IN CONST PCH_PCIE_CONFIG      *PcieConfig
  )
{
  UINT32 Index;

  DEBUG ((DEBUG_INFO, "------------------ PCH PCIE Config ------------------\n"));
  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    DEBUG ((DEBUG_INFO, " RootPort[%d] HotPlug= %x\n", Index, PcieConfig->RootPort[Index].HotPlug));
    DEBUG ((DEBUG_INFO, " RootPort[%d] PmSci= %x\n", Index, PcieConfig->RootPort[Index].PmSci));
    DEBUG ((DEBUG_INFO, " RootPort[%d] ExtSync= %x\n", Index, PcieConfig->RootPort[Index].ExtSync));
    DEBUG ((DEBUG_INFO, " RootPort[%d] ClkReqDetect= %x\n", Index, PcieConfig->RootPort[Index].ClkReqDetect));
    DEBUG ((DEBUG_INFO, " RootPort[%d] UnsupportedRequestReport= %x\n", Index, PcieConfig->RootPort[Index].UnsupportedRequestReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] FatalErrorReport= %x\n", Index, PcieConfig->RootPort[Index].FatalErrorReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] NoFatalErrorReport= %x\n", Index, PcieConfig->RootPort[Index].NoFatalErrorReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] CorrectableErrorReport= %x\n", Index, PcieConfig->RootPort[Index].CorrectableErrorReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SystemErrorOnFatalError= %x\n", Index, PcieConfig->RootPort[Index].SystemErrorOnFatalError));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SystemErrorOnNonFatalError= %x\n", Index, PcieConfig->RootPort[Index].SystemErrorOnNonFatalError));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SystemErrorOnCorrectableError= %x\n", Index, PcieConfig->RootPort[Index].SystemErrorOnCorrectableError));
    DEBUG ((DEBUG_INFO, " RootPort[%d] MaxPayload= %x\n", Index, PcieConfig->RootPort[Index].MaxPayload));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SlotImplemented= %x\n", Index, PcieConfig->RootPort[Index].SlotImplemented));
    DEBUG ((DEBUG_INFO, " RootPort[%d] AcsEnabled= %x\n", Index, PcieConfig->RootPort[Index].AcsEnabled));
    DEBUG ((DEBUG_INFO, " RootPort[%d] PtmEnabled= %x\n", Index, PcieConfig->RootPort[Index].PtmEnabled));
    DEBUG ((DEBUG_INFO, " RootPort[%d] AdvancedErrorReporting= %x\n", Index, PcieConfig->RootPort[Index].AdvancedErrorReporting));
    DEBUG ((DEBUG_INFO, " RootPort[%d] TransmitterHalfSwing= %x\n", Index, PcieConfig->RootPort[Index].TransmitterHalfSwing));
    DEBUG ((DEBUG_INFO, " RootPort[%d] PcieSpeed= %x\n", Index, PcieConfig->RootPort[Index].PcieSpeed));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen3EqPh3Method= %x\n", Index, PcieConfig->RootPort[Index].Gen3EqPh3Method));
    DEBUG ((DEBUG_INFO, " RootPort[%d] PhysicalSlotNumber= %x\n", Index, PcieConfig->RootPort[Index].PhysicalSlotNumber));
    DEBUG ((DEBUG_INFO, " RootPort[%d] CompletionTimeout= %x\n", Index, PcieConfig->RootPort[Index].CompletionTimeout));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Aspm= %x\n", Index, PcieConfig->RootPort[Index].Aspm));
    DEBUG ((DEBUG_INFO, " RootPort[%d] L1Substates= %x\n", Index, PcieConfig->RootPort[Index].L1Substates));
    DEBUG ((DEBUG_INFO, " RootPort[%d] LtrEnable= %x\n", Index, PcieConfig->RootPort[Index].LtrEnable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] LtrConfigLock= %x\n", Index, PcieConfig->RootPort[Index].LtrConfigLock));
    DEBUG ((DEBUG_INFO, " RootPort[%d] LtrMaxSnoopLatency= %x\n", Index, PcieConfig->RootPort[Index].LtrMaxSnoopLatency));
    DEBUG ((DEBUG_INFO, " RootPort[%d] LtrMaxNoSnoopLatency= %x\n", Index, PcieConfig->RootPort[Index].LtrMaxNoSnoopLatency));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SnoopLatencyOverrideMode= %x\n", Index, PcieConfig->RootPort[Index].SnoopLatencyOverrideMode));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SnoopLatencyOverrideMultiplier= %x\n", Index, PcieConfig->RootPort[Index].SnoopLatencyOverrideMultiplier));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SnoopLatencyOverrideValue= %x\n", Index, PcieConfig->RootPort[Index].SnoopLatencyOverrideValue));
    DEBUG ((DEBUG_INFO, " RootPort[%d] NonSnoopLatencyOverrideMode= %x\n", Index, PcieConfig->RootPort[Index].NonSnoopLatencyOverrideMode));
    DEBUG ((DEBUG_INFO, " RootPort[%d] NonSnoopLatencyOverrideMultiplier= %x\n", Index, PcieConfig->RootPort[Index].NonSnoopLatencyOverrideMultiplier));
    DEBUG ((DEBUG_INFO, " RootPort[%d] NonSnoopLatencyOverrideValue= %x\n", Index, PcieConfig->RootPort[Index].NonSnoopLatencyOverrideValue));
    DEBUG ((DEBUG_INFO, " RootPort[%d] ForceLtrOverride= %x\n", Index, PcieConfig->RootPort[Index].ForceLtrOverride));
    DEBUG ((DEBUG_INFO, " RootPort[%d] DetectTimeoutMs= %x\n", Index, PcieConfig->RootPort[Index].DetectTimeoutMs));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SlotPowerLimitScale= %x\n", Index, PcieConfig->RootPort[Index].SlotPowerLimitScale));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SlotPowerLimitValue= %x\n", Index, PcieConfig->RootPort[Index].SlotPowerLimitValue));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Uptp= %x\n", Index, PcieConfig->RootPort[Index].Uptp));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Dptp= %x\n", Index, PcieConfig->RootPort[Index].Dptp));
    DEBUG ((DEBUG_INFO, " RootPort[%d] EnableCpm= %x\n", Index, PcieConfig->RootPort[Index].EnableCpm));

  }
  for (Index = 0; Index < GetPchMaxPcieClockNum (); Index++) {
    DEBUG ((DEBUG_INFO, " Clock[%d] Usage= %x\n", Index, PcieConfig->PcieClock[Index].Usage));
    DEBUG ((DEBUG_INFO, " Clock[%d] ClkReq= %x\n", Index, PcieConfig->PcieClock[Index].ClkReq));
  }
  for (Index = 0; Index < PCH_PCIE_SWEQ_COEFFS_MAX; Index++) {
    DEBUG ((DEBUG_INFO, " SwEqCoeffCm[%d] = %x\n", Index, PcieConfig->SwEqCoeffList[Index].Cm));
    DEBUG ((DEBUG_INFO, " SwEqCoeffCp[%d] = %x\n", Index, PcieConfig->SwEqCoeffList[Index].Cp));
  }
  DEBUG ((DEBUG_INFO, " EnablePort8xhDecode= %x\n", PcieConfig->EnablePort8xhDecode));
  DEBUG ((DEBUG_INFO, " PchPciePort8xhDecodePortIndex= %x\n", PcieConfig->PchPciePort8xhDecodePortIndex));
  DEBUG ((DEBUG_INFO, " DisableRootPortClockGating= %x\n", PcieConfig->DisableRootPortClockGating));
  DEBUG ((DEBUG_INFO, " EnablePeerMemoryWrite= %x\n", PcieConfig->EnablePeerMemoryWrite));
  DEBUG ((DEBUG_INFO, " ComplianceTestMode= %x\n", PcieConfig->ComplianceTestMode));
  DEBUG ((DEBUG_INFO, " RpFunctionSwap= %x\n", PcieConfig->RpFunctionSwap));
  DEBUG ((DEBUG_INFO, " PcieDeviceOverrideTablePtr= %x\n", PcieConfig->PcieDeviceOverrideTablePtr));
}

/**
  Print PCH_SATA_CONFIG and serial out.

  @param[in]  SataCtrlIndex     SATA controller index
  @param[in]  SataConfig        Pointer to a PCH_SATA_CONFIG that provides the platform setting

**/
VOID
PchPrintSataConfig (
  IN UINT32                     SataCtrlIndex,
  IN CONST PCH_SATA_CONFIG      *SataConfig
  )
{
  UINT32 Index;

  DEBUG ((DEBUG_INFO, "--------------- PCH SATA Config for controller %d -----------\n", SataCtrlIndex));
  DEBUG ((DEBUG_INFO, " Enable= %x\n", SataConfig->Enable));
  DEBUG ((DEBUG_INFO, " SataMode= %x\n", SataConfig->SataMode));

  for (Index = 0; Index < GetPchMaxSataPortNum (SataCtrlIndex); Index++) {
    DEBUG ((DEBUG_INFO, " PortSettings[%d] Enabled= %x\n", Index, SataConfig->PortSettings[Index].Enable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HotPlug= %x\n", Index, SataConfig->PortSettings[Index].HotPlug));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] InterlockSw= %x\n", Index, SataConfig->PortSettings[Index].InterlockSw));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] External= %x\n", Index, SataConfig->PortSettings[Index].External));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] SpinUp= %x\n", Index, SataConfig->PortSettings[Index].SpinUp));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] SolidStateDrive= %x\n", Index, SataConfig->PortSettings[Index].SolidStateDrive));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] DevSlp= %x\n", Index, SataConfig->PortSettings[Index].DevSlp));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] EnableDitoConfig= %x\n", Index, SataConfig->PortSettings[Index].EnableDitoConfig));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] DmVal= %x\n", Index, SataConfig->PortSettings[Index].DmVal));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] DitoVal= %x\n", Index, SataConfig->PortSettings[Index].DitoVal));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] ZpOdd= %x\n", Index, SataConfig->PortSettings[Index].ZpOdd));
  }

  DEBUG ((DEBUG_INFO, " RaidDeviceId= %x\n", SataConfig->Rst.RaidDeviceId));
  DEBUG ((DEBUG_INFO, " Sata interrupt mode =  %x\n", SataConfig->Rst.SataRstInterrupt));
  DEBUG ((DEBUG_INFO, " Raid0= %x\n", SataConfig->Rst.Raid0));
  DEBUG ((DEBUG_INFO, " Raid1= %x\n", SataConfig->Rst.Raid1));
  DEBUG ((DEBUG_INFO, " Raid10= %x\n", SataConfig->Rst.Raid10));
  DEBUG ((DEBUG_INFO, " Raid5= %x\n", SataConfig->Rst.Raid5));
  DEBUG ((DEBUG_INFO, " Irrt= %x\n", SataConfig->Rst.Irrt));
  DEBUG ((DEBUG_INFO, " OromUiBanner= %x\n", SataConfig->Rst.OromUiBanner));
  DEBUG ((DEBUG_INFO, " OromUiDelay= %x\n", SataConfig->Rst.OromUiDelay));
  DEBUG ((DEBUG_INFO, " HddUnlock= %x\n", SataConfig->Rst.HddUnlock));
  DEBUG ((DEBUG_INFO, " LedLocate= %x\n", SataConfig->Rst.LedLocate));
  DEBUG ((DEBUG_INFO, " IrrtOnly= %x\n", SataConfig->Rst.IrrtOnly));
  DEBUG ((DEBUG_INFO, " SmartStorage= %x\n", SataConfig->Rst.SmartStorage));
  DEBUG ((DEBUG_INFO, " LegacyOrom= %x\n", SataConfig->Rst.LegacyOrom));
  DEBUG ((DEBUG_INFO, " OptaneMemory= %x\n", SataConfig->Rst.OptaneMemory));
  DEBUG ((DEBUG_INFO, " CpuAttachedStorage= %x\n", SataConfig->Rst.CpuAttachedStorage));

  DEBUG ((DEBUG_INFO, " SpeedSupport= %x\n", SataConfig->SpeedLimit));
  DEBUG ((DEBUG_INFO, " EsataSpeedLimit= %x\n", SataConfig->EsataSpeedLimit));
  DEBUG ((DEBUG_INFO, " LedEnable= %x\n", SataConfig->LedEnable));
  DEBUG ((DEBUG_INFO, " TestMode= %x\n", SataConfig->TestMode));
  DEBUG ((DEBUG_INFO, " SalpSupport= %x\n", SataConfig->SalpSupport));
  DEBUG ((DEBUG_INFO, " PwrOptEnable= %x\n", SataConfig->PwrOptEnable));

  for (Index = 0; Index < PCH_MAX_RST_PCIE_STORAGE_CR; Index++) {
    DEBUG ((DEBUG_INFO, " RstPcieStorageRemap[%d].Enable                  = %x\n", Index, SataConfig->RstPcieStorageRemap[Index].Enable));
    DEBUG ((DEBUG_INFO, " RstPcieStorageRemap[%d].RstPcieStoragePort      = %x\n", Index, SataConfig->RstPcieStorageRemap[Index].RstPcieStoragePort));
    DEBUG ((DEBUG_INFO, " RstPcieStorageRemap[%d].DeviceResetDelay        = %x\n", Index, SataConfig->RstPcieStorageRemap[Index].DeviceResetDelay));
  }
  DEBUG ((DEBUG_INFO, " ThermalThrottling P0T1M %x\n", SataConfig->ThermalThrottling.P0T1M));
  DEBUG ((DEBUG_INFO, " ThermalThrottling P0T2M %x\n", SataConfig->ThermalThrottling.P0T2M));
  DEBUG ((DEBUG_INFO, " ThermalThrottling P0T3M %x\n", SataConfig->ThermalThrottling.P0T3M));
  DEBUG ((DEBUG_INFO, " ThermalThrottling P0TDisp %x\n", SataConfig->ThermalThrottling.P0TDisp));
  DEBUG ((DEBUG_INFO, " ThermalThrottling P0Tinact %x\n", SataConfig->ThermalThrottling.P0Tinact));
  DEBUG ((DEBUG_INFO, " ThermalThrottling P0TDispFinit %x\n", SataConfig->ThermalThrottling.P0TDispFinit));
  DEBUG ((DEBUG_INFO, " ThermalThrottling P1T1M %x\n", SataConfig->ThermalThrottling.P1T1M));
  DEBUG ((DEBUG_INFO, " ThermalThrottling P1T2M %x\n", SataConfig->ThermalThrottling.P1T2M));
  DEBUG ((DEBUG_INFO, " ThermalThrottling P1T3M %x\n", SataConfig->ThermalThrottling.P1T3M));
  DEBUG ((DEBUG_INFO, " ThermalThrottling P1TDisp %x\n", SataConfig->ThermalThrottling.P1TDisp));
  DEBUG ((DEBUG_INFO, " ThermalThrottling P1Tinact %x\n", SataConfig->ThermalThrottling.P1Tinact));
  DEBUG ((DEBUG_INFO, " ThermalThrottling P1TDispFinit %x\n", SataConfig->ThermalThrottling.P1TDispFinit));
  DEBUG ((DEBUG_INFO, " ThermalThrottling SuggestedSetting %x\n", SataConfig->ThermalThrottling.SuggestedSetting));
}

/**
  Print PCH_IOAPIC_CONFIG and serial out.

  @param[in] IoApicConfig         Pointer to a PCH_IOAPIC_CONFIG that provides the platform setting

**/
VOID
PchPrintIoApicConfig (
  IN CONST PCH_IOAPIC_CONFIG   *IoApicConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH IOAPIC Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " IoApicEntry24_119= %x\n", IoApicConfig->IoApicEntry24_119));
  DEBUG ((DEBUG_INFO, " Enable8254ClockGating= %x\n", IoApicConfig->Enable8254ClockGating));
  DEBUG ((DEBUG_INFO, " Enable8254ClockGatingOnS3= %x\n", IoApicConfig->Enable8254ClockGatingOnS3));
  DEBUG ((DEBUG_INFO, " IoApicId= %x\n", IoApicConfig->IoApicId));
}

/**
  Print PCH_LOCK_DOWN_CONFIG and serial out.

  @param[in] LockDownConfig         Pointer to a PCH_LOCK_DOWN_CONFIG that provides the platform setting

**/
VOID
PchPrintLockDownConfig (
  IN CONST PCH_LOCK_DOWN_CONFIG   *LockDownConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH Lock Down Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " GlobalSmi= %x\n", LockDownConfig->GlobalSmi));
  DEBUG ((DEBUG_INFO, " BiosInterface= %x\n", LockDownConfig->BiosInterface));
  DEBUG ((DEBUG_INFO, " RtcMemoryLock= %x\n", LockDownConfig->RtcMemoryLock));
  DEBUG ((DEBUG_INFO, " BiosLock= %x\n", LockDownConfig->BiosLock));
  DEBUG ((DEBUG_INFO, " UnlockGpioPads= %x\n", LockDownConfig->UnlockGpioPads));
}

/**
  Print PCH_HDAUDIO_CONFIG and serial out.

  @param[in] HdaConfig         Pointer to a PCH_HDAUDIO_CONFIG that provides the platform setting

**/
VOID
PchPrintHdAudioConfig (
  IN CONST PCH_HDAUDIO_CONFIG   *HdaConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH HD-Audio Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " DSP Enable               = %x\n", HdaConfig->DspEnable));
  DEBUG ((DEBUG_INFO, " DSP UAA Compliance       = %x\n", HdaConfig->DspUaaCompliance));
  DEBUG ((DEBUG_INFO, " iDisp Codec Disconnect   = %x\n", HdaConfig->IDispCodecDisconnect));
  DEBUG ((DEBUG_INFO, " Pme                      = %x\n", HdaConfig->Pme));
  DEBUG ((DEBUG_INFO, " Codec Sx Wake Capability = %x\n", HdaConfig->CodecSxWakeCapability));
  DEBUG ((DEBUG_INFO, " VC Type                  = %x\n", HdaConfig->VcType));
  DEBUG ((DEBUG_INFO, " HD-A Link Frequency      = %x\n", HdaConfig->HdAudioLinkFrequency));
  DEBUG ((DEBUG_INFO, " iDisp Link Frequency     = %x\n", HdaConfig->IDispLinkFrequency));
  DEBUG ((DEBUG_INFO, " iDisp Link T-Mode        = %x\n", HdaConfig->IDispLinkTmode));
  DEBUG ((DEBUG_INFO, " Audio Link: HDA Link     = %x\n", HdaConfig->AudioLinkHda));
  DEBUG ((DEBUG_INFO, " Audio Link: DMIC#0       = %x\n", HdaConfig->AudioLinkDmic0));
  DEBUG ((DEBUG_INFO, " Audio Link: DMIC#1       = %x\n", HdaConfig->AudioLinkDmic1));
  DEBUG ((DEBUG_INFO, " Audio Link: SSP#0        = %x\n", HdaConfig->AudioLinkSsp0));
  DEBUG ((DEBUG_INFO, " Audio Link: SSP#1        = %x\n", HdaConfig->AudioLinkSsp1));
  DEBUG ((DEBUG_INFO, " Audio Link: SSP#2        = %x\n", HdaConfig->AudioLinkSsp1));
  DEBUG ((DEBUG_INFO, " Audio Link: SoundWire#1  = %x\n", HdaConfig->AudioLinkSndw1));
  DEBUG ((DEBUG_INFO, " Audio Link: SoundWire#2  = %x\n", HdaConfig->AudioLinkSndw2));
  DEBUG ((DEBUG_INFO, " Audio Link: SoundWire#3  = %x\n", HdaConfig->AudioLinkSndw3));
  DEBUG ((DEBUG_INFO, " Audio Link: SoundWire#4  = %x\n", HdaConfig->AudioLinkSndw4));
  DEBUG ((DEBUG_INFO, " SoundWire Buffer RCOMP   = %x\n", HdaConfig->SndwBufferRcomp));
  DEBUG ((DEBUG_INFO, " ResetWaitTimer           = %x\n", HdaConfig->ResetWaitTimer));
  DEBUG ((DEBUG_INFO, " VerbTableEntryNum        = %x\n", HdaConfig->VerbTableEntryNum));
  DEBUG ((DEBUG_INFO, " VerbTablePtr             = %x\n", HdaConfig->VerbTablePtr));
}

/**
  Print PCH_PM_CONFIG and serial out.

  @param[in] PmConfig         Pointer to a PCH_PM_CONFIG that provides the platform setting

**/
VOID
PchPrintPmConfig (
  IN CONST PCH_PM_CONFIG   *PmConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH PM Config ------------------\n"));

  DEBUG ((DEBUG_INFO, " WakeConfig PmeB0S5Dis               = %x\n", PmConfig->WakeConfig.PmeB0S5Dis));
  DEBUG ((DEBUG_INFO, " WakeConfig WolEnableOverride        = %x\n", PmConfig->WakeConfig.WolEnableOverride));
  DEBUG ((DEBUG_INFO, " WakeConfig LanWakeFromDeepSx        = %x\n", PmConfig->WakeConfig.LanWakeFromDeepSx));
  DEBUG ((DEBUG_INFO, " WakeConfig PcieWakeFromDeepSx       = %x\n", PmConfig->WakeConfig.PcieWakeFromDeepSx));
  DEBUG ((DEBUG_INFO, " WakeConfig WoWlanEnable             = %x\n", PmConfig->WakeConfig.WoWlanEnable));
  DEBUG ((DEBUG_INFO, " WakeConfig WoWlanDeepSxEnable       = %x\n", PmConfig->WakeConfig.WoWlanDeepSxEnable));

  DEBUG ((DEBUG_INFO, " PchDeepSxPol                        = %x\n", PmConfig->PchDeepSxPol));
  DEBUG ((DEBUG_INFO, " PchSlpS3MinAssert                   = %x\n", PmConfig->PchSlpS3MinAssert));
  DEBUG ((DEBUG_INFO, " PchSlpS4MinAssert                   = %x\n", PmConfig->PchSlpS4MinAssert));
  DEBUG ((DEBUG_INFO, " PchSlpSusMinAssert                  = %x\n", PmConfig->PchSlpSusMinAssert));
  DEBUG ((DEBUG_INFO, " PchSlpAMinAssert                    = %x\n", PmConfig->PchSlpAMinAssert));
  DEBUG ((DEBUG_INFO, " LpcClockRun                         = %x\n", PmConfig->LpcClockRun));
  DEBUG ((DEBUG_INFO, " SlpStrchSusUp                       = %x\n", PmConfig->SlpStrchSusUp));
  DEBUG ((DEBUG_INFO, " SlpLanLowDc                         = %x\n", PmConfig->SlpLanLowDc));
  DEBUG ((DEBUG_INFO, " PwrBtnOverridePeriod                = %x\n", PmConfig->PwrBtnOverridePeriod));
  DEBUG ((DEBUG_INFO, " DisableEnergyReport                 = %x\n", PmConfig->DisableEnergyReport));
  DEBUG ((DEBUG_INFO, " DisableDsxAcPresentPulldown         = %x\n", PmConfig->DisableDsxAcPresentPulldown));
  DEBUG ((DEBUG_INFO, " PchPwrCycDur                        = %x\n", PmConfig->PchPwrCycDur));
  DEBUG ((DEBUG_INFO, " PciePllSsc                          = %x\n", PmConfig->PciePllSsc));
  DEBUG ((DEBUG_INFO, " DisableNativePowerButton            = %x\n", PmConfig->DisableNativePowerButton));
  DEBUG ((DEBUG_INFO, " SlpS0Enabled                        = %x\n", PmConfig->SlpS0Enable));
  DEBUG ((DEBUG_INFO, " MeWakeSts                           = %x\n", PmConfig->MeWakeSts));
  DEBUG ((DEBUG_INFO, " WolOvrWkSts                         = %x\n", PmConfig->WolOvrWkSts));
  DEBUG ((DEBUG_INFO, " EnableTcoTimer                      = %x\n", PmConfig->EnableTcoTimer));
  DEBUG ((DEBUG_INFO, " VrAlert                             = %x\n", PmConfig->VrAlert));
  DEBUG ((DEBUG_INFO, " PowerButtonDebounce                 = %x\n", PmConfig->PowerButtonDebounce));
  DEBUG ((DEBUG_INFO, " SlpS0VmRuntimeControl               = %x\n", PmConfig->SlpS0VmRuntimeControl));
  DEBUG ((DEBUG_INFO, " SlpS0Vm070VSupport                  = %x\n", PmConfig->SlpS0Vm070VSupport));
  DEBUG ((DEBUG_INFO, " SlpS0Vm075VSupport                  = %x\n", PmConfig->SlpS0Vm075VSupport));
  DEBUG ((DEBUG_INFO, " SlpS0Override                       = %x\n", PmConfig->SlpS0Override));
  DEBUG ((DEBUG_INFO, " SlpS0DisQForDebug                   = %x\n", PmConfig->SlpS0DisQForDebug));
  DEBUG ((DEBUG_INFO, " PsOnEnable                          = %x\n", PmConfig->PsOnEnable));
  DEBUG ((DEBUG_INFO, " CpuC10GatePinEnable                 = %x\n", PmConfig->CpuC10GatePinEnable));
  DEBUG ((DEBUG_INFO, " PmcDbgMsgEn                         = %x\n", PmConfig->PmcDbgMsgEn));
  DEBUG ((DEBUG_INFO, " ModPhySusPgEnable                   = %x\n", PmConfig->ModPhySusPgEnable));
  DEBUG ((DEBUG_INFO, " SlpS0WithGbeSupport                 = %x\n", PmConfig->SlpS0WithGbeSupport));
}

/**
  Print PCH_DMI_CONFIG and serial out.

  @param[in] DmiConfig         Pointer to a PCH_DMI_CONFIG that provides the platform setting

**/
VOID
PchPrintDmiConfig (
  IN CONST PCH_DMI_CONFIG   *DmiConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH DMI Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " PwrOptEnable= %x\n", DmiConfig->PwrOptEnable));
  DEBUG ((DEBUG_INFO, " DmiAspmCtrl= %x\n", DmiConfig->DmiAspmCtrl));
}
/**
  Print PCH_LPC_SIRQ_CONFIG and serial out.

  @param[in] SerialIrqConfig         Pointer to a PCH_LPC_SIRQ_CONFIG that provides the platform setting

**/
VOID
PchPrintSerialIrqConfig (
  IN CONST PCH_LPC_SIRQ_CONFIG   *SerialIrqConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH LPC SIRQ Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " SirqEnable= %x\n", SerialIrqConfig->SirqEnable));
  DEBUG ((DEBUG_INFO, " SirqMode= %x\n", SerialIrqConfig->SirqMode));
  DEBUG ((DEBUG_INFO, " StartFramePulse= %x\n", SerialIrqConfig->StartFramePulse));
}
/**
  Print PCH_THERMAL_CONFIG and serial out.

  @param[in] ThermalConfig         Pointer to a PCH_THERMAL_CONFIG that provides the platform setting

**/
VOID
PchPrintThermalConfig (
  IN CONST PCH_THERMAL_CONFIG   *ThermalConfig
  )
{
  UINTN Index;

  DEBUG ((DEBUG_INFO, "------------------ PCH Thermal Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " TsmicLock= %x\n", ThermalConfig->TsmicLock));
  DEBUG ((DEBUG_INFO, " TTLevels T0Level %x centigrade degree\n", ThermalConfig->TTLevels.T0Level));
  DEBUG ((DEBUG_INFO, " TTLevels T1Level %x centigrade degree\n", ThermalConfig->TTLevels.T1Level));
  DEBUG ((DEBUG_INFO, " TTLevels T2Level %x centigrade degree\n", ThermalConfig->TTLevels.T2Level));
  DEBUG ((DEBUG_INFO, " TTLevels TTEnable %x\n", ThermalConfig->TTLevels.TTEnable));
  DEBUG ((DEBUG_INFO, " TTLevels TTState13Enable %x\n", ThermalConfig->TTLevels.TTState13Enable));
  DEBUG ((DEBUG_INFO, " TTLevels TTLock %x\n", ThermalConfig->TTLevels.TTLock));
  DEBUG ((DEBUG_INFO, " TTLevels SuggestedSetting %x\n", ThermalConfig->TTLevels.SuggestedSetting));
  DEBUG ((DEBUG_INFO, " TTLevels PchCrossThrottling %x\n", ThermalConfig->TTLevels.PchCrossThrottling));

  DEBUG ((DEBUG_INFO, " DmiHaAWC DmiTsawEn %x\n", ThermalConfig->DmiHaAWC.DmiTsawEn));
  DEBUG ((DEBUG_INFO, " DmiHaAWC TS0TW %x\n", ThermalConfig->DmiHaAWC.TS0TW));
  DEBUG ((DEBUG_INFO, " DmiHaAWC TS1TW %x\n", ThermalConfig->DmiHaAWC.TS1TW));
  DEBUG ((DEBUG_INFO, " DmiHaAWC TS2TW %x\n", ThermalConfig->DmiHaAWC.TS2TW));
  DEBUG ((DEBUG_INFO, " DmiHaAWC TS3TW %x\n", ThermalConfig->DmiHaAWC.TS3TW));
  DEBUG ((DEBUG_INFO, " DmiHaAWC SuggestedSetting %x\n", ThermalConfig->DmiHaAWC.SuggestedSetting));

  DEBUG ((DEBUG_INFO, " MemoryThrottling Enable= %x\n", ThermalConfig->MemoryThrottling.Enable));
  for (Index = 0; Index < MaxTsGpioPin; Index++) {
    DEBUG ((DEBUG_INFO, " MemoryThrottling TsGpioPinSetting PmsyncEnable= %x\n", ThermalConfig->MemoryThrottling.TsGpioPinSetting[Index].PmsyncEnable));
    DEBUG ((DEBUG_INFO, " MemoryThrottling TsGpioPinSetting C0TransmitEnable= %x\n", ThermalConfig->MemoryThrottling.TsGpioPinSetting[Index].C0TransmitEnable));
    DEBUG ((DEBUG_INFO, " MemoryThrottling TsGpioPinSetting PinSelection= %x\n", ThermalConfig->MemoryThrottling.TsGpioPinSetting[Index].PinSelection));
  }
  DEBUG ((DEBUG_INFO, " PchHotEnable = %x\n", ThermalConfig->PchHotEnable));
  DEBUG ((DEBUG_INFO, " PchHotLevel = %x\n", ThermalConfig->PchHotLevel));
}

/**
  Print PCH_GENERAL_CONFIG and serial out.

  @param[in] PchGeneralConfig   Pointer to a PCH_GENERAL_CONFIG that provides the platform setting

**/
VOID
PchPrintGeneralConfig (
  IN CONST PCH_GENERAL_CONFIG   *PchGeneralConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH General Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " Crid= %x\n", PchGeneralConfig->Crid));
  DEBUG ((DEBUG_INFO, " LegacyIoLowLatency = %x\n", PchGeneralConfig->LegacyIoLowLatency));
}

/**
  Print PCH_LAN_CONFIG and serial out.

  @param[in] LanConfig         Pointer to a PCH_LAN_CONFIG that provides the platform setting

**/
VOID
PchPrintLanConfig (
  IN CONST PCH_LAN_CONFIG   *LanConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH LAN Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " Enable= %x\n", LanConfig->Enable));
  DEBUG ((DEBUG_INFO, " LtrEnable= %x\n", LanConfig->LtrEnable));
}

/**
  Print PCH_SERIAL_IO_CONFIG and serial out.

  @param[in] SerialIoConfig         Pointer to a PCH_SERIAL_IO_CONFIG that provides the platform setting

**/
VOID
PchPrintSerialIoConfig (
  IN CONST PCH_SERIAL_IO_CONFIG   *SerialIoConfig
  )
{
  UINTN Index;
#ifndef MDEPKG_NDEBUG
  static UINT8 DeviceName[PCH_MAX_SERIALIO_CONTROLLERS][5] = {"I2C0","I2C1","I2C2","I2C3","I2C4","I2C5","SPI0","SPI1","SPI2","UA00","UA01","UA02"};
#endif

  DEBUG ((DEBUG_INFO, "------------------ PCH Serial IO Config ------------------\n"));
  DEBUG_CODE_BEGIN ();
  for (Index = 0; Index < GetPchMaxSerialIoControllersNum (); Index++) {
    DEBUG ((DEBUG_INFO, " SerialIoController %a: Mode 0x%x\n", DeviceName[Index], SerialIoConfig->DevMode[Index]));
  }
  DEBUG_CODE_END ();
  for (Index = 0; Index < GetPchMaxSerialIoSpiControllersNum (); Index++) {
    DEBUG ((DEBUG_INFO, " SpiCsPolarity[%d] = 0x%x\n", Index, SerialIoConfig->SpiCsPolarity[Index]));
  }
  for (Index = 0; Index < GetPchMaxSerialIoUartControllersNum (); Index++) {
    DEBUG ((DEBUG_INFO, " UartHwFlowCtrl[%d] = 0x%x\n", Index, SerialIoConfig->UartHwFlowCtrl[Index]));
  }
  for (Index = 0; Index < GetPchMaxSerialIoI2cControllersNum (); Index ++) {
    DEBUG ((DEBUG_INFO, " I2cPadsTermination[%d] = 0x%x\n", Index, SerialIoConfig->I2cPadsTermination[Index]));
  }
  DEBUG ((DEBUG_INFO, " DebugUartNumber = 0x%x\n", SerialIoConfig->DebugUartNumber));
  DEBUG ((DEBUG_INFO, " EnableDebugUartAfterPost = 0x%x\n", SerialIoConfig->EnableDebugUartAfterPost));
  DEBUG ((DEBUG_INFO, " Uart0PinMuxing = 0x%x\n", SerialIoConfig->Uart0PinMuxing));
}

/**
  Print PCH_INTERRUPT_CONFIG and serial out

  @param[in] InterruptConfig        Pointer to Interrupt Configuration structure

**/
VOID
PchPrintInterruptConfig (
  IN CONST PCH_INTERRUPT_CONFIG     *InterruptConfig
  )
{
  UINTN Index;
  //
  // Print interrupt information
  //
  DEBUG ((DEBUG_INFO, "------------------ PCH Interrupt Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " Interrupt assignment:\n"));
  DEBUG ((DEBUG_INFO, "  Dxx:Fx INTx IRQ\n"));
  for (Index = 0; Index < InterruptConfig->NumOfDevIntConfig; Index++) {
    DEBUG ((DEBUG_INFO, "  D%02d:F%d    %d %03d\n",
            InterruptConfig->DevIntConfig[Index].Device,
            InterruptConfig->DevIntConfig[Index].Function,
            InterruptConfig->DevIntConfig[Index].IntX,
            InterruptConfig->DevIntConfig[Index].Irq));
  }
  DEBUG ((DEBUG_INFO, " Legacy PIC interrupt routing:\n"));
  DEBUG ((DEBUG_INFO, "  PIRQx    IRQx\n"));
  for (Index = 0; Index < PCH_MAX_PXRC_CONFIG; Index++) {
    DEBUG ((DEBUG_INFO, "  PIRQ%c -> IRQ%d\n", Index + 65, InterruptConfig->PxRcConfig[Index]));
  }
  DEBUG ((DEBUG_INFO, " Other interrupt configuration:\n"));
  DEBUG ((DEBUG_INFO, "  GpioIrqRoute= %d\n", InterruptConfig->GpioIrqRoute));
  DEBUG ((DEBUG_INFO, "  SciIrqSelect= %d\n", InterruptConfig->SciIrqSelect));
  DEBUG ((DEBUG_INFO, "  TcoIrqEnable= %d\n", InterruptConfig->TcoIrqEnable));
  DEBUG ((DEBUG_INFO, "  TcoIrqSelect= %d\n", InterruptConfig->TcoIrqSelect));
}

/**
  Print PCH_SCS_CONFIG and serial out.

  @param[in] ScsConfig         Pointer to a PCH_SCS_CONFIG that provides the platform setting

**/
VOID
PchPrintScsConfig (
  IN CONST PCH_SCS_CONFIG   *ScsConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH SCS Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " ScsEmmcEnabled = %x\n", ScsConfig->ScsEmmcEnabled));
  DEBUG ((DEBUG_INFO, " ScsSdcardEnabled = %x\n", ScsConfig->ScsSdcardEnabled));
  DEBUG ((DEBUG_INFO, " SdCardPowerEnableActiveHigh = %x\n", ScsConfig->SdCardPowerEnableActiveHigh));
  DEBUG ((DEBUG_INFO, " ScsUfsEnabled = %x\n", ScsConfig->ScsUfsEnabled));
  DEBUG ((DEBUG_INFO, " ScsEmmcHs400Enabled = %x\n", ScsConfig->ScsEmmcHs400Enabled));
  DEBUG ((DEBUG_INFO, " ScsEmmcHs400TuningRequired = %x\n", ScsConfig->ScsEmmcHs400TuningRequired));
  DEBUG ((DEBUG_INFO, " ScsEmmcHs400DllDataValid = %x\n", ScsConfig->ScsEmmcHs400DllDataValid));
  DEBUG ((DEBUG_INFO, " ScsEmmcHs400RxStrobeDll1 = %x\n", ScsConfig->ScsEmmcHs400RxStrobeDll1));
  DEBUG ((DEBUG_INFO, " ScsEmmcHs400TxDataDll = %x\n", ScsConfig->ScsEmmcHs400TxDataDll));
  DEBUG ((DEBUG_INFO, " ScsEmmcHs400DriverStrength = %x\n", ScsConfig->ScsEmmcHs400DriverStrength));
}

/**
  Print PCH_ISH_CONFIG and serial out.

  @param[in] IshConfig         Pointer to a PCH_ISH_CONFIG that provides the platform setting

**/
VOID
PchPrintIshConfig (
  IN CONST PCH_ISH_CONFIG   *IshConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH ISH Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " SPI GPIO Assigned   = %x\n", IshConfig->SpiGpioAssign));
  DEBUG ((DEBUG_INFO, " UART0 GPIO Assigned = %x\n", IshConfig->Uart0GpioAssign));
  DEBUG ((DEBUG_INFO, " UART1 GPIO Assigned = %x\n", IshConfig->Uart1GpioAssign));
  DEBUG ((DEBUG_INFO, " I2C0 GPIO Assigned  = %x\n", IshConfig->I2c0GpioAssign));
  DEBUG ((DEBUG_INFO, " I2C1 GPIO Assigned  = %x\n", IshConfig->I2c1GpioAssign));
  DEBUG ((DEBUG_INFO, " I2C2 GPIO Assigned  = %x\n", IshConfig->I2c2GpioAssign));
  DEBUG ((DEBUG_INFO, " GP_0 GPIO Assigned  = %x\n", IshConfig->Gp0GpioAssign));
  DEBUG ((DEBUG_INFO, " GP_1 GPIO Assigned  = %x\n", IshConfig->Gp1GpioAssign));
  DEBUG ((DEBUG_INFO, " GP_2 GPIO Assigned  = %x\n", IshConfig->Gp2GpioAssign));
  DEBUG ((DEBUG_INFO, " GP_3 GPIO Assigned  = %x\n", IshConfig->Gp3GpioAssign));
  DEBUG ((DEBUG_INFO, " GP_4 GPIO Assigned  = %x\n", IshConfig->Gp4GpioAssign));
  DEBUG ((DEBUG_INFO, " GP_5 GPIO Assigned  = %x\n", IshConfig->Gp5GpioAssign));
  DEBUG ((DEBUG_INFO, " GP_6 GPIO Assigned  = %x\n", IshConfig->Gp6GpioAssign));
  DEBUG ((DEBUG_INFO, " GP_7 GPIO Assigned  = %x\n", IshConfig->Gp7GpioAssign));
}

/**
  Print PCH_FLASH_PROTECTION_CONFIG and serial out.

  @param[in] FlashProtectConfig  Pointer to a PCH_FLASH_PROTECTION_CONFIG that provides the platform setting

**/
VOID
PchPrintFlashProtectionConfig (
  IN CONST PCH_FLASH_PROTECTION_CONFIG   *FlashProtectConfig
  )
{
  UINT32 Index;

  DEBUG ((DEBUG_INFO, "------------------ PCH Flash Protection Config ------------------\n"));
  for (Index = 0; Index < PCH_FLASH_PROTECTED_RANGES; ++Index) {
    DEBUG ((DEBUG_INFO, " WriteProtectionEnable[%d] = %x\n", Index, FlashProtectConfig->ProtectRange[Index].WriteProtectionEnable));
    DEBUG ((DEBUG_INFO, " ReadProtectionEnable[%d]  = %x\n", Index, FlashProtectConfig->ProtectRange[Index].ReadProtectionEnable));
    DEBUG ((DEBUG_INFO, " ProtectedRangeLimit[%d]   = %x\n", Index, FlashProtectConfig->ProtectRange[Index].ProtectedRangeLimit));
    DEBUG ((DEBUG_INFO, " ProtectedRangeBase[%d]    = %x\n", Index, FlashProtectConfig->ProtectRange[Index].ProtectedRangeBase));
  }
}

/**
  Print PCH_P2SB_CONFIG and serial out.

  @param[in] P2sbConfig                 Pointer to a PCH_P2SB_CONFIG that provides the platform setting

**/
VOID
PchPrintP2sbConfig (
  IN CONST PCH_P2SB_CONFIG              *P2sbConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH P2SB Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "SbAccessUnlock= %x\n", P2sbConfig->SbAccessUnlock));
}

/**
  Print PCH_ESPI_CONFIG.

  @param[in] EspiConfig         Pointer to a PCH_ESPI_CONFIG that provides the eSPI setting

**/
VOID
PchPrintEspiConfig (
  IN CONST PCH_ESPI_CONFIG   *EspiConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH eSPI Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " LGMR Enable %x\n", EspiConfig->LgmrEnable));
  DEBUG ((DEBUG_INFO, " BME for Master and Slave Enabled %x\n", EspiConfig->BmeMasterSlaveEnabled));
}

/**
  Print PCH_CNVI_CONFIG.

  @param[in] CnviConfig         Pointer to a PCH_CNVI_CONFIG that provides the CNVi settings

**/
VOID
PchPrintCnviConfig (
  IN CONST PCH_CNVI_CONFIG   *CnviConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH CNVi Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "CNVi Mode = %x\n", CnviConfig->Mode));
  DEBUG ((DEBUG_INFO, "CNVi MfUart1 type = %x\n", CnviConfig->MfUart1Type));
}

/**
  Print PCH_HSIO_CONFIG.

  @param[in] HsioConfig         Pointer to a PCH_HSIO_CONFIG that provides the eSPI setting

**/
VOID
PchPrintHsioConfig (
  IN CONST PCH_HSIO_CONFIG   *HsioConfig
  )
{
  PCH_HSIO_VER_INFO             *BiosChipsetInitVerInfoPtr;
  DEBUG ((DEBUG_INFO, "------------------ PCH HSIO Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " ChipsetInit Binary Pointer            = %x\n", HsioConfig->ChipsetInitBinPtr));
  DEBUG ((DEBUG_INFO, " ChipsetInit Binary Length             = %x\n", HsioConfig->ChipsetInitBinLen));
  BiosChipsetInitVerInfoPtr = (PCH_HSIO_VER_INFO *) HsioConfig->ChipsetInitBinPtr;
  if (HsioConfig->ChipsetInitBinPtr && HsioConfig->ChipsetInitBinLen) {
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary Base CRC           = %x\n", BiosChipsetInitVerInfoPtr->BaseCrc));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary OEM CRC            = %x\n", BiosChipsetInitVerInfoPtr->OemCrc));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary SUS CRC            = %x\n", BiosChipsetInitVerInfoPtr->SusCrc));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary Version            = %x\n", BiosChipsetInitVerInfoPtr->Version));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary Product            = %x\n", BiosChipsetInitVerInfoPtr->Product));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary Metal Layer        = %x\n", BiosChipsetInitVerInfoPtr->MetalLayer));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary Base Layer         = %x\n", BiosChipsetInitVerInfoPtr->BaseLayer));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary OEM Version        = %x\n", BiosChipsetInitVerInfoPtr->OemVersion));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary Debug Mode         = %x\n", BiosChipsetInitVerInfoPtr->DebugMode));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary OEM CRC Valid      = %x\n", BiosChipsetInitVerInfoPtr->OemCrcValid));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary SUS CRC Valid      = %x\n", BiosChipsetInitVerInfoPtr->SusCrcValid));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary Base CRC Valid     = %x\n", BiosChipsetInitVerInfoPtr->BaseCrcValid));
  }
}

/**
  Print whole PCH config blocks and serial out.

  @param[in] SiPolicyPpi    The RC Policy PPI instance

**/
VOID
EFIAPI
PchPrintPolicyPpi (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  )
{
DEBUG_CODE_BEGIN();
  EFI_STATUS                      Status;
  PCH_GENERAL_CONFIG              *PchGeneralConfig;
  PCH_PCIE_CONFIG                 *PcieRpConfig;
  PCH_SATA_CONFIG                 *SataConfig;
  PCH_IOAPIC_CONFIG               *IoApicConfig;
  PCH_DMI_CONFIG                  *DmiConfig;
  PCH_FLASH_PROTECTION_CONFIG     *FlashProtectionConfig;
  PCH_HDAUDIO_CONFIG              *HdAudioConfig;
  PCH_INTERRUPT_CONFIG            *InterruptConfig;
  PCH_ISH_CONFIG                  *IshConfig;
  PCH_LAN_CONFIG                  *LanConfig;
  PCH_P2SB_CONFIG                 *P2sbConfig;
  PCH_LOCK_DOWN_CONFIG            *LockDownConfig;
  PCH_PM_CONFIG                   *PmConfig;
  PCH_SCS_CONFIG                  *ScsConfig;
  PCH_SERIAL_IO_CONFIG            *SerialIoConfig;
  PCH_LPC_SIRQ_CONFIG             *SerialIrqConfig;
  PCH_THERMAL_CONFIG              *ThermalConfig;
  USB_CONFIG                      *UsbConfig;
  PCH_ESPI_CONFIG                 *EspiConfig;
  PCH_CNVI_CONFIG                 *CnviConfig;
  PCH_HSIO_CONFIG                 *HsioConfig;
  UINT32                          SataCtrlIndex;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPcieRpConfigGuid, (VOID *) &PcieRpConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gIoApicConfigGuid, (VOID *) &IoApicConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gDmiConfigGuid, (VOID *) &DmiConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gFlashProtectionConfigGuid, (VOID *) &FlashProtectionConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gHdAudioConfigGuid, (VOID *) &HdAudioConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gInterruptConfigGuid, (VOID *) &InterruptConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gIshConfigGuid, (VOID *) &IshConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gLanConfigGuid, (VOID *) &LanConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gLockDownConfigGuid, (VOID *) &LockDownConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gP2sbConfigGuid, (VOID *) &P2sbConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gScsConfigGuid, (VOID *) &ScsConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSerialIoConfigGuid, (VOID *) &SerialIoConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSerialIrqConfigGuid, (VOID *) &SerialIrqConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gThermalConfigGuid, (VOID *) &ThermalConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gUsbConfigGuid, (VOID *) &UsbConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gEspiConfigGuid, (VOID *) &EspiConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCnviConfigGuid, (VOID *) &CnviConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gHsioConfigGuid, (VOID *) &HsioConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Print Policy Start ------------------------\n"));
  DEBUG ((DEBUG_INFO, " Revision= %x\n", SiPolicyPpi->TableHeader.Header.Revision));

  PchPrintGeneralConfig (PchGeneralConfig);
  PchPrintPcieConfig (PcieRpConfig);
  for (SataCtrlIndex = 0; SataCtrlIndex < GetPchMaxSataControllerNum (); SataCtrlIndex++) {
    SataConfig = GetPchSataConfig (SiPolicyPpi, SataCtrlIndex);
    PchPrintSataConfig (SataCtrlIndex, SataConfig);
  }
  PchPrintUsbConfig (UsbConfig);
  PchPrintIoApicConfig (IoApicConfig);
  PchPrintHdAudioConfig (HdAudioConfig);
  PchPrintLanConfig (LanConfig);
  PchPrintLockDownConfig (LockDownConfig);
  PchPrintThermalConfig (ThermalConfig);
  PchPrintPmConfig (PmConfig);
  PchPrintDmiConfig (DmiConfig);
  PchPrintSerialIrqConfig (SerialIrqConfig);
  PchPrintSerialIoConfig (SerialIoConfig);
  PchPrintInterruptConfig (InterruptConfig);
  PchPrintScsConfig (ScsConfig);
  PchPrintIshConfig (IshConfig);
  PchPrintFlashProtectionConfig (FlashProtectionConfig);
  PchPrintP2sbConfig (P2sbConfig);
  PchPrintEspiConfig (EspiConfig);
  PchPrintCnviConfig (CnviConfig);
  PchPrintHsioConfig (HsioConfig);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Print Platform Protocol End --------------------------\n"));
DEBUG_CODE_END();
}
