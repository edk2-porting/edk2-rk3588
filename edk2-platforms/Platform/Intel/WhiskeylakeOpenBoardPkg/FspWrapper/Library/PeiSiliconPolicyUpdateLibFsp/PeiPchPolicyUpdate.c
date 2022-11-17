/** @file
  This file is SampleCode of the library for Intel PCH PEI Policy initialization.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiPchPolicyUpdate.h"
#include <Library/BaseMemoryLib.h>
#include <Library/HdaVerbTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Guid/GlobalVariable.h>
#include <Library/PchGbeLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchHsioLib.h>
#include <Library/PchSerialIoLib.h>
#include <Library/PchPcieRpLib.h>
#include <GpioConfig.h>
#include <GpioPinsSklH.h>
#include <Library/DebugLib.h>
#include <Library/PchGbeLib.h>
#include <PcieDeviceOverrideTable.h>

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
  )
{
  FspsUpd->FspsConfig.PchPcieDeviceOverrideTablePtr = (UINT32) mPcieDeviceTable;

  AddPlatformVerbTables (
    PchHdaCodecPlatformOnboard,
    &(FspsUpd->FspsConfig.PchHdaVerbTableEntryNum),
    &(FspsUpd->FspsConfig.PchHdaVerbTablePtr)
    );

DEBUG_CODE_BEGIN();
if ((PcdGet8 (PcdSerialIoUartDebugEnable) == 1) &&
      FspsUpd->FspsConfig.SerialIoDevMode[PchSerialIoIndexUart0 + PcdGet8 (PcdSerialIoUartNumber)] == PchSerialIoDisabled ) {
    FspsUpd->FspsConfig.SerialIoDevMode[PchSerialIoIndexUart0 + PcdGet8 (PcdSerialIoUartNumber)] = PchSerialIoHidden;
  }
DEBUG_CODE_END();

  return EFI_SUCCESS;
}


