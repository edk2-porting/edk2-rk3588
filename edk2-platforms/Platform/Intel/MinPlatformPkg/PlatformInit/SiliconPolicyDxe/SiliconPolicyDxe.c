/** @file
  Silicon DXE policy.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
 
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/SiliconPolicyInitLib.h>
#include <Library/SiliconPolicyUpdateLib.h>


/**
  Silicon Policy Init DXE module entry point
  
  @param[in] ImageHandle       Image handle of this driver.
  @param[in] SystemTable       Global system service table.

  @retval     EFI_SUCCESS          The function completes successfully
**/
EFI_STATUS
EFIAPI
SiliconPolicyDxeEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  VOID  *Policy;

  Policy = SiliconPolicyInitLate (NULL);
  SiliconPolicyUpdateLate (Policy);
  SiliconPolicyDoneLate (Policy);

  return EFI_SUCCESS;
}