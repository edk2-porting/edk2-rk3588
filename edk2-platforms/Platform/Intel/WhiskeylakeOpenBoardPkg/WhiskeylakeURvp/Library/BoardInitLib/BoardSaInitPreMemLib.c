/** @file
 Source code for the board SA configuration Pcd init functions in Pre-Memory init phase.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "BoardSaConfigPreMem.h"
#include "SaPolicyCommon.h"
#include "WhiskeylakeURvpInit.h"
#include <PlatformBoardConfig.h>
#include <Library/CpuPlatformLib.h>

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
    case BoardIdWhiskeyLakeRvp:
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
  //  Whiskey Lake U RVP has removable DIMM slots.
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
  PcdSet32S (PcdMrcSpdData, 0);
  PcdSet16S (PcdMrcSpdDataSize, 0);
  PcdSet8S (PcdMrcSpdAddressTable0, 0xA0);
  PcdSet8S (PcdMrcSpdAddressTable1, 0xA2);
  PcdSet8S (PcdMrcSpdAddressTable2, 0xA4);
  PcdSet8S (PcdMrcSpdAddressTable3, 0xA6);

  //
  // DRAM SPD Data & related configuration
  //
  // Setting the PCD's to default value (WHL RVP3). It will be overriden to board specific settings below.
  PcdSet32S (PcdMrcDqByteMap, (UINTN) mDqByteMapWhlUDdr4Rvp);
  PcdSet16S (PcdMrcDqByteMapSize, sizeof (mDqByteMapWhlUDdr4Rvp));
  PcdSet32S (PcdMrcDqsMapCpu2Dram, (UINTN) mDqsMapCpu2DramWhlUDdr4Rvp);
  PcdSet16S (PcdMrcDqsMapCpu2DramSize, sizeof (mDqsMapCpu2DramWhlUDdr4Rvp));

  switch (BoardId) {

    case BoardIdWhiskeyLakeRvp:
      PcdSet32S (PcdMrcRcompResistor, (UINTN) RcompResistorCflUDdr4Interposer);
      PcdSet32S (PcdMrcRcompTarget, (UINTN) RcompTargetWhlUDdr4Interposer);
      PcdSetBoolS (PcdMrcDqPinsInterleavedControl, TRUE);
      PcdSetBoolS (PcdMrcDqPinsInterleaved, TRUE);
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
    case BoardIdWhiskeyLakeRvp:
      PcdSet8S (PcdMrcCaVrefConfig, 2); // DDR4 boards
      break;

    default:
      PcdSet8S (PcdMrcCaVrefConfig, 0); // All DDR3L/LPDDR3/LPDDR4 boards
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
    case BoardIdWhiskeyLakeRvp:
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
    case BoardIdWhiskeyLakeRvp:
      PcdSet32S (PcdSaDisplayConfigTable, (UINTN) mWhlErbRowDisplayDdiConfig);
      PcdSet16S (PcdSaDisplayConfigTableSize, sizeof (mWhlErbRowDisplayDdiConfig));
      break;

    default:
      break;
  }

  return EFI_SUCCESS;
}

