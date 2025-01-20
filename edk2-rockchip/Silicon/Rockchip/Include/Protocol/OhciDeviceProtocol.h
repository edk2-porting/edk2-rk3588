/** @file

  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _OHCI_DEVICE_PROTOCOL_H_
#define _OHCI_DEVICE_PROTOCOL_H_

#include <Uefi/UefiBaseType.h>

#define OHCI_DEVICE_PROTOCOL_GUID \
  {0x54bce5e6, 0xbaae, 0x488a, {0x82, 0x67, 0xc0, 0x85, 0x7f, 0xb4, 0xe8, 0x05}}

typedef struct {
  UINT32    BaseAddress;
} OHCI_DEVICE_PROTOCOL;

extern EFI_GUID  gOhciDeviceProtocolGuid;

#endif // _OHCI_DEVICE_PROTOCOL_H_
