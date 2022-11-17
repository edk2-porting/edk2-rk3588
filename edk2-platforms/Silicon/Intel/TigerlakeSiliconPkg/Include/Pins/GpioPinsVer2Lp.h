/** @file
  GPIO pins for TGL-PCH-LP,

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _GPIO_PINS_VER2_LP_H_
#define _GPIO_PINS_VER2_LP_H_
///
/// This header file should be used together with
/// PCH GPIO lib in C and ASL. All defines used
/// must match both ASL/C syntax
///

///
/// Unique ID used in GpioPad defines
///
#define GPIO_VER2_LP_CHIPSET_ID     0x9

///
/// TGL LP GPIO Groups
/// Use below for functions from PCH GPIO Lib which
/// require GpioGroup as argument
///
#define GPIO_VER2_LP_GROUP_GPP_B    0x0900
#define GPIO_VER2_LP_GROUP_GPP_A    0x0902
#define GPIO_VER2_LP_GROUP_GPP_R    0x0903
#define GPIO_VER2_LP_GROUP_GPD      0x0905
#define GPIO_VER2_LP_GROUP_GPP_S    0x0906
#define GPIO_VER2_LP_GROUP_GPP_H    0x0907
#define GPIO_VER2_LP_GROUP_GPP_D    0x0908
#define GPIO_VER2_LP_GROUP_GPP_C    0x090B
#define GPIO_VER2_LP_GROUP_GPP_F    0x090C
#define GPIO_VER2_LP_GROUP_GPP_E    0x090E


///
/// TGL LP GPIO pins
/// Use below for functions from PCH GPIO Lib which
/// require GpioPad as argument. Encoding used here
/// has all information required by library functions
///
#define GPIO_VER2_LP_GPP_B2                  0x09000002
#define GPIO_VER2_LP_GPP_B4                  0x09000004
#define GPIO_VER2_LP_GPP_B14                 0x0900000E
#define GPIO_VER2_LP_GPP_B15                 0x0900000F
#define GPIO_VER2_LP_GPP_B16                 0x09000010
#define GPIO_VER2_LP_GPP_B18                 0x09000012
#define GPIO_VER2_LP_GPP_B23                 0x09000017
#define GPIO_VER2_LP_GSPI0_CLK_LOOPBK        0x09000018

#define GPIO_VER2_LP_GPP_A10                 0x0902000A
#define GPIO_VER2_LP_GPP_A11                 0x0902000B
#define GPIO_VER2_LP_GPP_A13                 0x0902000D
#define GPIO_VER2_LP_GPP_A14                 0x0902000E
#define GPIO_VER2_LP_GPP_A23                 0x09020017
#define GPIO_VER2_LP_ESPI_CLK_LOOPBK         0x09020018

#define GPIO_VER2_LP_GPP_R5                  0x09030005
#define GPIO_VER2_LP_GPP_R6                  0x09030006

#define GPIO_VER2_LP_GPD7                    0x09050007

#define GPIO_VER2_LP_INPUT3VSEL              0x0905000C

#define GPIO_VER2_LP_GPP_H0                  0x09070000
#define GPIO_VER2_LP_GPP_H1                  0x09070001
#define GPIO_VER2_LP_GPP_H12                 0x0907000C
#define GPIO_VER2_LP_GPP_H13                 0x0907000D
#define GPIO_VER2_LP_GPP_H15                 0x0907000F
#define GPIO_VER2_LP_GPP_H19                 0x09070013

#define GPIO_VER2_LP_GPP_D16                 0x09080010
#define GPIO_VER2_LP_GSPI2_CLK_LOOPBK        0x09080014

#define GPIO_VER2_LP_GPP_C2                  0x090B0002
#define GPIO_VER2_LP_GPP_C5                  0x090B0005
#define GPIO_VER2_LP_GPP_C8                  0x090B0008
#define GPIO_VER2_LP_GPP_C12                 0x090B000C
#define GPIO_VER2_LP_GPP_C13                 0x090B000D
#define GPIO_VER2_LP_GPP_C14                 0x090B000E
#define GPIO_VER2_LP_GPP_C15                 0x090B000F
#define GPIO_VER2_LP_GPP_C22                 0x090B0016
#define GPIO_VER2_LP_GPP_C23                 0x090B0017

#define GPIO_VER2_LP_GPP_F4                  0x090C0004
#define GPIO_VER2_LP_GPP_F5                  0x090C0005
#define GPIO_VER2_LP_GPP_F9                  0x090C0009
#define GPIO_VER2_LP_GPP_F10                 0x090C000A
#define GPIO_VER2_LP_GPP_F20                 0x090C0014
#define GPIO_VER2_LP_GPP_F21                 0x090C0015
#define GPIO_VER2_LP_GPPF_CLK_LOOPBK         0x090C0018

#define GPIO_VER2_LP_GPP_E3                  0x090E0003
#define GPIO_VER2_LP_GPP_E7                  0x090E0007
#define GPIO_VER2_LP_GPP_E8                  0x090E0008
#define GPIO_VER2_LP_GPP_E22                 0x090E0016
#define GPIO_VER2_LP_GPP_E23                 0x090E0017
#define GPIO_VER2_LP_GPPE_CLK_LOOPBK         0x090E0018

//
// GPIO Pin Muxing
// Determines a selection of physical pad for a given signal.
// Please refer to GPIO_NATIVE_PAD type.
// If certain signal is not listed below it means that it can be enabled
// only on a single pad and muxing setting is not needed.
//
#define GPIO_VER2_LP_MUXING_SERIALIO_I2C4_SDA_GPP_H8         0x1947CC08
#define GPIO_VER2_LP_MUXING_SERIALIO_I2C4_SCL_GPP_H9         0x1947AC09
#endif // _GPIO_PINS_VER2_LP_H_
