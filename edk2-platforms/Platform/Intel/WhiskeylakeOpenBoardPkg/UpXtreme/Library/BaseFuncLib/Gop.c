/** @file
  Others Board's PCD function hook.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <GopConfigLib.h>

//
// Null function for nothing GOP VBT update.
//
VOID
EFIAPI
GopVbtSpecificUpdateNull (
  IN CHILD_STRUCT **ChildStructPtr
  )
{
  return;
}

VOID
EFIAPI
UpXtremeSpecificUpdate (
  IN CHILD_STRUCT **ChildStructPtr
  )
{
  ChildStructPtr[1]->DeviceClass = DISPLAY_PORT_ONLY;
  ChildStructPtr[1]->DVOPort     = DISPLAY_PORT_B;
  ChildStructPtr[2]->DeviceClass = DISPLAY_PORT_HDMI_DVI_COMPATIBLE;
  ChildStructPtr[2]->DVOPort     = DISPLAY_PORT_C;
  ChildStructPtr[2]->AUX_Channel = AUX_CHANNEL_C;
  ChildStructPtr[3]->DeviceClass = NO_DEVICE;
}

