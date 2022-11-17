/**@file
  Build up platform processor information of SiFive U54 core.

  Copyright (c) 2019 - 2020, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <IndustryStandard/RiscVOpensbi.h>

//
// The Library classes this module consumes
//
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/FirmwareContextProcessorSpecificLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/RiscVEdk2SbiLib.h>

#include <RiscVImpl.h>
#include <sbi/sbi_hart.h>
#include <sbi/sbi_platform.h>
#include <sbi/sbi_scratch.h>

/**
  Function to build core specific information HOB for U54 or E51 core.

  @param  ParentProcessorGuid    Parent processor of this core. ParentProcessorGuid
                                 could be the same as CoreGuid if one processor has
                                 only one core.
  @param  ParentProcessorUid     Unique ID of pysical processor which owns this core.
  @param  HartId                 Hart ID of this core.
  @param  IsBootHart             TRUE means this is the boot HART.
  @param  IsManagementCore       TRUE means this is for the E51 management core, not U54
  @param  GuidHobData            Pointer to RISC_V_PROCESSOR_SPECIFIC_HOB_DATA.

  @return EFI_SUCCESS     The PEIM initialized successfully.
          EFI_UNSUPPORTED HART is ignored by platform.
          EFI_NOT_FOUND   Processor specific data hob is not available.
**/
EFI_STATUS
EFIAPI
CreateU54E51CoreProcessorSpecificDataHob (
  IN EFI_GUID  *ParentProcessorGuid,
  IN UINTN     ParentProcessorUid,
  IN UINTN     HartId,
  IN BOOLEAN   IsBootHart,
  IN BOOLEAN   IsManagementCore,
  OUT RISC_V_PROCESSOR_SPECIFIC_HOB_DATA **GuidHobData
  )
{
  RISC_V_PROCESSOR_SPECIFIC_HOB_DATA *CoreGuidHob;
  EFI_GUID *ProcessorSpecDataHobGuid;
  RISC_V_PROCESSOR_SPECIFIC_HOB_DATA ProcessorSpecDataHob;
  EFI_RISCV_OPENSBI_FIRMWARE_CONTEXT *FirmwareContext;
  EFI_RISCV_FIRMWARE_CONTEXT_HART_SPECIFIC *FirmwareContextHartSpecific;

  DEBUG ((DEBUG_INFO, "%a: Entry.\n", __FUNCTION__));

  if (GuidHobData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SbiGetFirmwareContext (&FirmwareContext);
  ASSERT (FirmwareContext != NULL);
  if (FirmwareContext == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to get the pointer of EFI_RISCV_OPENSBI_FIRMWARE_CONTEXT of hart %d\n", HartId));
    return EFI_NOT_FOUND;
  }
  DEBUG ((DEBUG_INFO, "    Firmware Context is at 0x%x.\n", FirmwareContext));

  FirmwareContextHartSpecific = FirmwareContext->HartSpecific[HartId];
  DEBUG ((DEBUG_INFO, "    Firmware Context Hart specific is at 0x%x.\n", FirmwareContextHartSpecific));
  if (FirmwareContextHartSpecific == NULL) {
    DEBUG ((DEBUG_INFO, "    This hart: %d is ignored by platform.\n", HartId));
    return EFI_UNSUPPORTED;
  }
  //
  // Build up RISC_V_PROCESSOR_SPECIFIC_HOB_DATA.
  //
  CommonFirmwareContextHartSpecificInfo (
      FirmwareContextHartSpecific,
      ParentProcessorGuid,
      ParentProcessorUid,
      (EFI_GUID *)PcdGetPtr (PcdSiFiveU54CoreGuid),
      HartId,
      IsBootHart,
      &ProcessorSpecDataHob
      );

  ProcessorSpecDataHob.ProcessorSpecificData.MModeExcepDelegation.Value64_L     = TO_BE_FILLED;
  ProcessorSpecDataHob.ProcessorSpecificData.MModeExcepDelegation.Value64_H     = TO_BE_FILLED;
  ProcessorSpecDataHob.ProcessorSpecificData.MModeInterruptDelegation.Value64_L = TO_BE_FILLED;
  ProcessorSpecDataHob.ProcessorSpecificData.MModeInterruptDelegation.Value64_H = TO_BE_FILLED;
  ProcessorSpecDataHob.ProcessorSpecificData.HartXlen                 = RegisterLen64;
  ProcessorSpecDataHob.ProcessorSpecificData.MachineModeXlen          = RegisterLen64;
  ProcessorSpecDataHob.ProcessorSpecificData.UserModeXlen             = RegisterLen64;

  if (IsManagementCore) {
    // Configuration for E51
    ProcessorSpecDataHob.ProcessorSpecificData.SupervisorModeXlen       = RegisterUnsupported;
  } else {
    // Configuration for U54
    ProcessorSpecDataHob.ProcessorSpecificData.SupervisorModeXlen       = RegisterLen64;
  }

  DebugPrintHartSpecificInfo (&ProcessorSpecDataHob);

  //
  // Build GUID HOB for core, this is for SMBIOS type 44
  //
  ProcessorSpecDataHobGuid = PcdGetPtr (PcdProcessorSpecificDataGuidHobGuid);
  CoreGuidHob = (RISC_V_PROCESSOR_SPECIFIC_HOB_DATA *)BuildGuidDataHob (ProcessorSpecDataHobGuid, (VOID *)&ProcessorSpecDataHob, sizeof (RISC_V_PROCESSOR_SPECIFIC_HOB_DATA));
  if (CoreGuidHob == NULL) {
    DEBUG ((DEBUG_ERROR, "Fail to create GUID HOB of SiFive U54 core.\n"));
    ASSERT (FALSE);
  }
  *GuidHobData = CoreGuidHob;
  return EFI_SUCCESS;
}

/**
  Function to build cache related SMBIOS information. RISC-V SMBIOS DXE driver collects
  this information and builds SMBIOS Type 7 record.

  The caller can adjust the allocated hob data to their needs.

  @param  ProcessorUid      Unique ID of physical processor which owns this core.
  @param  L1CacheDataHobPtr Pointer to allocated HOB data.

**/
VOID
EFIAPI
CreateU54SmbiosType7L1DataHob (
  IN UINTN     ProcessorUid,
  OUT RISC_V_PROCESSOR_TYPE7_HOB_DATA **L1CacheDataHobPtr
  )
{
  EFI_GUID *GuidPtr;
  RISC_V_PROCESSOR_TYPE7_HOB_DATA L1CacheDataHob;

  //
  // Build up SMBIOS type 7 L1 cache record.
  //
  ZeroMem((VOID *)&L1CacheDataHob, sizeof (RISC_V_PROCESSOR_TYPE7_HOB_DATA));
  L1CacheDataHob.PrcessorGuid = *((EFI_GUID *)PcdGetPtr (PcdSiFiveU5MCCoreplexGuid));
  L1CacheDataHob.ProcessorUid = ProcessorUid;
  L1CacheDataHob.SmbiosType7Cache.SocketDesignation = TO_BE_FILLED_BY_VENDOR;
  L1CacheDataHob.SmbiosType7Cache.CacheConfiguration = RISC_V_CACHE_CONFIGURATION_CACHE_LEVEL_1 | \
      RISC_V_CACHE_CONFIGURATION_LOCATION_INTERNAL | \
      RISC_V_CACHE_CONFIGURATION_ENABLED | \
      RISC_V_CACHE_CONFIGURATION_MODE_UNKNOWN;
  L1CacheDataHob.SmbiosType7Cache.MaximumCacheSize = TO_BE_FILLED_BY_VENDOR;
  L1CacheDataHob.SmbiosType7Cache.InstalledSize = TO_BE_FILLED_BY_VENDOR;
  L1CacheDataHob.SmbiosType7Cache.SupportedSRAMType.Unknown = 1;
  L1CacheDataHob.SmbiosType7Cache.CurrentSRAMType.Unknown = 1;
  L1CacheDataHob.SmbiosType7Cache.CacheSpeed = TO_BE_FILLED_BY_VENDOR;
  L1CacheDataHob.SmbiosType7Cache.ErrorCorrectionType = TO_BE_FILLED_BY_VENDOR;
  L1CacheDataHob.SmbiosType7Cache.SystemCacheType = CacheTypeUnified;
  L1CacheDataHob.SmbiosType7Cache.Associativity = TO_BE_FILLED_BY_VENDOR;

  GuidPtr = (EFI_GUID *)PcdGetPtr (PcdProcessorSmbiosType7GuidHobGuid);
  *L1CacheDataHobPtr = (RISC_V_PROCESSOR_TYPE7_HOB_DATA *)BuildGuidDataHob (GuidPtr, (VOID *)&L1CacheDataHob, sizeof (RISC_V_PROCESSOR_TYPE7_HOB_DATA));
  if (L1CacheDataHobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Fail to create GUID HOB of SiFive U5 MC Coreplex L1 cache RISC_V_PROCESSOR_TYPE7_HOB_DATA.\n"));
    ASSERT (FALSE);
  }
}

/**
  Function to build processor related SMBIOS information. RISC-V SMBIOS DXE driver collects
  this information and builds SMBIOS Type 4 record.

  The caller can adjust the allocated hob data to their needs.

  @param  ProcessorUid      Unique ID of physical processor which owns this core.
  @param  ProcessorDataHobPtr Pointer to allocated HOB data.

**/
VOID
EFIAPI
CreateU54SmbiosType4DataHob (
  IN UINTN     ProcessorUid,
  OUT RISC_V_PROCESSOR_TYPE4_HOB_DATA **ProcessorDataHobPtr
  )
{
  EFI_GUID *GuidPtr;
  RISC_V_PROCESSOR_TYPE4_HOB_DATA ProcessorDataHob;

  //
  // Build up SMBIOS type 4 record.
  //
  ZeroMem((VOID *)&ProcessorDataHob, sizeof (RISC_V_PROCESSOR_TYPE4_HOB_DATA));
  ProcessorDataHob.PrcessorGuid = *((EFI_GUID *)PcdGetPtr (PcdSiFiveU5MCCoreplexGuid));
  ProcessorDataHob.ProcessorUid = ProcessorUid;
  ProcessorDataHob.SmbiosType4Processor.Socket = TO_BE_FILLED_BY_VENDOR;
  ProcessorDataHob.SmbiosType4Processor.ProcessorType = CentralProcessor;
  ProcessorDataHob.SmbiosType4Processor.ProcessorFamily = ProcessorFamilyIndicatorFamily2;
  ProcessorDataHob.SmbiosType4Processor.ProcessorManufacturer = TO_BE_FILLED_BY_VENDOR;
  SetMem ((VOID *)&ProcessorDataHob.SmbiosType4Processor.ProcessorId, sizeof (PROCESSOR_ID_DATA), TO_BE_FILLED_BY_CODE);
  ProcessorDataHob.SmbiosType4Processor.ProcessorVersion = TO_BE_FILLED_BY_VENDOR;
  ProcessorDataHob.SmbiosType4Processor.Voltage.ProcessorVoltageCapability3_3V = 1;
  ProcessorDataHob.SmbiosType4Processor.ExternalClock = TO_BE_FILLED_BY_VENDOR;
  ProcessorDataHob.SmbiosType4Processor.MaxSpeed = TO_BE_FILLED_BY_VENDOR;
  ProcessorDataHob.SmbiosType4Processor.CurrentSpeed = TO_BE_FILLED_BY_VENDOR;
  ProcessorDataHob.SmbiosType4Processor.Status = TO_BE_FILLED_BY_CODE;
  ProcessorDataHob.SmbiosType4Processor.ProcessorUpgrade = TO_BE_FILLED_BY_VENDOR;
  ProcessorDataHob.SmbiosType4Processor.L1CacheHandle = TO_BE_FILLED_BY_RISC_V_SMBIOS_DXE_DRIVER;
  ProcessorDataHob.SmbiosType4Processor.L2CacheHandle = TO_BE_FILLED_BY_RISC_V_SMBIOS_DXE_DRIVER;
  ProcessorDataHob.SmbiosType4Processor.L3CacheHandle = 0xffff;
  ProcessorDataHob.SmbiosType4Processor.SerialNumber = TO_BE_FILLED_BY_CODE;
  ProcessorDataHob.SmbiosType4Processor.AssetTag = TO_BE_FILLED_BY_VENDOR;
  ProcessorDataHob.SmbiosType4Processor.PartNumber = TO_BE_FILLED_BY_VENDOR;
  ProcessorDataHob.SmbiosType4Processor.CoreCount = (UINT8)FixedPcdGet32 (PcdNumberofU5Cores) + (UINT8)PcdGetBool (PcdE5MCSupported);
  ProcessorDataHob.SmbiosType4Processor.EnabledCoreCount = (UINT8)FixedPcdGet32 (PcdNumberofU5Cores) + (UINT8)PcdGetBool (PcdE5MCSupported);
  ProcessorDataHob.SmbiosType4Processor.ThreadCount = (UINT8)FixedPcdGet32 (PcdNumberofU5Cores) + (UINT8)PcdGetBool (PcdE5MCSupported);
  ProcessorDataHob.SmbiosType4Processor.ProcessorCharacteristics = (UINT16)(1 << 2); // 64-bit capable
  ProcessorDataHob.SmbiosType4Processor.ProcessorFamily2 = ProcessorFamilyRiscVRV64;
  ProcessorDataHob.SmbiosType4Processor.CoreCount2 = 0;
  ProcessorDataHob.SmbiosType4Processor.EnabledCoreCount2 = 0;
  ProcessorDataHob.SmbiosType4Processor.ThreadCount2 = 0;

  GuidPtr = (EFI_GUID *)PcdGetPtr (PcdProcessorSmbiosType4GuidHobGuid);
  *ProcessorDataHobPtr = (RISC_V_PROCESSOR_TYPE4_HOB_DATA *)BuildGuidDataHob (GuidPtr, (VOID *)&ProcessorDataHob, sizeof (RISC_V_PROCESSOR_TYPE4_HOB_DATA));
  if (ProcessorDataHobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Fail to create GUID HOB of SiFive U5MC Coreplex RISC_V_PROCESSOR_TYPE4_HOB_DATA.\n"));
    ASSERT (FALSE);
  }
}
