/** @file
  Data format for Ioh Memory Config Data Structure

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef   __SOCKET_MEMORY_CONFIG_DATA_H__
#define   __SOCKET_MEMORY_CONFIG_DATA_H__


#include <UncoreCommonIncludes.h>
#include <MemDefaults.h>

extern EFI_GUID gEfiSocketMemoryVariableGuid;
#define SOCKET_MEMORY_CONFIGURATION_NAME L"SocketMemoryConfig"

#pragma pack(1)
typedef struct {
  UINT8                   ReservedS48;
  UINT8                   MemoryHotPlugBase;
  UINT8                   MemoryHotPlugLen;
  UINT8                   Srat;
  UINT8                   SratMemoryHotPlug;
  UINT8                   SratCpuHotPlug;
  UINT8                   PagePolicy;
  UINT8                   PatrolScrub;
  UINT8                   PatrolScrubDuration;
  UINT8                   TempRefreshOption;
  UINT8                   HalfxRefreshValue;
  UINT8                   TwoxRefreshValue;
  UINT8                   FourxRefreshValue;
  UINT8                   NsddcEn;
  UINT8                   EsddcEn;
  UINT8                   ColumnCorrectionDisable;
  UINT8                   SaiPolicyGroupWaBiosW;
  UINT8                   PatrolScrubAddrMode;
  UINT8                   partialmirrorsad0;
  UINT8                   PartialMirrorUefi;
  UINT16                  PartialMirrorUefiPercent;
  UINT16                  partialmirrorsize[MAX_PARTIAL_MIRROR]; // Array of sizes of different partial mirrors
  UINT8                   ImmediateFailoverAction;
  UINT8                   PlusOneEn;
  UINT8                   MemCeFloodPolicy;
  UINT16                  spareErrTh;
  UINT8                   TriggerSWErrThEn;
  UINT16                  SpareSwErrTh;
  UINT16                  timeWindow;
  UINT8                   DieSparing;
  UINT8                   PclsEn;
  UINT8                   ADDDCEn;
  UINT8                   DcpmmEccModeSwitch;
  UINT8                   AdddcErrInjEn;
  UINT8                   leakyBktTimeWindow;
  UINT8                   leakyBktLo;
  UINT8                   leakyBktHi;
  UINT16                  leakyBktHour;
  UINT8                   leakyBktMinute;
  UINT8                   CmdNormalization;
  UINT8                   LrDimmBacksideVrefEn;
  UINT8                   CmdVrefEnable;
  UINT8                   DramRonEn;
  UINT8                   McRonEn;
  UINT8                   RxCtleTrnEn;
  UINT8                   RxOdtEn;
  UINT8                   LrDimmWrVrefEn;
  UINT8                   LrDimmRdVrefEn;
  UINT8                   LrDimmTxDqCentering;
  UINT8                   LrDimmRxDqCentering;
  UINT8                   txEqCalibration;
  UINT8                   CmdTxEqCalibration;
  UINT8                   RxDfe;
  UINT8                   TxRiseFallSlewRate;
  UINT8                   iModeTraining;
  UINT8                   TcoCompTraining;
  UINT8                   RoundTripLatency;
  UINT8                   DutyCycleTraining;
  UINT8                   PxcTraining;
  UINT8                   DdjcTraining;
  UINT8                   refreshMode;
  UINT8                   dllResetTestLoops;
  UINT8                   DdrMemoryType;
  UINT8                   HwMemTest;
  UINT16                  MemTestLoops;
  UINT8                   TrainingResultOffsetFunctionEnable;
  UINT16                  OffsetTxDq;
  UINT16                  OffsetRxDq;
  UINT16                  OffsetTxVref;
  UINT16                  OffsetRxVref;
  UINT16                  OffsetCmdAll;
  UINT16                  OffsetCmdVref;
  UINT16                  OffsetCtlAll;
  UINT32                  AdvMemTestOptions;
  UINT8                   AdvMemTestResetList;
  UINT8                   AdvMemTestCondition;
  UINT16                  AdvMemTestCondVdd;
  UINT8                   AdvMemTestCondTwr;
  UINT16                  AdvMemTestCondTrefi;
  UINT32                  AdvMemTestCondPause;
  UINT8                   EccSupport;
  UINT8                   EccEnable;
  UINT8                   ReservedS49;
  UINT8                   ReservedS50;
  UINT8                   ReservedS51;
  UINT8                   ReservedS52;
  UINT16                  ReservedS53;
  UINT16                  ReservedS54;
  UINT16                  ReservedS55;
  UINT16                  ReservedS56;
  UINT16                  ReservedS57;
  UINT8                   ReservedS58;
  UINT16                  ReservedS59;
  UINT8                   ReservedS60;
  UINT16                  ReservedS61;
  UINT8                   ReservedS62;
  UINT8                   ReservedS63;
  UINT16                  ReservedS64;
  UINT16                  ReservedS65;
  UINT8                   ReservedS66;
  UINT8                   ReservedS67;
  UINT8                   ReservedS68;
  UINT8                   ReservedS69;
  UINT8                   ReservedS70;
  UINT8                   ReservedS71;
  UINT8                   ReservedS72;
  UINT8                   ReservedS73;
  UINT8                   ReservedS74[16];    //[MAX_SOCKET * MAX_IMC] = [8]
  UINT8                   volMemMode;
  UINT8                   CacheMemType; //Only valid if volMemMode is 2LM
  UINT8                   DdrCacheSize;
  UINT8                   PmemCaching;
  UINT8                   ReservedS75;
  UINT8                   memInterleaveGran1LM;
  UINT8                   ReservedS76;
  UINT8                   ReservedS77;
  UINT8                   ReservedS78;
  UINT8                   ReservedS79;
  UINT8                   ReservedS80;
  UINT8                   CkeProgramming;
  UINT8                   SrefProgramming;
  UINT8                   PkgcSrefEn;
  UINT8                   CkeIdleTimer;
  UINT8                   ApdEn;
  UINT8                   PpdEn;
  UINT8                   DdrtCkeEn;
  UINT8                   OppSrefEn;
  UINT8                   OppSrefVisible;    //Setup variable to hide Opportunistic Self Refresh Knob
  UINT8                   DdrtSrefEn;
  UINT8                   DataDllOff;
  UINT8                   MdllOffEn;
  UINT8                   CkMode;
  UINT8                   MemTestOnColdFastBoot;
  UINT8                   AttemptFastBoot;
  UINT8                   AttemptFastBootCold;
  UINT8                   bdatEn;
  UINT8                   ScrambleEnDDRT;
  UINT8                   ScrambleEn;  // for ddr4
  UINT8                   allowCorrectableError;
  UINT8                   allowCorrectableMemTestError;
  UINT16                  ScrambleSeedLow;
  UINT16                  ScrambleSeedHigh;
  UINT8                   CustomRefreshRateEn;
  UINT8                   CustomRefreshRate;
  UINT8                   readVrefCenter;
  UINT8                   wrVrefCenter;
  UINT8                   haltOnMemErr;
  UINT8                   thermalthrottlingsupport;
  UINT8                   MemTripReporting;
  UINT8                   OffPkgMemToThermTrip;
  UINT8                   OffPkgMemToMemTrip;
  UINT8                   InPkgMemToThermTrip;
  UINT8                   InPkgMemToMemTrip;
  UINT8                   DimmTempStatValue;
  UINT8                   XMPProfilesSup;
  UINT8                   XMPMode;
  UINT8                   tCAS;
  UINT8                   tRP;
  UINT8                   tRCD;
  UINT8                   tRAS;
  UINT8                   tWR;
  UINT16                  tRFC;
  UINT8                   tRRD;
  UINT8                   tRRD_L;
  UINT8                   tRTP;
  UINT8                   tWTR;
  UINT8                   tFAW;
  UINT8                   tCWL;
  UINT8                   tRC;
  UINT8                   commandTiming;
  UINT16                  tREFI;
  UINT8                   DdrFreqLimit;
  UINT16                  Vdd;
  UINT8                   lrdimmModuleDelay;
  UINT32                  rmtPatternLength;
  UINT32                  rmtPatternLengthExt;
  UINT8                   RecEnDelayAverage;
  UINT8                   check_pm_sts;
  UINT8                   check_platform_detect;
  UINT8                   MemPwrSave;
  UINT8                   ElectricalThrottlingMode;
  UINT8                   MultiThreaded;
  UINT8                   promoteMrcWarnings;
  UINT8                   promoteWarnings;
  UINT8                   oppReadInWmm;
  UINT16                  normOppInterval;
  UINT8                   ReservedS81[96]; //[MAX_SETUP_SOCKET * MAX_SETUP_IMC * MAX_SETUP_MC_CH] = [8 * 4 * 3 = 96]
  UINT8                   mdllSden;
  UINT8                   memhotSupport;
  UINT8                   MemHotIn;
  UINT8                   MemHotOut;
  UINT8                   MemHotOuputAssertThreshold;
  UINT8                   ADREn;
  UINT8                   RankMargin;
  UINT8                   EnableBacksideRMT;
  UINT8                   EnableBacksideCMDRMT;
  UINT8                   EnableNgnBcomMargining;
  UINT8                   ReservedS82;
  UINT8                   RankSparing;
  UINT8                   multiSparingRanks;
  UINT8                   ReservedS83;
  UINT8                   dimmIsolation;
  UINT8                   smbSpeed;
  UINT8                   SmbSpdAccess;
  UINT8                   SpdPrintEn;
  UINT16                  SpdPrintLength;
  UINT8                   EnforcePOR;
  UINT8                   pda;
  UINT8                   turnaroundOpt;
  UINT8                   turnaroundOptDdrt;
  UINT8                   oneRankTimingMode;
  UINT8                   eyeDiagram;
  UINT8                   NvmdimmPerfConfig;
  UINT8                   ReservedS84;
  UINT8                   ReservedS85;
  UINT8                   ReservedS86;
  UINT8                   ReservedS87;
  UINT8                   ReservedS88;
  UINT8                   ReservedS89;
  UINT8                   ReservedS90;
  UINT8                   ReservedS91;
  UINT8                   ReservedS92;
  UINT8                   ReservedS93;
  UINT8                   ReservedS94;
  UINT8                   ReservedS95;
  UINT8                   ReservedS96;
  UINT8                   ReservedS97;
  UINT8                   ReservedS98;
  UINT8                   ReservedS99;
  UINT8                   ReservedS100;
  UINT8                   ReservedS101;
  UINT8                   ReservedS102;
  UINT8                   ReservedS103;
  UINT8                   ReservedS104;
  UINT8                   DramRaplPwrLimitLockCsr;
  UINT8                   DramRaplEnable;
  UINT8                   BwLimitTfOvrd;
  UINT8                   perbitmargin;
  UINT8                   DramRaplExtendedRange;
  UINT8                   CmsEnableDramPm;
  UINT8                   logParsing;
  UINT8                   WritePreamble;
  UINT8                   ReadPreamble;
  UINT8                   WrCRC;
  UINT8                   AepOnSystem;
  UINT8                   EkvOnSystem;   // 0 => Do not suppress power management policy for BWV, 1 => suppress power management policy for BWV.
  UINT8                   BwvOnSystem;   // 0 => Do not suppress power management policy for EKV, 1 => suppress power management policy for EKV.
  // NGN options
  UINT8                   LockNgnCsr;
  UINT8                   NgnCmdTime;
  UINT8                   NgnEccCorr;
  UINT8                   NgnEccWrChk;
  UINT8                   NgnEccRdChk;
  UINT8                   NgnEccExitCorr;
  UINT8                   NgnDebugLock;
  UINT8                   NgnArsPublish;
  UINT8                   RmtOnColdFastBoot;
  UINT8                   LegacyRmt;
  UINT8                   mrcRepeatTest;
  UINT8                   ReservedS105;
  UINT8                   ReservedS106;
  UINT8                   ReservedS107;
  UINT8                   staggerref;
  UINT32                  memFlows;
  UINT32                  memFlowsExt;
  UINT32                  memFlowsExt2;
  UINT32                  memFlowsExt3;
  UINT8                   Blockgnt2cmd1cyc;
  UINT8                   TrefiPerChannel;
  UINT8                   TrefiNumofRank;
  UINT16                  TrefiDelay;
  UINT8                   Disddrtopprd;
  UINT16                  Reserved;
  UINT8                   setSecureEraseAllDIMMs;
  UINT8                   setSecureEraseSktCh[64];  // [MAX_SOCKET * MAX_IMC * MAX_MC_CH]
  UINT8                   SetSecureEraseSktChHob[64]; // [MAX_SOCKET * MAX_IMC * MAX_MC_CH]
  UINT8                   AppDirectMemoryHole;
  UINT8                   LatchSystemShutdownState;
  UINT8                   ExtendedType17;
  //
  // PPR related
  //
  UINT8                   pprType;
  UINT8                   pprErrInjTest;
  // CR QoS Configuration Profiles
  UINT8                   FastGoConfig;
  UINT8                   ReservedS108;
  UINT8                   LegacyADRModeEn;
  UINT8                   MinNormalMemSize;
  UINT8                   ADRDataSaveMode;
  UINT8                   eraseArmNVDIMMS;
  UINT8                   restoreNVDIMMS;
  UINT8                   interNVDIMMS;
  UINT8                   imcBclk;
  UINT8                   spdCrcCheck;
  UINT8                   ReservedS109;
  UINT8                   SETUP_REMOVE_SanitizeOverwriteNvmDimm;  // removed
  UINT8                   EliminateDirectoryInFarMemory;
  UINT8                   NvmdimmPowerCyclePolicy;

  //CMI Init option
  UINT8                   CmiInitOption;
  //
  // Cmd setup hold percent offset for 2n timing
  //
  UINT8                   cmdSetupPercentOffset;
  UINT8                   ShortStroke2GB;
  UINT8                   NvDimmEnergyPolicy;     //Energy Policy Management
  UINT8                   ReservedS110;
  UINT8                   ReservedS111;
  UINT8                   ReservedS112;
  UINT8                   ReservedS113;
  UINT8                   DisableDirForAppDirect;
  UINT8                   NvmMediaStatusException;
  UINT8                   NvmQos;
  UINT8                   ReservedS114;
  UINT8                   ReservedS115;
  UINT8                   ReservedS116;

  //
  // FIS 2.x
  //
  UINT16                  DcpmmAveragePowerLimit;        // Power limit in mW used for averaged power ( Valid range starts from 10000mW).
  UINT8                   DcpmmAveragePowerTimeConstant;    // Value used as a base time window for power usage measurements Turbo Mode Support(in mSec).
  UINT32                  DcpmmMbbAveragePowerTimeConstant; // Value used as a base time window for power usage measurements Memory Bandwidth Boost Support(in mSec).
  UINT8                   DcpmmMbbFeature;                  // Allows enabling and disabling the feature (Turbo Mode State/Memory Bandwidth Boost).
  UINT16                  DcpmmMbbMaxPowerLimit;            // Power limit in mW used for limiting the Turbo Mode/Memory Bandwidth Boost
                                                            // power consumption (Valid range starts from 15000mW).

  UINT8                   ReservedS117;
  UINT8                   ReservedS118;
  UINT8                   ReservedS119;

  UINT8                   ReservedS120;
  UINT8                   LsxImplementation;
  UINT8                   FactoryResetClear;
  UINT8                   EadrSupport;
  UINT32                  NvdimmSmbusMaxAccessTime;
  UINT32                  NvdimmSmbusReleaseDelay;
  UINT8                   NfitPublishMailboxStructs;

  //
  // fADR setup option
  //
  UINT8                   FadrSupport;

  //
  // Biased 2-way near memory cache support options
  //
  UINT8                   EnableTwoWayNmCache;          // Enable or disable biased 2-way near memory cache.
  UINT16                  NonPreferredWayMask;          // A 10-bit mask to control the bias counter ratio.
  UINT8                   PreferredReadFirst;           // Reads are issued to the non-preferred or preferred way first.

  //
  // Boot-time fast zero memory setup option
  //
  UINT8                   FastZeroMemSupport;           // Enable or disable boot-time fast zero memory support.

  //
  // XOR decoder options
  //
  UINT8                   ReservedS121;
  UINT8                   ReservedS122;
  UINT8                   ReservedS123;
  UINT8                   ReservedS124;
  UINT8                   ReservedS125;
  UINT8                   ReservedS126;
  UINT8                   ReservedS127;

  UINT8                   ReservedS128;
  UINT8                   ReservedS129;

  UINT8                   ReservedS130;
  UINT16                  ReservedS131;
  UINT8                   ReservedS132;
  UINT8                   ReservedS133;
  UINT8                   ReservedS134;
  UINT8                   ReservedS135;
  UINT8                   DcpmmApiVersion200OnSystem;   // 0 => Suppress DcpmmAveragePowerTimeConstant, 1 => Do not suppress DcpmmAveragePowerTimeConstant.
  UINT8                   DcpmmApiVersion201OnSystem;   // 0 => Suppress DcpmmAveragePowerTimeConstant, 1 => Do not suppress DcpmmAveragePowerTimeConstant.
  UINT8                   DcpmmApiVersion200OrGreaterOnSystem;
  UINT8                   ReservedS136;
  UINT8                   EnforcePopulationPor;

  //
  // DFE Path Finding
  //
  UINT8                   EnableTapSweep;

  UINT8                   DfeGainBias;

  UINT8                   Tap1Start;
  UINT8                   Tap1End;
  UINT8                   Tap1Size;

  UINT8                   Tap2Start;
  UINT8                   Tap2End;
  UINT8                   Tap2Size;

  UINT8                   Tap3Start;
  UINT8                   Tap3End;
  UINT8                   Tap3Size;

  UINT8                   Tap4Start;
  UINT8                   Tap4End;
  UINT8                   Tap4Size;

  UINT8                   TrainingCompOptions;          // Memory Training Comp Options Values.

  UINT8                   PeriodicRcomp;                // Memory Periodic Rcomp Auto/Enable/Disable.
  UINT8                   PeriodicRcompInterval;        // Memory Periodic Rcomp Interval.

  UINT8                   ReservedS137;

  UINT8                   AepNotSupportedException;

  UINT8                   ReservedS138;

  UINT8                   ReservedS139;
  UINT8                   PanicWm;

  UINT16                  OffsetRecEn;

  UINT8                   EadrCacheFlushMode;
  UINT8                   ReservedS140;
  UINT8                   ReservedS141;
  UINT8                   ReservedS142;
  UINT8                   ReservedS143;

  UINT8                   ReservedS144;
  UINT8                   ReservedS145;
  UINT8                   LrdimmDbDfeTraining;

  UINT8                   ReservedS146;
  UINT8                   ReservedS147;
  UINT8                   ReservedS148;
  UINT8                   AdvMemTestRetryAfterRepair;
  UINT8                   AdvMemTestPpr;
  UINT8                   AdvMemTestRankListNumEntries;
  UINT32                  AdvMemTestRankList[ADV_MT_LIST_LIMIT];

  UINT32                  SmartTestKey;
  UINT8                   SetMemTested;

  //
  // RMT minimum margin check
  //
  UINT8                   RmtMinimumMarginCheck;

  UINT8                   ReservedS149;
} SOCKET_MEMORY_CONFIGURATION;

#pragma pack()

#endif

