/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
#ifndef _APEI_H_
#define _APEI_H_

#include <IndustryStandard/Acpi.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/AcpiTable.h>

#define EFI_ACPI_MAX_NUM_TABLES         20
#define PRIVATE_ARM_SMC_ID_APEI         0x83000100
#define PRIVATE_ARM_SMC_ID_APEI_S       0x83000101

typedef struct {
  EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE  *HestCorrectedErrorGhesV2;
  EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE  *HestFatalErrorGhesV2;
  EFI_ACPI_6_1_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE  *HestRecoverableErrorGhesV2;
  EFI_PHYSICAL_ADDRESS                                            HestTableAddress;
  EFI_PHYSICAL_ADDRESS                                            EinjTableAddress;
  EFI_PHYSICAL_ADDRESS                                            EinjDataStruct;
  VOID                                                            *ErstContext;
} APEI_TRUSTED_FIRMWARE_STRUCTURE;

extern EFI_ACPI_TABLE_PROTOCOL       *mAcpiTableProtocol;
extern EFI_ACPI_SDT_PROTOCOL         *mAcpiSdtProtocol;
extern APEI_TRUSTED_FIRMWARE_STRUCTURE     *mApeiTrustedfirmwareData;


#endif    // _APEI_H_
