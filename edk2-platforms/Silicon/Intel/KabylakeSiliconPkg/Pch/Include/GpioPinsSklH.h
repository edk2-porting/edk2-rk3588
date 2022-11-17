/** @file
  GPIO pins for SKL-PCH-H,

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _GPIO_PINS_SKL_H_H_
#define _GPIO_PINS_SKL_H_H_
///
/// This header file should be used together with
/// PCH GPIO lib in C and ASL. All defines used
/// must match both ASL/C syntax
///
///
/// SKL H GPIO Groups
/// Use below for functions from PCH GPIO Lib which
/// require GpioGroup as argument
///
#define GPIO_SKL_H_GROUP_GPP_A  0x0100
#define GPIO_SKL_H_GROUP_GPP_B  0x0101
#define GPIO_SKL_H_GROUP_GPP_C  0x0102
#define GPIO_SKL_H_GROUP_GPP_D  0x0103
#define GPIO_SKL_H_GROUP_GPP_E  0x0104
#define GPIO_SKL_H_GROUP_GPP_F  0x0105
#define GPIO_SKL_H_GROUP_GPP_G  0x0106
#define GPIO_SKL_H_GROUP_GPP_H  0x0107
#define GPIO_SKL_H_GROUP_GPP_I  0x0108
#define GPIO_SKL_H_GROUP_GPD    0x0109

///
/// SKL H GPIO pins
/// Use below for functions from PCH GPIO Lib which
/// require GpioPad as argument. Encoding used here
/// has all information required by library functions
///
#define GPIO_SKL_H_GPP_A0       0x01000000
#define GPIO_SKL_H_GPP_A1       0x01000001
#define GPIO_SKL_H_GPP_A2       0x01000002
#define GPIO_SKL_H_GPP_A3       0x01000003
#define GPIO_SKL_H_GPP_A4       0x01000004
#define GPIO_SKL_H_GPP_A5       0x01000005
#define GPIO_SKL_H_GPP_A6       0x01000006
#define GPIO_SKL_H_GPP_A7       0x01000007
#define GPIO_SKL_H_GPP_A8       0x01000008
#define GPIO_SKL_H_GPP_A9       0x01000009
#define GPIO_SKL_H_GPP_A10      0x0100000A
#define GPIO_SKL_H_GPP_A11      0x0100000B
#define GPIO_SKL_H_GPP_A12      0x0100000C
#define GPIO_SKL_H_GPP_A13      0x0100000D
#define GPIO_SKL_H_GPP_A14      0x0100000E
#define GPIO_SKL_H_GPP_A15      0x0100000F
#define GPIO_SKL_H_GPP_A16      0x01000010
#define GPIO_SKL_H_GPP_A17      0x01000011
#define GPIO_SKL_H_GPP_A18      0x01000012
#define GPIO_SKL_H_GPP_A19      0x01000013
#define GPIO_SKL_H_GPP_A20      0x01000014
#define GPIO_SKL_H_GPP_A21      0x01000015
#define GPIO_SKL_H_GPP_A22      0x01000016
#define GPIO_SKL_H_GPP_A23      0x01000017
#define GPIO_SKL_H_GPP_B0       0x01010000
#define GPIO_SKL_H_GPP_B1       0x01010001
#define GPIO_SKL_H_GPP_B2       0x01010002
#define GPIO_SKL_H_GPP_B3       0x01010003
#define GPIO_SKL_H_GPP_B4       0x01010004
#define GPIO_SKL_H_GPP_B5       0x01010005
#define GPIO_SKL_H_GPP_B6       0x01010006
#define GPIO_SKL_H_GPP_B7       0x01010007
#define GPIO_SKL_H_GPP_B8       0x01010008
#define GPIO_SKL_H_GPP_B9       0x01010009
#define GPIO_SKL_H_GPP_B10      0x0101000A
#define GPIO_SKL_H_GPP_B11      0x0101000B
#define GPIO_SKL_H_GPP_B12      0x0101000C
#define GPIO_SKL_H_GPP_B13      0x0101000D
#define GPIO_SKL_H_GPP_B14      0x0101000E
#define GPIO_SKL_H_GPP_B15      0x0101000F
#define GPIO_SKL_H_GPP_B16      0x01010010
#define GPIO_SKL_H_GPP_B17      0x01010011
#define GPIO_SKL_H_GPP_B18      0x01010012
#define GPIO_SKL_H_GPP_B19      0x01010013
#define GPIO_SKL_H_GPP_B20      0x01010014
#define GPIO_SKL_H_GPP_B21      0x01010015
#define GPIO_SKL_H_GPP_B22      0x01010016
#define GPIO_SKL_H_GPP_B23      0x01010017
#define GPIO_SKL_H_GPP_C0       0x01020000
#define GPIO_SKL_H_GPP_C1       0x01020001
#define GPIO_SKL_H_GPP_C2       0x01020002
#define GPIO_SKL_H_GPP_C3       0x01020003
#define GPIO_SKL_H_GPP_C4       0x01020004
#define GPIO_SKL_H_GPP_C5       0x01020005
#define GPIO_SKL_H_GPP_C6       0x01020006
#define GPIO_SKL_H_GPP_C7       0x01020007
#define GPIO_SKL_H_GPP_C8       0x01020008
#define GPIO_SKL_H_GPP_C9       0x01020009
#define GPIO_SKL_H_GPP_C10      0x0102000A
#define GPIO_SKL_H_GPP_C11      0x0102000B
#define GPIO_SKL_H_GPP_C12      0x0102000C
#define GPIO_SKL_H_GPP_C13      0x0102000D
#define GPIO_SKL_H_GPP_C14      0x0102000E
#define GPIO_SKL_H_GPP_C15      0x0102000F
#define GPIO_SKL_H_GPP_C16      0x01020010
#define GPIO_SKL_H_GPP_C17      0x01020011
#define GPIO_SKL_H_GPP_C18      0x01020012
#define GPIO_SKL_H_GPP_C19      0x01020013
#define GPIO_SKL_H_GPP_C20      0x01020014
#define GPIO_SKL_H_GPP_C21      0x01020015
#define GPIO_SKL_H_GPP_C22      0x01020016
#define GPIO_SKL_H_GPP_C23      0x01020017
#define GPIO_SKL_H_GPP_D0       0x01030000
#define GPIO_SKL_H_GPP_D1       0x01030001
#define GPIO_SKL_H_GPP_D2       0x01030002
#define GPIO_SKL_H_GPP_D3       0x01030003
#define GPIO_SKL_H_GPP_D4       0x01030004
#define GPIO_SKL_H_GPP_D5       0x01030005
#define GPIO_SKL_H_GPP_D6       0x01030006
#define GPIO_SKL_H_GPP_D7       0x01030007
#define GPIO_SKL_H_GPP_D8       0x01030008
#define GPIO_SKL_H_GPP_D9       0x01030009
#define GPIO_SKL_H_GPP_D10      0x0103000A
#define GPIO_SKL_H_GPP_D11      0x0103000B
#define GPIO_SKL_H_GPP_D12      0x0103000C
#define GPIO_SKL_H_GPP_D13      0x0103000D
#define GPIO_SKL_H_GPP_D14      0x0103000E
#define GPIO_SKL_H_GPP_D15      0x0103000F
#define GPIO_SKL_H_GPP_D16      0x01030010
#define GPIO_SKL_H_GPP_D17      0x01030011
#define GPIO_SKL_H_GPP_D18      0x01030012
#define GPIO_SKL_H_GPP_D19      0x01030013
#define GPIO_SKL_H_GPP_D20      0x01030014
#define GPIO_SKL_H_GPP_D21      0x01030015
#define GPIO_SKL_H_GPP_D22      0x01030016
#define GPIO_SKL_H_GPP_D23      0x01030017
#define GPIO_SKL_H_GPP_E0       0x01040000
#define GPIO_SKL_H_GPP_E1       0x01040001
#define GPIO_SKL_H_GPP_E2       0x01040002
#define GPIO_SKL_H_GPP_E3       0x01040003
#define GPIO_SKL_H_GPP_E4       0x01040004
#define GPIO_SKL_H_GPP_E5       0x01040005
#define GPIO_SKL_H_GPP_E6       0x01040006
#define GPIO_SKL_H_GPP_E7       0x01040007
#define GPIO_SKL_H_GPP_E8       0x01040008
#define GPIO_SKL_H_GPP_E9       0x01040009
#define GPIO_SKL_H_GPP_E10      0x0104000A
#define GPIO_SKL_H_GPP_E11      0x0104000B
#define GPIO_SKL_H_GPP_E12      0x0104000C
#define GPIO_SKL_H_GPP_F0       0x01050000
#define GPIO_SKL_H_GPP_F1       0x01050001
#define GPIO_SKL_H_GPP_F2       0x01050002
#define GPIO_SKL_H_GPP_F3       0x01050003
#define GPIO_SKL_H_GPP_F4       0x01050004
#define GPIO_SKL_H_GPP_F5       0x01050005
#define GPIO_SKL_H_GPP_F6       0x01050006
#define GPIO_SKL_H_GPP_F7       0x01050007
#define GPIO_SKL_H_GPP_F8       0x01050008
#define GPIO_SKL_H_GPP_F9       0x01050009
#define GPIO_SKL_H_GPP_F10      0x0105000A
#define GPIO_SKL_H_GPP_F11      0x0105000B
#define GPIO_SKL_H_GPP_F12      0x0105000C
#define GPIO_SKL_H_GPP_F13      0x0105000D
#define GPIO_SKL_H_GPP_F14      0x0105000E
#define GPIO_SKL_H_GPP_F15      0x0105000F
#define GPIO_SKL_H_GPP_F16      0x01050010
#define GPIO_SKL_H_GPP_F17      0x01050011
#define GPIO_SKL_H_GPP_F18      0x01050012
#define GPIO_SKL_H_GPP_F19      0x01050013
#define GPIO_SKL_H_GPP_F20      0x01050014
#define GPIO_SKL_H_GPP_F21      0x01050015
#define GPIO_SKL_H_GPP_F22      0x01050016
#define GPIO_SKL_H_GPP_F23      0x01050017
#define GPIO_SKL_H_GPP_G0       0x01060000
#define GPIO_SKL_H_GPP_G1       0x01060001
#define GPIO_SKL_H_GPP_G2       0x01060002
#define GPIO_SKL_H_GPP_G3       0x01060003
#define GPIO_SKL_H_GPP_G4       0x01060004
#define GPIO_SKL_H_GPP_G5       0x01060005
#define GPIO_SKL_H_GPP_G6       0x01060006
#define GPIO_SKL_H_GPP_G7       0x01060007
#define GPIO_SKL_H_GPP_G8       0x01060008
#define GPIO_SKL_H_GPP_G9       0x01060009
#define GPIO_SKL_H_GPP_G10      0x0106000A
#define GPIO_SKL_H_GPP_G11      0x0106000B
#define GPIO_SKL_H_GPP_G12      0x0106000C
#define GPIO_SKL_H_GPP_G13      0x0106000D
#define GPIO_SKL_H_GPP_G14      0x0106000E
#define GPIO_SKL_H_GPP_G15      0x0106000F
#define GPIO_SKL_H_GPP_G16      0x01060010
#define GPIO_SKL_H_GPP_G17      0x01060011
#define GPIO_SKL_H_GPP_G18      0x01060012
#define GPIO_SKL_H_GPP_G19      0x01060013
#define GPIO_SKL_H_GPP_G20      0x01060014
#define GPIO_SKL_H_GPP_G21      0x01060015
#define GPIO_SKL_H_GPP_G22      0x01060016
#define GPIO_SKL_H_GPP_G23      0x01060017
#define GPIO_SKL_H_GPP_H0       0x01070000
#define GPIO_SKL_H_GPP_H1       0x01070001
#define GPIO_SKL_H_GPP_H2       0x01070002
#define GPIO_SKL_H_GPP_H3       0x01070003
#define GPIO_SKL_H_GPP_H4       0x01070004
#define GPIO_SKL_H_GPP_H5       0x01070005
#define GPIO_SKL_H_GPP_H6       0x01070006
#define GPIO_SKL_H_GPP_H7       0x01070007
#define GPIO_SKL_H_GPP_H8       0x01070008
#define GPIO_SKL_H_GPP_H9       0x01070009
#define GPIO_SKL_H_GPP_H10      0x0107000A
#define GPIO_SKL_H_GPP_H11      0x0107000B
#define GPIO_SKL_H_GPP_H12      0x0107000C
#define GPIO_SKL_H_GPP_H13      0x0107000D
#define GPIO_SKL_H_GPP_H14      0x0107000E
#define GPIO_SKL_H_GPP_H15      0x0107000F
#define GPIO_SKL_H_GPP_H16      0x01070010
#define GPIO_SKL_H_GPP_H17      0x01070011
#define GPIO_SKL_H_GPP_H18      0x01070012
#define GPIO_SKL_H_GPP_H19      0x01070013
#define GPIO_SKL_H_GPP_H20      0x01070014
#define GPIO_SKL_H_GPP_H21      0x01070015
#define GPIO_SKL_H_GPP_H22      0x01070016
#define GPIO_SKL_H_GPP_H23      0x01070017
#define GPIO_SKL_H_GPP_I0       0x01080000
#define GPIO_SKL_H_GPP_I1       0x01080001
#define GPIO_SKL_H_GPP_I2       0x01080002
#define GPIO_SKL_H_GPP_I3       0x01080003
#define GPIO_SKL_H_GPP_I4       0x01080004
#define GPIO_SKL_H_GPP_I5       0x01080005
#define GPIO_SKL_H_GPP_I6       0x01080006
#define GPIO_SKL_H_GPP_I7       0x01080007
#define GPIO_SKL_H_GPP_I8       0x01080008
#define GPIO_SKL_H_GPP_I9       0x01080009
#define GPIO_SKL_H_GPP_I10      0x0108000A
#define GPIO_SKL_H_GPD0         0x01090000
#define GPIO_SKL_H_GPD1         0x01090001
#define GPIO_SKL_H_GPD2         0x01090002
#define GPIO_SKL_H_GPD3         0x01090003
#define GPIO_SKL_H_GPD4         0x01090004
#define GPIO_SKL_H_GPD5         0x01090005
#define GPIO_SKL_H_GPD6         0x01090006
#define GPIO_SKL_H_GPD7         0x01090007
#define GPIO_SKL_H_GPD8         0x01090008
#define GPIO_SKL_H_GPD9         0x01090009
#define GPIO_SKL_H_GPD10        0x0109000A
#define GPIO_SKL_H_GPD11        0x0109000B

#endif
