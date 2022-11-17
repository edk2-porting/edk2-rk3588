/** @file
*
*  Copyright (c) 2011, ARM Limited. All rights reserved.
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*  Based on the files under ArmPlatformPkg/Library/PL031RealTimeClockLib/PL031RealTimeClockLib.inf
**/


#ifndef __DS3231_REAL_TIME_CLOCK_H__
#define __DS3231_REAL_TIME_CLOCK_H__

#define DS3231_REGADDR_SECONDS      0x00
#define DS3231_REGADDR_MIUTES       0x01
#define DS3231_REGADDR_HOURS        0x02
#define DS3231_REGADDR_DAY          0x03
#define DS3231_REGADDR_DATE         0x04
#define DS3231_REGADDR_MONTH        0x05
#define DS3231_REGADDR_YEAR         0x06
#define DS3231_REGADDR_ALARM1SEC    0x07
#define DS3231_REGADDR_ALARM1MIN    0x08
#define DS3231_REGADDR_ALARM1HOUR   0x09
#define DS3231_REGADDR_ALARM1DAY    0x0A
#define DS3231_REGADDR_ALARM2MIN    0x0B
#define DS3231_REGADDR_ALARM2HOUR   0x0C
#define DS3231_REGADDR_ALARM2DAY    0x0D
#define DS3231_REGADDR_CONTROL      0x0E
#define DS3231_REGADDR_STATUS       0x0F
#define DS3231_REGADDR_AGOFFSET     0x10
#define DS3231_REGADDR_TEMPMSB      0x11
#define DS3231_REGADDR_TEMPLSB      0x12


typedef union {
  struct{
    UINT8 A1IE:1;
    UINT8 A2IE:1;
    UINT8 INTCN:1;
    UINT8 RSV:2;
    UINT8 CONV:1;
    UINT8 BBSQW:1;
    UINT8 EOSC_N:1;
  }bits;
  UINT8 u8;
}RTC_DS3231_CONTROL;

typedef union {
  struct{
    UINT8 A1F:1;
    UINT8 A2F:1;
    UINT8 BSY:1;
    UINT8 EN32KHZ:2;
    UINT8 Rsv:3;
    UINT8 OSF:1;
  }bits;
  UINT8 u8;
}RTC_DS3231_STATUS;


typedef union {
  struct{
    UINT8 Data:7;
    UINT8 Sign:1;
  }bits;
  UINT8 u8;
}RTC_DS3231_AGOFFSET;

typedef union {
  struct{
    UINT8 Data:7;
    UINT8 Sign:1;
  }bits;
  UINT8 u8;
}RTC_DS3231_TEMPMSB;


typedef union {
  struct{
    UINT8 Rsv:6;
    UINT8 Data:2;
  }bits;
  UINT8 u8;
}RTC_DS3231_TEMPLSB;

typedef union {
  struct{
    UINT8 Seconds:4;
    UINT8 Seconds10:3;
    UINT8 Rsv:1;
  }bits;
  UINT8 u8;
}RTC_DS3231_SECONDS;

typedef union {
  struct{
    UINT8 Minutes:4;
    UINT8 Minutes10:3;
    UINT8 Rsv:1;
  }bits;
  UINT8 u8;
}RTC_DS3231_MINUTES;

typedef union {
  struct{
    UINT8 Hour:4;
    UINT8 Hours10:1;
    UINT8 PM_20Hours:1;
    UINT8 Hour24_n:1;
    UINT8 Rsv:1;
  }bits;
  UINT8 u8;
}RTC_DS3231_HOURS;

typedef union {
  struct{
    UINT8 Day:3;
    UINT8 Rsv:5;
  }bits;
  UINT8 u8;
}RTC_DS3231_DAY;

typedef union {
  struct{
    UINT8 Month:4;
    UINT8 Month10:1;
    UINT8 Rsv:2;
    UINT8 Century:1;
  }bits;
  UINT8 u8;
}RTC_DS3231_MONTH;

typedef union {
  struct{
    UINT8 Year:4;
    UINT8 Year10:4;
  }bits;
  UINT8 u8;
}RTC_DS3231_YEAR;

typedef union {
  struct{
    UINT8 Seconds:4;
    UINT8 Seconds10:3;
    UINT8 A1M1:1;
  }bits;
  UINT8 u8;
}RTC_DS3231_ALARM1SEC;

typedef union {
  struct{
    UINT8 Minutes:4;
    UINT8 Minutes10:3;
    UINT8 A1M2:1;
  }bits;
  UINT8 u8;
}RTC_DS3231_ALARM1MIN;

typedef union {
  struct{
    UINT8 Hour:4;
    UINT8 Hours10:1;
    UINT8 PM_20Hours:1;
    UINT8 Hours24:1;
    UINT8 A1M3:1;
  }bits;
  UINT8 u8;
}RTC_DS3231_ALARM1HOUR;

#endif
