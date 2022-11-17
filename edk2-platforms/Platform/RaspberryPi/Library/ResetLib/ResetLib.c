/** @file
 *
 *  Support ResetSystem Runtime call using PSCI calls.
 *  Signals the gRaspberryPiEventResetGuid event group on reset.
 *
 *  Copyright (c) 2018, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) 2014, Linaro Ltd. All rights reserved.
 *  Copyright (c) 2013-2015, ARM Ltd. All rights reserved.
 *  Copyright (c) 2008-2009, Apple Inc. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <PiDxe.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/EfiResetSystemLib.h>
#include <Library/ArmSmcLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeLib.h>

#include <IndustryStandard/ArmStdSmc.h>

/**
  Resets the entire platform.

  @param  ResetType             The type of reset to perform.
  @param  ResetStatus           The status code for the reset.
  @param  DataSize              The size, in bytes, of WatchdogData.
  @param  ResetData             For a ResetType of EfiResetCold, EfiResetWarm, or
                                EfiResetShutdown the data buffer starts with a Null-terminated
                                Unicode string, optionally followed by additional binary data.

**/
EFI_STATUS
EFIAPI
LibResetSystem (
  IN EFI_RESET_TYPE   ResetType,
  IN EFI_STATUS       ResetStatus,
  IN UINTN            DataSize,
  IN CHAR16           *ResetData OPTIONAL
  )
{
  ARM_SMC_ARGS ArmSmcArgs;
  UINT32 Delay;

  if (!EfiAtRuntime ()) {
    /*
     * Only if still in UEFI.
     */
    EfiEventGroupSignal (&gRaspberryPiEventResetGuid);

    Delay = PcdGet32 (PcdPlatformResetDelay);
    if (Delay != 0) {
      DEBUG ((DEBUG_INFO, "Platform will be reset in %d.%d seconds...\n",
              Delay / 1000000, (Delay % 1000000) / 100000));
      MicroSecondDelay (Delay);
    }
  }
  DEBUG ((DEBUG_INFO, "Platform %a.\n",
          (ResetType == EfiResetShutdown) ? "shutdown" : "reset"));

  switch (ResetType) {
  case EfiResetPlatformSpecific:
    // Map the platform specific reset as reboot
  case EfiResetWarm:
    // Map a warm reset into a cold reset
  case EfiResetCold:
    // Send a PSCI 0.2 SYSTEM_RESET command
    ArmSmcArgs.Arg0 = ARM_SMC_ID_PSCI_SYSTEM_RESET;
    break;
  case EfiResetShutdown:
    // Send a PSCI 0.2 SYSTEM_OFF command
    ArmSmcArgs.Arg0 = ARM_SMC_ID_PSCI_SYSTEM_OFF;
    break;
  default:
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  ArmCallSmc (&ArmSmcArgs);

  // We should never be here
  DEBUG ((DEBUG_ERROR, "%a: PSCI Reset failed\n", __FUNCTION__));
  CpuDeadLoop ();
  return EFI_UNSUPPORTED;
}

/**
  Initialize any infrastructure required for LibResetSystem () to function.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
LibInitializeResetSystem (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return EFI_SUCCESS;
}
