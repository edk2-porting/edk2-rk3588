/**@file
  Build up platform processor information.

  Copyright (c) 2019, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

//
// The package level header files this module uses
//
#include <PiPei.h>

//
// The Library classes this module consumes
//
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>

#include <SmbiosProcessorSpecificData.h>
#include <ProcessorSpecificHobData.h>
#include <SiFiveU5MCCoreplex.h>
#include <Library/SiFiveU54.h>

/**
  Build up processor-specific HOB for U5MC Coreplex

  @param  UniqueId      Unique ID of this U5MC Coreplex processor

  @return EFI_SUCCESS     The PEIM initialized successfully.

**/
EFI_STATUS
EFIAPI
CreateU5MCCoreplexProcessorSpecificDataHob (
  IN UINTN UniqueId
  )
{
  EFI_STATUS Status;
  UINT32 HartIdNumber;
  RISC_V_PROCESSOR_SPECIFIC_HOB_DATA *GuidHobData;
  EFI_GUID *ParentCoreGuid;
  BOOLEAN MCSupport;

  DEBUG ((DEBUG_INFO, "Building U5 Coreplex processor information HOB\n"));

  HartIdNumber = 0;
  ParentCoreGuid = PcdGetPtr(PcdSiFiveU5MCCoreplexGuid);
  MCSupport = PcdGetBool (PcdE5MCSupported);
  if (MCSupport == TRUE) {
    Status = CreateU54E51CoreProcessorSpecificDataHob (ParentCoreGuid, UniqueId, HartIdNumber, FALSE, TRUE, &GuidHobData);
    if (EFI_ERROR (Status) && Status != EFI_UNSUPPORTED) {
      DEBUG ((DEBUG_ERROR, "Faile to build U5MC processor informatino HOB\n"));
      ASSERT (FALSE);
    } else {
      if (!EFI_ERROR (Status)) {
        DEBUG((DEBUG_INFO, "Support E5 Monitor core on U5 platform, HOB at address 0x%x\n", GuidHobData));
      }
    }
    HartIdNumber ++;
  }
  for (; HartIdNumber < (FixedPcdGet32 (PcdNumberofU5Cores) + (UINT32)MCSupport); HartIdNumber ++) {
    Status = CreateU54E51CoreProcessorSpecificDataHob (ParentCoreGuid, UniqueId, HartIdNumber, (HartIdNumber == FixedPcdGet32 (PcdBootHartId)), FALSE, &GuidHobData);
    if (EFI_ERROR (Status) && Status != EFI_UNSUPPORTED) {
      DEBUG ((DEBUG_ERROR, "Faile to build U5MC processor informatino HOB\n"));
      ASSERT (FALSE);
    }  else {
      if (!EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Support U5 application core on U5 platform, HOB Data at address 0x%x\n", GuidHobData));
      }
    }
  }
  DEBUG ((DEBUG_INFO, "Support %d U5 application cores on U5 platform\n", HartIdNumber - (UINT32)MCSupport));

  if (HartIdNumber != FixedPcdGet32 (PcdHartCount)) {
    DEBUG ((DEBUG_ERROR, "Improper core settings...\n"));
    DEBUG ((DEBUG_ERROR, "    PcdHartCount\n"));
    DEBUG ((DEBUG_ERROR, "    PcdNumberofU5Cores\n"));
    DEBUG ((DEBUG_ERROR, "    PcdE5MCSupported\n\n"));
    ASSERT (FALSE);
  }
  return Status;
}

/**
  Function to build processor related SMBIOS information. RISC-V SMBIOS DXE driver collect
  this information and build SMBIOS Type4 and Type7 record.

  @param  ProcessorUid    Unique ID of physical processor which owns this core.
  @param  SmbiosHobPtr    Pointer to receive RISC_V_PROCESSOR_SMBIOS_HOB_DATA. The pointers
                          maintained in this structure is only valid before memory is discovered.
                          Access to those pointers after memory is installed will cause unexpected issues.

  @return EFI_SUCCESS     The SMBIOS Hobs were created successfully.

**/
EFI_STATUS
EFIAPI
CreateU5MCProcessorSmbiosDataHob (
  IN UINTN     ProcessorUid,
  OUT RISC_V_PROCESSOR_SMBIOS_HOB_DATA **SmbiosHobPtr
  )
{
  EFI_GUID *GuidPtr;
  RISC_V_PROCESSOR_TYPE7_HOB_DATA L2CacheDataHob;
  RISC_V_PROCESSOR_SMBIOS_HOB_DATA SmbiosDataHob;
  RISC_V_PROCESSOR_TYPE4_HOB_DATA *ProcessorDataHobPtr;
  RISC_V_PROCESSOR_TYPE7_HOB_DATA *L1CacheDataHobPtr;
  RISC_V_PROCESSOR_TYPE7_HOB_DATA *L2CacheDataHobPtr;
  RISC_V_PROCESSOR_SMBIOS_HOB_DATA *SmbiosDataHobPtr;

  DEBUG ((DEBUG_INFO, "%a: Entry\n", __FUNCTION__));

  if (SmbiosHobPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  CreateU54SmbiosType7L1DataHob (ProcessorUid, &L1CacheDataHobPtr);
  CreateU54SmbiosType4DataHob (ProcessorUid, &ProcessorDataHobPtr);

  //
  // Build up SMBIOS type 7 L2 cache record.
  //
  ZeroMem((VOID *)&L2CacheDataHob, sizeof (RISC_V_PROCESSOR_TYPE7_HOB_DATA));
  L2CacheDataHob.PrcessorGuid = *((EFI_GUID *)PcdGetPtr (PcdSiFiveU5MCCoreplexGuid));
  L2CacheDataHob.ProcessorUid = ProcessorUid;
  L2CacheDataHob.SmbiosType7Cache.SocketDesignation = TO_BE_FILLED_BY_VENDOR;
  L2CacheDataHob.SmbiosType7Cache.CacheConfiguration = RISC_V_CACHE_CONFIGURATION_CACHE_LEVEL_2 | \
      RISC_V_CACHE_CONFIGURATION_LOCATION_EXTERNAL | \
      RISC_V_CACHE_CONFIGURATION_ENABLED | \
      RISC_V_CACHE_CONFIGURATION_MODE_UNKNOWN;
  L2CacheDataHob.SmbiosType7Cache.MaximumCacheSize = TO_BE_FILLED_BY_VENDOR;
  L2CacheDataHob.SmbiosType7Cache.InstalledSize = TO_BE_FILLED_BY_VENDOR;
  L2CacheDataHob.SmbiosType7Cache.SupportedSRAMType.Unknown = 1;
  L2CacheDataHob.SmbiosType7Cache.CurrentSRAMType.Unknown = 1;
  L2CacheDataHob.SmbiosType7Cache.CacheSpeed = TO_BE_FILLED_BY_VENDOR;
  L2CacheDataHob.SmbiosType7Cache.ErrorCorrectionType = TO_BE_FILLED_BY_VENDOR;
  L2CacheDataHob.SmbiosType7Cache.SystemCacheType = CacheTypeUnified;
  L2CacheDataHob.SmbiosType7Cache.Associativity = TO_BE_FILLED_BY_VENDOR;
  GuidPtr = (EFI_GUID *)PcdGetPtr (PcdProcessorSmbiosType7GuidHobGuid);
  L2CacheDataHobPtr = (RISC_V_PROCESSOR_TYPE7_HOB_DATA *)BuildGuidDataHob (GuidPtr, (VOID *)&L2CacheDataHob, sizeof (RISC_V_PROCESSOR_TYPE7_HOB_DATA));
  if (L2CacheDataHobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Fail to create GUID HOB of SiFive U5 MC Coreplex L2 cache RISC_V_PROCESSOR_TYPE7_HOB_DATA.\n"));
    ASSERT (FALSE);
  }

  ZeroMem((VOID *)&SmbiosDataHob, sizeof (RISC_V_PROCESSOR_SMBIOS_HOB_DATA));
  SmbiosDataHob.Processor = ProcessorDataHobPtr;
  SmbiosDataHob.L1Cache = L1CacheDataHobPtr;
  SmbiosDataHob.L2Cache = L2CacheDataHobPtr;
  SmbiosDataHob.L3Cache = NULL;

  GuidPtr = (EFI_GUID *)PcdGetPtr (PcdProcessorSmbiosGuidHobGuid);
  SmbiosDataHobPtr = (RISC_V_PROCESSOR_SMBIOS_HOB_DATA *)BuildGuidDataHob (GuidPtr, (VOID *)&SmbiosDataHob, sizeof (RISC_V_PROCESSOR_SMBIOS_HOB_DATA));
  if (SmbiosDataHobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Fail to create GUID HOB of SiFive U5MC Coreplex RISC_V_PROCESSOR_SMBIOS_HOB_DATA.\n"));
    ASSERT (FALSE);
  }
  *SmbiosHobPtr = SmbiosDataHobPtr;
  DEBUG ((DEBUG_INFO, "%a: Exit\n", __FUNCTION__));

  return EFI_SUCCESS;
}
