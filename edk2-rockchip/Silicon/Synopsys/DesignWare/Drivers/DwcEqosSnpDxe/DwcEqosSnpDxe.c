/** @file

  Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>
  Copyright (c) 2022 Jared McNeill <jmcneill@invisible.ca>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Eqos.h"

STATIC EFI_EVENT  mProtocolNotifyEvent;
STATIC VOID       *mProtocolNotifyEventRegistration;

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
STATIC
EFI_STATUS
EFIAPI
EqosDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  )
{
  DWC_EQOS_PLATFORM_DEVICE_PROTOCOL  *Platform;
  EFI_STATUS                         Status;

  //
  // Open the platform device protocol instance.
  //
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gDwcEqosPlatformDeviceProtocolGuid,
                  (VOID **)&Platform,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Clean up.
  //
  gBS->CloseProtocol (
         ControllerHandle,
         &gDwcEqosPlatformDeviceProtocolGuid,
         This->DriverBindingHandle,
         ControllerHandle
         );

  return EFI_SUCCESS;
}

/**
  ExitBootServices notification to stop the network controller.

  @param  Event      Pointer to this event.
  @param  Context    Event handler private data.

**/
STATIC
VOID
EFIAPI
EqosNotifyExitBootServices (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EQOS_PRIVATE_DATA  *Eqos = (EQOS_PRIVATE_DATA *)Context;

  EqosDisableTxRx (Eqos);
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
STATIC
EFI_STATUS
EFIAPI
EqosDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath   OPTIONAL
  )
{
  EQOS_PRIVATE_DATA  *Eqos;
  EFI_STATUS         Status;

  Eqos = AllocateZeroPool (sizeof (EQOS_PRIVATE_DATA));
  if (Eqos == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to allocate device context!\n", __func__));
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gDwcEqosPlatformDeviceProtocolGuid,
                  (VOID **)&Eqos->Platform,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to open protocol! Status=%r\n", __func__, Status));
    goto Free;
  }

  Status = EqosDmaAllocate (Eqos);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to allocate DMA buffers! Status=%r\n", __func__, Status));
    goto Free;
  }

  Eqos->Signature = EQOS_DRIVER_SIGNATURE;
  Eqos->Base      = Eqos->Platform->BaseAddress;

  EfiInitializeLock (&Eqos->Lock, TPL_CALLBACK);

  CopyMem (&Eqos->Snp, &gEqosSnpTemplate, sizeof Eqos->Snp);
  CopyMem (&Eqos->Aip, &gEqosAipTemplate, sizeof Eqos->Aip);

  Eqos->Snp.Mode                  = &Eqos->SnpMode;
  Eqos->SnpMode.State             = EfiSimpleNetworkStopped;
  Eqos->SnpMode.HwAddressSize     = NET_ETHER_ADDR_LEN;
  Eqos->SnpMode.MediaHeaderSize   = sizeof (ETHER_HEAD);
  Eqos->SnpMode.MaxPacketSize     = MAX_ETHERNET_PACKET_SIZE;
  Eqos->SnpMode.NvRamSize         = 0;
  Eqos->SnpMode.NvRamAccessSize   = 0;
  Eqos->SnpMode.ReceiveFilterMask = EFI_SIMPLE_NETWORK_RECEIVE_UNICAST |
                                    EFI_SIMPLE_NETWORK_RECEIVE_MULTICAST |
                                    EFI_SIMPLE_NETWORK_RECEIVE_BROADCAST |
                                    EFI_SIMPLE_NETWORK_RECEIVE_PROMISCUOUS |
                                    EFI_SIMPLE_NETWORK_RECEIVE_PROMISCUOUS_MULTICAST;
  Eqos->SnpMode.ReceiveFilterSetting = EFI_SIMPLE_NETWORK_RECEIVE_UNICAST |
                                       EFI_SIMPLE_NETWORK_RECEIVE_BROADCAST;
  Eqos->SnpMode.MaxMCastFilterCount   = MAX_MCAST_FILTER_CNT;
  Eqos->SnpMode.MCastFilterCount      = 0;
  Eqos->SnpMode.IfType                = NET_IFTYPE_ETHERNET;
  Eqos->SnpMode.MacAddressChangeable  = TRUE;
  Eqos->SnpMode.MultipleTxSupported   = FALSE;
  Eqos->SnpMode.MediaPresentSupported = TRUE;
  Eqos->SnpMode.MediaPresent          = FALSE;

  SetMem (&Eqos->SnpMode.BroadcastAddress, sizeof (EFI_MAC_ADDRESS), 0xFF);

  CopyMem (
    &Eqos->SnpMode.PermanentAddress,
    &Eqos->Platform->MacAddress,
    sizeof (EFI_MAC_ADDRESS)
    );
  CopyMem (
    &Eqos->SnpMode.CurrentAddress,
    &Eqos->Platform->MacAddress,
    sizeof (EFI_MAC_ADDRESS)
    );

  Status = EqosIdentify (Eqos);
  if (EFI_ERROR (Status)) {
    goto Free;
  }

  EqosInitializeConfig (Eqos);

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  EqosNotifyExitBootServices,
                  Eqos,
                  &gEfiEventExitBootServicesGuid,
                  &Eqos->ExitBootServicesEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_WARN,
      "%a: Failed to register for ExitBootServices event. Status=%r\n",
      __func__,
      Status
      ));
    goto Free;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ControllerHandle,
                  &gEfiSimpleNetworkProtocolGuid,
                  &Eqos->Snp,
                  &gEfiAdapterInformationProtocolGuid,
                  &Eqos->Aip,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Failed to install protocol interfaces. Status=%r\n",
      __func__,
      Status
      ));
    gBS->CloseProtocol (
           ControllerHandle,
           &gDwcEqosPlatformDeviceProtocolGuid,
           This->DriverBindingHandle,
           ControllerHandle
           );
    goto Free;
  }

  Eqos->ControllerHandle = ControllerHandle;
  return EFI_SUCCESS;

Free:
  if (Eqos->ExitBootServicesEvent != NULL) {
    gBS->CloseEvent (Eqos->ExitBootServicesEvent);
  }

  EqosDmaFree (Eqos);
  FreePool (Eqos);

  return Status;
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
STATIC
EFI_STATUS
EFIAPI
EqosDriverBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer   OPTIONAL
  )
{
  EFI_SIMPLE_NETWORK_PROTOCOL  *Snp;
  EQOS_PRIVATE_DATA            *Eqos;
  EFI_STATUS                   Status;

  Status = gBS->HandleProtocol (
                  ControllerHandle,
                  &gEfiSimpleNetworkProtocolGuid,
                  (VOID **)&Snp
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Eqos = EQOS_PRIVATE_DATA_FROM_SNP_THIS (Snp);

  ASSERT (Eqos->ControllerHandle == ControllerHandle);

  Status = gBS->UninstallMultipleProtocolInterfaces (
                  ControllerHandle,
                  &gEfiSimpleNetworkProtocolGuid,
                  &Eqos->Snp,
                  &gEfiAdapterInformationProtocolGuid,
                  &Eqos->Aip,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  gBS->CloseEvent (Eqos->ExitBootServicesEvent);

  Status = EqosStop (Eqos);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->CloseProtocol (
                  ControllerHandle,
                  &gDwcEqosPlatformDeviceProtocolGuid,
                  This->DriverBindingHandle,
                  ControllerHandle
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  EqosDmaFree (Eqos);
  FreePool (Eqos);

  return EFI_SUCCESS;
}

EFI_DRIVER_BINDING_PROTOCOL  gEqosDriverBinding = {
  EqosDriverBindingSupported,
  EqosDriverBindingStart,
  EqosDriverBindingStop,
  0x0a,
  NULL,
  NULL
};

/**
  Platform device protocol notification to program the MAC address
  into each existing controller, regardless of whether UEFI uses the
  interface (and performs the driver binding), so that the OS will
  still be able to read it.

  @param  Event      Pointer to this event.
  @param  Context    Event handler private data.

**/
STATIC
VOID
EFIAPI
OnProtocolNotify (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                         Status;
  EFI_HANDLE                         *HandleBuffer;
  UINTN                              HandleCount;
  UINTN                              Index;
  DWC_EQOS_PLATFORM_DEVICE_PROTOCOL  *Platform;

  while (TRUE) {
    Status = gBS->LocateHandleBuffer (
                    ByRegisterNotify,
                    NULL,
                    mProtocolNotifyEventRegistration,
                    &HandleCount,
                    &HandleBuffer
                    );
    if (EFI_ERROR (Status)) {
      break;
    }

    for (Index = 0; Index < HandleCount; Index++) {
      Status = gBS->HandleProtocol (
                      HandleBuffer[Index],
                      &gDwcEqosPlatformDeviceProtocolGuid,
                      (VOID **)&Platform
                      );
      ASSERT_EFI_ERROR (Status);

      EqosSetMacAddress (Platform->BaseAddress, &Platform->MacAddress);
    }

    FreePool (HandleBuffer);
  }
}

/**
  The entry point of EQOS UEFI Driver.

  @param  ImageHandle           The firmware allocated handle for the UEFI image.
  @param  SystemTable           A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.

**/
EFI_STATUS
EFIAPI
EqosEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gEqosDriverBinding,
             ImageHandle,
             &gEqosComponentName,
             &gEqosComponentName2
             );
  ASSERT_EFI_ERROR (Status);

  mProtocolNotifyEvent = EfiCreateProtocolNotifyEvent (
                           &gDwcEqosPlatformDeviceProtocolGuid,
                           TPL_CALLBACK,
                           OnProtocolNotify,
                           NULL,
                           &mProtocolNotifyEventRegistration
                           );
  ASSERT (mProtocolNotifyEvent != NULL);

  return EFI_SUCCESS;
}

/**
  Unload function of EQOS UEFI Driver.

  @param  ImageHandle            The allocated handle for the EFI image

  @retval EFI_SUCCESS            The driver was unloaded successfully
  @retval EFI_INVALID_PARAMETER  ImageHandle is not a valid image handle.

**/
EFI_STATUS
EFIAPI
EqosUnload (
  IN EFI_HANDLE  ImageHandle
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  *HandleBuffer;
  UINTN       HandleCount;
  UINTN       Index;

  //
  // Retrieve all platform device handles in the handle database
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gDwcEqosPlatformDeviceProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Disconnect the driver from the handles in the handle database
  //
  for (Index = 0; Index < HandleCount && !EFI_ERROR (Status); Index++) {
    Status = gBS->DisconnectController (
                    HandleBuffer[Index],
                    gImageHandle,
                    NULL
                    );
  }

  //
  // Free the handle array
  //
  gBS->FreePool (HandleBuffer);

  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_WARN,
      "%a: Failed to disconnect all controllers. Status=%r\n",
      __func__,
      Status
      ));
    return Status;
  }

  //
  // Uninstall protocols installed by the driver in its entrypoint
  //
  Status = EfiLibUninstallDriverBindingComponentName2 (
             &gEqosDriverBinding,
             &gEqosComponentName,
             &gEqosComponentName2
             );
  ASSERT_EFI_ERROR (Status);

  gBS->CloseEvent (mProtocolNotifyEvent);

  return EFI_SUCCESS;
}
