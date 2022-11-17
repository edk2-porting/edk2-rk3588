/** @file
 *
 *  Copyright (c) 2020, ARM Limited. All rights reserved.
 *  Copyright (c) 2017-2018, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/Cpu.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/DevicePath.h>
#include <Protocol/RpiFirmware.h>

extern EFI_GRAPHICS_OUTPUT_PROTOCOL gDisplayProto;
extern EFI_COMPONENT_NAME_PROTOCOL  gComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL gComponentName2;
extern EFI_DRIVER_BINDING_PROTOCOL  mDriverBinding;

VOID
RegisterScreenshotHandlers (
  VOID
  );

#endif /* _DISPLAY_H_ */
