/** @file
  Header file for PCH PCI Express helpers library

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_PCI_EXPRESS_HELPERS_LIB_H_
#define _PCH_PCI_EXPRESS_HELPERS_LIB_H_

#include <PchPolicyCommon.h>

typedef enum {
  TpoScale2us,
  TpoScale10us,
  TpoScale100us,
  TpoScaleMax
} T_PO_SCALE;

typedef struct {
  UINT32     Value;
  T_PO_SCALE Scale;
} T_POWER_ON;

//
// Function prototypes
//

/**
  Get PCIe port number for enabled port.
  @param[in] RpBase    Root Port pci segment base address
  @return Root Port number (1 based)
**/
UINT32
PciePortNum (
  IN     UINT64  RpBase
  );

/**
  Get PCIe root port index
  @param[in] RpBase    Root Port pci segment base address
  @return Root Port index (0 based)
**/
UINT32
PciePortIndex (
  IN     UINT64  RpBase
  );

/**
  Translate PCIe Port/Lane pair to 0-based PCIe lane number.

  @param[in] RpIndex    Root Port index
  @param[in] RpLane     Root Port Lane (0-3)

  @retval PCIe lane number (0-based)
**/
UINT32
PchPciePhysicalLane (
  UINT32 RpIndex,
  UINT32 RpLane
  );

/**
  Checks if lane reversal is enabled on a given root port

  @param[in] RpIndex  Root port index (0-based)

  @retval TRUE if lane reversal is enbabled, FALSE otherwise
**/
BOOLEAN
IsPcieLaneReversalEnabled (
  IN     UINT32  RpIndex
  );

/**
  Calculates the index of the first port on the same controller.

  @param[in] RpIndex     Root Port Number (0-based)

  @retval Index of the first port on the first controller.
**/
UINT32
PchGetPcieFirstPortIndex (
  IN     UINT32  RpIndex
  );

/*
  Returns Tpower_on capability of device

  @param[in] DeviceBase       device's PCI segment base address
  @param[in] L1ssCapOffset    offset to L1substates capability in device's extended config space

  @retval                     structure containing Tpoweron scale and value
*/
T_POWER_ON
GetTpoCapability (
  UINT64 DeviceBase,
  UINT32 L1ssCapOffset
  );

/*
  Converts Tpower_on from value:scale notation to microseconds

  @param[in] TpoScale   T power on scale
  @param[in] TpoValue   T power on value

  @retval    number of microseconds
*/
UINT32
TpoToUs (
  UINT32 TpoScale,
  UINT32 TpoValue
  );

/**
  Find the Offset to a given Capabilities ID
  CAPID list:
    0x01 = PCI Power Management Interface
    0x04 = Slot Identification
    0x05 = MSI Capability
    0x10 = PCI Express Capability

  @param[in] DeviceBase           device's base address
  @param[in] CapId                CAPID to search for

  @retval 0                       CAPID not found
  @retval Other                   CAPID found, Offset of desired CAPID
**/
UINT8
PcieBaseFindCapId (
  IN UINT64  DeviceBase,
  IN UINT8   CapId
  );

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
  );

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
  );

/**
  Search and return the offset of desired Pci Express Capability ID
  CAPID list:
    0x0001 = Advanced Error Rreporting Capability
    0x0002 = Virtual Channel Capability
    0x0003 = Device Serial Number Capability
    0x0004 = Power Budgeting Capability

  @param[in] Segment              Pci Segment Number
  @param[in] Bus                  Pci Bus Number
  @param[in] Device               Pci Device Number
  @param[in] Function             Pci Function Number
  @param[in] CapId                Extended CAPID to search for

  @retval 0                       CAPID not found
  @retval Other                   CAPID found, Offset of desired CAPID
**/
UINT16
PcieFindExtendedCapId (
  IN UINT8   Segment,
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT16  CapId
  );

/*
  Checks device's Slot Clock Configuration

  @param[in] Base            device's base address

  @retval TRUE when device device uses slot clock, FALSE otherwise
*/
BOOLEAN
GetScc (
  UINT64    Base,
  UINT8     PcieCapOffset
  );

/*
  Sets Common Clock Configuration bit for given device.

  @param[in] Base            device's base address
*/
VOID
EnableCcc (
  UINT64    Base,
  UINT8     PcieCapOffset
  );

/*
  Retrains link behind given device.
  It only makes sense to call it for downstream ports.
  If called for upstream port nothing will happen, it won't enter infinite loop.

  @param[in] Base            device's base address
*/
VOID
RetrainLink (
  UINT64  Base,
  UINT8   PcieCapOffset,
  BOOLEAN WaitUntilDone
  );

/*
  Checks if device at given address exists

  @retval TRUE when device exists; FALSE otherwise
*/
BOOLEAN
IsDevicePresent (
  UINT64 Base
  );

/*
  Checks if device is a multifunction device

  @param[in] Base            device's base address

  @retval TRUE if multifunction; FALSE otherwise
*/
BOOLEAN
IsMultifunctionDevice (
  UINT64 Base
  );

/*
  Initializes the following features in rootport and devices behind it:
  Maximum Payload Size (generic)
  Rootport packet split (proprietary)
  EonOfInterrupt forwarding (proprietary)
  Common Clock Configuration (generic)

  Generic: any code written according to PCIE Express base specification can do that.
  Proprietary: code uses registers and features that are specific to Intel silicon
  and probably only this Reference Code knows how to handle that.

  If OEM implemented generic feature enabling in his platform code or trusts Operating System
  to do it, then those features can be deleted from here.

  CCC requires link retrain, which takes a while. CCC must happen before L0s/L1 programming.
  If there was guarantee no code would access PCI while links retrain, it would be possible to skip this waiting

  @param[in] RpSegment  address of rootport on PCIe
  @param[in] RpBus      address of rootport on PCIe
  @param[in] RpDevice   address of rootport on PCIe
  @param[in] RpFunction address of rootport on PCIe
  @param[in] BusMin     minimum Bus number that can be assigned below this rootport
  @param[in] BusMax     maximum Bus number that can be assigned below this rootport
*/
VOID
RootportDownstreamConfiguration (
  UINT8                     RpSegment,
  UINT8                     RpBus,
  UINT8                     RpDevice,
  UINT8                     RpFunction,
  UINT8                     BusMin,
  UINT8                     BusMax
  );

/*
  Configures the following power-management related features in rootport and devices behind it:
  LTR limit (generic)
  LTR override (proprietary)
  Clock Power Management (generic)
  L1 substates (generic except for the override table)
  L1.LOW substate (proprietary)
  L0s and L1 (generic)

  Generic: any code written according to PCIE Express base specification can do that.
  Proprietary: code uses registers and features that are specific to Intel silicon
  and probably only this Reference Code knows how to handle that.

  If OEM implemented generic feature enabling in his platform code or trusts Operating System
  to do it, then those features can be deleted from here.

  @param[in] RpSegment                address of rootport on PCIe
  @param[in] RpBus                    address of rootport on PCIe
  @param[in] RpDevice                 address of rootport on PCIe
  @param[in] RpFunction               address of rootport on PCIe
  @param[in] BusLimit                 maximum Bus number that can be assigned below this rootport
  @param[in] AspmOverrideTableSize    size of override array
  @param[in] AspmOverrideTable        array of device that need exceptions in configuration
*/
VOID
RootportDownstreamPmConfiguration (
  UINT8                     RpSegment,
  UINT8                     RpBus,
  UINT8                     RpDevice,
  UINT8                     RpFunction,
  UINT8                     BusMin,
  UINT8                     BusMax,
  PCH_PCIE_ROOT_PORT_CONFIG *RpConfig,
  UINT32                    AspmOverrideTableSize,
  PCH_PCIE_DEVICE_OVERRIDE  *AspmOverrideTable
  );

/**
  Get current PCIe link speed.

  @param[in] RpBase    Root Port base address
  @return Link speed
**/
UINT32
GetLinkSpeed (
  UINT64  RpBase
  );

/**
  Get max PCIe link speed supported by the root port.

  @param[in] RpBase    Root Port pci segment base address
  @return Max link speed
**/
UINT32
GetMaxLinkSpeed (
  UINT64 RpBase
  );

/**
  PCIe controller configuration.
**/
typedef enum {
  Pcie4x1      = 0,
  Pcie1x2_2x1  = 1,
  Pcie2x2      = 2,
  Pcie1x4      = 3
} PCIE_CONTROLLER_CONFIG;

#endif // _PEI_DXE_SMM_PCH_PCI_EXPRESS_HELPERS_LIB_H_
