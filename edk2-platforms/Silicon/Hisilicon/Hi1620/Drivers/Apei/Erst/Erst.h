/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _ERST_H_
#define _ERST_H_

#include "Apei.h"

#define ERST_STATUS_SUCCESS                 EFI_ACPI_6_1_ERST_STATUS_SUCCESS
#define ERST_STATUS_NOT_ENOUGH_SPACE        EFI_ACPI_6_1_ERST_STATUS_NOT_ENOUGH_SPACE
#define ERST_STATUS_HARDWARE_NOT_AVAILABLE  EFI_ACPI_6_1_ERST_STATUS_HARDWARE_NOT_AVAILABLE
#define ERST_STATUS_FAILED                  EFI_ACPI_6_1_ERST_STATUS_FAILED
#define ERST_STATUS_RECORD_STORE_EMPTY      EFI_ACPI_6_1_ERST_STATUS_RECORD_STORE_EMPTY
#define ERST_STATUS_RECORD_NOT_FOUND        EFI_ACPI_6_1_ERST_STATUS_RECORD_NOT_FOUND

#define ERST_BEGIN_WRITE_VALUE              0x01
#define ERST_BEGIN_WRITE_MASK               0xFFFFFFFFFFFFFFFF
#define ERST_BEGIN_READ_VALUE               0x02
#define ERST_BEGIN_READ_MASK                0xFFFFFFFFFFFFFFFF
#define ERST_BEGIN_CLEAR_VALUE              0x03
#define ERST_BEGIN_CLEAR_MASK               0xFFFFFFFFFFFFFFFF
#define ERST_END_OPERATION_VALUE            0x04
#define ERST_END_OPERATION_MASK             0xFFFFFFFFFFFFFFFF
#define ERST_MASK                           0xFFFFFFFFFFFFFFFF

#define ERST_BEGIN_WRITE_OPERATION          EFI_ACPI_6_1_ERST_BEGIN_WRITE_OPERATION
#define ERST_BEGIN_READ_OPERATION           EFI_ACPI_6_1_ERST_BEGIN_READ_OPERATION
#define ERST_BEGIN_CLEAR_OPERATION          EFI_ACPI_6_1_ERST_BEGIN_CLEAR_OPERATION
#define ERST_END_OPERATION                  EFI_ACPI_6_1_ERST_END_OPERATION
#define ERST_BEGIN_DUMMY_WRITE_OPERATION    EFI_ACPI_6_1_ERST_BEGIN_DUMMY_WRITE_OPERATION

#define ERST_ACTION_NO                      16

#define ERST_RECORD_FREE                    0x00
#define ERST_RECORD_INUSE                   0x01

#define ERST_RECORD_STORE_IN_NVRAM          0
#define ERST_RECORD_STORE_IN_MEM            1
#define ERST_RECORD_STORE_IN_SPI_FLASH      2

#define ERST_LOG_ATTR_NVRAM                 0x02

typedef struct {
  UINT64 OperationId;
  UINT64 RecordOffset;
  UINT64 BusyStatus;
  UINT64 CommandStatus;
  UINT64 GetRecordId;
  UINT64 SetRecordId;
  UINT64 RecordCount;
  UINT64 DummyWrite;
  UINT64 Reserved;
  UINT64 ErrorLogAddrRange;
  UINT64 ErrorLogAddrRangeLength;
  UINT64 ErrorLogAttributes;
  UINT64 NvRamLogAddrNext;
  UINT64 NvRamLogSizeRemain;
} ERST_DATA_STRUCTURE;

typedef struct {
  UINT16 Signature;
  UINT16 Data0;
  UINT16 Data1;
  UINT8  Data2;
  UINT8  Attributes; //0: free
} ERST_ERROR_RECORD_INFO;

///
/// ERST Table
///



typedef struct _ERST_CONTEXT {
  UINT64 Operation;                  // WRITE,READ,CLEAR,END,
  UINT64 DummyWrite;                 //DUMMY_WRITE_OPEATION
  UINT64 RecordOffset;               // Offset form the buffer(error log  adress range)
  UINT32 BusyStatus;
  UINT32 CommandStatus;
  UINT64 KeyRecordId;                //OS Set the Record ID To Read/Write/Search
  UINT64 MaxTimeOfExecuteOperation;
  UINT64 RecordCount;                // Num of Record In NVRAM
  UINT64 ErrorLogAddressRange;       // Address Of Range Top
  UINT64 ErrorLogAddressRangeLength; // Address Of Range Top
  UINT64 ErrorLogAttributes;
  VOID   *NvRamLogAddrRange;
  UINT64 NvRamLogAddrRangeLength;
  UINT64 NvRamRecordOffset;
  UINT64 NvRamNextVallidRecordId;    //Get RecordId entry
  UINT64 NvRamNextValidRecordOffset;
  UINT64 NvRamAllRecordLength;
} ERST_RT_CONTEXT;

typedef struct _ERST_BOOT_CONTEXT {
  EFI_ACPI_6_1_ERROR_RECORD_SERIALIZATION_TABLE_HEADER *ErstHeader;
  EFI_ACPI_6_1_ERST_SERIALIZATION_INSTRUCTION_ENTRY    *ExecuteOperationEntry;
  EFI_ACPI_6_1_ERST_SERIALIZATION_INSTRUCTION_ENTRY    *GetErrorLogAddrRangeAttributes;
  ERST_RT_CONTEXT                                      *Rt;
} ERST_BOOT_CONTEXT;

extern EFI_ACPI_TABLE_PROTOCOL              *mAcpiTableProtocol;
extern APEI_TRUSTED_FIRMWARE_STRUCTURE      *mApeiTrustedfirmwareData;

EFI_STATUS
ErstHeaderCreator(
  ERST_BOOT_CONTEXT  *Context,
  UINT64             BufferSize,//ERST_DATASTORE_SIZE
  VOID               *NvRamAddrRange,
  UINT64             NvRamAllRecordLength,
  UINT64             NvRamAddrRangeLength
);

/***OEM***/
EFI_STATUS
OemInitErstTable (VOID);

EFI_STATUS
OemErstConfigExecuteOperationEntry (
  ERST_BOOT_CONTEXT *Context
);

VOID
ErstSetAcpiTable (
  ERST_BOOT_CONTEXT *Context
);


#endif    // _ERST_H_
