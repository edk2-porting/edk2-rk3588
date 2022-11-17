/** @file

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiPei.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/SmramMemoryReserve.h>
#include <Ppi/SmmAccess.h>

CHAR8 *mSmramStateName[] = {
  "Open",
  "Closed",
  "Locked",
  "Cacheable",
  "Allocated",
  "NeedTest",
  "NeedEccInit",
};

VOID
DumpSmramDescriptor (
  IN UINTN                 NumberOfSmmReservedRegions,
  IN EFI_SMRAM_DESCRIPTOR  *Descriptor
  )
{
  UINTN                                 Index;
  UINTN                                 BitIndex;

  for (Index = 0; Index < NumberOfSmmReservedRegions; Index++) {
    DEBUG ((DEBUG_INFO,
      "  BA=%016lx (A=%016lx) L=%016lx  State=%016lx",
      Descriptor[Index].PhysicalStart,
      Descriptor[Index].CpuStart,
      Descriptor[Index].PhysicalSize,
      Descriptor[Index].RegionState
      ));
    DEBUG ((DEBUG_INFO, "  ("));
    for (BitIndex = 0; BitIndex < sizeof(mSmramStateName)/sizeof(mSmramStateName[0]); BitIndex++) {
      if ((Descriptor[Index].RegionState & LShiftU64 (1, BitIndex)) != 0) {
        DEBUG ((DEBUG_INFO, mSmramStateName[BitIndex]));
        DEBUG ((DEBUG_INFO, ","));
      }
    }
    DEBUG ((DEBUG_INFO, ")\n"));
  }
}

EFI_STATUS
CheckSmramDescriptor (
  IN UINTN                 NumberOfSmmReservedRegions,
  IN EFI_SMRAM_DESCRIPTOR  *Descriptor
  )
{
  UINTN   Index;
  UINT64  Base;
  UINT64  Length;

  Base = 0;
  Length = 0;
  for (Index = 0; Index < NumberOfSmmReservedRegions; Index++) {
    if (Base == 0) {
      Base   = Descriptor[Index].PhysicalStart;
      Length = Descriptor[Index].PhysicalSize;
    } else {
      if (Base + Length == Descriptor[Index].PhysicalStart) {
        Length = Length + Descriptor[Index].PhysicalSize;
      } else if (Descriptor[Index].PhysicalStart + Descriptor[Index].PhysicalSize == Base) {
        Base = Descriptor[Index].PhysicalStart;
        Length = Length + Descriptor[Index].PhysicalSize;
      } else {
        DEBUG ((DEBUG_ERROR, "Smram is not adjacent\n"));
        TestPointLibAppendErrorString (
          PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
          NULL,
          TEST_POINT_BYTE2_END_OF_PEI_SYSTEM_RESOURCE_FUNCTIONAL_ERROR_CODE \
            TEST_POINT_END_OF_PEI \
            TEST_POINT_BYTE2_END_OF_PEI_SYSTEM_RESOURCE_FUNCTIONAL_ERROR_STRING
          );
        return EFI_INVALID_PARAMETER;
      }
    }
  }        

  if (Length != GetPowerOfTwo64 (Length)) {
    DEBUG ((DEBUG_ERROR, "Smram is not aligned\n"));
    TestPointLibAppendErrorString (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      TEST_POINT_BYTE2_END_OF_PEI_SYSTEM_RESOURCE_FUNCTIONAL_ERROR_CODE \
        TEST_POINT_MEMORY_DISCOVERED \
        TEST_POINT_BYTE2_END_OF_PEI_SYSTEM_RESOURCE_FUNCTIONAL_ERROR_STRING
      );
    return EFI_INVALID_PARAMETER;
  }
  return EFI_SUCCESS;
}

#if 0
EFI_STATUS
TestPointCheckSmramHob (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS                  Hob;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK        *SmramHobDescriptorBlock;
  UINT64                                Base;
  UINT64                                Length;
  UINTN                                 Index;
  
  DEBUG ((DEBUG_INFO, "==== TestPointCheckSmramHob - Enter\n"));
  Hob.Raw = GetHobList ();
  DEBUG ((DEBUG_INFO, "SMRAM HOB\n"));
  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION) {
      if (CompareGuid (&Hob.Guid->Name, &gEfiSmmSmramMemoryGuid)) {
        SmramHobDescriptorBlock = (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK *) (Hob.Guid + 1);
        DumpSmramDescriptor (SmramHobDescriptorBlock->NumberOfSmmReservedRegions, SmramHobDescriptorBlock->Descriptor);
        break;
      }
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }
  DEBUG ((DEBUG_INFO, "==== TestPointCheckSmramHob - Exit\n"));

  Base = 0;
  Length = 0;
  Hob.Raw = GetHobList ();
  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION) {
      if (CompareGuid (&Hob.Guid->Name, &gEfiSmmSmramMemoryGuid)) {
        SmramHobDescriptorBlock = (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK *) (Hob.Guid + 1);
        for (Index = 0; Index < SmramHobDescriptorBlock->NumberOfSmmReservedRegions; Index++) {
          if (Base == 0) {
            Base = SmramHobDescriptorBlock->Descriptor[Index].PhysicalStart;
            Length = SmramHobDescriptorBlock->Descriptor[Index].PhysicalSize;
          } else {
            if (Base + Length == SmramHobDescriptorBlock->Descriptor[Index].PhysicalStart) {
              Length = Length + SmramHobDescriptorBlock->Descriptor[Index].PhysicalSize;
            } else if (SmramHobDescriptorBlock->Descriptor[Index].PhysicalStart + SmramHobDescriptorBlock->Descriptor[Index].PhysicalSize == Base) {
              Base = SmramHobDescriptorBlock->Descriptor[Index].PhysicalStart;
              Length = Length + SmramHobDescriptorBlock->Descriptor[Index].PhysicalSize;
            } else {
              DEBUG ((DEBUG_ERROR, "SmramHob is not adjacent\n"));
              TestPointLibAppendErrorString (
                PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
                NULL,
                TEST_POINT_BYTE2_END_OF_PEI_SYSTEM_RESOURCE_FUNCTIONAL_ERROR_CODE \
                  TEST_POINT_END_OF_PEI \
                  TEST_POINT_BYTE2_END_OF_PEI_SYSTEM_RESOURCE_FUNCTIONAL_ERROR_STRING
                );
              return EFI_INVALID_PARAMETER;
            }
          }
        }
        break;
      }
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }

  if (Length != GetPowerOfTwo64 (Length)) {
    DEBUG ((DEBUG_ERROR, "SmramHob is not aligned\n"));
    TestPointLibAppendErrorString (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      TEST_POINT_BYTE1_MEMORY_DISCOVERED_MEMORY_RESOURCE_FUNCTIONAL_ERROR_CODE \
        TEST_POINT_MEMORY_DISCOVERED \
        TEST_POINT_BYTE1_MEMORY_DISCOVERED_MEMORY_RESOURCE_FUNCTIONAL_ERROR_STRING
      );
    return EFI_INVALID_PARAMETER;
  }
  return EFI_SUCCESS;
}
#endif

EFI_STATUS
TestPointCheckSmmInfoPei (
  VOID
  )
{
  EFI_STATUS               Status;
  PEI_SMM_ACCESS_PPI       *SmmAccess;
  UINTN                    Size;
  EFI_SMRAM_DESCRIPTOR     *SmramRanges;
  
  DEBUG ((DEBUG_INFO, "==== TestPointCheckSmmInfoPei - Enter\n"));
  
  Status = PeiServicesLocatePpi (&gPeiSmmAccessPpiGuid, 0, NULL, (VOID **)&SmmAccess);
  if (EFI_ERROR (Status)) {
    Status = EFI_SUCCESS;
    goto Done ;
  }
  
  Size = 0;
  Status = SmmAccess->GetCapabilities ((EFI_PEI_SERVICES **)GetPeiServicesTablePointer (), SmmAccess, &Size, NULL);
  ASSERT (Status == EFI_BUFFER_TOO_SMALL);

  SmramRanges = (EFI_SMRAM_DESCRIPTOR *) AllocateZeroPool (Size);
  ASSERT (SmramRanges != NULL);

  Status = SmmAccess->GetCapabilities ((EFI_PEI_SERVICES **)GetPeiServicesTablePointer (), SmmAccess, &Size, SmramRanges);
  ASSERT_EFI_ERROR (Status);
  
  DEBUG ((DEBUG_INFO, "SMRAM Info\n"));
  DumpSmramDescriptor (Size / sizeof (EFI_SMRAM_DESCRIPTOR), SmramRanges);

  Status = CheckSmramDescriptor (Size / sizeof (EFI_SMRAM_DESCRIPTOR), SmramRanges);

  FreePool (SmramRanges);

Done:
  DEBUG ((DEBUG_INFO, "==== TestPointCheckSmmInfoPei - Exit\n"));

  return Status;
}
