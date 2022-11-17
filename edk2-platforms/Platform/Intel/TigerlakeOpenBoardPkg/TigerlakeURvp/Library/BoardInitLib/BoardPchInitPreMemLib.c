/** @file
  Source code for the board PCH configuration Pcd init functions for Pre-Memory Init phase.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "TigerlakeURvpInit.h"
#include "GpioTableTigerlakeUDdr4RvpPreMem.h"

#include <PlatformBoardConfig.h>
#include <Include/PlatformBoardId.h>

#include <Library/GpioLib.h>

/**
  Board Root Port Clock Info configuration init function for PEI pre-memory phase.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
RootPortClkInfoInit (
  IN UINT16    BoardId
  )
{
  PCD64_BLOB Clock[PCH_MAX_PCIE_CLOCKS];
  UINT32 Index;

  //
  // The default clock assignment will be FREE_RUNNING, which corresponds to PchClockUsageUnspecified
  // This is safe but power-consuming setting. If Platform code doesn't contain port-clock map for a given board,
  // the clocks will keep on running anyway, allowing PCIe devices to operate. Downside is that clocks will
  // continue to draw power. To prevent this, remember to provide port-clock map for every board.
  //
  for (Index = 0; Index < PCH_MAX_PCIE_CLOCKS; Index++) {
    Clock[Index].PcieClock.ClkReqSupported = TRUE;
    Clock[Index].PcieClock.ClockUsage = FREE_RUNNING;
  }

  ///
  /// Assign ClkReq signal to root port. (Base 0)
  /// For LP, Set 0 - 6
  /// For H,  Set 0 - 15
  /// Note that if GbE is enabled, ClkReq assigned to GbE will not be available for Root Port.
  ///

  switch (BoardId) {
    // CLKREQ
    case BoardIdTglUDdr4:
      Clock[0].PcieClock.ClockUsage = PCIE_PEG;
      Clock[1].PcieClock.ClockUsage = PCIE_PCH + 2;
      Clock[2].PcieClock.ClockUsage = PCIE_PCH + 3;
      Clock[3].PcieClock.ClockUsage = PCIE_PCH + 8;
      Clock[4].PcieClock.ClockUsage = LAN_CLOCK;
      Clock[5].PcieClock.ClockUsage = PCIE_PCH + 7;
      Clock[6].PcieClock.ClockUsage = PCIE_PCH + 4;
      break;
    default:

      break;
  }

  PcdSet64S (PcdPcieClock0,  Clock[ 0].Blob);
  PcdSet64S (PcdPcieClock1,  Clock[ 1].Blob);
  PcdSet64S (PcdPcieClock2,  Clock[ 2].Blob);
  PcdSet64S (PcdPcieClock3,  Clock[ 3].Blob);
  PcdSet64S (PcdPcieClock4,  Clock[ 4].Blob);
  PcdSet64S (PcdPcieClock5,  Clock[ 5].Blob);
  PcdSet64S (PcdPcieClock6,  Clock[ 6].Blob);
  PcdSet64S (PcdPcieClock7,  Clock[ 7].Blob);
  PcdSet64S (PcdPcieClock8,  Clock[ 8].Blob);
  PcdSet64S (PcdPcieClock9,  Clock[ 9].Blob);
  PcdSet64S (PcdPcieClock10, Clock[10].Blob);
  PcdSet64S (PcdPcieClock11, Clock[11].Blob);
  PcdSet64S (PcdPcieClock12, Clock[12].Blob);
  PcdSet64S (PcdPcieClock13, Clock[13].Blob);
  PcdSet64S (PcdPcieClock14, Clock[14].Blob);
  PcdSet64S (PcdPcieClock15, Clock[15].Blob);

  return EFI_SUCCESS;
}

/**
  Board USB related configuration init function for PEI pre-memory phase.

  @param[in]  BoardId   An unsigned integrer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
UsbConfigInit (
  IN UINT16 BoardId
  )
{

  return EFI_SUCCESS;
}


/**
  Board GPIO Group Tier configuration init function for PEI pre-memory phase.

  @param[in]  BoardId   An unsigned integrer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
GpioGroupTierInit (
  IN UINT16 BoardId
  )
{

  return EFI_SUCCESS;
}


/**
  GPIO init function for PEI pre-memory phase.

  @param[in]  BoardId   An unsigned integrer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
GpioTablePreMemInit (
  IN UINT16 BoardId
  )
{

  //
  // GPIO Table Init.
  //
  switch (BoardId) {
    case BoardIdTglUDdr4:
      PcdSet32S (PcdBoardGpioTablePreMem, (UINTN) mGpioTablePreMemTglUDdr4);
      PcdSet16S (PcdBoardGpioTablePreMemSize, mGpioTablePreMemTglUDdr4Size);
      break;

    default:
      break;
  }

  return EFI_SUCCESS;
}

/**
  PmConfig init function for PEI pre-memory phase.

  @param[in]  BoardId   An unsigned integrer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
PchPmConfigInit (
  IN UINT16 BoardId
  )
{

  return EFI_SUCCESS;
}
