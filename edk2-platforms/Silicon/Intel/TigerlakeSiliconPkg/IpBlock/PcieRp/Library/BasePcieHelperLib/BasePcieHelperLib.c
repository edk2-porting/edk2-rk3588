/** @file
  This file contains routines that support PCI Express initialization

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Library/PcieHelperLib.h>
#include <Library/BaseMemoryLib.h>
#include <PcieRegs.h>

#define ASPM_L1_NO_LIMIT 0xFF
#define LINK_RETRAIN_WAIT_TIME 1000 // microseconds

/**
  Finds the Offset to a given Capabilities ID
  Each capability has an ID and a pointer to next Capability, so they form a linked list.
  This function walks the list of Capabilities present in device's pci cfg. If requested capability
  can be found, its offset is returned.
  If the capability can't be found or if device doesn't exist, function returns 0
  CAPID list:
    0x01 = PCI Power Management Interface
    0x04 = Slot Identification
    0x05 = MSI Capability
    0x10 = PCI Express Capability

  @param[in] DeviceBase           device's base address
  @param[in] CapId                CAPID to search for

  @retval 0                       CAPID not found (this includes situation where device doesn't exit)
  @retval Other                   CAPID found, Offset of desired CAPID
**/
UINT8
PcieBaseFindCapId (
  IN UINT64  DeviceBase,
  IN UINT8   CapId
  )
{
  UINT8  CapHeaderOffset;
  UINT8  CapHeaderId;
  UINT16 Data16;
  //
  // We do not explicitly check if device exists to save time and avoid unnecessary PCI access
  // If the device doesn't exist, check for CapHeaderId != 0xFF will fail and function will return offset 0
  //
  if ((PciSegmentRead8 (DeviceBase + PCI_PRIMARY_STATUS_OFFSET) & EFI_PCI_STATUS_CAPABILITY) == 0x00) {
    ///
    /// Function has no capability pointer
    ///
    return 0;
  } else {
    ///
    /// Check the header layout to determine the Offset of Capabilities Pointer Register
    ///
    if ((PciSegmentRead8 (DeviceBase + PCI_HEADER_TYPE_OFFSET) & HEADER_LAYOUT_CODE) == (HEADER_TYPE_CARDBUS_BRIDGE)) {
      ///
      /// If CardBus bridge, start at Offset 0x14
      ///
      CapHeaderOffset = EFI_PCI_CARDBUS_BRIDGE_CAPABILITY_PTR;
    } else {
      ///
      /// Otherwise, start at Offset 0x34
      ///
      CapHeaderOffset = PCI_CAPBILITY_POINTER_OFFSET;
    }
    ///
    /// Get Capability Header, A pointer value of 00h is used to indicate the last capability in the list.
    ///
    CapHeaderId     = 0;
    CapHeaderOffset = PciSegmentRead8 (DeviceBase + CapHeaderOffset) & ((UINT8) ~(BIT0 | BIT1));
    while (CapHeaderOffset != 0 && CapHeaderId != 0xFF) {
      Data16 = PciSegmentRead16 (DeviceBase + CapHeaderOffset);
      CapHeaderId = (UINT8)(Data16 & 0xFF);
      if (CapHeaderId == CapId) {
        if (CapHeaderOffset > PCI_MAXLAT_OFFSET) {
          ///
          /// Return valid capability offset
          ///
          return CapHeaderOffset;
        } else {
          ASSERT ((FALSE));
          return 0;
        }
      }
      ///
      /// Each capability must be DWORD aligned.
      /// The bottom two bits of all pointers (including the initial pointer at 34h) are reserved
      /// and must be implemented as 00b although software must mask them to allow for future uses of these bits.
      ///
      CapHeaderOffset = (UINT8)(Data16 >> 8);
    }
    return 0;
  }
}

/**
  Find the Offset to a given Capabilities ID
  CAPID list:
    0x01 = PCI Power Management Interface
    0x04 = Slot Identification
    0x05 = MSI Capability
    0x10 = PCI Express Capability

  @param[in] Segment              Pci Segment Number
  @param[in] Bus                  Pci Bus Number
  @param[in] Device               Pci Device Number
  @param[in] Function             Pci Function Number
  @param[in] CapId                CAPID to search for

  @retval 0                       CAPID not found
  @retval Other                   CAPID found, Offset of desired CAPID
**/
UINT8
PcieFindCapId (
  IN UINT8   Segment,
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT8   CapId
  )
{
  UINT64 DeviceBase;

  DeviceBase = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, Device, Function, 0);
  return PcieBaseFindCapId (DeviceBase, CapId);
}

/**
  Search and return the offset of desired Pci Express Capability ID
  CAPID list:
    0x0001 = Advanced Error Reporting Capability
    0x0002 = Virtual Channel Capability
    0x0003 = Device Serial Number Capability
    0x0004 = Power Budgeting Capability

  @param[in] DeviceBase           device base address
  @param[in] CapId                Extended CAPID to search for

  @retval 0                       CAPID not found, this includes situation where device doesn't exist
  @retval Other                   CAPID found, Offset of desired CAPID
**/
UINT16
PcieBaseFindExtendedCapId (
  IN UINT64  DeviceBase,
  IN UINT16  CapId
  )
{
  UINT16  CapHeaderOffset;
  UINT16  CapHeaderId;
  ///
  /// Start to search at Offset 0x100
  /// Get Capability Header, A pointer value of 00h is used to indicate the last capability in the list.
  ///
  CapHeaderId     = 0;
  CapHeaderOffset = R_PCIE_CFG_EXCAP_OFFSET;
  while (CapHeaderOffset != 0 && CapHeaderId != MAX_UINT16) {
    CapHeaderId = PciSegmentRead16 (DeviceBase + CapHeaderOffset);
    if (CapHeaderId == CapId) {
      return CapHeaderOffset;
    }
    ///
    /// Each capability must be DWORD aligned.
    /// The bottom two bits of all pointers are reserved and must be implemented as 00b
    /// although software must mask them to allow for future uses of these bits.
    ///
    CapHeaderOffset = (PciSegmentRead16 (DeviceBase + CapHeaderOffset + 2) >> 4) & ((UINT16) ~(BIT0 | BIT1));
  }

  return 0;
}

/**
  Search and return the offset of desired Pci Express Capability ID
  CAPID list:
    0x0001 = Advanced Error Reporting Capability
    0x0002 = Virtual Channel Capability
    0x0003 = Device Serial Number Capability
    0x0004 = Power Budgeting Capability

  @param[in] Segment              Pci Segment Number
  @param[in] Bus                  Pci Bus Number
  @param[in] Device               Pci Device Number
  @param[in] Function             Pci Function Number
  @param[in] CapId                Extended CAPID to search for

  @retval 0                       CAPID not found, this includes situation where device doesn't exist
  @retval Other                   CAPID found, Offset of desired CAPID
**/
UINT16
PcieFindExtendedCapId (
  IN UINT8   Segment,
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT16  CapId
  )
{
  UINT64  DeviceBase;

  DeviceBase = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, Device, Function, 0);
  return PcieBaseFindExtendedCapId (DeviceBase, CapId);
}

/**
  Checks if PCI device at given address exists

  @param[in] Base            device's base address

  @retval TRUE if exists
**/
BOOLEAN
IsDevicePresent (
  UINT64 Base
  )
{
  if (PciSegmentRead16 (Base) == 0xFFFF) {
    return FALSE;
  }
  return TRUE;
}

/**
  Checks if device is a multifunction device
  Besides comparing Multifunction bit (BIT7) it checks if contents of HEADER_TYPE register
  make sense (header != 0xFF) to prevent false positives when called on devices which do not exist

  @param[in] Base            device's base address

  @retval TRUE if multifunction; FALSE otherwise
**/
BOOLEAN
IsMultifunctionDevice (
  UINT64 Base
  )
{
  UINT8 HeaderType;
  HeaderType = PciSegmentRead8(Base + PCI_HEADER_TYPE_OFFSET);
  if ((HeaderType == 0xFF) || ((HeaderType & HEADER_TYPE_MULTI_FUNCTION) == 0)) {
    return FALSE;
  }
  return TRUE;
}

/**
  Checks device's Slot Clock Configuration

  @param[in] Base            device's base address
  @param[in] PcieCapOffset   devices Pci express capability list register offset

  @retval TRUE when device uses slot clock, FALSE otherwise
**/
BOOLEAN
GetScc (
  UINT64    Base,
  UINT8     PcieCapOffset
  )
{
  return !!(PciSegmentRead16 (Base + PcieCapOffset + R_PCIE_LSTS_OFFSET) & B_PCIE_LSTS_SCC);
}

/**
  Sets Common Clock Configuration bit for given device.

  @param[in] PcieCapOffset   devices Pci express capability list register offset
  @param[in] Base            device's base address
**/
VOID
EnableCcc (
  UINT64    Base,
  UINT8     PcieCapOffset
  )
{
  PciSegmentOr8 (Base + PcieCapOffset + R_PCIE_LCTL_OFFSET, B_PCIE_LCTL_CCC);
}

/**
  Retrains link behind given device.
  It only makes sense to call it for downstream ports. If called for upstream port nothing will happen.
  If WaitUntilDone is TRUE function will wait until link retrain had finished, otherwise it will return immediately.
  Link must finish retrain before software can access the device on the other side. If it's not going to access it
  then considerable time can be saved by not waiting here.

  @param[in] Base             device's base address
  @param[in] PcieCapOffset    devices Pci express capability list register offset
  @param[in] WaitUntilDone    when TRUE, function waits until link has retrained
**/
VOID
RetrainLink (
  UINT64  Base,
  UINT8   PcieCapOffset,
  BOOLEAN WaitUntilDone
  )
{
  UINT16 LinkTraining;
  UINT32 TimeoutUs;

  TimeoutUs = LINK_RETRAIN_WAIT_TIME;
  //
  // Before triggering link retrain make sure it's not already retraining. Otherwise
  // settings recently entered in LCTL register might go unnoticed
  //
  do {
    LinkTraining = (PciSegmentRead16 (Base + PcieCapOffset + R_PCIE_LSTS_OFFSET) & B_PCIE_LSTS_LT);
    TimeoutUs--;
  } while (LinkTraining && (TimeoutUs != 0));

  PciSegmentOr8 (Base + PcieCapOffset + R_PCIE_LCTL_OFFSET, B_PCIE_LCTL_RL);

  TimeoutUs = LINK_RETRAIN_WAIT_TIME;
  if (WaitUntilDone) {
    do {
      LinkTraining = (PciSegmentRead16 (Base + PcieCapOffset + R_PCIE_LSTS_OFFSET) & B_PCIE_LSTS_LT);
      TimeoutUs--;
    } while (LinkTraining && (TimeoutUs != 0));
  }
}
