/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include "ErrorSource/Ghes.h"
#include "Hest.h"
#include <IndustryStandard/Acpi62.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <PlatformArch.h>

EFI_STATUS HestAddErrorSourceDescriptor (
  IN OUT HEST_CONTEXT  *Context,
  IN VOID              *ErrorSourceDescriptor,
  IN UINT32            SizeOfDescriptor
)
{
  UINT16                                          *pSourceId;
  VOID                                            *Descriptor;
  EFI_ACPI_6_1_HARDWARE_ERROR_SOURCE_TABLE_HEADER *HestHeader;

  if ((Context == NULL) || (ErrorSourceDescriptor == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  HestHeader = Context->HestHeader;
  if (HestHeader->Header.Length + SizeOfDescriptor > Context->OccupiedMemorySize) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL]: Hest Size Too small\n", __FUNCTION__, __LINE__));
    return EFI_BUFFER_TOO_SMALL;
  }
  Descriptor = (UINT8*)HestHeader + HestHeader->Header.Length;
  gBS->CopyMem ((VOID*)Descriptor , ErrorSourceDescriptor, SizeOfDescriptor);
  pSourceId = Descriptor + sizeof (UINT16);
  *pSourceId = HestHeader->ErrorSourceCount;
  HestHeader->Header.Length += SizeOfDescriptor;
  HestHeader->ErrorSourceCount++;
  Context->KeyErrorSource = Descriptor;
  return EFI_SUCCESS;
}

VOID
HestSetAcpiTable (
  IN HEST_CONTEXT  *Context
)
{
  UINTN                     AcpiTableHandle;
  EFI_STATUS                Status;
  UINT8                     Checksum;
  EFI_ACPI_6_1_HARDWARE_ERROR_SOURCE_TABLE_HEADER *HestHeader;
  EFI_ACPI_SDT_HEADER       *Table;
  EFI_ACPI_TABLE_VERSION    TableVersion;
  UINTN                     TableKey;
  UINT32                    Index;
  if (Context == NULL) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL]: ERROR\n", __FUNCTION__, __LINE__));
    return;
  }

  HestHeader = Context->HestHeader;
  Checksum = CalculateCheckSum8 ((UINT8*)(HestHeader),  HestHeader->Header.Length);
  HestHeader->Header.Checksum = Checksum;
  AcpiTableHandle = 0;
  // see AcpiTableProtocol.c InstallAcpiTable
  Status = mAcpiTableProtocol->InstallAcpiTable (
                                  mAcpiTableProtocol,
                                  HestHeader,
                                  HestHeader->Header.Length,
                                  &AcpiTableHandle);
  for (Index = 0; Index < EFI_ACPI_MAX_NUM_TABLES; Index++) {
    Status = mAcpiSdtProtocol->GetAcpiTable (Index, &Table, &TableVersion, &TableKey);
    if (EFI_ERROR (Status)) {
      break;
    }
    if (Table->Signature != EFI_ACPI_6_1_HARDWARE_ERROR_SOURCE_TABLE_SIGNATURE) {
      continue;
    }
    mApeiTrustedfirmwareData->HestTableAddress = (EFI_PHYSICAL_ADDRESS)Table;
    DEBUG ((DEBUG_INFO, "Acpi HestSetAcpiTable Table = 0x%x.\n", (EFI_PHYSICAL_ADDRESS)Table));
  }
  DEBUG ((DEBUG_INFO, "[%a]:[%dL]:OUT %llx, IN %llx \n", __FUNCTION__, __LINE__,
          AcpiTableHandle, Context->HestHeader));
  return;
}

EFI_STATUS
HestHeaderCreator (
  HEST_CONTEXT  *Context,
  UINT32        PreAllocatedHestSize
)
{
  if (PreAllocatedHestSize < sizeof (EFI_ACPI_6_1_HARDWARE_ERROR_SOURCE_TABLE_HEADER)) {
    return EFI_BUFFER_TOO_SMALL;
  }
  Context->HestHeader = AllocateReservedZeroPool (PreAllocatedHestSize);
  *Context->HestHeader = (EFI_ACPI_6_1_HARDWARE_ERROR_SOURCE_TABLE_HEADER) {
   ARM_ACPI_HEADER (
      EFI_ACPI_6_1_HARDWARE_ERROR_SOURCE_TABLE_SIGNATURE,
      EFI_ACPI_6_1_HARDWARE_ERROR_SOURCE_TABLE_HEADER,
      EFI_ACPI_6_1_HARDWARE_ERROR_SOURCE_TABLE_REVISION
    ),
    0x0
  };
  Context->KeyErrorSource = Context->HestHeader + 1;
  Context->OccupiedMemorySize = PreAllocatedHestSize;
  return EFI_SUCCESS;
}
