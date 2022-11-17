/** @file
  Definitions and GUID for BIOS INFO.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _BIOS_INFO_H_
#define _BIOS_INFO_H_

//
// BIOS INFO data structure
// This is self contained data structure for BIOS info for TXT
//
#pragma pack (1)
#define BIOS_INFO_SIGNATURE  SIGNATURE_64 ('$', 'B', 'I', 'O', 'S', 'I', 'F', '$')
typedef struct {
  UINT64            Signature;
  UINT32            EntryCount;
  UINT32            Reserved;
//BIOS_INFO_STRUCT  Struct[EntryCount];
} BIOS_INFO_HEADER;

//
// BIOS_INFO_STRUCT attributes
// bits[0:3] means general attributes
// bits[4:7] means type specific attributes
//
#define BIOS_INFO_STRUCT_ATTRIBUTE_GENERAL_EXCLUDE_FROM_FIT  0x01
#define BIOS_INFO_STRUCT_ATTRIBUTE_MICROCODE_WHOLE_REGION    0x10
#define BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB             0x10
#define BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_NON_IBB              0x20

typedef struct {
  //
  // FitTable entry type
  //
  UINT8    Type;
  //
  // BIOS_INFO_STRUCT attributes
  //
  UINT8    Attributes;
  //
  // FitTable entry version
  //
  UINT16   Version;
  //
  // FitTable entry real size
  //
  UINT32   Size;
  //
  // FitTable entry address
  //
  UINT64   Address;
} BIOS_INFO_STRUCT;

extern EFI_GUID  gBiosInfoGuid;

#pragma pack ()

#endif
