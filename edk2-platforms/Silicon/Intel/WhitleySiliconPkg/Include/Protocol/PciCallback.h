/** @file

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _EFI_PCI_CALLBACK_H
#define _EFI_PCI_CALLBACK_H

#include <Include/IndustryStandard/Pci22.h>
#include <Protocol/CpuIo2.h>
#include <Protocol/PciRootBridgeIo.h>


//
// Global Id for PCI callback
//
#define EFI_PCI_CALLBACK_PROTOCOL_GUID \
  { \
    0x1ca0e202, 0xfe9e, 0x4776, 0x9f, 0xaa, 0x57, 0xc, 0x19, 0x61, 0x7a, 0x06 \
  }

typedef struct _EFI_PCI_CALLBACK_PROTOCOL EFI_PCI_CALLBACK_PROTOCOL;

typedef enum {
  EfiPciEnumerationDeviceScanning    = 1,
  EfiPciEnumerationBusNumberAssigned = 2,
  EfiPciEnumerationResourceAssigned  = 4,
} EFI_PCI_ENUMERATION_PHASE;

typedef struct {
  PCI_TYPE00                              PciHeader;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *PciRootBridgeIo;
  EFI_CPU_IO2_PROTOCOL                    *CpuIo;
} EFI_PCI_CALLBACK_CONTEXT;

typedef
VOID
(EFIAPI *EFI_PCI_CALLBACK_FUNC) (
  IN  EFI_HANDLE                                   RootBridgeHandle,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS  PciAddress,
  IN  EFI_PCI_ENUMERATION_PHASE                    Phase,
  IN  EFI_PCI_CALLBACK_CONTEXT                     *Context
);

typedef
EFI_STATUS
(EFIAPI *EFI_REGISTER_PCI_CALLBACK) (
  IN  EFI_PCI_CALLBACK_PROTOCOL                   *This,
  IN  EFI_PCI_CALLBACK_FUNC                        Function,
  IN  EFI_PCI_ENUMERATION_PHASE                    Phase
)
/*++

Routine Description:

  Register a callback during PCI bus enumeration

Arguments:

  This       -  Protocol instance pointer.
  Function   -  Callback function pointer.
  Phase      -  PCI enumeration phase.

Returns:

  EFI_SUCCESS           - Function has registed successfully
  EFI_UNSUPPORTED       - The function has been regisered
  EFI_InVALID_PARAMETER - The parameter is incorrect

--*/
;

//
// Protocol definition
//
typedef struct _EFI_PCI_CALLBACK_PROTOCOL {
  EFI_REGISTER_PCI_CALLBACK   RegisterPciCallback;
} EFI_PCI_CALLBACK_PROTOCOL;

extern EFI_GUID gEfiPciCallbackProtocolGuid;

#endif
