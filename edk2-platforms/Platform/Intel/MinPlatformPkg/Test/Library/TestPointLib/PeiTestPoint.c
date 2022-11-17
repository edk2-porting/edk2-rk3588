/** @file

  Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "PeiTestPoint.h"

UINTN
InternalGetTestPointInfoSize (
  IN ADAPTER_INFO_PLATFORM_TEST_POINT  *TestPoint,
  IN UINTN                             MaxSize
  )
{
  CHAR16  *ErrorString;
  UINTN   ErrorStringLength;
  UINTN   ErrorStringMaxSize;
  CHAR16  ErrorChar;

  ErrorString = (CHAR16 *)((UINTN)TestPoint + sizeof(ADAPTER_INFO_PLATFORM_TEST_POINT) + TEST_POINT_FEATURES_ITEM_NUMBER * TestPoint->FeaturesSize);

  ErrorStringMaxSize = MaxSize - sizeof(ADAPTER_INFO_PLATFORM_TEST_POINT) - TestPoint->FeaturesSize * TEST_POINT_FEATURES_ITEM_NUMBER;
  //
  // ErrorString might not be CHAR16 aligned.
  //
  CopyMem (&ErrorChar, ErrorString, sizeof(ErrorChar));
  for (ErrorStringLength = 0; (ErrorChar != 0) && (ErrorStringLength < (ErrorStringMaxSize/2)); ErrorStringLength++) {
    ErrorString++;
    CopyMem (&ErrorChar, ErrorString, sizeof(ErrorChar));
  }

  return sizeof(ADAPTER_INFO_PLATFORM_TEST_POINT) + TEST_POINT_FEATURES_ITEM_NUMBER * TestPoint->FeaturesSize + (ErrorStringLength + 1) * sizeof(CHAR16);
}

/**
  Find TestPoint table in AIP protocol, and return the data.
  This API will return the TestPoint table with indicated Role and ImplementationID,
  NULL ImplementationID means to find the first TestPoint table with indicated Role.

  @param Role             Role of TestPoint data.
  @param ImplementationID ImplementationID of TestPoint data.
                          NULL means find the first one match Role.
  @param TestPointData    TestPoint data. This buffer is allocated by callee, and it
                          is the responsibility of the caller to free it after
                          using it.
  @param TestPointSize    TestPoint size

  @return Status
**/
EFI_STATUS
InternalTestPointFindAip (
  IN UINT32                   Role,
  IN CHAR16                   *ImplementationID OPTIONAL,
  OUT VOID                    **TestPointData OPTIONAL,
  OUT UINTN                   *TestPointSize OPTIONAL,
  OUT UINTN                   *TestPointMaxSize OPTIONAL
  )
{
  ADAPTER_INFO_PLATFORM_TEST_POINT  *TestPoint;
  UINTN                             Size;
  UINTN                             MaxSize;
  EFI_PEI_HOB_POINTERS              Hob;

  Hob.Raw = GetHobList ();
  while (TRUE) {
    Hob.Raw = GetNextGuidHob (&gAdapterInfoPlatformTestPointGuid, Hob.Raw);
    if (Hob.Raw == NULL) {
      return EFI_NOT_FOUND;
    }
    TestPoint = GET_GUID_HOB_DATA (Hob);
    MaxSize = GET_GUID_HOB_DATA_SIZE (Hob);
    Size = InternalGetTestPointInfoSize (TestPoint, MaxSize);

    if ((TestPoint->Role == Role) && 
        ((ImplementationID == NULL) || (StrCmp (ImplementationID, TestPoint->ImplementationID) == 0))) {
      break;
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
    if (Hob.Raw == NULL) {
      return EFI_NOT_FOUND;
    }
  }

  if (TestPointData != NULL) {
    *TestPointData = TestPoint;
  }
  if (TestPointSize != NULL) {
    *TestPointSize = Size;
  }
  if (TestPointMaxSize != NULL) {
    *TestPointMaxSize = MaxSize;
  }
  return EFI_SUCCESS;
}

/**
  Return if input TestPoint data is valid.

  @param TestPointData  TestPoint data
  @param TestPointSize  TestPoint size

  @retval TRUE  TestPoint data is valid.
  @retval FALSE TestPoint data is not valid.
**/
BOOLEAN
InternalTestPointIsValidTable (
  IN VOID                     *TestPointData,
  IN UINTN                    TestPointSize
  )
{
  ADAPTER_INFO_PLATFORM_TEST_POINT *TestPoint;
  UINTN                            Index;
  CHAR16                           *ErrorString;
  CHAR16                           ErrorChar;
  UINTN                            ErrorStringSize;
  UINTN                            ErrorStringLength;

  TestPoint = TestPointData;

  //
  // basic check for header
  //
  if (TestPointData == NULL) {
    DEBUG ((EFI_D_ERROR, "TestPointData == NULL\n"));
    return FALSE;
  }
  if (TestPointSize < sizeof(ADAPTER_INFO_PLATFORM_TEST_POINT)) {
    DEBUG ((EFI_D_ERROR, "TestPointSize < sizeof(ADAPTER_INFO_PLATFORM_TEST_POINT)\n"));
    return FALSE;
  }
  if (((TestPointSize - sizeof(ADAPTER_INFO_PLATFORM_TEST_POINT)) / TEST_POINT_FEATURES_ITEM_NUMBER) < TestPoint->FeaturesSize) {
    DEBUG ((EFI_D_ERROR, "((TestPointSize - sizeof(ADAPTER_INFO_PLATFORM_TEST_POINT)) / TEST_POINT_FEATURES_ITEM_NUMBER) < FeaturesSize\n"));
    return FALSE;
  }

  //
  // Check Version
  //
  if (TestPoint->Version != PLATFORM_TEST_POINT_VERSION) {
    DEBUG ((EFI_D_ERROR, "Version != PLATFORM_TEST_POINT_VERSION\n"));
    return FALSE;
  }

  //
  // Check Role
  //
  if ((TestPoint->Role < PLATFORM_TEST_POINT_ROLE_PLATFORM_REFERENCE) ||
      (TestPoint->Role > PLATFORM_TEST_POINT_ROLE_IMPLEMENTOR_ODM)) {
    DEBUG ((EFI_D_ERROR, "Role < PLATFORM_TEST_POINT_ROLE_PLATFORM_REFERENCE ||\n"));
    DEBUG ((EFI_D_ERROR, "Role > PLATFORM_TEST_POINT_ROLE_IMPLEMENTOR_ODM\n"));
    return FALSE;
  }

  //
  // Check ImplementationID
  //
  for (Index = 0; Index < sizeof(TestPoint->ImplementationID)/sizeof(TestPoint->ImplementationID[0]); Index++) {
    if (TestPoint->ImplementationID[Index] == 0) {
      break;
    }
  }
  if (Index == sizeof(TestPoint->ImplementationID)/sizeof(TestPoint->ImplementationID[0])) {
    DEBUG ((EFI_D_ERROR, "ImplementationID is no NUL CHAR\n"));
    return FALSE;
  }

  ErrorStringSize = TestPointSize - sizeof(ADAPTER_INFO_PLATFORM_TEST_POINT) - TestPoint->FeaturesSize * TEST_POINT_FEATURES_ITEM_NUMBER;
  ErrorString = (CHAR16 *)((UINTN)TestPoint + sizeof(ADAPTER_INFO_PLATFORM_TEST_POINT) - TestPoint->FeaturesSize * TEST_POINT_FEATURES_ITEM_NUMBER);

  //
  // basic check for ErrorString
  //
  if (ErrorStringSize == 0) {
    DEBUG ((EFI_D_ERROR, "ErrorStringSize == 0\n"));
    return FALSE;
  }
  if ((ErrorStringSize & BIT0) != 0) {
    DEBUG ((EFI_D_ERROR, "(ErrorStringSize & BIT0) != 0\n"));
    return FALSE;
  }

  //
  // ErrorString might not be CHAR16 aligned.
  //
  CopyMem (&ErrorChar, ErrorString, sizeof(ErrorChar));
  for (ErrorStringLength = 0; (ErrorChar != 0) && (ErrorStringLength < (ErrorStringSize/2)); ErrorStringLength++) {
    ErrorString++;
    CopyMem (&ErrorChar, ErrorString, sizeof(ErrorChar));
  }

  //
  // check the length of ErrorString
  //
  if (ErrorChar != 0) {
    DEBUG ((EFI_D_ERROR, "ErrorString has no NUL CHAR\n"));
    return FALSE;
  }
  if (ErrorStringLength == (ErrorStringSize/2)) {
    DEBUG ((EFI_D_ERROR, "ErrorString Length incorrect\n"));
    return FALSE;
  }

  return TRUE;
}

/**
  Publish TestPoint table in hob.

  One system should have only one PLATFORM_TEST_POINT_ROLE_PLATFORM_REFERENCE.

  @param TestPoint      TestPoint data
  @param TestPointSize  TestPoint size

  @retval EFI_SUCCESS          The TestPoint data is published in hob.
  @retval EFI_ALREADY_STARTED  There is already TestPoint table with Role and ImplementationID published in system.
  @retval EFI_VOLUME_CORRUPTED The input TestPoint data is invalid.
  @retval EFI_OUT_OF_RESOURCES There is not enough system resource to publish TestPoint data in hob.
**/
EFI_STATUS
EFIAPI
TestPointLibSetTable (
  IN VOID                     *TestPoint,
  IN UINTN                    TestPointSize
  )
{
  VOID                             *NewTestPoint;
  EFI_STATUS                       Status;
  UINT32                           Role;
  CHAR16                           *ImplementationID;

  DEBUG ((EFI_D_ERROR, "TestPointLibSetTable\n"));

  if (!InternalTestPointIsValidTable (TestPoint, TestPointSize)) {
    DEBUG ((EFI_D_ERROR, "InternalTestPointIsValidTable\n"));
    return EFI_VOLUME_CORRUPTED;
  }

  Role = ((ADAPTER_INFO_PLATFORM_TEST_POINT *)TestPoint)->Role;
  ImplementationID = ((ADAPTER_INFO_PLATFORM_TEST_POINT *)TestPoint)->ImplementationID;
  Status = InternalTestPointFindAip (Role, ImplementationID, NULL, NULL, NULL);
  if (!EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "Aip (0x%x, %S) is found\n", Role, ImplementationID));
    return EFI_ALREADY_STARTED;
  }

  NewTestPoint = BuildGuidHob (&gAdapterInfoPlatformTestPointGuid, TestPointSize);
  if (NewTestPoint == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (NewTestPoint, TestPoint, TestPointSize);

  return EFI_SUCCESS;
}

/**
  Search TestPoint table in hob, and return the data.
  This API will return the TestPoint table with indicated Role and ImplementationID,
  NULL ImplementationID means to find the first TestPoint table with indicated Role.

  @param Role             Role of TestPoint data.
  @param ImplementationID ImplementationID of TestPoint data.
                          NULL means find the first one match Role.
  @param TestPoint        TestPoint data.
  @param TestPointSize    TestPoint size

  @retval EFI_SUCCESS          The TestPoint data in hob is returned.
  @retval EFI_NOT_FOUND        There is not TestPoint table with the Role and ImplementationID published in system.
**/
EFI_STATUS
EFIAPI
TestPointLibGetTable (
  IN UINT32                   Role,
  IN CHAR16                   *ImplementationID OPTIONAL,
  OUT VOID                    **TestPoint,
  OUT UINTN                   *TestPointSize
  )
{
  EFI_STATUS  Status;

  Status = InternalTestPointFindAip (Role, ImplementationID, TestPoint, TestPointSize, NULL);
  return Status;
}

/**
  Record FeaturesVerified in published TestPoint table.
  This API will update the TestPoint table with indicated Role and ImplementationID,
  NULL ImplementationID means to find the first TestPoint table with indicated Role.

  @param Role             Role of TestPoint data.
  @param ImplementationID ImplementationID of TestPoint data.
                          NULL means find the first one match Role.
  @param ByteIndex        Byte index of FeaturesVerified of TestPoint data.
  @param BitMask          Bit mask of FeaturesVerified of TestPoint data.
  @param Set              TRUE means to set the FeaturesVerified bit.
                          FALSE means to clear the FeaturesVerified bit.

  @retval EFI_SUCCESS          The FeaturesVerified of TestPoint data updated in AIP protocol.
  @retval EFI_NOT_STARTED      There is not TestPoint table with the Role and ImplementationID published in system.
  @retval EFI_UNSUPPORTED      The ByteIndex is invalid.
**/
EFI_STATUS
InternalTestPointRecordFeaturesVerified (
  IN UINT32                   Role,
  IN CHAR16                   *ImplementationID, OPTIONAL
  IN UINT32                   ByteIndex,
  IN UINT8                    Bit,
  IN BOOLEAN                  Set
  )
{
  ADAPTER_INFO_PLATFORM_TEST_POINT *TestPoint;
  UINTN                            TestPointSize;
  UINT8                            *FeaturesVerified;
  EFI_STATUS                       Status;

  Status = InternalTestPointFindAip (Role, ImplementationID, (VOID **)&TestPoint, &TestPointSize, NULL);
  if (EFI_ERROR(Status)) {
    return EFI_NOT_STARTED;
  }

  if (ByteIndex >= TestPoint->FeaturesSize) {
    return EFI_UNSUPPORTED;
  }

  FeaturesVerified = (UINT8 *)((UINTN)TestPoint + sizeof(ADAPTER_INFO_PLATFORM_TEST_POINT) + TestPoint->FeaturesSize * 1);

  if (Set) {
    FeaturesVerified[ByteIndex] = (UINT8)(FeaturesVerified[ByteIndex] | (Bit));
  } else {
    FeaturesVerified[ByteIndex] = (UINT8)(FeaturesVerified[ByteIndex] & (~Bit));
  }

  return EFI_SUCCESS;
}

/**
  Set FeaturesVerified in published TestPoint table.
  This API will update the TestPoint table with indicated Role and ImplementationID,
  NULL ImplementationID means to find the first TestPoint table with indicated Role.

  @param Role             Role of TestPoint data.
  @param ImplementationID ImplementationID of TestPoint data.
                          NULL means find the first one match Role.
  @param ByteIndex        Byte index of FeaturesVerified of TestPoint data.
  @param BitMask          Bit mask of FeaturesVerified of TestPoint data.

  @retval EFI_SUCCESS          The FeaturesVerified of TestPoint data updated in hob.
  @retval EFI_NOT_STARTED      There is not TestPoint table with the Role and ImplementationID published in system.
  @retval EFI_UNSUPPORTED      The ByteIndex is invalid.
**/
EFI_STATUS
EFIAPI
TestPointLibSetFeaturesVerified (
  IN UINT32                   Role,
  IN CHAR16                   *ImplementationID, OPTIONAL
  IN UINT32                   ByteIndex,
  IN UINT8                    BitMask
  )
{
  DEBUG ((DEBUG_INFO, "TestPointLibSetFeaturesVerified - Index:0x%x Mask:0x%02x\n", ByteIndex, BitMask));
  return InternalTestPointRecordFeaturesVerified (
           Role,
           ImplementationID,
           ByteIndex,
           BitMask,
           TRUE
           );
}

/**
  Clear FeaturesVerified in published TestPoint table.
  This API will update the TestPoint table with indicated Role and ImplementationID,
  NULL ImplementationID means to find the first TestPoint table with indicated Role.

  @param Role             Role of TestPoint data.
  @param ImplementationID ImplementationID of TestPoint data.
                          NULL means find the first one match Role.
  @param ByteIndex        Byte index of FeaturesVerified of TestPoint data.
  @param BitMask          Bit mask of FeaturesVerified of TestPoint data.

  @retval EFI_SUCCESS          The FeaturesVerified of TestPoint data updated in hob.
  @retval EFI_NOT_STARTED      There is not TestPoint table with the Role and ImplementationID published in system.
  @retval EFI_UNSUPPORTED      The ByteIndex is invalid.
**/
EFI_STATUS
EFIAPI
TestPointLibClearFeaturesVerified (
  IN UINT32                   Role,
  IN CHAR16                   *ImplementationID, OPTIONAL
  IN UINT32                   ByteIndex,
  IN UINT8                    BitMask
  )
{
  DEBUG ((DEBUG_INFO, "TestPointLibClearFeaturesVerified - Index:0x%x Mask:0x%02x\n", ByteIndex, BitMask));
  return InternalTestPointRecordFeaturesVerified (
           Role,
           ImplementationID,
           ByteIndex,
           BitMask,
           FALSE
           );
}

/**
  Record ErrorString in published TestPoint table.
  This API will update the TestPoint table with indicated Role and ImplementationID,
  NULL ImplementationID means to find the first TestPoint table with indicated Role.

  @param Role             Role of TestPoint data.
  @param ImplementationID ImplementationID of TestPoint data.
                          NULL means find the first one match Role.
  @param ErrorString      ErrorString of TestPoint data.
  @param Append           TRUE means to append the ErrorString to TestPoint table.
                          FALSE means to set the ErrorString in TestPoint table.

  @retval EFI_SUCCESS          The ErrorString of TestPoint data is published in hob.
  @retval EFI_NOT_STARTED      There is not TestPoint table with the Role and ImplementationID published in system.
  @retval EFI_OUT_OF_RESOURCES There is not enough system resource to update ErrorString.
**/
EFI_STATUS
InternalTestPointRecordErrorString (
  IN UINT32                   Role,
  IN CHAR16                   *ImplementationID, OPTIONAL
  IN CHAR16                   *ErrorString,
  IN BOOLEAN                  Append
  )
{
  ADAPTER_INFO_PLATFORM_TEST_POINT *TestPoint;
  UINTN                            TestPointSize;
  UINTN                            StringSize;
  VOID                             *NewTestPoint;
  UINTN                            NewTestPointSize;
  UINTN                            Offset;
  EFI_STATUS                       Status;
  UINTN                            TestPointMaxSize;
  EFI_HOB_GUID_TYPE                *GuidHob;

  Status = InternalTestPointFindAip (Role, ImplementationID, (VOID **)&TestPoint, &TestPointSize, &TestPointMaxSize);
  if (EFI_ERROR(Status)) {
    return EFI_NOT_STARTED;
  }

  if (Append) {
    Offset = TestPointSize - sizeof(CHAR16);
  } else {
    Offset = sizeof(ADAPTER_INFO_PLATFORM_TEST_POINT) + TestPoint->FeaturesSize * TEST_POINT_FEATURES_ITEM_NUMBER;
  }
  StringSize = StrSize (ErrorString);

  NewTestPointSize = Offset + StringSize;
  if (NewTestPointSize > TestPointMaxSize) {
    NewTestPoint = BuildGuidHob (&gAdapterInfoPlatformTestPointGuid, NewTestPointSize);
    if (NewTestPoint == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    CopyMem (NewTestPoint, TestPoint, TestPointSize);

    // remove old data
    GuidHob = (VOID *)((UINT8 *)TestPoint - sizeof (EFI_HOB_GUID_TYPE));
    ZeroMem (&GuidHob->Name, sizeof(GuidHob->Name));

    TestPoint = NewTestPoint;
  }

  CopyMem ((UINT8 *)TestPoint + Offset, ErrorString, StringSize);

  return Status;
}

/**
  Append ErrorString in published TestPoint table.
  This API will update the TestPoint table with indicated Role and ImplementationID,
  NULL ImplementationID means to find the first TestPoint table with indicated Role.

  @param Role             Role of TestPoint data.
  @param ImplementationID ImplementationID of TestPoint data.
                          NULL means find the first one match Role.
  @param ErrorString      ErrorString of TestPoint data.

  @retval EFI_SUCCESS          The ErrorString of TestPoint data is updated in hob.
  @retval EFI_NOT_STARTED      There is not TestPoint table with the Role and ImplementationID published in system.
  @retval EFI_OUT_OF_RESOURCES There is not enough system resource to update ErrorString.
**/
EFI_STATUS
EFIAPI
TestPointLibAppendErrorString (
  IN UINT32                   Role,
  IN CHAR16                   *ImplementationID, OPTIONAL
  IN CHAR16                   *ErrorString
  )
{
  DEBUG ((DEBUG_INFO, "TestPointLibAppendErrorString - (0x%x) %s\n", Role, ErrorString));
  return InternalTestPointRecordErrorString (
           Role,
           ImplementationID,
           ErrorString,
           TRUE
           );
}

/**
  Set a new ErrorString in published TestPoint table.
  This API will update the TestPoint table with indicated Role and ImplementationID,
  NULL ImplementationID means to find the first TestPoint table with indicated Role.

  @param Role             Role of TestPoint data.
  @param ImplementationID ImplementationID of TestPoint data.
                          NULL means find the first one match Role.
  @param ErrorString      ErrorString of TestPoint data.

  @retval EFI_SUCCESS          The ErrorString of TestPoint data is updated in hob.
  @retval EFI_NOT_STARTED      There is not TestPoint table with the Role and ImplementationID published in system.
  @retval EFI_OUT_OF_RESOURCES There is not enough system resource to update ErrorString.
**/
EFI_STATUS
EFIAPI
TestPointLibSetErrorString (
  IN UINT32                   Role,
  IN CHAR16                   *ImplementationID, OPTIONAL
  IN CHAR16                   *ErrorString
  )
{
  DEBUG ((DEBUG_INFO, "TestPointLibSetErrorString - %s\n", ErrorString));
  return InternalTestPointRecordErrorString (
           Role,
           ImplementationID,
           ErrorString,
           FALSE
           );
}
