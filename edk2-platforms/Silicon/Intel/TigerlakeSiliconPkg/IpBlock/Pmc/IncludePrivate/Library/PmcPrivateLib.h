/** @file
  Header file for private PmcLib.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PMC_PRIVATE_LIB_H_
#define _PMC_PRIVATE_LIB_H_

#include <Library/PmcLib.h>
#include "Register/PmcRegs.h"

/**
  This function checks if GbE device is supported (not disabled by fuse)

  @retval GbE support state
**/
BOOLEAN
PmcIsGbeSupported (
  VOID
  );

/**
  This function checks if LAN wake from DeepSx is enabled

  @retval Lan Wake state
**/
BOOLEAN
PmcIsLanDeepSxWakeEnabled (
  VOID
  );

/**
  This function sets SMI Lock with S3 Boot Script programming
**/
VOID
PmcLockSmiWithS3BootScript (
  VOID
  );

/**
  This function sets eSPI SMI Lock
  @attention This function must be called after eSPI SMI generation has been enabled.
    This setting is required in all boot modes and before EndOfDxe.
    If set value will be restored upon S3 resume by bootscript.
**/
VOID
PmcLockEspiSmiWithS3BootScript (
  VOID
  );

/**
  This function checks if eSPI SMI Lock is set

  @retval eSPI SMI Lock state
**/
BOOLEAN
PmcIsEspiSmiLockSet (
  VOID
  );

typedef enum {
  PmcSwSmiRate1p5ms = 0,
  PmcSwSmiRate16ms,
  PmcSwSmiRate32ms,
  PmcSwSmiRate64ms
} PMC_SWSMI_RATE;

/**
  This function sets SW SMI Rate.

  @param[in] SwSmiRate        Refer to PMC_SWSMI_RATE for possible values
**/
VOID
PmcSetSwSmiRate (
  IN PMC_SWSMI_RATE          SwSmiRate
  );

typedef enum {
  PmcPeriodicSmiRate8s = 0,
  PmcPeriodicSmiRate16s,
  PmcPeriodicSmiRate32s,
  PmcPeriodicSmiRate64s
} PMC_PERIODIC_SMI_RATE;

/**
  This function sets Periodic SMI Rate.

  @param[in] PeriodicSmiRate        Refer to PMC_PERIODIC_SMI_RATE for possible values
**/
VOID
PmcSetPeriodicSmiRate (
  IN PMC_PERIODIC_SMI_RATE    PeriodicSmiRate
  );

/**
  This function reads Power Button Level

  @retval State of PWRBTN# signal (0: Low, 1: High)
**/
UINT8
PmcGetPwrBtnLevel (
  VOID
  );

/**
  This function gets Group to GPE0 configuration

  @param[out] GpeDw0Value       GPIO Group to GPE_DW0 assignment
  @param[out] GpeDw1Value       GPIO Group to GPE_DW1 assignment
  @param[out] GpeDw2Value       GPIO Group to GPE_DW2 assignment
**/
VOID
PmcGetGpioGpe (
  OUT UINT32    *GpeDw0Value,
  OUT UINT32    *GpeDw1Value,
  OUT UINT32    *GpeDw2Value
  );

#endif // _PMC_PRIVATE_LIB_H_
