/** @file
  This file includes all the data structures that the MRC considers "global data".

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MrcInterface_h_
#define _MrcInterface_h_

#define MAX_CPU_SOCKETS          (1)       ///< The maximum number of CPUs per system.
#define MAX_CONTROLLERS          (1)       ///< The maximum number of memory controllers per CPU socket.
#define MAX_CHANNEL              (2)       ///< The maximum number of channels per memory controller.
#define MAX_DIMMS_IN_CHANNEL     (2)       ///< The maximum number of DIMMs per channel.
#define MAX_RANK_IN_DIMM         (2)       ///< The maximum number of ranks per DIMM.
#define MAX_RANK_IN_CHANNEL      (MAX_DIMMS_IN_CHANNEL * MAX_RANK_IN_DIMM) ///< The maximum number of ranks per channel.
#define MAX_SDRAM_IN_DIMM        (9)       ///< The maximum number of SDRAMs per DIMM when ECC is enabled.
#define MAX_MR_IN_DIMM           (7)       ///< Maximum number of mode registers in a DIMM.
#define MAX_DEVICES_IN_DDR4      (8)       ///< The maximum number of SDRAMs per DDR4 DIMM.
#define MAX_BITS                 (8)       ///< BITS per byte.
#define MAX_STROBE               (18)      ///< Number of strobe groups.
#define MAX_DQ                   (72)      ///< Number of Dq bits used by the rank.
#define CHAR_BITS                (8)       ///< Number of bits in a char.
#define PSMI_SIZE_MB             (64)      ///< Define the max size of PSMI needed in MB
#define BCLK_DEFAULT             (100 * 1000 * 1000) ///< BCLK default value, in Hertz.
#define MAX_COMMAND_GROUPS       (2)
#define MAX_EDGES                (2) ///< Maximum number of edges.
#define SUPPORT_DDR3             SUPPORT   ///< SUPPORT means that DDR3 is supported by the MRC.
#define ULT_SUPPORT_LPDDR3       SUPPORT   ///< SUPPORT means that LPDDR3 is supported by the MRC.
#define TRAD_SUPPORT_LPDDR3      /*UN*/SUPPORT ///< SUPPORT means that LPDDR3 is supported by the MRC.
#define BDW_SUPPORT_LPDDR3       SUPPORT   ///< SUPPORT means that LPDDR3 is supported by the MRC.
#define JEDEC_SUPPORT_LPDDR      SUPPORT   ///< SUPPORT means that JEDEC SPD Spec for LPDDR3 is supported by the MRC.
#define SUPPORT_DDR4             SUPPORT   ///< SUPPORT means that DDR4 is supported by the MRC.
#define SUPPORT_LPDDR3           (ULT_SUPPORT_LPDDR3 || TRAD_SUPPORT_LPDDR3 || BDW_SUPPORT_LPDDR3 || JEDEC_SUPPORT_LPDDR)
#define MRC_ALL_DDR_SUPPORTED    ((SUPPORT_DDR4 == SUPPORT) && ((SUPPORT_DDR3 == SUPPORT) && (SUPPORT_LPDDR3 == SUPPORT)))
#define MRC_DDR3_LPDDR_SUPPORTED ((SUPPORT_DDR3 == SUPPORT) || (SUPPORT_LPDDR3 == SUPPORT))
#define SPD3_MANUF_START       117         ///< The starting point for the SPD manufacturing data.
#define SPD3_MANUF_END         127         ///< The ending point for the SPD manufacturing data.
#if (SUPPORT_DDR4 == SUPPORT)
#define SPD4_MANUF_START       320         ///< The starting point for the SPD manufacturing data.
#define SPD4_MANUF_END         328         ///< The ending point for the SPD manufacturing data.
#endif
#if (JEDEC_SUPPORT_LPDDR == SUPPORT)
#define SPDLP_MANUF_START      320         ///< The starting point for the SPD manufacturing data.
#define SPDLP_MANUF_END        328         ///< The ending point for the SPD manufacturing data.
#endif
#include "MrcSpdData.h"
#include "MrcRmtData.h"
#include "MrcCommonTypes.h"
#pragma pack (push, 1)


///
//////////////////////////////////////////////////////////////////////////////////////
///                           OEM platform  routines and types                      //
//////////////////////////////////////////////////////////////////////////////////////
///
/// define the oem check points the OEM can define more point and locate them in the code.
///
typedef enum {
  OemFastBootPermitted,     ///<  before fast boot.
  OemRestoreNonTraining,
  OemPrintInputParameters,  ///<  before printing input parameters
  OemSpdProcessingRun,      ///<  before spd processing code
  OemSetOverridePreSpd,     ///<  before set overrides pre spd
  OemSetOverride,           ///<  before set overrides
  OemMcCapability,          ///<  before MC capability
  OemMcInitRun,             ///<  before mc init code
  OemMcMemoryMap,           ///<  before memory map
  OemMcResetRun,            ///<  before jedec reset
  OemPreTraining,           ///<  before the training.
  OemMcTrainingRun,         ///<  before training code
  OemEarlyCommandTraining,  ///<  before Early Command training
  OemJedecInitLpddr3,       ///<  before Jedec Init Lpddr3
  OemSenseAmpTraining,      ///<  before Sense Amp Training
  OemReadMprTraining,       ///<  before Read MPR Training
  OemReceiveEnable,         ///<  before Read Leveling
  OemJedecWriteLeveling,    ///<  before Jedec Write Leveling
  OemLpddrLatencySetB,      ///<  before LPDDR Latency Set B
  OemWriteDqDqs,            ///<  before Write Timing Centering
  OemWriteVoltage,          ///<  before Write Voltage Centering
  OemEarlyWriteDqDqs2D,     ///<  before Early Write Timing Centering 2D
  OemEarlyWrDsEq,           ///<  before Early Write Drive Strength / Equalization
  OemEarlyReadDqDqs2D,      ///<  before Early Read Timing Centering 2D
  OemEarlyReadMprDqDqs2D,   ///<  before Early MPR Read Timing Centering 2D
  OemReadDqDqs,             ///<  before Read Timing Centering
  OemDdr4Map,               ///<  before DDR4 PDA Mapping
  OemDimmRonTraining,       ///<  before DIMM Ron Training
  OemDimmODTTraining,       ///<  before DIMM ODT Training
  OemWriteDriveStrengthEq,  ///<  before Write Drive Strength/Equalization 2D Training
  OemWriteDriveUpDn,        ///<  before Write Drive Strength Up/Dn 2D Training
  OemWriteSlewRate,         ///<  before Write Slew Rate Training
  OemReadODTTraining,       ///<  before Read ODT algorithm.
  OemReadEQTraining,        ///<  before Read Equalization Training
  OemReadAmplifierPower,    ///<  before Read Amplifier Power
  OemOptimizeComp,          ///<  before Comp Optimization Training
  OemPowerSavingMeter,      ///<  before PowerSavingMeter step
  OemWriteDqDqs2D,          ///<  before Write Timing Centering 2D
  OemReadDqDqs2D,           ///<  before Read Timing Centering 2D
  OemCmdVoltCenterPreLct,   ///<  before Command Voltage Centering that runs pre-LCT
  OemCmdSlewRate,           ///<  before CMD Slew Rate
  OemCmdVoltCentering,      ///<  before Command Voltage Centering
  OemCmdDriveStrengthEq,    ///<  before Command Drive Strength Equalization
  OemWriteVoltCentering2D,  ///<  before Write Voltage Centering 2D
  OemReadVoltCentering2D,   ///<  before Read Voltage Centering 2D
  OemLateCommandTraining,   ///<  before Late Command training
  OemCmdNormalization,      ///<  before CMD Normalization
  OemRoundTripLatency,      ///<  before Round Trip Latency Traiing
  OemTurnAroundTimes,       ///<  before Turn Aorund Times.
  OemRcvEnCentering1D,      ///<  before Receive Enable Centring
  OemSaveMCValues,          ///<  before saving memory controller values
  OemRmt,                   ///<  before RMT crosser tool.
  OemMemTest,               ///<  before Memory testing
  OemRestoreTraining,       ///<  before Restoring Training Values
  OemJedecResetDdr4Fast,    ///<  before JEDEC reset for DDR4 in Fast flow
  OemSelfRefreshExit,       ///<  before Self Refresh Exit
  OemNormalMode,            ///<  before Normal Mode on non-cold boots.
  OemThermalConfig,         ///<  set Thermal config values.
  OemTxtAliasCheck,         ///<  before TxT Alias Check Call.
  OemAliasCheck,            ///<  before alias checking on cold boots.
  OemHwMemInit,

  OemPostTraining,          ///<  after the training.
  OemForceOltm,             ///<  before MrcForceOltm
  OemMrcActivate,           ///<  before MrcActivate call.
  OemMrcRhPrevention,       ///<  before MrcRhPrevention
  OemSaGvSwitch,            ///<  before SA GV switch
  OemEngPerfGain,           ///<  before Energy Performance Gain.
  OemMrcDone,               ///<  call to MrcOemCheckPoint when MRC was done.
  OemFrequencySet,          ///<  do operation before frequency set.
  OemFrequencySetDone,      ///<  do operation after frequency set.
  OemStartMemoryConfiguration,
  OemBeforeNormalMode,      ///<  call to MrcOemCheckPoint before normal mode is enalbed
  OemAfterNormalMode,       ///<  call to MrcOemCheckPoint after normal mode is enalbed
  OemMrcFillBdat,
  OemRetrainMarginCheck,
  OemRmtPerBit,             ///< before Rank Margin Tool Per-Bit.
  OemUpdateSaveMCValues,    ///< before Updating memory controller values.
  ///
  ///*********************************************************************************
  ///
  OemNumOfCommands          ///<  Should always be last in the list!
} MrcOemStatusCommand;

typedef UINT8 MrcIteration; ///< Mrc invocation sequence number, start with 0 and increment by one each time MRC library is called.
#define MRC_ITERATION_MAX ((1 << ((sizeof (MrcIteration) * 8) - 1)) + ((1 << ((sizeof (MrcIteration) * 8) - 1)) - 1))

#define MAX_RCOMP         (3)
#define MAX_RCOMP_TARGETS (5)

///
/// Thermal Options
///
typedef struct {
  UINT8  RaplLim2WindX;                                    ///< Offset 110  - Power Limit 2 Time Window X value: 0=Minimal, 3=Maximum, <b>1=Default</b>
  UINT8  RaplLim2WindY;                                    ///< Offset 111  - Power Limit 2 Time Window Y value: 0=Minimal, 3=Maximum, <b>1=Default</b>
  UINT8  RaplLim1WindX;                                    ///< Offset 112  - Power Limit 1 Time Window X value: <b>0=Minimal</b>, 3=Maximum
  UINT8  RaplLim1WindY;                                    ///< Offset 113  - Power Limit 1 Time Window Y value: <b>0=Minimal</b>, 31=Maximum
  UINT16 RaplLim2Pwr;                                      ///< Offset 114  - Power Limit 2: 0=Minimal, 16383=Maximum, <b>222=Default</b>
  UINT16 RaplLim1Pwr;                                      ///< Offset 116  - Power Limit 1: <b>0=Minimal</b>, 16383=Maximum
  UINT8  WarmThreshold[MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL]; ///< Offset 118  - Warm Threshold (Channel 0, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8  HotThreshold[MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];  ///< Offset 122  - Hot Threshold (Channel 0, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8  WarmBudget[MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];    ///< Offset 126  - Warm Budget (Channel 0, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8  HotBudget[MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];     ///< Offset 130  - Hot Budget (Channel 0, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8  IdleEnergy[MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT8  PdEnergy[MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT8  ActEnergy[MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT8  RdEnergy[MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT8  WrEnergy[MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
} ThermalMngmtEn;


typedef struct {
  UINT8      GdxcEnable;   ///< GDXC  MOT enable
  UINT8      GdxcIotSize;  ///< IOT size in multiples of 8MEG
  UINT8      GdxcMotSize;  ///< MOT size in multiples of 8MEG
} MrcGdxc;

typedef struct {
  UINT32 ECT : 1;        ///< BIT0 - Early Command Training
  UINT32 SOT : 1;        ///< BIT1 - Sense Amp Offset Training
  UINT32 ERDMPRTC2D : 1; ///< BIT2 - Early ReadMPR Timing Centering 2D
  UINT32 RDMPRT : 1;     ///< BIT3 - Read MPR Training
  UINT32 RCVET : 1;      ///< BIT4 - Read Leveling Training (RcvEn)
  UINT32 JWRL : 1;       ///< BIT5 - Jedec Write Leveling
  UINT32 EWRTC2D : 1;    ///< BIT6 - Early Write Time Centering 2D
  UINT32 ERDTC2D : 1;    ///< BIT7  - Early Read Time Centering 2D
  UINT32 WRTC1D : 1;     ///< BIT8 - Write Timing Centering 1D
  UINT32 WRVC1D : 1;     ///< BIT9 - Write Voltage Centering 1D
  UINT32 RDTC1D : 1;     ///< BIT10 - Read Timing Centering 1D
  UINT32 DIMMODTT : 1;   ///< BIT11 - Dimm ODT Training
  UINT32 DIMMRONT : 1;   ///< BIT12 - Dimm Ron Training
  UINT32 WRDSEQT : 1;    ///< BIT13 - Write Drive Strength / Equalization Training 2D
  UINT32 WRSRT : 1;      ///< BIT14 - Write Slew Rate Training
  UINT32 RDODTT : 1;     ///< BIT15 - Read ODT Training
  UINT32 RDEQT : 1;      ///< BIT16 - Read Equalization Training
  UINT32 RDAPT : 1;      ///< BIT17 - Read Amplifier Power Training
  UINT32 WRTC2D : 1;     ///< BIT18 - Write Timing Centering 2D
  UINT32 RDTC2D : 1;     ///< BIT19 - Read Timing Centering 2D
  UINT32 WRVC2D : 1;     ///< BIT20 - Write Voltage Centering 2D
  UINT32 RDVC2D : 1;     ///< BIT21 - Read Voltage Centering 2D
  UINT32 CMDVC : 1;      ///< BIT22 - Command Voltage Centering
  UINT32 LCT : 1;        ///< BIT23 - Late Command Training
  UINT32 RTL : 1;        ///< BIT24 - Round Trip latency
  UINT32 TAT : 1;        ///< BIT25 - Turn Around Timing
  UINT32 RMT : 1;        ///< BIT26 - RMT Tool
  UINT32 MEMTST : 1;     ///< BIT27 - Memory Test
  UINT32 ALIASCHK: 1;    ///< BIT28 - SPD Alias Check
  UINT32 RCVENC1D: 1;    ///< BIT29 - Receive Enable Centering 1D
  UINT32 RMC : 1;        ///< BIT30 - Retrain Margin Check
  UINT32 WRDSUDT : 1;    ///< BIT31 - Write Drive Strength Up/Dn independently
} TrainingStepsEn;

typedef struct {
  UINT32 CMDSR    : 1;   ///< BIT0  - CMD Slew Rate Training
  UINT32 CMDDSEQ  : 1;   ///< BIT1  - CMD Drive Strength and Tx Equalization
  UINT32 CMDNORM  : 1;   ///< BIT2  - CMD Normalization
  UINT32 EWRDSEQ  : 1;   ///< BIT3  - Early DQ Write Drive Strength and Equalization Training
  UINT32 TeRsv4   : 1;   ///< BIT4 - Reserved
  UINT32 TeRsv5   : 1;   ///< BIT5 - Reserved
  UINT32 TeRsv6   : 1;   ///< BIT6 - Reserved
  UINT32 RsvdBits :25;
} TrainingStepsEn2;

///
/// Defines whether the MRC is executing in full or mini BIOS mode.
///
typedef enum {
  MrcModeFull,   ///< Select full BIOS MRC execution.
  MrcModeMini,   ///< Select mini BIOS MRC execution.
  MrcModeMaximum ///< Delimiter.
} MrcMode;

typedef enum {
  MrcTmPower,
  MrcTmMargin,
  MrcTmMax
} TrainingModeType;

typedef enum {
  LastRxV,
  LastRxT,
  LastTxV,
  LastTxT,
  LastRcvEna,
  LastWrLevel,
  LastCmdT,
  LastCmdV,
  MAX_RESULT_TYPE
} MrcMarginResult;

typedef enum {
  MSG_LEVEL_NEVER,
  MSG_LEVEL_ERROR,
  MSG_LEVEL_WARNING,
  MSG_LEVEL_NOTE,
  MSG_LEVEL_EVENT,
  MSG_LEVEL_ALGO,
  MSG_LEVEL_MMIO,
  MSG_LEVEL_CSV,
  MSG_LEVEL_TIME,
  MSG_LEVEL_ALL = MRC_INT32_MAX
} MrcDebugMsgLevel;

///
/// Define the frequencies that may be possible in the memory controller.
/// Note that not all these values may be supported.
///
#define fNoInit     (0)
#define f800        (800)
#define f1000       (1000)
#define f1100       (1100)
#define f1067       (1067)
#define f1200       (1200)
#define f1300       (1300)
#define f1333       (1333)
#define f1400       (1400)
#define f1467       (1467)
#define f1500       (1500)
#define f1600       (1600)
#define f1700       (1700)
#define f1733       (1733)
#define f1800       (1800)
#define f1867       (1867)
#define f1900       (1900)
#define f2000       (2000)
#define f2100       (2100)
#define f2133       (2133)
#define f2200       (2200)
#define f2267       (2267)
#define f2300       (2300)
#define f2400       (2400)
#define f2500       (2500)
#define f2533       (2533)
#define f2600       (2600)
#define f2667       (2667)
#define f2700       (2700)
#define f2800       (2800)
#define f2900       (2900)
#define f2933       (2933)
#define f3000       (3000)
#define f3067       (3067)
#define f3100       (3100)
#define f3200       (3200)
#define f3333       (3333)
#define f3467       (3467)
#define f3600       (3600)
#define f3733       (3733)
#define f3867       (3867)
#define f4000       (4000)
#define f4133       (4133)
#define fInvalid    (0x7FFFFFFF)
typedef UINT32 MrcFrequency;

//
// Max supported frequency in OC mode
// RefClk133: 15*266 + 100 = 4133 (using Odd ratio mode)
// RefClk100: 15*200 + 100 = 3100 (using Odd ratio mode)
//
#define MAX_FREQ_OC_133   f4133
#define MAX_FREQ_OC_100   f3100

//
// tCK value in femtoseconds for various frequencies
// If Freq is in MHz, then tCK[fs] = 10^9 * 1/(Freq/2)
//
#define MRC_DDR_800_TCK_MIN      2500000
#define MRC_DDR_1000_TCK_MIN     2000000
#define MRC_DDR_1067_TCK_MIN     1875000
#define MRC_DDR_1100_TCK_MIN     1818182
#define MRC_DDR_1200_TCK_MIN     1666667
#define MRC_DDR_1300_TCK_MIN     1538462
#define MRC_DDR_1333_TCK_MIN     1500000
#define MRC_DDR_1400_TCK_MIN     1428571
#define MRC_DDR_1467_TCK_MIN     1363636
#define MRC_DDR_1500_TCK_MIN     1333333
#define MRC_DDR_1600_TCK_MIN     1250000
#define MRC_DDR_1700_TCK_MIN     1176471
#define MRC_DDR_1733_TCK_MIN     1153846
#define MRC_DDR_1800_TCK_MIN     1111111
#define MRC_DDR_1867_TCK_MIN     1071429
#define MRC_DDR_1900_TCK_MIN     1052632
#define MRC_DDR_2000_TCK_MIN     1000000
#define MRC_DDR_2100_TCK_MIN     952381
#define MRC_DDR_2133_TCK_MIN     938000
#define MRC_DDR_2200_TCK_MIN     909091
#define MRC_DDR_2267_TCK_MIN     882353
#define MRC_DDR_2300_TCK_MIN     869565
#define MRC_DDR_2400_TCK_MIN     833333
#define MRC_DDR_2500_TCK_MIN     800000
#define MRC_DDR_2533_TCK_MIN     789474
#define MRC_DDR_2600_TCK_MIN     769231
#define MRC_DDR_2667_TCK_MIN     750000
#define MRC_DDR_2700_TCK_MIN     740741
#define MRC_DDR_2800_TCK_MIN     714286
#define MRC_DDR_2900_TCK_MIN     689655
#define MRC_DDR_2933_TCK_MIN     681818
#define MRC_DDR_3000_TCK_MIN     666667
#define MRC_DDR_3067_TCK_MIN     652174
#define MRC_DDR_3100_TCK_MIN     645161
#define MRC_DDR_3200_TCK_MIN     625000
#define MRC_DDR_3333_TCK_MIN     600000
#define MRC_DDR_3467_TCK_MIN     576923
#define MRC_DDR_3600_TCK_MIN     555556
#define MRC_DDR_3733_TCK_MIN     535714
#define MRC_DDR_3867_TCK_MIN     517241
#define MRC_DDR_4000_TCK_MIN     500000
#define MRC_DDR_4133_TCK_MIN     483871

///
/// Define the memory nominal voltage (VDD).
/// Note that not all these values may be supported.
///
typedef enum {
  VDD_INVALID,
  VDD_1_00    = 1000,
  VDD_1_05    = 1050,
  VDD_1_10    = 1100,
  VDD_1_15    = 1150,
  VDD_1_20    = 1200,
  VDD_1_25    = 1250,
  VDD_1_30    = 1300,
  VDD_1_35    = 1350,
  VDD_1_40    = 1400,
  VDD_1_45    = 1450,
  VDD_1_50    = 1500,
  VDD_1_55    = 1550,
  VDD_1_60    = 1600,
  VDD_1_65    = 1650,
  VDD_1_70    = 1700,
  VDD_1_75    = 1750,
  VDD_1_80    = 1800,
  VDD_1_85    = 1850,
  VDD_1_90    = 1900,
  VDD_1_95    = 1950,
  VDD_2_00    = 2000,
  VDD_2_05    = 2050,
  VDD_2_10    = 2100,
  VDD_2_15    = 2150,
  VDD_2_20    = 2200,
  VDD_2_25    = 2250,
  VDD_2_30    = 2300,
  VDD_2_35    = 2350,
  VDD_2_40    = 2400,
  VDD_2_45    = 2450,
  VDD_2_50    = 2500,
  VDD_2_55    = 2550,
  VDD_2_60    = 2600,
  VDD_2_65    = 2650,
  VDD_2_70    = 2700,
  VDD_2_75    = 2750,
  VDD_2_80    = 2800,
  VDD_2_85    = 2850,
  VDD_2_90    = 2900,
  VDD_2_95    = 2950,
  VDD_MAXIMUM = 0x7FFFFFFF
} MrcVddSelect;

///
/// SA GV points
///
typedef enum {
  MrcSaGvPointLow,
  MrcSaGvPointHigh,
} MrcSaGvPoint;

///
/// SA GV modes
///  Disabled:  SA GV Disabled, run all MRC tasks
///  FixedLow:  SA GV Disabled, run only MRC tasks marked with MRC_PF_GV_LOW
///  FixedHigh: SA GV Disabled, run only MRC tasks marked with MRC_PF_GV_HIGH
///  Enabled:   SA GV Enabled
///
typedef enum {
  MrcSaGvDisabled,
  MrcSaGvFixedLow,
  MrcSaGvFixedHigh,
  MrcSaGvEnabled,
} MrcSaGv;

///
/// DIMM SPD Security Status
///
typedef enum {
  MrcSpdStatusGood,      ///< Memory is in a secure state.
  MrcSpdStatusAliased,   ///< Memory is aliased.
  MrcSpdStatusLast       ///< Must be last in the list
} MrcSpdStatus;

///
/// Define the virtual channel.
///
typedef enum {
  vcL,  ///< Virtual channel L
  vcS,  ///< Virtual channel S
} MrcVirtualChannel;

///
/// Define the board types.
///
typedef enum {
  btCRBMB,    ///< 0 - CRB Mobile
  btCRBDT,    ///< 1 - CRB Desktop
  btUser1,    ///< 2 - SV Karkom
  btUser2,    ///< 3 - SV desktop
  btUser3,    ///< 4 - SV miniDVP
  btUser4,    ///< 5 - Ult
  btCRBEMB,   ///< 6 - CRB Embedded
  btUpServer, ///< 7 - Up Server
  btUnknown,  ///< 8 - Unknown
  btMaximum   ///< Delimiter
} MrcBoardType;

///
/// Define the CPU family/model.
///
typedef enum {
  cmCFL_ULX_ULT   = CPUID_FULL_FAMILY_MODEL_COFFEELAKE_ULT_ULX,  ///< Coffeelake ULT/ULX
  cmCFL_DT_HALO   = CPUID_FULL_FAMILY_MODEL_COFFEELAKE_DT_HALO,  ///< Coffeelake DT/Halo
  cmCML_ULX_ULT   = CPUID_FULL_FAMILY_MODEL_COMETLAKE_ULT        ///< Cometlake ULT/ULX
} MrcCpuModel;

///
/// Define the CPU Tick/Tock.
///
typedef enum {
  cfCfl    = 0,  ///< Coffeelake
  cfMax
} MrcCpuFamily;

///
/// Define the CPU stepping number.
///
typedef enum {
  ///
  /// Coffeelake ULX/ULT
  ///
  csKblH0         = EnumKblH0,
  csCflD0         = EnumCflD0,
  csCflW0         = EnumCflW0,
  csCflV0         = EnumCflV0,
  csCflUlxUltLast = csCflV0,

  ///
  /// Coffeelake DT/Halo
  ///
  csCflU0         = EnumCflU0,
  csCflB0         = EnumCflB0,
  csCflP0         = EnumCflP0,
  csCflR0         = EnumCflR0,
  csCflDtHaloLast = csCflR0,
} MrcCpuStepping;

typedef enum {
  CONTROLLER_NOT_PRESENT, ///< There is no controller present in the system.
  CONTROLLER_DISABLED,    ///< There is a controller present but it is disabled.
  CONTROLLER_PRESENT,     ///< There is a controller present and it is enabled.
  MAX_CONTROLLER_STATUS   ///< Delimiter
} MrcControllerSts;

typedef enum {
  CHANNEL_NOT_PRESENT,    ///< There is no channel present on the controller.
  CHANNEL_DISABLED,       ///< There is a channel present but it is disabled.
  CHANNEL_PRESENT,        ///< There is a channel present and it is enabled.
  MAX_CHANNEL_STATUS      ///< Delimiter
} MrcChannelSts;

typedef enum {
  DIMM_ENABLED,           ///< DIMM/rank Pair is enabled, presence will be detected.
  DIMM_DISABLED,          ///< DIMM/rank Pair is disabled, regardless of presence.
  DIMM_PRESENT,           ///< There is a DIMM present in the slot/rank pair and it will be used.
  DIMM_NOT_PRESENT,       ///< There is no DIMM present in the slot/rank pair.
  MAX_DIMM_STATUS         ///< Delimiter
} MrcDimmSts;

typedef enum {
  STD_PROFILE,            ///< Standard DIMM profile select.
  USER_PROFILE,           ///< User specifies various override values.
  XMP_PROFILE1,           ///< XMP enthusiast settings select (XMP profile #1).
  XMP_PROFILE2,           ///< XMP extreme settings select (XMP profile #2).
  MAX_PROFILE             ///< Delimiter
} MrcProfile;

#define XMP_PROFILES_ENABLE   (0x3)
#define XMP1_PROFILE_ENABLE   (0x1)
#define XMP2_PROFILE_ENABLE   (0x2)

typedef enum {
  MRC_REF_CLOCK_133,      ///< 133MHz reference clock
  MRC_REF_CLOCK_100,      ///< 100MHz reference clock
  MRC_REF_CLOCK_MAXIMUM   ///< Delimiter
} MrcRefClkSelect;        ///< This value times the MrcClockRatio determines the MrcFrequency.

typedef enum {
  MRC_FREQ_INVALID       = 0,
  MRC_FREQ_133           = (MRC_BIT0 << MRC_REF_CLOCK_133), // Bit 0
  MRC_FREQ_100           = (MRC_BIT0 << MRC_REF_CLOCK_100), // Bit 1
  MRC_FREQ_133_ODD_RATIO = (MRC_BIT2 << MRC_REF_CLOCK_133), // Bit 2
  MRC_FREQ_100_ODD_RATIO = (MRC_BIT2 << MRC_REF_CLOCK_100), // Bit 3
  MRC_FREQ_MAX                                              // Delimiter
} MrcFreqFlag;

typedef UINT32 MrcBClkRef;   ///< Base clock, in Hertz, Default is 100MHz or leave at zero for default.

//
// This encoding matches CFL SC_GS_CFG.DRAM_technology and MAD_INTER_CHANNEL.DDR_TYPE registers
//
typedef enum {
  MRC_DDR_TYPE_DDR4     = 0,
  MRC_DDR_TYPE_DDR3     = 1,
  MRC_DDR_TYPE_LPDDR3   = 2,
  MRC_DDR_TYPE_UNKNOWN  = 3,
  MAX_MRC_DDR_TYPE        ///< Delimiter
} MrcDdrType;

typedef enum {
  MrcIterationClock,
  MrcIterationCmdN,
  MrcIterationCmdS,
  MrcIterationCke,
  MrcIterationCtl,
  MrcIterationCmdV,
  MrcIterationMax
} MrcIterationType;

typedef enum {
  UpmLimit,
  PowerLimit,
  RetrainLimit,
  MarginLimitMax
} MRC_MARGIN_LIMIT_TYPE;


typedef enum {
  HardwareRhp,
  Refresh2x
} MrcRhpType;

typedef enum {
  OneIn2To1 = 1,
  OneIn2To2,
  OneIn2To3,
  OneIn2To4,
  OneIn2To5,
  OneIn2To6,
  OneIn2To7,
  OneIn2To8,
  OneIn2To9,
  OneIn2To10,
  OneIn2To11,
  OneIn2To12,
  OneIn2To13,
  OneIn2To14,
  OneIn2To15
} MrcRhProbType;

typedef enum {
  MRC_POST_CODE,
  MRC_POST_CODE_WRITE,
  MRC_POST_CODE_READ,
  MRC_POST_CODE_MAX
} MrcDebugPostCode;

typedef struct {
  UINT32 MrcData;
  UINT32 Stream;
  UINT32 Start;
  UINT32 End;
  UINT32 Current;
  int Level;
  UINT16 PostCode[MRC_POST_CODE_MAX];
  UINT32 TopStackAddr;     ///< Initial stack address.
  UINT32 LowestStackAddr;  ///< Track the lowest stack address used through MrcPrintfVaList()
} MrcDebug;

typedef UINT16 MrcPostCode;
typedef UINT8  MrcClockRatio;  ///< This value times the MrcRefClkSelect determines the MrcFrequency.
typedef UINT32 MrcGfxDataSize; ///< The size of the stolen graphics data memory, in MBytes.
typedef UINT32 MrcGfxGttSize;  ///< The size of the graphics translation table, in MBytes.


///
/// This data structure contains all the "DDR power saving data" values that are considered output by the MRC.
/// The following are memory controller level definitions. All channels on a controller are set to these values.
///
typedef struct {
  BOOLEAN    BaseFlag;      ///< Indicates if the base line of power was already calculated.
  UINT16     BaseSavingRd;  ///< Indicates the base line of power consume by the ddr on read.
  UINT16     BaseSavingWr;  ///< Indicates the base line of power consume by the ddr on write.
  UINT16     BaseSavingCmd; ///< Indicates the base line of power consume by the ddr on command.
  UINT16     MrcSavingRd;   ///< Indicates the power consume by the ddr on read at the end of MRC.
  UINT16     MrcSavingWr;   ///< Indicates the power consume by the ddr on write at the end of MRC.
  UINT16     MrcSavingCmd;  ///< Indicates the power consume by the ddr on command at the end of MRC.
} MrcOdtPowerSaving;

///
/// The memory controller capabilities.
///
typedef union {
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MrcCapabilityIdA;

typedef union {
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MrcCapabilityIdB;

typedef union {
  UINT64 Data;
  struct {
    MrcCapabilityIdA A;
    MrcCapabilityIdB B;
  } Data32;
} MrcCapabilityId;

///
/// MRC version description.
///
typedef struct {
  UINT8  Major;  ///< Major version number
  UINT8  Minor;  ///< Minor version number
  UINT8  Rev;    ///< Revision number
  UINT8  Build;  ///< Build number
} MrcVersion;

///
/// Memory map configuration information.
///
typedef struct {
  UINT32     TomMinusMe;
  UINT32     ToludBase;
  UINT32     BdsmBase;
  UINT32     GttBase;
  UINT32     GraphicsControlRegister;
  UINT32     TsegBase;
  BOOLEAN    ReclaimEnable;
  UINT32     RemapBase;
  UINT32     RemapLimit;
  UINT32     TouudBase;
  UINT32     TotalPhysicalMemorySize;
  UINT32     MeStolenBase;
  UINT32     MeStolenSize;
  UINT32     GdxcMotBase;
  UINT32     GdxcMotSize;
  UINT32     GdxcIotBase;
  UINT32     GdxcIotSize;
  UINT32     DprSize;
  UINT32     PttStolenBase;
  UINT32     PrmrrBase;
  UINT32     LowestBase;
} MrcMemoryMap;

///
/// Real time clock information.
///
typedef struct {
  UINT8  Seconds;    ///< Seconds, 0-59
  UINT8  Minutes;    ///< Minutes, 0-59
  UINT8  Hours;      ///< Hours, 0-23
  UINT8  DayOfMonth; ///< Day of the month, 1-31
  UINT8  Month;      ///< Month of the year, 1-12
  UINT16 Year;       ///< Year, 0-65535
} MrcBaseTime;

///
/// DIMM timings
///
typedef struct {
  UINT32 tCK;     ///< Memory cycle time, in femtoseconds.
  UINT16 NMode;   ///< Number of tCK cycles for the channel DIMM's command rate mode.
  UINT16 tCL;     ///< Number of tCK cycles for the channel DIMM's CAS latency.
  UINT16 tCWL;    ///< Number of tCK cycles for the channel DIMM's minimum CAS write latency time.
  UINT16 tFAW;    ///< Number of tCK cycles for the channel DIMM's minimum four activate window delay time.
  UINT16 tRAS;    ///< Number of tCK cycles for the channel DIMM's minimum active to precharge delay time.
  UINT16 tRCDtRP; ///< Number of tCK cycles for the channel DIMM's minimum RAS# to CAS# delay time and Row Precharge delay time.
  UINT16 tREFI;   ///< Number of tCK cycles for the channel DIMM's minimum Average Periodic Refresh Interval.
  UINT16 tRFC;    ///< Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRFCpb;  ///< Number of tCK cycles for the channel DIMM's minimum per bank refresh recovery delay time.
  UINT16 tRFC2;   ///< Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRFC4;   ///< Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRPab;   ///< Number of tCK cycles for the channel DIMM's minimum row precharge delay time for all banks.
  UINT16 tRRD;    ///< Number of tCK cycles for the channel DIMM's minimum row active to row active delay time.
  UINT16 tRRD_L;  ///< Number of tCK cycles for the channel DIMM's minimum row active to row active delay time for same bank groups.
  UINT16 tRRD_S;  ///< Number of tCK cycles for the channel DIMM's minimum row active to row active delay time for different bank groups.
  UINT16 tRTP;    ///< Number of tCK cycles for the channel DIMM's minimum internal read to precharge command delay time.
  UINT16 tWR;     ///< Number of tCK cycles for the channel DIMM's minimum write recovery time.
  UINT16 tWTR;    ///< Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time.
  UINT16 tWTR_L;  ///< Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time for same bank groups.
  UINT16 tWTR_S;  ///< Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time for different bank groups.
  UINT16 tCCD_L;  ///< Number of tCK cycles for the channel DIMM's minimum CAS-to-CAS delay for same bank group.
} MrcTiming;

typedef struct {
  UINT8 SG;       ///< Number of tCK cycles between transactions in the same bank group.
  UINT8 DG;       ///< Number of tCK cycles between transactions when switching bank groups.
  UINT8 DR;       ///< Number of tCK cycles between transactions when switching between Ranks (in the same DIMM).
  UINT8 DD;       ///< Number of tCK cycles between transactions when switching between DIMMs
} MrcTurnaroundTimes;

typedef struct {
  INT32 Mtb;    ///< Medium time base.
  INT32 Ftb;    ///< Fine time base.
} MrcTimeBase;

typedef struct {
  UINT8  Left;   ///< The left side of the timing eye.
  UINT8  Center; ///< The center of the timing eye.
  UINT8  Right;  ///< The right side of the timing eye.
} MrcDqTimeMargin;

typedef struct {
  UINT8  High;   ///< The high side of the Vref eye.
  UINT8  Center; ///< The center of the Vref eye.
  UINT8  Low;    ///< The low side of the Vref eye.
} MrcDqVrefMargin;

typedef struct {
  UINT8  Left;   ///< The left side of the command eye.
  UINT8  Right;  ///< The right side of the command eye.
  UINT8  High;   ///< The high side of the command eye.
  UINT8  Low;    ///< The low side of the command eye.
} MrcCommandMargin;

typedef struct {
  UINT8  Left;   ///< The left side of the receive enable eye.
  UINT8  Right;  ///< The right side of the receive enableeye.
} MrcRecvEnMargin;

typedef struct {
  UINT8  Left;   ///< The left side of the write leveling eye.
  UINT8  Right;  ///< The right side of the write leveling eye.
} MrcWrLevelMargin;

typedef struct {
  UINT8     SpdValid[sizeof (MrcSpd) / (CHAR_BITS * sizeof (UINT8))]; ///< Each valid bit maps to SPD byte.
  UINT8     MrcSpdString[3]; ///< The SPD data start marker. This must be located at the start of the SPD data structure. It includes this string plus the following flag.
  union {
    struct {
      UINT8   DimmNumber    : 4; ///< SPD zero based DIMM number.
      UINT8   ChannelNumber : 3; ///< SPD zero based channel number.
      UINT8   MdSocket      : 1; ///< 0 = memory down, 1 = socketed.
    } Bit;
    UINT8 Data;
  } Flag;
  MrcSpd Data;            ///< The SPD data for each DIMM. SPDGeneral field = 0 when absent.
} MrcSpdData;

typedef UINT8        (*MRC_IO_READ_8)               (UINT32 IoAddress);
typedef UINT16       (*MRC_IO_READ_16)              (UINT32 IoAddress);
typedef UINT32       (*MRC_IO_READ_32)              (UINT32 IoAddress);
typedef void         (*MRC_IO_WRITE_8)              (UINT32 IoAddress, UINT8 Value);
typedef void         (*MRC_IO_WRITE_16)             (UINT32 IoAddress, UINT16 Value);
typedef void         (*MRC_IO_WRITE_32)             (UINT32 IoAddress, UINT32 Value);
typedef UINT8        (*MRC_MMIO_READ_8)             (UINT32 Address);
typedef UINT16       (*MRC_MMIO_READ_16)            (UINT32 Address);
typedef UINT32       (*MRC_MMIO_READ_32)            (UINT32 Address);
typedef UINT64       (*MRC_MMIO_READ_64)            (UINT32 Address);
typedef UINT8        (*MRC_MMIO_WRITE_8)            (UINT32 Address, UINT8 Value);
typedef UINT16       (*MRC_MMIO_WRITE_16)           (UINT32 Address, UINT16 Value);
typedef UINT32       (*MRC_MMIO_WRITE_32)           (UINT32 Address, UINT32 Value);
typedef UINT64       (*MRC_MMIO_WRITE_64)           (UINT32 Address, UINT64 Value);
typedef UINT8        (*MRC_SMBUS_READ_8)            (UINT32 Address, UINT32 *Status);
typedef UINT16       (*MRC_SMBUS_READ_16)           (UINT32 Address, UINT32 *Status);
typedef UINT8        (*MRC_SMBUS_WRITE_8)           (UINT32 Address, UINT8 Value, UINT32 *Status);
typedef UINT16       (*MRC_SMBUS_WRITE_16)          (UINT32 Address, UINT16 Value, UINT32 *Status);
typedef UINT32       (*MRC_GET_PCI_DEVICE_ADDRESS)  (UINT8 Bus, UINT8 Device, UINT8 Function, UINT8 Offset);
typedef UINT32       (*MRC_GET_PCIE_DEVICE_ADDRESS) (UINT8 Bus, UINT8 Device, UINT8 Function, UINT8 Offset);
typedef void         (*MRC_GET_RTC_TIME)            (UINT8 *Second, UINT8 *Minute, UINT8 *Hour, UINT8 *Day, UINT8 *Month, UINT16 *Year);
typedef UINT64       (*MRC_GET_CPU_TIME)            (void *MrcData);
typedef void *       (*MRC_MEMORY_COPY)             (UINT8 *Destination, UINT8 *Source, UINT32 NumBytes);
typedef void *       (*MRC_MEMORY_SET_BYTE)         (UINT8 *Destination, UINT32 NumBytes, UINT8 Value);
typedef void *       (*MRC_MEMORY_SET_WORD)         (UINT16 *Destination, UINT32 NumWords, UINT16 Value);
typedef void *       (*MRC_MEMORY_SET_DWORD)        (UINT32 *Destination, UINT32 NumDwords, UINT32 Value);
typedef UINT64       (*MRC_LEFT_SHIFT_64)           (UINT64 Data, UINT32 NumBits);
typedef UINT64       (*MRC_RIGHT_SHIFT_64)          (UINT64 Data, UINT32 NumBits);
typedef UINT64       (*MRC_MULT_U64_U32)            (UINT64 Multiplicand, UINT32 Multiplier);
typedef UINT64       (*MRC_DIV_U64_U64)             (UINT64 Dividend, UINT64 Divisor, UINT64 *Remainder);
typedef BOOLEAN      (*MRC_GET_SPD_DATA)            (UINT8 BootMode, UINT8 SpdAddress, UINT8 *SpdData, UINT8 *Ddr3Table, UINT32 Ddr3TableSize, UINT8 *Ddr4Table, UINT32 Ddr4TableSize, UINT8 *LpddrTable, UINT32 LpddrTableSize);
typedef BOOLEAN      (*MRC_GET_RANDOM_NUMBER)       (UINT32 *Rand);
typedef UINT32       (*MRC_CPU_MAILBOX_READ)        (UINT32 Type, UINT32 Command, UINT32 *Value, UINT32 *Status);
typedef UINT32       (*MRC_CPU_MAILBOX_WRITE)       (UINT32 Type, UINT32 Command, UINT32 Value, UINT32 *Status);
typedef UINT32       (*MRC_GET_MEMORY_VDD)          (void *MrcData, UINT32 DefaultVdd);
typedef UINT32       (*MRC_SET_MEMORY_VDD)          (void *MrcData, UINT32 DefaultVdd, UINT32 Value);
typedef UINT32       (*MRC_CHECKPOINT)              (void *MrcData, UINT32 CheckPoint, void *Scratch);
typedef void         (*MRC_DEBUG_HOOK)              (void *GlobalData, UINT16 DisplayDebugNumber);
typedef void         (*MRC_PRINT_STRING)            (void *String);
typedef UINT8        (*MRC_GET_RTC_CMOS)            (UINT8 Location);
typedef UINT64       (*MRC_MSR_READ_64)             (UINT32 Location);
typedef UINT64       (*MRC_MSR_WRITE_64)            (UINT32 Location, UINT64 Data);
typedef void         (*MRC_RETURN_FROM_SMC)         (void *GlobalData, UINT32 MrcStatus);
typedef void         (*MRC_DRAM_RESET)              (UINT32 PciEBaseAddress, UINT32 ResetValue);
typedef void         (*MRC_SET_LOCK_PRMRR)          (UINT32 PrmrrBase, UINT32 PrmrrSize);
typedef void         (*MRC_TXT_ACHECK)              (void);

///
/// Function calls that are called external to the MRC.
///   This structure needs to be aligned with SA_FUNCTION_CALLS.  All functions that are
///   not apart of SA_FUNCTION_CALLS need to be at the end of the structure.
///
typedef struct {
  MRC_IO_READ_8               MrcIoRead8;
  MRC_IO_READ_16              MrcIoRead16;
  MRC_IO_READ_32              MrcIoRead32;
  MRC_IO_WRITE_8              MrcIoWrite8;
  MRC_IO_WRITE_16             MrcIoWrite16;
  MRC_IO_WRITE_32             MrcIoWrite32;
  MRC_MMIO_READ_8             MrcMmioRead8;
  MRC_MMIO_READ_16            MrcMmioRead16;
  MRC_MMIO_READ_32            MrcMmioRead32;
  MRC_MMIO_READ_64            MrcMmioRead64;
  MRC_MMIO_WRITE_8            MrcMmioWrite8;
  MRC_MMIO_WRITE_16           MrcMmioWrite16;
  MRC_MMIO_WRITE_32           MrcMmioWrite32;
  MRC_MMIO_WRITE_64           MrcMmioWrite64;
  MRC_SMBUS_READ_8            MrcSmbusRead8;
  MRC_SMBUS_READ_16           MrcSmbusRead16;
  MRC_SMBUS_WRITE_8           MrcSmbusWrite8;
  MRC_SMBUS_WRITE_16          MrcSmbusWrite16;
  MRC_GET_PCI_DEVICE_ADDRESS  MrcGetPciDeviceAddress;
  MRC_GET_PCIE_DEVICE_ADDRESS MrcGetPcieDeviceAddress;
  MRC_GET_RTC_TIME            MrcGetRtcTime;
  MRC_GET_CPU_TIME            MrcGetCpuTime;
  MRC_MEMORY_COPY             MrcCopyMem;
  MRC_MEMORY_SET_BYTE         MrcSetMem;
  MRC_MEMORY_SET_WORD         MrcSetMemWord;
  MRC_MEMORY_SET_DWORD        MrcSetMemDword;
  MRC_LEFT_SHIFT_64           MrcLeftShift64;
  MRC_RIGHT_SHIFT_64          MrcRightShift64;
  MRC_MULT_U64_U32            MrcMultU64x32;
  MRC_DIV_U64_U64             MrcDivU64x64;
  MRC_GET_SPD_DATA            MrcGetSpdData;
  MRC_GET_RANDOM_NUMBER       MrcGetRandomNumber;
  MRC_CPU_MAILBOX_READ        MrcCpuMailboxRead;
  MRC_CPU_MAILBOX_WRITE       MrcCpuMailboxWrite;
  MRC_GET_MEMORY_VDD          MrcGetMemoryVdd;
  MRC_SET_MEMORY_VDD          MrcSetMemoryVdd;
  MRC_CHECKPOINT              MrcCheckpoint;
  MRC_DEBUG_HOOK              MrcDebugHook;
  MRC_PRINT_STRING            MrcPrintString;
  MRC_GET_RTC_CMOS            MrcRtcCmos;
  MRC_MSR_READ_64             MrcReadMsr64;
  MRC_MSR_WRITE_64            MrcWriteMsr64;
  MRC_RETURN_FROM_SMC         MrcReturnFromSmc;
  MRC_DRAM_RESET              MrcDramReset;
  MRC_SET_LOCK_PRMRR          MrcSetLockPrmrr;
  MRC_TXT_ACHECK              MrcTxtAcheck;
} MRC_FUNCTION;

///
///*****************************************
/// Output related "global data" structures.
///*****************************************
///
/// This data structure contains all the "global data" values that are considered output by the MRC.
/// The following are SDRAM level definitions. All ranks on a rank are set to these values.
///
/* Commented out until needed, in order to save space.
typedef struct {
} MrcSdramOut;
*/

///
/// This data structure contains all the "global data" values that are considered output by the MRC.
/// The following are rank level definitions. All ranks on a DIMM are set to these values.
///
typedef struct {
//MrcSdramOut     Sdram[MAX_SDRAM_IN_DIMM];         ///< The following are SDRAM level definitions.
  UINT16             MR[MAX_MR_IN_DIMM];            ///< DRAM mode register value.
  UINT16             MR11;                          ///< LPDDR3 ODT MR
  UINT8              Ddr4PdaMr6[MAX_SDRAM_IN_DIMM]; ///< DDR4 MR6[6:0] for per-DRAM VrefDQ (PDA)
#if (SUPPORT_DDR4 == SUPPORT)
  UINT8              Device[MAX_SDRAM_IN_DIMM];     ///< Which Bytes are tied to which Device where BIT0 set means Byte 0
#endif //SUPPORT_DDR4
} MrcRankOut;

///
/// This data structure contains all the "global data" values that are considered output by the MRC.
/// The following are DIMM level definitions. All ranks on a DIMM are set to these values.
///
typedef struct {
  MrcDimmSts     Status;                    ///< See MrcDimmSts for the definition of this field.
  MrcTiming      Timing[MAX_PROFILE];       ///< The DIMMs timing values.
  MrcVddSelect   VddVoltage[MAX_PROFILE];   ///< The voltage (VDD) setting for this DIMM, per profile.
  BOOLEAN        EccSupport;                ///< TRUE if ECC is enabled and supported on this DIMM.
  BOOLEAN        IgnoreNonEccDimm;          ///< TRUE if a DIMM without ECC capability should be ignored.
  BOOLEAN        AddressMirrored;           ///< TRUE if the DIMM is address mirrored.
  BOOLEAN        SelfRefreshTemp;           ///< TRUE if the DIMM supports self refresh extended operating temperature range (SRT).
  BOOLEAN        AutoSelfRefresh;           ///< TRUE if the DIMM supports automatic self refresh (ASR).
  BOOLEAN        PartialSelfRefresh;        ///< TRUE if the DIMM supports Partial Array Self Refresh (PASR).
  BOOLEAN        OnDieThermalSensor;        ///< TRUE if the DIMM supports On-die Thermal Sensor (ODTS) Readout.
  BOOLEAN        ExtendedTemperRange;       ///< TRUE if the DIMM supports Extended Temperature Range (ETR).
  BOOLEAN        ExtendedTemperRefresh;     ///< TRUE if the DIMM supports 1x Extended Temperature Refresh rate, FALSE = 2x.
  MrcDdrType     DdrType;                   ///< DDR type: DDR3 or LPDDR3
  MEMORY_PACKAGE ModuleType;                ///< Module type: UDIMM, SO-DIMM, etc.
  UINT32         SdramCount;                ///< The number of SDRAM components on a DIMM.
  UINT32         DimmCapacity;              ///< DIMM size in MBytes.
  UINT32         RowSize;                   ///< The DIMMs row address size.
  UINT16         ColumnSize;                ///< The DIMMs column address size.
  UINT16         Crc;                       ///< Calculated CRC16 of the DIMM's provided SPD. Can be used to detect DIMM change.
  UINT8          RankInDimm;                ///< The number of ranks in this DIMM.
  UINT8          Banks;                     ///< Number of banks the DIMM contains.
  UINT8          BankGroups;                ///< Number of bank groups the DIMM contains.
  UINT8          PrimaryBusWidth;           ///< DIMM primary bus width.
  UINT8          SdramWidth;                ///< DIMM SDRAM width.
  UINT8          SdramWidthIndex;           ///< DIMM SDRAM width index (0 = x4, 1 = x8, 2 = x16, 3 = x32).
  UINT8          DensityIndex;              ///< Total SDRAM capacity index (0 = 256Mb, 1 = 512Mb, 2 = 1Gb, etc).
  UINT8          tMAC;                      ///< Maximum Activate Count for pTRR.
  UINT8          ReferenceRawCard;          ///< Indicates which JEDEC reference design raw card was used as the basis for the module assembly.
  UINT8          ReferenceRawCardRevision;  ///< Indicates which JEDEC reference design raw card revision.
  UINT8          XmpSupport;                ///< Indicates if XMP profiles are supported. 0 = None, 1 = XMP1 only, 2 = XMP2 only, 3 = All.
  UINT8          XmpRevision;               ///< Indicates the XMP revision of this DIMM. 0 = None, 12h = 1.2, 13h = 1.3.
  MrcFrequency   Speed;                     ///< Max DIMM speed in the current profile - needed for SMBIOS.
  MrcRankOut     Rank[MAX_RANK_IN_DIMM];    ///< The following are rank level definitions.
} MrcDimmOut;

///
/// This data structure contains all the "global data" values that are considered output by the MRC.
/// The following are channel level definitions. All DIMMs on a memory channel are set to these values.
///
typedef struct {
  MrcChannelSts       Status;                                                       ///< Indicates whether this channel should be used.
  MrcVirtualChannel   VirtualChannel;                                               ///< define the virtual channel type A or B.
  MrcTiming           Timing[MAX_PROFILE];                                          ///< The channel timing values.
  MrcTimeBase         TimeBase[MAX_DIMMS_IN_CHANNEL][MAX_PROFILE];                  ///< Medium and fine timebases for each DIMM in the channel and each memory profile.
  UINT32              Capacity;                                                     ///< Amount of memory in this channel, in MBytes.
  UINT32              DimmCount;                                                    ///< Number of valid DIMMs that exist in the channel.
  UINT32              DataOffsetTrain[MAX_SDRAM_IN_DIMM];                           ///< DataOffsetTrain CR
  UINT32              DataCompOffset[MAX_SDRAM_IN_DIMM];                            ///< DataCompOffset CR
  UINT32              CkeCmdPiCode[MAX_COMMAND_GROUPS];                             ///< CKE  CmdPiCode CR, per group
  UINT32              CmdsCmdPiCode[MAX_COMMAND_GROUPS];                            ///< CmdS CmdPiCode CR, per group
  UINT32              CmdnCmdPiCode[MAX_COMMAND_GROUPS];                            ///< CmdN CmdPiCode CR, per group
  UINT16              TxDqs[MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM];                ///< TxDQS PI Code
  UINT16              TxDq[MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM];                 ///< TxDQ Pi Code
  UINT16              RcvEn[MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM];                ///< RcvEn PI Code
  UINT16              WlDelay[MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM];              ///< WlDelay PI Code
  UINT8               ClkPiCode[MAX_RANK_IN_CHANNEL];                               ///< Clk Pi Code
  UINT8               CtlPiCode[MAX_RANK_IN_CHANNEL];                               ///< Ctl Pi Code
  UINT8               CkePiCode[MAX_RANK_IN_CHANNEL];                               ///< Ctl Pi Code
  UINT8               TxEq[MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM];                 ///< TxEq setting
  MrcCommandMargin    Command[MAX_RANK_IN_CHANNEL];                                 ///< Cmd setting
  MrcDqTimeMargin     RxDqPb[MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS];     ///< Rx PerBit Pi Code
  MrcDqTimeMargin     TxDqPb[MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS];     ///< Tx PerBit Pi Code
  MrcDqVrefMargin     RxDqVrefPb[MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS]; ///< Rx PerBit Vref
  MrcDqVrefMargin     TxDqVrefPb[MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS]; ///< Rx PerBit Vref
  MrcRecvEnMargin     ReceiveEnable[MAX_RANK_IN_CHANNEL];                           ///< Receive enable per rank
  MrcWrLevelMargin    WriteLevel[MAX_RANK_IN_CHANNEL];                              ///< Write leveling per rank
  UINT8               IoLatency[MAX_RANK_IN_CHANNEL];                               ///< IOLatency
  UINT8               RTLatency[MAX_RANK_IN_CHANNEL];                               ///< RoundTripLatency
  UINT32              RTIoComp;                                                     ///< RoundTrip IO Compensation of the Channel
  UINT8               RxVref[MAX_SDRAM_IN_DIMM];                                    ///< RX Vref in steps of 7.9 mv
  UINT8               RxEq[MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM];                 ///< RxEQ Setting
  UINT8               RxDqsP[MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM];               ///< RxDQSP PI Code
  UINT8               RxDqsN[MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM];               ///< RxDQSN PI Code
  UINT8               ValidRankBitMask;                                             ///< Bit map of the populated ranks per channel
  UINT8               ValidCkeBitMask;                                              ///< Bit map of the used CKE pins per channel
  MrcDimmOut          Dimm[MAX_DIMMS_IN_CHANNEL];                                   ///< DIMM specific output variables.
  MrcTurnaroundTimes  tRd2Rd;                                                       ///< The system's minimal delay timings for Read  command followed by Read  command.
  MrcTurnaroundTimes  tRd2Wr;                                                       ///< The system's minimal delay timings for Read  command followed by Write command.
  MrcTurnaroundTimes  tWr2Rd;                                                       ///< The system's minimal delay timings for Write command followed by Read  command.
  MrcTurnaroundTimes  tWr2Wr;                                                       ///< The system's minimal delay timings for Write command followed by Write command.
} MrcChannelOut;

///
/// This data structure contains all the "global data" values that are considered output by the MRC.
/// The following are memory controller level definitions. All channels on a controller are set to these values.
///
typedef struct {
  MrcControllerSts Status;               ///< Indicates whether this controller should be used.
  UINT16           DeviceId;             ///< The PCI device id of this memory controller.
  UINT8            RevisionId;           ///< The PCI revision id of this memory controller.
  UINT8            ChannelCount;         ///< Number of valid channels that exist on the controller.
  MrcChannelOut    Channel[MAX_CHANNEL]; ///< The following are channel level definitions.
} MrcControllerOut;

///
///********************************************
/// Saved data related "global data" structures.
///********************************************
///

///
/// This data structure contains all the "global data" values that are considered to be needed
/// by the MRC between power state transitions (S0->S3->S0) and also fast and warm boot modes.
/// The following are DIMM level definitions.
///
typedef struct {
  UINT8 SpdDramDeviceType;       ///< Save SPD DramDeviceType information needed for SMBIOS structure creation.
  UINT8 SpdModuleType;           ///< Save SPD ModuleType information needed for SMBIOS structure creation.
  UINT8 SpdModuleMemoryBusWidth; ///< Save SPD ModuleMemoryBusWidth information needed for SMBIOS structure creation.
  UINT8 SpdSave[MAX_SPD_SAVE];   ///< Save SPD Manufacturing information needed for SMBIOS structure creation.
} MrcDimmSave;

///
/// This data structure contains all the "global data" values that are considered to be needed
/// by the MRC between power state transitions (S0->S3->S0) and also fast and warm boot modes.
/// The following are channel level definitions.
///
typedef struct {
  MrcChannelSts Status;                               ///< Indicates whether this channel should be used.
  UINT32        DimmCount;                            ///< Number of valid DIMMs that exist in the channel.
  UINT8         ValidRankBitMask;                     ///< Bit map of the populated ranks per channel
  MrcTiming     Timing[MAX_PROFILE];                  ///< The channel timing values.
  MrcDimmOut    Dimm[MAX_DIMMS_IN_CHANNEL];           ///< Save the DIMM output characteristics.
  MrcDimmSave   DimmSave[MAX_DIMMS_IN_CHANNEL];       ///< Save SPD information needed for SMBIOS structure creation.
} MrcChannelSave;

///
/// This data structure contains all the "global data" values that are considered to be needed
/// by the MRC between power state transitions (S0->S3->S0) and also fast and warm boot modes.
/// The following are controller level definitions.
///
typedef struct {
  MrcControllerSts  Status;               ///< Indicates whether this controller should be used.
  UINT8             ChannelCount;         ///< Number of valid channels that exist on the controller.
  MrcChannelSave    Channel[MAX_CHANNEL]; ///< The following are channel level definitions.
} MrcContSave;

///
/// This data structure contains all the "global data" values that are considered to be needed
/// by the MRC between power state transitions (S0->S3->S0) and also fast and warm boot modes.
/// The following are system level definitions.
///
typedef struct {
  UINT32 Crc;                  ///< The CRC-32 of the data in this structure.
} MrcSaveHeader;

//
// ------- IMPORTANT NOTE --------
// MRC_MC_REGISTER_COUNT in MrcInterface.h should match the table in MrcSaveRestore.c.
// Update this define whenever you add/remove registers from this table.
//
#define MRC_REGISTER_COUNT_COMMON (1376 / sizeof (UINT32)) ///< The number of MC registers that need to be saved (common)
#define MRC_REGISTER_COUNT_SAGV   (1528 / sizeof (UINT32)) ///< The number of MC registers that need to be saved (per SA GV point)

typedef struct {
  MrcCapabilityId McCapId;                                   ///< The memory controller's capabilities.
  UINT32          RegSaveCommon[MRC_REGISTER_COUNT_COMMON];  ///< The MC registers that are common to both SA GV points
  UINT32          RegSaveLow[MRC_REGISTER_COUNT_SAGV];       ///< The MC registers for the Low SA GV point
  UINT32          RegSaveHigh[MRC_REGISTER_COUNT_SAGV];      ///< The MC registers for the High SA GV point, or for SA GV Disabled case
  UINT32          MeStolenSize;                              ///< The managebility engine memory size, in Mbyte units.
  MrcCpuStepping  CpuStepping;                               ///< The last cold boot happended with this CPU stepping.
  MrcCpuModel     CpuModel;                                  ///< The last cold boot happended with this CPU model.
  MrcCpuFamily    CpuFamily;                                 ///< CPU is Coffeelake
  MrcVersion      Version;                                   ///< The last cold boot happended with this MRC version.
  UINT32          SaMemCfgCrc;                               ///< The CRC32 of the system agent memory configuration structure.
  MrcContSave     Controller[MAX_CONTROLLERS];               ///< The following are controller level definitions.
  MrcFrequency    FreqMax;                                   ///< The system's requested maximum frequency.
  MrcFrequency    Frequency;                                 ///< The system's common memory controller frequency.
  UINT32          MemoryClock;                               ///< The system's common memory controller clock, in femtoseconds.
  BOOLEAN         OddRatioModeLow;                           ///< If Odd Ratio Mode is enabled, QCLK frequency has an addition of 133/100 MHz. This is for SAGV Low point.
  BOOLEAN         OddRatioModeHigh;                          ///< If Odd Ratio Mode is enabled, QCLK frequency has an addition of 133/100 MHz. This is for SAGV High point, or SAGV disabled / fixed high / fixed low
  MrcRefClkSelect RefClk;                                    ///< The memory controller is going to use this reference clock.
  MrcClockRatio   Ratio;                                     ///< Request for this memory controller to use this clock ratio.
  MrcVddSelect    VddVoltage[MAX_PROFILE];                   ///< The voltage (VDD) setting for all DIMMs in the system, per profile.
  BOOLEAN         EccSupport;                                ///< TRUE if ECC is enabled and supported on this controller.
  MrcDdrType      DdrType;                                   ///< DDR type: DDR3, DDR4, or LPDDR3
  UINT32          DefaultXmptCK[MAX_PROFILE - XMP_PROFILE1]; ///< The Default XMP tCK values read from SPD.
  UINT8           XmpProfileEnable;                          ///< If XMP capable DIMMs are detected, this will indicate which XMP Profiles are common among all DIMMs.
  BOOLEAN         BinnedLpddrDevices;                        ///< Binned LPDDR3 devices (6Gb/12Gb/etc)
  BOOLEAN         TCRSensitiveHynixDDR4;                     ///< TCR sensitive Hynix DDR4 in the system
  BOOLEAN         TCRSensitiveMicronDDR4;                    ///< TCR sensitive Micron DDR4 in the system
  BOOLEAN         LpddrEctDone;                       ///< Set to TRUE once Early Command Training on LPDDR is done, and we can run JEDEC Init
  UINT8           BerEnable;                          ///< BER Enable (and # of Addresses)
  UINT64          BerAddress[4];                      ///< BER Addresses
  BOOLEAN         DmfcLimitedBoard;                   ///< Indicates if the system has 2DPC Memory Configuration which should be DMFC limited.
  BOOLEAN         DmfcLimited;                        ///< Indicates if the system has DMFC limited. Should only be set if DmfcLImitedBoard is TRUE.
  BOOLEAN         MixedUDimmConfig2Dpc;               ///< Indicates if the system has 2DPC Memory Configuration with Mixed U-DIMM Part Numbers
  BOOLEAN         ExtendedDdrOverclock;               ///< Indicates if MC is capable of extended Overclock Memory frequencies.
} MrcSaveData;

typedef struct {
  UINT32              Size;                        ///< The size of this structure, in bytes. Must be the first entry in this structure.
  MrcDebug            Debug;                       ///< MRC debug related variables used for serial output and debugging purposes.
  MrcVersion          Version;                     ///< The memory reference code version.
  MrcFrequency        FreqMax;                     ///< The requested maximum valid frequency.
  MrcFrequency        Frequency;                   ///< The system's common memory controller frequency.
  UINT32              MemoryClockMax;              ///< The system's common memory controller maximum clock, in femtoseconds.
  UINT32              MemoryClock;                 ///< The system's common memory controller clock, in femtoseconds.
  MrcRefClkSelect     RefClk;                      ///< The memory controller is going to use this reference clock.
  MrcClockRatio       Ratio;                       ///< Request for this memory controller to use this clock ratio.
  MrcMemoryMap        MemoryMapData;               ///< The system's memory map data.
  MrcGfxDataSize      GraphicsStolenSize;          ///< Graphics Data Stolen Memory size in MB
  MrcGfxGttSize       GraphicsGttSize;             ///< GTT graphics stolen memory size in MB
  MrcVddSelect        VddVoltage[MAX_PROFILE];     ///< The currently running voltage (VDD) setting for all DIMMs in the system, per profile.
  MrcGdxc             Gdxc;                        ///< GDXC enable and size.
  BOOLEAN             VddVoltageDone;              ///< To determine if VddVoltageDone update has been done already
  BOOLEAN             EccSupport;                  ///< TRUE if ECC is enabled and supported on this controller.
  BOOLEAN             EnDumRd;                     ///< Enable/Disable Logic Analyzer
  BOOLEAN             RestoreMRs;                  ///< Enable/Disable restoring
  BOOLEAN             LpddrEctDone;                ///< Set to TRUE once Early Command Training on LPDDR is done, and we can run JEDEC Init
  BOOLEAN             LpddrWLUpdated;              ///< Set to TRUE once LPDDR WL Memory Set has been updated
  BOOLEAN             JedecInitDone;               ///< Set to TRUE once JEDEC Init on LPDDR/DDR4 is done
  UINT32              DefaultXmptCK[MAX_PROFILE - XMP_PROFILE1];            ///< The Default XMP tCK values read from SPD.
  UINT8               XmpProfileEnable;            ///< If XMP capable DIMMs are detected, this will indicate which XMP Profiles are common among all DIMMs.
  BOOLEAN             Capable100;                  ///< The MC is capable of 100 reference clock (0 = no, 1 = yes).
  BOOLEAN             AutoSelfRefresh;             ///< Indicates ASR is supported for all the DIMMS for 2xRefresh
  MrcDdrType          DdrType;                     ///< Current memory type: DDR3, DDR4, or LPDDR3
  MrcSpdStatus        SpdSecurityStatus;           ///< Status variable to inform BIOS that memory contains an alias.
  UINT32              MrcTotalChannelLimit;        ///< The maximum allowed memory size per channel, in MBytes.
  UINT8               SdramCount;                  ///< The number of SDRAM components on a DIMM.
  UINT16              Qclkps;                      ///< Qclk period in pS
  UINT8               DQPat;                       ///< Global Variables storing the current DQPat REUT Test
  INT8                DQPatLC;                     ///< Global Variables storing the current DQPat Loopcount
  UINT16              NumCL;                       ///< Global Variables storing the current number of Cachelines
  UINT8               ValidRankMask;               ///< Rank bit map - includes both channels
  UINT8               ValidChBitMask;              ///< Channel bit map of the populated channels
  BOOLEAN             UpmPwrRetrainFlag;           ///< A flag that indicates if training with higher UPM/PWR limits.
  UINT32              MarginResult[MAX_RESULT_TYPE][MAX_RANK_IN_CHANNEL][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_EDGES]; ///< Stores last margin measurement.
  BOOLEAN             MarginSignReversed[MAX_RANK_IN_CHANNEL][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_EDGES]; ///< Indicates if the Margin Sign is Reversed
  MrcOdtPowerSaving   OdtPowerSavingData;          ///< ODT power savings data.
  BOOLEAN             TxDIMMVref[MAX_CHANNEL];     ///< Whether Write DIMM Vref is enabled based on Channel
  UINT32              MchBarWriteCount;            ///< The number of MMIO writes performed during MRC execution.
  UINT32              MchBarReadCount;             ///< The number of MMIO reads performed during MRC execution.
  UINT8               BerEnable;                   ///< BER Enable (and # of Addresses)
  UINT64              BerAddress[4];               ///< BER Addresses
  UINT8               CmdVLoop;                    ///< Keeps track of the # of CmdV training step runned
  UINT8               CmdVLoopStatus;              ///< Keeps the last status of the CmdV training step
  UINT8               tMAC;                        ///< Maximum Activate Count for pTRR.
  UINT8               LpddrMemWriteLatencySet;     ///< 0 = Set A (WL), 1 = Set B (WL) if supported
  BOOLEAN             Ddr4PdaEnable;               ///< Current status of PDA - if true all the Mr6 operations need to use PDA mode.
  BOOLEAN             BinnedLpddrDevices;          ///< Binned LPDDR3 devices (6Gb/12Gb/etc)
  MrcControllerOut    Controller[MAX_CONTROLLERS]; ///< The following are controller level definitions.
  BOOLEAN             TCRSensitiveHynixDDR4;       ///< TCR sensitive Hynix DDR4 in the system
  BOOLEAN             TCRSensitiveMicronDDR4;      ///< TCR sensitive Micron DDR4 in the system
  BOOLEAN             OddRatioMode;                ///< If Odd Ratio Mode is enabled, QCLK frequency has an addition of 133/100 MHz
  BOOLEAN             LpddrDramOdt;                ///< Indicates if LPDDR DRAM ODT is used - Only used for 2133+
  BOOLEAN             ExtendedDdrOverclock;        ///< Indicates if MC is capable of extended Overclock Memory frequencies.
  BOOLEAN             DmfcLimitedBoard;            ///< Indicates if the system has 2DPC Memory Configuration which should be DMFC limited.
  BOOLEAN             DmfcLimited;                 ///< Indicates if the system has DMFC has been limited. Should only be set if DmfcLImitedBoard is TRUE.
  BOOLEAN             MixedUDimmConfig2Dpc;        ///< Indicates if the system has 2DPC Memory Configuration with Mixed U-DIMM Part Numbers
#ifdef BDAT_SUPPORT
  union {
    MRC_BDAT_SCHEMA_LIST_HOB *Pointer;             ///< Pointer to the BDAT schema list.
    UINT64                   Data;
  } BdatSchemasHob;
  union {
    BDAT_MEMORY_DATA_HOB *Pointer;                 ///< Pointer to the BDAT memory data HOB.
    UINT64               Data;
  } BdatMemoryHob[MAX_SCHEMA_LIST_LENGTH];
  UINT8               Margin2DResult[MAX_2D_EYE_TYPE][MAX_RANK_IN_CHANNEL][MAX_CHANNEL][MAX_2D_EYE_OFFSETS][MAX_EDGES]; ///< Stores the 2D Eye Margin
#endif

#ifdef UP_SERVER_FLAG
  UINT8              ThermOffset[MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];                        ///< TSOD Thermal Offset
#endif
} MrcOutput;

///
///****************************************
/// Input related "global data" structures.
///****************************************
///
/// This data structure contains all the "global data" values that are considered input by the MRC.
/// The following are SDRAM level definitions. All ranks on a rank are set to these values.
///
/* Commented out until needed, in order to save space.
typedef struct {
  UINT8  Placeholder;  ///< TODO: Is there anything that needs to go in here?
} MrcSdramIn;
*/

///
/// This data structure contains all the "global data" values that are considered input by the MRC.
/// The following are rank level definitions. All ranks on a DIMM are set to these values.
///
/* Commented out until needed, in order to save space.
typedef struct {
  MrcSdramIn  Sdram[MAX_SDRAM_IN_DIMM]; ///< The following are SDRAM level definitions.
} MrcRankIn;
*/

///
/// This data structure contains all the "global data" values that are considered input by the MRC.
/// The following are DIMM level definitions. All ranks on a DIMM are set to these values.
///
typedef struct {
  MrcDimmSts  Status;                 ///< Indicates whether this DIMM should be used.
  MrcSpdData  Spd;                    ///< The SPD data for each DIMM. SPDGeneral field = 0 when absent.
  MrcTiming   Timing;                 ///< The DIMMs requested timing overrides.
  UINT8       SpdAddress;             ///< The SMBus address for the DIMM's SPD data.
//MrcRankIn   Rank[MAX_RANK_IN_DIMM]; ///< The following are rank level definitions.
} MrcDimmIn;

///
/// This data structure contains all the "global data" values that are considered input by the MRC.
/// The following are channel level definitions. All DIMMs on a memory channel are set to these values.
///
typedef struct {
  MrcChannelSts Status;                        ///< Indicates whether this channel should be used.
  UINT32        DimmCount;                     ///< The maximum number of DIMMs on this channel.
  MrcDimmIn     Dimm[MAX_DIMMS_IN_CHANNEL];    ///< The following are DIMM level definitions.
  UINT8         DqsMapCpu2Dram[8];             ///< Mapping from CPU DQS pins to SDRAM DQS pins
  UINT8         DqMapCpu2Dram[8][MAX_BITS];    ///< Mapping from CPU DQ pins to SDRAM DQ pins
  UINT8         DQByteMap[MrcIterationMax][2]; ///< Maps which PI clocks are used by what LPDDR DQ Bytes (from CPU side), per group
                                               ///< DQByteMap[0] - ClkDQByteMap:
                                               ///<   If clock is per rank, program to [0xFF, 0xFF]
                                               ///<   If clock is shared by 2 ranks, program to [0xFF, 0] or [0, 0xFF]
                                               ///<   If clock is shared by 2 ranks but does not go to all bytes,
                                               ///<           Entry[i] defines which DQ bytes Group i services
                                               ///< DQByteMap[1] - CmdNDQByteMap: Entry[0] is CmdN/CAA and Entry[1] is CmdN/CAB
                                               ///< DQByteMap[2] - CmdSDQByteMap: Entry[0] is CmdS/CAA and Entry[1] is CmdS/CAB
                                               ///< DQByteMap[3] - CkeDQByteMap : Entry[0] is CKE /CAA and Entry[1] is CKE /CAB
                                               ///<                For DDR, DQByteMap[3:1] = [0xFF, 0]
                                               ///< DQByteMap[4] - CtlDQByteMap : Always program to [0xFF, 0] since we have 1 CTL / rank
                                               ///<                               Variable only exists to make the code easier to use
                                               ///< DQByteMap[5] - CmdVDQByteMap: Always program to [0xFF, 0] since we have 1 CA Vref
                                               ///<                               Variable only exists to make the code easier to use
} MrcChannelIn;

///
/// This data structure contains all the "global data" values that are considered input by the MRC.
/// The following are memory controller level definitions. All channels on a controller are set to these values.
///
typedef struct {
  MrcControllerSts  Status;               ///< Indicates whether this controller should be used.
  UINT8             ChannelCount;         ///< Number of valid channels that are requested on the controller.
  MrcChannelIn      Channel[MAX_CHANNEL]; ///< The following are channel level definitions.
} MrcControllerIn;

/// This data structure contains all the "global data" values that are considered input by the MRC.
/// The following are system level definitions. All memory controllers in the system are set to these values.
typedef struct {
  // Start of synchronization to the SA MEMORY_CONFIGURATION structure.
  // Alignment of this block must be maintained and field offsets must match.
  UINT8   Header[28];             ///< Offset 0-27 Config Block Header
  UINT16  Size;                   ///< Offset 28 The size of this structure, in bytes. Must be the first entry in this structure.
  UINT8   HobBufferSize;          ///< Offset 30 Size of HOB buffer
  UINT8   MemoryProfile;          ///< Offset 31 SPD XMP profile selection - for XMP supported DIMM: <b>0=Default DIMM profile</b>, 1=Customized profile, 2=XMP profile 1, 3=XMP profile 2.
  // The following parameters are used only when MemoryProfile is UserDefined (CUSTOM PROFILE)
  UINT16  tCL;                    ///< Offset 32 User defined Memory Timing tCL value,   valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, 31=Maximum.
  UINT16  tRCDtRP;                ///< Offset 34 User defined Memory Timing tRCD value (same as tRP), valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, 63=Maximum.
  UINT16  tRAS;                   ///< Offset 36 User defined Memory Timing tRAS value,  valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, 64=Maximum.
  UINT16  tWR;                    ///< Offset 38 User defined Memory Timing tWR value,   valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, legal values: 5, 6, 7, 8, 10, 12, 14, 16, 18, 20, 24.
  UINT16  tRFC;                   ///< Offset 40 User defined Memory Timing tRFC value,  valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, 1023=Maximum.
  UINT16  tRRD;                   ///< Offset 42 User defined Memory Timing tRRD value,  valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, 15=Maximum.
  UINT16  tWTR;                   ///< Offset 44 User defined Memory Timing tWTR value,  valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, 28=Maximum.
  UINT16  tRTP;                   ///< Offset 46 User defined Memory Timing tRTP value,  valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, 15=Maximum. DDR4 legal values: 5, 6, 7, 8, 9, 10, 12
  UINT16  tFAW;                   ///< Offset 48 User defined Memory Timing tFAW value,  valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, 63=Maximum.
  UINT16  tCWL;                   ///< Offset 50 User defined Memory Timing tCWL value,  valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, 20=Maximum.
  UINT16  tREFI;                  ///< Offset 52 User defined Memory Timing tREFI value, valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, 65535=Maximum.
  UINT16  PciIndex;               ///< Offset 54 Pci index register address: <b>0xCF8=Default</b>
  UINT16  PciData;                ///< Offset 56 Pci data register address: <b>0xCFC=Default</b>
  UINT16  VddVoltage;             ///< Offset 58 DRAM voltage (Vdd) in millivolts: <b>0=Platform Default (no override)</b>, 1200=1.2V, 1350=1.35V etc.
  UINT16  Idd3n;                  ///< Offset 60 EPG Active standby current (Idd3N) in milliamps from DIMM datasheet.
  UINT16  Idd3p;                  ///< Offset 62 EPG Active power-down current (Idd3P) in milliamps from DIMM datasheet.

  UINT32  EccSupport:1;                   ///< Offset 64 DIMM Ecc Support option - for Desktop only: 0=Disable, <b>1=Enable</b>
  UINT32  MrcSafeConfig:1;                ///<  - MRC Safe Mode: <b>0=Disable</b>, 1=Enable
  UINT32  RemapEnable:1;                  ///<  - This option is used to control whether to enable/disable memory remap above 4GB: 0=Disable, <b>1=Enable</b>.
  UINT32  ScramblerEnable:1;              ///<  - Memory scrambler support: 0=Disable, <b>1=Enable</b>
  UINT32  Vc1ReadMeter:1;                 ///<  - VC1 Read Metering Enable: 0=Disable, <b>1=Enable</b>
  UINT32  DdrThermalSensor:1;             ///<  - Ddr Thermal Sensor: 0=Disable, <b>1=Enable</b>
  UINT32  LpddrMemWriteLatencySet:1;      ///<  - LPDDR3 Write Latency Set option: 0=Set A, <b>1=Set B</b>
  UINT32  Off64Bits7to8Rsvd:2;            ///<  - Bits 7-8 Reserved
  UINT32  SimicsFlag:1;                   ///<  - Option to Enable SIMICS: 0=Disable, <b>1=Enable</b>
  UINT32  Ddr4DdpSharedClock:1;           ///<  - New Select if CLK0 is shared between Rank0 and Rank1 in DDR4 DDP package. <b>0=Not shared</b>, 1=Shared
  UINT32  Ddr4DdpSharedZq:1;              ///<  - Select if ZQ pin is shared between Rank0 and Rank1 in DDR4 DDP package. <b>0=Not shared</b>, 1=Shared
  // Thermal Management
  UINT32  ThermalManagement:1;            ///<  - Memory Thermal Management Support: <b>0=Disable</b>, 1=Enable.
  UINT32  PeciInjectedTemp:1;             ///<  - Enable/Disable memory temperatures to be injected to the processor via PECI: <b>0=Disable</b>, 1=Enable.
  UINT32  ExttsViaTsOnBoard:1;            ///<  - Enable/Disable routing TS-on-Board's ALERT# and THERM# to EXTTS# pins on the PCH: <b>0=Disable</b>, 1=Enable.
  UINT32  ExttsViaTsOnDimm:1;             ///<  - Enable/Disable routing TS-on-DIMM's ALERT# to EXTTS# pin on the PCH: <b>0=Disable</b>, 1=Enable.
  UINT32  VirtualTempSensor:1;            ///<  - Enable/Disable Virtual Temperature Sensor (VTS): <b>0=Disable</b>, 1=Enable.
  UINT32  Lp4DqsOscEn:1;                  ///<  - DqDqsReTraining support: 0=Disable, <b>1=Enable</b>
  UINT32  DualDimmPerChannelBoardType:1;  ///<  - DualDimmPerChannelBoardType: Option to indicate if the Memory Design for the board includes two DIMMs per channel: <b>0=Single DIMM Design</b>, 1=Dual DIMM Design
  UINT32  ReservedBits1:13;
  /**
   Disables a DIMM slot in the channel even if a DIMM is present\n
   Array index represents the channel number (0 = channel 0, 1 = channel 1)\n
     <b>0x0 = DIMM 0 and DIMM 1 enabled</b>\n
     0x1 = DIMM 0 disabled, DIMM 1 enabled\n
     0x2 = DIMM 0 enabled, DIMM 1 disabled\n
     0x3 = DIMM 0 and DIMM 1 disabled (will disable the whole channel)\n
  **/
  UINT8   DisableDimmChannel[MAX_CHANNEL];  ///< Offset 68
  /**
   Selects the ratio to multiply the reference clock by for the DDR frequency\n
   When RefClk is 133MHz\n
   <b>0x00 = Auto</b>, 0x03 through 0x0C are valid values, all others are invalid\n
   When RefClk is 100MHz\n
   <b>0x00 = Auto</b>, 0x06 through 0x10 are valid values, all others are invalid\n
  **/
  UINT8   Ratio;                  ///< Offset 70
  UINT8   ProbelessTrace;         ///< Offset 71 Probeless Trace: <b>0=Disabled</b>, <b>1=Enabled</b>
  UINT32  BClkFrequency;          ///< Offset 72 Base reference clock value, in Hertz: <b>100000000 = 100Hz</b>, 125000000=125Hz, 167000000=167Hz, 250000000=250Hz
  /**
     - Channel Hash Enable.\n
    NOTE: BIT7 will interleave the channels at a 2 cacheline granularity, BIT8 at 4 and BIT9 at 8\n
    0=BIT6, <B>1=BIT7</B>, 2=BIT8, 3=BIT9
  **/
  UINT8   ChHashInterleaveBit;    ///< Offset 76
  UINT8   EnergyScaleFact;        ///< Offset 77 - Energy Scale Factor. 0=Minimal, 7=Maximum, <b>4=Default</b>
  UINT8   Reserved0;              ///< Offset 78 - Reserved for future use.
  UINT8   McLock;                 ///< Offset 79 - Enable/Disable memory configuration register locking: 0=Disable, <b>1=Enable</b>.
  // Training Algorithms
  TrainingStepsEn TrainingEnables;    ///< Offset 80 - Options to Enable individual training steps
  TrainingStepsEn2 TrainingEnables2;  ///< Offset 84 - Options to Enable individual training steps

  UINT32  OddRatioMode:1;             ///< Offset 88 - If Odd Ratio Mode is enabled, QCLK frequency has an addition of 133/100 MHz: <b>0=Disable</b>, 1=Enable
  UINT32  MrcTimeMeasure:1;           ///< - Enables serial debug level to display the MRC execution times only: <b>0=Disable</b>, 1=Enable
  UINT32  MrcFastBoot:1;              ///< - Enables the MRC fast boot path for faster cold boot execution: 0=Disable, <b>1=Enable</b>
  UINT32  DqPinsInterleaved:1;        ///< - Interleaving mode of DQ/DQS pins for HSW_ULT which depends on board routing: <b>0=Disable</b>, 1=Enable
  UINT32  RankInterleave:1;           ///< - Rank Interleave Mode: 0=Disable, <b>1=Enable</b>
  UINT32  EnhancedInterleave:1;       ///< - Enhanced Interleave Mode: 0=Disable, <b>1=Enable</b>
  UINT32  WeaklockEn:1;               ///< - Weak Lock Enable: 0=Disable, <b>1=Enable</b>
  UINT32  CmdTriStateDis:1;           ///< - CMD Tri-State Support: <b>0=Enable</b>, 1=Disable. Note: This should be set to 1 (Disable) if Command RTT is not present on the platform.
  UINT32  MemoryTrace:1;              ///< - Memory Trace to second DDR channel using Stacked Mode: <b>0=Disable</b>, 1=Enable
  UINT32  ChHashEnable:1;             ///< - Channel Hash Enable: 0=Disable, <b>1=Enable</b>
  UINT32  EnableExtts:1;              ///< - Enable Extts: <b>0=Disable</b>, 1=Enable
  UINT32  EnableCltm:1;               ///< - Enable Closed Loop Thermal Management: <b>0=Disable</b>, 1=Enable
  UINT32  EnableOltm:1;               ///< - Enable Open Loop Thermal Management: <b>0=Disable</b>, 1=Enable
  UINT32  EnablePwrDn:1;              ///< - Enable Power Down control for DDR: 0=PCODE control, <b>1=BIOS control</b>
  UINT32  EnablePwrDnLpddr:1;         ///< - Enable Power Down for LPDDR: 0=PCODE control, <b>1=BIOS control</b>
  UINT32  LockPTMregs:1;              ///< - Lock PCU Thermal Management registers: 0=Disable, <b>1=Enable</b>
  UINT32  UserPowerWeightsEn:1;       ///< - Allows user to explicitly set power weight, scale factor, and channel power floor values: <b>0=Disable</b>, 1=Enable
  UINT32  RaplLim2Lock:1;             ///< - Lock DDR_RAPL_LIMIT register: <b>0=Disable</b>, 1=Enable
  UINT32  RaplLim2Ena:1;              ///< - Enable Power Limit 2: <b>0=Disable</b>, 1=Enable
  UINT32  RaplLim1Ena:1;              ///< - Enable Power Limit 1: <b>0=Disable</b>, 1=Enable
  UINT32  SrefCfgEna:1;               ///< - Enable Self Refresh: 0=Disable, <b>1=Enable</b>
  UINT32  ThrtCkeMinDefeatLpddr:1;    ///< - Throttler CKE min defeature for LPDDR: 0=Disable, <b>1=Enable</b>
  UINT32  ThrtCkeMinDefeat:1;         ///< - Throttler CKE min defeature: <b>0=Disable</b>, 1=Enable
  UINT32  AutoSelfRefreshSupport:1;   ///< - FALSE = No auto self refresh support, <b>TRUE = auto self refresh support</b>
  UINT32  ExtTemperatureSupport:1;    ///< - FALSE = No extended temperature support, <b>TRUE = extended temperature support</b>
  UINT32  MobilePlatform:1;           ///< - Memory controller device id indicates: <b>TRUE if mobile</b>, FALSE if not. Note: This will be auto-detected and updated.
  UINT32  Force1Dpc:1;                ///< - TRUE means force one DIMM per channel, <b>FALSE means no limit</b>
  UINT32  ForceSingleRank:1;          ///< - TRUE means use Rank0 only (in each DIMM): <b>0=Disable</b>, 1=Enable
  UINT32  RhPrevention:1;             ///< - RH Prevention Enable/Disable: 0=Disable, <b>1=Enable</b>
  UINT32  VttTermination:1;           ///< - Vtt Termination for Data ODT: <b>0=Disable</b>, 1=Enable
  UINT32  VttCompForVsshi:1;          ///< - Enable/Disable Vtt Comparator For Vsshi: <b>0=Disable</b>, 1=Enable
  UINT32  ExitOnFailure:1;            ///< - MRC option for exit on failure or continue on failure: 0=Disable, <b>1=Enable</b>

  UINT32  VddSettleWaitTime;      ///< Offset 92 - Amount of time in microseconds to wait for Vdd to settle on top of 200us required by JEDEC spec: <b>Default=0</b>
  UINT16  FreqSaGvLow;            ///< Offset 96 - SA GV: 0 is Auto/default, otherwise holds the frequency value: <b>0=Default</b>, 1067, 1200, 1333, 1400, 1600, 1800, 1867.
  UINT16  SrefCfgIdleTmr;         ///< Offset 98 - Self Refresh idle timer: <b>512=Minimal</b>, 65535=Maximum
  UINT8   RhActProbability;       ///< Offset 100 - Activation probability for Hardware RHP
  UINT8   SmramMask;              ///< Offset 101 - Reserved memory ranges for SMRAM
  UINT16  Vc1ReadMeterThreshold;  ///< Offset 102 - VC1 Read Meter Threshold (within Time Window): 0=Minimal, 0xFFFF=Maximum, <b>0x118=Default</b>
  UINT32  Vc1ReadMeterTimeWindow; ///< Offset 104 - VC1 Read Meter Time Window: 0=Minimal, 0x1FFFF=Maximum, <b>0x320=Default</b>
  UINT64  BerAddress[4];          ///< Offset 108 - 139 BER Address(es): <b>0=Minimal</b>, 0xFFFFFFFFFFFFFFFF=Maximum (step is 0x40)

  UINT16  ChHashMask;             ///< Offset 140 - Channel Hash Mask: 0x0001=BIT6 set(Minimal), 0x3FFF=BIT[19:6] set(Maximum), <b>0x30CE= BIT[19:18, 13:12 ,9:7] set</b>
  UINT16  DdrFreqLimit;           ///< Offset 142 - Memory Frequency Limit: <b>0=Auto (limited by SPD/CPU capability)</b>, for valid values see MrcFrequency in MrcInterface.h
  ThermalMngmtEn  ThermalEnables; ///< Offset 144 - 187

  UINT8   MaxRttWr;               ///< Offset 188 - Maximum DIMM RTT_WR to use in power training: <b>0=ODT Off</b>, 1 = 120 ohms
  UINT8   ThrtCkeMinTmr;          ///< Offset 189 - Throttler CKE min timer: 0=Minimal, 0xFF=Maximum, <b>0x30=Default</b>
  UINT8   ThrtCkeMinTmrLpddr;     ///< Offset 190 - Throttler CKE min timer for LPDDR: 0=Minimal, 0xFF=Maximum, <b>0x40=Default</b>
  UINT8   BerEnable;              ///< Offset 191 - BER Enable and # of Addresses passed in: <b>0=Minimal</b>, 8=Maximum
  UINT8   CkeRankMapping;         ///< Offset 192 - Bits [7:4] - Channel 1, bits [3:0] - Channel 0. <b>0xAA=Default</b> Bit [i] specifies which rank CKE[i] goes to.
  UINT8   StrongWkLeaker;         ///< Offset 193 - Strong Weak Leaker: 1=Minimal, <b>7=Maximum</b>
  UINT8   CaVrefConfig;           ///< Offset 194 - 0=VREF_CA goes to both CH_A and CH_B, 1=VREF_CA to CH_A, VREF_DQ_A to CH_B, <b>2=VREF_CA to CH_A, VREF_DQ_B to CH_B</b>
  UINT8   SaGv;                   ///< Offset 195 - SA GV: <b>0=Disabled</b>, 1=FixedLow, 2=FixedHigh, 3=Enabled
  UINT8   RaplPwrFlCh1;           ///< Offset 196 - Power Channel 1 Floor value: <b>0=Minimal</b>, 255=Maximum
  UINT8   RaplPwrFlCh0;           ///< Offset 197 - Power Channel 0 Floor value: <b>0=Minimal</b>, 255=Maximum
  UINT8   NModeSupport;           ///< Offset 198 - Memory N Mode Support - Enable user to select Auto, 1N or 2N: <b>0=AUTO</b>, 1=1N, 2=2N.
  UINT8   RefClk;                 ///< Offset 199 - Selects the DDR base reference clock. 0x01 = 100MHz, <b>0x00 = 133MHz</b>
  UINT8   EnCmdRate;              ///< Offset 200 - CMD Rate Enable: 0=Disable, 1=1 CMD, 2=2 CMDs, <b>3=3 CMDs</b>, 4=4 CMDs, 5=5 CMDs, 6=6 CMDs, 7=7 CMDs
  UINT8   Refresh2X;              ///< Offset 201 - Refresh 2x: <b>0=Disable</b>, 1=Enable for WARM or HOT, 2=Enable for HOT only
  UINT8   EpgEnable;              ///< Offset 202 - Enable Energy Performance Gain.
  UINT8   RhSolution;             ///< Offset 203 - Type of solution to be used for RHP - 0/1 = HardwareRhp/Refresh2x
  UINT8   UserThresholdEnable;    ///< Offset 204 - Flag to manually select the DIMM CLTM Thermal Threshold, 0=Disable,  1=Enable, <b>0=Default</b>
  UINT8   UserBudgetEnable;       ///< Offset 205 - Flag to manually select the Budget Registers for CLTM Memory Dimms , 0=Disable,  1=Enable, <b>0=Default</b>
  UINT8   TsodTcritMax;           ///< Offset 206 - TSOD Tcrit Maximum Value  to be Configure , 0=Minimal, 128=Maximum, , <b>105=Default</b>

  UINT8   TsodEventMode;          ///< Offset 207 - Flag to Enable Event Mode Interruption in TSOD Configuration Register, 0=Disable,  1=Enable, <b>1=Default</b>
  UINT8   TsodEventPolarity;      ///< Offset 208 - Event Signal Polarity in TSOD Configuration Register, 0=Low,  1=High, <b>0=Default</b>
  UINT8   TsodCriticalEventOnly;  ///< Offset 209 - Critical Trigger Only in TSOD Configuration Register,0=Disable,  1=Enable, <b>1=Default</b>
  UINT8   TsodEventOutputControl; ///< Offset 210 - Event Output Control in TSOD Configuration Register,0=Disable,  1=Enable, <b>1=Default</b>
  UINT8   TsodAlarmwindowLockBit; ///< Offset 211 - Alarm Windows Lock Bit in TSOD Configuration Register,0=Unlock,  1=Lock, <b>0=Default</b>
  UINT8   TsodCriticaltripLockBit;///< Offset 212 - Critical Trip Lock Bit in TSOD Configuration Register,0=Unlock,  1=Lock, <b>0=Default</b>
  UINT8   TsodShutdownMode;       ///< Offset 213 - Shutdown Mode TSOD Configuration Register,0=Enable,  1=Disable, <b>0=Default</b>
  UINT8   TsodThigMax;            ///< Offset 214 - Thigh Max Value In the  for CLTM Memory Dimms , 0=Disable,  1=Enable, <b>0=Default</b>
  UINT8   TsodManualEnable;       ///< Offset 215 - Flag to manually select the TSOD Register Values , 0=Disable,  1=Enable, <b>0=Default</b>
  UINT8   DllBwEn0;               ///< Offset 216 - DllBwEn value for 1067
  UINT8   DllBwEn1;               ///< Offset 217 - DllBwEn value for 1333
  UINT8   DllBwEn2;               ///< Offset 218 - DllBwEn value for 1600
  UINT8   DllBwEn3;               ///< Offset 219 - DllBwEn value for 1867 and up
  UINT8   RetrainOnFastFail;      ///< Offset 220 - Restart MRC in Cold mode if SW MemTest fails during Fast flow. 0 = Disabled, <b>1 = Enabled</b>
  UINT8   ForceOltmOrRefresh2x;   ///< Offset 221 - Force OLTM or 2X Refresh when needed. <b>0 = Force OLTM</b>, 1 = Force 2x Refresh
  UINT8   PowerDownMode;          ///< Offset 222 - CKE Power Down Mode: <b>0xFF=AUTO</b>, 0=No Power Down, 1= APD mode, 6=PPD-DLL Off mode
  UINT8   PwdwnIdleCounter;       ///< Offset 223 - CKE Power Down Mode Idle Counter: 0=Minimal, 255=Maximum, <b>0x80=0x80 DCLK</b>
  UINT8   IsvtIoPort;             ///< Offset 224 ISVT IO Port Address: 0=Minimal, 0xFF=Maximum, <b>0x99=Default</b>
  UINT8   Reserved3;              ///< Offset 225 -  ConfigBlock size must be a multiple of DWORDs
  MrcGdxc Gdxc;                   ///< Offset 226 - 228 - GDXC enable and size.
  UINT8   RMTLoopCount;           ///< Offset 229 - Indicates the Loop Count to be used for Rank Margin Tool Testing: 1=Minimal, 32=Maximum, 0=AUTO, <b>0=Default</b>
  UINT8   Reserved4[2];           ///< Offset 230 - 231 Reserved for DWORD alignment.
  UINT32  RmtPerTask:1;                 ///< Offset 232 Bit 0: Rank Margin Tool Per Task. <b>0 = Disabled</b>, 1 = Enabled
  UINT32  Off232Bit1Rsvd:2;             ///< Offset 232 Bit 1-2: Reserved
  UINT32  EnBER:1;                      ///< Offset 232 Bit 3: Define if EnBER is enabled for Rank Margin Tool
  UINT32  Ddr4MixedUDimm2DpcLimit:1;    ///< Offset 232 Bit 4: Enable/Disable 2667 Frequency Limitation for DDR4 U-DIMM Mixed Dimm 2DPC population. 0 = Disabled, <b>1 = Enabled</b>
  UINT32  FastBootRmt:1;                ///< Offset 232 Bit 5: Enable/Disable RMT on FastBoot. <b>0 = Disabled</b>, 1 = Enabled
  UINT32  MrcTrainOnWarm:1;             ///< Offset 232 Bit 6: Enalbes MRC trainin on warm boot : <b>0=Disable</b>, 1 = Enabled
  UINT32  LongFlyByModeEnabled:1;       ///< Offset 232 Bit 7: Long FlyBy Mode Enabled : <b>0 = Disabled</b>, 1 = Enabled
  UINT32  Off232RsvdBits:24;            ///< Offset 232 Bit 8-31: Reserved

  //
  // TurnAround Timing
  //
  MrcTurnaroundTimes tRd2Rd;      ///< Offset 236 - User-defined overrides for Read-to-Read   Turn Around Timings. <b>0 = AUTO</b>
  MrcTurnaroundTimes tRd2Wr;      ///< Offset 240 - User-defined overrides for Read-to-Write  Turn Around Timings. <b>0 = AUTO</b>
  MrcTurnaroundTimes tWr2Rd;      ///< Offset 244 - User-defined overrides for Write-to-Read  Turn Around Timings. <b>0 = AUTO</b>
  MrcTurnaroundTimes tWr2Wr;      ///< Offset 248 - User-defined overrides for Write-to-Write Turn Around Timings. <b>0 = AUTO</b>
  UINT16  tRRD_L;                 ///< Offset 252 - User defined DDR4 Memory Timing tRRD_L value,  valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, 15=Maximum.
  UINT16  tRRD_S;                 ///< Offset 254 - User defined DDR4 Memory Timing tRRD_S value,  valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, 15=Maximum.
  UINT16  tWTR_L;                 ///< Offset 266 - User defined DDR4 Memory Timing tWTR_L value,  valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, 28=Maximum.
  UINT16  tWTR_S;                 ///< Offset 268 - User defined DDR4 Memory Timing tWTR_S value,  valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, 28=Maximum.

  //
  // End of synchronization to the SA MEMORY_CONFIGURATION structure.
  //
  MrcFrequency     FreqMax;                     ///< The requested maximum valid frequency.
  MrcBoardType     BoardType;                   ///< Define the board type (CRBMB,CRBDT,User1,User2). the OEM can add more boards.
  MrcCpuStepping   CpuStepping;                 ///< Define the CPU stepping.
  MrcCpuModel      CpuModel;                    ///< Define the CPU model.
  MrcCpuFamily     CpuFamily;                   ///< CPU is Coffeelake
  MrcGfxDataSize   GraphicsStolenSize;          ///< Graphics Data Stolen Memory size in MB
  MrcGfxGttSize    GraphicsGttSize;             ///< GTT graphics stolen memory size in MB
  MrcBaseTime      BaseTime;                    ///< RTC base time.
  MrcIteration     Iteration;                   ///< Number of iterations thru the MRC core call table.
  MrcMode          MrcMode;                     ///< The control for full or MiniBIOS MRC.
  MrcBootMode      BootMode;                    ///< The requested memory controller boot mode.
  BOOLEAN          TxtFlag;                     ///< Trusted eXecution Technology flag.
  BOOLEAN          SetRxDqs32;                  ///< Set DQS Delay to 32 control.
  BOOLEAN          GfxIsVersatileAcceleration;  ///< iGFX engines are in Versatile Acceleration
  BOOLEAN          DDR4MAP;                     ///< DDR4 PDA Mapping training control.
  POINTER_STRUCT   SaMemCfgAddress;             ///< Starting address of the input parameters to CRC.
  UINT32           SaMemCfgSize;                ///< The size of the input parameters to CRC.
  UINT32           PciEBaseAddress;             ///< define the PciE base address.
  UINT32           MchBarBaseAddress;           ///< define the MCH bar base address.
  UINT32           SmbusBaseAddress;            ///< This field defines the smbus base address.
  UINT32           GdxcBaseAddress;             ///< This field defines the GDXC base address.
  UINT32           HpetBaseAddress;             ///< This field defines the hpet base address.
  UINT32           MeStolenSize;                ///< define the size that the ME need in MB.
  UINT32           MmioSize;                    ///< define the MMIO size in MB.
  UINT32           TsegSize;                    ///< TSEG size that require by the system in MB.
  UINT32           IedSize;                     ///< IED size that require by the system in MB.
  UINT32           DprSize;                     ///< DPR size required by system in MB.
  UINT32           PrmrrSize;                   ///< Prmrr size required by the system in MB.
  POINTER_STRUCT   SerialBuffer;                ///< Pointer to the start of the serial buffer.
  UINT32           SerialBufferSize;            ///< The size of the serial buffer, in bytes.
  UINT32           DebugStream;                 ///< The debug port pointer.
  INT32            DebugLevel;                  ///< Indicates the level of debug messaging.
  UINT16           VccIomV;                     ///< VccIO logic voltage in mV.
  MrcControllerIn  Controller[MAX_CONTROLLERS]; ///< The following are controller level definitions.
  UINT32           HeapBase;                    ///< Starting address of the heap space.
  UINT32           HeapSize;                    ///< Size of the heap space, in bytes.
  UINT32           MrcStackTop;                 ///< Top of the stack at the beginning of MRC, for stack usage calculations.
  BOOLEAN          BdatEnable;                  ///< Option to enable output of training results into BDAT.
  UINT8            BdatTestType;                ///< When BdatEnable is set to TRUE, this option selects the type of training results data which will be populated into BDAT: <b>0=RMT</b>, 1=RMT Per Bit, 2=Margin 2D.
  BOOLEAN          LpddrDramOdt;                ///< TRUE if LPDDR DRAM ODT is used - depends on board design
  BOOLEAN          Ddr3DramOdt;                 ///< TRUE if DDR3  DRAM ODT is used - depends on board design
  BOOLEAN          Ddr4DramOdt;                 ///< TRUE if DDR4  DRAM ODT is used - depends on board design
  BOOLEAN          EnableVrefPwrDn;             ///< Setting this limits VrefGen to be off only during CKEPowerDown
  BOOLEAN          TxEqDis;                     ///< Disable TX Equalization
  BOOLEAN          EnVttOdt;                    ///< Enable VTT Termination for Data ODT
  UINT32           CpuidModel;                  ///< Unique CPU identifier.
  UINT8            CpuidStepping;               ///< Revision of the CPU.
  UINT8            CpuidSku;                    ///< SKU of the CPU.
  UINT32           SiPreMemPolicyPpi;
  TrainingModeType PowerTrainingMode;           ///< 0 - Power Training. 1 - Margin Training.
  union {
    MRC_FUNCTION  *Func;                        ///< External to MRC function pointers
    UINT64        Data;
  } Call;
  UINT16          RcompResistor[MAX_RCOMP];     ///< Reference RCOMP resistors on motherboard
  UINT16          RcompTarget[MAX_RCOMP_TARGETS]; ///< RCOMP target values for DqOdt, DqDrv, CmdDrv, CtlDrv, ClkDrv
  UINT32          CleanMemory:1;                ///< TRUE to request a memory clean
  UINT32          OcSupport:1;                  ///< TRUE if Overclocking is enabled in BIOS
  UINT32          RsvdBits5:30;
  /**
   Sets the serial debug message level\n
     0x00 = Disabled\n
     0x01 = Errors only\n
     0x02 = Errors and Warnings\n
     <b>0x03 = Errors, Warnings, and Info</b>\n
     0x04 = Errors, Warnings, Info, and Events\n
     0x05 = Displays Memory Init Execution Time Summary only\n
  **/
  UINT8           SerialDebugLevel;
} MrcInput;

typedef struct {
  UINT32        Size;   ///< The size of this structure, in bytes. Must be the first entry in this structure.
  MrcSaveHeader Header; ///< The header portion of the MRC saved data.
  MrcSaveData   Data;   ///< The data portion of the MRC saved data.
} MrcSave;

typedef struct {
  // Global variables that will be copied to the HOB follow.
  UINT8        MrcDataString[4]; ///< Beginning of global data marker, starts with "MRC". Must be the first entry in this structure.
  UINT32       MrcDataSize;      ///< The size of the MRC global data area, in bytes. Must be the second entry in this structure.
  MrcSave      Save;             ///< System specific save variables.
  MrcInput     Inputs;           ///< System specific input variables.
  MrcOutput    Outputs;          ///< System specific output variables.

  // Global variables that will remain internal to the MRC library follow.
  union {
    void   *Internal; ///< System specific output variables that remain internal to the library.
    UINT64 Data;
  } IntOutputs;
} MrcParameters;

#pragma pack (pop)
#endif
