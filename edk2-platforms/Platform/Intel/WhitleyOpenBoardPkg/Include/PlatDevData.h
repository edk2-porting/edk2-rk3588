/** @file
  EFI Platform Device Data Definition Header File.

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _EFI_PLAT_DEVICE_DATA_H_
#define _EFI_PLAT_DEVICE_DATA_H_

#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/PciIo.h>

#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/Pci.h>


typedef struct {
  UINT8 Dev;                            // Device numbers of a chain of bridges starting at PCI Bus, behind which this device is located.
  UINT8 Fun;                            // Function numbers of a chain of bridges starting at PCI Bus, behind which this device is located.
} DEVICE_DATA_DEV_PATH;

typedef struct {
  UINT32  UID;                          // The Root Bridge ID as appears in the device path for that bridge.
} DEVICE_DATA_RBRG_PATH;

typedef struct {
  DEVICE_DATA_RBRG_PATH RootBridgePath; // Path to starting PCI Bus from which the SourceBusPath begins. This is used if there are multiple root bridges. Each such bridge will originate a lowest level PCI bus.
  DEVICE_DATA_DEV_PATH  BridgePath[4];  // Pairs of device/function numbers of a chain of bridges starting at PCI Bus, behind which this device is located. Must terminate with Dev = 0xFF. The size of 3 may be bumped up if there is more bus depth levels than 3 on a particular platform.
} DEVICE_DATA_BUS_PATH;

//
// Holds live system PCI Root Bridge info.
//
typedef struct {
  EFI_HANDLE                        Handle;       // Handle to the PCI device.
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *PciRbIoProt; // Root Bridge IO protocol.
  EFI_DEVICE_PATH_PROTOCOL          *DevPath;     // Device path to the bridge.
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *Resources;   // Bus/IO/Mem ranges exposed via the root bridge.
} DEVICE_DATA_PCI_RBRG;

//
// Holds live system PCI device info.
//
typedef struct {
  UINTN Seg;
  UINTN Bus;
  UINTN Dev;
  UINTN Fun;
} LOCATION;

typedef struct {
  EFI_HANDLE          Handle;   // Handle to the PCI device.
  EFI_PCI_IO_PROTOCOL *PciIoProt;
  LOCATION Location;               // Bus/Dev/Fun location of this device.
  PCI_TYPE_GENERIC  ConfSpace;  // First 40h bytes of PCI config space for each device.
} DEVICE_DATA_PCI_DEV;

//
// Holds live system CPU device info.
//
typedef struct {
  UINTN PackageNumber;
  UINTN CoreNumber;
  UINTN ThreadNumber;
  UINT8 Present;
  UINT8 Enabled;
  UINT8 Stepping;
  UINT8 Model;
  UINT8 Family;
  UINT8 Bsp;
  UINT8 Apic;
  UINT8 ApicId;
  UINT8 ApicVer;
  UINT8 Fpu;
  UINT8 Mce;
  UINT8 Cx8;
} DEVICE_DATA_CPU;

//
// Platform hardwired data describing all I/O APICs in the system.
//
typedef struct {
  UINT8   Enabled;
  UINT32  Address;
  UINT8   Id;
  UINT8   Version;
} DEVICE_DATA_HW_IO_APIC;

//
// Platform hardwired data describing connection of interrupt sources to local APICs.
//
typedef struct {
  DEVICE_DATA_BUS_PATH  SourceBusPath;
  UINT8                 SourceBusIrq;
  UINT8                 DestApicId;
  UINT8                 DestApicIntIn;
  UINT8                 IntType;
  UINT16                Polarity;
  UINT16                Trigger;
} DEVICE_DATA_HW_LOCAL_INT;

//
// Platform hardwired data describing the built-in devices.
//
typedef struct {
  DEVICE_DATA_BUS_PATH  BusPath;        // Path to the device, includes root bridge and P2P bridge chain.
  DEVICE_DATA_DEV_PATH  DevFun;         // Device/function number of the built-in PCI device being described. 0xff if not applicable e.g., it's an ISA device.
  UINT8                 DestApicId;     // Destination APIC.
  UINT8                 DestApicIntIn;  // The pin of the destination APIC the interrupt wire is connected to.
  UINT8                 IntType;        // As defined in the MPS.
  UINT16                Polarity;       // As defined in the MPS.
  UINT16                Trigger;        // As defined in the MPS.
} DEVICE_DATA_HW_BUILT_IN;

//
// Platform hardwired data describing the add-in devices.
// An add-in device is defined here as a pluggable into a PCI slot.
// Thus there must be as many entries as there are slots in the system.
// The devices as defined above may have any complexity (wile complying
// with the PCI spec) including possibly multiple levels of bridges and buses
// possibly with multiple devices possibly with multiple functions.
// The routing of the interrupts from such functions is governed by the
// PCI-to-PCI Bridge Architecture Specification.
// It short it requires that functions rotate mod 4 the interrupt assignments
// (A/B/C/D) with PCI devices of single function devices and that the bus depth
// does not cause such a rotation.
//
typedef struct {
  DEVICE_DATA_BUS_PATH  BusPath;          // Path to the device, includes root bridge and P2P bridge chain.
  UINT8                 Dev;              // Device number of the slot being described.
  UINT8                 DestApicId;       // Destination APIC. As defined in the MPS.
  UINT8                 DestApicIntIn[4]; // Interrupt pins to destination APIC, indexes correspond to PCI interrupt pins A/B/C/D.
} DEVICE_DATA_HW_PCI_SLOT;

//
// Platform hardwired data describing the address space mapping.
//
typedef struct {
  DEVICE_DATA_RBRG_PATH RootBridgePath;
  UINT8                 AddressType;
  UINT64                AddressBase;
  UINT64                AddressLength;
} DEVICE_DATA_HW_ADDR_SPACE_MAPPING;

//
// This is the module global containing platform device data.
//
typedef struct {
  DEVICE_DATA_HW_LOCAL_INT          *LocalIntData;
  UINTN                             LocalIntDataSize;
  DEVICE_DATA_HW_ADDR_SPACE_MAPPING *AddrDataMapping;
  UINTN                             AddrDataMappingSize;

  DEVICE_DATA_PCI_RBRG              *PciRBridgeInfo;  // Info for PCI Root Bridges in the system.
  DEVICE_DATA_PCI_DEV               *PciDevInfo;      // Info for PCI devices in the system.
  UINT8                             PciBusNo;         // Number of PCI buses. Assumes that PCI bus numbers are continous and start from 0.
  UINT8                             LegacyBusIdx;     // Bus number of the legacy bus like ISA. EISA etc. There could only be one legacy bus. It has to be the last bus after all the PCI buses.
  DEVICE_DATA_CPU                   *CpuInfo;         // Info for all processors.
  UINTN                             CpuMax;
} DEVICE_DATA;

//
// This is the module global containing platform device data.
//
typedef struct {
  DEVICE_DATA_HW_PCI_SLOT          *HwPciSlotUpdate;
  UINTN                             HwPciSlotUpdateSize;
  DEVICE_DATA_HW_BUILT_IN          *HwBuiltInUpdate;
  UINTN                             HwBuiltInUpdateSize;
} DEVICE_UPDATE_DATA;


typedef struct _MP_TABLE_CPU_INFO {
  UINT8   ApicVersion;
  UINT32  CpuSignature;
  UINT32  FeatureFlags;
} MP_TABLE_CPU_INFO;

#endif  //_EFI_PLAT_DEVICE_DATA_H_

