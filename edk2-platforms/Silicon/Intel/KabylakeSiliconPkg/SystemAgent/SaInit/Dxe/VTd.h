/** @file
  This code provides a initialization of intel VT-d (Virtualization Technology for Directed I/O).
  
Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
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
#include <Library/MmPciLib.h>
#include <Protocol/SaPolicy.h>
#include <SiConfigHob.h>
#include <PchConfigHob.h>
#include <CpuRegs.h>
#include <Library/CpuPlatformLib.h>


#define VTD_ECAP_REG  0x10
#define IR            BIT3

#define KBL_OEM_TABLE_ID    0x204C424B  ///< "KBL "
#define SKL_OEM_TABLE_ID    0x204C4B53  ///< "SKL "

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
  EndOfDxe routine for update DMAR
**/
VOID
UpdateDmarEndOfDxe (
  VOID
  );
#endif
