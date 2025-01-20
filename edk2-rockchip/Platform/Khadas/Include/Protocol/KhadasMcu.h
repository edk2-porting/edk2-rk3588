/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __KHADAS_MCU_H__
#define __KHADAS_MCU_H__

#define KHADAS_MCU_PROTOCOL_GUID \
    { 0x162d2a6e, 0x33af, 0x4ca5, { 0xaa, 0x68, 0xbc, 0xd3, 0xb2, 0x4c, 0xca, 0xf5 } }

typedef struct _KHADAS_MCU_PROTOCOL KHADAS_MCU_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *KHADAS_MCU_SET_FAN_SPEED_PERCENTAGE)(
  IN KHADAS_MCU_PROTOCOL      *This,
  IN UINT8                    Percentage
  );

struct _KHADAS_MCU_PROTOCOL {
  KHADAS_MCU_SET_FAN_SPEED_PERCENTAGE    SetFanSpeedPercentage;
};

extern EFI_GUID  gKhadasMcuProtocolGuid;

#endif // __KHADAS_MCU_H__
