/** @file
  Provide FSP wrapper platform related function.

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/FspWrapperApiLib.h>
#include <Library/SiliconPolicyUpdateLib.h>

#include <FspEas.h>
#include <FspmUpd.h>
#include <FspsUpd.h>

#include "PeiSaPolicyUpdate.h"
#include "PeiMiscPolicyUpdate.h"

/**
  Performs FSP PCH PEI Policy initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspPchPolicyUpdate (
  IN OUT FSPS_UPD    *FspsUpd
  );

VOID
InternalPrintVariableData (
  IN UINT8   *Data8,
  IN UINTN   DataSize
  )
{
  UINTN      Index;

  for (Index = 0; Index < DataSize; Index++) {
    if (Index % 0x10 == 0) {
      DEBUG ((DEBUG_INFO, "\n%08X:", Index));
    }
    DEBUG ((DEBUG_INFO, " %02X", *Data8++));
  }
  DEBUG ((DEBUG_INFO, "\n"));
}

/**
  Performs silicon pre-mem policy update.

  The meaning of Policy is defined by silicon code.
  It could be the raw data, a handle, a PPI, etc.

  The input Policy must be returned by SiliconPolicyDonePreMem().

  1) In FSP path, the input Policy should be FspmUpd.
  A platform may use this API to update the FSPM UPD policy initialized
  by the silicon module or the default UPD data.
  The output of FSPM UPD data from this API is the final UPD data.

  2) In non-FSP path, the board may use additional way to get
  the silicon policy data field based upon the input Policy.

  @param[in, out] Policy       Pointer to policy.

  @return the updated policy.
**/
VOID *
EFIAPI
SiliconPolicyUpdatePreMem (
  IN OUT VOID    *FspmUpd
  )
{
  FSPM_UPD              *FspmUpdDataPtr;

  FspmUpdDataPtr = FspmUpd;

  PeiFspSaPolicyUpdatePreMem (FspmUpdDataPtr);
  PeiFspMiscUpdUpdatePreMem (FspmUpdDataPtr);
  InternalPrintVariableData ((VOID *) FspmUpdDataPtr, sizeof (FSPM_UPD));

  return FspmUpd;
}

/**
  Performs silicon post-mem policy update.

  The meaning of Policy is defined by silicon code.
  It could be the raw data, a handle, a PPI, etc.

  The input Policy must be returned by SiliconPolicyDonePostMem().

  1) In FSP path, the input Policy should be FspsUpd.
  A platform may use this API to update the FSPS UPD policy initialized
  by the silicon module or the default UPD data.
  The output of FSPS UPD data from this API is the final UPD data.

  2) In non-FSP path, the board may use additional way to get
  the silicon policy data field based upon the input Policy.

  @param[in, out] Policy       Pointer to policy.

  @return the updated policy.
**/
VOID *
EFIAPI
SiliconPolicyUpdatePostMem (
  IN OUT VOID    *FspsUpd
  )
{
  FSPS_UPD              *FspsUpdDataPtr;

  FspsUpdDataPtr = FspsUpd;

  PeiFspPchPolicyUpdate (FspsUpd);
  InternalPrintVariableData ((VOID * ) FspsUpdDataPtr, sizeof (FSPS_UPD));

  return FspsUpd;
}
