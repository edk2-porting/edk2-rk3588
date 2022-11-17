/** @file
  DXE FRU Library to initialize Vtd

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Base.h>

/**
  For device that specified by Device Num and Function Num,
  mDevEnMap is used to check device presence.
  0x80 means use Device ID to detemine presence
  0x8F means force to update

  The structure is used to check if device scope is valid when update DMAR table
**/
UINT16  mDevEnMap[][2] = {{0x0200, 0x80}, {0x0500, 0x80}, {0x1400, 0x80}, {0x1401, 0x80}, {0x0700, 0x80}, {0x0701, 0x80}, {0x0702, 0x80}, {0x0703, 0x80}, {0x1302, 0x8F}, {0x1303, 0x8F}};
UINTN   mDevEnMapSize = sizeof (mDevEnMap) / (sizeof (UINT16) * 2);
