/** @file
  UBA PCH Softstrap Update Library Header File.

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UBA_SOFT_STRAP_UPDATE_LIB_H
#define _UBA_SOFT_STRAP_UPDATE_LIB_H

#include <Base.h>
#include <Uefi.h>
#include <Protocol/Spi.h>

#define PLATFORM_SOFT_STRAP_UPDATE_SIGNATURE  SIGNATURE_32 ('P', 'S', 'T', 'P')
#define PLATFORM_SOFT_STRAP_UPDATE_VERSION    02

// {F06383FE-54BD-4ae1-9C99-1DC83B6A7277}
#define   PLATFORM_PCH_SOFTSTRAP_CONFIG_DATA_GUID \
{ 0xf06383fe, 0x54bd, 0x4ae1, { 0x9c, 0x99, 0x1d, 0xc8, 0x3b, 0x6a, 0x72, 0x77 } }

STATIC  EFI_GUID gPlatformPchSoftStrapConfigDataGuid = PLATFORM_PCH_SOFTSTRAP_CONFIG_DATA_GUID;

typedef struct _PLATFORM_PCH_SOFTSTRAP_FIXUP_ENTRY {
  UINT8  SoftStrapNumber;
  UINT8  BitfieldOffset;
  UINT8  BitfieldLength;
  UINT32 Value;
} PLATFORM_PCH_SOFTSTRAP_FIXUP_ENTRY;

typedef
VOID
(*PLATFORM_SPECIFIC_PCH_SOFTSTRAP_UPDATE) (
  IN UINT8            *FlashDescriptorCopy
  );

typedef struct {
  UINT32                                  Signature;
  UINT32                                  Version;
  PLATFORM_PCH_SOFTSTRAP_FIXUP_ENTRY      *PchSoftStrapTablePtr;
  PLATFORM_SPECIFIC_PCH_SOFTSTRAP_UPDATE  PchSoftStrapPlatformSpecificUpdate;
} PLATFORM_PCH_SOFTSTRAP_UPDATE;


EFI_STATUS
GetPchSoftSoftStrapTable (
  IN  VOID                      **PchSoftStrapTable
  );

VOID
PlatformSpecificPchSoftStrapUpdate (
  IN OUT  UINT8                 *FlashDescriptorCopy
  );

#endif //_UBA_SOFT_STRAP_UPDATE_LIB_H
