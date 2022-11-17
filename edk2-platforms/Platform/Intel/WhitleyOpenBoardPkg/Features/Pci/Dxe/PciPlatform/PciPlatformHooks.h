/** @file
  This code supports a the private implementation
  of the Legacy BIOS Platform protocol

  @copyright
  Copyright 2004 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCI_PLATFORM_HOOKS_H_
#define _PCI_PLATFORM_HOOKS_H_

#include <Library/IoLib.h>

VOID
ChipsetCallback (
  IN  EFI_HANDLE                                    RootBridgeHandle,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS   PciAddress,
  IN  EFI_PCI_ENUMERATION_PHASE                     Phase,
  EFI_PCI_CALLBACK_CONTEXT                          *Context
  );

EFI_STATUS
PciTreeTraverse (
  IN  UINT8                                        Socket,
  IN  UINT8                                        Stack,
  IN  UINT8                                        StartBus
  );

#endif
