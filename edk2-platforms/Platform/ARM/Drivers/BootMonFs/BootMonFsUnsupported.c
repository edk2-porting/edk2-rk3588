/** @file
*
*  Copyright (c) 2012-2014, ARM Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include "BootMonFsInternal.h"

EFIAPI
EFI_STATUS
BootMonFsSetPositionUnsupported (
  IN EFI_FILE_PROTOCOL  *This,
  IN UINT64             Position
  )
{
  ASSERT(0);
  return EFI_UNSUPPORTED;
}

EFIAPI
EFI_STATUS
BootMonFsGetPositionUnsupported (
  IN EFI_FILE_PROTOCOL  *This,
  OUT UINT64            *Position
  )
{
  ASSERT(0);
  return EFI_UNSUPPORTED;
}
