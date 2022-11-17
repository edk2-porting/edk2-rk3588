/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2021, American Megatrends International LLC.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef   __PCH_RC_CONFIG_DATA_H__
#define   __PCH_RC_CONFIG_DATA_H__

#include <PchLimits.h>
#define HDAUDIO_FEATURES         3
#define HDAUDIO_PP_MODULES       2


/// sSATA max ports for Wellsburg
#define PCH_SSATA_MAX_PORTS      6

#pragma pack(1)
typedef struct {

  UINT8   BiosGuard;
  UINT8   Reserved1; 
  UINT8   Dwr_Enable;
  UINT8   Dwr_Stall;
  UINT8   Dwr_BmcRootPort;

  UINT8   DwrEn_PMCGBL;
  UINT8   DwrEn_CPUTHRM;
  UINT8   DwrEn_PCHTHRM;
  UINT8   DwrEn_PBO;
  UINT8   DwrEn_MEPBO;
  UINT8   DwrEn_MEWDT;
  UINT8   DwrEn_MEGBL;
  UINT8   DwrEn_CTWDT;
  UINT8   DwrEn_PMCWDT;
  UINT8   DwrEn_ME_UERR;
  UINT8   DwrEn_SYSPWR;
  UINT8   DwrEn_OCWDT;
  UINT8   DwrEn_IEPBO;
  UINT8   DwrEn_IEWDT;
  UINT8   DwrEn_IEGBLN;
  UINT8   DwrEn_IE_UERRN;
  UINT8   DwrEn_ACRU_ERR_2H_EN;

  UINT8   DwrPmcEn_HOST_RESET_TIMEOUT;
  UINT8   DwrPmcEn_SX_ENTRY_TIMEOUT;
  UINT8   DwrPmcEn_HOST_RST_PROM;
  UINT8   DwrPmcEn_HSMB_MSG;
  UINT8   DwrPmcEn_IE_MTP_TIMEOUT;
  UINT8   DwrPmcEn_MTP_TIMEOUT;
  UINT8   DwrPmcEn_ESPI_ERROR_DETECT;

  UINT8   Dwr_MeResetPrepDone;
  UINT8   Dwr_IeResetPrepDone;

  //
  // PCH_DEVICE_ENABLES
  //
  UINT8   BoardCapability;
  UINT8   DeepSxMode;
  UINT8   Gp27WakeFromDeepSx;
  UINT8   GbeRegionInvalid;
  UINT8   LomLanSupported;
  UINT8   PchWakeOnLan;
  UINT8   PchSlpLanLowDc;
  UINT8   PchSmbus;
  UINT8   PchPciClockRun;
  UINT8   PchDisplay;
  UINT8   PchCrid;
  UINT8   PchRtcLock;
  UINT8   PchBiosLock;
  UINT8   PchAllUnLock;
  UINT8   PchThermalUnlock;
  UINT8   PchSerm;
  UINT8   PchGbeFlashLockDown;
  UINT8   PchSmmBwp;

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
  UINT8   RouteUsb2PinsToWhichHc;
  UINT8   RouteUsb2Pin0;
  UINT8   RouteUsb2Pin1;
  UINT8   RouteUsb2Pin2;
  UINT8   RouteUsb2Pin3;
  UINT8   RouteUsb2Pin4;
  UINT8   RouteUsb2Pin5;
  UINT8   RouteUsb2Pin6;
  UINT8   RouteUsb2Pin7;
  UINT8   RouteUsb2Pin8;
  UINT8   RouteUsb2Pin9;
  UINT8   RouteUsb2Pin10;
  UINT8   RouteUsb2Pin11;
  UINT8   RouteUsb2Pin12;
  UINT8   RouteUsb2Pin13;
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
  UINT8   SataRaidLoadEfiDriver;
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
  UINT8   sSataPort[PCH_SSATA_MAX_PORTS];
  UINT8   sSataHotPlug[PCH_SSATA_MAX_PORTS];
  UINT8   sSataSpinUp[PCH_SSATA_MAX_PORTS];
  UINT8   sSataExternal[PCH_SSATA_MAX_PORTS];
  UINT8   sPxDevSlp[PCH_SSATA_MAX_PORTS];
  UINT8   sSataType[PCH_SSATA_MAX_PORTS];
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
  UINT8   sSataRaidLoadEfiDriver;
  UINT8   sSataRaidOromDelay;
  UINT8   sSataAlternateId;
  UINT8   sSataSalp;
  UINT8   sSataTestMode;
  UINT8   sEnableDitoConfig[PCH_SSATA_MAX_PORTS];
  UINT8   sDmVal[PCH_SSATA_MAX_PORTS];
  UINT8   sDitoVal[PCH_SSATA_MAX_PORTS];
  UINT8   sSataTopology[PCH_SSATA_MAX_PORTS];  




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
  UINT8   PcieRootPortMSIE[PCH_MAX_PCIE_ROOT_PORTS];
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

  UINT8   RtoHdaVcType;
  //
  // DMI Configuration
  //
  UINT8   TestDmiAspmCtrl;


  //
  //
  // PCIe LTR Configuration
  //
  UINT8   PchPcieLtrEnable[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieLtrConfigLock[PCH_MAX_PCIE_ROOT_PORTS];
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

  //
  // DPTF SETUP items begin
  //
  UINT8   EnableDptf;
  UINT8   EnablePchDevice;

  //
  // CPU
  //
  UINT8   DebugDciEnable;
  UINT8   DebugInterfaceEnable;

  //
  // Miscellaneous options
  //
  UINT8   OsDebugPort;
  UINT8   SlpLanLowDc;
  UINT8   PchLanK1Off;
  UINT8   PchWakeOnWlan;
  UINT8   PchWakeOnWlanDeepSx;
  UINT8   StateAfterG3;
  UINT8   PciePllSsc;
  UINT8   FirmwareConfiguration;
  UINT8   PchDciEn;
  UINT8   PchDciAutoDetect;

  // Acpi.sd
  UINT8   CSNotifyEC;
  UINT8   EcLowPowerMode;

  //
  // TraceHub Setup Options
  //
  UINT8   TraceHubEnableMode;
  UINT8   MemRegion0BufferSize;
  UINT8   MemRegion1BufferSize;

  //
  // PCH P2SB hide and lock options
  //
  UINT8   PchP2sbDevReveal;
  UINT8   PchP2sbUnlock;

  //
  // PCH SPI hide and lock options
  //
  UINT8   FlashLockDown;

  //
  // PCH PMC option
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
  // Miscellaneous options
  //

  UINT8   PchEvaMrom0HookEnable;
  UINT8   PchEvaMrom1HookEnable;
  UINT8   TestMctpBroadcastCycle;
  UINT8   PchEvaLockDown;
  UINT8   PchTraceHubHide;
} PCH_RC_CONFIGURATION;
#pragma pack()

#endif


