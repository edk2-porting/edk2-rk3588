/** @file
  This file contains sample DXE driver for publishing IBV HSTI.
  This driver does not actually implement the test, but merely
  publishes the IBV role HSTI structure indicating all tests pass

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "HstiIbvPlatformDxe.h"

ADAPTER_INFO_PLATFORM_SECURITY_STRUCT  mHstiStruct = {
  PLATFORM_SECURITY_VERSION_VNEXTCS,
  PLATFORM_SECURITY_ROLE_PLATFORM_IBV,
  {HSTI_IMPLEMENTATION_ID_PLATFORM},
  HSTI_SECURITY_FEATURE_SIZE,
  {0}, // SecurityFeaturesRequired
  {0}, // SecurityFeaturesImplemented
  {0}, // SecurityFeaturesVerified
  0,
};

UINT8  mFeatureImplemented[HSTI_SECURITY_FEATURE_SIZE];

/**
  Initialize HSTI feature data
**/
VOID
InitData (
  IN UINT32                   Role
  )
{
  EFI_STATUS                             Status;

  ASSERT (PcdGetSize(PcdHstiIbvPlatformFeature) == sizeof(mFeatureImplemented));
  CopyMem (mFeatureImplemented, PcdGetPtr(PcdHstiIbvPlatformFeature), sizeof(mFeatureImplemented));

  mHstiStruct.Role = Role;
  CopyMem (mHstiStruct.SecurityFeaturesImplemented, mFeatureImplemented, sizeof(mFeatureImplemented));
  Status = HstiLibSetTable (
             &mHstiStruct,
             sizeof(mHstiStruct)
             );
  if (EFI_ERROR (Status)) {
    if (Status != EFI_ALREADY_STARTED) {
      ASSERT_EFI_ERROR (Status);
    }
  }
}

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
  )
{
  UINTN                            Offset;
  UINTN                            StringSize;
  CHAR16                           *ErrorStringOut;

  ErrorStringOut = NULL;
  Offset = 0;

  StringSize = StrSize (HSTI_ERROR) + StrSize(ErrorCodeString) + StrSize(HSTI_PLATFORM_SECURITY_SPECIFICATION) + StrSize(ErrorCategoryString) + StrSize(ErrorString);
  ErrorStringOut = AllocatePool (StringSize);
  ASSERT (ErrorStringOut != NULL);
  if (ErrorStringOut != NULL) {
    CopyMem (ErrorStringOut, HSTI_ERROR, StrSize (HSTI_ERROR)-1);
    Offset += StrLen (HSTI_ERROR);

    CopyMem (ErrorStringOut + Offset, ErrorCodeString,  StrSize(ErrorCodeString)-1);
    Offset += StrLen(ErrorCodeString);

    CopyMem (ErrorStringOut + Offset, HSTI_PLATFORM_SECURITY_SPECIFICATION, StrSize(HSTI_PLATFORM_SECURITY_SPECIFICATION)-1);
    Offset += StrLen(HSTI_PLATFORM_SECURITY_SPECIFICATION);

    CopyMem (ErrorStringOut + Offset, ErrorCategoryString,  StrSize(ErrorCategoryString)-1);
    Offset += StrLen(ErrorCategoryString);

    CopyMem (ErrorStringOut + Offset, ErrorString,  StrSize(ErrorString));
    Offset += StrLen(ErrorString);
  }
  return  ErrorStringOut;
}

/**
  Update HSTI feature data
**/
VOID
UpdateData (
  IN UINT32                   Role
  )
{
  DEBUG ((EFI_D_INFO, "2.0 Firmware Trust Continuation Crypto Strength\n"));
  CheckFirmwareTrustContinuationCryptoStrength (Role);

  DEBUG ((EFI_D_INFO, "2.1 No Test Key Verification\n"));
  CheckNoTestKeyVerification (Role);

  DEBUG ((EFI_D_INFO, "2.2 Firmware Version Rollback Protection\n"));
  CheckFirmwareVersionRollbackProtection (Role);

  DEBUG ((EFI_D_INFO, "2.3 SecureBoot Bypass Checking\n"));
  CheckSecureBootBypass (Role);

  DEBUG ((EFI_D_INFO, "2.4 External Device DMA Protection\n"));
  CheckExternalDeviceDmaProtection (Role);

  DEBUG ((EFI_D_INFO, "2.5 MOR Support\n"));
  CheckMorSupport (Role);
}

/**
  Dump HSTI info to serial

  @param[in] HstiData     - Pointer to HSTI data
**/
VOID
DumpHsti (
  IN VOID                     *HstiData
  )
{
  ADAPTER_INFO_PLATFORM_SECURITY *Hsti;
  UINT8                          *SecurityFeatures;
  CHAR16                         *ErrorString;
  UINTN                          Index;
  CHAR16                         ErrorChar;

  Hsti = HstiData;
  DEBUG ((EFI_D_INFO, "HSTI\n"));
  DEBUG ((EFI_D_INFO, "  Version                     - 0x%08x\n", Hsti->Version));
  DEBUG ((EFI_D_INFO, "  Role                        - 0x%08x\n", Hsti->Role));
  DEBUG ((EFI_D_INFO, "  ImplementationID            - %S\n", Hsti->ImplementationID));
  DEBUG ((EFI_D_INFO, "  SecurityFeaturesSize        - 0x%08x\n", Hsti->SecurityFeaturesSize));

  SecurityFeatures = (UINT8 *)(Hsti + 1);
  DEBUG ((EFI_D_INFO, "  SecurityFeaturesRequired    - "));
  for (Index = 0; Index < Hsti->SecurityFeaturesSize; Index++) {
    DEBUG ((EFI_D_INFO, "%02x ", SecurityFeatures[Index]));
  }
  DEBUG ((EFI_D_INFO, "\n"));

  SecurityFeatures = (UINT8 *)(SecurityFeatures + Hsti->SecurityFeaturesSize);
  DEBUG ((EFI_D_INFO, "  SecurityFeaturesImplemented - "));
  for (Index = 0; Index < Hsti->SecurityFeaturesSize; Index++) {
    DEBUG ((EFI_D_INFO, "%02x ", SecurityFeatures[Index]));
  }
  DEBUG ((EFI_D_INFO, "\n"));

  SecurityFeatures = (UINT8 *)(SecurityFeatures + Hsti->SecurityFeaturesSize);
  DEBUG ((EFI_D_INFO, "  SecurityFeaturesVerified    - "));
  for (Index = 0; Index < Hsti->SecurityFeaturesSize; Index++) {
    DEBUG ((EFI_D_INFO, "%02x ", SecurityFeatures[Index]));
  }
  DEBUG ((EFI_D_INFO, "\n"));

  ErrorString = (CHAR16 *)(SecurityFeatures + Hsti->SecurityFeaturesSize);
  DEBUG ((EFI_D_INFO, "  ErrorString                 - \""));
  CopyMem (&ErrorChar, ErrorString, sizeof(ErrorChar));
  for (; ErrorChar != 0;) {
    DEBUG ((EFI_D_INFO, "%c", ErrorChar));
    ErrorString++;
    CopyMem (&ErrorChar, ErrorString, sizeof(ErrorChar));
  }
  DEBUG ((EFI_D_INFO, "\"\n"));
}

/**
  Retrieve HSTI Table from AIP
**/
VOID
DumpData (
  IN UINT32                   Role
  )
{
  VOID        *Hsti;
  UINTN       HstiSize;
  EFI_STATUS  Status;

  Status = HstiLibGetTable (Role, NULL, &Hsti, &HstiSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "HSTI (Role - 0x%08x) not found!\n", Role));
    return ;
  }

  DumpHsti (Hsti);

  FreePool (Hsti);
}

/**
  Handler to gather and publish HSTI results on ReadyToBootEvent

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context
**/
VOID
EFIAPI
OnReadyToBoot (
  EFI_EVENT                               Event,
  VOID                                    *Context
  )
{
  UpdateData (PLATFORM_SECURITY_ROLE_PLATFORM_IBV);
  DumpData (PLATFORM_SECURITY_ROLE_PLATFORM_IBV);

  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
}

/**
  The driver's entry point.

  @param[in] ImageHandle  The firmware allocated handle for the EFI image.
  @param[in] SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS     The entry point is executed successfully.
  @retval other           Some error occurs when executing this entry point.
**/
EFI_STATUS
EFIAPI
HstiPlatformDxeEntrypoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   Event;

  InitData (PLATFORM_SECURITY_ROLE_PLATFORM_IBV);

  Status = EfiCreateEventReadyToBootEx (
            TPL_NOTIFY,
            OnReadyToBoot,
            NULL,
            &Event
            );

  return Status;
}
