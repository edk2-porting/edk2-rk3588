/** @file

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiBoardInit.h"
#include <KtiSetupDefinitions.h>
#include <UbaKti.h>

extern EFI_GUID gPlatformKtiEparamUpdateDataGuid;

ALL_LANES_EPARAM_LINK_INFO  CooperCityRP2SAllLanesEparamTable[] = {
  //
  //SocketID, Freq, Link, TXEQ, CTLEPEAK
  //

  //
  // Socket 0
  //
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK0), 0x2C33383F, ADAPTIVE_CTLE},
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK1), 0x2B35353F, ADAPTIVE_CTLE},
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK2), 0x2D37353F, ADAPTIVE_CTLE},
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK3), 0x2D37353F, ADAPTIVE_CTLE}, // temporary data
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK4), 0x2D37353F, ADAPTIVE_CTLE}, // temporary data
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK5), 0x2C35363F, ADAPTIVE_CTLE}, // temporary data

  //
  // Socket 1
  //
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK0), 0x2C33383F, ADAPTIVE_CTLE}, // temporary data
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK1), 0x2B35353F, ADAPTIVE_CTLE}, // temporary data
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK2), 0x2D35373F, ADAPTIVE_CTLE},
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK3), 0x2D35373F, ADAPTIVE_CTLE}, // temporary data
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK4), 0x2A2F3A3F, ADAPTIVE_CTLE},
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK5), 0x2C35363F, ADAPTIVE_CTLE}
};

ALL_LANES_EPARAM_LINK_INFO  CooperCityRP4SAllLanesEparamTable[] = {
  //
  //SocketID, Freq, Link, TXEQ, CTLEPEAK
  //

  //
  // Socket 0
  //
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK0), 0x2A30393F, ADAPTIVE_CTLE},
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK1), 0x2A33363F, ADAPTIVE_CTLE},
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK2), 0x2D37353F, ADAPTIVE_CTLE},
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK3), 0x2A34353F, ADAPTIVE_CTLE},
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK4), 0x2B33373F, ADAPTIVE_CTLE},
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK5), 0x2A33363F, ADAPTIVE_CTLE},

  //
  // Socket 1
  //
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK0), 0x2D35373F, ADAPTIVE_CTLE},
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK1), 0x2A33363F, ADAPTIVE_CTLE},
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK2), 0x2C34373F, ADAPTIVE_CTLE},
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK3), 0x2B35353F, ADAPTIVE_CTLE},
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK4), 0x2C32393F, ADAPTIVE_CTLE},
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK5), 0x2A33363F, ADAPTIVE_CTLE},

  //
  // Socket 2
  //
  {0x2, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK0), 0x2B31393F, ADAPTIVE_CTLE},
  {0x2, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK1), 0x2A32373F, ADAPTIVE_CTLE},
  {0x2, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK2), 0x2D36363F, ADAPTIVE_CTLE},
  {0x2, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK3), 0x2B34363F, ADAPTIVE_CTLE},
  {0x2, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK4), 0x2A32373F, ADAPTIVE_CTLE},
  {0x2, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK5), 0x2A33363F, ADAPTIVE_CTLE},

  //
  // Socket 3
  //
  {0x3, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK0), 0x2A31383F, ADAPTIVE_CTLE},
  {0x3, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK1), 0x2C36353F, ADAPTIVE_CTLE},
  {0x3, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK2), 0x2A32373F, ADAPTIVE_CTLE},
  {0x3, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK3), 0x2B35353F, ADAPTIVE_CTLE},
  {0x3, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK4), 0x2A2F3A3F, ADAPTIVE_CTLE},
  {0x3, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK5), 0x2A33363F, ADAPTIVE_CTLE}
};

ALL_LANES_EPARAM_LINK_INFO  CooperCityRP8SAllLanesEparamTable[] = {
  //
  //SocketID, Freq, Link, TXEQ, CTLEPEAK
  //

  //
  // Socket 0
  //
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK0), 0x2A30393F, ADAPTIVE_CTLE},
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK1), 0x2A33363F, ADAPTIVE_CTLE},
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK2), 0x2D37353F, ADAPTIVE_CTLE},
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK3), 0x2A34353F, ADAPTIVE_CTLE},
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK4), 0x2B33373F, ADAPTIVE_CTLE},
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK5), 0x2A33363F, ADAPTIVE_CTLE},

  //
  // Socket 1
  //
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK0), 0x2D35373F, ADAPTIVE_CTLE},
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK1), 0x2A33363F, ADAPTIVE_CTLE},
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK2), 0x2C34373F, ADAPTIVE_CTLE},
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK3), 0x2B35353F, ADAPTIVE_CTLE},
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK4), 0x2C32393F, ADAPTIVE_CTLE},
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK5), 0x2A33363F, ADAPTIVE_CTLE},

  //
  // Socket 2
  //
  {0x2, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK0), 0x2B31393F, ADAPTIVE_CTLE},
  {0x2, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK1), 0x2A32373F, ADAPTIVE_CTLE},
  {0x2, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK2), 0x2D36363F, ADAPTIVE_CTLE},
  {0x2, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK3), 0x2B34363F, ADAPTIVE_CTLE},
  {0x2, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK4), 0x2A32373F, ADAPTIVE_CTLE},
  {0x2, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK5), 0x2A33363F, ADAPTIVE_CTLE},

  //
  // Socket 3
  //
  {0x3, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK0), 0x2A31383F, ADAPTIVE_CTLE},
  {0x3, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK1), 0x2C36353F, ADAPTIVE_CTLE},
  {0x3, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK2), 0x2A32373F, ADAPTIVE_CTLE},
  {0x3, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK3), 0x2B35353F, ADAPTIVE_CTLE},
  {0x3, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK4), 0x2A2F3A3F, ADAPTIVE_CTLE},
  {0x3, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK5), 0x2A33363F, ADAPTIVE_CTLE},

  //
  // Socket 4
  //
  {0x4, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK0), 0x2A30393F, ADAPTIVE_CTLE},
  {0x4, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK1), 0x2A33363F, ADAPTIVE_CTLE},
  {0x4, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK2), 0x2D37353F, ADAPTIVE_CTLE},
  {0x4, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK3), 0x2A34353F, ADAPTIVE_CTLE},
  {0x4, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK4), 0x2B33373F, ADAPTIVE_CTLE},
  {0x4, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK5), 0x2A33363F, ADAPTIVE_CTLE},

  //
  // Socket 5
  //
  {0x5, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK0), 0x2D35373F, ADAPTIVE_CTLE},
  {0x5, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK1), 0x2A33363F, ADAPTIVE_CTLE},
  {0x5, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK2), 0x2C34373F, ADAPTIVE_CTLE},
  {0x5, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK3), 0x2B35353F, ADAPTIVE_CTLE},
  {0x5, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK4), 0x2C32393F, ADAPTIVE_CTLE},
  {0x5, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK5), 0x2A33363F, ADAPTIVE_CTLE},

  //
  // Socket 6
  //
  {0x6, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK0), 0x2B31393F, ADAPTIVE_CTLE},
  {0x6, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK1), 0x2A32373F, ADAPTIVE_CTLE},
  {0x6, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK2), 0x2D36363F, ADAPTIVE_CTLE},
  {0x6, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK3), 0x2B34363F, ADAPTIVE_CTLE},
  {0x6, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK4), 0x2A32373F, ADAPTIVE_CTLE},
  {0x6, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK5), 0x2A33363F, ADAPTIVE_CTLE},

  //
  // Socket 7
  //
  {0x7, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK0), 0x2A31383F, ADAPTIVE_CTLE},
  {0x7, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK1), 0x2C36353F, ADAPTIVE_CTLE},
  {0x7, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK2), 0x2A32373F, ADAPTIVE_CTLE},
  {0x7, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK3), 0x2B35353F, ADAPTIVE_CTLE},
  {0x7, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK4), 0x2A2F3A3F, ADAPTIVE_CTLE},
  {0x7, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK5), 0x2A33363F, ADAPTIVE_CTLE}
};

//PER_LANE_EPARAM_LINK_INFO  KtiCooperCityRPPerLaneEparamTable[] = { 0 };
PLATFORM_KTI_EPARAM_UPDATE_TABLE  TypeCooperCityRP2SKtiEparamUpdate =
{
  PLATFORM_KTIEP_UPDATE_SIGNATURE,
  PLATFORM_KTIEP_UPDATE_VERSION,
  CooperCityRP2SAllLanesEparamTable,
  sizeof (CooperCityRP2SAllLanesEparamTable),
  NULL,
  0
};

PLATFORM_KTI_EPARAM_UPDATE_TABLE  TypeCooperCityRP4SKtiEparamUpdate =
{
  PLATFORM_KTIEP_UPDATE_SIGNATURE,
  PLATFORM_KTIEP_UPDATE_VERSION,
  CooperCityRP4SAllLanesEparamTable,
  sizeof (CooperCityRP4SAllLanesEparamTable),
  NULL,
  0
};

PLATFORM_KTI_EPARAM_UPDATE_TABLE  TypeCooperCityRP8SKtiEparamUpdate =
{
  PLATFORM_KTIEP_UPDATE_SIGNATURE,
  PLATFORM_KTIEP_UPDATE_VERSION,
  CooperCityRP8SAllLanesEparamTable,
  sizeof (CooperCityRP8SAllLanesEparamTable),
  NULL,
  0
};


EFI_STATUS
TypeCooperCityRPInstallKtiEparamData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi,
  IN UINT8                      PlatformCapabilities
  )
{
  EFI_STATUS                            Status;
  PLATFORM_KTI_EPARAM_UPDATE_TABLE      *KtiEparamTable;
  UINTN                                 KtiEparamTableSize;

  switch (PlatformCapabilities) {
  case PLATFORM_CAPABILITY_2_SOCKET:
    KtiEparamTable = &TypeCooperCityRP2SKtiEparamUpdate;
    KtiEparamTableSize = sizeof (TypeCooperCityRP2SKtiEparamUpdate);
    break;

  case PLATFORM_CAPABILITY_4_SOCKET:
    KtiEparamTable = &TypeCooperCityRP4SKtiEparamUpdate;
    KtiEparamTableSize = sizeof (TypeCooperCityRP4SKtiEparamUpdate);
    break;

  case PLATFORM_CAPABILITY_8_SOCKET:
  default:
    KtiEparamTable = &TypeCooperCityRP8SKtiEparamUpdate;
    KtiEparamTableSize = sizeof (TypeCooperCityRP8SKtiEparamUpdate);
    break;
  }

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformKtiEparamUpdateDataGuid,
                                 KtiEparamTable,
                                 KtiEparamTableSize
                                 );

  return Status;
}
