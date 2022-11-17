/** @file

  Copyright (c) 2018, Hisilicon Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef RTC_HELPER_LIB_H__
#define RTC_HELPER_LIB_H__

// The delay is need for cpld and I2C. This is a empirical value. MemoryFence is no need.
#define RTC_DELAY_30_MS            30000
// The delay is need for cpld and I2C. This is a empirical value. MemoryFence is no need.
#define RTC_DELAY_1000_MICROSECOND 1000
// The delay is need for cpld and I2C. This is a empirical value. MemoryFence is no need.
#define RTC_DELAY_2_MICROSECOND    2

EFI_STATUS
SwitchRtcI2cChannelAndLock (
  VOID
  );

VOID
ReleaseOwnershipOfRtc (
  VOID
  );

#endif
