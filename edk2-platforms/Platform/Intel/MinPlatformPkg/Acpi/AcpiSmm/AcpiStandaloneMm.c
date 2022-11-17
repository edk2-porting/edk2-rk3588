/** @file
  Standalone MM driver for ACPI initialization.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
Copyright (c) Microsoft Corporation.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiMm.h>

#include "AcpiMm.h"

/**
  The Standalone MM driver entry point.

  @param[in] ImageHandle   - Pointer to the loaded image protocol for this driver
  @param[in] SystemTable   - Pointer to the EFI MM System Table

  @retval Status           - EFI_SUCCESS
  @retval Assert, otherwise.

**/
EFI_STATUS
EFIAPI
AcpiStandaloneMmEntryPoint (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_MM_SYSTEM_TABLE    *MmSystemTable
  )
{
  InitializeAcpiMm ();

  return EFI_SUCCESS;
}
