/** @file
  This file is PeiPchPolicy library.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "PeiPchPolicyLibrary.h"

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
  {30, 0, PchIntA, 20}, // SerialIo: UART #0, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[7]
  {30, 1, PchIntB, 21}, // SerialIo: UART #1, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[8]
  {30, 2, PchIntC, 22}, // SerialIo: SPI #0, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[10]
  {30, 3, PchIntD, 23}, // SerialIo: SPI #1, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[11]
  {30, 4, PchIntB, 21}, // SCS: eMMC (SKL PCH-LP Only)
  {30, 5, PchIntC, 22}, // SCS: SDIO (SKL PCH-LP Only)
  {30, 6, PchIntD, 23}, // SCS: SDCard (SKL PCH-LP Only)
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
  {27, 0, PchIntA, 16}, // PCI Express Port 17 (PCH-H Only), INT is default, programmed in PciCfgSpace + FCh
  {27, 1, PchIntB, 17}, // PCI Express Port 18 (PCH-H Only), INT is default, programmed in PciCfgSpace + FCh
  {27, 2, PchIntC, 18}, // PCI Express Port 19 (PCH-H Only), INT is default, programmed in PciCfgSpace + FCh
  {27, 3, PchIntD, 19}, // PCI Express Port 20 (PCH-H Only), INT is default, programmed in PciCfgSpace + FCh
  {27, 4, PchIntA, 16}, // PCI Express Port 21 (KBL PCH-H Only), INT is default, programmed in PciCfgSpace + FCh
  {27, 5, PchIntB, 17}, // PCI Express Port 22 (KBL PCH-H Only), INT is default, programmed in PciCfgSpace + FCh
  {27, 6, PchIntC, 18}, // PCI Express Port 23 (KBL PCH-H Only), INT is default, programmed in PciCfgSpace + FCh
  {27, 7, PchIntD, 19}, // PCI Express Port 24 (KBL PCH-H Only), INT is default, programmed in PciCfgSpace + FCh
  {25, 0, PchIntA, 32}, // SerialIo UART Controller #2, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[9]
//  {24, 0, 0, 0}, // Reserved (used by RST PCIe Storage Cycle Router)
  {23, 0, PchIntA, 16}, // SATA Controller, INTA is default, programmed in PciCfgSpace + 3Dh
  {22, 0, PchIntA, 16}, // CSME: HECI #1
  {22, 1, PchIntB, 17}, // CSME: HECI #2
  {22, 2, PchIntC, 18}, // CSME: IDE-Redirection (IDE-R)
  {22, 3, PchIntD, 19}, // CSME: Keyboard and Text (KT) Redirection
  {22, 4, PchIntA, 16}, // CSME: HECI #3
//  {22, 7, PchNoInt, 0}, // CSME: WLAN
  {21, 0, PchIntA, 16}, // SerialIo I2C Controller #0, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[1]
  {21, 1, PchIntB, 17}, // SerialIo I2C Controller #1, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[2]
  {21, 2, PchIntC, 18}, // SerialIo I2C Controller #2, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[3]
  {21, 3, PchIntD, 19}, // SerialIo I2C Controller #3, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[4]
  {20, 0, PchIntA, 16}, // USB 3.0 xHCI Controller, no default value, programmed in PciCfgSpace 3Dh
  {20, 1, PchIntB, 17}, // USB Device Controller (OTG)
  {20, 2, PchIntC, 18}, // Thermal Subsystem
  {20, 3, PchIntA, 16}, // Camera IO Host Controller
//  {20, 4, 0, 0}, // TraceHub Phantom (ACPI) Function
  {19, 0, PchIntA, 20}, // Integrated Sensor Hub
//  {18, 0, PchNoInt, 0}, // CSME: KVMcc,  doesn't use interrupts
//  {18, 1, PchNoInt, 0}, // CSME: Clink,  doesn't use interrupts
//  {18, 2, PchNoInt, 0}, // CSME: PMT,  doesn't use interrupts
//  {18, 3, 0, 0}, // CSME: CSE UMA
//  {18, 4, 0, 0}  // CSME: fTPM DMA
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

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPchGeneralConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_GENERAL_CONFIG  *PchGeneralConfig;
  PchGeneralConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "PchGeneralConfig->Header.GuidHob.Name = %g\n", &PchGeneralConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PchGeneralConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PchGeneralConfig->Header.GuidHob.Header.HobLength));

  /********************************
    PCH general configuration
  ********************************/
  //
  // Default SVID SDID configuration
  //
  PchGeneralConfig->SubSystemVendorId = V_PCH_INTEL_VENDOR_ID;
  PchGeneralConfig->SubSystemId       = V_PCH_DEFAULT_SID;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPcieRpConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINTN            PortIndex;
  PCH_SERIES       PchSeries;
  PCH_PCIE_CONFIG  *PcieRpConfig;

  PcieRpConfig = ConfigBlockPointer;
  PchSeries = GetPchSeries ();

  DEBUG ((DEBUG_INFO, "PcieRpConfig->Header.GuidHob.Name = %g\n", &PcieRpConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PcieRpConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PcieRpConfig->Header.GuidHob.Header.HobLength));

  /********************************
    PCI Express related settings
  ********************************/
  PcieRpConfig->RpFunctionSwap = TRUE;

  for (PortIndex = 0; PortIndex < GetPchMaxPciePortNum (); PortIndex++) {
    PcieRpConfig->RootPort[PortIndex].Aspm                 = PchPcieAspmAutoConfig;
    PcieRpConfig->RootPort[PortIndex].PmSci                = TRUE;
    PcieRpConfig->RootPort[PortIndex].AcsEnabled           = TRUE;
    PcieRpConfig->RootPort[PortIndex].MaxPayload           = PchPcieMaxPayload256;
    PcieRpConfig->RootPort[PortIndex].EnableHotplugSmi     = TRUE;
    PcieRpConfig->RootPort[PortIndex].PhysicalSlotNumber   = (UINT8) PortIndex;
    PcieRpConfig->RootPort[PortIndex].L1Substates          = PchPcieL1SubstatesL1_1_2;
    PcieRpConfig->RootPort[PortIndex].EnableCpm            = TRUE;

    //
    // PCIe LTR Configuration.
    //
    PcieRpConfig->RootPort[PortIndex].LtrEnable             = TRUE;
    if (PchSeries == PchLp) {
      PcieRpConfig->RootPort[PortIndex].LtrMaxSnoopLatency               = 0x1003;
      PcieRpConfig->RootPort[PortIndex].LtrMaxNoSnoopLatency             = 0x1003;
    }
    if (PchSeries == PchH) {
      PcieRpConfig->RootPort[PortIndex].LtrMaxSnoopLatency               = 0x0846;
      PcieRpConfig->RootPort[PortIndex].LtrMaxNoSnoopLatency             = 0x0846;
    }
    PcieRpConfig->RootPort[PortIndex].SnoopLatencyOverrideMode           = 2;
    PcieRpConfig->RootPort[PortIndex].SnoopLatencyOverrideMultiplier     = 2;
    PcieRpConfig->RootPort[PortIndex].SnoopLatencyOverrideValue          = 60;
    PcieRpConfig->RootPort[PortIndex].NonSnoopLatencyOverrideMode        = 2;
    PcieRpConfig->RootPort[PortIndex].NonSnoopLatencyOverrideMultiplier  = 2;
    PcieRpConfig->RootPort[PortIndex].NonSnoopLatencyOverrideValue       = 60;

    PcieRpConfig->RootPort[PortIndex].Uptp                               = 5;
    PcieRpConfig->RootPort[PortIndex].Dptp                               = 7;

    PcieRpConfig->EqPh3LaneParam[PortIndex].Cm                           = 6;
    PcieRpConfig->EqPh3LaneParam[PortIndex].Cp                           = 2;
  }

  PcieRpConfig->SwEqCoeffList[0].Cm = 6;
  PcieRpConfig->SwEqCoeffList[0].Cp = 2;
  PcieRpConfig->SwEqCoeffList[1].Cm = 4;
  PcieRpConfig->SwEqCoeffList[1].Cp = 2;
  PcieRpConfig->SwEqCoeffList[2].Cm = 8;
  PcieRpConfig->SwEqCoeffList[2].Cp = 2;
  PcieRpConfig->SwEqCoeffList[3].Cm = 2;
  PcieRpConfig->SwEqCoeffList[3].Cp = 2;
  PcieRpConfig->SwEqCoeffList[4].Cm = 10;
  PcieRpConfig->SwEqCoeffList[4].Cp = 2;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadSataConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINTN            PortIndex;
  UINTN            Index;
  PCH_SATA_CONFIG  *SataConfig;
  SataConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "SataConfig->Header.GuidHob.Name = %g\n", &SataConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "SataConfig->Header.GuidHob.Header.HobLength = 0x%x\n", SataConfig->Header.GuidHob.Header.HobLength));

  /********************************
    SATA related settings
  ********************************/
  SataConfig->Enable               = TRUE;
  SataConfig->SalpSupport          = TRUE;
  SataConfig->SataMode             = PchSataModeAhci;

  for (PortIndex = 0; PortIndex < GetPchMaxSataPortNum (); PortIndex++) {
    SataConfig->PortSettings[PortIndex].Enable           = TRUE;
    SataConfig->PortSettings[PortIndex].DmVal            = 15;
    SataConfig->PortSettings[PortIndex].DitoVal          = 625;
  }

  SataConfig->Rst.Raid0            = TRUE;
  SataConfig->Rst.Raid1            = TRUE;
  SataConfig->Rst.Raid10           = TRUE;
  SataConfig->Rst.Raid5            = TRUE;
  SataConfig->Rst.Irrt             = TRUE;
  SataConfig->Rst.OromUiBanner     = TRUE;
  SataConfig->Rst.OromUiDelay      = PchSataOromDelay2sec;
  SataConfig->Rst.HddUnlock        = TRUE;
  SataConfig->Rst.LedLocate        = TRUE;
  SataConfig->Rst.IrrtOnly         = TRUE;
  SataConfig->Rst.SmartStorage     = TRUE;
  SataConfig->Rst.OptaneMemory     = TRUE;

  for (Index = 0; Index < PCH_MAX_RST_PCIE_STORAGE_CR; Index++) {
    SataConfig->RstPcieStorageRemap[Index].DeviceResetDelay             = 100;
  }

  SataConfig->PwrOptEnable     = TRUE;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadIoApicConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_IOAPIC_CONFIG  *IoApicConfig;
  IoApicConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "IoApicConfig->Header.GuidHob.Name = %g\n", &IoApicConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "IoApicConfig->Header.GuidHob.Header.HobLength = 0x%x\n", IoApicConfig->Header.GuidHob.Header.HobLength));

  /********************************
    Io Apic configuration
  ********************************/
  IoApicConfig->IoApicId           = 0x02;
  IoApicConfig->IoApicEntry24_119  = TRUE;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadCio2ConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_CIO2_CONFIG  *Cio2Config;
  Cio2Config = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "Cio2Config->Header.GuidHob.Name = %g\n", &Cio2Config->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "Cio2Config->Header.GuidHob.Header.HobLength = 0x%x\n", Cio2Config->Header.GuidHob.Header.HobLength));

  /********************************
    Initialize CIO2 Config and FLS config
  ********************************/
  Cio2Config->DeviceEnable         = TRUE;
  Cio2Config->PortATrimEnable      = TRUE;
  Cio2Config->PortBTrimEnable      = TRUE;
  Cio2Config->PortCTrimEnable      = TRUE;
  Cio2Config->PortDTrimEnable      = TRUE;
  Cio2Config->PortACtleEnable      = TRUE;
  Cio2Config->PortBCtleEnable      = TRUE;
  Cio2Config->PortCDCtleEnable     = TRUE;
  Cio2Config->PortACtleCapValue    = 0xE;
  Cio2Config->PortBCtleCapValue    = 0xE;
  Cio2Config->PortCDCtleCapValue   = 0xE;
  Cio2Config->PortACtleResValue    = 0xD;
  Cio2Config->PortBCtleResValue    = 0xD;
  Cio2Config->PortCDCtleResValue   = 0xD;
  Cio2Config->PortAClkTrimValue    = 0xA;
  Cio2Config->PortBClkTrimValue    = 0xA;
  Cio2Config->PortCClkTrimValue    = 0x9;
  Cio2Config->PortDClkTrimValue    = 0xA;
  Cio2Config->PortADataTrimValue   = 0xBBBB;
  Cio2Config->PortBDataTrimValue   = 0xBBBB;
  Cio2Config->PortCDDataTrimValue  = 0xCCCC;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadDmiConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_DMI_CONFIG  *DmiConfig;
  DmiConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "DmiConfig->Header.GuidHob.Name = %g\n", &DmiConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "DmiConfig->Header.GuidHob.Header.HobLength = 0x%x\n", DmiConfig->Header.GuidHob.Header.HobLength));

  /********************************
    DMI related settings
  ********************************/
  DmiConfig->DmiAspm = TRUE;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadFlashProtectionConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_FLASH_PROTECTION_CONFIG  *FlashProtectionConfig;
  FlashProtectionConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "FlashProtectionConfig->Header.GuidHob.Name = %g\n", &FlashProtectionConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "FlashProtectionConfig->Header.GuidHob.Header.HobLength = 0x%x\n", FlashProtectionConfig->Header.GuidHob.Header.HobLength));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadHdAudioConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_HDAUDIO_CONFIG  *HdaAudioConfig;
  HdaAudioConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "HdaAudioConfig->Header.GuidHob.Name = %g\n", &HdaAudioConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "HdaAudioConfig->Header.GuidHob.Header.HobLength = 0x%x\n", HdaAudioConfig->Header.GuidHob.Header.HobLength));

  /********************************
    HD-Audio configuration
  ********************************/
  HdaAudioConfig->Enable               = PCH_HDAUDIO_AUTO;
  HdaAudioConfig->DspEnable            = TRUE;
  HdaAudioConfig->HdAudioLinkFrequency = PchHdaLinkFreq24MHz;
  HdaAudioConfig->IDispLinkFrequency   = PchHdaLinkFreq96MHz;
  HdaAudioConfig->ResetWaitTimer       = 600; // Must be at least 521us (25 frames)
  HdaAudioConfig->DspEndpointDmic      = PchHdaDmic4chArray;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadInterruptConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINT8                 IntConfigTableEntries;
  PCH_INTERRUPT_CONFIG  *InterruptConfig;
  InterruptConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "InterruptConfig->Header.GuidHob.Name = %g\n", &InterruptConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "InterruptConfig->Header.GuidHob.Header.HobLength = 0x%x\n", InterruptConfig->Header.GuidHob.Header.HobLength));

  /********************************
    Interrupt Configuration
  ********************************/
  IntConfigTableEntries = sizeof (mDevIntConfig) / sizeof (PCH_DEVICE_INTERRUPT_CONFIG);
  ASSERT (IntConfigTableEntries <= PCH_MAX_DEVICE_INTERRUPT_CONFIG);
  InterruptConfig->NumOfDevIntConfig = IntConfigTableEntries;
  CopyMem (
    InterruptConfig->DevIntConfig,
    mDevIntConfig,
    sizeof (mDevIntConfig)
    );
  if (GetPchSeries () == PchLp) {
    CopyMem (
      &(InterruptConfig->DevIntConfig[IntConfigTableEntries]),
      mLpOnlyDevIntConfig,
      sizeof (mLpOnlyDevIntConfig)
      );
    InterruptConfig->NumOfDevIntConfig += (sizeof (mLpOnlyDevIntConfig) / sizeof (PCH_DEVICE_INTERRUPT_CONFIG));
  }

  ASSERT ((sizeof (mPxRcConfig) / sizeof (UINT8)) <= PCH_MAX_PXRC_CONFIG);
  CopyMem (
    InterruptConfig->PxRcConfig,
    mPxRcConfig,
    sizeof (mPxRcConfig)
    );

  InterruptConfig->GpioIrqRoute = 14;
  InterruptConfig->SciIrqSelect = 9;
  InterruptConfig->TcoIrqSelect = 9;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadIshConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_ISH_CONFIG  *IshConfig;
  IshConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "IshConfig->Header.GuidHob.Name = %g\n", &IshConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "IshConfig->Header.GuidHob.Header.HobLength = 0x%x\n", IshConfig->Header.GuidHob.Header.HobLength));

  /********************************
    ISH Configuration
  ********************************/
  IshConfig->Enable            = TRUE;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadLanConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_LAN_CONFIG  *LanConfig;
  LanConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "LanConfig->Header.GuidHob.Name = %g\n", &LanConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "LanConfig->Header.GuidHob.Header.HobLength = 0x%x\n", LanConfig->Header.GuidHob.Header.HobLength));

  /********************************
    Lan configuration
  ********************************/
  LanConfig->Enable               = TRUE;
  LanConfig->LtrEnable            = TRUE;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadLockDownConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_LOCK_DOWN_CONFIG  *LockDownConfig;
  LockDownConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "LockDownConfig->Header.GuidHob.Name = %g\n", &LockDownConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "LockDownConfig->Header.GuidHob.Header.HobLength = 0x%x\n", LockDownConfig->Header.GuidHob.Header.HobLength));

  /********************************
    Lockdown configuration
  ********************************/
  LockDownConfig->GlobalSmi       = TRUE;
  //
  // PCH BIOS Spec Flash Security Recommendations,
  // Intel strongly recommends that BIOS sets the BIOS Interface Lock Down bit. Enabling this bit
  // will mitigate malicious software attempts to replace the system BIOS option ROM with its own code.
  // Here we always enable this as a Policy.
  //
  LockDownConfig->BiosInterface   = TRUE;
  LockDownConfig->RtcLock         = TRUE;

  //
  // Enable BIOS region lock in SPI
  //
  LockDownConfig->BiosLock        = TRUE;
  LockDownConfig->SpiEiss         = TRUE;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadP2sbConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_P2SB_CONFIG  *P2sbConfig;
  P2sbConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "P2sbConfig->Header.GuidHob.Name = %g\n", &P2sbConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "P2sbConfig->Header.GuidHob.Header.HobLength = 0x%x\n", P2sbConfig->Header.GuidHob.Header.HobLength));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPmConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_PM_CONFIG  *PmConfig;
  PmConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "PmConfig->Header.GuidHob.Name = %g\n", &PmConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PmConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PmConfig->Header.GuidHob.Header.HobLength));

  /********************************
    MiscPm Configuration
  ********************************/
  PmConfig->MeWakeSts                            = TRUE;
  PmConfig->WolOvrWkSts                          = TRUE;

  PmConfig->WakeConfig.WolEnableOverride         = TRUE;
  PmConfig->WakeConfig.LanWakeFromDeepSx         = TRUE;

  PmConfig->PchSlpS3MinAssert                    = PchSlpS350ms;
  PmConfig->PchSlpS4MinAssert                    = PchSlpS44s;
  PmConfig->PchSlpSusMinAssert                   = PchSlpSus4s;
  PmConfig->PchSlpAMinAssert                     = PchSlpA2s;

  PmConfig->PmcReadDisable                       = TRUE;
  PmConfig->SlpLanLowDc                          = TRUE;
  PmConfig->LpcClockRun                          = TRUE;
  PmConfig->SlpS0Enable                          = TRUE;
  PmConfig->SlpS0VmEnable                        = TRUE;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPort61ConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_PORT61H_SMM_CONFIG  *Port61Config;
  Port61Config = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "Port61Config->Header.GuidHob.Name = %g\n", &Port61Config->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "Port61Config->Header.GuidHob.Header.HobLength = 0x%x\n", Port61Config->Header.GuidHob.Header.HobLength));

  /********************************
    Port 61h emulation
  ********************************/
  Port61Config->Enable   = TRUE;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadScsConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_SCS_CONFIG  *ScsConfig;
  ScsConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "ScsConfig->Header.GuidHob.Name = %g\n", &ScsConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "ScsConfig->Header.GuidHob.Header.HobLength = 0x%x\n", ScsConfig->Header.GuidHob.Header.HobLength));

  /********************************
    SCS Configuration
  ********************************/
  ScsConfig->ScsEmmcEnabled             = TRUE;
  ScsConfig->ScsSdSwitch                = PchScsSdcardMode;
  ScsConfig->ScsEmmcHs400DriverStrength = DriverStrength40Ohm;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadSerialIoConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINTN                 Index;
  PCH_SERIAL_IO_CONFIG  *SerialIoConfig;
  SerialIoConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "SerialIoConfig->Header.GuidHob.Name = %g\n", &SerialIoConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "SerialIoConfig->Header.GuidHob.Header.HobLength = 0x%x\n", SerialIoConfig->Header.GuidHob.Header.HobLength));

  /********************************
    SerialIo Configuration
  ********************************/
  for (Index = 0; Index < PCH_SERIALIO_MAX_CONTROLLERS; Index++) {
    SerialIoConfig->DevMode[Index]        = PchSerialIoPci;
  }
  SerialIoConfig->Gpio                     = TRUE;
  SerialIoConfig->DebugUartNumber          = PcdGet8 (PcdSerialIoUartNumber);
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadSerialIrqConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_LPC_SIRQ_CONFIG  *SerialIrqConfig;
  SerialIrqConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "SerialIrqConfig->Header.GuidHob.Name = %g\n", &SerialIrqConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "SerialIrqConfig->Header.GuidHob.Header.HobLength = 0x%x\n", SerialIrqConfig->Header.GuidHob.Header.HobLength));

  /********************************
    Serial IRQ Configuration
  ********************************/
  SerialIrqConfig->SirqEnable       = TRUE;
  SerialIrqConfig->SirqMode         = PchQuietMode;
  SerialIrqConfig->StartFramePulse  = PchSfpw4Clk;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadSpiConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_SPI_CONFIG  *SpiConfig;
  SpiConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "SpiConfig->Header.GuidHob.Name = %g\n", &SpiConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "SpiConfig->Header.GuidHob.Header.HobLength = 0x%x\n", SpiConfig->Header.GuidHob.Header.HobLength));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadThermalConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_THERMAL_CONFIG  *ThermalConfig;
  ThermalConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "ThermalConfig->Header.GuidHob.Name = %g\n", &ThermalConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "ThermalConfig->Header.GuidHob.Header.HobLength = 0x%x\n", ThermalConfig->Header.GuidHob.Header.HobLength));

  /********************************
    Thermal configuration.
  ********************************/
  ThermalConfig->ThermalDeviceEnable         = 1;
  ThermalConfig->TsmicLock                   = TRUE;
  ThermalConfig->TTLevels.SuggestedSetting   = TRUE;
  ThermalConfig->TTLevels.PchCrossThrottling = TRUE;
  ThermalConfig->DmiHaAWC.SuggestedSetting   = TRUE;
  ThermalConfig->SataTT.SuggestedSetting     = TRUE;
  ThermalConfig->MemoryThrottling.TsGpioPinSetting[TsGpioC].PmsyncEnable     = TRUE;
  ThermalConfig->MemoryThrottling.TsGpioPinSetting[TsGpioC].C0TransmitEnable = TRUE;
  ThermalConfig->MemoryThrottling.TsGpioPinSetting[TsGpioD].PmsyncEnable     = TRUE;
  ThermalConfig->MemoryThrottling.TsGpioPinSetting[TsGpioD].C0TransmitEnable = TRUE;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadUsbConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINTN           PortIndex;
  PCH_USB_CONFIG  *UsbConfig;
  UsbConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "UsbConfig->Header.GuidHob.Name = %g\n", &UsbConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "UsbConfig->Header.GuidHob.Header.HobLength = 0x%x\n", UsbConfig->Header.GuidHob.Header.HobLength));

  /********************************
    USB related configuration
  ********************************/
  for (PortIndex = 0; PortIndex < GetPchXhciMaxUsb2PortNum (); PortIndex++) {
    UsbConfig->PortUsb20[PortIndex].Enable  = TRUE;
  }

  for (PortIndex = 0; PortIndex < GetPchXhciMaxUsb3PortNum (); PortIndex++) {
    UsbConfig->PortUsb30[PortIndex].Enable  = TRUE;
  }
  //
  // USB Port Over Current Pins mapping, please set as per board layout.
  // Default is PchUsbOverCurrentPin0(0)
  //
  UsbConfig->PortUsb20[ 2].OverCurrentPin = PchUsbOverCurrentPin1;
  UsbConfig->PortUsb20[ 3].OverCurrentPin = PchUsbOverCurrentPin1;
  UsbConfig->PortUsb20[ 4].OverCurrentPin = PchUsbOverCurrentPin2;
  UsbConfig->PortUsb20[ 5].OverCurrentPin = PchUsbOverCurrentPin2;
  UsbConfig->PortUsb20[ 6].OverCurrentPin = PchUsbOverCurrentPin3;
  UsbConfig->PortUsb20[ 7].OverCurrentPin = PchUsbOverCurrentPin3;
  UsbConfig->PortUsb20[ 8].OverCurrentPin = PchUsbOverCurrentPin4;
  UsbConfig->PortUsb20[ 9].OverCurrentPin = PchUsbOverCurrentPin4;
  UsbConfig->PortUsb20[10].OverCurrentPin = PchUsbOverCurrentPin5;
  UsbConfig->PortUsb20[11].OverCurrentPin = PchUsbOverCurrentPin5;
  UsbConfig->PortUsb20[12].OverCurrentPin = PchUsbOverCurrentPin6;
  UsbConfig->PortUsb20[13].OverCurrentPin = PchUsbOverCurrentPin6;

  UsbConfig->PortUsb30[2].OverCurrentPin  = PchUsbOverCurrentPin1;
  UsbConfig->PortUsb30[3].OverCurrentPin  = PchUsbOverCurrentPin1;
  UsbConfig->PortUsb30[4].OverCurrentPin  = PchUsbOverCurrentPin2;
  UsbConfig->PortUsb30[5].OverCurrentPin  = PchUsbOverCurrentPin2;

  //
  // Default values of USB2 AFE settings.
  //
  for (PortIndex = 0; PortIndex < PCH_H_XHCI_MAX_USB2_PORTS; PortIndex++) {
    UsbConfig->PortUsb20[PortIndex].Afe.Petxiset  = 3;
    UsbConfig->PortUsb20[PortIndex].Afe.Txiset    = 2;
    UsbConfig->PortUsb20[PortIndex].Afe.Predeemp  = 1;
    UsbConfig->PortUsb20[PortIndex].Afe.Pehalfbit = 1;
  }

  //
  // Enable/Disable SSIC support in the setup menu
  //
  for (PortIndex = 0; PortIndex < PCH_XHCI_MAX_SSIC_PORT_COUNT; PortIndex++) {
    UsbConfig->SsicConfig.SsicPort[PortIndex].Enable   = FALSE;
  }

  //
  // xDCI configuration
  //
  UsbConfig->XdciConfig.Enable = FALSE;
}

/**
  Load Espi Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadEspiConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_ESPI_CONFIG  *EspiConfig;
  EspiConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "EspiConfig->Header.GuidHob.Name = %g\n", &EspiConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "EspiConfig->Header.GuidHob.Header.HobLength = 0x%x\n", EspiConfig->Header.GuidHob.Header.HobLength));

  /********************************
    Espi configuration.
  ********************************/
  EspiConfig->BmeMasterSlaveEnabled = TRUE;
}

static COMPONENT_BLOCK_ENTRY  mPchIpBlocks [] = {
  {&gPchGeneralConfigGuid,       sizeof (PCH_GENERAL_CONFIG),           PCH_GENERAL_CONFIG_REVISION,       LoadPchGeneralConfigDefault},
  {&gPcieRpConfigGuid,           sizeof (PCH_PCIE_CONFIG),              PCIE_RP_CONFIG_REVISION,           LoadPcieRpConfigDefault},
  {&gSataConfigGuid,             sizeof (PCH_SATA_CONFIG),              SATA_CONFIG_REVISION,              LoadSataConfigDefault},
  {&gIoApicConfigGuid,           sizeof (PCH_IOAPIC_CONFIG),            IOAPIC_CONFIG_REVISION,            LoadIoApicConfigDefault},
  {&gCio2ConfigGuid,             sizeof (PCH_CIO2_CONFIG),              CIO2_CONFIG_REVISION,              LoadCio2ConfigDefault},
  {&gDmiConfigGuid,              sizeof (PCH_DMI_CONFIG),               DMI_CONFIG_REVISION,               LoadDmiConfigDefault},
  {&gFlashProtectionConfigGuid,  sizeof (PCH_FLASH_PROTECTION_CONFIG),  FLASH_PROTECTION_CONFIG_REVISION,  LoadFlashProtectionConfigDefault},
  {&gHdAudioConfigGuid,          sizeof (PCH_HDAUDIO_CONFIG),           HDAUDIO_CONFIG_REVISION,           LoadHdAudioConfigDefault},
  {&gInterruptConfigGuid,        sizeof (PCH_INTERRUPT_CONFIG),         INTERRUPT_CONFIG_REVISION,         LoadInterruptConfigDefault},
  {&gIshConfigGuid,              sizeof (PCH_ISH_CONFIG),               ISH_CONFIG_REVISION,               LoadIshConfigDefault},
  {&gLanConfigGuid,              sizeof (PCH_LAN_CONFIG),               LAN_CONFIG_REVISION,               LoadLanConfigDefault},
  {&gLockDownConfigGuid,         sizeof (PCH_LOCK_DOWN_CONFIG),         LOCK_DOWN_CONFIG_REVISION,         LoadLockDownConfigDefault},
  {&gP2sbConfigGuid,             sizeof (PCH_P2SB_CONFIG),              P2SB_CONFIG_REVISION,              LoadP2sbConfigDefault},
  {&gPmConfigGuid,               sizeof (PCH_PM_CONFIG),                PM_CONFIG_REVISION,                LoadPmConfigDefault},
  {&gPort61ConfigGuid,           sizeof (PCH_PORT61H_SMM_CONFIG),       PORT_61_CONFIG_REVISION,           LoadPort61ConfigDefault},
  {&gScsConfigGuid,              sizeof (PCH_SCS_CONFIG),               SCS_CONFIG_REVISION,               LoadScsConfigDefault},
  {&gSerialIoConfigGuid,         sizeof (PCH_SERIAL_IO_CONFIG),         SERIAL_IO_CONFIG_REVISION,         LoadSerialIoConfigDefault},
  {&gSerialIrqConfigGuid,        sizeof (PCH_LPC_SIRQ_CONFIG),          SERIAL_IRQ_CONFIG_REVISION,        LoadSerialIrqConfigDefault},
  {&gSpiConfigGuid,              sizeof (PCH_SPI_CONFIG),               SPI_CONFIG_REVISION,               LoadSpiConfigDefault},
  {&gThermalConfigGuid,          sizeof (PCH_THERMAL_CONFIG),           THERMAL_CONFIG_REVISION,           LoadThermalConfigDefault},
  {&gUsbConfigGuid,              sizeof (PCH_USB_CONFIG),               USB_CONFIG_REVISION,               LoadUsbConfigDefault},
  {&gEspiConfigGuid,             sizeof (PCH_ESPI_CONFIG),              ESPI_CONFIG_REVISION,              LoadEspiConfigDefault}


};

/**
  Get PCH config block table total size.

  @retval                               Size of PCH config block table
**/
UINT16
EFIAPI
PchGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mPchIpBlocks[0], sizeof (mPchIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  PchAddConfigBlocks add all PCH config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add PCH config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PchAddConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  )
{
  DEBUG ((DEBUG_INFO, "PCH AddConfigBlocks\n"));

  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mPchIpBlocks[0], sizeof (mPchIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}
