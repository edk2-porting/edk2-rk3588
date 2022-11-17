/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>

EFI_STATUS
EFIAPI
TestPointCheckPiSignedFvBoot (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "==== TestPointCheckPiSignedFvBoot - Enter\n"));

  DEBUG ((DEBUG_INFO, "==== TestPointCheckPiSignedFvBoot - Exit\n"));
  return EFI_UNSUPPORTED;
}
