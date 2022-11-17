/** @file
  Header file for DXE Graphics Init Lib.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _DXE_GRAPHICS_INIT_LIB_H_
#define _DXE_GRAPHICS_INIT_LIB_H_

#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/Pci.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/PciSegmentLib.h>
#include <Protocol/SaPolicy.h>
#include <Protocol/SaNvsArea.h>
#include <Protocol/GopComponentName2.h>
#include <SiConfigHob.h>
#include <Register/SaRegsHostBridge.h>

/**
  Initialize GT ACPI tables

  @param[in] ImageHandle - Handle for the image of this driver
  @param[in] SaPolicy    - SA DXE Policy protocol

  @retval EFI_SUCCESS          - GT ACPI initialization complete
  @retval EFI_NOT_FOUND        - Dxe System Table not found.
  @retval EFI_OUT_OF_RESOURCES - Mmio not allocated successfully.
**/
EFI_STATUS
GraphicsInit (
  IN EFI_HANDLE             ImageHandle,
  IN SA_POLICY_PROTOCOL     *SaPolicy
  );

/**
  Do Post GT PM Init Steps after VBIOS Initialization.

  @retval EFI_SUCCESS          Succeed.
**/
EFI_STATUS
PostPmInitEndOfDxe (
  VOID
  );
#endif
