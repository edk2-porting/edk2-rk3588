/** @file
  Read FFS Library

  @copyright
  Copyright 2014 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseMemoryLib.h>
#include <Pi/PiFirmwareFile.h>
#include <Library/ReadFfsLib.h>

//
//EFI_FIRMWARE_FILE_SYSTEM3_GUID indicates support for FFS_ATTRIB_LARGE_SIZE
//And thus support for files 16MB or larger.
//
UINT8 *
PreMemReadFFSFile (
  IN EFI_FIRMWARE_VOLUME_HEADER* FwVolHeader,
  IN EFI_GUID FFSGuid,
  IN UINT32   FFSDataSize,
  IN BOOLEAN skipheader
  )
/*++

Routine Description:

  Read FFS file from specified FV in PreMem phase

Arguments:

  FwVolHeader   - FV Base Address

  FFSGuid       - FFS to find & Read

  FFSDataSize   - Data size to read.If this value equal 0, will read the whole FFS size

  skipheader    - TRUE: skip to read the ffs and first section header,read from data directly, for one data section only ffs
                  FALSE:read from header

Returns:

  None

--*/
{
  EFI_FFS_FILE_HEADER         *ffsHdr;
  UINT64                       FvSize;
  UINT32                       FileOccupiedSize;
  UINT32                       FFSDataOffset;
  EFI_FIRMWARE_VOLUME_EXT_HEADER        *FwVolExtHeader;
  UINT32                      FFSSize = 0;

  FvSize = 0;
  FFSDataOffset = 0;
  if (FwVolHeader->ExtHeaderOffset != 0) {
    //
    // Searching for files starts on an 8 byte aligned boundary after the end of the Extended Header if it exists.
    //
    FwVolExtHeader = (EFI_FIRMWARE_VOLUME_EXT_HEADER *) ((UINTN)FwVolHeader + FwVolHeader->ExtHeaderOffset );
    ffsHdr = (EFI_FFS_FILE_HEADER *) ((UINT8 *) FwVolExtHeader + FwVolExtHeader->ExtHeaderSize);
  } else {
    ffsHdr =  (EFI_FFS_FILE_HEADER*)(((UINT8 *) FwVolHeader)+ FwVolHeader->HeaderLength);
  }
  ffsHdr = (EFI_FFS_FILE_HEADER *) ALIGN_POINTER (ffsHdr, 8);

  if(FwVolHeader->FvLength == 0xFFFFFFFFFFFFFFFF)  return NULL;

  FvSize =     (UINTN)ffsHdr-(UINTN)FwVolHeader;
  while((FvSize < FwVolHeader->FvLength)&&((UINTN)ffsHdr <((UINTN)FwVolHeader+ (UINTN)FwVolHeader->FvLength))){
     if(CompareGuid (&ffsHdr->Name, &FFSGuid)) break;

     if (IS_FFS_FILE2 (ffsHdr)) {
       FileOccupiedSize = FFS_FILE2_SIZE (ffsHdr) ;
     } else {
       FileOccupiedSize = FFS_FILE_SIZE (ffsHdr) ;
     }

     FvSize+= FileOccupiedSize;
     ffsHdr = (EFI_FFS_FILE_HEADER *)((UINT8 *)ffsHdr + FileOccupiedSize);
     ffsHdr = (EFI_FFS_FILE_HEADER *) ALIGN_POINTER (ffsHdr, 8);
  }

  if (FvSize < FwVolHeader->FvLength) {
     if (IS_FFS_FILE2 (ffsHdr)) {
       FileOccupiedSize = FFS_FILE2_SIZE (ffsHdr) ;
     } else {
       FileOccupiedSize = FFS_FILE_SIZE (ffsHdr) ;
     }
    FFSSize = FileOccupiedSize;

    if(FFSDataSize == 0)
      FFSDataSize= FFSSize;

    if(skipheader){
      if (IS_FFS_FILE2 (ffsHdr)) {
        FFSDataOffset = sizeof(EFI_FFS_FILE_HEADER2) + sizeof(EFI_COMMON_SECTION_HEADER2);
      } else {
        FFSDataOffset = sizeof(EFI_FFS_FILE_HEADER) + sizeof(EFI_COMMON_SECTION_HEADER);
      }
      if(FFSDataSize == 0) {
        if (IS_FFS_FILE2 (ffsHdr)) {
          FFSDataSize = FFSDataSize - sizeof(EFI_FFS_FILE_HEADER2) - sizeof(EFI_COMMON_SECTION_HEADER2);
        } else {
          FFSDataSize = FFSDataSize - sizeof(EFI_FFS_FILE_HEADER) - sizeof(EFI_COMMON_SECTION_HEADER);
        }
      }
    }
  }

   return (UINT8 *)ffsHdr + FFSDataOffset;
}

EFI_STATUS
ReadFFSFile (
  IN EFI_FIRMWARE_VOLUME_HEADER* FwVolHeader,
  IN EFI_GUID FFSGuid,
  IN UINT32   FFSDataSize,
  IN OUT VOID *FFSData,
  OUT UINT32  *FFSSize,
  IN BOOLEAN skipheader
  )
/*++

Routine Description:

  Read FFS file from FV

Arguments:

  FwVolHeader - FV Base Address

  FFSGuid       - FFS to find & Read

  FFSDataSize   - Data size to read.If this value equal 0, will read the whole FFS size

  FFSData       - Pointer to buffer for read.

  FFSSize  - FFS file size FYI for caller.

  skipheader - TRUE: skip to read the ffs and first section header,read from data directly, for one data section only ffs
               FALSE:read from header

Returns:

  None

--*/
{
  EFI_FFS_FILE_HEADER         *ffsHdr;
  UINT64                       FvSize;
  UINT32                       FileOccupiedSize;
  UINT32                       FFSDataOffset;
  EFI_STATUS                   Status;
  EFI_FIRMWARE_VOLUME_EXT_HEADER        *FwVolExtHeader;

  Status = EFI_NOT_FOUND;
  *FFSSize = 0;
  FvSize = 0;
  FFSDataOffset = 0;
  if (FwVolHeader->ExtHeaderOffset != 0) {
    //
    // Searching for files starts on an 8 byte aligned boundary after the end of the Extended Header if it exists.
    //
    FwVolExtHeader = (EFI_FIRMWARE_VOLUME_EXT_HEADER *) ((UINTN)FwVolHeader + FwVolHeader->ExtHeaderOffset );
    ffsHdr = (EFI_FFS_FILE_HEADER *) ((UINT8 *) FwVolExtHeader + FwVolExtHeader->ExtHeaderSize);
  } else {
    ffsHdr =  (EFI_FFS_FILE_HEADER*)(((UINT8 *) FwVolHeader)+ FwVolHeader->HeaderLength);
  }
  ffsHdr = (EFI_FFS_FILE_HEADER *) ALIGN_POINTER (ffsHdr, 8);

  if(FwVolHeader->FvLength == 0xFFFFFFFFFFFFFFFF) {
    return EFI_VOLUME_CORRUPTED;
  }

  FvSize =     (UINTN)ffsHdr-(UINTN)FwVolHeader;
  while((FvSize < FwVolHeader->FvLength)&&((UINTN)ffsHdr <((UINTN)FwVolHeader+ (UINTN)FwVolHeader->FvLength))){
     if(CompareGuid (&ffsHdr->Name, &FFSGuid)){
       Status = EFI_SUCCESS;
       break;
     }
     if (IS_FFS_FILE2 (ffsHdr)) {
       FileOccupiedSize = FFS_FILE2_SIZE (ffsHdr) ;
     } else {
       FileOccupiedSize = FFS_FILE_SIZE (ffsHdr) ;
     }

     FvSize+= FileOccupiedSize;
     ffsHdr = (EFI_FFS_FILE_HEADER *)((UINT8 *)ffsHdr + FileOccupiedSize);
     ffsHdr = (EFI_FFS_FILE_HEADER *) ALIGN_POINTER (ffsHdr, 8);
  }

  if (FvSize < FwVolHeader->FvLength) {
     if (IS_FFS_FILE2 (ffsHdr)) {
       FileOccupiedSize = FFS_FILE2_SIZE (ffsHdr) ;
     } else {
       FileOccupiedSize = FFS_FILE_SIZE (ffsHdr) ;
     }
    *FFSSize = FileOccupiedSize;

  if(FFSDataSize == 0) {
    FFSDataSize= *FFSSize;
  }

  if(skipheader){
    if (IS_FFS_FILE2 (ffsHdr)) {
      FFSDataOffset = sizeof(EFI_FFS_FILE_HEADER2) + sizeof(EFI_COMMON_SECTION_HEADER2);
    } else {
      FFSDataOffset = sizeof(EFI_FFS_FILE_HEADER) + sizeof(EFI_COMMON_SECTION_HEADER);
    }
    if(FFSDataSize == 0) {
      if (IS_FFS_FILE2 (ffsHdr)) {
        FFSDataSize = FFSDataSize - sizeof(EFI_FFS_FILE_HEADER2) - sizeof(EFI_COMMON_SECTION_HEADER2);
      } else {
        FFSDataSize = FFSDataSize - sizeof(EFI_FFS_FILE_HEADER) - sizeof(EFI_COMMON_SECTION_HEADER);
      }
    }
  }

  CopyMem(FFSData,(UINT8*)ffsHdr + FFSDataOffset,FFSDataSize);
  }
  return Status;
}

BOOLEAN
NormalHobToCompressHob(IN OUT VOID* hobAddr,IN OUT UINTN* size)
{
  UINTN i,j,k;
  COMPRESS_HOBO_DATA CompressHob;
  COMPRESS_ITEM Hobitem[MAX_COMPRESS_ITEM];
  UINT16 TempBuffer[MAX_FFS_BUFFER_SIZE/4];
  UINTN offset=0;
  UINTN RemainSize = 0;

  CompressHob.Signature = 0x5A45524F;
  CompressHob.Count =0;
  j=0;
  i=0;
  RemainSize = (*size % 2) + 2;
  if(MAX_COMPRESS_ITEM*sizeof(COMPRESS_ITEM)+MAX_FFS_BUFFER_SIZE/4+sizeof(COMPRESS_HOBO_DATA) > MAX_FFS_BUFFER_SIZE) {
    return FALSE;
  }

  if((*size) > MAX_FFS_BUFFER_SIZE) {
    return FALSE;
  }

 ZeroMem(TempBuffer,MAX_FFS_BUFFER_SIZE/2);

  while(i < (*size - RemainSize)){

  if(j>=MAX_COMPRESS_ITEM) {
    return FALSE;
  }

   //search for duplicate array
   if(*(UINT16*)((UINTN)hobAddr+i)==*(UINT16*)((UINTN)hobAddr+i+2) ){

    for(k=2;(i+k)<(*size- RemainSize);k+=2 ){
         if(*(UINT16*)((UINTN)hobAddr+i)!=*(UINT16*)((UINTN)hobAddr+i+k)){
           break;
         }
    }

    if(i+k>= *size - (*size % 2)) {
      k -=2;
    }

    Hobitem[j].Value = *(UINT16*)((UINTN)hobAddr+i);
    Hobitem[j].Length =(UINT16) k;
    Hobitem[j].Type = COMPRESS_DUPLICATE;
    Hobitem[j].Offset = 0;
    j++;
    CompressHob.Count =(UINT32)j;
    i+=k;

   }
   else{//single array
    for(k=2;i+k+2 <= (*size- RemainSize);k+=2){
       if(offset >= MAX_FFS_BUFFER_SIZE/4) {
         return FALSE;
       }
       TempBuffer[offset]= *(UINT16*)((UINTN)hobAddr+i+k-2);
       offset +=1;
       if(*(UINT16*)((UINTN)hobAddr+i+k)==*(UINT16*)((UINTN)hobAddr+i+k+2)){
         k += 2;
         break;
       }

    }


    Hobitem[j].Length = (UINT16) k - 2;
    Hobitem[j].Type = COMPRESS_SINGLE;
    Hobitem[j].Offset = (UINT16)(offset - (Hobitem[j].Length/2) );
    Hobitem[j].Value =0;
    j++;
    CompressHob.Count  =(UINT32)j;
    i+=k - 2;

   }

  }

  if(j>=MAX_COMPRESS_ITEM) {
    //not worth to compress
    return FALSE;
  }

  //process last one
  CopyMem((UINT8*)(&TempBuffer[offset]), (UINT8*)((UINTN)hobAddr + *size - RemainSize), RemainSize);
  Hobitem[j].Length = (UINT16)RemainSize;
  Hobitem[j].Type = COMPRESS_SINGLE;
  Hobitem[j].Offset = (UINT16)offset;
  Hobitem[j].Value =0;
  j++;
  CompressHob.Count  =(UINT32)j;

  CopyMem(hobAddr,(VOID*)&CompressHob,sizeof(COMPRESS_HOBO_DATA));
  offset = sizeof(COMPRESS_HOBO_DATA);
  for(i=0; i < CompressHob.Count;i++){
    CopyMem((UINT8*)((UINTN)hobAddr+offset),(UINT8*)&Hobitem[i],sizeof(COMPRESS_ITEM));
    offset += sizeof(COMPRESS_ITEM);
    if(Hobitem[i].Type == COMPRESS_SINGLE){
      CopyMem((UINT8*)((UINTN)hobAddr+offset),&TempBuffer[Hobitem[i].Offset],Hobitem[i].Length);
      offset += Hobitem[i].Length;
    }
  }

  *size = offset;

  return TRUE;
}

BOOLEAN
CompressHobToNormalHob(IN OUT VOID* hobAddr,OUT UINTN* size)
{
  UINTN i;
  COMPRESS_HOBO_DATA CompressHob  ;
  COMPRESS_ITEM Hobitem[MAX_COMPRESS_ITEM];
  UINTN offset=0;
  UINT16 TempBuffer[MAX_FFS_BUFFER_SIZE/4];

  CopyMem((VOID*)&CompressHob,hobAddr,sizeof(COMPRESS_HOBO_DATA));

  if(CompressHob.Signature != 0x5A45524F) {
    return FALSE;
  }

  if(CompressHob.Count>MAX_COMPRESS_ITEM) {
    //not worth to compress
    return FALSE;
  }

  if(*size > MAX_FFS_BUFFER_SIZE) {
    return FALSE;
  }

  if(MAX_COMPRESS_ITEM*sizeof(COMPRESS_ITEM)+MAX_FFS_BUFFER_SIZE/4+sizeof(COMPRESS_HOBO_DATA) > MAX_FFS_BUFFER_SIZE) {
    return FALSE;
  }

  offset = sizeof(COMPRESS_HOBO_DATA);
  for(i=0; i < CompressHob.Count;i++){
    CopyMem((VOID*)&Hobitem[i],(UINT8*)((UINTN)hobAddr+offset),sizeof(COMPRESS_ITEM));
    offset += sizeof(COMPRESS_ITEM);
    if(Hobitem[i].Type == COMPRESS_SINGLE){
      CopyMem(&TempBuffer[Hobitem[i].Offset],(UINT8*)((UINTN)hobAddr+offset),Hobitem[i].Length);
      offset += Hobitem[i].Length;
    }
  }

  offset =0;
  for(i=0; i < CompressHob.Count;i++){
    if(Hobitem[i].Type ==COMPRESS_DUPLICATE){
     SetMem16((VOID*)((UINTN)hobAddr+offset),Hobitem[i].Length,Hobitem[i].Value);
     offset += Hobitem[i].Length;
    }
    else if(Hobitem[i].Type ==COMPRESS_SINGLE){
     CopyMem((UINT8*)((UINTN)hobAddr+offset),&TempBuffer[Hobitem[i].Offset],Hobitem[i].Length);
     offset += Hobitem[i].Length;

    }

  }

  *size = offset;

  return TRUE;
}

EFI_STATUS
ValidateCommonFvHeader (
  EFI_FIRMWARE_VOLUME_HEADER            *FwVolHeader
  )
/*++

Routine Description:
  Check the integrity of firmware volume header

Arguments:
  FwVolHeader           - A pointer to a firmware volume header

Returns:
  EFI_SUCCESS           - The firmware volume is consistent
  EFI_NOT_FOUND         - The firmware volume has corrupted. So it is not an FV

--*/
{
  UINT16  *Ptr;
  UINT16  HeaderLength;
  UINT16  Checksum;

  //
  // Verify the header revision, header signature, length
  // Length of FvBlock cannot be 2**64-1
  // HeaderLength cannot be an odd number
  //
  if ((FwVolHeader->Revision != EFI_FVH_REVISION) ||
      (FwVolHeader->Signature != EFI_FVH_SIGNATURE) ||
      (FwVolHeader->FvLength == ((UINTN) -1)) ||
      ((FwVolHeader->HeaderLength & 0x01) != 0)
      ) {
    return EFI_NOT_FOUND;
  }
  //
  // Verify the header checksum
  //
  HeaderLength  = (UINT16) (FwVolHeader->HeaderLength / 2);
  Ptr           = (UINT16 *) FwVolHeader;
  Checksum      = 0;
  while (HeaderLength > 0) {
    Checksum = Checksum + (*Ptr);
    Ptr++;
    HeaderLength--;
  }

  if (Checksum != 0) {
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}
