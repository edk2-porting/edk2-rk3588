/*
#  Copyright (c) 2014-2018, Linaro Limited. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
 */
#include <RdkBootManagerLib.h>

EFI_STATUS
EFIAPI
DriSecureBootEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status;

  Status = RdkHttpBoot ();

  Status = RdkSecureBoot (
    ImageHandle,
    SystemTable->BootServices
    );

  return Status;
}
