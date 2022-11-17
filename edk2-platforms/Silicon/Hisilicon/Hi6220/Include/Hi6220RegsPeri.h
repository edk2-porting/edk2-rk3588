/** @file
*
*  Copyright (c) 2018, Linaro Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __HI6220_REGS_PERI_H__
#define __HI6220_REGS_PERI_H__

#define SC_PERIPH_CLKEN3                0x230
#define SC_PERIPH_RSTEN3                0x330
#define SC_PERIPH_RSTDIS0               0x304
#define SC_PERIPH_RSTDIS3               0x334
#define SC_PERIPH_RSTSTAT3              0x338

/* SC_PERIPH_RSTEN0/RSTDIS0/RSTSTAT0 */
#define PERIPH_RST0_MMC2                (1 << 2)

/* SC_PERIPH_RSTEN3/RSTDIS3/RSTSTAT3 */
#define PERIPH_RST3_CSSYS               (1 << 0)
#define PERIPH_RST3_I2C0                (1 << 1)
#define PERIPH_RST3_I2C1                (1 << 2)
#define PERIPH_RST3_I2C2                (1 << 3)
#define PERIPH_RST3_I2C3                (1 << 4)
#define PERIPH_RST3_UART1               (1 << 5)
#define PERIPH_RST3_UART2               (1 << 6)
#define PERIPH_RST3_UART3               (1 << 7)
#define PERIPH_RST3_UART4               (1 << 8)
#define PERIPH_RST3_SSP                 (1 << 9)
#define PERIPH_RST3_PWM                 (1 << 10)
#define PERIPH_RST3_BLPWM               (1 << 11)
#define PERIPH_RST3_TSENSOR             (1 << 12)
#define PERIPH_RST3_DAPB                (1 << 18)
#define PERIPH_RST3_HKADC               (1 << 19)
#define PERIPH_RST3_CODEC_SSI           (1 << 20)
#define PERIPH_RST3_PMUSSI1             (1 << 22)

#define PMUSSI_REG(x)                   (PMUSSI_BASE + ((x) << 2))
#define PMUSSI_ONOFF8_REG               (PMUSSI_BASE + (0x1c << 2))
#define PMUSSI_ONOFF8_EN_32KB           BIT6

#endif /* __HI6220_REGS_PERI_H__ */
