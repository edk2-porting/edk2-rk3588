/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/SiliconPolicyInitLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

VOID *
EFIAPI
SiliconPolicyInitPreMem (
  IN VOID *Policy
  )
{
  return Policy;
}

RETURN_STATUS
EFIAPI
SiliconPolicyDonePreMem (
  IN VOID *Policy
  )
{
  return RETURN_SUCCESS;
}

VOID *
EFIAPI
SiliconPolicyInitPostMem (
  IN VOID *Policy
  )
{
  return Policy;
}

RETURN_STATUS
EFIAPI
SiliconPolicyDonePostMem (
  IN VOID *Policy
  )
{
  return RETURN_SUCCESS;
}

VOID *
EFIAPI
SiliconPolicyInitLate (
  IN VOID *Policy
  )
{
  return Policy;
}

RETURN_STATUS
EFIAPI
SiliconPolicyDoneLate (
  IN VOID *Policy
  )
{
  return RETURN_SUCCESS;
}
