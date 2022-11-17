/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
#include "Erst.h"
#include <Guid/Cper.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <PlatformArch.h>

typedef struct {
  EFI_ACPI_6_1_ERROR_RECORD_SERIALIZATION_TABLE_HEADER ErstTableHeader;
  EFI_ACPI_6_1_ERST_SERIALIZATION_INSTRUCTION_ENTRY    ErstInstructionEntry[ERST_ACTION_NO];
} ERST_TABLE;

ERST_TABLE mErst = {
  {
    ARM_ACPI_HEADER (
      EFI_ACPI_6_0_ERROR_RECORD_SERIALIZATION_TABLE_SIGNATURE,
      EFI_ACPI_6_0_ERROR_RECORD_SERIALIZATION_TABLE_HEADER,
      EFI_ACPI_6_0_ERROR_RECORD_SERIALIZATION_TABLE_REVISION
    ),
    sizeof (EFI_ACPI_6_0_ERROR_RECORD_SERIALIZATION_TABLE_HEADER),
    {
      0x0,
      0x0,
      0x0,
      0x0
    },
    ERST_ACTION_NO,
  },
  {
    {
      // 0 EFI_ACPI_6_0_ERST_BEGIN_WRITE_OPERATION
      EFI_ACPI_6_0_ERST_BEGIN_WRITE_OPERATION,
      EFI_ACPI_6_0_ERST_WRITE_REGISTER_VALUE,
      0,
      0,
      {
        EFI_ACPI_6_0_SYSTEM_MEMORY,
        64,
        0,
        EFI_ACPI_6_0_QWORD,
        0
      },
      ERST_BEGIN_WRITE_VALUE,
      ERST_BEGIN_WRITE_MASK
    },
    {
      // 1 EFI_ACPI_6_0_ERST_BEGIN_READ_OPERATION
      EFI_ACPI_6_0_ERST_BEGIN_READ_OPERATION,
      EFI_ACPI_6_0_ERST_WRITE_REGISTER_VALUE,
      0,
      0,
      {
        EFI_ACPI_6_0_SYSTEM_MEMORY,
        64,
        0,
        EFI_ACPI_6_0_QWORD,
        0
      },
      ERST_BEGIN_READ_VALUE,
      ERST_BEGIN_READ_MASK
    },
    {
      // 2 EFI_ACPI_6_0_ERST_BEGIN_CLEAR_OPERATION
      EFI_ACPI_6_0_ERST_BEGIN_CLEAR_OPERATION,
      EFI_ACPI_6_0_ERST_WRITE_REGISTER_VALUE,
      0,
      0,
      {
        EFI_ACPI_6_0_SYSTEM_MEMORY,
        64,
        0,
        EFI_ACPI_6_0_QWORD,
        0
      },
      ERST_BEGIN_CLEAR_VALUE,
      ERST_BEGIN_CLEAR_MASK
    },
    {
      // 3 EFI_ACPI_6_0_ERST_END_OPERATION
      EFI_ACPI_6_0_ERST_END_OPERATION,
      EFI_ACPI_6_0_ERST_WRITE_REGISTER_VALUE,
      EFI_ACPI_6_0_ERST_PRESERVE_REGISTER,
      0,
      {
        EFI_ACPI_6_0_SYSTEM_MEMORY,
        64,
        0,
        EFI_ACPI_6_0_QWORD,
        0
      },
      ERST_END_OPERATION_VALUE,
      ERST_END_OPERATION_MASK
    },
    {
      // 4 EFI_ACPI_6_0_ERST_SET_RECORD_OFFSET
      EFI_ACPI_6_0_ERST_SET_RECORD_OFFSET,
      EFI_ACPI_6_0_ERST_WRITE_REGISTER,
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
      ERST_MASK
    },
    {
      // 5 EFI_ACPI_6_0_ERST_EXECUTE_OPERATION
      EFI_ACPI_6_0_ERST_EXECUTE_OPERATION,
      EFI_ACPI_6_0_ERST_WRITE_REGISTER_VALUE,
      0,
      0,
      {
        EFI_ACPI_6_0_SYSTEM_MEMORY,
        64,
        0,
        EFI_ACPI_6_0_DWORD,
        0x94730000
      },
      0x0002,
      0x0002
    },
    {
      // 6 EFI_ACPI_6_0_ERST_CHECK_BUSY_STATUS
      EFI_ACPI_6_0_ERST_CHECK_BUSY_STATUS,
      EFI_ACPI_6_0_ERST_READ_REGISTER_VALUE,
      0,
      0,
      {
        EFI_ACPI_6_0_SYSTEM_MEMORY,
        64,
        0,
        EFI_ACPI_6_0_QWORD,
        0
      },
      0x00000001,
      0x00000001
    },
    {
      // 7 EFI_ACPI_6_0_ERST_GET_COMMAND_STATUS
      EFI_ACPI_6_0_ERST_GET_COMMAND_STATUS,
      EFI_ACPI_6_0_ERST_READ_REGISTER,
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
      ERST_MASK
    },
    {
      // 8 EFI_ACPI_6_0_ERST_GET_RECORD_IDENTIFIER
      EFI_ACPI_6_0_ERST_GET_RECORD_IDENTIFIER,
      EFI_ACPI_6_0_ERST_READ_REGISTER,
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
      ERST_MASK
    },
    {
      // 9 EFI_ACPI_6_0_ERST_SET_RECORD_IDENTIFIER
      EFI_ACPI_6_0_ERST_SET_RECORD_IDENTIFIER,
      EFI_ACPI_6_0_ERST_WRITE_REGISTER,
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
      ERST_MASK
    },
    {
      // A EFI_ACPI_6_0_ERST_GET_RECORD_COUNT
      EFI_ACPI_6_0_ERST_GET_RECORD_COUNT,
      EFI_ACPI_6_0_ERST_READ_REGISTER,
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
      ERST_MASK
    },
    {
      // B EFI_ACPI_6_0_ERST_BEGIN_DUMMY_WRITE_OPERATION
      EFI_ACPI_6_0_ERST_BEGIN_DUMMY_WRITE_OPERATION,
      EFI_ACPI_6_0_ERST_WRITE_REGISTER,
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
      ERST_MASK
    },
    {
      // C RESERVED
      0x0C,
      EFI_ACPI_6_0_ERST_WRITE_REGISTER,
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
      ERST_MASK
    },
    {
      // D EFI_ACPI_6_0_ERST_GET_ERROR_LOG_ADDRESS_RANGE
      EFI_ACPI_6_0_ERST_GET_ERROR_LOG_ADDRESS_RANGE,
      EFI_ACPI_6_0_ERST_READ_REGISTER,
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
      ERST_MASK
    },
    {
      // E EFI_ACPI_6_0_ERST_GET_ERROR_LOG_ADDRESS_RANGE_LENGTH
      EFI_ACPI_6_0_ERST_GET_ERROR_LOG_ADDRESS_RANGE_LENGTH,
      EFI_ACPI_6_0_ERST_READ_REGISTER,
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
      ERST_MASK
    },
    {
      // F EFI_ACPI_6_0_ERST_GET_ERROR_LOG_ADDRESS_RANGE_ATTRIBUTES
      EFI_ACPI_6_0_ERST_GET_ERROR_LOG_ADDRESS_RANGE_ATTRIBUTES,
      EFI_ACPI_6_0_ERST_READ_REGISTER,
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
      ERST_MASK
    }
  }
};

VOID
ErstSetAcpiTable (
  ERST_BOOT_CONTEXT *Context
)
{
  UINTN                     AcpiTableHandle;
  EFI_STATUS                Status;
  UINT8                     Checksum;
  mErst.ErstTableHeader.Header.Length = sizeof (ERST_TABLE);
  Checksum = CalculateCheckSum8 ((UINT8*)(&mErst), mErst.ErstTableHeader.Header.Length);
  mErst.ErstTableHeader.Header.Checksum = Checksum;
  AcpiTableHandle = 0;
  Status = mAcpiTableProtocol->InstallAcpiTable (
             mAcpiTableProtocol,
             &mErst,
             mErst.ErstTableHeader.Header.Length,
             &AcpiTableHandle
             );
  ASSERT_EFI_ERROR (Status) ;
}

EFI_STATUS
ErstHeaderCreator (
  ERST_BOOT_CONTEXT  *Context,
  UINT64             BufferSize,//ERST_DATASTORE_SIZE
  VOID               *NvRamAddrRange,
  UINT64             NvRamAllRecordLength,
  UINT64             NvRamAddrRangeLength
)
{
  EFI_STATUS            Status = EFI_SUCCESS;
  ERST_RT_CONTEXT       *ErstRtCtx;
  //
  ErstRtCtx = AllocateReservedZeroPool (sizeof (ERST_RT_CONTEXT));
  ErstRtCtx->Operation = ERST_END_OPERATION;
  ErstRtCtx->RecordOffset = 0;
  ErstRtCtx->BusyStatus = 0;
  ErstRtCtx->CommandStatus = 0;
  ErstRtCtx->KeyRecordId = 0;
  ErstRtCtx->MaxTimeOfExecuteOperation = MAX_UINT64;
  ErstRtCtx->RecordCount = 0;
  ErstRtCtx->ErrorLogAddressRange = (UINT64)AllocateReservedZeroPool (BufferSize);
  ErstRtCtx->ErrorLogAddressRangeLength = BufferSize;
  ErstRtCtx->ErrorLogAttributes = 0;
  ErstRtCtx->NvRamLogAddrRange = NvRamAddrRange;
  ErstRtCtx->NvRamLogAddrRangeLength = NvRamAddrRangeLength;
  ErstRtCtx->NvRamRecordOffset = 0;
  ErstRtCtx->NvRamNextVallidRecordId = MAX_UINT64;
  ErstRtCtx->NvRamNextValidRecordOffset = 0;
  ErstRtCtx->NvRamAllRecordLength = NvRamAllRecordLength;
  mErst.ErstInstructionEntry[0].RegisterRegion.Address = (UINT64)(&(ErstRtCtx->Operation));
  mErst.ErstInstructionEntry[1].RegisterRegion.Address = (UINT64)(&(ErstRtCtx->Operation));
  mErst.ErstInstructionEntry[2].RegisterRegion.Address = (UINT64)(&(ErstRtCtx->Operation));
  mErst.ErstInstructionEntry[3].RegisterRegion.Address = (UINT64)(&(ErstRtCtx->Operation));
  mErst.ErstInstructionEntry[4].RegisterRegion.Address = (UINT64)(&(ErstRtCtx->RecordOffset));
  mErst.ErstInstructionEntry[6].RegisterRegion.Address = (UINT64)(&(ErstRtCtx->BusyStatus));
  mErst.ErstInstructionEntry[7].RegisterRegion.Address = (UINT64)(&(ErstRtCtx->CommandStatus));
  mErst.ErstInstructionEntry[8].RegisterRegion.Address = (UINT64)(&(ErstRtCtx->NvRamNextVallidRecordId));
  mErst.ErstInstructionEntry[9].RegisterRegion.Address = (UINT64)(&(ErstRtCtx->KeyRecordId));
  mErst.ErstInstructionEntry[10].RegisterRegion.Address = (UINT64)(&(ErstRtCtx->RecordCount));
  mErst.ErstInstructionEntry[11].RegisterRegion.Address = (UINT64)(&(ErstRtCtx->DummyWrite));
  mErst.ErstInstructionEntry[12].RegisterRegion.Address = 0;
  mErst.ErstInstructionEntry[13].RegisterRegion.Address = (UINT64)(&(ErstRtCtx->ErrorLogAddressRange));
  mErst.ErstInstructionEntry[14].RegisterRegion.Address = (UINT64)(&(ErstRtCtx->ErrorLogAddressRangeLength));
  mErst.ErstInstructionEntry[15].RegisterRegion.Address = (UINT64)(&(ErstRtCtx->ErrorLogAttributes));
  Context->ErstHeader = (EFI_ACPI_6_1_ERROR_RECORD_SERIALIZATION_TABLE_HEADER*)&mErst;
  Context->ExecuteOperationEntry = &(mErst.ErstInstructionEntry[5]);
  Context->GetErrorLogAddrRangeAttributes = &(mErst.ErstInstructionEntry[15]);
  Context->Rt = ErstRtCtx;
  return Status;
};

