/** @file
Cmos address definition macros header file.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _CMOS_MAP_H_
#define _CMOS_MAP_H_

//
// CMOS 0x34/0x35 specifies the system memory above 16 MB.
// * CMOS(0x35) is the high byte
// * CMOS(0x34) is the low byte
// * The size is specified in 64kb chunks
// * Since this is memory above 16MB, the 16MB must be added
//   into the calculation to get the total memory size.
//
#define CMOS_SYSTEM_MEM_ABOVE_16MB_LOW_BYTE         0x34
#define CMOS_SYSTEM_MEM_ABOVE_16MB_HIGH_BYTE        0x35

//
// CMOS 0x5b-0x5d specifies the system memory above 4GB MB.
// * CMOS(0x5d) is the most significant size byte
// * CMOS(0x5c) is the middle size byte
// * CMOS(0x5b) is the least significant size byte
// * The size is specified in 64kb chunks
//
#define CMOS_SYSTEM_MEM_ABOVE_4GB_LOW_BYTE          0x5b
#define CMOS_SYSTEM_MEM_ABOVE_4GB_MIDDLE_BYTE       0x5c
#define CMOS_SYSTEM_MEM_ABOVE_4GB_HIGH_BYTE         0x5d


#endif // _CMOS_MAP_H_
