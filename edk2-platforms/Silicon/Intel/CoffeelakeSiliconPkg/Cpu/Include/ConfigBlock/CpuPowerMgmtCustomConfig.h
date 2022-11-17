/** @file
  CPU Power Managment Custom Config Block.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPU_POWER_MGMT_CUSTOM_CONFIG_H_
#define _CPU_POWER_MGMT_CUSTOM_CONFIG_H_

#define CPU_POWER_MGMT_CUSTOM_CONFIG_REVISION 1

extern EFI_GUID gCpuPowerMgmtCustomConfigGuid;

#pragma pack (push,1)

///
/// Defines the maximum number of custom ratio states supported.
///
#define MAX_CUSTOM_RATIO_TABLE_ENTRIES    40
#define MAX_16_CUSTOM_RATIO_TABLE_ENTRIES 16

///
/// Defines the maximum number of custom ConfigTdp entries supported.
/// @warning: Changing this define would cause DWORD alignment issues in policy structures.
///
#define MAX_CUSTOM_CTDP_ENTRIES 3

///
/// This structure is used to describe the custom processor ratio table desired by the platform.
///
typedef struct {
  UINT8  MaxRatio;                                           ///< The maximum ratio of the custom ratio table.
  UINT8  NumberOfEntries;                                    ///< The number of custom ratio state entries, ranges from 2 to 40 for a valid custom ratio table.
  UINT8  Rsvd0[2];                                           ///< Reserved for DWORD alignment.
  UINT32 Cpuid;                                              ///< The CPU ID for which this custom ratio table applies.
  UINT8  StateRatio[MAX_CUSTOM_RATIO_TABLE_ENTRIES];         ///< The processor ratios in the custom ratio table.
  ///
  /// If there are more than 16 total entries in the StateRatio table, then use these 16 entries to fill max 16 table.
  /// @note If NumberOfEntries is 16 or less, or the first entry of this table is 0, then this table is ignored,
  /// and up to the top 16 values from the StateRatio table is used instead.
  ///
  UINT8  StateRatioMax16[MAX_16_CUSTOM_RATIO_TABLE_ENTRIES];
#if ((MAX_CUSTOM_RATIO_TABLE_ENTRIES + MAX_16_CUSTOM_RATIO_TABLE_ENTRIES) % 4)
  UINT8  Rsvd1[4 - ((MAX_CUSTOM_RATIO_TABLE_ENTRIES + MAX_16_CUSTOM_RATIO_TABLE_ENTRIES) % 4)];  ///< If needed, add padding for dword alignment.
#endif
} PPM_CUSTOM_RATIO_TABLE;

///
/// PPM Custom ConfigTdp Settings
///
typedef struct _PPM_CUSTOM_CTDP_TABLE {
  UINT32 CustomPowerLimit1Time      :  8;            ///< Short term Power Limit time window value for custom cTDP level.
  UINT32 CustomTurboActivationRatio :  8;            ///< Turbo Activation Ratio for custom cTDP level.
  UINT32 RsvdBits                   : 16;            ///< Bits reserved for DWORD alignment.
  UINT16 CustomPowerLimit1;                          ///< Short term Power Limit value for custom cTDP level. Units are based on POWER_MGMT_CONFIG.CustomPowerUnit.
  UINT16 CustomPowerLimit2;                          ///< Long term Power Limit value for custom cTDP level. Units are based on POWER_MGMT_CONFIG.CustomPowerUnit.
} PPM_CUSTOM_CTDP_TABLE;

/**
  CPU Power Management Custom Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER    Header;                                                ///< Config Block Header
  PPM_CUSTOM_RATIO_TABLE CustomRatioTable;                                      ///< Custom Processor Ratio Table Instance
  PPM_CUSTOM_CTDP_TABLE  CustomConfigTdpTable[MAX_CUSTOM_CTDP_ENTRIES];         ///< Custom ConfigTdp Settings Instance
  UINT32                 ConfigTdpLock  : 1;                                    ///< Lock the ConfigTdp mode settings from runtime changes; <b>0: Disable</b>; 1: Enable.
  UINT32                 ConfigTdpBios  : 1;                                    ///< Configure whether to load Configurable TDP SSDT; <b>0: Disable</b>; 1: Enable.
  UINT32                 RsvdBits       : 30;                                   ///< Reserved for future use
} CPU_POWER_MGMT_CUSTOM_CONFIG;

#pragma pack (pop)

#endif // _CPU_POWER_MGMT_CUSTOM_CONFIG_H_
