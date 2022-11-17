/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Guid/Cper.h>
#include <IndustryStandard/Acpi62.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "Bert/Bert.h"
#include "Einj/Einj.h"
#include "ErrorSource/Ghes.h"
#include "Erst/Erst.h"
#include "Hest/Hest.h"
#include "OemApeiHi1620.h"

VOID
GpioCombInit (
  UINTN  Base,
  UINT32 Pin
)
{
  UINT32 Val = MmioRead32 (Base + GPIO_INT_MASK);
  MmioWrite32 (Base + GPIO_INT_MASK, Val | Pin);
  Val = MmioRead32 (Base + GPIO_INT_EN);
  MmioWrite32 (Base + GPIO_INT_EN, Val | Pin);
  Val = MmioRead32 (Base + GPIO_SWPORT_DDR);
  MmioWrite32 (Base + GPIO_SWPORT_DDR, Val & (~Pin));
  Val = MmioRead32 (Base + GPIO_INT_TYPE);
  MmioWrite32 (Base + GPIO_INT_TYPE, Val & (~Pin));
  Val = MmioRead32 (Base + GPIO_INT_POLARITY);
  MmioWrite32 (Base + GPIO_INT_POLARITY, Val | Pin);
  Val = MmioRead32 (Base + GPIO_LS_SYNC);
  MmioWrite32 (Base + GPIO_LS_SYNC, Val & (~Pin));
  MmioWrite32 (Base + GPIO_INT_COMB, 1);
  return;
}
/************************************************
*************** HEST ***************
************************************************/

/*****************************************************************************
* @param EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE* GhesV2,Vector of GhesV2
* @param UINT8 NumOfGhesV2
* @param_out
* @retval EFI_STATUS
*****************************************************************************/
EFI_STATUS
GhesV2ContextForHest (
  IN EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE GhesV2[MAX_GHES],
  IN UINT8                                                          NumOfGhesV2
)
{
  // ensuce the size is expected
  if ((GhesV2 == NULL) || (NumOfGhesV2 != MAX_GHES)) {
    return EFI_INVALID_PARAMETER;
  }

  UINT8  NumOfBlockPerGhes = 1;
  UINT8  Iter = 0;
  UINT32 BlockMemorySize;
  UINT32 ErrorSeverityArray[MAX_GHES] = {
           EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE,
           EFI_ACPI_6_2_ERROR_SEVERITY_FATAL,
           EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED};
  VOID   *ErrorBlockHead;
  VOID   *ErrorBlock;
  VOID   *BlockMemory;
  GHES_REGISTER  *GhesRegisters;
  EFI_STATUS     Status = EFI_SUCCESS;

  BlockMemorySize = MAX_GHES *
    (sizeof (GHES_REGISTER) + NumOfBlockPerGhes * GENERIC_HARDWARE_ERROR_BLOCK_SIZE);
  Status = gBS->AllocatePool (
             EfiReservedMemoryType,
             BlockMemorySize,
             &BlockMemory
           );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  gBS->SetMem (BlockMemory, BlockMemorySize, 0);
  GhesRegisters = BlockMemory;
  ErrorBlockHead = BlockMemory + MAX_GHES * sizeof (GHES_REGISTER);
  ErrorBlock = ErrorBlockHead;
  for (Iter = 0; Iter < MAX_GHES; Iter++) {
    GhesV2Initial (&GhesV2[Iter], GENERIC_HARDWARE_ERROR_BLOCK_SIZE);
    GhesV2AddNotification (&GhesV2[Iter], EFI_ACPI_6_1_HARDWARE_ERROR_NOTIFICATION_GSIV);
    ErrorBlockInitial (ErrorBlock, ErrorSeverityArray[Iter]);
    GhesV2LinkErrorBlock (&GhesV2[Iter], &GhesRegisters[Iter], ErrorBlock);
    ErrorBlock += GhesV2[Iter].ErrorStatusBlockLength;
  }
  return EFI_SUCCESS;
}
/*****************************************************************************
* @param EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE* GhesV2,Vector of GhesV2
* @param UINT8 NumOfGhesV2
* @param_out
* @retval EFI_STATUS
*****************************************************************************/

EFI_STATUS
GhesV1ContextForHest (
  IN EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE GhesV1[MAX_GHES],
  IN UINT8                                                NumOfGhesV1
)
{
  // ensuce the size is expected
  if ((GhesV1 == NULL) || (NumOfGhesV1 != MAX_GHES)) {
    return EFI_INVALID_PARAMETER;
  }

  UINT8      NumOfBlockPerGhes = 1;
  UINT8      Iter = 0;
  UINT32     BlockMemorySize = MAX_GHES *
             (sizeof (UINT64) + NumOfBlockPerGhes * GENERIC_HARDWARE_ERROR_BLOCK_SIZE);
  UINT32     ErrorSeverityArray[MAX_GHES] = {
             EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE,
             EFI_ACPI_6_2_ERROR_SEVERITY_FATAL,
             EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED};
  VOID       *ErrorBlockHead;
  VOID       *ErrorBlock;
  VOID       *BlockMemory;
  UINT64     *ptrBlockAddress;
  EFI_STATUS Status = EFI_SUCCESS;
  Status = gBS->AllocatePool (
             EfiReservedMemoryType,
             BlockMemorySize,
             &BlockMemory
           );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  gBS->SetMem (BlockMemory, BlockMemorySize, 0);
  ptrBlockAddress = BlockMemory;
  ErrorBlockHead = BlockMemory + MAX_GHES * sizeof (UINT64);
  ErrorBlock = ErrorBlockHead;
  for (Iter = 0; Iter < MAX_GHES; Iter++) {
    GhesV1Initial (&GhesV1[Iter], GENERIC_HARDWARE_ERROR_BLOCK_SIZE);
    GhesV1AddNotification (&GhesV1[Iter], EFI_ACPI_6_1_HARDWARE_ERROR_NOTIFICATION_GSIV);
    ErrorBlockInitial (ErrorBlock, ErrorSeverityArray[Iter]);
    GhesV1LinkErrorBlock (&GhesV1[Iter], &ptrBlockAddress[Iter], ErrorBlock);
    ErrorBlock += GhesV1[Iter].ErrorStatusBlockLength;
  }
  return EFI_SUCCESS;
}

VOID
OemHestInitialNotification ()
{
  // GPIO init
  // use int_msk to simulate
  UINTN Base = IOMUX_REG_BASE;
  //GPIO9, in document 'PhosphorV680 Totemiomux' iomg051,
  //Set GPIO9 to pad_ex_int1
  MmioWrite32 (Base + IOMG051, PAD_EX_INT1);
  return;
}

VOID
OemEinjInitialNotification ()
{
  UINTN Base = IOMUX_REG_BASE;
  //use TB_GPIO_PIN10 for EINJ
  MmioWrite32 (Base + IOMG052, PAD_EX_INT1);
  return;
}

EFI_STATUS
OemInitHestTable (
  IN EFI_HANDLE         ImageHandle
)
{
  EFI_STATUS                                                     Status = EFI_SUCCESS;
  HEST_CONTEXT                                                   HestContext;
  EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE GhesV2[MAX_GHES];
  Status = HestHeaderCreator (&HestContext, HEST_TABLE_SIZE);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = GhesV2ContextForHest(GhesV2, MAX_GHES);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status |= HestAddErrorSourceDescriptor (
              &HestContext,
              &GhesV2[0],
              sizeof (EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE)
              );
  mApeiTrustedfirmwareData->HestRecoverableErrorGhesV2 = HestContext.KeyErrorSource;
  Status |= HestAddErrorSourceDescriptor (
              &HestContext,
              &GhesV2[1],
              sizeof (EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE)
              );
  mApeiTrustedfirmwareData->HestFatalErrorGhesV2 = HestContext.KeyErrorSource;
  Status |= HestAddErrorSourceDescriptor (
              &HestContext,
              &GhesV2[2],
              sizeof (EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE)
              );
  mApeiTrustedfirmwareData->HestCorrectedErrorGhesV2 = HestContext.KeyErrorSource;
  OemHestInitialNotification ();
  HestSetAcpiTable (&HestContext);
  return Status;
}
/************************************************
*************** BERT ***************
************************************************/

EFI_STATUS
OemInitBertTable (
  IN EFI_HANDLE ImageHandle
)
{
  BERT_CONTEXT Context;
  BOOLEAN      Status;
  Status = BertHeaderCreator (&Context, BOOT_ERROR_REGION_SIZE);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ErrorBlockInitial (Context.Block, EFI_ACPI_6_2_ERROR_SEVERITY_NONE);
  BertSetAcpiTable (&Context);
  DEBUG ((DEBUG_INFO, "[%a]:[%dL]: %r\n", __FUNCTION__, __LINE__, Status));
  return EFI_SUCCESS;
}
/************************************************
*************** EINJ ***************
************************************************/
EFI_STATUS
OemEinjConfigExecuteOperationEntry (
  EINJ_CONTEXT *Context
)
{
  EFI_ACPI_6_0_EINJ_INJECTION_INSTRUCTION_ENTRY* KeyEntry = Context->ExecuteOperationEntry;
  OemEinjInitialNotification ();
  //use TB_GPIO_PIN10 for EINJ
  KeyEntry->RegisterRegion.Address = PcdGet64 (PcdCpldBaseAddress) + CPLD_GPIO10_INT_OFFSET;
  KeyEntry->Mask = CPLD_MASK;
  KeyEntry->Value = CPLD_VALUE;
  return EFI_SUCCESS;
}

EFI_STATUS
OemInitEinjTable (
)
{
  EFI_STATUS    Status;
  EINJ_CONTEXT  Context;
  Status = EinjHeaderCreator (&Context);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  (VOID)EinjConfigErrorInjectCapability (&Context, 0xFFF);// TBD
  (VOID)OemEinjConfigExecuteOperationEntry (&Context);
  EinjSetAcpiTable (&Context);
  DEBUG ((DEBUG_INFO, "[%a]:[%dL]: %d\n", __FUNCTION__, __LINE__, Status));
  return EFI_SUCCESS;
}
/************************************************
*************** ERST ***************
************************************************/

EFI_STATUS
OemErstConfigExecuteOperationEntry (
  ERST_BOOT_CONTEXT *Context
)
{
  EFI_ACPI_6_1_ERST_SERIALIZATION_INSTRUCTION_ENTRY *KeyEntry;
  KeyEntry = Context->ExecuteOperationEntry;
  KeyEntry->RegisterRegion.Address = GPIO1_BASE + GPIO_INT_MASK;
  KeyEntry->Value = 0x10;
  KeyEntry->Mask = 0xFFFFFFFF;
  GpioCombInit (GPIO1_BASE, 0xFFFF);
  return EFI_SUCCESS;
}

BOOLEAN
GetNvRamRegion (
  OUT  VOID  **NvRamAddrRange,
  OUT UINT64 *NvRamAddrRangeLength
)
{
  UINT32 Store = ERST_RECORD_STORE_IN_MEM;
  switch (Store) {
    case (ERST_RECORD_STORE_IN_NVRAM):
      break;
    case (ERST_RECORD_STORE_IN_MEM):
      * NvRamAddrRangeLength = ERST_DATASTORE_SIZE;
      * NvRamAddrRange = AllocateReservedZeroPool (ERST_DATASTORE_SIZE);
      break;
    case (ERST_RECORD_STORE_IN_SPI_FLASH):
      break;
    default:
      ;
  }
  return TRUE;
}

/***OEM***/
EFI_STATUS
OemInitErstTable (
)
{
  EFI_STATUS        Status = ERST_STATUS_SUCCESS;
  ERST_BOOT_CONTEXT Context;
  UINT64            BufferSize = ERST_DATASTORE_SIZE;
  VOID              *NvRamAddrRange;
  UINT64            NvRamAddrRangeLength;
  UINT64            NvRamAllRecordLength;

  GetNvRamRegion (&NvRamAddrRange, &NvRamAddrRangeLength);
  NvRamAllRecordLength = 0;
  Status = ErstHeaderCreator (
             &Context,
             BufferSize,
             NvRamAddrRange,
             NvRamAllRecordLength,
             NvRamAddrRangeLength);
  OemErstConfigExecuteOperationEntry (&Context);
  mApeiTrustedfirmwareData->ErstContext = (VOID*)Context.Rt;
  ErstSetAcpiTable (&Context);
  return Status;
};
