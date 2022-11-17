/** @file
  EFI Driver following Driver Binding Protocol.

  Copyright (c) 2010 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "SioDriver.h"


//
// This driver is for ACPI(PNP0A03,0)/PCI(0x1f,0)
//
//
//  Sio Driver Global Variables
//
EFI_DRIVER_BINDING_PROTOCOL mSioDriver = {
  SioDriverSupported,
  SioDriverStart,
  SioDriverStop,
  1,
  NULL,
  NULL
};

//
// The list of the created SIO_DEV
//
LIST_ENTRY                  mSioDevPool = INITIALIZE_LIST_HEAD_VARIABLE (mSioDevPool);

//
// Template structure to create SIO_DEV
//
SIO_DEV                     mSioDevTemplate = {
  SIO_DEV_SIGNATURE,        // Signature
  NULL,                     // PciHandle
  {
    0x00000000,             // HID
    0x00000000              // UID
  },
  NULL,                     // Handle
  {                         // Sio Instance
    SioRegisterAccess,
    SioGetResources,
    SioSetResources,
    SioPossibleResources,
    SioModify
  },
  NULL,                     // DevicePath
  {
    NULL,                   // ForwardLink
    NULL,                   // BackLink
  }
};

//
// Template ACPI_HID_DEVICE_PATH structure to create device path
//
ACPI_HID_DEVICE_PATH        mAcpiNodeTemplate = {
  {
    ACPI_DEVICE_PATH,       // Type
    ACPI_DP,                // SubType
    {
      sizeof (ACPI_HID_DEVICE_PATH),  // Length[0]
      0                               // Length[1]
    }
  },
  0x00000000,               // HID
  0x00000000                // UID
};


/**
  The user Entry Point for module Lpc47m17x. The user code starts with this function.

  @param[in]  ImageHandle    The firmware allocated handle for the EFI image.
  @param[in]  SystemTable    A pointer to the EFI System Table.

  @retval     EFI_SUCCESS    The entry point is executed successfully.
  @retval     other          Some error occurs when executing this entry point.
**/
EFI_STATUS
EFIAPI
SioDriverEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  if (EFI_ERROR (SioInit())) {
    return EFI_UNSUPPORTED;
  } else {

    //
    // Install protocols
    //
    return EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &mSioDriver,
             ImageHandle,
             &mSioComponentName,
             &mSioComponentName2
             );
  }
}


/**
  Compares a PCI to ISA bridge device segment, bus, device and function to the
  PcdSuperIoPciIsaBridgeDevice values.

  @param[in]  CurrentDevice       The device to be compared with the PcdSuperIoPciIsaBridgeDevice information
  @retval     TRUE                This device matches PcdSuperIoPciIsaBridgeDevice values
  @retval     FALSE               This device does not match the PcdSuperIoPciIsaBridgeDevice values
**/
BOOLEAN
EFIAPI
SioDeviceEnabled (
  IN SIO_PCI_ISA_BRIDGE_DEVICE_INFO *CurrentDevice
  )
{
  SIO_PCI_ISA_BRIDGE_DEVICE_INFO *Device;
  Device = (SIO_PCI_ISA_BRIDGE_DEVICE_INFO *) FixedPcdGetPtr (PcdSuperIoPciIsaBridgeDevice);
  if(CompareMem (Device, CurrentDevice, sizeof (SIO_PCI_ISA_BRIDGE_DEVICE_INFO)) == 0) {
    return TRUE;
  }
  return FALSE;
}

/**
  Test to see if this driver supports Controller Handle.

  @param[in]  This                Protocol instance pointer.
  @param[in]  Controller          Handle of device to test
  @param[in]  RemainingDevicePath Optional parameter use to pick a specific child
                                  device to start.

  @retval     EFI_SUCCESS         This driver supports this device
  @retval     EFI_ALREADY_STARTED This driver is already running on this device
  @retval     other               This driver does not support this device
**/
EFI_STATUS
EFIAPI
SioDriverSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  )
{
  EFI_STATUS                Status;
  EFI_PCI_IO_PROTOCOL       *PciIo;
  EFI_DEVICE_PATH_PROTOCOL  *ParentDevicePath;
  ACPI_HID_DEVICE_PATH      *AcpiNode;
  PCI_TYPE00                Pci;
  UINTN                     Index;
  EFI_SIO_ACPI_DEVICE_ID    *Devices;
  UINTN                     Count;
  UINTN                     SegmentNumber;
  UINTN                     BusNumber;
  UINTN                     DeviceNumber;
  UINTN                     FunctionNumber;
  SIO_PCI_ISA_BRIDGE_DEVICE_INFO  SioDevice;

  //
  // If RemainingDevicePath is not NULL, it should verify that the first device
  // path node in RemainingDevicePath is an ACPI Device path node which is a
  // legal Device Path Node for this bus driver's children.
  //
  if (RemainingDevicePath != NULL) {
    if (!IsDevicePathEnd (RemainingDevicePath)) {
      if ((RemainingDevicePath->Type != ACPI_DEVICE_PATH) ||
          (((RemainingDevicePath->SubType != ACPI_DP) || (DevicePathNodeLength (RemainingDevicePath) != sizeof (ACPI_HID_DEVICE_PATH))) &&
          ((RemainingDevicePath->SubType != ACPI_EXTENDED_DP) || (DevicePathNodeLength (RemainingDevicePath) != sizeof (ACPI_EXTENDED_HID_DEVICE_PATH))))
          ) {
        return EFI_UNSUPPORTED;
      }

      DeviceGetList (&Devices, &Count);
      if (Devices == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      AcpiNode = (ACPI_HID_DEVICE_PATH *) RemainingDevicePath;
      for (Index = 0; Index < Count; Index++) {
        if ((AcpiNode->HID == Devices[Index].HID) &&
            (AcpiNode->UID == Devices[Index].UID)) {
          break;
        }
      }
      FreePool (Devices);
      if (Index == Count) {
        return EFI_UNSUPPORTED;
      }
    }
  }

  //
  // See if the parent device path can be opened BY_DRIVER
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &ParentDevicePath,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status) && (Status != EFI_ALREADY_STARTED)) {
    return Status;
  }

  gBS->CloseProtocol (
         Controller,
         &gEfiDevicePathProtocolGuid,
         This->DriverBindingHandle,
         Controller
         );

  //
  // Get PciIo protocol instance
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );

  if (!EFI_ERROR (Status)) {
    Status = PciIo->Pci.Read (
                          PciIo,
                          EfiPciIoWidthUint32,
                          0,
                          sizeof (Pci) / sizeof (UINT32),
                          &Pci
                          );
    ASSERT_EFI_ERROR (Status);

    Status = EFI_UNSUPPORTED;
    if ((Pci.Hdr.Command & (EFI_PCI_COMMAND_IO_SPACE | EFI_PCI_COMMAND_MEMORY_SPACE))
                        == (EFI_PCI_COMMAND_IO_SPACE | EFI_PCI_COMMAND_MEMORY_SPACE)
       ) {
      if (Pci.Hdr.ClassCode[2] == PCI_CLASS_BRIDGE) {
        //
        // See if this is a standard PCI to ISA Bridge from the Base Code and Class Code
        //
        if (Pci.Hdr.ClassCode[1] == PCI_CLASS_BRIDGE_ISA) {
          Status = EFI_SUCCESS;
        }

        //
        // See if this is an Intel PCI to ISA Bridge in Positive Decode Mode
        //
        if ((Pci.Hdr.ClassCode[1] == PCI_CLASS_BRIDGE_ISA_PDECODE) &&
            (Pci.Hdr.VendorId == 0x8086)) {
          //
          // See if this is on Function #0 to avoid false positive on
          // PCI_CLASS_BRIDGE_OTHER that has the same value as
          // PCI_CLASS_BRIDGE_ISA_PDECODE
          //
          Status = PciIo->GetLocation (
                            PciIo,
                            &SegmentNumber,
                            &BusNumber,
                            &DeviceNumber,
                            &FunctionNumber
                            );
          if (!EFI_ERROR (Status) && (FunctionNumber == 0)) {
            Status = EFI_SUCCESS;
          } else {
            Status = EFI_UNSUPPORTED;
          }
        }
        if(!EFI_ERROR (Status)) {
          Status = PciIo->GetLocation (
                            PciIo,
                            &SegmentNumber,
                            &BusNumber,
                            &DeviceNumber,
                            &FunctionNumber
                            );
          if(!EFI_ERROR (Status)) {
            SioDevice.Segment = (UINT8) SegmentNumber;
            SioDevice.Bus = (UINT8) BusNumber;
            SioDevice.Device = (UINT8) DeviceNumber;
            SioDevice.Funtion = (UINT8) FunctionNumber;
            if(!SioDeviceEnabled (&SioDevice)) {
              Status = EFI_UNSUPPORTED;
            }
          }
        }
      }
    }

    gBS->CloseProtocol (
           Controller,
           &gEfiPciIoProtocolGuid,
           This->DriverBindingHandle,
           Controller
           );
  }
  if (EFI_ERROR (Status) && (Status != EFI_ALREADY_STARTED)) {
    return Status;
  }

  return EFI_SUCCESS;
}


/**
  Destroy the SIO controller handle.

  @param[in]  ChildHandle     The SIO controller handle.

  @retval     EFI_SUCCESS     The SIO controller handle is destroyed successfully.
**/
EFI_STATUS
SioDestroyDevice (
  IN EFI_HANDLE                ChildHandle
  )
{
  EFI_STATUS                Status;
  SIO_DEV                   *SioDev;
  EFI_SIO_PROTOCOL          *Sio;
  EFI_PCI_IO_PROTOCOL       *PciIo;

  Status = gBS->HandleProtocol (
                  ChildHandle,
                  &gEfiSioProtocolGuid,
                  (VOID **) &Sio
                  );
  ASSERT_EFI_ERROR (Status);

  SioDev = SIO_DEV_FROM_THIS (Sio);

  Status = gBS->CloseProtocol (
                  SioDev->PciHandle,
                  &gEfiPciIoProtocolGuid,
                  mSioDriver.DriverBindingHandle,
                  ChildHandle
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->UninstallMultipleProtocolInterfaces (
                  ChildHandle,
                  &gEfiDevicePathProtocolGuid,
                  SioDev->DevicePath,
                  &gEfiSioProtocolGuid,
                  &SioDev->Sio,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    gBS->OpenProtocol (
           SioDev->PciHandle,
           &gEfiPciIoProtocolGuid,
           (VOID **) &PciIo,
           mSioDriver.DriverBindingHandle,
           ChildHandle,
           EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER
           );
    return Status;
  }

  RemoveEntryList (&SioDev->Link);
  FreePool (SioDev->DevicePath);
  FreePool (SioDev);
  return EFI_SUCCESS;
}


/**
  Create the SIO controller handle.

  @param[in]  Controller       The parent PCI controller handle.
  @param[in]  Device           Pointer to EFI_SIO_ACPI_DEVICE_ID.
  @param[in]  ParentDevicePath The device path of the parent controller.
  @param[out] PciIo            The PciIo instance of the parent controller.
**/
VOID
SioCreateDevice (
  IN  EFI_HANDLE                Controller,
  IN  EFI_SIO_ACPI_DEVICE_ID    *Device,
  IN  EFI_DEVICE_PATH_PROTOCOL  *ParentDevicePath,
  OUT EFI_PCI_IO_PROTOCOL       *PciIo
  )
{
  EFI_STATUS                Status;
  SIO_DEV                   *SioDev;

  DeviceEnable (Device);
  SioDev = AllocateCopyPool (sizeof (SIO_DEV), &mSioDevTemplate);
  ASSERT (SioDev != NULL);
  if (SioDev == NULL) {
    return;
  }
  InsertHeadList (&mSioDevPool, &SioDev->Link);

  SioDev->PciHandle       = Controller;

  CopyMem (&SioDev->Device, Device, sizeof (*Device));

  mAcpiNodeTemplate.HID = Device->HID;
  mAcpiNodeTemplate.UID = Device->UID;
  SioDev->DevicePath = AppendDevicePathNode (ParentDevicePath, (EFI_DEVICE_PATH_PROTOCOL *) &mAcpiNodeTemplate);
  ASSERT (SioDev->DevicePath != NULL);

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &SioDev->Handle,
                  &gEfiSioProtocolGuid,        &SioDev->Sio,
                  &gEfiDevicePathProtocolGuid, SioDev->DevicePath,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo,
                  mSioDriver.DriverBindingHandle,
                  SioDev->Handle,
                  EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER
                  );
  ASSERT_EFI_ERROR (Status);
}


/**
  Start this driver on ControllerHandle.

  @param[in]  This                 Protocol instance pointer.
  @param[in]  Controller           Handle of device to bind driver to
  @param[in]  RemainingDevicePath  Optional parameter use to pick a specific child
                                   device to start.

  @retval     EFI_SUCCESS          This driver is added to ControllerHandle
  @retval     EFI_ALREADY_STARTED  This driver is already running on ControllerHandle
  @retval     other                This driver does not support this device
**/
EFI_STATUS
EFIAPI
SioDriverStart (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  )
{
  EFI_STATUS                          Status;
  EFI_PCI_IO_PROTOCOL                 *PciIo;
  EFI_DEVICE_PATH_PROTOCOL            *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL            *ParentDevicePath;
  EFI_SIO_ACPI_DEVICE_ID              *Devices;
  SIO_DEV                             *SioDev;
  UINTN                               Count;
  UINTN                               Index;
  ACPI_HID_DEVICE_PATH                *AcpiNode;
  BOOLEAN                             *HasCreated;
  BOOLEAN                             *RequestCreate;
  LIST_ENTRY                          *Node;

  HasCreated    = NULL;
  RequestCreate = NULL;
  //
  // Get the ISA bridge's Device Path
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &ParentDevicePath,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status) && Status != EFI_ALREADY_STARTED) {
    return Status;
  }

  //
  // Get Pci IO
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );

  if (EFI_ERROR (Status) && Status != EFI_ALREADY_STARTED) {
    gBS->CloseProtocol (
           Controller,
           &gEfiDevicePathProtocolGuid,
           This->DriverBindingHandle,
           Controller
           );
    return Status;
  }

  if ((RemainingDevicePath != NULL) && IsDevicePathEnd (RemainingDevicePath)) {
    return EFI_SUCCESS;
  }

  DeviceGetList (&Devices, &Count);
  if (Devices == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit_Start;
  }
  HasCreated    = AllocatePool (sizeof (BOOLEAN) * Count);
  ASSERT (HasCreated != NULL);
  if (HasCreated == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit_Start;
  }
  RequestCreate = AllocatePool (sizeof (BOOLEAN) * Count);
  ASSERT (RequestCreate != NULL);
  if (RequestCreate == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit_Start;
  }

  //
  // Assume no children has been created.
  // Assume the SIO interface hasn't been initialized.
  //
  ZeroMem (HasCreated, sizeof (BOOLEAN) * Count);

  if (Status == EFI_ALREADY_STARTED) {
    for (Node = GetFirstNode (&mSioDevPool);
         !IsNull (&mSioDevPool, Node);
         Node = GetNextNode (&mSioDevPool, Node)
        ) {
      SioDev = CR (Node, SIO_DEV, Link, SIO_DEV_SIGNATURE);
      Status = gBS->HandleProtocol (
                      SioDev->PciHandle,
                      &gEfiDevicePathProtocolGuid,
                      (VOID **) &DevicePath
                      );
      ASSERT_EFI_ERROR (Status);

      //
      // See if they are under the same PCI to ISA Bridge
      //
      if (CompareMem (DevicePath, ParentDevicePath, GetDevicePathSize (DevicePath)) == 0) {
        for (Index = 0; Index < Count; Index++) {
          if (CompareMem (&SioDev->Device, &Devices[Index], sizeof (EFI_SIO_ACPI_DEVICE_ID)) == 0) {
            HasCreated[Index] = TRUE;
            break;
          }
        }
      }
    }
  }

  AcpiNode = (ACPI_HID_DEVICE_PATH *) RemainingDevicePath;
  for (Index = 0; Index < Count; Index++) {
    if ((AcpiNode == NULL) ||
        ((AcpiNode->HID == Devices[Index].HID) && (AcpiNode->UID == Devices[Index].UID))
       ) {
      RequestCreate[Index] = TRUE;
    } else {
      RequestCreate[Index] = FALSE;
    }
  }

  for (Index = 0; Index < Count; Index++) {
    if (RequestCreate[Index] && !HasCreated[Index]) {
      SioCreateDevice (Controller, &Devices[Index], ParentDevicePath, PciIo);
    }
  }
Exit_Start:
  if (Devices != NULL) {
    FreePool (Devices);
  }
  if (HasCreated != NULL) {
    FreePool (HasCreated);
  }
  if (RequestCreate != NULL) {
    FreePool (RequestCreate);
  }

  return Status;
}


/**
  Stop this driver on ControllerHandle.

  @param[in]  This              Protocol instance pointer.
  @param[in]  Controller        Handle of device to stop driver on
  @param[in]  NumberOfChildren  Number of Handles in ChildHandleBuffer. If number of
                                children is zero stop the entire bus driver.
  @param[in]  ChildHandleBuffer List of Child Handles to Stop.

  @retval     EFI_SUCCESS       This driver is removed ControllerHandle
  @retval     other             This driver was not removed from this device
**/
EFI_STATUS
EFIAPI
SioDriverStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN  EFI_HANDLE                     Controller,
  IN  UINTN                          NumberOfChildren,
  IN  EFI_HANDLE                     *ChildHandleBuffer
  )
{
  EFI_STATUS  Status;
  UINTN       Index;
  BOOLEAN     AllChildrenStopped;

  if (NumberOfChildren == 0) {
    gBS->CloseProtocol (
           Controller,
           &gEfiDevicePathProtocolGuid,
           This->DriverBindingHandle,
           Controller
           );
    gBS->CloseProtocol (
           Controller,
           &gEfiPciIoProtocolGuid,
           This->DriverBindingHandle,
           Controller
           );
    return EFI_SUCCESS;
  }

  AllChildrenStopped = TRUE;
  for (Index = 0; Index < NumberOfChildren; Index++) {
    Status = SioDestroyDevice (ChildHandleBuffer[Index]);
    if (EFI_ERROR (Status)) {
      AllChildrenStopped = FALSE;
    }
  }

  if (AllChildrenStopped) {
    return EFI_SUCCESS;
  } else {
    return EFI_DEVICE_ERROR;
  }
}
