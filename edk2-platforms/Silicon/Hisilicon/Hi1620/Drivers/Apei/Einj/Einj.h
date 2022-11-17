/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _EINJ_H_
#define _EINJ_H_

#include "Apei.h"

#define EINJ_ACTION_NO             10
#define EINJ_BEGIN_OPERATION_VALUE 0xFFFF
#define EINJ_END_OPERATION_VALUE   0
#define EINJ_WRITE_MASK            0xFFFFFFFF
#define EINJ_READ_VALUE            0xFFFF
#define EINJ_READ_MASK             0xFFFFFFFF

#define EINJ_TRIGGER_ERROR_ACTION_NO                                    1

#define EFI_ACPI_EINJ_SET_ERROR_TYPE_WITH_ADDRESS                       0x08
#define EFI_ACPI_EINJ_GET_EXCUTE_OPERATION_TIMINGS                      0x09


extern EFI_ACPI_TABLE_PROTOCOL            *mAcpiTableProtocol;
extern EFI_ACPI_SDT_PROTOCOL              *mAcpiSdtProtocol;
extern APEI_TRUSTED_FIRMWARE_STRUCTURE    *mApeiTrustedfirmwareData;

//
// Error Type Definition
//
#define EINJ_PROCESSOR_CORRECTABLE                                      BIT0
#define EINJ_PROCESSOR_UNCORRECTABLE_NONFATAL                           BIT1
#define EINJ_PROCESSOR_UNCORRECTABLE_FATAL                              BIT2
#define EINJ_MEMORY_CORRECTABLE                                         BIT3
#define EINJ_MEMORY_UNCORRECTABLE_NONFATAL                              BIT4
#define EINJ_MEMORY_UNCORRECTABLE_FATAL                                 BIT5
#define EINJ_PCIE_CORRECTABLE                                           BIT6
#define EINJ_PCIE_UNCORRECTABLE_NONFATAL                                BIT7
#define EINJ_PCIE_UNCORRECTABLE_FATAL                                   BIT8
#define EINJ_PLATFORM_CORRECTABLE                                       BIT9
#define EINJ_PLATFORM_UNCORRECTABLE_NONFATAL                            BIT10
#define EINJ_PLATFORM_UNCORRECTABLE_FATAL                               BIT11
#define EINJ_VENDOR_DEFINED_ERROR_TYPE                                  BIT31

#define EINJ_PROCESSOR_APIC_VALID                                       BIT0
#define EINJ_MEMORY_ADDRESS_VALID                                       BIT1
#define EINJ_PCIE_SBDF_VALID                                            BIT2

///
/// EINJ Table
///
typedef struct {
  EFI_ACPI_6_0_ERROR_INJECTION_TABLE_HEADER        EinjTableHeader;
  EFI_ACPI_6_0_EINJ_INJECTION_INSTRUCTION_ENTRY    EinjInstructionEntry[EINJ_ACTION_NO];
} EINJ_TABLE;

typedef struct {
  EFI_ACPI_6_0_EINJ_TRIGGER_ACTION_TABLE           TriggerErrorHeader;
  EFI_ACPI_6_0_EINJ_INJECTION_INSTRUCTION_ENTRY    ErrorInstructionEntry[EINJ_TRIGGER_ERROR_ACTION_NO];
} EINJ_TRIGGER_ERROR_ACTION;

typedef struct {
  UINT32 Reserved: 8;
  UINT32 Function: 3;
  UINT32 Device: 5;
  UINT32 PrimaryOrDeviceBus: 8;
  UINT32 Segment: 8;
} EINJ_PCIE_SBDF;

typedef struct {
  UINT32         ErrorType;
  UINT32         VendorErrorTypeOffset;
  UINT32         Flags;
  UINT32         ApicId;
  UINT64         MemAddress;
  UINT64         MemAddressRange;
  EINJ_PCIE_SBDF PcieSBDF;
} EINJ_SET_ERROR_TYPE_WITH_ADDRESS;

typedef struct {
  UINT32 Length;
  UINT32 SBDF;
  UINT16 VendorId;
  UINT16 DeviceId;
  UINT8  RevId;
  UINT8  Reserved[3];
} EINJ_VENDOR_ERROR_TYPE;

typedef struct {
  UINT64                           OperationBegin;
  UINT64                           ErrorType;
  UINT64                           ErrorCapabilities;
  UINT64                           BusyStatus;
  UINT64                           CommandStatus;
  UINT64                           Timing;
  EINJ_TRIGGER_ERROR_ACTION        *TriggerErrorActionTablePtr;
  EINJ_SET_ERROR_TYPE_WITH_ADDRESS ErrorTypeWithAddress;
  EINJ_VENDOR_ERROR_TYPE           VendorErrorTypeExtension;
  EINJ_TRIGGER_ERROR_ACTION        TriggerErrorActionTable;
} EINJ_DATA_STRUCTURE;

// V2
typedef struct _EINJ_CONTEXT {
  EINJ_TABLE                                    *EINJ;
  EINJ_DATA_STRUCTURE                           *EinjData;
  EFI_ACPI_6_0_EINJ_INJECTION_INSTRUCTION_ENTRY *GetErrorTypeEntry;
  EFI_ACPI_6_0_EINJ_INJECTION_INSTRUCTION_ENTRY *ExecuteOperationEntry;
} EINJ_CONTEXT;


EFI_STATUS
InitEinjTable(VOID);
// Version2
EFI_STATUS
EinjConfigErrorInjectCapability(
  EINJ_CONTEXT  *Context,
  UINT32        BitsSupportedErrorType
);
EFI_STATUS
EinjHeaderCreator(
  EINJ_CONTEXT  *Context
);
/***OEM***/
EFI_STATUS
OemInitEinjTable(VOID);
EFI_STATUS
OemEinjConfigExecuteOperationEntry(
  EINJ_CONTEXT *Context
);
VOID
EinjSetAcpiTable(
  EINJ_CONTEXT *Context
);


#endif    // _EINJ_H_
