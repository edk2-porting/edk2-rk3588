/** @file

Copyright (c) 2017 - 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiPei.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>

EFI_STATUS
TestPointCheckMtrr (
  IN BOOLEAN   IsForDxe
  );

EFI_STATUS
TestPointDumpHob (
  IN BOOLEAN  PhitHobOnly
  );

EFI_STATUS
TestPointCheckFvInfo (
  VOID
  );

EFI_STATUS
TestPointCheckMemoryResource (
  VOID
  );

EFI_STATUS
TestPointCheckSmramHob (
  VOID
  );

EFI_STATUS
TestPointCheckSmmInfoPei (
  VOID
  );

EFI_STATUS
TestPointCheckPciBusMaster (
  VOID
  );

EFI_STATUS
TestPointVtdEngine (
  VOID
  );

GLOBAL_REMOVE_IF_UNREFERENCED ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT  mTestPointStruct = {
  PLATFORM_TEST_POINT_VERSION,
  PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
  {TEST_POINT_IMPLEMENTATION_ID_PLATFORM_PEI},
  TEST_POINT_FEATURE_SIZE,
  {0}, // FeaturesImplemented
  {0}, // FeaturesVerified
  0,
};

UINT8 *
GetFeatureImplemented (
  VOID
  )
{
  EFI_STATUS  Status;
  VOID        *TestPoint;
  UINTN       TestPointSize;

  Status = TestPointLibGetTable (
             PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
             TEST_POINT_IMPLEMENTATION_ID_PLATFORM_PEI,
             &TestPoint,
             &TestPointSize
             );
  ASSERT_EFI_ERROR(Status);

  return (UINT8 *)TestPoint + sizeof(ADAPTER_INFO_PLATFORM_TEST_POINT);
}

/**
  This service tests debug configuration after debug device initialization.

  Test subject: Debug Capability
  Test overview: Dumps a struct of debug configuration parameters to the debug log.
  Reporting mechanism: Serial port shows the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointDebugInitDone (
  VOID
  )
{
  UINT8       *FeatureImplemented;

  FeatureImplemented = GetFeatureImplemented ();

  //
  // If code can run here, always set TEMP INIT DONE here.
  //
  TestPointLibSetFeaturesVerified (
    PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
    NULL,
    0,
    TEST_POINT_BYTE0_TEMP_INIT_DONE
    );

  if ((FeatureImplemented[0] & TEST_POINT_BYTE0_DEBUG_INIT_DONE) == 0) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "======== TestPointDebugInitDone - Enter\n"));
  DEBUG ((DEBUG_INFO, "!!! DebugInitialized !!!\n"));

  TestPointLibSetFeaturesVerified (
    PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
    NULL,
    0,
    TEST_POINT_BYTE0_DEBUG_INIT_DONE
    );
  DEBUG ((DEBUG_INFO, "======== TestPointDebugInitDone - Exit\n"));
  return EFI_SUCCESS;
}

/**
  This service verifies MTRR settings after memory is discovered.

  Test subject: MTRRs after memory is discovered.
  Test overview: Verifies MTRR settings.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps results to the debug log.

  Examples of settings verified: No MTRR overlap, PEI data memory is writeback, and flash region is WP, MMIO is UC, etc.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointMemoryDiscoveredMtrrFunctional (
  VOID
  )
{
  EFI_STATUS  Status;
  BOOLEAN     Result;
  UINT8       *FeatureImplemented;

  FeatureImplemented = GetFeatureImplemented ();

  if ((FeatureImplemented[1] & TEST_POINT_BYTE1_MEMORY_DISCOVERED_MTRR_FUNCTIONAL) == 0) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "======== TestPointMemoryDiscoveredMtrrFunctional - Enter\n"));

  TestPointDumpHob (FALSE);

  Result = TRUE;
  Status = TestPointCheckMtrr (FALSE);
  if (EFI_ERROR(Status)) {
    Result = FALSE;
  }

  if (Result) {
    TestPointLibSetFeaturesVerified (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      1,
      TEST_POINT_BYTE1_MEMORY_DISCOVERED_MTRR_FUNCTIONAL
      );
  }

  DEBUG ((DEBUG_INFO, "======== TestPointMemoryDiscoveredMtrrFunctional - Exit\n"));
  return EFI_SUCCESS;
}

/**
  This service verifies discovered memory resources after memory is discovered.

  Test subject: Resource description HOBs.
  Test overview: No memory resource overlap.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps results to the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointMemoryDiscoveredMemoryResourceFunctional (
  VOID
  )
{
  EFI_STATUS  Status;
  BOOLEAN     Result;
  UINT8       *FeatureImplemented;

  FeatureImplemented = GetFeatureImplemented ();

  if ((FeatureImplemented[1] & TEST_POINT_BYTE1_MEMORY_DISCOVERED_MEMORY_RESOURCE_FUNCTIONAL) == 0) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "======== TestPointMemoryDiscoveredMemoryResourceFunctional - Enter\n"));
  Result = TRUE;

  Status = TestPointCheckMemoryResource ();
  if (EFI_ERROR(Status)) {
    Result = FALSE;
  }

  if (Result) {
    TestPointLibSetFeaturesVerified (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      1,
      TEST_POINT_BYTE1_MEMORY_DISCOVERED_MEMORY_RESOURCE_FUNCTIONAL
      );
  }

  DEBUG ((DEBUG_INFO, "======== TestPointMemoryDiscoveredMemoryResourceFunctional - Exit\n"));
  return EFI_SUCCESS;
}

/**
  This service verifies the validity of published firmware volume resources.

  Test subject: FV HOB and FV Info PPI.
  Test overview: FV HOB and FV Info PPI.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps results to the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointMemoryDiscoveredFvInfoFunctional (
  VOID
  )
{
  EFI_STATUS  Status;
  BOOLEAN     Result;
  UINT8       *FeatureImplemented;

  FeatureImplemented = GetFeatureImplemented ();

  if ((FeatureImplemented[1] & TEST_POINT_BYTE1_MEMORY_DISCOVERED_FV_INFO_FUNCTIONAL) == 0) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "======== TestPointMemoryDiscoveredFvInfoFunctional - Enter\n"));
  Result = TRUE;
  Status = TestPointCheckFvInfo ();
  if (EFI_ERROR(Status)) {
    Result = FALSE;
  }

  if (Result) {
    TestPointLibSetFeaturesVerified (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      1,
      TEST_POINT_BYTE1_MEMORY_DISCOVERED_FV_INFO_FUNCTIONAL
      );
  }

  DEBUG ((DEBUG_INFO, "======== TestPointMemoryDiscoveredFvInfoFunctional - Exit\n"));
  return EFI_SUCCESS;
}

/**
  This service verifies DMA protection configuration in PEI after memory is discovered.

  Test subject: DMA protection.
  Test overview: DMA protection in PEI.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointMemoryDiscoveredDmaProtectionEnabled (
  VOID
  )
{
  EFI_STATUS  Status;
  BOOLEAN     Result;
  UINT8       *FeatureImplemented;

  FeatureImplemented = GetFeatureImplemented ();

  if ((FeatureImplemented[1] & TEST_POINT_BYTE1_MEMORY_DISCOVERED_DMA_PROTECTION_ENABLED) == 0) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "======== TestPointMemoryDiscoveredDmaProtectionEnabled - Enter\n"));

  Result = TRUE;
  Status = TestPointVtdEngine ();
  if (EFI_ERROR(Status)) {
    Result = FALSE;
  }

  if (Result) {
    TestPointLibSetFeaturesVerified (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      1,
      TEST_POINT_BYTE1_MEMORY_DISCOVERED_DMA_PROTECTION_ENABLED
      );
  }

  DEBUG ((DEBUG_INFO, "======== TestPointMemoryDiscoveredDmaProtectionEnabled - Exit\n"));
  return EFI_SUCCESS;
}

/**
  This service verifies system resources at the end of PEI.

  Test subject: Resource HOB and SMRAM HOB.
  Test overview: SMRAM configuration and no system resource overlap.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps results to the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointEndOfPeiSystemResourceFunctional (
  VOID
  )
{
  EFI_STATUS  Status;
  BOOLEAN     Result;
  UINT8       *FeatureImplemented;

  FeatureImplemented = GetFeatureImplemented ();

  if ((FeatureImplemented[2] & TEST_POINT_BYTE2_END_OF_PEI_SYSTEM_RESOURCE_FUNCTIONAL) == 0) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "======== TestPointEndOfPeiSystemResourceFunctional - Enter\n"));

  TestPointDumpHob (FALSE);

  Result = TRUE;
  Status = TestPointCheckSmmInfoPei ();
  if (EFI_ERROR(Status)) {
    Result = FALSE;
  }

  if (Result) {
    TestPointLibSetFeaturesVerified (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      2,
      TEST_POINT_BYTE2_END_OF_PEI_SYSTEM_RESOURCE_FUNCTIONAL
      );
  }

  DEBUG ((DEBUG_INFO, "======== TestPointEndOfPeiSystemResourceFunctional - Exit\n"));
  return EFI_SUCCESS;
}

/**
  This service verifies MTRR settings at the end of PEI.

  Test subject: MTRRs after end of PEI.
  Test overview: Verifies MTRR settings.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps results to the debug log.

  Examples of settings verified: No MTRR overlap, DXE data memory is writeback, flash region may be UC, MMIO is UC, etc.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointEndOfPeiMtrrFunctional (
  VOID
  )
{
  EFI_STATUS  Status;
  BOOLEAN     Result;
  UINT8       *FeatureImplemented;

  FeatureImplemented = GetFeatureImplemented ();

  if ((FeatureImplemented[2] & TEST_POINT_BYTE2_END_OF_PEI_MTRR_FUNCTIONAL) == 0) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "======== TestPointEndOfPeiMtrrFunctional - Enter\n"));
  Result = TRUE;
  Status = TestPointCheckMtrr (TRUE);
  if (EFI_ERROR(Status)) {
    Result = FALSE;
  }

  if (Result) {
    TestPointLibSetFeaturesVerified (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      2,
      TEST_POINT_BYTE2_END_OF_PEI_MTRR_FUNCTIONAL
      );
  }

  DEBUG ((DEBUG_INFO, "======== TestPointEndOfPeiMtrrFunctional - Exit\n"));
  return EFI_SUCCESS;
}

/**
  This service verifies bus master enable (BME) is disabled at the end of PEI.

  Test subject: PCI device BME.
  Test overview: Verify BME is cleared.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps results to the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointEndOfPeiPciBusMasterDisabled (
  VOID
  )
{
  EFI_STATUS  Status;
  BOOLEAN     Result;
  UINT8       *FeatureImplemented;

  FeatureImplemented = GetFeatureImplemented ();

  if ((FeatureImplemented[2] & TEST_POINT_BYTE2_END_OF_PEI_PCI_BUS_MASTER_DISABLED) == 0) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "======== TestPointEndOfPeiPciBusMasterDisabled - Enter\n"));
  Result = TRUE;
  Status = TestPointCheckPciBusMaster ();
  if (EFI_ERROR(Status)) {
    Result = FALSE;
  }

  if (Result) {
    TestPointLibSetFeaturesVerified (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      2,
      TEST_POINT_BYTE2_END_OF_PEI_PCI_BUS_MASTER_DISABLED
      );
  }

  DEBUG ((DEBUG_INFO, "======== TestPointEndOfPeiPciBusMasterDisabled - Exit\n"));
  return EFI_SUCCESS;
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
  EFI_STATUS                               Status;
  UINT8                                    FeatureImplemented[TEST_POINT_FEATURE_SIZE];
  ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT  TestPointStruct;

  ASSERT (PcdGetSize(PcdTestPointIbvPlatformFeature) == sizeof(FeatureImplemented));
  CopyMem (FeatureImplemented, PcdGetPtr(PcdTestPointIbvPlatformFeature), sizeof(FeatureImplemented));

  CopyMem (&TestPointStruct, &mTestPointStruct, sizeof(TestPointStruct));
  TestPointStruct.Role = Role;
  CopyMem (TestPointStruct.FeaturesImplemented, FeatureImplemented, sizeof(FeatureImplemented));
  Status = TestPointLibSetTable (
             &TestPointStruct,
             sizeof(TestPointStruct)
             );
  if (EFI_ERROR (Status)) {
    if (Status != EFI_ALREADY_STARTED) {
      ASSERT_EFI_ERROR (Status);
    }
  }
}

/**
  The library constructor.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The function always return EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
PeiTestPointCheckLibConstructor (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  InitData (PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV);

  return EFI_SUCCESS;
}
