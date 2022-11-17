/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _TEST_POINT_PRIVATE_H_
#define _TEST_POINT_PRIVATE_H_

#include <Uefi.h>
#include <PiSmm.h>
#include <Library/TestPointCheckLib.h>
#include <Library/DebugLib.h>

#define TEST_POINT_SMM_COMMUNICATION_VERSION                    0x1
#define TEST_POINT_SMM_COMMUNICATION_FUNC_ID_UEFI_GCD_MAP_INFO  0x1

typedef struct {
  UINT32     Version;
  UINT32     FuncId;
  UINT64     Size;
} TEST_POINT_SMM_COMMUNICATION_HEADER;

typedef struct {
  TEST_POINT_SMM_COMMUNICATION_HEADER  Header;
  UINT64                               UefiMemoryMapOffset;
  UINT64                               UefiMemoryMapSize;
  UINT64                               GcdMemoryMapOffset;
  UINT64                               GcdMemoryMapSize;
  UINT64                               GcdIoMapOffset;
  UINT64                               GcdIoMapSize;
  UINT64                               UefiMemoryAttributeTableOffset;
  UINT64                               UefiMemoryAttributeTableSize;
} TEST_POINT_SMM_COMMUNICATION_UEFI_GCD_MAP_INFO;

#define TEST_POINT_SMM_COMMUNICATION_GUID { \
  0x9cfa432a, 0x17cd, 0x4eb7, { 0x96, 0x54, 0x2e, 0xb2, 0x5, 0x91, 0xef, 0x8f } \
  }

extern EFI_GUID  mTestPointSmmCommunciationGuid;

#endif