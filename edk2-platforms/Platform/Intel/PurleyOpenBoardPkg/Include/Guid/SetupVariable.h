/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2021, American Megatrends International LLC.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __SETUP_VARIABLE_H__
#define __SETUP_VARIABLE_H__

#include "UncoreCommonIncludes.h"
// ---------------------------------------------------------------------------
//
// Driver Configuration
//
// ---------------------------------------------------------------------------
//

#define MAX_PCH_PCI_EXPRESS_ROOT_PORTS  8
#define PASSWORD_MAX_SIZE              16
#define SHA256_DIGEST_LENGTH           32
#define EFI_VARIABLE_BOOTSERVICE_ACCESS 0x00000002

#pragma pack(1)

typedef struct {

  UINT8   UserPassword[SHA256_DIGEST_LENGTH];
  UINT8   AdminPassword[SHA256_DIGEST_LENGTH];
  UINT8   Access;

  //
  // Keyboard
  //
  UINT8   Numlock;
  UINT8   Ps2PortSwap;

  //
  // TPM
  //
  UINT8         TpmEnable;
  UINT8         TpmState;
  UINT8         MorState;

  //
  // XmlCli
  //
  UINT8 XmlCliSupport;
  UINT8 SkipXmlComprs;
  UINT8 PublishSetupPgPtr;

  //
  // Breakpoints
  //
  UINT8   ValidationBreakpointType;
  UINT16   bsdBreakpoint;

  //
  // Power State
  //
  UINT8   PowerState;

  //
  // Wake On Lan
  //
  UINT8   WakeOnLanS5;

  //
  // Boot from Network
  //
  UINT8   BootNetwork;

  //
  // Video
  //
  UINT8   VideoSelect;
  UINT8   EfiWindowsInt10Workaround;
  UINT8   UefiOptimizedBootToggle;

  //
  // Fan PWM Offset
  //
  UINT8    FanPwmOffset;

  //
  // PCI Minimum Secondary Bus Number
  //
  UINT8   PCIe_MultiSeg_Support;

  //
  UINT8   WakeOnLanSupport;
  //
  // Enable/disable for PCIe LOM by using GPO44/45
  // NOT PCH LAN
  //
  UINT8   LomDisableByGpio;

  UINT8   FpkPortConfig[4];
  UINT8   FpkPortConfigPrev[4];
  UINT8   FpkPortPresent[4];

  // RTC WAKE
  //
  UINT8   WakeOnRTCS4S5;
  UINT8   RTCWakeupTimeHour;
  UINT8   RTCWakeupTimeMinute;
  UINT8   RTCWakeupTimeSecond;
  // PCI_EXPRESS_CONFIG, ROOT PORTS
  //
  // AJW: these cross the line, but depend on Platform Info
  UINT8   PcieClockGating;
  UINT8   PcieDmiAspm;
  UINT8   PcieSBDE;
  UINT8   GbePciePortNum;
  UINT8   PciePortConfig1;
  UINT8   PciePortConfig2;
  UINT8   PciePortConfig3;
  UINT8   PciePortConfig4;
  UINT8   PciePortConfig5;

  // GBE
  UINT8 GbeEnabled;

  // PCH Stepping
  UINT8 PchStepping;

  //
  // XHCI Wake On USB
  //
  UINT8   XhciWakeOnUsbEnabled;

  //
  // EventLog
  //
//
// SKX_TODO: add these for RAS, may be best to find new home for them in a new setup variable and setup page
//
  UINT8   SystemErrorEn;
  //Viral, and IoMca are not supported in EP. Will need to wrap in an EX flag
  //UINT8   ViralEn;
  UINT8   PoisonEn;
  UINT8   ViralEn;
  UINT8   ClearViralStatus;
  UINT8   CloakingEn;
  UINT8   UboxToPcuMcaEn;
  UINT8   FatalErrSpinLoopEn;

  UINT8   EmcaEn;
  UINT8   EmcaIgnOptin;
  UINT8   EmcaCsmiEn;
  UINT8   EmcaMsmiEn;
  UINT8   ElogCorrErrEn;
  UINT8   ElogMemErrEn;
  UINT8   ElogProcErrEn;
  UINT8   LmceEn;

  UINT8   WheaSupportEn;
  UINT8   WheaLogMemoryEn;
  UINT8   WheaLogProcEn;

  UINT8   WheaLogPciEn;

  UINT8   WheaErrorInjSupportEn;
  UINT8   McaBankErrInjEn;
  UINT8   WheaErrInjEn;
  UINT8   WheaPcieErrInjEn;
  UINT8   MeSegErrorInjEn;
  UINT8   PcieErrInjActionTable;
  UINT8   ParityCheckEn;

  UINT8   MemErrEn;
  UINT8   CorrMemErrEn;
  UINT32  LeakyBktHiLeakyBktLo;
  UINT8   SpareIntSelect;
  UINT8   FnvErrorEn;
  UINT8   FnvErrorLowPrioritySignal;  // 0 - No Log, 1 - SMI, 2 - ERR0#, 3 - BOTH
  UINT8   FnvErrorHighPrioritySignal; // 0 - No Log, 1 - SMI, 2 - ERR0#, 3 - BOTH
  UINT8   Reserved_1;
  UINT8   Reserved_2;
  UINT8   Reserved_3;

  UINT8   IioErrorEn;
  UINT8   IoMcaEn;
  UINT8   IioErrRegistersClearEn;
  UINT8   IioErrorPinEn;
  UINT8   LerEn;
  UINT8   DisableMAerrorLoggingDueToLER;
  UINT8   IioIrpErrorEn;
  UINT8   IioMiscErrorEn;
  UINT8   IioVtdErrorEn;
  UINT8   IioDmaErrorEn;
  UINT8   IioDmiErrorEn;
  UINT8   IioPcieAddCorrErrorEn;
  UINT8   IioPcieAddUnCorrEn;
  UINT8   IioPcieAerSpecCompEn;

  UINT8   PcieErrEn;
  UINT8   PcieCorrErrEn;
  UINT8   PcieUncorrErrEn;
  UINT8   PcieFatalErrEn;
  UINT8   PcieCorErrCntr;
  UINT8   PcieCorErrMaskBitMap;
  UINT16  PcieCorErrThres;
  UINT8   PcieAerCorrErrEn;
  UINT8   PcieAerAdNfatErrEn;
  UINT8   PcieAerNfatErrEn;
  UINT8   PcieAerFatErrEn;
  UINT8   SerrPropEn;
  UINT8   PerrPropEn;
  UINT8   OsSigOnSerrEn;
  UINT8   OsSigOnPerrEn;

  UINT8   CaterrGpioSmiEn;

// Endof RAS add
  //Viral, and IoMca are not supported in EP. Will need to wrap in an EX flag
  //UINT8   IoMcaEn;

  UINT8   McBankWarmBootClearError;
  UINT8   KTIFailoverSmiEn;

  UINT8   irpp0_parityError;
  UINT8   irpp0_qtOverflow;
  UINT8   irpp0_unexprsp;
  UINT8   irpp0_csraccunaligned;
  UINT8   irpp0_unceccCs1;
  UINT8   irpp0_unceccCs0;
  UINT8   irpp0_rcvdpoison;
  UINT8   irpp0_crreccCs1;
  UINT8   irpp0_crreccCs0;

  UINT8   PropagateSerr;
  UINT8   PropagatePerr;

  //
  // Boot Options
  //
  UINT8   serialDebugMsgLvl;
  UINT8   serialDebugTrace;
  UINT8   serialDebugMsgLvlTrainResults;
  UINT8   ResetOnMemMapChange;
  UINT8   ForceSetup;
  UINT8   BiosGuardEnabled;
  UINT8   BiosGuardPlatformSupported;
  UINT8   EnableAntiFlashWearout;
  UINT8   AntiFlashWearoutSupported;
  UINT8   RtoPopulateBGDirectory;

  UINT8   Use1GPageTable;
  //
  // UINT8   QuietBoot;
  //
  UINT8   FastBoot;

  //
  // Reserve Memory that is hidden from the OS.
  //
  UINT8   ReserveMem;
  UINT64  ReserveStartAddr;

  //
  // Reserve TAGEC Memory
  //
  UINT8  TagecMem;

  //Usb Configdata
  UINT8   UsbMassDevNum;
  UINT8   UsbLegacySupport;
  UINT8   UsbEmul6064;
  UINT8   UsbMassResetDelay;
  UINT8   UsbNonBoot;
  UINT8   UsbEmu1;
  UINT8   UsbEmu2;
  UINT8   UsbEmu3;
  UINT8   UsbEmu4;
  UINT8   UsbEmu5;
  UINT8   UsbEmu6;
  UINT8   UsbEmu7;
  UINT8   UsbEmu8;
  UINT8   UsbEmu9;
  UINT8   UsbEmu10;
  UINT8   UsbEmu11;
  UINT8   UsbEmu12;
  UINT8   UsbEmu13;
  UINT8   UsbEmu14;
  UINT8   UsbEmu15;
  UINT8   UsbEmu16;
  UINT8   UsbStackSupport;

  // Console Redirection
  UINT8   ConsoleRedirection;
  UINT8   FlowControl;
  UINT64  BaudRate;
  UINT8   TerminalType;
  UINT8   LegacyOsRedirection;
  UINT8   TerminalResolution;
  UINT8   DataBits;
  UINT8   Parity;
  UINT8   StopBits;

#ifdef EFI_PCI_IOV_SUPPORT
  UINT8   SystemPageSize;
  UINT8   ARIEnable;
  UINT8   ARIForward;
  UINT8   SRIOVEnable;
  UINT8   MRIOVEnable;
#endif
  //
  // RAS
  //

//
// Network setup entries - start here <><><><><>
//
  UINT8  LegacyPxeRom;
  UINT8  EfiNetworkSupport;
//
// Network setup entries - end here <><><><><>
//

//
// SERIALPORT BAUD RATE: Begin
//
  UINT32        SerialBaudRate;
//
// SERIALPORT BAUD RATE: END
//

  UINT8         BootAllOptions;
  UINT8         SetShellFirst;

  //
  // Overclocking related setup variables
  //
  UINT8  PlatformOCSupport;
  UINT8  FilterPll;
  UINT8  OverclockingSupport;

  UINT8  CoreMaxOcRatio;
  UINT8  CoreVoltageMode;
  UINT16 CoreVoltageOverride;
  UINT16 CoreVoltageOffset;
  UINT8  CoreVoltageOffsetPrefix;
  UINT16 CoreExtraTurboVoltage;

  //
  // OC related
  //
  UINT8  MemoryVoltage;
  UINT8  MemoryVoltageDefault;
  UINT8  tCL;

  //
  // CLR Related
  //
  UINT8   ClrMaxOcRatio;
  UINT8   ClrVoltageMode;
  UINT16  ClrVoltageOverride;
  UINT16  ClrVoltageOffset;
  UINT8   ClrVoltageOffsetPrefix;
  UINT16  ClrExtraTurboVoltage;

  //
  // Uncore Related
  //
  UINT16   UncoreVoltageOffset;
  UINT8    UncoreVoltageOffsetPrefix;
  UINT16   IoaVoltageOffset;
  UINT8    IoaVoltageOffsetPrefix;
  UINT16   IodVoltageOffset;
  UINT8    IodVoltageOffsetPrefix;

  //
  //  SVID and FIVR Related
  //
  UINT8   SvidEnable;
  UINT16  SvidVoltageOverride;
  UINT8   FivrFaultsEnable;
  UINT8   FivrEfficiencyEnable;

  UINT8 SataInterfaceRAIDMode;
  UINT8 sSataInterfaceRAIDMode;

  UINT16  C01MemoryVoltage;
  UINT16  C23MemoryVoltage;

  UINT16  CpuVccInVoltage;

  UINT8   VccIoVoltage;

  UINT8   CloudProfile;
  UINT16  VariablePlatId;

  //XTU 3.0

  UINT8 FlexRatioOverrideDefault;
  UINT8 RatioLimit1Default;
  UINT8 RatioLimit2Default;
  UINT8 RatioLimit3Default;
  UINT8 RatioLimit4Default;
  UINT8 OverclockingLockDefault;
  UINT8 DdrRefClkDefault;
  UINT8 DdrRatioDefault;
  UINT8 tCLDefault;
  UINT8 tCWLDefault;
  UINT16 tFAWDefault;
  UINT16 tRASDefault;
  UINT16 tRCDefault;
  UINT8 tRCDDefault;
  UINT16 tREFIDefault;
  UINT16 tRFCDefault;
  UINT8 tRPDefault;
  UINT8 tRPabDefault;
  UINT8 tRRDDefault;
  UINT8 tRTPDefault;
  UINT8 tWRDefault;
  UINT8 tWTRDefault;
  UINT8 NModeDefault;
  UINT8 CoreMaxOcRatioDefault;
  UINT8 CoreVoltageModeDefault;
  UINT16 CoreVoltageOverrideDefault;
  UINT16 CoreVoltageOffsetDefault;
  UINT8 CoreVoltageOffsetPrefixDefault;
  UINT16 CoreExtraTurboVoltageDefault;
  UINT8 GtOcSupportDefault;
  UINT8 GtOcFrequencyDefault;
  UINT16 GtExtraTurboVoltageDefault;
  UINT16 GtOcVoltageDefault;
  UINT8 GtVoltageModeDefault;
  UINT16 GtVoltageOverrideDefault;
  UINT16 GtVoltageOffsetDefault;
  UINT8 GtVoltageOffsetPrefixDefault;
  UINT8 ClrMaxOcRatioDefault;
  UINT8 ClrVoltageModeDefault;
  UINT16 ClrVoltageOverrideDefault;
  UINT16 ClrVoltageOffsetDefault;
  UINT8 ClrVoltageOffsetPrefixDefault;
  UINT16 ClrExtraTurboVoltageDefault;
  UINT16 UncoreVoltageOffsetDefault;
  UINT8 UncoreVoltageOffsetPrefixDefault;
  UINT16 IoaVoltageOffsetDefault;
  UINT8 IoaVoltageOffsetPrefixDefault;
  UINT16 IodVoltageOffsetDefault;
  UINT8 IodVoltageOffsetPrefixDefault;
  UINT8 SvidEnableDefault;
  UINT16 SvidVoltageOverrideDefault;
  UINT8 FivrFaultsEnableDefault;
  UINT8 FivrEfficiencyEnableDefault;
  UINT16 VrCurrentLimitDefault;
  UINT8 EnableGvDefault;
  UINT8 TurboModeDefault;
  UINT8 PowerLimit1TimeDefault;
  UINT16 PowerLimit1Default;
  UINT16 PowerLimit2Default;


  UINT8 RatioLimit1; //ratiolimit handling has changed in SKX. knobs might need to change too. Will have to revisit again.
  UINT8 RatioLimit2;
  UINT8 RatioLimit3;
  UINT8 RatioLimit4;
  UINT8 CpuRatio; // need to understand what is the difference between maxnonturboratio and cpuratio. if cpuratiooverride is 0, then cpuratio is same as maxnonturboratio. add this to platform cpu policy or socketsetup.
  UINT8 CpuRatioOverride;
  UINT8 IsTurboRatioDefaultsInitalized; // related to initializing all the vardefault. is this flow needed for HEDT/intended only for clients? no need for set up creation.


  UINT8 DdrRefClk; //cant find any in purley. new one?
  UINT8 PcieRatioDisabled;//need to check if this is applicable to HEDT. also no need to create a setup variable.
  UINT8 NMode ;
  UINT8 Pmtt;

  UINT16 GtVoltageOffset; //existing but no set up option
  UINT16 VrCurrentLimit;//done
  //UINT8 SpdProfileSelected; same as XMPMode
  UINT8 NModeSupport;
  UINT8 WDTSupportforNextOSBoot; // no setup option needed
  UINT16 TimeforNextOSBoot; // no setup optiom needed
  UINT8 PlatformUnstable; // no set up option needed. this decides if all the vardefaults are needed.
  UINT8 GtVoltageMode; //existing but no set up option
  UINT8 DdrRatio;
  UINT8 GtOcFrequency;
  UINT16 GtExtraTurboVoltage; //existing but no set up option
  UINT16 GtVoltageOverride; //existing but no set up option
  UINT8 GtVoltageOffsetPrefix;
  UINT8 GtOcSupport;
  //
  // CPU releated
  //
  UINT8 FlexOverrideEnable;
  UINT8 FlexRatioOverride;
  UINT8 PowerLimit3Override;
  UINT32 PowerLimit3;
  UINT8 PowerLimit3Time;
  UINT8 PowerLimit3DutyCycle;
  UINT8 PowerLimit3Lock;
  UINT8 MemoryVoltageOverride;

  //
  // ICC Related
  //
  UINT8 BClkOverride;
  UINT8 BclkAdjustable;
  UINT8 DmiPegRatio;
  UINT8 DfxAdvDebugJumper;
  UINT8 DfxAltPostCode;

  //
  // Validation Related
  //
  UINT8   ValidationResetType;
  UINT16  ValidationCountOuter;
  UINT16  ValidationCountInner;
  UINT8   ValidationStopOnError;
  UINT8   ValidationBootWhenDone;
  UINT8   ValidationSkxPciError;
  UINT8   ValidationSkxPciLinkError;
  UINT8   ValidationPchPciError;
  UINT8   ValidationSkxPciLinkRecoveryCountError;
  UINT16  ValidationSkxPciLinkRecoveryCountThreshold;
  UINT8   ValidationKtiError;

  UINT8 TraceHubDebugInterface;
  UINT8 RamDebugInterface;
  UINT8   StorageOpROMSuppression;
//
// PC_SIO_END
//
  UINT8   RsaSupport;
  UINT8   FnvErrorMailbox;             // 0 - DDRT, 1 - SMBUS
  UINT8   ReportAlertSPA;              // Include SPA when reporting DDRT alert. Only to disable for MCE recovery test.
  UINT8   AEPErrorInjEn;
  UINT8   NgnHostAlertPatrolScrubUNC;  // Signal DDRT interrupt upon receiving Uncorrectable Error for NGN Patrol Scrub
  UINT8   DcpmmUncPoison;
  UINT8   UCErrChkForVariableSrv;

} SYSTEM_CONFIGURATION;

#pragma pack()

#define EFI_HDD_PRESENT       0x01
#define EFI_HDD_NOT_PRESENT   0x00
#define EFI_CD_PRESENT        0x02
#define EFI_CD_NOT_PRESENT    0x00

#define EFI_HDD_WARNING_ON    0x01
#define EFI_CD_WARNING_ON     0x02
#define EFI_SMART_WARNING_ON  0x04
#define EFI_HDD_WARNING_OFF   0x00
#define EFI_CD_WARNING_OFF    0x00
#define EFI_SMART_WARNING_OFF 0x00

#endif // #ifndef _SETUP_VARIABLE
