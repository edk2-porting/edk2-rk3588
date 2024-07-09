/** @file
*
*  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _ROCKCHIP_DSI_PANEL_PROTOCOL_H_
#define _ROCKCHIP_DSI_PANEL_PROTOCOL_H_

#include <Library/RockchipDisplayLib.h>
#include <Library/drm_mipi_dsi.h>

#define ROCKCHIP_DSI_PANEL_PROTOCOL_GUID   \
    { 0xb4bcf881, 0xc8b3, 0x46d7, { 0xaf, 0xbe, 0x5a, 0x2d, 0x94, 0x9d, 0x93, 0xc3 } }

typedef struct _ROCKCHIP_DSI_PANEL_PROTOCOL ROCKCHIP_DSI_PANEL_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_DSI_PANEL_PREPARE)(
  IN ROCKCHIP_DSI_PANEL_PROTOCOL          *This
  );

struct _ROCKCHIP_DSI_PANEL_PROTOCOL {
  UINT32                        DsiId;
  UINT32                        DsiLaneRate;
  UINT32                        DsiLanes;
  UINT32                        DsiFlags;
  UINT32                        DsiFormat;
  BOOLEAN                       DscEnable;
  BOOLEAN                       CPhyEnable;
  BOOLEAN                       ScramblingEnable;
  UINT32                        SliceWidth;
  UINT32                        SliceHeight;
  UINT32                        VersionMajor;
  UINT32                        VersionMinor;

  UINT8                         *InitSequence;
  UINT32                        InitSequenceLength;

  DISPLAY_MODE                  NativeMode;

  ROCKCHIP_DSI_PANEL_PREPARE    Prepare;
};

extern EFI_GUID  gRockchipDsiPanelProtocolGuid;

#endif // _ROCKCHIP_DSI_PANEL_PROTOCOL_H_
