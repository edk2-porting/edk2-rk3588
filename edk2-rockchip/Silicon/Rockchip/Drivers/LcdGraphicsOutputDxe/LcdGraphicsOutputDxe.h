/** @file

  Copyright (c) 2011-2018, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2022 Rockchip Electronics Co. Ltd.
  Copyright (c) 2023-2025, Mario Bălănică <mariobalanica02@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef LCD_GRAPHICS_OUTPUT_DXE_H_
#define LCD_GRAPHICS_OUTPUT_DXE_H_

#include <Base.h>

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/RockchipDisplayLib.h>

#include <Protocol/Cpu.h>
#include <Protocol/DevicePath.h>
#include <Protocol/RockchipCrtcProtocol.h>
#include <Protocol/RockchipConnectorProtocol.h>

#include <VarStoreData.h>

#define RK_BYTES_PER_PIXEL  (sizeof (UINT32))

//
// Device structures
//
typedef struct {
  VENDOR_DEVICE_PATH          Guid;
  EFI_DEVICE_PATH_PROTOCOL    End;
} LCD_GRAPHICS_DEVICE_PATH;

typedef struct {
  UINT32                                  Signature;
  EFI_HANDLE                              Handle;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION    ModeInfo;
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE       Mode;
  EFI_GRAPHICS_OUTPUT_PROTOCOL            Gop;
  LCD_GRAPHICS_DEVICE_PATH                DevicePath;
  DISPLAY_STATE                           *DisplayStates[VOP_OUTPUT_IF_NUMS];
  UINT32                                  DisplayStatesCount;
  DISPLAY_MODE                            *DisplayModes;
} LCD_INSTANCE;

#define LCD_INSTANCE_SIGNATURE  SIGNATURE_32('l', 'c', 'd', '0')

#define LCD_INSTANCE_FROM_GOP_THIS(a)  CR (a, LCD_INSTANCE, Gop, LCD_INSTANCE_SIGNATURE)

EFI_STATUS
EFIAPI
LcdGraphicsQueryMode (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL          *This,
  IN  UINT32                                ModeNumber,
  OUT UINTN                                 *SizeOfInfo,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  **Info
  );

EFI_STATUS
EFIAPI
LcdGraphicsSetMode (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *This,
  IN UINT32                        ModeNumber
  );

EFI_STATUS
EFIAPI
LcdGraphicsBlt (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL       *This,
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL  *BltBuffer  OPTIONAL,
  IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION  BltOperation,
  IN UINTN                              SourceX,
  IN UINTN                              SourceY,
  IN UINTN                              DestinationX,
  IN UINTN                              DestinationY,
  IN UINTN                              Width,
  IN UINTN                              Height,
  IN UINTN                              Delta       OPTIONAL
  );

EFI_STATUS
EFIAPI
LcdGraphicsBlt90 (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL       *This,
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL  *BltBuffer  OPTIONAL,
  IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION  BltOperation,
  IN UINTN                              SourceX,
  IN UINTN                              SourceY,
  IN UINTN                              DestinationX,
  IN UINTN                              DestinationY,
  IN UINTN                              Width,
  IN UINTN                              Height,
  IN UINTN                              Delta       OPTIONAL
  );

BOOLEAN
IsDisplayModeSupported (
  IN CONNECTOR_STATE     *ConnectorState,
  IN CONST DISPLAY_MODE  *DisplayMode
  );

UINT32
GetPredefinedDisplayModesCount (
  VOID
  );

CONST DISPLAY_MODE *
GetPredefinedDisplayMode (
  IN UINT32  Index
  );

CONST DISPLAY_MODE *
GetPredefinedDisplayModeByVic (
  IN UINT8  Vic
  );

CONST DISPLAY_MODE *
GetPredefinedDisplayModeByResolution (
  IN UINT32  HorizontalResolution,
  IN UINT32  VerticalResolution,
  IN UINT32  RefreshRate
  );

CONST DISPLAY_MODE *
MatchPredefinedDisplayMode (
  IN CONST DISPLAY_MODE  *DisplayMode,
  IN UINT32              ClockTolerance
  );

EFI_STATUS
EdidGetDisplaySinkInfo (
  IN CONNECTOR_STATE  *ConnectorState
  );

#endif /* LCD_GRAPHICS_OUTPUT_DXE_H_ */
