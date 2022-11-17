/** @file
  Simics Platform definitions

  Copyright (C) 2015, Red Hat, Inc.
  Copyright (c) 2014, Gabriel L. Somlo <somlo@cmu.edu>
  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __SIMICS_PLATFORMS_H__
#define __SIMICS_PLATFORMS_H__

#include <Library/PciLib.h>
#include <IndustryStandard/Pci22.h>
#include <Register/X58Ich10.h>
#include <IndustryStandard/I440FxPiix4.h>

//
// Simics Host Bridge DID Address
//
#define SIMICS_HOSTBRIDGE_DID \
  PCI_LIB_ADDRESS (0, 0, 0, PCI_DEVICE_ID_OFFSET)

//
// Simics SideBand PCI device registers
//
#define SIMICS_SIDEBANDPCI_DEV  0
#define SIMICS_SIDEBANDPCI_FUNC 7
#define SIMICS_SIDEBANDPCI_SVID \
  PCI_LIB_ADDRESS (0, 0, 7, PCI_SVID_OFFSET)
#define SIMICS_SIDEBANDPCI_SDID \
  PCI_LIB_ADDRESS (0, 0, 7, PCI_SID_OFFSET)
#define SIMICS_SIDEBANDPCI_CAP \
  PCI_LIB_ADDRESS (0, 0, 7, PCI_CAPBILITY_POINTER_OFFSET)
#define SIMICS_SIDEBANDPCI_CAP_Offset 0x40
#define SIMICS_SIDEBANDPCI_CAP_ID     0xFF

//
// Values we program into the PM base address registers
//
#define PIIX4_PMBA_VALUE  0xB000
#define ICH10_PMBASE_VALUE 0x0400

//
// Common bits in same-purpose registers
//
#define PMBA_RTE BIT0

//
// Common IO ports relative to the Power Management Base Address
//
#define ACPI_TIMER_OFFSET 0x8

#endif
