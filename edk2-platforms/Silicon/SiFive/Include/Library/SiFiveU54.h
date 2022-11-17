/** @file
  SiFive U54 Core library definitions.

  Copyright (c) 2019, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef SIFIVE_U54_CORE_H_
#define SIFIVE_U54_CORE_H_

#include <PiPei.h>

#include <ProcessorSpecificHobData.h>

/**
  Function to build core specific information HOB for U54 or E51 core.

  @param  ParentProcessorGuid    Parent processor od this core. ParentProcessorGuid
                                 could be the same as CoreGuid if one processor has
                                 only one core.
  @param  ParentProcessorUid     Unique ID of pysical processor which owns this core.
  @param  HartId                 Hart ID of this core.
  @param  IsBootHart             TRUE means this is the boot HART.
  @param  IsManagementCore       TRUE means this is for the E51 management core, not U54
  @param  GuidHobData            Pointer to RISC_V_PROCESSOR_SPECIFIC_HOB_DATA.

  @return EFI_SUCCESS     The PEIM initialized successfully.

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
);

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
);

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
);

#endif
