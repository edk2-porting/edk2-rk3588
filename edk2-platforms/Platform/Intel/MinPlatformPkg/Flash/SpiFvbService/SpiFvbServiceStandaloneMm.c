/** @file
  MM driver source for several Serial Flash devices
  which are compliant with the Intel(R) Serial Flash Interface Compatibility Specification.

  Copyright (c) Microsoft Corporation.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SpiFvbServiceCommon.h"
#include "SpiFvbServiceMm.h"

/**
  The driver Standalone MM entry point.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] MmSystemTable        A pointer to the MM system table.

  @retval EFI_SUCCESS             This function always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
SpiFvbStandaloneMmInitialize (
  IN EFI_HANDLE            ImageHandle,
  IN EFI_MM_SYSTEM_TABLE   *MmSystemTable
  )
{
  FvbInitialize ();

  return EFI_SUCCESS;
}
