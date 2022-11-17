/** @file

  Copyright (c) 2022 Rockchip Electronics Co. Ltd.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/


#ifndef __ROCKCHIP_DISPLAY_H__
#define __ROCKCHIP_DISPLAY_H__

#include <Uefi/UefiBaseType.h>

#define LIST_FOR_EACH_ENTRY(Pos, Head, Member)				\
	for (Pos = BASE_CR((Head)->ForwardLink, typeof(*Pos), Member);	\
	     &Pos->Member != (Head);					\
	     Pos = BASE_CR(Pos->Member.ForwardLink, typeof(*Pos), Member))

#define __ROUND_MASK(x, y) ((__typeof__(x))((y)-1))
#define ROUNDUP(x, y) ((((x)-1) | __ROUND_MASK(x, y))+1)
#define ROUNDDOWN(x, y) ((x) & ~__ROUND_MASK(x, y))
#define ALIGN(x, a) (((x) + (a) - 1) & ~((a) - 1))

#define INLINE static inline

#define BIT(x)                (1 << x)

#define EDID_SIZE             128

typedef enum {
  ROCKCHIP_DISPLAY_FULLSCREEN,
  ROCKCHIP_DISPLAY_CENTER,
} LOGO_DISPLAY_MODE;

typedef enum {
  ROCKCHIP_FMT_ARGB8888 = 0,
  ROCKCHIP_FMT_RGB888,
  ROCKCHIP_FMT_RGB565,
  ROCKCHIP_FMT_YUV420SP = 4,
  ROCKCHIP_FMT_YUV422SP,
  ROCKCHIP_FMT_YUV444SP,
} DATA_FORMAT;

/*
 * display output interface supported by rockchip lcdc
 */
#define ROCKCHIP_OUT_MODE_P888          0
#define ROCKCHIP_OUT_MODE_BT1120        0
#define ROCKCHIP_OUT_MODE_P666          1
#define ROCKCHIP_OUT_MODE_P565          2
#define ROCKCHIP_OUT_MODE_BT656         5
#define ROCKCHIP_OUT_MODE_S888          8
#define ROCKCHIP_OUT_MODE_S888_DUMMY    12
#define ROCKCHIP_OUT_MODE_YUV420        14
/* for use special outface */
#define ROCKCHIP_OUT_MODE_AAAA          15

#define VOP_OUTPUT_IF_RGB               BIT(0)
#define VOP_OUTPUT_IF_BT1120            BIT(1)
#define VOP_OUTPUT_IF_BT656             BIT(2)
#define VOP_OUTPUT_IF_LVDS0             BIT(3)
#define VOP_OUTPUT_IF_LVDS1             BIT(4)
#define VOP_OUTPUT_IF_MIPI0             BIT(5)
#define VOP_OUTPUT_IF_MIPI1             BIT(6)
#define VOP_OUTPUT_IF_eDP0              BIT(7)
#define VOP_OUTPUT_IF_eDP1              BIT(8)
#define VOP_OUTPUT_IF_DP0               BIT(9)
#define VOP_OUTPUT_IF_DP1               BIT(10)
#define VOP_OUTPUT_IF_HDMI0             BIT(11)
#define VOP_OUTPUT_IF_HDMI1             BIT(12)

typedef struct {
  UINT32                      Mode;
  UINT32                      Offset;
  UINT32                      Width;
  UINT32                      Height;
  UINT32                      Bpp;
  CHAR8                       *Memory;
  BOOLEAN                     YMirror;
} LOGO_INFO;

typedef struct {
  UINT32                      Width;
  UINT32                      Height;
} VOP_RECT;

typedef struct {
  UINT32                      LeftMargin;
  UINT32                      RightMargin;
  UINT32                      TopMargin;
  UINT32                      BottomMargin;
} OVER_SCAN;

typedef struct {
  /* Proposed mode values */
  UINT32                      Clock;		/* in kHz */
  UINT32                      HDisplay;
  UINT32                      HSyncStart;
  UINT32                      HSyncEnd;
  UINT32                      HTotal;
  UINT32                      VDisplay;
  UINT32                      VSyncStart;
  UINT32                      VSyncEnd;
  UINT32                      VTotal;
  UINT32                      VRefresh;
  UINT32                      VScan;
  UINT32                      Flags;
  UINT32                      PictureAspectRatio;
  UINT32                      HSkew;
  UINT32                      Type;
  /* Actual mode we give to hw */
  UINT32                      CrtcClock;         /* in KHz */
  UINT32                      CrtcHDisplay;
  UINT32                      CrtcHBlankStart;
  UINT32                      CrtcHBblankEnd;
  UINT32                      CrtcHSyncStart;
  UINT32                      CrtcHSyncEnd;
  UINT32                      CrtcHTotal;
  UINT32                      CrtcHSkew;
  UINT32                      CrtcVDisplay;
  UINT32                      CrtcVBlankStart;
  UINT32                      CrtcVBlankEnd;
  UINT32                      CrtcVSyncStart;
  UINT32                      CrtcVSyncEnd;
  UINT32                      CrtcVTotal;
  BOOLEAN                     Invalid;
} DRM_DISPLAY_MODE;

typedef struct {
  UINT16 Brightness;
  UINT16 Contrast;
  UINT16 Saturation;
  UINT16 Hue;
} BASE_BCSH_INFO;

typedef struct {
  INT8 DispHeadFlag[6];
  /* struct base2_screen_info screen_info[4]; --- todo */
  BASE_BCSH_INFO BCSHInfo;
  /* struct base_overscan overscan_info; --- todo */
  /* struct base2_gamma_lut_data gamma_lut_data; --- todo */
  /* struct base2_cubic_lut_data cubic_lut_data; --- todo */
  /* struct framebuffer_info framebuffer_info; --- todo */
  UINT32 Reserved[244];
  UINT32 CRC;
} BASE2_DISP_INFO;

typedef struct {
  VOID                        *Connector;
  OVER_SCAN                   OverScan;
  DRM_DISPLAY_MODE            DisplayMode;
  BASE2_DISP_INFO             *DispInfo; /* disp_info from baseparameter 2.0 */
  UINT8                       EDID[EDID_SIZE * 4];
  UINT32                      BusFormat;
  UINT32                      OutputMode;
  UINT32                      Type;
  UINT32                      OutputInterface;
  UINT32                      OutputFlags;
  UINT32                      ColorSpace;
  UINT32                      BPC;

  /**
   * @hold_mode: enabled when it's:
   * (1) mcu hold mode
   * (2) mipi dsi cmd mode
   * (3) edp psr mode
   */
  BOOLEAN                     hold_mode;
} CONNECTOR_STATE;

typedef struct {
  VOID                        *Crtc;
  VOID                        *Private;
  UINT32                      CrtcID;
  UINT32                      Format;
  UINT32                      YMirror;
  UINT32                      RBSwap;
  UINT32                      XVirtual;
  UINT32                      SrcX;
  UINT32                      SrcY;
  UINT32                      SrcW;
  UINT32                      SrcH;
  UINT32                      CrtcX;
  UINT32                      CrtcY;
  UINT32                      CrtcW;
  UINT32                      CrtcH;
  UINT32                      Feature;
  UINT32                      DMAAddress;
  BOOLEAN                     YUVOverlay;
  VOP_RECT                    MaxOutput;
} CRTC_STATE;

typedef struct {
  LIST_ENTRY                  ListHead;
  CRTC_STATE                  CrtcState;
  CONNECTOR_STATE             ConnectorState;

  UINT32                      ModeNumber;
  INT32                       VpsConfigModeID;

  VOID                        *MemoryBase;
  UINT32                      MemorySize;

  BOOLEAN                     IsInit;
  BOOLEAN                     IsEnable;

  BOOLEAN                     IsForceOutput;
  UINT32                      ForceOutputFormat;
} DISPLAY_STATE;

EFIAPI
EFI_STATUS
DisplaySetCrtcInfo (
  OUT DRM_DISPLAY_MODE                     *Mode,
  IN  UINT32                               AdjustFlags
  );

EFIAPI
EFI_STATUS
DisplaySetFramebuffer (
  OUT EFI_PHYSICAL_ADDRESS*                VramBaseAddress,
  OUT UINTN*                               VramSize
  );

#endif
