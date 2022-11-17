/** @file
  The file describes the PPI which notifies other drivers 
  of the PEIM being initialized by the PEI Dispatcher.

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _BASE_MEMORY_TEST_H_
#define _BASE_MEMORY_TEST_H_

#define PEI_BASE_MEMORY_TEST_GUID \
  { \
    0xb6ec423c, 0x21d2, 0x490d, {0x85, 0xc6, 0xdd, 0x58, 0x64, 0xea, 0xa6, 0x74} \
  }

typedef struct _PEI_BASE_MEMORY_TEST_PPI PEI_BASE_MEMORY_TEST_PPI;

typedef enum {
  Ignore,
  Quick,
  Sparse,
  Extensive
} PEI_MEMORY_TEST_OP;

typedef
EFI_STATUS
(EFIAPI *PEI_BASE_MEMORY_TEST) (
  IN  EFI_PEI_SERVICES                   **PeiServices,
  IN PEI_BASE_MEMORY_TEST_PPI            * This,
  IN  EFI_PHYSICAL_ADDRESS               BeginAddress,
  IN  UINT64                             MemoryLength,
  IN  PEI_MEMORY_TEST_OP                 Operation,
  OUT EFI_PHYSICAL_ADDRESS               * ErrorAddress
  );

struct _PEI_BASE_MEMORY_TEST_PPI {
  PEI_BASE_MEMORY_TEST  BaseMemoryTest;
};

extern EFI_GUID gPeiBaseMemoryTestPpiGuid;

#endif
