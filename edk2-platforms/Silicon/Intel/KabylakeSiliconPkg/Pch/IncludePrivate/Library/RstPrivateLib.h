/** @file
  Header file for Rst Library

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _RST_PRIVATE_LIB_
#define _RST_PRIVATE_LIB_
#include <Uefi/UefiBaseType.h>
#include <Library/PchInfoLib.h>
#include <PchLimits.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/MmPciLib.h>
#include <Library/PchInitCommonLib.h>
#include <PcieRegs.h>

#define RST_PCIE_STORAGE_INTERFACE_NONE             0
#define RST_PCIE_STORAGE_INTERFACE_AHCI             1
#define RST_PCIE_STORAGE_INTERFACE_NVME             2

#define RST_STONY_BEACH_VENDOR_ID       0x8086
#define RST_STONY_BEACH_DEVICE_ID       0x2522

#define PCI_CLASS_MASS_STORAGE_NVME     0x08
#define PCI_CLASS_MASS_STORAGE_AHCI     PCI_CLASS_MASS_STORAGE_SATADPA


/**
  Check if there is a remap capable storage device present on Root Port

  @param[in]  RpNumber               Number of a Root Port to check
  @param[in]  TempPciBus             Temporary Pci Bus Number

  @retval  UINT8                     Device Programming interface, 0 if no remap capable device present

**/
UINT8
RstGetProgInterfaceForRemapDevice (
  IN UINT32        RpNumber,
  IN UINT32        TempPciBus
  );

/**
  Check the lane occupied by certain root port according to the root port number and configuration strap
  Return 8-bit bitmap where each bit represents the lane number (e.g.: return 00000011b means the root port owns 2 lane)

  @param[in] RootPortNum            Root Port Number

  @retval UINT8                     Lane Occupied by the Root Port (bitmap)
**/
UINT8
RstGetRpLaneOccupyMask (
  IN  UINT32                   RootPortNum
  );


/**
  Checks PCH generation and returns SATA's GCR.PLS bit number according to root port number with a PCIe storage device
  connected to.

  @param[in] RootPortNum        Root port number which PCIe storage device is connected to

  @retval UINT32                Number of GCR.PLS bit representing root port
**/
UINT32
RstPlsBitNumber (
  IN UINT32           RootPortNum
  );

/**
  Checks if device with given PCI config space address is Intel's Stony Beach.

  @param[in] EndPointPciBase    Address of device's PCI config space

  @retval BOOLEAN               TRUE if device is Intel's Stony Beach, FALSE othrwise
**/
BOOLEAN
RstIsStonyBeach (
  IN UINTN            EndPointPciBase
  );
#endif
