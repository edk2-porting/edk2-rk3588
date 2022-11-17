/** @file
  Produces the SIO Protocols for ISA devices integrated in the Platform
  Controller Unit (PCU).

  Copyright (c) 2004 - 2019, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "PcuSio.h"

//
//  PCU SIO Driver Global Variables
//
EFI_DRIVER_BINDING_PROTOCOL gPcuSioDriverBinding = {
  PcuSioDriverBindingSupported,
  PcuSioDriverBindingStart,
  PcuSioDriverBindingStop,
  0x10,
  NULL,
  NULL
};

//
// Super I/O Protocol interfaces
//
EFI_SIO_PROTOCOL mSioInterface = {
  SioRegisterAccess,
  SioGetResources,
  SioSetResources,
  SioPossibleResources,
  SioModify
};

//
// COM 1 UART Controller
//
GLOBAL_REMOVE_IF_UNREFERENCED
SIO_RESOURCES_IO mCom1Resources = {
  { { ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR }, 0x3F8, 8 },
  { ACPI_END_TAG_DESCRIPTOR,                    0        }
};

//
// Table of SIO Controllers
//
GLOBAL_REMOVE_IF_UNREFERENCED
SIO_DEVICE_INFO mDevicesInfo[] = {
  {
    EISA_PNP_ID (0x501),
    0,
    { (ACPI_SMALL_RESOURCE_HEADER *) &mCom1Resources }
  }
};

//
// ACPI Device Path Node template
//
GLOBAL_REMOVE_IF_UNREFERENCED
ACPI_HID_DEVICE_PATH mAcpiDeviceNodeTemplate = {
  {        // Header
    ACPI_DEVICE_PATH,
    ACPI_DP,
    {
      (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)),
      (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8)
    }
  },
  0x0,     // HID
  0x0      // UID
};

/**
  Provides a low level access to the registers for the Super I/O.

  @param[in]     This           Indicates a pointer to the calling context.
  @param[in]     Write          Specifies the type of the register operation.
                                If this parameter is TRUE, Value is interpreted
                                as an input parameter and the operation is a
                                register write. If this parameter is FALSE,
                                Value is interpreted as an output parameter and
                                the operation is a register read.
  @param[in]     ExitCfgMode    Exit Configuration Mode Indicator. If this
                                parameter is set to TRUE, the Super I/O driver
                                will turn off configuration mode of the Super
                                I/O prior to returning from this function. If
                                this parameter is set to FALSE, the Super I/O
                                driver will leave Super I/O in the
                                configuration mode. The Super I/O driver must
                                track the current state of the Super I/O and
                                enable the configuration mode of Super I/O if
                                necessary prior to register access.
  @param[in]     Register       Register number.
  @param[in,out] Value          If Write is TRUE, Value is a pointer to the
                                buffer containing the byte of data to be
                                written to the Super I/O register. If Write is
                                FALSE, Value is a pointer to the destination
                                buffer for the byte of data to be read from the
                                Super I/O register.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_INVALID_PARAMETER The Value is NULL.
  @retval EFI_INVALID_PARAMETER Invalid Register number.

**/
EFI_STATUS
EFIAPI
SioRegisterAccess (
  IN CONST EFI_SIO_PROTOCOL    *This,
  IN       BOOLEAN             Write,
  IN       BOOLEAN             ExitCfgMode,
  IN       UINT8               Register,
  IN OUT   UINT8               *Value
  )
{
  return EFI_SUCCESS;
}

/**
  Provides an interface to get a list of the current resources consumed by the
  device in the ACPI Resource Descriptor format.

  GetResources() returns a list of resources currently consumed by the device.
  The ResourceList is a pointer to the buffer containing resource descriptors
  for the device. The descriptors are in the format of Small or Large ACPI
  resource descriptor as defined by ACPI specification (2.0 & 3.0). The buffer
  of resource descriptors is terminated with the 'End tag' resource descriptor.

  @param[in]  This              Indicates a pointer to the calling context.
  @param[out] ResourceList      A pointer to an ACPI resource descriptor list
                                that defines the current resources used by the
                                device.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_INVALID_PARAMETER ResourceList is NULL.

**/
EFI_STATUS
EFIAPI
SioGetResources (
  IN CONST EFI_SIO_PROTOCOL            *This,
  OUT      ACPI_RESOURCE_HEADER_PTR    *ResourceList
  )
{
  SIO_DEV  *SioDevice;

  if (ResourceList == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SioDevice = SIO_DEV_FROM_SIO (This);
  if (SioDevice->DeviceIndex < ARRAY_SIZE (mDevicesInfo)) {
    *ResourceList = mDevicesInfo[SioDevice->DeviceIndex].Resources;
  }

  return EFI_SUCCESS;
}

/**
  Sets the resources for the device.

  @param[in] This               Indicates a pointer to the calling context.
  @param[in] ResourceList       Pointer to the ACPI resource descriptor list.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_INVALID_PARAMETER ResourceList is invalid.
  @retval EFI_ACCESS_DENIED     Some of the resources in ResourceList are in
                                use.

**/
EFI_STATUS
EFIAPI
SioSetResources (
  IN CONST EFI_SIO_PROTOCOL            *This,
  IN       ACPI_RESOURCE_HEADER_PTR    ResourceList
  )
{
  return EFI_SUCCESS;
}

/**
  Provides a collection of resource descriptor lists. Each resource descriptor
  list in the collection defines a combination of resources that can
  potentially be used by the device.

  @param[in]  This                  Indicates a pointer to the calling context.
  @param[out] ResourceCollection    Collection of the resource descriptor
                                    lists.

  @retval EFI_SUCCESS               The operation completed successfully.
  @retval EFI_INVALID_PARAMETER     ResourceCollection is NULL.

**/
EFI_STATUS
EFIAPI
SioPossibleResources (
  IN CONST EFI_SIO_PROTOCOL            *This,
  OUT      ACPI_RESOURCE_HEADER_PTR    *ResourceCollection
  )
{
  return EFI_SUCCESS;
}

/**
  Provides an interface for a table based programming of the Super I/O
  registers.

  The Modify() function provides an interface for table based programming of
  the Super I/O registers. This function can be used to perform programming of
  multiple Super I/O registers with a single function call. For each table
  entry, the Register is read, its content is bitwise ANDed with AndMask, and
  then ORed with OrMask before being written back to the Register. The Super
  I/O driver must track the current state of the Super I/O and enable the
  configuration mode of Super I/O if necessary prior to table processing. Once
  the table is processed, the Super I/O device has to be returned to the
  original state.

  @param[in] This               Indicates a pointer to the calling context.
  @param[in] Command            A pointer to an array of NumberOfCommands
                                EFI_SIO_REGISTER_MODIFY structures. Each
                                structure specifies a single Super I/O register
                                modify operation.
  @param[in] NumberOfCommands   Number of elements in the Command array.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_INVALID_PARAMETER Command is NULL.

**/
EFI_STATUS
EFIAPI
SioModify (
  IN CONST EFI_SIO_PROTOCOL           *This,
  IN CONST EFI_SIO_REGISTER_MODIFY    *Command,
  IN       UINTN                      NumberOfCommands
  )
{
  return EFI_SUCCESS;
}

/**
  Create the child device with a given device index.

  @param[in] This              The EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in] Controller        The handle of ISA bus controller.
  @param[in] PciIo             The pointer to the PCI protocol.
  @param[in] ParentDevicePath  Device path of the ISA bus controller.
  @param[in] DeviceIndex       Index of the device supported by this driver.

  @retval EFI_SUCCESS          The child device has been created successfully.
  @retval Others               Error occurred during the child device creation.

**/
EFI_STATUS
SioCreateChildDevice (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN EFI_PCI_IO_PROTOCOL          *PciIo,
  IN EFI_DEVICE_PATH_PROTOCOL     *ParentDevicePath,
  IN UINT32                       DeviceIndex
  )
{
  EFI_STATUS  Status;
  SIO_DEV     *SioDevice;

  //
  // Initialize the SIO_DEV structure
  //
  SioDevice = AllocateZeroPool (sizeof (SIO_DEV));
  if (SioDevice == NULL) {
    DEBUG ((DEBUG_ERROR, "SioCreateChildDevice(): Can not allocate SIO Device structure\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  SioDevice->Signature   = SIO_DEV_SIGNATURE;
  SioDevice->Handle      = NULL;
  SioDevice->PciIo       = PciIo;
  CopyMem (&SioDevice->Sio, &mSioInterface, sizeof (EFI_SIO_PROTOCOL));
  SioDevice->DeviceIndex = DeviceIndex;

  //
  // Construct the child device path
  //
  mAcpiDeviceNodeTemplate.HID = mDevicesInfo[DeviceIndex].Hid;
  mAcpiDeviceNodeTemplate.UID = mDevicesInfo[DeviceIndex].Uid;
  SioDevice->DevicePath = AppendDevicePathNode (
                            ParentDevicePath,
                            (EFI_DEVICE_PATH_PROTOCOL *) &mAcpiDeviceNodeTemplate
                            );
  if (SioDevice->DevicePath == NULL) {
    DEBUG ((DEBUG_ERROR, "SioCreateChildDevice(): Can not allocate SIO Device path\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }

  //
  // Create a child handle and install Device Path and Super I/O protocols
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &SioDevice->Handle,
                  &gEfiDevicePathProtocolGuid,
                  SioDevice->DevicePath,
                  &gEfiSioProtocolGuid,
                  &SioDevice->Sio,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SioCreateChildDevice(): Can not install child protocols %r\n", Status));
    goto Done;
  }

  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo,
                  This->DriverBindingHandle,
                  SioDevice->Handle,
                  EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SioCreateChildDevice(): Can not open parent PCI I/O Protocol by child controller %r\n", Status));
    gBS->UninstallMultipleProtocolInterfaces (
           SioDevice->Handle,
           &gEfiDevicePathProtocolGuid,
           SioDevice->DevicePath,
           &gEfiSioProtocolGuid,
           &SioDevice->Sio,
           NULL
           );
  }

Done:
  if (EFI_ERROR (Status)) {
    if (SioDevice->DevicePath != NULL) {
      FreePool (SioDevice->DevicePath);
    }
    FreePool (SioDevice);
  }

  return Status;
}

/**
  Create all the ISA child devices on the ISA bus controller (PCI to ISA
  bridge).

  @param[in] This              The EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in] Controller        The handle of ISA bus controller.
  @param[in] PciIo             The pointer to the PCI protocol.
  @param[in] ParentDevicePath  Device path of the ISA bus controller.

  @retval The number of child device that is successfully created.

**/
UINT32
SioCreateAllChildDevices (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN EFI_PCI_IO_PROTOCOL          *PciIo,
  IN EFI_DEVICE_PATH_PROTOCOL     *ParentDevicePath
  )
{
  UINT32        Index;
  UINT32        ChildDeviceNumber;
  EFI_STATUS    Status;

  ChildDeviceNumber = 0;

  for (Index = 0; Index < ARRAY_SIZE (mDevicesInfo); Index++) {
    Status = SioCreateChildDevice (
               This,
               Controller,
               PciIo,
               ParentDevicePath,
               Index
               );
    if (!EFI_ERROR (Status)) {
      ChildDeviceNumber++;
    }
  }

  return ChildDeviceNumber;
}

/**
  Tests to see if this driver supports a given controller. If a child device is
  provided, it further tests to see if this driver supports creating a handle
  for the specified child device.

  This function checks to see if the driver specified by This supports the
  device specified by ControllerHandle. Drivers will typically use the device
  path attached to ControllerHandle and/or the services from the bus I/O
  abstraction attached to ControllerHandle to determine if the driver supports
  ControllerHandle. This function may be called many times during platform
  initialization. In order to reduce boot times, the tests performed by this
  function must be very small, and take as little time as possible to execute.
  This function must not change the state of any hardware devices, and this
  function must be aware that the device specified by ControllerHandle may
  already be managed by the same driver or a different driver. This function
  must match its calls to AllocatePages() with FreePages(), AllocatePool() with
  FreePool(), and OpenProtocol() with CloseProtocol(). Since ControllerHandle
  may have been previously started by the same driver, if a protocol is already
  in the opened state, then it must not be closed with CloseProtocol(). This is
  required to guarantee the state of ControllerHandle is not modified by this
  function.

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL
                                   instance.
  @param[in]  ControllerHandle     The handle of the controller to test. This
                                   handle must support a protocol interface
                                   that supplies an I/O abstraction to the
                                   driver.
  @param[in]  RemainingDevicePath  A pointer to the remaining portion of a
                                   device path.  This parameter is ignored by
                                   device drivers, and is optional for bus
                                   drivers. For bus drivers, if this parameter
                                   is not NULL, then the bus driver must
                                   determine if the bus controller specified by
                                   ControllerHandle and the child controller
                                   specified by RemainingDevicePath are both
                                   supported by this bus driver.

  @retval EFI_SUCCESS              The device specified by ControllerHandle and
                                   RemainingDevicePath is supported by the
                                   driver specified by This.
  @retval EFI_ALREADY_STARTED      The device specified by ControllerHandle and
                                   RemainingDevicePath is already being managed
                                   by the driver specified by This.
  @retval EFI_ACCESS_DENIED        The device specified by ControllerHandle and
                                   RemainingDevicePath is already being managed
                                   by a different driver or an application that
                                   requires exclusive access.
  @retval EFI_UNSUPPORTED          The device specified by ControllerHandle and
                                   RemainingDevicePath is not supported by the
                                   driver specified by This.

**/
EFI_STATUS
EFIAPI
PcuSioDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  )
{
  EFI_STATUS           Status;
  EFI_PCI_IO_PROTOCOL  *PciIo;
  PCI_TYPE00           Pci;

  //
  // Get PciIo protocol instance
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  (VOID **)&PciIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = PciIo->Pci.Read (
                        PciIo,
                        EfiPciIoWidthUint32,
                        0,
                        sizeof(Pci) / sizeof(UINT32),
                        &Pci
                        );

  if (!EFI_ERROR (Status)) {
    Status = EFI_UNSUPPORTED;
    if ((Pci.Hdr.Command & 0x03) == 0x03) {
      if (Pci.Hdr.ClassCode[2] == PCI_CLASS_BRIDGE) {
        //
        // See if this is a standard PCI to ISA Bridge from the Base Code
        // and Class Code
        //
        if (Pci.Hdr.ClassCode[1] == PCI_CLASS_BRIDGE_ISA) {
          Status = EFI_SUCCESS;
        }
        //
        // See if this is an Intel PCI to ISA bridge in Positive Decode Mode
        //
        if (Pci.Hdr.ClassCode[1] == PCI_CLASS_BRIDGE_ISA_PDECODE &&
            Pci.Hdr.VendorId == 0x8086 && Pci.Hdr.DeviceId == 0x7110) {
          Status = EFI_SUCCESS;
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

  return Status;
}

/**
  Starts a device controller or a bus controller.

  The Start() function is designed to be invoked from the EFI boot service
  ConnectController(). As a result, much of the error checking on the
  parameters to Start() has been moved into this common boot service. It is
  legal to call Start() from other locations, but the following calling
  restrictions must be followed or the system behavior will not be
  deterministic.
  1. ControllerHandle must be a valid EFI_HANDLE.
  2. If RemainingDevicePath is not NULL, then it must be a pointer to a
     naturally aligned EFI_DEVICE_PATH_PROTOCOL.
  3. Prior to calling Start(), the Supported() function for the driver
     specified by This must have been called with the same calling parameters,
     and Supported() must have returned EFI_SUCCESS.

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL
                                   instance.
  @param[in]  ControllerHandle     The handle of the controller to start. This
                                   handle must support a protocol interface
                                   that supplies an I/O abstraction to the
                                   driver.
  @param[in]  RemainingDevicePath  A pointer to the remaining portion of a
                                   device path.  This parameter is ignored by
                                   device drivers, and is optional for bus
                                   drivers. For a bus driver, if this parameter
                                   is NULL, then handles for all the children
                                   of Controller are created by this driver. If
                                   this parameter is not NULL and the first
                                   Device Path Node is not the End of Device
                                   Path Node, then only the handle for the
                                   child device specified by the first Device
                                   Path Node of RemainingDevicePath is created
                                   by this driver. If the first Device Path
                                   Node of RemainingDevicePath is the End of
                                   Device Path Node, no child handle is created
                                   by this driver.

  @retval EFI_SUCCESS              The device was started.
  @retval EFI_DEVICE_ERROR         The device could not be started due to a
                                   device error.
  @retval EFI_OUT_OF_RESOURCES     The request could not be completed due to a
                                   lack of resources.
  @retval Others                   The driver failed to start the device.

**/
EFI_STATUS
EFIAPI
PcuSioDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  )
{
  EFI_STATUS                     Status;
  EFI_PCI_IO_PROTOCOL            *PciIo;
  EFI_DEVICE_PATH_PROTOCOL       *ParentDevicePath;
  UINT64                         Supports;
  UINT64                         OriginalAttributes;
  UINT64                         Attributes;
  BOOLEAN                        Enabled;
  SIO_BUS_DRIVER_PRIVATE_DATA    *Private;
  UINT32                         ChildDeviceNumber;

  Enabled            = FALSE;
  Supports           = 0;
  OriginalAttributes = 0;
  Private            = NULL;

  //
  // Open the PCI I/O Protocol Interface
  //
  PciIo = NULL;
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  (VOID**) &PciIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PcuSioDriverBindingStart(): No PCI I/O Protocol %r\n", Status));
    return Status;
  }

  //
  // Open Device Path Protocol
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &ParentDevicePath,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PcuSioDriverBindingStart(): No Device Path Protocol %r\n", Status));
    gBS->CloseProtocol (
           Controller,
           &gEfiPciIoProtocolGuid,
           This->DriverBindingHandle,
           Controller
           );
    return Status;
  }

  //
  // Get supported PCI attributes
  //
  Status = PciIo->Attributes (
                    PciIo,
                    EfiPciIoAttributeOperationSupported,
                    0,
                    &Supports
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PcuSioDriverBindingStart(): Error reading PCI I/O Supported Attributes %r\n", Status));
    goto Done;
  }
  Supports &= (UINT64) (EFI_PCI_IO_ATTRIBUTE_ISA_IO |
                        EFI_PCI_IO_ATTRIBUTE_ISA_IO_16);

  Status = PciIo->Attributes (
                    PciIo,
                    EfiPciIoAttributeOperationGet,
                    0,
                    &OriginalAttributes
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PcuSioDriverBindingStart(): Error reading PCI I/O Attributes %r\n", Status));
    goto Done;
  }

  Attributes = EFI_PCI_DEVICE_ENABLE |
               Supports |
               EFI_PCI_IO_ATTRIBUTE_ISA_MOTHERBOARD_IO;

  Status = PciIo->Attributes (
                    PciIo,
                    EfiPciIoAttributeOperationEnable,
                    Attributes,
                    NULL
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PcuSioDriverBindingStart(): Error enabling PCI I/O  Attributes %r\n", Status));
    goto Done;
  }

  Enabled = TRUE;

  //
  // Store the OriginalAttributes for the restore in BindingStop()
  //
  Private = AllocateZeroPool (sizeof (SIO_BUS_DRIVER_PRIVATE_DATA));
  if (Private == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "PcuSioDriverBindingStart(): Error allocating SIO private data structure %r\n", Status));
    goto Done;
  }
  Private->PciIo              = PciIo;
  Private->OriginalAttributes = OriginalAttributes;

  Status = gBS->InstallProtocolInterface (
                  &Controller,
                  &gEfiCallerIdGuid,
                  EFI_NATIVE_INTERFACE,
                  Private
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PcuSioDriverBindingStart(): Error installing gEfiCallerIdGuid %r\n", Status));
    goto Done;
  }

  //
  // Report status code for the start of general controller initialization
  //
  REPORT_STATUS_CODE_WITH_DEVICE_PATH (
    EFI_PROGRESS_CODE,
    (EFI_IO_BUS_LPC | EFI_IOB_PC_INIT),
    ParentDevicePath
    );

  //
  // Report status code for the start of enabling devices on the bus
  //
  REPORT_STATUS_CODE_WITH_DEVICE_PATH (
    EFI_PROGRESS_CODE,
    (EFI_IO_BUS_LPC | EFI_IOB_PC_ENABLE),
    ParentDevicePath
    );

  //
  // Create all the children upon the first entrance
  //
  ChildDeviceNumber = SioCreateAllChildDevices (
                        This,
                        Controller,
                        PciIo,
                        ParentDevicePath
                        );
  if (ChildDeviceNumber == 0) {
    Status = EFI_DEVICE_ERROR;
    DEBUG ((DEBUG_ERROR, "PcuSioDriverBindingStart(): Error creating child SIO devices %r\n", Status));
    goto Done;
  }

Done:
  if (EFI_ERROR (Status)) {
    if (PciIo != NULL && Enabled) {
      PciIo->Attributes (
               PciIo,
               EfiPciIoAttributeOperationSet,
               OriginalAttributes,
               NULL
               );
    }

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

    if (Private != NULL) {
      gBS->UninstallMultipleProtocolInterfaces (
             Controller,
             &gEfiCallerIdGuid,
             Private,
             NULL
             );
      FreePool (Private);
    }
  }

  return Status;
}

/**
  Stops a device controller or a bus controller.

  The Stop() function is designed to be invoked from the EFI boot service
  DisconnectController(). As a result, much of the error checking on the
  parameters to Stop() has been moved into this common boot service. It is
  legal to call Stop() from other locations, but the following calling
  restrictions must be followed or the system behavior will not be
  deterministic.
  1. ControllerHandle must be a valid EFI_HANDLE that was used on a previous
     call to this same driver's Start() function.
  2. The first NumberOfChildren handles of ChildHandleBuffer must all be a
     valid EFI_HANDLE. In addition, all of these handles must have been created
     in this driver's Start() function, and the Start() function must have
     called OpenProtocol() on ControllerHandle with an Attribute of
     EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER.

  @param[in]  This               A pointer to the EFI_DRIVER_BINDING_PROTOCOL
                                 instance.
  @param[in]  ControllerHandle   A handle to the device being stopped. The
                                 handle must support a bus specific I/O
                                 protocol for the driver to use to stop the
                                 device.
  @param[in]  NumberOfChildren   The number of child device handles in
                                 ChildHandleBuffer.
  @param[in]  ChildHandleBuffer  An array of child handles to be freed. May be
                                 NULL if NumberOfChildren is 0.

  @retval EFI_SUCCESS            The device was stopped.
  @retval EFI_DEVICE_ERROR       The device could not be stopped due to a
                                 device error.

**/
EFI_STATUS
EFIAPI
PcuSioDriverBindingStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN  EFI_HANDLE                     Controller,
  IN  UINTN                          NumberOfChildren,
  IN  EFI_HANDLE                     *ChildHandleBuffer
  )
{
  EFI_STATUS                     Status;
  SIO_BUS_DRIVER_PRIVATE_DATA    *Private;
  UINTN                          Index;
  BOOLEAN                        AllChildrenStopped;
  EFI_SIO_PROTOCOL               *Sio;
  SIO_DEV                        *SioDevice;
  EFI_PCI_IO_PROTOCOL            *PciIo;

  if (NumberOfChildren == 0) {
    //
    // Restore PCI attributes
    //
    Status = gBS->OpenProtocol (
                    Controller,
                    &gEfiCallerIdGuid,
                    (VOID **) &Private,
                    This->DriverBindingHandle,
                    Controller,
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = Private->PciIo->Attributes (
                               Private->PciIo,
                               EfiPciIoAttributeOperationSet,
                               Private->OriginalAttributes,
                               NULL
                               );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    gBS->UninstallProtocolInterface (
          Controller,
          &gEfiCallerIdGuid,
          Private
          );
    FreePool (Private);

    //
    // Close the bus driver
    //
    Status = gBS->CloseProtocol (
                    Controller,
                    &gEfiDevicePathProtocolGuid,
                    This->DriverBindingHandle,
                    Controller
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = gBS->CloseProtocol (
                    Controller,
                    &gEfiPciIoProtocolGuid,
                    This->DriverBindingHandle,
                    Controller
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    return EFI_SUCCESS;
  }

  //
  // Stop all the children
  //
  AllChildrenStopped = TRUE;

  for (Index = 0; Index < NumberOfChildren; Index++) {
    Status = gBS->OpenProtocol (
                    ChildHandleBuffer[Index],
                    &gEfiSioProtocolGuid,
                    (VOID **) &Sio,
                    This->DriverBindingHandle,
                    Controller,
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL
                    );
    if (!EFI_ERROR (Status)) {
      SioDevice = SIO_DEV_FROM_SIO (Sio);

      //
      // Close the child handle
      //
      Status = gBS->CloseProtocol (
                      Controller,
                      &gEfiPciIoProtocolGuid,
                      This->DriverBindingHandle,
                      ChildHandleBuffer[Index]
                      );
      Status = gBS->UninstallMultipleProtocolInterfaces (
                      ChildHandleBuffer[Index],
                      &gEfiDevicePathProtocolGuid,
                      SioDevice->DevicePath,
                      &gEfiSioProtocolGuid,
                      &SioDevice->Sio,
                      NULL
                      );

      if (!EFI_ERROR (Status)) {
        FreePool (SioDevice->DevicePath);
        FreePool (SioDevice);
      } else {
        //
        // Re-open PCI IO Protocol on behalf of the child device
        // because of failure of destroying the child device handle
        //
        gBS->OpenProtocol (
               Controller,
               &gEfiPciIoProtocolGuid,
               (VOID **) &PciIo,
               This->DriverBindingHandle,
               ChildHandleBuffer[Index],
               EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER
               );
      }
    }

    if (EFI_ERROR (Status)) {
      AllChildrenStopped = FALSE;
    }
  }

  if (!AllChildrenStopped) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  The entry point for the PCU SIO driver.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
PcuSioDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return EfiLibInstallDriverBinding (
            ImageHandle, 
            SystemTable, 
            &gPcuSioDriverBinding, 
            ImageHandle
            );
}
