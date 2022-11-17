/** @file
*
*  Copyright (c) 2014-2017, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __HI6220_H__
#define __HI6220_H__

/***********************************************************************************
// Platform Memory Map
************************************************************************************/

// SOC peripherals (UART, I2C, I2S, USB, etc)
#define HI6220_PERIPH_BASE                      0xF4000000
#define HI6220_PERIPH_SZ                        0x05800000

#define IOMG_BASE                               0xF7010000
#define IOMG_080_REG                            (IOMG_BASE + 0x140)

#define IOCG_BASE                               0xF7010800
#define IOCG_084_REG                            (IOCG_BASE + 0x150)

#define PERI_CTRL_BASE                          0xF7030000
#define SC_PERIPH_CTRL4                         0x00C
#define CTRL4_FPGA_EXT_PHY_SEL                  BIT3
#define CTRL4_PICO_SIDDQ                        BIT6
#define CTRL4_PICO_OGDISABLE                    BIT8
#define CTRL4_PICO_VBUSVLDEXT                   BIT10
#define CTRL4_PICO_VBUSVLDEXTSEL                BIT11
#define CTRL4_OTG_PHY_SEL                       BIT21

#define SC_PERIPH_CTRL5                         0x010

#define CTRL5_USBOTG_RES_SEL                    BIT3
#define CTRL5_PICOPHY_ACAENB                    BIT4
#define CTRL5_PICOPHY_BC_MODE                   BIT5
#define CTRL5_PICOPHY_CHRGSEL                   BIT6
#define CTRL5_PICOPHY_VDATSRCEND                BIT7
#define CTRL5_PICOPHY_VDATDETENB                BIT8
#define CTRL5_PICOPHY_DCDENB                    BIT9
#define CTRL5_PICOPHY_IDDIG                     BIT10

#define SC_PERIPH_CTRL8                         0x018
#define SC_PERIPH_CLKEN0                        0x200
#define SC_PERIPH_CLKDIS0                       0x204
#define SC_PERIPH_CLKSTAT0                      0x208

#define SC_PERIPH_RSTEN0                        0x300
#define SC_PERIPH_RSTDIS0                       0x304
#define SC_PERIPH_RSTSTAT0                      0x308

#define RST0_USBOTG_BUS                         BIT4
#define RST0_POR_PICOPHY                        BIT5
#define RST0_USBOTG                             BIT6
#define RST0_USBOTG_32K                         BIT7

#define EYE_PATTERN_PARA                        0x7053348c

#define MDDRC_AXI_BASE                          0xF7120000
#define AXI_REGION_MAP                          0x100
#define HIKEY_REGION_SIZE_MASK                  (7 << 8)
// (0 << 8) means 16MB, (7 << 8) means 2GB
#define HIKEY_REGION_SIZE(x)                    (1U << ((((x) & HIKEY_REGION_SIZE_MASK) >> 8) + 24))

#define AO_CTRL_BASE                            0xF7800000
#define SC_PW_MTCMOS_EN0                        0x830
#define SC_PW_MTCMOS_DIS0                       0x834
#define SC_PW_MTCMOS_STAT0                      0x838
#define SC_PW_MTCMOS_ACK_STAT0                  0x83c
#define PW_EN0_G3D                              (1 << 1)

#define PMUSSI_BASE                             0xF8000000

#endif /* __HI6220_H__ */
