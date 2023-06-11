/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _OEM_SET_VIRTUAL_MAP_DESC_H_
#define _OEM_SET_VIRTUAL_MAP_DESC_H_


UINTN OemSetVirtualMapDesc (
  ARM_MEMORY_REGION_DESCRIPTOR  *VirtualMemoryTable,
  ARM_MEMORY_REGION_ATTRIBUTES  CacheAttributes
  );

#endif

