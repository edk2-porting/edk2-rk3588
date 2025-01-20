/** @file

  Copyright (c) 2018, Linaro. All rights reserved.
  Copyright (c) 2021, Intel Corporation. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __PLATFORM_DW_MMC_H__
#define __PLATFORM_DW_MMC_H__

typedef enum {
  RemovableSlot,
  EmbeddedSlot,
  SharedBusSlot,
  UnknownSlot
} EFI_SD_MMC_SLOT_TYPE;

typedef enum {
  UnknownCardType,
  SdCardType,
  SdioCardType,
  MmcCardType,
  EmmcCardType
} SD_MMC_CARD_TYPE;

typedef struct {
  UINT32        DefaultSpeed : 1;  // bit 0
  UINT32        HighSpeed    : 1;  // bit 1
  UINT32        Sdr12        : 1;  // bit 2
  UINT32        Sdr25        : 1;  // bit 3
  UINT32        Sdr50        : 1;  // bit 4
  UINT32        Sdr104       : 1;  // bit 5
  UINT32        Ddr50        : 1;  // bit 6
  UINT32        SysBus64     : 1;  // bit 7
  UINT32        BusWidth     : 4;  // bit 11:8
  UINT32        SlotType     : 2;  // bit 13:12
  UINT32        CardType     : 3;  // bit 16:14
  UINT32        Voltage18    : 1;  // bit 17
  UINT32        Voltage30    : 1;  // bit 18
  UINT32        Voltage33    : 1;  // bit 19
  UINT32        BaseClkFreq;
  EFI_HANDLE    Controller;
} DW_MMC_HC_SLOT_CAP;

//
// Protocol interface structure
//
typedef struct _PLATFORM_DW_MMC_PROTOCOL PLATFORM_DW_MMC_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *PLATFORM_DW_MMC_GET_CAPABILITY)(
  IN     EFI_HANDLE             Controller,
  IN     UINT8                  Slot,
  OUT DW_MMC_HC_SLOT_CAP     *Capability
  );

typedef
BOOLEAN
(EFIAPI *PLATFORM_DW_MMC_CARD_DETECT)(
  IN EFI_HANDLE                 Controller,
  IN UINT8                      Slot
  );

struct _PLATFORM_DW_MMC_PROTOCOL {
  PLATFORM_DW_MMC_GET_CAPABILITY    GetCapability;
  PLATFORM_DW_MMC_CARD_DETECT       CardDetect;
};

extern EFI_GUID  gPlatformDwMmcProtocolGuid;

#endif /* __PLATFORM_DW_MMC_H__ */
