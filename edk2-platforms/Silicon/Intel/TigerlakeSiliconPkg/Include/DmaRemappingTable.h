/** @file
  This code defines ACPI DMA Remapping table related definitions.
  See the System Agent BIOS specification for definition of the table.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _DMA_REMAPPING_TABLE_H_
#define _DMA_REMAPPING_TABLE_H_

#include <Uefi.h>
#include <Base.h>
#include <IndustryStandard/DmaRemappingReportingTable.h>
#include <IndustryStandard/Acpi.h>
#include <TcssInfo.h>

#pragma pack(1)
///
/// DMAR table signature
///
#define EFI_ACPI_VTD_DMAR_TABLE_SIGNATURE   0x52414D44  ///< "DMAR"
#define EFI_ACPI_DMAR_TABLE_REVISION        2
#define EFI_ACPI_DRHD_ENGINE_HEADER_LENGTH  0x10
#define EFI_ACPI_RMRR_HEADER_LENGTH         0x18
#define MAX_PCI_DEPTH                       5

typedef struct {
  EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER DeviceScopeStructureHeader;
  EFI_ACPI_DMAR_PCI_PATH                      PciPath;     // device, function
} EFI_ACPI_DEV_SCOPE_STRUCTURE;

typedef struct {
  EFI_ACPI_DMAR_DRHD_HEADER     DrhdHeader;
  EFI_ACPI_DEV_SCOPE_STRUCTURE  DeviceScope[1];
} EFI_ACPI_DRHD_ENGINE1_STRUCT;

typedef struct {
  EFI_ACPI_DMAR_DRHD_HEADER     DrhdHeader;
  //
  // @todo use PCD
  //
  EFI_ACPI_DEV_SCOPE_STRUCTURE  DeviceScope[2];
} EFI_ACPI_DRHD_ENGINE3_STRUCT;

typedef struct {
  EFI_ACPI_DMAR_RMRR_HEADER     RmrrHeader;
  EFI_ACPI_DEV_SCOPE_STRUCTURE  DeviceScope[2];
} EFI_ACPI_RMRR_USB_STRUC;

typedef struct {
  EFI_ACPI_DMAR_RMRR_HEADER     RmrrHeader;
  EFI_ACPI_DEV_SCOPE_STRUCTURE  DeviceScope[1];    // IGD
} EFI_ACPI_RMRR_IGD_STRUC;

typedef struct {
  EFI_ACPI_DMAR_RMRR_HEADER     RmrrHeader;
  EFI_ACPI_DEV_SCOPE_STRUCTURE  DeviceScope[1];    // IGD - DiSM
} EFI_ACPI_RMRR_IGD_DISM_STRUC;

typedef struct {
  EFI_ACPI_DMAR_ANDD_HEADER     AnddHeader;
  UINT8                         AcpiObjectName[20];
} EFI_ACPI_ANDD_STRUC;

typedef struct {
  EFI_ACPI_DMAR_HEADER          DmarHeader;
  EFI_ACPI_DRHD_ENGINE1_STRUCT  DrhdEngine1;
  EFI_ACPI_DRHD_ENGINE3_STRUCT  DrhdEngine3;
  EFI_ACPI_RMRR_IGD_STRUC       RmrrIgd;
  EFI_ACPI_RMRR_IGD_DISM_STRUC  RmrrIgdDism;
} EFI_ACPI_DMAR_TABLE;

#pragma pack()

#endif
