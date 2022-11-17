/** @file
  Library that contains common parts of WdtPei and WdtDxe. Not a standalone module.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PmcLib.h>
#include <Register/PchRegsPmc.h>

GLOBAL_REMOVE_IF_UNREFERENCED UINT8     mAllowExpectedReset = 0;

/**
  Reads LPC bridge to get Watchdog Timer address


  @retval UINT32                  Watchdog's address
**/
UINT32
WdtGetAddress (
  VOID
  )
{
  return PmcGetAcpiBase () + R_ACPI_IO_OC_WDT_CTL;
}

/**
  Reloads WDT with new timeout value and starts it. Also sets Unexpected Reset bit, which
  causes the next reset to be treated as watchdog expiration - unless AllowKnownReset()
  function was called too.

  @param[in] TimeoutValue         Time in seconds before WDT times out. Supported range = 1 - 1024.

  @retval EFI_SUCCESS             if everything's OK
  @retval EFI_INVALID_PARAMETER   if TimeoutValue parameter is wrong
**/
EFI_STATUS
EFIAPI
WdtReloadAndStart (
  IN  UINT32  TimeoutValue
  )
{
  UINT32  Readback;

  DEBUG ((DEBUG_INFO, "\n(Wdt) ReloadAndStartTimer(%d)\n", TimeoutValue));

  if ((TimeoutValue > B_ACPI_IO_OC_WDT_CTL_TOV_MASK) || (TimeoutValue == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  Readback = IoRead32 (WdtGetAddress ());
  Readback |= (B_ACPI_IO_OC_WDT_CTL_EN | B_ACPI_IO_OC_WDT_CTL_FORCE_ALL | B_ACPI_IO_OC_WDT_CTL_ICCSURV);
  if (mAllowExpectedReset == 0) {
    Readback |= B_ACPI_IO_OC_WDT_CTL_UNXP_RESET_STS;
  }

  if (PcdGetBool (PcdOcEnableWdtforDebug) == FALSE) {
    ///
    /// WDT will not be turned on. This is to prevent platform reboots triggered
    /// by WDT expiration, which can be expected when processor is halted for debugging
    ///
    Readback &= ~(B_ACPI_IO_OC_WDT_CTL_EN | B_ACPI_IO_OC_WDT_CTL_FORCE_ALL | B_ACPI_IO_OC_WDT_CTL_UNXP_RESET_STS);
    DEBUG ((DEBUG_INFO, "(Wdt) Wdt disabled in Debug BIOS\n"));
  }

  Readback &= ~(B_ACPI_IO_OC_WDT_CTL_TOV_MASK);
  Readback |= ((TimeoutValue - 1) & B_ACPI_IO_OC_WDT_CTL_TOV_MASK);
  IoWrite32 (WdtGetAddress (), Readback);
  Readback |= B_ACPI_IO_OC_WDT_CTL_RLD;
  IoWrite32 (WdtGetAddress (), Readback);
  return EFI_SUCCESS;
}

/**
  Disables WDT timer.


**/
VOID
EFIAPI
WdtDisable (
  VOID
  )
{
  UINT32  Readback;

  DEBUG ((DEBUG_INFO, "(Wdt) DisableTimer\n"));

  Readback = IoRead32 (WdtGetAddress ());
  Readback &= ~(B_ACPI_IO_OC_WDT_CTL_EN | B_ACPI_IO_OC_WDT_CTL_FORCE_ALL | B_ACPI_IO_OC_WDT_CTL_UNXP_RESET_STS);
  IoWrite32 (WdtGetAddress (), Readback);
}

/**
  Returns WDT failure status.


  @retval V_ACPI_IO_OC_WDT_CTL_STATUS_FAILURE   If there was WDT expiration or unexpected reset
  @retval V_ACPI_IO_OC_WDT_CTL_STATUS_OK        Otherwise
**/
UINT8
EFIAPI
WdtCheckStatus (
  VOID
  )
{
  UINT32  Readback;

  DEBUG ((DEBUG_INFO, "(Wdt) CheckTimerStatus\n"));

  Readback = IoRead32 (WdtGetAddress ());

  DEBUG ((DEBUG_INFO, "(Wdt) Readback = (%x)\n", Readback));

  if (Readback & B_ACPI_IO_OC_WDT_CTL_FAILURE_STS) {
    DEBUG ((DEBUG_INFO, "(Wdt) Status = FAILURE\n"));
    return V_ACPI_IO_OC_WDT_CTL_STATUS_FAILURE;
  } else {
    return V_ACPI_IO_OC_WDT_CTL_STATUS_OK;
  }
}

/**
  Normally, each reboot performed while watchdog runs is considered a failure.
  This function allows platform to perform expected reboots with WDT running,
  without being interpreted as failures.
  In DXE phase, it is enough to call this function any time before reset.
  In PEI phase, between calling this function and performing reset, ReloadAndStart()
  must not be called.


**/
VOID
EFIAPI
WdtAllowKnownReset (
  VOID
  )
{
  UINT32  Readback;

  DEBUG ((DEBUG_INFO, "(Wdt) AllowKnownReset\n"));

  mAllowExpectedReset = 1;

  Readback  = IoRead32 (WdtGetAddress ());
  Readback &= ~(B_ACPI_IO_OC_WDT_CTL_UNXP_RESET_STS | B_ACPI_IO_OC_WDT_CTL_FORCE_ALL);
  IoWrite32 (WdtGetAddress (), Readback);
}

/**
  Returns information if WDT coverage for the duration of BIOS execution
  was requested by an OS application


  @retval TRUE                    if WDT was requested
  @retval FALSE                   if WDT was not requested
**/
UINT8
EFIAPI
IsWdtRequired (
  VOID
  )
{
  UINT32  Readback;

  DEBUG ((DEBUG_INFO, "(Wdt) IsWdtRequired"));

  Readback = IoRead32 (WdtGetAddress ());


  if ((Readback & B_ACPI_IO_OC_WDT_CTL_AFTER_POST) != 0) {
    DEBUG ((DEBUG_INFO, " - yes\n"));
    return TRUE;
  } else {
    DEBUG ((DEBUG_INFO, " - no\n"));
    return FALSE;
  }

}

/**
  Returns WDT enabled/disabled status.


  @retval TRUE                    if WDT is enabled
  @retval FALSE                   if WDT is disabled
**/
UINT8
EFIAPI
IsWdtEnabled (
  VOID
  )
{
  UINT32  Readback;

  DEBUG ((DEBUG_INFO, "(Wdt) IsWdtEnabled"));

  Readback = IoRead32 (WdtGetAddress ());


  if ((Readback & B_ACPI_IO_OC_WDT_CTL_EN) != 0) {
    DEBUG ((DEBUG_INFO, " - yes\n"));
    return TRUE;
  } else {
    DEBUG ((DEBUG_INFO, " - no\n"));
    return FALSE;
  }

}

/**
  Returns WDT locked status.


  @retval TRUE                    if WDT is locked
  @retval FALSE                   if WDT is unlocked
**/
UINT8
EFIAPI
IsWdtLocked (
  VOID
  )
{
  UINT32  Readback;

  DEBUG ((DEBUG_INFO, "(Wdt) IsWdtLocked"));

  Readback = IoRead32 (WdtGetAddress ());


  if ((Readback & B_ACPI_IO_OC_WDT_CTL_LCK) != 0) {
    DEBUG ((DEBUG_INFO, " - yes\n"));
    return TRUE;
  } else {
    DEBUG ((DEBUG_INFO, " - no\n"));
    return FALSE;
  }
}
