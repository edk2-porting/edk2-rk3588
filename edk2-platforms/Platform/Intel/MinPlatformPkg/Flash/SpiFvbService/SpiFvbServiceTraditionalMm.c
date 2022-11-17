/** @file
  MM driver source for several Serial Flash devices
  which are compliant with the Intel(R) Serial Flash Interface Compatibility Specification.

  Copyright (c) Microsoft Corporation.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SpiFvbServiceCommon.h"
#include "SpiFvbServiceMm.h"

/**
  The driver Traditional MM entry point.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          A pointer to the EFI system table.

  @retval EFI_SUCCESS             This function always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
SpiFvbTraditionalMmInitialize (
  IN EFI_HANDLE            ImageHandle,
  IN EFI_SYSTEM_TABLE      *SystemTable
  )
{
  FvbInitialize ();

  return EFI_SUCCESS;
}
