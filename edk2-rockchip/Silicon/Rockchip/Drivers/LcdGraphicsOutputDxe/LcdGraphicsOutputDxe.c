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
  { 0 },                                       // DisplayStates
  0,                                           // DisplayStatesCount
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
PrepareDisplays (
  IN LCD_INSTANCE  *Instance
  )
{
  EFI_STATUS                                 Status;
  ROCKCHIP_CRTC_PROTOCOL                     *Crtc;
  UINTN                                      ConnectorCount;
  EFI_HANDLE                                 *ConnectorHandles = NULL;
  DISPLAY_CONNECTORS_PRIORITY_VARSTORE_DATA  *ConnectorsPriority;
  ROCKCHIP_CONNECTOR_PROTOCOL                *Connector;
  UINTN                                      ConnectorIndex;
  UINTN                                      Index;
  DISPLAY_STATE                              *DisplayState;
  CONNECTOR_STATE                            *ConnectorState;
  CRTC_STATE                                 *CrtcState;

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

  ConnectorsPriority = PcdGetPtr (PcdDisplayConnectorsPriority);
  if (ConnectorsPriority == NULL) {
    ASSERT (FALSE);
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Instance->DisplayStatesCount = PcdGetSize (PcdDisplayConnectors) / sizeof (UINT32);
  ASSERT (Instance->DisplayStatesCount <= VOP_OUTPUT_IF_NUMS);

  for (ConnectorIndex = 0; ConnectorIndex < ConnectorCount; ConnectorIndex++) {
    Status = gBS->HandleProtocol (
                    ConnectorHandles[ConnectorIndex],
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

    ConnectorState = &DisplayState->ConnectorState;
    CrtcState      = &DisplayState->CrtcState;

    ConnectorState->Connector = (VOID *)Connector;
    CrtcState->Crtc           = (VOID *)Crtc;

    if (Connector->Preinit != NULL) {
      Status = Connector->Preinit (Connector, DisplayState);
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        goto DiscardState;
      }
    }

    if (Connector->Init != NULL) {
      Status = Connector->Init (Connector, DisplayState);
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        goto DiscardState;
      }
    } else if (Connector->Preinit == NULL) {
      ASSERT (FALSE);
      goto DiscardState;
    }

    for (Index = 0; Index < Instance->DisplayStatesCount; Index++) {
      if (ConnectorsPriority->Order[Index] == ConnectorState->OutputInterface) {
        Instance->DisplayStates[Index] = DisplayState;
        break;
      }
    }

    if (Index == Instance->DisplayStatesCount) {
      ASSERT (FALSE);
      goto DiscardState;
    }

    continue;

DiscardState:
    FreePool (DisplayState);
  }

  Status = EFI_SUCCESS;

Exit:
  if (EFI_ERROR (Status)) {
    for (Index = 0; Index < Instance->DisplayStatesCount; Index++) {
      if (Instance->DisplayStates[Index] != NULL) {
        FreePool (Instance->DisplayStates[Index]);
        Instance->DisplayStates[Index] = NULL;
      }
    }
  }

  if (ConnectorHandles != NULL) {
    FreePool (ConnectorHandles);
  }

  return Status;
}

STATIC
EFI_STATUS
DetectDisplays (
  IN  LCD_INSTANCE   *Instance,
  IN  BOOLEAN        ForceDetect,
  IN  BOOLEAN        DetectAll,
  OUT DISPLAY_STATE  **PrimaryDisplayState
  )
{
  EFI_STATUS                   Status;
  UINTN                        Index;
  UINTN                        NewCount;
  DISPLAY_STATE                *DisplayState;
  CONNECTOR_STATE              *ConnectorState;
  ROCKCHIP_CONNECTOR_PROTOCOL  *Connector;

  for (Index = 0, NewCount = 0; Index < Instance->DisplayStatesCount; Index++) {
    DisplayState = Instance->DisplayStates[Index];
    if (DisplayState == NULL) {
      continue;
    }

    ConnectorState = &DisplayState->ConnectorState;
    Connector      = (ROCKCHIP_CONNECTOR_PROTOCOL *)ConnectorState->Connector;

    if (Connector->Detect != NULL) {
      Status = Connector->Detect (Connector, DisplayState);
    } else {
      Status = EFI_SUCCESS; // Assume always connected.
    }

    DEBUG ((
      DEBUG_INFO,
      "%a: %a status: %r\n",
      __func__,
      GetVopOutputIfName (ConnectorState->OutputInterface),
      Status
      ));

    if (!EFI_ERROR (Status)) {
      if (*PrimaryDisplayState == NULL) {
        *PrimaryDisplayState = DisplayState;
      }
    } else if (!ForceDetect) {
      FreePool (DisplayState);
      Instance->DisplayStates[Index] = NULL;
      continue;
    }

    Instance->DisplayStates[NewCount++] = DisplayState;

    if ((*PrimaryDisplayState != NULL) && !DetectAll) {
      break;
    }
  }

  Instance->DisplayStatesCount = NewCount;

  if (Instance->DisplayStatesCount == 0) {
    DEBUG ((DEBUG_ERROR, "%a: No displays found!\n", __func__));
    return EFI_NOT_FOUND;
  }

  if (*PrimaryDisplayState != NULL) {
    DEBUG ((
      DEBUG_INFO,
      "%a: Primary: %a\n",
      __func__,
      GetVopOutputIfName ((*PrimaryDisplayState)->ConnectorState.OutputInterface)
      ));
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
IdentifyDisplay (
  IN DISPLAY_STATE  *DisplayState
  )
{
  EFI_STATUS                   Status;
  CONNECTOR_STATE              *ConnectorState;
  ROCKCHIP_CONNECTOR_PROTOCOL  *Connector;

  ConnectorState = &DisplayState->ConnectorState;
  Connector      = (ROCKCHIP_CONNECTOR_PROTOCOL *)ConnectorState->Connector;

  Status = EFI_UNSUPPORTED;

  //
  // Get predefined native mode.
  //
  if (Connector->GetTiming != NULL) {
    Status = Connector->GetTiming (Connector, DisplayState);
    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "%a: Failed to get predefined native mode. Status=%r\n",
        __func__,
        Status
        ));
    }

    return Status;
  }

  //
  // TODO: Read EDID here.
  //

  return Status;
}

STATIC
EFI_STATUS
SetupDisplay (
  IN DISPLAY_STATE  *DisplayState
  )
{
  EFI_STATUS              Status;
  CRTC_STATE              *CrtcState;
  ROCKCHIP_CRTC_PROTOCOL  *Crtc;
  CONNECTOR_STATE         *ConnectorState;

  CrtcState      = &DisplayState->CrtcState;
  Crtc           = (ROCKCHIP_CRTC_PROTOCOL *)CrtcState->Crtc;
  ConnectorState = &DisplayState->ConnectorState;

  //
  // Only support CRTC port 2.
  //
  CrtcState->CrtcID             = 2;
  DisplayState->VpsConfigModeID = 1;

  //
  // Use RGB888_1X24 by default as it's the most compatible.
  // The connector could override this, but it may break other
  // sinks if the output is shared between multiple connectors.
  //
  ConnectorState->BusFormat = MEDIA_BUS_FMT_RGB888_1X24;

  Crtc->Vps[CrtcState->CrtcID].Enable     = TRUE;
  Crtc->Vps[CrtcState->CrtcID].OutputType = ConnectorState->Type;

  if (Crtc->Preinit != NULL) {
    Status = Crtc->Preinit (Crtc, DisplayState);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  DisplayState->IsEnable = TRUE;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
SetupAllDisplays (
  IN LCD_INSTANCE   *Instance,
  IN DISPLAY_STATE  *PrimaryDisplayState
  )
{
  EFI_STATUS     Status;
  UINTN          Index;
  DISPLAY_STATE  *DisplayState;

  for (Index = 0; Index < Instance->DisplayStatesCount; Index++) {
    DisplayState = Instance->DisplayStates[Index];
    if ((DisplayState == NULL) || DisplayState->IsEnable) {
      continue;
    }

    if (PrimaryDisplayState != NULL) {
      //
      // Clone primary display sink info.
      // This is a best effort to support multiple outputs on
      // a single CRTC port. All sinks are assumed to have more
      // or less the same capabilities.
      //
      CopyMem (
        &DisplayState->ConnectorState.SinkInfo,
        &PrimaryDisplayState->ConnectorState.SinkInfo,
        sizeof (DisplayState->ConnectorState.SinkInfo)
        );
    }

    Status = SetupDisplay (DisplayState);
    if (EFI_ERROR (Status)) {
      continue;
    }
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
GetSupportedDisplayModes (
  IN LCD_INSTANCE   *Instance,
  IN DISPLAY_STATE  *PrimaryDisplayState
  )
{
  CONST DISPLAY_MODE                 *Mode;
  DISPLAY_MODE_PRESET_VARSTORE_DATA  *ModePreset;
  DISPLAY_SINK_INFO                  *SinkInfo;

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
    return EFI_OUT_OF_RESOURCES;
  }

  Mode = NULL;

  ModePreset = PcdGetPtr (PcdDisplayModePreset);
  ASSERT (ModePreset != NULL);

  if (ModePreset != NULL) {
    if (ModePreset->Preset == DISPLAY_MODE_NATIVE) {
      if (PrimaryDisplayState != NULL) {
        SinkInfo = &PrimaryDisplayState->ConnectorState.SinkInfo;
        if (SinkInfo->PreferredMode.OscFreq != 0) {
          Mode = &SinkInfo->PreferredMode;
        }
      }
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

  return EFI_SUCCESS;
}

STATIC
VOID
LcdGraphicsOutputDestroy (
  IN LCD_INSTANCE  *Instance
  )
{
  UINTN  Index;

  if (Instance == NULL) {
    return;
  }

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

  for (Index = 0; Index < Instance->DisplayStatesCount; Index++) {
    if (Instance->DisplayStates[Index] != NULL) {
      FreePool (Instance->DisplayStates[Index]);
    }
  }

  FreePool (Instance);
}

STATIC
EFI_STATUS
EFIAPI
LcdGraphicsOutputInit (
  VOID
  )
{
  EFI_STATUS     Status;
  LCD_INSTANCE   *Instance = NULL;
  DISPLAY_STATE  *DisplayState;
  DISPLAY_STATE  *PrimaryDisplayState;
  BOOLEAN        ForceOutput;
  BOOLEAN        DuplicateOutput;

  Instance = AllocateCopyPool (sizeof (LCD_INSTANCE), &mLcdTemplate);
  if (Instance == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  Status = PrepareDisplays (Instance);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  ForceOutput     = PcdGetBool (PcdDisplayForceOutput);
  DuplicateOutput = PcdGetBool (PcdDisplayDuplicateOutput);

  PrimaryDisplayState = NULL;

  Status = DetectDisplays (
             Instance,
             ForceOutput,
             DuplicateOutput,
             &PrimaryDisplayState
             );
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  DisplayState = NULL;

  if (PrimaryDisplayState != NULL) {
    DisplayState = PrimaryDisplayState;

    IdentifyDisplay (PrimaryDisplayState);
  } else if (ForceOutput) {
    DisplayState = Instance->DisplayStates[0];

    DuplicateOutput = TRUE;
  }

  if (DisplayState == NULL) {
    ASSERT (FALSE);
    goto Exit;
  }

  Status = SetupDisplay (DisplayState);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  if (DuplicateOutput) {
    SetupAllDisplays (Instance, PrimaryDisplayState);
  }

  Instance->Gop.Mode  = &Instance->Mode;
  Instance->Mode.Info = &Instance->ModeInfo;

  Status = GetSupportedDisplayModes (Instance, PrimaryDisplayState);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

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
    LcdGraphicsOutputDestroy (Instance);
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
  DISPLAY_STATE                  *DisplayState;
  ROCKCHIP_CRTC_PROTOCOL         *Crtc;
  CRTC_STATE                     *CrtcState;
  ROCKCHIP_CONNECTOR_PROTOCOL    *Connector;
  CONNECTOR_STATE                *ConnectorState;
  UINTN                          Index;

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

  for (Index = 0; Index < Instance->DisplayStatesCount; Index++) {
    DisplayState = Instance->DisplayStates[Index];
    if ((DisplayState == NULL) || !DisplayState->IsEnable) {
      continue;
    }

    Crtc           = (ROCKCHIP_CRTC_PROTOCOL *)DisplayState->CrtcState.Crtc;
    Connector      = (ROCKCHIP_CONNECTOR_PROTOCOL *)DisplayState->ConnectorState.Connector;
    CrtcState      = &DisplayState->CrtcState;
    ConnectorState = &DisplayState->ConnectorState;
    DrmMode        = &DisplayState->ConnectorState.DisplayMode;

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

    if (Crtc->Init != NULL) {
      Status = Crtc->Init (Crtc, DisplayState);
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

    if (Crtc->SetPlane != NULL) {
      Crtc->SetPlane (Crtc, DisplayState);
    }

    if (Crtc->Enable != NULL) {
      Crtc->Enable (Crtc, DisplayState);
    }

    if (Connector->Enable != NULL) {
      Connector->Enable (Connector, DisplayState);
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
