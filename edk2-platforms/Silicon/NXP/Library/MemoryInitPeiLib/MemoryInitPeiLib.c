/** @file
*
*  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
*
*  Copyright 2019-2020 NXP
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <PiPei.h>

#include <Library/ArmMmuLib.h>
#include <Library/ArmPlatformLib.h>
#include <Library/ArmSmcLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>

#include "MemoryInitPeiLib.h"


VOID
BuildMemoryTypeInformationHob (
  VOID
  );

VOID
InitMmu (
  IN ARM_MEMORY_REGION_DESCRIPTOR  *MemoryTable
  )
{

  VOID                          *TranslationTableBase;
  UINTN                         TranslationTableSize;
  RETURN_STATUS                 Status;

  //Note: Because we called PeiServicesInstallPeiMemory() before
  //to call InitMmu() the MMU Page Table resides in DRAM
  //(even at the top of DRAM as it is the first permanent memory allocation)
  Status = ArmConfigureMmu (MemoryTable, &TranslationTableBase, &TranslationTableSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: Failed to enable MMU\n"));
  }
}

STATIC
UINTN
GetDramSize (
  IN VOID
  )
{
  ARM_SMC_ARGS  ArmSmcArgs;

  ArmSmcArgs.Arg0 = SMC_DRAM_BANK_INFO;
  ArmSmcArgs.Arg1 = SMC_DRAM_TOTAL_DRAM_ARG1;

  ArmCallSmc (&ArmSmcArgs);

  if (ArmSmcArgs.Arg0 == SMC_OK) {
    return ArmSmcArgs.Arg1;
  }

  // return 0 means no DDR found.
  return 0;
}

STATIC
EFI_STATUS
GetDramRegionsInfo (
  OUT DRAM_REGION_INFO *DramRegions,
  IN  UINT32           NumRegions
  )
{
  ARM_SMC_ARGS  ArmSmcArgs;
  UINT32        Index;
  UINTN         RemainingDramSize;
  UINTN         BaseAddress;
  UINTN         Size;

  RemainingDramSize = GetDramSize ();
  DEBUG ((DEBUG_INFO, "DRAM Total Size 0x%lx \n", RemainingDramSize));

  // Ensure Total Dram Size is valid
  ASSERT (RemainingDramSize != 0);

  for (Index = 0; Index < NumRegions; Index++) {
    ArmSmcArgs.Arg0 = SMC_DRAM_BANK_INFO;
    ArmSmcArgs.Arg1 = Index;

    ArmCallSmc (&ArmSmcArgs);

    if (ArmSmcArgs.Arg0 == SMC_OK) {
      BaseAddress = ArmSmcArgs.Arg1;
      Size = ArmSmcArgs.Arg2;
      ASSERT (BaseAddress && Size);

      DramRegions[Index].BaseAddress = BaseAddress;
      DramRegions[Index].Size = Size;
      RemainingDramSize -= Size;

      DEBUG ((DEBUG_INFO, "DRAM Region[%d]: start 0x%lx, size 0x%lx\n",
              Index, BaseAddress, Size));

      if (RemainingDramSize == 0) {
        return EFI_SUCCESS;
      }
    } else {
      break;
    }
  }

  DEBUG ((DEBUG_ERROR, "RemainingDramSize = %u !! Ensure that all DDR regions "
          "have been accounted for\n", RemainingDramSize));

  return EFI_BUFFER_TOO_SMALL;
}

/**
  Get the installed RAM information.
  Initialize Memory HOBs (Resource Descriptor HOBs)
  Set the PcdSystemMemoryBase and PcdSystemMemorySize.

  @return  EFI_SUCCESS  Successfuly retrieved the system memory information
**/
EFI_STATUS
EFIAPI
MemoryInitPeiLibConstructor (
  VOID
  )
{
  INT32                         Index;
  UINTN                         BaseAddress;
  UINTN                         Size;
  UINTN                         Top;
  DRAM_REGION_INFO              DramRegions[MAX_DRAM_REGIONS];
  EFI_RESOURCE_ATTRIBUTE_TYPE   ResourceAttributes;
  UINTN                         FdBase;
  UINTN                         FdTop;
  BOOLEAN                       FoundSystemMem;

  ResourceAttributes = (
    EFI_RESOURCE_ATTRIBUTE_PRESENT |
    EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
    EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
    EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
    EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
    EFI_RESOURCE_ATTRIBUTE_TESTED
  );

  FoundSystemMem = FALSE;
  ZeroMem (DramRegions, sizeof (DramRegions));

  (VOID)GetDramRegionsInfo (DramRegions, ARRAY_SIZE (DramRegions));

  FdBase = (UINTN)PcdGet64 (PcdFdBaseAddress);
  FdTop = FdBase + (UINTN)PcdGet32 (PcdFdSize);

  // Declare memory regions to system
  // The DRAM region info is sorted based on the RAM address is SOC memory map.
  // i.e. DramRegions[0] is at lower address, as compared to DramRegions[1].
  // The goal to start from last region is to find the topmost RAM region that
  // can contain UEFI DXE region i.e. PcdSystemMemoryUefiRegionSize.
  // If UEFI were to allocate any reserved or runtime region, it would be
  // allocated from topmost RAM region.
  // This ensures that maximum amount of lower RAM (32 bit addresses) are left
  // for OS to allocate to devices that can only work with 32bit physical
  // addresses. E.g. legacy devices that need to DMA to 32bit addresses.
  for (Index = MAX_DRAM_REGIONS - 1; Index >= 0; Index--) {
    if (DramRegions[Index].Size == 0) {
      continue;
    }

    BaseAddress = DramRegions[Index].BaseAddress;
    Top = DramRegions[Index].BaseAddress + DramRegions[Index].Size;

    // EDK2 does not have the concept of boot firmware copied into DRAM.
    // To avoid the DXE core to overwrite this area we must create a memory
    // allocation HOB for the region, but this only works if we split off the
    // underlying resource descriptor as well.
    if (FdBase >= BaseAddress && FdTop <= Top) {
      // Update Size
      Size = FdBase - BaseAddress;
      if (Size) {
        BuildResourceDescriptorHob (
          EFI_RESOURCE_SYSTEM_MEMORY,
          ResourceAttributes,
          BaseAddress,
          Size
        );
      }
      // create the System Memory HOB for the firmware
      BuildResourceDescriptorHob (
        EFI_RESOURCE_SYSTEM_MEMORY,
        ResourceAttributes,
        FdBase,
        PcdGet32 (PcdFdSize)
      );
      // Create the System Memory HOB for the remaining region (top of the FD)s
      Size = Top - FdTop;
      if (Size) {
        BuildResourceDescriptorHob (
          EFI_RESOURCE_SYSTEM_MEMORY,
          ResourceAttributes,
          FdTop,
          Size
        );
      };
      // Mark the memory covering the Firmware Device as boot services data
      BuildMemoryAllocationHob (PcdGet64 (PcdFdBaseAddress),
                                PcdGet32 (PcdFdSize),
                                EfiBootServicesData);
    } else {
      BuildResourceDescriptorHob (
          EFI_RESOURCE_SYSTEM_MEMORY,
          ResourceAttributes,
          DramRegions[Index].BaseAddress,
          DramRegions[Index].Size
      );
    }

    if (FoundSystemMem == TRUE) {
      continue;
    }

    Size = DramRegions[Index].Size;

    if (FdBase >= BaseAddress && FdTop <= Top) {
      Size -= (UINTN)PcdGet32 (PcdFdSize);
    }

    if (Size >= FixedPcdGet32 (PcdSystemMemoryUefiRegionSize)) {
      FoundSystemMem = TRUE;
      PcdSet64S (PcdSystemMemoryBase, BaseAddress);
      PcdSet64S (PcdSystemMemorySize, Size);
    }
  }

  ASSERT (FoundSystemMem == TRUE);

  return EFI_SUCCESS;
}

/**
  Initialize MMU

  @param[in] UefiMemoryBase  Base address of region used by UEFI in
                             permanent memory
  @param[in] UefiMemorySize  Size of the region used by UEFI in permanent memory

  @return  EFI_SUCCESS  Successfuly Initialize MMU
**/
EFI_STATUS
EFIAPI
MemoryPeim (
  IN EFI_PHYSICAL_ADDRESS               UefiMemoryBase,
  IN UINT64                             UefiMemorySize
  )
{
  ARM_MEMORY_REGION_DESCRIPTOR *MemoryTable;

  // Get Virtual Memory Map from the Platform Library
  ArmPlatformGetVirtualMemoryMap (&MemoryTable);

  // Initialize Mmu
  InitMmu (MemoryTable);

  if (FeaturePcdGet (PcdPrePiProduceMemoryTypeInformationHob)) {
    // Optional feature that helps prevent EFI memory map fragmentation.
    BuildMemoryTypeInformationHob ();
  }

  return EFI_SUCCESS;
}
