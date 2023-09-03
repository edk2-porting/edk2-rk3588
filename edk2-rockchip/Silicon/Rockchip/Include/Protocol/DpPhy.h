/** @file
*
*  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _DP_PHY_PROTOCOL_H_
#define _DP_PHY_PROTOCOL_H_

#define DP_PHY_PROTOCOL_GUID   \
    { 0xb4bcf881, 0xc8b3, 0x46d7, { 0xaf, 0xbe, 0x5a, 0x2d, 0x94, 0x9d, 0x93, 0xc3 } }

typedef struct _DP_PHY_PROTOCOL DP_PHY_PROTOCOL;

#define DP_PHY_MAX_LANES  4

typedef struct {
  UINT32    LinkRate;
  UINT32    LaneCount;
  UINT32    LaneVoltageSwingLevels[DP_PHY_MAX_LANES];
  UINT32    LanePreEmphasisLevels[DP_PHY_MAX_LANES];
  BOOLEAN   EnableSpreadSpectrum;
  BOOLEAN   SetLinkRateAndSpreadSpectrum;
  BOOLEAN   SetLaneCount;
  BOOLEAN   SetVoltageSwingAndPreEmphasisLevels;
} DP_PHY_CONFIGURATION;

typedef struct {
  UINT32  BusWidth;
  UINT32  MaximumLinkRate;
} DP_PHY_CAPABILITIES;

typedef
EFI_STATUS
(EFIAPI *DP_PHY_POWER_ON) (
  IN DP_PHY_PROTOCOL          *This
  );

typedef
EFI_STATUS
(EFIAPI *DP_PHY_POWER_OFF) (
  IN DP_PHY_PROTOCOL          *This
  );

typedef
EFI_STATUS
(EFIAPI *DP_PHY_CONFIGURE) (
  IN DP_PHY_PROTOCOL          *This,
  IN DP_PHY_CONFIGURATION     *Config
  );

struct _DP_PHY_PROTOCOL {
  DP_PHY_POWER_ON           PowerOn;
  DP_PHY_POWER_OFF          PowerOff;
  DP_PHY_CONFIGURE          Configure;
  DP_PHY_CAPABILITIES       Capabilities;
  UINT32                    Id;
};

extern EFI_GUID gDpPhyProtocolGuid;

#endif // _DP_PHY_PROTOCOL_H_
