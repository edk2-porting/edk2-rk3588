/** @file
 *
 *  Copyright (c) 2017-2018, Andrey Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <PiPei.h>

#include <Library/ArmMmuLib.h>
#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/RPiMem.h>

VOID
BuildMemoryTypeInformationHob (
  VOID
  );

STATIC
VOID
InitMmu (
  IN ARM_MEMORY_REGION_DESCRIPTOR  *MemoryTable
  )
{
  RETURN_STATUS Status;

  //Note: Because we called PeiServicesInstallPeiMemory() before to call InitMmu() the MMU Page Table
  //      resides in DRAM (even at the top of DRAM as it is the first permanent memory allocation)
  Status = ArmConfigureMmu (MemoryTable, NULL, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: Failed to enable MMU\n"));
  }
}

STATIC
VOID
AddBasicMemoryRegion (
  IN ARM_MEMORY_REGION_DESCRIPTOR *Desc
)
{
  BuildResourceDescriptorHob (
    EFI_RESOURCE_SYSTEM_MEMORY,
    EFI_RESOURCE_ATTRIBUTE_PRESENT |
    EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
    EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
    EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
    EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
    EFI_RESOURCE_ATTRIBUTE_TESTED,
    Desc->PhysicalBase,
    Desc->Length
  );
}

STATIC
VOID
AddRuntimeServicesRegion (
  IN ARM_MEMORY_REGION_DESCRIPTOR *Desc
)
{
  AddBasicMemoryRegion (Desc);

  BuildMemoryAllocationHob (
    Desc->PhysicalBase,
    Desc->Length,
    EfiRuntimeServicesData
  );
}

STATIC
VOID
AddUnmappedMemoryRegion (
  IN ARM_MEMORY_REGION_DESCRIPTOR *Desc
  )
{
  // Do nothing
}

STATIC
VOID
AddReservedMemoryRegion (
  IN ARM_MEMORY_REGION_DESCRIPTOR *Desc
  )
{
  AddBasicMemoryRegion (Desc);

  BuildMemoryAllocationHob (
    Desc->PhysicalBase,
    Desc->Length,
    EfiReservedMemoryType
  );
}

void (*AddRegion[]) (IN ARM_MEMORY_REGION_DESCRIPTOR *Desc) = {
  AddUnmappedMemoryRegion,
  AddBasicMemoryRegion,
  AddRuntimeServicesRegion,
  AddReservedMemoryRegion,
  };

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
  IN EFI_PHYSICAL_ADDRESS  UefiMemoryBase,
  IN UINT64                UefiMemorySize
  )
{
  ARM_MEMORY_REGION_DESCRIPTOR *MemoryTable;
  RPI_MEMORY_REGION_INFO       *MemoryInfo;
  UINTN                        Index;

  // Get Virtual Memory Map from the Platform Library
  ArmPlatformGetVirtualMemoryMap (&MemoryTable);

  // Get additional info not provided by MemoryTable
  RpiPlatformGetVirtualMemoryInfo (&MemoryInfo);

  // Register each memory region
  for (Index = 0; MemoryTable[Index].Length != 0; Index++) {
    ASSERT (MemoryInfo[Index].Type < ARRAY_SIZE (AddRegion));
    DEBUG ((DEBUG_INFO, "%s:\n"
      "\tPhysicalBase: 0x%lX\n"
      "\tVirtualBase: 0x%lX\n"
      "\tLength: 0x%lX\n",
      MemoryInfo[Index].Name,
      MemoryTable[Index].PhysicalBase,
      MemoryTable[Index].VirtualBase,
      MemoryTable[Index].Length));
    AddRegion[MemoryInfo[Index].Type] (&MemoryTable[Index]);
  }

  // Build Memory Allocation Hob
  InitMmu (MemoryTable);

  if (FeaturePcdGet (PcdPrePiProduceMemoryTypeInformationHob)) {
    // Optional feature that helps prevent EFI memory map fragmentation.
    BuildMemoryTypeInformationHob ();
  }

  return EFI_SUCCESS;
}
