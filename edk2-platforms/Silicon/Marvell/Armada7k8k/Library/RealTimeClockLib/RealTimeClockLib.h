/********************************************************************************
Copyright (C) 2017 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#ifndef __RTCLIB_H__
#define __RTCLIB_H__

/* Armada 70x0 SoC registers */
#define RTC_STATUS_REG                          0x0
#define RTC_TIME_REG                            0xC
#define RTC_IRQ_2_CONFIG_REG                    0x8
#define RTC_IRQ_ALARM_EN                        0x1
#define RTC_ALARM_2_REG                         0x14
#define RTC_BRIDGE_TIMING_CTRL0_REG_OFFS        0x80
#define RTC_BRIDGE_TIMING_CTRL1_REG_OFFS        0x84
#define RTC_IRQ_STATUS_REG                      0x90
#define RTC_IRQ_ALARM_MASK                      0x2
#define RTC_WRITE_PERIOD_DELAY_MASK             0xFFFF
#define RTC_WRITE_PERIOD_DELAY_DEFAULT          0x3FF
#define RTC_WRITE_SETUP_DELAY_MASK              (0xFFFF << 16)
#define RTC_WRITE_SETUP_DELAY_DEFAULT           (0x29 << 16)
#define RTC_READ_OUTPUT_DELAY_MASK              0xFFFF
#define RTC_READ_OUTPUT_DELAY_DEFAULT           0x3F

#endif /* __RTCLIB_H__ */
