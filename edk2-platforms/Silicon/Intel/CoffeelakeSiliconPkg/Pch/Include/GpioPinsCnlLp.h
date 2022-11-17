/** @file
  GPIO pins,

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _GPIO_PINS_CNL_LP_H_
#define _GPIO_PINS_CNL_LP_H_
///
/// This header file should be used together with
/// PCH GPIO lib in C and ASL. All defines used
/// must match both ASL/C syntax
///

///
/// Unique ID used in GpioPad defines
///
#define GPIO_CNL_LP_CHIPSET_ID      0x4

///
/// Use below for functions from PCH GPIO Lib which
/// require GpioGroup as argument
///
#define GPIO_CNL_LP_GROUP_GPP_A  0x0400
#define GPIO_CNL_LP_GROUP_GPP_B  0x0401
#define GPIO_CNL_LP_GROUP_GPP_C  0x0402
#define GPIO_CNL_LP_GROUP_GPP_D  0x0403
#define GPIO_CNL_LP_GROUP_GPP_E  0x0404
#define GPIO_CNL_LP_GROUP_GPP_F  0x0405
#define GPIO_CNL_LP_GROUP_GPP_G  0x0406
#define GPIO_CNL_LP_GROUP_GPP_H  0x0407
#define GPIO_CNL_LP_GROUP_GPD    0x0408
#define GPIO_CNL_LP_GROUP_VGPIO  0x0409
#define GPIO_CNL_LP_GROUP_SPI    0x040A
#define GPIO_CNL_LP_GROUP_AZA    0x040B
#define GPIO_CNL_LP_GROUP_CPU    0x040C
#define GPIO_CNL_LP_GROUP_JTAG   0x040D
#define GPIO_CNL_LP_GROUP_HVMOS  0x040E

///
/// Use below for functions from PCH GPIO Lib which
/// require GpioPad as argument. Encoding used here
/// has all information required by library functions
///
#define GPIO_CNL_LP_GPP_A0               0x04000000
#define GPIO_CNL_LP_GPP_A1               0x04000001
#define GPIO_CNL_LP_GPP_A2               0x04000002
#define GPIO_CNL_LP_GPP_A3               0x04000003
#define GPIO_CNL_LP_GPP_A4               0x04000004
#define GPIO_CNL_LP_GPP_A5               0x04000005
#define GPIO_CNL_LP_GPP_A6               0x04000006
#define GPIO_CNL_LP_GPP_A7               0x04000007
#define GPIO_CNL_LP_GPP_A8               0x04000008
#define GPIO_CNL_LP_GPP_A9               0x04000009
#define GPIO_CNL_LP_GPP_A10              0x0400000A
#define GPIO_CNL_LP_GPP_A11              0x0400000B
#define GPIO_CNL_LP_GPP_A12              0x0400000C
#define GPIO_CNL_LP_GPP_A13              0x0400000D
#define GPIO_CNL_LP_GPP_A14              0x0400000E
#define GPIO_CNL_LP_GPP_A15              0x0400000F
#define GPIO_CNL_LP_GPP_A16              0x04000010
#define GPIO_CNL_LP_GPP_A17              0x04000011
#define GPIO_CNL_LP_GPP_A18              0x04000012
#define GPIO_CNL_LP_GPP_A19              0x04000013
#define GPIO_CNL_LP_GPP_A20              0x04000014
#define GPIO_CNL_LP_GPP_A21              0x04000015
#define GPIO_CNL_LP_GPP_A22              0x04000016
#define GPIO_CNL_LP_GPP_A23              0x04000017
#define GPIO_CNL_LP_ESPI_CLK_LOOPBK      0x04000018

#define GPIO_CNL_LP_GPP_B0               0x04010000
#define GPIO_CNL_LP_GPP_B1               0x04010001
#define GPIO_CNL_LP_GPP_B2               0x04010002
#define GPIO_CNL_LP_GPP_B3               0x04010003
#define GPIO_CNL_LP_GPP_B4               0x04010004
#define GPIO_CNL_LP_GPP_B5               0x04010005
#define GPIO_CNL_LP_GPP_B6               0x04010006
#define GPIO_CNL_LP_GPP_B7               0x04010007
#define GPIO_CNL_LP_GPP_B8               0x04010008
#define GPIO_CNL_LP_GPP_B9               0x04010009
#define GPIO_CNL_LP_GPP_B10              0x0401000A
#define GPIO_CNL_LP_GPP_B11              0x0401000B
#define GPIO_CNL_LP_GPP_B12              0x0401000C
#define GPIO_CNL_LP_GPP_B13              0x0401000D
#define GPIO_CNL_LP_GPP_B14              0x0401000E
#define GPIO_CNL_LP_GPP_B15              0x0401000F
#define GPIO_CNL_LP_GPP_B16              0x04010010
#define GPIO_CNL_LP_GPP_B17              0x04010011
#define GPIO_CNL_LP_GPP_B18              0x04010012
#define GPIO_CNL_LP_GPP_B19              0x04010013
#define GPIO_CNL_LP_GPP_B20              0x04010014
#define GPIO_CNL_LP_GPP_B21              0x04010015
#define GPIO_CNL_LP_GPP_B22              0x04010016
#define GPIO_CNL_LP_GPP_B23              0x04010017
#define GPIO_CNL_LP_GSPI0_CLK_LOOPBK     0x04010018
#define GPIO_CNL_LP_GSPI1_CLK_LOOPBK     0x04010019

#define GPIO_CNL_LP_GPP_C0               0x04020000
#define GPIO_CNL_LP_GPP_C1               0x04020001
#define GPIO_CNL_LP_GPP_C2               0x04020002
#define GPIO_CNL_LP_GPP_C3               0x04020003
#define GPIO_CNL_LP_GPP_C4               0x04020004
#define GPIO_CNL_LP_GPP_C5               0x04020005
#define GPIO_CNL_LP_GPP_C6               0x04020006
#define GPIO_CNL_LP_GPP_C7               0x04020007
#define GPIO_CNL_LP_GPP_C8               0x04020008
#define GPIO_CNL_LP_GPP_C9               0x04020009
#define GPIO_CNL_LP_GPP_C10              0x0402000A
#define GPIO_CNL_LP_GPP_C11              0x0402000B
#define GPIO_CNL_LP_GPP_C12              0x0402000C
#define GPIO_CNL_LP_GPP_C13              0x0402000D
#define GPIO_CNL_LP_GPP_C14              0x0402000E
#define GPIO_CNL_LP_GPP_C15              0x0402000F
#define GPIO_CNL_LP_GPP_C16              0x04020010
#define GPIO_CNL_LP_GPP_C17              0x04020011
#define GPIO_CNL_LP_GPP_C18              0x04020012
#define GPIO_CNL_LP_GPP_C19              0x04020013
#define GPIO_CNL_LP_GPP_C20              0x04020014
#define GPIO_CNL_LP_GPP_C21              0x04020015
#define GPIO_CNL_LP_GPP_C22              0x04020016
#define GPIO_CNL_LP_GPP_C23              0x04020017

#define GPIO_CNL_LP_GPP_D0               0x04030000
#define GPIO_CNL_LP_GPP_D1               0x04030001
#define GPIO_CNL_LP_GPP_D2               0x04030002
#define GPIO_CNL_LP_GPP_D3               0x04030003
#define GPIO_CNL_LP_GPP_D4               0x04030004
#define GPIO_CNL_LP_GPP_D5               0x04030005
#define GPIO_CNL_LP_GPP_D6               0x04030006
#define GPIO_CNL_LP_GPP_D7               0x04030007
#define GPIO_CNL_LP_GPP_D8               0x04030008
#define GPIO_CNL_LP_GPP_D9               0x04030009
#define GPIO_CNL_LP_GPP_D10              0x0403000A
#define GPIO_CNL_LP_GPP_D11              0x0403000B
#define GPIO_CNL_LP_GPP_D12              0x0403000C
#define GPIO_CNL_LP_GPP_D13              0x0403000D
#define GPIO_CNL_LP_GPP_D14              0x0403000E
#define GPIO_CNL_LP_GPP_D15              0x0403000F
#define GPIO_CNL_LP_GPP_D16              0x04030010
#define GPIO_CNL_LP_GPP_D17              0x04030011
#define GPIO_CNL_LP_GPP_D18              0x04030012
#define GPIO_CNL_LP_GPP_D19              0x04030013
#define GPIO_CNL_LP_GPP_D20              0x04030014
#define GPIO_CNL_LP_GPP_D21              0x04030015
#define GPIO_CNL_LP_GPP_D22              0x04030016
#define GPIO_CNL_LP_GPP_D23              0x04030017

#define GPIO_CNL_LP_GPP_E0               0x04040000
#define GPIO_CNL_LP_GPP_E1               0x04040001
#define GPIO_CNL_LP_GPP_E2               0x04040002
#define GPIO_CNL_LP_GPP_E3               0x04040003
#define GPIO_CNL_LP_GPP_E4               0x04040004
#define GPIO_CNL_LP_GPP_E5               0x04040005
#define GPIO_CNL_LP_GPP_E6               0x04040006
#define GPIO_CNL_LP_GPP_E7               0x04040007
#define GPIO_CNL_LP_GPP_E8               0x04040008
#define GPIO_CNL_LP_GPP_E9               0x04040009
#define GPIO_CNL_LP_GPP_E10              0x0404000A
#define GPIO_CNL_LP_GPP_E11              0x0404000B
#define GPIO_CNL_LP_GPP_E12              0x0404000C
#define GPIO_CNL_LP_GPP_E13              0x0404000D
#define GPIO_CNL_LP_GPP_E14              0x0404000E
#define GPIO_CNL_LP_GPP_E15              0x0404000F
#define GPIO_CNL_LP_GPP_E16              0x04040010
#define GPIO_CNL_LP_GPP_E17              0x04040011
#define GPIO_CNL_LP_GPP_E18              0x04040012
#define GPIO_CNL_LP_GPP_E19              0x04040013
#define GPIO_CNL_LP_GPP_E20              0x04040014
#define GPIO_CNL_LP_GPP_E21              0x04040015
#define GPIO_CNL_LP_GPP_E22              0x04040016
#define GPIO_CNL_LP_GPP_E23              0x04040017

#define GPIO_CNL_LP_GPP_F0               0x04050000
#define GPIO_CNL_LP_GPP_F1               0x04050001
#define GPIO_CNL_LP_GPP_F2               0x04050002
#define GPIO_CNL_LP_GPP_F3               0x04050003
#define GPIO_CNL_LP_GPP_F4               0x04050004
#define GPIO_CNL_LP_GPP_F5               0x04050005
#define GPIO_CNL_LP_GPP_F6               0x04050006
#define GPIO_CNL_LP_GPP_F7               0x04050007
#define GPIO_CNL_LP_GPP_F8               0x04050008
#define GPIO_CNL_LP_GPP_F9               0x04050009
#define GPIO_CNL_LP_GPP_F10              0x0405000A
#define GPIO_CNL_LP_GPP_F11              0x0405000B
#define GPIO_CNL_LP_GPP_F12              0x0405000C
#define GPIO_CNL_LP_GPP_F13              0x0405000D
#define GPIO_CNL_LP_GPP_F14              0x0405000E
#define GPIO_CNL_LP_GPP_F15              0x0405000F
#define GPIO_CNL_LP_GPP_F16              0x04050010
#define GPIO_CNL_LP_GPP_F17              0x04050011
#define GPIO_CNL_LP_GPP_F18              0x04050012
#define GPIO_CNL_LP_GPP_F19              0x04050013
#define GPIO_CNL_LP_GPP_F20              0x04050014
#define GPIO_CNL_LP_GPP_F21              0x04050015
#define GPIO_CNL_LP_GPP_F22              0x04050016
#define GPIO_CNL_LP_GPP_F23              0x04050017

#define GPIO_CNL_LP_GPP_G0               0x04060000
#define GPIO_CNL_LP_GPP_G1               0x04060001
#define GPIO_CNL_LP_GPP_G2               0x04060002
#define GPIO_CNL_LP_GPP_G3               0x04060003
#define GPIO_CNL_LP_GPP_G4               0x04060004
#define GPIO_CNL_LP_GPP_G5               0x04060005
#define GPIO_CNL_LP_GPP_G6               0x04060006
#define GPIO_CNL_LP_GPP_G7               0x04060007

#define GPIO_CNL_LP_GPP_H0               0x04070000
#define GPIO_CNL_LP_GPP_H1               0x04070001
#define GPIO_CNL_LP_GPP_H2               0x04070002
#define GPIO_CNL_LP_GPP_H3               0x04070003
#define GPIO_CNL_LP_GPP_H4               0x04070004
#define GPIO_CNL_LP_GPP_H5               0x04070005
#define GPIO_CNL_LP_GPP_H6               0x04070006
#define GPIO_CNL_LP_GPP_H7               0x04070007
#define GPIO_CNL_LP_GPP_H8               0x04070008
#define GPIO_CNL_LP_GPP_H9               0x04070009
#define GPIO_CNL_LP_GPP_H10              0x0407000A
#define GPIO_CNL_LP_GPP_H11              0x0407000B
#define GPIO_CNL_LP_GPP_H12              0x0407000C
#define GPIO_CNL_LP_GPP_H13              0x0407000D
#define GPIO_CNL_LP_GPP_H14              0x0407000E
#define GPIO_CNL_LP_GPP_H15              0x0407000F
#define GPIO_CNL_LP_GPP_H16              0x04070010
#define GPIO_CNL_LP_GPP_H17              0x04070011
#define GPIO_CNL_LP_GPP_H18              0x04070012
#define GPIO_CNL_LP_GPP_H19              0x04070013
#define GPIO_CNL_LP_GPP_H20              0x04070014
#define GPIO_CNL_LP_GPP_H21              0x04070015
#define GPIO_CNL_LP_GPP_H22              0x04070016
#define GPIO_CNL_LP_GPP_H23              0x04070017

#define GPIO_CNL_LP_GPD0                 0x04080000
#define GPIO_CNL_LP_GPD1                 0x04080001
#define GPIO_CNL_LP_GPD2                 0x04080002
#define GPIO_CNL_LP_GPD3                 0x04080003
#define GPIO_CNL_LP_GPD4                 0x04080004
#define GPIO_CNL_LP_GPD5                 0x04080005
#define GPIO_CNL_LP_GPD6                 0x04080006
#define GPIO_CNL_LP_GPD7                 0x04080007
#define GPIO_CNL_LP_GPD8                 0x04080008
#define GPIO_CNL_LP_GPD9                 0x04080009
#define GPIO_CNL_LP_GPD10                0x0408000A
#define GPIO_CNL_LP_GPD11                0x0408000B
#define GPIO_CNL_LP_SLP_LANB             0x0408000C
#define GPIO_CNL_LP_SLP_SUSB             0x0408000D
#define GPIO_CNL_LP_SLP_WAKEB            0x0408000E
#define GPIO_CNL_LP_SLP_DRAM_RESETB      0x0408000F

#define GPIO_CNL_LP_VGPIO0               0x04090000
#define GPIO_CNL_LP_VGPIO1               0x04090001
#define GPIO_CNL_LP_VGPIO2               0x04090002
#define GPIO_CNL_LP_VGPIO3               0x04090003
#define GPIO_CNL_LP_VGPIO4               0x04090004
#define GPIO_CNL_LP_VGPIO5               0x04090005
#define GPIO_CNL_LP_VGPIO6               0x04090006
#define GPIO_CNL_LP_VGPIO7               0x04090007
#define GPIO_CNL_LP_VGPIO8               0x04090008
#define GPIO_CNL_LP_VGPIO9               0x04090009
#define GPIO_CNL_LP_VGPIO10              0x0409000A
#define GPIO_CNL_LP_VGPIO11              0x0409000B
#define GPIO_CNL_LP_VGPIO12              0x0409000C
#define GPIO_CNL_LP_VGPIO13              0x0409000D
#define GPIO_CNL_LP_VGPIO14              0x0409000E
#define GPIO_CNL_LP_VGPIO15              0x0409000F
#define GPIO_CNL_LP_VGPIO16              0x04090010
#define GPIO_CNL_LP_VGPIO17              0x04090011
#define GPIO_CNL_LP_VGPIO18              0x04090012
#define GPIO_CNL_LP_VGPIO19              0x04090013
#define GPIO_CNL_LP_VGPIO20              0x04090014
#define GPIO_CNL_LP_VGPIO21              0x04090015
#define GPIO_CNL_LP_VGPIO22              0x04090016
#define GPIO_CNL_LP_VGPIO23              0x04090017
#define GPIO_CNL_LP_VGPIO24              0x04090018
#define GPIO_CNL_LP_VGPIO25              0x04090019
#define GPIO_CNL_LP_VGPIO26              0x0409001A
#define GPIO_CNL_LP_VGPIO27              0x0409001B
#define GPIO_CNL_LP_VGPIO28              0x0409001C
#define GPIO_CNL_LP_VGPIO29              0x0409001D
#define GPIO_CNL_LP_VGPIO30              0x0409001E
#define GPIO_CNL_LP_VGPIO31              0x0409001F
#define GPIO_CNL_LP_VGPIO32              0x04090020
#define GPIO_CNL_LP_VGPIO33              0x04090021
#define GPIO_CNL_LP_VGPIO34              0x04090022
#define GPIO_CNL_LP_VGPIO35              0x04090023
#define GPIO_CNL_LP_VGPIO36              0x04090024
#define GPIO_CNL_LP_VGPIO37              0x04090025
#define GPIO_CNL_LP_VGPIO38              0x04090026
#define GPIO_CNL_LP_VGPIO39              0x04090027

#define GPIO_CNL_LP_SPI0_IO_2            0x040A0000
#define GPIO_CNL_LP_SPI0_IO_3            0x040A0001
#define GPIO_CNL_LP_SPI0_MOSI_IO_0       0x040A0002
#define GPIO_CNL_LP_SPI0_MOSI_IO_1       0x040A0003
#define GPIO_CNL_LP_SPI0_TPM_CSB         0x040A0004
#define GPIO_CNL_LP_SPI0_FLASH_0_CSB     0x040A0005
#define GPIO_CNL_LP_SPI0_FLASH_1_CSB     0x040A0006
#define GPIO_CNL_LP_SPI0_CLK             0x040A0007
#define GPIO_CNL_LP_SPI0_CLK_LOOPBK      0x040A0008

#define GPIO_CNL_LP_HDA_BCLK             0x040B0000
#define GPIO_CNL_LP_HDA_RSTB             0x040B0001
#define GPIO_CNL_LP_HDA_SYNC             0x040B0002
#define GPIO_CNL_LP_HDA_SDO              0x040B0003
#define GPIO_CNL_LP_HDA_SDI_0            0x040B0004
#define GPIO_CNL_LP_HDA_SDI_1            0x040B0005
#define GPIO_CNL_LP_SSP1_SFRM            0x040B0006
#define GPIO_CNL_LP_SSP1_TXD             0x040B0007

#define GPIO_CNL_LP_HDACPU_SDI           0x040C0000
#define GPIO_CNL_LP_HDACPU_SDO           0x040C0001
#define GPIO_CNL_LP_HDACPU_SCLK          0x040C0002
#define GPIO_CNL_LP_PM_SYNC              0x040C0003
#define GPIO_CNL_LP_PECI                 0x040C0004
#define GPIO_CNL_LP_CPUPWRGD             0x040C0005
#define GPIO_CNL_LP_THRMTRIPB            0x040C0006
#define GPIO_CNL_LP_PLTRST_CPUB          0x040C0007
#define GPIO_CNL_LP_PM_DOWN              0x040C0008
#define GPIO_CNL_LP_TRIGGER_IN           0x040C0009
#define GPIO_CNL_LP_TRIGGER_OUT          0x040C000A

#define GPIO_CNL_LP_JTAG_TDO             0x040D0000
#define GPIO_CNL_LP_JTAGX                0x040D0001
#define GPIO_CNL_LP_PRDYB                0x040D0002
#define GPIO_CNL_LP_PREQB                0x040D0003
#define GPIO_CNL_LP_CPU_TRSTB            0x040D0004
#define GPIO_CNL_LP_JTAG_TDI             0x040D0005
#define GPIO_CNL_LP_JTAG_TMS             0x040D0006
#define GPIO_CNL_LP_JTAG_TCK             0x040D0007
#define GPIO_CNL_LP_ITP_PMODE            0x040D0008

#define GPIO_CNL_LP_HVMOS_L_BKLTEN       0x040E0000
#define GPIO_CNL_LP_HVMOS_L_BKLTCTL      0x040E0001
#define GPIO_CNL_LP_HVMOS_L_VDDEN        0x040E0002
#define GPIO_CNL_LP_HVMOS_SYS_PWROK      0x040E0003
#define GPIO_CNL_LP_HVMOS_SYS_RESETB     0x040E0004
#define GPIO_CNL_LP_HVMOS_MLK_RSTB       0x040E0005

#endif
