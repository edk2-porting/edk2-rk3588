/**
  PPI for describing the minimum platform memory size in order to successfully
  pass control into DXE

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PEI_PLATFORM_MEMORY_SIZE_H_
#define _PEI_PLATFORM_MEMORY_SIZE_H_

#define PEI_PLATFORM_MEMORY_SIZE_PPI_GUID \
  { \
    0x9a7ef41e, 0xc140, 0x4bd1, {0xb8, 0x84, 0x1e, 0x11, 0x24, 0xb, 0x4c, 0xe6 } \
  }

typedef struct _PEI_PLATFORM_MEMORY_SIZE_PPI PEI_PLATFORM_MEMORY_SIZE_PPI;

typedef
EFI_STATUS
(EFIAPI *PEI_GET_MINIMUM_PLATFORM_MEMORY_SIZE) (
  IN      EFI_PEI_SERVICES                       **PeiServices,
  IN PEI_PLATFORM_MEMORY_SIZE_PPI                * This,
  IN OUT  UINT64                                 *MemorySize
  );

typedef struct _PEI_PLATFORM_MEMORY_SIZE_PPI {
  PEI_GET_MINIMUM_PLATFORM_MEMORY_SIZE  GetPlatformMemorySize;
} PEI_PLATFORM_MEMORY_SIZE_PPI;

extern EFI_GUID gPeiPlatformMemorySizePpiGuid;

#endif
