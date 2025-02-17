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
  OUT DRM_DISPLAY_MODE  *Mode,
  IN  UINT32            AdjustFlags
  )
{
  if ((Mode == NULL) || ((Mode->Type & DRM_MODE_TYPE_CRTC_C) == DRM_MODE_TYPE_BUILTIN)) {
    return EFI_UNSUPPORTED;
  }

  if (Mode->Flags & DRM_MODE_FLAG_DBLCLK) {
    Mode->CrtcClock = 2 * Mode->Clock;
  } else {
    Mode->CrtcClock = Mode->Clock;
  }

  Mode->CrtcHDisplay   = Mode->HDisplay;
  Mode->CrtcHSyncStart = Mode->HSyncStart;
  Mode->CrtcHSyncEnd   = Mode->HSyncEnd;
  Mode->CrtcHTotal     = Mode->HTotal;
  Mode->CrtcHSkew      = Mode->HSkew;
  Mode->CrtcVDisplay   = Mode->VDisplay;
  Mode->CrtcVSyncStart = Mode->VSyncStart;
  Mode->CrtcVSyncEnd   = Mode->VSyncEnd;
  Mode->CrtcVTotal     = Mode->VTotal;

  if (Mode->Flags & DRM_MODE_FLAG_INTERLACE) {
    if (AdjustFlags & CRTC_INTERLACE_HALVE_V) {
      Mode->CrtcVDisplay   /= 2;
      Mode->CrtcVSyncStart /= 2;
      Mode->CrtcVSyncEnd   /= 2;
      Mode->CrtcVTotal     /= 2;
    }
  }

  if (!(AdjustFlags & CRTC_NO_DBLSCAN)) {
    if (Mode->Flags & DRM_MODE_FLAG_DBLSCAN) {
      Mode->CrtcVDisplay   *= 2;
      Mode->CrtcVSyncStart *= 2;
      Mode->CrtcVSyncEnd   *= 2;
      Mode->CrtcVTotal     *= 2;
    }
  }

  if (!(AdjustFlags & CRTC_NO_VSCAN)) {
    if (Mode->VScan > 1) {
      Mode->CrtcVDisplay   *= Mode->VScan;
      Mode->CrtcVSyncStart *= Mode->VScan;
      Mode->CrtcVSyncEnd   *= Mode->VScan;
      Mode->CrtcVTotal     *= Mode->VScan;
    }
  }

  Mode->CrtcVBlankStart = MIN (Mode->CrtcVSyncStart, Mode->CrtcVDisplay);
  Mode->CrtcVBlankEnd   = MAX (Mode->CrtcVSyncEnd, Mode->CrtcVTotal);
  Mode->CrtcHBlankStart = MIN (Mode->CrtcHSyncStart, Mode->CrtcHDisplay);
  Mode->CrtcHBblankEnd  = MAX (Mode->CrtcHSyncEnd, Mode->CrtcHTotal);

  return EFI_SUCCESS;
}

UINT32
EFIAPI
DrmModeVRefresh (
  DRM_DISPLAY_MODE  *Mode
  )
{
  UINT32  Refresh = 0;
  UINT32  CalcVal;

  if (Mode->VRefresh > 0) {
    Refresh = Mode->VRefresh;
  } else if ((Mode->HTotal > 0) && (Mode->VTotal > 0)) {
    int  VTotal;

    VTotal = Mode->VTotal;
    /* work out VRefresh the value will be x1000 */
    CalcVal  = (Mode->Clock * 1000);
    CalcVal /= Mode->HTotal;
    Refresh  = (CalcVal + VTotal / 2) / VTotal;

    if (Mode->Flags & DRM_MODE_FLAG_INTERLACE) {
      Refresh *= 2;
    }

    if (Mode->Flags & DRM_MODE_FLAG_DBLSCAN) {
      Refresh /= 2;
    }

    if (Mode->VScan > 1) {
      Refresh /= Mode->VScan;
    }
  }

  return Refresh;
}

VOID
DisplayModeToDrm (
  IN  CONST DISPLAY_MODE  *DisplayMode,
  OUT DRM_DISPLAY_MODE    *DrmDisplayMode
  )
{
  if ((DisplayMode == NULL) || (DrmDisplayMode == NULL)) {
    ASSERT (FALSE);
    return;
  }

  DrmDisplayMode->Flags |= DisplayMode->HSyncActive ? DRM_MODE_FLAG_PHSYNC : DRM_MODE_FLAG_NHSYNC;
  DrmDisplayMode->Flags |= DisplayMode->VSyncActive ? DRM_MODE_FLAG_PVSYNC : DRM_MODE_FLAG_NVSYNC;
  DrmDisplayMode->Flags |= DisplayMode->ClkActive ? DRM_MODE_FLAG_PPIXDATA : 0;

  DrmDisplayMode->Clock      = DisplayMode->OscFreq;
  DrmDisplayMode->HDisplay   = DisplayMode->HActive;
  DrmDisplayMode->HSyncStart = DrmDisplayMode->HDisplay + DisplayMode->HFrontPorch;
  DrmDisplayMode->HSyncEnd   = DrmDisplayMode->HSyncStart + DisplayMode->HSync;
  DrmDisplayMode->HTotal     = DrmDisplayMode->HSyncEnd + DisplayMode->HBackPorch;
  DrmDisplayMode->VDisplay   = DisplayMode->VActive;
  DrmDisplayMode->VSyncStart = DrmDisplayMode->VDisplay + DisplayMode->VFrontPorch;
  DrmDisplayMode->VSyncEnd   = DrmDisplayMode->VSyncStart + DisplayMode->VSync;
  DrmDisplayMode->VTotal     = DrmDisplayMode->VSyncEnd + DisplayMode->VBackPorch;
  DrmDisplayMode->VRefresh   = DrmModeVRefresh (DrmDisplayMode);
}

UINT8
ConvertCeaToHdmiVic (
  IN UINT8  CeaVic
  )
{
  switch (CeaVic) {
    case 95: return 1;
    case 94: return 2;
    case 93: return 3;
    case 98: return 4;
    default: return 0;
  }
}

UINT8
ConvertHdmiToCeaVic (
  IN UINT8  CeaVic
  )
{
  switch (CeaVic) {
    case 1: return 95;
    case 2: return 94;
    case 3: return 93;
    case 4: return 98;
    default: return 0;
  }
}
