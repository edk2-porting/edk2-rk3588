/** @file

  Copyright (c) 2022 Rockchip Electronics Co. Ltd.
  Copyright (c) 2024-2025, Mario Bălănică <mariobalanica02@gmail.com>

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

UINT32
DisplayModeVRefresh (
  IN CONST DISPLAY_MODE  *DisplayMode
  )
{
  DRM_DISPLAY_MODE  DrmDisplayMode = { 0 };

  DisplayModeToDrm (DisplayMode, &DrmDisplayMode);

  return DrmDisplayMode.VRefresh;
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

CHAR8 *
GetVopOutputIfName (
  IN UINT32  OutputInterface
  )
{
  switch (OutputInterface) {
    case VOP_OUTPUT_IF_RGB:    return "RGB";
    case VOP_OUTPUT_IF_BT1120: return "BT1120";
    case VOP_OUTPUT_IF_BT656:  return "BT656";
    case VOP_OUTPUT_IF_LVDS0:  return "LVDS0";
    case VOP_OUTPUT_IF_LVDS1:  return "LVDS1";
    case VOP_OUTPUT_IF_MIPI0:  return "MIPI0";
    case VOP_OUTPUT_IF_MIPI1:  return "MIPI1";
    case VOP_OUTPUT_IF_EDP0:   return "EDP0";
    case VOP_OUTPUT_IF_EDP1:   return "EDP1";
    case VOP_OUTPUT_IF_DP0:    return "DP0";
    case VOP_OUTPUT_IF_DP1:    return "DP1";
    case VOP_OUTPUT_IF_HDMI0:  return "HDMI0";
    case VOP_OUTPUT_IF_HDMI1:  return "HDMI1";
    default:                   return "Unknown";
  }
}

STATIC
BOOLEAN
IsEdidBlockChecksumValid (
  IN UINT8  *EdidBlock
  )
{
  UINT8   Checksum = 0;
  UINT32  Index;

  for (Index = 0; Index < EDID_BLOCK_SIZE; Index++) {
    Checksum += EdidBlock[Index];
  }

  return (Checksum == 0);
}

EFI_STATUS
CheckEdidBlock (
  IN UINT8  *EdidBlock,
  IN UINT8  BlockIndex
  )
{
  STATIC CONST UINT8  ExpectedEdidHeader[EDID_HEADER_SIZE] = {
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00
  };

  if ((BlockIndex == 0) &&
      (CompareMem (EdidBlock, ExpectedEdidHeader, EDID_HEADER_SIZE) != 0))
  {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Invalid header for EDID block %u\n",
      __func__,
      BlockIndex
      ));
    return EFI_CRC_ERROR;
  }

  if (!IsEdidBlockChecksumValid (EdidBlock)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Invalid checksum for EDID block %u\n",
      __func__,
      BlockIndex
      ));

    //
    // Workaround for some KVM switches which update the
    // CEA-861 extension block without fixing its checksum.
    //
    if ((BlockIndex > 0) && (EdidBlock[0] == EDID_EXTENSION_CEA_861)) {
      DEBUG ((
        DEBUG_WARN,
        "%a:  (ignoring for CEA-861 extension block)\n",
        __func__
        ));
    } else {
      return EFI_CRC_ERROR;
    }
  }

  return EFI_SUCCESS;
}

VOID
DebugPrintEdid (
  IN UINT8  *Edid
  )
{
  UINT32  BlockCount;
  UINT32  BlockIndex;
  UINT32  Index;

  if (!DebugPrintEnabled () || !DebugPrintLevelEnabled (DEBUG_INFO)) {
    return;
  }

  BlockCount = EDID_GET_BLOCK_COUNT (Edid);

  DEBUG ((DEBUG_INFO, "EDID with %u blocks:\n", BlockCount));

  for (BlockIndex = 0; BlockIndex < BlockCount; BlockIndex++) {
    for (Index = 0; Index < EDID_BLOCK_SIZE; Index++) {
      DEBUG ((DEBUG_INFO, "%02x ", Edid[(BlockIndex * EDID_BLOCK_SIZE) + Index]));

      if ((Index + 1) % 16 == 0) {
        DEBUG ((DEBUG_INFO, "\n"));
      }
    }

    DEBUG ((DEBUG_INFO, "\n"));
  }
}

VOID
DebugPrintDisplayMode (
  IN CONST DISPLAY_MODE  *DisplayMode,
  IN UINT32              Indent,
  IN BOOLEAN             PrintVic,
  IN BOOLEAN             PrintTimings
  )
{
  if (!DebugPrintEnabled () || !DebugPrintLevelEnabled (DEBUG_INFO)) {
    return;
  }

  DEBUG ((
    DEBUG_INFO,
    "%*a%u x %u @ %u Hz",
    Indent,
    "",
    DisplayMode->HActive,
    DisplayMode->VActive,
    DisplayModeVRefresh (DisplayMode)
    ));

  if (PrintVic && (DisplayMode->Vic != 0)) {
    DEBUG ((DEBUG_INFO, " (VIC %u)", DisplayMode->Vic));
  }

  if (PrintTimings) {
    DEBUG ((DEBUG_INFO, "\n"));

    DEBUG ((
      DEBUG_INFO,
      "%*aOscFreq: %u  ClkActive: %u  DenActive: %u\n",
      Indent,
      "",
      DisplayMode->OscFreq,
      DisplayMode->ClkActive,
      DisplayMode->DenActive
      ));
    DEBUG ((
      DEBUG_INFO,
      "%*aHFrontPorch: %4u  HSync: %4u  HBackPorch: %4u  HSyncActive: %u\n",
      Indent,
      "",
      DisplayMode->HFrontPorch,
      DisplayMode->HSync,
      DisplayMode->HBackPorch,
      DisplayMode->HSyncActive
      ));
    DEBUG ((
      DEBUG_INFO,
      "%*aVFrontPorch: %4u  VSync: %4u  VBackPorch: %4u  VSyncActive: %u\n",
      Indent,
      "",
      DisplayMode->VFrontPorch,
      DisplayMode->VSync,
      DisplayMode->VBackPorch,
      DisplayMode->VSyncActive
      ));
  }
}

VOID
DebugPrintDisplaySinkInfo (
  IN CONST DISPLAY_SINK_INFO  *SinkInfo,
  IN UINT32                   Indent
  )
{
  if (!DebugPrintEnabled () || !DebugPrintLevelEnabled (DEBUG_INFO)) {
    return;
  }

  DEBUG ((DEBUG_INFO, "%*aIsHdmi:                  %a\n", Indent, "", SinkInfo->IsHdmi ? "TRUE" : "FALSE"));
  DEBUG ((DEBUG_INFO, "%*aHdmiInfo:\n", Indent, ""));
  DEBUG ((DEBUG_INFO, "%*a  Hdmi20Supported:       %a\n", Indent, "", SinkInfo->HdmiInfo.Hdmi20Supported ? "TRUE" : "FALSE"));
  DEBUG ((DEBUG_INFO, "%*a  Hdmi20SpeedLimited:    %a\n", Indent, "", SinkInfo->HdmiInfo.Hdmi20SpeedLimited ? "TRUE" : "FALSE"));
  DEBUG ((DEBUG_INFO, "%*a  ScdcSupported:         %a\n", Indent, "", SinkInfo->HdmiInfo.ScdcSupported ? "TRUE" : "FALSE"));
  DEBUG ((DEBUG_INFO, "%*aSelectableRgbRange:      %a\n", Indent, "", SinkInfo->SelectableRgbRange ? "TRUE" : "FALSE"));
  DEBUG ((DEBUG_INFO, "%*aPreferredMode:\n", Indent, ""));
  DebugPrintDisplayMode (&SinkInfo->PreferredMode, 4, TRUE, TRUE);
}
