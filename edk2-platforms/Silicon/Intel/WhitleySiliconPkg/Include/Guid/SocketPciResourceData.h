/** @file
  UEFI variables used by DXE to pass PCI resource reconfiguration request to PEI.

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SOCKET_PCI_RESOURCE_CONFIG_DATA_H_
#define _SOCKET_PCI_RESOURCE_CONFIG_DATA_H_

extern EFI_GUID gEfiSocketPciResourceDataGuid;

/**
 * PCI MMIO and IO resource reconfiguration request structure.
 **/
#define SYSTEM_PCI_RESOURCE_CONFIGURATION_DATA_NAME L"SystemPciResourceConfigData"

typedef struct {
  UINT64 Base;    ///< Base (starting) address of a range (I/O, 32 and 64-bit mmio regions)
  UINT64 Limit;   ///< Limit (last valid) address of a range
} PCIE_BASE_LIMIT;

typedef struct {
  PCIE_BASE_LIMIT Io;        ///< Base and limit of I/O range assigned to entity
  PCIE_BASE_LIMIT LowMmio;   ///< Base and limit of low MMIO region for entity
  PCIE_BASE_LIMIT HighMmio;  ///< Base and limit of high (64-bit) MMIO region for entity
} PCI_BASE_LIMITS;

typedef struct {
  PCI_BASE_LIMITS SocketLimits;                     ///< Base and Limit of all PCIe resources for the socket
  PCI_BASE_LIMITS StackLimits[MAX_LOGIC_IIO_STACK]; ///< Base and Limit of all PCIe resources for each stack of the socket
} SOCKET_PCI_BASE_LIMITS;

typedef struct {
  //
  // Save basic system configuration parameters along with the resource map to detect a change.
  // Remember low and high I/O memory range when saving recource configuration. It is used to verify
  // whether system memory map changed. Remember also stacks configured when creating the map.
  // If anything changed reset the system PCI resource configuration.
  //
  UINT64                 MmioHBase;
  UINT64                 MmioHLimit;
  UINT32                 MmioLBase;
  UINT32                 MmioLLimit;
  UINT16                 IoBase;
  UINT16                 IoLimit;
  UINT16                 StackPresentBitmap[MAX_SOCKET];
  //
  // Used by the PciHostBridge DXE driver, these variables don't need to be exposed through setup options
  // The variables are used as a communication vehicle from the PciHostBridge DXE driver to an OEM hook
  // which updates the KTI resource map.
  //
  SOCKET_PCI_BASE_LIMITS Socket[MAX_SOCKET];        ///< Base and limit of all PCIe resources for each socket
} SYSTEM_PCI_BASE_LIMITS;


#endif // _SOCKET_PCI_RESOURCE_CONFIG_DATA_H_

