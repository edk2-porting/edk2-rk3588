/** @file
  This file contains the definitions common to the MRC API and other APIs.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MrcCommonTypes_h_
#define _MrcCommonTypes_h_

#define INT32_MIN                       (0x80000000)
#ifndef INT32_MAX  //INT32_MAX->Already defined
#define INT32_MAX                       (0x7FFFFFFF)
#endif
#define INT16_MIN                       (0x8000)
#define INT16_MAX                       (0x7FFF)

///
/// System boot mode.
///
typedef enum {
  bmCold,                                 ///< Cold boot
  bmWarm,                                 ///< Warm boot
  bmS3,                                   ///< S3 resume
  bmFast,                                 ///< Fast boot
  MrcBootModeMax,                         ///< MRC_BOOT_MODE enumeration maximum value.
  MrcBootModeDelim = INT32_MAX            ///< This value ensures the enum size is consistent on both sides of the PPI.
} MrcBootMode;

///
/// DIMM memory package
/// This enum matches SPD Module Type - SPD byte 3, bits [3:0]
/// Note that DDR4 have different encoding for some module types
///
typedef enum {
  RDimmMemoryPackage          = 1,
  UDimmMemoryPackage          = 2,
  SoDimmMemoryPackage         = 3,
  MicroDimmMemoryPackageDdr3  = 4,
  LrDimmMemoryPackageDdr4     = 4,
  MiniRDimmMemoryPackage      = 5,
  MiniUDimmMemoryPackage      = 6,
  MiniCDimmMemoryPackage      = 7,
  LpDimmMemoryPackage         = 7,
  SoUDimmEccMemoryPackageDdr3 = 8,
  SoRDimmEccMemoryPackageDdr4 = 8,
  SoRDimmEccMemoryPackageDdr3 = 9,
  SoUDimmEccMemoryPackageDdr4 = 9,
  SoCDimmEccMemoryPackage     = 10,
  LrDimmMemoryPackage         = 11,
  SoDimm16bMemoryPackage      = 12,
  SoDimm32bMemoryPackage      = 13,
  NonDimmMemoryPackage        = 14,
  MemoryPackageMax,                       ///< MEMORY_PACKAGE enumeration maximum value.
  MemoryPackageDelim = INT32_MAX          ///< This value ensures the enum size is consistent on both sides of the PPI.
} MEMORY_PACKAGE;

///
/// Memory training I/O levels.
///
typedef enum {
  DdrLevel   = 0,                         ///< Refers to frontside of DIMM
  LrbufLevel = 1,                         ///< Refers to data level at backside of LRDIMM or AEP buffer
  RegALevel  = 2,                         ///< Refers to cmd level at backside of register - side A
  RegBLevel  = 3,                         ///< Refers to cmd level at backside of register - side B
  GsmLtMax,                               ///< GSM_LT enumeration maximum value.
  GsmLtDelim = INT32_MAX                  ///< This value ensures the enum size is consistent on both sides of the PPI.
} GSM_LT;

///
/// Memory training margin group selectors.
///
typedef enum {
  RecEnDelay       = 0,                   ///< Linear delay (PI ticks), where the positive increment moves the RCVEN sampling window later in time relative to the RX DQS strobes.
  RxDqsDelay       = 1,                   ///< Linear delay (PI ticks), where the positive increment moves the RX DQS strobe later in time relative to the RX DQ signal (i.e. toward the hold side of the eye).
  RxDqDelay        = 2,                   ///< Linear delay (PI ticks), where the positive increment moves the RX DQ byte/nibble/bitlane later in time relative to the RX DQS signal (i.e.closing the gap between DQ and DQS in the setup side of the eye).
  RxDqsPDelay      = 3,                   ///< Linear delay (PI ticks), where the positive increment moves the RX DQS strobe for "even" chunks later in time relative to the RX DQ signal. Even chunks are 0, 2, 4, 6 within the 0 to 7 chunks of an 8 burst length cacheline, for example.
  RxDqsNDelay      = 4,                   ///< Linear delay (PI ticks), where the positive increment moves the RX DQS strobe for "odd" chunks later in time relative to the RX DQ signal. Odd chunks are 1, 3, 5, 7 within the 0 to 7 chunks of an 8 burst length cacheline, for example.
  RxVref           = 5,                   ///< Linear increment (Vref ticks), where the positive increment moves the byte/nibble/bitlane RX Vref to a higher voltage.
  RxEq             = 6,                   ///< RX CTLE setting indicating a set of possible resistances, capacitance, current steering, etc. values, which may be a different set of values per product. The setting combinations are indexed by integer values.
  RxDqBitDelay     = 7,                   ///< Linear delay (PI ticks), where the positive increment moves the RX DQ bitlane later in time relative to the RX DQS signal (i.e.closing the gap between DQ and DQS in the setup side of the eye).
  RxVoc            = 8,                   ///< Monotonic increment (Sense Amp setting), where the positive increment moves the byte/nibble/bitlane's effective switching point to a lower Vref value.
  RxOdt            = 9,                   ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  RxOdtUp          = 10,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  RxOdtDn          = 11,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  DramDrvStr       = 12,                  ///< Drive strength setting resistance setting within a set of possible resistances (or currents), which may be a different set of values per product. Indexed by integer values.
  McOdtDelay       = 13,                  ///<
  McOdtDuration    = 14,                  ///<
  SenseAmpDelay    = 15,                  ///< This may be used to indicate CmdToDiffAmpEn for SoC's.
  SenseAmpDuration = 16,                  ///<
  RoundTripDelay   = 17,                  ///< This may be used to indicate CmdToRdDataValid for SoC's.
  RxDqsBitDelay    = 18,                  ///< Linear delay (PI ticks), where the positive increment moves the RX DQS within the bitlane later in time relative to the RX DQ signal (i.e.closing the gap between DQ and DQS in the hold side of the eye).
  RxDqDqsDelay     = 19,                  ///< Linear delay (PI ticks), where the positive increment moves the RX DQS per strobe later in time relative to the RX DQ signal (i.e. closing the gap between DQS and DQ in the hold side of the eye. The difference between this parameter and RxDqsDelay is that both the DQ and DQS timings may be moved in order to increase the total range of DQDQS timings.
  WrLvlDelay       = 20,                  ///< Linear delay (PI ticks), where the positive increment moves both the TX DQS and TX DQ signals later in time relative to all other bus signals.
  TxDqsDelay       = 21,                  ///< Linear delay (PI ticks), where the positive increment moves the TX DQS strobe later in time relative to all other bus signals.
  TxDqDelay        = 22,                  ///< Linear delay (PI ticks), where the positive increment moves the TX DQ byte/nibble/bitlane later in time relative to all other bus signals.
  TxVref           = 23,                  ///< Linear increment (Vref ticks), where the positive increment moves the byte/nibble/bitlane TX Vref to a higher voltage. (Assuming this will abstract away from the range specifics for DDR4, for example.)
  TxEq             = 24,                  ///< TX EQ setting indicating a set of possible equalization levels, which may be a different set of values per product. The setting combinations are indexed by integer values.
  TxDqBitDelay     = 25,                  ///< Linear delay (PI ticks), where the positive increment moves the TX DQ bitlane later in time relative to all other bus signals.
  TxRon            = 26,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  TxRonUp          = 27,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  TxRonDn          = 28,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  TxSlewRate       = 29,                  ///< Monotonic increment, where the positive increment moves the byte/nibble/bitlane's effective slew rate to a higher slope.
  TxImode          = 30,                  ///< TX I-Mode Boost setting indicating a set of possible current boost levels, which may be a different set of values per product. The setting combinations are indexed by integer values.
  WrOdt            = 31,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  NomOdt           = 32,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  ParkOdt          = 33,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  TxTco            = 34,                  ///<
  RxCtleR          = 36,                  ///<
  RxCtleC          = 37,                  ///<
  RxDqsPBitDelay   = 38,                  ///< Linear delay (PI ticks), where the positive increment moves the RX DQS bitlane timing for "even" chunks later in time relative to the RX DQ bitlane signal. Even chunks are 0, 2, 4, 6 within the 0 to 7 chunks of an 8 burst length cacheline, for example.
  RxDqsNBitDelay   = 39,                  ///< Linear delay (PI ticks), where the positive increment moves the RX DQS bitlane timing for "odd" chunks later in time relative to the RX DQ bitlane signal. Odd chunks are 1, 3, 5, 7 within the 0 to 7 chunks of an 8 burst length cacheline, for example.
  CmdAll           = 40,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CMD_ALL category later in time relative to all other signals on the bus.
  CmdGrp0          = 41,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CMD_GRP0 category later in time relative to all other signals on the bus.
  CmdGrp1          = 42,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CMD_GRP1 category later in time relative to all other signals on the bus.
  CmdGrp2          = 43,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CMD_GRP2 category later in time relative to all other signals on the bus.
  CtlAll           = 44,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_ALL category later in time relative to all other signals on the bus.
  CtlGrp0          = 45,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRP0 category later in time relative to all other signals on the bus.
  CtlGrp1          = 46,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRP1 category later in time relative to all other signals on the bus.
  CtlGrp2          = 47,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRP2 category later in time relative to all other signals on the bus.
  CtlGrp3          = 48,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRP3 category later in time relative to all other signals on the bus.
  CtlGrp4          = 49,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRP4 category later in time relative to all other signals on the bus.
  CtlGrp5          = 50,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRP5 category later in time relative to all other signals on the bus.
  CmdCtlAll        = 51,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CMD_CTL_ALL category later in time relative to all other signals on the bus.
  CkAll            = 52,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CK_ALL category later in time relative to all other signals on the bus.
  CmdVref          = 53,                  ///< Linear increment (Vref ticks), where the positive increment moves the CMD Vref to a higher voltage.
  AlertVref        = 54,                  ///< Linear increment (Vref ticks), where the positive increment moves the ALERT Vref to a higher voltage.
  CmdRon           = 55,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.

  EridDelay        = 60,                  ///< Linear delay (PI ticks), where the positive increment moves the ERID signals later in time relative to the internal sampling clock (i.e.closing the gap between ERID and internal sampling clock in the setup side of the eye). This group is applicable for DDRT DIMMs.
  EridVref         = 61,                  ///< Linear increment (Vref ticks), where the positive increment moves the ERID Vref to a higher voltage. This group is applicable for DDRT DIMMs.
  ErrorVref        = 62,                  ///< Linear increment (Vref ticks), where the positive increment moves the ERROR Vref to a higher voltage. This group is applicable for DDRT DIMMs.
  ReqVref          = 63,                  ///< Linear increment (Vref ticks), where the positive increment moves the REQ Vref to a higher voltage. This group is applicable for DDRT DIMMs.
  RecEnOffset      = 64,                  ///< Linear delay (PI ticks), where the positive increment moves the RCVEN sampling window later in time relative to the RX DQS strobes.
  RxDqsOffset      = 65,                  ///< Linear delay (PI ticks), where the positive increment moves the RX DQS strobe later in time relative to the RX DQ signal (i.e. toward the hold side of the eye).
  RxVrefOffset     = 66,                  ///< Linear increment (Vref ticks), where the positive increment moves the byte/nibble/bitlane RX Vref to a higher voltage.
  TxDqsOffset      = 67,                  ///< Linear delay (PI ticks), where the positive increment moves the TX DQS strobe later in time relative to all other bus signals.
  TxDqOffset       = 68,                  ///< Linear delay (PI ticks), where the positive increment moves the TX DQ byte/nibble/bitlane later in time relative to all other bus signals.
  GsmGtMax,                               ///< SSA_GSM_GT enumeration maximum value.
  GsmGtDelim = INT32_MAX                  ///< This value ensures the enum size is consistent on both sides of the PPI.
} GSM_GT;

typedef enum {
  SigRasN   = 0,
  SigCasN   = 1,
  SigWeN    = 2,
  SigBa0    = 3,
  SigBa1    = 4,
  SigBa2    = 5,
  SigA0     = 6,
  SigA1     = 7,
  SigA2     = 8,
  SigA3     = 9,
  SigA4     = 10,
  SigA5     = 11,
  SigA6     = 12,
  SigA7     = 13,
  SigA8     = 14,
  SigA9     = 15,
  SigA10    = 16,
  SigA11    = 17,
  SigA12    = 18,
  SigA13    = 19,
  SigA14    = 20,
  SigA15    = 21,
  SigA16    = 22,
  SigA17    = 23,
  SigCs0N   = 24,
  SigCs1N   = 25,
  SigCs2N   = 26,
  SigCs3N   = 27,
  SigCs4N   = 28,
  SigCs5N   = 29,
  SigCs6N   = 30,
  SigCs7N   = 31,
  SigCs8N   = 32,
  SigCs9N   = 33,
  SigCke0   = 34,
  SigCke1   = 35,
  SigCke2   = 36,
  SigCke3   = 37,
  SigCke4   = 38,
  SigCke5   = 39,
  SigOdt0   = 40,     //could also be used for CA-ODT for LP4
  SigOdt1   = 41,     //could also be used for CA-ODT for LP4
  SigOdt2   = 42,
  SigOdt3   = 43,
  SigOdt4   = 44,
  SigOdt5   = 45,
  SigPar    = 46,
  SigAlertN = 47,
  SigBg0    = 48,
  SigBg1    = 49,
  SigActN   = 50,
  SigCid0   = 51,
  SigCid1   = 52,
  SigCid2   = 53,
  SigCk0    = 54,
  SigCk1    = 55,
  SigCk2    = 56,
  SigCk3    = 57,
  SigCk4    = 58,
  SigCk5    = 59,
  SigGnt0   = 60,
  SigGnt1   = 61,
  SigErid00 = 62,
  SigErid01 = 63,
  SigErid10 = 64,
  SigErid11 = 65,
  SigErr0   = 66,
  SigErr1   = 67,
  SigCa00   = 68,    // First instantiation of the CA bus for a given channel
  SigCa01   = 69,
  SigCa02   = 70,
  SigCa03   = 71,
  SigCa04   = 72,
  SigCa05   = 73,
  SigCa10   = 74,    // Second instantiation of the CA bus for a given channel
  SigCa11   = 75,
  SigCa12   = 76,
  SigCa13   = 77,
  SigCa14   = 78,
  SigCa15   = 79,
  GsmCsnMax,
  GsmCsnDelim = INT32_MAX
} GSM_CSN;


#endif // _MrcCommonTypes_h_
