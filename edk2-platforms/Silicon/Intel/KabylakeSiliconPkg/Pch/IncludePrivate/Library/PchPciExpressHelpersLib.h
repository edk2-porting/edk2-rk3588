/** @file
  Header file for PCH PCI Express helpers library

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
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
T_POWER_ON GetTpoCapability (
  UINTN  DeviceBase,
  UINT32 L1ssCapOffset
  );

T_POWER_ON GetTpo (
  UINTN  DeviceBase,
  UINT32 L1ssCapOffset
  );

/*
  Sets Tpower_on in a device

  According to spec, Tpower_on can only be updated while L1_2 is disabled
  @param[in] DeviceBase       device base address
  @param[in] L1ssCapOffset    offset to L1substates capability in device's extended config space
  @param[in] Tpo              value to be programmed into Tpower_on
*/
VOID SetTpo (
  UINTN      DeviceBase,
  UINT32     L1ssCapOffset,
  T_POWER_ON Tpo
  );

/*
  Converts Tpower_on from value:scale notation to microseconds
*/
UINT32 TpoToUs (
  T_POWER_ON Tpo
  );

/**
  Find the Offset to a given Capabilities ID
  CAPID list:
    0x01 = PCI Power Management Interface
    0x04 = Slot Identification
    0x05 = MSI Capability
    0x10 = PCI Express Capability

  @param[in] Bus                  Pci Bus Number
  @param[in] Device               Pci Device Number
  @param[in] Function             Pci Function Number
  @param[in] CapId                CAPID to search for

  @retval 0                       CAPID not found
  @retval Other                   CAPID found, Offset of desired CAPID
**/
UINT8
PcieFindCapId (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT8   CapId
  );

/**
  Search and return the offset of desired Pci Express Capability ID
  CAPID list:
    0x0001 = Advanced Error Rreporting Capability
    0x0002 = Virtual Channel Capability
    0x0003 = Device Serial Number Capability
    0x0004 = Power Budgeting Capability

  @param[in] Bus                  Pci Bus Number
  @param[in] Device               Pci Device Number
  @param[in] Function             Pci Function Number
  @param[in] CapId                Extended CAPID to search for

  @retval 0                       CAPID not found
  @retval Other                   CAPID found, Offset of desired CAPID
**/
UINT16
PcieFindExtendedCapId (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT16  CapId
  );

/**
  This returns ClkReq Number from Port Number

  @param[in]    PortIndex     PCIe Port Number (Zero Base. Please use 23 for GBe)

  @retval       ClkReq Number
**/
UINT8
GetPortClkReqNumber (
  IN      UINT8           PortIndex
  );

/**
  Set Common clock to Root port and Endpoint PCI device

  @param[in] Bus1                 Root port Pci Bus Number
  @param[in] Device1              Root port Pci Device Number
  @param[in] Function1            Root port Pci Function Number
  @param[in] Bus2                 Endpoint Pci Bus Number
  @param[in] Device2              Endpoint Pci Device Number

  @exception EFI_UNSUPPORTED      Unsupported operation.
  @retval EFI_SUCCESS             VC mapping correctly initialized
**/
EFI_STATUS
PcieSetCommonClock (
  IN UINT8   Bus1,
  IN UINT8   Device1,
  IN UINT8   Function1,
  IN UINT8   Bus2,
  IN UINT8   Device2
  );

/**
  This function enables the CLKREQ# PM on all the end point functions

  @param[in] Bus                  Pci Bus Number
  @param[in] Device               Pci Device Number
  @param[in] RootDevice           Rootport Device Number
  @param[in] RootFunction         Rootport Function Number

  @retval None
**/
VOID
PcieSetClkreq (
  IN  UINT8   EndPointBus,
  IN  UINT8   EndPointDevice,
  IN  UINT8   RootDevice,
  IN  UINT8   RootFunction
  );

/**
  This function get or set the Max Payload Size on all the end point functions

  @param[in] EndPointBus          The Bus Number of the Endpoint
  @param[in] EndPointDevice       The Device Number of the Endpoint
  @param[in] MaxPayload           The Max Payolad Size of the root port
  @param[in] Operation            True:  Set the Max Payload Size on all the end point functions
                                  False: Get the Max Payload Size on all the end point functions

  @retval EFI_SUCCESS             Successfully completed.
**/
EFI_STATUS
PcieMaxPayloadSize (
  IN      UINT8           EndPointBus,
  IN      UINT8           EndPointDevice,
  IN OUT  UINT16          *MaxPayload,
  IN      BOOLEAN         Operation
  );

/**
  This function disable the forwarding of EOI messages unless it discovers
  an IOAPIC behind this root port.

  @param[in] RootBus              The Bus Number of the root port
  @param[in] RootDevice           The Device Number of the root port
  @param[in] RootFunction         The Function Number of the root port
  @param[in] EndPointBus          The Bus Number of the Endpoint
  @param[in] EndPointDevice       The Device Number of the Endpoint

  @exception EFI_UNSUPPORTED      Unsupported operation.
  @retval EFI_SUCCESS             Successfully completed.
**/
EFI_STATUS
PcieSetEoiFwdDisable (
  IN  UINT8   RootBus,
  IN  UINT8   RootDevice,
  IN  UINT8   RootFunction,
  IN  UINT8   EndPointBus,
  IN  UINT8   EndPointDevice
  );


/**
  Initializes the root port and its down stream devices

  @param[in]  RootPortBus           Pci Bus Number of the root port
  @param[in]  RootPortDevice        Pci Device Number of the root port
  @param[in]  RootPortFunc          Pci Function Number of the root port
  @param[in]  TempBusNumberMin      Minimal temp bus number that can be assigned to the root port (as secondary
                                    bus number) and its down stream switches
  @param[in]  TempBusNumberMax      Maximal temp bus number that can be assigned to the root port (as subordinate
                                    bus number) and its down stream switches
  @param[in] EnableCpm              Enables Clock Power Management; even if disabled, CLKREQ# can still be used by L1 PM substates mechanism

  @retval EFI_SUCCESS             Successfully completed
  @retval EFI_NOT_FOUND           Can not find device.
**/
EFI_STATUS
PchPcieInitRootPortDownstreamDevices (
  IN UINT8                          RootPortBus,
  IN UINT8                          RootPortDevice,
  IN UINT8                          RootPortFunc,
  IN UINT8                          TempBusNumberMin,
  IN UINT8                          TempBusNumberMax,
  IN BOOLEAN                        EnableCpm
  );

/**
  Get current PCIe link speed.

  @param[in] RpBase    Root Port base address
  @return Link speed
**/
UINT32
GetLinkSpeed (
  UINTN  RpBase
  );

/**
  Get max PCIe link speed supported by the root port.

  @param[in] RpBase    Root Port base address
  @return Max link speed
**/
UINT32
GetMaxLinkSpeed (
  UINTN  RpBase
  );

/**
  Get Pch Maximum Pcie Controller Number

  @retval Pch Maximum Pcie Controller Number
**/
UINT8
EFIAPI
GetPchMaxPcieControllerNum (
  VOID
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

/**
  Returns the PCIe controller configuration (4x1, 1x2-2x1, 2x2, 1x4)

  @param[in] ControllerIndex        Number of PCIe controller (0 based)

  @retval PCIe controller configuration
**/
PCIE_CONTROLLER_CONFIG
GetPcieControllerConfig (
  IN     UINT32        ControllerIndex
  );
#endif // _PEI_DXE_SMM_PCH_PCI_EXPRESS_HELPERS_LIB_H_
