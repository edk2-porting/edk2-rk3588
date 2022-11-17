/** @file
  Print whole PCH_POLICY_PPI

Copyright (c) 2017 - 2020 Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "PeiPchPolicyLibrary.h"
#include <Library/ConfigBlockLib.h>

/**
  Print PCH_USB_CONFIG and serial out.

  @param[in] UsbConfig         Pointer to a PCH_USB_CONFIG that provides the platform setting

**/
VOID
PchPrintUsbConfig (
  IN CONST PCH_USB_CONFIG     *UsbConfig
  )
{
  UINT32 Index;

  DEBUG ((DEBUG_INFO, "------------------ PCH USB Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " DisableComplianceMode = %x\n", UsbConfig->DisableComplianceMode));
  DEBUG ((DEBUG_INFO, " DelayPdoProgramming   = %x\n", UsbConfig->DelayPdoProgramming));

  for (Index = 0; Index < GetPchUsbMaxPhysicalPortNum (); Index++) {
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
    DEBUG ((DEBUG_INFO, " PortUsb30[%d].HsioTxDeEmph             = %x\n", Index, UsbConfig->PortUsb30[Index].HsioTxDeEmph));
    DEBUG ((DEBUG_INFO, " PortUsb30[%d].HsioTxDeEmphEnable       = %x\n", Index, UsbConfig->PortUsb30[Index].HsioTxDeEmphEnable));
    DEBUG ((DEBUG_INFO, " PortUsb30[%d].HsioTxDownscaleAmp       = %x\n", Index, UsbConfig->PortUsb30[Index].HsioTxDownscaleAmp));
    DEBUG ((DEBUG_INFO, " PortUsb30[%d].HsioTxDownscaleAmpEnable = %x\n", Index, UsbConfig->PortUsb30[Index].HsioTxDownscaleAmpEnable));
  }

  DEBUG ((DEBUG_INFO, " XdciConfig.Enable = %x\n", UsbConfig->XdciConfig.Enable));

  for (Index = 0; Index < PCH_XHCI_MAX_SSIC_PORT_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, " SsicPort[%d].Enable    = %x\n", Index, UsbConfig->SsicConfig.SsicPort[Index].Enable));
  }

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
    DEBUG ((DEBUG_INFO, " RootPort[%d] ClkReqSupported= %x\n", Index, PcieConfig->RootPort[Index].ClkReqSupported));
    DEBUG ((DEBUG_INFO, " RootPort[%d] ClkReqNumber= %x\n", Index, PcieConfig->RootPort[Index].ClkReqNumber));
    DEBUG ((DEBUG_INFO, " RootPort[%d] ClkReqDetect= %x\n", Index, PcieConfig->RootPort[Index].ClkReqDetect));
    DEBUG ((DEBUG_INFO, " RootPort[%d] UnsupportedRequestReport= %x\n", Index, PcieConfig->RootPort[Index].UnsupportedRequestReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] FatalErrorReport= %x\n", Index, PcieConfig->RootPort[Index].FatalErrorReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] NoFatalErrorReport= %x\n", Index, PcieConfig->RootPort[Index].NoFatalErrorReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] CorrectableErrorReport= %x\n", Index, PcieConfig->RootPort[Index].CorrectableErrorReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SystemErrorOnFatalError= %x\n", Index, PcieConfig->RootPort[Index].SystemErrorOnFatalError));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SystemErrorOnNonFatalError= %x\n", Index, PcieConfig->RootPort[Index].SystemErrorOnNonFatalError));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SystemErrorOnCorrectableError= %x\n", Index, PcieConfig->RootPort[Index].SystemErrorOnCorrectableError));
    DEBUG ((DEBUG_INFO, " RootPort[%d] MaxPayload= %x\n", Index, PcieConfig->RootPort[Index].MaxPayload));
    DEBUG ((DEBUG_INFO, " RootPort[%d] AcsEnabled= %x\n", Index, PcieConfig->RootPort[Index].AcsEnabled));
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
  for (Index = 0; Index < PCH_PCIE_SWEQ_COEFFS_MAX; Index++) {
    DEBUG ((DEBUG_INFO, " SwEqCoeffCm[%d] = %x\n", Index, PcieConfig->SwEqCoeffList[Index].Cm));
    DEBUG ((DEBUG_INFO, " SwEqCoeffCp[%d] = %x\n", Index, PcieConfig->SwEqCoeffList[Index].Cp));
  }
  DEBUG ((DEBUG_INFO, " EnablePort8xhDecode= %x\n", PcieConfig->EnablePort8xhDecode));
  DEBUG ((DEBUG_INFO, " PchPciePort8xhDecodePortIndex= %x\n", PcieConfig->PchPciePort8xhDecodePortIndex));
  DEBUG ((DEBUG_INFO, " DisableRootPortClockGating= %x\n", PcieConfig->DisableRootPortClockGating));
  DEBUG ((DEBUG_INFO, " EnablePeerMemoryWrite= %x\n", PcieConfig->EnablePeerMemoryWrite));
  DEBUG ((DEBUG_INFO, " AllowNoLtrIccPllShutdown= %x\n", PcieConfig->AllowNoLtrIccPllShutdown));
  DEBUG ((DEBUG_INFO, " ComplianceTestMode= %x\n", PcieConfig->ComplianceTestMode));
  DEBUG ((DEBUG_INFO, " RpFunctionSwap= %x\n", PcieConfig->RpFunctionSwap));
  DEBUG ((DEBUG_INFO, " PcieDeviceOverrideTablePtr= %x\n", PcieConfig->PcieDeviceOverrideTablePtr));
}

/**
  Print PCH_SATA_CONFIG and serial out.

  @param[in] SataConfig         Pointer to a PCH_SATA_CONFIG that provides the platform setting

**/
VOID
PchPrintSataConfig (
  IN CONST PCH_SATA_CONFIG      *SataConfig
  )
{
  UINT32 Index;

  DEBUG ((DEBUG_INFO, "------------------ PCH SATA Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " Enable= %x\n", SataConfig->Enable));
  DEBUG ((DEBUG_INFO, " SataMode= %x\n", SataConfig->SataMode));

  for (Index = 0; Index < GetPchMaxSataPortNum (); Index++) {
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

  DEBUG ((DEBUG_INFO, " RaidAlternateId= %x\n", SataConfig->Rst.RaidAlternateId));
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

  DEBUG ((DEBUG_INFO, " SpeedSupport= %x\n", SataConfig->SpeedLimit));
  DEBUG ((DEBUG_INFO, " EsataSpeedLimit= %x\n", SataConfig->EsataSpeedLimit));
  DEBUG ((DEBUG_INFO, " TestMode= %x\n", SataConfig->TestMode));
  DEBUG ((DEBUG_INFO, " SalpSupport= %x\n", SataConfig->SalpSupport));
  DEBUG ((DEBUG_INFO, " PwrOptEnable= %x\n", SataConfig->PwrOptEnable));

  for (Index = 0; Index < PCH_MAX_RST_PCIE_STORAGE_CR; Index++) {
    DEBUG ((DEBUG_INFO, " RstPcieStorageRemap[%d].Enable                  = %x\n", Index, SataConfig->RstPcieStorageRemap[Index].Enable));
    DEBUG ((DEBUG_INFO, " RstPcieStorageRemap[%d].RstPcieStoragePort      = %x\n", Index, SataConfig->RstPcieStorageRemap[Index].RstPcieStoragePort));
    DEBUG ((DEBUG_INFO, " RstPcieStorageRemap[%d].DeviceResetDelay        = %x\n", Index, SataConfig->RstPcieStorageRemap[Index].DeviceResetDelay));
  }
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
  DEBUG ((DEBUG_INFO, " BdfValid= %x\n", IoApicConfig->BdfValid));
  DEBUG ((DEBUG_INFO, " BusNumber= %x\n", IoApicConfig->BusNumber));
  DEBUG ((DEBUG_INFO, " DeviceNumber= %x\n", IoApicConfig->DeviceNumber));
  DEBUG ((DEBUG_INFO, " FunctionNumber= %x\n", IoApicConfig->FunctionNumber));
  DEBUG ((DEBUG_INFO, " IoApicEntry24_119= %x\n", IoApicConfig->IoApicEntry24_119));
  DEBUG ((DEBUG_INFO, " Early8254ClockGatingEnable= %x\n", IoApicConfig->Early8254ClockGatingEnable));
  DEBUG ((DEBUG_INFO, " IoApicId= %x\n", IoApicConfig->IoApicId));
  DEBUG ((DEBUG_INFO, " ApicRangeSelect= %x\n", IoApicConfig->ApicRangeSelect));
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
  DEBUG ((DEBUG_INFO, " RtcLock= %x\n", LockDownConfig->RtcLock));
  DEBUG ((DEBUG_INFO, " BiosLock= %x\n", LockDownConfig->BiosLock));
  DEBUG ((DEBUG_INFO, " SpiEiss= %x\n", LockDownConfig->SpiEiss));
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
  DEBUG ((DEBUG_INFO, " HDA Enable             = %x\n", HdaConfig->Enable));
  DEBUG ((DEBUG_INFO, " DSP Enable             = %x\n", HdaConfig->DspEnable));
  DEBUG ((DEBUG_INFO, " DSP UAA Compliance     = %x\n", HdaConfig->DspUaaCompliance));
  DEBUG ((DEBUG_INFO, " iDisp Codec Disconnect = %x\n", HdaConfig->IDispCodecDisconnect));
  DEBUG ((DEBUG_INFO, " Pme                    = %x\n", HdaConfig->Pme));
  DEBUG ((DEBUG_INFO, " I/O Buffer Ownership   = %x\n", HdaConfig->IoBufferOwnership));
  DEBUG ((DEBUG_INFO, " I/O Buffer Voltage     = %x\n", HdaConfig->IoBufferVoltage));
  DEBUG ((DEBUG_INFO, " VC Type                = %x\n", HdaConfig->VcType));
  DEBUG ((DEBUG_INFO, " HD-A Link Frequency    = %x\n", HdaConfig->HdAudioLinkFrequency));
  DEBUG ((DEBUG_INFO, " iDisp Link Frequency   = %x\n", HdaConfig->IDispLinkFrequency));
  DEBUG ((DEBUG_INFO, " iDisp Link T-Mode      = %x\n", HdaConfig->IDispLinkTmode));
  DEBUG ((DEBUG_INFO, " DSP Endpoint DMIC      = %x\n", HdaConfig->DspEndpointDmic));
  DEBUG ((DEBUG_INFO, " DSP Endpoint I2S       = %x\n", HdaConfig->DspEndpointI2s));
  DEBUG ((DEBUG_INFO, " DSP Endpoint BT        = %x\n", HdaConfig->DspEndpointBluetooth));
  DEBUG ((DEBUG_INFO, " DSP Feature Mask       = %x\n", HdaConfig->DspFeatureMask));
  DEBUG ((DEBUG_INFO, " ResetWaitTimer         = %x\n", HdaConfig->ResetWaitTimer));
  DEBUG ((DEBUG_INFO, " VerbTableEntryNum      = %x\n", HdaConfig->VerbTableEntryNum));
  DEBUG ((DEBUG_INFO, " VerbTablePtr           = %x\n", HdaConfig->VerbTablePtr));
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
  DEBUG ((DEBUG_INFO, " PmcReadDisable                      = %x\n", PmConfig->PmcReadDisable));
  DEBUG ((DEBUG_INFO, " PchPwrCycDur                        = %x\n", PmConfig->PchPwrCycDur));
  DEBUG ((DEBUG_INFO, " DisableNativePowerButton            = %x\n", PmConfig->DisableNativePowerButton));
  DEBUG ((DEBUG_INFO, " SlpS0Enabled                        = %x\n", PmConfig->SlpS0Enable));
  DEBUG ((DEBUG_INFO, " MeWakeSts                           = %x\n", PmConfig->MeWakeSts));
  DEBUG ((DEBUG_INFO, " WolOvrWkSts                         = %x\n", PmConfig->WolOvrWkSts));
  DEBUG ((DEBUG_INFO, " EnableTcoTimer                      = %x\n", PmConfig->EnableTcoTimer));
  DEBUG ((DEBUG_INFO, " SlpS0VmEnable                       = %x\n", PmConfig->SlpS0VmEnable));

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
  DEBUG ((DEBUG_INFO, " DmiAspm= %x\n", DmiConfig->DmiAspm));
  DEBUG ((DEBUG_INFO, " PwrOptEnable= %x\n", DmiConfig->PwrOptEnable));
  DEBUG ((DEBUG_INFO, " LegacyIoLowLatency= %x\n", DmiConfig->LegacyIoLowLatency));

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

  DEBUG ((DEBUG_INFO, " SataTT P0T1M %x\n", ThermalConfig->SataTT.P0T1M));
  DEBUG ((DEBUG_INFO, " SataTT P0T2M %x\n", ThermalConfig->SataTT.P0T2M));
  DEBUG ((DEBUG_INFO, " SataTT P0T3M %x\n", ThermalConfig->SataTT.P0T3M));
  DEBUG ((DEBUG_INFO, " SataTT P0TDisp %x\n", ThermalConfig->SataTT.P0TDisp));
  DEBUG ((DEBUG_INFO, " SataTT P0Tinact %x\n", ThermalConfig->SataTT.P0Tinact));
  DEBUG ((DEBUG_INFO, " SataTT P0TDispFinit %x\n", ThermalConfig->SataTT.P0TDispFinit));
  DEBUG ((DEBUG_INFO, " SataTT P1T1M %x\n", ThermalConfig->SataTT.P1T1M));
  DEBUG ((DEBUG_INFO, " SataTT P1T2M %x\n", ThermalConfig->SataTT.P1T2M));
  DEBUG ((DEBUG_INFO, " SataTT P1T3M %x\n", ThermalConfig->SataTT.P1T3M));
  DEBUG ((DEBUG_INFO, " SataTT P1TDisp %x\n", ThermalConfig->SataTT.P1TDisp));
  DEBUG ((DEBUG_INFO, " SataTT P1Tinact %x\n", ThermalConfig->SataTT.P1Tinact));
  DEBUG ((DEBUG_INFO, " SataTT P1TDispFinit %x\n", ThermalConfig->SataTT.P1TDispFinit));
  DEBUG ((DEBUG_INFO, " SataTT SuggestedSetting %x\n", ThermalConfig->SataTT.SuggestedSetting));

  DEBUG ((DEBUG_INFO, " MemoryThrottling Enable= %x\n", ThermalConfig->MemoryThrottling.Enable));
  for (Index = 0; Index < MaxTsGpioPin; Index++) {
    DEBUG ((DEBUG_INFO, " MemoryThrottling TsGpioPinSetting PmsyncEnable= %x\n", ThermalConfig->MemoryThrottling.TsGpioPinSetting[Index].PmsyncEnable));
    DEBUG ((DEBUG_INFO, " MemoryThrottling TsGpioPinSetting C0TransmitEnable= %x\n", ThermalConfig->MemoryThrottling.TsGpioPinSetting[Index].C0TransmitEnable));
    DEBUG ((DEBUG_INFO, " MemoryThrottling TsGpioPinSetting PinSelection= %x\n", ThermalConfig->MemoryThrottling.TsGpioPinSetting[Index].PinSelection));
  }
  DEBUG ((DEBUG_INFO, " PchHotLevel = %x\n", ThermalConfig->PchHotLevel));
  DEBUG ((DEBUG_INFO, " ThermalDeviceEnable %x\n", ThermalConfig->ThermalDeviceEnable));
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
  DEBUG ((DEBUG_INFO, " SubSystemVendorId= %x\n", PchGeneralConfig->SubSystemVendorId));
  DEBUG ((DEBUG_INFO, " SubSystemId= %x\n", PchGeneralConfig->SubSystemId));
  DEBUG ((DEBUG_INFO, " Crid= %x\n", PchGeneralConfig->Crid));
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
  DEBUG ((DEBUG_INFO, " K1OffEnable= %x\n", LanConfig->K1OffEnable));
  DEBUG ((DEBUG_INFO, " ClkReqSupported= %d\n", LanConfig->ClkReqSupported));
  DEBUG ((DEBUG_INFO, " ClkReqNumber= %d\n", LanConfig->ClkReqNumber));
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
  static CHAR16  DeviceName[11][5] = {L"I2C0",L"I2C1",L"I2C2",L"I2C3",L"I2C4",L"I2C5",L"SPI0",L"SPI1",L"UA00",L"UA01",L"UA02"};
#endif

  DEBUG ((DEBUG_INFO, "------------------ PCH Serial IO Config ------------------\n"));
  DEBUG_CODE_BEGIN ();
  for (Index=0; Index<PCH_SERIALIO_MAX_CONTROLLERS; Index++) {
    DEBUG ((DEBUG_INFO, " SerialIoController %s: Mode %x\n", DeviceName[Index], SerialIoConfig->DevMode[Index]));
  }
  DEBUG_CODE_END ();
  DEBUG ((DEBUG_INFO, " Gpio= %x\n", SerialIoConfig->Gpio));
  for (Index=0; Index<PCH_SERIALIO_MAX_I2C_CONTROLLERS; Index++) {
    DEBUG ((DEBUG_INFO, " I2cVoltage[%d] = %x\n", Index, SerialIoConfig->I2cVoltage[Index]));
  }
  for (Index=0; Index<PCH_SERIALIO_MAX_SPI_CONTROLLERS; Index++) {
    DEBUG ((DEBUG_INFO, " SpiCsPolarity[%d] = %x\n", Index, SerialIoConfig->SpiCsPolarity[Index]));
  }
  for (Index=0; Index<PCH_SERIALIO_MAX_UART_CONTROLLERS; Index++) {
    DEBUG ((DEBUG_INFO, " UartHwFlowCtrl[%d] = %x\n", Index, SerialIoConfig->UartHwFlowCtrl[Index]));
  }
  DEBUG ((DEBUG_INFO, " DebugUartNumber = %x\n", SerialIoConfig->DebugUartNumber));
  DEBUG ((DEBUG_INFO, " EnableDebugUartAfterPost = %x\n", SerialIoConfig->EnableDebugUartAfterPost));
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
  Print PCH_SKYCAM_CIO2_CONFIG and serial out.

  @param[in] Cio2Config                 Pointer to a PCH_SKYCAM_CIO2_CONFIG that provides the platform setting

**/
VOID
PchPrintCio2Config (
  IN CONST PCH_CIO2_CONFIG              *Cio2Config
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH CIO2 Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "DeviceEnable= %x\n", Cio2Config->DeviceEnable));
  DEBUG ((DEBUG_INFO, "SkyCamPortATermOvrEnable= %x\n", Cio2Config->SkyCamPortATermOvrEnable));
  DEBUG ((DEBUG_INFO, "SkyCamPortBTermOvrEnable= %x\n", Cio2Config->SkyCamPortBTermOvrEnable));
  DEBUG ((DEBUG_INFO, "SkyCamPortCTermOvrEnable= %x\n", Cio2Config->SkyCamPortCTermOvrEnable));
  DEBUG ((DEBUG_INFO, "SkyCamPortDTermOvrEnable= %x\n", Cio2Config->SkyCamPortDTermOvrEnable));
  DEBUG ((DEBUG_INFO, "------------------ PCH CIO2 FLS Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "PortATrimEnable= %x\n", Cio2Config->PortATrimEnable));
  DEBUG ((DEBUG_INFO, "PortBTrimEnable= %x\n", Cio2Config->PortBTrimEnable));
  DEBUG ((DEBUG_INFO, "PortCTrimEnable= %x\n", Cio2Config->PortCTrimEnable));
  DEBUG ((DEBUG_INFO, "PortDTrimEnable= %x\n", Cio2Config->PortDTrimEnable));
  DEBUG ((DEBUG_INFO, "PortACtleEnable= %x\n", Cio2Config->PortACtleEnable));
  DEBUG ((DEBUG_INFO, "PortBCtleEnable= %x\n", Cio2Config->PortBCtleEnable));
  DEBUG ((DEBUG_INFO, "PortCDCtleEnable= %x\n", Cio2Config->PortCDCtleEnable));
  DEBUG ((DEBUG_INFO, "PortACtleCapValue= %x\n", Cio2Config->PortACtleCapValue));
  DEBUG ((DEBUG_INFO, "PortBCtleCapValue= %x\n", Cio2Config->PortBCtleCapValue));
  DEBUG ((DEBUG_INFO, "PortCDCtleCapValue= %x\n", Cio2Config->PortCDCtleCapValue));
  DEBUG ((DEBUG_INFO, "PortADataTrimValue= %x\n", Cio2Config->PortADataTrimValue));
  DEBUG ((DEBUG_INFO, "PortBDataTrimValue= %x\n", Cio2Config->PortBDataTrimValue));
  DEBUG ((DEBUG_INFO, "PortCDDataTrimValue= %x\n", Cio2Config->PortCDDataTrimValue));
  DEBUG ((DEBUG_INFO, "PortACtleResValue= %x\n", Cio2Config->PortACtleResValue));
  DEBUG ((DEBUG_INFO, "PortBCtleResValue= %x\n", Cio2Config->PortBCtleResValue));
  DEBUG ((DEBUG_INFO, "PortCDCtleResValue= %x\n", Cio2Config->PortCDCtleResValue));
  DEBUG ((DEBUG_INFO, "PortAClkTrimValue= %x\n", Cio2Config->PortAClkTrimValue));
  DEBUG ((DEBUG_INFO, "PortBClkTrimValue= %x\n", Cio2Config->PortBClkTrimValue));
  DEBUG ((DEBUG_INFO, "PortCClkTrimValue= %x\n", Cio2Config->PortCClkTrimValue));
  DEBUG ((DEBUG_INFO, "PortDClkTrimValue= %x\n", Cio2Config->PortDClkTrimValue));
}

/**
  Print PCH_PORT61H_SMM_CONFIG and serial out.

  @param[in] Port61hSmmConfig           Pointer to a PCH_PORT61H_SMM_CONFIG that provides the platform setting

**/
VOID
PchPrintPort61hSmmConfig (
  IN CONST PCH_PORT61H_SMM_CONFIG       *Port61hSmmConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH Port61 Smm Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "Enable= %x\n", Port61hSmmConfig->Enable));
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
  DEBUG ((DEBUG_INFO, " ScsSdSwitch = %x\n", ScsConfig->ScsSdSwitch));
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
  DEBUG ((DEBUG_INFO, " ISH Enabled         = %x\n", IshConfig->Enable));
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
    DEBUG ((DEBUG_INFO, " WriteProtectionEnable[%d]= %x\n", Index, FlashProtectConfig->ProtectRange[Index].WriteProtectionEnable));
    DEBUG ((DEBUG_INFO, " ReadProtectionEnable[%d]= %x\n", Index, FlashProtectConfig->ProtectRange[Index].ReadProtectionEnable));
    DEBUG ((DEBUG_INFO, " ProtectedRangeLimit[%d]= %x\n", Index, FlashProtectConfig->ProtectRange[Index].ProtectedRangeLimit));
    DEBUG ((DEBUG_INFO, " ProtectedRangeBase[%d]= %x\n", Index, FlashProtectConfig->ProtectRange[Index].ProtectedRangeBase));
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
  DEBUG ((DEBUG_INFO, "SbiUnlock= %x\n", P2sbConfig->SbiUnlock));
  DEBUG ((DEBUG_INFO, "SbAccessUnlock= %x\n", P2sbConfig->SbAccessUnlock));
}

/**
  Print PCH_SPI_CONFIG and serial out.

  @param[in] SpiConfig                  Pointer to a PCH_SPI_CONFIG that provides the platform setting

**/
VOID
PchPrintSpiConfig (
  IN CONST PCH_SPI_CONFIG               *SpiConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH SPI Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "ShowSpiController= %x\n", SpiConfig->ShowSpiController));
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
  DEBUG ((DEBUG_INFO, " BME for Master and Slave Enabled %x\n", EspiConfig->BmeMasterSlaveEnabled));
}

/**
  Print whole PCH config blocks and serial out.

  @param[in] SiPolicyPpi    The RC Policy PPI instance

**/
VOID
PchPrintPolicyPpi (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                      Status;
  PCH_GENERAL_CONFIG              *PchGeneralConfig;
  PCH_PCIE_CONFIG                 *PcieRpConfig;
  PCH_SATA_CONFIG                 *SataConfig;
  PCH_IOAPIC_CONFIG               *IoApicConfig;
  PCH_CIO2_CONFIG                 *Cio2Config;
  PCH_DMI_CONFIG                  *DmiConfig;
  PCH_FLASH_PROTECTION_CONFIG     *FlashProtectionConfig;
  PCH_HDAUDIO_CONFIG              *HdAudioConfig;
  PCH_INTERRUPT_CONFIG            *InterruptConfig;
  PCH_ISH_CONFIG                  *IshConfig;
  PCH_LAN_CONFIG                  *LanConfig;
  PCH_P2SB_CONFIG                 *P2sbConfig;
  PCH_LOCK_DOWN_CONFIG            *LockDownConfig;
  PCH_PM_CONFIG                   *PmConfig;
  PCH_PORT61H_SMM_CONFIG          *Port61hConfig;
  PCH_SCS_CONFIG                  *ScsConfig;
  PCH_SERIAL_IO_CONFIG            *SerialIoConfig;
  PCH_LPC_SIRQ_CONFIG             *SerialIrqConfig;
  PCH_SPI_CONFIG                  *SpiConfig;
  PCH_THERMAL_CONFIG              *ThermalConfig;
  PCH_USB_CONFIG                  *UsbConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPcieRpConfigGuid, (VOID *) &PcieRpConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSataConfigGuid, (VOID *) &SataConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gIoApicConfigGuid, (VOID *) &IoApicConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCio2ConfigGuid, (VOID *) &Cio2Config);
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
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPort61ConfigGuid, (VOID *) &Port61hConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gScsConfigGuid, (VOID *) &ScsConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSerialIoConfigGuid, (VOID *) &SerialIoConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSerialIrqConfigGuid, (VOID *) &SerialIrqConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSpiConfigGuid, (VOID *) &SpiConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gThermalConfigGuid, (VOID *) &ThermalConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gUsbConfigGuid, (VOID *) &UsbConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Print Policy Start ------------------------\n"));
  DEBUG ((DEBUG_INFO, " Revision= %x\n", SiPolicyPpi->TableHeader.Header.Revision));

  PchPrintGeneralConfig (PchGeneralConfig);
  PchPrintPcieConfig (PcieRpConfig);
  PchPrintSataConfig (SataConfig);
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
  PchPrintCio2Config (Cio2Config);
  PchPrintPort61hSmmConfig (Port61hConfig);
  PchPrintScsConfig (ScsConfig);
  PchPrintIshConfig (IshConfig);
  PchPrintFlashProtectionConfig (FlashProtectionConfig);
  PchPrintP2sbConfig (P2sbConfig);
  PchPrintSpiConfig (SpiConfig);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Print Policy End --------------------------\n"));
  DEBUG_CODE_END ();
}

