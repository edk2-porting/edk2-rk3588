# Overview
* **Feature Name:** Virtual Keyboard
* **PI Phase(s) Supported:** DXE
* **SMM Required?** No

## Purpose
This feature provides a DXE virtual keyboard driver, used with a touch panel.

# High-Level Theory of Operation
This driver will use the following protocol:
  gEfiAbsolutePointerProtocolGuid
  gEfiGraphicsOutputProtocolGuid
  gEdkiiTouchPanelGuid

It will show a picture like a keyboard in the graphic output, then detect
position when user touch the touch panel, then calculate the key which the
user want to type.
Touch panel driver use gEdkiiTouchPanelGuid to indicate the touch panel is ready for support virtual keyboard.

## Firmware Volumes
FvAdvancedUncompact
FvAdvanced

## Modules
VirtualKeyboardDxe: The main driver of virtual keyboard.

## VirtualKeyboardDxe
This driver support the virtual keyboard feature.

## Key Functions
* Show a small icon in the corner to show/hide a keyboard picture in the screen.
* If the user press the button in the picture, then convert it to the keyboard input.
* Support general simple keyboard input.

## Configuration
There is not PCD or EFI variable used to config this feature.
Platform maybe use its own solution to load/unload this driver.

## Data Flows
User touch the touch panel -> absolute pointer and status -> virtual keyboard event -> user keyboard input.

## Control Flows
This driver use AbsolutePointer protocol to get the info of user input, and use GraphicsOutput protocol to show the picture.

## Build Flows
There is not special build flow.

## Test Point Results
Test this driver in a device with touch panel and the BIOS support touch panel driver.
If there is not touch panel, then can modify a mouse driver to support absolute pointer protocol to verify this driver.

## Functional Exit Criteria
* If the touch panel driver is not ready, then will not start this driver.
* Show small icon in the corner of the screen.
* Touch the icon, can show/hide the keyboard layout picture in the screen.
* Touch the key in the keyboardy layout picture, can input the correct key.

## Feature Enabling Checklist
Make sure the following protocols are supported:
  gEfiAbsolutePointerProtocolGuid
  gEfiGraphicsOutputProtocolGuid
  gEdkiiTouchPanelGuid

## Performance Impact
* VirtualKeyboardDxe.efi: debug version size: 734KB before compress, release version size: 698KB before compress.

## Common Optimizations
* The picture use bmp format, cost 684KB total. Use JPEG or other format can save the efi size before compress.
