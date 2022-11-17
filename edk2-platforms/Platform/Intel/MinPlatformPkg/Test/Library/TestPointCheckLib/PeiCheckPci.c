/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiPei.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PciSegmentInfoLib.h>
#include <IndustryStandard/Pci.h>

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
                TEST_POINT_BYTE2_END_OF_PEI_PCI_BUS_MASTER_DISABLED_ERROR_CODE \
                  TEST_POINT_END_OF_PEI \
                  TEST_POINT_BYTE2_END_OF_PEI_PCI_BUS_MASTER_DISABLED_ERROR_STRING
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
