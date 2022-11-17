/** @file
  Support PCI chipset initialization.

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef  _EFI_PCI_SUPPORT_H_
#define  _EFI_PCI_SUPPORT_H_

#include <Protocol/PciRootBridgeIo.h>

#include <Guid/SetupVariable.h>

typedef struct {
  UINT8  PcieCapPtr;
  UINT8  Function;
  UINT8  Device;
  UINT8  Bus;
  UINT16 PcieLnkCap;
  UINT16 PcieDevCap;
  //Added to Support AtomicOp Request-->Start
  UINT16 PcieDevCap2;
  //Added to Support AtomicOp Request-->End
} PCIE_CAP_INFO;

typedef struct {
  INTN            Top;
  PCIE_CAP_INFO   PcieCapInfo[FixedPcdGet32(PcdMaxNestedLevel)];
} PCIE_STACK;

extern  PCIE_STACK  mPcieStack;

BOOLEAN
IsPciDevicePresent (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *PciRootBridgeIo,
  OUT PCI_TYPE00                      *Pci,
  UINT8                               Bus,
  UINT8                               Device,
  UINT8                               Func
  );


#endif
