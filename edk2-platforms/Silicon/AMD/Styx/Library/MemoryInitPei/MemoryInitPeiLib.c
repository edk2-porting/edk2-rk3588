/** @file

  Copyright (c) 2011-2014, ARM Limited. All rights reserved.<BR>
  Copyright (c) 2014 - 2016 AMD Inc. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
/**
  Derived from:
   ArmPlatformPkg/MemoryInitPei/MemoryInitPeiLib.c

**/

#include <PiPei.h>

#include <Library/ArmMmuLib.h>
#include <Library/ArmPlatformLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>

VOID
BuildMemoryTypeInformationHob (
  VOID
  );

VOID
InitMmu (
  VOID
  )
{
  ARM_MEMORY_REGION_DESCRIPTOR  *MemoryTable;
  VOID                          *TranslationTableBase;
  UINTN                         TranslationTableSize;
  RETURN_STATUS                 Status;

  // Get Virtual Memory Map from the Platform Library
  ArmPlatformGetVirtualMemoryMap (&MemoryTable);

  // Note: Because we called PeiServicesInstallPeiMemory() before to call
  //       InitMmu() the MMU Page Table resides in DRAM (even at the top
  //       of DRAM as it is the first permanent memory allocation)
  Status = ArmConfigureMmu (MemoryTable, &TranslationTableBase, &TranslationTableSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Error: Failed to enable MMU\n"));
  }
}

STATIC
VOID
MoveNvStoreImage (
  VOID
  )
{
  VOID        *OldBase, *NewBase;
  UINTN       Size;
  EFI_STATUS  Status;

  //
  // Move the in-memory image of the NV store firmware volume to a dynamically
  // allocated buffer. This gets rid of the annoying static memory reservation
  // at the base of memory where all other UEFI allocations are near the top.
  //
  OldBase = (VOID *)FixedPcdGet64 (PcdFlashNvStorageOriginalBase);

  Size = FixedPcdGet32 (PcdFlashNvStorageVariableSize) +
         FixedPcdGet32 (PcdFlashNvStorageFtwWorkingSize) +
         FixedPcdGet32 (PcdFlashNvStorageFtwSpareSize);

  NewBase = AllocateAlignedRuntimePages (EFI_SIZE_TO_PAGES (Size), SIZE_64KB);
  ASSERT (NewBase != NULL);

  CopyMem (NewBase, OldBase, Size);

  DEBUG ((EFI_D_INFO, "%a: Relocating NV store FV from %p to %p\n",
    __FUNCTION__, OldBase, NewBase));

  Status = PcdSet64S (PcdFlashNvStorageVariableBase64, (UINT64)NewBase);
  ASSERT_EFI_ERROR (Status);

  Status = PcdSet64S (PcdFlashNvStorageFtwWorkingBase64,
             (UINT64)NewBase + FixedPcdGet32 (PcdFlashNvStorageVariableSize));
  ASSERT_EFI_ERROR (Status);

  Status = PcdSet64S (PcdFlashNvStorageFtwSpareBase64, (UINT64)NewBase +
             FixedPcdGet32 (PcdFlashNvStorageVariableSize) +
             FixedPcdGet32 (PcdFlashNvStorageFtwWorkingSize));
  ASSERT_EFI_ERROR (Status);
}

/*++

Routine Description:



Arguments:

  FileHandle  - Handle of the file being invoked.
  PeiServices - Describes the list of possible PEI Services.

Returns:

  Status -  EFI_SUCCESS if the boot mode could be set

--*/
EFI_STATUS
EFIAPI
MemoryPeim (
  IN EFI_PHYSICAL_ADDRESS               UefiMemoryBase,
  IN UINT64                             UefiMemorySize
  )
{
  UINT64  Base, Size;

  // Ensure PcdSystemMemorySize has been set
  ASSERT (PcdGet64 (PcdSystemMemorySize) != 0);

  //
  // Now, the permanent memory has been installed, we can call AllocatePages()
  //

  Base = PcdGet64 (PcdSystemMemoryBase);
  Size = PcdGet64 (PcdSystemMemorySize);

  //
  // For now, we assume that the trusted firmware region is at the base of
  // system memory, since that is much easier to deal with.
  //
  ASSERT (Base == PcdGet64 (PcdTrustedFWMemoryBase));

  Base += PcdGet64 (PcdTrustedFWMemorySize);
  Size -= PcdGet64 (PcdTrustedFWMemorySize);

  // Reserved Trusted Firmware region
  BuildResourceDescriptorHob (
      EFI_RESOURCE_SYSTEM_MEMORY,
    ( EFI_RESOURCE_ATTRIBUTE_PRESENT |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
      EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
      EFI_RESOURCE_ATTRIBUTE_TESTED ),
      PcdGet64 (PcdTrustedFWMemoryBase),
      PcdGet64 (PcdTrustedFWMemorySize)
    );

  BuildMemoryAllocationHob (
    PcdGet64 (PcdTrustedFWMemoryBase),
    PcdGet64 (PcdTrustedFWMemorySize),
    EfiReservedMemoryType
  );

  // Declare system memory
  BuildResourceDescriptorHob (
      EFI_RESOURCE_SYSTEM_MEMORY,
    ( EFI_RESOURCE_ATTRIBUTE_PRESENT |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
      EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
      EFI_RESOURCE_ATTRIBUTE_TESTED ),
      Base,
      Size
  );

  // Build Memory Allocation Hob
  InitMmu ();

  // Optional feature that helps prevent EFI memory map fragmentation.
  if (FeaturePcdGet (PcdPrePiProduceMemoryTypeInformationHob)) {
    BuildMemoryTypeInformationHob ();
  }

  MoveNvStoreImage ();

  return EFI_SUCCESS;
}
