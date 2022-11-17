/** @file

Copyright (c) 2017 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/BoardEcLib.h>
#include <Library/DebugLib.h>
#include <Library/EcLib.h>
#include <Library/GpioLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PciLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/SiliconInitLib.h>
#include <PchAccess.h>
#include <GpioPinsSklLp.h>

#include "PeiAspireVn7Dash572GInitLib.h"

/**
  Init from vendor's PeiOemModule. KbcPeim does not appear to be used
  (It implements commands also found in RtKbcDriver and SmmKbcDriver).

  Mostly, this puts the system back to sleep if the lid is closed during
  an S3 resume.

**/
VOID
EcInit (
  VOID
  )
{
  EFI_BOOT_MODE  BootMode;
  UINT8          PowerRegister;
  UINT8          OutData;
  UINT16         ABase;
  UINT16         Pm1Sts;
  UINT32         GpeSts;

  /* This is called via a "$FNC" in a PeiOemModule pointer table, with "$DPX" on SiInit */
  IoWrite8 (0x6C, 0x5A);  // 6Ch is the EC sideband port
  PeiServicesGetBootMode (&BootMode);
  if (BootMode == BOOT_ON_S3_RESUME) {
    /* "MLID" in LGMR-based memory map is equivalent to "ELID" in EC-based
     * memory map. Vendor firmware accesses through LGMR; remapped
     * - EcCmd* function calls will not remapped */
    EcRead (0x70, &PowerRegister);
    if (!(PowerRegister & BIT1)) {   // Lid is closed
      EcCmd90Read (0x0A, &OutData);
      if (!(OutData & BIT1)) {
        EcCmd91Write (0x0A, OutData | BIT1);
      }

      /* Clear events and go back to sleep */
      PchAcpiBaseGet (&ABase);
      /* Clear ABase PM1_STS - RW/1C set bits */
      Pm1Sts = IoRead16 (ABase + R_PCH_ACPI_PM1_STS);
      IoWrite16 (ABase + R_PCH_ACPI_PM1_STS, Pm1Sts);
      /* Clear ABase GPE0_STS[127:96] - RW/1C set bits */
      GpeSts = IoRead32 (ABase + R_PCH_ACPI_GPE0_STS_127_96);
      IoWrite32 (ABase + R_PCH_ACPI_GPE0_STS_127_96, GpeSts);
      /* Clear xHCI PM_CS[PME_Status] - RW/1C - and disable xHCI PM_CS[PME_En] */
      PciAndThenOr16 (PCI_LIB_ADDRESS(PCI_BUS_NUMBER_PCH_XHCI, PCI_DEVICE_NUMBER_PCH_XHCI, PCI_FUNCTION_NUMBER_PCH_XHCI, R_PCH_XHCI_PWR_CNTL_STS),
                      (UINT16) ~B_PCH_XHCI_PWR_CNTL_STS_PME_EN,
                      B_PCH_XHCI_PWR_CNTL_STS_PME_STS
                      );

      /* Enter S3 sleep */
      IoAndThenOr32 (ABase + R_PCH_ACPI_PM1_CNT,
                     (UINT32) ~(B_PCH_ACPI_PM1_CNT_SLP_TYP | B_PCH_ACPI_PM1_CNT_SLP_EN),
                     V_PCH_ACPI_PM1_CNT_S3
                     );
      IoWrite32 (ABase + R_PCH_ACPI_PM1_CNT, B_PCH_ACPI_PM1_CNT_SLP_EN);
      CpuDeadLoop ();
    }
  }
}

/**
  Aspire VN7-572G board configuration init function for PEI post memory phase.

**/
VOID
AspireVn7Dash572GInit (
  VOID
  )
{
  PcdSet32S (PcdHdaVerbTable, (UINTN) &HdaVerbTableAlc255AspireVn7Dash572G);
  PcdSet32S (PcdDisplayAudioHdaVerbTable, (UINTN) &HdaVerbTableDisplayAudio);
}

/**
  Configures GPIO

**/
EFI_STATUS
EFIAPI
GpioInitPostMem (
  VOID
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "GpioInitPostMem() Start\n"));

  Status = GpioConfigurePads (mGpioTableAspireVn7Dash572GSize, mGpioTableAspireVn7Dash572G);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to configure early GPIOs!\n"));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "GpioInitPostMem() End\n"));
  return EFI_SUCCESS;
}

/**
  Configure GPIO and SIO

  @retval  EFI_SUCCESS   Operation success.
**/
EFI_STATUS
EFIAPI
AspireVn7Dash572GBoardInitBeforeSiliconInit (
  VOID
  )
{
  GpioInitPostMem ();
  AspireVn7Dash572GInit ();

  ///
  /// Do Late PCH init
  ///
  LateSiliconInit ();

  return EFI_SUCCESS;
}

/**
  Notify EC

  @retval  EFI_SUCCESS   Operation success.
**/
EFI_STATUS
EFIAPI
AspireVn7Dash572GBoardInitAfterSiliconInit (
  VOID
  )
{
  EcInit ();

  return EFI_SUCCESS;
}
