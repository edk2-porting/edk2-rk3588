/** @file
    IPMI FRB Driver.

Copyright (c) 2018 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/IpmiCommandLib.h>
#include <IndustryStandard/Ipmi.h>

EFI_STATUS
EfiDisableFrb (
  VOID
  )
/*++

  Routine Description:
    This routine disables the specified FRB timer.

  Arguments:
    This        - This pointer
    FrbType     - Type of FRB timer to get data on

  Returns:
    EFI_SUCCESS     - FRB timer was disabled
    EFI_ABORTED     - Timer was already stopped
    EFI_UNSUPPORTED - This type of FRB timer is not supported.

--*/
{
  EFI_STATUS                       Status;
  IPMI_SET_WATCHDOG_TIMER_REQUEST  SetWatchdogTimer;
  UINT8                            CompletionCode;
  IPMI_GET_WATCHDOG_TIMER_RESPONSE GetWatchdogTimer;

  Status = IpmiGetWatchdogTimer (&GetWatchdogTimer);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Check if timer is still running, if not abort disable routine.
  //
  if (GetWatchdogTimer.TimerUse.Bits.TimerRunning == 0) {
    return EFI_ABORTED;
  }

  ZeroMem (&SetWatchdogTimer, sizeof(SetWatchdogTimer));
  //
  // Just flip the Timer Use bit. This should release the timer.
  //
  SetWatchdogTimer.TimerUse.Bits.TimerRunning    = 0;
  SetWatchdogTimer.TimerUse.Bits.TimerUse        = IPMI_WATCHDOG_TIMER_BIOS_FRB2;
  SetWatchdogTimer.TimerUseExpirationFlagsClear &= ~BIT2;
  SetWatchdogTimer.TimerUseExpirationFlagsClear |= BIT1 | BIT4;

  Status = IpmiSetWatchdogTimer (&SetWatchdogTimer, &CompletionCode);
  return Status;
}

VOID
EFIAPI
DisableFRB2Handler (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
/*++

  Routine Description:
    Disables FRB2. This function gets called each time the
    EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled

  Arguments:
    Standard event notification function arguments:
    Event         - the event that is signaled.
    Context       - not used here.

  Returns:

--*/
{
  DEBUG((DEBUG_ERROR, "!!! enter DisableFRB2Handler()!!!\n"));

  EfiDisableFrb ();
}

EFI_STATUS
CheckForAndReportErrors(
  VOID
  )
/*++

  Routine Description:
    Check the Watchdog timer expiration flags and report the kind of watchdog
    timeout occurred to the Error Manager.

  Arguments:

  Returns:
    EFI_SUCCESS     - Errors retrieved and reported

--*/
{
  EFI_STATUS                          Status;
  IPMI_GET_WATCHDOG_TIMER_RESPONSE    GetWatchdogTimer;
  IPMI_SET_WATCHDOG_TIMER_REQUEST     SetWatchdogTimer;
  UINT8                               CompletionCode;

  //
  // Get the Watchdog timer info to find out what kind of timer expiration occurred.
  //
  Status = IpmiGetWatchdogTimer (&GetWatchdogTimer);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // If FRB2 Failure occurred, report it to the error manager and log a SEL.
  //
  if ((GetWatchdogTimer.TimerUseExpirationFlagsClear & BIT1) != 0) {
    //
    // Report the FRB2 time-out error
    //
  } else if ((GetWatchdogTimer.TimerUseExpirationFlagsClear & BIT3) != 0) {
    //
    // Report the OS Watchdog timer failure
    //
  }

  //
  // Need to clear Timer expiration flags after checking.
  //
  ZeroMem (&SetWatchdogTimer, sizeof(SetWatchdogTimer));
  SetWatchdogTimer.TimerUse                       = GetWatchdogTimer.TimerUse;
  SetWatchdogTimer.TimerActions                   = GetWatchdogTimer.TimerActions;
  SetWatchdogTimer.PretimeoutInterval             = GetWatchdogTimer.PretimeoutInterval;
  SetWatchdogTimer.TimerUseExpirationFlagsClear   = GetWatchdogTimer.TimerUseExpirationFlagsClear;
  SetWatchdogTimer.InitialCountdownValue          = GetWatchdogTimer.InitialCountdownValue;
  SetWatchdogTimer.TimerUse.Bits.TimerRunning     = 1;
  SetWatchdogTimer.TimerUseExpirationFlagsClear  |= BIT1 | BIT2 | BIT3;

  Status = IpmiSetWatchdogTimer (&SetWatchdogTimer, &CompletionCode);

  return Status;
}

EFI_STATUS
ReportFrb2Status (
  VOID
  )
/*++

  Routine Description:
    This routine is built only when DEBUG_MODE is enabled.  It is used
    to report the status of FRB2 when the FRB2 driver is installed.

  Arguments:
    none

  Returns:
    EFI_SUCCESS: All info was retrieved and reported
    EFI_ERROR:   There was an error during info retrieval

--*/
{
  EFI_STATUS                          Status;
  IPMI_GET_WATCHDOG_TIMER_RESPONSE    GetWatchdogTimer;

  //
  // Get the Watchdog timer info to find out what kind of timer expiration occurred.
  //
  Status = IpmiGetWatchdogTimer (&GetWatchdogTimer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Failed to get Watchdog Timer info from BMC.\n"));
    return Status;
  }

  //
  // Check if timer is running, report status to DEBUG_MODE output.
  //
  if (GetWatchdogTimer.TimerUse.Bits.TimerRunning == 1) {
    DEBUG ((DEBUG_INFO, "FRB2 Timer is running.\n"));
  } else {
    DEBUG ((DEBUG_INFO, "FRB2 Timer is not running.\n"));
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
FrbDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
/*++

  Routine Description:
    This is the standard EFI driver point. This function intitializes
    the private data required for creating FRB Driver.

  Arguments:
    ImageHandle     - Handle for the image of this driver
    SystemTable     - Pointer to the EFI System Table

  Returns:
    EFI_SUCCESS     - Protocol successfully started and installed
    EFI_UNSUPPORTED - Protocol can't be started

--*/
{
  EFI_EVENT   ReadyToBootEvent;
  EFI_STATUS  Status;

  CheckForAndReportErrors();
  ReportFrb2Status ();

  //
  // Register the event to Disable FRB2 before Boot.
  //
  Status = EfiCreateEventReadyToBootEx (
            TPL_NOTIFY,
            DisableFRB2Handler,
            NULL,
            &ReadyToBootEvent
            );

  return Status;
}
