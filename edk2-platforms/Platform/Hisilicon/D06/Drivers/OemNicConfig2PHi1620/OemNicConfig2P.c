/** @file
*
*  Copyright (c) 2016-2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2016-2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <OemNicConfig.h>


EFI_STATUS
EFIAPI OemGetMac2P (
  IN OUT EFI_MAC_ADDRESS *Mac,
  IN     UINTN           Port
  )
{
  OemGetMac (Mac, Port);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI OemSetMac2P (
  IN EFI_MAC_ADDRESS *Mac,
  IN UINTN           Port
  )
{
  OemSetMac (Mac, Port);

  return EFI_SUCCESS;
}

HISI_BOARD_NIC_PROTOCOL mHisiBoardNicProtocol2P = {
  .GetMac = OemGetMac2P,
  .SetMac = OemSetMac2P,
};


EFI_STATUS
EFIAPI
OemNicConfigEntry (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS Status;

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gHisiBoardNicProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mHisiBoardNicProtocol2P
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Install Protocol failed %r\n",
        __FUNCTION__, __LINE__, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

