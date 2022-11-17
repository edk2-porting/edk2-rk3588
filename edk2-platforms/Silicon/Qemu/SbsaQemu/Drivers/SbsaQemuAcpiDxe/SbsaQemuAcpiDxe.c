/** @file
*  This file is an ACPI driver for the Qemu SBSA platform.
*
*  Copyright (c) 2020, Linaro Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/AcpiAml.h>
#include <IndustryStandard/SbsaQemuAcpi.h>
#include <Library/AcpiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/FdtHelperLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include <Protocol/AcpiTable.h>

/*
 * A Function to Compute the ACPI Table Checksum
 */
VOID
AcpiPlatformChecksum (
  IN UINT8      *Buffer,
  IN UINTN      Size
  )
{
  UINTN ChecksumOffset;

  ChecksumOffset = OFFSET_OF (EFI_ACPI_DESCRIPTION_HEADER, Checksum);

  // Set checksum field to 0 since it is used as part of the calculation
  Buffer[ChecksumOffset] = 0;

  Buffer[ChecksumOffset] = CalculateCheckSum8(Buffer, Size);
}

/*
 * A function that add the MADT ACPI table.
  IN EFI_ACPI_COMMON_HEADER    *CurrentTable
 */
EFI_STATUS
AddMadtTable (
  IN EFI_ACPI_TABLE_PROTOCOL   *AcpiTable
  )
{
  EFI_STATUS            Status;
  UINTN                 TableHandle;
  UINT32                TableSize;
  EFI_PHYSICAL_ADDRESS  PageAddress;
  UINT8                 *New;
  UINT32                NumCores;
  UINT32                CoreIndex;

  // Initialize MADT ACPI Header
  EFI_ACPI_6_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER Header = {
     SBSAQEMU_ACPI_HEADER (EFI_ACPI_6_0_MULTIPLE_APIC_DESCRIPTION_TABLE_SIGNATURE,
                           EFI_ACPI_6_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER,
                           EFI_ACPI_6_0_MULTIPLE_APIC_DESCRIPTION_TABLE_REVISION),
      0, 0 };

  // Initialize GICC Structure
  EFI_ACPI_6_0_GIC_STRUCTURE Gicc = EFI_ACPI_6_0_GICC_STRUCTURE_INIT (
    0,                                     /* GicID */
    0,                                     /* AcpiCpuUid */
    0,                                     /* Mpidr */
    EFI_ACPI_6_0_GIC_ENABLED,              /* Flags */
    SBSAQEMU_MADT_GIC_PMU_IRQ,             /* PMU Irq */
    FixedPcdGet32 (PcdGicDistributorBase), /* PhysicalBaseAddress */
    SBSAQEMU_MADT_GIC_VBASE,               /* GicVBase */
    SBSAQEMU_MADT_GIC_HBASE,               /* GicHBase */
    25,                                    /* GsivId */
    0,                                     /* GicRBase */
    0                                      /* Efficiency */
    );

  // Initialize GIC Distributor Structure
  EFI_ACPI_6_0_GIC_DISTRIBUTOR_STRUCTURE Gicd =
    EFI_ACPI_6_0_GIC_DISTRIBUTOR_INIT (
      0,
      FixedPcdGet32 (PcdGicDistributorBase),
      0,
      3 /* GicVersion */
    );

 // Initialize GIC Redistributor Structure
  EFI_ACPI_6_0_GICR_STRUCTURE Gicr = SBSAQEMU_MADT_GICR_INIT();

  // Get CoreCount which was determined eariler after parsing device tree
  NumCores = PcdGet32 (PcdCoreCount);

  // Calculate the new table size based on the number of cores
  TableSize = sizeof (EFI_ACPI_6_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER) +
               (sizeof (EFI_ACPI_6_0_GIC_STRUCTURE) * NumCores) +
               sizeof (EFI_ACPI_6_0_GIC_DISTRIBUTOR_STRUCTURE) +
               sizeof (EFI_ACPI_6_0_GICR_STRUCTURE);

  Status = gBS->AllocatePages (
                  AllocateAnyPages,
                  EfiACPIReclaimMemory,
                  EFI_SIZE_TO_PAGES (TableSize),
                  &PageAddress
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate pages for MADT table\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  New = (UINT8 *)(UINTN) PageAddress;
  ZeroMem (New, TableSize);

  // Add the  ACPI Description table header
  CopyMem (New, &Header, sizeof (EFI_ACPI_6_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER));
  ((EFI_ACPI_DESCRIPTION_HEADER*) New)->Length = TableSize;
  New += sizeof (EFI_ACPI_6_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER);

  // Add new GICC structures for the Cores
  for (CoreIndex = 0; CoreIndex < PcdGet32 (PcdCoreCount); CoreIndex++) {
    EFI_ACPI_6_0_GIC_STRUCTURE *GiccPtr;

    CopyMem (New, &Gicc, sizeof (EFI_ACPI_6_0_GIC_STRUCTURE));
    GiccPtr = (EFI_ACPI_6_0_GIC_STRUCTURE *) New;
    GiccPtr->AcpiProcessorUid = CoreIndex;
    GiccPtr->MPIDR = FdtHelperGetMpidr (CoreIndex);
    New += sizeof (EFI_ACPI_6_0_GIC_STRUCTURE);
  }

  // GIC Distributor Structure
  CopyMem (New, &Gicd, sizeof (EFI_ACPI_6_0_GIC_DISTRIBUTOR_STRUCTURE));
  New += sizeof (EFI_ACPI_6_0_GIC_DISTRIBUTOR_STRUCTURE);

  // GIC ReDistributor Structure
  CopyMem (New, &Gicr, sizeof (EFI_ACPI_6_0_GICR_STRUCTURE));
  New += sizeof (EFI_ACPI_6_0_GICR_STRUCTURE);

  AcpiPlatformChecksum ((UINT8*) PageAddress, TableSize);

  Status = AcpiTable->InstallAcpiTable (
                        AcpiTable,
                        (EFI_ACPI_COMMON_HEADER *)PageAddress,
                        TableSize,
                        &TableHandle
                        );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to install MADT table\n"));
  }

  return Status;
}

/*
 * Function to calculate the PkgLength field in ACPI tables
 */
STATIC
UINT32
SetPkgLength (
  IN UINT8  *TablePtr,
  IN UINT32 Length
)
{
  UINT8  ByteCount;
  UINT8  *PkgLeadByte = TablePtr;

  if (Length < 64) {
    *TablePtr = Length;
    return 1;
  }

  // Set the LSB of Length in PkgLeadByte and advance Length
  *PkgLeadByte = Length & 0xF;
  Length = Length >> 4;

  while (Length) {
    TablePtr++;
    *TablePtr = (Length & 0xFF);
    Length = (Length >> 8);
  }

  // Calculate the number of bytes the Length field uses
  // and set the ByteCount field in PkgLeadByte.
  ByteCount = (TablePtr - PkgLeadByte) & 0xF;
  *PkgLeadByte |= (ByteCount << 6);

  return ByteCount + 1;
}

/*
 * A function that adds SSDT ACPI table.
 */
EFI_STATUS
AddSsdtTable (
  IN EFI_ACPI_TABLE_PROTOCOL   *AcpiTable
  )
{
  EFI_STATUS            Status;
  UINTN                 TableHandle;
  UINT32                TableSize;
  EFI_PHYSICAL_ADDRESS  PageAddress;
  UINT8                 *New;
  UINT8                 *HeaderAddr;
  UINT32                CpuId;
  UINT32                Offset;
  UINT8                 ScopeOpName[] =  SBSAQEMU_ACPI_SCOPE_NAME;
  UINT32                NumCores = PcdGet32 (PcdCoreCount);

  EFI_ACPI_DESCRIPTION_HEADER Header =
    SBSAQEMU_ACPI_HEADER (
      EFI_ACPI_6_0_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE,
      EFI_ACPI_DESCRIPTION_HEADER,
      EFI_ACPI_6_0_SECONDARY_SYSTEM_DESCRIPTION_TABLE_REVISION);

  SBSAQEMU_ACPI_CPU_DEVICE CpuDevice = {
    { AML_EXT_OP, AML_EXT_DEVICE_OP }, /* Device () */
    SBSAQEMU_ACPI_CPU_DEV_LEN,         /* Length */
    SBSAQEMU_ACPI_CPU_DEV_NAME,        /* Device Name "C000" */
    SBSAQEMU_ACPI_CPU_HID,             /* Name (HID, "ACPI0007") */
    SBSAQEMU_ACPI_CPU_UID,             /* Name (UID, 0) */
  };

  // Calculate the new table size based on the number of cores
  TableSize = sizeof (EFI_ACPI_DESCRIPTION_HEADER) +
              SBSAQEMU_ACPI_SCOPE_OP_MAX_LENGTH + sizeof (ScopeOpName) +
              (sizeof (CpuDevice) * NumCores);

  Status = gBS->AllocatePages (
                  AllocateAnyPages,
                  EfiACPIReclaimMemory,
                  EFI_SIZE_TO_PAGES (TableSize),
                  &PageAddress
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate pages for SSDT table\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  HeaderAddr = New = (UINT8 *)(UINTN) PageAddress;
  ZeroMem (New, TableSize);

  // Add the ACPI Description table header
  CopyMem (New, &Header, sizeof (EFI_ACPI_DESCRIPTION_HEADER));

  New += sizeof (EFI_ACPI_DESCRIPTION_HEADER);

  // Insert the top level ScopeOp
  *New = AML_SCOPE_OP;
  New++;
  Offset = SetPkgLength (New,
             (TableSize - sizeof (EFI_ACPI_DESCRIPTION_HEADER) - 1));

  // Adjust TableSize now we know header length of _SB
  TableSize -= (SBSAQEMU_ACPI_SCOPE_OP_MAX_LENGTH - Offset);
  ((EFI_ACPI_DESCRIPTION_HEADER*) HeaderAddr)->Length = TableSize;

  New += Offset;
  CopyMem (New, &ScopeOpName, sizeof (ScopeOpName));
  New += sizeof (ScopeOpName);

  // Add new Device structures for the Cores
  for (CpuId = 0; CpuId < NumCores; CpuId++) {
    SBSAQEMU_ACPI_CPU_DEVICE *CpuDevicePtr;

    CopyMem (New, &CpuDevice, sizeof (SBSAQEMU_ACPI_CPU_DEVICE));
    CpuDevicePtr = (SBSAQEMU_ACPI_CPU_DEVICE *) New;

    AsciiSPrint((CHAR8 *)&CpuDevicePtr->dev_name[1], 4, "%03X", CpuId);

    /* replace character lost by above NULL termination */
    CpuDevicePtr->hid[0] = AML_NAME_OP;

    CpuDevicePtr->uid[6] = CpuId & 0xFF;
    CpuDevicePtr->uid[7] = (CpuId >> 8) & 0xFF;
    New += sizeof (SBSAQEMU_ACPI_CPU_DEVICE);
  }

  // Perform Checksum
  AcpiPlatformChecksum ((UINT8*) PageAddress, TableSize);

  Status = AcpiTable->InstallAcpiTable (
                        AcpiTable,
                        (EFI_ACPI_COMMON_HEADER *)PageAddress,
                        TableSize,
                        &TableHandle
                        );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to install SSDT table\n"));
  }

  return Status;
}

/*
 * A function that adds the SSDT ACPI table.
 */
EFI_STATUS
AddPpttTable (
  IN EFI_ACPI_TABLE_PROTOCOL   *AcpiTable
  )
{
  EFI_STATUS            Status;
  UINTN                 TableHandle;
  UINT32                TableSize;
  EFI_PHYSICAL_ADDRESS  PageAddress;
  UINT8                 *New;
  UINT32                CpuId;
  UINT32                NumCores = PcdGet32 (PcdCoreCount);

  EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE L1DCache = SBSAQEMU_ACPI_PPTT_L1_D_CACHE_STRUCT;
  EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE L1ICache = SBSAQEMU_ACPI_PPTT_L1_I_CACHE_STRUCT;
  EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE L2Cache = SBSAQEMU_ACPI_PPTT_L2_CACHE_STRUCT;

  EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR Cluster = SBSAQEMU_ACPI_PPTT_CLUSTER_STRUCT;
  EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR Core = SBSAQEMU_ACPI_PPTT_CORE_STRUCT;

  EFI_ACPI_DESCRIPTION_HEADER Header =
    SBSAQEMU_ACPI_HEADER (
      EFI_ACPI_6_3_PROCESSOR_PROPERTIES_TOPOLOGY_TABLE_STRUCTURE_SIGNATURE,
      EFI_ACPI_DESCRIPTION_HEADER,
      EFI_ACPI_6_3_PROCESSOR_PROPERTIES_TOPOLOGY_TABLE_REVISION);

  TableSize = sizeof (EFI_ACPI_DESCRIPTION_HEADER) +
    sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR) +
    (sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE) * 3) +
    (sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR) * NumCores) +
    (sizeof (UINT32) * 2 * NumCores);

  Status = gBS->AllocatePages (
                  AllocateAnyPages,
                  EfiACPIReclaimMemory,
                  EFI_SIZE_TO_PAGES (TableSize),
                  &PageAddress
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate pages for PPTT table\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  New = (UINT8 *)(UINTN) PageAddress;
  ZeroMem (New, TableSize);

  // Add the ACPI Description table header
  CopyMem (New, &Header, sizeof (EFI_ACPI_DESCRIPTION_HEADER));
  ((EFI_ACPI_DESCRIPTION_HEADER*) New)->Length = TableSize;
  New += sizeof (EFI_ACPI_DESCRIPTION_HEADER);

  // Add the Cluster PPTT structure
  CopyMem (New, &Cluster, sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR));
  New += sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR);

  // Add L1 D Cache structure
  CopyMem (New, &L1DCache, sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE));
  ((EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE*) New)->NextLevelOfCache = L2_CACHE_INDEX;
  New += sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE);

  // Add L1 I Cache structure
  CopyMem (New, &L1ICache, sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE));
  ((EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE*) New)->NextLevelOfCache = L2_CACHE_INDEX;
  New += sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE);

  // Add L2 Cache structure
  CopyMem (New, &L2Cache, sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE));
  ((EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE*) New)->NextLevelOfCache = 0; /* L2 is LLC */
  New += sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE);

  for (CpuId = 0; CpuId < NumCores; CpuId++) {
    EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR *CorePtr;
    UINT32                                *PrivateResourcePtr;

    CopyMem (New, &Core, sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR));
    CorePtr = (EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR *) New;
    CorePtr->Parent = CLUSTER_INDEX;
    CorePtr->AcpiProcessorId = CpuId;
    New += sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR);

    PrivateResourcePtr = (UINT32 *) New;
    PrivateResourcePtr[0] = L1_D_CACHE_INDEX;
    PrivateResourcePtr[1] = L1_I_CACHE_INDEX;
    New += (2 * sizeof (UINT32));
  }

  // Perform Checksum
  AcpiPlatformChecksum ((UINT8*) PageAddress, TableSize);

  Status = AcpiTable->InstallAcpiTable (
                        AcpiTable,
                        (EFI_ACPI_COMMON_HEADER *)PageAddress,
                        TableSize,
                        &TableHandle
                        );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to install PPTT table\n"));
  }

  return Status;
}

EFI_STATUS
EFIAPI
InitializeSbsaQemuAcpiDxe (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                     Status;
  EFI_ACPI_TABLE_PROTOCOL        *AcpiTable;
  UINT32                         NumCores;

  // Parse the device tree and get the number of CPUs
  NumCores = FdtHelperCountCpus ();
  Status = PcdSet32S (PcdCoreCount, NumCores);
  ASSERT_RETURN_ERROR (Status);

  // Check if ACPI Table Protocol has been installed
  Status = gBS->LocateProtocol (
                  &gEfiAcpiTableProtocolGuid,
                  NULL,
                  (VOID **)&AcpiTable
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to locate ACPI Table Protocol\n"));
    return Status;
  }

  Status = AddMadtTable (AcpiTable);
  if (EFI_ERROR(Status)) {
     DEBUG ((DEBUG_ERROR, "Failed to add MADT table\n"));
  }

  Status = AddSsdtTable (AcpiTable);
  if (EFI_ERROR(Status)) {
     DEBUG ((DEBUG_ERROR, "Failed to add SSDT table\n"));
  }

  Status = AddPpttTable (AcpiTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to add PPTT table\n"));
  }

  return EFI_SUCCESS;
}
