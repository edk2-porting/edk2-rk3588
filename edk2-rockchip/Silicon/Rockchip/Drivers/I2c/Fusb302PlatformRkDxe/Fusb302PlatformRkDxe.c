/** @file
 *
 *  Publishes this board's description of its FUSB302 Type-C ports.
 *
 *  The board itself says what each port may source, what it will sink and how
 *  to switch its supply, through RockchipPlatformLib. This driver pairs each
 *  answer with the I2C device the controller answers on and hands the result
 *  to Fusb302Dxe, which needs to know nothing else about the platform.
 *
 *  Copyright (c) 2026 Corey Moyer <cronmod.dev@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/RockchipPlatformLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/Fusb302PlatformDevice.h>
#include <Protocol/I2c.h>

//
// One bus and one combo PHY per controller address, or a board with two
// Type-C ports would read the other port's wiring for one of them.
//
STATIC_ASSERT (
  (FixedPcdGetSize (PcdFusb302Buses) == FixedPcdGetSize (PcdFusb302Addresses)) &&
  (FixedPcdGetSize (PcdFusb302PhyIds) == FixedPcdGetSize (PcdFusb302Addresses)),
  "PcdFusb302* entry counts disagree"
  );

//
// The device class this platform's I2C bus driver publishes its devices under.
//
STATIC CONST EFI_GUID  mI2cGuid = {
  0xadc1901b, 0xb83c, 0x4831, { 0x8f, 0x59, 0x70, 0x89, 0x8f, 0x26, 0x57, 0x1e }
};

EFI_STATUS
EFIAPI
Fusb302PlatformRkInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  FUSB302_PLATFORM_DEVICE_PROTOCOL  *Port;
  CONST UINT8                       *Addresses;
  CONST UINT8                       *Buses;
  CONST UINT8                       *PhyIds;
  EFI_HANDLE                        Handle;
  EFI_STATUS                        Status;
  UINTN                             Count;
  UINTN                             Index;
  UINTN                             Published;

  Addresses = PcdGetPtr (PcdFusb302Addresses);
  Buses     = PcdGetPtr (PcdFusb302Buses);
  PhyIds    = PcdGetPtr (PcdFusb302PhyIds);
  Count     = PcdGetSize (PcdFusb302Addresses);

  if ((Addresses == NULL) || (Buses == NULL) || (PhyIds == NULL)) {
    DEBUG ((DEBUG_ERROR, "%a: board named no Type-C ports\n", __func__));
    return EFI_NOT_FOUND;
  }

  Published = 0;

  for (Index = 0; Index < Count; Index++) {
    //
    // A zero address is how a board leaves a slot in the table empty.
    //
    if (Addresses[Index] == 0) {
      continue;
    }

    Port = AllocateZeroPool (sizeof (FUSB302_PLATFORM_DEVICE_PROTOCOL));
    if (Port == NULL) {
      //
      // Give up on the rest rather than fail the entry point: the SetVbus
      // callbacks a published port hands out live in this image, and failing
      // here would have it unloaded from under them.
      //
      break;
    }

    //
    // What the board will do with this port is the board's to say. A port it
    // does not describe is one this driver has no business publishing.
    //
    Status = PlatformGetTypeCPort (Index, Port);
    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_WARN,
        "%a: board describes no port %u; skipping it\n",
        __func__,
        (UINT32)Index
        ));
      FreePool (Port);
      continue;
    }

    CopyGuid (&Port->DeviceGuid, &mI2cGuid);
    Port->DeviceIndex = I2C_DEVICE_INDEX (Buses[Index], Addresses[Index]);
    Port->PhyId       = PhyIds[Index];

    Handle = NULL;
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &Handle,
                    &gFusb302PlatformDeviceProtocolGuid,
                    Port,
                    NULL
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "%a: could not publish port %u. Status=%r\n",
        __func__,
        (UINT32)Index,
        Status
        ));
      FreePool (Port);
      continue;
    }

    Published++;

    DEBUG ((
      DEBUG_INFO,
      "%a: port %u at i2c%u:0x%02x on PHY %u, %a source its rail\n",
      __func__,
      (UINT32)Index,
      Buses[Index],
      Addresses[Index],
      Port->PhyId,
      Port->SetVbus != NULL ? "can" : "cannot"
      ));
  }

  //
  // Nothing published means nothing holds a pointer into this image, so it
  // may as well go. Fusb302Dxe waits on the protocol and simply never starts.
  //
  return Published > 0 ? EFI_SUCCESS : EFI_NOT_FOUND;
}
