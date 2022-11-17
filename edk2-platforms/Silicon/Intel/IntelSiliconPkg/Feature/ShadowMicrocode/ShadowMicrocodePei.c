/** @file
  FIT based microcode shadow PEIM.

Copyright (c) 2020 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Ppi/ShadowMicrocode.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MicrocodeLib.h>
#include <IndustryStandard/FirmwareInterfaceTable.h>
#include <Register/Intel/Microcode.h>
#include <Register/Intel/Cpuid.h>
#include <Guid/MicrocodeShadowInfoHob.h>
//
// Data structure for microcode patch information
//
typedef struct {
  UINTN    Address;
  UINTN    Size;
} MICROCODE_PATCH_INFO;

/**
  Shadow microcode update patches to memory.

  The function is used for shadowing microcode update patches to a continuous memory.
  It shall allocate memory buffer and only shadow the microcode patches for those
  processors specified by MicrocodeCpuId array. The checksum verification may be
  skiped in this function so the caller must perform checksum verification before
  using the microcode patches in returned memory buffer.

  @param[in]  This                 The PPI instance pointer.
  @param[in]  CpuIdCount           Number of elements in MicrocodeCpuId array.
  @param[in]  MicrocodeCpuId       A pointer to an array of EDKII_PEI_MICROCODE_CPU_ID
                                   structures.
  @param[out] BufferSize           Pointer to receive the total size of Buffer.
  @param[out] Buffer               Pointer to receive address of allocated memory
                                   with microcode patches data in it.

  @retval EFI_SUCCESS              The microcode has been shadowed to memory.
  @retval EFI_OUT_OF_RESOURCES     The operation fails due to lack of resources.

**/
EFI_STATUS
EFIAPI
ShadowMicrocode (
  IN  EDKII_PEI_SHADOW_MICROCODE_PPI        *This,
  IN  UINTN                                 CpuIdCount,
  IN  EDKII_PEI_MICROCODE_CPU_ID            *MicrocodeCpuId,
  OUT UINTN                                 *BufferSize,
  OUT VOID                                  **Buffer
  );


EDKII_PEI_SHADOW_MICROCODE_PPI   mPeiShadowMicrocodePpi = {
  ShadowMicrocode
};


EFI_PEI_PPI_DESCRIPTOR           mPeiShadowMicrocodePpiList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEdkiiPeiShadowMicrocodePpiGuid,
    &mPeiShadowMicrocodePpi
  }
};

/**
  Actual worker function that shadows the required microcode patches into memory.

  @param[in]       Patches          The pointer to an array of information on
                                    the microcode patches that will be loaded
                                    into memory.
  @param[in]       PatchCount       The number of microcode patches that will
                                    be loaded into memory.
  @param[in]       TotalLoadSize    The total size of all the microcode patches
                                    to be loaded.
  @param[out] BufferSize            Pointer to receive the total size of Buffer.
  @param[out] Buffer                Pointer to receive address of allocated memory
                                    with microcode patches data in it.
**/
VOID
ShadowMicrocodePatchWorker (
  IN  MICROCODE_PATCH_INFO       *Patches,
  IN  UINTN                      PatchCount,
  IN  UINTN                      TotalLoadSize,
  OUT UINTN                      *BufferSize,
  OUT VOID                       **Buffer
  )
{
  UINTN                                     Index;
  VOID                                      *MicrocodePatchInRam;
  UINT8                                     *Walker;
  EDKII_MICROCODE_SHADOW_INFO_HOB           *MicrocodeShadowHob;
  UINTN                                     HobDataLength;
  UINT64                                    *MicrocodeAddressInMemory;
  EFI_MICROCODE_STORAGE_TYPE_FLASH_CONTEXT  *Flashcontext;

  ASSERT ((Patches != NULL) && (PatchCount != 0));

  //
  // Init microcode shadow info HOB content.
  //
  HobDataLength = sizeof (EDKII_MICROCODE_SHADOW_INFO_HOB) +
                  sizeof (UINT64) * PatchCount * 2;
  MicrocodeShadowHob  = AllocatePool (HobDataLength);
  if (MicrocodeShadowHob == NULL) {
    ASSERT (FALSE);
    return;
  }
  MicrocodeShadowHob->MicrocodeCount = PatchCount;
  CopyGuid (
    &MicrocodeShadowHob->StorageType,
    &gEdkiiMicrocodeStorageTypeFlashGuid
    );
  MicrocodeAddressInMemory = (UINT64 *) (MicrocodeShadowHob + 1);
  Flashcontext = (EFI_MICROCODE_STORAGE_TYPE_FLASH_CONTEXT *) (MicrocodeAddressInMemory + PatchCount);

  //
  // Allocate memory for microcode shadow operation.
  //
  MicrocodePatchInRam = AllocatePages (EFI_SIZE_TO_PAGES (TotalLoadSize));
  if (MicrocodePatchInRam == NULL) {
    ASSERT (FALSE);
    return;
  }

  //
  // Shadow all the required microcode patches into memory
  //
  for (Walker = MicrocodePatchInRam, Index = 0; Index < PatchCount; Index++) {
    CopyMem (
      Walker,
      (VOID *) Patches[Index].Address,
      Patches[Index].Size
      );
    MicrocodeAddressInMemory[Index] = (UINT64) (UINTN) Walker;
    Flashcontext->MicrocodeAddressInFlash[Index]  = (UINT64) Patches[Index].Address;
    Walker += Patches[Index].Size;
  }

  //
  // Update the microcode patch related fields in CpuMpData
  //
  *Buffer     = (VOID *) (UINTN) MicrocodePatchInRam;
  *BufferSize = TotalLoadSize;

  BuildGuidDataHob (
    &gEdkiiMicrocodeShadowInfoHobGuid,
    MicrocodeShadowHob,
    HobDataLength
    );

  DEBUG ((
    DEBUG_INFO,
    "%a: Required microcode patches have been loaded at 0x%lx, with size 0x%lx.\n",
    __FUNCTION__, *Buffer, *BufferSize
    ));

  return;
}

/**
  Check if FIT table content is valid according to FIT BIOS specification.

**/
BOOLEAN
IsValidFitTable (
  IN  UINT64               FitPointer
  )
{
  UINT64                            FitEnding;
  FIRMWARE_INTERFACE_TABLE_ENTRY    *FitEntry;
  UINT32                            EntryNum;
  UINT32                            Type0Count;
  UINT32                            Index;

  //
  // The entire FIT table must reside with in the firmware address range
  // of (4GB-16MB) to (4GB-40h).
  //
  if ((FitPointer < (SIZE_4GB - SIZE_16MB)) || (FitPointer >= (SIZE_4GB - 0x40))) {
    //
    // Invalid FIT address, treat it as no FIT table.
    //
    DEBUG ((DEBUG_ERROR, "Error: Invalid FIT pointer 0x%p.\n", FitPointer));
    return FALSE;
  }

  //
  // Check FIT header.
  //
  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *) (UINTN) FitPointer;
  if ((FitEntry[0].Type != FIT_TYPE_00_HEADER) ||
      (FitEntry[0].Address != FIT_TYPE_00_SIGNATURE)) {
    DEBUG ((DEBUG_ERROR, "Error: Invalid FIT header.\n"));
    return FALSE;
  }

  //
  // Check FIT version.
  //
  if (FitEntry[0].Version != FIT_TYPE_VERSION) {
    DEBUG ((DEBUG_ERROR, "Error: Unsupported FIT version.\n"));
    return FALSE;
  }

  //
  // Check FIT ending address in valid range.
  //
  EntryNum = *(UINT32 *)(&FitEntry[0].Size[0]) & 0xFFFFFF;
  FitEnding = FitPointer + sizeof (FIRMWARE_INTERFACE_TABLE_ENTRY) * EntryNum;
  if (FitEnding  > (SIZE_4GB - 0x40)) {
    DEBUG ((DEBUG_ERROR, "Error: FIT table exceeds valid range.\n"));
    return FALSE;
  }

  //
  // Calculate FIT checksum if Checksum Valid bit is set.
  //
  if (FitEntry[0].C_V &&
      CalculateSum8 ((UINT8*) FitEntry, sizeof (FIRMWARE_INTERFACE_TABLE_ENTRY) * EntryNum) != 0) {
    DEBUG ((DEBUG_ERROR, "Error: Invalid FIT checksum.\n"));
    return FALSE;
  }

  //
  // Check type 0 entry count.
  //
  Type0Count = 0;
  for (Index = 0; Index < EntryNum; Index++) {
    if (FitEntry[Index].Type == FIT_TYPE_00_HEADER) {
      Type0Count++;
    }
  }
  if (Type0Count != 1) {
    DEBUG ((DEBUG_ERROR, "Error: There can be only one Type 0 entry in FIT.\n"));
    return FALSE;
  }

  return TRUE;
}


/**
  Shadow microcode update patches to memory.

  The function is used for shadowing microcode update patches to a continuous memory.
  It shall allocate memory buffer and only shadow the microcode patches for those
  processors specified by MicrocodeCpuId array. The checksum verification may be
  skiped in this function so the caller must perform checksum verification before
  using the microcode patches in returned memory buffer.

  @param[in]  This                 The PPI instance pointer.
  @param[in]  CpuIdCount           Number of elements in MicrocodeCpuId array.
  @param[in]  MicrocodeCpuId       A pointer to an array of EDKII_PEI_MICROCODE_CPU_ID
                                   structures.
  @param[out] BufferSize           Pointer to receive the total size of Buffer.
  @param[out] Buffer               Pointer to receive address of allocated memory
                                   with microcode patches data in it.

  @retval EFI_SUCCESS              The microcode has been shadowed to memory.
  @retval EFI_INVALID_PARAMETER    BufferSize or Buffer is NULL.
  @retval EFI_INVALID_PARAMETER    CpuIdCount not equal to 0 and MicrocodeCpuId is NULL.
  @retval EFI_NOT_FOUND            No valid microcode found.
  @retval EFI_OUT_OF_RESOURCES     The operation fails due to lack of resources.

**/
EFI_STATUS
EFIAPI
ShadowMicrocode (
  IN  EDKII_PEI_SHADOW_MICROCODE_PPI        *This,
  IN  UINTN                                 CpuIdCount,
  IN  EDKII_PEI_MICROCODE_CPU_ID            *MicrocodeCpuId,
  OUT UINTN                                 *BufferSize,
  OUT VOID                                  **Buffer
  )
{
  EFI_STATUS                        Status;
  UINT64                            FitPointer;
  FIRMWARE_INTERFACE_TABLE_ENTRY    *FitEntry;
  UINT32                            EntryNum;
  UINT32                            Index;
  MICROCODE_PATCH_INFO              *PatchInfoBuffer;
  UINTN                             MaxPatchNumber;
  CPU_MICROCODE_HEADER              *MicrocodeEntryPoint;
  UINTN                             PatchCount;
  UINTN                             TotalSize;
  UINTN                             TotalLoadSize;

  if (BufferSize == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (CpuIdCount != 0 && MicrocodeCpuId == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  FitPointer = *(UINT64 *) (UINTN) FIT_POINTER_ADDRESS;
  if (!IsValidFitTable (FitPointer)) {
    return EFI_NOT_FOUND;
  }

  //
  // Calculate microcode entry number
  //
  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *) (UINTN) FitPointer;
  EntryNum = *(UINT32 *)(&FitEntry[0].Size[0]) & 0xFFFFFF;
  MaxPatchNumber = 0;
  for (Index = 0; Index < EntryNum; Index++) {
    if (FitEntry[Index].Type == FIT_TYPE_01_MICROCODE) {
      MaxPatchNumber++;
    }
  }
  if (MaxPatchNumber == 0) {
    return EFI_NOT_FOUND;
  }

  PatchInfoBuffer = AllocatePool (MaxPatchNumber * sizeof (MICROCODE_PATCH_INFO));
  if (PatchInfoBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (FeaturePcdGet (PcdShadowAllMicrocode)) {
    MicrocodeCpuId = NULL;
    CpuIdCount     = 0;
  }

  //
  // Fill up microcode patch info buffer according to FIT table.
  //
  PatchCount = 0;
  TotalLoadSize = 0;
  for (Index = 0; Index < EntryNum; Index++) {
    if (FitEntry[Index].Type == FIT_TYPE_01_MICROCODE) {
      MicrocodeEntryPoint = (CPU_MICROCODE_HEADER *) (UINTN) FitEntry[Index].Address;
      TotalSize = GetMicrocodeLength (MicrocodeEntryPoint);
      if (IsValidMicrocode (MicrocodeEntryPoint, TotalSize, 0, MicrocodeCpuId, CpuIdCount, FALSE)) {
        PatchInfoBuffer[PatchCount].Address = (UINTN) MicrocodeEntryPoint;
        PatchInfoBuffer[PatchCount].Size    = TotalSize;
        TotalLoadSize += TotalSize;
        PatchCount++;
      }
    }
  }

  if (PatchCount != 0) {
    DEBUG ((
      DEBUG_INFO,
      "%a: 0x%x microcode patches will be loaded into memory, with size 0x%x.\n",
      __FUNCTION__, PatchCount, TotalLoadSize
      ));

    ShadowMicrocodePatchWorker (PatchInfoBuffer, PatchCount, TotalLoadSize, BufferSize, Buffer);
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_NOT_FOUND;
  }

  FreePool (PatchInfoBuffer);
  return Status;
}


/**
  Platform Init PEI module entry point

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS          The function completes successfully
  @retval     EFI_OUT_OF_RESOURCES Insufficient resources to create database
**/
EFI_STATUS
EFIAPI
ShadowMicrocodePeimInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                       Status;

  //
  // Install EDKII Shadow Microcode PPI
  //
  Status = PeiServicesInstallPpi(mPeiShadowMicrocodePpiList);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
