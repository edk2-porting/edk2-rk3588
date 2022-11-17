/** @file

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Library/MultiPlatSupportLib.h>
#include <Library/HobLib.h>
#include <MultiPlatSupport.h>
#include <Ppi/MemoryDiscovered.h>
#include <Library/ReadFfsLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/AuthenticatedVariableFormat.h>


//notes
//1: security variable and non-security variable may have different implementation
//2. BP core variable sync may need update this module
//3 different generation need to sync base on RP variable and Generation variable
// this library to use a lightweight get variable service to patch variable hob before PEI variable service is ready






/**
   Gets a vairable store header from FFS inserted by FCE

  Arguments:

    DefaultId - Specifies the type of defaults to retrieve.
    BoardId   - Specifies the platform board of defaults to retrieve.


  @return The start address of VARIABLE_STORE_HEADER *. Null if cannot find it

**/

VOID * FindDefaultHobinFfs (
  IN UINT16  DefaultId,
  IN UINT16   BoardId
  )
{
  EFI_PEI_SERVICES           **PeiServices;
  UINTN                      FvInstance;
  EFI_FIRMWARE_VOLUME_HEADER *FvHeader;
  EFI_FFS_FILE_HEADER        *FfsHeader;
  UINT32                     FileSize;
  EFI_COMMON_SECTION_HEADER  *Section;
  UINT32                     SectionLength;
  BOOLEAN                    DefaultFileIsFound;
  DEFAULT_DATA               *DefaultData;
  DEFAULT_INFO               *DefaultInfo;
  VARIABLE_STORE_HEADER      *VarStoreHeader;
  UINT32                      FFSSize = 0;



  //
  // Get PeiService pointer
  //
  PeiServices = (EFI_PEI_SERVICES **)GetPeiServicesTablePointer ();

  //
  // Find the FFS file that stores all default data.
  //
  DefaultFileIsFound = FALSE;
  FvInstance         = 0;
  FfsHeader          = NULL;
  while (((*PeiServices)->FfsFindNextVolume (PeiServices, FvInstance, &FvHeader) == EFI_SUCCESS) &&
         (!DefaultFileIsFound)) {
    FfsHeader = NULL;
    while ((*PeiServices)->FfsFindNextFile (PeiServices, EFI_FV_FILETYPE_FREEFORM, FvHeader, &FfsHeader) == EFI_SUCCESS) {
      if (CompareGuid ((EFI_GUID *) FfsHeader, &gDefaultDataFileGuid)) {
        DefaultFileIsFound = TRUE;
        break;
      }
    }
    FvInstance ++;
  }

  //
  // FFS file is not found.
  //
  if (!DefaultFileIsFound) {

    if(PcdGet32(PcdFailSafeVarFfsSize)!=0 ){
      //try to search other FVS
      FfsHeader = (EFI_FFS_FILE_HEADER *) AllocatePool(PcdGet32(PcdFailSafeVarFfsSize) );
      if(FfsHeader == NULL) {
        return NULL;
      }
      if(EFI_SUCCESS != ReadFFSFile( (EFI_FIRMWARE_VOLUME_HEADER *) PcdGet32(PcdFailSafeVarFvBase), gDefaultDataFileGuid, 0, FfsHeader, &FFSSize, FALSE)) {
        return NULL;
      }
      ASSERT(PcdGet32(PcdFailSafeVarFfsSize) <FFSSize);
    } else {
      return NULL;
    }

  }

  //
  // Find the matched default data for the input default ID and plat ID.
  //
  VarStoreHeader = NULL;
  Section  = (EFI_COMMON_SECTION_HEADER *)(FfsHeader + 1);
  FileSize = *(UINT32 *)(FfsHeader->Size) & 0x00FFFFFF;
  while (((UINTN) Section < (UINTN) FfsHeader + FileSize) && (VarStoreHeader == NULL)) {
    DefaultData = (DEFAULT_DATA *) (Section + 1);
    DefaultInfo = &(DefaultData->DefaultInfo[0]);
    while ((UINTN) DefaultInfo < (UINTN) DefaultData + DefaultData->HeaderSize) {
      if (DefaultInfo->DefaultId == DefaultId && DefaultInfo->BoardId == BoardId) {
        VarStoreHeader = (VARIABLE_STORE_HEADER *) ((UINT8 *) DefaultData + DefaultData->HeaderSize);
        break;
      }
      DefaultInfo ++;
    }
    //
    // Size is 24 bits wide so mask upper 8 bits.
    // SectionLength is adjusted it is 4 byte aligned.
    // Go to the next section
    //
    SectionLength = *(UINT32 *)Section->Size & 0x00FFFFFF;
    SectionLength = (SectionLength + 3) & (~3);
    ASSERT (SectionLength != 0);
    Section = (EFI_COMMON_SECTION_HEADER *)((UINT8 *)Section + SectionLength);
  }

  return VarStoreHeader;

}


/**
  This function searches the first instance of a HOB from the starting HOB pointer.
  Such HOB should satisfy two conditions:
  its HOB type is EFI_HOB_TYPE_GUID_EXTENSION and its GUID Name equals to the input Guid.
  If there does not exist such HOB from the starting HOB pointer, it will return NULL.
  Caller is required to apply GET_GUID_HOB_DATA () and GET_GUID_HOB_DATA_SIZE ()
  to extract the data section and its size info respectively.
  In contrast with macro GET_NEXT_HOB(), this function does not skip the starting HOB pointer
  unconditionally: it returns HobStart back if HobStart itself meets the requirement;
  caller is required to use GET_NEXT_HOB() if it wishes to skip current HobStart.
  If Guid is NULL, then ASSERT().
  If HobStart is NULL, then ASSERT().

  @param  Guid          The GUID to match with in the HOB list.
  @param  HobStart      A pointer to a Guid.

  @return The next instance of the matched GUID HOB from the starting HOB.

**/
VOID *
InternalGetNextGuidHob (
  IN CONST EFI_GUID         *Guid,
  IN CONST VOID             *HobStart
  )
{
  EFI_PEI_HOB_POINTERS  GuidHob;

  GuidHob.Raw = (UINT8 *) HobStart;
  while (!END_OF_HOB_LIST (GuidHob)) {
    if (GuidHob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION && CompareGuid (Guid, &GuidHob.Guid->Name)) {
      break;
    }
    GuidHob.Raw = GET_NEXT_HOB (GuidHob);
  }
  return GuidHob.Raw;
}

EFI_STATUS
CreateDefaultVariableHob (
  IN UINT16  DefaultId,
  IN UINT16   BoardId
  )
/*++
Description:

  This function finds the matched default data and create GUID hob for it.

Arguments:

  DefaultId - Specifies the type of defaults to retrieve.
  BoardId   - Specifies the platform board of defaults to retrieve.

Returns:

  EFI_SUCCESS - The matched default data is found.
  EFI_NOT_FOUND - The matched default data is not found.
  EFI_OUT_OF_RESOURCES - No enough resource to create HOB.

--*/
{
  VARIABLE_STORE_HEADER      *VarStoreHeader;
  VARIABLE_STORE_HEADER      *VarStoreHeaderHob;
  UINT8                      *VarHobPtr;
  UINT8                      *VarPtr;
  UINT32                     VarDataOffset;
  UINT32                     VarHobDataOffset;
  EFI_PEI_SERVICES           **PeiServices;

  //
  // Get PeiService pointer
  //
  PeiServices = (EFI_PEI_SERVICES **)GetPeiServicesTablePointer ();

  VarStoreHeader = (VARIABLE_STORE_HEADER*)FindDefaultHobinFfs( DefaultId, BoardId);

  //
  // Matched default data is not found.
  //
  if (VarStoreHeader == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Create HOB to store defautl data so that Variable driver can use it.
  // Allocate more data for header alignment.
  //
  VarStoreHeaderHob = (VARIABLE_STORE_HEADER *) BuildGuidHob (&VarStoreHeader->Signature, VarStoreHeader->Size + HEADER_ALIGNMENT - 1);
  if (VarStoreHeaderHob == NULL) {
    //
    // No enough hob resource.
    //
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Copy variable storage header.
  //
  CopyMem (VarStoreHeaderHob, VarStoreHeader, sizeof (VARIABLE_STORE_HEADER));
  //
  // Copy variable data.
  //
  VarPtr           = (UINT8 *) HEADER_ALIGN ((UINTN) (VarStoreHeader + 1));
  VarDataOffset    = (UINT32) ((UINTN) VarPtr - (UINTN) VarStoreHeader);
  VarHobPtr        = (UINT8 *) HEADER_ALIGN ((UINTN) (VarStoreHeaderHob + 1));
  VarHobDataOffset = (UINT32) ((UINTN) VarHobPtr - (UINTN) VarStoreHeaderHob);
  CopyMem (VarHobPtr, VarPtr, VarStoreHeader->Size - VarDataOffset);
  //
  // Update variable size.
  //
  VarStoreHeaderHob->Size = VarStoreHeader->Size - VarDataOffset + VarHobDataOffset;

  // SyncSetupVariable(PeiServices,VarStoreHeaderHob,FALSE);

  return EFI_SUCCESS;
}


