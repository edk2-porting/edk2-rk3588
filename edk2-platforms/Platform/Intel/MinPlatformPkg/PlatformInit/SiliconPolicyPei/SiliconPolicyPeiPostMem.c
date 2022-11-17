/** @file
  Silicon post-mem policy PEIM.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/SiliconPolicyInitLib.h>
#include <Library/SiliconPolicyUpdateLib.h>


/**
  Silicon Policy Init after memory PEI module entry point

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS          The function completes successfully
**/
EFI_STATUS
EFIAPI
SiliconPolicyPeiPostMemEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  VOID  *Policy;

  Policy = SiliconPolicyInitPostMem (NULL);
  SiliconPolicyUpdatePostMem (Policy);
  SiliconPolicyDonePostMem (Policy);

  return EFI_SUCCESS;
}