/** @file
 *
 *  Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>
 *  Copyright (c) 2018-2019, DisplayLink (UK) Ltd. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __EDID_H__
#define __EDID_H__

#pragma pack (1)

#define EDID_BLOCK_SIZE  128

#define EDID_HEADER_SIZE  8

#define EDID_EXTENSION_CEA_861  0x02
#define EDID_EXTENSION_VTB      0x10

#define EDID_NUMBER_OF_ESTABLISHED_TIMINGS_BYTES  3
#define EDID_NUMBER_OF_STANDARD_TIMINGS           8
#define EDID_NUMBER_OF_DETAILED_TIMINGS           4

typedef struct {
  UINT16    HorizontalResolution;
  UINT16    VerticalResolution;
  UINT16    RefreshRate;
} EDID_TIMING;

typedef struct {
  UINT8    HorizontalActivePixels;     // X resolution, from 256->2288 in increments of 8 pixels
  UINT8    ImageAspectRatioAndrefresh; // Bits 7,6 Aspect ratio - 0=16:10 1=4:3 2=5:4 3=16:9
  // Bits 5,0 Refresh rate Range 60->1213Hz
} EDID_STANDARD_TIMING;

#define EDID_DETAILED_TIMING_CLOCK_RESOLUTION  10

typedef enum {
  EdidDetailedTimingFeatureInterlaced             = 0x80,
  EdidDetailedTimingFeatureStereoModeMask         = 0x60,
  EdidDetailedTimingFeatureSyncSchemeMask         = 0x18,
  EdidDetailedTimingFeatureHorizontalSyncPositive = 0x02,
  EdidDetailedTimingFeatureVerticalSyncPositive   = 0x04,
} EDID_DETAILED_TIMING_FEATURES;

typedef struct {
  UINT16    PixelClock;               // wPixelClock in VideoMode struct
  UINT8     HActiveLo;                // wHActive
  UINT8     HBlankingLo;              // wHBlanking
  UINT8     HActiveHiBlankingHi;
  UINT8     VActiveLo;                // wVActive
  UINT8     VBlankingLo;              // wVBlanking
  UINT8     VActiveHiBlankingHi;
  UINT8     HSyncOffsetLo;            // wHSyncOffset, front porch
  UINT8     HSyncWidthLo;             // wHSyncWidth
  UINT8     VSyncOffsetLoSyncWidthLo;
  UINT8     HSyncOffsetHiHSyncWidthHiVSyncOffsetHiSyncWidthHi;
  UINT8     HImageSizeHi;
  UINT8     VImageSizeHi;
  UINT8     HImageSizeLoVImageSizeLo;
  UINT8     HBorder;
  UINT8     VBorder;
  UINT8     Features;
} EDID_DETAILED_TIMING;

typedef struct {
  UINT8                   Header[EDID_HEADER_SIZE];         // EDID header "00 FF FF FF FF FF FF 00"
  UINT16                  ManufactureName;                  // EISA 3-character ID
  UINT16                  ProductCode;                      // Vendor assigned code
  UINT32                  SerialNumber;                     // 32-bit serial number
  UINT8                   WeekOfManufacture;                // Week number
  UINT8                   YearOfManufacture;                // Year
  UINT8                   EdidVersion;                      // EDID Structure Version
  UINT8                   EdidRevision;                     // EDID Structure Revision
  UINT8                   VideoInputDefinition;
  UINT8                   MaxHorizontalImageSize;           // cm
  UINT8                   MaxVerticalImageSize;             // cm
  UINT8                   DisplayTransferCharacteristic;
  UINT8                   FeatureSupport;
  UINT8                   RedGreenLowBits;                  // Rx1 Rx0 Ry1 Ry0 Gx1 Gx0 Gy1Gy0
  UINT8                   BlueWhiteLowBits;                 // Bx1 Bx0 By1 By0 Wx1 Wx0 Wy1 Wy0
  UINT8                   RedX;                             // Red-x Bits 9 - 2
  UINT8                   RedY;                             // Red-y Bits 9 - 2
  UINT8                   GreenX;                           // Green-x Bits 9 - 2
  UINT8                   GreenY;                           // Green-y Bits 9 - 2
  UINT8                   BlueX;                            // Blue-x Bits 9 - 2
  UINT8                   BlueY;                            // Blue-y Bits 9 - 2
  UINT8                   WhiteX;                           // White-x Bits 9 - 2
  UINT8                   WhiteY;                           // White-x Bits 9 - 2
  UINT8                   EstablishedTimings[EDID_NUMBER_OF_ESTABLISHED_TIMINGS_BYTES];
  EDID_STANDARD_TIMING    StandardTimings[EDID_NUMBER_OF_STANDARD_TIMINGS];
  EDID_DETAILED_TIMING    DetailedTimings[EDID_NUMBER_OF_DETAILED_TIMINGS];
  UINT8                   ExtensionFlag;                    // Number of (optional) 128-byte EDID extension blocks to follow
  UINT8                   Checksum;
} EDID_BASE;

#pragma pack ()

#endif
