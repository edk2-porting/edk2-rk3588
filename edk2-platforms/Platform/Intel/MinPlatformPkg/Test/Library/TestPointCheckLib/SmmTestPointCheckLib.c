/** @file

Copyright (c) 2017 - 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiSmm.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiLib.h>
#include <Library/SmmMemLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Guid/MemoryAttributesTable.h>

#include "TestPointInternal.h"

GLOBAL_REMOVE_IF_UNREFERENCED EFI_GUID mTestPointSmmCommunciationGuid = TEST_POINT_SMM_COMMUNICATION_GUID;

EFI_STATUS
TestPointCheckSmrr (
  VOID
  );

EFI_STATUS
TestPointDumpSmmLoadedImage (
  VOID
  );

EFI_STATUS
TestPointCheckSmmMemAttribute (
  VOID
  );

EFI_STATUS
TestPointCheckSmmPaging (
  VOID
  );

EFI_STATUS
TestPointCheckSmmCommunicationBuffer (
  IN EFI_MEMORY_DESCRIPTOR        *UefiMemoryMap,
  IN UINTN                        UefiMemoryMapSize,
  IN UINTN                        UefiDescriptorSize,
  IN EFI_MEMORY_ATTRIBUTES_TABLE  *MemoryAttributesTable
  );

VOID
TestPointDumpGcd (
  OUT EFI_GCD_MEMORY_SPACE_DESCRIPTOR **GcdMemoryMap,  OPTIONAL
  OUT UINTN                           *GcdMemoryMapNumberOfDescriptors,  OPTIONAL
  OUT EFI_GCD_IO_SPACE_DESCRIPTOR     **GcdIoMap,  OPTIONAL
  OUT UINTN                           *GcdIoMapNumberOfDescriptors,  OPTIONAL
  IN  BOOLEAN                         DumpPrint
  );

VOID
TestPointDumpUefiMemoryMap (
  OUT EFI_MEMORY_DESCRIPTOR **UefiMemoryMap, OPTIONAL
  OUT UINTN                 *UefiMemoryMapSize, OPTIONAL
  OUT UINTN                 *UefiDescriptorSize, OPTIONAL
  IN  BOOLEAN               DumpPrint
  );

GLOBAL_REMOVE_IF_UNREFERENCED EFI_MEMORY_DESCRIPTOR *mUefiMemoryMap;
GLOBAL_REMOVE_IF_UNREFERENCED UINTN                 mUefiMemoryMapSize;
GLOBAL_REMOVE_IF_UNREFERENCED UINTN                 mUefiDescriptorSize;

GLOBAL_REMOVE_IF_UNREFERENCED EFI_GCD_MEMORY_SPACE_DESCRIPTOR *mGcdMemoryMap;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_GCD_IO_SPACE_DESCRIPTOR     *mGcdIoMap;
GLOBAL_REMOVE_IF_UNREFERENCED UINTN                           mGcdMemoryMapNumberOfDescriptors;
GLOBAL_REMOVE_IF_UNREFERENCED UINTN                           mGcdIoMapNumberOfDescriptors;

EFI_MEMORY_ATTRIBUTES_TABLE  *mUefiMemoryAttributesTable;

GLOBAL_REMOVE_IF_UNREFERENCED ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT  mTestPointStruct = {
  PLATFORM_TEST_POINT_VERSION,
  PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
  {TEST_POINT_IMPLEMENTATION_ID_PLATFORM_SMM},
  TEST_POINT_FEATURE_SIZE,
  {0}, // FeaturesImplemented
  {0}, // FeaturesVerified
  0,
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT8  mFeatureImplemented[TEST_POINT_FEATURE_SIZE];

/**
  This service verifies SMRR configuration at the End of DXE.

  Test subject: SMRR.
  Test overview: Verify SMRR is aligned and SMRR matches SMRAM_INFO.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps SMRR and SMRAM_INFO.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointSmmEndOfDxeSmrrFunctional (
  VOID
  )
{
  EFI_STATUS  Status;
  BOOLEAN     Result;

  if ((mFeatureImplemented[TEST_POINT_INDEX_BYTE6_SMM] & TEST_POINT_BYTE6_SMM_END_OF_DXE_SMRR_FUNCTIONAL) == 0) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "======== TestPointSmmEndOfDxeSmrrFunctional - Enter\n"));

  Result = TRUE;
  Status = TestPointCheckSmrr ();
  if (EFI_ERROR(Status)) {
    Result = FALSE;
  }

  if (Result) {
    TestPointLibSetFeaturesVerified (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      TEST_POINT_INDEX_BYTE6_SMM,
      TEST_POINT_BYTE6_SMM_END_OF_DXE_SMRR_FUNCTIONAL
      );
  }

  DEBUG ((DEBUG_INFO, "======== TestPointSmmEndOfDxe - Exit\n"));
  return EFI_SUCCESS;
}

/**
  This service verifies the validity of the SMM memory atttribute table at SMM Ready To Lock.

  Test subject: SMM memory attribute table.
  Test overview: Verify the SMM memory attribute table is reported.
                 Verify image code/data is consistent with the SMM memory attribute table.
                 Verify the GDT/IDT/PageTable is RO, data is NX, and code is RO.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps the SMM memory attribute table and SMM image information.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointSmmReadyToLockSmmMemoryAttributeTableFunctional (
  VOID
  )
{
  EFI_STATUS  Status;
  BOOLEAN     Result;

  if ((mFeatureImplemented[TEST_POINT_INDEX_BYTE6_SMM] &
    TEST_POINT_BYTE6_SMM_READY_TO_LOCK_SMM_MEMORY_ATTRIBUTE_TABLE_FUNCTIONAL) == 0) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "======== TestPointSmmReadyToLock - Enter\n"));

  Result = TRUE;
  TestPointDumpSmmLoadedImage ();
  Status = TestPointCheckSmmMemAttribute ();
  if (EFI_ERROR(Status)) {
    Result = FALSE;
  }

  if (Result) {
    TestPointLibSetFeaturesVerified (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      TEST_POINT_INDEX_BYTE6_SMM,
      TEST_POINT_BYTE6_SMM_READY_TO_LOCK_SMM_MEMORY_ATTRIBUTE_TABLE_FUNCTIONAL
      );
  }

  DEBUG ((DEBUG_INFO, "======== TestPointSmmReadyToLock - Exit\n"));
  return EFI_SUCCESS;
}

/**
  This service verifies the security of SMM communication buffers at SMM Ready To Lock.

  Test subject: SMM communication buffer.
  Test overview: Verify only CommBuffer and MMIO are mapped in the page table.
  Reporting mechanism: Dumps the memory map and GCD map at SmmReadyToLock and checks at SmmReadyToBoot.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointSmmReadyToLockSecureSmmCommunicationBuffer (
  VOID
  )
{
  EFI_STATUS                   Status;
  EFI_MEMORY_ATTRIBUTES_TABLE  *MemoryAttributesTable;
  UINTN                        MemoryAttributesTableSize;

  if ((mFeatureImplemented[TEST_POINT_INDEX_BYTE6_SMM] &
    TEST_POINT_BYTE6_SMM_READY_TO_LOCK_SECURE_SMM_COMMUNICATION_BUFFER) == 0) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "======== TestPointSmmReadyToLockSecureSmmCommunicationBuffer - Enter\n"));

  //
  // Collect information here, because it is last chance to access outside SMRAM.
  //
  TestPointDumpUefiMemoryMap (&mUefiMemoryMap, &mUefiMemoryMapSize, &mUefiDescriptorSize, TRUE);
  TestPointDumpGcd (&mGcdMemoryMap, &mGcdMemoryMapNumberOfDescriptors, &mGcdIoMap, &mGcdIoMapNumberOfDescriptors, TRUE);

  Status = EfiGetSystemConfigurationTable (&gEfiMemoryAttributesTableGuid, (VOID **)&MemoryAttributesTable);
  if (!EFI_ERROR (Status)) {
    MemoryAttributesTableSize = sizeof(EFI_MEMORY_ATTRIBUTES_TABLE) + MemoryAttributesTable->DescriptorSize * MemoryAttributesTable->NumberOfEntries;
    mUefiMemoryAttributesTable = AllocateCopyPool (MemoryAttributesTableSize, MemoryAttributesTable);
    ASSERT (mUefiMemoryAttributesTable != NULL);
  }
  //
  // Defer the validation to TestPointSmmReadyToBootSecureSmmCommunicationBuffer, because page table setup later.
  //

  DEBUG ((DEBUG_INFO, "======== TestPointSmmReadyToLockSecureSmmCommunicationBuffer - Exit\n"));
  return EFI_SUCCESS;
}

/**
  This service verifies the validity of the SMM page table at SMM Ready To Boot.

  Test subject: SMM page table.
  Test overview: Verify the SMM page table matches the SMM memory attribute table.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Reports an error message upon checking.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointSmmReadyToBootSmmPageProtection (
  VOID
  )
{
  EFI_STATUS  Status;
  BOOLEAN     Result;

  if ((mFeatureImplemented[TEST_POINT_INDEX_BYTE6_SMM]
    & TEST_POINT_BYTE6_SMM_READY_TO_BOOT_SMM_PAGE_LEVEL_PROTECTION) == 0) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "======== TestPointSmmReadyToBootSmmPageProtection - Enter\n"));

  Result = TRUE;

  Status = TestPointCheckSmmPaging ();
  if (EFI_ERROR(Status)) {
    Result = FALSE;
  }
  if (Result) {
    TestPointLibSetFeaturesVerified (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      TEST_POINT_INDEX_BYTE6_SMM,
      TEST_POINT_BYTE6_SMM_READY_TO_BOOT_SMM_PAGE_LEVEL_PROTECTION
      );
  }

  if (mUefiMemoryMap != NULL) {
    Result = TRUE;

    Status = TestPointCheckSmmCommunicationBuffer (mUefiMemoryMap, mUefiMemoryMapSize, mUefiDescriptorSize, mUefiMemoryAttributesTable);
    if (EFI_ERROR(Status)) {
      Result = FALSE;
    }
    if (Result) {
      TestPointLibSetFeaturesVerified (
        PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
        NULL,
        TEST_POINT_INDEX_BYTE6_SMM,
        TEST_POINT_BYTE6_SMM_READY_TO_LOCK_SECURE_SMM_COMMUNICATION_BUFFER
        );
    }
  }
  DEBUG ((DEBUG_INFO, "======== TestPointSmmReadyToBootSmmPageProtection - Exit\n"));
  return EFI_SUCCESS;
}

/**
  Dispatch function for a Software SMI handler.

  Caution: This function may receive untrusted input.
  Communicate buffer and buffer size are external input, so this function will do basic validation.

  @param CommBuffer      A pointer to a collection of data in memory that will
                         be conveyed from a non-SMM environment into an SMM environment.
  @param CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS Command is handled successfully.
**/
EFI_STATUS
TestPointSmmReadyToBootSmmPageProtectionHandler (
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS  Status;
  BOOLEAN     Result;
  TEST_POINT_SMM_COMMUNICATION_UEFI_GCD_MAP_INFO      *CommData;
  UINTN                                               TempCommBufferSize;

  if ((mFeatureImplemented[TEST_POINT_INDEX_BYTE6_SMM]
    & TEST_POINT_BYTE6_SMM_READY_TO_BOOT_SMM_PAGE_LEVEL_PROTECTION) == 0) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "======== TestPointSmmReadyToBootSmmPageProtectionHandler - Enter\n"));

  TempCommBufferSize = *CommBufferSize;

  if (TempCommBufferSize < sizeof(TEST_POINT_SMM_COMMUNICATION_UEFI_GCD_MAP_INFO)) {
    DEBUG((DEBUG_ERROR, "TestPointSmmReadyToBootSmmPageProtectionHandler: SMM communication buffer size invalid!\n"));
    return EFI_SUCCESS;
  }

  if (!SmmIsBufferOutsideSmmValid((UINTN)CommBuffer, TempCommBufferSize)) {
    DEBUG((DEBUG_ERROR, "TestPointSmmReadyToBootSmmPageProtectionHandler: SMM communication buffer in SMRAM or overflow!\n"));
    return EFI_SUCCESS;
  }
  DEBUG ((DEBUG_INFO, "TempCommBufferSize - 0x%x\n", TempCommBufferSize));
  CommData = AllocateCopyPool (TempCommBufferSize, CommBuffer);
  if (CommData == NULL) {
    DEBUG((DEBUG_ERROR, "TestPointSmmReadyToBootSmmPageProtectionHandler: SMM communication buffer size too big!\n"));
    return EFI_SUCCESS;
  }
  if (CommData->UefiMemoryMapOffset != sizeof(TEST_POINT_SMM_COMMUNICATION_UEFI_GCD_MAP_INFO)) {
    DEBUG((DEBUG_ERROR, "TestPointSmmReadyToBootSmmPageProtectionHandler: UefiMemoryMapOffset invalid!\n"));
    goto Done;
  }
  if (CommData->UefiMemoryMapSize >= TempCommBufferSize - CommData->UefiMemoryMapOffset) {
    DEBUG((DEBUG_ERROR, "TestPointSmmReadyToBootSmmPageProtectionHandler: UefiMemoryMapSize invalid!\n"));
    goto Done;
  }
  if (CommData->GcdMemoryMapOffset != CommData->UefiMemoryMapOffset + CommData->UefiMemoryMapSize) {
    DEBUG((DEBUG_ERROR, "TestPointSmmReadyToBootSmmPageProtectionHandler: GcdMemoryMapOffset invalid!\n"));
    goto Done;
  }
  if (CommData->GcdMemoryMapSize >= TempCommBufferSize - CommData->GcdMemoryMapOffset) {
    DEBUG((DEBUG_ERROR, "TestPointSmmReadyToBootSmmPageProtectionHandler: GcdMemoryMapSize invalid!\n"));
    goto Done;
  }
  if (CommData->GcdIoMapOffset != CommData->GcdMemoryMapOffset + CommData->GcdMemoryMapSize) {
    DEBUG((DEBUG_ERROR, "TestPointSmmReadyToBootSmmPageProtectionHandler: GcdIoMapOffset invalid!\n"));
    goto Done;
  }
  if (CommData->GcdIoMapSize >= TempCommBufferSize - CommData->GcdIoMapOffset) {
    DEBUG((DEBUG_ERROR, "TestPointSmmReadyToBootSmmPageProtectionHandler: GcdIoMapSize invalid!\n"));
    goto Done;
  }
  if (CommData->UefiMemoryAttributeTableOffset != CommData->GcdIoMapOffset + CommData->GcdIoMapSize) {
    DEBUG((DEBUG_ERROR, "TestPointSmmReadyToBootSmmPageProtectionHandler: UefiMemoryAttributeTableOffset invalid!\n"));
    goto Done;
  }
  if (CommData->UefiMemoryAttributeTableSize != TempCommBufferSize - CommData->UefiMemoryAttributeTableOffset) {
    DEBUG((DEBUG_ERROR, "TestPointSmmReadyToBootSmmPageProtectionHandler: UefiMemoryAttributeTableSize invalid!\n"));
    goto Done;
  }

  if (CommData->UefiMemoryMapSize != 0) {
    //
    // The SpeculationBarrier() call here is to ensure the previous range/content
    // checks for the CommBuffer (copied in to CommData) have been completed before
    // calling into TestPointCheckSmmCommunicationBuffer().
    //
    SpeculationBarrier ();
    Result = TRUE;

    Status = TestPointCheckSmmCommunicationBuffer (
               (EFI_MEMORY_DESCRIPTOR *)(UINTN)((UINTN)CommData + CommData->UefiMemoryMapOffset),
               (UINTN)CommData->UefiMemoryMapSize,
               mUefiDescriptorSize,
               (CommData->UefiMemoryAttributeTableSize != 0) ? (EFI_MEMORY_ATTRIBUTES_TABLE *)(UINTN)((UINTN)CommData + CommData->UefiMemoryAttributeTableOffset) : NULL
               );
    if (EFI_ERROR(Status)) {
      Result = FALSE;
    }
    if (Result) {
      TestPointLibSetFeaturesVerified (
        PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
        NULL,
        TEST_POINT_INDEX_BYTE6_SMM,
        TEST_POINT_BYTE6_SMM_READY_TO_LOCK_SECURE_SMM_COMMUNICATION_BUFFER
        );
    } else {
      TestPointLibClearFeaturesVerified (
        PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
        NULL,
        TEST_POINT_INDEX_BYTE6_SMM,
        TEST_POINT_BYTE6_SMM_READY_TO_LOCK_SECURE_SMM_COMMUNICATION_BUFFER
        );
    }
  }
Done:
  FreePool (CommData);

  DEBUG ((DEBUG_INFO, "======== TestPointSmmReadyToBootSmmPageProtectionHandler - Exit\n"));
  return EFI_SUCCESS;
}

/**
  Dispatch function for a Software SMI handler.

  Caution: This function may receive untrusted input.
  Communicate buffer and buffer size are external input, so this function will do basic validation.

  @param DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
  @param Context         Points to an optional handler context which was specified when the
                         handler was registered.
  @param CommBuffer      A pointer to a collection of data in memory that will
                         be conveyed from a non-SMM environment into an SMM environment.
  @param CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS Command is handled successfully.
**/
EFI_STATUS
EFIAPI
TestPointSmmHandler (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID  *Context         OPTIONAL,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{
  TEST_POINT_SMM_COMMUNICATION_HEADER      CommData;
  UINTN                                    TempCommBufferSize;

  //
  // If input is invalid, stop processing this SMI
  //
  if (CommBuffer == NULL || CommBufferSize == NULL) {
    return EFI_SUCCESS;
  }

  TempCommBufferSize = *CommBufferSize;

  if (TempCommBufferSize < sizeof(TEST_POINT_SMM_COMMUNICATION_HEADER)) {
    DEBUG((DEBUG_ERROR, "TestPointSmmHandler: SMM communication buffer size invalid!\n"));
    return EFI_SUCCESS;
  }
  CopyMem (&CommData, CommBuffer, sizeof(CommData));
  if (CommData.Version != TEST_POINT_SMM_COMMUNICATION_VERSION) {
    DEBUG((DEBUG_ERROR, "TestPointSmmHandler: SMM communication Version invalid!\n"));
    return EFI_SUCCESS;
  }
  switch (CommData.FuncId) {
  case TEST_POINT_SMM_COMMUNICATION_FUNC_ID_UEFI_GCD_MAP_INFO:
    return TestPointSmmReadyToBootSmmPageProtectionHandler (CommBuffer, CommBufferSize);
  }
  return EFI_SUCCESS;
}

/**
  This service verifies the system state within SMM after Exit Boot Services is invoked.

  @retval EFI_SUCCESS         The test point check was performed successfully.
**/
EFI_STATUS
EFIAPI
TestPointSmmExitBootServices (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "======== TestPointSmmExitBootServices - Enter\n"));

  DEBUG ((DEBUG_INFO, "======== TestPointSmmExitBootServices - Exit\n"));
  return EFI_SUCCESS;
}

/**
  Register SMM Test Point handler.
**/
VOID
RegisterSmmTestPointHandler (
  VOID
  )
{
  EFI_STATUS    Status;
  EFI_HANDLE    DispatchHandle;

  Status = gSmst->SmiHandlerRegister (
                    TestPointSmmHandler,
                    &mTestPointSmmCommunciationGuid,
                    &DispatchHandle
                    );
  ASSERT_EFI_ERROR (Status);
}

/**
  Initialize feature data.

  @param[in]  Role    The test point role being requested.
**/
VOID
InitData (
  IN UINT32                   Role
  )
{
  EFI_STATUS                             Status;

  ASSERT (PcdGetSize(PcdTestPointIbvPlatformFeature) == sizeof(mFeatureImplemented));
  CopyMem (mFeatureImplemented, PcdGetPtr(PcdTestPointIbvPlatformFeature), sizeof(mFeatureImplemented));

  mTestPointStruct.Role = Role;
  CopyMem (mTestPointStruct.FeaturesImplemented, mFeatureImplemented, sizeof(mFeatureImplemented));
  Status = TestPointLibSetTable (
             &mTestPointStruct,
             sizeof(mTestPointStruct)
             );
  if (EFI_ERROR (Status)) {
    if (Status != EFI_ALREADY_STARTED) {
      ASSERT_EFI_ERROR (Status);
    }
  }
}

/**
  The library constructuor.

  The function does the necessary initialization work for this library
  instance.

  @param[in]  ImageHandle       The firmware allocated handle for the UEFI image.
  @param[in]  SystemTable       A pointer to the EFI system table.

  @retval     EFI_SUCCESS       The function always return EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
SmmTestPointCheckLibConstructor (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  InitData (PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV);

  RegisterSmmTestPointHandler ();

  return EFI_SUCCESS;
}
