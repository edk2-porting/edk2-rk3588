/** @file

  Sample ACPI Platform Driver

  Copyright (c) 2008 - 2011, Intel Corporation. All rights reserved.<BR>
  Copyright (c) 2014 - 2016, AMD Inc. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
/**
  Derived from:
   MdeModulePkg/Universal/Acpi/AcpiPlatformDxe/AcpiPlatform.c
**/

#include <Guid/ArmMpCoreInfo.h>

#include <Protocol/AcpiTable.h>
#include <Protocol/AmdMpCoreInfo.h>

#include <Library/ArmLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <IndustryStandard/Acpi61.h>
#include <IndustryStandard/IoRemappingTable.h>

#include <SocVersion.h>

#include "AcpiPlatform.h"

STATIC EFI_ACPI_TABLE_PROTOCOL   *mAcpiTableProtocol;
STATIC AMD_MP_CORE_INFO_PROTOCOL *mAmdMpCoreInfoProtocol;


STATIC CONST UINT8 mDefaultMacPackageA[] = {
  0x12, 0xe, 0x6, 0xa, 0x2, 0xa, 0xa1, 0xa, 0xa2, 0xa, 0xa3, 0xa, 0xa4, 0xa, 0xa5
};

STATIC CONST UINT8 mDefaultMacPackageB[] = {
  0x12, 0xe, 0x6, 0xa, 0x2, 0xa, 0xb1, 0xa, 0xb2, 0xa, 0xb3, 0xa, 0xb4, 0xa, 0xb5
};

#define PACKAGE_MAC_OFFSET  4
#define PACKAGE_MAC_INCR    2

STATIC
VOID
SetPackageAddress (
  UINT8         *Package,
  UINT8         *MacAddress,
  UINTN         Size
  )
{
  UINTN   Index;

  for (Index = PACKAGE_MAC_OFFSET; Index < Size; Index += PACKAGE_MAC_INCR) {
    Package[Index] = *MacAddress++;
  }
}

STATIC
VOID
PatchAmlPackage (
  CONST UINT8   *Pattern,
  CONST UINT8   *Replacement,
  UINTN         PatternLength,
  UINT8         *SsdtTable,
  UINTN         TableSize
  )
{
  UINTN         Offset;

  for (Offset = 0; Offset < (TableSize - PatternLength); Offset++) {
    if (CompareMem (SsdtTable + Offset, Pattern, PatternLength) == 0) {
      CopyMem (SsdtTable + Offset, Replacement, PatternLength);
      break;
    }
  }
}

STATIC
VOID
EnableAvailableCores (
  EFI_ACPI_5_1_GIC_STRUCTURE            *GicC
  )
{
  ARM_CORE_INFO                         *ArmCoreInfoTable;
  UINTN                                 CoreCount;
  UINTN                                 Index;

  CoreCount = 0;
  ArmCoreInfoTable = mAmdMpCoreInfoProtocol->GetArmCoreInfoTable (&CoreCount);
  ASSERT (ArmCoreInfoTable != NULL);

  while (CoreCount--) {
    for (Index = 0; Index < MAX_CORES; Index++) {
      if (GicC[Index].MPIDR == GET_MPID (ArmCoreInfoTable->ClusterId,
                                         ArmCoreInfoTable->CoreId)) {
        GicC[Index].Flags |= EFI_ACPI_5_1_GIC_ENABLED;
        break;
      }
    }
    ArmCoreInfoTable++;
  }
}

STATIC
VOID
InstallSystemDescriptionTables (
  VOID
  )
{
  EFI_ACPI_DESCRIPTION_HEADER                         *Table;
  EFI_STATUS                                          Status;
  UINT32                                              CpuId;
  UINTN                                               Index;
  UINTN                                               TableSize;
  UINTN                                               TableHandle;
  EFI_ACPI_5_1_GENERIC_TIMER_DESCRIPTION_TABLE        *Gtdt;
  EFI_ACPI_5_1_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER *Madt;
  EFI_ACPI_5_1_GIC_STRUCTURE                          *GicC;
  UINT8                         MacPackage[sizeof(mDefaultMacPackageA)];

  CpuId = PcdGet32 (PcdSocCpuId);

  Status = EFI_SUCCESS;
  for (Index = 0; !EFI_ERROR (Status); Index++) {
    Status = GetSectionFromFv (&gEfiCallerIdGuid, EFI_SECTION_RAW, Index,
               (VOID **) &Table, &TableSize);
    if (EFI_ERROR (Status)) {
      break;
    }

    switch (Table->OemTableId) {
    case SIGNATURE_64 ('S', 't', 'y', 'x', 'B', '1', ' ', ' '):
      if ((CpuId & STYX_SOC_VERSION_MASK) < STYX_SOC_VERSION_B1) {
        continue;
      }
      break;

    case SIGNATURE_64 ('S', 't', 'y', 'x', 'X', 'g', 'b', 'e'):
      if (!FixedPcdGetBool (PcdXgbeEnable)) {
        continue;
      }

      //
      // Patch the SSDT binary with the correct MAC addresses
      //
      CopyMem (MacPackage, mDefaultMacPackageA, sizeof (MacPackage));

      SetPackageAddress (MacPackage, PcdGetPtr (PcdEthMacA), sizeof (MacPackage));
      PatchAmlPackage (mDefaultMacPackageA, MacPackage, sizeof (MacPackage),
        (UINT8 *)Table, TableSize);

      SetPackageAddress (MacPackage, PcdGetPtr (PcdEthMacB), sizeof (MacPackage));
      PatchAmlPackage (mDefaultMacPackageB, MacPackage, sizeof (MacPackage),
        (UINT8 *)Table, TableSize);

      break;

    case SIGNATURE_64 ('S', 't', 'y', 'x', 'K', 'c', 's', ' '):
      if (!FixedPcdGetBool (PcdEnableKcs) ||
          (CpuId & STYX_SOC_VERSION_MASK) < STYX_SOC_VERSION_B1) {
        continue;
      }

    default:
      switch (Table->Signature) {
      case EFI_ACPI_6_0_IO_REMAPPING_TABLE_SIGNATURE:
        if (!PcdGetBool (PcdEnableSmmus)) {
          continue;
        }
        break;

      case EFI_ACPI_5_1_GENERIC_TIMER_DESCRIPTION_TABLE_SIGNATURE:
        if ((CpuId & STYX_SOC_VERSION_MASK) < STYX_SOC_VERSION_B1) {
          Gtdt = (EFI_ACPI_5_1_GENERIC_TIMER_DESCRIPTION_TABLE *)Table;
          Gtdt->Header.Length = sizeof (*Gtdt);
          Gtdt->PlatformTimerCount = 0;
          Gtdt->PlatformTimerOffset = 0;
        }
        break;

      case EFI_ACPI_5_1_MULTIPLE_APIC_DESCRIPTION_TABLE_SIGNATURE:
        Madt = (EFI_ACPI_5_1_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER *)Table;
        GicC = (EFI_ACPI_5_1_GIC_STRUCTURE *)(Madt + 1);

        EnableAvailableCores (GicC);
        break;
      }
    }

    Status = mAcpiTableProtocol->InstallAcpiTable (mAcpiTableProtocol, Table,
                                   Table->Length, &TableHandle);

    DEBUG ((DEBUG_WARN,
      "Installing %c%c%c%c Table (Revision %d, Length %d) ... %r\n",
      ((UINT8 *)&Table->Signature)[0], ((UINT8 *)&Table->Signature)[1],
      ((UINT8 *)&Table->Signature)[2], ((UINT8 *)&Table->Signature)[3],
      Table->Revision, Table->Length, Status));

    FreePool (Table);
  }
}

/**
  Entrypoint of Acpi Platform driver.

  @param  ImageHandle
  @param  SystemTable

  @return EFI_SUCCESS
  @return EFI_LOAD_ERROR
  @return EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
EFIAPI
AcpiPlatformEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                  Status;

  //
  // Find the AcpiTable protocol
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL,
                  (VOID**)&mAcpiTableProtocol);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gAmdMpCoreInfoProtocolGuid, NULL,
                  (VOID**)&mAmdMpCoreInfoProtocol);
  ASSERT_EFI_ERROR (Status);

  InstallSystemDescriptionTables ();

  return EFI_REQUEST_UNLOAD_IMAGE;
}
