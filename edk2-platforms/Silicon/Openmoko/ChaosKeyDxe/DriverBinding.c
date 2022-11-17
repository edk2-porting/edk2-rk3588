/** @file
  Device driver for the ChaosKey hardware random number generator.

  Copyright (c) 2016 - 2017, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/UefiDriverEntryPoint.h>

#include "ChaosKeyDriver.h"

/**
  Tests to see if this driver supports a given controller.

  @param  This[in]                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL
                                   instance.
  @param  ControllerHandle[in]     The handle of the controller to test.
  @param  RemainingDevicePath[in]  The remaining device path.
                                   (Ignored - this is not a bus driver.)

  @retval EFI_SUCCESS              The driver supports this controller.
  @retval EFI_ALREADY_STARTED      The device specified by ControllerHandle is
                                   already being managed by the driver specified
                                   by This.
  @retval EFI_UNSUPPORTED          The device specified by ControllerHandle is
                                   not supported by the driver specified by This.

**/
EFI_STATUS
EFIAPI
UsbHwrngDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  )
{
  EFI_USB_DEVICE_DESCRIPTOR  Device;
  EFI_USB_IO_PROTOCOL        *UsbIo;
  EFI_STATUS                 Status;

  //
  //  Connect to the USB stack
  //
  Status = gBS->OpenProtocol (ControllerHandle,
                              &gEfiUsbIoProtocolGuid,
                              (VOID **) &UsbIo,
                              This->DriverBindingHandle,
                              ControllerHandle,
                              EFI_OPEN_PROTOCOL_BY_DRIVER);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  //  Get the interface descriptor to check the USB class and find a transport
  //  protocol handler.
  //
  Status = UsbIo->UsbGetDeviceDescriptor (UsbIo, &Device);
  if (!EFI_ERROR (Status)) {
    //
    //  Validate the adapter
    //
    if ((Device.IdVendor != CHAOSKEY_VENDOR_ID) ||
        (Device.IdProduct != CHAOSKEY_PRODUCT_ID)) {
      Status = EFI_UNSUPPORTED;
    } else {
      DEBUG ((DEBUG_INIT | DEBUG_INFO,
        "Detected ChaosKey RNG device (USB VendorID:0x%04x ProductID:0x%04x)\n",
        Device.IdVendor, Device.IdProduct));
      Status = EFI_SUCCESS;
    }
  }

  //
  // Clean up.
  //
  gBS->CloseProtocol (ControllerHandle,
                      &gEfiUsbIoProtocolGuid,
                      This->DriverBindingHandle,
                      ControllerHandle);

  return Status;
}


/**
  Starts a device controller or a bus controller.

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL
                                   instance.
  @param[in]  ControllerHandle     The handle of the device to start. This
                                   handle must support a protocol interface that
                                   supplies an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath  The remaining portion of the device path.
                                   (Ignored - this is not a bus driver.)

  @retval EFI_SUCCESS              The device was started.
  @retval EFI_DEVICE_ERROR         The device could not be started due to a
                                   device error.
  @retval EFI_OUT_OF_RESOURCES     The request could not be completed due to a
                                   lack of resources.

**/
EFI_STATUS
EFIAPI
UsbHwrngDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  return ChaosKeyInit (This->DriverBindingHandle, ControllerHandle);
}


/**
  Stops a device controller or a bus controller.

  @param[in]  This              A pointer to the EFI_DRIVER_BINDING_PROTOCOL
                                instance.
  @param[in]  ControllerHandle  A handle to the device being stopped. The handle
                                must support a bus specific I/O protocol for the
                                driver to use to stop the device.
  @param[in]  NumberOfChildren  The number of child device handles in
                                ChildHandleBuffer.
  @param[in]  ChildHandleBuffer An array of child handles to be freed. May be
                                NULL if NumberOfChildren is 0.

  @retval EFI_SUCCESS           The device was stopped.
  @retval EFI_DEVICE_ERROR      The device could not be stopped due to a device
                                error.

**/
EFI_STATUS
EFIAPI
UsbHwrngDriverBindingStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN  EFI_HANDLE                  ControllerHandle,
  IN  UINTN                       NumberOfChildren,
  IN  EFI_HANDLE                  *ChildHandleBuffer OPTIONAL
  )
{
  return ChaosKeyRelease (This->DriverBindingHandle, ControllerHandle);
}


STATIC
EFI_DRIVER_BINDING_PROTOCOL  gUsbDriverBinding = {
  UsbHwrngDriverBindingSupported,
  UsbHwrngDriverBindingStart,
  UsbHwrngDriverBindingStop,
  0xa,
  NULL,
  NULL
};


/**
  The entry point of ChaosKey UEFI Driver.

  @param  ImageHandle                The image handle of the UEFI Driver.
  @param  SystemTable                A pointer to the EFI System Table.

  @retval  EFI_SUCCESS               The Driver or UEFI Driver exited normally.
  @retval  EFI_INCOMPATIBLE_VERSION  _gUefiDriverRevision is greater than
                                     SystemTable->Hdr.Revision.

**/
EFI_STATUS
EFIAPI
EntryPoint (
  IN  EFI_HANDLE          ImageHandle,
  IN  EFI_SYSTEM_TABLE    *SystemTable
  )
{
  EFI_STATUS    Status;

  //
  //  Add the driver to the list of drivers
  //
  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle, SystemTable, &gUsbDriverBinding, ImageHandle,
             NULL, &gChaosKeyDriverComponentName2);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INIT | DEBUG_INFO, "*** Installed ChaosKey driver! ***\n"));

  return EFI_SUCCESS;
}


/**
  Unload function for the ChaosKey Driver.

  @param  ImageHandle[in]        The allocated handle for the EFI image

  @retval EFI_SUCCESS            The driver was unloaded successfully
  @retval EFI_INVALID_PARAMETER  ImageHandle is not a valid image handle.

**/
EFI_STATUS
EFIAPI
UnloadImage (
  IN EFI_HANDLE  ImageHandle
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  *HandleBuffer;
  UINTN       HandleCount;
  UINTN       Index;

  //
  // Retrieve all USB I/O handles in the handle database
  //
  Status = gBS->LocateHandleBuffer (ByProtocol,
                                    &gEfiUsbIoProtocolGuid,
                                    NULL,
                                    &HandleCount,
                                    &HandleBuffer);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Disconnect the driver from the handles in the handle database
  //
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->DisconnectController (HandleBuffer[Index],
                                        gImageHandle,
                                        NULL);
  }

  //
  // Free the handle array
  //
  gBS->FreePool (HandleBuffer);

  //
  // Uninstall protocols installed by the driver in its entrypoint
  //
  Status = gBS->UninstallMultipleProtocolInterfaces (ImageHandle,
                  &gEfiDriverBindingProtocolGuid,
                  &gUsbDriverBinding,
                  NULL
                  );

  return EFI_SUCCESS;
}
