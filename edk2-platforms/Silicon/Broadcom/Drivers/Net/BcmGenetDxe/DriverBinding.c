/** @file
  Device driver for the Broadcom GENET controller

  Copyright (c) 2020 Jared McNeill. All rights reserved.
  Copyright (c) 2020, ARM Limited. All rights reserved.
  Copyright (c) 2020 Andrey Warkentin <andrey.warkentin@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/NetLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/BcmGenetPlatformDevice.h>

#include "BcmGenetDxe.h"

STATIC EFI_EVENT      mProtocolNotifyEvent;
STATIC VOID           *mProtocolNotifyEventRegistration;

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
GenetDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  )
{
  BCM_GENET_PLATFORM_DEVICE_PROTOCOL *Dev;
  EFI_STATUS                         Status;

  //
  //  Connect to the non-discoverable device
  //
  Status = gBS->OpenProtocol (ControllerHandle,
                              &gBcmGenetPlatformDeviceProtocolGuid,
                              (VOID **)&Dev,
                              This->DriverBindingHandle,
                              ControllerHandle,
                              EFI_OPEN_PROTOCOL_BY_DRIVER);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Clean up.
  //
  gBS->CloseProtocol (ControllerHandle,
                      &gBcmGenetPlatformDeviceProtocolGuid,
                      This->DriverBindingHandle,
                      ControllerHandle);

  return EFI_SUCCESS;
}

/**
  Callback function to shut down the network device at ExitBootServices

  @param  Event                   Pointer to this event
  @param  Context                 Event handler private data

**/
STATIC
VOID
EFIAPI
GenetNotifyExitBootServices (
  EFI_EVENT     Event,
  VOID          *Context
  )
{
  GenetDisableTxRx ((GENET_PRIVATE_DATA *)Context);
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
GenetDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath   OPTIONAL
  )
{
  GENET_PRIVATE_DATA      *Genet;
  EFI_STATUS              Status;

  // Allocate Resources
  Genet = AllocateZeroPool (sizeof (GENET_PRIVATE_DATA));
  if (Genet == NULL) {
    DEBUG ((DEBUG_ERROR,
      "%a: Couldn't allocate private data\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gBS->OpenProtocol (ControllerHandle,
                              &gBcmGenetPlatformDeviceProtocolGuid,
                              (VOID **)&Genet->Dev,
                              This->DriverBindingHandle,
                              ControllerHandle,
                              EFI_OPEN_PROTOCOL_BY_DRIVER);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Couldn't open protocol: %r\n", __FUNCTION__, Status));
    goto FreeDevice;
  }

  Status = GenetDmaAlloc (Genet);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Couldn't allocate DMA buffers: %r\n", __FUNCTION__, Status));
    goto FreeDevice;
  }

  Genet->Signature                     = GENET_DRIVER_SIGNATURE;
  Genet->RegBase                       = Genet->Dev->BaseAddress;
  Genet->Phy.PrivateData               = Genet;
  Genet->Phy.Read                      = GenetPhyRead;
  Genet->Phy.Write                     = GenetPhyWrite;
  Genet->Phy.Configure                 = GenetPhyConfigure;
  Genet->Phy.ResetAction               = GenetPhyResetAction;
  Genet->PhyMode                       = GENET_PHY_MODE_RGMII_RXID;

  EfiInitializeLock (&Genet->Lock, TPL_CALLBACK);
  CopyMem (&Genet->Snp, &gGenetSimpleNetworkTemplate, sizeof Genet->Snp);
  CopyMem (&Genet->Aip, &gGenetAdapterInfoTemplate, sizeof Genet->Aip);

  Genet->Snp.Mode                       = &Genet->SnpMode;
  Genet->SnpMode.State                  = EfiSimpleNetworkStopped;
  Genet->SnpMode.HwAddressSize          = NET_ETHER_ADDR_LEN;
  Genet->SnpMode.MediaHeaderSize        = sizeof (ETHER_HEAD);
  Genet->SnpMode.MaxPacketSize          = MAX_ETHERNET_PKT_SIZE;
  Genet->SnpMode.NvRamSize              = 0;
  Genet->SnpMode.NvRamAccessSize        = 0;
  Genet->SnpMode.ReceiveFilterMask      = EFI_SIMPLE_NETWORK_RECEIVE_UNICAST |
                                          EFI_SIMPLE_NETWORK_RECEIVE_BROADCAST |
                                          EFI_SIMPLE_NETWORK_RECEIVE_PROMISCUOUS;
  Genet->SnpMode.ReceiveFilterSetting   = EFI_SIMPLE_NETWORK_RECEIVE_UNICAST |
                                          EFI_SIMPLE_NETWORK_RECEIVE_BROADCAST;
  Genet->SnpMode.MaxMCastFilterCount    = 0;
  Genet->SnpMode.MCastFilterCount       = 0;
  Genet->SnpMode.IfType                 = NET_IFTYPE_ETHERNET;
  Genet->SnpMode.MacAddressChangeable   = TRUE;
  Genet->SnpMode.MultipleTxSupported    = FALSE;
  Genet->SnpMode.MediaPresentSupported  = TRUE;
  Genet->SnpMode.MediaPresent           = FALSE;

  SetMem (&Genet->SnpMode.BroadcastAddress, sizeof (EFI_MAC_ADDRESS), 0xff);

  CopyMem (&Genet->SnpMode.PermanentAddress, &Genet->Dev->MacAddress,
    sizeof(EFI_MAC_ADDRESS));
  CopyMem (&Genet->SnpMode.CurrentAddress, &Genet->Dev->MacAddress,
    sizeof(EFI_MAC_ADDRESS));

  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK,
                  GenetNotifyExitBootServices, Genet,
                  &gEfiEventExitBootServicesGuid,
                  &Genet->ExitBootServicesEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN,
      "GenetDriverBindingStart: failed to register for ExitBootServices event - %r\n",
      Status));
    goto FreeDevice;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (&ControllerHandle,
                  &gEfiSimpleNetworkProtocolGuid,       &Genet->Snp,
                  &gEfiAdapterInformationProtocolGuid,  &Genet->Aip,
                  NULL);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Couldn't install protocol interfaces: %r\n", __FUNCTION__, Status));
    gBS->CloseProtocol (ControllerHandle,
                        &gBcmGenetPlatformDeviceProtocolGuid,
                        This->DriverBindingHandle,
                        ControllerHandle);
    goto FreeEvent;
  }

  Genet->ControllerHandle = ControllerHandle;
  return EFI_SUCCESS;

FreeEvent:
  gBS->CloseEvent (Genet->ExitBootServicesEvent);
FreeDevice:
  DEBUG ((DEBUG_WARN, "%a: Returning %r\n", __FUNCTION__, Status));
  FreePool (Genet);
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
GenetDriverBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer   OPTIONAL
  )
{
  EFI_SIMPLE_NETWORK_PROTOCOL     *SnpProtocol;
  GENET_PRIVATE_DATA              *Genet;
  EFI_STATUS                      Status;

  Status = gBS->HandleProtocol (ControllerHandle,
                                &gEfiSimpleNetworkProtocolGuid,
                                (VOID **)&SnpProtocol
                                );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Genet = GENET_PRIVATE_DATA_FROM_SNP_THIS (SnpProtocol);

  ASSERT (Genet->ControllerHandle == ControllerHandle);

  Status = gBS->UninstallMultipleProtocolInterfaces (ControllerHandle,
                  &gEfiSimpleNetworkProtocolGuid,       &Genet->Snp,
                  &gEfiAdapterInformationProtocolGuid,  &Genet->Aip,
                  NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->CloseEvent (Genet->ExitBootServicesEvent);
  ASSERT_EFI_ERROR (Status);

  GenetDmaFree (Genet);

  Status = gBS->CloseProtocol (ControllerHandle,
                               &gBcmGenetPlatformDeviceProtocolGuid,
                               This->DriverBindingHandle,
                               ControllerHandle);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  FreePool (Genet);

  return EFI_SUCCESS;
}

EFI_DRIVER_BINDING_PROTOCOL mGenetDriverBinding = {
  GenetDriverBindingSupported,
  GenetDriverBindingStart,
  GenetDriverBindingStop,
  GENET_VERSION,
  NULL,
  NULL
};

STATIC
VOID
EFIAPI
OnProtocolNotify (
  IN  EFI_EVENT       Event,
  IN  VOID            *Context
  )
{
  BCM_GENET_PLATFORM_DEVICE_PROTOCOL  *GenetDevice;
  EFI_STATUS                          Status;
  EFI_HANDLE                          *HandleBuffer;
  UINTN                               HandleCount;
  UINTN                               Index;
  UINT32                              Mac0, Mac1;

  while (TRUE) {
    Status = gBS->LocateHandleBuffer (ByRegisterNotify, NULL,
                    mProtocolNotifyEventRegistration, &HandleCount,
                    &HandleBuffer);
    if (EFI_ERROR (Status)) {
      break;
    }

    for (Index = 0; Index < HandleCount; Index++) {
      Status = gBS->HandleProtocol (HandleBuffer[Index],
                      &gBcmGenetPlatformDeviceProtocolGuid,
                      (VOID **)&GenetDevice);
      ASSERT_EFI_ERROR (Status);

      Mac0 = (GenetDevice->MacAddress.Addr[3])       |
             (GenetDevice->MacAddress.Addr[2] << 8)  |
             (GenetDevice->MacAddress.Addr[1] << 16) |
             (GenetDevice->MacAddress.Addr[0] << 24);
      Mac1 = (GenetDevice->MacAddress.Addr[5])       |
             (GenetDevice->MacAddress.Addr[4] << 8);

      MmioOr32 (GenetDevice->BaseAddress + GENET_SYS_RBUF_FLUSH_CTRL,
        GENET_SYS_RBUF_FLUSH_RESET);
      gBS->Stall (10);
      MmioAnd32 (GenetDevice->BaseAddress + GENET_SYS_RBUF_FLUSH_CTRL,
        ~GENET_SYS_RBUF_FLUSH_RESET);

      MemoryFence ();

      MmioWrite32 (GenetDevice->BaseAddress + GENET_UMAC_MAC0, Mac0);
      MmioWrite32 (GenetDevice->BaseAddress + GENET_UMAC_MAC1, Mac1);
    }
    FreePool (HandleBuffer);
  }
}

/**
  The entry point of GENET UEFI Driver.

  @param  ImageHandle                The image handle of the UEFI Driver.
  @param  SystemTable                A pointer to the EFI System Table.

  @retval  EFI_SUCCESS               The Driver or UEFI Driver exited normally.
  @retval  EFI_INCOMPATIBLE_VERSION  _gUefiDriverRevision is greater than
                                     SystemTable->Hdr.Revision.

**/
EFI_STATUS
EFIAPI
GenetEntryPoint (
  IN  EFI_HANDLE          ImageHandle,
  IN  EFI_SYSTEM_TABLE    *SystemTable
  )
{
  EFI_STATUS Status;

  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &mGenetDriverBinding,
             ImageHandle,
             &gGenetComponentName,
             &gGenetComponentName2
             );

  ASSERT_EFI_ERROR (Status);

  //
  // We need to program the MAC address into each existing controller,
  // regardless of whether UEFI ever makes use of the interface, so that
  // the OS driver will not have to care about this.
  //
  mProtocolNotifyEvent = EfiCreateProtocolNotifyEvent (
                           &gBcmGenetPlatformDeviceProtocolGuid,
                           TPL_CALLBACK, OnProtocolNotify, NULL,
                           &mProtocolNotifyEventRegistration);
  ASSERT (mProtocolNotifyEvent != NULL);

  DEBUG ((DEBUG_INIT | DEBUG_INFO, "Installed GENET UEFI driver!\n"));

  return EFI_SUCCESS;
}

/**
  Unload function of GENET UEFI Driver.

  @param  ImageHandle            The allocated handle for the EFI image

  @retval EFI_SUCCESS            The driver was unloaded successfully
  @retval EFI_INVALID_PARAMETER  ImageHandle is not a valid image handle.

**/
EFI_STATUS
EFIAPI
GenetUnload (
  IN EFI_HANDLE  ImageHandle
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  *HandleBuffer;
  UINTN       HandleCount;
  UINTN       Index;

  //
  // Retrieve all BcmGenetPlatformDevice handles in the handle database
  //
  Status = gBS->LocateHandleBuffer (ByProtocol,
                                    &gBcmGenetPlatformDeviceProtocolGuid,
                                    NULL,
                                    &HandleCount,
                                    &HandleBuffer);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Disconnect the driver from the handles in the handle database
  //
  for (Index = 0; Index < HandleCount && !EFI_ERROR (Status); Index++) {
    Status = gBS->DisconnectController (HandleBuffer[Index],
                                        gImageHandle,
                                        NULL);
  }

  //
  // Free the handle array
  //
  gBS->FreePool (HandleBuffer);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a: failed to disconnect all controllers - %r\n",
      __FUNCTION__, Status));
    return Status;
  }

  //
  // Uninstall protocols installed by the driver in its entrypoint
  //
  Status = EfiLibUninstallDriverBindingComponentName2 (
             &mGenetDriverBinding,
             &gGenetComponentName,
             &gGenetComponentName2
             );

  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
