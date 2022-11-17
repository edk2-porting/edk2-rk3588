/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/SiliconPolicyUpdateLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

VOID *
EFIAPI
SiliconPolicyUpdatePreMem (
  IN VOID *Policy
  )
{
  return Policy;
}

VOID *
EFIAPI
SiliconPolicyUpdatePostMem (
  IN VOID *Policy
  )
{
  return Policy;
}

VOID *
EFIAPI
SiliconPolicyUpdateLate (
  IN VOID *Policy
  )
{
  return Policy;
}
