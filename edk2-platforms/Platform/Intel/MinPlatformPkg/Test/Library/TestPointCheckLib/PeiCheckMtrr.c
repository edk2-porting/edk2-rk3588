/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiPei.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MtrrLib.h>

#define MEMORY_ATTRIBUTE_MASK (EFI_RESOURCE_ATTRIBUTE_PRESENT | \
                               EFI_RESOURCE_ATTRIBUTE_INITIALIZED | \
                               EFI_RESOURCE_ATTRIBUTE_TESTED | \
                               EFI_RESOURCE_ATTRIBUTE_16_BIT_IO | \
                               EFI_RESOURCE_ATTRIBUTE_32_BIT_IO | \
                               EFI_RESOURCE_ATTRIBUTE_64_BIT_IO \
                               )

#define TESTED_MEMORY_ATTRIBUTES      (EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_TESTED)

#define INITIALIZED_MEMORY_ATTRIBUTES (EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED)

#define PRESENT_MEMORY_ATTRIBUTES     (EFI_RESOURCE_ATTRIBUTE_PRESENT)

MTRR_MEMORY_CACHE_TYPE
SetCurrentCacheType (
  IN MTRR_MEMORY_CACHE_TYPE  CurrentCacheType,
  IN MTRR_MEMORY_CACHE_TYPE  NewCacheType
  )
{
  switch (CurrentCacheType) {
  case CacheUncacheable:
    return CacheUncacheable;
    break;
  case CacheWriteBack:
    if (NewCacheType == CacheWriteThrough) {
      return CacheWriteThrough;
    } else {
      return CacheInvalid;
    }
    break;
  case CacheWriteThrough:
    if (NewCacheType == CacheWriteBack) {
      return CacheWriteThrough;
    } else {
      return CacheInvalid;
    }
    break;
  default:
    if (NewCacheType == CacheUncacheable) {
      return CacheUncacheable;
    } else {
      return CacheInvalid;
    }
    break;
  } 
}

EFI_STATUS
TestPointCheckCacheType (
  IN MTRR_SETTINGS           *Mtrrs,
  IN VARIABLE_MTRR           *VariableMtrr,
  IN UINT64                  Base,
  IN UINT64                  Length,
  IN MTRR_MEMORY_CACHE_TYPE  ExpectedCacheType
  )
{
  UINT64         TempBase;
  UINT64         TempLength;
  UINTN          VariableMtrrIndex;
  UINTN          VariableMtrrCount;

  if (Base < BASE_1MB) {
    // Check Fixed MTRR
    return EFI_SUCCESS;
  }

  //
  // Check
  //
  VariableMtrrCount = GetVariableMtrrCount ();
  for (VariableMtrrIndex = 0; VariableMtrrIndex < VariableMtrrCount; VariableMtrrIndex++) {
    if (!VariableMtrr[VariableMtrrIndex].Valid) {
      continue;
    }
    if (((Base >= VariableMtrr[VariableMtrrIndex].BaseAddress) && (Base < VariableMtrr[VariableMtrrIndex].BaseAddress + VariableMtrr[VariableMtrrIndex].Length)) ||
        ((VariableMtrr[VariableMtrrIndex].BaseAddress >= Base) && (VariableMtrr[VariableMtrrIndex].BaseAddress < Base + Length))) {
      // Overlap check
      if (VariableMtrr[VariableMtrrIndex].Type != ExpectedCacheType) {
        DEBUG ((DEBUG_ERROR, "Cache [0x%lx, 0x%lx] is not expected\n", Base, Length));
        return EFI_INVALID_PARAMETER;
      }
    }
  }
  TempBase = Base;
  TempLength = Length;
  for (VariableMtrrIndex = 0; VariableMtrrIndex < VariableMtrrCount; VariableMtrrIndex++) {
    if (!VariableMtrr[VariableMtrrIndex].Valid) {
      continue;
    }
    if (((TempBase >= VariableMtrr[VariableMtrrIndex].BaseAddress) && (TempBase < VariableMtrr[VariableMtrrIndex].BaseAddress + VariableMtrr[VariableMtrrIndex].Length)) ||
        ((VariableMtrr[VariableMtrrIndex].BaseAddress >= TempBase) && (VariableMtrr[VariableMtrrIndex].BaseAddress < TempBase + TempLength))) {
      // Update checked region
      if (TempBase >= VariableMtrr[VariableMtrrIndex].BaseAddress) {
        if (TempBase + TempLength > VariableMtrr[VariableMtrrIndex].BaseAddress + VariableMtrr[VariableMtrrIndex].Length) {
          TempLength = TempBase + TempLength - (VariableMtrr[VariableMtrrIndex].BaseAddress + VariableMtrr[VariableMtrrIndex].Length);
          TempBase = VariableMtrr[VariableMtrrIndex].BaseAddress + VariableMtrr[VariableMtrrIndex].Length;
        } else {
          TempLength = 0;
        }
      } else {
        TempLength = VariableMtrr[VariableMtrrIndex].BaseAddress - TempBase;
      }
    }
  }

  if (TempLength != 0) {
    if ((Mtrrs->MtrrDefType & 0xFF) != ExpectedCacheType) {
      DEBUG ((DEBUG_ERROR, "Cache [0x%lx, 0x%lx] is not expected in default\n", TempBase, TempLength));
      return EFI_INVALID_PARAMETER;
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
TestPointCheckMtrrMask (
  IN MTRR_SETTINGS  *Mtrrs
  )
{
  UINTN  Index;
  UINT64 Length;
  UINT32 RegEax;
  UINT8  PhysicalAddressBits;
  UINTN  VariableMtrrCount;

  AsmCpuid (0x80000000, &RegEax, NULL, NULL, NULL);
  if (RegEax >= 0x80000008) {
    AsmCpuid (0x80000008, &RegEax, NULL, NULL, NULL);
    PhysicalAddressBits = (UINT8) RegEax;
  } else {
    PhysicalAddressBits = 36;
  }

  VariableMtrrCount = GetVariableMtrrCount ();
  for (Index = 0; Index < VariableMtrrCount; Index++) {
    if ((Mtrrs->Variables.Mtrr[Index].Mask & BIT11) == 0) {
      continue;
    }
    Length = Mtrrs->Variables.Mtrr[Index].Mask & ~0xFFFull;
    Length = ~Length + 1;
    Length = Length & (LShiftU64 (1, PhysicalAddressBits) - 1);
    if (Length != GetPowerOfTwo64 (Length)) {
      DEBUG ((DEBUG_ERROR, "MTRR Mask (0x%016lx) is invalid\n", Mtrrs->Variables.Mtrr[Index].Mask));
      return EFI_INVALID_PARAMETER;
    }
  }
  return EFI_SUCCESS;
}

VOID
TestPointMtrrConvert (
  IN  MTRR_SETTINGS  *Mtrrs,
  OUT VARIABLE_MTRR  *VariableMtrr
  )
{
  UINT32         RegEax;
  UINT8          PhysicalAddressBits;
  VARIABLE_MTRR  TempVariableMtrr;
  UINTN          Index;
  UINTN          VariableMtrrIndex;
  UINTN          VariableMtrrCount;

  AsmCpuid (0x80000000, &RegEax, NULL, NULL, NULL);
  if (RegEax >= 0x80000008) {
    AsmCpuid (0x80000008, &RegEax, NULL, NULL, NULL);
    PhysicalAddressBits = (UINT8) RegEax;
  } else {
    PhysicalAddressBits = 36;
  }

  //
  // Calculate Length
  //
  VariableMtrrIndex = 0;
  VariableMtrrCount = GetVariableMtrrCount ();
  for (Index = 0; Index < VariableMtrrCount; Index++) {
    if ((Mtrrs->Variables.Mtrr[Index].Mask & BIT11) == 0) {
      continue;
    }
    VariableMtrr[VariableMtrrIndex].Length = Mtrrs->Variables.Mtrr[Index].Mask & ~0xFFFull;
    VariableMtrr[VariableMtrrIndex].Length = ~VariableMtrr[VariableMtrrIndex].Length + 1;
    VariableMtrr[VariableMtrrIndex].Length = VariableMtrr[VariableMtrrIndex].Length & (LShiftU64 (1, PhysicalAddressBits) - 1);
    VariableMtrr[VariableMtrrIndex].BaseAddress = Mtrrs->Variables.Mtrr[Index].Base & ~0xFFFull;
    VariableMtrr[VariableMtrrIndex].Type = Mtrrs->Variables.Mtrr[Index].Base & 0xFF;
    VariableMtrr[VariableMtrrIndex].Valid = TRUE;
    VariableMtrrIndex ++;
  }
  VariableMtrrCount = VariableMtrrIndex;

  //
  // Sort
  //
  if (VariableMtrrCount > 1) {
    for (VariableMtrrIndex = 0; VariableMtrrIndex < VariableMtrrCount; VariableMtrrIndex++) {
      Index = VariableMtrrIndex + 1;
      for (Index = VariableMtrrIndex + 1; Index < VariableMtrrCount; Index++) {
        if (VariableMtrr[VariableMtrrIndex].BaseAddress > VariableMtrr[Index].BaseAddress) {
          CopyMem (&TempVariableMtrr, &VariableMtrr[VariableMtrrIndex], sizeof(VARIABLE_MTRR));
          CopyMem (&VariableMtrr[VariableMtrrIndex], &VariableMtrr[Index], sizeof(VARIABLE_MTRR));
          CopyMem (&VariableMtrr[Index], &TempVariableMtrr, sizeof(VARIABLE_MTRR));
        }
      }
    }
  }

  //
  // Dump
  //
  DEBUG ((DEBUG_INFO, "CACHE Result:\n"));
  for (VariableMtrrIndex = 0; VariableMtrrIndex < VariableMtrrCount; VariableMtrrIndex++) {
    if (VariableMtrr[VariableMtrrIndex].Valid) {
      DEBUG ((DEBUG_INFO, "CACHE - 0x%016lx 0x%016lx %d\n",
        VariableMtrr[VariableMtrrIndex].BaseAddress,
        VariableMtrr[VariableMtrrIndex].Length,
        VariableMtrr[VariableMtrrIndex].Type
        ));
    }
  }

  //
  // Remove overlap
  //
  if (VariableMtrrCount > 1) {
    for (VariableMtrrIndex = 0; VariableMtrrIndex < VariableMtrrCount; VariableMtrrIndex++) {
      Index = VariableMtrrIndex + 1;
      for (Index = VariableMtrrIndex + 1; Index < VariableMtrrCount - 1; Index++) {
        if (VariableMtrr[VariableMtrrIndex].BaseAddress + VariableMtrr[VariableMtrrIndex].Length > VariableMtrr[Index].BaseAddress) {
          VariableMtrr[VariableMtrrIndex].Length = VariableMtrr[Index].BaseAddress - VariableMtrr[VariableMtrrIndex].BaseAddress;
        }
      }
    }
  }

  //
  // Dump
  //
  DEBUG ((DEBUG_INFO, "CACHE Final:\n"));
  for (VariableMtrrIndex = 0; VariableMtrrIndex < VariableMtrrCount; VariableMtrrIndex++) {
    if (VariableMtrr[VariableMtrrIndex].Valid) {
      DEBUG ((DEBUG_INFO, "CACHE - 0x%016lx 0x%016lx %d\n",
        VariableMtrr[VariableMtrrIndex].BaseAddress,
        VariableMtrr[VariableMtrrIndex].Length,
        VariableMtrr[VariableMtrrIndex].Type
        ));
    }
  }
}

EFI_STATUS
TestPointCheckMtrrForPei (
  IN MTRR_SETTINGS  *Mtrrs,
  IN VARIABLE_MTRR  *VariableMtrr
  )
{
  EFI_STATUS                  Status;
  VOID                        *HobList;
  EFI_HOB_HANDOFF_INFO_TABLE  *PhitHob;
  
  HobList = GetHobList ();
  PhitHob = HobList;

  //
  // DRAM must be WB
  //
  DEBUG ((DEBUG_INFO, "MTRR Checking 0x%lx 0x%lx\n", PhitHob->EfiMemoryBottom, PhitHob->EfiMemoryTop - PhitHob->EfiMemoryBottom));
  Status = TestPointCheckCacheType (
             Mtrrs,
             VariableMtrr,
             PhitHob->EfiMemoryBottom,
             PhitHob->EfiMemoryTop - PhitHob->EfiMemoryBottom,
             CacheWriteBack
             );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // FV must be WB or WP
  //

  //
  // MMIO must be UC
  //

  return EFI_SUCCESS;
}

EFI_STATUS
TestPointCheckMtrrForDxe (
  IN MTRR_SETTINGS  *Mtrrs,
  IN VARIABLE_MTRR  *VariableMtrr
  )
{
  EFI_STATUS                  Status;
  VOID                        *HobList;
  EFI_PEI_HOB_POINTERS        Hob;
  EFI_HOB_RESOURCE_DESCRIPTOR *ResourceHob;
  
  HobList = GetHobList ();

  //
  // DRAM must be WB
  //
  for (Hob.Raw = HobList; !END_OF_HOB_LIST (Hob); Hob.Raw = GET_NEXT_HOB (Hob)) {
    if (GET_HOB_TYPE (Hob) == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) {
      ResourceHob = Hob.ResourceDescriptor;
      switch (ResourceHob->ResourceType) {
      case EFI_RESOURCE_SYSTEM_MEMORY:
        if (((ResourceHob->ResourceAttribute & MEMORY_ATTRIBUTE_MASK) == TESTED_MEMORY_ATTRIBUTES) ||
            ((ResourceHob->ResourceAttribute & MEMORY_ATTRIBUTE_MASK) == INITIALIZED_MEMORY_ATTRIBUTES)) {
          DEBUG ((DEBUG_INFO, "MTRR Checking 0x%lx 0x%lx\n", ResourceHob->PhysicalStart, ResourceHob->ResourceLength));
          Status = TestPointCheckCacheType (
                     Mtrrs,
                     VariableMtrr,
                     ResourceHob->PhysicalStart,
                     ResourceHob->ResourceLength,
                     CacheWriteBack
                     );
          if (EFI_ERROR(Status)) {
            return Status;
          }
        }
        break;
      default:
        break;
      }
    }
  }

  //
  // FV must be WB or WP
  //

  //
  // MMIO must be UC
  //

  return EFI_SUCCESS;
}

EFI_STATUS
TestPointCheckMtrr (
  IN BOOLEAN   IsForDxe
  )
{
  EFI_STATUS     Status;
  MTRR_SETTINGS  LocalMtrrs;
  MTRR_SETTINGS  *Mtrrs;
  UINTN          Index;
  UINTN          VariableMtrrCount;
  BOOLEAN        Result;
  VARIABLE_MTRR  VariableMtrr[MTRR_NUMBER_OF_VARIABLE_MTRR];

  DEBUG ((DEBUG_INFO, "==== TestPointCheckMtrr - Enter\n"));

  MtrrGetAllMtrrs (&LocalMtrrs);
  Mtrrs = &LocalMtrrs;
  DEBUG ((DEBUG_INFO, "MTRR Default Type: %016lx\n", Mtrrs->MtrrDefType));
  for (Index = 0; Index < MTRR_NUMBER_OF_FIXED_MTRR; Index++) {
    DEBUG ((DEBUG_INFO, "Fixed MTRR[%02d]   : %016lx\n", Index, Mtrrs->Fixed.Mtrr[Index]));
  }

  VariableMtrrCount = GetVariableMtrrCount ();
  for (Index = 0; Index < VariableMtrrCount; Index++) {
    DEBUG ((DEBUG_INFO, "Variable MTRR[%02d]: Base=%016lx Mask=%016lx\n",
      Index,
      Mtrrs->Variables.Mtrr[Index].Base,
      Mtrrs->Variables.Mtrr[Index].Mask
      ));
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DEBUG ((DEBUG_INFO, "==== TestPointCheckMtrr - Exit\n"));
  
  //
  // Check Mask
  //
  Status = TestPointCheckMtrrMask (Mtrrs);
  if (EFI_ERROR(Status)) {
    Result = FALSE;
  } else {

    ZeroMem (VariableMtrr, sizeof(VariableMtrr));
    TestPointMtrrConvert (Mtrrs, VariableMtrr);

    if (IsForDxe) {
      Status = TestPointCheckMtrrForDxe (Mtrrs, VariableMtrr);
    } else {
      Status = TestPointCheckMtrrForPei (Mtrrs, VariableMtrr);
    }
    if (EFI_ERROR(Status)) {
      Result = FALSE;
    } else {
      Result = TRUE;
    }
  }

  if (!Result) {
    if (IsForDxe) {
      TestPointLibAppendErrorString (
        PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
        NULL,
        TEST_POINT_BYTE2_END_OF_PEI_MTRR_FUNCTIONAL_ERROR_CODE \
          TEST_POINT_END_OF_PEI \
          TEST_POINT_BYTE2_END_OF_PEI_MTRR_FUNCTIONAL_ERROR_STRING
        );
    } else {
      TestPointLibAppendErrorString (
        PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
        NULL,
        TEST_POINT_BYTE1_MEMORY_DISCOVERED_MTRR_FUNCTIONAL_ERROR_CODE \
          TEST_POINT_MEMORY_DISCOVERED \
          TEST_POINT_BYTE1_MEMORY_DISCOVERED_MTRR_FUNCTIONAL_ERROR_STRING
        );
    }
  }

  return Status;
}
