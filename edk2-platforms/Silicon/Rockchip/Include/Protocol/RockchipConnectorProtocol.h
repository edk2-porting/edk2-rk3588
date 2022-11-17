/** @file

  Copyright (c) 2022 Rockchip Electronics Co. Ltd.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/


#ifndef _ROCKCHIP_CONNECTOR_PROTOCOL_H_
#define _ROCKCHIP_CONNECTOR_PROTOCOL_H_

#include <Library/RockchipDisplayLib.h>

#define RK_CONNECTOR_PROTOCOL_GUID   \
    {0x50439CB6, 0x9B85, 0x11EC, {0x95, 0x73, 0xF4, 0x2A, 0x7D, 0xCB, 0x92, 0x5D}}

typedef struct _ROCKCHIP_CONNECTOR_PROTOCOL ROCKCHIP_CONNECTOR_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_CONNECTOR_PREINIT) (
    IN ROCKCHIP_CONNECTOR_PROTOCOL      *This,
    IN OUT DISPLAY_STATE                *DisplayState
    );

typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_CONNECTOR_INIT) (
    IN ROCKCHIP_CONNECTOR_PROTOCOL      *This,
    IN OUT DISPLAY_STATE                *DisplayState
    );


typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_CONNECTOR_DEINIT) (
    IN ROCKCHIP_CONNECTOR_PROTOCOL      *This,
    IN OUT DISPLAY_STATE                *DisplayState
    );

typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_CONNECTOR_DETECT) (
    IN ROCKCHIP_CONNECTOR_PROTOCOL      *This,
    IN OUT DISPLAY_STATE                *DisplayState
    );

typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_CONNECTOR_GET_TIMING) (
    IN ROCKCHIP_CONNECTOR_PROTOCOL      *This,
    IN OUT DISPLAY_STATE                *DisplayState
    );

typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_CONNECTOR_GET_EDIE) (
    IN ROCKCHIP_CONNECTOR_PROTOCOL      *This,
    IN OUT DISPLAY_STATE                *DisplayState
    );

typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_CONNECTOR_PREPARE) (
    IN ROCKCHIP_CONNECTOR_PROTOCOL      *This,
    IN OUT DISPLAY_STATE                *DisplayState
    );

typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_CONNECTOR_ENABLE) (
    IN ROCKCHIP_CONNECTOR_PROTOCOL      *This,
    IN OUT DISPLAY_STATE                *DisplayState
    );

typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_CONNECTOR_DISABLE) (
    IN ROCKCHIP_CONNECTOR_PROTOCOL      *This,
    IN OUT DISPLAY_STATE                *DisplayState
    );

typedef
EFI_STATUS
(EFIAPI *ROCKCHIP_CONNECTOR_UNPREPARE) (
    IN ROCKCHIP_CONNECTOR_PROTOCOL      *This,
    IN OUT DISPLAY_STATE                *DisplayState
    );

struct _ROCKCHIP_CONNECTOR_PROTOCOL {
  VOID                                *Private;
  ROCKCHIP_CONNECTOR_PREINIT          Preinit;
  ROCKCHIP_CONNECTOR_INIT             Init;
  ROCKCHIP_CONNECTOR_DEINIT           Deinit;
  ROCKCHIP_CONNECTOR_DETECT           Detect;
  ROCKCHIP_CONNECTOR_GET_TIMING       GetTiming;
  ROCKCHIP_CONNECTOR_GET_EDIE         GetEdid;
  ROCKCHIP_CONNECTOR_PREPARE          Prepare;
  ROCKCHIP_CONNECTOR_ENABLE           Enable;
  ROCKCHIP_CONNECTOR_DISABLE          Disable;
  ROCKCHIP_CONNECTOR_UNPREPARE        Unprepare;
};

extern EFI_GUID gRockchipConnectorProtocolGuid;

#endif
