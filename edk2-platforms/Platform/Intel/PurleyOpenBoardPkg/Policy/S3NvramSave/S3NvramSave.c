/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "S3NvramSave.h"
#include <Protocol/Decompress.h>
#include <Library/CompressLib.h>
#include <Protocol/VariableLock.h>

/**

  This function will retrieve the S3 data from HOBs produced by MRC
  and will save it to NVRAM if the data is absent or different from
  the previously saved data.

  @param VOID

  @retval VOID

**/
VOID
SaveS3StructToNvram (
  VOID
  )
{
  EFI_STATUS                      Status;
  UINTN                           BufferSize;
  UINTN                           CurrentHobSize;
  UINTN                           S3ChunkSize;
  CHAR16                          EfiMemoryConfigVariable[] = L"MemoryConfig0";
  EFI_HOB_GUID_TYPE               *GuidHob      = NULL;
  VOID                            *HobData      = NULL;
  VOID                            *VariableData = NULL;
  
  UINTN                           CompressedDataSize;
  UINT32                          ScratchSize;
  VOID                            *CompressedData = NULL;
  VOID                            *Scratch = NULL;
  EFI_DECOMPRESS_PROTOCOL         *Decompress = NULL;
  VOID                            *CompressedVariableData = NULL;
  UINTN                           CompressedBufferSize;
  EDKII_VARIABLE_LOCK_PROTOCOL    *VariableLock = NULL;
  
  //
  // Get first S3 data HOB
  //
  GuidHob = GetFirstGuidHob (&gEfiMemoryConfigDataHobGuid);

  Status = gBS->LocateProtocol (&gEfiDecompressProtocolGuid, NULL, (VOID **) &Decompress);
  DEBUG((DEBUG_INFO, "[SaveMemoryConfigEntryPoint] Locate Decompress protocol - %r\n", Status));
  if(EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }
  
  Status = gBS->LocateProtocol(&gEdkiiVariableLockProtocolGuid, NULL, (VOID **)&VariableLock);
  DEBUG((DEBUG_INFO, "[SaveMemoryConfigEntryPoint] Locate Variable Lock protocol - %r\n", Status));
  ASSERT_EFI_ERROR(Status); 

  while (TRUE) {
    if (GuidHob == NULL) {
      break;
    }
    HobData = GET_GUID_HOB_DATA(GuidHob); 
    CurrentHobSize = GET_GUID_HOB_DATA_SIZE (GuidHob);

    DEBUG((EFI_D_INFO, "   Current Hob Size(bytes) is: %d\n", CurrentHobSize));
    //
    // Use the HOB data to save Memory Configuration Data
    //
    BufferSize = CurrentHobSize;
    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    BufferSize,
                    (VOID**)&VariableData
                    );

    ASSERT (VariableData != NULL); 
    S3ChunkSize = MAX_HOB_ENTRY_SIZE / 8;
    DEBUG((EFI_D_INFO, "   S3ChunkSize Hob Size(bytes): %d\n", S3ChunkSize));

    while (CurrentHobSize) {
      if (S3ChunkSize > CurrentHobSize) {
        S3ChunkSize = CurrentHobSize;
      }
      BufferSize = S3ChunkSize;
      CompressedDataSize = 0;
      ScratchSize = 0;
      Status = gRT->GetVariable (
                      EfiMemoryConfigVariable,
                      &gEfiMemoryConfigDataGuid,
                      NULL,
                      &CompressedDataSize,
                      NULL
                      );
                    
      if(Status == EFI_BUFFER_TOO_SMALL) {
        Status = gBS->AllocatePool (
                        EfiBootServicesData,
                        CompressedDataSize,
                        (VOID**)&CompressedData
                        );
        ASSERT (Status == EFI_SUCCESS); 
      }

      if(!EFI_ERROR (Status))
      {
        Status = gRT->GetVariable (
                        EfiMemoryConfigVariable,
                        &gEfiMemoryConfigDataGuid,
                        NULL,
                        &CompressedDataSize,
                        CompressedData
                        );

        if (!EFI_ERROR (Status)) {
         Status = Decompress->GetInfo (
                                Decompress,
                                CompressedData,
                                (UINT32)CompressedDataSize,
                                (UINT32*)&BufferSize,
                                &ScratchSize
                                );
        }

        if (!EFI_ERROR (Status)) {
          Status = gBS->AllocatePool (
                          EfiBootServicesData,
                          ScratchSize,
                          (VOID**)&Scratch
                          );
        }

        if (!EFI_ERROR (Status)) {
          Status = Decompress->Decompress (
                                 Decompress,
                                 CompressedData,
                                 (UINT32)CompressedDataSize,
                                 VariableData,
                                 (UINT32)BufferSize,
                                 Scratch,
                                 ScratchSize
                                 );
        }

        if (EFI_ERROR (Status)) {
          DEBUG((EFI_D_ERROR, "Getting variables error: 0x%x\n", Status));
          ASSERT (Status == EFI_SUCCESS); 
        }

        if(Scratch != NULL) {
          gBS->FreePool (Scratch);
          Scratch = NULL;
        }
      }

      if(CompressedData != NULL) {
        gBS->FreePool (CompressedData);
        CompressedData = NULL;
      }

      if ( (EFI_ERROR(Status)) || (CompareMem (HobData, VariableData, S3ChunkSize) != 0) ) {
        Status = gBS->AllocatePool (
                        EfiBootServicesData,
                        BufferSize,
                        (VOID**)&CompressedVariableData
                        );
        ASSERT (CompressedVariableData != NULL); 
        if (Status == EFI_SUCCESS) {
          CompressedBufferSize = BufferSize;
          Status = Compress(HobData, S3ChunkSize, CompressedVariableData, &CompressedBufferSize);
          if (Status == EFI_BUFFER_TOO_SMALL){
            gBS->FreePool(CompressedVariableData);
            Status = gBS->AllocatePool(
                            EfiBootServicesData,
                            CompressedBufferSize,
                            (VOID**)&CompressedVariableData
                            );
            ASSERT (CompressedVariableData != NULL);
            Status = Compress(HobData, S3ChunkSize, CompressedVariableData, &CompressedBufferSize);
          }
          if(Status == EFI_SUCCESS) {
            Status = gRT->SetVariable (
                            EfiMemoryConfigVariable,
                            &gEfiMemoryConfigDataGuid,
                            EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                            CompressedBufferSize,
                            CompressedVariableData
                            );
          }
          if(CompressedVariableData != NULL) {
            gBS->FreePool(CompressedVariableData);
            CompressedVariableData = NULL;
          }
      }
    
        if (EFI_ERROR (Status)) {
          DEBUG((EFI_D_ERROR, "Set variable error. Status: 0x%x\n", Status));
          ASSERT_EFI_ERROR (Status);
        }
      }
      //
      // Lock the Memory Config Variable
      //
      Status = VariableLock->RequestToLock(VariableLock, EfiMemoryConfigVariable, &gEfiMemoryConfigDataGuid);
      ASSERT_EFI_ERROR(Status);
      HobData = (UINT8 *) (HobData) + S3ChunkSize;
    
      CurrentHobSize -= S3ChunkSize;
      EfiMemoryConfigVariable[12]++;  // Increment number in the string
    }
    //
    // Get next S3 Config data hob, if none left, results NULL
    //
    GuidHob = GET_NEXT_HOB (GuidHob); // Increment to next HOB
    GuidHob = GetNextGuidHob (&gEfiMemoryConfigDataHobGuid, GuidHob);  // Now search for next MemConfig HOB

    if(VariableData != NULL) {
      gBS->FreePool(VariableData);
      VariableData = NULL;
    }
  }

  return;
}

EFI_STATUS
EFIAPI
S3NvramSaveEntry ( 
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  ) 
/**

  This is the main entry point of the S3 NVRAM Save module.

  @param ImageHandle  -  Handle for the image of this driver.
  @param SystemTable  -  Pointer to the EFI System Table.

  @retval EFI_SUCCESS  -  Module launched successfully.

**/
{
  EFI_STATUS Status = EFI_SUCCESS;
  
  //
  // Save the s3 strututre from MRC into NVRAM if needed
  //
  SaveS3StructToNvram();
  
  return Status;
  
}
