/** @file
  Boot service PEI BIOS ID library implementation.

Copyright (c) 2-015 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BiosIdLib.h>

#include <Guid/BiosId.h>

/**
  This function returns BIOS ID by searching HOB or FV.
  It also debug print the BIOS ID found.

  @param[out] BiosIdImage   The BIOS ID got from HOB or FV. It is optional,
                            no BIOS ID will be returned if it is NULL as input.

  @retval EFI_SUCCESS               BIOS ID has been got successfully.
  @retval EFI_NOT_FOUND             BIOS ID image is not found, and no parameter will be modified.

**/
EFI_STATUS
EFIAPI
GetBiosId (
  OUT BIOS_ID_IMAGE     *BiosIdImage OPTIONAL
  )
{
  EFI_STATUS            Status;
  BIOS_ID_IMAGE         TempBiosIdImage;
  VOID                  *Address;
  UINTN                 Size;
  UINTN                 Instance;
  EFI_PEI_FV_HANDLE     VolumeHandle;
  EFI_PEI_FILE_HANDLE   FileHandle;

  Address = NULL;
  Size = 0;

  if (BiosIdImage == NULL) {
    //
    // It is NULL as input, so no BIOS ID will be returned.
    // Use temp buffer to hold the BIOS ID.
    //
    BiosIdImage = &TempBiosIdImage;
  }

  Address = GetFirstGuidHob (&gBiosIdGuid);
  if (Address != NULL) {
    Size = sizeof (BIOS_ID_IMAGE);
    CopyMem ((VOID *) BiosIdImage, GET_GUID_HOB_DATA (Address), Size);

    DEBUG ((EFI_D_INFO, "PEI get BIOS ID from HOB successfully\n"));
    DEBUG ((EFI_D_INFO, "BIOS ID: %s\n", (CHAR16 *) (&(BiosIdImage->BiosIdString))));
    return EFI_SUCCESS;
  }

  VolumeHandle = NULL;
  Instance = 0;
  while (TRUE) {
    //
    // Traverse all firmware volume instances.
    //
    Status = PeiServicesFfsFindNextVolume (Instance, &VolumeHandle);
    if (EFI_ERROR (Status)) {
      break;
    }

    FileHandle = NULL;
    Status = PeiServicesFfsFindFileByName (&gBiosIdGuid, VolumeHandle, &FileHandle);
    if (!EFI_ERROR (Status)) {
      //
      // Search RAW section.
      //
      Status = PeiServicesFfsFindSectionData (EFI_SECTION_RAW, FileHandle, &Address);
      if (!EFI_ERROR (Status)) {
        //
        // BIOS ID image is present in this FV.
        //
        Size = sizeof (BIOS_ID_IMAGE);
        CopyMem ((VOID *) BiosIdImage, Address, Size);

        DEBUG ((EFI_D_INFO, "PEI get BIOS ID from FV successfully\n"));
        DEBUG ((EFI_D_INFO, "BIOS ID: %s\n", (CHAR16 *) (&(BiosIdImage->BiosIdString))));

        //
        // Build GUID HOB for the BIOS ID image.
        //
        BuildGuidDataHob (&gBiosIdGuid, Address, Size);
        return EFI_SUCCESS;
      }
    }

    //
    // Search the next volume.
    //
    Instance++;
  }

  DEBUG ((EFI_D_ERROR, "PEI get BIOS ID failed: %r\n", EFI_NOT_FOUND));
  return EFI_NOT_FOUND;
}

/**
  This function returns the BIOS Version & Release Date and Time by getting and converting BIOS ID.

  @param[out] BiosVersion       The Bios Version out of the conversion.
  @param[out] BiosReleaseDate   The Bios Release Date out of the conversion.
  @param[out] BiosReleaseTime   The Bios Release Time out of the conversion.

  @retval EFI_SUCCESS               BIOS Version & Release Date and Time have been got successfully.
  @retval EFI_NOT_FOUND             BIOS ID image is not found, and no parameter will be modified.
  @retval EFI_INVALID_PARAMETER     All the parameters are NULL.

**/
EFI_STATUS
EFIAPI
GetBiosVersionDateTime (
  OUT CHAR16    *BiosVersion, OPTIONAL
  OUT CHAR16    *BiosReleaseDate, OPTIONAL
  OUT CHAR16    *BiosReleaseTime OPTIONAL
  )
{
  EFI_STATUS        Status;
  BIOS_ID_IMAGE     BiosIdImage;

  if ((BiosVersion == NULL) && (BiosReleaseDate == NULL) && (BiosReleaseTime == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetBiosId (&BiosIdImage);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  if (BiosVersion != NULL) {
    //
    // Fill the BiosVersion data from the BIOS ID.
    //
    CopyMem (BiosVersion, &(BiosIdImage.BiosIdString), sizeof (BIOS_ID_STRING));
  }

  if (BiosReleaseDate != NULL) {
    //
    // Fill the build timestamp date from the BIOS ID in the "MM/DD/YY" format.
    //
    BiosReleaseDate[0] = BiosIdImage.BiosIdString.TimeStamp[2];
    BiosReleaseDate[1] = BiosIdImage.BiosIdString.TimeStamp[3];
    BiosReleaseDate[2] = (CHAR16) ((UINT8) ('/'));

    BiosReleaseDate[3] = BiosIdImage.BiosIdString.TimeStamp[4];
    BiosReleaseDate[4] = BiosIdImage.BiosIdString.TimeStamp[5];
    BiosReleaseDate[5] = (CHAR16) ((UINT8) ('/'));

    //
    // Add 20 for SMBIOS table
    // Current Linux kernel will misjudge 09 as year 0, so using 2009 for SMBIOS table
    //
    BiosReleaseDate[6] = '2';
    BiosReleaseDate[7] = '0';
    BiosReleaseDate[8] = BiosIdImage.BiosIdString.TimeStamp[0];
    BiosReleaseDate[9] = BiosIdImage.BiosIdString.TimeStamp[1];

    BiosReleaseDate[10] = (CHAR16) ((UINT8) ('\0'));
  }

  if (BiosReleaseTime != NULL) {

    //
    // Fill the build timestamp time from the BIOS ID in the "HH:MM" format.
    //
    BiosReleaseTime[0] = BiosIdImage.BiosIdString.TimeStamp[6];
    BiosReleaseTime[1] = BiosIdImage.BiosIdString.TimeStamp[7];
    BiosReleaseTime[2] = (CHAR16) ((UINT8) (':'));

    BiosReleaseTime[3] = BiosIdImage.BiosIdString.TimeStamp[8];
    BiosReleaseTime[4] = BiosIdImage.BiosIdString.TimeStamp[9];

    BiosReleaseTime[5] = (CHAR16) ((UINT8) ('\0'));
  }

  return  EFI_SUCCESS;
}

