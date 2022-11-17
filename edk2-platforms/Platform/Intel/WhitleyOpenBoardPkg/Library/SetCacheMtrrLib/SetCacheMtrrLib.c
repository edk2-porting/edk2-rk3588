/** @file
  SetCacheMtrr library functions.

  @copyright
  Copyright 2006 - 2021 Intel Corporation. <BR>

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

#include <CpuAndRevisionDefines.h>
#include <Library/PeiServicesTablePointerLib.h>

#include <Ppi/MpServices.h>

#include <Guid/PlatformInfo.h>
#include <Guid/MemoryMapData.h>
#include <Protocol/IioUds.h>
#include <Register/ArchitecturalMsr.h>
#include <Register/Cpuid.h>
#include <Ppi/DynamicSiLibraryPpi.h>
#include <Cpu/CpuIds.h>

#define EFI_MAX_ADDRESS   0xFFFFFFFF

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

  This function finds the start address and size of control or Block window region in the system.

  @param Host - pointer to sysHost structure on stack
  @param Memtype - Type of the memory range
  @param RangeSize - pointer to the variable to store the StartAddress
  @param RangeSize - pointer to the variable to store RangeSize

  @retval EFI_SUCCESS - success
          EFI_NOT_FOUND - Region not found.

**/
EFI_STATUS
GetMemoryRegionRange(
  struct SystemMemoryMapHob  *systemMemoryMap,
  UINT16                     Memtype,
  EFI_PHYSICAL_ADDRESS       *StartAddress,
  EFI_PHYSICAL_ADDRESS       *RangeSize)
{
  UINT8         Index;
  EFI_STATUS    Status = EFI_NOT_FOUND;
  UINT8         Socket = 0;
  EFI_PHYSICAL_ADDRESS Limit = 0;

  *RangeSize = 0;
  *StartAddress = 0;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (systemMemoryMap->Socket[Socket].SAD[0].Enable == 0) {
      continue;
    }

    for (Index = 0; Index < MAX_SAD_RULES; Index++) {
      if (systemMemoryMap->Socket[Socket].SAD[Index].Enable == 0) {
        continue;
      }
      if (systemMemoryMap->Socket[Socket].SAD[Index].type == Memtype) {
        if ((*StartAddress == 0) && (Index > 0)) {
          //Store the start address for the specified range in bytes
          *StartAddress = (EFI_PHYSICAL_ADDRESS)MultU64x32 ((UINT64)systemMemoryMap->Socket[Socket].SAD[Index-1].Limit, CONVERT_64MB_TO_BYTE);
        }

        if (MultU64x32((UINT64)systemMemoryMap->Socket[Socket].SAD[Index].Limit, CONVERT_64MB_TO_BYTE) > Limit) {
          //Store/Update the end address for the specified range in bytes if greater than previous limit
          Limit = (EFI_PHYSICAL_ADDRESS)MultU64x32 ((UINT64)systemMemoryMap->Socket[Socket].SAD[Index].Limit, CONVERT_64MB_TO_BYTE);
        }
      }
    }
  }

  if (Limit != 0) {
    *RangeSize = Limit - *StartAddress;
    Status = EFI_SUCCESS;
  }

  return Status;
}

/**

  MP programming MSR_MCA_ON_NONNEM_CACHABLEMMIO_EN at socket level.

  @param PeiServices -      Ptr of EFI_PEI_SERVICES ptr
  @param mPeiMpServices -   Ptr of EFI_PEI_MP_SERVICES_PPI

  @retval EFI_SUCCESS     -  Programming done
  @retval EFI_UNSUPPORTED -  Not support this platform

**/
EFI_STATUS
PkgMpEnableMcaOnCacheableMmio(
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN EFI_PEI_MP_SERVICES_PPI    *mPeiMpServices
)
{
  UINT16                    PackageDoneBitmap;
  UINTN                     NumberOfProcessors;
  UINTN                     NumberEnabledProcessors;
  UINTN                     Index;
  UINT8                     TotalSockets = 0;
  EFI_PROCESSOR_INFORMATION ProcInfo;
  EFI_STATUS                Status;
  DYNAMIC_SI_LIBARY_PPI     *DynamicSiLibraryPpi = NULL;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  if (!DynamicSiLibraryPpi->IsCpuAndRevision (CPU_SKX, REV_ALL) && !DynamicSiLibraryPpi->IsCpuAndRevision (CPU_CLX, REV_ALL) && !DynamicSiLibraryPpi->IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    return EFI_UNSUPPORTED;
  }

  PackageDoneBitmap = 0;
  mPeiMpServices->GetNumberOfProcessors(
                    PeiServices,
                    mPeiMpServices,
                    &NumberOfProcessors,
                    &NumberEnabledProcessors
                    );

  //
  // Find total number of sockets present on the board
  //
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if (DynamicSiLibraryPpi->SocketPresent (Index)) {
      TotalSockets++;
    }
  }

  //
  // Loop through all the enabled processors and find one thread per socket present
  // to write the MSR.  Remote sockets need to use StartupThisAP.
  //
  for (Index = 0; Index < NumberOfProcessors; Index++) {
    mPeiMpServices->GetProcessorInfo(
                      PeiServices,
                      mPeiMpServices,
                      Index,
                      &ProcInfo
                      );

    if (!(PackageDoneBitmap & (1 << ProcInfo.Location.Package)) && (ProcInfo.StatusFlag & PROCESSOR_ENABLED_BIT)) {

      PackageDoneBitmap |= (1 << ProcInfo.Location.Package);
      TotalSockets--;

      if (ProcInfo.StatusFlag & PROCESSOR_AS_BSP_BIT) {
        DynamicSiLibraryPpi->EnableMcaOnCacheableMmio();
      } else {
        Status = mPeiMpServices->StartupThisAP (
                      PeiServices,
                      mPeiMpServices,
                      (EFI_AP_PROCEDURE)DynamicSiLibraryPpi->EnableMcaOnCacheableMmio,
                      Index,
                      0,
                      NULL
                      );
      }

      //
      // All sockets are programmed, skip checking rest of threads
      //
      if (TotalSockets == 0) {
        break;
      }
    }
  }

  return EFI_SUCCESS;
}

/**

  Function to set all MTRRs on the current thread to the MTRR set passed in.

  @param MtrrSettings -  Pointer to MTRR Settings to use

  @retval None

**/
VOID
SetAllMtrrs(
    VOID    *MtrrSettings
)
{
  MtrrSetAllMtrrs((MTRR_SETTINGS*)MtrrSettings);
}

/**

  Function that analyzes memory length passed in to see if there is an
  appropriate address to perform a more efficient top down coverage of
  the memory range.  At this address bit, it is more efficient to overshoot
  memory range with WB cache type and cover the gap with UC cache type.

  @param MemoryLength -  Memory length of entire memory region

  @retval Zero            - No efficient top down approaches found.  More efficient
                            to cover memory range conventional way (bottom up).
          PwrOfTwo Value  - PowerOfTwo bit where the top down approach is most efficient

**/
UINT64
FindTopDownPowerOfTwo (
  IN UINT64               MemoryLength
  )
{
  UINT8   Index;
  BOOLEAN FoundOne = FALSE;
  UINT8   ZeroCount = 0;
  UINT8   OnesCount = 0;
  UINT64  TopDownBit = 0;
  UINT8   MtrrSavings = 0;

  for (Index = 0; Index < 64; Index++){
    if (MemoryLength & LShiftU64(1, Index)) {
      OnesCount++;
      FoundOne = TRUE;
    } else {
      //
      // If there are more 1's than 0's plus 2 between least significant bit set
      // and current bit under test, then top down approach is more efficient.
      // Continue to loop through memory length to look for more efficiencies
      // and compare them against previous efficiencies found to pick best power of two.
      //
      if (((ZeroCount + 2) < OnesCount) && ((OnesCount - (ZeroCount + 2)) > MtrrSavings)) {
        TopDownBit = LShiftU64(1, (Index - 1));
        MtrrSavings = OnesCount - (ZeroCount + 2);
      }
      if (FoundOne) {
        ZeroCount++;
      }
    }
  }

  //
  // MtrrLib can handle this case efficiently
  //
  if (TopDownBit == GetPowerOfTwo64(MemoryLength)) {
    TopDownBit = 0;
  }

  return TopDownBit;
}

/**

  Recalculate the memory length to prevent MTRR out of resource error.

  @param MemoryLength      -  Memory Length that we want to truncate

  @retval UINT64   - New truncated memory length

**/
UINT64
MemLengthRecalculation (
  IN UINT64                     MemoryLength
  )
{
  UINT8  BitIndex;

  for (BitIndex = 0;BitIndex < 64; BitIndex++) {
    if ((RShiftU64(MemoryLength, BitIndex) & 1) == 1) {
      //
      // Clear lowest power of two bit found
      //
      MemoryLength &= ~LShiftU64(1, BitIndex);
      break;
    }
  }
  return MemoryLength;
}

/**

  Sets the uncached part of upper memory as reserved to prevent OS from using.
  The uncached region will always be at the top of high memory.

  @param OriginalMemoryLength   -  Original top of memory value
  @param NewMemoryLength        -  New memory range used for successful programming

  @retval None

**/
VOID
ReserveUncachedMemory (
  IN UINT64           OriginalMemoryLength,
  IN UINT64           NewMemoryLength
  )
{
  EFI_PEI_HOB_POINTERS          Hob;
  EFI_HOB_RESOURCE_DESCRIPTOR   *ResourceHob;
  VOID                          *HobStart;
  UINT64                        TempLength;

  HobStart = GetFirstHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR);
  //
  // Scan resource descriptor hobs to set our required range attribute as tested
  //
  for (Hob.Raw = HobStart; !END_OF_HOB_LIST(Hob); Hob.Raw = GET_NEXT_HOB(Hob)) {
    if (GET_HOB_TYPE (Hob) == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) {
      ResourceHob = Hob.ResourceDescriptor;
      if ((ResourceHob->PhysicalStart >= NewMemoryLength) &&
        ((ResourceHob->PhysicalStart + ResourceHob->ResourceLength) <= OriginalMemoryLength)) {
        //
        // Range is completely included in the uncached area, mark as tested
        //
        ResourceHob->ResourceAttribute |= EFI_RESOURCE_ATTRIBUTE_TESTED;
      } else if ((ResourceHob->PhysicalStart < NewMemoryLength) &&
        (ResourceHob->PhysicalStart + ResourceHob->ResourceLength > NewMemoryLength)) {
        //
        // Shrink previous HOB to base of uncached region, create new hob to cover uncached space
        //
        TempLength = ResourceHob->ResourceLength;
        ResourceHob->ResourceLength = NewMemoryLength - ResourceHob->PhysicalStart;
        BuildResourceDescriptorHob(
          EFI_RESOURCE_SYSTEM_MEMORY,
          (
            EFI_RESOURCE_ATTRIBUTE_PRESENT |
            EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
            EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
            EFI_RESOURCE_ATTRIBUTE_TESTED
            ),
          ResourceHob->PhysicalStart + ResourceHob->ResourceLength,
          TempLength - ResourceHob->ResourceLength
          );
        if ((ResourceHob->PhysicalStart + ResourceHob->ResourceLength +
          (TempLength - ResourceHob->ResourceLength)) == OriginalMemoryLength) {
            break;
        }
      }
      if (ResourceHob->PhysicalStart + ResourceHob->ResourceLength == OriginalMemoryLength) {
        break;
      }
    }
  }

  HobStart = GetFirstHob (EFI_HOB_TYPE_MEMORY_ALLOCATION);
  //
  // Scan memory allocation hobs to make sure this range is free
  //
  for (Hob.Raw = HobStart; !END_OF_HOB_LIST(Hob); Hob.Raw = GET_NEXT_HOB(Hob)) {
    if (GET_HOB_TYPE (Hob) == EFI_HOB_TYPE_MEMORY_ALLOCATION) {
      if ((Hob.MemoryAllocation->AllocDescriptor.MemoryBaseAddress > NewMemoryLength) &&
        (OriginalMemoryLength > Hob.MemoryAllocation->AllocDescriptor.MemoryBaseAddress)) {
        //
        // If hob range is above the range we want to reserve, set limit to that base.
        //
        OriginalMemoryLength = Hob.MemoryAllocation->AllocDescriptor.MemoryBaseAddress;
      } else if ((Hob.MemoryAllocation->AllocDescriptor.MemoryBaseAddress +
        Hob.MemoryAllocation->AllocDescriptor.MemoryLength - 1) > NewMemoryLength) {
        //
        // If there is a range allocated that starts below but comes into the reserved range,
        // adjust the base so it starts above previous allocation
        //
        NewMemoryLength = Hob.MemoryAllocation->AllocDescriptor.MemoryBaseAddress +
          Hob.MemoryAllocation->AllocDescriptor.MemoryLength;
      }
    }
  }

  //
  // Create hob to reserve the memory
  //
  if (NewMemoryLength < OriginalMemoryLength) {
    BuildMemoryAllocationHob (
      (EFI_PHYSICAL_ADDRESS)NewMemoryLength,
      OriginalMemoryLength - NewMemoryLength,
      EfiReservedMemoryType
    );
  }
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
  EFI_STATUS            Status;
  UINT64                LowUncableBase;
  UINT64                MemoryBase;
  UINT64                TempQword;
  UINT64                MemoryLength;
  UINT64                TempMemoryLength;
  UINT64                OriginalMemoryLength;
  UINT64                TopDownBit;
  EFI_PEI_HOB_POINTERS  Hob;
  EFI_PLATFORM_INFO     *PlatformInfo;
  EFI_HOB_GUID_TYPE     *GuidHob;
  IIO_UDS               *IioUds;
  MTRR_SETTINGS         MtrrSettings;
  EFI_GUID              UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
  EFI_PEI_MP_SERVICES_PPI    *mPeiMpServices = NULL;
  EFI_BOOT_MODE              BootMode;
  CONST EFI_PEI_SERVICES **  PeiServices;


  PeiServices = GetPeiServicesTablePointer();
  BootMode = GetBootModeHob();
  if (BootMode != BOOT_ON_S3_RESUME) {
    //
    // Get required HOBs to be used to generate MTRR programming
    //
    GuidHob = GetFirstGuidHob (&UniversalDataGuid);
    if (GuidHob == NULL) {
      ASSERT(GuidHob != NULL);
      return EFI_NOT_FOUND;
    }
    IioUds = GET_GUID_HOB_DATA (GuidHob);

    GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
    if (GuidHob == NULL) {
      ASSERT(GuidHob != NULL);
      return EFI_NOT_FOUND;
    }
    PlatformInfo = GET_GUID_HOB_DATA(GuidHob);
    //
    // Calculate the low uncacheable base address
    //
    if (IioUds->PlatformData.PciExpressBase < IioUds->PlatformData.PlatGlobalMmio32Base) {
      LowUncableBase = IioUds->PlatformData.PciExpressBase;
    } else {
      LowUncableBase = IioUds->PlatformData.PlatGlobalMmio32Base;
    }

    LowUncableBase &= (0x0FFF00000);

    //
    // Reset all Mtrrs to 0 including fixed MTRR and variable MTRR
    //
    ZeroMem(&MtrrSettings, sizeof(MTRR_SETTINGS));

    //
    // Set fixed cache for memory range below 1MB
    //
    Status = MtrrSetMemoryAttributeInMtrrSettings(
      &MtrrSettings,
      0,
      0xA0000,
      EFI_CACHE_WRITEBACK
      );
    ASSERT_EFI_ERROR(Status);

    Status = MtrrSetMemoryAttributeInMtrrSettings(
      &MtrrSettings,
      0xA0000,
      0x60000,
      EFI_CACHE_UNCACHEABLE
      );
    ASSERT_EFI_ERROR(Status);

    //
    // Base set to 1mb due to MtrrLib programming method
    //
    MemoryBase = BASE_1MB;
    MemoryLength = LowUncableBase;

    Status = (*PeiServices)->GetHobList (PeiServices, &Hob.Raw);
    while (!END_OF_HOB_LIST (Hob)) {
      if (Hob.Header->HobType == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) {
        if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY) ||
            (Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_MEMORY_MAPPED_IO &&
            (Hob.ResourceDescriptor->ResourceAttribute & EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE)) ||
            (Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_MEMORY_RESERVED &&
             (Hob.ResourceDescriptor->ResourceAttribute & EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE))) {
          if (Hob.ResourceDescriptor->PhysicalStart > EFI_MAX_ADDRESS) {
            TempQword = Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength;
            if (TempQword > MemoryLength) {
              MemoryLength = TempQword;
            }
          }
        }
      }
      Hob.Raw = GET_NEXT_HOB (Hob);
    }

    TempMemoryLength = MemoryLength;
    OriginalMemoryLength = MemoryLength;

    DEBUG((DEBUG_ERROR, "Total Memory size: 0x%lx\n", MemoryLength));

    Status = EFI_SUCCESS;
    //
    // Loop will continue until MTRR programming is successfully done.
    // All regions reserved in this loop are considered essential regions
    // If any of them fail to fit, memory is truncated by lowest power of
    // two until all regions fit into the programming
    //
    do {
      if (Status == RETURN_OUT_OF_RESOURCES){
        //
        // Ran out of MTRRs: lower top of high memory by lowest power of two bit and retry
        //
        MemoryLength = MemLengthRecalculation (TempMemoryLength);
        DEBUG((DEBUG_ERROR, "MTRR: %r, attempting: 0x%lx\n", Status, MemoryLength));
        //
        // Restore the MemoryBase to its original value, save MemoryLength
        //
        MemoryBase = BASE_1MB;
        TempMemoryLength = MemoryLength;
        ZeroMem(&(MtrrSettings.Variables), sizeof(MTRR_VARIABLE_SETTINGS));
      }

      TopDownBit = FindTopDownPowerOfTwo(MemoryLength);

      //
      // If TopDownBit has a value, then we found a more efficient address length
      // to use a top down approach.  We will walk through the full address length to
      // program MTRRs individually.  BASE_1MB fixups are due to MtrrLib program method.
      //
      if(TopDownBit){
        DEBUG((DEBUG_INFO, "Efficient Top Down Power of Two = %lx\n\n", TopDownBit));
        while (MemoryLength != 0) {
          if (GetPowerOfTwo64(MemoryLength) == TopDownBit) {
            //
            // Overshoot address with WB and cover remaining gap with UC
            //
            TempQword = MemoryLength;
            MemoryLength = LShiftU64(GetPowerOfTwo64 (MemoryLength), 1);

            if(MemoryBase == BASE_1MB) {
              MemoryLength -= BASE_1MB;
            }

            Status = MtrrSetMemoryAttributeInMtrrSettings(
                &MtrrSettings,
                MemoryBase,
                MemoryLength,
                EFI_CACHE_WRITEBACK
                );

            if (Status == RETURN_OUT_OF_RESOURCES) {
              break;
            }

            if(MemoryBase == BASE_1MB) {
              MemoryBase = 0;
              MemoryLength += BASE_1MB;
            }

            MemoryBase += TempQword;
            MemoryLength -= TempQword;

            //
            // Program UC region gap between top of memory and WB MTRR
            //
            Status = MtrrSetMemoryAttributeInMtrrSettings(
                &MtrrSettings,
                MemoryBase,
                MemoryLength,
                EFI_CACHE_UNCACHEABLE
                );

            if (Status == RETURN_OUT_OF_RESOURCES) {
              break;
            }

            MemoryLength = 0;
          } else {
            //
            // Grow next power of two upwards and adjust base and length
            //
            TempQword = GetPowerOfTwo64(MemoryLength);
            MemoryLength -= TempQword;

            if(MemoryBase == BASE_1MB) {
              TempQword -= BASE_1MB;
            }

            Status = MtrrSetMemoryAttributeInMtrrSettings(
              &MtrrSettings,
              MemoryBase,
              TempQword,
              EFI_CACHE_WRITEBACK
              );

            if (Status == RETURN_OUT_OF_RESOURCES) {
              break;
            }

            MemoryBase += TempQword;
          }
        }
        if (Status == RETURN_OUT_OF_RESOURCES) {
          continue;
        }
      } else {
        //
        // Create a WB region for the entire memory region
        //
        Status = MtrrSetMemoryAttributeInMtrrSettings(
          &MtrrSettings,
          MemoryBase,
          MemoryLength - BASE_1MB,
          EFI_CACHE_WRITEBACK
          );

        if (Status == RETURN_OUT_OF_RESOURCES) {
          continue;
        }
      }

      //
      // Punch UC hole for lower MMIO region
      //
      Status = MtrrSetMemoryAttributeInMtrrSettings(
        &MtrrSettings,
        LowUncableBase,
        EFI_MAX_ADDRESS - LowUncableBase + 1,
        EFI_CACHE_UNCACHEABLE
        );

      if (Status == RETURN_OUT_OF_RESOURCES) {
        continue;
      }

    } while (Status == RETURN_OUT_OF_RESOURCES);
    //
    // Assert if there was an error other than resource issue
    //
    ASSERT_EFI_ERROR (Status);

    //
    // Set PSMI Trace Region to uncached
    //
    if (PlatformInfo->MemData.PsmiUcTraceRegionSize != 0) {
      Status = MtrrSetMemoryAttributeInMtrrSettings(
          &MtrrSettings,
          PlatformInfo->MemData.PsmiUcTraceRegionBase,
          PlatformInfo->MemData.PsmiUcTraceRegionSize,
          EFI_CACHE_UNCACHEABLE
          );
      if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "PSMI- Could not set Trace Region MemType to UC\n"));
      }
    }

    //
    // Set the calculated MTRR settings for the BSP
    //
    MtrrSetAllMtrrs (&MtrrSettings);

    //
    // Need to mark the uncached memory as reserved
    //
    if (OriginalMemoryLength > TempMemoryLength) {
      DEBUG((DEBUG_ERROR, "New TOHM: 0x%lx, Previous: 0x%lx\n", TempMemoryLength, OriginalMemoryLength));
      ReserveUncachedMemory (OriginalMemoryLength, TempMemoryLength);
    }
  }

  Status = (*PeiServices)->LocatePpi (
                    PeiServices,
                    &gEfiPeiMpServicesPpiGuid,
                    0,
                    NULL,
                    &mPeiMpServices
                    );

  if (BootMode != BOOT_ON_S3_RESUME) {
    if(!EFI_ERROR(Status)){
      //
      // Sync all AP MTRRs with BSP
      //
      Status = mPeiMpServices->StartupAllAPs (
                    PeiServices,
                    mPeiMpServices,
                    (EFI_AP_PROCEDURE)SetAllMtrrs,
                    FALSE,
                    0,
                    (VOID*)&MtrrSettings
                    );
    }
  }

  PkgMpEnableMcaOnCacheableMmio (PeiServices, mPeiMpServices);


  return EFI_SUCCESS;
}
