/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiSmm.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Guid/MemoryAttributesTable.h>

#define NEXT_MEMORY_DESCRIPTOR(MemoryDescriptor, Size) \
  ((EFI_MEMORY_DESCRIPTOR *)((UINT8 *)(MemoryDescriptor) + (Size)))

extern EFI_MEMORY_DESCRIPTOR *mUefiMemoryMap;
extern UINTN                 mUefiMemoryMapSize;
extern UINTN                 mUefiDescriptorSize;

extern EFI_GCD_MEMORY_SPACE_DESCRIPTOR *mGcdMemoryMap;
extern EFI_GCD_IO_SPACE_DESCRIPTOR     *mGcdIoMap;
extern UINTN                           mGcdMemoryMapNumberOfDescriptors;
extern UINTN                           mGcdIoMapNumberOfDescriptors;

EFI_STATUS
TestPointCheckPageTable (
  IN EFI_PHYSICAL_ADDRESS   BaseAddress,
  IN UINTN                  Length,
  IN BOOLEAN                IsCode,
  IN BOOLEAN                IsOutsideSmram
  );

BOOLEAN
IsUefiPageNotPresent (
  IN EFI_MEMORY_DESCRIPTOR  *MemoryMap
  )
{
  switch (MemoryMap->Type) {
  case EfiLoaderCode:
  case EfiLoaderData:
  case EfiBootServicesCode:
  case EfiBootServicesData:
  case EfiConventionalMemory:
  case EfiUnusableMemory:
  case EfiACPIReclaimMemory:
    return TRUE;
  default:
    return FALSE;
  }
}

EFI_STATUS
TestPointCheckSmmCommunicationBuffer (
  IN EFI_MEMORY_DESCRIPTOR        *UefiMemoryMap,
  IN UINTN                        UefiMemoryMapSize,
  IN UINTN                        UefiDescriptorSize,
  IN EFI_MEMORY_ATTRIBUTES_TABLE  *MemoryAttributesTable
  )
{
  EFI_STATUS            ReturnStatus;
  EFI_STATUS            Status;
  EFI_MEMORY_DESCRIPTOR *MemoryMap;
  UINTN                 MemoryMapEntryCount;
  UINTN                 Index;
  EFI_MEMORY_DESCRIPTOR *Entry;

  DEBUG ((DEBUG_INFO, "==== TestPointCheckSmmCommunicationBuffer - Enter\n"));

  ReturnStatus = EFI_SUCCESS;
  MemoryMapEntryCount = UefiMemoryMapSize/UefiDescriptorSize;
  MemoryMap = UefiMemoryMap;
  for (Index = 0; Index < MemoryMapEntryCount; Index++) {
    if (IsUefiPageNotPresent(MemoryMap)) {
      DEBUG ((DEBUG_INFO, "UEFI MemoryMap Checking 0x%lx - 0x%x\n", MemoryMap->PhysicalStart, EFI_PAGES_TO_SIZE(MemoryMap->NumberOfPages)));
      Status = TestPointCheckPageTable (
                 MemoryMap->PhysicalStart,
                 EFI_PAGES_TO_SIZE((UINTN)MemoryMap->NumberOfPages),
                 FALSE,
                 TRUE
                 );
      if (EFI_ERROR(Status)) {
        ReturnStatus = Status;
      }
    }
    MemoryMap = NEXT_MEMORY_DESCRIPTOR(MemoryMap, UefiDescriptorSize);
  }

  if (MemoryAttributesTable != NULL) {
    Entry = (EFI_MEMORY_DESCRIPTOR *)(MemoryAttributesTable + 1);
    for (Index = 0; Index < MemoryAttributesTable->NumberOfEntries; Index++) {
      if (Entry->Type == EfiRuntimeServicesCode || Entry->Type == EfiRuntimeServicesData) {
        if ((Entry->Attribute & EFI_MEMORY_RO) != 0) {
          DEBUG ((DEBUG_INFO, "UEFI MemoryAttributeTable Checking 0x%lx - 0x%x\n", Entry->PhysicalStart, EFI_PAGES_TO_SIZE(Entry->NumberOfPages)));
          Status = TestPointCheckPageTable (
                     Entry->PhysicalStart,
                     EFI_PAGES_TO_SIZE((UINTN)Entry->NumberOfPages),
                     FALSE,
                     TRUE
                     );
          if (EFI_ERROR(Status)) {
            ReturnStatus = Status;
          }
        }
      }
      Entry = NEXT_MEMORY_DESCRIPTOR (Entry, MemoryAttributesTable->DescriptorSize);
    }
  }

  if (EFI_ERROR (ReturnStatus)) {
    TestPointLibAppendErrorString (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      TEST_POINT_BYTE6_SMM_READY_TO_LOCK_SECURE_SMM_COMMUNICATION_BUFFER_ERROR_CODE \
        TEST_POINT_SMM_READY_TO_BOOT \
        TEST_POINT_BYTE6_SMM_READY_TO_LOCK_SECURE_SMM_COMMUNICATION_BUFFER_ERROR_STRING
      );
  }

  DEBUG ((DEBUG_INFO, "==== TestPointCheckSmmCommunicationBuffer - Exit\n"));
  return EFI_SUCCESS;
}
