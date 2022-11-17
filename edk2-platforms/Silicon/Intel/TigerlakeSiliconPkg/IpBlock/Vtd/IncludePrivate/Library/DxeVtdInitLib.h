/** @file
  Header file for DXE VTD Init Lib.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _DXE_VTD_INIT_LIB_H_
#define _DXE_VTD_INIT_LIB_H_

#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/VtdInfoLib.h>
#include <Library/SaPlatformLib.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/Pci.h>
#include <VtdDataHob.h>
#include <PchConfigHob.h>
#include <PchInfoHob.h>
#include <Register/P2sbRegs.h>
#include <Register/IpuRegs.h>
#include <Register/IgdRegs.h>
#include <Register/VtdRegs.h>
#include <DmaRemappingTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/SaPolicy.h>
#include <Protocol/SaNvsArea.h>
#include <Protocol/FirmwareVolume2.h>

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
  EndOfPcieEnum routine for update DMAR
**/
VOID
UpdateDmarEndOfPcieEnum (
  VOID
  );
#endif
