/** @file
  Implementation the driver used to add SPCR device info to ACPI table.
  SPCR is abbreviation of Serial Port Console Redirection Table (SPCR).

  Copyright (c) 2004 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SpcrAcpi.h"

#define PCANSITYPE                0
#define VT100TYPE                 1
#define VT100PLUSTYPE             2
#define VTUTF8TYPE                3

#define PCI_MAX_BAR_NO            6

UINT8  TerminalTypeValue[4] =
{
  PCANSITYPE,
  VT100TYPE,
  VT100PLUSTYPE,
  VTUTF8TYPE
};

///
/// Guid for messaging path, used in Serial port setting.
///
EFI_GUID  TerminalTypeGuid[4] = {
  DEVICE_PATH_MESSAGING_PC_ANSI,
  DEVICE_PATH_MESSAGING_VT_100,
  DEVICE_PATH_MESSAGING_VT_100_PLUS,
  DEVICE_PATH_MESSAGING_VT_UTF8
};

//
// Serial Port Console Redirection Table
// Please modify all values in Spcr.h only.
//
EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE gSpcrInfo = {
  {
    EFI_ACPI_2_0_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_SIGNATURE,
    sizeof (EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE),
    EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_REVISION,

    //
    // Checksum will be updated at runtime
    //
    0x00,

    //
    // It is expected that these values will be programmed at runtime
    //
    {'I', 'N', 'T', 'E', 'L', ' '},

    SIGNATURE_64 ('S', 'P', 'C', 'R', 'T', 'a', 'b', 'l'),
    0x00,               // Spcr revision,
    SIGNATURE_32 ('I', 'N', 'T', 'L'),
    0x01
  },

  //
  // Beginning of SPCR specific fields
  //
  0x00,               //INTERFACE_TYPE,
  {
    0x00,               //RESERVED_BYTE,
    0x00,               //RESERVED_BYTE,
    0x00               //RESERVED_BYTE,
  },

  //
  // Base Address Block
  //
  {
    EFI_ACPI_3_0_SYSTEM_IO,
    0x08,               //BASE_ADDRESS_BIT_WIDTH,
    0x00,               //BASE_ADDRESS_BIT_OFFSET,
    0x00,               //RESERVED_BYTE,
    0x0000000000000000  //BASE_ADDRESS_ADDRESS,
  },

  0x03,               //INTERRUPT_TYPE,
  0x04,               //IRQ,
  0x04,               //GLOBAL_SYSTEM_INTERRUPT,
  0x07,               //BAUD_RATE,
  0x00,               //PARITY,
  0x01,               //STOP_BITS,
  0x00,               //FLOW_CONTROL,
  0x03,               //TERMINAL_TYPE,
  0x00,               //LANGUAGE,
  0xFFFF,             //PCI_DEVICE_ID,
  0xFFFF,             //PCI_VENDOR_ID,
  0x00,               //PCI_BUS_NUMBER,
  0x00,               //PCI_DEVICE_NUMBER,
  0x00,               //PCI_FUNCTION_NUMBER,
  0x00000000,         //PCI_FLAGS,
  0x00,               //PCI_SEGMENT,
  EFI_ACPI_RESERVED_DWORD
};


/**

  Install Boot Device Selection Protocol

  @param ImageHandle     The image handle.
  @param SystemTable     The system table.

  @retval  EFI_SUCEESS  BDS has finished initializing.
                        Return the dispatcher and recall BDS.Entry
  @retval  Other        Return status from AllocatePool() or gBS->InstallProtocolInterface

**/
EFI_STATUS
EFIAPI
SpcrInitialize (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_EVENT                       ReadyToBootEvent;
  //
  // Install protocol interface
  //
  Status = EfiCreateEventReadyToBootEx (
            TPL_CALLBACK,
            OutOfBandACPITableConstruction,
            NULL,
            &ReadyToBootEvent
            );
  return Status;
}

/**
  Check whether the device path node is ISA Serial Node.

  @param DevicePath     Device path node to be checked

  @retval TRUE          It's ISA Serial Node.
  @retval FALSE         It's NOT ISA Serial Node.

**/
BOOLEAN
HasIsaSerialNode (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *Node;
  ACPI_HID_DEVICE_PATH      *Acpi;

  for (Node = DevicePath; !IsDevicePathEnd (Node); Node = NextDevicePathNode (Node)) {
    Acpi = (ACPI_HID_DEVICE_PATH *) Node;
    if ((DevicePathType (Acpi) == ACPI_DEVICE_PATH) && (DevicePathSubType (Acpi) == ACPI_DP) &&
        (ReadUnaligned32 (&Acpi->HID) == EISA_PNP_ID (0x0501))) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Get Irq and resource info from isa type console device.

  @param SavedDevicePath           Device path for isa device.

**/
VOID
GetIsaTypeInfo (
  IN EFI_DEVICE_PATH_PROTOCOL *SavedDevicePath
  )
{
  EFI_STATUS               Status;
  EFI_HANDLE               Handle;
  ACPI_RESOURCE_HEADER_PTR Resources;
  EFI_SIO_PROTOCOL         *Sio;
  EFI_ACPI_IRQ_NOFLAG_DESCRIPTOR   *Irq;
  EFI_ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR *FixedLocation;

    //
    // Try to get IRP and BaseAddress info.
    //
    Status = gBS->LocateDevicePath(
                    &gEfiSioProtocolGuid,
                    &SavedDevicePath,
                    &Handle
                    );
    if (EFI_ERROR(Status)) {
      return;
    }

    Status = gBS->HandleProtocol (
                    Handle,
                    &gEfiSioProtocolGuid,
                    (VOID **) &Sio
                    );
    if (EFI_ERROR(Status)) {
      return;
    }

    Status = Sio->GetResources (Sio, &Resources);
    if (EFI_ERROR(Status)) {
      return;
    }

    while (Resources.SmallHeader->Byte != ACPI_END_TAG_DESCRIPTOR) {
      switch (Resources.SmallHeader->Byte) {
        case ACPI_IRQ_DESCRIPTOR:
        case ACPI_IRQ_NOFLAG_DESCRIPTOR:
          Irq = (EFI_ACPI_IRQ_NOFLAG_DESCRIPTOR *) Resources.SmallHeader;
          gSpcrInfo.Irq = 0;
          while (Irq->Mask > 1) {
            gSpcrInfo.Irq ++;
            Irq->Mask = Irq->Mask >> 1;
          }
          break;
        case ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR:
          FixedLocation = (EFI_ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR *)Resources.SmallHeader;
          gSpcrInfo.BaseAddress.Address = FixedLocation->BaseAddress;
          break;
        default:
          break;
      }

      if (Resources.SmallHeader->Bits.Type == 0) {
        Resources.SmallHeader = (ACPI_SMALL_RESOURCE_HEADER *) ((UINT8 *) Resources.SmallHeader
                                + Resources.SmallHeader->Bits.Length
                                + sizeof (*Resources.SmallHeader));
      } else {
        Resources.LargeHeader = (ACPI_LARGE_RESOURCE_HEADER *) ((UINT8 *) Resources.LargeHeader
                                + Resources.LargeHeader->Length
                                + sizeof (*Resources.LargeHeader));
      }
    }
}

/**
  Get Irq and resource info from Pci type console device.

  @param SavedDevicePath           Device path for PCI device.

**/
VOID
GetPciTypeInfo (
  IN EFI_DEVICE_PATH_PROTOCOL *SavedDevicePath
  )
{
  EFI_STATUS               Status;
  EFI_HANDLE               PciHandle;
  EFI_PCI_IO_PROTOCOL      *PciIo;
  UINTN                    Bus;
  UINTN                    Device;
  UINTN                    Function;
  UINTN                    SegNum;
  UINTN                    Irq;
  UINT16                   BaseAddress;
  UINT8                    Count;
  UINT64                   Supports;
  UINT16                   VendorId;
  UINT16                   DeviceId;
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR   *Resources;

  PciIo     = NULL;
  Resources = NULL;
  Supports  = 0x01;
  Bus       = 0;
  Device    = 0;
  Function  = 0;
  SegNum    = 0;
  Irq       = 0;
  BaseAddress = 0;
  VendorId  = 0;
  DeviceId  = 0;

  Status = gBS->LocateDevicePath (&gEfiPciIoProtocolGuid, &SavedDevicePath, (VOID **) &PciHandle);
  if (EFI_ERROR (Status)) {
    Status = EFI_NOT_FOUND;
    return;
  }

  Status = gBS->HandleProtocol(PciHandle, &gEfiPciIoProtocolGuid, (VOID **) &PciIo);
  if (EFI_ERROR (Status) || (PciIo == NULL)) {
    Status = EFI_NOT_FOUND;
    return;
  }

  PciIo->GetLocation(PciIo, &SegNum, &Bus, &Device, &Function);
  if (EFI_ERROR (Status)) {
    Status = EFI_NOT_FOUND;
    return;
  }

  if (Bus == 0x00 && Device == 0x16 && Function == 3)
  {
    for (Count = 0; Count < PCI_MAX_BAR_NO; Count++) {
      Status  = PciIo->GetBarAttributes (PciIo, Count, &Supports, (VOID **) &Resources);
      if (!EFI_ERROR(Status)) {
        if (Resources->ResType == 1) { // TYPE_IO
          BaseAddress= (UINT16)Resources->AddrRangeMin;
          break;
        }
        FreePool (Resources);
        Resources = NULL;
      }
    }
  }

  Status = PciIo->Pci.Read(PciIo, EfiPciIoWidthUint8, 0x3c, 1, &Irq);
  if (EFI_ERROR(Status)) {
    goto Done;
  }

  Status = PciIo->Pci.Read(PciIo, EfiPciIoWidthUint16, 0x00, 1, &VendorId);
  if (EFI_ERROR(Status)) {
    goto Done;
  }

  Status = PciIo->Pci.Read(PciIo, EfiPciIoWidthUint16, 0x02, 1, &DeviceId);
  if (EFI_ERROR(Status)) {
    goto Done;
  }

  gSpcrInfo.BaseAddress.Address = BaseAddress;
  gSpcrInfo.Irq = (UINT8) Irq;

  gSpcrInfo.PciDeviceId       = DeviceId;
  gSpcrInfo.PciVendorId       = VendorId;
  gSpcrInfo.PciBusNumber      = (UINT8) Bus;
  gSpcrInfo.PciDeviceNumber   = (UINT8) Device;
  gSpcrInfo.PciFunctionNumber = (UINT8) Function;
  gSpcrInfo.PciSegment        = (UINT8) SegNum;

Done:
  if (Resources != NULL) {
    FreePool (Resources);
  }
}

/**
  Installs the Smbios Table to the System Table. This function gets called
  when the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled

  @param  Event                The event to signal
  @param  Context              Event contex

**/
VOID
EFIAPI
OutOfBandACPITableConstruction (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_DEVICE_PATH_PROTOCOL *SavedDevicePath;
  EFI_DEVICE_PATH_PROTOCOL *TmpDevicePath;
  EFI_DEVICE_PATH_PROTOCOL *Tmp2DevicePath;
  EFI_ACPI_TABLE_PROTOCOL  *AcpiTablProtocol;
  UINTN                    TurnKey;
  EFI_STATUS               Status;
  EFI_HANDLE               Handle;
  EFI_SERIAL_IO_PROTOCOL   *SerialIo;
  UINT32                   FlowControl;
  VENDOR_DEVICE_PATH       *Vendor;
  UINT8                    Index;

  Handle          = NULL;

  gBS->CloseEvent (Event);

  SavedDevicePath = GetSpcrDevice();
  if (SavedDevicePath == NULL) {
    return;

  }
  //
  // Get TerminalType info from the last device path node.
  //
  TmpDevicePath  = SavedDevicePath;
  Tmp2DevicePath = TmpDevicePath;
  while (!IsDevicePathEnd (TmpDevicePath)) {
    Tmp2DevicePath = TmpDevicePath;
    TmpDevicePath  = NextDevicePathNode(TmpDevicePath);
  }
  Vendor = (VENDOR_DEVICE_PATH *) Tmp2DevicePath;
  for (Index=0; Index < 4; Index ++) {
    if (CompareGuid (&Vendor->Guid, &TerminalTypeGuid[Index])) {
      gSpcrInfo.TerminalType = TerminalTypeValue[Index];
      break;
    }
  }
  SetDevicePathEndNode (Tmp2DevicePath);
  TmpDevicePath = SavedDevicePath;
  //
  // Get Console attribute from the serial io protocol.
  //
  Status = gBS->LocateDevicePath(
                  &gEfiSerialIoProtocolGuid,
                  &TmpDevicePath,
                  &Handle
                  );
  if (EFI_ERROR(Status)) {
    goto out;
  }

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiSerialIoProtocolGuid,
                  (VOID **) &SerialIo
                  );
  if (EFI_ERROR(Status)) {
    goto out;
  }

  switch (SerialIo->Mode->BaudRate) {
    case 9600:
      gSpcrInfo.BaudRate = 3;
      break;
    case 19200:
      gSpcrInfo.BaudRate = 4;
      break;
    case 57600:
      gSpcrInfo.BaudRate = 6;
      break;
    case 115200:
    default:
      gSpcrInfo.BaudRate = 7;
      break;
  }

  gSpcrInfo.FlowControl = 0;
  SerialIo->GetControl(SerialIo, &FlowControl);
  if ((FlowControl & EFI_SERIAL_HARDWARE_FLOW_CONTROL_ENABLE) != 0) {
    gSpcrInfo.FlowControl = UART_FLOW_CONTROL_HARDWARE;
  }

  if (HasIsaSerialNode(SavedDevicePath)) {
    GetIsaTypeInfo (SavedDevicePath);
  } else {
    GetPciTypeInfo (SavedDevicePath);
  }
  //
  // Not create before, create new Spcr ACPI table.
  //
  Status = gBS->LocateProtocol (
                  &gEfiAcpiTableProtocolGuid,
                  NULL,
                  (VOID**)&AcpiTablProtocol
                  );
  if (EFI_ERROR (Status)) {
    goto out;
  }

  //
  // Add table
  //
  Status = AcpiTablProtocol->InstallAcpiTable (
                                  AcpiTablProtocol,
                                  &gSpcrInfo,
                                  sizeof (EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE),
                                  &TurnKey
                                  );
  ASSERT_EFI_ERROR (Status);

out:
  FreePool(SavedDevicePath);
}
