/** @file

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __SETUP_VARIABLE_H__
#define __SETUP_VARIABLE_H__

// ---------------------------------------------------------------------------
//
// Driver Configuration
//
// ---------------------------------------------------------------------------
//
//Copied from client SetupVariable.h
#ifndef PLATFORM_SETUP_VARIABLE_NAME
#define PLATFORM_SETUP_VARIABLE_NAME             L"Setup"
#endif

#ifndef PCH_SETUP_VARIABLE
#define PCH_SETUP_VARIABLE_NAME             L"PchSetup"
#endif

#define EFI_EPG_GENERIC_VARIABLE_GUID \
  { \
    0x8302cc54, 0xbb1a, 0x4564, {0x92, 0xdc, 0xae, 0x1a, 0xbb, 0x15, 0x5f, 0x35} \
  }

//
// {EC87D643-EBA4-4bb5-A1E5-3F3E36B20DA9}
//
#define SYSTEM_CONFIGURATION_GUID \
  { \
    0xec87d643, 0xeba4, 0x4bb5, {0xa1, 0xe5, 0x3f, 0x3e, 0x36, 0xb2, 0xd, 0xa9} \
  }

#define PCH_SETUP_GUID \
 { \
  0x4570b7f1, 0xade8, 0x4943, {0x8d, 0xc3, 0x40, 0x64, 0x72, 0x84, 0x23, 0x84} \
 }

#define FAKE_VARSTORE_GUID \
  { \
    0xe57d5cb9, 0x148f, 0x444d, {0x9e, 0xcb, 0x9d, 0xf8, 0x65, 0x30, 0xa0, 0xb4} \
  }

#define SYSTEM_CONFIGURATION_CONTROL_GUID \
  { \
    0x8d247131, 0x385e, 0x491f, {0xba, 0x68, 0x8d, 0xe9, 0x55, 0x30, 0xb3, 0xa6} \
  }

#define SYSTEM_CONFIGURATION_ICHPCIE_GUID \
  { \
    0x10e023a7, 0x4ce5, 0x4a6a, {0xa1, 0xbb, 0xbd, 0xd0, 0x8d, 0x16, 0x37, 0x57} \
  }

#define SYSTEM_DEFAULT_CONFIGURATION_GUID \
  { \
    0x99a96812, 0x4730, 0x4290, {0x8b, 0xfe, 0x7b, 0x4e, 0x51, 0x4f, 0xf9, 0x3b} \
  }

#define RESERVEMEMFLAG_VARIABLE_GUID \
{\
   0xb87aa73f, 0xdcb3, 0x4533, {0x83, 0x98, 0x6c, 0x12, 0x84, 0x27, 0x28, 0x40} \
}

#define BMC_ENABLE_FLAG_GUID \
{\
   0x7bb08ce4, 0x6988, 0x4c59, {0xb5, 0x37, 0xb4, 0xb1, 0xd5, 0xbe, 0xb0, 0x6e} \
}

#define MAX_PCH_PCI_EXPRESS_ROOT_PORTS  8
#define PASSWORD_MAX_SIZE              16
#define SHA256_DIGEST_LENGTH           32
#define EFI_VARIABLE_BOOTSERVICE_ACCESS 0x00000002

#define FAKE_VARSTORE_ID    0x1234
#define ME_SETUP_STORAGE_ID 0x1235
#define ICC_SETUP_ID        0x1236

extern EFI_GUID gReserveMemFlagVariableGuid;

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
  UINT32  XmlCLiDramCmosAddr;

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
  // Benchmark
  //
  UINT8   ApplicationProfile;

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
  UINT8   PcieClockGatingDisabled ;
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
  UINT8   RasLogLevel;
  UINT8   PoisonEn;
  UINT8   ViralEn;
  UINT8   CloakDevHideRegistersOs;
  UINT8   ClearViralStatus;
  UINT8   CloakingEn;
  UINT8   UboxToPcuMcaEn;
  UINT8   FatalErrSpinLoopEn;

  UINT8   EmcaEn;
  UINT8   EmcaIgnOptin;
  UINT8   EmcaCsmiEn;
  UINT16  EmcaCsmiThreshold;
  UINT8   CsmiDynamicDisable;
  UINT8   EmcaMsmiEn;
  UINT8   ElogCorrErrEn;
  UINT8   ElogMemErrEn;
  UINT8   ElogProcErrEn;
  UINT8   LmceEn;
  UINT8   HideWriteDataParityLogs;
  UINT8   UboxErrorMask;

  UINT8   WheaSupportEn;
  UINT8   WheaLogMemoryEn;
  UINT8   WheaLogProcEn;

  UINT8   WheaLogPciEn;
  UINT8   AEPErrorInjEn;
  UINT8   WheaErrorInjSupportEn;
  UINT8   McaBankErrInjEn;
  UINT8   WheaErrInjEn;
  UINT8   WheaPcieErrInjEn;
  UINT8   MeSegErrorInjEn;
  UINT8   SgxErrorInjEn;
  UINT8   PcieErrInjActionTable;

  UINT8   MemErrEn;
  UINT8   CorrMemErrEn;
  UINT8   SpareIntSelect;
  UINT8   PfdEn;
  UINT8   DcpmmEccModeSwitch;
  UINT8   FnvErrorEn;
  UINT8   FnvErrorLowPrioritySignal;  // 0 - No Log, 1 - SMI, 2 - ERR0#, 3 - BOTH
  UINT8   FnvErrorHighPrioritySignal; // 0 - No Log, 1 - SMI, 2 - ERR0#, 3 - BOTH
  UINT8   NgnAddressRangeScrub;
  UINT8   NgnHostAlertDpa;        //Signal Poison or Viral upon receiving DIMM Physical Address Error
  UINT8   NgnHostAlertPatrolScrubUNC;  //Signal DDRT interrupt upon receiving Uncorrectable Error for NGN Patrol Scrub
  UINT8   ReportAlertSPA;              //Include SPA when reporting DDRT alert. Only to disable for MCE recovery test.
  UINT8   DcpmmUncPoison;              // Poison media location for Uncorrectable error on Read (via patrol scrubber..)
  UINT8   DdrtInternalAlertEn;

  UINT8   IioErrorEn;
  UINT8   OsNativeAerSupport;
  UINT8   IoMcaEn;
  UINT8   IioSev1Pcc;
  UINT8   IioErrRegistersClearEn;
  UINT8   IioErrorPin0En;
  UINT8   IioErrorPin1En;
  UINT8   IioErrorPin2En;
  UINT8   LerEn;
  UINT8   DisableMAerrorLoggingDueToLER;
  UINT8   EdpcEn;
  UINT8   EdpcInterrupt;
  UINT8   EdpcErrCorMsg;
  UINT8   PciePtlpEgrBlk;

  UINT8   IioIrpErrorEn;
  UINT8   IioMiscErrorEn;
  UINT8   IioVtdErrorEn;
  UINT8   IioDmaErrorEn;
  UINT8   IioDmiErrorEn;
  UINT8   IioPcieAddCorrErrorEn;
  UINT8   IioPcieAddUnCorrEn;
  UINT8   IioPcieAddRcvComWithUr;
  UINT8   IioPcieAerSpecCompEn;
  UINT8   ItcOtcCaMaEnable;
  UINT8   PsfUrEnable;
  UINT8   PmsbRouterParityErrEn;
  UINT8   PcieErrEn;
  UINT8   PcieCorrErrEn;
  UINT8   PcieUncorrErrEn;
  UINT8   PcieFatalErrEn;
  UINT8   PcieCorErrCntr;
  UINT16  PcieCorErrThres;
  UINT8   PcieCorErrLimitEn;
  UINT32  PcieCorErrLimit;
  UINT8   PcieAerCorrErrEn;
  UINT8   PcieAerAdNfatErrEn;
  UINT8   PcieAerNfatErrEn;
  UINT8   PcieAerFatErrEn;
  UINT8   PcieAerEcrcEn;
  UINT8   PcieAerSurpriseLinkDownEn;
  UINT8   PcieAerUreEn;
  UINT8   McaSpinLoop;
  UINT8   IioOOBMode;

// Endof RAS add
  //Viral, and IoMca are not supported in EP. Will need to wrap in an EX flag

  UINT8   McBankWarmBootClearError;
  UINT8   ShutdownSuppression;
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
  UINT8   ReservedS1;

  UINT8   Use1GPageTable;
  //
  // UINT8   QuietBoot;
  //
  UINT8   FastBoot;

  // PFR {
  UINT8   PfrSupported;
  UINT8   PfrCpldRotReleaseVersion;
  UINT8   PfrCpldRotSvn;
  UINT8   PfrPchPfrActiveSvn;
  UINT8   PfrPchPfmActiveMajorVersion;
  UINT8   PfrPchPfmActiveMinorVersion;
  UINT8   PfrBmcPfrActiveSvn;
  UINT8   PfrBmcPfmActiveMajorVersion;
  UINT8   PfrBmcPfmActiveMinorVersion;
  UINT8   PfrPchPfrRecoverySvn;
  UINT8   PfrPchPfmRecoveryMajorVersion;
  UINT8   PfrPchPfmRecoveryMinorVersion;
  UINT8   PfrBmcPfrRecoverySvn;
  UINT8   PfrBmcPfmRecoveryMajorVersion;
  UINT8   PfrBmcPfmRecoveryMinorVersion;
  UINT8   PfrLockStatus;
  UINT8   PfrProvisionStatus;
  UINT8   PfrPitL1Status;
  UINT8   PfrPitL2Status;
  UINT8   PfrLock;
  UINT8   PfrProvision;
  UINT8   PfrUnProvision;
  UINT8   PfrPitL1;
  UINT8   PfrPitL2;
  // PFR }

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
  UINT8         ShellEntryTime;

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

//
// UBA_START
//
  UINT8 SataInterfaceRAIDMode;
  UINT8 sSataInterfaceRAIDMode;
//
// UBA_END
//
  UINT16  C01MemoryVoltage;
  UINT16  C23MemoryVoltage;

  UINT16  CpuVccInVoltage;

  UINT8   VccIoVoltage;

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
  UINT8 ReservedS2;
  UINT8 ReservedS3;

  UINT8   StorageOpROMSuppression;
  UINT8   RsaSupport;

  UINT8   ReservedS4;
  UINT8   ReservedS5;

  //
  // PCIe Leaky Bucket Feature (requires Gen4 IP)
  //
  UINT64  ExpectedBer;
  UINT32  Gen12TimeWindow;
  UINT8   Gen34TimeWindow;
  UINT8   Gen12ErrorThreshold;
  UINT8   Gen34ErrorThreshold;
  UINT8   Gen34ReEqualization;
  UINT8   Gen2LinkDegradation;
  UINT8   Gen3LinkDegradation;
  UINT8   Gen4LinkDegradation;

  //
  // Crash Log Feature
  //
  UINT8   CrashLogFeature;
  UINT8   CrashLogOnAllReset;
  UINT8   CrashLogClear;
  UINT8   CrashLogReArm;

  //
  // Error Control
  //
  UINT8   Ce2LmLoggingEn;
  UINT8   KtiFirstCeLatchEn;
  UINT8   PatrolScrubErrorReporting;
  UINT8   LlcEwbErrorControl;
  UINT8   KcsAccessPolicy;

  //
  // Platform Deep S5 Feature
  //
  UINT8   PlatformDeepS5;
  UINT8   DeepS5DelayTime;

  UINT8   EnableClockSpreadSpec;

  //
  // TCC Mode
  //
  UINT8   TccMode;
} SYSTEM_CONFIGURATION;

typedef struct {
  UINT8   FakeItem;
} FAKE_VARSTORE;

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


extern EFI_GUID  gMainPkgListGuid;
extern EFI_GUID  gAdvancedPkgListGuid;
extern EFI_GUID  gTpmPkgListGuid;
extern EFI_GUID  gSecurityPkgListGuid;
extern EFI_GUID  gBootOptionsPkgListGuid;
extern EFI_GUID  gServerMgmtPkgListGuid;


#ifndef VFRCOMPILE

extern EFI_GUID gEfiSetupVariableGuid;
extern EFI_GUID gEfiSetupVariableDefaultGuid;
extern EFI_GUID gEfiGlobalVariableControlGuid;

typedef struct {
  UINT8 ProcessController;
} SYSTEM_CONFIGURATION_CONTROL;

#endif

#define SYSTEM_PASSWORD_ADMIN   0
#define SYSTEM_PASSWORD_USER    1

#define PASSWORD_ADMIN_NAME     L"AdminName"
#define PASSWORD_USER_NAME      L"UserName"

#endif // #ifndef _SETUP_VARIABLE
