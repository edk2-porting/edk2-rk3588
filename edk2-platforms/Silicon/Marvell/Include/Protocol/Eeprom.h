/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#ifndef __MARVELL_EEPROM_H__
#define __MARVELL_EEPROM_H__

#define MARVELL_EEPROM_PROTOCOL_GUID { 0x71954bda, 0x60d3, 0x4ef8, { 0x8e, 0x3c, 0x0e, 0x33, 0x9f, 0x3b, 0xc2, 0x2b }}

typedef struct _MARVELL_EEPROM_PROTOCOL MARVELL_EEPROM_PROTOCOL;

#define EEPROM_READ   0x1
#define EEPROM_WRITE  0x0
typedef
EFI_STATUS
(EFIAPI *EFI_EEPROM_TRANSFER) (
  IN CONST MARVELL_EEPROM_PROTOCOL *This,
  IN UINT16 Address,
  IN UINT32 Length,
  IN UINT8 *Buffer,
  IN UINT8 Operation
  );

struct _MARVELL_EEPROM_PROTOCOL {
  EFI_EEPROM_TRANSFER Transfer;
  UINT32 Identifier;
};

extern EFI_GUID gMarvellEepromProtocolGuid;
#endif
