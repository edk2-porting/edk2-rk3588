/** @file
  GUID and definitions for BIOS ID.

Copyright (c) 2015 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _BIOS_ID_GUID_H_
#define _BIOS_ID_GUID_H_

#include <Pi/PiHob.h>

extern EFI_GUID gBiosIdGuid;

//
// $(BOARD_ID)$(BOARD_REV).$(BOARD_EXT).$(VERSION_MAJOR).$(BUILD_TYPE)$(VERSION_MINOR).YYMMDDHHMM
//
// Example: "TRFTCRB1.000.0008.D03.1501301017"
//
#pragma pack(1)

typedef struct {
  CHAR16    BoardId[7];             // "TRFTCRB"
  CHAR16    BoardRev;               // "1"
  CHAR16    Dot1;                   // "."
  CHAR16    BoardExt[3];            // "000"
  CHAR16    Dot2;                   // "."
  CHAR16    VersionMajor[4];        // "0008"
  CHAR16    Dot3;                   // "."
  CHAR16    BuildType;              // "D"
  CHAR16    VersionMinor[2];        // "03"
  CHAR16    Dot4;                   // "."
  CHAR16    TimeStamp[10];          // "YYMMDDHHMM"
  CHAR16    NullTerminator;         // 0x0000
} BIOS_ID_STRING;

//
// A signature precedes the BIOS ID string in the FV to enable search by external tools.
//
typedef struct {
  UINT8             Signature[8];   // "$IBIOSI$"
  BIOS_ID_STRING    BiosIdString;   // "TRFTCRB1.000.0008.D03.1501301017"
} BIOS_ID_IMAGE;

#pragma pack()

typedef struct {
  EFI_HOB_GUID_TYPE GuidType;
  BIOS_ID_IMAGE     BiosIdImage;
} BIOS_ID_HOB;

#endif

