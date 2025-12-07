/** @file
Provides the definition of Usb Hc Protocol and OHCI controller
private data structure.

Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>
Copyright (c) 2013-2016 Intel Corporation.

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _OHCI_H
#define _OHCI_H

#include <Uefi.h>

#include <Protocol/Usb2HostController.h>
#include <Library/DmaLib.h>

#include <Guid/EventGroup.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>

#include <Protocol/OhciDeviceProtocol.h>

typedef struct _USB_OHCI_HC_DEV USB_OHCI_HC_DEV;

#include "UsbHcMem.h"
#include "OhciReg.h"
#include "OhciSched.h"
#include "OhciUrb.h"
#include "Descriptor.h"
#include "OhciDebug.h"
#include "ComponentName.h"

extern EFI_DRIVER_BINDING_PROTOCOL   gOhciDriverBinding;
extern EFI_COMPONENT_NAME_PROTOCOL   gOhciComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL  gOhciComponentName2;

#define USB_OHCI_HC_DEV_SIGNATURE  SIGNATURE_32('o','h','c','i')

typedef struct _HCCA_MEMORY_BLOCK {
  UINT32    HccaInterruptTable[32];                    // 32-bit Physical Address to ED_DESCRIPTOR
  UINT16    HccaFrameNumber;
  UINT16    HccaPad;
  UINT32    HccaDoneHead;                              // 32-bit Physical Address to TD_DESCRIPTOR
  UINT8     Reserved[116];
} HCCA_MEMORY_BLOCK;

typedef struct {
  VENDOR_DEVICE_PATH          Guid;
  UINTN                       Instance;
  EFI_DEVICE_PATH_PROTOCOL    End;
} OHCI_DEVICE_PATH;

struct _USB_OHCI_HC_DEV {
  UINTN                       Signature;
  EFI_USB2_HC_PROTOCOL        Usb2Hc;
  UINT32                      UsbHcBaseAddress;
  HCCA_MEMORY_BLOCK           *HccaMemoryBlock;
  VOID                        *HccaMemoryBuf;
  VOID                        *HccaMemoryMapping;
  UINTN                       HccaMemoryPages;
  ED_DESCRIPTOR               *IntervalList[6][32];
  INTERRUPT_CONTEXT_ENTRY     *InterruptContextList;
  VOID                        *MemPool;

  UINT32                      ToggleFlag;

  EFI_EVENT                   HouseKeeperTimer;
  //
  // ExitBootServicesEvent is used to stop the OHC DMA operation
  // after exit boot service.
  //
  EFI_EVENT                   ExitBootServiceEvent;

  EFI_UNICODE_STRING_TABLE    *ControllerNameTable;

  OHCI_DEVICE_PROTOCOL        *Protocol;
};

#define USB_OHCI_HC_DEV_FROM_THIS(a)  CR(a, USB_OHCI_HC_DEV, Usb2Hc, USB_OHCI_HC_DEV_SIGNATURE)

//
// Func List
//

/**
  Retrieves the Host Controller capabilities.

  @param  This           A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  MaxSpeed       Host controller data transfer speed.
  @param  PortNumber     Number of the root hub ports.
  @param  Is64BitCapable TRUE if controller supports 64-bit memory addressing,
                         FALSE otherwise.

  @retval EFI_SUCCESS           The host controller capabilities were retrieved successfully.
  @retval EFI_INVALID_PARAMETER One of the input args was NULL.
  @retval EFI_DEVICE_ERROR      An error was encountered while attempting to
                                retrieve the capabilities.

**/
EFI_STATUS
EFIAPI
OhciGetCapability (
  IN  EFI_USB2_HC_PROTOCOL  *This,
  OUT UINT8                 *MaxSpeed,
  OUT UINT8                 *PortNumber,
  OUT UINT8                 *Is64BitCapable
  );

/**
  Provides software reset for the USB host controller.

  @param  This       A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  Attributes A bit mask of the reset operation to perform.

  @retval EFI_SUCCESS           The reset operation succeeded.
  @retval EFI_INVALID_PARAMETER Attributes is not valid.
  @retval EFI_UNSUPPORTED       The type of reset specified by Attributes is not currently
                                supported by the host controller hardware.
  @retval EFI_ACCESS_DENIED     Reset operation is rejected due to the debug port being configured
                                and active; only EFI_USB_HC_RESET_GLOBAL_WITH_DEBUG or
                                EFI_USB_HC_RESET_HOST_WITH_DEBUG reset Attributes can be used to
                                perform reset operation for this host controller.
  @retval EFI_DEVICE_ERROR      An error was encountered while attempting to
                                retrieve the capabilities.

**/
EFI_STATUS
EFIAPI
OhciReset (
  IN EFI_USB2_HC_PROTOCOL  *This,
  IN UINT16                Attributes
  );

/**
  Retrieves current state of the USB host controller.

  @param  This  A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  State A pointer to the EFI_USB_HC_STATE data structure that
                indicates current state of the USB host controller.

  @retval EFI_SUCCESS           The state information of the host controller was returned in State.
  @retval EFI_INVALID_PARAMETER State is NULL.
  @retval EFI_DEVICE_ERROR      An error was encountered while attempting to retrieve the
                                host controller's current state.

**/
EFI_STATUS
EFIAPI
OhciGetState (
  IN  EFI_USB2_HC_PROTOCOL  *This,
  OUT EFI_USB_HC_STATE      *State
  );

/**
  Sets the USB host controller to a specific state.

  @param  This  A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  State Indicates the state of the host controller that will be set.

  @retval EFI_SUCCESS           The USB host controller was successfully placed in the state
                                specified by State.
  @retval EFI_INVALID_PARAMETER State is not valid.
  @retval EFI_DEVICE_ERROR      Failed to set the state specified by State due to device error.

**/
EFI_STATUS
EFIAPI
OhciSetState (
  IN EFI_USB2_HC_PROTOCOL  *This,
  IN EFI_USB_HC_STATE      State
  );

/**
  Submits control transfer to a target USB device.

  @param  This                A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  DeviceAddress       Represents the address of the target device on the USB.
  @param  DeviceSpeed         Indicates device speed.
  @param  MaximumPacketLength Indicates the maximum packet size that the default control transfer
                              endpoint is capable of sending or receiving.
  @param  Request             A pointer to the USB device request that will be sent to the USB device.
  @param  TransferDirection   Specifies the data direction for the transfer. There are three values
                              available, EfiUsbDataIn, EfiUsbDataOut and EfiUsbNoData.
  @param  Data                A pointer to the buffer of data that will be transmitted to USB device or
                              received from USB device.
  @param  DataLength          On input, indicates the size, in bytes, of the data buffer specified by Data.
                              On output, indicates the amount of data actually transferred.
  @param  TimeOut             Indicates the maximum time, in milliseconds, which the transfer is
                              allowed to complete.
  @param  Translator          A pointer to the transaction translator data.
  @param  TransferResult      A pointer to the detailed result information generated by this control
                              transfer.

  @retval EFI_SUCCESS           The control transfer was completed successfully.
  @retval EFI_INVALID_PARAMETER Some parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The control transfer could not be completed due to a lack of resources.
  @retval EFI_TIMEOUT           The control transfer failed due to timeout.
  @retval EFI_DEVICE_ERROR      The control transfer failed due to host controller or device error.
                                Caller should check TransferResult for detailed error information.

**/
EFI_STATUS
EFIAPI
OhciControlTransfer (
  IN     EFI_USB2_HC_PROTOCOL                *This,
  IN     UINT8                               DeviceAddress,
  IN     UINT8                               DeviceSpeed,
  IN     UINTN                               MaximumPacketLength,
  IN     EFI_USB_DEVICE_REQUEST              *Request,
  IN     EFI_USB_DATA_DIRECTION              TransferDirection,
  IN OUT VOID                                *Data       OPTIONAL,
  IN OUT UINTN                               *DataLength OPTIONAL,
  IN     UINTN                               TimeOut,
  IN     EFI_USB2_HC_TRANSACTION_TRANSLATOR  *Translator,
  OUT    UINT32                              *TransferResult
  );

/**
  Submits bulk transfer to a bulk endpoint of a USB device.

  @param  This                A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  DeviceAddress       Represents the address of the target device on the USB.
  @param  EndPointAddress     The combination of an endpoint number and an endpoint direction of the
                              target USB device.
  @param  DeviceSpeed         Indicates device speed.
  @param  MaximumPacketLength Indicates the maximum packet size the target endpoint is capable of
                              sending or receiving.
  @param  DataBuffersNumber   Number of data buffers prepared for the transfer.
  @param  Data                Array of pointers to the buffers of data that will be transmitted to USB
                              device or received from USB device.
  @param  DataLength          When input, indicates the size, in bytes, of the data buffers specified by
                              Data. When output, indicates the actually transferred data size.
  @param  DataToggle          A pointer to the data toggle value.
  @param  TimeOut             Indicates the maximum time, in milliseconds, which the transfer is
                              allowed to complete.
  @param  Translator          A pointer to the transaction translator data.
  @param  TransferResult      A pointer to the detailed result information of the bulk transfer.

  @retval EFI_SUCCESS           The bulk transfer was completed successfully.
  @retval EFI_INVALID_PARAMETER Some parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The bulk transfer could not be submitted due to a lack of resources.
  @retval EFI_TIMEOUT           The bulk transfer failed due to timeout.
  @retval EFI_DEVICE_ERROR      The bulk transfer failed due to host controller or device error.
                                Caller should check TransferResult for detailed error information.

**/
EFI_STATUS
EFIAPI
OhciBulkTransfer (
  IN     EFI_USB2_HC_PROTOCOL                *This,
  IN     UINT8                               DeviceAddress,
  IN     UINT8                               EndPointAddress,
  IN     UINT8                               DeviceSpeed,
  IN     UINTN                               MaximumPacketLength,
  IN     UINT8                               DataBuffersNumber,
  IN OUT VOID                                *Data[EFI_USB_MAX_BULK_BUFFER_NUM],
  IN OUT UINTN                               *DataLength,
  IN OUT UINT8                               *DataToggle,
  IN     UINTN                               TimeOut,
  IN     EFI_USB2_HC_TRANSACTION_TRANSLATOR  *Translator,
  OUT    UINT32                              *TransferResult
  );

/**

  Submits an interrupt transfer to an interrupt endpoint of a USB device.

  @param  Ohc                   Device private data
  @param  DeviceAddress         Represents the address of the target device on the USB,
                                which is assigned during USB enumeration.
  @param  EndPointAddress       The combination of an endpoint number and an endpoint
                                direction of the target USB device. Each endpoint address
                                supports data transfer in one direction except the
                                control endpoint (whose default endpoint address is 0).
                                It is the caller's responsibility to make sure that
                                the EndPointAddress represents an interrupt endpoint.
  @param  IsSlowDevice          Indicates whether the target device is slow device
                                or full-speed device.
  @param  MaximumPacketLength   Indicates the maximum packet size the target endpoint
                                is capable of sending or receiving.
  @param  IsNewTransfer         If TRUE, an asynchronous interrupt pipe is built between
                                the host and the target interrupt endpoint.
                                If FALSE, the specified asynchronous interrupt pipe
                                is canceled.
  @param  DataToggle            A pointer to the data toggle value.  On input, it is valid
                                when IsNewTransfer is TRUE, and it indicates the initial
                                data toggle value the asynchronous interrupt transfer
                                should adopt.
                                On output, it is valid when IsNewTransfer is FALSE,
                                and it is updated to indicate the data toggle value of
                                the subsequent asynchronous interrupt transfer.
  @param  PollingInterval       Indicates the interval, in milliseconds, that the
                                asynchronous interrupt transfer is polled.
                                This parameter is required when IsNewTransfer is TRUE.
  @param  UCBuffer              Uncacheable buffer
  @param  DataLength            Indicates the length of data to be received at the
                                rate specified by PollingInterval from the target
                                asynchronous interrupt endpoint.  This parameter
                                is only required when IsNewTransfer is TRUE.
  @param  CallBackFunction      The Callback function.This function is called at the
                                rate specified by PollingInterval.This parameter is
                                only required when IsNewTransfer is TRUE.
  @param  Context               The context that is passed to the CallBackFunction.
                                This is an optional parameter and may be NULL.
  @param  IsPeriodic            Periodic interrupt or not
  @param  OutputED              The correspoding ED carried out
  @param  OutputTD              The correspoding TD carried out


  @retval EFI_SUCCESS           The asynchronous interrupt transfer request has been successfully
                                submitted or canceled.
  @retval EFI_INVALID_PARAMETER Some parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.

**/
EFI_STATUS
OhciInterruptTransfer (
  IN     USB_OHCI_HC_DEV                  *Ohc,
  IN     UINT8                            DeviceAddress,
  IN     UINT8                            EndPointAddress,
  IN     BOOLEAN                          IsSlowDevice,
  IN     UINT8                            MaximumPacketLength,
  IN     BOOLEAN                          IsNewTransfer,
  IN OUT UINT8                            *DataToggle        OPTIONAL,
  IN     UINTN                            PollingInterval    OPTIONAL,
  IN     VOID                             *UCBuffer          OPTIONAL,
  IN     UINTN                            DataLength         OPTIONAL,
  IN     EFI_ASYNC_USB_TRANSFER_CALLBACK  CallBackFunction   OPTIONAL,
  IN     VOID                             *Context           OPTIONAL,
  IN     BOOLEAN                          IsPeriodic         OPTIONAL,
  OUT    ED_DESCRIPTOR                    **OutputED         OPTIONAL,
  OUT    TD_DESCRIPTOR                    **OutputTD         OPTIONAL
  );

/**
  Submits an asynchronous interrupt transfer to an interrupt endpoint of a USB device.
  Translator parameter doesn't exist in UEFI2.0 spec, but it will be updated in the following specification version.

  @param  This                A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  DeviceAddress       Represents the address of the target device on the USB.
  @param  EndPointAddress     The combination of an endpoint number and an endpoint direction of the
                              target USB device.
  @param  DeviceSpeed         Indicates device speed.
  @param  MaximumPacketLength Indicates the maximum packet size the target endpoint is capable of
                              sending or receiving.
  @param  IsNewTransfer       If TRUE, an asynchronous interrupt pipe is built between the host and the
                              target interrupt endpoint. If FALSE, the specified asynchronous interrupt
                              pipe is canceled. If TRUE, and an interrupt transfer exists for the target
                              end point, then EFI_INVALID_PARAMETER is returned.
  @param  DataToggle          A pointer to the data toggle value.
  @param  PollingInterval     Indicates the interval, in milliseconds, that the asynchronous interrupt
                              transfer is polled.
  @param  DataLength          Indicates the length of data to be received at the rate specified by
                              PollingInterval from the target asynchronous interrupt endpoint.
  @param  Translator          A pointr to the transaction translator data.
  @param  CallBackFunction    The Callback function. This function is called at the rate specified by
                              PollingInterval.
  @param  Context             The context that is passed to the CallBackFunction. This is an
                              optional parameter and may be NULL.

  @retval EFI_SUCCESS           The asynchronous interrupt transfer request has been successfully
                                submitted or canceled.
  @retval EFI_INVALID_PARAMETER Some parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.

**/
EFI_STATUS
EFIAPI
OhciAsyncInterruptTransfer (
  IN     EFI_USB2_HC_PROTOCOL                *This,
  IN     UINT8                               DeviceAddress,
  IN     UINT8                               EndPointAddress,
  IN     UINT8                               DeviceSpeed,
  IN     UINTN                               MaxiumPacketLength,
  IN     BOOLEAN                             IsNewTransfer,
  IN OUT UINT8                               *DataToggle,
  IN     UINTN                               PollingInterval  OPTIONAL,
  IN     UINTN                               DataLength       OPTIONAL,
  IN     EFI_USB2_HC_TRANSACTION_TRANSLATOR  *Translator      OPTIONAL,
  IN     EFI_ASYNC_USB_TRANSFER_CALLBACK     CallBackFunction OPTIONAL,
  IN     VOID                                *Context         OPTIONAL
  );

/**
  Submits synchronous interrupt transfer to an interrupt endpoint of a USB device.
  Translator parameter doesn't exist in UEFI2.0 spec, but it will be updated in the following specification version.

  @param  This                  A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  DeviceAddress         Represents the address of the target device on the USB.
  @param  EndPointAddress       The combination of an endpoint number and an endpoint direction of the
                                target USB device.
  @param  DeviceSpeed           Indicates device speed.
  @param  MaximumPacketLength   Indicates the maximum packet size the target endpoint is capable of
                                sending or receiving.
  @param  Data                  A pointer to the buffer of data that will be transmitted to USB device or
                                received from USB device.
  @param  DataLength            On input, the size, in bytes, of the data buffer specified by Data. On
                                output, the number of bytes transferred.
  @param  DataToggle            A pointer to the data toggle value.
  @param  TimeOut               Indicates the maximum time, in milliseconds, which the transfer is
                                allowed to complete.
  @param  Translator            A pointr to the transaction translator data.
  @param  TransferResult        A pointer to the detailed result information from the synchronous
                                interrupt transfer.

  @retval EFI_SUCCESS           The synchronous interrupt transfer was completed successfully.
  @retval EFI_INVALID_PARAMETER Some parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The synchronous interrupt transfer could not be submitted due to a lack of resources.
  @retval EFI_TIMEOUT           The synchronous interrupt transfer failed due to timeout.
  @retval EFI_DEVICE_ERROR      The synchronous interrupt transfer failed due to host controller or device error.
                                Caller should check TransferResult for detailed error information.

**/
EFI_STATUS
EFIAPI
OhciSyncInterruptTransfer (
  IN     EFI_USB2_HC_PROTOCOL                *This,
  IN     UINT8                               DeviceAddress,
  IN     UINT8                               EndPointAddress,
  IN     UINT8                               DeviceSpeed,
  IN     UINTN                               MaximumPacketLength,
  IN OUT VOID                                *Data,
  IN OUT UINTN                               *DataLength,
  IN OUT UINT8                               *DataToggle,
  IN     UINTN                               TimeOut,
  IN     EFI_USB2_HC_TRANSACTION_TRANSLATOR  *Translator,
  OUT    UINT32                              *TransferResult
  );

/**
  Submits isochronous transfer to an isochronous endpoint of a USB device.

  This function is used to submit isochronous transfer to a target endpoint of a USB device.
  The target endpoint is specified by DeviceAddressand EndpointAddress. Isochronous transfers are
  used when working with isochronous date. It provides periodic, continuous communication between
  the host and a device. Isochronous transfers can beused only by full-speed, high-speed, and
  super-speed devices.

  High-speed isochronous transfers can be performed using multiple data buffers. The number of
  buffers that are actually prepared for the transfer is specified by DataBuffersNumber. For
  full-speed isochronous transfers this value is ignored.

  Data represents a list of pointers to the data buffers. For full-speed isochronous transfers
  only the data pointed by Data[0]shall be used. For high-speed isochronous transfers and for
  the split transactions depending on DataLengththere several data buffers canbe used. For the
  high-speed isochronous transfers the total number of buffers must not exceed EFI_USB_MAX_ISO_BUFFER_NUM.

  For split transactions performed on full-speed device by high-speed host controller the total
  number of buffers is limited to EFI_USB_MAX_ISO_BUFFER_NUM1.
  If the isochronous transfer is successful, then EFI_SUCCESSis returned. The isochronous transfer
  is designed to be completed within one USB frame time, if it cannot be completed, EFI_TIMEOUT
  is returned. If an error other than timeout occurs during the USB transfer, then EFI_DEVICE_ERROR
  is returned and the detailed status code will be returned in TransferResult.

  EFI_INVALID_PARAMETERis returned if one of the following conditionsis satisfied:
    - Data is NULL.
    - DataLength is 0.
    - DeviceSpeed is not one of the supported values listed above.
    - MaximumPacketLength is invalid. MaximumPacketLength must be 1023 or less for full-speed devices,
      and 1024 or less for high-speed and super-speed devices.
    - TransferResult is NULL.

  @param  This                  A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  DeviceAddress         Represents the address of the target device on the USB.
  @param  EndPointAddress       The combination of an endpoint number and an endpoint direction of the
                                target USB device.
  @param  DeviceSpeed           Indicates device speed. The supported values are EFI_USB_SPEED_FULL,
                                EFI_USB_SPEED_HIGH, or EFI_USB_SPEED_SUPER.
  @param  MaximumPacketLength   Indicates the maximum packet size the target endpoint is capable of
                                sending or receiving.
  @param  DataBuffersNumber     Number of data buffers prepared for the transfer.
  @param  Data                  Array of pointers to the buffers of data that will be transmitted to USB
                                device or received from USB device.
  @param  DataLength            Specifies the length, in bytes, of the data to be sent to or received from
                                the USB device.
  @param  Translator            A pointer to the transaction translator data.
  @param  TransferResult        A pointer to the detailed result information of the isochronous transfer.

  @retval EFI_SUCCESS           The isochronous transfer was completed successfully.
  @retval EFI_INVALID_PARAMETER Some parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The isochronous transfer could not be submitted due to a lack of resources.
  @retval EFI_TIMEOUT           The isochronous transfer cannot be completed within the one USB frame time.
  @retval EFI_DEVICE_ERROR      The isochronous transfer failed due to host controller or device error.
                                Caller should check TransferResult for detailed error information.

**/
EFI_STATUS
EFIAPI
OhciIsochronousTransfer (
  IN     EFI_USB2_HC_PROTOCOL                *This,
  IN     UINT8                               DeviceAddress,
  IN     UINT8                               EndPointAddress,
  IN     UINT8                               DeviceSpeed,
  IN     UINTN                               MaximumPacketLength,
  IN     UINT8                               DataBuffersNumber,
  IN OUT VOID                                *Data[EFI_USB_MAX_ISO_BUFFER_NUM],
  IN     UINTN                               DataLength,
  IN     EFI_USB2_HC_TRANSACTION_TRANSLATOR  *Translator,
  OUT    UINT32                              *TransferResult
  );

/**
  Submits nonblocking isochronous transfer to an isochronous endpoint of a USB device.

  This is an asynchronous type of USB isochronous transfer. If the caller submits a USB
  isochronous transfer request through this function, this function will return immediately.

  When the isochronous transfer completes, the IsochronousCallbackfunction will be triggered,
  the caller can know the transfer results. If the transfer is successful, the caller can get
  the data received or sent in this callback function.

  The target endpoint is specified by DeviceAddressand EndpointAddress. Isochronous transfers
  are used when working with isochronous date. It provides periodic, continuous communication
  between the host and a device. Isochronous transfers can be used only by full-speed, high-speed,
  and super-speed devices.

  High-speed isochronous transfers can be performed using multiple data buffers. The number of
  buffers that are actually prepared for the transfer is specified by DataBuffersNumber. For
  full-speed isochronous transfers this value is ignored.

  Data represents a list of pointers to the data buffers. For full-speed isochronous transfers
  only the data pointed by Data[0] shall be used. For high-speed isochronous transfers and for
  the split transactions depending on DataLength there several data buffers can be used. For
  the high-speed isochronous transfers the total number of buffers must not exceed EFI_USB_MAX_ISO_BUFFER_NUM.

  For split transactions performed on full-speed device by high-speed host controller the total
  number of buffers is limited to EFI_USB_MAX_ISO_BUFFER_NUM1.

  EFI_INVALID_PARAMETER is returned if one of the following conditionsis satisfied:
    - Data is NULL.
    - DataLength is 0.
    - DeviceSpeed is not one of the supported values listed above.
    - MaximumPacketLength is invalid. MaximumPacketLength must be 1023 or less for full-speed
      devices and 1024 or less for high-speed and super-speed devices.

  @param  This                  A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  DeviceAddress         Represents the address of the target device on the USB.
  @param  EndPointAddress       The combination of an endpoint number and an endpoint direction of the
                                target USB device.
  @param  DeviceSpeed           Indicates device speed. The supported values are EFI_USB_SPEED_FULL,
                                EFI_USB_SPEED_HIGH, or EFI_USB_SPEED_SUPER.
  @param  MaximumPacketLength   Indicates the maximum packet size the target endpoint is capable of
                                sending or receiving.
  @param  DataBuffersNumber     Number of data buffers prepared for the transfer.
  @param  Data                  Array of pointers to the buffers of data that will be transmitted to USB
                                device or received from USB device.
  @param  DataLength            Specifies the length, in bytes, of the data to be sent to or received from
                                the USB device.
  @param  Translator            A pointer to the transaction translator data.
  @param  IsochronousCallback   The Callback function. This function is called if the requested
                                isochronous transfer is completed.
  @param  Context               Data passed to the IsochronousCallback function. This is an
                                optional parameter and may be NULL.

  @retval EFI_SUCCESS           The asynchronous isochronous transfer request has been successfully
                                submitted or canceled.
  @retval EFI_INVALID_PARAMETER Some parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The asynchronous isochronous transfer could not be submitted due to
                                a lack of resources.

**/
EFI_STATUS
EFIAPI
OhciAsyncIsochronousTransfer (
  IN     EFI_USB2_HC_PROTOCOL                *This,
  IN     UINT8                               DeviceAddress,
  IN     UINT8                               EndPointAddress,
  IN     UINT8                               DeviceSpeed,
  IN     UINTN                               MaximumPacketLength,
  IN     UINT8                               DataBuffersNumber,
  IN OUT VOID                                *Data[EFI_USB_MAX_ISO_BUFFER_NUM],
  IN     UINTN                               DataLength,
  IN     EFI_USB2_HC_TRANSACTION_TRANSLATOR  *Translator,
  IN     EFI_ASYNC_USB_TRANSFER_CALLBACK     IsochronousCallBack,
  IN     VOID                                *Context OPTIONAL
  );

/**
  Retrieves the current status of a USB root hub port.

  @param  This       A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  PortNumber Specifies the root hub port from which the status is to be retrieved.
                     This value is zero based.
  @param  PortStatus A pointer to the current port status bits and port status change bits.

  @retval EFI_SUCCESS           The status of the USB root hub port specified by PortNumber
                                was returned in PortStatus.
  @retval EFI_INVALID_PARAMETER PortNumber is invalid.

**/
EFI_STATUS
EFIAPI
OhciGetRootHubPortStatus (
  IN        EFI_USB2_HC_PROTOCOL  *This,
  IN        UINT8                 PortNumber,
  OUT       EFI_USB_PORT_STATUS   *PortStatus
  );

/**
  Sets a feature for the specified root hub port.

  @param  This        A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  PortNumber  Specifies the root hub port whose feature is requested to be set. This
                      value is zero based.
  @param  PortFeature Indicates the feature selector associated with the feature set request.

  @retval EFI_SUCCESS           The feature specified by PortFeature was set for the USB
                                root hub port specified by PortNumber.
  @retval EFI_INVALID_PARAMETER PortNumber is invalid or PortFeature is invalid for this function.

**/
EFI_STATUS
EFIAPI
OhciSetRootHubPortFeature (
  IN EFI_USB2_HC_PROTOCOL  *This,
  IN UINT8                 PortNumber,
  IN EFI_USB_PORT_FEATURE  PortFeature
  );

/**
  Clears a feature for the specified root hub port.

  @param  This        A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  PortNumber  Specifies the root hub port whose feature is requested to be cleared. This
                      value is zero based.
  @param  PortFeature Indicates the feature selector associated with the feature clear request.

  @retval EFI_SUCCESS           The feature specified by PortFeature was cleared for the USB
                                root hub port specified by PortNumber.
  @retval EFI_INVALID_PARAMETER PortNumber is invalid or PortFeature is invalid for this function.

**/
EFI_STATUS
EFIAPI
OhciClearRootHubPortFeature (
  IN EFI_USB2_HC_PROTOCOL  *This,
  IN UINT8                 PortNumber,
  IN EFI_USB_PORT_FEATURE  PortFeature
  );

/**
  Test to see if this driver supports ControllerHandle. Any
  ControllerHandle that has UsbHcProtocol installed will be supported.

  @param  This                 Protocol instance pointer.
  @param  Controller           Handle of device to test.
  @param  RemainingDevicePath  Not used.

  @return EFI_SUCCESS          This driver supports this device.
  @return EFI_UNSUPPORTED      This driver does not support this device.

**/
EFI_STATUS
EFIAPI
OHCIDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  );

/**
  Starting the Usb OHCI Driver.

  @param  This                  Protocol instance pointer.
  @param  Controller            Handle of device to test.
  @param  RemainingDevicePath   Not used.

  @retval EFI_SUCCESS           This driver supports this device.
  @retval EFI_UNSUPPORTED       This driver does not support this device.
  @retval EFI_DEVICE_ERROR      This driver cannot be started due to device Error.
                                EFI_OUT_OF_RESOURCES- Failed due to resource shortage.

**/
EFI_STATUS
EFIAPI
OHCIDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  );

/**
  Stop this driver on ControllerHandle. Support stopping any child handles
  created by this driver.

  @param  This                  Protocol instance pointer.
  @param  Controller            Handle of device to stop driver on.
  @param  NumberOfChildren      Number of Children in the ChildHandleBuffer.
  @param  ChildHandleBuffer     List of handles for the children we need to stop.

  @return EFI_SUCCESS
  @return others

**/
EFI_STATUS
EFIAPI
OHCIDriverBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer
  );

#endif
