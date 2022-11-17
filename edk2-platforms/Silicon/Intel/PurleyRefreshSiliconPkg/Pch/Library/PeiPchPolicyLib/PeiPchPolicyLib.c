/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "PeiPchPolicyLibrary.h"
#include <Library/PchPmcLib.h>

/**
  mDevIntConfig[] table contains data on INTx and IRQ for each device.
  IRQ value for devices which use ITSS INTx->PIRQx mapping need to be set in a way
  that for each multifunctional Dxx:Fy same interrupt pins must map to the same IRQ.
  Those IRQ values will be used to update ITSS.PIRx register.
  In APIC relationship between PIRQs and IRQs is:
  PIRQA -> IRQ16
  PIRQB -> IRQ17
  PIRQC -> IRQ18
  PIRQD -> IRQ19
  PIRQE -> IRQ20
  PIRQF -> IRQ21
  PIRQG -> IRQ22
  PIRQH -> IRQ23

  Devices which use INTx->PIRQy mapping are: cAVS(in PCI mode), SMBus, GbE, TraceHub, PCIe,
  SATA, HECI, IDE-R, KT Redirection, xHCI, Thermal Subsystem, Camera IO Host Controller

  PCI Express Root Ports mapping should be programmed only with values as in below table (D27/28/29)
  otherwise _PRT methods in ACPI for RootPorts would require additional patching as
  PCIe Endpoint Device Interrupt is further subjected to INTx to PIRQy Mapping

  Configured IRQ values are not used if an OS chooses to be in PIC instead of APIC mode
**/
GLOBAL_REMOVE_IF_UNREFERENCED PCH_DEVICE_INTERRUPT_CONFIG mDevIntConfig[] = {
//  {31, 0, PchNoInt, 0}, // LPC/eSPI Interface, doesn't use interrupts
//  {31, 1, PchNoInt, 0}, // P2SB, doesn't use interrupts
//  {31, 2, PchNoInt, 0}, // PMC , doesn't use interrupts
  {31, 3, PchIntA, 16}, // cAVS(Audio, Voice, Speach), INTA is default, programmed in PciCfgSpace 3Dh
  {31, 4, PchIntA, 16}, // SMBus Controller, no default value, programmed in PciCfgSpace 3Dh
//  {31, 5, PchNoInt, 0}, // SPI , doesn't use interrupts
  {31, 6, PchIntA, 16}, // GbE Controller, INTA is default, programmed in PciCfgSpace 3Dh
  {31, 7, PchIntA, 16}, // TraceHub, INTA is default, RO register
  {29, 0, PchIntA, 16}, // PCI Express Port 9, INT is default, programmed in PciCfgSpace + FCh
  {29, 1, PchIntB, 17}, // PCI Express Port 10, INT is default, programmed in PciCfgSpace + FCh
  {29, 2, PchIntC, 18}, // PCI Express Port 11, INT is default, programmed in PciCfgSpace + FCh
  {29, 3, PchIntD, 19}, // PCI Express Port 12, INT is default, programmed in PciCfgSpace + FCh
  {29, 4, PchIntA, 16}, // PCI Express Port 13 (SKL PCH-H Only), INT is default, programmed in PciCfgSpace + FCh
  {29, 5, PchIntB, 17}, // PCI Express Port 14 (SKL PCH-H Only), INT is default, programmed in PciCfgSpace + FCh
  {29, 6, PchIntC, 18}, // PCI Express Port 15 (SKL PCH-H Only), INT is default, programmed in PciCfgSpace + FCh
  {29, 7, PchIntD, 19}, // PCI Express Port 16 (SKL PCH-H Only), INT is default, programmed in PciCfgSpace + FCh
  {28, 0, PchIntA, 16}, // PCI Express Port 1, INT is default, programmed in PciCfgSpace + FCh
  {28, 1, PchIntB, 17}, // PCI Express Port 2, INT is default, programmed in PciCfgSpace + FCh
  {28, 2, PchIntC, 18}, // PCI Express Port 3, INT is default, programmed in PciCfgSpace + FCh
  {28, 3, PchIntD, 19}, // PCI Express Port 4, INT is default, programmed in PciCfgSpace + FCh
  {28, 4, PchIntA, 16}, // PCI Express Port 5, INT is default, programmed in PciCfgSpace + FCh
  {28, 5, PchIntB, 17}, // PCI Express Port 6, INT is default, programmed in PciCfgSpace + FCh
  {28, 6, PchIntC, 18}, // PCI Express Port 7, INT is default, programmed in PciCfgSpace + FCh
  {28, 7, PchIntD, 19}, // PCI Express Port 8, INT is default, programmed in PciCfgSpace + FCh
  {27, 0, PchIntA, 16}, // PCI Express Port 17 (SKL PCH-H Only), INT is default, programmed in PciCfgSpace + FCh
  {27, 1, PchIntB, 17}, // PCI Express Port 18 (SKL PCH-H Only), INT is default, programmed in PciCfgSpace + FCh
  {27, 2, PchIntC, 18}, // PCI Express Port 19 (SKL PCH-H Only), INT is default, programmed in PciCfgSpace + FCh
  {27, 3, PchIntD, 19}, // PCI Express Port 20 (SKL PCH-H Only), INT is default, programmed in PciCfgSpace + FCh
//  {24, 0, 0, 0}, // Reserved (used by RST PCIe Storage Cycle Router)
  {23, 0, PchIntA, 16}, // SATA Controller, INTA is default, programmed in PciCfgSpace + 3Dh
  {22, 0, PchIntA, 16}, // CSME: HECI #1
  {22, 1, PchIntB, 17}, // CSME: HECI #2
  {22, 2, PchIntC, 18}, // CSME: IDE-Redirection (IDE-R)
  {22, 3, PchIntD, 19}, // CSME: Keyboard and Text (KT) Redirection
  {22, 4, PchIntA, 16}, // CSME: HECI #3
//  {22, 7, PchNoInt, 0}, // CSME: WLAN
  {20, 0, PchIntA, 16}, // USB 3.0 xHCI Controller, no default value, programmed in PciCfgSpace 3Dh
  {20, 2, PchIntC, 18}, // Thermal Subsystem
//  {20, 4, 0, 0}, // TraceHub Phantom (ACPI) Function
//  {18, 0, PchNoInt, 0}, // CSME: KVMcc,  doesn't use interrupts
//  {18, 1, PchNoInt, 0}, // CSME: Clink,  doesn't use interrupts
//  {18, 2, PchNoInt, 0}, // CSME: PMT,  doesn't use interrupts
//  {18, 3, 0, 0}, // CSME: CSE UMA
//  {18, 4, 0, 0}  // CSME: fTPM DMA
  {17, 5, PchIntA, 16} // SSATA controller.
#ifdef IE_SUPPORT
  ,
//  {16, 0, PchIntA, 16}, // IE: HECI #1
//  {16, 1, PchIntB, 17}, // IE: HECI #2
//  {16, 2, PchIntC, 18}, // IE: IDE-Redirection (IDE-R)
  {16, 3, PchIntD, 19} // IE: Keyboard and Text (KT) Redirection
//  {16, 4, PchIntA, 16}  // IE: HECI #3
#endif // IE_SUPPORT
};

//
// mLpOnlyDevIntConfig[] table contains data on INTx and IRQ for devices that exist on SPT-LP but not on SPT-H.
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_DEVICE_INTERRUPT_CONFIG mLpOnlyDevIntConfig[] = {
  {25, 1, PchIntB, 33}, // SerialIo I2C Controller #5, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[6]
  {25, 2, PchIntC, 34}  // SerialIo I2C Controller #4, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[5]
};
/**
  mPxRcConfig[] table contains data for 8259 routing (how PIRQx is mapped to IRQy).
  This information is used by systems which choose to use legacy PIC
  interrupt controller. Only IRQ3-7,9-12,14,15 are valid. Values from this table
  will be programmed into ITSS.PxRC registers.
**/
GLOBAL_REMOVE_IF_UNREFERENCED UINT8 mPxRcConfig[] = {
  11,  // PARC: PIRQA -> IRQ11
  10,  // PBRC: PIRQB -> IRQ10
  11,  // PCRC: PIRQC -> IRQ11
  11,  // PDRC: PIRQD -> IRQ11
  11,  // PERC: PIRQE -> IRQ11
  11,  // PFRC: PIRQF -> IRQ11
  11,  // PGRC: PIRQG -> IRQ11
  11   // PHRC: PIRQH -> IRQ11
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT8 mSmbusRsvdAddresses[] = {
  0xA0,
  0xA2,
  0xA4,
  0xA6
};

/**
  PchCreatePolicyDefaults creates the default setting of PCH Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] PchPolicyPpi      The pointer to get PCH Policy PPI instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PchCreatePolicyDefaults (
  OUT  PCH_POLICY_PPI                   **PchPolicyPpi
  )
{
  PCH_POLICY_PPI           *PchPolicy;
  PCH_SERIES               PchSeries;
  UINT32                   PortIndex;
  UINT32                   Index;
  UINT8                    IntConfigTableEntries;

  PchSeries = GetPchSeries ();

  PchPolicy = (PCH_POLICY_PPI *) AllocateZeroPool (sizeof (PCH_POLICY_PPI));
  if (PchPolicy == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Policy not listed here are set to 0/FALSE as default.
  //

  /********************************
    General initialization
  ********************************/
  PchPolicy->Revision                = PCH_POLICY_REVISION;
  PchPolicy->AcpiBase                = PcdGet16 (PcdPchAcpiIoPortBaseAddress);
  PchPolicy->TempMemBaseAddr         = PCH_TEMP_BASE_ADDRESS;

  /********************************
    PCH general configuration
  ********************************/
  //
  // Default Svid Sdid configuration
  //
  PchPolicy->PchConfig.SubSystemVendorId = V_PCH_INTEL_VENDOR_ID;
  PchPolicy->PchConfig.SubSystemId       = V_PCH_DEFAULT_SID;

  /********************************
    PCI Express related settings
  ********************************/

  PchPolicy->TempPciBusMin = 2;
  PchPolicy->TempPciBusMax = 10;

  PchPolicy->PcieConfig.RpFunctionSwap = TRUE;

  for (PortIndex = 0; PortIndex < GetPchMaxPciePortNum (); PortIndex++) {
    PchPolicy->PcieConfig.RootPort[PortIndex].Aspm                 = PchPcieAspmAutoConfig;
    PchPolicy->PcieConfig.RootPort[PortIndex].Enable               = TRUE;
    PchPolicy->PcieConfig.RootPort[PortIndex].PmSci                = TRUE;
    PchPolicy->PcieConfig.RootPort[PortIndex].AcsEnabled           = TRUE;

    PchPolicy->PcieConfig.RootPort[PortIndex].MaxPayload           = PchPcieMaxPayload256;

    PchPolicy->PcieConfig.RootPort[PortIndex].PhysicalSlotNumber   = (UINT8) PortIndex;

    PchPolicy->PcieConfig.RootPort[PortIndex].L1Substates          = PchPcieL1SubstatesL1_1_2;

    //
    // PCIe LTR Configuration.
    //
    PchPolicy->PcieConfig.RootPort[PortIndex].LtrEnable             = TRUE;
    if (PchSeries == PchLp) {
      PchPolicy->PcieConfig.RootPort[PortIndex].LtrMaxSnoopLatency               = 0x1003;
      PchPolicy->PcieConfig.RootPort[PortIndex].LtrMaxNoSnoopLatency             = 0x1003;
    }
    if (PchSeries == PchH) {
      PchPolicy->PcieConfig.RootPort[PortIndex].LtrMaxSnoopLatency               = 0x0846;
      PchPolicy->PcieConfig.RootPort[PortIndex].LtrMaxNoSnoopLatency             = 0x0846;
    }
    PchPolicy->PcieConfig.RootPort[PortIndex].SnoopLatencyOverrideMode           = 2;
    PchPolicy->PcieConfig.RootPort[PortIndex].SnoopLatencyOverrideMultiplier     = 2;
    PchPolicy->PcieConfig.RootPort[PortIndex].SnoopLatencyOverrideValue          = 60;
    PchPolicy->PcieConfig.RootPort[PortIndex].NonSnoopLatencyOverrideMode        = 2;
    PchPolicy->PcieConfig.RootPort[PortIndex].NonSnoopLatencyOverrideMultiplier  = 2;
    PchPolicy->PcieConfig.RootPort[PortIndex].NonSnoopLatencyOverrideValue       = 60;

    PchPolicy->PcieConfig.RootPort[PortIndex].Uptp                               = 5;
    PchPolicy->PcieConfig.RootPort[PortIndex].Dptp                               = 7;
  }

  for (Index = 0; Index < GetPchMaxPciePortNum (); ++Index) {
    PchPolicy->PcieConfig.EqPh3LaneParam[Index].Cm = 6;
    PchPolicy->PcieConfig.EqPh3LaneParam[Index].Cp = 6;
  }

  PchPolicy->PcieConfig2.SwEqCoeffList[0].Cm = 6;
  PchPolicy->PcieConfig2.SwEqCoeffList[0].Cp = 8;
  PchPolicy->PcieConfig2.SwEqCoeffList[1].Cm = 8;
  PchPolicy->PcieConfig2.SwEqCoeffList[1].Cp = 2;
  PchPolicy->PcieConfig2.SwEqCoeffList[2].Cm = 10;
  PchPolicy->PcieConfig2.SwEqCoeffList[2].Cp = 6;
  PchPolicy->PcieConfig2.SwEqCoeffList[3].Cm = 12;
  PchPolicy->PcieConfig2.SwEqCoeffList[3].Cp = 8;
  PchPolicy->PcieConfig2.SwEqCoeffList[4].Cm = 14;
  PchPolicy->PcieConfig2.SwEqCoeffList[4].Cp = 2;

  /********************************
    SATA related settings
  ********************************/
  PchPolicy->SataConfig.Enable               = TRUE;
  PchPolicy->SataConfig.SalpSupport          = TRUE;
  PchPolicy->SataConfig.SataMode             = PchSataModeAhci;

  for (PortIndex = 0; PortIndex < GetPchMaxSataPortNum (); PortIndex++) {
    PchPolicy->SataConfig.PortSettings[PortIndex].Enable           = TRUE;
    PchPolicy->SataConfig.PortSettings[PortIndex].DmVal            = 15;
    PchPolicy->SataConfig.PortSettings[PortIndex].DitoVal          = 625;
  }

  PchPolicy->SataConfig.Rst.Raid0            = TRUE;
  PchPolicy->SataConfig.Rst.Raid1            = TRUE;
  PchPolicy->SataConfig.Rst.Raid10           = TRUE;
  PchPolicy->SataConfig.Rst.Raid5            = TRUE;
  PchPolicy->SataConfig.Rst.Irrt             = TRUE;
  PchPolicy->SataConfig.Rst.OromUiBanner     = TRUE;
  PchPolicy->SataConfig.Rst.OromUiDelay      = PchSataOromDelay2sec;
  PchPolicy->SataConfig.Rst.HddUnlock        = TRUE;
  PchPolicy->SataConfig.Rst.LedLocate        = TRUE;
  PchPolicy->SataConfig.Rst.IrrtOnly         = TRUE;
  PchPolicy->SataConfig.Rst.SmartStorage     = TRUE;

  for (Index = 0; Index < PCH_MAX_RST_PCIE_STORAGE_CR; Index++) {
    PchPolicy->SataConfig.RstPcieStorageRemap[Index].DeviceResetDelay             = 100;
  }
  /********************************
    sSATA related settings
  ********************************/
    PchPolicy->sSataConfig.Enable               = TRUE;
  //  PchPolicy->sSataConfig.TestMode             = FALSE;
  //  PchPolicy->sSataConfig.LegacyMode           = FALSE;
    PchPolicy->sSataConfig.SalpSupport          = TRUE;
  //  PchPolicy->sSataConfig.eSATASpeedLimit      = FALSE;
    PchPolicy->sSataConfig.SataMode             = PchSataModeAhci;
  //  PchPolicy->sSataConfig.SpeedLimit           = PchsSataSpeedDefault;

  for (PortIndex = 0; PortIndex < GetPchMaxsSataPortNum (); PortIndex++) {
      PchPolicy->sSataConfig.PortSettings[PortIndex].Enable           = TRUE;
  //    PchPolicy->sSataConfig.PortSettings[PortIndex].HotPlug          = FALSE;
  //    PchPolicy->sSataConfig.PortSettings[PortIndex].InterlockSw      = FALSE;
  //    PchPolicy->sSataConfig.PortSettings[PortIndex].External         = FALSE;
  //    PchPolicy->sSataConfig.PortSettings[PortIndex].SpinUp           = FALSE;
  //    PchPolicy->sSataConfig.PortSettings[PortIndex].SolidStateDrive  = FALSE;
  //    PchPolicy->sSataConfig.PortSettings[PortIndex].DevSlp           = FALSE;
  //    PchPolicy->sSataConfig.PortSettings[PortIndex].EnableDitoConfig = FALSE;
      PchPolicy->sSataConfig.PortSettings[PortIndex].DmVal            = 15;
      PchPolicy->sSataConfig.PortSettings[PortIndex].DitoVal          = 625;
  }

  //  PchPolicy->sSataConfig.Rst.RaidAlternateId  = FALSE;
    PchPolicy->sSataConfig.Rst.Raid0            = TRUE;
    PchPolicy->sSataConfig.Rst.Raid1            = TRUE;
    PchPolicy->sSataConfig.Rst.Raid10           = TRUE;
    PchPolicy->sSataConfig.Rst.Raid5            = TRUE;
    PchPolicy->sSataConfig.Rst.Irrt             = TRUE;
    PchPolicy->sSataConfig.Rst.OromUiBanner     = TRUE;
    PchPolicy->sSataConfig.Rst.OromUiDelay      = PchSataOromDelay2sec;
    PchPolicy->sSataConfig.Rst.HddUnlock        = TRUE;
    PchPolicy->sSataConfig.Rst.LedLocate        = TRUE;
    PchPolicy->sSataConfig.Rst.IrrtOnly         = TRUE;
    PchPolicy->sSataConfig.Rst.SmartStorage     = TRUE;

    for (Index = 0; Index < PCH_MAX_RST_PCIE_STORAGE_CR; Index++) {
      //PchPolicy->sSataConfig.RstPcieStorageRemap[Index].Enable                     = 0;
      //PchPolicy->sSataConfig.RstPcieStorageRemap[Index].RstPcieStoragePort         = 0;
      PchPolicy->sSataConfig.RstPcieStorageRemap[Index].DeviceResetDelay             = 100;
  }

  /********************************
    USB related configuration
  ********************************/
  for (PortIndex = 0; PortIndex < GetPchXhciMaxUsb2PortNum (); PortIndex++) {
    PchPolicy->UsbConfig.PortUsb20[PortIndex].Enable  = TRUE;
  }

  for (PortIndex = 0; PortIndex < GetPchXhciMaxUsb3PortNum (); PortIndex++) {
    PchPolicy->UsbConfig.PortUsb30[PortIndex].Enable  = TRUE;
  }
  //
  //XHCI Wake On USB Disabled
  //
  PchPolicy->UsbConfig.XhciWakeOnUsb = FALSE;
  //
  // USB Port Over Current Pins mapping, please set as per board layout.
  // Default is PchUsbOverCurrentPin0(0)
  //
  PchPolicy->UsbConfig.PortUsb20[ 2].OverCurrentPin = PchUsbOverCurrentPin1;
  PchPolicy->UsbConfig.PortUsb20[ 3].OverCurrentPin = PchUsbOverCurrentPin1;
  PchPolicy->UsbConfig.PortUsb20[ 4].OverCurrentPin = PchUsbOverCurrentPin2;
  PchPolicy->UsbConfig.PortUsb20[ 5].OverCurrentPin = PchUsbOverCurrentPin2;
  PchPolicy->UsbConfig.PortUsb20[ 6].OverCurrentPin = PchUsbOverCurrentPin3;
  PchPolicy->UsbConfig.PortUsb20[ 7].OverCurrentPin = PchUsbOverCurrentPin3;
  PchPolicy->UsbConfig.PortUsb20[ 8].OverCurrentPin = PchUsbOverCurrentPin4;
  PchPolicy->UsbConfig.PortUsb20[ 9].OverCurrentPin = PchUsbOverCurrentPin4;
  PchPolicy->UsbConfig.PortUsb20[10].OverCurrentPin = PchUsbOverCurrentPin5;
  PchPolicy->UsbConfig.PortUsb20[11].OverCurrentPin = PchUsbOverCurrentPin5;
  PchPolicy->UsbConfig.PortUsb20[12].OverCurrentPin = PchUsbOverCurrentPin6;
  PchPolicy->UsbConfig.PortUsb20[13].OverCurrentPin = PchUsbOverCurrentPin6;
  PchPolicy->UsbConfig.PortUsb20[14].OverCurrentPin = PchUsbOverCurrentPin7;
  PchPolicy->UsbConfig.PortUsb20[15].OverCurrentPin = PchUsbOverCurrentPin7;

  PchPolicy->UsbConfig.PortUsb30[2].OverCurrentPin  = PchUsbOverCurrentPin1;
  PchPolicy->UsbConfig.PortUsb30[3].OverCurrentPin  = PchUsbOverCurrentPin1;
  PchPolicy->UsbConfig.PortUsb30[4].OverCurrentPin  = PchUsbOverCurrentPin2;
  PchPolicy->UsbConfig.PortUsb30[5].OverCurrentPin  = PchUsbOverCurrentPin2;
  PchPolicy->UsbConfig.PortUsb30[6].OverCurrentPin  = PchUsbOverCurrentPin3;
  PchPolicy->UsbConfig.PortUsb30[7].OverCurrentPin  = PchUsbOverCurrentPin3;
  PchPolicy->UsbConfig.PortUsb30[8].OverCurrentPin  = PchUsbOverCurrentPin4;
  PchPolicy->UsbConfig.PortUsb30[9].OverCurrentPin  = PchUsbOverCurrentPin4;

  //
  // Default values of USB2 AFE settings.
  //
  for (Index = 0; Index < PCH_H_XHCI_MAX_USB2_PORTS; Index++) {

    PchPolicy->UsbConfig.PortUsb20[Index].Afe.Petxiset  = 7;
    PchPolicy->UsbConfig.PortUsb20[Index].Afe.Txiset    = 0;
    PchPolicy->UsbConfig.PortUsb20[Index].Afe.Predeemp  = 2;

	PchPolicy->UsbConfig.PortUsb20[Index].Afe.Pehalfbit = 1;
  }

  //
  // Enable/Disable SSIC support in the setup menu
  //
  for (PortIndex = 0; PortIndex < PCH_XHCI_MAX_SSIC_PORT_COUNT; PortIndex++) {
    PchPolicy->UsbConfig.SsicConfig.SsicPort[PortIndex].Enable   = FALSE;
  }

  //
  // xDCI configuration
  //
  PchPolicy->UsbConfig.XdciConfig.Enable = FALSE;


  /********************************
    Io Apic configuration
  ********************************/
  PchPolicy->IoApicConfig.IoApicId           = 0x02;
  PchPolicy->IoApicConfig.IoApicEntry24_119  = FALSE;

  /********************************
    HPET Configuration
  ********************************/
  PchPolicy->HpetConfig.Enable              = TRUE;
  PchPolicy->HpetConfig.Base                = PCH_HPET_BASE_ADDRESS;

  /********************************
    HD-Audio configuration
  ********************************/
  PchPolicy->HdAudioConfig.Enable            = PCH_HDAUDIO_AUTO;
  PchPolicy->HdAudioConfig.DspEnable         = TRUE;
  PchPolicy->HdAudioConfig.HdAudioLinkFrequency = PchHdaLinkFreq24MHz;
  PchPolicy->HdAudioConfig.IDispLinkFrequency   = PchHdaLinkFreq96MHz;
  PchPolicy->HdAudioConfig.ResetWaitTimer       = 600; // Must be at least 521us (25 frames)
  PchPolicy->HdAudioConfig.DspEndpointDmic      = PchHdaDmic4chArray;

  /********************************
    Lan configuration
  ********************************/
  PchPolicy->LanConfig.Enable               = TRUE;
  /********************************
    SMBus configuration
  ********************************/
  PchPolicy->SmbusConfig.Enable                = TRUE;
  PchPolicy->SmbusConfig.SmbusIoBase           = PcdGet16 (PcdSmbusBaseAddress);
  ASSERT (sizeof (mSmbusRsvdAddresses) <= PCH_MAX_SMBUS_RESERVED_ADDRESS);
  PchPolicy->SmbusConfig.NumRsvdSmbusAddresses = sizeof (mSmbusRsvdAddresses);
  CopyMem (
    PchPolicy->SmbusConfig.RsvdSmbusAddressTable,
    mSmbusRsvdAddresses,
    sizeof (mSmbusRsvdAddresses)
    );

  /********************************
    Lockdown configuration
  ********************************/
  PchPolicy->LockDownConfig.GlobalSmi       = TRUE;
  //
  // PCH BIOS Spec Flash Security Recommendations,
  // Intel strongly recommends that BIOS sets the BIOS Interface Lock Down bit. Enabling this bit
  // will mitigate malicious software attempts to replace the system BIOS option ROM with its own code.
  // Here we always enable this as a Policy.
  //
  PchPolicy->LockDownConfig.BiosInterface   = TRUE;
  PchPolicy->LockDownConfig.RtcLock         = TRUE;

  /********************************
    Thermal configuration.
  ********************************/
  PchPolicy->ThermalConfig.ThermalDeviceEnable                           = 0;
  PchPolicy->ThermalConfig.TsmicLock                                     = TRUE;
  PchPolicy->ThermalConfig.ThermalThrottling.TTLevels.SuggestedSetting   = TRUE;
  PchPolicy->ThermalConfig.ThermalThrottling.TTLevels.PchCrossThrottling = TRUE;
  PchPolicy->ThermalConfig.ThermalThrottling.DmiHaAWC.SuggestedSetting   = TRUE;
  PchPolicy->ThermalConfig.ThermalThrottling.SataTT.SuggestedSetting     = TRUE;
  PchPolicy->ThermalConfig.MemoryThrottling.TsGpioPinSetting[TsGpioC].PmsyncEnable     = TRUE;
  PchPolicy->ThermalConfig.MemoryThrottling.TsGpioPinSetting[TsGpioC].C0TransmitEnable = TRUE;
  PchPolicy->ThermalConfig.MemoryThrottling.TsGpioPinSetting[TsGpioD].PmsyncEnable     = TRUE;
  PchPolicy->ThermalConfig.MemoryThrottling.TsGpioPinSetting[TsGpioD].C0TransmitEnable = TRUE;

  /********************************
    MiscPm Configuration
  ********************************/
  PchPolicy->PmConfig.PowerResetStatusClear.MeWakeSts      = TRUE;
  PchPolicy->PmConfig.PowerResetStatusClear.MeHrstColdSts  = TRUE;
  PchPolicy->PmConfig.PowerResetStatusClear.MeHrstWarmSts  = TRUE;
  PchPolicy->PmConfig.PowerResetStatusClear.WolOvrWkSts    = TRUE;

  PchPolicy->PmConfig.WakeConfig.WolEnableOverride         = TRUE;
  PchPolicy->PmConfig.WakeConfig.LanWakeFromDeepSx         = TRUE;

  PchPolicy->PmConfig.PchSlpS3MinAssert                    = PchSlpS350ms;
  PchPolicy->PmConfig.PchSlpS4MinAssert                    = PchSlpS44s;
  PchPolicy->PmConfig.PchSlpSusMinAssert                   = PchSlpSus4s;
  PchPolicy->PmConfig.PchSlpAMinAssert                     = PchSlpA2s;

  PchPolicy->PmConfig.PmcReadDisable                       = TRUE;
  PchPolicy->PmConfig.SlpLanLowDc                          = TRUE;
  PchPolicy->PmConfig.PciePllSsc                           = 0xFF;

  PchPolicy->PmConfig.SlpS0Enable                          = TRUE;

  PchPolicy->PmConfig.GrPfetDurOnDef                       = PchPmGrPfetDur5us;

  /********************************
    DMI related settings
  ********************************/
  PchPolicy->DmiConfig.DmiAspm = TRUE;
  PchPolicy->DmiConfig.DmiStopAndScreamEnable = FALSE;

  /********************************
    Serial IRQ Configuration
  ********************************/
  PchPolicy->SerialIrqConfig.SirqEnable       = TRUE;
  PchPolicy->SerialIrqConfig.SirqMode         = PchQuietMode;
  PchPolicy->SerialIrqConfig.StartFramePulse  = PchSfpw4Clk;


  /********************************
    Interrupt Configuration
  ********************************/
  IntConfigTableEntries = sizeof (mDevIntConfig) / sizeof (PCH_DEVICE_INTERRUPT_CONFIG);
  ASSERT (IntConfigTableEntries <= PCH_MAX_DEVICE_INTERRUPT_CONFIG);
  PchPolicy->PchInterruptConfig.NumOfDevIntConfig = IntConfigTableEntries;
  CopyMem (
    PchPolicy->PchInterruptConfig.DevIntConfig,
    mDevIntConfig,
    sizeof (mDevIntConfig)
    );
  if (GetPchSeries () == PchLp) {
    CopyMem (
      &(PchPolicy->PchInterruptConfig.DevIntConfig[IntConfigTableEntries]),
      mLpOnlyDevIntConfig,
      sizeof (mLpOnlyDevIntConfig)
      );
    PchPolicy->PchInterruptConfig.NumOfDevIntConfig += (sizeof (mLpOnlyDevIntConfig) / sizeof (PCH_DEVICE_INTERRUPT_CONFIG));
  }

  ASSERT ((sizeof (mPxRcConfig) / sizeof (UINT8)) <= PCH_MAX_PXRC_CONFIG);
  CopyMem (
    PchPolicy->PchInterruptConfig.PxRcConfig,
    mPxRcConfig,
    sizeof (mPxRcConfig)
    );

  PchPolicy->PchInterruptConfig.GpioIrqRoute = 14;
  PchPolicy->PchInterruptConfig.SciIrqSelect = 9;
  PchPolicy->PchInterruptConfig.TcoIrqSelect = 9;


  /********************************
    Port 61h emulation
  ********************************/
  PchPolicy->Port61hSmmConfig.Enable   = TRUE;


  /********************************
    DCI Configuration
  ********************************/
  PchPolicy->DciConfig.DciAutoDetect     = TRUE;

  /********************************
    LPC Configuration
  ********************************/
  PchPolicy->LpcConfig.EnhancePort8xhDecoding     = TRUE;

  /********************************
    Power Optimizer related settings
  ********************************/
  PchPolicy->SataConfig.PwrOptEnable     = TRUE;
  PchPolicy->sSataConfig.PwrOptEnable     = TRUE;


  PchPolicy->AdrConfig.PchAdrEn = FORCE_ENABLE;
  PchPolicy->AdrConfig.AdrGpioSel = PM_SYNC_GPIO_B;
  PchPolicy->AdrConfig.AdrHostPartitionReset = FORCE_DISABLE;
  PchPolicy->AdrConfig.AdrTimerEn = FORCE_ENABLE;
  PchPolicy->AdrConfig.AdrTimerVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_100US;
  PchPolicy->AdrConfig.AdrMultiplierVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_1;

  *PchPolicyPpi = PchPolicy;
  return EFI_SUCCESS;
}

/**
  PchInstallPolicyPpi installs PchPolicyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] PchPolicyPpi      The pointer to PCH Policy PPI instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PchInstallPolicyPpi (
  IN  PCH_POLICY_PPI           *PchPolicyPpi
  )
{
  EFI_STATUS                            Status;
  EFI_PEI_PPI_DESCRIPTOR                *PchPolicyPpiDesc;

  PchPolicyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (PchPolicyPpiDesc == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  PchPolicyPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  PchPolicyPpiDesc->Guid  = &gPchPlatformPolicyPpiGuid;
  PchPolicyPpiDesc->Ppi   = PchPolicyPpi;

  //
  // Print whole PCH_POLICY_PPI and serial out.
  //
  if (PchIsDwrFlow() == FALSE) {
    PchPrintPolicyPpi (PchPolicyPpi);
  }

  //
  // Install PCH Policy PPI
  //
  Status = PeiServicesInstallPpi (PchPolicyPpiDesc);
  ASSERT_EFI_ERROR (Status);
  return Status;
}
