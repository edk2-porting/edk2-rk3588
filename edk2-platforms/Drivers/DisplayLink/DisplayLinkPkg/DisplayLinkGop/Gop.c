/**
 * @file Gop.c
 * @brief UEFI GOP protocol API implementation for USB DisplayLink driver.
 *
 * Copyright (c) 2018-2019, DisplayLink (UK) Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
**/

#include "UsbDisplayLink.h"
#include "Edid.h"


/**
 *
 * @param This            Pointer to the instance of the GOP protocol
 * @param BltOperation
 * @param SourceX
 * @param SourceY
 * @param Width
 * @param Height
 * @param DestinationX
 * @param DestinationY
 * @return
 */
STATIC EFI_STATUS
CheckBounds (
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL* This,
    IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation,
    IN UINTN SourceX,
    IN UINTN SourceY,
    IN UINTN Width,
    IN UINTN Height,
    IN UINTN DestinationX,
    IN UINTN DestinationY
    )
{
  USB_DISPLAYLINK_DEV *UsbDisplayLinkDev;
  EFI_GRAPHICS_OUTPUT_PROTOCOL* Gop;

  UsbDisplayLinkDev = USB_DISPLAYLINK_DEV_FROM_GRAPHICS_OUTPUT_PROTOCOL(This);
  Gop = &UsbDisplayLinkDev->GraphicsOutputProtocol;

  CONST EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* ScreenMode = Gop->Mode->Info;

  if (BltOperation == EfiBltVideoToBltBuffer || BltOperation == EfiBltVideoToVideo) {
    if (SourceY + Height > ScreenMode->VerticalResolution) {
      return EFI_INVALID_PARAMETER;
    }

    if (SourceX + Width > ScreenMode->HorizontalResolution) {
      return EFI_INVALID_PARAMETER;
    }
  }

  if (BltOperation == EfiBltBufferToVideo
    || BltOperation == EfiBltVideoToVideo
    || BltOperation == EfiBltVideoFill) {
    if (DestinationY + Height > ScreenMode->VerticalResolution) {
      return EFI_INVALID_PARAMETER;
    }

    if (DestinationX + Width > ScreenMode->HorizontalResolution) {
      return EFI_INVALID_PARAMETER;
    }
  }

  return EFI_SUCCESS;
}

/**
 * Update the local copy of the Frame Buffer. This local copy is periodically transmitted to the
 * DisplayLink device (via DlGopSendScreenUpdate)
 * @param UsbDisplayLinkDev
 * @param BltBuffer
 * @param BltOperation
 * @param SourceX
 * @param SourceY
 * @param DestinationX
 * @param DestinationY
 * @param Width
 * @param Height
 * @param BltBufferStride
 * @param PixelsPerScanLine
 */
STATIC VOID
BuildBackBuffer (
  IN  USB_DISPLAYLINK_DEV                     *UsbDisplayLinkDev,
  IN  EFI_GRAPHICS_OUTPUT_BLT_PIXEL           *BltBuffer, OPTIONAL
  IN  EFI_GRAPHICS_OUTPUT_BLT_OPERATION       BltOperation,
  IN  UINTN                                   SourceX,
  IN  UINTN                                   SourceY,
  IN  UINTN                                   DestinationX,
  IN  UINTN                                   DestinationY,
  IN  UINTN                                   Width,
  IN  UINTN                                   Height,
  IN  UINTN                                   BltBufferStride,
  IN  UINTN                                   PixelsPerScanLine
)
{
  UINTN H;
  UINTN W;
  switch (BltOperation) {
  case EfiBltVideoToBltBuffer:
  {
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL* Blt;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL* SrcB;
    Blt = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)((UINT8 *)BltBuffer + (DestinationY * BltBufferStride) + DestinationX * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
    SrcB = UsbDisplayLinkDev->Screen + SourceY * PixelsPerScanLine + SourceX;

    for (H = 0; H < Height; H++) {
      for (W = 0; W < Width; W++) {
        Blt[W] = *SrcB++;
      }
      Blt = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)(((UINT8*)Blt) + BltBufferStride);
      SrcB += PixelsPerScanLine - Width;
    }
  }
  break;

  case EfiBltBufferToVideo:
  {
    // Update the store of the area of the screen that is "dirty" - that we need to send in the next screen update.
    if (DestinationY < UsbDisplayLinkDev->LastY1) {
      UsbDisplayLinkDev->LastY1 = DestinationY;
    }
    if ((DestinationY + Height) > UsbDisplayLinkDev->LastY2) {
      UsbDisplayLinkDev->LastY2 = DestinationY + Height;
    }

    EFI_GRAPHICS_OUTPUT_BLT_PIXEL* Blt;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL* DstB;
    Blt = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)(((UINT8 *)BltBuffer) + (SourceY * BltBufferStride) + SourceX * sizeof *Blt);
    DstB = UsbDisplayLinkDev->Screen + DestinationY * PixelsPerScanLine + DestinationX;

    for (H = 0; H < Height; H++) {
      for (W = 0; W < Width; W++) {
        *DstB++ = Blt[W];
      }
      Blt = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)(((UINT8*)Blt) + BltBufferStride);
      DstB += PixelsPerScanLine - Width;
    }
  }
  break;

  case EfiBltVideoToVideo:
  {
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL* SrcB;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL* DstB;
    SrcB = UsbDisplayLinkDev->Screen + SourceY * PixelsPerScanLine + SourceX;
    DstB = UsbDisplayLinkDev->Screen + DestinationY * PixelsPerScanLine + DestinationX;

    for (H = 0; H < Height; H++) {
      for (W = 0; W < Width; W++) {
        *DstB++ = *SrcB++;
      }
      SrcB += PixelsPerScanLine - Width;
      DstB += PixelsPerScanLine - Width;
    }
  }
  break;

  case EfiBltVideoFill:
  {
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL* DstB;
    DstB = UsbDisplayLinkDev->Screen + DestinationY * PixelsPerScanLine + DestinationX;
    for (H = 0; H < Height; H++) {
      for (W = 0; W < Width; W++) {
        *DstB++ = *BltBuffer;
      }
      DstB += PixelsPerScanLine - Width;
    }
  }
  break;
  default: break;
  }
}

/**
 * Display a colour bar pattern on the DisplayLink device.
 * @param UsbDisplayLinkDev
 * @param PatternNumber
 * @return
 */
EFI_STATUS
DlGopSendTestPattern (
    IN USB_DISPLAYLINK_DEV* UsbDisplayLinkDev,
    IN UINTN PatternNumber)
{
  EFI_STATUS Status;
  UINTN DataLen;
  UINT8 *DstBuf;
  UINT32 USBStatus;

  Status = EFI_SUCCESS;
  DataLen = UsbDisplayLinkDev->GraphicsOutputProtocol.Mode->Info->HorizontalResolution * 3; // Send 1 line @ 24 bits per pixel
  DstBuf = AllocateZeroPool (DataLen);

  if (DstBuf == NULL) {
    DEBUG ((DEBUG_ERROR, "SendTestPattern Failed to allocate memory\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  //DEBUG ((DEBUG_ERROR, "Called DlGopSendTestPattern %d\n", PatternNumber));

  CONST UINT8 RedPixel[3] = { 0xFF, 0x00, 0x00 };
  CONST UINT8 GreenPixel[3] = { 0x00, 0xFF, 0x00 };
  CONST UINT8 BluePixel[3] = { 0x00, 0x00, 0xFF };
  CONST UINT8 YellowPixel[3] = { 0xFF, 0xFF, 0x00 };
  CONST UINT8 MagentaPixel[3] = { 0xFF, 0x00, 0xFF };
  CONST UINT8 CyanPixel[3] = { 0x00, 0xFF, 0xFF };

  UINTN Row;
  UINTN Column;
  for (Row = 0; Row < UsbDisplayLinkDev->GraphicsOutputProtocol.Mode->Info->VerticalResolution; Row++) {
    for (Column = 0; Column < UsbDisplayLinkDev->GraphicsOutputProtocol.Mode->Info->HorizontalResolution; Column++) {

      if (0 == PatternNumber) {
        if (Row < UsbDisplayLinkDev->GraphicsOutputProtocol.Mode->Info->VerticalResolution / 3) {
          CopyMem (&DstBuf[Column * 3], RedPixel, sizeof (RedPixel));
        }
        else if (Row < (UsbDisplayLinkDev->GraphicsOutputProtocol.Mode->Info->VerticalResolution * 2) / 3)
        {
          CopyMem (&DstBuf[Column * 3], GreenPixel, sizeof (GreenPixel));
        }
        else {
          CopyMem (&DstBuf[Column * 3], BluePixel, sizeof (BluePixel));
        }
      }
      else {
        if (Column < UsbDisplayLinkDev->GraphicsOutputProtocol.Mode->Info->HorizontalResolution / 3) {
          CopyMem (&DstBuf[Column * 3], YellowPixel, sizeof (RedPixel));
        }
        else if (Column < (UsbDisplayLinkDev->GraphicsOutputProtocol.Mode->Info->HorizontalResolution * 2) / 3)
        {
          CopyMem (&DstBuf[Column * 3], MagentaPixel, sizeof (GreenPixel));
        }
        else {
          CopyMem (&DstBuf[Column * 3], CyanPixel, sizeof (BluePixel));
        }
      }
    }
    DlUsbBulkWrite (UsbDisplayLinkDev, DstBuf, DataLen, &USBStatus);
  }
  // Payload with length of 1 to terminate the frame
  DlUsbBulkWrite (UsbDisplayLinkDev, DstBuf, 1, &USBStatus);
  FreePool (DstBuf);

  return Status;
}


/**
 * Transfer the latest copy of the Blt buffer over USB to the DisplayLink device
 * @param UsbDisplayLinkDev
 * @return
 */
EFI_STATUS
DlGopSendScreenUpdate (
    IN USB_DISPLAYLINK_DEV* UsbDisplayLinkDev
    )
{
  EFI_STATUS Status;
  UINT32 USBStatus;
  Status = EFI_SUCCESS;

  // If it has been a while since we sent an update, send a full screen.
  // This allows us to update a hot-plugged monitor quickly.
  if (UsbDisplayLinkDev->TimeSinceLastScreenUpdate > DISPLAYLINK_FULL_SCREEN_UPDATE_PERIOD) {
    UsbDisplayLinkDev->LastY1 = 0;
    UsbDisplayLinkDev->LastY2 = UsbDisplayLinkDev->GraphicsOutputProtocol.Mode->Info->HorizontalResolution - 1;
  }

  // If there has been no BLT since the last update/poll, drop out quietly.
  if (UsbDisplayLinkDev->LastY2 < UsbDisplayLinkDev->LastY1) {
    UsbDisplayLinkDev->TimeSinceLastScreenUpdate += (DISPLAYLINK_SCREEN_UPDATE_TIMER_PERIOD / 1000);  // Convert us to ms
    return EFI_SUCCESS;
  }

  UsbDisplayLinkDev->TimeSinceLastScreenUpdate = 0;

  EFI_TPL OriginalTPL = gBS->RaiseTPL (TPL_NOTIFY);

  UINTN DataLen;
  UINTN Width;
  UINTN Height;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL* SrcPtr;
  UINT8* DstPtr;
  UINT8 DstBuffer[1920 * 3]; // Get rid of the magic numbers at some point
                             // Could also use a buffer allocated at runtime to store the line, stored in the USB_DISPLAYLINK_DEV structure.
  UINTN H;

  DataLen = UsbDisplayLinkDev->GraphicsOutputProtocol.Mode->Info->HorizontalResolution * 3; // Send 1 line @ 24 bits per pixel
  Width = UsbDisplayLinkDev->GraphicsOutputProtocol.Mode->Info->HorizontalResolution;
  Height = UsbDisplayLinkDev->GraphicsOutputProtocol.Mode->Info->VerticalResolution;
  SrcPtr = UsbDisplayLinkDev->Screen;
  DstPtr = DstBuffer;

  for (H = 0; H < Height; H++) {
    DstPtr = DstBuffer;

    UINTN W;
    for (W = 0; W < Width; W++) {
      // Need to swap round the RGB values
      DstPtr[0] = ((UINT8 *)SrcPtr)[2];
      DstPtr[1] = ((UINT8 *)SrcPtr)[1];
      DstPtr[2] = ((UINT8 *)SrcPtr)[0];
      SrcPtr++;
      DstPtr += 3;
    }

    Status = DlUsbBulkWrite (UsbDisplayLinkDev, DstBuffer, DataLen, &USBStatus);

    // USBStatus values defined in usbio.h, e.g. EFI_USB_ERR_TIMEOUT 0x40
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Screen update - USB bulk transfer of pixel data failed. Line %d len %d, failure code %r USB status x%x\n", H, DataLen, Status, USBStatus));
      break;
    }
    // Need an extra DlUsbBulkWrite if the data length is divisible by USB MaxPacketSize. This spare data will just get written into the (invisible) stride area.
    // Note that the API doesn't let us do a bulk write of 0.
    if ((DataLen & (UsbDisplayLinkDev->BulkOutEndpointDescriptor.MaxPacketSize - 1)) == 0) {
      Status = DlUsbBulkWrite (UsbDisplayLinkDev, DstBuffer, 2, &USBStatus);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Screen update - USB bulk transfer of pixel data failed. Line %d len %d, failure code %r USB status x%x\n", H, DataLen, Status, USBStatus));
        break;
      }
    }
  }

  if (!EFI_ERROR (Status)) {
    // If we've successfully transmitted the frame, reset the values that store which area of the screen has been BLTted to.
    // If we haven't succeeded, this will mean we'll try to resend it after the next poll period.
    UsbDisplayLinkDev->LastY2 = 0;
    UsbDisplayLinkDev->LastY1 = (UINTN)-1;
  }

  // Payload with length of 1 to terminate the frame
  // We need to do this even if we had an error, to indicate to the DL device that it should now expect a new frame.
  DlUsbBulkWrite (UsbDisplayLinkDev, DstBuffer, 1, &USBStatus);

  gBS->RestoreTPL (OriginalTPL);

  return Status;
}

/**
 * Calculate the video refresh rate from the video timing parameters (pixel clock etc)
 * @param videoMode
 * @return
 */
#ifndef MDEPKG_NDEBUG
STATIC inline UINT16
CalculateRefreshRate (
    IN CONST struct VideoMode *VideoMode)
{
  UINT16 RefreshRate;
  UINT16 Rmod;
  UINT16 Rate10Hz;

  RefreshRate = (VideoMode->PixelClock * 10000) / ((VideoMode->HActive + VideoMode->HBlanking) * (VideoMode->VActive + VideoMode->VBlanking));
  Rmod = RefreshRate % 10;
  Rate10Hz = RefreshRate - Rmod;

  if (Rmod >= 5) {
    Rate10Hz += 10;
  }
  return Rate10Hz;
}
#endif // MDEPKG_NDEBUG
/* ***************************************************************************************************** */
/* ***************************************************************************************************** */
/* ******************        START OF FUNCTIONS WHICH IMPLEMENT GOP INTERFACE         ****************** */
/* ***************************************************************************************************** */
/* ***************************************************************************************************** */


/**
 *
 * @param Gop          Pointer to the instance of the GOP protocol
 * @param ModeNumber
 * @param SizeOfInfo
 * @param Info
 * @return
 */
EFI_STATUS
EFIAPI
DisplayLinkQueryMode (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL          *Gop,
  IN  UINT32                                ModeNumber,
  OUT UINTN                                 *SizeOfInfo,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  **Info
)
{
  USB_DISPLAYLINK_DEV *Dev;
  CONST struct VideoMode *VideoMode;
  EFI_STATUS Status;

  Dev = USB_DISPLAYLINK_DEV_FROM_GRAPHICS_OUTPUT_PROTOCOL(Gop);
  Status = EFI_INVALID_PARAMETER;

  if ((SizeOfInfo == NULL) || (Info == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  // Get a video mode from the EDID
  Status = DlEdidGetSupportedVideoModeWithFallback (ModeNumber, Dev->EdidActive.Edid, Dev->EdidActive.SizeOfEdid, &VideoMode);

  if (!EFI_ERROR (Status)) {

    *Info = (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION*)AllocatePool (sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
    if (*Info == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    DEBUG ((DEBUG_INFO, "BIOS querying mode number %d - returning %dx%d @ %dHz\n", ModeNumber, VideoMode->HActive, VideoMode->VActive, CalculateRefreshRate (VideoMode)));

    *SizeOfInfo = sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);

    (*Info)->Version = 0;
    (*Info)->HorizontalResolution = VideoMode->HActive;
    (*Info)->VerticalResolution = VideoMode->VActive;
    (*Info)->PixelFormat = PixelBltOnly;
    (*Info)->PixelsPerScanLine = (*Info)->HorizontalResolution;
    (*Info)->PixelInformation.RedMask = 0;
    (*Info)->PixelInformation.GreenMask = 0;
    (*Info)->PixelInformation.BlueMask = 0;
    (*Info)->PixelInformation.ReservedMask = 0;
  }
  return Status;
}

/**
 *
 * @param Gop         Pointer to the instance of the GOP protocol
 * @param ModeNumber
 * @return
 */
EFI_STATUS
EFIAPI
DisplayLinkSetMode (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL  *Gop,
  IN  UINT32                        ModeNumber
)
{
  USB_DISPLAYLINK_DEV *UsbDisplayLinkDev;
  EFI_STATUS Status;
  CONST struct VideoMode *VideoMode;

  UsbDisplayLinkDev = USB_DISPLAYLINK_DEV_FROM_GRAPHICS_OUTPUT_PROTOCOL(Gop);

  // Prevent the DisplayLinkPeriodicTimer from interrupting us (bug 28877).
  // When the driver is manually loaded, the TPL is TPL_NOTIFY (16) which prevents the interrupt from the timer.
  // When the GOP driver is sideloaded, the TPL of this call is TPL_APPLICATION (4) and the timer can interrupt us.
  Gop->Mode->Mode = GRAPHICS_OUTPUT_INVALID_MODE_NUMBER;

  // Get a video mode from the EDID
  Status = DlEdidGetSupportedVideoModeWithFallback (ModeNumber, UsbDisplayLinkDev->EdidActive.Edid, UsbDisplayLinkDev->EdidActive.SizeOfEdid, &VideoMode);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  Gop->Mode->Info->Version = 0;
  Gop->Mode->Info->HorizontalResolution = VideoMode->HActive;
  Gop->Mode->Info->VerticalResolution = VideoMode->VActive;
  Gop->Mode->Info->PixelFormat = PixelBltOnly;
  Gop->Mode->Info->PixelsPerScanLine = Gop->Mode->Info->HorizontalResolution;
  Gop->Mode->SizeOfInfo = sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
  Gop->Mode->FrameBufferBase = (EFI_PHYSICAL_ADDRESS)(UINTN)NULL;
  Gop->Mode->FrameBufferSize = 0;

  //
  // Allocate the back buffer
  //
  if (UsbDisplayLinkDev->Screen != NULL) {
    FreePool (UsbDisplayLinkDev->Screen);
  }

  UsbDisplayLinkDev->Screen = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)AllocateZeroPool (
    Gop->Mode->Info->HorizontalResolution *
    Gop->Mode->Info->VerticalResolution *
    sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));

  if (UsbDisplayLinkDev->Screen == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "Video mode %d selected by BIOS - %d x %d.\n", ModeNumber, VideoMode->HActive, VideoMode->VActive));
  // Wait until we are sure that we can set the video mode before we tell the firmware
  Status = DlUsbSendControlWriteMessage (UsbDisplayLinkDev, SET_VIDEO_MODE, 0, VideoMode, sizeof (struct VideoMode));

  if (Status != EFI_SUCCESS) {
    // Flag up that we haven't set the video mode correctly yet.
    DEBUG ((DEBUG_ERROR, "Failed to send USB message to DisplayLink device to set monitor video mode. Monitor connected correctly?\n"));
    Gop->Mode->Mode = GRAPHICS_OUTPUT_INVALID_MODE_NUMBER;
    FreePool (UsbDisplayLinkDev->Screen);
    UsbDisplayLinkDev->Screen = NULL;
  } else {
    BuildBackBuffer (
      UsbDisplayLinkDev,
      UsbDisplayLinkDev->Screen,
      EfiBltBufferToVideo,
      0, 0,
      0, 0,
      Gop->Mode->Info->HorizontalResolution,
      Gop->Mode->Info->VerticalResolution,
      Gop->Mode->Info->HorizontalResolution * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL),
      Gop->Mode->Info->HorizontalResolution);
    // unlock the DisplayLinkPeriodicTimer
    Gop->Mode->Mode = ModeNumber;
  }

  return Status;
}

/**
 * Implementation of the GOP protocol Blt API function
 * @param This            Pointer to the instance of the GOP protocol
 * @param BltBuffer
 * @param BltOperation
 * @param SourceX
 * @param SourceY
 * @param DestinationX
 * @param DestinationY
 * @param Width
 * @param Height
 * @param Delta
 * @return
 */
EFI_STATUS
EFIAPI
DisplayLinkBlt (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL            *This,
  IN  EFI_GRAPHICS_OUTPUT_BLT_PIXEL           *BltBuffer, OPTIONAL
  IN  EFI_GRAPHICS_OUTPUT_BLT_OPERATION       BltOperation,
  IN  UINTN                                   SourceX,
  IN  UINTN                                   SourceY,
  IN  UINTN                                   DestinationX,
  IN  UINTN                                   DestinationY,
  IN  UINTN                                   Width,
  IN  UINTN                                   Height,
  IN  UINTN                                   Delta         OPTIONAL
)
{
  USB_DISPLAYLINK_DEV* UsbDisplayLinkDev;
  UsbDisplayLinkDev = USB_DISPLAYLINK_DEV_FROM_GRAPHICS_OUTPUT_PROTOCOL(This);

  // Drop out if we haven't set the video mode up yet
  if (This->Mode->Mode == GRAPHICS_OUTPUT_INVALID_MODE_NUMBER) {
    return EFI_SUCCESS;
  }

  if ((BltOperation < 0) || (BltOperation >= EfiGraphicsOutputBltOperationMax)) {
    return EFI_INVALID_PARAMETER;
  }

  if (Width == 0 || Height == 0) {
    return EFI_INVALID_PARAMETER;
  }

  // Lock so we make an atomic write the frame buffer.
  // We would not want a timer based event (Cursor, ...) to come in while we are doing this operation.
  EFI_TPL OriginalTPL = gBS->RaiseTPL (TPL_NOTIFY);

  CONST UINTN BltBufferStride = (Delta == 0) ? Width * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL) : Delta;
  CONST EFI_STATUS boundsCheckStatus = CheckBounds (This, BltOperation, SourceX, SourceY, Width, Height, DestinationX, DestinationY);
  if (EFI_ERROR (boundsCheckStatus)) {
    gBS->RestoreTPL (OriginalTPL);
    return boundsCheckStatus;
  }

  BuildBackBuffer (UsbDisplayLinkDev, BltBuffer, BltOperation, SourceX, SourceY, DestinationX, DestinationY, Width, Height, BltBufferStride, This->Mode->Info->PixelsPerScanLine);

  gBS->RestoreTPL (OriginalTPL);
  return EFI_SUCCESS;
}

