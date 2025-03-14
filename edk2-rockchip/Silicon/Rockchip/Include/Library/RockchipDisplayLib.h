/** @file

  Copyright (c) 2022 Rockchip Electronics Co. Ltd.
  Copyright (c) 2024-2025, Mario Bălănică <mariobalanica02@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __ROCKCHIP_DISPLAY_LIB_H__
#define __ROCKCHIP_DISPLAY_LIB_H__

#include <Uefi/UefiBaseType.h>
#include <IndustryStandard/Edid.h>
#include <Library/uboot-env.h>
#include <Library/drm_dsc.h>
#include <RockchipDisplay.h>

#define __ROUND_MASK(x, y)  ((__typeof__(x))((y)-1))
#define ROUNDUP(x, y)       ((((x)-1) | __ROUND_MASK(x, y))+1)
#define ROUNDDOWN(x, y)     ((x) & ~__ROUND_MASK(x, y))
#define ALIGN(x, a)         (((x) + (a) - 1) & ~((a) - 1))

#define INLINE  static inline

#define EDID_MAX_EXTENSION_BLOCKS  4

#define EDID_MAX_SIZE  ((1 + EDID_MAX_EXTENSION_BLOCKS) * EDID_BLOCK_SIZE)

#define EDID_GET_BLOCK_COUNT(base)            \
  (1 + MIN (                                  \
         EDID_MAX_EXTENSION_BLOCKS,           \
         ((EDID_BASE *)(base))->ExtensionFlag \
         ))

#define EDID_GET_SIZE(base)  (EDID_GET_BLOCK_COUNT(base) * EDID_BLOCK_SIZE)

#define EDID_BLOCK(base, idx)  ((UINT8 *)(base) + ((idx) * EDID_BLOCK_SIZE))

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

#define ROCKCHIP_OUTPUT_DUAL_CHANNEL_LEFT_RIGHT_MODE  BIT(0)
#define ROCKCHIP_OUTPUT_DUAL_CHANNEL_ODD_EVEN_MODE    BIT(1)
#define ROCKCHIP_OUTPUT_DATA_SWAP                     BIT(2)
#define ROCKCHIP_OUTPUT_MIPI_DS_MODE                  BIT(3)

#define ROCKCHIP_DSC_PPS_SIZE_BYTE  88

/*
 * display output interface supported by rockchip lcdc
 */
#define ROCKCHIP_OUT_MODE_P888        0
#define ROCKCHIP_OUT_MODE_BT1120      0
#define ROCKCHIP_OUT_MODE_P666        1
#define ROCKCHIP_OUT_MODE_P565        2
#define ROCKCHIP_OUT_MODE_BT656       5
#define ROCKCHIP_OUT_MODE_S888        8
#define ROCKCHIP_OUT_MODE_S888_DUMMY  12
#define ROCKCHIP_OUT_MODE_YUV420      14
/* for use special outface */
#define ROCKCHIP_OUT_MODE_AAAA  15

typedef struct {
  UINT32     Mode;
  UINT32     Offset;
  UINT32     Width;
  UINT32     Height;
  UINT32     Bpp;
  CHAR8      *Memory;
  BOOLEAN    YMirror;
} LOGO_INFO;

typedef struct {
  UINT32    Width;
  UINT32    Height;
} VOP_RECT;

struct rockchip_dsc_sink_cap {
  /**
   * @slice_width: the number of pixel columns that comprise the slice width
   * @slice_height: the number of pixel rows that comprise the slice height
   * @block_pred: Does block prediction
   * @native_420: Does sink support DSC with 4:2:0 compression
   * @bpc_supported: compressed bpc supported by sink : 10, 12 or 16 bpc
   * @version_major: DSC major version
   * @version_minor: DSC minor version
   * @target_bits_per_pixel_x16: bits num after compress and multiply 16
   */
  UINT16     slice_width;
  UINT16     slice_height;
  BOOLEAN    block_pred;
  BOOLEAN    native_420;
  UINT8      bpc_supported;
  UINT8      version_major;
  UINT8      version_minor;
  UINT16     target_bits_per_pixel_x16;
};

typedef struct {
  UINT32    LeftMargin;
  UINT32    RightMargin;
  UINT32    TopMargin;
  UINT32    BottomMargin;
} OVER_SCAN;

typedef struct {
  /* Proposed mode values */
  UINT32     Clock;                             /* in kHz */
  UINT32     HDisplay;
  UINT32     HSyncStart;
  UINT32     HSyncEnd;
  UINT32     HTotal;
  UINT32     VDisplay;
  UINT32     VSyncStart;
  UINT32     VSyncEnd;
  UINT32     VTotal;
  UINT32     VRefresh;
  UINT32     VScan;
  UINT32     Flags;
  UINT32     PictureAspectRatio;
  UINT32     HSkew;
  UINT32     Type;
  /* Actual mode we give to hw */
  UINT32     CrtcClock;                          /* in KHz */
  UINT32     CrtcHDisplay;
  UINT32     CrtcHBlankStart;
  UINT32     CrtcHBblankEnd;
  UINT32     CrtcHSyncStart;
  UINT32     CrtcHSyncEnd;
  UINT32     CrtcHTotal;
  UINT32     CrtcHSkew;
  UINT32     CrtcVDisplay;
  UINT32     CrtcVBlankStart;
  UINT32     CrtcVBlankEnd;
  UINT32     CrtcVSyncStart;
  UINT32     CrtcVSyncEnd;
  UINT32     CrtcVTotal;
  BOOLEAN    Invalid;
} DRM_DISPLAY_MODE;

typedef struct {
  UINT16    Brightness;
  UINT16    Contrast;
  UINT16    Saturation;
  UINT16    Hue;
} BASE_BCSH_INFO;

typedef struct {
  INT8              DispHeadFlag[6];
  /* struct base2_screen_info screen_info[4]; --- todo */
  BASE_BCSH_INFO    BCSHInfo;
  /* struct base_overscan overscan_info; --- todo */
  /* struct base2_gamma_lut_data gamma_lut_data; --- todo */
  /* struct base2_cubic_lut_data cubic_lut_data; --- todo */
  /* struct framebuffer_info framebuffer_info; --- todo */
  UINT32            Reserved[244];
  UINT32            CRC;
} BASE2_DISP_INFO;

typedef struct {
  BOOLEAN    Hdmi20Supported;
  BOOLEAN    Hdmi20SpeedLimited;
  BOOLEAN    ScdcSupported;
} HDMI_SINK_INFO;

typedef struct {
  BOOLEAN           IsHdmi;
  HDMI_SINK_INFO    HdmiInfo;

  BOOLEAN           SelectableRgbRange;

  DISPLAY_MODE      PreferredMode;
} DISPLAY_SINK_INFO;

typedef struct {
  VOID                 *Connector;
  OVER_SCAN            OverScan;
  DRM_DISPLAY_MODE     DisplayMode;
  UINT32               DisplayModeVic;
  BASE2_DISP_INFO      *DispInfo;        /* disp_info from baseparameter 2.0 */
  UINT8                Edid[EDID_MAX_SIZE];
  UINT32               BusFormat;
  UINT32               OutputMode;
  UINT32               Type;
  UINT32               OutputInterface;
  UINT32               OutputFlags;
  UINT32               ColorSpace;
  UINT32               BPC;

  /**
   * @hold_mode: enabled when it's:
   * (1) mcu hold mode
   * (2) mipi dsi cmd mode
   * (3) edp psr mode
   */
  BOOLEAN              hold_mode;

  DISPLAY_SINK_INFO    SinkInfo;
} CONNECTOR_STATE;

typedef struct {
  VOID                                    *Crtc;
  VOID                                    *Private;
  UINT32                                  CrtcID;
  UINT32                                  Format;
  UINT32                                  YMirror;
  UINT32                                  RBSwap;
  UINT32                                  XVirtual;
  UINT32                                  SrcX;
  UINT32                                  SrcY;
  UINT32                                  SrcW;
  UINT32                                  SrcH;
  UINT32                                  CrtcX;
  UINT32                                  CrtcY;
  UINT32                                  CrtcW;
  UINT32                                  CrtcH;
  UINT32                                  Feature;
  UINT32                                  DMAAddress;
  BOOLEAN                                 YUVOverlay;
  VOP_RECT                                MaxOutput;

  UINT32                                  DclkCoreDiv;
  UINT32                                  DclkOutDiv;

  UINT8                                   dsc_id;
  UINT8                                   dsc_enable;
  UINT8                                   dsc_slice_num;
  UINT8                                   dsc_pixel_num;
  UINT64                                  dsc_txp_clk_rate;
  UINT64                                  dsc_pxl_clk_rate;
  UINT64                                  dsc_cds_clk_rate;
  struct drm_dsc_picture_parameter_set    pps;
  struct rockchip_dsc_sink_cap            dsc_sink_cap;
} CRTC_STATE;

typedef struct {
  CRTC_STATE         CrtcState;
  CONNECTOR_STATE    ConnectorState;

  INT32              VpsConfigModeID;

  BOOLEAN            IsEnable;
} DISPLAY_STATE;

EFIAPI
EFI_STATUS
DisplaySetCrtcInfo (
  OUT DRM_DISPLAY_MODE  *Mode,
  IN  UINT32            AdjustFlags
  );

UINT32
EFIAPI
DrmModeVRefresh (
  DRM_DISPLAY_MODE  *Mode
  );

VOID
DisplayModeToDrm (
  IN  CONST DISPLAY_MODE  *DisplayMode,
  OUT DRM_DISPLAY_MODE    *DrmDisplayMode
  );

UINT32
DisplayModeVRefresh (
  IN CONST DISPLAY_MODE  *DisplayMode
  );

UINT8
ConvertCeaToHdmiVic (
  IN UINT8  CeaVic
  );

UINT8
ConvertHdmiToCeaVic (
  IN UINT8  CeaVic
  );

CHAR8 *
GetVopOutputIfName (
  IN UINT32  OutputInterface
  );

EFI_STATUS
CheckEdidBlock (
  IN UINT8  *EdidBlock,
  IN UINT8  BlockIndex
  );

VOID
DebugPrintEdid (
  IN UINT8  *Edid
  );

VOID
DebugPrintDisplayMode (
  IN CONST DISPLAY_MODE  *DisplayMode,
  IN UINT32              Indent,
  IN BOOLEAN             PrintVic,
  IN BOOLEAN             PrintTimings
  );

VOID
DebugPrintDisplaySinkInfo (
  IN CONST DISPLAY_SINK_INFO  *SinkInfo,
  IN UINT32                   Indent
  );

#endif
