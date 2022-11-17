/** @file

  Copyright (c) 2018, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2018, Linaro Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __M41T83_REAL_TIME_CLOCK_H__
#define __M41T83_REAL_TIME_CLOCK_H__

#define M41T83_REGADDR_DOTSECONDS       0x00
#define M41T83_REGADDR_SECONDS          0x01
#define M41T83_REGADDR_MINUTES          0x02
#define M41T83_REGADDR_HOURS            0x03
#define M41T83_REGADDR_WEEK_DAY         0x04
#define M41T83_REGADDR_DAY              0x05
#define M41T83_REGADDR_MONTH            0x06
#define M41T83_REGADDR_YEAR             0x07
#define M41T83_REGADDR_ALARM1SEC        0x0E
#define M41T83_REGADDR_ALARM1MIN        0x0D
#define M41T83_REGADDR_ALARM1HOUR       0x0C
#define M41T83_REGADDR_ALARM1DATE       0x0B
#define M41T83_REGADDR_ALARM1MONTH      0x0A

#define M41T83_REGADDR_TIMERCONTROL     0x11

#define M41T83_REGADDR_ALARM2SEC        0x18
#define M41T83_REGADDR_ALARM2MIN        0x17
#define M41T83_REGADDR_ALARM2HOUR       0x16
#define M41T83_REGADDR_ALARM2DATE       0x15
#define M41T83_REGADDR_ALARM2MONTH      0x14

typedef union {
  struct {
    UINT8 TD0:1;
    UINT8 TD1:1;
    UINT8 RSV:3;
    UINT8 TIE:1;
    UINT8 TITP:1;
    UINT8 TE:1;
  } Bits;
  UINT8 Uint8;
} RTC_M41T83_TIMERCONTROL;

typedef union {
  struct {
    UINT8 MicroSeconds;
  } Bits;
  UINT8 Uint8;
} RTC_M41T83_DOTSECOND;

typedef union {
  struct{
    UINT8 Seconds:7;
    UINT8 ST:1;
  } Bits;
  UINT8 Uint8;
} RTC_M41T83_SECOND;

typedef union {
  struct {
    UINT8 Minutes:7;
    UINT8 Rsv:1;
  } Bits;
  UINT8 Uint8;
} RTC_M41T83_MINUTE;

typedef union {
  struct {
    UINT8 Hours:6;
    UINT8 CB:2;
  } Bits;
  UINT8 Uint8;
} RTC_M41T83_HOUR;

typedef union {
  struct{
    UINT8 Days:3;
    UINT8 Rsv:5;
  } Bits;
  UINT8 Uint8;
} RTC_M41T83_WEEK_DAY;

typedef union {
  struct{
    UINT8 Days:6;
    UINT8 Rsv:2;
  } Bits;
  UINT8 Uint8;
} RTC_M41T83_MONTH_DAY;

typedef union {
  struct {
    UINT8 Months:5;
    UINT8 Rsv:3;
  } Bits;
  UINT8 Uint8;
} RTC_M41T83_MONTH;

typedef union {
  struct {
    UINT8 Years:8;
  } Bits;
  UINT8 Uint8;
} RTC_M41T83_YEAR;

typedef union {
  struct {
    UINT8 Second:7;
    UINT8 RPT11:1;
  } Bits;
  UINT8 Uint8;
} RTC_M41T83_ALARM1SEC;

typedef union {
  struct {
    UINT8 Minute:7;
    UINT8 RPT12:1;
  } Bits;
  UINT8 Uint8;
} RTC_M41T83_ALARM1MIN;

typedef union {
  struct {
    UINT8 Hour:6;
    UINT8 HT:1;
    UINT8 RPT13:1;
  } Bits;
  UINT8 Uint8;
} RTC_M41T83_ALARM1HOUR;

typedef struct {
  RTC_M41T83_DOTSECOND  DotSecond;
  RTC_M41T83_SECOND     Second;
  RTC_M41T83_MINUTE     Minute;
  RTC_M41T83_HOUR       Hour;
  RTC_M41T83_WEEK_DAY   WeekDay;
  RTC_M41T83_MONTH_DAY  Day;
  RTC_M41T83_MONTH      Month;
  RTC_M41T83_YEAR       Year;
} RTC_M41T83_TIME;

#endif
