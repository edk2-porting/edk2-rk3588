/** @file
 *
 *  Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __ROCKCHIP_DISPLAY_H__
#define __ROCKCHIP_DISPLAY_H__

#define VOP_OUTPUT_IF_RGB     0x00000001
#define VOP_OUTPUT_IF_BT1120  0x00000002
#define VOP_OUTPUT_IF_BT656   0x00000004
#define VOP_OUTPUT_IF_LVDS0   0x00000008
#define VOP_OUTPUT_IF_LVDS1   0x00000010
#define VOP_OUTPUT_IF_MIPI0   0x00000020
#define VOP_OUTPUT_IF_MIPI1   0x00000040
#define VOP_OUTPUT_IF_EDP0    0x00000080
#define VOP_OUTPUT_IF_EDP1    0x00000100
#define VOP_OUTPUT_IF_DP0     0x00000200
#define VOP_OUTPUT_IF_DP1     0x00000400
#define VOP_OUTPUT_IF_HDMI0   0x00000800
#define VOP_OUTPUT_IF_HDMI1   0x00001000
#define VOP_OUTPUT_IF_NUMS    13

#define VOP_PIXEL_CLOCK_MIN  25175
#define VOP_PIXEL_CLOCK_MAX  600000

#define VOP_HORIZONTAL_RES_MIN  640
#define VOP_HORIZONTAL_RES_MAX  4096
#define VOP_VERTICAL_RES_MIN    480
#define VOP_VERTICAL_RES_MAX    4320

#pragma pack (1)

typedef struct {
  UINT32    Vic;
  UINT32    OscFreq;
  UINT32    HActive;
  UINT32    HFrontPorch;
  UINT32    HSync;
  UINT32    HBackPorch;
  UINT32    HSyncActive;
  UINT32    VActive;
  UINT32    VFrontPorch;
  UINT32    VSync;
  UINT32    VBackPorch;
  UINT32    VSyncActive;
  UINT32    DenActive;
  UINT32    ClkActive;
} DISPLAY_MODE;

#pragma pack ()

#endif
