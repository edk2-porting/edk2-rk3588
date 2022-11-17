/** @file
 Source code for the board SA configuration Pcd init functions in Pre-Memory init phase.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "BoardSaConfigPreMem.h"
#include "SaPolicyCommon.h"
#include "CometlakeURvpInit.h"
#include <PlatformBoardConfig.h>
#include <Library/CpuPlatformLib.h>
//
// LPDDR3 178b 8Gb die, DDP, x32
// Micron MT52L512M32D2PF-093
// 2133, 16-20-20-45
// 2 ranks per channel, 2 SDRAMs per rank, 4x8Gb = 4GB total per channel
//
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 mLpddr3Ddp8Gb178b2133Spd[] = {
  0x24, // 512 SPD bytes used, 512 total
  0x01, // SPD Revision 0.1
  0x0F, // DRAM Type: LPDDR3 SDRAM
  0x0E, // Module Type: Non-DIMM Solution
  0x15, // 8 Banks, 8 Gb SDRAM density
  0x19, // 15 Rows, 10 Columns
  0x90, // SDRAM Package Type: DDP, 1 Channel per package
  0x00, // SDRAM Optional Features: none, tMAW = 8192 * tREFI
  0x00, // SDRAM Thermal / Refresh options: none
  0x00, // Other SDRAM Optional Features: none
  0x00, // Reserved
  0x0B, // Module Nominal Voltage, VDD = 1.2v
  0x0B, // SDRAM width: 32 bits, 2 Ranks
  0x03, // SDRAM bus width: 1 Channel, 64 bits channel width
  0x00, // Module Thermal Sensor: none
  0x00, // Extended Module Type: Reserved
  0x00, // Signal Loading: Unspecified
  0x00, // MTB = 0.125ns, FTB = 1 ps
  0x08, // tCKmin = 0.938 ns (LPDDR3-2133)
  0xFF, // tCKmax = 32.002 ns
  0xD4, // CAS Latencies supported (tCK): 16, 14, 12, 10, 8 (First Byte)
  0x01, // CAS Latencies supported (tCK): 16, 14, 12, 10, 8 (Second Byte)
  0x00, // CAS Latencies supported (tCK): 16, 14, 12, 10, 8 (Third Byte)
  0x00, // CAS Latencies supported (tCK): 16, 14, 12, 10, 8 (Fourth Byte)
  0x78, // Minimum CAS Latency (tAAmin) = 15.008 ns
  0x00, // Read and Write Latency Set options: none
  0x90, // Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
  0xA8, // Row precharge time for all banks (tRPab) = 21 ns
  0x90, // Minimum row precharge time (tRPmin) = 18 ns
  0x90, // tRFCab = 210 ns (8 Gb)
  0x06, // tRFCab MSB
  0xD0, // tRFCpb = 90 ns (8 Gb)
  0x02, // tRFCpb MSB
  0, 0, 0, 0, 0, 0, 0,          // 33-39
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 40-49
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 50-59
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 60-69
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 70-79
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 80-89
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 90-99
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 100-109
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 110-119
  0x00,                         // FTB for Row precharge time per bank (tRPpb) = 18 ns
  0x00,                         // FTB for Row precharge time for all banks (tRPab) = 21 ns
  0x00,                         // FTB for Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
  0x08,                         // FTB for tAAmin = 15.008 ns (LPDDR3-2133)
  0x7F,                         // FTB for tCKmax = 32.002 ns
  0xC2,                         // FTB for tCKmin = 0.938 ns (LPDDR3-2133)
  0, 0, 0, 0,                   // 126-129
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 130-139
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 140-149
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 150-159
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 160-169
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 170-179
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 180-189
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 190-199
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 200-209
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 210-219
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 220-229
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 230-239
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 240-249
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 250-259
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 260-269
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 270-279
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 280-289
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 290-299
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 300-309
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 310-319
  0, 0, 0, 0, 0,                // 320-324
  0x55, 0, 0, 0,                // 325-328: Module ID: Module Serial Number
  0x20, 0x20, 0x20, 0x20, 0x20, // 329-333: Module Part Number: Unused bytes coded as ASCII Blanks (0x20)
  0x20, 0x20, 0x20, 0x20, 0x20, // 334-338: Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20, // 339-343: Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20, // 344-348: Module Part Number
  0x00,                         // 349 Module Revision Code
  0x00,                         // 350 DRAM Manufacturer ID Code, Least Significant Byte
  0x00,                         // 351 DRAM Manufacturer ID Code, Most Significant Byte
  0x00,                         // 352 DRAM Stepping
  0, 0, 0, 0, 0, 0, 0,          // 353 - 359
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 360 - 369
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 370 - 379
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 380 - 389
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 390 - 399
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 400 - 409
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 410 - 419
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 420 - 429
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 430 - 439
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 440 - 449
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 450 - 459
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 460 - 469
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 470 - 479
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 480 - 489
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 490 - 499
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 500 - 509
  0, 0                          // 510 - 511
};

//
// Display DDI settings for WHL ERB
//
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 mWhlErbRowDisplayDdiConfig[9] = {
  DdiPortAEdp,     // DDI Port A Config : DdiPortADisabled = Disabled, DdiPortAEdp = eDP, DdiPortAMipiDsi = MIPI DSI
  DdiHpdEnable,    // DDI Port B HPD : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
  DdiHpdEnable,    // DDI Port C HPD : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
  DdiHpdDisable,   // DDI Port D HPD : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
  DdiHpdDisable,   // DDI Port F HPD : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
  DdiDdcEnable,    // DDI Port B DDC : DdiDisable = Disable, DdiDdcEnable = Enable DDC
  DdiDdcEnable,    // DDI Port C DDC : DdiDisable = Disable, DdiDdcEnable = Enable DDC
  DdiDdcEnable,    // DDI Port D DDC : DdiDisable = Disable, DdiDdcEnable = Enable DDC
  DdiDisable       // DDI Port F DDC : DdiDisable = Disable, DdiDdcEnable = Enable DDC
};

/**
  MRC configuration init function for PEI pre-memory phase.

  @param[in]  BoardId           An unsigned integer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
SaMiscConfigInit (
  IN UINT16         BoardId
  )
{
  //
  // UserBd
  //
  switch (BoardId) {
    case BoardIdCometLakeULpddr3Rvp:
      //
      // Assign UserBd to 5 which is assigned to MrcInputs->BoardType btUser4 for ULT platforms.
      // This is required to skip Memory voltage programming based on GPIO's in MRC
      //
      PcdSet8S (PcdSaMiscUserBd, 5); // MrcBoardType btUser4 for ULT platform
      break;

    default:
      // MiscPeiPreMemConfig.UserBd = 0 by default.
      break;
  }

  PcdSet16S (PcdSaDdrFreqLimit, 0);

  return EFI_SUCCESS;
}

/**
  Board Memory Init related configuration init function for PEI pre-memory phase.

  @param[in]  BoardId   An unsigned integrer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
MrcConfigInit (
  IN UINT16 BoardId
  )
{
  CPU_FAMILY    CpuFamilyId;

  CpuFamilyId = GetCpuFamily();

  if (CpuFamilyId == EnumCpuCflDtHalo) {
    PcdSetBoolS (PcdDualDimmPerChannelBoardType, TRUE);
  } else {
    PcdSetBoolS (PcdDualDimmPerChannelBoardType, FALSE);
  }

  //
  // Example policy for DIMM slots implementation boards:
  // 1. Assign Smbus address of DIMMs and SpdData will be updated later
  //    by reading from DIMM SPD.
  // 2. No need to apply hardcoded SpdData buffers here for such board.
  //
  //  Comet Lake U LP3 has removable DIMM slots.
  //  So assign all Smbus address of DIMMs and leave PcdMrcSpdData set to 0.
  //   Example:
  //   PcdMrcSpdData = 0
  //   PcdMrcSpdDataSize = 0
  //   PcdMrcSpdAddressTable0 = 0xA0
  //   PcdMrcSpdAddressTable1 = 0xA2
  //   PcdMrcSpdAddressTable2 = 0xA4
  //   PcdMrcSpdAddressTable3 = 0xA6
  //
  //  If a board has soldered down memory. It should use the following settings.
  //   Example:
  //   PcdMrcSpdAddressTable0 = 0
  //   PcdMrcSpdAddressTable1 = 0
  //   PcdMrcSpdAddressTable2 = 0
  //   PcdMrcSpdAddressTable3 = 0
  //   PcdMrcSpdData = static data buffer
  //   PcdMrcSpdDataSize = sizeof (static data buffer)
  //

  //
  // SPD Address Table
  //
  PcdSet32S (PcdMrcSpdData, (UINTN)mLpddr3Ddp8Gb178b2133Spd);
  PcdSet16S (PcdMrcSpdDataSize, sizeof(mLpddr3Ddp8Gb178b2133Spd));
  PcdSet8S (PcdMrcSpdAddressTable0, 0);
  PcdSet8S (PcdMrcSpdAddressTable1, 0);
  PcdSet8S (PcdMrcSpdAddressTable2, 0);
  PcdSet8S (PcdMrcSpdAddressTable3, 0);

  //
  // DRAM SPD Data & related configuration
  //
  // Setting the PCD's to default value (CML LP3). It will be overriden to board specific settings below.
  PcdSet32S(PcdMrcDqByteMap, (UINTN) DqByteMapCmlULpddr3Rvp);
  PcdSet16S (PcdMrcDqByteMapSize, sizeof (DqByteMapCmlULpddr3Rvp));
  PcdSet32S(PcdMrcDqsMapCpu2Dram, (UINTN) DqsMapCpu2DramCmlULpddr3Rvp);
  PcdSet16S (PcdMrcDqsMapCpu2DramSize, sizeof (DqsMapCpu2DramCmlULpddr3Rvp));

  switch (BoardId) {

    case BoardIdCometLakeULpddr3Rvp:
      PcdSet32S(PcdMrcRcompResistor, (UINTN)RcompResistorCmlULpKc);
      PcdSet32S(PcdMrcRcompTarget, (UINTN)RcompTargetCmlULpKc);
      PcdSetBoolS(PcdMrcDqPinsInterleavedControl, TRUE);
      PcdSetBoolS(PcdMrcDqPinsInterleaved, FALSE);
      break;

    default:
      break;
  }

  //
  // CA Vref routing: board-dependent
  // 0 - VREF_CA goes to both CH_A and CH_B (LPDDR3/DDR3L)
  // 1 - VREF_CA to CH_A, VREF_DQ_A to CH_B (should not be used)
  // 2 - VREF_CA to CH_A, VREF_DQ_B to CH_B (DDR4)
  //
  switch (BoardId) {
    case BoardIdCometLakeULpddr3Rvp:
      PcdSet8S (PcdMrcCaVrefConfig, 0); // All DDR3L/LPDDR3/LPDDR4 boards
      break;

    default:
      PcdSet8S (PcdMrcCaVrefConfig, 2); // DDR4 boards
      break;
  }

  return EFI_SUCCESS;
}

/**
  Board SA related GPIO configuration init function for PEI pre-memory phase.

  @param[in]  BoardId   An unsigned integer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
SaGpioConfigInit (
  IN UINT16 BoardId
  )
{
  //
  // Update board's GPIO for PEG slot reset
  //
  PcdSetBoolS (PcdPegGpioResetControl, TRUE);
  PcdSetBoolS (PcdPegGpioResetSupoort, FALSE);
  PcdSet32S (PcdPeg0ResetGpioPad, 0);
  PcdSetBoolS (PcdPeg0ResetGpioActive, FALSE);
  PcdSet32S (PcdPeg3ResetGpioPad, 0);
  PcdSetBoolS (PcdPeg3ResetGpioActive, FALSE);

  //
  // PCIE RTD3 GPIO
  //
  switch (BoardId) {
    case BoardIdCometLakeULpddr3Rvp:
      PcdSet8S(PcdRootPortIndex, 4);
      PcdSet8S (PcdPcie0GpioSupport, PchGpio);
      PcdSet32S (PcdPcie0WakeGpioNo, 0);
      PcdSet8S (PcdPcie0HoldRstExpanderNo, 0);
      PcdSet32S (PcdPcie0HoldRstGpioNo, GPIO_CNL_LP_GPP_C15);
      PcdSetBoolS (PcdPcie0HoldRstActive, FALSE);
      PcdSet8S (PcdPcie0PwrEnableExpanderNo, 0);
      PcdSet32S (PcdPcie0PwrEnableGpioNo, GPIO_CNL_LP_GPP_C14);
      PcdSetBoolS (PcdPcie0PwrEnableActive, FALSE);

      PcdSet8S  (PcdPcie1GpioSupport, NotSupported);
      PcdSet32S (PcdPcie1WakeGpioNo, 0);
      PcdSet8S  (PcdPcie1HoldRstExpanderNo, 0);
      PcdSet32S (PcdPcie1HoldRstGpioNo, 0);
      PcdSetBoolS (PcdPcie1HoldRstActive, FALSE);
      PcdSet8S  (PcdPcie1PwrEnableExpanderNo, 0);
      PcdSet32S (PcdPcie1PwrEnableGpioNo, 0);
      PcdSetBoolS (PcdPcie1PwrEnableActive, FALSE);

      PcdSet8S  (PcdPcie2GpioSupport, NotSupported);
      PcdSet32S (PcdPcie2WakeGpioNo, 0);
      PcdSet8S  (PcdPcie2HoldRstExpanderNo, 0);
      PcdSet32S (PcdPcie2HoldRstGpioNo, 0);
      PcdSetBoolS (PcdPcie2HoldRstActive, FALSE);
      PcdSet8S  (PcdPcie2PwrEnableExpanderNo, 0);
      PcdSet32S (PcdPcie2PwrEnableGpioNo, 0);
      PcdSetBoolS (PcdPcie2PwrEnableActive, FALSE);
      break;

    default:
      PcdSet8S(PcdRootPortIndex, 0xFF);
      PcdSet8S  (PcdPcie0GpioSupport, NotSupported);
      PcdSet32S (PcdPcie0WakeGpioNo, 0);
      PcdSet8S  (PcdPcie0HoldRstExpanderNo, 0);
      PcdSet32S (PcdPcie0HoldRstGpioNo, 0);
      PcdSetBoolS (PcdPcie0HoldRstActive, FALSE);
      PcdSet8S  (PcdPcie0PwrEnableExpanderNo, 0);
      PcdSet32S (PcdPcie0PwrEnableGpioNo, 0);
      PcdSetBoolS (PcdPcie0PwrEnableActive, FALSE);

      PcdSet8S  (PcdPcie1GpioSupport, NotSupported);
      PcdSet32S (PcdPcie1WakeGpioNo, 0);
      PcdSet8S  (PcdPcie1HoldRstExpanderNo, 0);
      PcdSet32S (PcdPcie1HoldRstGpioNo, 0);
      PcdSetBoolS (PcdPcie1HoldRstActive, FALSE);
      PcdSet8S  (PcdPcie1PwrEnableExpanderNo, 0);
      PcdSet32S (PcdPcie1PwrEnableGpioNo, 0);
      PcdSetBoolS (PcdPcie1PwrEnableActive, FALSE);

      PcdSet8S  (PcdPcie2GpioSupport, NotSupported);
      PcdSet32S (PcdPcie2WakeGpioNo, 0);
      PcdSet8S  (PcdPcie2HoldRstExpanderNo, 0);
      PcdSet32S (PcdPcie2HoldRstGpioNo, 0);
      PcdSetBoolS (PcdPcie2HoldRstActive, FALSE);
      PcdSet8S  (PcdPcie2PwrEnableExpanderNo, 0);
      PcdSet32S (PcdPcie2PwrEnableGpioNo, 0);
      PcdSetBoolS (PcdPcie2PwrEnableActive, FALSE);
      break;
  }

  return EFI_SUCCESS;
}

/**
  SA Display DDI configuration init function for PEI pre-memory phase.

  @param[in]  BoardId       An unsigned integer represent the board id.

  @retval     EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
SaDisplayConfigInit (
  IN UINT16 BoardId
  )
{
  //
  // Update Display DDI Config
  //
  switch (BoardId) {
    case BoardIdCometLakeULpddr3Rvp:
      PcdSet32S (PcdSaDisplayConfigTable, (UINTN) mWhlErbRowDisplayDdiConfig);
      PcdSet16S (PcdSaDisplayConfigTableSize, sizeof (mWhlErbRowDisplayDdiConfig));
      break;

    default:
      break;
  }

  return EFI_SUCCESS;
}

