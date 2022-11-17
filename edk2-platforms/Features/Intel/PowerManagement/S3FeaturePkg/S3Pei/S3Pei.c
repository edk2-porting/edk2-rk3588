/** @file
  Source code file for S3 PEI module

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/PeiServicesLib.h>
#include <Library/SmmAccessLib.h>

/**
  S3 PEI module entry point

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS          The function completes successfully
  @retval     EFI_OUT_OF_RESOURCES Insufficient resources to create database
**/
EFI_STATUS
EFIAPI
S3PeiEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS Status;

  //
  // Install EFI_PEI_MM_ACCESS_PPI for S3 resume case
  //
  Status = PeiInstallSmmAccessPpi ();

  return Status;
}
