/** @file
  System Agent policy update.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
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
  UINT8                       BomId;

  FspmUpd->FspmConfig.SpdAddressTable[0] = PcdGet8 (PcdMrcSpdAddressTable0);
  FspmUpd->FspmConfig.SpdAddressTable[1] = PcdGet8 (PcdMrcSpdAddressTable1);
  FspmUpd->FspmConfig.SpdAddressTable[2] = PcdGet8 (PcdMrcSpdAddressTable2);
  FspmUpd->FspmConfig.SpdAddressTable[3] = PcdGet8 (PcdMrcSpdAddressTable3);
  FspmUpd->FspmConfig.MemorySpdDataLen = PcdGet16 (PcdMrcSpdDataSize);

  //
  // If SpdAddressTable are not all 0, it means DIMM slots implemented and
  // MemorySpdPtr* already updated by reading SPD from DIMM in SiliconPolicyInitPreMem.
  //
  // If SpdAddressTable all 0, this is memory down design and hardcoded SpdData
  // should be applied to MemorySpdPtr*.
  //
  if ((PcdGet8 (PcdMrcSpdAddressTable0) == 0) && (PcdGet8 (PcdMrcSpdAddressTable1) == 0)
      && (PcdGet8 (PcdMrcSpdAddressTable2) == 0) && (PcdGet8 (PcdMrcSpdAddressTable3) == 0)) {
    DEBUG ((DEBUG_INFO, "Using static SPD data for down memory.\n"));

    // BOMID [1:0]
    //   0: 16G A & B CH
    //   1:  8G A CH
    //   2:  8G A & B CH
    //   3:  4G A CH
    BomId = PcdGet8(PcdBoardBomId);

    if ((BomId & BIT0) == BIT0) {
      // Single Channel
      FspmUpd->FspmConfig.MemorySpdPtr00 = PcdGet32 (PcdMrcSpdData);
      FspmUpd->FspmConfig.MemorySpdPtr01 = 0;
      FspmUpd->FspmConfig.MemorySpdPtr10 = 0;
      FspmUpd->FspmConfig.MemorySpdPtr11 = 0;
    }else{
      // Dual Channel
      FspmUpd->FspmConfig.MemorySpdPtr00 = PcdGet32 (PcdMrcSpdData);
      FspmUpd->FspmConfig.MemorySpdPtr01 = 0;
      FspmUpd->FspmConfig.MemorySpdPtr10 = PcdGet32 (PcdMrcSpdData);
      FspmUpd->FspmConfig.MemorySpdPtr11 = 0;
    }
    // CopyMem (
    //   (VOID *) (UINTN) FspmUpd->FspmConfig.MemorySpdPtr00,
    //   (VOID *) (UINTN) PcdGet32 (PcdMrcSpdData),
    //   PcdGet16 (PcdMrcSpdDataSize)
    //   );
    // CopyMem (
    //   (VOID *) (UINTN) FspmUpd->FspmConfig.MemorySpdPtr10,
    //   (VOID *) (UINTN) PcdGet32 (PcdMrcSpdData),
    //   PcdGet16 (PcdMrcSpdDataSize)
    //   );
  }

  DEBUG((DEBUG_INFO, "Updating Dq Byte Map and DQS Byte Swizzling Settings...\n"));
  Buffer = (VOID *) (UINTN) PcdGet32 (PcdMrcDqByteMap);
  if (Buffer) {
    CopyMem ((VOID *) FspmUpd->FspmConfig.DqByteMapCh0, Buffer, sizeof (FspmUpd->FspmConfig.DqByteMapCh0));
    CopyMem (
      (VOID *) FspmUpd->FspmConfig.DqByteMapCh1,
      (UINT8 *) Buffer +  sizeof (FspmUpd->FspmConfig.DqByteMapCh0),
      sizeof (FspmUpd->FspmConfig.DqByteMapCh1)
      );
  }
  Buffer = (VOID *) (UINTN) PcdGet32 (PcdMrcDqsMapCpu2Dram);
  if (Buffer) {
    CopyMem ((VOID *) FspmUpd->FspmConfig.DqsMapCpu2DramCh0, Buffer, sizeof (FspmUpd->FspmConfig.DqsMapCpu2DramCh0));
    CopyMem (
      (VOID *) FspmUpd->FspmConfig.DqsMapCpu2DramCh1,
      (UINT8 *) Buffer + sizeof (FspmUpd->FspmConfig.DqsMapCpu2DramCh0),
      sizeof (FspmUpd->FspmConfig.DqsMapCpu2DramCh1)
      );
  }

  DEBUG((DEBUG_INFO, "Updating Dq Pins Interleaved,Rcomp Resistor & Rcomp Target Settings...\n"));
  FspmUpd->FspmConfig.DqPinsInterleaved = (PcdGetBool (PcdMrcDqPinsInterleaved) ? 1 : 0);
  Buffer = (VOID *) (UINTN) PcdGet32 (PcdMrcRcompResistor);
  if (Buffer) {
    CopyMem ((VOID *) FspmUpd->FspmConfig.RcompResistor, Buffer, sizeof (FspmUpd->FspmConfig.RcompResistor));
  }
  Buffer = (VOID *) (UINTN) PcdGet32 (PcdMrcRcompTarget);
  if (Buffer) {
    CopyMem ((VOID *) FspmUpd->FspmConfig.RcompTarget, Buffer, sizeof (FspmUpd->FspmConfig.RcompTarget));
  }
  return EFI_SUCCESS;
}


