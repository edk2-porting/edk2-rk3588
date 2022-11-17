/** @file Edid.c
 * @brief Reads and parses the EDID, checks if a requested video mode is in the supplied EDID
 *
 * Copyright (c) 2018-2019, DisplayLink (UK) Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
**/

#include "UsbDisplayLink.h"
#include "Edid.h"

CONST UINT8 ExpectedEdidHeader[EDID_HEADER_SIZE] = { 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00 };

//
// Standard timing defined by VESA EDID
//
CONST EDID_TIMING EstablishedTimings[EDID_NUMBER_OF_ESTABLISHED_TIMINGS_BYTES][8] = {
  //
  // Established Timing I
  //
  {
    { 800, 600, 60 },
    { 800, 600, 56 },
    { 640, 480, 75 },
    { 640, 480, 72 },
    { 640, 480, 67 },
    { 640, 480, 60 },
    { 720, 400, 88 },
    { 720, 400, 70 },
  },
  {
    //
    // Established Timing II
    //
    { 1280, 1024, 75 },
    { 1024,  768, 75 },
    { 1024,  768, 70 },
    { 1024,  768, 60 },
    { 1024,  768, 87 },
    { 832,   624, 75 },
    { 800,   600, 75 },
    { 800,   600, 72 },
  },
  //
  // Established Timing III
  //
  {
    { 1152, 870, 75 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
  }
};

/**
 * Requests the monitor EDID data from the connected DisplayLink device
 * @param UsbDisplayLinkDev
 * @param EdidDataBlock
 * @param EdidSize
 * @retval EFI_DEVICE_ERROR - No EDID received, or EDID is corrupted
 * @retval EFI_OUT_OF_RESOURCES - Cannot allocate memory
 * @retval EFI_SUCCESS
 *
 */
STATIC EFI_STATUS
ReadEdidData (
  IN USB_DISPLAYLINK_DEV      *UsbDisplayLinkDev,
  OUT UINT8                    **EdidDataBlock,
  OUT UINTN                    *EdidSize
)
{
  EFI_STATUS Status;

  UINT8 EdidDataRead[EDID_BLOCK_SIZE];
  UINT8 *EdidData = EdidDataRead;
  UINT8* ValidEdid;

  Status = DlUsbSendControlReadMessage (UsbDisplayLinkDev, GET_OUTPUT_EDID, 0, EdidDataRead, sizeof (EdidDataRead));

  if (EFI_ERROR (Status) || (EdidData[0] != 0)) {
    DEBUG ((DEBUG_ERROR, "No monitor EDID received from DisplayLink device - System error %r, EDID error %d. Monitor connected correctly?\n", Status, EdidData[0]));
    return EFI_DEVICE_ERROR;
  } else {
    //
    // Search for the EDID signature
    //
    ValidEdid = &EdidData[0];
    CONST UINT64 Signature = 0x00ffffffffffff00ull;
    if (CompareMem (ValidEdid, &Signature, 8) != 0) {
      //
      // No EDID signature found
      //
      DEBUG ((DEBUG_ERROR, "Monitor EDID received from DisplayLink device did not have a valid signature - corrupted?\n"));
      Status = EFI_DEVICE_ERROR;
      return Status;
    }
  }

  *EdidDataBlock = (UINT8*)AllocateCopyPool (
    EDID_BLOCK_SIZE,
    ValidEdid);

  if (*EdidDataBlock == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Currently only support EDID 1.x
  //
  *EdidSize = EDID_BLOCK_SIZE;

  return EFI_SUCCESS;
}


/**
Calculates the mod256 checksum of the EDID and compares it with the one supplied at the end of the EDID
@param  EDID        Pointer to the 128-byte EDID
@retval TRUE        The EDID checksum is correct
**/
BOOLEAN
IsEdidChecksumCorrect (
    CONST VOID *EDID
    )
{
  CONST UINT8 EdidChecksum = ((UINT8 *)EDID)[EDID_BLOCK_SIZE - 1];
  UINT8 CalculatedChecksum;

  CalculatedChecksum = 0;

  UINTN i;
  for (i = 0; i < EDID_BLOCK_SIZE - 1; i++) {
    CalculatedChecksum += ((UINT8 *)EDID)[i];
  }
  CalculatedChecksum = 0 - CalculatedChecksum;

  return (CalculatedChecksum == EdidChecksum);
}

/**
Check if a particular video mode is in the Established Timings section of the EDID.

@param  EDID        Pointer to the 128-byte EDID
@param  hRes        Horizontal resolution
@param  vRes        Vertical resolution
@param  refresh     Refresh rate
@retval TRUE        The requested mode is present in the Established Timings section

**/
STATIC BOOLEAN
IsModeInEstablishedTimings (
    IN CONST VOID *EDID,
    IN UINT16 HRes,
    IN UINT16 VRes,
    IN UINT16 Refresh
    )
{
  CONST struct Edid *pEDID = (CONST struct Edid *)EDID;
  BOOLEAN ModeSupported;

  ModeSupported = FALSE;

  int EstByteNum;
  int BitNum;
  for (EstByteNum = 0; EstByteNum < EDID_NUMBER_OF_ESTABLISHED_TIMINGS_BYTES; EstByteNum++) {
    for (BitNum = 0; BitNum < 8; BitNum++) {
      if (pEDID->EstablishedTimings[EstByteNum] & (1 << BitNum)) { // The bit is set in the established timings of the EDID

        if ((EstablishedTimings[EstByteNum][BitNum].HRes == HRes) && // The passed-in resolution matches the resolution represented by the set bit
          (EstablishedTimings[EstByteNum][BitNum].VRes == VRes) &&
          (EstablishedTimings[EstByteNum][BitNum].Refresh == Refresh)) {

          ModeSupported = TRUE;
          break;
        }
      }
    }
    if (ModeSupported == TRUE) {
      break;
    }
  }
  return ModeSupported;
}

/**
Extract the resolutions and refresh rate from one of the entries in the Standard Timings section of the EDID.

@param  EDID          Pointer to the 128-byte EDID
@param  timingNumber  The entry that we want to extract
@param  hRes          Output - Horizontal resolution
@param  vRes          Output - Vertical resolution
@param  refresh       Output - Refresh rate
@retval TRUE          The requested Standard Timings entry contains valid data

**/
STATIC BOOLEAN ReadStandardTiming (
  CONST VOID *EDID,
  IN UINT8 TimingNumber,
  OUT UINT16 *HRes,
  OUT UINT16 *VRes,
  OUT UINT8 *Refresh)
{
  CONST struct Edid *pEDID = (CONST struct Edid *)EDID;

  // See section 3.9.1 of the VESA EDID spec

  if (((pEDID->standardTimingIdentifications[TimingNumber].HorizontalActivePixels) == 0x01) &&
    (pEDID->standardTimingIdentifications[TimingNumber].ImageAspectRatioAndrefresh) == 0x01) {
    *HRes = 0;
    *VRes = 0;
    *Refresh = 0;
    return FALSE;
  }
  *HRes = (pEDID->standardTimingIdentifications[TimingNumber].HorizontalActivePixels + 31) * 8;

  UINT8 AspectRatio;
  AspectRatio = (pEDID->standardTimingIdentifications[TimingNumber].ImageAspectRatioAndrefresh >> 6) & 0x3;

  switch (AspectRatio) {
  case 0: *VRes = (*HRes * 10) / 16;
    break;
  case 1: *VRes = (*HRes * 3) / 4;
    break;
  case 2: *VRes = (*HRes * 4) / 5;
    break;
  case 3: *VRes = (*HRes * 9) / 16;
    break;
  default: break;
  }

  // WORKAROUND - 1360x768 is not a perfect aspect ratio
  if ((*HRes == 1360) && (*VRes == 765)) {
    *VRes = 768;
  }

  *Refresh = (pEDID->standardTimingIdentifications[TimingNumber].ImageAspectRatioAndrefresh & 0x1F) + 60;

  return TRUE;
}

/**
Extract the resolutions and refresh rate from one of the entries in the Detailed Timings section of the EDID.

@param  EDID          Pointer to the 128-byte EDID
@param  timingNumber  The entry that we want to extract
@param  videoMode     Output - Filled in with details from the detailed timing
@retval TRUE          The requested Detailed Timings entry contains valid data

**/
STATIC BOOLEAN
ReadDetailedTiming (
  IN CONST VOID *EDID,
  IN UINT8 TimingNumber,
  OUT struct VideoMode *VideoMode
  )
{
  if (TimingNumber >= EDID_NUMBER_OF_DETAILED_TIMINGS) {
    return FALSE;
  }

  UINT16 NumValidDetailedTimingsFound;
  NumValidDetailedTimingsFound = 0;

  // Spin through the detailed timings until we find a valid one - then check if this has the index that we want
  int BlockNumber;
  for (BlockNumber = 0; BlockNumber < EDID_NUMBER_OF_DETAILED_TIMINGS; BlockNumber++) {
    CONST struct Edid *pEDID = (CONST struct Edid *)EDID;
    CONST struct DetailedTimingIdentification *pTiming = &pEDID->detailedTimingDescriptions[BlockNumber];

    if (((BlockNumber == 0) && (pTiming->PixelClock == EDID_DETAILED_TIMING_INVALID_PIXEL_CLOCK)) ||
      (pTiming->PixelClock == 0)) {
      // This is not a valid detailed timing descriptor
      continue;
    }

    if ((pTiming->Features & EdidDetailedTimingsFeaturesSyncSchemeMask) != EdidDetailedTimingsFeaturesSyncSchemeMask) {
      DEBUG ((DEBUG_INFO, "EDID detailed timing with unsupported sync scheme found - not processing.\n"));
      continue;
    }

    if ((pTiming->Features & EdidDetailedTimingsFeaturesStereoModeMask) != 0) {
      DEBUG ((DEBUG_INFO, "EDID detailed timing with unsupported stereo mode found - not processing.\n"));
      continue;
    }

    // We've found a supported detailed timing - now see if this is the requested one
    if (TimingNumber != NumValidDetailedTimingsFound) {
      NumValidDetailedTimingsFound++;
      continue;
    }

    ZeroMem ((VOID *)VideoMode, sizeof (struct VideoMode));

    // Bit manipulations copied from host software class EDIDTimingDescriptor

    VideoMode->PixelClock = (UINT16)pTiming->PixelClock;
    VideoMode->HActive = pTiming->HActiveLo + ((pTiming->HActiveHiBlankingHi & 0xF0) << 4);
    VideoMode->VActive = pTiming->VActiveLo + ((pTiming->VActiveHiBlankingHi & 0xF0) << 4);

    VideoMode->HBlanking = pTiming->HBlankingLo + ((pTiming->HActiveHiBlankingHi & 0x0F) << 8);
    VideoMode->VBlanking = pTiming->VBlankingLo + ((pTiming->VActiveHiBlankingHi & 0x0F) << 8);

    VideoMode->HSyncOffset = pTiming->HSyncOffsetLo + ((pTiming->HSyncOffsetHiHSyncWidthHiVSyncOffsetHiSyncWidthHi & 0xC0) << 2);  // Horizontal Front Porch
    VideoMode->HSyncWidth = pTiming->HSyncWidthLo + ((pTiming->HSyncOffsetHiHSyncWidthHiVSyncOffsetHiSyncWidthHi & 0x30) << 4);

    VideoMode->VSyncOffset = ((pTiming->VSyncOffsetLoSyncWidthLo & 0xF0) >> 4) + ((pTiming->HSyncOffsetHiHSyncWidthHiVSyncOffsetHiSyncWidthHi & 0x0C) << 2); // Vertical Front Porch
    VideoMode->VSyncWidth = (pTiming->VSyncOffsetLoSyncWidthLo & 0x0F) + ((pTiming->HSyncOffsetHiHSyncWidthHiVSyncOffsetHiSyncWidthHi & 0x03) << 4);

    VideoMode->Reserved2 = 2;
    VideoMode->Accumulate = 1;

    // Horizontal and vertical sync inversions - positive if bit set in descriptor (EDID spec)
    // In the VideoMode, they are negative if the bit is set (NR-110497-TC 4.3.3 0x22 Set Video Mode)

    // Horizontal sync
    if ((pTiming->Features & EdidDetailedTimingsFeaturesHorizontalSyncPositive) == 0) {
      VideoMode->Flags |= VideoModeFlagsHorizontalSyncInverted;
    }
    // Vertical sync
    if ((pTiming->Features & EdidDetailedTimingsFeaturesVerticalSyncPositive) == 0) {
      VideoMode->Flags |= VideoModeFlagsVerticalSyncInverted;
    }
    // Interlace
    if ((pTiming->Features & EdidDetailedTimingsFeaturesInterlaced) == EdidDetailedTimingsFeaturesInterlaced) {
      VideoMode->Flags |= VideoModeFlagsInterlaced;
    }

    DEBUG ((DEBUG_INFO, "Read mode %dx%d from detailed timings\n", VideoMode->HActive, VideoMode->VActive));
    return TRUE;
  }
  return FALSE;
}

/**
Check if a particular video mode is in either the Established or Standard Timings section of the EDID.

@param  EDID        Pointer to the 128-byte EDID
@param  hRes        Horizontal resolution
@param  vRes        Vertical resolution
@param  refresh     Refresh rate
@retval TRUE        The requested mode is present in the EDID

**/
STATIC BOOLEAN
IsModeInEdid (
    IN CONST VOID *EDID,
    IN UINT16 HRes,
    IN UINT16 VRes,
    IN UINT16 Refresh
    )
{
  UINT16 EdidHRes;
  UINT16 EdidVRes;
  UINT8 EdidRefresh;
  BOOLEAN ModeSupported;

  ModeSupported = IsModeInEstablishedTimings (EDID, HRes, VRes, Refresh);

  if (ModeSupported == FALSE) {
    // Check if the mode is in the Standard Timings section of the EDID
    UINT8 i;
    for (i = 0; i < EDID_NUMBER_OF_STANDARD_TIMINGS; i++) {
      if (TRUE == ReadStandardTiming (EDID, i, &EdidHRes, &EdidVRes, &EdidRefresh)) {
        if ((HRes == EdidHRes) && (VRes == EdidVRes) && (Refresh == EdidRefresh)) {
          ModeSupported = TRUE;
          break;
        }
      }
    }
  }
  return ModeSupported;
}

/**
Returns the (index)'th entry from the list of pre-calculated video timings that is also present in the EDID,
or, video mode data corresponding to any detailed timings present in the EDID.

Like QueryVideoMode, finds the number (and contents) of video modes available by repeatedly calling this function
with an increasing index value, until it returns FALSE
@param  index       The caller wants the _index_'th video mode
@param  EDID        Pointer to the 128-byte EDID
@param  edidSize    Size in bytes of the EDID
@param  videoMode   Video timings extracted from the modeData structure
@retval EFI_SUCCESS The requested mode is present in the EDID
@retval EFI_INVALID_PARAMETER The requested mode is not present in the EDID
**/
EFI_STATUS
DlEdidGetSupportedVideoMode (
    IN UINT32 Index,
    IN CONST VOID *EDID,
    IN UINT32 EdidSize,
    OUT CONST struct VideoMode **VideoMode
    )
{
  UINTN SupportedVideoModesFoundInEdid;
  EFI_STATUS Status;

  SupportedVideoModesFoundInEdid = 0;
  Status = EFI_INVALID_PARAMETER;

  // If we didn't manage to find an EDID earlier, just use one of the hard-coded video modes
  if ((EDID == NULL) || (EdidSize != EDID_BLOCK_SIZE)) {
    if (Index >= DlVideoModeGetNumSupportedVideoModes ()) {
      return EFI_INVALID_PARAMETER;
    }
    else {
      *VideoMode = DlVideoModeGetSupportedVideoMode (Index);
      DEBUG ((DEBUG_WARN, "No monitor EDID loaded - returning mode from default list (%dx%d)\n", (*VideoMode)->HActive, (*VideoMode)->VActive));
      return EFI_SUCCESS;
    }
  }

  UINT16 ModeNumber;
  for (ModeNumber = 0; ModeNumber < DlVideoModeGetNumSupportedVideoModes (); ModeNumber++) {

    CONST struct VideoMode *SupportedVideoMode = DlVideoModeGetSupportedVideoMode (ModeNumber);
    ASSERT (SupportedVideoMode);

    if (IsModeInEdid (EDID, SupportedVideoMode->HActive, SupportedVideoMode->VActive, DISPLAYLINK_FIXED_VERTICAL_REFRESH_RATE)) {
      if (Index == SupportedVideoModesFoundInEdid) {
        *VideoMode = SupportedVideoMode;
        Status = EFI_SUCCESS;
        break;
      }
      SupportedVideoModesFoundInEdid++;
    }
  }

  if (EFI_ERROR (Status)) {
    // Have a look in the detailed timings
    UINTN DetailedTimingNumber;
    STATIC struct VideoMode TmpVideoMode;
    DetailedTimingNumber = Index - SupportedVideoModesFoundInEdid;

    if (DetailedTimingNumber < EDID_NUMBER_OF_DETAILED_TIMINGS) {
      if (ReadDetailedTiming (EDID, (UINT8)DetailedTimingNumber, &TmpVideoMode)) {
        *VideoMode = &TmpVideoMode;
        Status = EFI_SUCCESS;
      }
    }
  }

  return Status;
}

/**
 * Like GetSupportedEdidVideoMode, but will return a fallback fixed mode of 640x480@60Hz
 * for index 0 if no suitable modes found in EDID.
 * @param index
 * @param EDID
 * @param edidSize
 * @param videoMode
 * @return EFI_SUCCESS
 */
EFI_STATUS
DlEdidGetSupportedVideoModeWithFallback (
    IN UINT32 Index,
    IN CONST VOID *EDID,
    IN UINT32 EdidSize,
    OUT CONST struct VideoMode **VideoMode
    )
{
  EFI_STATUS Status;
  Status = DlEdidGetSupportedVideoMode (Index, EDID, EdidSize, VideoMode);

  if (EFI_ERROR (Status)) {
    // Special case - if we didn't find any matching video modes in the EDID, fall back to 640x480@60Hz
    if (Index == 0) {
      *VideoMode = DlVideoModeGetSupportedVideoMode (0);
      DEBUG ((DEBUG_WARN, "No video modes supported by driver found in monitor EDID received from DL device - falling back to %dx%d\n", (*VideoMode)->HActive, (*VideoMode)->VActive));
      Status = EFI_SUCCESS;
    }
  }

  return Status;
}

/**
Count the number of video modes that we have timing information for that are present in the EDID
@param  EDID        Pointer to the 128-byte EDID
@param  edidSize
@retval             The number of modes in the EDID

**/
UINT32
DlEdidGetNumSupportedModesInEdid (
    IN CONST VOID *EDID,
    IN UINT32 EdidSize
    )
{
  UINT32 MaxMode;

  if ((EDID == NULL) || (EdidSize != EDID_BLOCK_SIZE)) {
    return DlVideoModeGetNumSupportedVideoModes ();
  }

  for (MaxMode = 0; ; MaxMode++) {
    CONST struct VideoMode *videoMode;
    if (EFI_ERROR (DlEdidGetSupportedVideoMode (MaxMode, EDID, EdidSize, &videoMode))) {
      break;
    }
  }
  DEBUG ((DEBUG_INFO, "Found %d video modes supported by driver in monitor EDID.\n", MaxMode));
  return MaxMode;
}



/**
 * Read the EDID from the connected monitor, store it in the local data structure
 * @param UsbDisplayLinkDev
 * @retval EFI_OUT_OF_RESOURCES - Could not allocate memory
 * @retval EFI_SUCCESS
 */
EFI_STATUS
DlReadEdid (
    IN USB_DISPLAYLINK_DEV* UsbDisplayLinkDev
    )
{
  EFI_STATUS Status;
  BOOLEAN EdidFound;
  EFI_EDID_OVERRIDE_PROTOCOL* EdidOverride;

  //
  // setup EDID information
  //
  UsbDisplayLinkDev->EdidDiscovered.Edid = (UINT8 *)NULL;
  UsbDisplayLinkDev->EdidDiscovered.SizeOfEdid = 0;

  EdidFound = FALSE;

  //
  // Find EDID Override protocol firstly, this protocol is installed by platform if needed.
  //
  Status = gBS->LocateProtocol (&gEfiEdidOverrideProtocolGuid, NULL, (VOID**)&EdidOverride);

  if (!EFI_ERROR (Status)) {
    UINT32 EdidAttributes = 0xff;
    UINTN EdidDataSize = 0;
    UINT8* EdidDataBlock = (UINT8*)NULL;

    // Allocate double size of VESA_BIOS_EXTENSIONS_EDID_BLOCK_SIZE to avoid overflow
    EdidDataBlock = (UINT8*)AllocatePool (EDID_BLOCK_SIZE * 2);

    if (NULL == EdidDataBlock) {
      return EFI_OUT_OF_RESOURCES;
    }

    Status = EdidOverride->GetEdid (
      EdidOverride,
      &UsbDisplayLinkDev->Handle,
      &EdidAttributes,
      &EdidDataSize,
      &EdidDataBlock);

    if (!EFI_ERROR (Status) && EdidAttributes == 0 && EdidDataSize != 0) {
      UsbDisplayLinkDev->EdidDiscovered.SizeOfEdid = (UINT32)EdidDataSize;
      UsbDisplayLinkDev->EdidDiscovered.Edid = EdidDataBlock;
      EdidFound = TRUE;
    }
    else {
      FreePool (EdidDataBlock);
      EdidDataBlock = NULL;
    }
  }

  if (EdidFound != TRUE) {
    UINTN EdidDataSize = 0;
    UINT8* EdidDataBlock = (UINT8*)NULL;

    if (ReadEdidData (UsbDisplayLinkDev, &EdidDataBlock, &EdidDataSize) == EFI_SUCCESS) {

      if (IsEdidChecksumCorrect (EdidDataBlock)) {
        UsbDisplayLinkDev->EdidDiscovered.SizeOfEdid = (UINT32)EdidDataSize;
        UsbDisplayLinkDev->EdidDiscovered.Edid = EdidDataBlock;
        EdidFound = TRUE;
      } else {
        DEBUG ((DEBUG_WARN, "Monitor EDID received from DisplayLink device had an invalid checksum. Corrupted?\n"));
      }
    }
  }

  if (EdidFound == FALSE) {
    DEBUG ((DEBUG_WARN, "No valid monitor EDID received from DisplayLink device. Cannot detect resolutions supported by monitor.\n"));
  }

  // Set the EDID active.
  // In an error case this will be set 0/NULL, which flags to the parsing code that there is no EDID.
  UsbDisplayLinkDev->EdidActive.SizeOfEdid = UsbDisplayLinkDev->EdidDiscovered.SizeOfEdid;
  UsbDisplayLinkDev->EdidActive.Edid = UsbDisplayLinkDev->EdidDiscovered.Edid;

  return EFI_SUCCESS;
}
