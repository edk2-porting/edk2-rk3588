/** @file
  LbgPchH Sx HSIO PTSS C File

  @copyright
  Copyright 2014 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PchLbgHsioPtssTablesSx.h"
#include <PlatformInfoTypes.h>

HSIO_PTSS_TABLES PchLbgHsioPtss_Sx[] = {
  {{0xA9, 12, V_PCH_PCR_FIA_LANE_OWN_SATA, 0x150, 0x02000000, (UINT32) ~0x3F000000}, PchSataTopoUnknown, TypeNeonCityEPRP},
  {{0xA9, 13, V_PCH_PCR_FIA_LANE_OWN_SATA, 0x150, 0x02000000, (UINT32) ~0x3F000000}, PchSataTopoUnknown, TypeNeonCityEPRP},
  {{0xA9, 18, V_PCH_PCR_FIA_LANE_OWN_SATA, 0x150, 0x02000000, (UINT32) ~0x3F000000}, PchSataTopoUnknown, TypeNeonCityEPRP},
  {{0xA9, 19, V_PCH_PCR_FIA_LANE_OWN_SATA, 0x150, 0x02000000, (UINT32) ~0x3F000000}, PchSataTopoUnknown, TypeNeonCityEPRP},
  {{0xA9, 20, V_PCH_PCR_FIA_LANE_OWN_SATA, 0x150, 0x02000000, (UINT32) ~0x3F000000}, PchSataTopoUnknown, TypeNeonCityEPRP},
  {{0xA9, 21, V_PCH_PCR_FIA_LANE_OWN_SATA, 0x150, 0x02000000, (UINT32) ~0x3F000000}, PchSataTopoUnknown, TypeNeonCityEPRP},
  {{0xA9, 22, V_PCH_PCR_FIA_LANE_OWN_SATA, 0x150, 0x02000000, (UINT32) ~0x3F000000}, PchSataTopoUnknown, TypeNeonCityEPRP},
  {{0xA9, 23, V_PCH_PCR_FIA_LANE_OWN_SATA, 0x150, 0x02000000, (UINT32) ~0x3F000000}, PchSataTopoUnknown, TypeNeonCityEPRP},
  {{0xA9, 24, V_PCH_PCR_FIA_LANE_OWN_SATA, 0x150, 0x02000000, (UINT32) ~0x3F000000}, PchSataTopoUnknown, TypeNeonCityEPRP},
  {{0xA9, 25, V_PCH_PCR_FIA_LANE_OWN_SATA, 0x150, 0x02000000, (UINT32) ~0x3F000000}, PchSataTopoUnknown, TypeNeonCityEPRP},
};

UINT32 PchLbgHsioPtss_Sx_Size = sizeof (PchLbgHsioPtss_Sx) / sizeof (PchLbgHsioPtss_Sx[0]);

