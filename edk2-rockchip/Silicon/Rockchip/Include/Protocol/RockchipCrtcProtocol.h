/** @file

  Copyright (c) 2022 Rockchip Electronics Co. Ltd.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/


#ifndef _ROCKCHIP_CRTC_PROTOCOL_H_
#define _ROCKCHIP_CRTC_PROTOCOL_H_

#include <Library/RockchipDisplayLib.h>

#define ROCKCHIP_CRTC_PROTOCOL_GUID   \
    {0xC128406A, 0x99D9, 0x11EC, {0x99, 0x27, 0xF4, 0x2A, 0x7D, 0xCB, 0x92, 0x5D}}

typedef struct _ROCKCHIP_CRTC_PROTOCOL ROCKCHIP_CRTC_PROTOCOL;

typedef struct {
  BOOLEAN                        Enable;
  UINT8                          BgOvlDly;
  UINT32                         PlaneMask;
  UINT32                         PrimaryPlane;
  INT32                          OutputType;
  INT32                          CursorPlane;
} VPS_CONFIG;

typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_CRTC_PREINIT) (
    IN ROCKCHIP_CRTC_PROTOCOL      *This,
    IN OUT DISPLAY_STATE           *DisplayState
    );

typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_CRTC_INIT) (
    IN ROCKCHIP_CRTC_PROTOCOL      *This,
    IN OUT DISPLAY_STATE           *DisplayState
    );


typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_CRTC_DEINIT) (
    IN ROCKCHIP_CRTC_PROTOCOL      *This,
    IN OUT DISPLAY_STATE           *DisplayState
    );

typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_CRTC_SET_PLANE) (
    IN ROCKCHIP_CRTC_PROTOCOL      *This,
    IN OUT DISPLAY_STATE           *DisplayState
    );

typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_CRTC_PREPARE) (
    IN ROCKCHIP_CRTC_PROTOCOL      *This,
    IN OUT DISPLAY_STATE           *DisplayState
    );

typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_CRTC_ENABLE) (
    IN ROCKCHIP_CRTC_PROTOCOL      *This,
    IN OUT DISPLAY_STATE           *DisplayState
    );

typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_CRTC_DISABLE) (
    IN ROCKCHIP_CRTC_PROTOCOL      *This,
    IN OUT DISPLAY_STATE           *DisplayState
    );

typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_CRTC_UNPREPARE) (
    IN ROCKCHIP_CRTC_PROTOCOL      *This,
    IN OUT DISPLAY_STATE           *DisplayState
    );

struct _ROCKCHIP_CRTC_PROTOCOL {
  VOID                           *Private;
  ROCKCHIP_CRTC_PREINIT          Preinit;
  ROCKCHIP_CRTC_INIT             Init;
  ROCKCHIP_CRTC_DEINIT           Deinit;
  ROCKCHIP_CRTC_SET_PLANE        SetPlane;
  ROCKCHIP_CRTC_PREPARE          Prepare;
  ROCKCHIP_CRTC_ENABLE           Enable;
  ROCKCHIP_CRTC_DISABLE          Disable;
  ROCKCHIP_CRTC_UNPREPARE        Unprepare;
  DRM_DISPLAY_MODE               ActiveMode;
  VPS_CONFIG                     Vps[4];
  BOOLEAN                        HdmiHpd;
  BOOLEAN                        Active;
  BOOLEAN                        AssignPlane;
};

extern EFI_GUID gRockchipCrtcProtocolGuid;

#endif
