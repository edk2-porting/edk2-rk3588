/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
#include "Einj.h"
#include <IndustryStandard/Acpi62.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include "OemApeiHi1620.h"
#include <PlatformArch.h>


EINJ_TABLE mEinj = {
  {
    ARM_ACPI_HEADER (
      EFI_ACPI_6_0_ERROR_INJECTION_TABLE_SIGNATURE,
      EFI_ACPI_6_0_ERROR_INJECTION_TABLE_HEADER,
      EFI_ACPI_6_0_ERROR_INJECTION_TABLE_REVISION
    ),
    sizeof (EFI_ACPI_6_0_ERROR_INJECTION_TABLE_HEADER),
    0x0,
    {
      0x0,
      0x0,
      0x0
    },
    EINJ_ACTION_NO
  },
  {
    {
      // 0 EFI_ACPI_6_0_EINJ_BEGIN_INJECTION_OPERATION
      EFI_ACPI_6_0_EINJ_BEGIN_INJECTION_OPERATION,
      EFI_ACPI_6_0_EINJ_WRITE_REGISTER_VALUE,
      EFI_ACPI_6_0_EINJ_PRESERVE_REGISTER,
      0,
      {
        EFI_ACPI_6_0_SYSTEM_MEMORY,
        64,
        0,
        EFI_ACPI_6_0_QWORD,
        0
      },
      EINJ_BEGIN_OPERATION_VALUE,
      EINJ_WRITE_MASK
    },
    {
      // 1 EFI_ACPI_6_0_EINJ_GET_TRIGGER_ERROR_ACTION_TABLE
      EFI_ACPI_6_0_EINJ_GET_TRIGGER_ERROR_ACTION_TABLE,
      EFI_ACPI_6_0_EINJ_READ_REGISTER,
      0,
      0,
      {
        EFI_ACPI_6_0_SYSTEM_MEMORY,
        64,
        0,
        EFI_ACPI_6_0_QWORD,
        0
      },
      0,
      EINJ_READ_MASK
    },
    {
      // 2 EFI_ACPI_6_0_EINJ_SET_ERROR_TYPE
      EFI_ACPI_6_0_EINJ_SET_ERROR_TYPE,
      EFI_ACPI_6_0_EINJ_WRITE_REGISTER,
      EFI_ACPI_6_0_EINJ_PRESERVE_REGISTER,
      0,
      {
        EFI_ACPI_6_0_SYSTEM_MEMORY,
        64,
        0,
        EFI_ACPI_6_0_QWORD,
        0
      },
      0,
      EINJ_WRITE_MASK
    },
    {
      // 3 EFI_ACPI_6_0_EINJ_GET_ERROR_TYPE
      EFI_ACPI_6_0_EINJ_GET_ERROR_TYPE,
      EFI_ACPI_6_0_EINJ_READ_REGISTER,
      0,
      0,
      {
        EFI_ACPI_6_0_SYSTEM_MEMORY,
        64,
        0,
        EFI_ACPI_6_0_QWORD,
        0
      },
      0,
      0xFFFFFFFF
    },
    {
      // 4 EFI_ACPI_6_0_EINJ_END_OPERATION
      EFI_ACPI_6_0_EINJ_END_OPERATION,
      EFI_ACPI_6_0_EINJ_WRITE_REGISTER_VALUE,
      EFI_ACPI_6_0_EINJ_PRESERVE_REGISTER,
      0,
      {
        EFI_ACPI_6_0_SYSTEM_MEMORY,
        64,
        0,
        EFI_ACPI_6_0_QWORD,
        0
      },
      EINJ_END_OPERATION_VALUE,
      0xFFFFFFFF
    },
    {
      // 5 EFI_ACPI_6_0_EINJ_EXECUTE_OPERATION
      EFI_ACPI_6_0_EINJ_EXECUTE_OPERATION,
      EFI_ACPI_6_0_EINJ_WRITE_REGISTER_VALUE,
      EFI_ACPI_6_0_EINJ_PRESERVE_REGISTER,
      0,
      {
        EFI_ACPI_6_0_SYSTEM_MEMORY,
        32,
        0,
        EFI_ACPI_6_0_DWORD,
        GPIO1_BASE + GPIO_INT_MASK //0x40070008//0x4d000F00//GPIO0_BASE + GPIO0_SWPORT_DR_OFFSET
      },
      0,
      0xFFFFFFFF //BIT0
    },
    {
      // 6 EFI_ACPI_6_0_EINJ_CHECK_BUSY_STATUS
      EFI_ACPI_6_0_EINJ_CHECK_BUSY_STATUS,
      EFI_ACPI_6_0_EINJ_READ_REGISTER_VALUE,
      0,
      0,
      {
        EFI_ACPI_6_0_SYSTEM_MEMORY,
        64,
        0,
        EFI_ACPI_6_0_QWORD,
        0
      },
      0x01,
      0x01
    },
    {
      // 7 EFI_ACPI_6_0_EINJ_GET_COMMAND_STATUS
      EFI_ACPI_6_0_EINJ_GET_COMMAND_STATUS,
      EFI_ACPI_6_0_EINJ_READ_REGISTER,
      0,
      0,
      {
        EFI_ACPI_6_0_SYSTEM_MEMORY,
        64,
        0,
        EFI_ACPI_6_0_QWORD,
        0
      },
      0,
      0x3
    },
    {
      // 8 EFI_ACPI_EINJ_SET_ERROR_TYPE_WITH_ADDRESS
      EFI_ACPI_EINJ_SET_ERROR_TYPE_WITH_ADDRESS,
      EFI_ACPI_6_0_EINJ_WRITE_REGISTER,
      EFI_ACPI_6_0_EINJ_PRESERVE_REGISTER,
      0,
      {
        EFI_ACPI_6_0_SYSTEM_MEMORY,
        64,
        0,
        EFI_ACPI_6_0_QWORD,
        0
      },
      0,
      0xFFFFFF
    },
    {
      // 9 EFI_ACPI_EINJ_GET_EXCUTE_OPERATION_TIMINGS
      EFI_ACPI_EINJ_GET_EXCUTE_OPERATION_TIMINGS,
      EFI_ACPI_6_0_EINJ_WRITE_REGISTER,
      EFI_ACPI_6_0_EINJ_PRESERVE_REGISTER,
      0,
      {
        EFI_ACPI_6_0_SYSTEM_MEMORY,
        64,
        0,
        EFI_ACPI_6_0_QWORD,
        0
      },
      0,
      0xFFFFFF
    }
  }
};

EINJ_TRIGGER_ERROR_ACTION mEinjTriggerErrorAction = {
  {
    sizeof (EFI_ACPI_6_0_EINJ_TRIGGER_ACTION_TABLE),
    0,
    sizeof (EINJ_TRIGGER_ERROR_ACTION),
    EINJ_TRIGGER_ERROR_ACTION_NO
  },
  {
    {
      EFI_ACPI_6_0_EINJ_TRIGGER_ERROR,
      EFI_ACPI_6_0_EINJ_NOOP,
      0,
      0,
      {
        EFI_ACPI_6_0_SYSTEM_MEMORY,
        64,
        0,
        EFI_ACPI_6_0_DWORD,
        0
      },
      0,
      0
    }
  }
};


VOID
EinjSetAcpiTable (
  EINJ_CONTEXT *Context
)
{
  UINTN                   AcpiTableHandle;
  EFI_STATUS              Status;
  UINT8                   Checksum;
  EFI_ACPI_SDT_HEADER     *Table;
  EFI_ACPI_TABLE_VERSION  TableVersion;
  UINTN                   TableKey;
  UINTN                   i;

  Context->EINJ->EinjTableHeader.Header.Length = sizeof (EINJ_TABLE);
  Checksum = CalculateCheckSum8 (
               (UINT8*)(Context->EINJ),
               Context->EINJ->EinjTableHeader.Header.Length);
  Context->EINJ->EinjTableHeader.Header.Checksum = Checksum;
  AcpiTableHandle = 0;
  Status = mAcpiTableProtocol->InstallAcpiTable (
             mAcpiTableProtocol,
             Context->EINJ,
             Context->EINJ->EinjTableHeader.Header.Length,
             &AcpiTableHandle
           );
  for (i = 0; i < EFI_ACPI_MAX_NUM_TABLES; i++) {
    Status = mAcpiSdtProtocol->GetAcpiTable (i, &Table, &TableVersion, &TableKey);
    if (EFI_ERROR (Status)) {
      break;
    }
    if (Table->Signature != EFI_ACPI_6_0_ERROR_INJECTION_TABLE_SIGNATURE) {
      continue;
    }
    mApeiTrustedfirmwareData->EinjTableAddress = (EFI_PHYSICAL_ADDRESS)Table;
    mApeiTrustedfirmwareData->EinjDataStruct = (EFI_PHYSICAL_ADDRESS)Context->EinjData;
  }
  ASSERT_EFI_ERROR (Status) ;
}
//V2
EFI_STATUS
EinjHeaderCreator (
  EINJ_CONTEXT *Context
)
{
  EFI_STATUS           Status;
  EINJ_DATA_STRUCTURE  *EinjData = NULL;
  Status = gBS->AllocatePool (
             EfiReservedMemoryType,
             sizeof (EINJ_DATA_STRUCTURE),
             (VOID**)&EinjData
           );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  gBS->SetMem (
    EinjData,
    sizeof (EINJ_DATA_STRUCTURE),
    0
  );

  DEBUG ((DEBUG_INFO, "EINJ EinjData is at 0x%X,size =0x%x\n",
          EinjData, sizeof (EINJ_DATA_STRUCTURE)));
  EinjData->TriggerErrorActionTablePtr =
    (EINJ_TRIGGER_ERROR_ACTION*)(&(EinjData->TriggerErrorActionTable));
  gBS->CopyMem (
         EinjData->TriggerErrorActionTablePtr,
         &mEinjTriggerErrorAction,
         sizeof (EINJ_TRIGGER_ERROR_ACTION));
  EinjData->OperationBegin = 0;
  EinjData->ErrorType = 0;
  EinjData->ErrorCapabilities = 0xFFF;
  EinjData->BusyStatus = 0;
  EinjData->CommandStatus = 0;
  mEinj.EinjInstructionEntry[0].RegisterRegion.Address =
    (UINT64)(&(EinjData->OperationBegin));
  mEinj.EinjInstructionEntry[1].RegisterRegion.Address =
    (UINT64)(&(EinjData->TriggerErrorActionTablePtr));
  mEinj.EinjInstructionEntry[2].RegisterRegion.Address =
    (UINT64)(&(EinjData->ErrorType));
  mEinj.EinjInstructionEntry[3].RegisterRegion.Address =
    (UINT64)(&(EinjData->ErrorCapabilities));
  mEinj.EinjInstructionEntry[4].RegisterRegion.Address =
    (UINT64)(&(EinjData->OperationBegin));
  mEinj.EinjInstructionEntry[6].RegisterRegion.Address =
    (UINT64)(&(EinjData->BusyStatus));
  mEinj.EinjInstructionEntry[7].RegisterRegion.Address =
    (UINT64)(&(EinjData->CommandStatus));
  mEinj.EinjInstructionEntry[8].RegisterRegion.Address =
    (UINT64)(&(EinjData->ErrorTypeWithAddress));
  mEinj.EinjInstructionEntry[9].RegisterRegion.Address =
    (UINT64)(&(EinjData->Timing));
  EinjData->ErrorTypeWithAddress.VendorErrorTypeOffset =
    (UINT32)((UINTN)&(EinjData->VendorErrorTypeExtension) -
    (UINTN)&(EinjData->ErrorTypeWithAddress));
  Context->EinjData = EinjData;
  Context->EINJ = &mEinj;
  Context->ExecuteOperationEntry = &mEinj.EinjInstructionEntry[5];
  Context->GetErrorTypeEntry = &mEinj.EinjInstructionEntry[3];
  return EFI_SUCCESS;
}


EFI_STATUS
EinjConfigErrorInjectCapability (
  EINJ_CONTEXT* Context,
  UINT32        BitsSupportedErrorType
)
{
  EFI_ACPI_6_0_EINJ_INJECTION_INSTRUCTION_ENTRY* KeyEntry;
  UINT32*                                        EinjCapablity;

  KeyEntry = Context->GetErrorTypeEntry;
  EinjCapablity = (UINT32*)KeyEntry->RegisterRegion.Address;
  *EinjCapablity = BitsSupportedErrorType;
  KeyEntry->Value = BitsSupportedErrorType;
  return EFI_SUCCESS;
}

