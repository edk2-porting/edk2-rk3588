/** @file

  Copyright (c) 2022 Rockchip Electronics Co. Ltd.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/


#include <Protocol/RockchipCrtcProtocol.h>

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MediaBusFormat.h>
#include <Library/DrmModes.h>
#include <Library/RockchipDisplayLib.h>

EFIAPI
EFI_STATUS
DisplaySetCrtcInfo (
  OUT DRM_DISPLAY_MODE                     *Mode,
  IN  UINT32                               AdjustFlags
  )
{
  if ((Mode == NULL) || ((Mode->Type & DRM_MODE_TYPE_CRTC_C) == DRM_MODE_TYPE_BUILTIN))
    return EFI_UNSUPPORTED;

  if (Mode->Flags & DRM_MODE_FLAG_DBLCLK)
    Mode->CrtcClock = 2 * Mode->Clock;
  else
    Mode->CrtcClock = Mode->Clock;
  Mode->CrtcHDisplay = Mode->HDisplay;
  Mode->CrtcHSyncStart = Mode->HSyncStart;
  Mode->CrtcHSyncEnd = Mode->HSyncEnd;
  Mode->CrtcHTotal = Mode->HTotal;
  Mode->CrtcHSkew = Mode->HSkew;
  Mode->CrtcVDisplay = Mode->VDisplay;
  Mode->CrtcVSyncStart = Mode->VSyncStart;
  Mode->CrtcVSyncEnd = Mode->VSyncEnd;
  Mode->CrtcVTotal = Mode->VTotal;

  if (Mode->Flags & DRM_MODE_FLAG_INTERLACE) {
    if (AdjustFlags & CRTC_INTERLACE_HALVE_V) {
      Mode->CrtcVDisplay /= 2;
      Mode->CrtcVSyncStart /= 2;
      Mode->CrtcVSyncEnd /= 2;
      Mode->CrtcVTotal /= 2;
    }
  }

  if (!(AdjustFlags & CRTC_NO_DBLSCAN)) {
    if (Mode->Flags & DRM_MODE_FLAG_DBLSCAN) {
      Mode->CrtcVDisplay *= 2;
      Mode->CrtcVSyncStart *= 2;
      Mode->CrtcVSyncEnd *= 2;
      Mode->CrtcVTotal *= 2;
    }
  }

  if (!(AdjustFlags & CRTC_NO_VSCAN)) {
    if (Mode->VScan > 1) {
      Mode->CrtcVDisplay *= Mode->VScan;
      Mode->CrtcVSyncStart *= Mode->VScan;
      Mode->CrtcVSyncEnd *= Mode->VScan;
      Mode->CrtcVTotal *= Mode->VScan;
    }
  }

  Mode->CrtcVBlankStart = MIN(Mode->CrtcVSyncStart, Mode->CrtcVDisplay);
  Mode->CrtcVBlankEnd = MAX(Mode->CrtcVSyncEnd, Mode->CrtcVTotal);
  Mode->CrtcHBlankStart = MIN(Mode->CrtcHSyncStart, Mode->CrtcHDisplay);
  Mode->CrtcHBblankEnd = MAX(Mode->CrtcHSyncEnd, Mode->CrtcHTotal);

  return EFI_SUCCESS;
}

EFIAPI
EFI_STATUS
DisplaySetFramebuffer (
  OUT EFI_PHYSICAL_ADDRESS*                VramBaseAddress,
  OUT UINTN*                               VramSize
  )
{
  // Set the VRAM size.
  *VramSize = SIZE_128MB;

  // Check if base address is already reserved for the framebuffer.
  *VramBaseAddress = (EFI_PHYSICAL_ADDRESS)AllocatePool(SIZE_128MB);

  return EFI_SUCCESS;
}

