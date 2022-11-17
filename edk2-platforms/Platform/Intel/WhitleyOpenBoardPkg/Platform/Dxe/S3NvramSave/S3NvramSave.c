/** @file

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "S3NvramSave.h"
#include <Library/MemoryAllocationLib.h>
#include <Library/LargeVariableReadLib.h>
#include <Library/LargeVariableWriteLib.h>

/**
  Verify the SysHost structure size.

  Verifies that the size of the SysHost structure in PEI is
  the same as the size of the SysHost structure and DXE and
  ASSERT's is the size is not the same.

  This is typically caused by the use of pointers or UINTNs
  in the SysHost structure, neither of those datatypes are
  allowed in SysHost.

  @param  None

  @retval None

**/

VOID
VerifySysHostStructureSize (
  VOID
  )
{

  if (PcdGet32 (PcdPeiSyshostMemorySize) != sizeof (SYSHOST)) {

    DEBUG ((EFI_D_ERROR, "ERROR: In DXE sizeof SysHost = %d, in PEI sizeof SysHost = %d\n",
      sizeof (SYSHOST),
      PcdGet32 (PcdPeiSyshostMemorySize)
      ));

    DEBUG ((EFI_D_ERROR, "Size of SysHost must match in PEI and DXE\n"));
    ASSERT (FALSE);

  }

  return;

} // VerifySysHostStructureSize

/**
  Saves the FSP Non-Volatile Storage HOB to the UEFI Variable Services

  @param      None

  @retval     EFI_SUCCESS  The FSP Non-Volatile Storage HOB was successfully saved.
  @retval     EFI_ERROR    The FSP Non-Volatile Storage HOB was not successfully saved.
**/
EFI_STATUS
SaveFspNonVolatileStorageHob (
  VOID
  )
{
  EFI_STATUS                          Status;
  EFI_HOB_GUID_TYPE                   *GuidHob;
  VOID                                *HobData;
  VOID                                *VariableData;
  UINTN                               DataSize;
  UINTN                               FspNvsBufferSize;
  BOOLEAN                             DataIsIdentical;

  FspNvsBufferSize  = 0;
  DataSize          = 0;
  VariableData      = NULL;
  GuidHob           = NULL;
  HobData           = NULL;
  DataIsIdentical   = FALSE;
  Status            = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "Saving FSP / MRC Training Data\n"));
  GuidHob = GetFirstGuidHob (&gFspNonVolatileStorageHobGuid);
  if (GuidHob != NULL) {
    HobData  = GET_GUID_HOB_DATA (GuidHob);
    DataSize = GET_GUID_HOB_DATA_SIZE (GuidHob);
    if (DataSize > 0) {

      //
      // Check if the presently saved data is identical to the data given by MRC/FSP
      //
      Status = GetLargeVariable (L"FspNvsBuffer", &gFspNonVolatileStorageHobGuid, &FspNvsBufferSize, NULL);
      if (Status == EFI_BUFFER_TOO_SMALL) {
        if (FspNvsBufferSize == DataSize) {
          VariableData = AllocatePool (FspNvsBufferSize);
          if (VariableData != NULL) {
            Status = GetLargeVariable (L"FspNvsBuffer", &gFspNonVolatileStorageHobGuid, &FspNvsBufferSize, VariableData);
            if (!EFI_ERROR (Status) && (FspNvsBufferSize == DataSize) && (0 == CompareMem (HobData, VariableData, DataSize))) {
              DataIsIdentical = TRUE;
            }
            FreePool (VariableData);
          }
        }
      }
      Status = EFI_SUCCESS;

      if (!DataIsIdentical) {
        Status = SetLargeVariable (L"FspNvsBuffer", &gFspNonVolatileStorageHobGuid, TRUE, DataSize, HobData);
        ASSERT_EFI_ERROR (Status);
        DEBUG ((DEBUG_INFO, "Saved size of FSP / MRC Training Data: 0x%x\n", DataSize));
      } else {
        DEBUG ((DEBUG_INFO, "FSP / MRC Training Data is identical to data from last boot, no need to save.\n"));
      }
    }
  }

  return Status;
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
  EFI_STATUS                       Status = EFI_SUCCESS;

  //
  // Check that SysHost size in DXE is the same as PEI.
  //

#ifndef FSP_API_MODE
  VerifySysHostStructureSize ();
#endif

  //
  // Save structures into NVRAM if needed
  //
  Status = SaveFspNonVolatileStorageHob ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: FSP NVRAM Data NOT Saved! Status: %r\n", Status));
    Status = EFI_SUCCESS;
  }

  return Status;
}
