/** @file

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiBoardInit.h"
#include <KtiSetupDefinitions.h>
#include <UbaKti.h>

extern EFI_GUID gPlatformKtiEparamUpdateDataGuid;

ALL_LANES_EPARAM_LINK_INFO  KtiWilsonCitySMTIcxAllLanesEparamTable[] = {
  //
  // SocketID, Freq, Link, TXEQL, CTLEPEAK
  //
  //
  // Socket 0
  //
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT) | (1 << SPEED_REC_112GT), (1 << KTI_LINK0), 0x2B33373F, ADAPTIVE_CTLE},
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT) | (1 << SPEED_REC_112GT), (1 << KTI_LINK1), 0x2A33363F, ADAPTIVE_CTLE},
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT) | (1 << SPEED_REC_112GT), (1 << KTI_LINK2), 0x2B34363F, ADAPTIVE_CTLE},
  //
  // Socket 1
  //
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT) | (1 << SPEED_REC_112GT), (1 << KTI_LINK0), 0x2A31383F, ADAPTIVE_CTLE},
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT) | (1 << SPEED_REC_112GT), (1 << KTI_LINK1), 0x2A30393F, ADAPTIVE_CTLE},
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT) | (1 << SPEED_REC_112GT), (1 << KTI_LINK2), 0x2C34373F, ADAPTIVE_CTLE},
  //
  // Socket 2
  //
  {0x2, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT) | (1 << SPEED_REC_112GT), (1 << KTI_LINK0), 0x2D37353F, ADAPTIVE_CTLE},
  {0x2, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT) | (1 << SPEED_REC_112GT), (1 << KTI_LINK1), 0x2F3A343F, ADAPTIVE_CTLE},
  {0x2, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT) | (1 << SPEED_REC_112GT), (1 << KTI_LINK2), 0x2F3A343F, ADAPTIVE_CTLE},
  //
  // Socket 3
  //
  {0x3, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT) | (1 << SPEED_REC_112GT), (1 << KTI_LINK0), 0x2D37353F, ADAPTIVE_CTLE},
  {0x3, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT) | (1 << SPEED_REC_112GT), (1 << KTI_LINK1), 0x2F3A343F, ADAPTIVE_CTLE},
  {0x3, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT) | (1 << SPEED_REC_112GT), (1 << KTI_LINK2), 0x2F3A343F, ADAPTIVE_CTLE}
};

PLATFORM_KTI_EPARAM_UPDATE_TABLE  TypeWilsonCitySMTIcxKtiEparamUpdate = {
  PLATFORM_KTIEP_UPDATE_SIGNATURE,
  PLATFORM_KTIEP_UPDATE_VERSION,
  KtiWilsonCitySMTIcxAllLanesEparamTable,
  sizeof (KtiWilsonCitySMTIcxAllLanesEparamTable),
  NULL,
  0
};


EFI_STATUS
TypeWilsonCitySMTInstallKtiEparamData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
)
{
  EFI_STATUS                            Status;
  EFI_HOB_GUID_TYPE                     *GuidHob;
  EFI_PLATFORM_INFO                     *PlatformInfo;

  GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  PlatformInfo = GET_GUID_HOB_DATA (GuidHob);

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformKtiEparamUpdateDataGuid,
                                 &TypeWilsonCitySMTIcxKtiEparamUpdate,
                                 sizeof(TypeWilsonCitySMTIcxKtiEparamUpdate)
                                 );

  return Status;
}
