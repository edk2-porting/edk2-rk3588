/** @file
  UBA ClockGen Update Library Header File.

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UBA_CLOCKGEN_UPDATE_LIB_H
#define _UBA_CLOCKGEN_UPDATE_LIB_H

#include <Base.h>
#include <Uefi.h>

#define PLATFORM_CLOCKGEN_UPDATE_SIGNATURE  SIGNATURE_32 ('P', 'C', 'L', 'K')
#define PLATFORM_CLOCKGEN_UPDATE_VERSION    0x01

// {CF3845B1-7EB0-44ef-9D67-A80ECE6FED73}
#define   PLATFORM_CLOCKGEN_CONFIG_DATA_GUID \
{ 0xcf3845b1, 0x7eb0, 0x44ef, { 0x9d, 0x67, 0xa8, 0xe, 0xce, 0x6f, 0xed, 0x73 } }

#define PLATFORM_NUMBER_OF_CLOCKGEN_DATA    20
#define PLATFORM_CLOCKGEN_NO_ID             0xFF

typedef struct {
  UINT32                  Signature;
  UINT32                  Version;

  UINTN                   IdOffset;     // Clockgen ID register offset
  UINT8                   Id;           // Clockgen ID
  UINTN                   DataLength;   // Number of clockgen data for write

  UINTN                   SpreadSpectrumByteOffset;
  UINT8                   SpreadSpectrumValue;

  UINT8                   Data[PLATFORM_NUMBER_OF_CLOCKGEN_DATA];

} PLATFORM_CLOCKGEN_UPDATE_TABLE;


EFI_STATUS
PlatformUpdateClockgen (
  IN  BOOLEAN     EnableSpreadSpectrum
);

STATIC  EFI_GUID gPlatformClockgenConfigDataGuid = PLATFORM_CLOCKGEN_CONFIG_DATA_GUID;

#endif //_UBA_CLOCKGEN_UPDATE_LIB_H
