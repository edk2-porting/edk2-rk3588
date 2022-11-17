/** @file
  PEI DTBT Init Dispatch library Header file


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __PEI_DTBT_INIT_LIB_H__
#define __PEI_DTBT_INIT_LIB_H__

#include <Private/Library/PeiTbtCommonInitLib.h>
#include <Library/PeiTbtTaskDispatchLib.h>

extern TBT_CALL_TABLE_ENTRY DTbtCallTable[];

/**
  Get Thunderbolt(TM) (TBT) PEI Policy Data.

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtGetPeiTbtPolicyData (
  IN  PEI_TBT_POLICY  *PeiTbtConfig
);

/**
  Toggle related GPIO pin for DTBT.

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtToggleGPIO (
  IN  PEI_TBT_POLICY  *PeiTbtConfig
);

/**
  set tPCH25 Timing to 10 ms for DTBT.

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtSetTPch25Timing (
  IN  PEI_TBT_POLICY  *PeiTbtConfig
);

/**
  Do ForcePower for DTBT Controller

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtForcePower (
  IN  PEI_TBT_POLICY  *PeiTbtConfig
);

/**
  Clear VGA Registers for DTBT.

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtClearVgaRegisters (
  IN  PEI_TBT_POLICY  *PeiTbtConfig
);

/**
  Exectue Mail box command "Boot On".

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtBootOn (
  IN  PEI_TBT_POLICY  *PeiTbtConfig
);

/**
  Exectue Mail box command "USB On".

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtUsbOn (
  IN  PEI_TBT_POLICY  *PeiTbtConfig
);

/**
  Exectue Mail box command "Sx Exit".

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtSxExitFlow (
  IN  PEI_TBT_POLICY  *PeiTbtConfig
);

#endif

