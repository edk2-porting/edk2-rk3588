/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#ifndef __MDIO_H__
#define __MDIO_H__

#define MARVELL_MDIO_PROTOCOL_GUID { 0x40010b03, 0x5f08, 0x496a, { 0xa2, 0x64, 0x10, 0x5e, 0x72, 0xd3, 0x71, 0xaa }}

typedef struct _MARVELL_MDIO_PROTOCOL MARVELL_MDIO_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *MARVELL_MDIO_READ) (
  IN CONST MARVELL_MDIO_PROTOCOL *This,
  IN UINT32 PhyAddr,
  IN UINT32 MdioIndex,
  IN UINT32 RegOff,
  IN UINT32 *Data
  );

typedef
EFI_STATUS
(EFIAPI *MARVELL_MDIO_WRITE) (
  IN CONST MARVELL_MDIO_PROTOCOL *This,
  IN UINT32 PhyAddr,
  IN UINT32 MdioIndex,
  IN UINT32 RegOff,
  IN UINT32 Data
  );

struct _MARVELL_MDIO_PROTOCOL {
  MARVELL_MDIO_READ Read;
  MARVELL_MDIO_WRITE Write;
  UINTN *BaseAddresses;
  UINTN ControllerCount;
};

extern EFI_GUID gMarvellMdioProtocolGuid;
#endif
