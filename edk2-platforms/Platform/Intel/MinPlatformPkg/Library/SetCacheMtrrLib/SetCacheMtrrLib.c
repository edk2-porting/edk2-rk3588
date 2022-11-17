/** @file

SetCacheMtrr library functions.
This implementation is for typical platforms and may not be
needed when cache MTRR will be initialized by FSP.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiPei.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MtrrLib.h>
#include <Library/PeiServicesLib.h>
#include <Guid/SmramMemoryReserve.h>

/**
  Set Cache Mtrr.
**/
VOID
EFIAPI
SetCacheMtrr (
  VOID
  )
{
  EFI_STATUS                  Status;
  EFI_PEI_HOB_POINTERS        Hob;
  MTRR_SETTINGS               MtrrSetting;
  UINT64                      MemoryBase;
  UINT64                      MemoryLength;
  UINT64                      LowMemoryLength;
  UINT64                      HighMemoryLength;
  EFI_BOOT_MODE               BootMode;
  EFI_RESOURCE_ATTRIBUTE_TYPE ResourceAttribute;
  UINT64                      CacheMemoryLength;

  ///
  /// Reset all MTRR setting.
  ///
  ZeroMem(&MtrrSetting, sizeof(MTRR_SETTINGS));

  ///
  /// Cache the Flash area as WP to boost performance
  ///
  Status = MtrrSetMemoryAttributeInMtrrSettings (
                &MtrrSetting,
                (UINTN) PcdGet32 (PcdFlashAreaBaseAddress),
                (UINTN) PcdGet32 (PcdFlashAreaSize),
                CacheWriteProtected
                );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Update MTRR setting from MTRR buffer for Flash Region to be WP to boost performance
  ///
  MtrrSetAllMtrrs (&MtrrSetting);

  ///
  /// Set low to 1 MB. Since 1MB cacheability will always be set
  /// until override by CSM.
  /// Initialize high memory to 0.
  ///
  LowMemoryLength   = 0x100000;
  HighMemoryLength  = 0;
  ResourceAttribute = (
                       EFI_RESOURCE_ATTRIBUTE_PRESENT |
                       EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                       EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
                       EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
                       EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
                       EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
                       );

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  if (BootMode != BOOT_ON_S3_RESUME) {
    ResourceAttribute |= EFI_RESOURCE_ATTRIBUTE_TESTED;
  }

  Status = PeiServicesGetHobList ((VOID **) &Hob.Raw);
  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) {
      if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY) ||
          ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_MEMORY_RESERVED) &&
           (Hob.ResourceDescriptor->ResourceAttribute == ResourceAttribute))
         ) {
        if (Hob.ResourceDescriptor->PhysicalStart >= 0x100000000ULL) {
          HighMemoryLength += Hob.ResourceDescriptor->ResourceLength;
        } else if (Hob.ResourceDescriptor->PhysicalStart >= 0x100000) {
          LowMemoryLength += Hob.ResourceDescriptor->ResourceLength;
        }
      }
    }

    Hob.Raw = GET_NEXT_HOB (Hob);
  }

  DEBUG ((DEBUG_INFO, "Memory Length (Below 4GB) = %lx.\n", LowMemoryLength));
  DEBUG ((DEBUG_INFO, "Memory Length (Above 4GB) = %lx.\n", HighMemoryLength));

  ///
  /// Assume size of main memory is multiple of 256MB
  ///
  MemoryLength = (LowMemoryLength + 0xFFFFFFF) & 0xF0000000;
  MemoryBase = 0;

  CacheMemoryLength = MemoryLength;
  ///
  /// Programming MTRRs to avoid override SPI region with UC when MAX TOLUD Length >= 3.5GB
  ///
  if (MemoryLength > 0xDC000000) {
     CacheMemoryLength = 0xC0000000;
     Status = MtrrSetMemoryAttributeInMtrrSettings (
                &MtrrSetting,
                MemoryBase,
                CacheMemoryLength,
                CacheWriteBack
                );
     ASSERT_EFI_ERROR (Status);

     MemoryBase = 0xC0000000;
     CacheMemoryLength = MemoryLength - 0xC0000000;
     if (MemoryLength > 0xE0000000) {
        CacheMemoryLength = 0x20000000;
        Status = MtrrSetMemoryAttributeInMtrrSettings (
                 &MtrrSetting,
                 MemoryBase,
                 CacheMemoryLength,
                 CacheWriteBack
                 );
        ASSERT_EFI_ERROR (Status);

        MemoryBase = 0xE0000000;
        CacheMemoryLength = MemoryLength - 0xE0000000;
     }
  }

  Status = MtrrSetMemoryAttributeInMtrrSettings (
                &MtrrSetting,
                MemoryBase,
                CacheMemoryLength,
                CacheWriteBack
                );
  ASSERT_EFI_ERROR (Status);

  if (LowMemoryLength != MemoryLength) {
     MemoryBase = LowMemoryLength;
     MemoryLength -= LowMemoryLength;
     Status = MtrrSetMemoryAttributeInMtrrSettings (
                   &MtrrSetting,
                   MemoryBase,
                   MemoryLength,
                   CacheUncacheable
                   );
      ASSERT_EFI_ERROR (Status);
  }

  ///
  /// VGA-MMIO - 0xA0000 to 0xC0000 to be UC
  ///
  Status = MtrrSetMemoryAttributeInMtrrSettings (
                &MtrrSetting,
                0xA0000,
                0x20000,
                CacheUncacheable
                );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Update MTRR setting from MTRR buffer
  ///
  MtrrSetAllMtrrs (&MtrrSetting);

  return ;
}

/**
  Update MTRR setting in EndOfPei phase.
  This function will clear temporary memory (CAR) phase MTRR settings
  and configure MTRR to cover permanent memory.

  @retval  EFI_SUCCESS  The function completes successfully.
  @retval  Others       Some error occurs.
**/
EFI_STATUS
EFIAPI
SetCacheMtrrAfterEndOfPei (
  VOID
  )
{
  EFI_STATUS                            Status;
  MTRR_SETTINGS                         MtrrSetting;
  EFI_PEI_HOB_POINTERS                  Hob;
  UINT64                                MemoryBase;
  UINT64                                MemoryLength;
  UINT64                                Power2Length;
  EFI_BOOT_MODE                         BootMode;
  UINTN                                 Index;
  UINT64                                SmramSize;
  UINT64                                SmramBase;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK        *SmramHobDescriptorBlock;
  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  if (BootMode == BOOT_ON_S3_RESUME) {
    return EFI_SUCCESS;
  }
  //
  // Clear the CAR Settings
  //
  ZeroMem(&MtrrSetting, sizeof(MTRR_SETTINGS));

  //
  // Default Cachable attribute will be set to WB to support large memory size/hot plug memory
  //
  MtrrSetting.MtrrDefType &= ~((UINT64)(0xFF));
  MtrrSetting.MtrrDefType |= (UINT64) CacheWriteBack;

  //
  // Set fixed cache for memory range below 1MB
  //
  Status = MtrrSetMemoryAttributeInMtrrSettings (
                         &MtrrSetting,
                         0x0,
                         0xA0000,
                         CacheWriteBack
                         );
  ASSERT_EFI_ERROR (Status);

  Status = MtrrSetMemoryAttributeInMtrrSettings (
                         &MtrrSetting,
                         0xA0000,
                         0x20000,
                         CacheUncacheable
                         );
  ASSERT_EFI_ERROR (Status);

  Status = MtrrSetMemoryAttributeInMtrrSettings (
                         &MtrrSetting,
                         0xC0000,
                         0x40000,
                         CacheWriteProtected
                         );
  ASSERT_EFI_ERROR ( Status);

  //
  // PI SMM IPL can't set SMRAM to WB because at that time CPU ARCH protocol is not available.
  // Set cacheability of SMRAM to WB here to improve SMRAM initialization performance.
  //
  SmramSize = 0;
  SmramBase = 0;
  Status = PeiServicesGetHobList ((VOID **) &Hob.Raw);
  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION) {
      if (CompareGuid (&Hob.Guid->Name, &gEfiSmmSmramMemoryGuid)) {
        SmramHobDescriptorBlock = (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK *) (Hob.Guid + 1);
        for (Index = 0; Index < SmramHobDescriptorBlock->NumberOfSmmReservedRegions; Index++) {
          if (SmramHobDescriptorBlock->Descriptor[Index].PhysicalStart > 0x100000) {
            SmramSize += SmramHobDescriptorBlock->Descriptor[Index].PhysicalSize;
            if (SmramBase == 0 || SmramBase > SmramHobDescriptorBlock->Descriptor[Index].CpuStart) {
              SmramBase = SmramHobDescriptorBlock->Descriptor[Index].CpuStart;
            }
          }
        }
        break;
      }
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }

  //
  // Set non system memory as UC
  //
  MemoryBase   = 0x100000000;

  //
  // Add IED size to set whole SMRAM as WB to save MTRR count
  //
  MemoryLength = MemoryBase - (SmramBase + SmramSize);
  while (MemoryLength != 0) {
    Power2Length = GetPowerOfTwo64 (MemoryLength);
    MemoryBase -= Power2Length;
    Status = MtrrSetMemoryAttributeInMtrrSettings (
                &MtrrSetting,
                MemoryBase,
                Power2Length,
                CacheUncacheable
                );
    ASSERT_EFI_ERROR (Status);
    MemoryLength -= Power2Length;
  }

  DEBUG ((DEBUG_INFO, "PcdPciReservedMemAbove4GBLimit - 0x%lx\n", PcdGet64 (PcdPciReservedMemAbove4GBLimit)));
  DEBUG ((DEBUG_INFO, "PcdPciReservedMemAbove4GBBase - 0x%lx\n", PcdGet64 (PcdPciReservedMemAbove4GBBase)));
  if (PcdGet64 (PcdPciReservedMemAbove4GBLimit) > PcdGet64 (PcdPciReservedMemAbove4GBBase)) {
    Status = MtrrSetMemoryAttributeInMtrrSettings (
                           &MtrrSetting,
                           PcdGet64 (PcdPciReservedMemAbove4GBBase),
                           PcdGet64 (PcdPciReservedMemAbove4GBLimit) - PcdGet64 (PcdPciReservedMemAbove4GBBase) + 1,
                           CacheUncacheable
                           );
    ASSERT_EFI_ERROR ( Status);
  }

  DEBUG ((DEBUG_INFO, "PcdPciReservedPMemAbove4GBLimit - 0x%lx\n", PcdGet64 (PcdPciReservedPMemAbove4GBLimit)));
  DEBUG ((DEBUG_INFO, "PcdPciReservedPMemAbove4GBBase - 0x%lx\n", PcdGet64 (PcdPciReservedPMemAbove4GBBase)));
  if (PcdGet64 (PcdPciReservedPMemAbove4GBLimit) > PcdGet64 (PcdPciReservedPMemAbove4GBBase)) {
    Status = MtrrSetMemoryAttributeInMtrrSettings (
                           &MtrrSetting,
                           PcdGet64 (PcdPciReservedPMemAbove4GBBase),
                           PcdGet64 (PcdPciReservedPMemAbove4GBLimit) - PcdGet64 (PcdPciReservedPMemAbove4GBBase) + 1,
                           CacheUncacheable
                           );
    ASSERT_EFI_ERROR ( Status);
  }

  //
  // Update MTRR setting from MTRR buffer
  //
  MtrrSetAllMtrrs (&MtrrSetting);

  return Status;
}
