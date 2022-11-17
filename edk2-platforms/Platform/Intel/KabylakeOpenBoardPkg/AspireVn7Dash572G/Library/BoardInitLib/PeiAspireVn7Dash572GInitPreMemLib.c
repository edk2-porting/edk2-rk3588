/** @file

Copyright (c) 2017 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchResetLib.h>
#include <Library/SiliconInitLib.h>
#include <Library/TimerLib.h>
#include <Library/PeiLib.h>

#include <Library/GpioLib.h>
#include <GpioPinsSklLp.h>
#include <IndustryStandard/TpmPtp.h>
#include <PchAccess.h>

#include "PeiAspireVn7Dash572GInitLib.h"

#include <ConfigBlock.h>
#include <ConfigBlock/MemoryConfig.h>

#ifndef STALL_ONE_MILLI_SECOND
#define STALL_ONE_MILLI_SECOND  1000
#endif

//
// Reference RCOMP resistors on motherboard - for Aspire VN7-572G
//
GLOBAL_REMOVE_IF_UNREFERENCED const UINT16 RcompResistorAspireVn7Dash572G[SA_MRC_MAX_RCOMP] = { 121, 80, 100 };
//
// RCOMP target values for RdOdt, WrDS, WrDSCmd, WrDSCtl, WrDSClk - for Aspire VN7-572G
//
GLOBAL_REMOVE_IF_UNREFERENCED const UINT16 RcompTargetAspireVn7Dash572G[SA_MRC_MAX_RCOMP_TARGETS] = { 100, 40, 40, 23, 40 };

//
// dGPU power GPIO definitions
#define DGPU_PRESENT	GPIO_SKL_LP_GPP_A20	/* Active low */
#define DGPU_HOLD_RST	GPIO_SKL_LP_GPP_B4	/* Active low */
#define DGPU_PWR_EN	GPIO_SKL_LP_GPP_B21	/* Active low */

EFI_STATUS
EFIAPI
AspireVn7Dash572GBoardDetect (
  VOID
  );

/**
  Aspire VN7-572G board configuration init function for PEI pre-memory phase.

**/
VOID
AspireVn7Dash572GInitPreMem (
  VOID
  )
{
  //
  // HSIO PTSS Table
  //
  PcdSet32S (PcdSpecificLpHsioPtssTable1,     (UINTN) PchLpHsioPtss_AspireVn7Dash572G);
  PcdSet16S (PcdSpecificLpHsioPtssTable1Size, (UINTN) PchLpHsioPtss_AspireVn7Dash572G_Size);
  PcdSet32S (PcdSpecificLpHsioPtssTable2,     (UINTN) PchLpHsioPtss_AspireVn7Dash572G);
  PcdSet16S (PcdSpecificLpHsioPtssTable2Size, (UINTN) PchLpHsioPtss_AspireVn7Dash572G_Size);

  //
  // DRAM related definition
  //
  PcdSet8S (PcdSaMiscUserBd, 5);     // ULT/ULX/Mobile Halo
  PcdSet8S (PcdMrcCaVrefConfig, 2);  // DDR4: "VREF_CA to CH_A and VREF_DQ_B to CH_B"
  // TODO: Clear Dq/Dqs?
  PcdSetBoolS (PcdMrcDqPinsInterleaved, TRUE);

  PcdSet32S (PcdMrcRcompResistor, (UINTN) RcompResistorAspireVn7Dash572G);
  PcdSet32S (PcdMrcRcompTarget,   (UINTN) RcompTargetAspireVn7Dash572G);
  //
  // Example policy for DIMM slots implementation boards:
  // 1. Assign Smbus address of DIMMs and SpdData will be updated later
  //    by reading from DIMM SPD.
  // 2. No need to apply hardcoded SpdData buffers here for such board.
  //   Example:
  //   PcdMrcSpdAddressTable0 = 0xA0
  //   PcdMrcSpdAddressTable1 = 0xA2
  //   PcdMrcSpdAddressTable2 = 0xA4
  //   PcdMrcSpdAddressTable3 = 0xA6
  //   PcdMrcSpdData = 0
  //   PcdMrcSpdDataSize = 0
  //
  PcdSet8S (PcdMrcSpdAddressTable0, 0xA0);
  PcdSet8S (PcdMrcSpdAddressTable1, 0);
  PcdSet8S (PcdMrcSpdAddressTable2, 0xA4);
  PcdSet8S (PcdMrcSpdAddressTable3, 0);
  PcdSet32S (PcdMrcSpdData, 0);
  PcdSet16S (PcdMrcSpdDataSize, 0);
}

/**
  Configures GPIO before memory is ready.

**/
EFI_STATUS
EFIAPI
GpioInitPreMem (
  VOID
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "GpioInitPreMem() Start\n"));

  Status = GpioConfigurePads (mGpioTableAspireVn7Dash572G_earlySize, mGpioTableAspireVn7Dash572G_early);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to configure early GPIOs!\n"));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "GpioInitPreMem() End\n"));
  return EFI_SUCCESS;
}

/**
  Initialises the dGPU.

**/
VOID
DgpuPowerOn (
  VOID
  )
{
  UINT32         OutputVal;

  DEBUG ((DEBUG_INFO, "DgpuPowerOn() Start\n"));

  GpioGetOutputValue (DGPU_PRESENT, &OutputVal);
  if (!OutputVal) {
    DEBUG ((DEBUG_INFO, "dGPU present, enable power...\n"));
    GpioSetOutputValue (DGPU_HOLD_RST, 0);  // Assert dGPU_HOLD_RST#
    MicroSecondDelay (2 * STALL_ONE_MILLI_SECOND);
    GpioSetOutputValue (DGPU_PWR_EN, 0);    // Assert dGPU_PWR_EN#
    MicroSecondDelay (7 * STALL_ONE_MILLI_SECOND);
    GpioSetOutputValue (DGPU_HOLD_RST, 1);  // Deassert dGPU_HOLD_RST#
    MicroSecondDelay (30 * STALL_ONE_MILLI_SECOND);
  } else {
    DEBUG ((DEBUG_INFO, "dGPU not present, disable power...\n"));
    GpioSetOutputValue (DGPU_HOLD_RST, 0);  // Assert dGPU_HOLD_RST#
    GpioSetOutputValue (DGPU_PWR_EN, 1);    // Deassert dGPU_PWR_EN#
  }

  DEBUG ((DEBUG_INFO, "DgpuPowerOn() End\n"));
}

/**
  Configure LPC.

**/
VOID
LpcInit (
  VOID
  )
{
  //
  // Program and Enable EC (sideband) Port Addresses and range
  //
  PchLpcGenIoRangeSet (0x68, 0x08);

  //
  // Program and Enable EC (index) Port Addresses and range
  //
  PchLpcGenIoRangeSet (0x1200, 0x10);
}

/**
  Configure GPIO and SIO before memory ready.

  @retval  EFI_SUCCESS   Operation success.
**/
EFI_STATUS
EFIAPI
AspireVn7Dash572GBoardInitBeforeMemoryInit (
  VOID
  )
{
  EFI_STATUS    Status;

  Status = GpioInitPreMem ();
  if (!EFI_ERROR (Status)) {
    DgpuPowerOn ();
  }
  AspireVn7Dash572GInitPreMem ();

  ///
  /// Do basic PCH init
  ///
  SiliconInit ();

  //
  // Fix-up LPC configuration
  // Enable I/O decoding for COM1(3F8h-3FFh), COM2(2F8h-2FFh), I/O port 2Eh/2Fh, 4Eh/4Fh, 60h/64h and 62h/66h.
  //
  PchLpcIoDecodeRangesSet (PcdGet16 (PcdLpcIoDecodeRange));
  PchLpcIoEnableDecodingSet (PcdGet16 (PchLpcIoEnableDecoding));

  //
  // Install PCH RESET PPI and EFI RESET2 PeiService
  //
  Status = PchInitializeReset ();
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
  Configure GPIO and SIO before memory ready.

  @retval  EFI_SUCCESS   Operation success.
**/
EFI_STATUS
EFIAPI
AspireVn7Dash572GBoardInitAfterMemoryInit (
  VOID
  )
{
  EFI_STATUS  Status;

  // BUGBUG: Workaround for a misbehaving system firmware not setting goIdle
  // - Based on prior investigation for coreboot, I suspect FSP
  if ((MmioRead32 (0xFED40044) & PTP_CRB_CONTROL_AREA_STATUS_TPM_IDLE) == 0) {
    DEBUG ((DEBUG_WARN, "TPM no-IdleBypass bug: workaround enabled\n"));
    MmioWrite32 (0xFED40040, PTP_CRB_CONTROL_AREA_REQUEST_GO_IDLE);
  }

  // Program the same 64K range of EC memory as vendor FW
  Status = PchLpcMemRangeSet (0xFE800000);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Failed to enable LGMR. Were ACPI tables built for LGMR memory map?\n"));
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AspireVn7Dash572GBoardDebugInit (
  VOID
  )
{
  ///
  /// Do Early PCH init
  ///
  EarlySiliconInit ();
  LpcInit ();

  // NB: MinPlatform specification defines platform initialisation flow.
  // Therefore, we defer board detection until we can program LPC.
  // - Alternatively, move the preceding calls to BoardDetect()
  AspireVn7Dash572GBoardDetect ();

  return EFI_SUCCESS;
}

EFI_BOOT_MODE
EFIAPI
AspireVn7Dash572GBoardBootModeDetect (
  VOID
  )
{
  UINT16         ABase;
  UINT32         SleepType;

  DEBUG ((DEBUG_INFO, "Performing boot mode detection\n"));

  // TODO: Perform advanced detection (recovery/capsule)
  // FIXME: This violates PI specification? But BOOT_WITH* would always take precedence
  //        over BOOT_ON_S{4,5}...
  PchAcpiBaseGet (&ABase);
  SleepType = IoRead32 (ABase + R_PCH_ACPI_PM1_CNT) & B_PCH_ACPI_PM1_CNT_SLP_TYP;

  switch (SleepType) {
    case V_PCH_ACPI_PM1_CNT_S3:
      return BOOT_ON_S3_RESUME;
    case V_PCH_ACPI_PM1_CNT_S4:
      return BOOT_ON_S4_RESUME;
//    case V_PCH_ACPI_PM1_CNT_S5:
//      return BOOT_ON_S5_RESUME;
    default:
      return BOOT_WITH_FULL_CONFIGURATION;
  }
}
