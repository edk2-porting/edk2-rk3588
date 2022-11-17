/** @file
  96boards Secure96 mezzanine board DXE driver.

  Copyright (c) 2018, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiDxe.h>
#include <libfdt.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/LsConnector.h>
#include <Protocol/Mezzanine.h>

#include "Secure96.h"

#define SECURE96_SSDT_OEM_TABLE_ID SIGNATURE_64('S','E','C','U','R','E','9','6')

STATIC CONST UINT32 mI2cAtmelSha204aSlaveAddress[] = {
  ATSHA204A_SLAVE_ADDRESS,

  //
  // The Atmel AtSha204a has an annoying 'wake' mode where it will only wake
  // up if SDA is held low for a certain amount of time. Attempting to access
  // a device at address 0x0 at 100 kHz should be sufficient to create this
  // wake condition, so add address 0x0 to the slave addresses.
  //
  0
};

STATIC CONST EFI_I2C_DEVICE mI2c0Devices[] = {
  {
    &gAtSha204aI2cDeviceGuid,                     // DeviceGuid
    0,                                            // DeviceIndex
    0,                                            // HardwareRevision
    0,                                            // I2C bus configuration
    ARRAY_SIZE (mI2cAtmelSha204aSlaveAddress),    // SlaveAddressCount
    mI2cAtmelSha204aSlaveAddress                  // SlaveAddressArray
  }
};

STATIC CONST CHAR8 mLedNodes[][46] = {
  "/fragment@2/__overlay__/gpio-leds/secure96-u1",
  "/fragment@2/__overlay__/gpio-leds/secure96-u2",
  "/fragment@2/__overlay__/gpio-leds/secure96-u3",
  "/fragment@2/__overlay__/gpio-leds/secure96-u4",
};

STATIC
VOID
SetOverlayFragmentTarget (
  VOID            *Overlay,
  CONST CHAR8     *NodeName,
  CONST CHAR8     *Target
  )
{
  INT32       Node;
  INT32       Err;

  Node = fdt_path_offset (Overlay, NodeName);
  ASSERT (Node > 0);

  Err = fdt_setprop (Overlay, Node, "target-path", Target,
          AsciiStrLen (Target) + 1);
  if (Err) {
    DEBUG ((DEBUG_ERROR, "%a: fdt_setprop() failed - %a\n",
      __FUNCTION__, fdt_strerror (Err)));
  }
}

STATIC
VOID
FixupOverlay (
  VOID        *Dtb,
  VOID        *Overlay
  )
{
  INT32       Node;
  UINT32      GpioPhandle;
  UINTN       Idx;
  UINT32      *GpioProp;
  INT32       Err;

  //
  // Set the correct GPIO phandle in the LED nodes
  //
  Node = fdt_path_offset (Dtb, FixedPcdGetPtr (PcdGpioParent));
  ASSERT (Node > 0);

  GpioPhandle = fdt_get_phandle (Dtb, Node);
  if (!GpioPhandle) {
    //
    // Node has no phandle yet -> create one
    //
    GpioPhandle = 1 + fdt_get_max_phandle (Dtb);
    ASSERT (GpioPhandle >= 1);

    Err = fdt_setprop_u32 (Dtb, Node, "phandle", GpioPhandle);
    if (Err) {
      DEBUG ((DEBUG_ERROR,
        "%a: fdt_setprop_u32(.., .., \"phandle\", 0x%x) failed - %a\n",
        __FUNCTION__, GpioPhandle, fdt_strerror (Err)));
    }
  }

  for (Idx = 0; Idx < ARRAY_SIZE (mLedNodes); Idx++) {
    Node = fdt_path_offset (Overlay, mLedNodes[Idx]);
    ASSERT (Node > 0);

    GpioProp = fdt_getprop_w (Overlay, Node, "gpios", NULL);
    ASSERT (GpioProp != NULL);

    *GpioProp = cpu_to_fdt32 (GpioPhandle);
  }

  SetOverlayFragmentTarget (Overlay, "/fragment@0",
    FixedPcdGetPtr (PcdI2c0Parent));

  SetOverlayFragmentTarget (Overlay, "/fragment@1",
    FixedPcdGetPtr (PcdSpiParent));
}

/**
  Apply the mezzanine's DT overlay

  @param[in]      This      Pointer to the MEZZANINE_PROTOCOL instance.
  @param[in,out]  Dtb       Pointer to the device tree blob

  @return   EFI_SUCCESS     Operation succeeded.
  @return   other           An error has occurred.
**/
STATIC
EFI_STATUS
ApplyDeviceTreeOverlay (
  IN      MEZZANINE_PROTOCOL    *This,
  IN  OUT VOID                  *Dtb
  )
{
  VOID            *Overlay;
  UINTN           OverlaySize;
  EFI_STATUS      Status;
  INT32           Err;
  UINTN           Index;

  //
  // Load the raw overlay DTB image from the raw section of this FFS file.
  //
  for (Index = 0;; Index++) {
    Status = GetSectionFromFv (&gEfiCallerIdGuid,
               EFI_SECTION_RAW, Index, &Overlay, &OverlaySize);
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }
    if (!fdt_check_header (Overlay)) {
      break;
    }
  }

  //
  // Fix up unresolved references in the overlay.
  //
  FixupOverlay (Dtb, Overlay);

  //
  // Merge the overlay with the DTB
  //
  Err = fdt_overlay_apply (Dtb, Overlay);
  if (Err) {
    DEBUG ((DEBUG_ERROR, "%a: fdt_overlay_apply() failed - %a\n",
      __FUNCTION__, fdt_strerror (Err)));
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

/**
  Install the mezzanine's SSDT table

  @param[in]      This      Pointer to the MEZZANINE_PROTOCOL instance.
  @param[in]      Dtb       Pointer to the device tree blob

  @return   EFI_SUCCESS     Operation succeeded.
  @return   other           An error has occurred.
**/
STATIC
EFI_STATUS
EFIAPI
InstallSsdtTable (
  IN      MEZZANINE_PROTOCOL        *This,
  IN      EFI_ACPI_TABLE_PROTOCOL   *AcpiProtocol
  )
{
  EFI_ACPI_DESCRIPTION_HEADER   *Ssdt;
  UINTN                         SsdtSize;
  EFI_STATUS                    Status;
  UINTN                         Index;
  UINTN                         TableKey;

  //
  // Load SSDT table from the raw section of this FFS file.
  //
  for (Index = 0;; Index++) {
    Status = GetSectionFromFv (&gEfiCallerIdGuid, EFI_SECTION_RAW, Index,
               (VOID **)&Ssdt, &SsdtSize);
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }
    if (SsdtSize >= sizeof (EFI_ACPI_DESCRIPTION_HEADER) &&
        Ssdt->OemTableId == SECURE96_SSDT_OEM_TABLE_ID) {
      break;
    }
  }
  return AcpiProtocol->InstallAcpiTable (AcpiProtocol, Ssdt, SsdtSize,
                         &TableKey);
}

STATIC MEZZANINE_PROTOCOL mMezzanine = {
  ApplyDeviceTreeOverlay,
  InstallSsdtTable,
  ARRAY_SIZE (mI2c0Devices),
  0,
  mI2c0Devices,
  NULL,
  NULL,
};

EFI_STATUS
EFIAPI
Secure96DxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS              Status;
  LS_CONNECTOR_PROTOCOL   *LsConnector;

  Status = gBS->LocateProtocol (&g96BoardsLsConnectorProtocolGuid, NULL,
                  (VOID **)&LsConnector);
  ASSERT_EFI_ERROR (Status);

  if (LsConnector->MezzanineType != MezzanineSecure96) {
    return EFI_NOT_FOUND;
  }

  return gBS->InstallProtocolInterface (&ImageHandle,
                &g96BoardsMezzanineProtocolGuid,
                EFI_NATIVE_INTERFACE,
                &mMezzanine);
}
