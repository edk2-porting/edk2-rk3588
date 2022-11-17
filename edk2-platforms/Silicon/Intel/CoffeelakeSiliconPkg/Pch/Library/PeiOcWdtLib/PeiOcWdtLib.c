/** @file
  The PEI Library Implements OcWdt Support.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/Wdt.h>
#include <Library/PchWdtCommonLib.h>
#include <Library/PmcLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/IoLib.h>
#include <Register/PchRegsPmc.h>

static WDT_PPI mWdtPpi = {
  WdtReloadAndStart,
  WdtCheckStatus,
  WdtDisable,
  WdtAllowKnownReset,
  IsWdtRequired,
  IsWdtEnabled
};

static EFI_PEI_PPI_DESCRIPTOR  mInstallWdtPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gWdtPpiGuid,
  &mWdtPpi
};

/**
  Reads PCH registers to check if platform wakes from S3/S4

  @retval TRUE                    if platfrom wakes from S3/S4
  @retval FALSE                   otherwise
**/
BOOLEAN
IsWakeFromS3S4 (
  VOID
  )
{
  PMC_SLEEP_STATE  SleepType;

  SleepType = PmcGetSleepTypeAfterWake ();
  if ((SleepType == PmcS3SleepState) || (SleepType == PmcS4SleepState)) {
    return TRUE;
  }

  return FALSE;

}

/**
  Check for unexpected reset.
  If there was an unexpected reset, enforces WDT expiration.
**/
VOID
OcWdtResetCheck (
  VOID
  )
{
  UINT32      Readback;

  Readback = IoRead32 (WdtGetAddress ());
  DEBUG ((DEBUG_INFO, "(WDT) OcWdtResetCheck()\n"));

  ///
  /// If there was a WDT expiration, set Failure Status and clear timeout status bits
  /// Timeout status bits are cleared by writing '1'
  ///
  if (Readback & (B_ACPI_IO_OC_WDT_CTL_ICCSURV_STS | B_ACPI_IO_OC_WDT_CTL_NO_ICCSURV_STS)) {
    DEBUG ((DEBUG_ERROR, "(WDT) Expiration detected.\n", Readback));
    Readback |= B_ACPI_IO_OC_WDT_CTL_FAILURE_STS;
    Readback |= (B_ACPI_IO_OC_WDT_CTL_ICCSURV_STS | B_ACPI_IO_OC_WDT_CTL_NO_ICCSURV_STS);
    Readback &= ~(B_ACPI_IO_OC_WDT_CTL_UNXP_RESET_STS);
  } else {
    ///
    /// If there was unexpected reset but no WDT expiration and no resume from S3/S4,
    /// clear unexpected reset status and enforce expiration. This is to inform Firmware
    /// which has no access to unexpected reset status bit, that something went wrong.
    ///
    if ((Readback & B_ACPI_IO_OC_WDT_CTL_UNXP_RESET_STS) && !IsWakeFromS3S4()) {
      if (PcdGetBool (PcdOcEnableWdtforDebug)) {
        DEBUG ((DEBUG_ERROR, "(WDT) Unexpected reset detected and ignored.\n"));
        Readback &= ~(B_ACPI_IO_OC_WDT_CTL_FAILURE_STS | B_ACPI_IO_OC_WDT_CTL_UNXP_RESET_STS);
        Readback |= (B_ACPI_IO_OC_WDT_CTL_ICCSURV_STS | B_ACPI_IO_OC_WDT_CTL_NO_ICCSURV_STS);
      } else {
        DEBUG ((DEBUG_ERROR, "(WDT) Unexpected reset detected. Enforcing Wdt expiration.\n"));
        WdtReloadAndStart (1);
        ///
        /// wait for reboot caused by WDT expiration
        ///
        CpuDeadLoop ();
      }
    } else {
      ///
      /// No WDT expiration and no unexpected reset - clear Failure status
      ///
      DEBUG ((DEBUG_INFO, "(WDT) Status OK.\n", Readback));
      Readback &= ~(B_ACPI_IO_OC_WDT_CTL_FAILURE_STS);
      Readback |= (B_ACPI_IO_OC_WDT_CTL_ICCSURV_STS | B_ACPI_IO_OC_WDT_CTL_NO_ICCSURV_STS);
    }
  }

  IoWrite32 (WdtGetAddress (), Readback);

  return;
}

/**
  This function install WDT PPI

  @retval EFI_STATUS  Results of the installation of the WDT PPI
**/
EFI_STATUS
EFIAPI
OcWdtInit (
  VOID
  )
{
  EFI_STATUS  Status;

  Status = PeiServicesInstallPpi (&mInstallWdtPpi);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

