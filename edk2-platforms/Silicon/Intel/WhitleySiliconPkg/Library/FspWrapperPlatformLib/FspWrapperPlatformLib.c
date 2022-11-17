/** @file
  Sample to provide FSP wrapper related function.

  @copyright
  Copyright 2014 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/LargeVariableReadLib.h>

#include <FspmUpd.h>
#include <Guid/PlatformInfo.h>
#include <Ppi/UpiPolicyPpi.h>

VOID *
GetFspNvsBuffer (
  VOID
)
{
  EFI_STATUS          Status;
  UINTN                     FspNvsBufferSize;
  VOID                      *FspNvsBufferPtr;

  FspNvsBufferPtr   = NULL;
  FspNvsBufferSize  = 0;
  Status = GetLargeVariable (L"FspNvsBuffer", &gFspNonVolatileStorageHobGuid, &FspNvsBufferSize, NULL);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    DEBUG ((DEBUG_INFO, "FspNvsBuffer Size = %d\n", FspNvsBufferSize));
    FspNvsBufferPtr = AllocateZeroPool (FspNvsBufferSize);
    if (FspNvsBufferPtr == NULL) {
      DEBUG ((DEBUG_ERROR, "Error: Cannot create FspNvsBuffer, out of memory!\n"));
    ASSERT (FALSE);
    return NULL;
  }
    Status = GetLargeVariable (L"FspNvsBuffer", &gFspNonVolatileStorageHobGuid, &FspNvsBufferSize, FspNvsBufferPtr);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Error: Unable to read FspNvsBuffer UEFI variable Status: %r\n", Status));
      ASSERT_EFI_ERROR (Status);
    return NULL;
  }

    return FspNvsBufferPtr;

  } else if (Status == EFI_NOT_FOUND) {
    DEBUG ((DEBUG_INFO, "Cannot create FSP NVS Buffer, UEFI variable does not exist (this is likely a first boot)\n"));
  } else {
    DEBUG ((DEBUG_ERROR, "Error: Unable to read FspNvsBuffer UEFI variable Status: %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

    return NULL;
  }

VOID
EFIAPI
UpdateFspmUpdData (
  IN OUT VOID         *FspUpdRgnPtr
  )
{
  FSPM_UPD                     *FspmUpd;
  EFI_STATUS                   Status;
  UPI_POLICY_PPI               *UpiPolicyPpi;
  KTI_HOST_IN                  *Upi;
  UINTN                        Index;
  VOID                         *FSPTempMem;
  EFI_HOB_GUID_TYPE               *GuidHob;
  EFI_PLATFORM_INFO               *PlatformInfo;

  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT (GuidHob != NULL);
  PlatformInfo  = GET_GUID_HOB_DATA(GuidHob);

  Status = PeiServicesLocatePpi (&gUpiSiPolicyPpiGuid, 0, NULL, &UpiPolicyPpi);
  ASSERT_EFI_ERROR(Status);

  Upi = &UpiPolicyPpi->Upi;
  FspmUpd = (FSPM_UPD*)FspUpdRgnPtr;

  FSPTempMem = (VOID *)(UINTN)(PcdGet32(PcdTemporaryRamBase) + PcdGet32(PcdPeiTemporaryRamRcHeapSize));;
  if (FSPTempMem != NULL) {
    FspmUpd->FspmArchUpd.StackBase = (VOID*)(((UINTN)FSPTempMem) & (~0xFFF));
    FspmUpd->FspmArchUpd.StackSize = PcdGet32(PcdFspTemporaryRamSize);
  }

  for (Index = 0; Index < MAX_SOCKET; Index ++)  {
    FspmUpd->FspmConfig.BusRatio[Index] = Upi->BusRatio[Index];
    FspmUpd->FspmConfig.KtiFpgaEnable[Index] = Upi->KtiFpgaEnable[Index];
  }

  FspmUpd->FspmConfig.D2KCreditConfig = Upi->D2KCreditConfig;
  FspmUpd->FspmConfig.SnoopThrottleConfig = Upi->SnoopThrottleConfig;
  FspmUpd->FspmConfig.LegacyVgaSoc = Upi->LegacyVgaSoc;
  FspmUpd->FspmConfig.LegacyVgaStack = Upi->LegacyVgaStack;
  FspmUpd->FspmConfig.P2pRelaxedOrdering = Upi->P2pRelaxedOrdering;
  FspmUpd->FspmConfig.SncEn = Upi->SncEn;
  FspmUpd->FspmConfig.UmaClustering = Upi->UmaClustering;
  FspmUpd->FspmConfig.IoDcMode = Upi->IoDcMode;
  FspmUpd->FspmConfig.DegradePrecedence = Upi->DegradePrecedence;
  FspmUpd->FspmConfig.Degrade4SPreference = Upi->Degrade4SPreference;
  FspmUpd->FspmConfig.DirectoryModeEn = Upi->DirectoryModeEn;
  FspmUpd->FspmConfig.XptPrefetchEn = Upi->XptPrefetchEn;
  FspmUpd->FspmConfig.KtiPrefetchEn = Upi->KtiPrefetchEn;
  FspmUpd->FspmConfig.XptRemotePrefetchEn = Upi->XptRemotePrefetchEn;
  FspmUpd->FspmConfig.DdrtQosMode = Upi->DdrtQosMode;
  FspmUpd->FspmConfig.KtiLinkSpeedMode = Upi->KtiLinkSpeedMode;
  FspmUpd->FspmConfig.KtiLinkSpeed = Upi->KtiLinkSpeed;
  FspmUpd->FspmConfig.KtiLinkL0pEn = Upi->KtiLinkL0pEn;
  FspmUpd->FspmConfig.KtiLinkL1En = Upi->KtiLinkL1En;
  FspmUpd->FspmConfig.KtiFailoverEn = Upi->KtiFailoverEn;
  FspmUpd->FspmConfig.KtiLbEn = Upi->KtiLbEn;
  FspmUpd->FspmConfig.KtiCrcMode = Upi->KtiCrcMode;
  FspmUpd->FspmConfig.KtiCpuSktHotPlugEn = Upi->KtiCpuSktHotPlugEn;
  FspmUpd->FspmConfig.KtiCpuSktHotPlugTopology = Upi->KtiCpuSktHotPlugTopology;
  FspmUpd->FspmConfig.KtiSkuMismatchCheck = Upi->KtiSkuMismatchCheck;
  FspmUpd->FspmConfig.IrqThreshold = Upi->IrqThreshold;
  FspmUpd->FspmConfig.TorThresLoctoremNorm = Upi->TorThresLoctoremNorm;
  FspmUpd->FspmConfig.TorThresLoctoremEmpty = Upi->TorThresLoctoremEmpty;
  FspmUpd->FspmConfig.MbeBwCal = Upi->MbeBwCal;
  FspmUpd->FspmConfig.TscSyncEn = Upi->TscSyncEn;
  FspmUpd->FspmConfig.StaleAtoSOptEn = Upi->StaleAtoSOptEn;
  FspmUpd->FspmConfig.LLCDeadLineAlloc = Upi->LLCDeadLineAlloc;
  FspmUpd->FspmConfig.SplitLock = Upi->SplitLock;
  FspmUpd->FspmConfig.mmCfgBase = Upi->mmCfgBase;
  FspmUpd->FspmConfig.mmCfgSize = Upi->mmCfgSize;
  FspmUpd->FspmConfig.mmiohBase = Upi->mmiohBase;
  FspmUpd->FspmConfig.CpuPaLimit = Upi->CpuPaLimit;
  FspmUpd->FspmConfig.highGap = Upi->highGap;
  FspmUpd->FspmConfig.mmiohSize = Upi->mmiohSize;
  FspmUpd->FspmConfig.isocEn = Upi->isocEn;
  FspmUpd->FspmConfig.dcaEn = Upi->dcaEn;
  FspmUpd->FspmConfig.BoardTypeBitmask = Upi->BoardTypeBitmask;
  FspmUpd->FspmConfig.AllLanesPtr = Upi->AllLanesPtr;
  FspmUpd->FspmConfig.PerLanePtr = Upi->PerLanePtr;
  FspmUpd->FspmConfig.AllLanesSizeOfTable = Upi->AllLanesSizeOfTable;
  FspmUpd->FspmConfig.PerLaneSizeOfTable = Upi->PerLaneSizeOfTable;
  FspmUpd->FspmConfig.WaitTimeForPSBP = Upi->WaitTimeForPSBP;
  FspmUpd->FspmConfig.WaSerializationEn = Upi->WaSerializationEn;
  FspmUpd->FspmConfig.KtiInEnableMktme = Upi->KtiInEnableMktme;
  FspmUpd->FspmConfig.BoardId = PlatformInfo->BoardId;
  FspmUpd->FspmArchUpd.NvsBufferPtr = GetFspNvsBuffer ();
}

/**
  This function overrides the default configurations in the FSP-S UPD data region.

  @param[in,out] FspUpdRgnPtr   A pointer to the UPD data region data strcture.

**/
VOID
EFIAPI
UpdateFspsUpdData (
  IN OUT VOID        *FspUpdRgnPtr
  )
{
}

/**
  Update TempRamExit parameter.

  @note At this point, memory is ready, PeiServices are available to use.

  @return TempRamExit parameter.
**/
VOID *
EFIAPI
UpdateTempRamExitParam (
  VOID
  )
{
  return NULL;
}

/**
  Get S3 PEI memory information.

  @note At this point, memory is ready, and PeiServices are available to use.
  Platform can get some data from SMRAM directly.

  @param[out] S3PeiMemSize  PEI memory size to be installed in S3 phase.
  @param[out] S3PeiMemBase  PEI memory base to be installed in S3 phase.

  @return If S3 PEI memory information is got successfully.
**/
EFI_STATUS
EFIAPI
GetS3MemoryInfo (
  OUT UINT64               *S3PeiMemSize,
  OUT EFI_PHYSICAL_ADDRESS *S3PeiMemBase
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Perform platform related reset in FSP wrapper.

  This function will reset the system with requested ResetType.

  @param[in] FspStatusResetType  The type of reset the platform has to perform.
**/
VOID
EFIAPI
CallFspWrapperResetSystem (
  IN UINT32    FspStatusResetType
  )
{
  //
  // Perform reset according to the type.
  //

  CpuDeadLoop();
}
