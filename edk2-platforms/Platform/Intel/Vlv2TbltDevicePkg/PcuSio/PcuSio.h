/** @file
  Produces the SIO Protocols for ISA devices integrated in the Platform
  Controller Unit (PCU).

  Copyright (c) 2004 - 2019, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __PCU_SIO_H__
#define __PCU_SIO_H__

#include <Uefi.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/PciIo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/SuperIo.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiLib.h>
#include <IndustryStandard/Pci.h>

#pragma pack(1)

typedef struct {
  EFI_ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR  Io;
  EFI_ACPI_END_TAG_DESCRIPTOR                 End;
} SIO_RESOURCES_IO;

#pragma pack()

typedef struct {
  UINT32                    Hid;
  UINT32                    Uid;
  ACPI_RESOURCE_HEADER_PTR  Resources;
} SIO_DEVICE_INFO;

//
// SIO Bus driver private data structure
//
typedef struct {
  EFI_PCI_IO_PROTOCOL  *PciIo;
  UINT64               OriginalAttributes;
} SIO_BUS_DRIVER_PRIVATE_DATA;

//
// SIO device private data structure
//
typedef struct {
  UINT32                    Signature;
  EFI_HANDLE                Handle;
  EFI_PCI_IO_PROTOCOL       *PciIo;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  EFI_SIO_PROTOCOL          Sio;
  UINT32                    DeviceIndex;
} SIO_DEV;
#define SIO_DEV_SIGNATURE      SIGNATURE_32 ('S', 'I', 'O', 'D')
#define SIO_DEV_FROM_SIO(a)    CR (a, SIO_DEV, Sio, SIO_DEV_SIGNATURE)

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
  );

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
  @retval Others                   The driver failded to start the device.

**/
EFI_STATUS
EFIAPI
PcuSioDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

#endif
