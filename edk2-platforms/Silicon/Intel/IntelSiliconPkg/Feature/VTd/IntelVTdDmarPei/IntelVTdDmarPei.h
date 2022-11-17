/** @file
  The definition for DMA access Library.

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __DMA_ACCESS_LIB_H__
#define __DMA_ACCESS_LIB_H__

#define MAX_VTD_PCI_DATA_NUMBER             0x100

#define VTD_64BITS_ADDRESS(Lo, Hi) (LShiftU64 (Lo, 12) | LShiftU64 (Hi, 32))

typedef struct {
  UINT8                            DeviceType;
  VTD_SOURCE_ID                    PciSourceId;
} PEI_PCI_DEVICE_DATA;

typedef struct {
  BOOLEAN                          IncludeAllFlag;
  UINT32                           PciDeviceDataNumber;
  UINT32                           PciDeviceDataMaxNumber;
  UINT32                           PciDeviceDataPageSize;
  UINT32                           PciDeviceData;
} PEI_PCI_DEVICE_INFORMATION;

typedef struct {
  UINT32                           VtdUnitBaseAddress;
  UINT16                           Segment;
  VTD_VER_REG                      VerReg;
  VTD_CAP_REG                      CapReg;
  VTD_ECAP_REG                     ECapReg;
  BOOLEAN                          Is5LevelPaging;
  UINT32                           FixedSecondLevelPagingEntry;
  UINT32                           RmrrSecondLevelPagingEntry;
  UINT32                           RootEntryTable;
  UINT32                           ExtRootEntryTable;
  UINT16                           RootEntryTablePageSize;
  UINT16                           ExtRootEntryTablePageSize;
  PEI_PCI_DEVICE_INFORMATION       PciDeviceInfo;
  UINT8                            EnableQueuedInvalidation;
  UINT16                           QiDescLength;
  QI_DESC                          *QiDesc;
  UINT16                           QiFreeHead;
} VTD_UNIT_INFO;

typedef struct {
  UINT32                           AcpiDmarTable;
  UINT8                            HostAddressWidth;
  UINT32                           VTdEngineCount;
  VTD_UNIT_INFO                    VtdUnitInfo[1];
} VTD_INFO;

typedef struct {
  UINT64                            DmaBufferBase;
  UINT64                            DmaBufferSize;
  UINT64                            DmaBufferLimit;
  UINT64                            DmaBufferCurrentTop;
  UINT64                            DmaBufferCurrentBottom;
} DMA_BUFFER_INFO;

/**
  Enable VTd translation table protection.

  @param[in]  VTdInfo           The VTd engine context information.
  @param[in]  EngineMask        The mask of the VTd engine to be accessed.
**/
VOID
EnableVTdTranslationProtectionAll (
  IN VTD_INFO                   *VTdInfo,
  IN UINT64                     EngineMask
  );

/**
  Enable VTd translation table protection.

  @param[in]  VTdInfo           The VTd engine context information.

  @retval EFI_SUCCESS           DMAR translation is enabled.
  @retval EFI_DEVICE_ERROR      DMAR translation is not enabled.
**/
EFI_STATUS
EnableVTdTranslationProtection (
  IN VTD_INFO                   *VTdInfo
  );

/**
  Disable VTd translation table protection.

  @param[in]  VTdInfo           The VTd engine context information.
  @param[in]  EngineMask        The mask of the VTd engine to be accessed.
**/
VOID
DisableVTdTranslationProtection (
  IN VTD_INFO                   *VTdInfo,
  IN UINT64                     EngineMask
  );

/**
  Parse DMAR DRHD table.

  @param[in]  AcpiDmarTable     DMAR ACPI table

  @return EFI_SUCCESS           The DMAR DRHD table is parsed.
**/
EFI_STATUS
ParseDmarAcpiTableDrhd (
  IN EFI_ACPI_DMAR_HEADER       *AcpiDmarTable
  );

/**
  Parse DMAR DRHD table.

  @param[in]  VTdInfo           The VTd engine context information.
**/
VOID
ParseDmarAcpiTableRmrr (
  IN VTD_INFO                   *VTdInfo
  );

/**
  Dump DMAR ACPI table.

  @param[in]  Dmar              DMAR ACPI table
**/
VOID
DumpAcpiDMAR (
  IN EFI_ACPI_DMAR_HEADER       *Dmar
  );

/**
  Prepare VTD cache invalidation configuration.

  @param[in]  VTdInfo           The VTd engine context information.

  @retval EFI_SUCCESS           Prepare Vtd config success
**/
EFI_STATUS
PrepareVtdCacheInvalidationConfig (
  IN VTD_INFO                   *VTdInfo
  );

/**
  Prepare VTD configuration.

  @param[in]  VTdInfo           The VTd engine context information.

  @retval EFI_SUCCESS           Prepare Vtd config success
**/
EFI_STATUS
PrepareVtdConfig (
  IN VTD_INFO                   *VTdInfo
  );

/**
  Setup VTd translation table.

  @param[in]  VTdInfo           The VTd engine context information.

  @retval EFI_SUCCESS           Setup translation table successfully.
  @retval EFI_OUT_OF_RESOURCE   Setup translation table fail.
**/
EFI_STATUS
SetupTranslationTable (
  IN VTD_INFO                   *VTdInfo
  );

/**
  Flush VTD page table and context table memory.

  This action is to make sure the IOMMU engine can get final data in memory.

  @param[in]  VTdUnitInfo       The VTd engine unit information.
  @param[in]  Base              The base address of memory to be flushed.
  @param[in]  Size              The size of memory in bytes to be flushed.
**/
VOID
FlushPageTableMemory (
  IN VTD_UNIT_INFO              *VTdUnitInfo,
  IN UINTN                      Base,
  IN UINTN                      Size
  );

/**
  Allocate zero pages.

  @param[in]  Pages             the number of pages.

  @return the page address.
  @retval NULL No resource to allocate pages.
**/
VOID *
EFIAPI
AllocateZeroPages (
  IN UINTN                      Pages
  );

/**
  Return the index of PCI data.

  @param[in]  VTdUnitInfo       The VTd engine unit information.
  @param[in]  Segment           The Segment used to identify a VTd engine.
  @param[in]  SourceId          The SourceId used to identify a VTd engine and table entry.

  @return The index of the PCI data.
  @retval (UINTN)-1  The PCI data is not found.
**/
UINTN
GetPciDataIndex (
  IN VTD_UNIT_INFO              *VTdUnitInfo,
  IN UINT16                     Segment,
  IN VTD_SOURCE_ID              SourceId
  );

/**
  Always enable the VTd page attribute for the device.

  @param[in]  VTdInfo           The VTd engine context information.
  @param[in]  Segment           The Segment used to identify a VTd engine.
  @param[in]  SourceId          The SourceId used to identify a VTd engine and table entry.
  @param[in]  MemoryBase        The base of the memory.
  @param[in]  MemoryLimit       The limit of the memory.
  @param[in]  IoMmuAccess       The IOMMU access.

  @retval EFI_SUCCESS           The VTd entry is updated to always enable all DMA access for the specific device.
**/
EFI_STATUS
EnableRmrrPageAttribute (
  IN VTD_INFO                   *VTdInfo,
  IN UINT16                     Segment,
  IN VTD_SOURCE_ID              SourceId,
  IN UINT64                     MemoryBase,
  IN UINT64                     MemoryLimit,
  IN UINT64                     IoMmuAccess
  );

extern EFI_GUID mVTdInfoGuid;
extern EFI_GUID mDmaBufferInfoGuid;

#endif

