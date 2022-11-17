/** @file
  Definitions shared with HFR/VFR files.

  @copyright
  Copyright 2006 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IIOSETUPDEFINITIONS_H_
#define _IIOSETUPDEFINITIONS_H_

//-----------------------------------------------------------------------------------
// Number of ports per socket for CPUs
//------------------------------------------------------------------------------------
#define NUMBER_PORTS_PER_SOCKET_ICX       21
#define NUMBER_PORTS_PER_SOCKET_SKX       21
#define NUMBER_PORTS_PER_SOCKET_CPX       13

//-----------------------------------------------------------------------------------
// General NUMBER_PORTS_PER_SOCKET definition
//------------------------------------------------------------------------------------
#define NUMBER_PORTS_PER_SOCKET       NUMBER_PORTS_PER_SOCKET_SKX

/**
==================================================================================================
================= Equates common for Setup options (.vfr/.hfr) and source files (.c/.h) ==========
==================================================================================================
**/

#define IIO_BIFURCATE_xxxxxxxx          0xFE
#define IIO_BIFURCATE_x4x4x4x4          0x0
#define IIO_BIFURCATE_x4x4xxx8          0x1
#define IIO_BIFURCATE_xxx8x4x4          0x2
#define IIO_BIFURCATE_xxx8xxx8          0x3
#define IIO_BIFURCATE_xxxxxx16          0x4
#define IIO_BIFURCATE_x2x2x4x8          0x5
#define IIO_BIFURCATE_x4x2x2x8          0x6
#define IIO_BIFURCATE_x8x2x2x4          0x7
#define IIO_BIFURCATE_x8x4x2x2          0x8
#define IIO_BIFURCATE_x2x2x4x4x4        0x9
#define IIO_BIFURCATE_x4x2x2x4x4        0xA
#define IIO_BIFURCATE_x4x4x2x2x4        0xB
#define IIO_BIFURCATE_x4x4x4x2x2        0xC
#define IIO_BIFURCATE_x2x2x2x2x8        0xD
#define IIO_BIFURCATE_x8x2x2x2x2        0xE
#define IIO_BIFURCATE_x2x2x2x2x4x4      0xF
#define IIO_BIFURCATE_x2x2x4x2x2x4      0x10
#define IIO_BIFURCATE_x2x2x4x4x2x2      0x11
#define IIO_BIFURCATE_x4x2x2x2x2x4      0x12
#define IIO_BIFURCATE_x4x2x2x4x2x2      0x13
#define IIO_BIFURCATE_x4x4x2x2x2x2      0x14
#define IIO_BIFURCATE_x2x2x2x2x2x2x4    0x15
#define IIO_BIFURCATE_x2x2x2x2x4x2x2    0x16
#define IIO_BIFURCATE_x2x2x4x2x2x2x2    0x17
#define IIO_BIFURCATE_x4x2x2x2x2x2x2    0x18
#define IIO_BIFURCATE_x2x2x2x2x2x2x2x2  0x19
#define IIO_BIFURCATE_AUTO              0xFF

#define IIO_OPTION_AUTO    2
#define IIO_OPTION_ENABLE  1
#define IIO_OPTION_DISABLE 0

#endif /* _IIOSETUPDEFINITIONS_H_ */
