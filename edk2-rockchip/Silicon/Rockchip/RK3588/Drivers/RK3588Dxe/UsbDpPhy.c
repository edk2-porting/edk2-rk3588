/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <VarStoreData.h>

#include "RK3588DxeFormSetGuid.h"
#include "UsbDpPhy.h"

VOID
EFIAPI
ApplyUsbDpPhyVariables (
  VOID
  )
{
  /* nothing to do here, the PCDs are read by UsbDpPhyDxe */
}

VOID
EFIAPI
SetupUsbDpPhyVariables (
  VOID
  )
{
  UINTN       Size;
  UINT32      Var32;
  EFI_STATUS  Status;

  Size = sizeof (UINT32);

  Status = gRT->GetVariable (
                  L"UsbDpPhy0Usb3State",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdUsbDpPhy0Usb3State, USBDP_PHY_USB3_STATE_ENABLED);
    ASSERT_EFI_ERROR (Status);
  }

  Status = gRT->GetVariable (
                  L"UsbDpPhy1Usb3State",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdUsbDpPhy1Usb3State, USBDP_PHY_USB3_STATE_ENABLED);
    ASSERT_EFI_ERROR (Status);
  }
}
