/** @file Edid.h
 * @brief Helper routine and corresponding data struct used by USB DisplayLink Driver.
 * Reads and parses the EDID, checks if a requested video mode is in the supplied EDID
 *
 * Copyright (c) 2018-2019, DisplayLink (UK) Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
**/

#ifndef EDID_H
#define EDID_H

#include "UsbDisplayLink.h"

#define EDID_HEADER_SIZE                          ((UINTN)8)
#define EDID_NUMBER_OF_ESTABLISHED_TIMINGS_BYTES  ((UINTN)3)
#define EDID_NUMBER_OF_STANDARD_TIMINGS           ((UINTN)8)
#define EDID_NUMBER_OF_DETAILED_TIMINGS           ((UINTN)4)


typedef struct {
  UINT16  HRes;
  UINT16  VRes;
  UINT16  Refresh;
} EDID_TIMING;


EFI_STATUS
DlEdidGetSupportedVideoMode (
    UINT32 ModeNumber,
    CONST VOID *EDID,
    UINT32 EdidSize,
    CONST struct VideoMode **VideoMode
    );

EFI_STATUS
DlEdidGetSupportedVideoModeWithFallback (
    UINT32 ModeNumber,
    CONST VOID *EDID,
    UINT32 EdidSize,
    CONST struct VideoMode **VideoMode
    );

UINT32
DlEdidGetNumSupportedModesInEdid (
    CONST VOID *EDID,
    UINT32 EdidSize
    );

EFI_STATUS
DlReadEdid (
  USB_DISPLAYLINK_DEV* UsbDisplayLinkDev
);

// EDID Detailed timings section - Features
enum EdidDetailedTimingsFeatures {
  EdidDetailedTimingsFeaturesInterlaced              = 0x80,
  EdidDetailedTimingsFeaturesStereoModeMask          = 0x60,
  EdidDetailedTimingsFeaturesSyncSchemeMask          = 0x18,
  EdidDetailedTimingsFeaturesHorizontalSyncPositive  = 0x02,
  EdidDetailedTimingsFeaturesVerticalSyncPositive    = 0x04,
};

// NR-110497-TC-7ZM Section 4.3.3 0x22 Set Video Mode - Flags
enum VideoModeFlags {
  VideoModeFlagsInterlaced             = 0x0001,
  VideoModeFlagsHorizontalSyncInverted = 0x0100,
  VideoModeFlagsVerticalSyncInverted   = 0x0200,
};

struct StandardTimingIdentification {
  UINT8 HorizontalActivePixels;           // X resolution, from 256->2288 in increments of 8 pixels
  UINT8 ImageAspectRatioAndrefresh;   // Bits 7,6 Aspect ratio - 0=16:10 1=4:3 2=5:4 3=16:9
                                          // Bits 5,0 Refresh rate Range 60->1213Hz
};

struct DetailedTimingIdentification {
  UINT16 PixelClock;                  // wPixelClock in VideoMode struct
  UINT8  HActiveLo;                   // wHActive
  UINT8  HBlankingLo;                 // wHBlanking
  UINT8  HActiveHiBlankingHi;
  UINT8  VActiveLo;                   // wVActive
  UINT8  VBlankingLo;                 // wVBlanking
  UINT8  VActiveHiBlankingHi;
  UINT8  HSyncOffsetLo;               // wHSyncOffset, front porch
  UINT8  HSyncWidthLo;                // wHSyncWidth
  UINT8  VSyncOffsetLoSyncWidthLo;
  UINT8  HSyncOffsetHiHSyncWidthHiVSyncOffsetHiSyncWidthHi;
  UINT8  HImageSizeHi;
  UINT8  VImageSizeHi;
  UINT8  HImageSizeLoVImageSizeLo;
  UINT8  HBorder;
  UINT8  VBorder;
  UINT8  Features;
};

struct Edid {
  UINT8  Header[EDID_HEADER_SIZE];           //EDID header "00 FF FF FF FF FF FF 00"
  UINT16 ManufactureName;                  //EISA 3-character ID
  UINT16 ProductCode;                      //Vendor assigned code
  UINT32 SerialNumber;                     //32-bit serial number
  UINT8  WeekOfManufacture;                //Week number
  UINT8  YearOfManufacture;                //Year
  UINT8  EdidVersion;                      //EDID Structure Version
  UINT8  EdidRevision;                     //EDID Structure Revision
  UINT8  VideoInputDefinition;
  UINT8  MaxHorizontalImageSize;           //cm
  UINT8  MaxVerticalImageSize;             //cm
  UINT8  DisplayTransferCharacteristic;
  UINT8  FeatureSupport;
  UINT8  RedGreenLowBits;                  //Rx1 Rx0 Ry1 Ry0 Gx1 Gx0 Gy1Gy0
  UINT8  BlueWhiteLowBits;                 //Bx1 Bx0 By1 By0 Wx1 Wx0 Wy1 Wy0
  UINT8  RedX;                             //Red-x Bits 9 - 2
  UINT8  RedY;                             //Red-y Bits 9 - 2
  UINT8  GreenX;                           //Green-x Bits 9 - 2
  UINT8  GreenY;                           //Green-y Bits 9 - 2
  UINT8  BlueX;                            //Blue-x Bits 9 - 2
  UINT8  BlueY;                            //Blue-y Bits 9 - 2
  UINT8  WhiteX;                           //White-x Bits 9 - 2
  UINT8  WhiteY;                           //White-x Bits 9 - 2
  UINT8  EstablishedTimings[EDID_NUMBER_OF_ESTABLISHED_TIMINGS_BYTES];
  struct StandardTimingIdentification  standardTimingIdentifications[EDID_NUMBER_OF_STANDARD_TIMINGS];
  struct DetailedTimingIdentification  detailedTimingDescriptions[EDID_NUMBER_OF_DETAILED_TIMINGS];
  UINT8  ExtensionFlag;                    //Number of (optional) 128-byte EDID extension blocks to follow
  UINT8  Checksum;
};

#endif // EDID_H
