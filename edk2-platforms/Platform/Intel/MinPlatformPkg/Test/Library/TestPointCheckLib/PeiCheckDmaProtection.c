/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiPei.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/PeiServicesLib.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/DmaRemappingReportingTable.h>
#include <IndustryStandard/Vtd.h>
#include <Ppi/VtdInfo.h>

EFI_STATUS
CheckDrhd (
  IN EFI_ACPI_DMAR_HEADER  *Dmar
  )
{
  EFI_ACPI_DMAR_STRUCTURE_HEADER        *DmarStructHeader;
  INTN                                  DmarLen;
  EFI_ACPI_DMAR_DRHD_HEADER             *Drhd;
  UINT32                                Reg32;
  VTD_CAP_REG                           CapReg;
    
  //
  // Sub table
  //
  DmarLen  = Dmar->Header.Length - sizeof(EFI_ACPI_DMAR_HEADER);
  DmarStructHeader = (EFI_ACPI_DMAR_STRUCTURE_HEADER *)(Dmar + 1);
  while (DmarLen > 0) {
    switch (DmarStructHeader->Type) {
    case EFI_ACPI_DMAR_TYPE_DRHD:
      Drhd = (EFI_ACPI_DMAR_DRHD_HEADER *)DmarStructHeader;

      Reg32 = MmioRead32 ((UINTN)Drhd->RegisterBaseAddress + R_GSTS_REG);

      CapReg.Uint64 = MmioRead64 ((UINTN)Drhd->RegisterBaseAddress + R_CAP_REG);
      if (CapReg.Bits.PLMR == 0 || CapReg.Bits.PHMR == 0) {
        return EFI_INVALID_PARAMETER;
      }

      Reg32 = MmioRead32 ((UINTN)Drhd->RegisterBaseAddress + R_PMEN_ENABLE_REG);
      if ((Reg32 & BIT0) == 0) {
        return EFI_INVALID_PARAMETER;
      }

      break;
    case EFI_ACPI_DMAR_TYPE_RMRR:
    case EFI_ACPI_DMAR_TYPE_ATSR:
    case EFI_ACPI_DMAR_TYPE_RHSA:
    case EFI_ACPI_DMAR_TYPE_ANDD:
    default:
      break;
    }
    DmarStructHeader = (EFI_ACPI_DMAR_STRUCTURE_HEADER *)((UINT8 *)DmarStructHeader + DmarStructHeader->Length);
    DmarLen         -= DmarStructHeader->Length;
  }

  return EFI_SUCCESS;
}

VOID *
TestPointGetAcpi (
  IN UINT32  Signature
  );

EFI_STATUS
TestPointVtdEngine (
  VOID
  )
{
  EFI_ACPI_DMAR_HEADER  *Dmar;
  EFI_STATUS            Status;

  Status = PeiServicesLocatePpi (
             &gEdkiiVTdInfoPpiGuid,
             0,
             NULL,
             (VOID **) &Dmar
             );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "No DMAR table\n"));
  } else {
    Status = CheckDrhd (Dmar);
  }

  if (EFI_ERROR(Status)) {
    TestPointLibAppendErrorString (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      TEST_POINT_BYTE1_MEMORY_DISCOVERED_DMA_PROTECTION_ENABLED_ERROR_CODE \
        TEST_POINT_MEMORY_DISCOVERED \
        TEST_POINT_BYTE1_MEMORY_DISCOVERED_DMA_PROTECTION_ENABLED_ERROR_STRING
      );
  }

  return Status;
}