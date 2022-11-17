/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef GENERIC_HARDWARE_ERROR_SOURCE
#define GENERIC_HARDWARE_ERROR_SOURCE
#include "Apei.h"


typedef struct {
  UINT64 AckRegister;
  UINT64 ErrorStatusBlockAddress;
} GHES_REGISTER;

typedef enum {
  PROCESSOR_GENERIC   = 0,
  PROCESSOR_IA32_X64  = 1,
  PROCESSOR_IPF       = 2,
  PROCESSOR_ARM       = 3,
  PLATFORM_MEMORY     = 4,
  PLATFORM_MEMORY2    = 5,
  PCIE_EXPRESS        = 6,
  FIRMWARE_ERROR      = 7,
  PCI_BUS             = 8,
  PCI_COMPONENT       = 9
} EFI_CPER_SECTION_TYPE;
typedef enum {
  RECOVERABLE = 0,
  FATAL = 1,
  CORRECTED = 2,
  NONE = 3
} ERROR_SEVERITY;

EFI_ACPI_6_1_GENERIC_ERROR_STATUS_STRUCTURE*
ErrorBlockInitial(
  VOID   *Block,
  UINT32 Severity
);
BOOLEAN ErrorBlockAddErrorData (
  IN VOID                  *ErrorBlock,
  IN UINT32                MaxBlockLength,
  IN EFI_CPER_SECTION_TYPE TypeOfErrorData,
  IN VOID                  *GenericErrorData,
  IN UINT32                SizeOfGenericErrorData,
  IN ERROR_SEVERITY        ErrorSeverity,
  IN BOOLEAN               Correctable
);
BOOLEAN ErrorBlockAddErrorData (
  IN VOID                  *ErrorBlock,
  IN UINT32                MaxBlockLength,
  IN EFI_CPER_SECTION_TYPE TypeOfErrorData,
  IN VOID                  *GenericErrorData,
  IN UINT32                SizeOfGenericErrorData,
  IN ERROR_SEVERITY        ErrorSeverity,
  IN BOOLEAN               Correctable
);

VOID
GhesV2Initial (
  EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE  *GhesV2,
  UINT32                                                          BlockLength
);

/**
@param type - one of HARDWARE_ERROR_NOTIFICATION Type, GSIV For ARM,and SCI for X86,
              Notice: Windows OS hadn't support to GSIV, 20171026
*/
VOID
GhesV2AddNotification (
  EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE  *This,
  UINT8                                                           Type
);


EFI_STATUS
GhesV2LinkErrorBlock (
  EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE *GhesV2,
  GHES_REGISTER                                                  *Register,
  VOID                                                           *ErrorBlock
);
VOID
GhesV1Initial (
  EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE *GhesV1,
  UINT32                                               BlockLength
);
VOID
GhesV1AddNotification (
  EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE *This,
  UINT8                                                Type
);
EFI_STATUS
GhesV1LinkErrorBlock (
  EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE *This,
  UINT64                                               *ptrBlockAddress,
  VOID                                                 *ErrorBlock
);


#endif
