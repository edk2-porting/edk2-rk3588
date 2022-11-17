/** @file
  Header file for PchPmcLib.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PCH_PMC_LIB_H_
#define _PCH_PMC_LIB_H_

typedef enum {
  WarmBoot          = 1,
  ColdBoot,
  PwrFlr,
  PwrFlrSys,
  PwrFlrPch,
  PchPmStatusMax
} PCH_PM_STATUS;

/**
  Query PCH to determine the Pm Status

  @param[in] PmStatus - The Pch Pm Status to be probed

  @retval Status TRUE if Status querried is Valid or FALSE if otherwise
**/
BOOLEAN
GetPchPmStatus (
  PCH_PM_STATUS PmStatus
  );

/**
  Funtion to check if Battery lost or CMOS cleared.

  @reval TRUE  Battery is always present.
  @reval FALSE CMOS is cleared.
**/
BOOLEAN
EFIAPI
PchIsRtcBatteryGood (
  VOID
  );

/**
  Returns the sleep type after system wakeup.

  @param[out] SleepType  Sleep type to be returned.

  @retval     TRUE       A wake event occurred without power failure.
  @retval     FALSE      Power failure occurred or not a wakeup.

**/
BOOLEAN
EFIAPI
GetSleepTypeAfterWakeup (
  OUT UINT32            *SleepType
  );

#endif // _PCH_PMC_LIB_H_
