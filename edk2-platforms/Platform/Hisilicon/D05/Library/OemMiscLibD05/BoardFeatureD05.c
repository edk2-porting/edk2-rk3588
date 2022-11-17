/** @file
*
*  Copyright (c) 2016, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2016, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <PlatformArch.h>
#include <Uefi.h>
#include <IndustryStandard/SmBios.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/I2CLib.h>
#include <Library/IoLib.h>
#include <Library/OemMiscLib.h>
#include <Protocol/Smbios.h>


I2C_DEVICE gRtcDevice = {
  .Socket = 0,
  .Port = 4,
  .DeviceType = DEVICE_TYPE_SPD,
  .SlaveDeviceAddress = 0x68
};

SERDES_POLARITY_INVERT gSerdesPolarityTxDesc[] = {
  {SERDES_INVALID_MACRO_ID, SERDES_INVALID_LANE_NUM}
};

SERDES_POLARITY_INVERT gSerdesPolarityRxDesc[] = {
  {SERDES_INVALID_MACRO_ID, SERDES_INVALID_LANE_NUM}
};

SERDES_PARAM gSerdesParamNA = {
  .Hilink0Mode = EmHilink0Hccs1X8Width16,
  .Hilink1Mode = EmHilink1Hccs0X8Width16,
  .Hilink2Mode = EmHilink2Pcie2X8,
  .Hilink3Mode = 0x0,
  .Hilink4Mode = 0xF,
  .Hilink5Mode = EmHilink5Sas1X4,
  .Hilink6Mode = 0x0,
  .UseSsc      = 0,
};

SERDES_PARAM gSerdesParamNB = {
  .Hilink0Mode = EmHilink0Pcie1X8,
  .Hilink1Mode = EmHilink1Pcie0X8,
  .Hilink2Mode = EmHilink2Sas0X8,
  .Hilink3Mode = 0x0,
  .Hilink4Mode = 0xF,
  .Hilink5Mode = EmHilink5Pcie2X2Pcie3X2,
  .Hilink6Mode = 0xF,
  .UseSsc      = 0,
};

SERDES_PARAM gSerdesParamS1NA = {
  .Hilink0Mode = EmHilink0Hccs1X8Width16,
  .Hilink1Mode = EmHilink1Hccs0X8Width16,
  .Hilink2Mode = EmHilink2Pcie2X8,
  .Hilink3Mode = 0x0,
  .Hilink4Mode = 0xF,
  .Hilink5Mode = EmHilink5Sas1X4,
  .Hilink6Mode = 0x0,
  .UseSsc      = 0,
};

SERDES_PARAM gSerdesParamS1NB = {
  .Hilink0Mode = EmHilink0Pcie1X8,
  .Hilink1Mode = EmHilink1Pcie0X8,
  .Hilink2Mode = EmHilink2Sas0X8,
  .Hilink3Mode = 0x0,
  .Hilink4Mode = 0xF,
  .Hilink5Mode = EmHilink5Pcie2X2Pcie3X2,
  .Hilink6Mode = 0xF,
  .UseSsc      = 0,
};


EFI_STATUS
OemGetSerdesParam (
 OUT SERDES_PARAM *ParamA,
 OUT SERDES_PARAM *ParamB,
 IN  UINT32 SocketId
 )
{
  if (ParamA == NULL || ParamB == NULL) {
    DEBUG((DEBUG_ERROR, "[%a]:[%dL] Param == NULL!\n", __FUNCTION__, __LINE__));
    return EFI_INVALID_PARAMETER;
  }

  if (SocketId == 0) {
    (VOID) CopyMem(ParamA, &gSerdesParamNA, sizeof(*ParamA));
    (VOID) CopyMem(ParamB, &gSerdesParamNB, sizeof(*ParamB));
  } else {
    (VOID) CopyMem(ParamA, &gSerdesParamS1NA, sizeof(*ParamA));
    (VOID) CopyMem(ParamB, &gSerdesParamS1NB, sizeof(*ParamB));
  }

  return EFI_SUCCESS;
}

VOID
OemPcieResetAndOffReset (
  VOID
  )
{
  return;
}

SMBIOS_TABLE_TYPE9 gPcieSlotInfo[] = {
  // PCIe0 Slot 1
  {
    {                                                     // Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS,                       // Type,
      0,                                                  // Length,
      0                                                   // Handle
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
    0x00,                     // BusNum
    0                         // DevFuncNum
  },

  // PCIe0 Slot 4
  {
    {                                                  // Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS,                    // Type,
      0,                                               // Length,
      0                                                // Handle
    },
    1,                                                 // SlotDesignation
    SlotTypePciExpressX8,     // SlotType
    SlotDataBusWidth8X,       // SlotDataBusWidth
    SlotUsageAvailable,       // SlotUsage
    SlotLengthOther,          // SlotLength
    0x0004,                   // SlotId
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
  }
};


UINT8
OemGetPcieSlotNumber (
  VOID
  )
{
  return  sizeof (gPcieSlotInfo) / sizeof (SMBIOS_TABLE_TYPE9);
}

EFI_STRING_ID gDimmToDevLocator[MAX_SOCKET][MAX_CHANNEL][MAX_DIMM] = {
  {{STRING_TOKEN(STR_LEMON_C10_DIMM_000), STRING_TOKEN(STR_LEMON_C10_DIMM_001), STRING_TOKEN(STR_LEMON_C10_DIMM_002)},
   {STRING_TOKEN(STR_LEMON_C10_DIMM_010), STRING_TOKEN(STR_LEMON_C10_DIMM_011), STRING_TOKEN(STR_LEMON_C10_DIMM_012)},
   {STRING_TOKEN(STR_LEMON_C10_DIMM_020), STRING_TOKEN(STR_LEMON_C10_DIMM_021), STRING_TOKEN(STR_LEMON_C10_DIMM_022)},
   {STRING_TOKEN(STR_LEMON_C10_DIMM_030), STRING_TOKEN(STR_LEMON_C10_DIMM_031), STRING_TOKEN(STR_LEMON_C10_DIMM_032)}},

  {{STRING_TOKEN(STR_LEMON_C10_DIMM_100), STRING_TOKEN(STR_LEMON_C10_DIMM_101), STRING_TOKEN(STR_LEMON_C10_DIMM_102)},
   {STRING_TOKEN(STR_LEMON_C10_DIMM_110), STRING_TOKEN(STR_LEMON_C10_DIMM_111), STRING_TOKEN(STR_LEMON_C10_DIMM_112)},
   {STRING_TOKEN(STR_LEMON_C10_DIMM_120), STRING_TOKEN(STR_LEMON_C10_DIMM_121), STRING_TOKEN(STR_LEMON_C10_DIMM_122)},
   {STRING_TOKEN(STR_LEMON_C10_DIMM_130), STRING_TOKEN(STR_LEMON_C10_DIMM_131), STRING_TOKEN(STR_LEMON_C10_DIMM_132)}}
};

EFI_HII_HANDLE
EFIAPI
OemGetPackages (
  )
{
  return HiiAddPackages (
                        &gEfiCallerIdGuid,
                        NULL,
                        OemMiscLibHi1616EvbStrings,
                        NULL,
                        NULL
                        );
}


