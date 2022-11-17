/** @file
  GPIO pins,

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _GPIO_PINS_CNL_H_H_
#define _GPIO_PINS_CNL_H_H_
///
/// This header file should be used together with
/// PCH GPIO lib in C and ASL. All defines used
/// must match both ASL/C syntax
///

///
/// Unique ID used in GpioPad defines
///
#define GPIO_CNL_H_CHIPSET_ID       0x3

///
/// Use below for functions from PCH GPIO Lib which
/// require GpioGroup as argument
///
#define GPIO_CNL_H_GROUP_GPP_A  0x0300
#define GPIO_CNL_H_GROUP_GPP_B  0x0301
#define GPIO_CNL_H_GROUP_GPP_C  0x0302
#define GPIO_CNL_H_GROUP_GPP_D  0x0303
#define GPIO_CNL_H_GROUP_GPP_E  0x0304
#define GPIO_CNL_H_GROUP_GPP_F  0x0305
#define GPIO_CNL_H_GROUP_GPP_G  0x0306
#define GPIO_CNL_H_GROUP_GPP_H  0x0307
#define GPIO_CNL_H_GROUP_GPP_I  0x0308
#define GPIO_CNL_H_GROUP_GPP_J  0x0309
#define GPIO_CNL_H_GROUP_GPP_K  0x030A
#define GPIO_CNL_H_GROUP_GPD    0x030B
#define GPIO_CNL_H_GROUP_VGPIO  0x030C
#define GPIO_CNL_H_GROUP_SPI    0x030D
#define GPIO_CNL_H_GROUP_AZA    0x030E
#define GPIO_CNL_H_GROUP_CPU    0x030F
#define GPIO_CNL_H_GROUP_JTAG   0x0310

///
/// Use below for functions from PCH GPIO Lib which
/// require GpioPad as argument. Encoding used here
/// has all information required by library functions
///
#define GPIO_CNL_H_GPP_A0               0x03000000
#define GPIO_CNL_H_GPP_A1               0x03000001
#define GPIO_CNL_H_GPP_A2               0x03000002
#define GPIO_CNL_H_GPP_A3               0x03000003
#define GPIO_CNL_H_GPP_A4               0x03000004
#define GPIO_CNL_H_GPP_A5               0x03000005
#define GPIO_CNL_H_GPP_A6               0x03000006
#define GPIO_CNL_H_GPP_A7               0x03000007
#define GPIO_CNL_H_GPP_A8               0x03000008
#define GPIO_CNL_H_GPP_A9               0x03000009
#define GPIO_CNL_H_GPP_A10              0x0300000A
#define GPIO_CNL_H_GPP_A11              0x0300000B
#define GPIO_CNL_H_GPP_A12              0x0300000C
#define GPIO_CNL_H_GPP_A13              0x0300000D
#define GPIO_CNL_H_GPP_A14              0x0300000E
#define GPIO_CNL_H_GPP_A15              0x0300000F
#define GPIO_CNL_H_GPP_A16              0x03000010
#define GPIO_CNL_H_GPP_A17              0x03000011
#define GPIO_CNL_H_GPP_A18              0x03000012
#define GPIO_CNL_H_GPP_A19              0x03000013
#define GPIO_CNL_H_GPP_A20              0x03000014
#define GPIO_CNL_H_GPP_A21              0x03000015
#define GPIO_CNL_H_GPP_A22              0x03000016
#define GPIO_CNL_H_GPP_A23              0x03000017
#define GPIO_CNL_H_ESPI_CLK_LOOPBK      0x03000018

#define GPIO_CNL_H_GPP_B0               0x03010000
#define GPIO_CNL_H_GPP_B1               0x03010001
#define GPIO_CNL_H_GPP_B2               0x03010002
#define GPIO_CNL_H_GPP_B3               0x03010003
#define GPIO_CNL_H_GPP_B4               0x03010004
#define GPIO_CNL_H_GPP_B5               0x03010005
#define GPIO_CNL_H_GPP_B6               0x03010006
#define GPIO_CNL_H_GPP_B7               0x03010007
#define GPIO_CNL_H_GPP_B8               0x03010008
#define GPIO_CNL_H_GPP_B9               0x03010009
#define GPIO_CNL_H_GPP_B10              0x0301000A
#define GPIO_CNL_H_GPP_B11              0x0301000B
#define GPIO_CNL_H_GPP_B12              0x0301000C
#define GPIO_CNL_H_GPP_B13              0x0301000D
#define GPIO_CNL_H_GPP_B14              0x0301000E
#define GPIO_CNL_H_GPP_B15              0x0301000F
#define GPIO_CNL_H_GPP_B16              0x03010010
#define GPIO_CNL_H_GPP_B17              0x03010011
#define GPIO_CNL_H_GPP_B18              0x03010012
#define GPIO_CNL_H_GPP_B19              0x03010013
#define GPIO_CNL_H_GPP_B20              0x03010014
#define GPIO_CNL_H_GPP_B21              0x03010015
#define GPIO_CNL_H_GPP_B22              0x03010016
#define GPIO_CNL_H_GPP_B23              0x03010017
#define GPIO_CNL_H_GSPI0_CLK_LOOPBK     0x03010018
#define GPIO_CNL_H_GSPI1_CLK_LOOPBK     0x03010019

#define GPIO_CNL_H_GPP_C0               0x03020000
#define GPIO_CNL_H_GPP_C1               0x03020001
#define GPIO_CNL_H_GPP_C2               0x03020002
#define GPIO_CNL_H_GPP_C3               0x03020003
#define GPIO_CNL_H_GPP_C4               0x03020004
#define GPIO_CNL_H_GPP_C5               0x03020005
#define GPIO_CNL_H_GPP_C6               0x03020006
#define GPIO_CNL_H_GPP_C7               0x03020007
#define GPIO_CNL_H_GPP_C8               0x03020008
#define GPIO_CNL_H_GPP_C9               0x03020009
#define GPIO_CNL_H_GPP_C10              0x0302000A
#define GPIO_CNL_H_GPP_C11              0x0302000B
#define GPIO_CNL_H_GPP_C12              0x0302000C
#define GPIO_CNL_H_GPP_C13              0x0302000D
#define GPIO_CNL_H_GPP_C14              0x0302000E
#define GPIO_CNL_H_GPP_C15              0x0302000F
#define GPIO_CNL_H_GPP_C16              0x03020010
#define GPIO_CNL_H_GPP_C17              0x03020011
#define GPIO_CNL_H_GPP_C18              0x03020012
#define GPIO_CNL_H_GPP_C19              0x03020013
#define GPIO_CNL_H_GPP_C20              0x03020014
#define GPIO_CNL_H_GPP_C21              0x03020015
#define GPIO_CNL_H_GPP_C22              0x03020016
#define GPIO_CNL_H_GPP_C23              0x03020017

#define GPIO_CNL_H_GPP_D0               0x03030000
#define GPIO_CNL_H_GPP_D1               0x03030001
#define GPIO_CNL_H_GPP_D2               0x03030002
#define GPIO_CNL_H_GPP_D3               0x03030003
#define GPIO_CNL_H_GPP_D4               0x03030004
#define GPIO_CNL_H_GPP_D5               0x03030005
#define GPIO_CNL_H_GPP_D6               0x03030006
#define GPIO_CNL_H_GPP_D7               0x03030007
#define GPIO_CNL_H_GPP_D8               0x03030008
#define GPIO_CNL_H_GPP_D9               0x03030009
#define GPIO_CNL_H_GPP_D10              0x0303000A
#define GPIO_CNL_H_GPP_D11              0x0303000B
#define GPIO_CNL_H_GPP_D12              0x0303000C
#define GPIO_CNL_H_GPP_D13              0x0303000D
#define GPIO_CNL_H_GPP_D14              0x0303000E
#define GPIO_CNL_H_GPP_D15              0x0303000F
#define GPIO_CNL_H_GPP_D16              0x03030010
#define GPIO_CNL_H_GPP_D17              0x03030011
#define GPIO_CNL_H_GPP_D18              0x03030012
#define GPIO_CNL_H_GPP_D19              0x03030013
#define GPIO_CNL_H_GPP_D20              0x03030014
#define GPIO_CNL_H_GPP_D21              0x03030015
#define GPIO_CNL_H_GPP_D22              0x03030016
#define GPIO_CNL_H_GPP_D23              0x03030017

#define GPIO_CNL_H_GPP_E0               0x03040000
#define GPIO_CNL_H_GPP_E1               0x03040001
#define GPIO_CNL_H_GPP_E2               0x03040002
#define GPIO_CNL_H_GPP_E3               0x03040003
#define GPIO_CNL_H_GPP_E4               0x03040004
#define GPIO_CNL_H_GPP_E5               0x03040005
#define GPIO_CNL_H_GPP_E6               0x03040006
#define GPIO_CNL_H_GPP_E7               0x03040007
#define GPIO_CNL_H_GPP_E8               0x03040008
#define GPIO_CNL_H_GPP_E9               0x03040009
#define GPIO_CNL_H_GPP_E10              0x0304000A
#define GPIO_CNL_H_GPP_E11              0x0304000B
#define GPIO_CNL_H_GPP_E12              0x0304000C

#define GPIO_CNL_H_GPP_F0               0x03050000
#define GPIO_CNL_H_GPP_F1               0x03050001
#define GPIO_CNL_H_GPP_F2               0x03050002
#define GPIO_CNL_H_GPP_F3               0x03050003
#define GPIO_CNL_H_GPP_F4               0x03050004
#define GPIO_CNL_H_GPP_F5               0x03050005
#define GPIO_CNL_H_GPP_F6               0x03050006
#define GPIO_CNL_H_GPP_F7               0x03050007
#define GPIO_CNL_H_GPP_F8               0x03050008
#define GPIO_CNL_H_GPP_F9               0x03050009
#define GPIO_CNL_H_GPP_F10              0x0305000A
#define GPIO_CNL_H_GPP_F11              0x0305000B
#define GPIO_CNL_H_GPP_F12              0x0305000C
#define GPIO_CNL_H_GPP_F13              0x0305000D
#define GPIO_CNL_H_GPP_F14              0x0305000E
#define GPIO_CNL_H_GPP_F15              0x0305000F
#define GPIO_CNL_H_GPP_F16              0x03050010
#define GPIO_CNL_H_GPP_F17              0x03050011
#define GPIO_CNL_H_GPP_F18              0x03050012
#define GPIO_CNL_H_GPP_F19              0x03050013
#define GPIO_CNL_H_GPP_F20              0x03050014
#define GPIO_CNL_H_GPP_F21              0x03050015
#define GPIO_CNL_H_GPP_F22              0x03050016
#define GPIO_CNL_H_GPP_F23              0x03050017

#define GPIO_CNL_H_GPP_G0               0x03060000
#define GPIO_CNL_H_GPP_G1               0x03060001
#define GPIO_CNL_H_GPP_G2               0x03060002
#define GPIO_CNL_H_GPP_G3               0x03060003
#define GPIO_CNL_H_GPP_G4               0x03060004
#define GPIO_CNL_H_GPP_G5               0x03060005
#define GPIO_CNL_H_GPP_G6               0x03060006
#define GPIO_CNL_H_GPP_G7               0x03060007

#define GPIO_CNL_H_GPP_H0               0x03070000
#define GPIO_CNL_H_GPP_H1               0x03070001
#define GPIO_CNL_H_GPP_H2               0x03070002
#define GPIO_CNL_H_GPP_H3               0x03070003
#define GPIO_CNL_H_GPP_H4               0x03070004
#define GPIO_CNL_H_GPP_H5               0x03070005
#define GPIO_CNL_H_GPP_H6               0x03070006
#define GPIO_CNL_H_GPP_H7               0x03070007
#define GPIO_CNL_H_GPP_H8               0x03070008
#define GPIO_CNL_H_GPP_H9               0x03070009
#define GPIO_CNL_H_GPP_H10              0x0307000A
#define GPIO_CNL_H_GPP_H11              0x0307000B
#define GPIO_CNL_H_GPP_H12              0x0307000C
#define GPIO_CNL_H_GPP_H13              0x0307000D
#define GPIO_CNL_H_GPP_H14              0x0307000E
#define GPIO_CNL_H_GPP_H15              0x0307000F
#define GPIO_CNL_H_GPP_H16              0x03070010
#define GPIO_CNL_H_GPP_H17              0x03070011
#define GPIO_CNL_H_GPP_H18              0x03070012
#define GPIO_CNL_H_GPP_H19              0x03070013
#define GPIO_CNL_H_GPP_H20              0x03070014
#define GPIO_CNL_H_GPP_H21              0x03070015
#define GPIO_CNL_H_GPP_H22              0x03070016
#define GPIO_CNL_H_GPP_H23              0x03070017

#define GPIO_CNL_H_GPP_I0               0x03080000
#define GPIO_CNL_H_GPP_I1               0x03080001
#define GPIO_CNL_H_GPP_I2               0x03080002
#define GPIO_CNL_H_GPP_I3               0x03080003
#define GPIO_CNL_H_GPP_I4               0x03080004
#define GPIO_CNL_H_GPP_I5               0x03080005
#define GPIO_CNL_H_GPP_I6               0x03080006
#define GPIO_CNL_H_GPP_I7               0x03080007
#define GPIO_CNL_H_GPP_I8               0x03080008
#define GPIO_CNL_H_GPP_I9               0x03080009
#define GPIO_CNL_H_GPP_I10              0x0308000A
#define GPIO_CNL_H_GPP_I11              0x0308000B
#define GPIO_CNL_H_GPP_I12              0x0308000C
#define GPIO_CNL_H_GPP_I13              0x0308000D
#define GPIO_CNL_H_GPP_I14              0x0308000E
#define GPIO_CNL_H_SYS_PWROK            0x0308000F
#define GPIO_CNL_H_SYS_RESETB           0x03080010
#define GPIO_CNL_H_MLK_RSTB             0x03080011

#define GPIO_CNL_H_GPP_J0               0x03090000
#define GPIO_CNL_H_GPP_J1               0x03090001
#define GPIO_CNL_H_GPP_J2               0x03090002
#define GPIO_CNL_H_GPP_J3               0x03090003
#define GPIO_CNL_H_GPP_J4               0x03090004
#define GPIO_CNL_H_GPP_J5               0x03090005
#define GPIO_CNL_H_GPP_J6               0x03090006
#define GPIO_CNL_H_GPP_J7               0x03090007
#define GPIO_CNL_H_GPP_J8               0x03090008
#define GPIO_CNL_H_GPP_J9               0x03090009
#define GPIO_CNL_H_GPP_J10              0x0309000A
#define GPIO_CNL_H_GPP_J11              0x0309000B

#define GPIO_CNL_H_GPP_K0               0x030A0000
#define GPIO_CNL_H_GPP_K1               0x030A0001
#define GPIO_CNL_H_GPP_K2               0x030A0002
#define GPIO_CNL_H_GPP_K3               0x030A0003
#define GPIO_CNL_H_GPP_K4               0x030A0004
#define GPIO_CNL_H_GPP_K5               0x030A0005
#define GPIO_CNL_H_GPP_K6               0x030A0006
#define GPIO_CNL_H_GPP_K7               0x030A0007
#define GPIO_CNL_H_GPP_K8               0x030A0008
#define GPIO_CNL_H_GPP_K9               0x030A0009
#define GPIO_CNL_H_GPP_K10              0x030A000A
#define GPIO_CNL_H_GPP_K11              0x030A000B
#define GPIO_CNL_H_GPP_K12              0x030A000C
#define GPIO_CNL_H_GPP_K13              0x030A000D
#define GPIO_CNL_H_GPP_K14              0x030A000E
#define GPIO_CNL_H_GPP_K15              0x030A000F
#define GPIO_CNL_H_GPP_K16              0x030A0010
#define GPIO_CNL_H_GPP_K17              0x030A0011
#define GPIO_CNL_H_GPP_K18              0x030A0012
#define GPIO_CNL_H_GPP_K19              0x030A0013
#define GPIO_CNL_H_GPP_K20              0x030A0014
#define GPIO_CNL_H_GPP_K21              0x030A0015
#define GPIO_CNL_H_GPP_K22              0x030A0016
#define GPIO_CNL_H_GPP_K23              0x030A0017

#define GPIO_CNL_H_GPD0                 0x030B0000
#define GPIO_CNL_H_GPD1                 0x030B0001
#define GPIO_CNL_H_GPD2                 0x030B0002
#define GPIO_CNL_H_GPD3                 0x030B0003
#define GPIO_CNL_H_GPD4                 0x030B0004
#define GPIO_CNL_H_GPD5                 0x030B0005
#define GPIO_CNL_H_GPD6                 0x030B0006
#define GPIO_CNL_H_GPD7                 0x030B0007
#define GPIO_CNL_H_GPD8                 0x030B0008
#define GPIO_CNL_H_GPD9                 0x030B0009
#define GPIO_CNL_H_GPD10                0x030B000A
#define GPIO_CNL_H_GPD11                0x030B000B
#define GPIO_CNL_H_SLP_LANB             0x030B000C
#define GPIO_CNL_H_SLP_SUSB             0x030B000D
#define GPIO_CNL_H_SLP_WAKEB            0x030B000E
#define GPIO_CNL_H_SLP_DRAM_RESETB      0x030B000F

#define GPIO_CNL_H_VGPIO0               0x030C0000
#define GPIO_CNL_H_VGPIO1               0x030C0001
#define GPIO_CNL_H_VGPIO2               0x030C0002
#define GPIO_CNL_H_VGPIO3               0x030C0003
#define GPIO_CNL_H_VGPIO4               0x030C0004
#define GPIO_CNL_H_VGPIO5               0x030C0005
#define GPIO_CNL_H_VGPIO6               0x030C0006
#define GPIO_CNL_H_VGPIO7               0x030C0007
#define GPIO_CNL_H_VGPIO8               0x030C0008
#define GPIO_CNL_H_VGPIO9               0x030C0009
#define GPIO_CNL_H_VGPIO10              0x030C000A
#define GPIO_CNL_H_VGPIO11              0x030C000B
#define GPIO_CNL_H_VGPIO12              0x030C000C
#define GPIO_CNL_H_VGPIO13              0x030C000D
#define GPIO_CNL_H_VGPIO14              0x030C000E
#define GPIO_CNL_H_VGPIO15              0x030C000F
#define GPIO_CNL_H_VGPIO16              0x030C0010
#define GPIO_CNL_H_VGPIO17              0x030C0011
#define GPIO_CNL_H_VGPIO18              0x030C0012
#define GPIO_CNL_H_VGPIO19              0x030C0013
#define GPIO_CNL_H_VGPIO20              0x030C0014
#define GPIO_CNL_H_VGPIO21              0x030C0015
#define GPIO_CNL_H_VGPIO22              0x030C0016
#define GPIO_CNL_H_VGPIO23              0x030C0017
#define GPIO_CNL_H_VGPIO24              0x030C0018
#define GPIO_CNL_H_VGPIO25              0x030C0019
#define GPIO_CNL_H_VGPIO26              0x030C001A
#define GPIO_CNL_H_VGPIO27              0x030C001B
#define GPIO_CNL_H_VGPIO28              0x030C001C
#define GPIO_CNL_H_VGPIO29              0x030C001D
#define GPIO_CNL_H_VGPIO30              0x030C001E
#define GPIO_CNL_H_VGPIO31              0x030C001F
#define GPIO_CNL_H_VGPIO32              0x030C0020
#define GPIO_CNL_H_VGPIO33              0x030C0021
#define GPIO_CNL_H_VGPIO34              0x030C0022
#define GPIO_CNL_H_VGPIO35              0x030C0023
#define GPIO_CNL_H_VGPIO36              0x030C0024
#define GPIO_CNL_H_VGPIO37              0x030C0025
#define GPIO_CNL_H_VGPIO38              0x030C0026
#define GPIO_CNL_H_VGPIO39              0x030C0027

#define GPIO_CNL_H_SPI0_IO_2            0x030D0000
#define GPIO_CNL_H_SPI0_IO_3            0x030D0001
#define GPIO_CNL_H_SPI0_MOSI_IO_0       0x030D0002
#define GPIO_CNL_H_SPI0_MOSI_IO_1       0x030D0003
#define GPIO_CNL_H_SPI0_TPM_CSB         0x030D0004
#define GPIO_CNL_H_SPI0_FLASH_0_CSB     0x030D0005
#define GPIO_CNL_H_SPI0_FLASH_1_CSB     0x030D0006
#define GPIO_CNL_H_SPI0_CLK             0x030D0007
#define GPIO_CNL_H_SPI0_CLK_LOOPBK      0x030D0008

#define GPIO_CNL_H_HDA_BCLK             0x030E0000
#define GPIO_CNL_H_HDA_RSTB             0x030E0001
#define GPIO_CNL_H_HDA_SYNC             0x030E0002
#define GPIO_CNL_H_HDA_SDO              0x030E0003
#define GPIO_CNL_H_HDA_SDI_0            0x030E0004
#define GPIO_CNL_H_HDA_SDI_1            0x030E0005
#define GPIO_CNL_H_SSP1_SFRM            0x030E0006
#define GPIO_CNL_H_SSP1_TXD             0x030E0007

#define GPIO_CNL_H_HDACPU_SDI           0x030F0000
#define GPIO_CNL_H_HDACPU_SDO           0x030F0001
#define GPIO_CNL_H_HDACPU_SCLK          0x030F0002
#define GPIO_CNL_H_PM_SYNC              0x030F0003
#define GPIO_CNL_H_PECI                 0x030F0004
#define GPIO_CNL_H_CPUPWRGD             0x030F0005
#define GPIO_CNL_H_THRMTRIPB            0x030F0006
#define GPIO_CNL_H_PLTRST_CPUB          0x030F0007
#define GPIO_CNL_H_PM_DOWN              0x030F0008
#define GPIO_CNL_H_TRIGGER_IN           0x030F0009
#define GPIO_CNL_H_TRIGGER_OUT          0x030F000A

#define GPIO_CNL_H_JTAG_TDO             0x03100000
#define GPIO_CNL_H_JTAGX                0x03100001
#define GPIO_CNL_H_PRDYB                0x03100002
#define GPIO_CNL_H_PREQB                0x03100003
#define GPIO_CNL_H_CPU_TRSTB            0x03100004
#define GPIO_CNL_H_JTAG_TDI             0x03100005
#define GPIO_CNL_H_JTAG_TMS             0x03100006
#define GPIO_CNL_H_JTAG_TCK             0x03100007
#define GPIO_CNL_H_ITP_PMODE            0x03100008

#endif
