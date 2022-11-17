/** @file
 *
 *  Copyright (c) 2020, Andrei Warkentin <andrey.warkentin@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include "ConfigDxe.h"
#include <IndustryStandard/Pci.h>
#include <Protocol/PciIo.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#pragma pack(1)
typedef struct {
  UINT8 ProgInterface;
  UINT8 SubClassCode;
  UINT8 BaseCode;
} USB_CLASSC;
#pragma pack()

STATIC VOID *mPciIoNotificationRegistration = NULL;

STATIC
VOID
EFIAPI
PciIoNotificationEvent (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                     Status;
  EFI_PCI_IO_PROTOCOL            *PciIo;
  USB_CLASSC                     UsbClassCReg;
  UINTN                          SegmentNumber;
  UINTN                          BusNumber;
  UINTN                          DeviceNumber;
  UINTN                          FunctionNumber;
  RASPBERRY_PI_FIRMWARE_PROTOCOL *FwProtocol = Context;

  Status = gBS->LocateProtocol (&gEfiPciIoProtocolGuid,
                                mPciIoNotificationRegistration, (VOID **)&PciIo);
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = PciIo->Pci.Read (PciIo,
                            EfiPciIoWidthUint8,
                            PCI_CLASSCODE_OFFSET,
                            sizeof (USB_CLASSC) / sizeof (UINT8),
                            &UsbClassCReg
                            );

  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Test whether the controller belongs to Xhci type
  //
  if ((UsbClassCReg.BaseCode != PCI_CLASS_SERIAL) ||
      (UsbClassCReg.SubClassCode != PCI_CLASS_SERIAL_USB) ||
      (UsbClassCReg.ProgInterface != PCI_IF_XHCI)) {
    return;
  }

  Status = PciIo->GetLocation (PciIo, &SegmentNumber, &BusNumber,
                               &DeviceNumber, &FunctionNumber);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a: failed to get SBDF for xHCI controller: %r\n",
            __FUNCTION__, Status));
    return;
  }

  DEBUG ((DEBUG_INFO, "xHCI found at %u:%u:%u:%u\n",
          SegmentNumber, BusNumber, DeviceNumber, FunctionNumber));

  ASSERT (SegmentNumber == 0);
  Status = FwProtocol->NotifyXhciReset(BusNumber, DeviceNumber, FunctionNumber);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a: couldn't signal xHCI firmware load: %r\n",
            __FUNCTION__, Status));
  }
}

VOID
RegisterXhciQuirkHandler (
  IN RASPBERRY_PI_FIRMWARE_PROTOCOL *FwProtocol
  )
{
  EfiCreateProtocolNotifyEvent (&gEfiPciIoProtocolGuid,
                                TPL_NOTIFY,
                                PciIoNotificationEvent,
                                FwProtocol,
                                &mPciIoNotificationRegistration
                                );
}
