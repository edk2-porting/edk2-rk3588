/** @file
  The definition for Microcode Shadow Info Hob.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/


#ifndef _MICROCODE_SHADOW_INFO_HOB_H_
#define _MICROCODE_SHADOW_INFO_HOB_H_

///
/// The Global ID of a GUIDed HOB used to pass microcode shadow info to DXE Driver.
///
#define EDKII_MICROCODE_SHADOW_INFO_HOB_GUID \
  { \
    0x658903f9, 0xda66, 0x460d, { 0x8b, 0xb0, 0x9d, 0x2d, 0xdf, 0x65, 0x44, 0x59 } \
  }

extern EFI_GUID gEdkiiMicrocodeShadowInfoHobGuid;

typedef struct {
  //
  // An EFI_GUID that defines the contents of StorageContext.
  //
  GUID      StorageType;
  //
  // Number of the microcode patches which have been
  // relocated to memory.
  //
  UINT64    MicrocodeCount;
  //
  // An array with MicrocodeCount elements that stores
  // the shadowed microcode patch address in memory.
  //
  UINT64    MicrocodeAddrInMemory[0];
  //
  // A buffer which contains details about the storage information
  // specific to StorageType.
  //
  // UINT8  StorageContext[];
} EDKII_MICROCODE_SHADOW_INFO_HOB;

//
// An EDKII_MICROCODE_SHADOW_INFO_HOB with StorageType set to below GUID will have
// the StorageContext of a EFI_MICROCODE_STORAGE_TYPE_FLASH_CONTEXT strucutre.
//
#define EFI_MICROCODE_STORAGE_TYPE_FLASH_GUID \
  { \
    0x2cba01b3, 0xd391, 0x4598, { 0x8d, 0x89, 0xb7, 0xfc, 0x39, 0x22, 0xfd, 0x71 } \
  }

extern EFI_GUID gEdkiiMicrocodeStorageTypeFlashGuid;

typedef struct {
  //
  // An array with MicrocodeCount elements that stores the original
  // microcode patch address on flash. The address is placed in same
  // order as the microcode patches in MicrocodeAddrInMemory.
  //
  UINT64  MicrocodeAddressInFlash[0];
} EFI_MICROCODE_STORAGE_TYPE_FLASH_CONTEXT;

#endif
