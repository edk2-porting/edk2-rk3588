/** @file
  This file is PeiPchPolicy library.

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiPchPolicyLibrary.h"
#include <Library/PchPcieRpLib.h>
#include <Library/CpuPlatformLib.h>
#include <Register/PchRegsLpcCnl.h>
#include <Library/ConfigBlockLib.h>

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
  UINTN            Index;
  PCH_PCIE_CONFIG  *PcieRpConfig;

  PcieRpConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "PcieRpConfig->Header.GuidHob.Name = %g\n", &PcieRpConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PcieRpConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PcieRpConfig->Header.GuidHob.Header.HobLength));

  /********************************
    PCI Express related settings
  ********************************/
  PcieRpConfig->RpFunctionSwap = TRUE;

  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    PcieRpConfig->RootPort[Index].Aspm                   = PchPcieAspmAutoConfig;
    PcieRpConfig->RootPort[Index].PmSci                  = TRUE;
    PcieRpConfig->RootPort[Index].AcsEnabled             = TRUE;
    PcieRpConfig->RootPort[Index].PtmEnabled             = TRUE;
    PcieRpConfig->RootPort[Index].DpcEnabled             = TRUE;
    PcieRpConfig->RootPort[Index].RpDpcExtensionsEnabled = TRUE;
    PcieRpConfig->RootPort[Index].MaxPayload             = PchPcieMaxPayload256;
    PcieRpConfig->RootPort[Index].SlotImplemented        = TRUE;
    PcieRpConfig->RootPort[Index].PhysicalSlotNumber     = (UINT8) Index;
    PcieRpConfig->RootPort[Index].L1Substates            = PchPcieL1SubstatesL1_1_2;
    PcieRpConfig->RootPort[Index].EnableCpm              = TRUE;
    PcieRpConfig->RootPort[Index].Gen3EqPh3Method        = PchPcieEqHardware;

    //
    // PCIe LTR Configuration.
    //
    PcieRpConfig->RootPort[Index].LtrEnable             = TRUE;

    PcieRpConfig->RootPort[Index].LtrMaxSnoopLatency               = 0x1003;
    PcieRpConfig->RootPort[Index].LtrMaxNoSnoopLatency             = 0x1003;

    PcieRpConfig->RootPort[Index].SnoopLatencyOverrideMode           = 2;
    PcieRpConfig->RootPort[Index].SnoopLatencyOverrideMultiplier     = 2;
    PcieRpConfig->RootPort[Index].SnoopLatencyOverrideValue          = 60;
    PcieRpConfig->RootPort[Index].NonSnoopLatencyOverrideMode        = 2;
    PcieRpConfig->RootPort[Index].NonSnoopLatencyOverrideMultiplier  = 2;
    PcieRpConfig->RootPort[Index].NonSnoopLatencyOverrideValue       = 60;

    PcieRpConfig->RootPort[Index].Uptp                               = 5;
    PcieRpConfig->RootPort[Index].Dptp                               = 7;

    PcieRpConfig->EqPh3LaneParam[Index].Cm                           = 6;
    PcieRpConfig->EqPh3LaneParam[Index].Cp                           = 2;
  }

  PcieRpConfig->SwEqCoeffList[0].Cm = 4;
  PcieRpConfig->SwEqCoeffList[0].Cp = 8;
  PcieRpConfig->SwEqCoeffList[1].Cm = 6;
  PcieRpConfig->SwEqCoeffList[1].Cp = 2;
  PcieRpConfig->SwEqCoeffList[2].Cm = 8;
  PcieRpConfig->SwEqCoeffList[2].Cp = 6;
  PcieRpConfig->SwEqCoeffList[3].Cm = 10;
  PcieRpConfig->SwEqCoeffList[3].Cp = 8;
  PcieRpConfig->SwEqCoeffList[4].Cm = 12;
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
  UINT32           SataCtrlIndex;
  PCH_SATA_CONFIG  *SataConfig;

  SataConfig = (PCH_SATA_CONFIG *)ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "SataConfig->Header.GuidHob.Name = %g\n", &SataConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "SataConfig->Header.GuidHob.Header.HobLength = 0x%x\n", SataConfig->Header.GuidHob.Header.HobLength));

  for (SataCtrlIndex = 0; SataCtrlIndex < GetPchMaxSataControllerNum (); SataCtrlIndex++, SataConfig++) {
    /********************************
      SATA related settings
    ********************************/
    SataConfig->Enable               = TRUE;
    SataConfig->SalpSupport          = TRUE;
    SataConfig->SataMode             = PchSataModeAhci;

    for (PortIndex = 0; PortIndex < GetPchMaxSataPortNum (SataCtrlIndex); PortIndex++) {
      SataConfig->PortSettings[PortIndex].Enable           = TRUE;
      SataConfig->PortSettings[PortIndex].DmVal            = 15;
      SataConfig->PortSettings[PortIndex].DitoVal          = 625;
    }

    SataConfig->Rst.Raid0              = TRUE;
    SataConfig->Rst.Raid1              = TRUE;
    SataConfig->Rst.Raid10             = TRUE;
    SataConfig->Rst.Raid5              = TRUE;
    SataConfig->Rst.Irrt               = TRUE;
    SataConfig->Rst.OromUiBanner       = TRUE;
    SataConfig->Rst.OromUiDelay        = PchSataOromDelay2sec;
    SataConfig->Rst.HddUnlock          = TRUE;
    SataConfig->Rst.LedLocate          = TRUE;
    SataConfig->Rst.IrrtOnly           = TRUE;
    SataConfig->Rst.SmartStorage       = TRUE;
    SataConfig->Rst.OptaneMemory       = TRUE;
    SataConfig->Rst.CpuAttachedStorage = TRUE;

    for (Index = 0; Index < PCH_MAX_RST_PCIE_STORAGE_CR; Index++) {
      SataConfig->RstPcieStorageRemap[Index].DeviceResetDelay             = 100;
    }

    SataConfig->PwrOptEnable     = TRUE;
    SataConfig->ThermalThrottling.SuggestedSetting = TRUE;
  }
}

/**
  Get Sata Config Policy

  @param[in]  SiPolicy            The RC Policy PPI instance
  @param[in]  SataCtrlIndex       SATA controller index

  @retval     SataConfig          Pointer to Sata Config Policy
**/
PCH_SATA_CONFIG *
GetPchSataConfig (
  IN SI_POLICY_PPI      *SiPolicy,
  IN UINT32             SataCtrlIndex
  )
{
  PCH_SATA_CONFIG     *SataConfig;
  EFI_STATUS          Status;

  ASSERT (SataCtrlIndex < GetPchMaxSataControllerNum ());

  Status = GetConfigBlock ((VOID *) SiPolicy, &gSataConfigGuid, (VOID *) &SataConfig);
  ASSERT_EFI_ERROR (Status);

  SataConfig += SataCtrlIndex;

  return SataConfig;
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
  IoApicConfig->IoApicId                   = 0x02;
  IoApicConfig->IoApicEntry24_119          = TRUE;
  IoApicConfig->Enable8254ClockGating      = TRUE;
  IoApicConfig->Enable8254ClockGatingOnS3  = TRUE;
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
  DmiConfig->DmiAspmCtrl = PchPcieAspmAutoConfig;
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
  PCH_HDAUDIO_CONFIG  *HdAudioConfig;
  HdAudioConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "HdAudioConfig->Header.GuidHob.Name = %g\n", &HdAudioConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "HdAudioConfig->Header.GuidHob.Header.HobLength = 0x%x\n", HdAudioConfig->Header.GuidHob.Header.HobLength));

  /********************************
    HD-Audio configuration
  ********************************/
  HdAudioConfig->DspEnable            = TRUE;
  HdAudioConfig->HdAudioLinkFrequency = PchHdaLinkFreq24MHz;
  HdAudioConfig->IDispLinkFrequency   = PchHdaLinkFreq96MHz;
  HdAudioConfig->IDispLinkTmode       = PchHdaIDispMode2T;
  HdAudioConfig->ResetWaitTimer       = 600; // Must be at least 521us (25 frames)
  HdAudioConfig->AudioLinkHda         = TRUE;
  HdAudioConfig->AudioLinkDmic0       = TRUE;
  HdAudioConfig->AudioLinkDmic1       = TRUE;
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
  PCH_INTERRUPT_CONFIG  *InterruptConfig;
  InterruptConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "InterruptConfig->Header.GuidHob.Name = %g\n", &InterruptConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "InterruptConfig->Header.GuidHob.Header.HobLength = 0x%x\n", InterruptConfig->Header.GuidHob.Header.HobLength));

  LoadDeviceInterruptConfig (InterruptConfig);

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
  LanConfig->Enable = TRUE;
  LanConfig->LtrEnable = TRUE;
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
  LockDownConfig->BiosInterface   = TRUE;
  LockDownConfig->RtcMemoryLock   = TRUE;
  LockDownConfig->BiosLock        = TRUE;
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
  PmConfig->PchSlpS4MinAssert                    = PchSlpS41s;
  PmConfig->PchSlpSusMinAssert                   = PchSlpSus4s;
  PmConfig->PchSlpAMinAssert                     = PchSlpA2s;

  PmConfig->SlpLanLowDc                          = TRUE;
  PmConfig->PciePllSsc                           = 0xFF;
  PmConfig->LpcClockRun                          = TRUE;
  PmConfig->SlpS0Enable                          = TRUE;
  PmConfig->CpuC10GatePinEnable                  = TRUE;
  if (IsWhlCpu () && (GetCpuStepping () == EnumCflV0)) {
    PmConfig->SlpS0WithGbeSupport                  = FALSE;
  } else {
    PmConfig->SlpS0WithGbeSupport                  = TRUE;
  }

  if (IsPchLp ()) {
    PmConfig->ModPhySusPgEnable                  = TRUE;
  }
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
  ScsConfig->ScsEmmcEnabled   = IsPchLp () ? TRUE : FALSE; // eMMC present on PCH-LP only
  ScsConfig->ScsEmmcHs400DriverStrength = DriverStrength40Ohm;
  //Enable Sd Card controller for Non-Desktop sku platforms
  if (GetCpuSku () != EnumCpuTrad) {
    ScsConfig->ScsSdcardEnabled = TRUE;
  }
  ScsConfig->SdCardPowerEnableActiveHigh = TRUE;
  ScsConfig->ScsUfsEnabled    = TRUE;
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
  for (Index = 0; Index < GetPchMaxSerialIoControllersNum (); Index++) {
    SerialIoConfig->DevMode[Index]         = PchSerialIoPci;
  }
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
  ThermalConfig->TsmicLock                   = TRUE;
  ThermalConfig->PchHotLevel                 = 0x154;
  ThermalConfig->TTLevels.SuggestedSetting   = TRUE;
  ThermalConfig->TTLevels.PchCrossThrottling = TRUE;
  ThermalConfig->DmiHaAWC.SuggestedSetting   = TRUE;

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
  USB_CONFIG      *UsbConfig;
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
  // BIOS should program PDO in PEI phase by default
  //
  UsbConfig->PdoProgramming = TRUE;

  //
  // Default values of USB2 AFE settings.
  //
  UsbConfig->Usb2PhySusPgEnable = TRUE;
  for (PortIndex = 0; PortIndex < GetPchXhciMaxUsb2PortNum (); PortIndex++) {
    UsbConfig->PortUsb20[PortIndex].Afe.Petxiset  = 3;
    UsbConfig->PortUsb20[PortIndex].Afe.Txiset    = 2;
    UsbConfig->PortUsb20[PortIndex].Afe.Predeemp  = 1;
    UsbConfig->PortUsb20[PortIndex].Afe.Pehalfbit = 1;
  }

  for (PortIndex = 0; PortIndex < GetPchXhciMaxUsb3PortNum (); PortIndex++) {
    UsbConfig->PortUsb30HsioRx[PortIndex].HsioOlfpsCfgPullUpDwnRes = 3;
  }

  UsbConfig->XhciOcLock = TRUE;

  //
  // xDCI configuration
  //
  UsbConfig->XdciConfig.Enable = FALSE;
}

/**
  Load Config block default

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

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadCnviConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_CNVI_CONFIG  *CnviConfig;
  CnviConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "CnviConfig->Header.GuidHob.Name = %g\n", &CnviConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "CnviConfig->Header.GuidHob.Header.HobLength = 0x%x\n", CnviConfig->Header.GuidHob.Header.HobLength));

  /********************************
    Cnvi configuration.
  ********************************/
  CnviConfig->Mode = CnviModeAuto; // Automatic detection
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadHsioConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_HSIO_CONFIG  *HsioConfig;
  HsioConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "HsioConfig->Header.GuidHob.Name = %g\n", &HsioConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "HsioConfig->Header.GuidHob.Header.HobLength = 0x%x\n", HsioConfig->Header.GuidHob.Header.HobLength));
}

GLOBAL_REMOVE_IF_UNREFERENCED COMPONENT_BLOCK_ENTRY  mPchIpBlocks [] = {
  {&gPchGeneralConfigGuid,       sizeof (PCH_GENERAL_CONFIG),           PCH_GENERAL_CONFIG_REVISION,       LoadPchGeneralConfigDefault},
  {&gPcieRpConfigGuid,           sizeof (PCH_PCIE_CONFIG),              PCIE_RP_CONFIG_REVISION,           LoadPcieRpConfigDefault},
  {&gSataConfigGuid,             sizeof (PCH_SATA_CONFIG),              SATA_CONFIG_REVISION,              LoadSataConfigDefault},
  {&gIoApicConfigGuid,           sizeof (PCH_IOAPIC_CONFIG),            IOAPIC_CONFIG_REVISION,            LoadIoApicConfigDefault},
  {&gDmiConfigGuid,              sizeof (PCH_DMI_CONFIG),               DMI_CONFIG_REVISION,               LoadDmiConfigDefault},
  {&gFlashProtectionConfigGuid,  sizeof (PCH_FLASH_PROTECTION_CONFIG),  FLASH_PROTECTION_CONFIG_REVISION,  LoadFlashProtectionConfigDefault},
  {&gHdAudioConfigGuid,          sizeof (PCH_HDAUDIO_CONFIG),           HDAUDIO_CONFIG_REVISION,           LoadHdAudioConfigDefault},
  {&gInterruptConfigGuid,        sizeof (PCH_INTERRUPT_CONFIG),         INTERRUPT_CONFIG_REVISION,         LoadInterruptConfigDefault},
  {&gIshConfigGuid,              sizeof (PCH_ISH_CONFIG),               ISH_CONFIG_REVISION,               LoadIshConfigDefault},
  {&gLanConfigGuid,              sizeof (PCH_LAN_CONFIG),               LAN_CONFIG_REVISION,               LoadLanConfigDefault},
  {&gLockDownConfigGuid,         sizeof (PCH_LOCK_DOWN_CONFIG),         LOCK_DOWN_CONFIG_REVISION,         LoadLockDownConfigDefault},
  {&gP2sbConfigGuid,             sizeof (PCH_P2SB_CONFIG),              P2SB_CONFIG_REVISION,              LoadP2sbConfigDefault},
  {&gPmConfigGuid,               sizeof (PCH_PM_CONFIG),                PM_CONFIG_REVISION,                LoadPmConfigDefault},
  {&gScsConfigGuid,              sizeof (PCH_SCS_CONFIG),               SCS_CONFIG_REVISION,               LoadScsConfigDefault},
  {&gSerialIoConfigGuid,         sizeof (PCH_SERIAL_IO_CONFIG),         SERIAL_IO_CONFIG_REVISION,         LoadSerialIoConfigDefault},
  {&gSerialIrqConfigGuid,        sizeof (PCH_LPC_SIRQ_CONFIG),          SERIAL_IRQ_CONFIG_REVISION,        LoadSerialIrqConfigDefault},
  {&gThermalConfigGuid,          sizeof (PCH_THERMAL_CONFIG),           THERMAL_CONFIG_REVISION,           LoadThermalConfigDefault},
  {&gUsbConfigGuid,              sizeof (USB_CONFIG),                   USB_CONFIG_REVISION,               LoadUsbConfigDefault},
  {&gEspiConfigGuid,             sizeof (PCH_ESPI_CONFIG),              ESPI_CONFIG_REVISION,              LoadEspiConfigDefault},
  {&gCnviConfigGuid,             sizeof (PCH_CNVI_CONFIG),              CNVI_CONFIG_REVISION,              LoadCnviConfigDefault},
  {&gHsioConfigGuid,             sizeof (PCH_HSIO_CONFIG),              HSIO_CONFIG_REVISION,              LoadHsioConfigDefault},
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
