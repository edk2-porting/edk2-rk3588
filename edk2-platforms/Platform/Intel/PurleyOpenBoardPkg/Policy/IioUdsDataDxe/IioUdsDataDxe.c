/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

//
// Statements that include other files
//
#include "IioUdsDataDxe.h"

#define STRING_WIDTH_40    40

//
// Instantiation of Driver's private data.
//
EFI_IIO_UDS_DRIVER_PRIVATE          mIioUdsPrivateData;
IIO_UDS                             *IioUdsData;          // Pointer to UDS in Allocated Memory Pool

/**

  Entry point for the driver.

  @param ImageHandle  -  Image Handle.
  @param SystemTable  -  EFI System Table.
  
  @retval EFI_SUCCESS  -  Function has completed successfully.
  
**/
EFI_STATUS
EFIAPI
IioUdsDataInit (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_HOB_GUID_TYPE               *GuidHob;
  IIO_UDS                         *UdsHobPtr;
  EFI_GUID                        UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;

  //
  // Time to get the IIO_UDS HOB data stored in the PEI driver
  //
  GuidHob    = GetFirstGuidHob (&UniversalDataGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  UdsHobPtr = GET_GUID_HOB_DATA(GuidHob); 

  //
  // Allocate Memory Pool for Universal Data Storage so that protocol can expose it
  //
  Status = gBS->AllocatePool ( EfiReservedMemoryType, sizeof (IIO_UDS), (VOID **) &IioUdsData );
  ASSERT_EFI_ERROR (Status);
  
  //
  // Initialize the Pool Memory with the data from the Hand-Off-Block
  //
  CopyMem(IioUdsData, UdsHobPtr, sizeof(IIO_UDS));

  //
  // Build the IIO_UDS driver instance for protocol publishing  
  //
  ZeroMem (&mIioUdsPrivateData, sizeof (mIioUdsPrivateData));
    
  mIioUdsPrivateData.Signature            = EFI_IIO_UDS_DRIVER_PRIVATE_SIGNATURE;
  mIioUdsPrivateData.IioUds.IioUdsPtr     = IioUdsData;
  mIioUdsPrivateData.IioUds.EnableVc      = NULL;

  //
  // Install the IioUds Protocol.
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mIioUdsPrivateData.Handle,
                  &gEfiIioUdsProtocolGuid,
                  &mIioUdsPrivateData.IioUds,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);  

  return EFI_SUCCESS;
}

