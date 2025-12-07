/** @file

  Provides some data structure definitions used by the Designware SD/MMC
  host controller driver.

  Copyright (c) 2015, Intel Corporation. All rights reserved.
  Copyright (C) 2018, Linaro Ltd. All rigths reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DW_MMC_HC_DXE_H_
#define _DW_MMC_HC_DXE_H_

#include <Uefi.h>

#include <Library/UefiLib.h>

#include <Protocol/ComponentName.h>
#include <Protocol/ComponentName2.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/SdMmcPassThru.h>

#include "DwMmcHci.h"

extern EFI_COMPONENT_NAME_PROTOCOL   gDwMmcHcComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL  gDwMmcHcComponentName2;
extern EFI_DRIVER_BINDING_PROTOCOL   gDwMmcHcDriverBinding;

#define DW_MMC_HC_PRIVATE_SIGNATURE  SIGNATURE_32 ('d', 'w', 's', 'd')

#define DW_MMC_HC_PRIVATE_FROM_THIS(a) \
    CR(a, DW_MMC_HC_PRIVATE_DATA, PassThru, DW_MMC_HC_PRIVATE_SIGNATURE)

//
// Generic time out value, 1 microsecond as unit.
//
#define DW_MMC_HC_GENERIC_TIMEOUT  (1 * 1000 * 1000)

//
// SD/MMC async transfer timer interval, set by experience.
// The unit is 100us, takes 1ms as interval.
//
#define DW_MMC_HC_ASYNC_TIMER  EFI_TIMER_PERIOD_MILLISECONDS(1)
//
// SD/MMC removable device enumeration timer interval, set by experience.
// The unit is 100us, takes 100ms as interval.
//
#define DW_MMC_HC_ENUM_TIMER  EFI_TIMER_PERIOD_MILLISECONDS(100)

typedef struct {
  BOOLEAN                 Enable;
  EFI_SD_MMC_SLOT_TYPE    SlotType;
  BOOLEAN                 MediaPresent;
  BOOLEAN                 Initialized;
  SD_MMC_CARD_TYPE        CardType;
} DW_MMC_HC_SLOT;

typedef struct {
  UINTN                            Signature;

  EFI_HANDLE                       ControllerHandle;

  // Mmio base address
  UINTN                            DevBase;

  EFI_SD_MMC_PASS_THRU_PROTOCOL    PassThru;

  PLATFORM_DW_MMC_PROTOCOL         *PlatformDwMmc;
  //
  // The field is used to record the previous slot in GetNextSlot().
  //
  UINT8                            PreviousSlot;
  //
  // For Non-blocking operation.
  //
  EFI_EVENT                        TimerEvent;
  //
  // For Sd removable device enumeration.
  //
  EFI_EVENT                        ConnectEvent;
  LIST_ENTRY                       Queue;

  DW_MMC_HC_SLOT                   Slot[DW_MMC_HC_MAX_SLOT];
  DW_MMC_HC_SLOT_CAP               Capability[DW_MMC_HC_MAX_SLOT];
  UINT64                           MaxCurrent[DW_MMC_HC_MAX_SLOT];

  UINT32                           ControllerVersion;
} DW_MMC_HC_PRIVATE_DATA;

#define DW_MMC_HC_TRB_SIG  SIGNATURE_32 ('D', 'T', 'R', 'B')

//
// TRB (Transfer Request Block) contains information for the cmd request.
//
typedef struct {
  UINT32                                 Signature;
  LIST_ENTRY                             TrbList;

  UINT8                                  Slot;
  UINT16                                 BlockSize;

  EFI_SD_MMC_PASS_THRU_COMMAND_PACKET    *Packet;
  VOID                                   *Data;
  UINT32                                 DataLen;
  BOOLEAN                                Read;
  EFI_PHYSICAL_ADDRESS                   DataPhy;
  VOID                                   *DataMap;
  DW_MMC_HC_TRANSFER_MODE                Mode;

  EFI_EVENT                              Event;
  BOOLEAN                                Started;
  UINT64                                 Timeout;

  DW_MMC_HC_DMA_DESC_LINE                *DmaDesc;
  EFI_PHYSICAL_ADDRESS                   DmaDescPhy;
  UINT32                                 DmaDescPages;
  VOID                                   *DmaMap;

  BOOLEAN                                UseFifo;
  BOOLEAN                                UseBE;               // Big-endian

  DW_MMC_HC_PRIVATE_DATA                 *Private;
} DW_MMC_HC_TRB;

#define DW_MMC_HC_TRB_FROM_THIS(a) \
    CR(a, DW_MMC_HC_TRB, TrbList, DW_MMC_HC_TRB_SIG)

//
// Task for Non-blocking mode.
//
typedef struct {
  UINT32                                 Signature;
  LIST_ENTRY                             Link;

  UINT8                                  Slot;
  EFI_SD_MMC_PASS_THRU_COMMAND_PACKET    *Packet;
  BOOLEAN                                IsStart;
  EFI_EVENT                              Event;
  UINT64                                 RetryTimes;
  BOOLEAN                                InfiniteWait;
  VOID                                   *Map;
  VOID                                   *MapAddress;
} DW_MMC_HC_QUEUE;

//
// Prototypes
//

/**
  Execute card identification procedure.

  @param[in] Private        A pointer to the DW_MMC_HC_PRIVATE_DATA instance.

  @retval EFI_SUCCESS       The card is identified correctly.
  @retval Others            The card can't be identified.

**/
typedef
EFI_STATUS
(*DWMMC_CARD_TYPE_DETECT_ROUTINE) (
  IN DW_MMC_HC_PRIVATE_DATA  *Private
  );

/**
  Sends SD command to an SD card that is attached to the SD controller.

  The PassThru() function sends the SD command specified by Packet to the SD
  card specified by Slot.

  If Packet is successfully sent to the SD card, then EFI_SUCCESS is returned.

  If a device error occurs while sending the Packet, then EFI_DEVICE_ERROR is
  returned.

  If Slot is not in a valid range for the SD controller, then
  EFI_INVALID_PARAMETER is returned.

  If Packet defines a data command but both InDataBuffer and OutDataBuffer are
  NULL, EFI_INVALID_PARAMETER is returned.

  @param[in]     This           A pointer to the EFI_SD_MMC_PASS_THRU_PROTOCOL
                                instance.
  @param[in]     Slot           The slot number of the SD card to send the
                                command to.
  @param[in,out] Packet         A pointer to the SD command data structure.
  @param[in]     Event          If Event is NULL, blocking I/O is performed. If
                                Event is not NULL, then nonblocking I/O is
                                performed, and Event will be signaled when the
                                Packet completes.

  @retval EFI_SUCCESS           The SD Command Packet was sent by the host.
  @retval EFI_DEVICE_ERROR      A device error occurred while attempting to send
                                the SD command Packet.
  @retval EFI_INVALID_PARAMETER Packet, Slot, or the contents of the Packet is
                                invalid.
  @retval EFI_INVALID_PARAMETER Packet defines a data command but both
                                InDataBuffer and OutDataBuffer are NULL.
  @retval EFI_NO_MEDIA          SD Device not present in the Slot.
  @retval EFI_UNSUPPORTED       The command described by the SD Command Packet
                                is not supported by the host controller.
  @retval EFI_BAD_BUFFER_SIZE   The InTransferLength or OutTransferLength
                                exceeds the limit supported by SD card ( i.e. if
                                the number of bytes exceed the Last LBA).

**/
EFI_STATUS
EFIAPI
DwMmcPassThruPassThru (
  IN     EFI_SD_MMC_PASS_THRU_PROTOCOL        *This,
  IN     UINT8                                Slot,
  IN OUT EFI_SD_MMC_PASS_THRU_COMMAND_PACKET  *Packet,
  IN     EFI_EVENT                            Event    OPTIONAL
  );

/**
  Used to retrieve next slot numbers supported by the SD controller. The
  function returns information about all available slots (populated or
  not-populated).

  The GetNextSlot() function retrieves the next slot number on an SD controller.
  If on input Slot is 0xFF, then the slot number of the first slot on the SD
  controller is returned.

  If Slot is a slot number that was returned on a previous call to
  GetNextSlot(), then the slot number of the next slot on the SD controller is
  returned.

  If Slot is not 0xFF and Slot was not returned on a previous call to
  GetNextSlot(), EFI_INVALID_PARAMETER is returned.

  If Slot is the slot number of the last slot on the SD controller, then
  EFI_NOT_FOUND is returned.

  @param[in]     This           A pointer to the EFI_SD_MMMC_PASS_THRU_PROTOCOL
                                instance.
  @param[in,out] Slot           On input, a pointer to a slot number on the SD
                                controller.
                                On output, a pointer to the next slot number on
                                the SD controller.
                                An input value of 0xFF retrieves the first slot
                                number on the SD controller.

  @retval EFI_SUCCESS           The next slot number on the SD controller was
                                returned in Slot.
  @retval EFI_NOT_FOUND         There are no more slots on this SD controller.
  @retval EFI_INVALID_PARAMETER Slot is not 0xFF and Slot was not returned on a
                                previous call to GetNextSlot().

**/
EFI_STATUS
EFIAPI
DwMmcPassThruGetNextSlot (
  IN     EFI_SD_MMC_PASS_THRU_PROTOCOL  *This,
  IN OUT UINT8                          *Slot
  );

/**
  Used to allocate and build a device path node for an SD card on the SD
  controller.

  The BuildDevicePath() function allocates and builds a single device node
  for the SD
  card specified by Slot.

  If the SD card specified by Slot is not present on the SD controller, then
  EFI_NOT_FOUND is returned.

  If DevicePath is NULL, then EFI_INVALID_PARAMETER is returned.

  If there are not enough resources to allocate the device path node, then
  EFI_OUT_OF_RESOURCES is returned.

  Otherwise, DevicePath is allocated with the boot service AllocatePool(), the
  contents of DevicePath are initialized to describe the SD card specified by
  Slot, and EFI_SUCCESS is returned.

  @param[in]     This           A pointer to the EFI_SD_MMMC_PASS_THRU_PROTOCOL
                                instance.
  @param[in]     Slot           Specifies the slot number of the SD card for
                                which a device path node is to be allocated and
                                built.
  @param[in,out] DevicePath     A pointer to a single device path node that
                                describes the SD card specified by Slot. This
                                function is responsible for allocating the
                                buffer DevicePath with the boot service
                                AllocatePool(). It is the caller's responsibi-
                                lity to free DevicePath when the caller is
                                finished with DevicePath.

  @retval EFI_SUCCESS           The device path node that describes the SD card
                                specified by Slot was allocated and returned in
                                DevicePath.
  @retval EFI_NOT_FOUND         The SD card specified by Slot does not exist on
                                the SD controller.
  @retval EFI_INVALID_PARAMETER DevicePath is NULL.
  @retval EFI_OUT_OF_RESOURCES  There are not enough resources to allocate
                                DevicePath.

**/
EFI_STATUS
EFIAPI
DwMmcPassThruBuildDevicePath (
  IN     EFI_SD_MMC_PASS_THRU_PROTOCOL  *This,
  IN     UINT8                          Slot,
  IN OUT EFI_DEVICE_PATH_PROTOCOL       **DevicePath
  );

/**
  This function retrieves an SD card slot number based on the input device path.

  The GetSlotNumber() function retrieves slot number for the SD card specified
  by the DevicePath node. If DevicePath is NULL, EFI_INVALID_PARAMETER is
  returned.

  If DevicePath is not a device path node type that the SD Pass Thru driver
  supports, EFI_UNSUPPORTED is returned.

  @param[in]  This              A pointer to the EFI_SD_MMC_PASS_THRU_PROTOCOL
                                instance.
  @param[in]  DevicePath        A pointer to the device path node that describes
                                a SD card on the SD controller.
  @param[out] Slot              On return, points to the slot number of an SD
                                card on the SD controller.

  @retval EFI_SUCCESS           SD card slot number is returned in Slot.
  @retval EFI_INVALID_PARAMETER Slot or DevicePath is NULL.
  @retval EFI_UNSUPPORTED       DevicePath is not a device path node type that
                                the SD Pass Thru driver supports.

**/
EFI_STATUS
EFIAPI
DwMmcPassThruGetSlotNumber (
  IN  EFI_SD_MMC_PASS_THRU_PROTOCOL  *This,
  IN  EFI_DEVICE_PATH_PROTOCOL       *DevicePath,
  OUT UINT8                          *Slot
  );

/**
  Resets an SD card that is connected to the SD controller.

  The ResetDevice() function resets the SD card specified by Slot.

  If this SD controller does not support a device reset operation,
  EFI_UNSUPPORTED is returned.

  If Slot is not in a valid slot number for this SD controller,
  EFI_INVALID_PARAMETER is returned.

  If the device reset operation is completed, EFI_SUCCESS is returned.

  @param[in]  This              A pointer to the EFI_SD_MMC_PASS_THRU_PROTOCOL
                                instance.
  @param[in]  Slot              Specifies the slot number of the SD card to be
                                reset.

  @retval EFI_SUCCESS           The SD card specified by Slot was reset.
  @retval EFI_UNSUPPORTED       The SD controller does not support a device
                                reset operation.
  @retval EFI_INVALID_PARAMETER Slot number is invalid.
  @retval EFI_NO_MEDIA          SD Device not present in the Slot.
  @retval EFI_DEVICE_ERROR      The reset command failed due to a device error

**/
EFI_STATUS
EFIAPI
DwMmcPassThruResetDevice (
  IN EFI_SD_MMC_PASS_THRU_PROTOCOL  *This,
  IN UINT8                          Slot
  );

//
// Driver model protocol interfaces
//

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
  FreePool(), and OpenProtocol() with CloseProtocol().
  Since ControllerHandle may have been previously started by the same driver, if
  a protocol is already in the opened state, then it must not be closed with
  CloseProtocol(). This is required to guarantee the state of ControllerHandle
  is not modified by this function.

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL
                                   instance.
  @param[in]  ControllerHandle     The handle of the controller to test. This
                                   handle must support a protocol interface that
                                   supplies an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath  A pointer to the remaining portion of a
                                   device path.  This parameter is ignored by
                                   device drivers, and is optional for bus
                                   drivers. For bus drivers, if this parameter
                                   is not NULL, then the bus driver must deter-
                                   mine if the bus controller specified by
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
                                   Currently not implemented.
  @retval EFI_UNSUPPORTED          The device specified by ControllerHandle and
                                   RemainingDevicePath is not supported by the
                                   driver specified by This.
**/
EFI_STATUS
EFIAPI
DwMmcHcDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  );

/**
  Starts a device controller or a bus controller.

  The Start() function is designed to be invoked from the EFI boot service
  ConnectController().
  As a result, much of the error checking on the parameters to Start() has been
  moved into this common boot service. It is legal to call Start() from other
  locations,
  but the following calling restrictions must be followed or the system behavior
  will not be deterministic.
  1. ControllerHandle must be a valid EFI_HANDLE.
  2. If RemainingDevicePath is not NULL, then it must be a pointer to a natural-
     ly aligned EFI_DEVICE_PATH_PROTOCOL.
  3. Prior to calling Start(), the Supported() function for the driver specified
     by This must have been called with the same calling parameters, and
     Supported() must have returned EFI_SUCCESS.

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL
                                   instance.
  @param[in]  ControllerHandle     The handle of the controller to start. This
                                   handle must support a protocol interface that
                                   supplies an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath  A pointer to the remaining portion of a
                                   device path.  This parameter is ignored by
                                   device drivers, and is optional for bus dri-
                                   vers. For a bus driver, if this parameter is
                                   NULL, then handles for all the children of
                                   Controller are created by this driver.
                                   If this parameter is not NULL and the first
                                   Device Path Node is not the End of Device
                                   Path Node, then only the handle for the
                                   child device specified by the first Device
                                   Path Node of RemainingDevicePath is created
                                   by this driver.
                                   If the first Device Path Node of
                                   RemainingDevicePath is the End of Device Path
                                   Node, no child handle is created by this
                                   driver.

  @retval EFI_SUCCESS              The device was started.
  @retval EFI_DEVICE_ERROR         The device could not be started due to a
                                   device error. Currently not implemented.
  @retval EFI_OUT_OF_RESOURCES     The request could not be completed due to a
                                   lack of resources.
  @retval Others                   The driver failded to start the device.

**/
EFI_STATUS
EFIAPI
DwMmcHcDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  );

/**
  Stops a device controller or a bus controller.

  The Stop() function is designed to be invoked from the EFI boot service
  DisconnectController().
  As a result, much of the error checking on the parameters to Stop() has been
  moved into this common boot service. It is legal to call Stop() from other
  locations, but the following calling restrictions must be followed or the
  system behavior will not be deterministic.
  1. ControllerHandle must be a valid EFI_HANDLE that was used on a previous
     call to this same driver's Start() function.
  2. The first NumberOfChildren handles of ChildHandleBuffer must all be a valid
     EFI_HANDLE. In addition, all of these handles must have been created in
     this driver's Start() function, and the Start() function must have called
     OpenProtocol() on ControllerHandle with an Attribute of
     EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER.

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
DwMmcHcDriverBindingStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN  EFI_HANDLE                   Controller,
  IN  UINTN                        NumberOfChildren,
  IN  EFI_HANDLE                   *ChildHandleBuffer
  );

//
// EFI Component Name Functions
//

/**
  Retrieves a Unicode string that is the user readable name of the driver.

  This function retrieves the user readable name of a driver in the form of a
  Unicode string. If the driver specified by This has a user readable name in
  the language specified by Language, then a pointer to the driver name is
  returned in DriverName, and EFI_SUCCESS is returned. If the driver specified
  by This does not support the language specified by Language,
  then EFI_UNSUPPORTED is returned.

  @param  This[in]              A pointer to the EFI_COMPONENT_NAME2_PROTOCOL or
                                EFI_COMPONENT_NAME_PROTOCOL instance.

  @param  Language[in]          A pointer to a Null-terminated ASCII string
                                array indicating the language. This is the
                                language of the driver name that the caller is
                                requesting, and it must match one of the
                                languages specified in SupportedLanguages. The
                                number of languages supported by a driver is up
                                to the driver writer. Language is specified
                                in RFC 4646 or ISO 639-2 language code format.

  @param  DriverName[out]       A pointer to the Unicode string to return.
                                This Unicode string is the name of the
                                driver specified by This in the language
                                specified by Language.

  @retval EFI_SUCCESS           The Unicode string for the Driver specified by
                                This and the language specified by Language was
                                returned in DriverName.

  @retval EFI_INVALID_PARAMETER Language is NULL.

  @retval EFI_INVALID_PARAMETER DriverName is NULL.

  @retval EFI_UNSUPPORTED       The driver specified by This does not support
                                the language specified by Language.

**/
EFI_STATUS
EFIAPI
DwMmcHcComponentNameGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
  );

/**
  Retrieves a Unicode string that is the user readable name of the controller
  that is being managed by a driver.

  This function retrieves the user readable name of the controller specified by
  ControllerHandle and ChildHandle in the form of a Unicode string. If the
  driver specified by This has a user readable name in the language specified by
  Language, then a pointer to the controller name is returned in ControllerName,
  and EFI_SUCCESS is returned.  If the driver specified by This is not currently
  managing the controller specified by ControllerHandle and ChildHandle,
  then EFI_UNSUPPORTED is returned.  If the driver specified by This does not
  support the language specified by Language, then EFI_UNSUPPORTED is returned.

  @param  This[in]              A pointer to the EFI_COMPONENT_NAME2_PROTOCOL or
                                EFI_COMPONENT_NAME_PROTOCOL instance.

  @param  ControllerHandle[in]  The handle of a controller that the driver
                                specified by This is managing.  This handle
                                specifies the controller whose name is to be
                                returned.

  @param  ChildHandle[in]       The handle of the child controller to retrieve
                                the name of.  This is an optional parameter that
                                may be NULL.  It will be NULL for device
                                drivers.  It will also be NULL for a bus drivers
                                that wish to retrieve the name of the bus
                                controller.  It will not be NULL for a bus
                                driver that wishes to retrieve the name of a
                                child controller.

  @param  Language[in]          A pointer to a Null-terminated ASCII string
                                array indicating the language.  This is the
                                language of the driver name that the caller is
                                requesting, and it must match one of the
                                languages specified in SupportedLanguages. The
                                number of languages supported by a driver is up
                                to the driver writer. Language is specified in
                                RFC 4646 or ISO 639-2 language code format.

  @param  ControllerName[out]   A pointer to the Unicode string to return.
                                This Unicode string is the name of the
                                controller specified by ControllerHandle and
                                ChildHandle in the language specified by
                                Language from the point of view of the driver
                                specified by This.

  @retval EFI_SUCCESS           The Unicode string for the user readable name in
                                the language specified by Language for the
                                driver specified by This was returned in
                                DriverName.

  @retval EFI_INVALID_PARAMETER ControllerHandle is not a valid EFI_HANDLE.

  @retval EFI_INVALID_PARAMETER ChildHandle is not NULL and it is not a valid
                                EFI_HANDLE.

  @retval EFI_INVALID_PARAMETER Language is NULL.

  @retval EFI_INVALID_PARAMETER ControllerName is NULL.

  @retval EFI_UNSUPPORTED       The driver specified by This is not currently
                                managing the controller specified by
                                ControllerHandle and ChildHandle.

  @retval EFI_UNSUPPORTED       The driver specified by This does not support
                                the language specified by Language.

**/
EFI_STATUS
EFIAPI
DwMmcHcComponentNameGetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL *This,
  IN  EFI_HANDLE ControllerHandle,
  IN  EFI_HANDLE ChildHandle, OPTIONAL
  IN  CHAR8                           *Language,
  OUT CHAR16                          **ControllerName
  );

/**
  Create a new TRB for the SD/MMC cmd request.

  @param[in] Private        A pointer to the DW_MMC_HC_PRIVATE_DATA instance.
  @param[in] Slot           The slot number of the SD card to send the command
                            to.
  @param[in] Packet         A pointer to the SD command data structure.
  @param[in] Event          If Event is NULL, blocking I/O is performed.
                            If Event is not NULL, then nonblocking I/O is
                            performed, and Event will be signaled when the
                            Packet completes.

  @return Created Trb or NULL.

**/
DW_MMC_HC_TRB *
DwMmcCreateTrb (
  IN DW_MMC_HC_PRIVATE_DATA               *Private,
  IN UINT8                                Slot,
  IN EFI_SD_MMC_PASS_THRU_COMMAND_PACKET  *Packet,
  IN EFI_EVENT                            Event
  );

/**
  Free the resource used by the TRB.

  @param[in] Trb            The pointer to the DW_MMC_HC_TRB instance.

**/
VOID
DwMmcFreeTrb (
  IN DW_MMC_HC_TRB  *Trb
  );

/**
  Check if the env is ready for execute specified TRB.

  @param[in] Private        A pointer to the DW_MMC_HC_PRIVATE_DATA instance.
  @param[in] Trb            The pointer to the DW_MMC_HC_TRB instance.

  @retval EFI_SUCCESS       The env is ready for TRB execution.
  @retval EFI_NOT_READY     The env is not ready for TRB execution.
  @retval Others            Some erros happen.

**/
EFI_STATUS
DwMmcCheckTrbEnv (
  IN DW_MMC_HC_PRIVATE_DATA  *Private,
  IN DW_MMC_HC_TRB           *Trb
  );

/**
  Wait for the env to be ready for execute specified TRB.

  @param[in] Private        A pointer to the DW_MMC_HC_PRIVATE_DATA instance.
  @param[in] Trb            The pointer to the DW_MMC_HC_TRB instance.

  @retval EFI_SUCCESS       The env is ready for TRB execution.
  @retval EFI_TIMEOUT       The env is not ready for TRB execution in time.
  @retval Others            Some erros happen.

**/
EFI_STATUS
DwMmcWaitTrbEnv (
  IN DW_MMC_HC_PRIVATE_DATA  *Private,
  IN DW_MMC_HC_TRB           *Trb
  );

/**
  Execute the specified TRB.

  @param[in] Private        A pointer to the DW_MMC_HC_PRIVATE_DATA instance.
  @param[in] Trb            The pointer to the DW_MMC_HC_TRB instance.

  @retval EFI_SUCCESS       The TRB is sent to host controller successfully.
  @retval Others            Some erros happen when sending this request to the
                            host controller.

**/
EFI_STATUS
DwMmcExecTrb (
  IN DW_MMC_HC_PRIVATE_DATA  *Private,
  IN DW_MMC_HC_TRB           *Trb
  );

/**
  Check the TRB execution result.

  @param[in] Private        A pointer to the DW_MMC_HC_PRIVATE_DATA instance.
  @param[in] Trb            The pointer to the DW_MMC_HC_TRB instance.

  @retval EFI_SUCCESS       The TRB is executed successfully.
  @retval EFI_NOT_READY     The TRB is not completed for execution.
  @retval Others            Some erros happen when executing this request.

**/
EFI_STATUS
DwMmcCheckTrbResult (
  IN DW_MMC_HC_PRIVATE_DATA  *Private,
  IN DW_MMC_HC_TRB           *Trb
  );

/**
  Wait for the TRB execution result.

  @param[in] Private        A pointer to the DW_MMC_HC_PRIVATE_DATA instance.
  @param[in] Trb            The pointer to the DW_MMC_HC_TRB instance.

  @retval EFI_SUCCESS       The TRB is executed successfully.
  @retval Others            Some erros happen when executing this request.

**/
EFI_STATUS
DwMmcWaitTrbResult (
  IN DW_MMC_HC_PRIVATE_DATA  *Private,
  IN DW_MMC_HC_TRB           *Trb
  );

/**
  Execute EMMC device identification procedure.

  Refer to EMMC Electrical Standard Spec 5.1 Section 6.4 for details.

  @param[in] Private        A pointer to the DW_MMC_HC_PRIVATE_DATA instance.

  @retval EFI_SUCCESS       There is a EMMC card.
  @retval Others            There is not a EMMC card.

**/
EFI_STATUS
EmmcIdentification (
  IN DW_MMC_HC_PRIVATE_DATA  *Private
  );

/**
  Execute EMMC device identification procedure.

  Refer to EMMC Electrical Standard Spec 5.1 Section 6.4 for details.

  @param[in] Private        A pointer to the DW_MMC_HC_PRIVATE_DATA instance.

  @retval EFI_SUCCESS       There is a EMMC card.
  @retval Others            There is not a EMMC card.

**/
EFI_STATUS
SdCardIdentification (
  IN DW_MMC_HC_PRIVATE_DATA  *Private
  );

#endif /* _DW_MMC_HC_DXE_H_ */
