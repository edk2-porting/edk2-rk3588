/** @file
  This code provides a initialization of intel VT-d (Virtualization Technology for Directed I/O).

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _VT_D_H_
#define _VT_D_H_

///
/// Include files
///
#include <DmaRemappingTable.h>
#include <SaAccess.h>
#include <PchAccess.h>
#include <Uefi.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/AcpiTable.h>
#include <Library/PciSegmentLib.h>
#include <Protocol/SaPolicy.h>
#include <Private/PchConfigHob.h>
#include <CpuRegs.h>
#include <Library/CpuPlatformLib.h>
#include <IndustryStandard/Pci22.h>


#define VTD_ECAP_REG  0x10
#define IR            BIT3

/**
  Locate the VT-d ACPI tables data file and read ACPI SSDT tables.
  Publish the appropriate SSDT based on current configuration and capabilities.

  @param[in] SaPolicy            SA DXE Policy protocol

  @retval EFI_SUCCESS - Vtd initialization complete
  @retval Other       - No Vtd function initiated
**/
EFI_STATUS
VtdInit (
  IN  SA_POLICY_PROTOCOL    *SaPolicy
  );

/**
  PciEnumerationComplete routine for update DMAR
**/
VOID
UpdateDmarPciEnumCompleteCallback (
  VOID
  );
#endif
