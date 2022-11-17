/** Ds1307Rtc.h
*
*  Copyright 2017 NXP
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef DS1307RTC_H_
#define DS1307RTC_H_

/*
 * RTC time register
 */
#define DS1307_SEC_REG_ADDR        0x00
#define DS1307_MIN_REG_ADDR        0x01
#define DS1307_HR_REG_ADDR         0x02
#define DS1307_DAY_REG_ADDR        0x03
#define DS1307_DATE_REG_ADDR       0x04
#define DS1307_MON_REG_ADDR        0x05
#define DS1307_YR_REG_ADDR         0x06

#define DS1307_SEC_BIT_CH          0x80  /* Clock Halt (in Register 0)   */

/*
 * RTC control register
 */
#define DS1307_CTL_REG_ADDR        0x07

#define START_YEAR                 1970
#define END_YEAR                   2070

/*
 * TIME MASKS
 */
#define MASK_SEC                   0x7F
#define MASK_MIN                   0x7F
#define MASK_HOUR                  0x3F
#define MASK_DAY                   0x3F
#define MASK_MONTH                 0x1F

typedef struct {
  UINTN                           OperationCount;
  EFI_I2C_OPERATION               SetAddressOp;
  EFI_I2C_OPERATION               GetSetDateTimeOp;
} RTC_I2C_REQUEST;

#endif // DS1307RTC_H_
