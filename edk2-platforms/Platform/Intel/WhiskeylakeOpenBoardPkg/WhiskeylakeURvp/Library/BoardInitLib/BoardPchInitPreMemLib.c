/** @file
 Source code for the board PCH configuration Pcd init functions for Pre-Memory Init phase.


  Copyright (c) 2019 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "WhiskeylakeURvpInit.h"
#include <GpioPinsCnlLp.h>
#include <GpioPinsCnlH.h>
#include <PlatformBoardConfig.h>        // for USB 20 AFE & Root Port Clk Info.
#include <Library/BaseMemoryLib.h>
#include <Library/GpioLib.h>

/**
  Board Root Port Clock Info configuration init function for PEI pre-memory phase.

  @param[in]  BoardId   An unsigned integrer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
RootPortClkInfoInit (
  IN UINT16 BoardId
  )
{
  PCD64_BLOB                      *Clock;
  UINT32                          Index;

  Clock = AllocateZeroPool (16 * sizeof (PCD64_BLOB));
  ASSERT (Clock != NULL);
  if (Clock == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  //
  // The default clock assignment will be FREE_RUNNING, which corresponds to PchClockUsageUnspecified
  // This is safe but power-consuming setting. If Platform code doesn't contain port-clock map for a given board,
  // the clocks will keep on running anyway, allowing PCIe devices to operate. Downside is that clocks will
  // continue to draw power. To prevent this, remember to provide port-clock map for every board.
  //
  for (Index = 0; Index < 16; Index++) {
    Clock[Index].PcieClock.ClkReqSupported = TRUE;
    Clock[Index].PcieClock.ClockUsage = FREE_RUNNING;
  }

  ///
  /// Assign ClkReq signal to root port. (Base 0)
  /// For LP, Set 0 - 5
  /// For H,  Set 0 - 15
  /// Note that if GbE is enabled, ClkReq assigned to GbE will not be available for Root Port.
  ///
  switch (BoardId) {
    // CLKREQ
    case BoardIdWhiskeyLakeRvp:
      Clock[0].PcieClock.ClockUsage = PCIE_PCH + 1;
      Clock[1].PcieClock.ClockUsage = PCIE_PCH + 8;
      Clock[2].PcieClock.ClockUsage = LAN_CLOCK;
      Clock[3].PcieClock.ClockUsage = PCIE_PCH + 13;
      Clock[4].PcieClock.ClockUsage = PCIE_PCH + 4;
      Clock[5].PcieClock.ClockUsage = PCIE_PCH + 14;
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
  PCD32_BLOB *UsbPort20Afe;

  UsbPort20Afe = AllocateZeroPool (PCH_MAX_USB2_PORTS * sizeof (PCD32_BLOB));
  ASSERT (UsbPort20Afe != NULL);
  if (UsbPort20Afe == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // USB2 AFE settings.
  //
  UsbPort20Afe[0].Info.Petxiset   = 7;
  UsbPort20Afe[0].Info.Txiset     = 5;
  UsbPort20Afe[0].Info.Predeemp   = 3;
  UsbPort20Afe[0].Info.Pehalfbit  = 0;

  UsbPort20Afe[1].Info.Petxiset   = 7;
  UsbPort20Afe[1].Info.Txiset     = 5;
  UsbPort20Afe[1].Info.Predeemp   = 3;
  UsbPort20Afe[1].Info.Pehalfbit  = 0;

  UsbPort20Afe[2].Info.Petxiset   = 7;
  UsbPort20Afe[2].Info.Txiset     = 5;
  UsbPort20Afe[2].Info.Predeemp   = 3;
  UsbPort20Afe[2].Info.Pehalfbit  = 0;

  UsbPort20Afe[3].Info.Petxiset   = 7;
  UsbPort20Afe[3].Info.Txiset     = 5;
  UsbPort20Afe[3].Info.Predeemp   = 3;
  UsbPort20Afe[3].Info.Pehalfbit  = 0;

  UsbPort20Afe[4].Info.Petxiset   = 7;
  UsbPort20Afe[4].Info.Txiset     = 5;
  UsbPort20Afe[4].Info.Predeemp   = 3;
  UsbPort20Afe[4].Info.Pehalfbit  = 0;

  UsbPort20Afe[5].Info.Petxiset   = 7;
  UsbPort20Afe[5].Info.Txiset     = 5;
  UsbPort20Afe[5].Info.Predeemp   = 3;
  UsbPort20Afe[5].Info.Pehalfbit  = 0;

  UsbPort20Afe[6].Info.Petxiset   = 7;
  UsbPort20Afe[6].Info.Txiset     = 5;
  UsbPort20Afe[6].Info.Predeemp   = 3;
  UsbPort20Afe[6].Info.Pehalfbit  = 0;

  UsbPort20Afe[7].Info.Petxiset   = 7;
  UsbPort20Afe[7].Info.Txiset     = 5;
  UsbPort20Afe[7].Info.Predeemp   = 3;
  UsbPort20Afe[7].Info.Pehalfbit  = 0;

  UsbPort20Afe[8].Info.Petxiset   = 7;
  UsbPort20Afe[8].Info.Txiset     = 5;
  UsbPort20Afe[8].Info.Predeemp   = 3;
  UsbPort20Afe[8].Info.Pehalfbit  = 0;

  UsbPort20Afe[9].Info.Petxiset   = 7;
  UsbPort20Afe[9].Info.Txiset     = 5;
  UsbPort20Afe[9].Info.Predeemp   = 3;
  UsbPort20Afe[9].Info.Pehalfbit  = 0;

  //
  // USB Port Over Current Pin
  //
  PcdSet8S (PcdUsb20OverCurrentPinPort0, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb20OverCurrentPinPort1, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb20OverCurrentPinPort2, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb20OverCurrentPinPort3, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb20OverCurrentPinPort4, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb20OverCurrentPinPort5, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb20OverCurrentPinPort6, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb20OverCurrentPinPort7, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb20OverCurrentPinPort8, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb20OverCurrentPinPort9, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb20OverCurrentPinPort10, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb20OverCurrentPinPort11, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb20OverCurrentPinPort12, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb20OverCurrentPinPort13, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb20OverCurrentPinPort14, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb20OverCurrentPinPort15, UsbOverCurrentPinMax);

  PcdSet8S (PcdUsb30OverCurrentPinPort0, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb30OverCurrentPinPort1, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb30OverCurrentPinPort2, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb30OverCurrentPinPort3, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb30OverCurrentPinPort4, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb30OverCurrentPinPort5, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb30OverCurrentPinPort6, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb30OverCurrentPinPort7, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb30OverCurrentPinPort8, UsbOverCurrentPinMax);
  PcdSet8S (PcdUsb30OverCurrentPinPort9, UsbOverCurrentPinMax);

  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      PcdSet8S (PcdUsb20OverCurrentPinPort0, UsbOverCurrentPin2);
      PcdSet8S (PcdUsb20OverCurrentPinPort1, UsbOverCurrentPinSkip);
      PcdSet8S (PcdUsb20OverCurrentPinPort2, UsbOverCurrentPin2);
      PcdSet8S (PcdUsb20OverCurrentPinPort3, UsbOverCurrentPin2);
      PcdSet8S (PcdUsb20OverCurrentPinPort4, UsbOverCurrentPin3);
      PcdSet8S (PcdUsb20OverCurrentPinPort5, UsbOverCurrentPin3);
      PcdSet8S (PcdUsb20OverCurrentPinPort6, UsbOverCurrentPin3);
      PcdSet8S (PcdUsb20OverCurrentPinPort7, UsbOverCurrentPin3);
      PcdSet8S (PcdUsb20OverCurrentPinPort8, UsbOverCurrentPinSkip);
      PcdSet8S (PcdUsb20OverCurrentPinPort9, UsbOverCurrentPinSkip);
      PcdSet8S (PcdUsb20OverCurrentPinPort10, UsbOverCurrentPinSkip);
      PcdSet8S (PcdUsb20OverCurrentPinPort11, UsbOverCurrentPinSkip);
      PcdSet8S (PcdUsb20OverCurrentPinPort12, UsbOverCurrentPinSkip);
      PcdSet8S (PcdUsb20OverCurrentPinPort13, UsbOverCurrentPinSkip);
      PcdSet8S (PcdUsb20OverCurrentPinPort14, UsbOverCurrentPinSkip);
      PcdSet8S (PcdUsb20OverCurrentPinPort15, UsbOverCurrentPinSkip);

      PcdSet8S (PcdUsb30OverCurrentPinPort0, UsbOverCurrentPin2);
      PcdSet8S (PcdUsb30OverCurrentPinPort1, UsbOverCurrentPinSkip);
      PcdSet8S (PcdUsb30OverCurrentPinPort2, UsbOverCurrentPin2);
      PcdSet8S (PcdUsb30OverCurrentPinPort3, UsbOverCurrentPin2);
      PcdSet8S (PcdUsb30OverCurrentPinPort4, UsbOverCurrentPinSkip);
      PcdSet8S (PcdUsb30OverCurrentPinPort5, UsbOverCurrentPinSkip);
      PcdSet8S (PcdUsb30OverCurrentPinPort6, UsbOverCurrentPinSkip);
      PcdSet8S (PcdUsb30OverCurrentPinPort7, UsbOverCurrentPinSkip);
      PcdSet8S (PcdUsb30OverCurrentPinPort8, UsbOverCurrentPinSkip);
      PcdSet8S (PcdUsb30OverCurrentPinPort9, UsbOverCurrentPinSkip);

      // USB2.0 AFE settings
      UsbPort20Afe[0].Info.Petxiset   = 6;
      UsbPort20Afe[0].Info.Txiset     = 0;
      UsbPort20Afe[0].Info.Predeemp   = 3;
      UsbPort20Afe[0].Info.Pehalfbit  = 0;

      UsbPort20Afe[1].Info.Petxiset   = 6;
      UsbPort20Afe[1].Info.Txiset     = 0;
      UsbPort20Afe[1].Info.Predeemp   = 3;
      UsbPort20Afe[1].Info.Pehalfbit  = 0;

      UsbPort20Afe[2].Info.Petxiset   = 6;
      UsbPort20Afe[2].Info.Txiset     = 0;
      UsbPort20Afe[2].Info.Predeemp   = 3;
      UsbPort20Afe[2].Info.Pehalfbit  = 0;

      UsbPort20Afe[3].Info.Petxiset   = 6;
      UsbPort20Afe[3].Info.Txiset     = 0;
      UsbPort20Afe[3].Info.Predeemp   = 3;
      UsbPort20Afe[3].Info.Pehalfbit  = 0;

      UsbPort20Afe[4].Info.Petxiset   = 6;
      UsbPort20Afe[4].Info.Txiset     = 0;
      UsbPort20Afe[4].Info.Predeemp   = 3;
      UsbPort20Afe[4].Info.Pehalfbit  = 0;

      UsbPort20Afe[5].Info.Petxiset   = 6;
      UsbPort20Afe[5].Info.Txiset     = 0;
      UsbPort20Afe[5].Info.Predeemp   = 3;
      UsbPort20Afe[5].Info.Pehalfbit  = 0;

      UsbPort20Afe[6].Info.Petxiset   = 6;
      UsbPort20Afe[6].Info.Txiset     = 0;
      UsbPort20Afe[6].Info.Predeemp   = 3;
      UsbPort20Afe[6].Info.Pehalfbit  = 0;

      UsbPort20Afe[7].Info.Petxiset   = 6;
      UsbPort20Afe[7].Info.Txiset     = 0;
      UsbPort20Afe[7].Info.Predeemp   = 3;
      UsbPort20Afe[7].Info.Pehalfbit  = 0;

      UsbPort20Afe[8].Info.Petxiset   = 6;
      UsbPort20Afe[8].Info.Txiset     = 0;
      UsbPort20Afe[8].Info.Predeemp   = 3;
      UsbPort20Afe[8].Info.Pehalfbit  = 0;

      UsbPort20Afe[9].Info.Petxiset   = 6;
      UsbPort20Afe[9].Info.Txiset     = 0;
      UsbPort20Afe[9].Info.Predeemp   = 3;
      UsbPort20Afe[9].Info.Pehalfbit  = 0;
      break;
  }

  //
  // Save USB2.0 AFE blobs
  //
  PcdSet32S (PcdUsb20Port0Afe,  UsbPort20Afe[ 0].Blob);
  PcdSet32S (PcdUsb20Port1Afe,  UsbPort20Afe[ 1].Blob);
  PcdSet32S (PcdUsb20Port2Afe,  UsbPort20Afe[ 2].Blob);
  PcdSet32S (PcdUsb20Port3Afe,  UsbPort20Afe[ 3].Blob);
  PcdSet32S (PcdUsb20Port4Afe,  UsbPort20Afe[ 4].Blob);
  PcdSet32S (PcdUsb20Port5Afe,  UsbPort20Afe[ 5].Blob);
  PcdSet32S (PcdUsb20Port6Afe,  UsbPort20Afe[ 6].Blob);
  PcdSet32S (PcdUsb20Port7Afe,  UsbPort20Afe[ 7].Blob);
  PcdSet32S (PcdUsb20Port8Afe,  UsbPort20Afe[ 8].Blob);
  PcdSet32S (PcdUsb20Port9Afe,  UsbPort20Afe[ 9].Blob);
  PcdSet32S (PcdUsb20Port10Afe, UsbPort20Afe[10].Blob);
  PcdSet32S (PcdUsb20Port11Afe, UsbPort20Afe[11].Blob);
  PcdSet32S (PcdUsb20Port12Afe, UsbPort20Afe[12].Blob);
  PcdSet32S (PcdUsb20Port13Afe, UsbPort20Afe[13].Blob);
  PcdSet32S (PcdUsb20Port14Afe, UsbPort20Afe[14].Blob);
  PcdSet32S (PcdUsb20Port15Afe, UsbPort20Afe[15].Blob);

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
  //
  // GPIO Group Tier
  //
  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      PcdSet32S (PcdGpioGroupToGpeDw0, GPIO_CNL_LP_GROUP_GPP_G);
      PcdSet32S (PcdGpioGroupToGpeDw1, GPIO_CNL_LP_GROUP_SPI);
      PcdSet32S (PcdGpioGroupToGpeDw2, GPIO_CNL_LP_GROUP_GPP_E);
      break;

    default:
      PcdSet32S (PcdGpioGroupToGpeDw0, 0);
      PcdSet32S (PcdGpioGroupToGpeDw1, 0);
      PcdSet32S (PcdGpioGroupToGpeDw2, 0);
      break;
  }

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
    case BoardIdWhiskeyLakeRvp:
      PcdSet32S (PcdBoardGpioTablePreMem, (UINTN) mGpioTableWhlUDdr4PreMem);
      PcdSet16S (PcdBoardGpioTablePreMemSize, mGpioTableWhlUDdr4PreMemSize);
      PcdSet32S (PcdBoardGpioTableWwanOnEarlyPreMem, (UINTN) mGpioTableWhlUDdr4WwanOnEarlyPreMem);
      PcdSet16S (PcdBoardGpioTableWwanOnEarlyPreMemSize, mGpioTableWhlUDdr4WwanOnEarlyPreMemSize);
      PcdSet32S (PcdBoardGpioTableWwanOffEarlyPreMem, (UINTN) mGpioTableWhlUDdr4WwanOffEarlyPreMem);
      PcdSet16S (PcdBoardGpioTableWwanOffEarlyPreMemSize, mGpioTableWhlUDdr4WwanOffEarlyPreMemSize);
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
  //
  // Update PmCofig policy: output voltage of VCCPRIMCORE RAIL when SLP_S0# is asserted based on board HW design
  // 1) Discete VR or Non Premium PMIC: 0.75V (PcdSlpS0Vm075VSupport)
  // 2) Premium PMIC: runtime control for voltage (PcdSlpS0VmRuntimeControl)
  // Only applys to board with PCH-LP. Board with Discrete PCH doesn't need this setting.
  //
  switch (BoardId) {
    // Discrete VR solution
    case BoardIdWhiskeyLakeRvp:
      PcdSetBoolS (PcdSlpS0VmRuntimeControl, FALSE);
      PcdSetBoolS (PcdSlpS0Vm070VSupport, FALSE);
      PcdSetBoolS (PcdSlpS0Vm075VSupport, TRUE);
      break;

    default:
      PcdSetBoolS (PcdSlpS0VmRuntimeControl, FALSE);
      PcdSetBoolS (PcdSlpS0Vm070VSupport, FALSE);
      PcdSetBoolS (PcdSlpS0Vm075VSupport, FALSE);
      break;
  }

  return EFI_SUCCESS;
}

