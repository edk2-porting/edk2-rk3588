/**
 * @file UsbDisplayLink.c
 * @brief USB DisplayLink Driver that manages USB DisplayLink device and produces Graphics Output Protocol
 * This file implements the functions of the Driver Binding / Start / Stop / Unload interface
 *
 * Copyright (c) 2018-2019, DisplayLink (UK) Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
**/

#include "UsbDisplayLink.h"

#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Protocol/HiiFont.h>

#include "Edid.h"
#include "UsbDescriptors.h"

//
// Functions of Driver Binding Protocol
//

EFI_STATUS
EFIAPI
UsbDisplayLinkDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
);

EFI_STATUS
EFIAPI
UsbDisplayLinkDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
);

EFI_STATUS
EFIAPI
UsbDisplayLinkDriverBindingStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL   *This,
  IN  EFI_HANDLE                    Controller,
  IN  UINTN                         NumberOfChildren,
  IN  EFI_HANDLE                    *ChildHandleBuffer
);

EFI_STATUS
EFIAPI
UsbDisplayLinkDriverBindingEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
);


EFI_STATUS
EFIAPI
UsbDisplayLinkDriverCombinedGopBindingEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable,
  IN EFI_HANDLE           DriverBindingHandle
);


// Generated with https://www.guidgen.com/ - "B70E5A79-C6D6-4267-B02E-9108C989E287"
EFI_GUID gEfiDlGopVariableGuid = { 0xB70E5A79, 0xC6D6, 0x4267,{ 0xB0, 0x2E, 0x91, 0x08, 0xC9, 0x89, 0xE2, 0x87 } };


EFI_DRIVER_BINDING_PROTOCOL gUsbDisplayLinkDriverBinding = {
  UsbDisplayLinkDriverBindingSupported,
  UsbDisplayLinkDriverBindingStart,
  UsbDisplayLinkDriverBindingStop,
  INF_DRIVER_VERSION,
  NULL,
  NULL
};


/**
 * Reads integer environment variable with default fallback.
 * @param variableName   variable name to read
 * @param defaultValue   default value to return if requested not found
 */
STATIC UINT32
ReadEnvironmentInt (
    CHAR16 *VariableName,
    UINT32 DefaultValue
    )
{
  UINT32 Result;
  UINTN DataSize;
  DataSize = sizeof (Result);
  CONST EFI_STATUS Status = gRT->GetVariable (VariableName, &gEfiDlGopVariableGuid, (UINT32*)NULL, &DataSize, &Result);
  if (!EFI_ERROR (Status) && (sizeof (Result) == DataSize)) {
    return Result;
  }
  return DefaultValue;
}

/**
* Reads boolean environment variable with default fallback.
* @param variableName   variable name to read
* @param defaultValue   default value to return if requested not found
*/
STATIC BOOLEAN
ReadEnvironmentBool (
    CHAR16 *VariableName,
    BOOLEAN DefaultValue
    )
{
  return ReadEnvironmentInt (VariableName, DefaultValue ? 1 : 0) == 1;
}


/**
*
* @param UsbDisplayLinkDev
* @return
*/
STATIC EFI_STATUS
InitializeUsbDisplayLinkDevice (
  IN OUT USB_DISPLAYLINK_DEV *UsbDisplayLinkDev
)
{
  EFI_GRAPHICS_OUTPUT_PROTOCOL* Gop;
  Gop = &UsbDisplayLinkDev->GraphicsOutputProtocol;
  Gop->QueryMode = DisplayLinkQueryMode;
  Gop->SetMode = DisplayLinkSetMode;
  Gop->Blt = DisplayLinkBlt;

  //
  // Allocate buffer for Graphics Output Protocol mode information
  //
  Gop->Mode = (EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE*)AllocatePool (sizeof (EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE));
  if (Gop->Mode == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Gop->Mode->Info = (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION*)AllocatePool (sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
  if (Gop->Mode->Info == NULL) {
    FreePool (Gop->Mode);
    Gop->Mode = NULL;
    return EFI_OUT_OF_RESOURCES;
  }

  Gop->Mode->MaxMode = MAX(1, DlEdidGetNumSupportedModesInEdid (UsbDisplayLinkDev->EdidActive.Edid, UsbDisplayLinkDev->EdidActive.SizeOfEdid));

  Gop->Mode->Mode = GRAPHICS_OUTPUT_INVALID_MODE_NUMBER;
  Gop->Mode->Info->Version = 0;
  // Initialising the horizontal resolution prevents certain BIOSs from hanging on boot, but
  // it is not yet clear why. See bug 28194.
  Gop->Mode->Info->HorizontalResolution = DlVideoModeGetSupportedVideoMode (0)->HActive;
  Gop->Mode->Info->VerticalResolution = 0;
  Gop->Mode->Info->PixelFormat = PixelBltOnly;
  Gop->Mode->Info->PixelsPerScanLine = 0;
  Gop->Mode->SizeOfInfo = sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
  Gop->Mode->FrameBufferBase = (EFI_PHYSICAL_ADDRESS)(UINTN)NULL;
  Gop->Mode->FrameBufferSize = 0;

  // Prevent DlGopSendScreenUpdate from running until we are sure that the video mode is set
  UsbDisplayLinkDev->LastY2 = 0;
  UsbDisplayLinkDev->LastY1 = (UINTN)-1;

  return EFI_SUCCESS;
}

/**
  Look for alternate settings for the UsbIo handle's interface
  which offer protocol DL_PROTOCOL_DIRECT_FB.

  @retval -1                     Not found
  @retval Other                  The alternate setting
**/
STATIC INTN
GetDirectFbAltSetting (
    IN EFI_USB_IO_PROTOCOL *UsbIo,
    IN UINT8 ParentInterfaceNumber
    )
{
  EFI_STATUS Status;
  INTN AltSettingIndex;
  UINT16 InterfaceIndex;

  AltSettingIndex = -1;

  for (InterfaceIndex = 0; InterfaceIndex <= 0xFF; InterfaceIndex++) {
    EFI_USB_INTERFACE_DESCRIPTOR interfaceDescriptor;
    Status = UsbDisplayLinkGetInterfaceDescriptor (UsbIo, &interfaceDescriptor, (UINT8)InterfaceIndex);
    if (EFI_ERROR (Status)) {
      break;
    }

    if (interfaceDescriptor.InterfaceNumber == ParentInterfaceNumber &&
       (interfaceDescriptor.InterfaceClass == CLASS_VENDOR) &&
        interfaceDescriptor.InterfaceProtocol == INTERFACE_PROTOCOL_DIRECT_FB) {
      AltSettingIndex = interfaceDescriptor.AlternateSetting;
      break;
    }
  }
  return AltSettingIndex;
}

/**
 *
 * @param UsbIo
 * @param altSettingIndex
 * @return
 */
STATIC EFI_STATUS
SelectAltSetting (
    IN EFI_USB_IO_PROTOCOL *UsbIo,
    IN UINTN AltSettingIndex)
{
  // Set alternate setting 1 on the interface
  EFI_STATUS Status;
  UINT32 UsbStatus;
  EFI_USB_DEVICE_REQUEST Request;
  ZeroMem (&Request, sizeof (Request));
  Request.RequestType = USB_REQ_TYPE_STANDARD | USB_TARGET_INTERFACE;
  Request.Request = USB_REQ_SET_INTERFACE;
  Request.Index = DISPLAYLINK_USB_INTERFACE_NUMBER_NIVO;
  Request.Value = (UINT16)AltSettingIndex;

  Status = UsbIo->UsbControlTransfer (
    UsbIo,
    &Request,
    EfiUsbNoData,
    DISPLAYLINK_USB_CTRL_TIMEOUT,
    NULL,
    0,
    &UsbStatus);

  if (EFI_ERROR (Status)) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "USB control transfer failed while attempting to select alt setting %d on interface (code %r, USB status x%x). DisplayLink device has unsupported firmware version?\n", AltSettingIndex, Status, UsbStatus));
  }
  return Status;
}


/**
  Report whether the driver can support the device attached via UsbIo
  by seeing what if any capabilities it reports.

  @retval TRUE     Device has sufficient capabilities for this driver.
  @retval FALSE    Device lacks sufficient capabilities.
**/
STATIC BOOLEAN
CapabilitiesSupported (
    IN EFI_USB_IO_PROTOCOL *UsbIo
    )
{
  UINT8 Buffer[256];
  EFI_STATUS Status;

  Status = ReadCapabilitiesDescriptor (UsbIo, Buffer, sizeof (Buffer));
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  VendorDescriptor descriptor;
  Status = UsbDisplayLinkParseCapabilitiesDescriptor (Buffer, sizeof (Buffer), &descriptor);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to parse capabilities descriptor (code %r)\n", Status));
    return FALSE;
  }

  return UsbDisplayLinkCapabilitiesSufficientToBind (&descriptor);
}


/**
 *
 * @param UsbIo
 * @param InterfaceDescriptor
 * @param altSettingIndex
 * @return
 */
STATIC BOOLEAN
IsDLDirectFbCapableInterface (
    IN EFI_USB_IO_PROTOCOL *UsbIo,
    IN EFI_USB_INTERFACE_DESCRIPTOR  *InterfaceDescriptor,
    IN INTN *AltSettingIndex)
{
  EFI_STATUS Status;
  EFI_USB_DEVICE_DESCRIPTOR DeviceDescriptor;

  Status = UsbIo->UsbGetDeviceDescriptor (UsbIo, &DeviceDescriptor);

  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  if (DeviceDescriptor.IdVendor != VENDOR_DISPLAYLINK) {
    return FALSE;
  }

  Status = UsbIo->UsbGetInterfaceDescriptor (UsbIo, InterfaceDescriptor);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  // We can assume that the interface that we want to talk to - the NIVO interface - is number 0
  if (InterfaceDescriptor->InterfaceNumber != DISPLAYLINK_USB_INTERFACE_NUMBER_NIVO) {
    return FALSE;
  }

  // Check if we have an interface (alt setting) descriptor with the correct interface protocol
  *AltSettingIndex = GetDirectFbAltSetting (UsbIo, InterfaceDescriptor->InterfaceNumber);

  if (*AltSettingIndex == -1) {
    DEBUG ((DEBUG_ERROR, "DisplayLink GOP: Failed to find setting on device which supports GOP functionality. Check firmware / device version?\n"));
    return FALSE;
  }

  // Now check that the capabilities that we need are properly supported
  if (CapabilitiesSupported (UsbIo) == FALSE) {
    DEBUG ((DEBUG_ERROR, "DisplayLink GOP: DL device detected, but it doesn't support the required GOP features. Check firmware / device version?\n"));
    return FALSE;
  }

  return TRUE;
}


/**
 * Prints a block of text in the framebuffer (helper function).
 * @param X   x coordinate
 * @param Y   y coordinate
 */
STATIC VOID
DisplayLinkPrintTextToScreenInternal (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput,
  UINTN X,
  UINTN Y,
  IN CHAR16 *Buffer
  )
{
  EFI_STATUS Status;
  EFI_HII_FONT_PROTOCOL *HiiFont;
  EFI_IMAGE_OUTPUT *Blt;
  EFI_FONT_DISPLAY_INFO FontInfo;
  EFI_HII_OUT_FLAGS Flags;

  Blt = (EFI_IMAGE_OUTPUT*)NULL;

  Status = gBS->LocateProtocol (&gEfiHiiFontProtocolGuid, NULL, (VOID **)&HiiFont);
  if (!EFI_ERROR (Status)) {
    Blt = (EFI_IMAGE_OUTPUT*)AllocateZeroPool (sizeof (EFI_IMAGE_OUTPUT));
    Blt->Width = (UINT16)GraphicsOutput->Mode->Info->HorizontalResolution;
    Blt->Height = (UINT16)GraphicsOutput->Mode->Info->VerticalResolution;
    Blt->Image.Screen = GraphicsOutput;

    ZeroMem (&FontInfo, sizeof (EFI_FONT_DISPLAY_INFO));
    FontInfo.ForegroundColor.Red = 0;
    FontInfo.ForegroundColor.Green = 0;
    FontInfo.ForegroundColor.Blue = 0;
    FontInfo.BackgroundColor.Red = 0xff;
    FontInfo.BackgroundColor.Green = 0xff;
    FontInfo.BackgroundColor.Blue = 0xff;

    Flags = EFI_HII_IGNORE_IF_NO_GLYPH | EFI_HII_OUT_FLAG_CLIP |
            EFI_HII_OUT_FLAG_CLIP_CLEAN_X | EFI_HII_OUT_FLAG_CLIP_CLEAN_Y |
            EFI_HII_IGNORE_LINE_BREAK | EFI_HII_DIRECT_TO_SCREEN;

    Status = HiiFont->StringToImage (
                           HiiFont,
                           Flags,
                           Buffer,
                           &FontInfo,
                           &Blt,
                           X,
                           Y,
                           (EFI_HII_ROW_INFO**)NULL,
                           (UINTN*)NULL,
                           (UINTN*)NULL);
  }

  if (Blt != NULL) {
    FreePool (Blt);
  }
}


/**
* Prints a block of text in the framebuffer.
* @param X       x coordinate
* @param Y       y coordinate
* @param Format  string format similar to stdlib's vsnprintf
* @param ...     arguments
*/
VOID
EFIAPI
DlGopPrintTextToScreen (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput,
  UINTN X,
  UINTN Y,
  IN CONST CHAR16 *Format,
  ...
  )
{
  VA_LIST Marker;
  CHAR16 *Buffer;
  UINTN BufferSize;

  ASSERT (Format != NULL);
  ASSERT (((UINTN) Format & BIT0) == 0);

  VA_START(Marker, Format);

  BufferSize = (PcdGet32 (PcdUefiLibMaxPrintBufferSize) + 1) * sizeof (CHAR16);

  Buffer = (CHAR16*)AllocatePool (BufferSize);
  ASSERT (Buffer != NULL);

  UnicodeVSPrint (Buffer, BufferSize, Format, Marker);

  VA_END(Marker);

  DisplayLinkPrintTextToScreenInternal (GraphicsOutput, X, Y, Buffer);

  FreePool (Buffer);
}


/**
 * Sometimes platforms only write to the first GOP device that they find. Enabling this function allows us to copy the pixels from this device.
 * @param UsbDisplayLinkDev
 */
#ifdef COPY_PIXELS_FROM_PRIMARY_GOP_DEVICE
STATIC VOID
DisplayLinkCopyFromPrimaryGopDevice (
  IN USB_DISPLAYLINK_DEV* UsbDisplayLinkDev
  )
{
  UINTN HandleCount;
  EFI_HANDLE *HandleBuffer;
  UINTN HandleIndex;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;

  gBS->LocateHandleBuffer (
    ByProtocol,
    &gEfiGraphicsOutputProtocolGuid,
    NULL,
    &HandleCount,
    &HandleBuffer);

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    gBS->HandleProtocol (HandleBuffer[HandleIndex], &gEfiGraphicsOutputProtocolGuid, (VOID**)&Gop);
    if (Gop != &UsbDisplayLinkDev->GraphicsOutputProtocol && Gop->Mode->FrameBufferBase != (EFI_PHYSICAL_ADDRESS)(UINTN)NULL) {

    // See if we need to do a screen update - calculate a really noddy hash to see if any screen updates have happened.
    STATIC UINT32 prevframeBufferHash = 0; // 4 bytes per pixel
    UINT32 currFrameBufferHash = 0;
    UINTN i;
    for (i = 0; i < (Gop->Mode->Info->HorizontalResolution * Gop->Mode->Info->VerticalResolution); i++) {
      currFrameBufferHash += ((UINT32*)(UINTN)Gop->Mode->FrameBufferBase)[i];
    }

    if (currFrameBufferHash != prevframeBufferHash) {
      prevframeBufferHash = currFrameBufferHash;

      DisplayLinkBlt (
        &UsbDisplayLinkDev->GraphicsOutputProtocol,
        (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)(UINTN)Gop->Mode->FrameBufferBase,
        EfiBltBufferToVideo,
        0,
        0,
        0,
        0,
        Gop->Mode->Info->HorizontalResolution,
        Gop->Mode->Info->VerticalResolution,
        0);
      }
      break;
    }
  }

  FreePool (HandleBuffer);
}
#endif // COPY_PIXELS_FROM_PRIMARY_GOP_DEVICE


/**
 * Exit from boot services: signal handler.
 */
STATIC VOID
EFIAPI
DisplayLinkDriverExitBootServices (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  USB_DISPLAYLINK_DEV* UsbDisplayLinkDev;
  UsbDisplayLinkDev = (USB_DISPLAYLINK_DEV*)Context;

  gBS->CloseEvent (UsbDisplayLinkDev->TimerEvent);
}

/**
 * Periodic screen update: timer callback.
 */
VOID
EFIAPI
DisplayLinkPeriodicTimer (
    IN EFI_EVENT Event,
    IN VOID* Context
    )
{
  EFI_STATUS Status;
  USB_DISPLAYLINK_DEV* UsbDisplayLinkDev;

  Status = EFI_SUCCESS;
  UsbDisplayLinkDev = (USB_DISPLAYLINK_DEV*)Context;

  // Drop out if we haven't set the video mode up yet
  if (UsbDisplayLinkDev->GraphicsOutputProtocol.Mode->Mode == GRAPHICS_OUTPUT_INVALID_MODE_NUMBER) {
    // Restart the one-shot timer to poll the status again.
    Status = gBS->SetTimer (UsbDisplayLinkDev->TimerEvent, TimerRelative, DISPLAYLINK_SCREEN_UPDATE_TIMER_PERIOD);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to create timer.\n"));
    }
    return;
  }

#ifdef COPY_PIXELS_FROM_PRIMARY_GOP_DEVICE
  DisplayLinkCopyFromPrimaryGopDevice (UsbDisplayLinkDev);
#endif // COPY_PIXELS_FROM_PRIMARY_GOP_DEVICE

  if (UsbDisplayLinkDev->ShowBandwidth) {
    STATIC UINTN Count = 0;

    if (Count++ % 50 == 0) {
      DlGopPrintTextToScreen (&UsbDisplayLinkDev->GraphicsOutputProtocol, 32, 48, (CONST CHAR16*)L"  Bandwidth: %d MB/s    ", UsbDisplayLinkDev->DataSent * 10000000 / DISPLAYLINK_SCREEN_UPDATE_TIMER_PERIOD / 50 / 1024 / 1024);
      UsbDisplayLinkDev->DataSent = 0;
    }
  }

  if (UsbDisplayLinkDev->ShowTestPattern)
  {
    if (UsbDisplayLinkDev->ShowTestPattern == 5) {
      DlGopSendTestPattern (UsbDisplayLinkDev, 0);
    } else if (UsbDisplayLinkDev->ShowTestPattern >= 10) {
      DlGopSendTestPattern (UsbDisplayLinkDev, 1);
      UsbDisplayLinkDev->ShowTestPattern = 0;
    }
    UsbDisplayLinkDev->ShowTestPattern++;

  }

  // Send the latest version of the frame buffer to the DL device over USB
  DlGopSendScreenUpdate (UsbDisplayLinkDev);

  // Restart the timer now we've finished
  Status = gBS->SetTimer (UsbDisplayLinkDev->TimerEvent, TimerRelative, DISPLAYLINK_SCREEN_UPDATE_TIMER_PERIOD);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to create timer.\n"));
  }
}

/* ****************************************************************************************************** */
/* ****************************************************************************************************** */
/* ******************   START OF FUNCTIONS WHICH IMPLEMENT DRIVER BINDING INTERFACE    ****************** */
/* ****************************************************************************************************** */
/* ****************************************************************************************************** */

/**
  Check whether USB DisplayLink driver supports this device.

  @param  This                   The USB DisplayLink driver binding protocol.
  @param  Controller             The controller handle to check.
  @param  RemainingDevicePath    The remaining device path.

  @retval EFI_SUCCESS            The driver supports this controller.
  @retval other                  This device isn't supported.

**/
EFI_STATUS
EFIAPI
UsbDisplayLinkDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  )
{
  EFI_STATUS          Status;
  EFI_USB_IO_PROTOCOL *UsbIo;

  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiUsbIoProtocolGuid,
                  (VOID **) &UsbIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Use the USB I/O Protocol interface to check whether Controller is
  // a DisplayLink device that can be managed by this driver.
  //
  Status = EFI_UNSUPPORTED;
  EFI_USB_INTERFACE_DESCRIPTOR DummyInterfaceDescriptor;
  INTN DummyAltSettingIndex;

  if (IsDLDirectFbCapableInterface (UsbIo, &DummyInterfaceDescriptor, &DummyAltSettingIndex)){
    Status = EFI_SUCCESS;
  }

  gBS->CloseProtocol (
        Controller,
        &gEfiUsbIoProtocolGuid,
        This->DriverBindingHandle,
        Controller);

  return Status;
}


/**
  Starts the DisplayLink device with this driver.

  This function consumes USB I/O Protocol, intializes USB DisplayLink device,
  installs Graphics Output Protocol
  Transfer to manage the USB DisplayLink device.

  @param  This                  The USB DisplayLink driver binding instance.
  @param  Controller            Handle of device to bind driver to.
  @param  RemainingDevicePath   Optional parameter use to pick a specific child
                                device to start.

  @retval EFI_SUCCESS           This driver supports this device.
  @retval EFI_UNSUPPORTED       This driver does not support this device.
  @retval EFI_DEVICE_ERROR      This driver cannot be started due to device Error.
  @retval EFI_OUT_OF_RESOURCES  Can't allocate memory resources.
  @retval EFI_ALREADY_STARTED   This driver has been started.

**/
EFI_STATUS
EFIAPI
UsbDisplayLinkDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  )
{
  EFI_STATUS                      Status;
  USB_DISPLAYLINK_DEV             *UsbDisplayLinkDev;
  UINT8                           EndpointNumber;
  EFI_USB_ENDPOINT_DESCRIPTOR     EndpointDescriptor;
  UINT8                           Index;
  BOOLEAN                         FoundOut;
  BOOLEAN                         FoundIn;
  EFI_TPL                         OriginalTPL;
  INTN                            altSettingIndex;

  OriginalTPL = gBS->RaiseTPL (TPL_CALLBACK);

  UsbDisplayLinkDev = (USB_DISPLAYLINK_DEV*)AllocateZeroPool (sizeof (USB_DISPLAYLINK_DEV));
  if (UsbDisplayLinkDev == NULL) {
    DEBUG ((DEBUG_ERROR, "Device initialialisation - Failed to allocate memory for device.\n"));
    gBS->RestoreTPL (OriginalTPL);
    return EFI_OUT_OF_RESOURCES;
  }

  UsbDisplayLinkDev->Signature = USB_DISPLAYLINK_DEV_SIGNATURE;

  UsbDisplayLinkDev->ShowBandwidth = ReadEnvironmentBool (L"DisplayLinkShowBandwidth", FALSE);
  UsbDisplayLinkDev->ShowTestPattern = ReadEnvironmentBool (L"DisplayLinkShowTestPatterns", FALSE);

  //
  // Open USB I/O Protocol
  //
  Status = gBS->OpenProtocol (
      Controller,
      &gEfiUsbIoProtocolGuid,
      (VOID **) &UsbDisplayLinkDev->UsbIo,
      This->DriverBindingHandle,
      Controller,
      EFI_OPEN_PROTOCOL_BY_DRIVER);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to open usbio protocol. Is USB correctly supported on this platform?.\n"));
    Status = EFI_UNSUPPORTED;
    goto ErrorExit2;
  }

  if (!IsDLDirectFbCapableInterface (UsbDisplayLinkDev->UsbIo, &UsbDisplayLinkDev->InterfaceDescriptor, &altSettingIndex)) {
    Status = EFI_UNSUPPORTED;
    goto ErrorExit4;
  }

  Status = SelectAltSetting (UsbDisplayLinkDev->UsbIo, altSettingIndex);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "DisplayLink GOP: Failed to select alternate setting.\n"));
    Status = EFI_UNSUPPORTED;
    goto ErrorExit4;
  }

  //
  // Parse endpoint descriptor
  //
  EndpointNumber = UsbDisplayLinkDev->InterfaceDescriptor.NumEndpoints;

  //
  // Traverse endpoints to find bulk endpoint
  //
  FoundOut = FALSE;
  FoundIn = FALSE;
  for (Index = 0; Index < EndpointNumber; Index++) {
    UsbDisplayLinkDev->UsbIo->UsbGetEndpointDescriptor (
             UsbDisplayLinkDev->UsbIo,
             Index,
             &EndpointDescriptor);

    if ((EndpointDescriptor.Attributes & (BIT0 | BIT1)) == USB_ENDPOINT_BULK) {
      if (!FoundOut && (EndpointDescriptor.EndpointAddress & BIT7) == 0) {
        CopyMem (&UsbDisplayLinkDev->BulkOutEndpointDescriptor, &EndpointDescriptor, sizeof (EndpointDescriptor));
        FoundOut = TRUE;
      } else if (!FoundIn && (EndpointDescriptor.EndpointAddress & BIT7) == BIT7) {
        CopyMem (&UsbDisplayLinkDev->BulkInEndpointDescriptor, &EndpointDescriptor, sizeof (EndpointDescriptor));
        FoundIn = TRUE;
      }
    }
  }

  if (FoundOut == FALSE) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "No endpoints found.  Num endpoints searched = %d.\n", EndpointNumber));
    goto ErrorExit4;
  }

  Status = DlReadEdid (UsbDisplayLinkDev);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to read monitor EDID from DisplayLink device (code %r)\n", Status));
    goto ErrorExit7;
  }

  Status = InitializeUsbDisplayLinkDevice (UsbDisplayLinkDev);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to initialise DisplayLink device (code %r)\n", Status));
    goto ErrorExit7;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
      &Controller,
      &gEfiGraphicsOutputProtocolGuid,
      &UsbDisplayLinkDev->GraphicsOutputProtocol,
      &gEfiEdidDiscoveredProtocolGuid,
      &UsbDisplayLinkDev->EdidDiscovered,
      &gEfiEdidActiveProtocolGuid,
      &UsbDisplayLinkDev->EdidActive,
      NULL);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to install Graphics Output and EDID protocol interfaces - driver not installed correctly - %r\n", Status));
    goto ErrorExit8;
  }

  UsbDisplayLinkDev->ControllerNameTable = (EFI_UNICODE_STRING_TABLE*)NULL;

  AddUnicodeString2 (
    "eng",
    mUsbDisplayLinkComponentName.SupportedLanguages,
    &UsbDisplayLinkDev->ControllerNameTable,
    (CONST CHAR16*)L"Generic Usb DisplayLink",
    TRUE);

  AddUnicodeString2 (
    "en",
    mUsbDisplayLinkComponentName2.SupportedLanguages,
    &UsbDisplayLinkDev->ControllerNameTable,
    (CONST CHAR16*)L"Generic Usb DisplayLink",
    FALSE);

  //
  // Setup a periodic timer
  //
  Status = gBS->CreateEvent (
                  EVT_TIMER | EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  DisplayLinkPeriodicTimer,
                  UsbDisplayLinkDev,
                  &UsbDisplayLinkDev->TimerEvent);

  if (EFI_ERROR (Status)) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "Failed to create screeen update polling event.\n"));
    goto ErrorExit8;
  }

  // Start one-shot timer. The rendering operations can take quite a long time, so we
  // don't want another timer event to happen until we have finished; so we'll restart
  // the timer from DisplayLinkPeriodicTimer, the event handler function.
  Status = gBS->SetTimer (UsbDisplayLinkDev->TimerEvent, TimerRelative, DISPLAYLINK_SCREEN_UPDATE_TIMER_PERIOD);
  if (EFI_ERROR (Status)) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "Failed to create screen update polling timer.\n"));
    goto ErrorExit8;
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  DisplayLinkDriverExitBootServices,
                  UsbDisplayLinkDev,
                  &gEfiEventExitBootServicesGuid,
                  &UsbDisplayLinkDev->DriverExitBootServicesEvent);

  if (EFI_ERROR (Status)) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "Failed to create event for bootexit.\n"));
    goto ErrorExit8;
  }

  gBS->RestoreTPL (OriginalTPL);

  DEBUG ((DEBUG_INFO, "DisplayLink GOP driver successfully bound to device.\n"));

  return EFI_SUCCESS;

  //
  // Error handler
  //
 ErrorExit8:
 ErrorExit7:
 ErrorExit4:
  gBS->CloseProtocol (
   Controller,
   &gEfiUsbIoProtocolGuid,
   This->DriverBindingHandle,
   Controller);

 ErrorExit2:
  if (UsbDisplayLinkDev != NULL) {
    FreePool (UsbDisplayLinkDev);
    UsbDisplayLinkDev = (USB_DISPLAYLINK_DEV*)NULL;
  }

  DEBUG ((DEBUG_ERROR, "Exiting - Failed to initialise driver.\n"));

  gBS->RestoreTPL (OriginalTPL);
  return Status;
}

/**
Entrypoint of USB DisplayLink Driver.

This function is the entrypoint of a combined USB DisplayLink GOP Driver. It installs Driver Binding
Protocols together with Component Name Protocols.

@param  ImageHandle       The firmware allocated handle for the EFI image.
@param  SystemTable       A pointer to the EFI System Table.
@param  DriverBindingHandle  The Driver binding handle

@retval EFI_SUCCESS       The entry point is executed successfully.

**/
EFI_STATUS
EFIAPI
UsbDisplayLinkDriverCombinedGopBindingEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable,
  IN EFI_HANDLE           DriverBindingHandle
)
{
  EFI_STATUS Status;

  Status = EfiLibInstallDriverBindingComponentName2 (
    ImageHandle,
    SystemTable,
    &gUsbDisplayLinkDriverBinding,
    DriverBindingHandle,
    &mUsbDisplayLinkComponentName,
    &mUsbDisplayLinkComponentName2);

  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}


/**
Entrypoint of USB DisplayLink Driver.

This function is the entrypoint of USB DisplayLink Driver. It installs Driver Binding
Protocols together with Component Name Protocols.

@param  ImageHandle       The firmware allocated handle for the EFI image.
@param  SystemTable       A pointer to the EFI System Table.

@retval EFI_SUCCESS       The entry point is executed successfully.

**/
EFI_STATUS
EFIAPI
UsbDisplayLinkDriverBindingEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
)
{
  return UsbDisplayLinkDriverCombinedGopBindingEntryPoint (ImageHandle, SystemTable, ImageHandle);
}


/**
Unloads an image.
@param  ImageHandle           Handle that identifies the image to be unloaded.
@retval EFI_SUCCESS           The image has been unloaded.
@retval EFI_INVALID_PARAMETER ImageHandle is not a valid image handle.
**/
EFI_STATUS
EFIAPI
UsbDisplayLinkDriverCombinedGopUnload (
  IN EFI_HANDLE  ImageHandle
)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  EFI_STATUS  handleDisconnectStatus;
  EFI_HANDLE  *HandleBuffer;
  UINTN       HandleCount;
  UINTN       Index;

  //
  // Retrieve array of all handles in the handle database
  //
  handleDisconnectStatus = gBS->LocateHandleBuffer (
    AllHandles,
    NULL,
    NULL,
    &HandleCount,
    &HandleBuffer
  );
  if (! EFI_ERROR (handleDisconnectStatus)) {
    //
    // Disconnect the current driver from handles in the handle database
    //
    for (Index = 0; Index < HandleCount; Index++) {
      Status = gBS->DisconnectController (HandleBuffer[Index], gImageHandle, NULL);
    }
    //
    // Free the array of handles
    //
    if (HandleBuffer != NULL)
    {
      FreePool (HandleBuffer);
    }
  }

  // Even if we didn't manage to disconnect the handles, try to uninstall the protocols
  //
  // Uninstall protocols installed in the driver entry point
  //
  Status = gBS->UninstallMultipleProtocolInterfaces (
    ImageHandle,
    &gEfiDriverBindingProtocolGuid,
    &gUsbDisplayLinkDriverBinding,
    &gEfiComponentNameProtocolGuid,
    &mUsbDisplayLinkComponentName,
    &gEfiComponentName2ProtocolGuid,
    &mUsbDisplayLinkComponentName2,
    NULL
  );

  if (EFI_ERROR (handleDisconnectStatus))
  {
    return handleDisconnectStatus;
  }
  return Status;
}


/**
  Stop the USB DisplayLink device handled by this driver.

  @param  This                   The USB DisplayLink driver binding protocol.
  @param  Controller             The controller to release.
  @param  NumberOfChildren       The number of handles in ChildHandleBuffer.
  @param  ChildHandleBuffer      The array of child handle.

  @retval EFI_SUCCESS            The device was stopped.
  @retval EFI_UNSUPPORTED        Simple Pointer Protocol is not installed on Controller.
  @retval Others                 Fail to uninstall protocols attached on the device.

**/
EFI_STATUS
EFIAPI
UsbDisplayLinkDriverBindingStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL   *This,
  IN  EFI_HANDLE                    Controller,
  IN  UINTN                         NumberOfChildren,
  IN  EFI_HANDLE                    *ChildHandleBuffer
  )
{
  EFI_STATUS                        Status;
  USB_DISPLAYLINK_DEV               *UsbDisplayLinkDev;
  EFI_GRAPHICS_OUTPUT_PROTOCOL      *GraphicsOutputProtocol;

  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiGraphicsOutputProtocolGuid,
                  (VOID **) &GraphicsOutputProtocol,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL);

  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  UsbDisplayLinkDev = USB_DISPLAYLINK_DEV_FROM_GRAPHICS_OUTPUT_PROTOCOL(GraphicsOutputProtocol);

  // Reset the video mode to clear the display. Don't drop out if there is a problem, just press on.
  // Note that this will also clear the frame buffer, as the screen buffer will be re-allocated with AllocateZeroPool.
  if ((GraphicsOutputProtocol->Mode != NULL) &&
      (GraphicsOutputProtocol->Mode->Mode != GRAPHICS_OUTPUT_INVALID_MODE_NUMBER)) {
    Status = DisplayLinkSetMode (GraphicsOutputProtocol, GraphicsOutputProtocol->Mode->Mode);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "Driver stop - Problem resetting video mode - %r.\n", Status));
    }
  }

  // Reset the alt setting on the interface (to the DL3 alt setting)
  Status = SelectAltSetting (UsbDisplayLinkDev->UsbIo, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Error resetting USB interface alternate setting - %r.\n", Status));
  }

  Status = gBS->UninstallMultipleProtocolInterfaces (
                  Controller,
                  &gEfiGraphicsOutputProtocolGuid,
                  &UsbDisplayLinkDev->GraphicsOutputProtocol,
                  &gEfiEdidDiscoveredProtocolGuid,
                  &UsbDisplayLinkDev->EdidDiscovered,
                  &gEfiEdidActiveProtocolGuid,
                  &UsbDisplayLinkDev->EdidActive,
                  NULL);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Error uninstalling Graphics Output and EDID protocol interfaces - %r.\n", Status));
    return Status;
  }

  gBS->CloseEvent (UsbDisplayLinkDev->TimerEvent);

  gBS->CloseProtocol (
         Controller,
         &gEfiUsbIoProtocolGuid,
         This->DriverBindingHandle,
         Controller);

  //
  // Free all resources.
  //
  if (UsbDisplayLinkDev->ControllerNameTable != NULL) {
    FreeUnicodeStringTable (UsbDisplayLinkDev->ControllerNameTable);
  }

  if (UsbDisplayLinkDev->Screen != NULL) {
    FreePool (UsbDisplayLinkDev->Screen);
    UsbDisplayLinkDev->Screen = NULL;
  }

  if (UsbDisplayLinkDev->GraphicsOutputProtocol.Mode) {
    if (UsbDisplayLinkDev->GraphicsOutputProtocol.Mode->Info) {
      FreePool (UsbDisplayLinkDev->GraphicsOutputProtocol.Mode->Info);
      UsbDisplayLinkDev->GraphicsOutputProtocol.Mode->Info = NULL;
    }
    FreePool (UsbDisplayLinkDev->GraphicsOutputProtocol.Mode);
    UsbDisplayLinkDev->GraphicsOutputProtocol.Mode = NULL;
  }

  FreePool (UsbDisplayLinkDev);

  return EFI_SUCCESS;

}

