/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include "Bert.h"
#include "ErrorSource/Ghes.h"
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <PlatformArch.h>

VOID
BertSetAcpiTable (
  IN BERT_CONTEXT *Context
)
{
  UINTN          AcpiTableHandle;
  EFI_STATUS     Status;
  if (Context == NULL) {
    return;
  }
  EFI_ACPI_6_0_BOOT_ERROR_RECORD_TABLE_HEADER* Bert = Context->BertHeader;
  Bert->Header.Checksum = CalculateCheckSum8 ((UINT8*)(Bert), Bert->Header.Length);
  AcpiTableHandle = 0;
  Status = mAcpiTableProtocol->InstallAcpiTable (
                                 mAcpiTableProtocol,
                                 Bert,
                                 Bert->Header.Length,
                                 &AcpiTableHandle);
  ASSERT_EFI_ERROR (Status);
  return;
}

BOOLEAN
BertAddGenericErrorData (
  IN EFI_ACPI_6_0_BOOT_ERROR_RECORD_TABLE_HEADER *Bert,
  IN EFI_CPER_SECTION_TYPE                       TypeOfErrorData,
  IN VOID                                        *GenericErrorData,
  IN UINT32                                      SizeOfGenericErrorData,
  IN ERROR_SEVERITY                              ErrorSeverity,
  IN BOOLEAN                                     Correctable
)
{
  BOOLEAN Status = ErrorBlockAddErrorData (
                     (VOID*)Bert->BootErrorRegion,
                     Bert->BootErrorRegionLength,
                     TypeOfErrorData,
                     GenericErrorData,
                     SizeOfGenericErrorData,
                     ErrorSeverity,
                     Correctable);
  return Status;
}

EFI_STATUS
BertHeaderCreator (
  IN BERT_CONTEXT  *Context,
  IN UINT32        ErrorBlockSize
)
{
  if (Context == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  Context->BertHeader = AllocateZeroPool (sizeof (EFI_ACPI_6_0_BOOT_ERROR_RECORD_TABLE_HEADER));
  Context->Block = AllocateReservedZeroPool (ErrorBlockSize);
  Context->BlockSize = ErrorBlockSize;
  *Context->BertHeader = (EFI_ACPI_6_0_BOOT_ERROR_RECORD_TABLE_HEADER) {
    ARM_ACPI_HEADER (
      EFI_ACPI_6_0_BOOT_ERROR_RECORD_TABLE_SIGNATURE,
      EFI_ACPI_6_0_BOOT_ERROR_RECORD_TABLE_HEADER,
      EFI_ACPI_6_0_BOOT_ERROR_RECORD_TABLE_REVISION
      ),
    Context->BlockSize,
    (UINT64)Context->Block
  };
  return EFI_SUCCESS;
}


