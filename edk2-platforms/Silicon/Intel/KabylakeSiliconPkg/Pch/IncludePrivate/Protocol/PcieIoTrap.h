/** @file
  This file defines the PCH PCIE IoTrap Protocol.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PCH_PCIE_IOTRAP_H_
#define _PCH_PCIE_IOTRAP_H_

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                       gPchPcieIoTrapProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _PCH_PCIE_IOTRAP_PROTOCOL PCH_PCIE_IOTRAP_PROTOCOL;

///
/// Pcie Trap valid types
///
typedef enum {
  PciePmTrap,
  PcieTrapTypeMaximum
} PCH_PCIE_TRAP_TYPE;

/**
 This protocol is used to provide the IoTrap address to trigger PCH PCIE call back events
**/
struct _PCH_PCIE_IOTRAP_PROTOCOL {
  UINT16      PcieTrapAddress;
};

#endif
