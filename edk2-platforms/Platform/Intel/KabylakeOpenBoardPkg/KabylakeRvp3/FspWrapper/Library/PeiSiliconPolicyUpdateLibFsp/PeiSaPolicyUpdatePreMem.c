/** @file
Do Platform Stage System Agent initialization.

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "PeiSaPolicyUpdate.h"
#include <CpuRegs.h>
#include <Library/CpuPlatformLib.h>
#include <Guid/MemoryTypeInformation.h>
#include <Guid/MemoryOverwriteControl.h>
#include <Library/HobLib.h>
#include <PchAccess.h>
#include <SaAccess.h>
#include <Library/CpuMailboxLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiSaPolicyLib.h>
#include <Library/GpioLib.h>
#include <GpioPinsSklH.h>


/**
  Performs FSP SA PEI Policy initialization in pre-memory.

  @param[in][out]  FspmUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspSaPolicyUpdatePreMem (
  IN OUT FSPM_UPD    *FspmUpd
  )
{
  VOID                        *Buffer;

  //
  // If SpdAddressTable are not all 0, it means DIMM slots implemented and
  // MemorySpdPtr* already updated by reading SPD from DIMM in SiliconPolicyInitPreMem.
  //
  // If SpdAddressTable all 0, this is memory down design and hardcoded SpdData
  // should be applied to MemorySpdPtr*.
  //
  if ((PcdGet8 (PcdMrcSpdAddressTable0) == 0) && (PcdGet8 (PcdMrcSpdAddressTable1) == 0)
      && (PcdGet8 (PcdMrcSpdAddressTable2) == 0) && (PcdGet8 (PcdMrcSpdAddressTable3) == 0)) {
    DEBUG((DEBUG_INFO, "Override MemorySpdPtr...\n"));
    CopyMem((VOID *)(UINTN)FspmUpd->FspmConfig.MemorySpdPtr00, (VOID *)(UINTN)PcdGet32 (PcdMrcSpdData), PcdGet16 (PcdMrcSpdDataSize));
    CopyMem((VOID *)(UINTN)FspmUpd->FspmConfig.MemorySpdPtr10, (VOID *)(UINTN)PcdGet32 (PcdMrcSpdData), PcdGet16 (PcdMrcSpdDataSize));
  }

  DEBUG((DEBUG_INFO, "Updating Dq Byte Map and DQS Byte Swizzling Settings...\n"));
  Buffer = (VOID *) (UINTN) PcdGet32 (PcdMrcDqByteMap);
  if (Buffer) {
    CopyMem ((VOID *)FspmUpd->FspmConfig.DqByteMapCh0, Buffer, 12);
    CopyMem ((VOID *)FspmUpd->FspmConfig.DqByteMapCh1, (UINT8*) Buffer + 12, 12);
  }
  Buffer = (VOID *) (UINTN) PcdGet32 (PcdMrcDqsMapCpu2Dram);
  if (Buffer) {
    CopyMem ((VOID *)FspmUpd->FspmConfig.DqsMapCpu2DramCh0, Buffer, 8);
    CopyMem ((VOID *)FspmUpd->FspmConfig.DqsMapCpu2DramCh1, (UINT8*) Buffer + 8, 8);
  }

  DEBUG((DEBUG_INFO, "Updating Dq Pins Interleaved,Rcomp Resistor & Rcomp Target Settings...\n"));
  Buffer = (VOID *) (UINTN) PcdGet32 (PcdMrcRcompResistor);
  if (Buffer) {
    CopyMem ((VOID *)FspmUpd->FspmConfig.RcompResistor, Buffer, 6);
  }
  Buffer = (VOID *) (UINTN) PcdGet32 (PcdMrcRcompTarget);
  if (Buffer) {
    CopyMem ((VOID *)FspmUpd->FspmConfig.RcompTarget, Buffer, 10);
  }
  return EFI_SUCCESS;
}

