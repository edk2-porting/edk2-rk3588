/** @file
  Support ResetSystem Runtime call using PSCI calls

  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2013, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2014, Linaro Ltd. All rights reserved.<BR>
  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  Copyright (c) 2024, Google Llc. All rights reserved.<BR>
  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>

#include <IndustryStandard/ArmStdSmc.h>

#include <Library/ArmMonitorLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/ResetUtilityLib.h>

#define ROCKCHIP_BOOT_MODE_REG  0xFD588080 // PMU0_GRF_OS_REG8

#define BOOT_BROM_DOWNLOAD  0xEF08A53C

#define PMU1GRF_SOC_CON1  0xfd58a004

/**
  Library constructor. This function does nothing, but this library may depend
  on other libraries that do have a non-trivial constructor, which the
  BaseToools fail to account for if a library has no constructor at all.
 **/
RETURN_STATUS
EFIAPI
ResetSystemLibConstructor (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This function causes a system-wide reset (cold reset), in which
  all circuitry within the system returns to its initial state. This type of reset
  is asynchronous to system operation and operates without regard to
  cycle boundaries.

  If this function returns, it means that the system does not support cold reset.
**/
VOID
EFIAPI
ResetCold (
  VOID
  )
{
  ARM_MONITOR_ARGS  Args;

  // Send a PSCI 0.2 SYSTEM_RESET command
  Args.Arg0 = ARM_SMC_ID_PSCI_SYSTEM_RESET;

  ArmMonitorCall (&Args);
}

/**
  This function causes a system-wide initialization (warm reset), in which all processors
  are set to their initial state. Pending cycles are not corrupted.

  If this function returns, it means that the system does not support warm reset.
**/
VOID
EFIAPI
ResetWarm (
  VOID
  )
{
  ARM_MONITOR_ARGS  Args;

  Args.Arg0 = ARM_SMC_ID_PSCI_SYSTEM_RESET2_AARCH64;

  // Is SYSTEM_RESET2 supported?
  ArmMonitorCall (&Args);
  if (Args.Arg0 == ARM_SMC_PSCI_RET_SUCCESS) {
    // Send PSCI SYSTEM_RESET2 command
    Args.Arg0 = ARM_SMC_ID_PSCI_SYSTEM_RESET2_AARCH64;

    ArmMonitorCall (&Args);
  } else {
    // Map a warm reset into a cold reset
    DEBUG ((
      DEBUG_INFO,
      "Warm reboot not supported by platform, issuing cold reboot\n"
      ));
    ResetCold ();
  }
}

/**
  This function causes the system to enter a power state equivalent
  to the ACPI G2/S5 or G3 states.

  If this function returns, it means that the system does not support shutdown reset.
**/
VOID
EFIAPI
ResetShutdown (
  VOID
  )
{
  ARM_MONITOR_ARGS  Args;

  // Send a PSCI 0.2 SYSTEM_RESET command
  Args.Arg0 = ARM_SMC_ID_PSCI_SYSTEM_OFF;

  ArmMonitorCall (&Args);
}

/**
  This function causes a systemwide reset. The exact type of the reset is
  defined by the EFI_GUID that follows the Null-terminated Unicode string passed
  into ResetData. If the platform does not recognize the EFI_GUID in ResetData
  the platform must pick a supported reset type to perform.The platform may
  optionally log the parameters from any non-normal reset that occurs.

  @param[in]  DataSize   The size, in bytes, of ResetData.
  @param[in]  ResetData  The data buffer starts with a Null-terminated string,
                         followed by the EFI_GUID.
**/
VOID
EFIAPI
ResetPlatformSpecific (
  IN UINTN  DataSize,
  IN VOID   *ResetData
  )
{
  GUID  *ResetSubtype;

  if ((DataSize == 0) || (ResetData == NULL)) {
    goto Exit;
  }

  ResetSubtype = GetResetPlatformSpecificGuid (DataSize, ResetData);

  if (CompareGuid (ResetSubtype, &gRockchipResetTypeMaskromGuid)) {
    MmioWrite32 (ROCKCHIP_BOOT_MODE_REG, BOOT_BROM_DOWNLOAD);
  } else {
    goto Exit;
  }

  //
  // It has been observed that the boot mode register does not persist
  // across reset after BL31 sets reset_width to 0xFFFF.
  // Setting it back to 0 causes a softer reset, which solves the issue.
  //
  MmioWrite32 (PMU1GRF_SOC_CON1, 0xFFFF0000);

Exit:
  ResetCold ();
}

/**
  The ResetSystem function resets the entire platform.

  @param[in] ResetType      The type of reset to perform.
  @param[in] ResetStatus    The status code for the reset.
  @param[in] DataSize       The size, in bytes, of ResetData.
  @param[in] ResetData      For a ResetType of EfiResetCold, EfiResetWarm, or EfiResetShutdown
                            the data buffer starts with a Null-terminated string, optionally
                            followed by additional binary data. The string is a description
                            that the caller may use to further indicate the reason for the
                            system reset.
**/
VOID
EFIAPI
ResetSystem (
  IN EFI_RESET_TYPE  ResetType,
  IN EFI_STATUS      ResetStatus,
  IN UINTN           DataSize,
  IN VOID            *ResetData OPTIONAL
  )
{
  switch (ResetType) {
    case EfiResetWarm:
      ResetWarm ();
      break;

    case EfiResetCold:
      ResetCold ();
      break;

    case EfiResetShutdown:
      ResetShutdown ();
      return;

    case EfiResetPlatformSpecific:
      ResetPlatformSpecific (DataSize, ResetData);
      return;

    default:
      return;
  }
}
