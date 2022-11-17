/** @file
  Super I/O specific implementation.

  Copyright (c) 2010 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "SioDriver.h"

//
// System configuration (setup) information
//
// SYSTEM_CONFIGURATION                mSystemConfiguration;

//
// COM 1 UART Controller
//
ACPI_SIO_RESOURCES_IO_IRQ      mCom1Resources = {
  {
    { ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR },
      FixedPcdGet16 (PcdUart1IoPort),
      FixedPcdGet8 (PcdUart1Length)
    },
    {
      { ACPI_IRQ_NOFLAG_DESCRIPTOR },
      FixedPcdGet16 (PcdUart1IrqMask)
    },
    {
      ACPI_END_TAG_DESCRIPTOR,
      0
    }
};

//
// COM 2 UART Controller
//
ACPI_SIO_RESOURCES_IO_IRQ      mCom2Resources = {
  {
    { ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR },
    FixedPcdGet16 (PcdUart2IoPort),
    FixedPcdGet8 (PcdUart2Length)
  },
  {
    { ACPI_IRQ_NOFLAG_DESCRIPTOR },
    FixedPcdGet16 (PcdUart2IrqMask),
  },
  {
    ACPI_END_TAG_DESCRIPTOR,
    0
  }
};

//
// PS/2 Keyboard Controller
//
ACPI_SIO_RESOURCES_IO_IRQ      mKeyboardResources = {
  {
    { ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR },
    0x60,
    5
  },
  {
    { ACPI_IRQ_NOFLAG_DESCRIPTOR },
    BIT1
  },
  {
    ACPI_END_TAG_DESCRIPTOR,
    0
  }
};

//
// PS/2 Mouse Controller
//
ACPI_SIO_RESOURCES_IO_IRQ      mMouseResources = {
  {
    { ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR },
    0x60,
    5
  },
  {
    { ACPI_IRQ_NOFLAG_DESCRIPTOR },
    BIT12
  },
  {
    ACPI_END_TAG_DESCRIPTOR,
    0
  }
};

//
// Table of SIO Controllers
//
DEVICE_INFO    mDeviceInfo[] = {
#if FixedPcdGet8 (PcdUart1Enable) == DEVICE_ENABLED
  {
    {
      EISA_PNP_ID(0x501),
      0
    },
    0,
    RESOURCE_IO | RESOURCE_IRQ,
    { (ACPI_SMALL_RESOURCE_HEADER *) &mCom1Resources },
    { (ACPI_SMALL_RESOURCE_HEADER *) &mCom1Resources }
  },  // COM 1 UART Controller
#endif
#if FixedPcdGet8 (PcdUart2Enable) == DEVICE_ENABLED
  {
    {
      EISA_PNP_ID(0x501),
      0
    },
    0,
    RESOURCE_IO | RESOURCE_IRQ,
    { (ACPI_SMALL_RESOURCE_HEADER *) &mCom2Resources },
    { (ACPI_SMALL_RESOURCE_HEADER *) &mCom2Resources }
  },  // COM 2 UART Controller
#endif
#if FixedPcdGet8 (PcdPs2KbMsEnable) == DEVICE_ENABLED
  {
    {
      EISA_PNP_ID(0x303),
      0
    },
    0,
    0,  // Cannot change resource
    { (ACPI_SMALL_RESOURCE_HEADER *) &mKeyboardResources },
    { (ACPI_SMALL_RESOURCE_HEADER *) &mKeyboardResources }
  },  // PS/2 Keyboard Controller
  {
    {
      EISA_PNP_ID(0xF03),
      0
    },
    0,
    0,  // Cannot change resource
    { (ACPI_SMALL_RESOURCE_HEADER *) &mMouseResources },
    { (ACPI_SMALL_RESOURCE_HEADER *) &mMouseResources }
  },  // PS/2 Mouse Controller
#endif
  DEVICE_INFO_END
};



/**
  Gets the number of devices in Table of SIO Controllers mDeviceInfo.

  @retval     Number of enabled devices in Table of SIO Controllers.
**/
UINTN
EFIAPI
GetDeviceCount (
  VOID
  )
{
   UINTN        Count;
   // Get mDeviceInfo item count
   // -1 to account for for the end device info
   Count = ARRAY_SIZE (mDeviceInfo) - 1;
   return Count;
}

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
  )
{
  EFI_SIO_ACPI_DEVICE_ID   *LocalDevices;
  UINTN                    LocalCount;
  UINTN                    DeviceCount;
  UINTN                    Index;

  //
  // Allocate enough memory for simplicity
  //
  DeviceCount = GetDeviceCount ();
  LocalDevices = AllocatePool (sizeof (EFI_SIO_ACPI_DEVICE_ID) * DeviceCount);
  ASSERT (LocalDevices != NULL);
  if (LocalDevices == NULL) {
    return;
  }
  LocalCount = 0;

  for (Index = 0; Index < DeviceCount; Index++) {
    CopyMem (&LocalDevices[LocalCount], &mDeviceInfo[Index].Device, sizeof (EFI_SIO_ACPI_DEVICE_ID));
    LocalCount++;
  }

  *Devices = LocalDevices;
  *Count   = LocalCount;
}


/**
  Super I/O controller initialization.

  @retval     EFI_SUCCESS       The super I/O controller is found and initialized.
  @retval     EFI_UNSUPPORTED   The super I/O controller is not found.
**/
EFI_STATUS
SioInit (
  VOID
  )
{
  return EFI_SUCCESS;
}


/**
  Find the DEVICE_INFO for specified Device.

  @param[in]  Device        Pointer to the EFI_SIO_ACPI_DEVICE_ID.

  @retval     DEVICE_INFO*  Pointer to the DEVICE_INFO.
**/
DEVICE_INFO *
DeviceSearch (
  IN EFI_SIO_ACPI_DEVICE_ID *Device
  )
{
  UINTN       Index;
  UINTN       DeviceCount;

  DeviceCount = GetDeviceCount ();
  for (Index = 0; Index < DeviceCount; Index++) {
    if (CompareMem (Device, &mDeviceInfo[Index].Device, sizeof (*Device)) == 0) {
      return &mDeviceInfo[Index];
    }
  }

  ASSERT (FALSE);
  return NULL;
}


/**
  Program the SIO chip to enable the specified device using the default resource.

  @param[in] Device          Pointer to EFI_SIO_ACPI_DEVICE_ID.
**/
VOID
DeviceEnable (
  IN EFI_SIO_ACPI_DEVICE_ID   *Device
  )
{
}


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
  )
{
  DEVICE_INFO               *DeviceInfo;

  DeviceInfo = DeviceSearch (Device);

  *Resources = DeviceInfo->Resources;

  return EFI_SUCCESS;
}


/**
  Set the ACPI resources for specified device.

  The SIO chip is programmed to use the new resources and the
  resources setting are saved. The function assumes the resources
  are valid.

  @param[in] Device          Pointer to EFI_SIO_ACPI_DEVICE_ID.
  @param[in] Resources       ACPI_RESOURCE_HEADER_PTR.

  @retval    EFI_UNSUPPORTED
**/
EFI_STATUS
DeviceSetResources (
  IN EFI_SIO_ACPI_DEVICE_ID   *Device,
  IN ACPI_RESOURCE_HEADER_PTR Resources
  )
{
  return EFI_UNSUPPORTED;
}


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
  )
{
  DEVICE_INFO               *DeviceInfo;

  DeviceInfo = DeviceSearch (Device);

  *Resources = DeviceInfo->PossibleResources;

  return EFI_SUCCESS;
}
