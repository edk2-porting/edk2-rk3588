/** @file
  Internal header file for PMC Private library

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PMC_PRIVATE_LIB_INTERNAL_H_
#define _PMC_PRIVATE_LIB_INTERNAL_H_

/**
  Check if MODPHY SUS PG is supported

  @retval  Status of MODPHY SUS PG support
**/
BOOLEAN
PmcIsModPhySusPgSupported (
  VOID
  );

/**
  This function is part of PMC init and configures which clock wake signals should
  set the SLOW_RING, SA, FAST_RING_CF and SLOW_RING_CF indication sent up to the CPU/PCH
**/
VOID
PmcInitClockWakeEnable (
  VOID
  );

/**
  This function configures PWRMBASE + 0x1E00 register
**/
VOID
PmcConfigureRegPwrm1E00 (
  VOID
  );

/**
  This function configures Misc PM_SYNC events settings
**/
VOID
PmcConfigurePmSyncEventsSettings (
  VOID
  );

#endif
