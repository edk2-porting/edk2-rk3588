/** @file
  This file implements the Graphics Output protocol for Arm platforms

  Copyright (c) 2011 - 2020, Arm Limited. All rights reserved.<BR>
  Copyright (c) 2022 Rockchip Electronics Co. Ltd.
  Copyright (c) 2023-2025, Mario Bălănică <mariobalanica02@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DrmModes.h>
#include <Library/MediaBusFormat.h>

#include <Guid/GlobalVariable.h>

#include "LcdGraphicsOutputDxe.h"

STATIC EFI_CPU_ARCH_PROTOCOL  *mCpu;

STATIC LCD_INSTANCE  mLcdTemplate = {
  LCD_INSTANCE_SIGNATURE,
  NULL,                                        // Handle
  {               // ModeInfo
    0,            // Version
    0,            // HorizontalResolution
    0,            // VerticalResolution
    PixelBltOnly, // PixelFormat
    { 0 },        // PixelInformation
    0,            // PixelsPerScanLine
  },
  {
    0,    // MaxMode;
    0,    // Mode;
    NULL, // Info;
    0,    // SizeOfInfo;
    0,    // FrameBufferBase;
    0     // FrameBufferSize;
  },
  {                       // Gop
    LcdGraphicsQueryMode, // QueryMode
    LcdGraphicsSetMode,   // SetMode
    LcdGraphicsBlt,       // Blt
    NULL                  // *Mode
  },
  { // DevicePath
    {
      {
        HARDWARE_DEVICE_PATH,                  HW_VENDOR_DP,
        {
          (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
          (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
        },
      },
      // Hardware Device Path for Lcd
      EFI_CALLER_ID_GUID // Use the driver's GUID
    },

    {
      END_DEVICE_PATH_TYPE,
      END_ENTIRE_DEVICE_PATH_SUBTYPE,
      {
        sizeof (EFI_DEVICE_PATH_PROTOCOL),
        0
      }
    }
  },
  { 0 },                                       // DisplayStateList
  NULL,                                        // DisplayModes
};

STATIC
UINT32
DisplayBppConvert (
  IN LCD_BPP  LcdBpp
  )
{
  UINT32  DisplayBpp;

  switch (LcdBpp) {
    case LcdBitsPerPixel_24:
      DisplayBpp = 32;
      break;
    default:
      DisplayBpp = 32;
      break;
  }

  return DisplayBpp;
}

STATIC
EFI_STATUS
DisplayPreInit (
  IN LCD_INSTANCE  *Instance
  )
{
  EFI_STATUS                   Status;
  DISPLAY_STATE                *StateInterate;
  ROCKCHIP_CRTC_PROTOCOL       *Crtc;
  ROCKCHIP_CONNECTOR_PROTOCOL  *Connector;

  LIST_FOR_EACH_ENTRY (StateInterate, &Instance->DisplayStateList, ListHead) {
    if (StateInterate->IsEnable) {
      CRTC_STATE       *CrtcState      = &StateInterate->CrtcState;
      CONNECTOR_STATE  *ConnectorState = &StateInterate->ConnectorState;
      Crtc      = (ROCKCHIP_CRTC_PROTOCOL *)StateInterate->CrtcState.Crtc;
      Connector = (ROCKCHIP_CONNECTOR_PROTOCOL *)StateInterate->ConnectorState.Connector;

      if (Connector && Connector->Preinit) {
        Status = Connector->Preinit (Connector, StateInterate);
      }

      Crtc->Vps[CrtcState->CrtcID].OutputType = ConnectorState->Type;

      if (Crtc && Crtc->Preinit) {
        Status = Crtc->Preinit (Crtc, StateInterate);
        if (EFI_ERROR (Status)) {
          goto EXIT;
        }
      }
    }
  }

EXIT:
  return Status;
}

STATIC
EFI_STATUS
EFIAPI
LcdGraphicsOutputInit (
  VOID
  )
{
  EFI_STATUS                         Status;
  LCD_INSTANCE                       *Instance = NULL;
  ROCKCHIP_CRTC_PROTOCOL             *Crtc;
  ROCKCHIP_CONNECTOR_PROTOCOL        *Connector;
  UINTN                              ConnectorCount;
  EFI_HANDLE                         *ConnectorHandles = NULL;
  UINTN                              Index;
  DISPLAY_STATE                      *DisplayState;
  DISPLAY_STATE                      *NextDisplayState;
  CRTC_STATE                         *CrtcState;
  CONNECTOR_STATE                    *ConnectorState;
  DISPLAY_MODE_PRESET_VARSTORE_DATA  *ModePreset;
  CONST DISPLAY_MODE                 *Mode;
  CONST DISPLAY_MODE                 *PreferredMode;

  Status = gBS->LocateProtocol (
                  &gRockchipCrtcProtocolGuid,
                  NULL,
                  (VOID **)&Crtc
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Failed to locate gRockchipCrtcProtocolGuid. Status=%r\n",
      __func__,
      Status
      ));
    return Status;
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gRockchipConnectorProtocolGuid,
                  NULL,
                  &ConnectorCount,
                  &ConnectorHandles
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Failed to locate gRockchipConnectorProtocolGuid. Status=%r\n",
      __func__,
      Status
      ));
    return Status;
  }

  Instance = AllocateCopyPool (sizeof (LCD_INSTANCE), &mLcdTemplate);
  if (Instance == NULL) {
    ASSERT (FALSE);
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  InitializeListHead (&Instance->DisplayStateList);

  ModePreset    = PcdGetPtr (PcdDisplayModePreset);
  PreferredMode = NULL;

  for (Index = 0; Index < ConnectorCount; Index++) {
    Status = gBS->HandleProtocol (
                    ConnectorHandles[Index],
                    &gRockchipConnectorProtocolGuid,
                    (VOID **)&Connector
                    );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      continue;
    }

    DisplayState = AllocateZeroPool (sizeof (DISPLAY_STATE));
    if (DisplayState == NULL) {
      ASSERT (FALSE);
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit;
    }

    InitializeListHead (&DisplayState->ListHead);

    CrtcState      = &DisplayState->CrtcState;
    ConnectorState = &DisplayState->ConnectorState;

    ConnectorState->Connector = (VOID *)Connector;

    if ((ModePreset->Preset == DISPLAY_MODE_NATIVE) && (PreferredMode == NULL)) {
      //
      // Get predefined native mode.
      //
      if (Connector->GetTiming != NULL) {
        Status = Connector->GetTiming (Connector, DisplayState);
        if (Status != EFI_UNSUPPORTED) {
          ASSERT_EFI_ERROR (Status);
        }
      } else {
        Status = EFI_UNSUPPORTED;
      }

      // if (EFI_ERROR (Status)) {
      //   TODO: Read EDID here.
      // }

      if (!EFI_ERROR (Status)) {
        PreferredMode = &ConnectorState->SinkInfo.PreferredMode;
      }
    }

    /* adapt to UEFI architecture */
    CrtcState->Crtc               = (VOID *)Crtc;
    CrtcState->CrtcID             = 2;
    DisplayState->VpsConfigModeID = 1;

    /* set MEDIA_BUS_FMT_RGB888_1X24 by default when using panel */
    /* move to panel driver later -- todo*/
    ConnectorState->BusFormat = MEDIA_BUS_FMT_RGB888_1X24;

    /* add BCSH data if needed --- todo */
    ConnectorState->DispInfo = NULL;

    Crtc->Vps[CrtcState->CrtcID].Enable = TRUE;
    DisplayState->IsEnable              = TRUE;

    InsertTailList (&Instance->DisplayStateList, &DisplayState->ListHead);
  }

  if (IsListEmpty (&Instance->DisplayStateList)) {
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  Status = DisplayPreInit (Instance);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: DisplayPreInit fail. Exit Status=%r\n",
      __func__,
      Status
      ));
    goto Exit;
  }

  Instance->Gop.Mode  = &Instance->Mode;
  Instance->Mode.Info = &Instance->ModeInfo;

  //
  // Only expose a single mode to GOP.
  //
  Instance->Gop.Mode->MaxMode = 1;

  Instance->Gop.Mode->Mode = MAX_UINT32;

  Instance->Mode.Info->PixelFormat = PixelBlueGreenRedReserved8BitPerColor;

  Instance->DisplayModes = AllocateZeroPool (
                             sizeof (DISPLAY_MODE) *
                             Instance->Gop.Mode->MaxMode
                             );
  if (Instance->DisplayModes == NULL) {
    ASSERT (FALSE);
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Mode = NULL;

  if (ModePreset->Preset == DISPLAY_MODE_NATIVE) {
    Mode = PreferredMode;
  } else if (ModePreset->Preset == DISPLAY_MODE_CUSTOM) {
    Mode = PcdGetPtr (PcdDisplayModeCustom);
    ASSERT (Mode != NULL);
    if (Mode != NULL) {
      if ((Mode->OscFreq < VOP_PIXEL_CLOCK_MIN) ||
          (Mode->OscFreq > VOP_PIXEL_CLOCK_MAX) ||
          (Mode->HActive < VOP_HORIZONTAL_RES_MIN) ||
          (Mode->HActive > VOP_HORIZONTAL_RES_MAX) ||
          (Mode->VActive < VOP_VERTICAL_RES_MIN) ||
          (Mode->VActive > VOP_VERTICAL_RES_MAX))
      {
        Mode = NULL;
        DEBUG ((
          DEBUG_ERROR,
          "%a: Custom mode unsupported! (OscFreq=%u, HActive=%u, VActive=%u)\n",
          __func__,
          Mode->OscFreq,
          Mode->HActive,
          Mode->VActive
          ));
        ASSERT (FALSE);
      }
    }
  } else if (ModePreset->Preset < GetPredefinedDisplayModesCount ()) {
    Mode = GetPredefinedDisplayMode (ModePreset->Preset);
  } else {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Invalid preferred mode: %u\n",
      __func__,
      ModePreset->Preset
      ));
    ASSERT (FALSE);
  }

  if (Mode == NULL) {
    DEBUG ((
      DEBUG_WARN,
      "%a: No usable mode found! Falling back to lowest available.\n",
      __func__
      ));
    Mode = GetPredefinedDisplayMode (0);
  }

  CopyMem (&Instance->DisplayModes[0], Mode, sizeof (*Mode));

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Instance->Handle,
                  &gEfiGraphicsOutputProtocolGuid,
                  &Instance->Gop,
                  &gEfiDevicePathProtocolGuid,
                  &Instance->DevicePath,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Failed to install GOP. Status=%r\n",
      __func__,
      Status
      ));
    goto Exit;
  }

Exit:
  if (EFI_ERROR (Status)) {
    if (Instance->Handle != NULL) {
      gBS->UninstallMultipleProtocolInterfaces (
             Instance->Handle,
             &gEfiGraphicsOutputProtocolGuid,
             &Instance->Gop,
             &gEfiDevicePathProtocolGuid,
             &Instance->DevicePath,
             NULL
             );
    }

    if (Instance->DisplayModes != NULL) {
      FreePool (Instance->DisplayModes);
    }

    LIST_FOR_EACH_ENTRY_SAFE (
      DisplayState,
      NextDisplayState,
      &Instance->DisplayStateList,
      ListHead
      ) {
      FreePool (DisplayState);
    }

    if (Instance != NULL) {
      FreePool (Instance);
    }
  }

  if (ConnectorHandles != NULL) {
    FreePool (ConnectorHandles);
  }

  return Status;
}

VOID
EFIAPI
LcdGraphicsOutputEndOfDxeEventHandler (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  gBS->CloseEvent (Event);

  LcdGraphicsOutputInit ();
}

EFI_STATUS
EFIAPI
LcdGraphicsOutputDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   EndOfDxeEvent;

  Status = gBS->LocateProtocol (
                  &gEfiCpuArchProtocolGuid,
                  NULL,
                  (VOID **)&mCpu
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  LcdGraphicsOutputEndOfDxeEventHandler,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

EFI_STATUS
EFIAPI
LcdGraphicsQueryMode (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL           *This,
  IN UINT32                                 ModeNumber,
  OUT UINTN                                 *SizeOfInfo,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  **Info
  )
{
  LCD_INSTANCE  *Instance;
  DISPLAY_MODE  *Mode;

  if ((This == NULL) ||
      (Info == NULL) ||
      (SizeOfInfo == NULL) ||
      (ModeNumber >= This->Mode->MaxMode))
  {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Invalid parameter! (ModeNumber=%d)\n",
      __func__,
      ModeNumber
      ));
    return EFI_INVALID_PARAMETER;
  }

  *Info = AllocateZeroPool (sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
  if (*Info == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  Instance = LCD_INSTANCE_FROM_GOP_THIS (This);
  Mode     = &Instance->DisplayModes[ModeNumber];

  *SizeOfInfo = sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);

  (*Info)->Version              = 0;
  (*Info)->HorizontalResolution = Mode->HActive;
  (*Info)->VerticalResolution   = Mode->VActive;
  (*Info)->PixelsPerScanLine    = Mode->HActive;
  (*Info)->PixelFormat          = This->Mode->Info->PixelFormat;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
LcdGraphicsSetMode (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *This,
  IN UINT32                        ModeNumber
  )
{
  EFI_STATUS                     Status;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL  FillColour;
  LCD_INSTANCE                   *Instance;
  LCD_BPP                        Bpp;
  EFI_PHYSICAL_ADDRESS           VramBaseAddress;
  UINTN                          VramSize;
  UINTN                          NumVramPages;
  UINTN                          NumPreviousVramPages;
  DISPLAY_MODE                   *Mode;
  DRM_DISPLAY_MODE               *DrmMode;
  DISPLAY_STATE                  *StateInterate;
  ROCKCHIP_CRTC_PROTOCOL         *Crtc;
  CRTC_STATE                     *CrtcState;
  ROCKCHIP_CONNECTOR_PROTOCOL    *Connector;
  CONNECTOR_STATE                *ConnectorState;

  Instance = LCD_INSTANCE_FROM_GOP_THIS (This);

  if (ModeNumber >= This->Mode->MaxMode) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Unsupported mode number %d\n",
      __func__,
      ModeNumber
      ));
    Status = EFI_UNSUPPORTED;
    goto EXIT;
  }

  if (ModeNumber == This->Mode->Mode) {
    Status = EFI_SUCCESS;
    goto EXIT;
  }

  Mode = &Instance->DisplayModes[ModeNumber];

  Status = LcdGraphicsGetBpp (ModeNumber, &Bpp);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Couldn't get bytes per pixel, status: %r\n",
      __func__,
      Status
      ));
    goto EXIT;
  }

  VramBaseAddress = This->Mode->FrameBufferBase;

  VramSize = Mode->HActive * Mode->VActive * GetBytesPerPixel (Bpp);

  NumVramPages         = EFI_SIZE_TO_PAGES (VramSize);
  NumPreviousVramPages = EFI_SIZE_TO_PAGES (This->Mode->FrameBufferSize);

  if (NumPreviousVramPages < NumVramPages) {
    if (NumPreviousVramPages != 0) {
      gBS->FreePages (VramBaseAddress, NumPreviousVramPages);
      This->Mode->FrameBufferSize = 0;
    }

    VramBaseAddress = SIZE_4GB - 1; // VOP2 only supports 32-bit addresses
    Status          = gBS->AllocatePages (
                             AllocateMaxAddress,
                             EfiRuntimeServicesData,
                             NumVramPages,
                             &VramBaseAddress
                             );
    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "%a: Could not allocate %u pages for mode %u: %r\n",
        __func__,
        NumVramPages,
        ModeNumber,
        Status
        ));
      return Status;
    }

    Status = mCpu->SetMemoryAttributes (
                     mCpu,
                     VramBaseAddress,
                     ALIGN_VALUE (VramSize, EFI_PAGE_SIZE),
                     EFI_MEMORY_WC
                     );
    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "%a: Couldn't set framebuffer attributes: %r\n",
        __func__,
        Status
        ));
      goto EXIT;
    }
  }

  // Update the UEFI mode information
  This->Mode->Mode = ModeNumber;

  This->Mode->Info->Version              = 0;
  This->Mode->Info->HorizontalResolution = Mode->HActive;
  This->Mode->Info->VerticalResolution   = Mode->VActive;
  This->Mode->Info->PixelsPerScanLine    = Mode->HActive;
  Instance->Mode.SizeOfInfo              = sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);

  This->Mode->FrameBufferBase = VramBaseAddress;
  This->Mode->FrameBufferSize = VramSize;

  // The UEFI spec requires that we now clear the visible portions of the
  // output display to black.

  // Set the fill colour to black
  SetMem (&FillColour, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0x0);

  // Fill the entire visible area with the same colour.
  Status = This->Blt (
                   This,
                   &FillColour,
                   EfiBltVideoFill,
                   0,
                   0,
                   0,
                   0,
                   This->Mode->Info->HorizontalResolution,
                   This->Mode->Info->VerticalResolution,
                   0
                   );

  LIST_FOR_EACH_ENTRY (StateInterate, &Instance->DisplayStateList, ListHead) {
    if (StateInterate->IsEnable) {
      Crtc           = (ROCKCHIP_CRTC_PROTOCOL *)StateInterate->CrtcState.Crtc;
      Connector      = (ROCKCHIP_CONNECTOR_PROTOCOL *)StateInterate->ConnectorState.Connector;
      CrtcState      = &StateInterate->CrtcState;
      ConnectorState = &StateInterate->ConnectorState;
      DrmMode        = &StateInterate->ConnectorState.DisplayMode;

      if (Connector && Connector->Init) {
        Status = Connector->Init (Connector, StateInterate);
      }

      DisplayModeToDrm (Mode, DrmMode);
      ConnectorState->DisplayModeVic = Mode->Vic;

      DEBUG ((
        DEBUG_INFO,
        "%a: detailed mode clock %u kHz, flags[%x]\n"
        "          H: %04d %04d %04d %04d\n"
        "          V: %04d %04d %04d %04d\n"
        "      bus_format: %x\n",
        __func__,
        DrmMode->Clock,
        DrmMode->Flags,
        DrmMode->HDisplay,
        DrmMode->HSyncStart,
        DrmMode->HSyncEnd,
        DrmMode->HTotal,
        DrmMode->VDisplay,
        DrmMode->VSyncStart,
        DrmMode->VSyncEnd,
        DrmMode->VTotal,
        ConnectorState->BusFormat
        ));

      Status = DisplaySetCrtcInfo (DrmMode, CRTC_INTERLACE_HALVE_V);
      if (EFI_ERROR (Status)) {
        goto EXIT;
      }

      if (Crtc && Crtc->Init) {
        Status = Crtc->Init (Crtc, StateInterate);
        if (EFI_ERROR (Status)) {
          goto EXIT;
        }
      }

      /* adapt to uefi display architecture */
      CrtcState->Format  = ROCKCHIP_FMT_ARGB8888;
      CrtcState->SrcW    = ConnectorState->DisplayMode.HDisplay;
      CrtcState->SrcH    = ConnectorState->DisplayMode.VDisplay;
      CrtcState->SrcX    = 0;
      CrtcState->SrcY    = 0;
      CrtcState->CrtcW   = ConnectorState->DisplayMode.HDisplay;
      CrtcState->CrtcH   = ConnectorState->DisplayMode.VDisplay;
      CrtcState->CrtcX   = 0;
      CrtcState->CrtcY   = 0;
      CrtcState->YMirror = 0;
      CrtcState->RBSwap  = 0;

      CrtcState->XVirtual   = ALIGN (CrtcState->SrcW * DisplayBppConvert (Bpp), 32) >> 5;
      CrtcState->DMAAddress = (UINT32)VramBaseAddress;

      if (Crtc && Crtc->SetPlane) {
        Crtc->SetPlane (Crtc, StateInterate);
      }

      if (Crtc && Crtc->Enable) {
        Crtc->Enable (Crtc, StateInterate);
      }

      if (Connector && Connector->Enable) {
        Connector->Enable (Connector, StateInterate);
      }
    }
  }

EXIT:
  return Status;
}

EFI_STATUS
EFIAPI
LcdGraphicsGetBpp (
  IN  UINT32   ModeNumber,
  OUT LCD_BPP  *Bpp
  )
{
  *Bpp = LcdBitsPerPixel_24;

  return EFI_SUCCESS;
}

UINTN
GetBytesPerPixel (
  IN  LCD_BPP  Bpp
  )
{
  switch (Bpp) {
    case LcdBitsPerPixel_24:
      return 4;

    case LcdBitsPerPixel_16_565:
    case LcdBitsPerPixel_16_555:
    case LcdBitsPerPixel_12_444:
      return 2;

    case LcdBitsPerPixel_8:
    case LcdBitsPerPixel_4:
    case LcdBitsPerPixel_2:
    case LcdBitsPerPixel_1:
      return 1;

    default:
      return 0;
  }
}
