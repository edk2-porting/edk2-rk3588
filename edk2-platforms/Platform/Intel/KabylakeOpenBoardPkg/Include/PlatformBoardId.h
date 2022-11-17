/** @file
Kaby Lake Platform Board Identifiers

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_BOARD_ID_H_
#define _PLATFORM_BOARD_ID_H_

#define FlavorUnknown                       0x0
#define FlavorMobile                        0x1
#define FlavorDesktop                       0x2
#define FlavorWorkstation                   0x3
#define FlavorUpServer                      0x4
#define FlavorEmbedded                      0x5
#define FlavorPlatformMax                   0x6

#define TypeUnknown                         0x0
#define TypeTrad                            0x1
#define TypeUltUlx                          0x2

#define BoardIdSkylakeRvp3                  0x4
#define BoardIdGalagoPro3                   0x20
#define BoardIdRayleighSLx_dGPU             0x41
#define BoardIdNewgateSLx_dGPU              0x42
#define BoardIdKabyLakeYLpddr3Rvp3          0x60

#define BoardIdUnknown1                     0xffff

#endif
