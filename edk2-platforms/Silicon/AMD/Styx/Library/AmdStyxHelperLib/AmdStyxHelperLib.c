/** @file

  Copyright (c) 2014 - 2016, AMD Inc. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <AmdStyxHelperLib.h>

#include <PiDxe.h>
#include <Library/HobLib.h>

extern  EFI_SYSTEM_TABLE        *gST;

#pragma pack(push, 1)
typedef struct {
  UINT32 MpId;
  UINT32 PmuSpi;
} PMU_INFO;

PMU_INFO mPmuInfo[] = {
 {0x000, 7},
 {0x001, 8},
 {0x100, 9},
 {0x101, 10},
 {0x200, 11},
 {0x201, 12},
 {0x300, 13},
 {0x301, 14}
};
#pragma pack(pop)

#define MAX_CPUS   sizeof(mPmuInfo) / sizeof(PMU_INFO)

EFI_STATUS
AmdStyxGetPmuSpiFromMpId (
  UINT32 MpId,
  UINT32 *PmuSpi
  )
{
  UINT32 i;

  for (i = 0; i < MAX_CPUS; ++i) {
    if (mPmuInfo[ i ].MpId == MpId) {
      *PmuSpi = mPmuInfo[ i ].PmuSpi;
      return EFI_SUCCESS;
    }
  }

  return EFI_INVALID_PARAMETER;
}

ARM_CORE_INFO *
AmdStyxGetArmCoreInfoTable (
  OUT UINTN   *NumEntries
  )
{
  EFI_HOB_GUID_TYPE   *Hob;

  ASSERT (NumEntries != NULL);

  Hob = GetFirstGuidHob (&gAmdStyxMpCoreInfoGuid);
  if (Hob == NULL) {
    return NULL;
  }

  *NumEntries = GET_GUID_HOB_DATA_SIZE (Hob) / sizeof (ARM_CORE_INFO);

  return GET_GUID_HOB_DATA (Hob);
}
