/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "PeiPchPolicyLibrary.h"

/**
  Print PCH_USB_CONFIG and serial out.

  @param[in] UsbConfig         Pointer to a PCH_USB_CONFIG that provides the platform setting

**/
VOID
PchPrintUsbConfig (
  IN CONST PCH_USB_CONFIG     *UsbConfig
  )
{
  UINT32 i;

  DEBUG ((DEBUG_INFO, "------------------ PCH USB Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " UsbPrecondition= %x\n", UsbConfig->UsbPrecondition));
  DEBUG ((DEBUG_INFO, " DisableComplianceMode= %x\n", UsbConfig->DisableComplianceMode));

  for (i = 0; i < GetPchUsbMaxPhysicalPortNum (); i++) {
    DEBUG ((DEBUG_INFO, " PortUsb20[%d].Enabled= %x\n", i, UsbConfig->PortUsb20[i].Enable));
    DEBUG ((DEBUG_INFO, " PortUsb20[%d].OverCurrentPin= OC%x\n", i, UsbConfig->PortUsb20[i].OverCurrentPin));
    DEBUG ((DEBUG_INFO, " PortUsb20[%d].Afe.Petxiset= %x\n", i, UsbConfig->PortUsb20[i].Afe.Petxiset));
    DEBUG ((DEBUG_INFO, " PortUsb20[%d].Afe.Txiset= %x\n", i, UsbConfig->PortUsb20[i].Afe.Txiset));
    DEBUG ((DEBUG_INFO, " PortUsb20[%d].Afe.Predeemp= %x\n", i, UsbConfig->PortUsb20[i].Afe.Predeemp));
    DEBUG ((DEBUG_INFO, " PortUsb20[%d].Afe.Pehalfbit= %x\n", i, UsbConfig->PortUsb20[i].Afe.Pehalfbit));
  }

  for (i = 0; i < GetPchXhciMaxUsb3PortNum (); i++) {
    DEBUG ((DEBUG_INFO, " PortUsb30[%d] Enabled= %x\n", i, UsbConfig->PortUsb30[i].Enable));
    DEBUG ((DEBUG_INFO, " PortUsb30[%d].OverCurrentPin= OC%x\n", i, UsbConfig->PortUsb30[i].OverCurrentPin));
    DEBUG ((DEBUG_INFO, " PortUsb30[%d].HsioTxDeEmph         = %x\n", i, UsbConfig->PortUsb30[i].HsioTxDeEmph));
    DEBUG ((DEBUG_INFO, " PortUsb30[%d].HsioTxDeEmphEnable   = %x\n", i, UsbConfig->PortUsb30[i].HsioTxDeEmphEnable));
    DEBUG ((DEBUG_INFO, " PortUsb30[%d].HsioTxDownscaleAmp          = %x\n", i, UsbConfig->PortUsb30[i].HsioTxDownscaleAmp));
    DEBUG ((DEBUG_INFO, " PortUsb30[%d].HsioTxDownscaleAmpEnable    = %x\n", i, UsbConfig->PortUsb30[i].HsioTxDownscaleAmpEnable));
  }

  DEBUG ((DEBUG_INFO, " XdciConfig.Enable= %x\n", UsbConfig->XdciConfig.Enable));

  for (i = 0; i < PCH_XHCI_MAX_SSIC_PORT_COUNT; i++) {
    DEBUG ((DEBUG_INFO, " SsicPort[%d].Enable    = %x\n", i, UsbConfig->SsicConfig.SsicPort[i].Enable));
  }

}

/**
  Print PCH_PCIE_CONFIG and serial out.

  @param[in] PcieConfig         Pointer to a PCH_PCIE_CONFIG that provides the platform setting
  @param[in] HsioPcieConfig     Pointer to a PCH_HSIO_PCIE_CONFIG that provides the platform setting

**/
VOID
PchPrintPcieConfig (
  IN CONST PCH_PCIE_CONFIG      *PcieConfig,
  IN CONST PCH_HSIO_PCIE_CONFIG *HsioPcieConfig
  )
{
  UINT32 i;

  DEBUG ((DEBUG_INFO, "------------------ PCH PCIE Config ------------------\n"));
  for (i = 0; i < GetPchMaxPciePortNum (); i++) {
    DEBUG ((DEBUG_INFO, " RootPort[%d] Enabled= %x\n", i, PcieConfig->RootPort[i].Enable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HotPlug= %x\n", i, PcieConfig->RootPort[i].HotPlug));
    DEBUG ((DEBUG_INFO, " RootPort[%d] PmSci= %x\n", i, PcieConfig->RootPort[i].PmSci));
    DEBUG ((DEBUG_INFO, " RootPort[%d] ExtSync= %x\n", i, PcieConfig->RootPort[i].ExtSync));
    DEBUG ((DEBUG_INFO, " RootPort[%d] ClkReqSupported= %x\n", i, PcieConfig->RootPort[i].ClkReqSupported));
    DEBUG ((DEBUG_INFO, " RootPort[%d] ClkReqNumber= %x\n", i, PcieConfig->RootPort[i].ClkReqNumber));
    DEBUG ((DEBUG_INFO, " RootPort[%d] ClkReqDetect= %x\n", i, PcieConfig->RootPort[i].ClkReqDetect));
    DEBUG ((DEBUG_INFO, " RootPort[%d] UnsupportedRequestReport= %x\n", i, PcieConfig->RootPort[i].UnsupportedRequestReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] FatalErrorReport= %x\n", i, PcieConfig->RootPort[i].FatalErrorReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] NoFatalErrorReport= %x\n", i, PcieConfig->RootPort[i].NoFatalErrorReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] CorrectableErrorReport= %x\n", i, PcieConfig->RootPort[i].CorrectableErrorReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SystemErrorOnFatalError= %x\n", i, PcieConfig->RootPort[i].SystemErrorOnFatalError));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SystemErrorOnNonFatalError= %x\n", i, PcieConfig->RootPort[i].SystemErrorOnNonFatalError));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SystemErrorOnCorrectableError= %x\n", i, PcieConfig->RootPort[i].SystemErrorOnCorrectableError));
    DEBUG ((DEBUG_INFO, " RootPort[%d] MaxPayload= %x\n", i, PcieConfig->RootPort[i].MaxPayload));
    DEBUG ((DEBUG_INFO, " RootPort[%d] AcsEnabled= %x\n", i, PcieConfig->RootPort[i].AcsEnabled));
    DEBUG ((DEBUG_INFO, " RootPort[%d] AdvancedErrorReporting= %x\n", i, PcieConfig->RootPort[i].AdvancedErrorReporting));
    DEBUG ((DEBUG_INFO, " RootPort[%d] TransmitterHalfSwing= %x\n", i, PcieConfig->RootPort[i].TransmitterHalfSwing));
    DEBUG ((DEBUG_INFO, " RootPort[%d] PcieSpeed= %x\n", i, PcieConfig->RootPort[i].PcieSpeed));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen3EqPh3Method= %x\n", i, PcieConfig->RootPort[i].Gen3EqPh3Method));
    DEBUG ((DEBUG_INFO, " RootPort[%d] PhysicalSlotNumber= %x\n", i, PcieConfig->RootPort[i].PhysicalSlotNumber));
    DEBUG ((DEBUG_INFO, " RootPort[%d] CompletionTimeout= %x\n", i, PcieConfig->RootPort[i].CompletionTimeout));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Aspm= %x\n", i, PcieConfig->RootPort[i].Aspm));
    DEBUG ((DEBUG_INFO, " RootPort[%d] L1Substates= %x\n", i, PcieConfig->RootPort[i].L1Substates));
    DEBUG ((DEBUG_INFO, " RootPort[%d] LtrEnable= %x\n", i, PcieConfig->RootPort[i].LtrEnable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] LtrConfigLock= %x\n", i, PcieConfig->RootPort[i].LtrConfigLock));
    DEBUG ((DEBUG_INFO, " RootPort[%d] LtrMaxSnoopLatency= %x\n", i, PcieConfig->RootPort[i].LtrMaxSnoopLatency));
    DEBUG ((DEBUG_INFO, " RootPort[%d] LtrMaxNoSnoopLatency= %x\n", i, PcieConfig->RootPort[i].LtrMaxNoSnoopLatency));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SnoopLatencyOverrideMode= %x\n", i, PcieConfig->RootPort[i].SnoopLatencyOverrideMode));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SnoopLatencyOverrideMultiplier= %x\n", i, PcieConfig->RootPort[i].SnoopLatencyOverrideMultiplier));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SnoopLatencyOverrideValue= %x\n", i, PcieConfig->RootPort[i].SnoopLatencyOverrideValue));
    DEBUG ((DEBUG_INFO, " RootPort[%d] NonSnoopLatencyOverrideMode= %x\n", i, PcieConfig->RootPort[i].NonSnoopLatencyOverrideMode));
    DEBUG ((DEBUG_INFO, " RootPort[%d] NonSnoopLatencyOverrideMultiplier= %x\n", i, PcieConfig->RootPort[i].NonSnoopLatencyOverrideMultiplier));
    DEBUG ((DEBUG_INFO, " RootPort[%d] NonSnoopLatencyOverrideValue= %x\n", i, PcieConfig->RootPort[i].NonSnoopLatencyOverrideValue));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SlotPowerLimitScale= %x\n", i, PcieConfig->RootPort[i].SlotPowerLimitScale));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SlotPowerLimitValue= %x\n", i, PcieConfig->RootPort[i].SlotPowerLimitValue));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Uptp= %x\n", i, PcieConfig->RootPort[i].Uptp));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Dptp= %x\n", i, PcieConfig->RootPort[i].Dptp));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioRxSetCtleEnable= %x\n", i, HsioPcieConfig->Lane[i].HsioRxSetCtleEnable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioRxSetCtle= %x\n", i, HsioPcieConfig->Lane[i].HsioRxSetCtle));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen1DownscaleAmpEnable= %x\n", i, HsioPcieConfig->Lane[i].HsioTxGen1DownscaleAmpEnable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen1DownscaleAmp= %x\n", i, HsioPcieConfig->Lane[i].HsioTxGen1DownscaleAmp));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen2DownscaleAmpEnable= %x\n", i, HsioPcieConfig->Lane[i].HsioTxGen2DownscaleAmpEnable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen2DownscaleAmp= %x\n", i, HsioPcieConfig->Lane[i].HsioTxGen2DownscaleAmp));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen3DownscaleAmpEnable= %x\n", i, HsioPcieConfig->Lane[i].HsioTxGen3DownscaleAmpEnable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen3DownscaleAmp= %x\n", i, HsioPcieConfig->Lane[i].HsioTxGen3DownscaleAmp));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen1DeEmphEnable= %x\n", i, HsioPcieConfig->Lane[i].HsioTxGen1DeEmphEnable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen1DeEmph= %x\n", i, HsioPcieConfig->Lane[i].HsioTxGen1DeEmph));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen2DeEmph3p5Enable= %x\n", i, HsioPcieConfig->Lane[i].HsioTxGen2DeEmph3p5Enable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen2DeEmph3p5= %x\n", i, HsioPcieConfig->Lane[i].HsioTxGen2DeEmph3p5));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen2DeEmph6p0Enable= %x\n", i, HsioPcieConfig->Lane[i].HsioTxGen2DeEmph6p0Enable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] HsioTxGen2DeEmph6p0= %x\n", i, HsioPcieConfig->Lane[i].HsioTxGen2DeEmph6p0));

  }
  DEBUG ((DEBUG_INFO, " EnablePort8xhDecode= %x\n", PcieConfig->EnablePort8xhDecode));
  DEBUG ((DEBUG_INFO, " PchPciePort8xhDecodePortIndex= %x\n", PcieConfig->PchPciePort8xhDecodePortIndex));
  DEBUG ((DEBUG_INFO, " DisableRootPortClockGating= %x\n", PcieConfig->DisableRootPortClockGating));
  DEBUG ((DEBUG_INFO, " EnablePeerMemoryWrite= %x\n", PcieConfig->EnablePeerMemoryWrite));
  DEBUG ((DEBUG_INFO, " AllowNoLtrIccPllShutdown= %x\n", PcieConfig->AllowNoLtrIccPllShutdown));
  DEBUG ((DEBUG_INFO, " ComplianceTestMode= %x\n", PcieConfig->ComplianceTestMode));
  DEBUG ((DEBUG_INFO, " RpFunctionSwap= %x\n", PcieConfig->RpFunctionSwap));
}

/**
  Print PCH_PCIE_CONFIG2 and serial out.

  @param[in] PcieConfig2        Pointer to a PCH_PCIE_CONFIG2 that provides the platform setting

**/
VOID
PchPrintPcieConfig2 (
  IN CONST PCH_PCIE_CONFIG2  *PcieConfig2
  )
{
  UINT32 Index;

  DEBUG ((DEBUG_INFO, "------------------ PCH PCIE Config2 -----------------\n"));
  for (Index = 0; Index < PCH_PCIE_SWEQ_COEFFS_MAX; Index++) {
    DEBUG ((DEBUG_INFO, " SwEqCoeffCm[%d] = %x\n", Index, PcieConfig2->SwEqCoeffList[Index].Cm));
    DEBUG ((DEBUG_INFO, " SwEqCoeffCp[%d] = %x\n", Index, PcieConfig2->SwEqCoeffList[Index].Cp));
  }
}

/**
  Print PCH_SATA_CONFIG and serial out.

  @param[in] SataConfig         Pointer to a PCH_SATA_CONFIG that provides the platform setting
  @param[in] HsioSataConfig     Pointer to a PCH_HSIO_SATA_CONFIG that provides the platform setting

**/
VOID
PchPrintSataConfig (
    IN CONST PCH_SATA_CONFIG      *SataConfig,
    IN VOID                       *HsioSataConfigPtr,
    IN UINT8                       SataControllerNo
  )
{
  UINT32 i;

  UINT32 MaxSataPortNum;
  PCH_HSIO_SATA_CONFIG  *HsioSataConfig;

  HsioSataConfig = HsioSataConfigPtr;

  if (SataControllerNo == PCH_SATA_FIRST_CONTROLLER) {
    DEBUG ((DEBUG_INFO, "------------------- PCH Primary SATA Config ------------------\n"));
    MaxSataPortNum = GetPchMaxSataPortNum ();
  } else {
    DEBUG ((DEBUG_INFO, "------------------ PCH Secondary SATA Config ------------------\n"));
    MaxSataPortNum  = GetPchMaxsSataPortNum ();
  }
  DEBUG ((DEBUG_INFO, " Enable= %x\n", SataConfig->Enable));
  DEBUG ((DEBUG_INFO, " SataMode= %x\n", SataConfig->SataMode));


  for (i = 0; i < MaxSataPortNum; i++) {
    DEBUG ((DEBUG_INFO, " PortSettings[%d] Enabled= %x\n", i, SataConfig->PortSettings[i].Enable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HotPlug= %x\n", i, SataConfig->PortSettings[i].HotPlug));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] InterlockSw= %x\n", i, SataConfig->PortSettings[i].InterlockSw));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] External= %x\n", i, SataConfig->PortSettings[i].External));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] SpinUp= %x\n", i, SataConfig->PortSettings[i].SpinUp));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] SolidStateDrive= %x\n", i, SataConfig->PortSettings[i].SolidStateDrive));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] DevSlp= %x\n", i, SataConfig->PortSettings[i].DevSlp));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] EnableDitoConfig= %x\n", i, SataConfig->PortSettings[i].EnableDitoConfig));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] DmVal= %x\n", i, SataConfig->PortSettings[i].DmVal));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] DitoVal= %x\n", i, SataConfig->PortSettings[i].DitoVal));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] ZpOdd= %x\n", i, SataConfig->PortSettings[i].ZpOdd));

    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioRxGen1EqBoostMagEnable= %x\n", i, HsioSataConfig->PortLane[i].HsioRxGen1EqBoostMagEnable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioRxGen1EqBoostMag= %x\n", i, HsioSataConfig->PortLane[i].HsioRxGen1EqBoostMag));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioRxGen2EqBoostMagEnable= %x\n", i, HsioSataConfig->PortLane[i].HsioRxGen2EqBoostMagEnable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioRxGen2EqBoostMag= %x\n", i, HsioSataConfig->PortLane[i].HsioRxGen2EqBoostMag));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioRxGen3EqBoostMagEnable= %x\n", i, HsioSataConfig->PortLane[i].HsioRxGen3EqBoostMagEnable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioRxGen3EqBoostMag= %x\n", i, HsioSataConfig->PortLane[i].HsioRxGen3EqBoostMag));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen1DownscaleAmpEnable= %x\n", i, HsioSataConfig->PortLane[i].HsioTxGen1DownscaleAmpEnable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen1DownscaleAmp= %x\n", i, HsioSataConfig->PortLane[i].HsioTxGen1DownscaleAmp));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen2DownscaleAmpEnable= %x\n", i, HsioSataConfig->PortLane[i].HsioTxGen2DownscaleAmpEnable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen2DownscaleAmp= %x\n", i, HsioSataConfig->PortLane[i].HsioTxGen2DownscaleAmp));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen3DownscaleAmpEnable= %x\n", i, HsioSataConfig->PortLane[i].HsioTxGen3DownscaleAmpEnable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen3DownscaleAmp= %x\n", i, HsioSataConfig->PortLane[i].HsioTxGen3DownscaleAmp));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen1DeEmphEnable= %x\n", i, HsioSataConfig->PortLane[i].HsioTxGen1DeEmphEnable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen1DeEmph= %x\n", i, HsioSataConfig->PortLane[i].HsioTxGen1DeEmph));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen2DeEmphEnable= %x\n", i, HsioSataConfig->PortLane[i].HsioTxGen2DeEmphEnable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen2DeEmph= %x\n", i, HsioSataConfig->PortLane[i].HsioTxGen2DeEmph));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen3DeEmphEnable= %x\n", i, HsioSataConfig->PortLane[i].HsioTxGen3DeEmphEnable));
    DEBUG ((DEBUG_INFO, " PortSettings[%d] HsioTxGen3DeEmph= %x\n", i, HsioSataConfig->PortLane[i].HsioTxGen3DeEmph));
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

  DEBUG ((DEBUG_INFO, " SpeedSupport= %x\n", SataConfig->SpeedLimit));
  DEBUG ((DEBUG_INFO, " eSATASpeedLimit= %x\n", SataConfig->eSATASpeedLimit));
  DEBUG ((DEBUG_INFO, " TestMode= %x\n", SataConfig->TestMode));
  DEBUG ((DEBUG_INFO, " SalpSupport= %x\n", SataConfig->SalpSupport));
  DEBUG ((DEBUG_INFO, " PwrOptEnable= %x\n", SataConfig->PwrOptEnable));

  for (i = 0; i < PCH_MAX_RST_PCIE_STORAGE_CR; i++) {
    DEBUG ((DEBUG_INFO, " RstPcieStorageRemap[%d].Enable                  = %x\n", i, SataConfig->RstPcieStorageRemap[i].Enable));
    DEBUG ((DEBUG_INFO, " RstPcieStorageRemap[%d].RstPcieStoragePort      = %x\n", i, SataConfig->RstPcieStorageRemap[i].RstPcieStoragePort));
    DEBUG ((DEBUG_INFO, " RstPcieStorageRemap[%d].DeviceResetDelay        = %x\n", i, SataConfig->RstPcieStorageRemap[i].DeviceResetDelay));
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
  DEBUG ((DEBUG_INFO, " IoApicId= %x\n", IoApicConfig->IoApicId));
  DEBUG ((DEBUG_INFO, " ApicRangeSelect= %x\n", IoApicConfig->ApicRangeSelect));
  DEBUG ((DEBUG_INFO, " IoApicEntry24_119= %x\n", IoApicConfig->IoApicEntry24_119));
}

/**
  Print PCH_HPET_CONFIG and serial out.

  @param[in] HpetConfig         Pointer to a PCH_HPET_CONFIG that provides the platform setting

**/
VOID
PchPrintHpetConfig (
  IN CONST PCH_HPET_CONFIG   *HpetConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH HPET Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " Enable %x\n", HpetConfig->Enable));
  DEBUG ((DEBUG_INFO, " BdfValid %x\n", HpetConfig->BdfValid));
  DEBUG ((DEBUG_INFO, " BusNumber %x\n",  HpetConfig->BusNumber));
  DEBUG ((DEBUG_INFO, " DeviceNumber %x\n", HpetConfig->DeviceNumber));
  DEBUG ((DEBUG_INFO, " FunctionNumber %x\n", HpetConfig->FunctionNumber));
  DEBUG ((DEBUG_INFO, " Base %x\n", HpetConfig->Base));
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
  Print PCH_SMBUS_CONFIG and serial out.

  @param[in] SmbusConfig         Pointer to a PCH_SMBUS_CONFIG that provides the platform setting

**/
VOID
PchPrintSmbusConfig (
  IN CONST PCH_SMBUS_CONFIG   *SmbusConfig
  )
{
  UINT32 i;

  DEBUG ((DEBUG_INFO, "------------------ PCH SMBUS Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " Enable= %x\n", SmbusConfig->Enable));
  DEBUG ((DEBUG_INFO, " ArpEnable= %x\n", SmbusConfig->ArpEnable));
  DEBUG ((DEBUG_INFO, " DynamicPowerGating= %x\n", SmbusConfig->DynamicPowerGating));
  DEBUG ((DEBUG_INFO, " SmbusIoBase= %x\n", SmbusConfig->SmbusIoBase));
  DEBUG ((DEBUG_INFO, " NumRsvdSmbusAddresses= %x\n", SmbusConfig->NumRsvdSmbusAddresses));
  DEBUG ((DEBUG_INFO, " RsvdSmbusAddressTable= {"));
  for (i = 0; i < SmbusConfig->NumRsvdSmbusAddresses; ++i) {
    DEBUG ((DEBUG_INFO, " %02xh", SmbusConfig->RsvdSmbusAddressTable[i]));
  }
  DEBUG ((DEBUG_INFO, " }\n"));
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
  DEBUG ((DEBUG_INFO, " DSP PP Module Mask     = %x\n", HdaConfig->DspPpModuleMask));
  DEBUG ((DEBUG_INFO, " ResetWaitTimer         = %x\n", HdaConfig->ResetWaitTimer));
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
  DEBUG ((DEBUG_INFO, " PowerResetStatusClear MeWakeSts     = %x\n", PmConfig->PowerResetStatusClear.MeWakeSts));
  DEBUG ((DEBUG_INFO, " PowerResetStatusClear MeHrstColdSts = %x\n", PmConfig->PowerResetStatusClear.MeHrstColdSts));
  DEBUG ((DEBUG_INFO, " PowerResetStatusClear MeHrstWarmSts = %x\n", PmConfig->PowerResetStatusClear.MeHrstWarmSts));
  DEBUG ((DEBUG_INFO, " PowerResetStatusClear MeHostPowerDn = %x\n", PmConfig->PowerResetStatusClear.MeHostPowerDn));
  DEBUG ((DEBUG_INFO, " PowerResetStatusClear WolOvrWkSts   = %x\n", PmConfig->PowerResetStatusClear.WolOvrWkSts));

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
  DEBUG ((DEBUG_INFO, " PciClockRun                         = %x\n", PmConfig->PciClockRun));
  DEBUG ((DEBUG_INFO, " SlpStrchSusUp                       = %x\n", PmConfig->SlpStrchSusUp));
  DEBUG ((DEBUG_INFO, " SlpLanLowDc                         = %x\n", PmConfig->SlpLanLowDc));
  DEBUG ((DEBUG_INFO, " PwrBtnOverridePeriod                = %x\n", PmConfig->PwrBtnOverridePeriod));
  DEBUG ((DEBUG_INFO, " DisableEnergyReport                 = %x\n", PmConfig->DisableEnergyReport));
  DEBUG ((DEBUG_INFO, " DisableDsxAcPresentPulldown         = %x\n", PmConfig->DisableDsxAcPresentPulldown));
  DEBUG ((DEBUG_INFO, " PmcReadDisable                      = %x\n", PmConfig->PmcReadDisable));
  DEBUG ((DEBUG_INFO, " PchPwrCycDur                        = %x\n", PmConfig->PchPwrCycDur));
  DEBUG ((DEBUG_INFO, " PciePllSsc                          = %x\n", PmConfig->PciePllSsc));
  DEBUG ((DEBUG_INFO, " CapsuleResetType                    = %x\n", PmConfig->CapsuleResetType));
  DEBUG ((DEBUG_INFO, " DisableNativePowerButton            = %x\n", PmConfig->DisableNativePowerButton));
  DEBUG ((DEBUG_INFO, " SlpS0Enabled                        = %x\n", PmConfig->SlpS0Enable));
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
  UINTN i;

  DEBUG ((DEBUG_INFO, "------------------ PCH Thermal Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " TsmicLock= %x\n", ThermalConfig->TsmicLock));
  DEBUG ((DEBUG_INFO, " ThermalThrottling TTLevels T0Level %x centigrade degree\n", ThermalConfig->ThermalThrottling.TTLevels.T0Level));
  DEBUG ((DEBUG_INFO, " ThermalThrottling TTLevels T1Level %x centigrade degree\n", ThermalConfig->ThermalThrottling.TTLevels.T1Level));
  DEBUG ((DEBUG_INFO, " ThermalThrottling TTLevels T2Level %x centigrade degree\n", ThermalConfig->ThermalThrottling.TTLevels.T2Level));
  DEBUG ((DEBUG_INFO, " ThermalThrottling TTLevels TTEnable %x\n", ThermalConfig->ThermalThrottling.TTLevels.TTEnable));
  DEBUG ((DEBUG_INFO, " ThermalThrottling TTLevels TTState13Enable %x\n", ThermalConfig->ThermalThrottling.TTLevels.TTState13Enable));
  DEBUG ((DEBUG_INFO, " ThermalThrottling TTLevels TTLock %x\n", ThermalConfig->ThermalThrottling.TTLevels.TTLock));
  DEBUG ((DEBUG_INFO, " ThermalThrottling TTLevels SuggestedSetting %x\n", ThermalConfig->ThermalThrottling.TTLevels.SuggestedSetting));
  DEBUG ((DEBUG_INFO, " ThermalThrottling TTLevels PchCrossThrottling %x\n", ThermalConfig->ThermalThrottling.TTLevels.PchCrossThrottling));

  DEBUG ((DEBUG_INFO, " ThermalThrottling DmiHaAWC DmiTsawEn %x\n", ThermalConfig->ThermalThrottling.DmiHaAWC.DmiTsawEn));
  DEBUG ((DEBUG_INFO, " ThermalThrottling DmiHaAWC TS0TW %x\n", ThermalConfig->ThermalThrottling.DmiHaAWC.TS0TW));
  DEBUG ((DEBUG_INFO, " ThermalThrottling DmiHaAWC TS1TW %x\n", ThermalConfig->ThermalThrottling.DmiHaAWC.TS1TW));
  DEBUG ((DEBUG_INFO, " ThermalThrottling DmiHaAWC TS2TW %x\n", ThermalConfig->ThermalThrottling.DmiHaAWC.TS2TW));
  DEBUG ((DEBUG_INFO, " ThermalThrottling DmiHaAWC TS3TW %x\n", ThermalConfig->ThermalThrottling.DmiHaAWC.TS3TW));
  DEBUG ((DEBUG_INFO, " ThermalThrottling DmiHaAWC SuggestedSetting %x\n", ThermalConfig->ThermalThrottling.DmiHaAWC.SuggestedSetting));

  DEBUG ((DEBUG_INFO, " ThermalThrottling SataTT P0T1M %x\n", ThermalConfig->ThermalThrottling.SataTT.P0T1M));
  DEBUG ((DEBUG_INFO, " ThermalThrottling SataTT P0T2M %x\n", ThermalConfig->ThermalThrottling.SataTT.P0T2M));
  DEBUG ((DEBUG_INFO, " ThermalThrottling SataTT P0T3M %x\n", ThermalConfig->ThermalThrottling.SataTT.P0T3M));
  DEBUG ((DEBUG_INFO, " ThermalThrottling SataTT P0TDisp %x\n", ThermalConfig->ThermalThrottling.SataTT.P0TDisp));
  DEBUG ((DEBUG_INFO, " ThermalThrottling SataTT P0Tinact %x\n", ThermalConfig->ThermalThrottling.SataTT.P0Tinact));
  DEBUG ((DEBUG_INFO, " ThermalThrottling SataTT P0TDispFinit %x\n", ThermalConfig->ThermalThrottling.SataTT.P0TDispFinit));
  DEBUG ((DEBUG_INFO, " ThermalThrottling SataTT P1T1M %x\n", ThermalConfig->ThermalThrottling.SataTT.P1T1M));
  DEBUG ((DEBUG_INFO, " ThermalThrottling SataTT P1T2M %x\n", ThermalConfig->ThermalThrottling.SataTT.P1T2M));
  DEBUG ((DEBUG_INFO, " ThermalThrottling SataTT P1T3M %x\n", ThermalConfig->ThermalThrottling.SataTT.P1T3M));
  DEBUG ((DEBUG_INFO, " ThermalThrottling SataTT P1TDisp %x\n", ThermalConfig->ThermalThrottling.SataTT.P1TDisp));
  DEBUG ((DEBUG_INFO, " ThermalThrottling SataTT P1Tinact %x\n", ThermalConfig->ThermalThrottling.SataTT.P1Tinact));
  DEBUG ((DEBUG_INFO, " ThermalThrottling SataTT P1TDispFinit %x\n", ThermalConfig->ThermalThrottling.SataTT.P1TDispFinit));
  DEBUG ((DEBUG_INFO, " ThermalThrottling SataTT SuggestedSetting %x\n", ThermalConfig->ThermalThrottling.SataTT.SuggestedSetting));

  DEBUG ((DEBUG_INFO, " MemoryThrottling Enable= %x\n", ThermalConfig->MemoryThrottling.Enable));
  for (i = 0; i < MaxTsGpioPin; i++) {
    DEBUG ((DEBUG_INFO, " MemoryThrottling TsGpioPinSetting PmsyncEnable= %x\n", ThermalConfig->MemoryThrottling.TsGpioPinSetting[i].PmsyncEnable));
    DEBUG ((DEBUG_INFO, " MemoryThrottling TsGpioPinSetting C0TransmitEnable= %x\n", ThermalConfig->MemoryThrottling.TsGpioPinSetting[i].C0TransmitEnable));
    DEBUG ((DEBUG_INFO, " MemoryThrottling TsGpioPinSetting PinSelection= %x\n", ThermalConfig->MemoryThrottling.TsGpioPinSetting[i].PinSelection));
  }
  DEBUG ((DEBUG_INFO, " PchHotLevel = %x\n", ThermalConfig->PchHotLevel));
  DEBUG ((DEBUG_INFO, " ThermalDeviceEnable %x\n", ThermalConfig->ThermalDeviceEnable));
}

/**
  Print PCH_GENERAL_CONFIG and serial out.

  @param[in] PchConfig         Pointer to a PCH_GENERAL_CONFIG that provides the platform setting

**/
VOID
PchPrintGeneralConfig (
  IN CONST PCH_GENERAL_CONFIG   *PchConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH General Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " SubSystemVendorId= %x\n", PchConfig->SubSystemVendorId));
  DEBUG ((DEBUG_INFO, " SubSystemId= %x\n", PchConfig->SubSystemId));
  DEBUG ((DEBUG_INFO, " Crid= %x\n", PchConfig->Crid));
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
  DEBUG ((DEBUG_INFO, " K1OffEnable= %x\n", LanConfig->K1OffEnable));
  DEBUG ((DEBUG_INFO, " ClkReqSupported= %d\n", LanConfig->ClkReqSupported));
  DEBUG ((DEBUG_INFO, " ClkReqNumber= %d\n", LanConfig->ClkReqNumber));
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
  UINTN i;
  //
  // Print interrupt information
  //
  DEBUG ((DEBUG_INFO, "------------------ PCH Interrupt Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " Interrupt assignment:\n"));
  DEBUG ((DEBUG_INFO, "  Dxx:Fx INTx IRQ\n"));
  for (i = 0; i < InterruptConfig->NumOfDevIntConfig; i++) {
    DEBUG ((DEBUG_INFO, "  D%02d:F%d    %d %03d\n",
              InterruptConfig->DevIntConfig[i].Device,
              InterruptConfig->DevIntConfig[i].Function,
              InterruptConfig->DevIntConfig[i].IntX,
              InterruptConfig->DevIntConfig[i].Irq));
  }
  DEBUG ((DEBUG_INFO, " Legacy PIC interrupt routing:\n"));
  DEBUG ((DEBUG_INFO, "  PIRQx    IRQx\n"));
  for (i = 0; i < PCH_MAX_PXRC_CONFIG; i++) {
    DEBUG ((DEBUG_INFO, "  PIRQ%c -> IRQ%d\n", i + 65, InterruptConfig->PxRcConfig[i]));
  }
  DEBUG ((DEBUG_INFO, " Other interrupt configuration:\n"));
  DEBUG ((DEBUG_INFO, "  GpioIrqRoute= %d\n", InterruptConfig->GpioIrqRoute));
  DEBUG ((DEBUG_INFO, "  SciIrqSelect= %d\n", InterruptConfig->SciIrqSelect));
  DEBUG ((DEBUG_INFO, "  TcoIrqEnable= %d\n", InterruptConfig->TcoIrqEnable));
  DEBUG ((DEBUG_INFO, "  TcoIrqSelect= %d\n", InterruptConfig->TcoIrqSelect));
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
  UINT32 i;

  DEBUG ((DEBUG_INFO, "------------------ PCH Flash Protection Config ------------------\n"));
  for (i = 0; i < PCH_FLASH_PROTECTED_RANGES; ++i) {
    DEBUG ((DEBUG_INFO, " WriteProtectionEnable[%d]= %x\n", i, FlashProtectConfig->ProtectRange[i].WriteProtectionEnable));
    DEBUG ((DEBUG_INFO, " ReadProtectionEnable[%d]= %x\n", i, FlashProtectConfig->ProtectRange[i].ReadProtectionEnable));
    DEBUG ((DEBUG_INFO, " ProtectedRangeLimit[%d]= %x\n", i, FlashProtectConfig->ProtectRange[i].ProtectedRangeLimit));
    DEBUG ((DEBUG_INFO, " ProtectedRangeBase[%d]= %x\n", i, FlashProtectConfig->ProtectRange[i].ProtectedRangeBase));
  }
}

/**
  Print PCH_WDT_CONFIG and serial out.

  @param[in] WdtConfig                  Pointer to a PCH_WDT_CONFIG that provides the platform setting

**/
VOID
PchPrintWdtConfig (
  IN CONST PCH_WDT_CONFIG               *WdtConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH WDT Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "DisableAndLock= %x\n", WdtConfig->DisableAndLock));
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
  DEBUG ((DEBUG_INFO, "PsfUnlock= %x\n", P2sbConfig->PsfUnlock));
}

/**
  Print PCH_DCI_CONFIG and serial out.

  @param[in] DciConfig                  Pointer to a PCH_DCI_CONFIG that provides the platform setting

**/
VOID
PchPrintDciConfig (
  IN CONST PCH_DCI_CONFIG               *DciConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH DCI Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "DciEn= %x\n", DciConfig->DciEn));
  DEBUG ((DEBUG_INFO, "DciAutoDetect= %x\n", DciConfig->DciAutoDetect));
}

/**
  Print PCH_LPC_CONFIG and serial out.

  @param[in] LpcConfig                  Pointer to a PCH_LPC_CONFIG that provides the platform setting

**/
VOID
PchPrintLpcConfig (
  IN CONST PCH_LPC_CONFIG               *LpcConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH LPC Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "EnhancePort8xhDecoding= %x\n", LpcConfig->EnhancePort8xhDecoding));
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
  Print whole PCH_POLICY_PPI and serial out.

  @param[in] PchPolicyPpi    The RC Policy PPI instance

**/
VOID
PchPrintPolicyPpi (
  IN  PCH_POLICY_PPI     *PchPolicyPpi
  )
{
DEBUG_CODE_BEGIN();
  DEBUG ((DEBUG_INFO, "------------------------ PCH Print Platform Protocol Start ------------------------\n"));
  DEBUG ((DEBUG_INFO, " Revision= %x\n", PchPolicyPpi->Revision));
  DEBUG ((DEBUG_INFO, " Port80Route= %x\n", PchPolicyPpi->Port80Route));
  DEBUG ((DEBUG_INFO, " AcpiBase= %x\n", PchPolicyPpi->AcpiBase));

  PchPrintGeneralConfig (&PchPolicyPpi->PchConfig);

  PchPrintPcieConfig (&PchPolicyPpi->PcieConfig, &PchPolicyPpi->HsioPcieConfig);

  PchPrintPcieConfig2 (&PchPolicyPpi->PcieConfig2);

  PchPrintSataConfig (&PchPolicyPpi->SataConfig,  &PchPolicyPpi->HsioSataConfig,  PCH_SATA_FIRST_CONTROLLER);
  PchPrintSataConfig (&PchPolicyPpi->sSataConfig, &PchPolicyPpi->HsiosSataConfig, PCH_SATA_SECOND_CONTROLLER);
  PchPrintUsbConfig (&PchPolicyPpi->UsbConfig);

  PchPrintIoApicConfig (&PchPolicyPpi->IoApicConfig);

  PchPrintHpetConfig (&PchPolicyPpi->HpetConfig);

  PchPrintHdAudioConfig (&PchPolicyPpi->HdAudioConfig);

  PchPrintLanConfig (&PchPolicyPpi->LanConfig);

  PchPrintSmbusConfig (&PchPolicyPpi->SmbusConfig);

  PchPrintLockDownConfig (&PchPolicyPpi->LockDownConfig);

  PchPrintThermalConfig (&PchPolicyPpi->ThermalConfig);

  PchPrintPmConfig (&PchPolicyPpi->PmConfig);

  PchPrintDmiConfig (&PchPolicyPpi->DmiConfig);

  PchPrintSerialIrqConfig (&PchPolicyPpi->SerialIrqConfig);


  PchPrintFlashProtectionConfig (&PchPolicyPpi->FlashProtectConfig);

  PchPrintWdtConfig (&PchPolicyPpi->WdtConfig);

  PchPrintP2sbConfig (&PchPolicyPpi->P2sbConfig);

  PchPrintDciConfig (&PchPolicyPpi->DciConfig);

  PchPrintLpcConfig (&PchPolicyPpi->LpcConfig);

  PchPrintSpiConfig (&PchPolicyPpi->SpiConfig);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Print Platform Protocol End --------------------------\n"));
DEBUG_CODE_END();
}

