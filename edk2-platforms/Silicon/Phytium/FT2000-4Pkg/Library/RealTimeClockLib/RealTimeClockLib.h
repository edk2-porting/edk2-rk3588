/** @file
  Phytium RealTime Clock Header.

  Copyright (C) 2020, Phytium Technology Co Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef REAL_TIME_CLOCK_H_
#define REAL_TIME_CLOCK_H_

#define RTC_CMR       0x4
#define RTC_AES_SEL   0x8
#define RTC_CCR       0xC
#define RTC_STAT      0x10
#define RTC_RSTAT     0x14
#define RTC_EOI       0x18
#define RTC_CDR_LOW   0x20
#define RTC_CCVR      0x24
#define RTC_CLR_LOW   0x28
#define RTC_CLR       0x2C

#endif // REAL_TIME_CLOCK_H_
