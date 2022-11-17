/** @file
 *
 *  Copyright (c) 2018, Andrey Warkentin <andrey.warkentin@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include "DwUsbHostDxe.h"

STATIC
EFI_STATUS
EFIAPI
DriverSupported (
  IN  EFI_DRIVER_BINDING_PROTOCOL *This,
  IN  EFI_HANDLE                  Controller,
  IN  EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
  );

STATIC
EFI_STATUS
EFIAPI
DriverStart (
  IN  EFI_DRIVER_BINDING_PROTOCOL *This,
  IN  EFI_HANDLE                  Controller,
  IN  EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
  );

STATIC
EFI_STATUS
EFIAPI
DriverStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL *This,
  IN  EFI_HANDLE                  Controller,
  IN  UINTN                       NumberOfChildren,
  IN  EFI_HANDLE                  *ChildHandleBuffer
  );

EFI_DRIVER_BINDING_PROTOCOL mDriverBinding = {
  DriverSupported,
  DriverStart,
  DriverStop,
  0xa,
  NULL,
  NULL
};

STATIC EFI_DW_DEVICE_PATH mDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8),
      }
    },
    EFI_CALLER_ID_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      sizeof (EFI_DEVICE_PATH_PROTOCOL),
      0
    }
  }
};

STATIC EFI_HANDLE mDevice;
STATIC RASPBERRY_PI_FIRMWARE_PROTOCOL *mFwProtocol;

STATIC
EFI_STATUS
EFIAPI
DriverSupported (
  IN  EFI_DRIVER_BINDING_PROTOCOL *This,
  IN  EFI_HANDLE                  Controller,
  IN  EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
  )
{
  VOID *Temp;
  EFI_STATUS Status;

  if (Controller != mDevice) {
    return EFI_UNSUPPORTED;
  }

  Status = gBS->LocateProtocol (&gRaspberryPiFirmwareProtocolGuid, NULL,
                  (VOID**)&mFwProtocol);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_READY;
  }

  if (gBS->HandleProtocol (Controller, &gEfiUsb2HcProtocolGuid,
             (VOID**)&Temp) == EFI_SUCCESS) {
    return EFI_ALREADY_STARTED;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
DriverStart (
  IN  EFI_DRIVER_BINDING_PROTOCOL *This,
  IN  EFI_HANDLE                  Controller,
  IN  EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
  )
{
  VOID *Dummy;
  EFI_STATUS Status;
  DWUSB_OTGHC_DEV *DwHc = NULL;

  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiCallerIdGuid,
                  (VOID**)&Dummy,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = mFwProtocol->SetPowerState (RPI_MBOX_POWER_STATE_USB_HCD, TRUE, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Couldn't power on USB: %r\n", Status));
    return Status;
  }

  Status = CreateDwUsbHc (&DwHc);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  /*
   * UsbBusDxe as of b4e96b82b4e2e47e95014b51787ba5b43abac784 expects
   * the HCD to do this. There is no agent invoking DwHcReset anymore.
   */
  DwHcReset (&DwHc->DwUsbOtgHc, EFI_USB_HC_RESET_HOST_CONTROLLER);
  DwHcSetState (&DwHc->DwUsbOtgHc, EfiUsbHcStateOperational);

  Status = gBS->InstallMultipleProtocolInterfaces (
    &Controller,
    &gEfiUsb2HcProtocolGuid, &DwHc->DwUsbOtgHc,
    NULL
  );

Exit:
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Could not start DwUsbHostDxe: %r\n", Status));

    DestroyDwUsbHc (DwHc);

    mFwProtocol->SetPowerState (RPI_MBOX_POWER_STATE_USB_HCD, FALSE, FALSE);

    gBS->CloseProtocol (
           Controller,
           &gEfiCallerIdGuid,
           This->DriverBindingHandle,
           Controller
         );
  }
  return Status;
}

STATIC
EFI_STATUS
EFIAPI
DriverStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL *This,
  IN  EFI_HANDLE                  Controller,
  IN  UINTN                       NumberOfChildren,
  IN  EFI_HANDLE                  *ChildHandleBuffer
  )
{
  EFI_STATUS Status;
  DWUSB_OTGHC_DEV *DwHc;
  EFI_USB2_HC_PROTOCOL *HcProtocol;

  Status = gBS->HandleProtocol (
                  Controller,
                  &gEfiUsb2HcProtocolGuid,
                  (VOID**)&HcProtocol
                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "DriverStop: HandleProtocol: %r\n", Status));
    return Status;
  }

  DwHc = DWHC_FROM_THIS (HcProtocol);

  Status = gBS->UninstallMultipleProtocolInterfaces (
    Controller,
    &gEfiUsb2HcProtocolGuid, &DwHc->DwUsbOtgHc,
    NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "DriverStop: UninstallMultipleProtocolInterfaces: %r\n",
      Status));
    return Status;
  }

  DwHcQuiesce (DwHc);
  DestroyDwUsbHc (DwHc);

  gBS->CloseProtocol (
         Controller,
         &gEfiCallerIdGuid,
         This->DriverBindingHandle,
         Controller
       );

  return EFI_SUCCESS;
}

/**
   UEFI Driver Entry Point API

   @param  ImageHandle       EFI_HANDLE.
   @param  SystemTable       EFI_SYSTEM_TABLE.

   @return EFI_SUCCESS       Success.
   EFI_DEVICE_ERROR  Fail.
**/

EFI_STATUS
EFIAPI
DwUsbHostEntryPoint (
  IN  EFI_HANDLE ImageHandle,
  IN  EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS Status;

  Status = gBS->InstallMultipleProtocolInterfaces (
    &mDevice,
    &gEfiDevicePathProtocolGuid, &mDevicePath,
    &gEfiCallerIdGuid, NULL,
    NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallMultipleProtocolInterfaces: %r\n", Status));
    return Status;
  }

  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &mDriverBinding,
             ImageHandle,
             &gComponentName,
             &gComponentName2
           );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "EfiLibInstallDriverBindingComponentName2: %r\n", Status));
    gBS->UninstallMultipleProtocolInterfaces (
           mDevice,
           &gEfiDevicePathProtocolGuid, &mDevicePath,
           &gEfiCallerIdGuid, NULL,
           NULL
         );
  }

  return Status;
}
