/*++

Copyright (c) 2006 - 2009, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2016, Hisilicon Limited. All rights reserved.<BR>
Copyright (c) 2016, Linaro Limited. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    MiscSystemSlotDesignationData.c

Abstract:

  This file provide OEM to define Smbios Type09 Data

**/

#include "SmbiosMisc.h"


//
// Static (possibly build generated) Bios Vendor data.
//
MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE9, MiscSystemSlotDesignationPcie0) = {
  {                                                     // Hdr
    EFI_SMBIOS_TYPE_SYSTEM_SLOTS,                         // Type,
    0,                                                    // Length,
    0                                                     // Handle
  },
  1,                                                    // SlotDesignation
  SlotTypePciExpressX8,     // SlotType
  SlotDataBusWidth8X,       // SlotDataBusWidth
  SlotUsageOther,           // SlotUsage
  SlotLengthOther,          // SlotLength
  0x0000,                   // SlotId
  {                         // SlotCharacteristics1
    0,                      // CharacteristicsUnknown  :1;
    0,                      // Provides50Volts         :1;
    0,                      // Provides33Volts         :1;
    0,                      // SharedSlot              :1;
    0,                      // PcCard16Supported       :1;
    0,                      // CardBusSupported        :1;
    0,                      // ZoomVideoSupported      :1;
    0                       // ModemRingResumeSupported:1;
  },
  {                         // SlotCharacteristics2
    0,                      // PmeSignalSupported      :1;
    0,                      // HotPlugDevicesSupported  :1;
    0,                      // SmbusSignalSupported    :1;
    0                       // Reserved                :5;
  },
  0x00,                     // SegmentGroupNum
  0x00,                     // BusNum
  0                         // DevFuncNum
};

MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE9, MiscSystemSlotDesignationPcie1) = {
  {                                                     // Hdr
    EFI_SMBIOS_TYPE_SYSTEM_SLOTS,                         // Type,
    0,                                                    // Length,
    0                                                     // Handle
  },
  1,                                                    // SlotDesignation
  SlotTypePciExpressX8,     // SlotType
  SlotDataBusWidth8X,       // SlotDataBusWidth
  SlotUsageAvailable,       // SlotUsage
  SlotLengthOther,          // SlotLength
  0x0001,                   // SlotId
  {                         // SlotCharacteristics1
    0,                      // CharacteristicsUnknown  :1;
    0,                      // Provides50Volts         :1;
    0,                      // Provides33Volts         :1;
    0,                      // SharedSlot              :1;
    0,                      // PcCard16Supported       :1;
    0,                      // CardBusSupported        :1;
    0,                      // ZoomVideoSupported      :1;
    0                       // ModemRingResumeSupported:1;
  },
  {                         // SlotCharacteristics2
    0,                      // PmeSignalSupported      :1;
    0,                      // HotPlugDevicesSupported  :1;
    0,                      // SmbusSignalSupported    :1;
    0                       // Reserved                :5;
  },
  0x00,                     // SegmentGroupNum
  0x40,                     // BusNum
  0                         // DevFuncNum
};

MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE9, MiscSystemSlotDesignationPcie2) = {
  {                                                     // Hdr
    EFI_SMBIOS_TYPE_SYSTEM_SLOTS,                         // Type,
    0,                                                    // Length,
    0                                                     // Handle
  },
  1,                                                    // SlotDesignation
  SlotTypePciExpressX8,     // SlotType
  SlotDataBusWidth8X,       // SlotDataBusWidth
  SlotUsageOther,           // SlotUsage
  SlotLengthOther,          // SlotLength
  0x0002,                   // SlotId
  {                         // SlotCharacteristics1
    0,                      // CharacteristicsUnknown  :1;
    0,                      // Provides50Volts         :1;
    0,                      // Provides33Volts         :1;
    0,                      // SharedSlot              :1;
    0,                      // PcCard16Supported       :1;
    0,                      // CardBusSupported        :1;
    0,                      // ZoomVideoSupported      :1;
    0                       // ModemRingResumeSupported:1;
  },
  {                         // SlotCharacteristics2
    0,                      // PmeSignalSupported      :1;
    0,                      // HotPlugDevicesSupported  :1;
    0,                      // SmbusSignalSupported    :1;
    0                       // Reserved                :5;
  },
  0x00,                     // SegmentGroupNum
  0x80,                     // BusNum
  0                         // DevFuncNum
};

MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE9, MiscSystemSlotDesignationPcie3) = {
  {                                                     // Hdr
    EFI_SMBIOS_TYPE_SYSTEM_SLOTS,                         // Type,
    0,                                                    // Length,
    0                                                     // Handle
  },
  1,                                                    // SlotDesignation
  SlotTypePciExpressX4,     // SlotType
  SlotDataBusWidth4X,       // SlotDataBusWidth
  SlotUsageOther,           // SlotUsage
  SlotLengthOther,          // SlotLength
  0x0003,                   // SlotId
  {                         // SlotCharacteristics1
    0,                      // CharacteristicsUnknown  :1;
    0,                      // Provides50Volts         :1;
    0,                      // Provides33Volts         :1;
    0,                      // SharedSlot              :1;
    0,                      // PcCard16Supported       :1;
    0,                      // CardBusSupported        :1;
    0,                      // ZoomVideoSupported      :1;
    0                       // ModemRingResumeSupported:1;
  },
  {                         // SlotCharacteristics2
    0,                      // PmeSignalSupported      :1;
    0,                      // HotPlugDevicesSupported  :1;
    0,                      // SmbusSignalSupported    :1;
    0                       // Reserved                :5;
  },
  0x00,                     // SegmentGroupNum
  0xC0,                     // BusNum
  0                         // DevFuncNum
};

/* eof - MiscSystemSlotsData.c */
