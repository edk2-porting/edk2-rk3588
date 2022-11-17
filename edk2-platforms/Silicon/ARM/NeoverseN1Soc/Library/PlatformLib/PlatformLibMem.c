/** @file
*
*  Copyright (c) 2018 - 2020, ARM Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <NeoverseN1Soc.h>

// The total number of descriptors, including the final "end-of-table" descriptor.
#define MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS 13

/**
  Returns the Virtual Memory Map of the platform.

  This Virtual Memory Map is used by MemoryInitPei Module to initialize the MMU
  on your platform.

  @param[out] VirtualMemoryMap Array of ARM_MEMORY_REGION_DESCRIPTOR describing
                               a Physical-to-Virtual Memory mapping. This array
                               must be ended by a zero-filled entry.
**/
VOID
ArmPlatformGetVirtualMemoryMap (
  IN     ARM_MEMORY_REGION_DESCRIPTOR  **VirtualMemoryMap
  )
{
  UINTN                         Index = 0;
  ARM_MEMORY_REGION_DESCRIPTOR  *VirtualMemoryTable;
  EFI_RESOURCE_ATTRIBUTE_TYPE   ResourceAttributes;
  NEOVERSEN1SOC_PLAT_INFO       *PlatInfo;
  UINT64                        DramBlock2Size;

  PlatInfo = (NEOVERSEN1SOC_PLAT_INFO *)NEOVERSEN1SOC_PLAT_INFO_STRUCT_BASE;
  DramBlock2Size = ((UINT64)(PlatInfo->LocalDdrSize -
                             NEOVERSEN1SOC_DRAM_BLOCK1_SIZE / SIZE_1GB) *
                            (UINT64)SIZE_1GB);

  ResourceAttributes =
    EFI_RESOURCE_ATTRIBUTE_PRESENT |
    EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
    EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
    EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
    EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
    EFI_RESOURCE_ATTRIBUTE_TESTED;

  BuildResourceDescriptorHob (
    EFI_RESOURCE_SYSTEM_MEMORY,
    ResourceAttributes,
    FixedPcdGet64 (PcdDramBlock2Base),
    DramBlock2Size);

  ASSERT (VirtualMemoryMap != NULL);
  Index = 0;

  VirtualMemoryTable = AllocatePool (sizeof (ARM_MEMORY_REGION_DESCRIPTOR) *
                                     MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS);
  if (VirtualMemoryTable == NULL) {
    return;
  }

  // SubSystem Peripherals - Generic Watchdog
  VirtualMemoryTable[Index].PhysicalBase    = NEOVERSEN1SOC_GENERIC_WDOG_BASE;
  VirtualMemoryTable[Index].VirtualBase     = NEOVERSEN1SOC_GENERIC_WDOG_BASE;
  VirtualMemoryTable[Index].Length          = NEOVERSEN1SOC_GENERIC_WDOG_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // SubSystem Peripherals - GIC-600
  VirtualMemoryTable[++Index].PhysicalBase  = NEOVERSEN1SOC_GIC_BASE;
  VirtualMemoryTable[Index].VirtualBase     = NEOVERSEN1SOC_GIC_BASE;
  VirtualMemoryTable[Index].Length          = NEOVERSEN1SOC_GIC_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // SubSystem Peripherals - GICR-600
  VirtualMemoryTable[++Index].PhysicalBase  = NEOVERSEN1SOC_GICR_BASE;
  VirtualMemoryTable[Index].VirtualBase     = NEOVERSEN1SOC_GICR_BASE;
  VirtualMemoryTable[Index].Length          = NEOVERSEN1SOC_GICR_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // OnChip non-secure SRAM
  VirtualMemoryTable[++Index].PhysicalBase  = NEOVERSEN1SOC_NON_SECURE_SRAM_BASE;
  VirtualMemoryTable[Index].VirtualBase     = NEOVERSEN1SOC_NON_SECURE_SRAM_BASE;
  VirtualMemoryTable[Index].Length          = NEOVERSEN1SOC_NON_SECURE_SRAM_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED;

  // PCIe RC Configuration Space
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet32 (PcdPcieRootPortConfigBaseAddress);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet32 (PcdPcieRootPortConfigBaseAddress);
  VirtualMemoryTable[Index].Length          = PcdGet32 (PcdPcieRootPortConfigBaseSize);
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // PCIe ECAM Configuration Space
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet64 (PcdPciExpressBaseAddress);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet64 (PcdPciExpressBaseAddress);
  VirtualMemoryTable[Index].Length          = (FixedPcdGet32 (PcdPcieBusMax) -
                                               FixedPcdGet32 (PcdPcieBusMin) + 1) *
                                              SIZE_1MB;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // PCIe MMIO32 Memory Space
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet32 (PcdPcieMmio32Base);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet32 (PcdPcieMmio32Base);
  VirtualMemoryTable[Index].Length          = PcdGet32 (PcdPcieMmio32Size);
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // PCIe MMIO64 Memory Space
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet64 (PcdPcieMmio64Base);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet64 (PcdPcieMmio64Base);
  VirtualMemoryTable[Index].Length          = PcdGet64 (PcdPcieMmio64Size);
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // SubSystem Pheripherals - UART0
  VirtualMemoryTable[++Index].PhysicalBase  = NEOVERSEN1SOC_UART0_BASE;
  VirtualMemoryTable[Index].VirtualBase     = NEOVERSEN1SOC_UART0_BASE;
  VirtualMemoryTable[Index].Length          = NEOVERSEN1SOC_UART0_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // DDR Primary (2GB)
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet64 (PcdSystemMemoryBase);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet64 (PcdSystemMemoryBase);
  VirtualMemoryTable[Index].Length          = PcdGet64 (PcdSystemMemorySize);
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;

  // DDR Secondary
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet64 (PcdDramBlock2Base);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet64 (PcdDramBlock2Base);
  VirtualMemoryTable[Index].Length          = DramBlock2Size;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;

  // Expansion Peripherals
  VirtualMemoryTable[++Index].PhysicalBase  = NEOVERSEN1SOC_EXP_PERIPH_BASE0;
  VirtualMemoryTable[Index].VirtualBase     = NEOVERSEN1SOC_EXP_PERIPH_BASE0;
  VirtualMemoryTable[Index].Length          = NEOVERSEN1SOC_EXP_PERIPH_BASE0_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // End of Table
  VirtualMemoryTable[++Index].PhysicalBase  = 0;
  VirtualMemoryTable[Index].VirtualBase     = 0;
  VirtualMemoryTable[Index].Length          = 0;
  VirtualMemoryTable[Index].Attributes      = (ARM_MEMORY_REGION_ATTRIBUTES)0;

  ASSERT((Index) < MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS);
  DEBUG ((DEBUG_INIT, "Virtual Memory Table setup complete.\n"));

  *VirtualMemoryMap = VirtualMemoryTable;
}
