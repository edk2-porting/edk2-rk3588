/** @file
  EFI Platform Pirq Data Definition Header File.

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _EFI_PLATF_PIRQ_DATA_H_
#define _EFI_PLATF_PIRQ_DATA_H_

#include <Protocol/LegacyBiosPlatform.h>

#define EFI_PIRQ_TABLE_SIGNATURE  0x52495024
#define EFI_PIRQ_TABLE_VERSION    0x100

//
// Common path types.
//
typedef struct {
  EFI_LEGACY_PIRQ_TABLE_HEADER  PirqTable;
} EFI_LEGACY_PIRQ_TABLE;

//
// This is the module global containing platform PIRQ data.
//
typedef struct {
  EFI_LEGACY_IRQ_PRIORITY_TABLE_ENTRY *PriorityTable;
  UINTN                                PriorityTableSize;
  EFI_LEGACY_PIRQ_TABLE               *TableHead;
  UINTN                                TableHeadSize;
} PLATFORM_PIRQ_DATA;

#endif  //_EFI_PLATF_PIRQ_DATA_H_

