/** @file
  This file contains sample DXE driver definitions for testing
  and publishing IBV HSTI

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _HSTI_IBV_PLATFORM_DXE_H_
#define _HSTI_IBV_PLATFORM_DXE_H_

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Guid/EventGroup.h>
#include <Protocol/DxeSmmReadyToLock.h>
#include <Library/UefiLib.h>

#include <IndustryStandard/Hsti.h>

#include <Protocol/AdapterInformation.h>
#include <Library/HstiLib.h>
#include <Include/HstiIbvFeatureBit.h>

#pragma pack(1)
typedef struct {
  UINT32  Version;
  UINT32  Role;
  CHAR16  ImplementationID[256];
  UINT32  SecurityFeaturesSize;
  UINT8   SecurityFeaturesRequired[HSTI_SECURITY_FEATURE_SIZE];
  UINT8   SecurityFeaturesImplemented[HSTI_SECURITY_FEATURE_SIZE];
  UINT8   SecurityFeaturesVerified[HSTI_SECURITY_FEATURE_SIZE];
  CHAR16  End;
} ADAPTER_INFO_PLATFORM_SECURITY_STRUCT;
#pragma pack()

extern UINT8  mFeatureImplemented[HSTI_SECURITY_FEATURE_SIZE];

/**
  Concatenate error string.

  @param[in] ErrorCodeString     - Error Code
  @param[in] ErrorCategoryString - Error Category
  @param[in] ErrorString         - Error Text

  @retval CHAR16 - Concatenated string.
**/
CHAR16 *
EFIAPI
BuildHstiErrorString (
  IN  CHAR16                   *ErrorCodeString,
  IN  CHAR16                   *ErrorCategoryString,
  IN  CHAR16                   *ErrorString
  );

/**
  This function sets the verified bit for FirmwareTrustContinuationCryptoStrength.
  Actual test implementation is not present in this sample function.
**/
VOID
CheckFirmwareTrustContinuationCryptoStrength (
  IN UINT32                   Role
  );

/**
  Sets the verified bit for NoTestKeyVerification.
  Actual test implementation is not present in this sample function.
**/
VOID
CheckNoTestKeyVerification (
  IN UINT32                   Role
  );

/**
  Sets the verified bit for FirmwareVersionRollbackProtection.
  Actual test implementation is not present in this sample function.
**/
VOID
CheckFirmwareVersionRollbackProtection (
  IN UINT32                   Role
  );

/**
  Sets verified bit for SecureBootBypass bit
  Actual test implementation is not present in this sample function.
**/
VOID
CheckSecureBootBypass (
  IN UINT32                   Role
  );

/**
  This function sets the verified bit for ExternalDeviceDmaProtection.
  Actual test implementation is not present in this sample function.
**/
VOID
CheckExternalDeviceDmaProtection (
  IN UINT32                   Role
  );

/**
  Sets the verified bit for MorSupport.
  Actual test implementation is not present in this sample function.
**/
VOID
CheckMorSupport (
  IN UINT32                   Role
  );

#endif
