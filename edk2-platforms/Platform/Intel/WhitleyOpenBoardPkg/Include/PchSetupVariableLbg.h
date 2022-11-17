/** @file
  Data format for Universal Data Structure

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef   _SETUP_VARIABLE_LBG_H_
#define   _SETUP_VARIABLE_LBG_H_

// for PCH_RC_CONFIGURATION
extern EFI_GUID gEfiPchSetupGuid;
#define PCH_SETUP_NAME L"PchSetup"


#include <PchLimits.h>
#define HDAUDIO_FEATURES         3
#define HDAUDIO_PP_MODULES       2

#define SATA_MODE_AHCI                    0
#define SATA_MODE_RAID                    1
#define SATA_TEST_MODE_ENABLE             1


/// sSATA max ports for Wellsburg
#define PCH_MAX_SSATA_PORTS      6

#pragma pack(1)
typedef struct {
  UINT8   Dwr_Enable;
  UINT8   Dwr_Stall;
  UINT8   Dwr_BmcRootPort;

  UINT8   DwrEn_PMCGBL;
  UINT8   DwrEn_MEWDT;
  UINT8   DwrEn_IEWDT;

  UINT8   Dwr_MeResetPrepDone;
  UINT8   Dwr_IeResetPrepDone;

  //
  // PCH_DEVICE_ENABLES
  //
  UINT8   DeepSxMode;
  UINT8   Gp27WakeFromDeepSx;
  UINT8   GbeRegionInvalid;
  UINT8   LomLanSupported;
  UINT8   PchWakeOnLan;
  UINT8   PchSlpLanLowDc;
  UINT8   PchCrid;
  UINT8   PchRtcLock;
  UINT8   PchBiosLock;
  UINT8   PchAllUnLock;
  UINT8   PchThermalUnlock;
  UINT8   PchSerm;

  UINT8   Hpet;
  UINT8   PchPort80Route;
  UINT8   EnableClockSpreadSpec;
  UINT8   IchPort80Route;
  UINT8   PchSirqMode;

  //
  // Usb Config
  //
  UINT8   PchUsbManualMode;
  UINT8   PchGpioLockDown;
  UINT8   Usb3PinsTermination;
  UINT8   EnableUsb3Pin[10];
  UINT8   PchUsbHsPort[16];
  UINT8   PchUsbSsPort[10];
  UINT8   PchUsbPortDisable;
  UINT8   UsbSensorHub;
  UINT8   UsbSsicSupport[2];
  UINT8   XhciDisMSICapability;
  UINT8   PchUsbPerPortCtl;
  UINT8   PchUsb30Port[6];
  UINT8   UsbPrecondition;
  UINT8   XhciIdleL1;
  UINT8   Btcg;
  UINT8   PchUsbDegradeBar;
  //
  // XHCI OC Map
  //
  UINT8   XhciOcMapEnabled;
  //
  // xDCI Config
  //
  UINT8   PchXdciSupport;
  //
  // Sata CONFIG
  //
  UINT8   PchSata;
  //
  // Sata Interface Mode
  // 0 - IDE  1 - RAID  2 - AHCI
  //
  UINT8   SataInterfaceMode;
  UINT8   SataPort[PCH_MAX_SATA_PORTS];
  UINT8   SataHotPlug[PCH_MAX_SATA_PORTS];
  UINT8   SataMechanicalSw[PCH_MAX_SATA_PORTS];
  UINT8   SataSpinUp[PCH_MAX_SATA_PORTS];
  UINT8   SataExternal[PCH_MAX_SATA_PORTS];
  UINT8   SataType[PCH_MAX_SATA_PORTS];
  UINT8   SataRaidR0;
  UINT8   SataRaidR1;
  UINT8   SataRaidR10;
  UINT8   SataRaidR5;
  UINT8   SataRaidIrrt;
  UINT8   SataRaidOub;
  UINT8   SataHddlk;
  UINT8   SataLedl;
  UINT8   SataRaidIooe;
  UINT8   SataRaidSrt;
  UINT8   SataRaidLoadEfiDriver[PCH_MAX_SATA_CONTROLLERS];
  UINT8   SataRaidOromDelay;
  UINT8   SataAlternateId;
  UINT8   SataSalp;
  UINT8   SataTestMode;
  UINT8   PxDevSlp[PCH_MAX_SATA_PORTS];
  UINT8   EnableDitoConfig[PCH_MAX_SATA_PORTS];
  UINT16  DitoVal[PCH_MAX_SATA_PORTS];
  UINT8   DmVal[PCH_MAX_SATA_PORTS];
  UINT8   SataTopology[PCH_MAX_SATA_PORTS];

  //
  // sSata CONFIG
  //
  UINT8   PchsSata;
  //
  // Sata Interface Mode
  // 0 - IDE  1 - RAID  2 - AHCI
  //
  UINT8   sSataInterfaceMode;
  UINT8   sSataPort[PCH_MAX_SSATA_PORTS];
  UINT8   sSataHotPlug[PCH_MAX_SSATA_PORTS];
  UINT8   sSataSpinUp[PCH_MAX_SSATA_PORTS];
  UINT8   sSataExternal[PCH_MAX_SSATA_PORTS];
  UINT8   sPxDevSlp[PCH_MAX_SSATA_PORTS];
  UINT8   sSataType[PCH_MAX_SSATA_PORTS];
  UINT8   sSataRaidR0;
  UINT8   sSataRaidR1;
  UINT8   sSataRaidR10;
  UINT8   sSataRaidR5;
  UINT8   sSataRaidIrrt;
  UINT8   sSataRaidOub;
  UINT8   sSataHddlk;
  UINT8   sSataLedl;
  UINT8   sSataRaidIooe;
  UINT8   sSataRaidSrt;
  UINT8   sSataRaidOromDelay;
  UINT8   sSataAlternateId;
  UINT8   sSataSalp;
  UINT8   sSataTestMode;
  UINT8   sEnableDitoConfig[PCH_MAX_SSATA_PORTS];
  UINT8   sDmVal[PCH_MAX_SSATA_PORTS];
  UINT8   sDitoVal[PCH_MAX_SSATA_PORTS];
  UINT8   sSataTopology[PCH_MAX_SSATA_PORTS];

  //PCH THERMAL SENSOR
  UINT8   ThermalDeviceEnable;
  UINT8   PchCrossThrottling;

  UINT8   PchDmiExtSync;
  UINT8   PcieDmiExtSync;
  // AcpiDebug Setup Options
  UINT8   PciDelayOptimizationEcr;
  UINT8   PchPcieGlobalAspm;

  UINT8   PcieDmiStopAndScreamEnable;
  UINT8   DmiLinkDownHangBypass;
  UINT8   XTpmLen;
  UINT8   PcieRootPort8xhDecode;
  UINT8   Pcie8xhDecodePortIndex;
  UINT8   PcieRootPortPeerMemoryWriteEnable;
  UINT8   PcieComplianceTestMode;


  UINT8   PcieRootPortSBDE;
  UINT8   PcieSBDEPort;

  UINT8   RstPcieStorageRemap[PCH_MAX_RST_PCIE_STORAGE_CR];
  UINT8   RstPcieStorageRemapPort[PCH_MAX_RST_PCIE_STORAGE_CR];
  UINT8   PcieRootPortFunctionSwapping;
  UINT8   PcieRootPortEn[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortAspm[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortURE[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortFEE[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortNFE[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortCEE[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortMaxPayLoadSize[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortAER[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieTopology[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   PcieLaneCm[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieLaneCp[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   PcieSwEqOverride;
  UINT8   PcieSwEqCoeffCm[PCH_PCIE_SWEQ_COEFFS_MAX];
  UINT8   PcieSwEqCoeffCp[PCH_PCIE_SWEQ_COEFFS_MAX];
  UINT8   PchPcieUX8MaxPayloadSize;
  UINT8   PchPcieUX16MaxPayloadSize;
  UINT8   PcieRootPortCompletionTimeout[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieClockGatingDisabled;
  UINT8   PcieUsbGlitchWa;
  UINT8   PcieRootPortPIE[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortACS[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortEqPh3Method[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortMaxReadRequestSize;
  UINT8   PcieRootPortSFE[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortSNE[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortSCE[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortPMCE[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortHPE[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortSpeed[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortTHS[PCH_MAX_PCIE_ROOT_PORTS];

  //
  // PCI Bridge Resources
  //
  UINT8   PcieRootPortL1SubStates[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   MemoryThermalManagement;
  UINT8   ExttsViaTsOnBoard;
  UINT8   ExttsViaTsOnDimm;
  UINT8   FixupPlatformSpecificSoftstraps;

  //
  // SMBUS Configuration
  //
  UINT8   TestSmbusSpdWriteDisable;

  //
  // HD-Audio Configuration
  //
  UINT8   PchHdAudio;
  UINT8   PchHdAudioDsp;
  UINT8   PchHdAudioPme;
  UINT8   PchHdAudioIoBufferOwnership;
  UINT8   PchHdAudioIoBufferVoltage;
  UINT8   PchHdAudioCodecSelect;
  UINT8   PchHdAudioFeature[HDAUDIO_FEATURES];
  UINT8   PchHdAudioPostProcessingMod[HDAUDIO_PP_MODULES];

  UINT8   DfxHdaVcType;
  //
  // DMI Configuration
  //
  UINT8   TestDmiAspmCtrl;


  //
  //
  // PCIe LTR Configuration
  //
  UINT8   PchPcieLtrEnable[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieSnoopLatencyOverrideMode[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieSnoopLatencyOverrideMultiplier[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieNonSnoopLatencyOverrideMode[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieNonSnoopLatencyOverrideMultiplier[PCH_MAX_PCIE_ROOT_PORTS];
  UINT16  PchPcieSnoopLatencyOverrideValue[PCH_MAX_PCIE_ROOT_PORTS];
  UINT16  PchPcieNonSnoopLatencyOverrideValue[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   PchPcieForceLtrOverride[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchSataLtrOverride;
  UINT8   PchSataLtrEnable;
  UINT16  PchSataSnoopLatencyOverrideValue;
  UINT8   PchSataSnoopLatencyOverrideMultiplier;
  UINT8   PchSataLtrConfigLock;

  UINT8   PchSSataLtrOverride;
  UINT16  PchSSataSnoopLatencyOverrideValue;
  UINT8   PchSSataSnoopLatencyOverrideMultiplier;
  UINT8   PchSSataLtrEnable;
  UINT8   PchSSataLtrConfigLock;

  UINT8   PchPcieUX16CompletionTimeout;
  UINT8   PchPcieUX8CompletionTimeout;

  //
  // Interrupt Configuration
  //
  UINT8   PchIoApic24119Entries;
  UINT8   ShutdownPolicySelect;

  //
  // DPTF SETUP items begin
  //
  UINT8   EnableDptf;
  UINT8   EnablePchDevice;

  //
  // Miscellaneous options
  //
  UINT8   SlpLanLowDc;
  UINT8   PchLanK1Off;
  UINT8   PchWakeOnWlan;
  UINT8   PchWakeOnWlanDeepSx;
  UINT8   StateAfterG3;
  UINT8   PciePllSsc;
  UINT8   FirmwareConfiguration;
  UINT8   DciEn;
  UINT8   PchDciAutoDetect;

  // Acpi.sd
  UINT8   CSNotifyEC;
  UINT8   EcLowPowerMode;

  //
  // TraceHub Setup Options
  //
  UINT8   PchTraceHubMode;
  UINT32  PchTraceHubMemReg0Size;
  UINT32  PchTraceHubMemReg1Size;
  UINT8   AetEnableMode;

  //
  // PCH P2SB hide and lock options
  //
  UINT8   PchP2sbDevReveal;
  UINT8   PchP2sbUnlock;

  //
  // PCH SPI hide and lock options
  //
  UINT8   ShowSpiController;
  UINT8   FlashLockDown;

  //
  // PCH PMC DFX options
  //
  UINT8   PmcReadDisable;


  //
  // ADR Configuration
  //
  UINT8   PchAdrEn;
  UINT8   AdrTimerEn;
  UINT8   AdrTimerVal;
  UINT8   AdrMultiplierVal;
  UINT8   AdrGpioSel;
  UINT8   AdrHostPartitionReset;
  UINT8   AdrSysPwrOk;
  UINT8   AdrOverClockingWdt;
  UINT8   AdrCpuThermalWdt;
  UINT8   AdrPmcParityError;

  //
  // Audio DSP Configuration
  //
  UINT8   PchAudioDsp;
  UINT8   PchAudioDspD3PowerGating;
  UINT8   PchAudioDspAcpiMode;
  UINT8   PchAudioDspBluetooth;
  UINT8   PchAudioDspAcpiInterruptMode;

  //
  // DFX Configuration
  //
  UINT8   PchEvaMrom0HookEnable;
  UINT8   PchEvaMrom1HookEnable;
  UINT8   TestMctpBroadcastCycle;
  UINT8   PchEvaLockDown;
  UINT8   PchTraceHubHide;
} PCH_SETUP;
#pragma pack()

#endif


