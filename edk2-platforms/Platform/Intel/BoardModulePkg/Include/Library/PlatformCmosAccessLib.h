/** @file
  Platform CMOS Access Library Header File.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PLATFORM_CMOS_ACCESS_LIB_H_
#define _PLATFORM_CMOS_ACCESS_LIB_H_

///
/// Flag indicating checksum calculation doesn't include this location.
/// NOTE: If a location isn't shown in platform CMOS entry table,
///       it means checksum calculation doesn't include the location.
///
#define CMOS_EXCLUDE_FROM_CHECKSUM    BIT0

///
/// Flag indicating initialization doesn't cover this location.
/// NOTE: If a location isn't shown in platform CMOS entry table,
///       it means the location is initialized with CMOS_DEFAULT_VALUE (0).
///
#define CMOS_EXCLUDE_FROM_INIT_DATA   BIT1

///
/// Flag indicating the location cannot be accessed.
/// NOTE: 0x0 ~ 0xD is implictly inaccessible.
///
#define CMOS_EXCLUDE_FROM_ACCESS      (BIT3 | CMOS_EXCLUDE_FROM_CHECKSUM | CMOS_EXCLUDE_FROM_INIT_DATA)

///
/// Flag indicating the checksum location
/// NOTE: At most two entries can have this flag set.
///
#define CMOS_CHECKSUM_LOCATION        (BIT2 | CMOS_EXCLUDE_FROM_CHECKSUM | CMOS_EXCLUDE_FROM_INIT_DATA)

#define CMOS_DEFAULT_VALUE            0x00

typedef struct {
  UINT8 Address;
  UINT8 DefaultValue;
  UINT8 Attributes;
} CMOS_ENTRY;

/**
  Return the platform CMOS entries.

  @param [out] EntryCount Return the count of platform CMOS entries.

  @return Platform CMOS entries.
**/
CMOS_ENTRY *
EFIAPI
PlatformCmosGetEntry (
  OUT UINTN       *EntryCount
  );

/**
  Return the NMI enable status.
**/
BOOLEAN
EFIAPI
PlatformCmosGetNmiState (
  VOID
  );

#endif // _PLATFORM_CMOS_ACCESS_LIB_H_
