/** @file
  Super I/O specific header.

  Copyright (c) 2010 - 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SIO_H_
#define _SIO_H_


#include "Register.h"

typedef
UINT8
(EFIAPI *LOCAL_IO_WRITE8) (
  IN      UINTN                     Port,
  IN      UINT8                     Value
  );

#define RESOURCE_IO    BIT0
#define RESOURCE_IRQ   BIT1
#define RESOURCE_DMA   BIT2
#define RESOURCE_MEM   BIT3

#define DEVICE_ENABLED  0x01
#define DEVICE_INFO_END { { 0xFFFFFFFF, 0xFFFFFFFF } }
#pragma pack(1)

typedef struct {
  EFI_ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR  Io;
  EFI_ACPI_IRQ_NOFLAG_DESCRIPTOR              Irq;
  EFI_ACPI_END_TAG_DESCRIPTOR                 End;
} ACPI_SIO_RESOURCES_IO_IRQ;
#pragma pack()

typedef struct {
  UINT32                      HID;
  UINT32                      UID;
} EFI_SIO_ACPI_DEVICE_ID;

typedef struct {
  EFI_SIO_ACPI_DEVICE_ID      Device;
  UINT8                       DeviceId;
  UINT8                       ResourceMask;
  ACPI_RESOURCE_HEADER_PTR    Resources;
  ACPI_RESOURCE_HEADER_PTR    PossibleResources;
} DEVICE_INFO;

typedef struct {
  UINT8 Segment;
  UINT8 Bus;
  UINT8 Device;
  UINT8 Funtion;
} SIO_PCI_ISA_BRIDGE_DEVICE_INFO;

/**
  Return the supported devices.

  @param[out] Devices         Pointer to pointer of EFI_SIO_ACPI_DEVICE_ID.
                              Caller is responsible to free the buffer.
  @param[out] Count           Pointer to UINTN holding the device count.
**/
VOID
DeviceGetList (
  OUT EFI_SIO_ACPI_DEVICE_ID **Devices,
  OUT UINTN                  *Count
  );


/**
  Program the SIO chip to enable the specified device using the default resource.

  @param[in]  Device          Pointer to EFI_SIO_ACPI_DEVICE_ID.
**/
VOID
DeviceEnable (
  IN EFI_SIO_ACPI_DEVICE_ID   *Device
  );


/**
  Get the possible ACPI resources for specified device.

  @param[in]  Device          Pointer to EFI_SIO_ACPI_DEVICE_ID.
  @param[out] Resources       Pointer to ACPI_RESOURCE_HEADER_PTR.

  @retval     EFI_SUCCESS     The resources are returned successfully.
**/
EFI_STATUS
DevicePossibleResources (
  IN  EFI_SIO_ACPI_DEVICE_ID   *Device,
  OUT ACPI_RESOURCE_HEADER_PTR *Resources
  );


/**
  Set the ACPI resources for specified device.

  The SIO chip is programmed to use the new resources and the
  resources setting are saved. The function assumes the resources
  are valid.

  @param[in]  Device          Pointer to EFI_SIO_ACPI_DEVICE_ID.
  @param[in]  Resources       ACPI_RESOURCE_HEADER_PTR.

  @retval     EFI_SUCCESS     The resources are set successfully.
**/
EFI_STATUS
DeviceSetResources (
  IN EFI_SIO_ACPI_DEVICE_ID   *Device,
  IN ACPI_RESOURCE_HEADER_PTR Resources
  );


/**
  Get the ACPI resources for specified device.

  @param[in]  Device          Pointer to EFI_SIO_ACPI_DEVICE_ID.
  @param[out] Resources       Pointer to ACPI_RESOURCE_HEADER_PTR.

  @retval     EFI_SUCCESS     The resources are returned successfully.
**/
EFI_STATUS
DeviceGetResources (
  IN  EFI_SIO_ACPI_DEVICE_ID   *Device,
  OUT ACPI_RESOURCE_HEADER_PTR *Resources
  );


/**
  Program the SIO chip to enter the configure mode.
**/
VOID
EnterConfigMode (
  VOID
  );


/**
  Program the SIO chip to exit the configure mode.
**/
VOID
ExitConfigMode (
  VOID
  );


/**
  Perform a 8-bit I/O write to SIO register.

  @param[in]  Index  The register index.
  @param[in]  Data   The value to write to register.
**/
VOID
WriteRegister (
  IN  UINT8            Index,
  IN  UINT8            Data
  );


/**
  Perform a 8-bit I/O read from SIO register.

  @param[in]  Index  The register index.

  @retval     Value  The value written to the register.
**/
UINT8
ReadRegister (
  IN  UINT8            Index
  );

//
// Prototypes for the sio internal function
//
//
// Internal function
//

/**
  Find Super I/O controller.

  @retval     EFI_SUCCESS       Super I/O controller exists.
  @retval     EFI_UNSUPPORTED   Super I/O controller does not exist.
**/
EFI_STATUS
SioInit (
  VOID
  );

#endif
