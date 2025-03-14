/** @file

  Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "LcdGraphicsOutputDxe.h"

//
// Standard timing defined by VESA EDID
//
CONST EDID_TIMING  mEstablishedTimings[EDID_NUMBER_OF_ESTABLISHED_TIMINGS_BYTES][8] = {
  //
  // Established Timing I
  //
  {
    { 800,  600,  60 },
    { 800,  600,  56 },
    { 640,  480,  75 },
    { 640,  480,  72 },
    { 640,  480,  67 },
    { 640,  480,  60 },
    { 720,  400,  88 },
    { 720,  400,  70 },
  },
  {
    //
    // Established Timing II
    //
    { 1280, 1024, 75 },
    { 1024, 768,  75 },
    { 1024, 768,  70 },
    { 1024, 768,  60 },
    { 1024, 768,  87 },
    { 832,  624,  75 },
    { 800,  600,  75 },
    { 800,  600,  72 },
  },
  //
  // Established Timing III
  //
  {
    { 1152, 870,  75 },
    { 0,    0,    0  },
    { 0,    0,    0  },
    { 0,    0,    0  },
    { 0,    0,    0  },
    { 0,    0,    0  },
    { 0,    0,    0  },
    { 0,    0,    0  },
  }
};

#define CEA_DATA_OFFSET  4
#define CEA_DATA_BLOCK_TAG(block)             ((((UINT8 *)(block))[0x00] >> 5) & 0x7)
#define CEA_DATA_BLOCK_PAYLOAD_LENGTH(block)  ((((UINT8 *)(block))[0x00]) & 0x1f)
#define CEA_DATA_BLOCK_PAYLOAD(block)         (((UINT8 *)(block)) + 0x01)

#define CEA_DATA_BLOCK_VIDEO     0x02
#define CEA_DATA_BLOCK_VENDOR    0x03
#define CEA_DATA_BLOCK_EXTENDED  0x07

#define CEA_DATA_BLOCK_EXTENDED_VCDB     0x00
#define CEA_DATA_BLOCK_EXTENDED_Y420VDB  0x0E
#define CEA_DATA_BLOCK_EXTENDED_HF_SCDB  0x79

#define IEEE_OUI_HDMI        0x000c03
#define IEEE_OUI_HDMI_FORUM  0xc45dd8

STATIC
BOOLEAN
CeaIsVideoDataBlock (
  IN UINT8  *DataBlock
  )
{
  return (CEA_DATA_BLOCK_TAG (DataBlock) == CEA_DATA_BLOCK_VIDEO);
}

STATIC
BOOLEAN
CeaIsVendorDataBlock (
  IN UINT8   *DataBlock,
  IN UINT32  Oui
  )
{
  UINT32  DataBlockOui;

  if (CEA_DATA_BLOCK_TAG (DataBlock) != CEA_DATA_BLOCK_VENDOR) {
    return FALSE;
  }

  if (CEA_DATA_BLOCK_PAYLOAD_LENGTH (DataBlock) < 3) {
    return FALSE;
  }

  DataBlockOui = (DataBlock[3] << 16) | (DataBlock[2] << 8) | DataBlock[1];

  if (DataBlockOui != Oui) {
    return FALSE;
  }

  return TRUE;
}

STATIC
BOOLEAN
CeaIsExtendedDataBlock (
  IN UINT8  *DataBlock,
  IN UINT8  Tag
  )
{
  if (CEA_DATA_BLOCK_TAG (DataBlock) != CEA_DATA_BLOCK_EXTENDED) {
    return FALSE;
  }

  if (CEA_DATA_BLOCK_PAYLOAD_LENGTH (DataBlock) < 1) {
    return FALSE;
  }

  if (CEA_DATA_BLOCK_PAYLOAD (DataBlock)[0] != Tag) {
    return FALSE;
  }

  return TRUE;
}

STATIC
BOOLEAN
CeaIsHdmiVsdbDataBlock (
  IN UINT8  *DataBlock
  )
{
  if (!CeaIsVendorDataBlock (DataBlock, IEEE_OUI_HDMI)) {
    return FALSE;
  }

  if (CEA_DATA_BLOCK_PAYLOAD_LENGTH (DataBlock) < 5) {
    return FALSE;
  }

  return TRUE;
}

STATIC
BOOLEAN
CeaIsVcdbDataBlock (
  IN UINT8  *DataBlock
  )
{
  if (!CeaIsExtendedDataBlock (DataBlock, CEA_DATA_BLOCK_EXTENDED_VCDB)) {
    return FALSE;
  }

  if (CEA_DATA_BLOCK_PAYLOAD_LENGTH (DataBlock) < 2) {
    return FALSE;
  }

  return TRUE;
}

STATIC
BOOLEAN
CeaIsY420VdbDataBlock (
  IN UINT8  *DataBlock
  )
{
  return CeaIsExtendedDataBlock (DataBlock, CEA_DATA_BLOCK_EXTENDED_Y420VDB);
}

STATIC
BOOLEAN
CeaIsHdmiForumScdsDataBlock (
  IN UINT8  *DataBlock
  )
{
  if (!CeaIsVendorDataBlock (DataBlock, IEEE_OUI_HDMI_FORUM) &&
      !CeaIsExtendedDataBlock (DataBlock, CEA_DATA_BLOCK_EXTENDED_HF_SCDB))
  {
    return FALSE;
  }

  if (CEA_DATA_BLOCK_PAYLOAD_LENGTH (DataBlock) < 7) {
    return FALSE;
  }

  return TRUE;
}

STATIC
UINT8
CeaSvdToVic (
  IN UINT8  Svd
  )
{
  if (((Svd >= 1) && (Svd <= 64)) || ((Svd >= 129) && (Svd <= 192))) {
    return Svd & 0x7f;
  }

  return Svd;
}

STATIC
UINT8
CeaSvdIsNative (
  IN UINT8  Svd
  )
{
  if (((Svd >= 1) && (Svd <= 64)) || ((Svd >= 129) && (Svd <= 192))) {
    return (Svd & 0x80) != 0;
  }

  return FALSE;
}

STATIC
EFI_STATUS
EdidGetFeaturesCea (
  IN CONNECTOR_STATE  *ConnectorState
  )
{
  UINT8              *Edid;
  UINT8              BlockIndex;
  UINT8              BlockCount;
  UINT8              DataBlocksEnd;
  UINT8              DataBlockOffset;
  UINT8              DataBlockLength;
  UINT8              *DataBlock;
  DISPLAY_SINK_INFO  *SinkInfo;

  SinkInfo = &ConnectorState->SinkInfo;

  BlockCount = EDID_GET_BLOCK_COUNT (ConnectorState->Edid);

  for (BlockIndex = 1; BlockIndex < BlockCount; BlockIndex++) {
    Edid = EDID_BLOCK (ConnectorState->Edid, BlockIndex);

    if ((Edid[0x00] != EDID_EXTENSION_CEA_861) || (Edid[0x01] < 3)) {
      continue;
    }

    DataBlocksEnd = Edid[0x02];
    if (DataBlocksEnd > EDID_BLOCK_SIZE - 1) {
      continue;
    }

    for (DataBlockOffset = CEA_DATA_OFFSET; DataBlockOffset < DataBlocksEnd;) {
      DataBlock       = Edid + DataBlockOffset;
      DataBlockLength = 1 + CEA_DATA_BLOCK_PAYLOAD_LENGTH (DataBlock);

      if (DataBlockOffset + DataBlockLength > DataBlocksEnd) {
        break;
      }

      if (CeaIsVcdbDataBlock (DataBlock)) {
        if (DataBlock[0x02] & BIT6) {
          SinkInfo->SelectableRgbRange = TRUE;
        }
      } else if (CeaIsHdmiVsdbDataBlock (DataBlock)) {
        SinkInfo->IsHdmi = TRUE;
      } else if (CeaIsHdmiForumScdsDataBlock (DataBlock)) {
        SinkInfo->HdmiInfo.Hdmi20Supported = TRUE;

        if ((DataBlock[0x05] * 5) < 594) {
          SinkInfo->HdmiInfo.Hdmi20SpeedLimited = TRUE;
        }

        if (DataBlock[0x06] & BIT7) {
          SinkInfo->HdmiInfo.ScdcSupported = TRUE;
        }
      } else if (CeaIsY420VdbDataBlock (DataBlock)) {
        //
        // Determine whether this is a full HDMI 2.0 sink or not.
        // YCC420-only modes are driven at half their pixel clock.
        // If a mode with clock rate between 340 and 600 MHz is listed
        // here, it means the sink is limited to HDMI 1.4 bandwidth.
        //
        UINT8  *Svds     = CEA_DATA_BLOCK_PAYLOAD (DataBlock) + 1;
        UINT8  SvdsCount = CEA_DATA_BLOCK_PAYLOAD_LENGTH (DataBlock) - 1;

        for (UINT8 SvdIndex = 0; SvdIndex < SvdsCount; SvdIndex++) {
          if (SinkInfo->HdmiInfo.Hdmi20SpeedLimited) {
            break;
          }

          UINT8  Vic = CeaSvdToVic (Svds[SvdIndex]);

          switch (Vic) {
            case 91:    // 2560x1080p @ 100Hz (371.25 MHz)
            case 92:    // 2560x1080p @ 120Hz (495 MHz)
            case 96:    // 3840x2160p @ 50Hz  (594 MHz)
            case 97:    // 3840x2160p @ 60Hz  (594 MHz)
            case 101:   // 4096x2160p @ 50Hz  (594 MHz)
            case 102:   // 4096x2160p @ 60Hz  (594 MHz)
            case 106:   // 3840x2160p @ 50Hz  (594 MHz)
            case 107:   // 3840x2160p @ 60Hz  (594 MHz)
            case 114:   // 3840x2160p @ 48Hz  (594 MHz)
            case 115:   // 4096x2160p @ 48Hz  (594 MHz)
            case 116:   // 3840x2160p @ 48Hz  (594 MHz)
            case 121:   // 5120x2160p @ 24Hz  (396 MHz)
            case 122:   // 5120x2160p @ 25Hz  (396 MHz)
            case 123:   // 5120x2160p @ 30Hz  (396 MHz)
              SinkInfo->HdmiInfo.Hdmi20SpeedLimited = TRUE;
              break;
            default:
              DEBUG ((DEBUG_WARN, "%a: Y420VDB: Unexpected VIC %u\n", __func__, Vic));
              break;
          }
        }
      }

      DataBlockOffset += DataBlockLength;
    }
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EdidGetFeatures (
  IN CONNECTOR_STATE  *ConnectorState
  )
{
  return EdidGetFeaturesCea (ConnectorState);
}

STATIC
VOID
EdidUpdatePreferredMode (
  IN CONNECTOR_STATE     *ConnectorState,
  IN CONST DISPLAY_MODE  *DisplayMode
  )
{
  CopyMem (
    &ConnectorState->SinkInfo.PreferredMode,
    DisplayMode,
    sizeof (DISPLAY_MODE)
    );
}

STATIC
EFI_STATUS
EdidDetailedTimingToDisplayMode (
  IN  EDID_DETAILED_TIMING  *DetailedTiming,
  OUT DISPLAY_MODE          *DisplayMode
  )
{
  if (DetailedTiming->PixelClock == 0) {
    return EFI_INVALID_PARAMETER;
  }

  if ((DetailedTiming->Features & EdidDetailedTimingFeatureInterlaced) != 0) {
    return EFI_UNSUPPORTED;
  }

  if ((DetailedTiming->Features & EdidDetailedTimingFeatureStereoModeMask) != 0) {
    return EFI_UNSUPPORTED;
  }

  DisplayMode->Vic = 0;

  DisplayMode->OscFreq =
    (UINT32)DetailedTiming->PixelClock * EDID_DETAILED_TIMING_CLOCK_RESOLUTION;

  DisplayMode->HActive =
    ((DetailedTiming->HActiveHiBlankingHi & 0xF0) << 4) | DetailedTiming->HActiveLo;

  DisplayMode->HFrontPorch =
    ((DetailedTiming->HSyncOffsetHiHSyncWidthHiVSyncOffsetHiSyncWidthHi & 0xC0) << 2) |
    DetailedTiming->HSyncOffsetLo;

  DisplayMode->HSync =
    ((DetailedTiming->HSyncOffsetHiHSyncWidthHiVSyncOffsetHiSyncWidthHi & 0x30) << 4) |
    DetailedTiming->HSyncWidthLo;

  DisplayMode->HBackPorch =
    (((DetailedTiming->HActiveHiBlankingHi & 0x0F) << 8) | DetailedTiming->HBlankingLo) -
    (DisplayMode->HFrontPorch + DisplayMode->HSync);

  DisplayMode->HSyncActive =
    (DetailedTiming->Features & EdidDetailedTimingFeatureHorizontalSyncPositive) != 0;

  DisplayMode->VActive =
    ((DetailedTiming->VActiveHiBlankingHi & 0xF0) << 4) | DetailedTiming->VActiveLo;

  DisplayMode->VFrontPorch =
    ((DetailedTiming->HSyncOffsetHiHSyncWidthHiVSyncOffsetHiSyncWidthHi & 0x0C) << 2) |
    (DetailedTiming->VSyncOffsetLoSyncWidthLo & 0xF0) >> 4;

  DisplayMode->VSync =
    ((DetailedTiming->HSyncOffsetHiHSyncWidthHiVSyncOffsetHiSyncWidthHi & 0x03) << 4) |
    (DetailedTiming->VSyncOffsetLoSyncWidthLo & 0x0F);

  DisplayMode->VBackPorch =
    (((DetailedTiming->VActiveHiBlankingHi & 0x0F) << 8) | DetailedTiming->VBlankingLo) -
    (DisplayMode->VFrontPorch + DisplayMode->VSync);

  DisplayMode->VSyncActive =
    (DetailedTiming->Features & EdidDetailedTimingFeatureVerticalSyncPositive) != 0;

  DisplayMode->DenActive = 0;
  DisplayMode->ClkActive = 0;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EdidDetailedTimingToDisplayModeMatch (
  IN  EDID_DETAILED_TIMING  *DetailedTiming,
  OUT DISPLAY_MODE          *DisplayMode
  )
{
  EFI_STATUS          Status;
  CONST DISPLAY_MODE  *PredefinedMode;

  Status = EdidDetailedTimingToDisplayMode (DetailedTiming, DisplayMode);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  PredefinedMode = MatchPredefinedDisplayMode (
                     DisplayMode,
                     (EDID_DETAILED_TIMING_CLOCK_RESOLUTION / 2)
                     );
  if (PredefinedMode != NULL) {
    DisplayMode->Vic = PredefinedMode->Vic;

    if (DisplayMode->OscFreq != PredefinedMode->OscFreq) {
      DisplayMode->OscFreq = PredefinedMode->OscFreq;
    }
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
EdidGetPreferredModeDetailed (
  IN CONNECTOR_STATE  *ConnectorState
  )
{
  EFI_STATUS            Status;
  UINT8                 *Edid;
  UINT8                 BlockIndex;
  UINT8                 BlockCount;
  UINT8                 Index;
  EDID_DETAILED_TIMING  *DetailedTimings;
  UINT8                 DetailedTimingsCount;
  DISPLAY_MODE          DisplayMode;

  BlockCount = EDID_GET_BLOCK_COUNT (ConnectorState->Edid);

  for (BlockIndex = 0; BlockIndex < BlockCount; BlockIndex++) {
    Edid = EDID_BLOCK (ConnectorState->Edid, BlockIndex);

    if (BlockIndex == 0) {
      DetailedTimings      = ((EDID_BASE *)Edid)->DetailedTimings;
      DetailedTimingsCount = EDID_NUMBER_OF_DETAILED_TIMINGS;
    } else {
      switch (Edid[0x00]) {
        case EDID_EXTENSION_CEA_861:
          UINT8  DtdOffset = Edid[0x02];
          UINT8  DtdEnd    = EDID_BLOCK_SIZE - 1;

          if ((DtdOffset < CEA_DATA_OFFSET) || (DtdOffset >= DtdEnd)) {
            continue;
          }

          DetailedTimings      = (EDID_DETAILED_TIMING *)(Edid + DtdOffset);
          DetailedTimingsCount = (DtdEnd - DtdOffset) / sizeof (EDID_DETAILED_TIMING);
          break;
        case EDID_EXTENSION_VTB:
          if (Edid[0x01] != 1) {
            continue;
          }

          DetailedTimings      = (EDID_DETAILED_TIMING *)(Edid + 0x05);
          DetailedTimingsCount = MIN (Edid[0x02], 6);
          break;
        default:
          continue;
      }
    }

    for (Index = 0; Index < DetailedTimingsCount; Index++) {
      DEBUG ((DEBUG_INFO, "%a: Block %u: DTD %u: ", __func__, BlockIndex, Index));

      Status = EdidDetailedTimingToDisplayModeMatch (
                 &DetailedTimings[Index],
                 &DisplayMode
                 );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "- ignoring (%r)\n", Status));
        continue;
      }

      DebugPrintDisplayMode (&DisplayMode, 0, TRUE, FALSE);

      if (!IsDisplayModeSupported (ConnectorState, &DisplayMode)) {
        DEBUG ((DEBUG_INFO, " - ignoring (Unsupported)\n"));
        continue;
      }

      DEBUG ((DEBUG_INFO, "\n"));

      EdidUpdatePreferredMode (ConnectorState, &DisplayMode);
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

STATIC
CONST DISPLAY_MODE *
CeaVideoDataBlockGetFirstSupportedMode (
  IN  CONNECTOR_STATE  *ConnectorState,
  IN  UINT8            *DataBlock,
  OUT BOOLEAN          *IsNative
  )
{
  UINT8               *Svds;
  UINT8               SvdsCount;
  UINT8               Index;
  UINT8               Vic;
  CONST DISPLAY_MODE  *PredefinedMode;

  Svds      = CEA_DATA_BLOCK_PAYLOAD (DataBlock);
  SvdsCount = CEA_DATA_BLOCK_PAYLOAD_LENGTH (DataBlock);

  for (Index = 0; Index < SvdsCount; Index++) {
    Vic = CeaSvdToVic (Svds[Index]);

    if (Vic == 0) {
      continue;
    }

    DEBUG ((DEBUG_INFO, "%a: VIC %u: ", __func__, Vic));

    PredefinedMode = GetPredefinedDisplayModeByVic (Vic);
    if (PredefinedMode == NULL) {
      DEBUG ((DEBUG_INFO, "- ignoring (Not Found)\n"));
      continue;
    }

    DebugPrintDisplayMode (PredefinedMode, 0, FALSE, FALSE);

    if (!IsDisplayModeSupported (ConnectorState, PredefinedMode)) {
      DEBUG ((DEBUG_INFO, " - ignoring (Unsupported)\n"));
      continue;
    }

    if (IsNative != NULL) {
      *IsNative = CeaSvdIsNative (Svds[Index]);

      if (*IsNative) {
        DEBUG ((DEBUG_INFO, " (Native)"));
      }
    }

    DEBUG ((DEBUG_INFO, "\n"));

    return PredefinedMode;
  }

  return NULL;
}

STATIC
CONST DISPLAY_MODE *
CeaHdmiVsdbDataBlockGetFirstSupportedMode (
  IN CONNECTOR_STATE  *ConnectorState,
  IN UINT8            *DataBlock
  )
{
  UINT8               VsdbLength;
  UINT8               VicsOffset;
  UINT8               VicsCount;
  UINT8               Index;
  UINT8               Vic;
  CONST DISPLAY_MODE  *PredefinedMode;

  VsdbLength = CEA_DATA_BLOCK_PAYLOAD_LENGTH (DataBlock);

  if ((VsdbLength < 8) || ((DataBlock[8] & BIT5) == 0)) {
    return NULL;
  }

  VicsOffset = 15;

  if ((DataBlock[8] & BIT7) == 0) {
    VicsOffset -= 2;
  }

  if ((DataBlock[8] & BIT6) == 0) {
    VicsOffset -= 2;
  }

  if (VsdbLength < VicsOffset) {
    return NULL;
  }

  VicsCount = DataBlock[VicsOffset - 1] >> 5;

  if (VicsOffset + VicsCount > VsdbLength + 1) {
    return NULL;
  }

  for (Index = 0; Index < VicsCount; Index++) {
    Vic = ConvertHdmiToCeaVic (DataBlock[VicsOffset + Index]);

    if (Vic == 0) {
      continue;
    }

    DEBUG ((DEBUG_INFO, "%a: VIC %u: ", __func__, Vic));

    PredefinedMode = GetPredefinedDisplayModeByVic (Vic);
    if (PredefinedMode == NULL) {
      DEBUG ((DEBUG_INFO, "- ignoring (Not Found)\n"));
      continue;
    }

    DebugPrintDisplayMode (PredefinedMode, 0, FALSE, FALSE);

    if (!IsDisplayModeSupported (ConnectorState, PredefinedMode)) {
      DEBUG ((DEBUG_INFO, " - ignoring (Unsupported)\n"));
      continue;
    }

    DEBUG ((DEBUG_INFO, "\n"));

    return PredefinedMode;
  }

  return NULL;
}

STATIC
EFI_STATUS
EFIAPI
EdidGetPreferredModeCea (
  IN CONNECTOR_STATE  *ConnectorState
  )
{
  UINT8               *Edid;
  UINT8               BlockIndex;
  UINT8               BlockCount;
  UINT8               DataBlocksEnd;
  UINT8               DataBlockOffset;
  UINT8               DataBlockLength;
  UINT8               *DataBlock;
  CONST DISPLAY_MODE  *CeaMode = NULL;
  BOOLEAN             CeaModeIsNative;
  CONST DISPLAY_MODE  *HdmiMode      = NULL;
  CONST DISPLAY_MODE  *PreferredMode = NULL;

  BlockCount = EDID_GET_BLOCK_COUNT (ConnectorState->Edid);

  for (BlockIndex = 1; BlockIndex < BlockCount; BlockIndex++) {
    Edid = EDID_BLOCK (ConnectorState->Edid, BlockIndex);

    if ((Edid[0x00] != EDID_EXTENSION_CEA_861) || (Edid[0x01] < 3)) {
      continue;
    }

    DataBlocksEnd = Edid[0x02];
    if (DataBlocksEnd > EDID_BLOCK_SIZE - 1) {
      continue;
    }

    for (DataBlockOffset = CEA_DATA_OFFSET; DataBlockOffset < DataBlocksEnd;) {
      DataBlock       = Edid + DataBlockOffset;
      DataBlockLength = 1 + CEA_DATA_BLOCK_PAYLOAD_LENGTH (DataBlock);

      if (DataBlockOffset + DataBlockLength > DataBlocksEnd) {
        break;
      }

      if (CeaIsVideoDataBlock (DataBlock)) {
        if (CeaMode == NULL) {
          CeaMode = CeaVideoDataBlockGetFirstSupportedMode (
                      ConnectorState,
                      DataBlock,
                      &CeaModeIsNative
                      );
        }
      } else if (CeaIsHdmiVsdbDataBlock (DataBlock)) {
        if (HdmiMode == NULL) {
          HdmiMode = CeaHdmiVsdbDataBlockGetFirstSupportedMode (
                       ConnectorState,
                       DataBlock
                       );
        }
      } else {
        goto NextDataBlock;
      }

      if ((CeaMode != NULL) && (HdmiMode != NULL)) {
        goto Exit;
      }

NextDataBlock:
      DataBlockOffset += DataBlockLength;
    }
  }

Exit:
  //
  // Prefer the HDMI mode if the CEA one is neither native nor larger.
  //
  if ((HdmiMode != NULL) &&
      ((CeaMode == NULL) ||
       (!CeaModeIsNative &&
        ((HdmiMode->HActive * HdmiMode->VActive > CeaMode->HActive * CeaMode->VActive) ||
         ((HdmiMode->HActive * HdmiMode->VActive == CeaMode->HActive * CeaMode->VActive) &&
          (HdmiMode->OscFreq > CeaMode->OscFreq))))))
  {
    PreferredMode = HdmiMode;
  } else if (CeaMode != NULL) {
    PreferredMode = CeaMode;
  }

  if (PreferredMode != NULL) {
    DEBUG ((
      DEBUG_INFO,
      "%a: %a: ",
      __func__,
      PreferredMode == HdmiMode ? "HDMI" : "CEA"
      ));
    DebugPrintDisplayMode (PreferredMode, 0, TRUE, FALSE);
    DEBUG ((DEBUG_INFO, "\n"));

    EdidUpdatePreferredMode (ConnectorState, PreferredMode);
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

STATIC
EFI_STATUS
EdidParseStandardTiming (
  IN  EDID_STANDARD_TIMING  *StandardTiming,
  OUT UINT16                *HorizontalResolution,
  OUT UINT16                *VerticalResolution,
  OUT UINT8                 *RefreshRate
  )
{
  UINT8  AspectRatio;

  if (((StandardTiming->HorizontalActivePixels == 0x00) &&
       (StandardTiming->ImageAspectRatioAndrefresh == 0x00)) ||
      ((StandardTiming->HorizontalActivePixels == 0x01) &&
       (StandardTiming->ImageAspectRatioAndrefresh == 0x01)) ||
      ((StandardTiming->HorizontalActivePixels == 0x20) &&
       (StandardTiming->ImageAspectRatioAndrefresh == 0x20)))
  {
    *HorizontalResolution = 0;
    *VerticalResolution   = 0;
    *RefreshRate          = 0;
    return EFI_INVALID_PARAMETER;
  }

  *HorizontalResolution = (StandardTiming->HorizontalActivePixels + 31) * 8;

  AspectRatio = (StandardTiming->ImageAspectRatioAndrefresh >> 6) & 0x3;

  switch (AspectRatio) {
    case 0: *VerticalResolution = (*HorizontalResolution * 10) / 16;
      break;
    case 1: *VerticalResolution = (*HorizontalResolution * 3) / 4;
      break;
    case 2: *VerticalResolution = (*HorizontalResolution * 4) / 5;
      break;
    case 3: *VerticalResolution = (*HorizontalResolution * 9) / 16;
      break;
    default:
      break;
  }

  // WORKAROUND - 1360x768/1366x768 is not a perfect aspect ratio
  if (((*HorizontalResolution == 1360) && (*VerticalResolution == 765)) ||
      ((*HorizontalResolution == 1368) && (*VerticalResolution == 769)))
  {
    *HorizontalResolution = 1360;
    *VerticalResolution   = 768;
  }

  *RefreshRate = (StandardTiming->ImageAspectRatioAndrefresh & 0x1F) + 60;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
EdidGetPreferredModeStandard (
  IN CONNECTOR_STATE  *ConnectorState
  )
{
  EFI_STATUS          Status;
  EDID_BASE           *Edid;
  UINT8               Index;
  CONST DISPLAY_MODE  *PredefinedMode;
  UINT16              HorizontalResolution;
  UINT16              VerticalResolution;
  UINT8               RefreshRate;

  Edid = (EDID_BASE *)EDID_BLOCK (ConnectorState->Edid, 0);

  for (Index = 0; Index < EDID_NUMBER_OF_STANDARD_TIMINGS; Index++) {
    DEBUG ((DEBUG_INFO, "%a: STD %u: ", __func__, Index));

    Status = EdidParseStandardTiming (
               &Edid->StandardTimings[Index],
               &HorizontalResolution,
               &VerticalResolution,
               &RefreshRate
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "- ignoring (%r)\n", Status));
      continue;
    }

    PredefinedMode = GetPredefinedDisplayModeByResolution (
                       HorizontalResolution,
                       VerticalResolution,
                       RefreshRate
                       );
    if (PredefinedMode == NULL) {
      DEBUG ((DEBUG_INFO, "- ignoring (Not Found)\n"));
      continue;
    }

    DebugPrintDisplayMode (PredefinedMode, 0, TRUE, FALSE);

    if (!IsDisplayModeSupported (ConnectorState, PredefinedMode)) {
      DEBUG ((DEBUG_INFO, " - ignoring (Unsupported)\n"));
      continue;
    }

    DEBUG ((DEBUG_INFO, "\n"));

    EdidUpdatePreferredMode (ConnectorState, PredefinedMode);
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

STATIC
BOOLEAN
EdidIsEstablishedModeSupported (
  IN UINT8   EstablishedTimings[EDID_NUMBER_OF_ESTABLISHED_TIMINGS_BYTES],
  IN UINT16  HorizontalResolution,
  IN UINT16  VerticalResolution,
  IN UINT8   RefreshRate
  )
{
  UINT8              EstByte;
  UINT8              EstBit;
  CONST EDID_TIMING  *EstTiming;

  for (EstByte = 0; EstByte < EDID_NUMBER_OF_ESTABLISHED_TIMINGS_BYTES; EstByte++) {
    for (EstBit = 0; EstBit < 8; EstBit++) {
      if ((EstablishedTimings[EstByte] & (1 << EstBit)) == 0) {
        continue;
      }

      EstTiming = &mEstablishedTimings[EstByte][EstBit];

      if ((EstTiming->HorizontalResolution == HorizontalResolution) &&
          (EstTiming->VerticalResolution == VerticalResolution) &&
          (EstTiming->RefreshRate == RefreshRate))
      {
        return TRUE;
      }
    }
  }

  return FALSE;
}

STATIC
EFI_STATUS
EFIAPI
EdidGetPreferredModeEstablished (
  IN CONNECTOR_STATE  *ConnectorState
  )
{
  EDID_BASE           *Edid;
  INT32               Index;
  CONST DISPLAY_MODE  *PredefinedMode;

  Edid = (EDID_BASE *)EDID_BLOCK (ConnectorState->Edid, 0);

  //
  // Prefer the largest mode (predefined list is in ascending order).
  //
  for (Index = GetPredefinedDisplayModesCount () - 1; Index >= 0; Index--) {
    PredefinedMode = GetPredefinedDisplayMode (Index);

    if (!EdidIsEstablishedModeSupported (
           Edid->EstablishedTimings,
           PredefinedMode->HActive,
           PredefinedMode->VActive,
           DisplayModeVRefresh (PredefinedMode)
           ))
    {
      continue;
    }

    DEBUG ((DEBUG_INFO, "%a: EST: ", __func__));
    DebugPrintDisplayMode (PredefinedMode, 0, TRUE, FALSE);

    if (!IsDisplayModeSupported (ConnectorState, PredefinedMode)) {
      DEBUG ((DEBUG_INFO, " - ignoring (Unsupported)\n"));
      continue;
    }

    DEBUG ((DEBUG_INFO, "\n"));

    EdidUpdatePreferredMode (ConnectorState, PredefinedMode);
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

typedef
EFI_STATUS
(EFIAPI *EDID_GET_PREFERRED_MODE)(
  IN CONNECTOR_STATE  *ConnectorState
  );

STATIC EDID_GET_PREFERRED_MODE  mEdidGetPreferredModeOps[] = {
  EdidGetPreferredModeDetailed,
  EdidGetPreferredModeCea,
  EdidGetPreferredModeStandard,
  EdidGetPreferredModeEstablished,
};

STATIC
EFI_STATUS
EdidGetPreferredMode (
  IN CONNECTOR_STATE  *ConnectorState
  )
{
  EFI_STATUS  Status;
  UINT32      Index;

  for (Index = 0; Index < ARRAY_SIZE (mEdidGetPreferredModeOps); Index++) {
    Status = mEdidGetPreferredModeOps[Index](ConnectorState);
    if (!EFI_ERROR (Status)) {
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
EdidGetDisplaySinkInfo (
  IN CONNECTOR_STATE  *ConnectorState
  )
{
  EFI_STATUS  Status;

  Status = EdidGetFeatures (ConnectorState);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Failed to get features. Status=%r\n",
      __func__,
      Status
      ));
    return Status;
  }

  Status = EdidGetPreferredMode (ConnectorState);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Failed to get preferred mode. Status=%r\n",
      __func__,
      Status
      ));
    return Status;
  }

  return EFI_SUCCESS;
}
