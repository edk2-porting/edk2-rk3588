/** @file
  Aspire VN7-572G Board Initialization Post-Memory library

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/BoardInitLib.h>

EFI_STATUS
EFIAPI
AspireVn7Dash572GBoardInitBeforeSiliconInit (
  VOID
  );

EFI_STATUS
EFIAPI
AspireVn7Dash572GBoardInitAfterSiliconInit (
  VOID
  );

EFI_STATUS
EFIAPI
BoardInitBeforeSiliconInit (
  VOID
  )
{
  return AspireVn7Dash572GBoardInitBeforeSiliconInit ();
}

EFI_STATUS
EFIAPI
BoardInitAfterSiliconInit (
  VOID
  )
{
  return AspireVn7Dash572GBoardInitAfterSiliconInit ();
}
