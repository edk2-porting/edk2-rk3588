/** @file
  Vop2 DXE Driver, install RK_CRTC_PROTOCOL.

  Copyright (c) 2022 Rockchip Electronics Co. Ltd.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/


#ifndef __VOP2_H__
#define __VOP2_H__

#include <Library/Vop2Regs.h>

/*
 * major: IP major vertion, used for IP structure
 * minor: big feature change under same structure
 */
#define VOP_VERSION(major, minor)	((major) << 8 | (minor))
#define VOP_MAJOR(version)		((version) >> 8)
#define VOP_MINOR(version)		((version) & 0xFF)

#define VOP_VERSION_RK3568		VOP_VERSION(0x40, 0x15)
#define VOP_VERSION_RK3588		VOP_VERSION(0x40, 0x17)

/*
 * display output interface supported by rockchip lcdc
 */
#define ROCKCHIP_OUT_MODE_P888		0
#define ROCKCHIP_OUT_MODE_BT1120	0
#define ROCKCHIP_OUT_MODE_P666		1
#define ROCKCHIP_OUT_MODE_P565		2
#define ROCKCHIP_OUT_MODE_BT656		5
#define ROCKCHIP_OUT_MODE_S888		8
#define ROCKCHIP_OUT_MODE_S888_DUMMY	12
#define ROCKCHIP_OUT_MODE_YUV420	14
/* for use special outface */
#define ROCKCHIP_OUT_MODE_AAAA		15

#define VOP_OUTPUT_IF_RGB		BIT(0)
#define VOP_OUTPUT_IF_BT1120		BIT(1)
#define VOP_OUTPUT_IF_BT656		BIT(2)
#define VOP_OUTPUT_IF_LVDS0		BIT(3)
#define VOP_OUTPUT_IF_LVDS1		BIT(4)
#define VOP_OUTPUT_IF_MIPI0		BIT(5)
#define VOP_OUTPUT_IF_MIPI1		BIT(6)
#define VOP_OUTPUT_IF_eDP0		BIT(7)
#define VOP_OUTPUT_IF_eDP1		BIT(8)
#define VOP_OUTPUT_IF_DP0		BIT(9)
#define VOP_OUTPUT_IF_DP1		BIT(10)
#define VOP_OUTPUT_IF_HDMI0		BIT(11)
#define VOP_OUTPUT_IF_HDMI1		BIT(12)

#define VOP_OUTPUT_IF_NUMS		13

#define VOP2_LAYER_MAX			8

#define VOP_FEATURE_OUTPUT_10BIT	BIT(0)

typedef enum {
  CSC_BT601L,
  CSC_BT709L,
  CSC_BT601F,
  CSC_BT2020,
} VOP2_CSC_FORMAT;

typedef enum {
  BCSH_OUT_MODE_BLACK,
  BCSH_OUT_MODE_BLUE,
  BCSH_OUT_MODE_COLOR_BAR,
  BCSH_OUT_MODE_NORMAL_VIDEO,
} VOP2_BCSH_OUT_MODE;

typedef enum {
  VOP2_VP0,
  VOP2_VP1,
  VOP2_VP2,
  VOP2_VP3,
  VOP2_VP_MAX,
} VOP2_VIDEO_PORTS_ID;

typedef enum {
  ROCKCHIP_VOP2_CLUSTER0 = 0,
  ROCKCHIP_VOP2_CLUSTER1,
  ROCKCHIP_VOP2_ESMART0,
  ROCKCHIP_VOP2_ESMART1,
  ROCKCHIP_VOP2_SMART0,
  ROCKCHIP_VOP2_SMART1,
  ROCKCHIP_VOP2_CLUSTER2,
  ROCKCHIP_VOP2_CLUSTER3,
  ROCKCHIP_VOP2_ESMART2,
  ROCKCHIP_VOP2_ESMART3,
  ROCKCHIP_VOP2_LAYER_MAX,
} VOP2_LAYER_PHY_ID;

typedef enum {
  CLUSTER_LAYER = 0,
  ESMART_LAYER = 1,
  SMART_LAYER = 2,
} VOP2_LAYER_TYPE;

typedef enum {
  VOP2_SCALE_UP_NRST_NBOR,
  VOP2_SCALE_UP_BIL,
  VOP2_SCALE_UP_BIC,
} VOP2_SCALE_UP_MODE;

typedef enum {
  VOP2_SCALE_DOWN_NRST_NBOR,
  VOP2_SCALE_DOWN_BIL,
  VOP2_SCALE_DOWN_AVG,
} VOP2_SCALE_DOWN_MODE;

typedef enum {
  SCALE_NONE = 0x0,
  SCALE_UP   = 0x1,
  SCALE_DOWN = 0x2,
} SCALE_MODE;

typedef struct {
  BOOLEAN                     IsParentNeeded;
  CHAR8                       PdEnShift;
  CHAR8                       PdStatusShift;
  CHAR8                       PmuStatusShift;
  CHAR8                       BisrEnStatusShift;
  CHAR8                       ParentPhyID;
} VOP2_POWER_DOMAIN_DATA;

typedef struct {
  VOP2_LAYER_TYPE             Type;
  CHAR8                       *Name;
  UINT8                       PhysID;
  UINT8                       WinSelPortOffset;
  UINT8                       LayerSelWinID;
  UINT32                      RegOffset;
  VOP2_POWER_DOMAIN_DATA      *PdData;
} VOP2_WIN_DATA;

typedef struct {
  UINT32                      Feature;
  UINT32                      MaxDclk;
  UINT8                       PreScanMaxDly;
  VOP_RECT                    MaxOutput;
} VOP2_VP_DATA;

typedef struct {
  UINT8                       PrimaryPlaneId;                 /* use this win to show logo */
  UINT8                       AttachedLayersNr;               /* number layers attach to this vp */
  UINT8                       AttachedLayers[VOP2_LAYER_MAX]; /* the layers attached to this vp */
  UINT32                      PlaneMask;
  INT32                       CursorPlaneID;
} VOP2_VP_PLANE_MASK;

typedef struct {
  UINT32                      Version;
  VOP2_VP_DATA                *VpData;
  VOP2_WIN_DATA               *WinData;
  /*
  struct vop2_plane_table *plane_table;
  */
  VOP2_VP_PLANE_MASK          *PlaneMask;

  UINT8                       NrVps;
  UINT8                       NrLayers;
  UINT8                       NrMixers;
  UINT8                       NrGammas;
  UINT8                       NrDscs;
  UINT32                      RegLen;
} VOP2_DATA;

typedef struct {
  UINT32                      BaseAddress;
  UINT32                      Version;
  BOOLEAN                     GlobalInit;
  VOP2_DATA                   *Data;
  VOP2_VP_PLANE_MASK          VpPlaneMask[VOP2_VP_MAX];
} VOP2;

#endif
