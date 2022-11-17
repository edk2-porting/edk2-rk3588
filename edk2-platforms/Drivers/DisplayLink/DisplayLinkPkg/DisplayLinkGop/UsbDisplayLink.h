/**
 * @file UsbDisplayLink.h
 * @brief Helper routine and corresponding data struct used by USB DisplayLink Driver.
 *
 * Copyright (c) 2018-2019, DisplayLink (UK) Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
**/

#ifndef _EFI_USB_DISPLAYLINK_H_
#define _EFI_USB_DISPLAYLINK_H_

#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>

#include <Protocol/EdidActive.h>
#include <Protocol/EdidDiscovered.h>
#include <Protocol/EdidOverride.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/UsbIo.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#define VENDOR_DISPLAYLINK      0x17e9
#define CLASS_VENDOR            0xFF

#define DISPLAYLINK_USB_INTERFACE_NUMBER_NIVO ((UINTN)0)

#define INTERFACE_PROTOCOL_DIRECT_FB    4

#define USB_TRANSFER_LENGTH           (64 * 1024)
#define DISPLAYLINK_MODE_DATA_LENGTH  (146)
#define DISPLAYLINK_USB_CTRL_TIMEOUT  (1000)
#define DISPLAYLINK_USB_BULK_TIMEOUT  (1)

#define DISPLAYLINK_SCREEN_UPDATE_TIMER_PERIOD  ((UINTN)1000000) // 0.1s in us
#define DISPLAYLINK_FULL_SCREEN_UPDATE_PERIOD   ((UINTN)30000) // 3s in ticks

#define DISPLAYLINK_FIXED_VERTICAL_REFRESH_RATE ((UINT16)60)

// Requests to read values from the firmware
#define EDID_BLOCK_SIZE 128
#define EDID_DETAILED_TIMING_INVALID_PIXEL_CLOCK ((UINT16)(0x64))

/** Structures ported from firmware - protocol.h */
enum ID {
  // VideoCommands
  GET_OUTPUT_EDID = 0,
  SET_VIDEO_MODE = 1
};

typedef struct {
  UINT32                     HorizontalResolution;
  UINT32                     VerticalResolution;
  UINT32                     ColorDepth;
  UINT32                     RefreshRate;
  UINT8                      Commands[DISPLAYLINK_MODE_DATA_LENGTH];
} DISPLAYLINK_MODE_DATA;

#define GRAPHICS_OUTPUT_INVALID_MODE_NUMBER 0xffff

/**
 *  Device instance of USB display.
 */
typedef struct {
  UINT64                        Signature;
  EFI_HANDLE                    Handle;
  EFI_USB_IO_PROTOCOL           *UsbIo;
  EFI_USB_INTERFACE_DESCRIPTOR  InterfaceDescriptor;
  EFI_USB_ENDPOINT_DESCRIPTOR   BulkOutEndpointDescriptor;
  EFI_USB_ENDPOINT_DESCRIPTOR   BulkInEndpointDescriptor;
  EFI_GRAPHICS_OUTPUT_PROTOCOL  GraphicsOutputProtocol;
  EFI_EDID_DISCOVERED_PROTOCOL  EdidDiscovered;
  EFI_EDID_ACTIVE_PROTOCOL      EdidActive;
  EFI_UNICODE_STRING_TABLE      *ControllerNameTable;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Screen;
  UINTN                         DataSent;                       /** Debug - used to track the bandwidth */
  EFI_EVENT                     TimerEvent;
  EFI_EVENT                     DriverExitBootServicesEvent;
  BOOLEAN                       ShowBandwidth;                 /** Debugging - show the bandwidth on the screen */
  BOOLEAN                       ShowTestPattern;               /** Show a colourbar pattern instead of the BLTd contents of the framebuffer */
  UINTN                         LastY1;                        /** Used to track if we can do a partial screen update */
  UINTN                         LastY2;
  UINTN                         LastWidth;
  UINTN                         TimeSinceLastScreenUpdate;     /** Do a full screen update every (x) seconds */
} USB_DISPLAYLINK_DEV;

#define USB_DISPLAYLINK_DEV_SIGNATURE SIGNATURE_32 ('d', 'l', 'i', 'n')

struct VideoMode {
  UINT8  Reserved1;          /* Reserved - must be 0. */
  UINT8  Reserved2;          /* Reserved - must be 2. */

  // Values matching the EDID Detailed Timing Descriptor spec
  UINT16 PixelClock;
  UINT16 HActive;
  UINT16 HBlanking;
  UINT16 HSyncOffset;  // Horizontal Front Porch
  UINT16 HSyncWidth;
  UINT16 VActive;
  UINT16 VBlanking;
  UINT16 VSyncOffset;  // Vertical Front Porch
  UINT16 VSyncWidth;
  // End of Edid Detailed Timing Descriptor

  UINT16 Flags               /*ModeFlags*/;
  UINT16 Accumulate;
  UINT16 Reserved3;         /* Reserved - must be 0. */
  UINT16 Reserved4;         /* Reserved - must be 0. */
  UINT16 InsetLeft;
  UINT16 InsetTop;
  UINT16 InsetRight;
  UINT16 InsetBottom;
  UINT32 FillValue;
  UINT32 Reserved5;        /* Reserved - must be 0. */
  UINT8  Vic;
  UINT8  ActiveFormat;
  UINT16 Reserved6;
};

#define USB_DISPLAYLINK_DEV_FROM_GRAPHICS_OUTPUT_PROTOCOL(a) \
  CR(a, USB_DISPLAYLINK_DEV, GraphicsOutputProtocol, USB_DISPLAYLINK_DEV_SIGNATURE)

//
// Global Variables
//
extern EFI_DRIVER_BINDING_PROTOCOL   gUsbDisplayLinkDriverBinding;
extern EFI_COMPONENT_NAME_PROTOCOL   mUsbDisplayLinkComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL  mUsbDisplayLinkComponentName2;


/* ******************************************* */
/* ********  GOP interface functions  ******** */
/* ******************************************* */

/**
 * Implementation of the GOP protocol QueryMode API function
 * @param This         Instance of the GOP protocol
 * @param ModeNumber
 * @param SizeOfInfo
 * @param Info
 * @return
 */
EFI_STATUS
  EFIAPI
  DisplayLinkQueryMode (
    IN  EFI_GRAPHICS_OUTPUT_PROTOCOL          *This,
    IN  UINT32                                ModeNumber,
    OUT UINTN                                 *SizeOfInfo,
    OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  **Info
  );


/**
 * Implementation of the GOP protocol SetMode API function
 * @param This
 * @param ModeNumber
 * @return
 */
EFI_STATUS
  EFIAPI
  DisplayLinkSetMode (
    IN  EFI_GRAPHICS_OUTPUT_PROTOCOL  *This,
    IN  UINT32                        ModeNumber
  );

/**
 * Implementation of the GOP protocol Blt API function
 * @param This
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
  );

/* *************************** */
/* **  GOP helper functions ** */
/* *************************** */

VOID
EFIAPI
DlGopPrintTextToScreen (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput,
  UINTN X,
  UINTN Y,
  IN CONST CHAR16 *Format,
  ...
);

EFI_STATUS
DlGopSendTestPattern (
  USB_DISPLAYLINK_DEV* UsbDisplayLinkDev,
  UINTN PatternNumber
);

EFI_STATUS
DlGopSendScreenUpdate (
  USB_DISPLAYLINK_DEV* UsbDisplayLinkDev
);


/* ******************************************* */
/* ********  USB interface functions  ******** */
/* ******************************************* */

EFI_STATUS
DlUsbSendControlWriteMessage (
  IN USB_DISPLAYLINK_DEV *Device,
  IN UINT8 Request,
  IN UINT16 Value,
  IN CONST VOID *ControlMsg,
  IN UINT16 ControlMsgLen
);

EFI_STATUS
DlUsbSendControlReadMessage (
  IN USB_DISPLAYLINK_DEV *Device,
  IN UINT8 Request,
  IN UINT16 Value,
  OUT VOID *ControlMsg,
  IN UINT16 ControlMsgLen
);

EFI_STATUS
DlUsbBulkWrite (
  USB_DISPLAYLINK_DEV* UsbDisplayLinkDev,
  CONST UINT8* Buffer,
  UINTN DataLen,
  UINT32 *USBStatus
);

UINTN
DlUsbBulkRead (
  USB_DISPLAYLINK_DEV* UsbDisplayLinkDev,
  UINT8* Buffer,
  UINTN BufferLen
);

/* ******************************************* */
/* ********   Video Mode functions    ******** */
/* ******************************************* */

// Pre-calculated video modes
UINT32
DlVideoModeGetNumSupportedVideoModes ();

CONST struct VideoMode *
DlVideoModeGetSupportedVideoMode (
  UINT32 index
);

#endif
