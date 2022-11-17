/** @file
  The definition for VTD Null Root Entry Table PPI.

  This is a lightweight VTd null root entry table report in PEI phase.

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __VTD_NULL_ROOT_ENTRY_TABLE_PPI_H__
#define __VTD_NULL_ROOT_ENTRY_TABLE_PPI_H__

#define EDKII_VTD_NULL_ROOT_ENTRY_TABLE_PPI_GUID \
    { \
      0x3de0593f, 0x6e3e, 0x4542, { 0xa1, 0xcb, 0xcb, 0xb2, 0xdb, 0xeb, 0xd8, 0xff } \
    }

//
// Null root entry table address is a fixed silicon reserved address,
//   which is used to block the DMA transfer.
//
typedef UINT64  EDKII_VTD_NULL_ROOT_ENTRY_TABLE_PPI;

extern EFI_GUID gEdkiiVTdNullRootEntryTableGuid;

#endif

