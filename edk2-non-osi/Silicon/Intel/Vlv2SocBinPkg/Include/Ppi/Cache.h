//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/** @file


  Cache PPI definition.

Copyright (c) 2006 - 2010, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#ifndef _PEI_CACHE_PPI_H_
#define _PEI_CACHE_PPI_H_

#define PEI_CACHE_PPI_GUID \
  { \
    0xc153205a, 0xe898, 0x4c24, {0x86, 0x89, 0xa4, 0xb4, 0xbc, 0xc5, 0xc8, 0xa2 } \
  }

typedef struct _PEI_CACHE_PPI PEI_CACHE_PPI;

//
// EFI_MEMORY_CACHE_TYPE
//
typedef INT32 EFI_MEMORY_CACHE_TYPE;

//
// PEI_SET_CACHE_PPI
//
typedef
EFI_STATUS
(EFIAPI *PEI_SET_CACHE_PPI) (
  IN  EFI_PEI_SERVICES       **PeiServices,
  IN  PEI_CACHE_PPI          *This,
  IN  EFI_PHYSICAL_ADDRESS   MemoryAddress,
  IN  UINT64                 MemoryLength,
  IN  EFI_MEMORY_CACHE_TYPE  MemoryCacheType
  );

//
// PEI_RESET_CACHE_PPI
//
typedef
EFI_STATUS
(EFIAPI *PEI_RESET_CACHE_PPI) (
  IN  EFI_PEI_SERVICES   **PeiServices,
  IN  PEI_CACHE_PPI      *This
  );

//
// PEI_CACHE_PPI
//
struct _PEI_CACHE_PPI {
  PEI_SET_CACHE_PPI    SetCache;
  PEI_RESET_CACHE_PPI  ResetCache;
};

extern EFI_GUID gPeiCachePpiGuid;

#endif
