/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Library/PciSegmentLib.h>
#include <Library/PciSegmentInfoLib.h>
#include <IndustryStandard/Pci.h>

#pragma pack(1)

//
// Data region after PCI configuration header(for cardbus bridge)
//
typedef struct {
  UINT16  SubVendorId;  // Subsystem Vendor ID
  UINT16  SubSystemId;  // Subsystem ID
  UINT32  LegacyBase;   // Optional 16-Bit PC Card Legacy
  // Mode Base Address
  //
  UINT32  Data[46];
} PCI_CARDBUS_DATA;

typedef union {
  PCI_DEVICE_HEADER_TYPE_REGION Device;
  PCI_BRIDGE_CONTROL_REGISTER   Bridge;
  PCI_CARDBUS_CONTROL_REGISTER  CardBus;
} NON_COMMON_UNION;

typedef struct {
  PCI_DEVICE_INDEPENDENT_REGION Common;
  NON_COMMON_UNION              NonCommon;
  UINT32                        Data[48];
} PCI_CONFIG_SPACE;

#pragma pack()

VOID
DumpPciDevice (
  IN UINT8                             Bus,
  IN UINT8                             Device,
  IN UINT8                             Function,
  IN PCI_TYPE00                        *PciData
  )
{
//DEBUG ((DEBUG_INFO, "  00/00/00 : [0000][0000] [00|00|00] 00000000 00000000 00000000 00000000 00000000 00000000 0000\n"));
  DEBUG ((DEBUG_INFO, "  %02x/%02x/%02x :",
    Bus,
    Device,
    Function
    ));
  DEBUG ((DEBUG_INFO, " [%04x][%04x]",
    PciData->Hdr.VendorId,
    PciData->Hdr.DeviceId
    ));
  DEBUG ((DEBUG_INFO, " [%02x|%02x|%02x]",
    PciData->Hdr.ClassCode[2],
    PciData->Hdr.ClassCode[1],
    PciData->Hdr.ClassCode[0]
    ));
  DEBUG ((DEBUG_INFO, " %08x %08x %08x %08x %08x %08x",
    PciData->Device.Bar[0],
    PciData->Device.Bar[1],
    PciData->Device.Bar[2],
    PciData->Device.Bar[3],
    PciData->Device.Bar[4],
    PciData->Device.Bar[6]
    ));
  DEBUG ((DEBUG_INFO, " %04x\n",
    PciData->Hdr.Command
    ));
}

VOID
DumpPciBridge (
  IN UINT8                             Bus,
  IN UINT8                             Device,
  IN UINT8                             Function,
  IN PCI_TYPE01                        *PciData
  )
{
//DEBUG ((DEBUG_INFO, "  00/00/00*: [0000][0000] [00|00|00] 00000000 00000000 [00|00|00] [00:00] [0000:0000] [0000:0000] [00000000:00000000] [0000:0000] 0000   0000\n"));
  DEBUG ((DEBUG_INFO, "  %02x/%02x/%02x*:",
    Bus,
    Device,
    Function
    ));
  DEBUG ((DEBUG_INFO, " [%04x][%04x]",
    PciData->Hdr.VendorId,
    PciData->Hdr.DeviceId
    ));
  DEBUG ((DEBUG_INFO, " [%02x|%02x|%02x]",
    PciData->Hdr.ClassCode[2],
    PciData->Hdr.ClassCode[1],
    PciData->Hdr.ClassCode[0]
    ));
  DEBUG ((DEBUG_INFO, " %08x %08x",
    PciData->Bridge.Bar[0],
    PciData->Bridge.Bar[1]
    ));
  DEBUG ((DEBUG_INFO, " [%02x|%02x|%02x]",
    PciData->Bridge.PrimaryBus,
    PciData->Bridge.SecondaryBus,
    PciData->Bridge.SubordinateBus
    ));
  DEBUG ((DEBUG_INFO, " [00:00] [0000:0000] [0000:0000]",
    PciData->Bridge.IoBase,
    PciData->Bridge.IoLimit,
    PciData->Bridge.MemoryBase,
    PciData->Bridge.MemoryLimit,
    PciData->Bridge.PrefetchableMemoryBase,
    PciData->Bridge.PrefetchableMemoryLimit
    ));
  DEBUG ((DEBUG_INFO, " [00000000:00000000] [0000:0000]",
    PciData->Bridge.PrefetchableBaseUpper32,
    PciData->Bridge.PrefetchableLimitUpper32,
    PciData->Bridge.IoBaseUpper16,
    PciData->Bridge.IoLimitUpper16
    ));
  DEBUG ((DEBUG_INFO, " %04x  ",
    PciData->Bridge.BridgeControl
    ));
  DEBUG ((DEBUG_INFO, " %04x\n",
    PciData->Hdr.Command
    ));
}

/**
  This function gets the protocol interface from the given handle, and
  obtains its address space descriptors.

  @param[in] Handle          The PCI_ROOT_BRIDIGE_IO_PROTOCOL handle.
  @param[out] IoDev          Handle used to access configuration space of PCI device.
  @param[out] Descriptors    Points to the address space descriptors.

  @retval EFI_SUCCESS     The command completed successfully
**/
EFI_STATUS
PciGetProtocolAndResource (
  IN  EFI_HANDLE                            Handle,
  OUT EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL       **IoDev,
  OUT EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR     **Descriptors
  )
{
  EFI_STATUS  Status;

  //
  // Get inferface from protocol
  //
  Status = gBS->HandleProtocol (
                Handle,
                &gEfiPciRootBridgeIoProtocolGuid,
                (VOID**)IoDev
               );

  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Call Configuration() to get address space descriptors
  //
  Status = (*IoDev)->Configuration (*IoDev, (VOID**)Descriptors);
  if (Status == EFI_UNSUPPORTED) {
    *Descriptors = NULL;
    return EFI_SUCCESS;

  } else {
    return Status;
  }
}

/**
  This function get the next bus range of given address space descriptors.
  It also moves the pointer backward a node, to get prepared to be called
  again.

  @param[in, out] Descriptors Points to current position of a serial of address space
                              descriptors.
  @param[out] MinBus          The lower range of bus number.
  @param[out] MaxBus          The upper range of bus number.
  @param[out] IsEnd           Meet end of the serial of descriptors.

  @retval EFI_SUCCESS     The command completed successfully.
**/
EFI_STATUS
PciGetNextBusRange (
  IN OUT EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR  **Descriptors,
  OUT    UINT16                             *MinBus,
  OUT    UINT16                             *MaxBus,
  OUT    BOOLEAN                            *IsEnd
  )
{
  *IsEnd = FALSE;

  //
  // When *Descriptors is NULL, Configuration() is not implemented, so assume
  // range is 0~PCI_MAX_BUS
  //
  if ((*Descriptors) == NULL) {
    *MinBus = 0;
    *MaxBus = PCI_MAX_BUS;
    return EFI_SUCCESS;
  }
  //
  // *Descriptors points to one or more address space descriptors, which
  // ends with a end tagged descriptor. Examine each of the descriptors,
  // if a bus typed one is found and its bus range covers bus, this handle
  // is the handle we are looking for.
  //

  while ((*Descriptors)->Desc != ACPI_END_TAG_DESCRIPTOR) {
    if ((*Descriptors)->ResType == ACPI_ADDRESS_SPACE_TYPE_BUS) {
      *MinBus = (UINT16) (*Descriptors)->AddrRangeMin;
      *MaxBus = (UINT16) (*Descriptors)->AddrRangeMax;
      (*Descriptors)++;
      return (EFI_SUCCESS);
    }

    (*Descriptors)++;
  }

  if ((*Descriptors)->Desc == ACPI_END_TAG_DESCRIPTOR) {
    *IsEnd = TRUE;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
TestPointCheckPciResource (
  VOID
  )
{
  UINT16                            Bus;
  UINT16                            Device;
  UINT16                            Func;
  UINT64                            Address;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *IoDev;
  EFI_STATUS                        Status;
  PCI_TYPE00                        PciData;
  UINTN                             Index;
  EFI_HANDLE                        *HandleBuf;
  UINTN                             HandleCount;
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *Descriptors;
  UINT16                            MinBus;
  UINT16                            MaxBus;
  BOOLEAN                           IsEnd;
  
  DEBUG ((DEBUG_INFO, "==== TestPointCheckPciResource - Enter\n"));
  HandleBuf = NULL;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciRootBridgeIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuf
                  );
  if (EFI_ERROR (Status)) {
    goto Done ;
  }

  DEBUG ((DEBUG_INFO, "  B  D  F*    VID   DID   Class[CSP]   Bar0     Bar1    Bus[PSS]   Io[BL]  Memory[BL]"));
  DEBUG ((DEBUG_INFO, " PMemory[BL]    PMemoryU[BL]       IoU[BL]   BriCtl Command\n"));

  DEBUG ((DEBUG_INFO, "  B  D  F     VID   DID   Class[CSP]   Bar0     Bar1     Bar2     Bar3     Bar4     Bar5   Command\n"));

  for (Index = 0; Index < HandleCount; Index++) {
    Status = PciGetProtocolAndResource (
               HandleBuf[Index],
               &IoDev,
               &Descriptors
               );
    while (TRUE) {
      Status = PciGetNextBusRange (&Descriptors, &MinBus, &MaxBus, &IsEnd);
      if (EFI_ERROR (Status)) {
        goto Done;
      }

      if (IsEnd) {
        break;
      }

      for (Bus = MinBus; Bus <= MaxBus; Bus++) {
        //
        // For each devices, enumerate all functions it contains
        //
        for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
          //
          // For each function, read its configuration space and print summary
          //
          for (Func = 0; Func <= PCI_MAX_FUNC; Func++) {
            Address = EFI_PCI_ADDRESS (Bus, Device, Func, 0);
            IoDev->Pci.Read (
                         IoDev,
                         EfiPciWidthUint16,
                         Address,
                         1,
                         &PciData.Hdr.VendorId
                         );

            //
            // If VendorId = 0xffff, there does not exist a device at this
            // location. For each device, if there is any function on it,
            // there must be 1 function at Function 0. So if Func = 0, there
            // will be no more functions in the same device, so we can break
            // loop to deal with the next device.
            //
            if (PciData.Hdr.VendorId == 0xffff && Func == 0) {
              break;
            }

            if (PciData.Hdr.VendorId != 0xffff) {
              IoDev->Pci.Read (
                           IoDev,
                           EfiPciWidthUint32,
                           Address,
                           sizeof (PciData) / sizeof (UINT32),
                           &PciData
                           );

              if (IS_PCI_BRIDGE(&PciData)) {
                // Bridge
                DumpPciBridge ((UINT8)Bus, (UINT8)Device, (UINT8)Func, (PCI_TYPE01 *)&PciData);
              } else if (IS_CARDBUS_BRIDGE(&PciData)) {
                // CardBus Bridge
              } else {
                // Device
                DumpPciDevice ((UINT8)Bus, (UINT8)Device, (UINT8)Func, &PciData);
              }
              
              //
              // If this is not a multi-function device, we can leave the loop
              // to deal with the next device.
              //
              if (Func == 0 && ((PciData.Hdr.HeaderType & HEADER_TYPE_MULTI_FUNCTION) == 0x00)) {
                break;
              }
            }
          }
        }
      }
      //
      // If Descriptor is NULL, Configuration() returns EFI_UNSUPPRORED,
      // we assume the bus range is 0~PCI_MAX_BUS. After enumerated all
      // devices on all bus, we can leave loop.
      //
      if (Descriptors == NULL) {
        break;
      }
    }
  }
  
Done:
  if (HandleBuf != NULL) {
    FreePool (HandleBuf);
  }

  DEBUG ((DEBUG_INFO, "==== TestPointCheckPciResource - Exit\n"));

  if (EFI_ERROR(Status)) {
    TestPointLibAppendErrorString (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      TEST_POINT_BYTE3_PCI_ENUMERATION_DONE_RESOURCE_ALLOCATED_ERROR_CODE \
        TEST_POINT_PCI_ENUMERATION_DONE \
        TEST_POINT_BYTE3_PCI_ENUMERATION_DONE_RESOURCE_ALLOCATED_ERROR_STRING
      );
  }

  return Status;
}

EFI_STATUS
TestPointCheckPciBusMaster (
  VOID
  )
{
  UINTN             Segment;
  UINTN             SegmentCount;
  UINTN             Bus;
  UINTN             Device;
  UINTN             Function;
  UINT16            VendorId;
  UINT16            Command;
  UINT8             HeaderType;
  EFI_STATUS        Status;
  PCI_SEGMENT_INFO  *PciSegmentInfo;

  PciSegmentInfo = GetPciSegmentInfo (&SegmentCount);
  if (PciSegmentInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = EFI_SUCCESS;
  for (Segment = 0; Segment < SegmentCount; Segment++) {
    for (Bus = PciSegmentInfo[Segment].StartBusNumber; Bus <= PciSegmentInfo[Segment].EndBusNumber; Bus++) {
      for (Device = 0; Device <= 0x1F; Device++) {
        for (Function = 0; Function <= 0x7; Function++) {
          VendorId = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS(PciSegmentInfo[Segment].SegmentNumber, Bus, Device, Function, PCI_VENDOR_ID_OFFSET));
          //
          // If VendorId = 0xffff, there does not exist a device at this
          // location. For each device, if there is any function on it,
          // there must be 1 function at Function 0. So if Func = 0, there
          // will be no more functions in the same device, so we can break
          // loop to deal with the next device.
          //
          if (VendorId == 0xffff && Function == 0) {
            break;
          }

          if (VendorId != 0xffff) {
            Command = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS(Segment, Bus, Device, Function, PCI_COMMAND_OFFSET));
            if ((Command & EFI_PCI_COMMAND_BUS_MASTER) != 0) {
              DEBUG ((DEBUG_INFO, "PCI BME enabled (S%04x.B%02x.D%02x.F%x - %04x)\n", Segment, Bus, Device, Function, Command));
              TestPointLibAppendErrorString (
                PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
                NULL,
                TEST_POINT_BYTE3_PCI_ENUMERATION_DONE_BUS_MASTER_DISABLED_ERROR_CODE \
                  TEST_POINT_PCI_ENUMERATION_DONE \
                  TEST_POINT_BYTE3_PCI_ENUMERATION_DONE_BUS_MASTER_DISABLED_ERROR_STRING
                );
              Status = EFI_INVALID_PARAMETER;
            }

            //
            // If this is not a multi-function device, we can leave the loop
            // to deal with the next device.
            //
            HeaderType = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS(Segment, Bus, Device, Function, PCI_HEADER_TYPE_OFFSET));
            if (Function == 0 && ((HeaderType & HEADER_TYPE_MULTI_FUNCTION) == 0x00)) {
              break;
            }
          }
        }
      }
    }
  }

  return Status;
}
