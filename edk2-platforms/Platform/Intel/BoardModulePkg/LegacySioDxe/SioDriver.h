/** @file
  Header file for Driver Binding Protocol.

  Copyright (c) 2010 - 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SIO_DRIVER_H_
#define _SIO_DRIVER_H_

#include <PiDxe.h>
#include <IndustryStandard/Pci.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiLib.h>
#include <Library/PcdLib.h>

//
// Driver Consumed Protocol Prototypes
//
#include <Protocol/DriverBinding.h>
#include <Protocol/PciIo.h>
#include <Protocol/DevicePath.h>

//
// Driver Produced Protocol Prototypes
//
#include <Protocol/SuperIo.h>


#include "SioChip.h"
#include "SioService.h"
#include "ComponentName.h"

//
// Global Variables definitions
//
extern EFI_DRIVER_BINDING_PROTOCOL   mSioDriver;
extern EFI_COMPONENT_NAME_PROTOCOL   mSioComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL  mSioComponentName2;

//
// SIO device private data structure
//
#define SIO_DEV_SIGNATURE SIGNATURE_32 ('_', 'S', 'I', 'O')

typedef struct _SIO_DEV {
  UINT32                   Signature;
  EFI_HANDLE               PciHandle;
  EFI_SIO_ACPI_DEVICE_ID   Device;
  EFI_HANDLE               Handle;
  EFI_SIO_PROTOCOL         Sio;
  EFI_DEVICE_PATH_PROTOCOL *DevicePath;
  LIST_ENTRY               Link;
} SIO_DEV;

#define SIO_DEV_FROM_THIS(a) CR (a, SIO_DEV, Sio, SIO_DEV_SIGNATURE)

//
// Prototypes for Driver model protocol interface
//


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
  );


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
  );


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
  );
#endif
