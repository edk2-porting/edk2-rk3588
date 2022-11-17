/** @file
  UP Xtreme Platform Hook library.

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_PLATFORM_HOOK_LIB_H_
#define _PEI_PLATFORM_HOOK_LIB_H_

#include <PlatformInfo.h>
#include <Library/PeiServicesLib.h>
#include <Library/GpioLib.h>

// EC Command to provide one byte of debug indication
#define BSSB_DEBUG_INDICATION 0xAE
/**
  Configure EC for specific devices

  @param[in] PchLan       - The PchLan of PCH_SETUP variable.
  @param[in] BootMode     - The current boot mode.
**/
VOID
EcInit (
  IN UINT8                PchLan,
  IN EFI_BOOT_MODE        BootMode
  );

/**
  Checks if Premium PMIC present

  @retval  TRUE  if present
  @retval  FALSE it discrete/other PMIC
**/
BOOLEAN
IsPremiumPmicPresent (
  VOID
  );

/**
  Pmic Programming to supprort LPAL Feature

  @retval     NONE
**/
VOID
PremiumPmicDisableSlpS0Voltage (
  VOID
  );

/**
Pmic Programming to supprort LPAL Feature
  @retval     NONE
**/
VOID
PremiumPmicEnableSlpS0Voltage(
  VOID
  );

/**
  Do platform specific programming pre-memory. For example, EC init, Chipset programming

  @retval  Status
**/
EFI_STATUS
PlatformSpecificInitPreMem (
  VOID
  );

/**
  Do platform specific programming post-memory.

  @retval  Status
**/
EFI_STATUS
PlatformSpecificInit (
  VOID
  );

/**
  Configure GPIO and SIO Before Memory is ready.

  @retval  EFI_SUCCESS   Operation success.
**/
EFI_STATUS
BoardInitPreMem (
  VOID
  );

/**
  Configure GPIO and SIO

  @retval  EFI_SUCCESS   Operation success.
**/
EFI_STATUS
BoardInit (
  VOID
  );

/**
Voltage Margining Routine

@retval  EFI_SUCCESS   Operation success
**/
EFI_STATUS
VoltageMarginingRoutine(
  VOID
  );

/**
  Detect recovery mode

  @retval  EFI_SUCCESS       System in Recovery Mode
  @retval  EFI_UNSUPPORTED   System doesn't support Recovery Mode
  @retval  EFI_NOT_FOUND     System is not in Recovery Mode
**/
EFI_STATUS
IsRecoveryMode (
  VOID
  );

/**
  Early board Configuration before Memory is ready.

  @retval  EFI_SUCCESS  Operation success.
**/
EFI_STATUS
BoardInitEarlyPreMem (
  VOID
  );

#endif
