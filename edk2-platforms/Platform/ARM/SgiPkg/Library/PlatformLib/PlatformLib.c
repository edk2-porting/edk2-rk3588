/** @file
*
*  Copyright (c) 2018-2021, ARM Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Library/ArmPlatformLib.h>
#include <Library/BaseLib.h>
#include <Library/HobLib.h>
#include <Ppi/ArmMpCoreInfo.h>
#include <Ppi/SgiPlatformId.h>

#include "SgiPlatform.h"

UINT64 NtFwConfigDtBlob;
STATIC SGI_NT_FW_CONFIG_INFO_PPI mNtFwConfigDtInfoPpi;

STATIC ARM_CORE_INFO mCoreInfoTable[] = {
  {
    // Cluster 0, Core 0
    0x0, 0x0,
  },
};

STATIC CONST SGI_PRODUCT_ID_LOOKUP SgiProductIdLookup[] = {
  {
    Sgi575,
    SGI575_PART_NUM,
    SGI575_CONF_NUM,
    0
  },
  {
    RdN1Edge,
    RD_N1E1_EDGE_PART_NUM,
    RD_N1_EDGE_CONF_ID,
    0
  },
  {
    RdN1EdgeX2,
    RD_N1E1_EDGE_PART_NUM,
    RD_N1_EDGE_CONF_ID,
    1
  },
  {
    RdE1Edge,
    RD_N1E1_EDGE_PART_NUM,
    RD_E1_EDGE_CONF_ID,
    0
  },
  {
    RdV1,
    RD_V1_PART_NUM,
    RD_V1_CONF_ID,
    0
  },
  {
    RdV1Mc,
    RD_V1_PART_NUM,
    RD_V1_MC_CONF_ID,
    1
  },
  {
    RdN2,
    RD_N2_PART_NUM,
    RD_N2_CONF_ID,
    0
  },
  {
    RdN2Cfg1,
    RD_N2_CFG1_PART_NUM,
    RD_N2_CFG1_CONF_ID,
    0
  },
};

EFI_BOOT_MODE
ArmPlatformGetBootMode (
  VOID
  )
{
  return BOOT_WITH_FULL_CONFIGURATION;
}

RETURN_STATUS
ArmPlatformInitialize (
  IN  UINTN                     MpId
  )
{
  mNtFwConfigDtInfoPpi.NtFwConfigDtAddr = NtFwConfigDtBlob;
  return RETURN_SUCCESS;
}

EFI_STATUS
PrePeiCoreGetMpCoreInfo (
  OUT UINTN                   *CoreCount,
  OUT ARM_CORE_INFO           **ArmCoreTable
  )
{
  *CoreCount = 1;
  *ArmCoreTable = mCoreInfoTable;
  return EFI_SUCCESS;
}

STATIC ARM_MP_CORE_INFO_PPI mMpCoreInfoPpi = {
  PrePeiCoreGetMpCoreInfo
};

EFI_PEI_PPI_DESCRIPTOR gPlatformPpiTable[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gArmMpCoreInfoPpiGuid,
    &mMpCoreInfoPpi
  },
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gNtFwConfigDtInfoPpiGuid,
    &mNtFwConfigDtInfoPpi
  }
};

VOID
ArmPlatformGetPlatformPpiList (
  OUT UINTN                   *PpiListSize,
  OUT EFI_PEI_PPI_DESCRIPTOR  **PpiList
  )
{
  *PpiListSize = sizeof (gPlatformPpiTable);
  *PpiList = gPlatformPpiTable;
}

/**
  Derermine the product ID.

  Determine the product ID by using the data in the Platform ID Descriptor HOB
  to lookup for a matching product ID.

  @retval Zero           Failed identify platform.
  @retval Others         ARM_RD_PRODUCT_ID of the identified platform.
**/
UINT8
SgiGetProductId (
  VOID
  )
{
  VOID *SystemIdHob;
  UINT8 Idx;
  SGI_PLATFORM_DESCRIPTOR *HobData;

  SystemIdHob = GetFirstGuidHob (&gArmSgiPlatformIdDescriptorGuid);
  if (SystemIdHob == NULL) {
    return UnknownId;
  }

  HobData = (SGI_PLATFORM_DESCRIPTOR *)GET_GUID_HOB_DATA (SystemIdHob);

  for (Idx = 0; Idx < ARRAY_SIZE (SgiProductIdLookup); Idx++) {
    if ((HobData->PlatformId == SgiProductIdLookup[Idx].PlatformId) &&
        (HobData->ConfigId == SgiProductIdLookup[Idx].ConfigId) &&
        (HobData->MultiChipMode == SgiProductIdLookup[Idx].MultiChipMode)) {
      return SgiProductIdLookup[Idx].ProductId;
    }
  }

  return UnknownId;
}
