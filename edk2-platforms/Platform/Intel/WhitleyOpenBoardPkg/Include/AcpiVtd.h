/** @file
  This file describes the contents of the VTD ACPI Support

  @copyright
  Copyright 1996 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _ACPI_VTD_H
#define _ACPI_VTD_H

//
// Statements that include other files
//
#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/DevicePathLib.h>
#include <Library/HobLib.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/DmaRemappingReportingTable.h>
#include <Protocol/DmaRemap.h>
#include <Guid/HobList.h>
#include <Guid/PlatformInfo.h>
#include <Guid/SocketVariable.h>

//
// equates used in DMAR Table.
//
#define ACPI_DMAR_OEM_REVISION          0x01
#define ACPI_DMAR_OEM_CREATOR_ID        0x01
#define ACPI_DMAR_OEM_CREATOR_REVISION  0x01

#define TABLE_SIZE                      4 * 1024

typedef struct {
  UINT32                                Signature;
  EFI_HANDLE                            Handle;
  DMA_REMAP_PROTOCOL                    DmaRemapProt;
  EFI_ACPI_DMAR_HEADER                  *Dmar;
  EFI_ACPI_DMAR_HEADER                  *DmarOrder;
} VTD_SUPPORT_INSTANCE;

#endif
