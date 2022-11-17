/** @file
  This file is the library for SA DXE Policy initialization.

Copyright (c) 2017 - 2020 Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "DxeSaPolicyInit.h"
#include <Library/ConfigBlockLib.h>

#define SA_VTD_RMRR_USB_LENGTH  0x20000

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PHYSICAL_ADDRESS                  mAddress;
GLOBAL_REMOVE_IF_UNREFERENCED UINTN                                 mSize;

/**
  Update RMRR Base and Limit Address for USB.

**/
VOID
UpdateRmrrUsbAddress (
  IN OUT  SA_POLICY_PROTOCOL    *SaPolicy
  )
{
  EFI_STATUS              Status;
  MISC_DXE_CONFIG         *MiscDxeConfig;

  Status = GetConfigBlock ((VOID *)SaPolicy, &gMiscDxeConfigGuid, (VOID *)&MiscDxeConfig);
  ASSERT_EFI_ERROR (Status);

  mSize = EFI_SIZE_TO_PAGES(SA_VTD_RMRR_USB_LENGTH);
  mAddress = SIZE_4GB;

  Status = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiReservedMemoryType,
                   mSize,
                   &mAddress
                  );
  ASSERT_EFI_ERROR (Status);

  MiscDxeConfig->RmrrUsbBaseAddress[0] = mAddress;
  MiscDxeConfig->RmrrUsbBaseAddress[1] = mAddress + SA_VTD_RMRR_USB_LENGTH - 1;
}

/**
  Get data for platform policy from setup options.

  @param[in] SaPolicy                  The pointer to get SA Policy protocol instance

  @retval EFI_SUCCESS                  Operation success.

**/
EFI_STATUS
EFIAPI
UpdateDxeSaPolicy (
  IN OUT  SA_POLICY_PROTOCOL    *SaPolicy
  )
{
  UpdateRmrrUsbAddress (SaPolicy);
  return EFI_SUCCESS;
}
