/** @file
  This file contains DXE driver for testing and publishing HSTI

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "HstiSiliconDxe.h"

ADAPTER_INFO_PLATFORM_SECURITY_STRUCT  mHstiStruct = {
  PLATFORM_SECURITY_VERSION_VNEXTCS,
  PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
  {HSTI_PLATFORM_NAME},
  HSTI_SECURITY_FEATURE_SIZE,
  {0}, // SecurityFeaturesRequired
  {0}, // SecurityFeaturesImplemented
  {0}, // SecurityFeaturesVerified
  0,
};

UINT8  mFeatureRequired[HSTI_SECURITY_FEATURE_SIZE] = {
  //
  // Byte 0
  //
  FixedPcdGet8 (PcdHstiIhvFeature1),
  //
  // Byte 1
  //
  FixedPcdGet8 (PcdHstiIhvFeature2),
  //
  // Byte 2
  //
  FixedPcdGet8 (PcdHstiIhvFeature3),
};

UINT8  mFeatureImplemented[HSTI_SECURITY_FEATURE_SIZE] = {
  //
  // Byte 0
  //
  FixedPcdGet8 (PcdHstiIhvFeature1),
  //
  // Byte 1
  //
  FixedPcdGet8 (PcdHstiIhvFeature2),
  //
  // Byte 2
  //
  FixedPcdGet8 (PcdHstiIhvFeature3),
};

DXE_SI_POLICY_PROTOCOL *mSiPolicyData;

/**
  Initialize HSTI feature data
**/
VOID
InitData (
  VOID
  )
{
  EFI_STATUS  Status;

  ADAPTER_INFO_PLATFORM_SECURITY  *Hsti;
  UINT8                           *SecurityFeatures;
  UINTN                           Index;

  if ((mSiPolicyData != NULL) && (mSiPolicyData->Hsti != NULL)) {

    ///
    /// Take cached HSTI feature bitmap data pointed to by policy and publish to OS
    ///
    Hsti = mSiPolicyData->Hsti;

    SecurityFeatures = (UINT8 *) (Hsti + 1);
    DEBUG ((DEBUG_INFO, "  SecurityFeaturesRequired    - "));
    for (Index = 0; Index < Hsti->SecurityFeaturesSize; Index++) {
      DEBUG ((DEBUG_INFO, "%02x ", SecurityFeatures[Index]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
    CopyMem (mHstiStruct.SecurityFeaturesRequired, SecurityFeatures, sizeof (mFeatureRequired));

    SecurityFeatures = (UINT8 *) (SecurityFeatures + Hsti->SecurityFeaturesSize);
    DEBUG ((DEBUG_INFO, "  SecurityFeaturesImplemented - "));
    for (Index = 0; Index < Hsti->SecurityFeaturesSize; Index++) {
      DEBUG ((DEBUG_INFO, "%02x ", SecurityFeatures[Index]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
    CopyMem (mHstiStruct.SecurityFeaturesImplemented, SecurityFeatures, sizeof (mFeatureImplemented));

    SecurityFeatures = (UINT8 *) (SecurityFeatures + Hsti->SecurityFeaturesSize);
    DEBUG ((DEBUG_INFO, "  SecurityFeaturesVerified    - "));
    for (Index = 0; Index < Hsti->SecurityFeaturesSize; Index++) {
      DEBUG ((DEBUG_INFO, "%02x ", SecurityFeatures[Index]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
    CopyMem (mHstiStruct.SecurityFeaturesVerified, SecurityFeatures, sizeof (mFeatureImplemented));
  } else {
    ///
    /// Set only the bitmaps not related to verified, those will get updated during test process
    ///
    CopyMem (mHstiStruct.SecurityFeaturesRequired, mFeatureRequired, sizeof (mFeatureRequired));
    CopyMem (mHstiStruct.SecurityFeaturesImplemented, mFeatureImplemented, sizeof (mFeatureImplemented));
  }
  Status = HstiLibSetTable (
             &mHstiStruct,
             sizeof (mHstiStruct)
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

  StringSize = StrSize (HSTI_ERROR) + StrSize (ErrorCodeString) + StrSize (HSTI_PLATFORM_SECURITY_SPECIFICATION) + StrSize (ErrorCategoryString) + StrSize (ErrorString);
  ErrorStringOut = AllocatePool (StringSize);

  CopyMem (ErrorStringOut, HSTI_ERROR, StrSize (HSTI_ERROR) -1);
  Offset += StrLen (HSTI_ERROR);

  CopyMem (ErrorStringOut + Offset, ErrorCodeString,  StrSize (ErrorCodeString) -1);
  Offset += StrLen (ErrorCodeString);

  CopyMem (ErrorStringOut + Offset, HSTI_PLATFORM_SECURITY_SPECIFICATION, StrSize (HSTI_PLATFORM_SECURITY_SPECIFICATION) -1);
  Offset += StrLen (HSTI_PLATFORM_SECURITY_SPECIFICATION);

  CopyMem (ErrorStringOut + Offset, ErrorCategoryString,  StrSize (ErrorCategoryString) -1);
  Offset += StrLen (ErrorCategoryString);

  CopyMem (ErrorStringOut + Offset, ErrorString,  StrSize (ErrorString));
  Offset += StrLen (ErrorString);

  return  ErrorStringOut;
}

/**
  Update HSTI feature data from cached results or rerun tests
**/
VOID
UpdateData (
  VOID
  )
{
  ADAPTER_INFO_PLATFORM_SECURITY *Hsti;
  UINT8                          *SecurityFeatures;
  CHAR16                         *ErrorString;

  if ((mSiPolicyData != NULL) && (mSiPolicyData->Hsti != NULL)) {

    Hsti = mSiPolicyData->Hsti;

    SecurityFeatures = (UINT8 *) (Hsti + 1);
    SecurityFeatures = (UINT8 *) (SecurityFeatures + Hsti->SecurityFeaturesSize);
    SecurityFeatures = (UINT8 *) (SecurityFeatures + Hsti->SecurityFeaturesSize);
    ErrorString      = (CHAR16 *) (SecurityFeatures + Hsti->SecurityFeaturesSize);

    HstiLibSetErrorString (PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,NULL,ErrorString);

  } else {
    DEBUG ((DEBUG_INFO, "0.0 Hardware Rooted Boot Integrity\n"));
    CheckHardwareRootedBootIntegrity ();

    DEBUG ((DEBUG_INFO, "0.1 Boot Firmware Media Protection (SPI)\n"));
    CheckBootFirmwareMediaProtection ();

    DEBUG ((DEBUG_INFO, "0.2 Signed Firmware Update\n"));
    CheckSignedFirmwareUpdate ();

    DEBUG ((DEBUG_INFO, "0.3 Measured Boot Enforcement\n"));
    CheckMeasuredBootEnforcement ();

    DEBUG ((DEBUG_INFO, "0.4 Integrated Device DMA Protection\n"));
    CheckIntegratedDeviceDmaProtection ();

    DEBUG ((DEBUG_INFO, "0.5 Debug Mode Disabled Verification\n"));
    CheckDebugModeDisabled ();

    DEBUG ((DEBUG_INFO, "0.6 Secure CPU Configuration\n"));
    CheckSecureCpuConfiguration ();

    DEBUG ((DEBUG_INFO, "0.7 Secure System Agent Configuration\n"));
    CheckSecureSystemAgentConfiguration ();

    DEBUG ((DEBUG_INFO, "1.0 Secure Memory Map Configuration\n"));
    CheckSecureMemoryMapConfiguration ();

    DEBUG ((DEBUG_INFO, "1.1 Secure Integrated Graphics Configuration\n"));
    CheckSecureIntegratedGraphicsConfiguration ();

    DEBUG ((DEBUG_INFO, "1.2 Secure PCH Configuration\n"));
    CheckSecurePchConfiguration ();
  }
}

/**
  Dump HSTI info to setial

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
  DEBUG ((DEBUG_INFO, "HSTI\n"));
  DEBUG ((DEBUG_INFO, "  Version                     - 0x%08x\n", Hsti->Version));
  DEBUG ((DEBUG_INFO, "  Role                        - 0x%08x\n", Hsti->Role));
  DEBUG ((DEBUG_INFO, "  ImplementationID            - %S\n", Hsti->ImplementationID));
  DEBUG ((DEBUG_INFO, "  SecurityFeaturesSize        - 0x%08x\n", Hsti->SecurityFeaturesSize));

  SecurityFeatures = (UINT8 *) (Hsti + 1);
  DEBUG ((DEBUG_INFO, "  SecurityFeaturesRequired    - "));
  for (Index = 0; Index < Hsti->SecurityFeaturesSize; Index++) {
    DEBUG ((DEBUG_INFO, "%02x ", SecurityFeatures[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  SecurityFeatures = (UINT8 *) (SecurityFeatures + Hsti->SecurityFeaturesSize);
  DEBUG ((DEBUG_INFO, "  SecurityFeaturesImplemented - "));
  for (Index = 0; Index < Hsti->SecurityFeaturesSize; Index++) {
    DEBUG ((DEBUG_INFO, "%02x ", SecurityFeatures[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  SecurityFeatures = (UINT8 *) (SecurityFeatures + Hsti->SecurityFeaturesSize);
  DEBUG ((DEBUG_INFO, "  SecurityFeaturesVerified    - "));
  for (Index = 0; Index < Hsti->SecurityFeaturesSize; Index++) {
    DEBUG ((DEBUG_INFO, "%02x ", SecurityFeatures[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  ErrorString = (CHAR16 *) (SecurityFeatures + Hsti->SecurityFeaturesSize);
  DEBUG ((DEBUG_INFO, "  ErrorString                 - \""));
  CopyMem (&ErrorChar, ErrorString, sizeof (ErrorChar));
  for (; ErrorChar != 0;) {
    DEBUG ((DEBUG_INFO, "%c", ErrorChar));
    ErrorString++;
    CopyMem (&ErrorChar, ErrorString, sizeof (ErrorChar));
  }
  DEBUG ((DEBUG_INFO, "\"\n"));
}

/**
  Retrieve HSTI Table from AIP
**/
VOID
DumpData (
  VOID
  )
{
  VOID        *Hsti;
  UINTN       HstiSize;
  EFI_STATUS  Status;

  Status = HstiLibGetTable (PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE, NULL, &Hsti, &HstiSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HSTI (Role - 0x%08x) not found!\n", PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE));
    return;
  }

  if (mSiPolicyData != NULL) {
    mSiPolicyData->Hsti = (ADAPTER_INFO_PLATFORM_SECURITY *) Hsti;
    mSiPolicyData->HstiSize = HstiSize;
  }

  DumpHsti (Hsti);
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
  EFI_HANDLE                  Handle;
  EFI_STATUS                  Status;

  InitMp ();
  InitData ();
  UpdateData ();
  DumpData ();

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gHstiPublishCompleteProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

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
HstiSiliconDxeEntrypoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   Event;

  //
  // Locate DxeSiPolicyProtocolGuid protocol instance and assign it to a global variable
  //
  Status = gBS->LocateProtocol (&gDxeSiPolicyProtocolGuid, NULL, (VOID **) &mSiPolicyData);
  if (EFI_ERROR (Status)) {
    mSiPolicyData = NULL;
    DEBUG ((DEBUG_ERROR,"Failed to locate DxeSiPolicyProtocolGuid Protocol\n"));
  }

  Status = gBS->InstallProtocolInterface (
                  &gImageHandle,
                  &gHstiProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  EfiCreateEventReadyToBootEx (
    TPL_NOTIFY,
    OnReadyToBoot,
    NULL,
    &Event
    );

  return EFI_SUCCESS;
}
