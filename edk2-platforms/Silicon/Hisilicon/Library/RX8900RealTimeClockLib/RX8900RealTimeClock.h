/** @file

  Copyright (c) 2020, Hisilicon Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef RX8900_REAL_TIME_CLOCK_H__
#define RX8900_REAL_TIME_CLOCK_H__

#define RX8900_REGADDR_SECONDS         0x00
#define RX8900_REGADDR_MIUTES          0x01
#define RX8900_REGADDR_HOURS           0x02
#define RX8900_REGADDR_DAY             0x03
#define RX8900_REGADDR_DATE            0x04
#define RX8900_REGADDR_MONTH           0x05
#define RX8900_REGADDR_YEAR            0x06
#define RX8900_REGADDR_RAM             0x07
#define RX8900_REGADDR_ALARMMIN        0x08
#define RX8900_REGADDR_ALARMHOUR       0x09
#define RX8900_REGADDR_ALARMDAY        0x0A
#define RX8900_REGADDR_ALARMWEEK       0x0A
#define RX8900_REGADDR_TIMECOUNTER_0   0x0B
#define RX8900_REGADDR_TIMECOUNTER_1   0x0C
#define RX8900_REGADDR_EXTENSIONREG    0x0D
#define RX8900_REGADDR_FLAGREG         0x0E
#define RX8900_REGADDR_CONTRLREG       0xF
#define RX8900_REGADDR_BACKUP_FUN      0x18

#define RX8900_VDETOFF_SWOFF           0x0C
#define TEMPERATURE_COMPENSATION_2S    0x40
#define OUTPUT_FREQUENCY_32768         0x0C
#define FLAG_REG_DEFAULT               0x00
#define RX8900_RAM_REG_DEFAULT         0x5A

#define EFI_TIMEOFFSET_TIMEZONE        0x5A0

#endif
