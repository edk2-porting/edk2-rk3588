/** @file
  Header file for Config Block Lib implementation

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CONFIG_BLOCK_H_
#define _CONFIG_BLOCK_H_

#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiMultiPhase.h>
#include <Pi/PiBootMode.h>
#include <Pi/PiHob.h>

#pragma pack (push,1)

///
/// Config Block Header
///
typedef struct _CONFIG_BLOCK_HEADER {
  EFI_HOB_GUID_TYPE GuidHob;                      ///< Offset 0-23  GUID extension HOB header
  UINT8             Revision;                     ///< Offset 24    Revision of this config block
  UINT8             Attributes;                   ///< Offset 25    The main revision for config block
  UINT8             Reserved[2];                  ///< Offset 26-27 Reserved for future use
} CONFIG_BLOCK_HEADER;

///
/// Config Block
///
typedef struct _CONFIG_BLOCK {
  CONFIG_BLOCK_HEADER            Header;          ///< Offset 0-27  Header of config block
  //
  // Config Block Data
  //
} CONFIG_BLOCK;

///
/// Config Block Table Header
///
typedef struct _CONFIG_BLOCK_TABLE_STRUCT {
  CONFIG_BLOCK_HEADER            Header;          ///< Offset 0-27  GUID number for main entry of config block
  UINT8                          Rsvd0[2];        ///< Offset 28-29 Reserved for future use
  UINT16                         NumberOfBlocks;  ///< Offset 30-31 Number of config blocks (N)
  UINT32                         AvailableSize;   ///< Offset 32-35 Current config block table size
///
/// Individual Config Block Structures are added here in memory as part of AddConfigBlock()
///
} CONFIG_BLOCK_TABLE_HEADER;
#pragma pack (pop)

#endif // _CONFIG_BLOCK_H_
