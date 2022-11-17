/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _TEST_POINT_LIB_H_
#define _TEST_POINT_LIB_H_

#include <PiPei.h>
#include <Uefi.h>

//
// Below is Test Point report definition.
//

//
// We reuse HSTI stype definition.
// ADAPTER_INFO_PLATFORM_TEST_POINT is similar to ADAPTER_INFO_PLATFORM_SECURITY.
//

#define PLATFORM_TEST_POINT_VERSION               0x00000001

#define PLATFORM_TEST_POINT_ROLE_PLATFORM_REFERENCE 0x00000001
#define PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV       0x00000002
#define PLATFORM_TEST_POINT_ROLE_IMPLEMENTOR_OEM    0x00000003 
#define PLATFORM_TEST_POINT_ROLE_IMPLEMENTOR_ODM    0x00000004  

#define TEST_POINT_FEATURES_ITEM_NUMBER 2

typedef struct {
  UINT32  Version;
  UINT32  Role;
  CHAR16  ImplementationID[256];
  UINT32  FeaturesSize;
//UINT8   FeaturesImplemented[];
//UINT8   FeaturesVerified[];
//CHAR16  ErrorString[];
} ADAPTER_INFO_PLATFORM_TEST_POINT;

//
// Below is test point report library
//

/**
  Publish TestPoint table in AIP protocol.

  One system should have only one PLATFORM_TEST_POINT_ROLE_PLATFORM_REFERENCE.

  @param TestPoint      TestPoint data
  @param TestPointSize  TestPoint size

  @retval EFI_SUCCESS          The TestPoint data is published in AIP protocol.
  @retval EFI_ALREADY_STARTED  There is already TestPoint table with Role and ImplementationID published in system.
  @retval EFI_VOLUME_CORRUPTED The input TestPoint data is invalid.
  @retval EFI_OUT_OF_RESOURCES There is not enough system resource to publish TestPoint data in AIP protocol.
**/
EFI_STATUS
EFIAPI
TestPointLibSetTable (
  IN VOID                     *TestPoint,
  IN UINTN                    TestPointSize
  );

/**
  Search TestPoint table in AIP protocol, and return the data.
  This API will return the TestPoint table with indicated Role and ImplementationID,
  NULL ImplementationID means to find the first TestPoint table with indicated Role.

  @param Role             Role of TestPoint data.
  @param ImplementationID ImplementationID of TestPoint data.
                          NULL means find the first one match Role.
  @param TestPoint        TestPoint data. This buffer is allocated by callee, and it
                          is the responsibility of the caller to free it after
                          using it.
  @param TestPointSize    TestPoint size

  @retval EFI_SUCCESS          The TestPoint data in AIP protocol is returned.
  @retval EFI_NOT_FOUND        There is not TestPoint table with the Role and ImplementationID published in system.
**/
EFI_STATUS
EFIAPI
TestPointLibGetTable (
  IN UINT32                   Role,
  IN CHAR16                   *ImplementationID OPTIONAL,
  OUT VOID                    **TestPoint,
  OUT UINTN                   *TestPointSize
  );

/**
  Set FeaturesVerified in published TestPoint table.
  This API will update the TestPoint table with indicated Role and ImplementationID,
  NULL ImplementationID means to find the first TestPoint table with indicated Role.

  @param Role             Role of TestPoint data.
  @param ImplementationID ImplementationID of TestPoint data.
                          NULL means find the first one match Role.
  @param ByteIndex        Byte index of FeaturesVerified of TestPoint data.
  @param BitMask          Bit mask of FeaturesVerified of TestPoint data.

  @retval EFI_SUCCESS          The FeaturesVerified of TestPoint data updated in AIP protocol.
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
  );

/**
  Clear FeaturesVerified in published TestPoint table.
  This API will update the TestPoint table with indicated Role and ImplementationID,
  NULL ImplementationID means to find the first TestPoint table with indicated Role.

  @param Role             Role of TestPoint data.
  @param ImplementationID ImplementationID of TestPoint data.
                          NULL means find the first one match Role.
  @param ByteIndex        Byte index of FeaturesVerified of TestPoint data.
  @param BitMask          Bit mask of FeaturesVerified of TestPoint data.

  @retval EFI_SUCCESS          The FeaturesVerified of TestPoint data updated in AIP protocol.
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
  );

/**
  Append ErrorString in published TestPoint table.
  This API will update the TestPoint table with indicated Role and ImplementationID,
  NULL ImplementationID means to find the first TestPoint table with indicated Role.

  @param Role             Role of TestPoint data.
  @param ImplementationID ImplementationID of TestPoint data.
                          NULL means find the first one match Role.
  @param ErrorString      ErrorString of TestPoint data.

  @retval EFI_SUCCESS          The ErrorString of TestPoint data is updated in AIP protocol.
  @retval EFI_NOT_STARTED      There is not TestPoint table with the Role and ImplementationID published in system.
  @retval EFI_OUT_OF_RESOURCES There is not enough system resource to update ErrorString.
**/
EFI_STATUS
EFIAPI
TestPointLibAppendErrorString (
  IN UINT32                   Role,
  IN CHAR16                   *ImplementationID, OPTIONAL
  IN CHAR16                   *ErrorString
  );

/**
  Set a new ErrorString in published TestPoint table.
  This API will update the TestPoint table with indicated Role and ImplementationID,
  NULL ImplementationID means to find the first TestPoint table with indicated Role.

  @param Role             Role of TestPoint data.
  @param ImplementationID ImplementationID of TestPoint data.
                          NULL means find the first one match Role.
  @param ErrorString      ErrorString of TestPoint data.

  @retval EFI_SUCCESS          The ErrorString of TestPoint data is updated in AIP protocol.
  @retval EFI_NOT_STARTED      There is not TestPoint table with the Role and ImplementationID published in system.
  @retval EFI_OUT_OF_RESOURCES There is not enough system resource to update ErrorString.
**/
EFI_STATUS
EFIAPI
TestPointLibSetErrorString (
  IN UINT32                   Role,
  IN CHAR16                   *ImplementationID, OPTIONAL
  IN CHAR16                   *ErrorString
  );

//
// TEST POINT SMM Communication command
//
#define SMI_HANDLER_TEST_POINT_COMMAND_GET_INFO           0x1
#define SMI_HANDLER_TEST_POINT_COMMAND_GET_DATA_BY_OFFSET 0x2

typedef struct {
  UINT32                            Command;
  UINT32                            DataLength;
  UINT64                            ReturnStatus;
} SMI_HANDLER_TEST_POINT_PARAMETER_HEADER;

typedef struct {
  SMI_HANDLER_TEST_POINT_PARAMETER_HEADER    Header;
  UINT64                                     DataSize;
} SMI_HANDLER_TEST_POINT_PARAMETER_GET_INFO;

typedef struct {
  SMI_HANDLER_TEST_POINT_PARAMETER_HEADER    Header;
  //
  // On input, data buffer size.
  // On output, actual data buffer size copied.
  //
  UINT64                                     DataSize;
  PHYSICAL_ADDRESS                           DataBuffer;
  //
  // On input, data buffer offset to copy.
  // On output, next time data buffer offset to copy.
  //
  UINT64                                     DataOffset;
} SMI_HANDLER_TEST_POINT_PARAMETER_GET_DATA_BY_OFFSET;

extern EFI_GUID gAdapterInfoPlatformTestPointGuid;

#endif
