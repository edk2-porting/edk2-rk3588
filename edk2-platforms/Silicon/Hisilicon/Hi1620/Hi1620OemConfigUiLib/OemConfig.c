/** @file
The OEM config reference implementation

Copyright (c) 2018, Hisilicon Limited. All rights reserved.
Copyright (c) 2018, Linaro Limited. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "OemConfig.h"

OEM_CONFIG_CALLBACK_DATA    mOemConfigPrivate = {
  OEM_CONFIG_CALLBACK_DATA_SIGNATURE,
  NULL,
  NULL,
  {
    OemExtractConfig,
    OemRouteConfig,
    OemCallback
  }
};

HII_VENDOR_DEVICE_PATH  mOemHiiVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    { 0x874c4dcb, 0x08ec, 0x4fe6, { 0xb5, 0x8e, 0x3a, 0x9e, 0x1c, 0x26, 0x70, 0xb9 } }
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),
      (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};






/**
  This function allows a caller to extract the current configuration for one
  or more named elements from the target driver.


  @param This            Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Request         A null-terminated Unicode string in <ConfigRequest> format.
  @param Progress        On return, points to a character in the Request string.
                         Points to the string's null terminator if request was successful.
                         Points to the most recent '&' before the first failing name/value
                         pair (or the beginning of the string if the failure is in the
                         first name/value pair) if the request was not successful.
  @param Results         A null-terminated Unicode string in <ConfigAltResp> format which
                         has all values filled in for the names in the Request string.
                         String to be allocated by the called function.

  @retval  EFI_INVALID_PARAMETER  Request is illegal syntax, or unknown name.
  @retval  EFI_NOT_FOUND          Routing data doesn't match any storage in this driver.

**/
EFI_STATUS
EFIAPI
OemExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  )
{
  if (Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  *Progress = Request;
  return EFI_NOT_FOUND;
}

/**
  This function processes the results of changes in configuration.

  @param This            Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Configuration   A null-terminated Unicode string in <ConfigResp> format.
  @param Progress        A pointer to a string filled in with the offset of the most
                         recent '&' before the first failing name/value pair (or the
                         beginning of the string if the failure is in the first
                         name/value pair) or the terminating NULL if all was successful.

  @retval  EFI_INVALID_PARAMETER  Configuration is NULL.
  @retval  EFI_NOT_FOUND          Routing data doesn't match any storage in this driver.

**/
EFI_STATUS
EFIAPI
OemRouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
  )
{
  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Progress = Configuration;

  return EFI_NOT_FOUND;
}

/**
  This function is invoked if user selected a interactive opcode from Device Manager's
  Formset. If user set VBIOS, the new value is saved to EFI variable.

  @param This            Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Action          Specifies the type of action taken by the browser.
  @param QuestionId      A unique value which is sent to the original exporting driver
                         so that it can identify the type of data to expect.
  @param Type            The type of value for the question.
  @param Value           A pointer to the data being sent to the original exporting driver.
  @param ActionRequest   On return, points to the action requested by the callback function.

  @retval  EFI_SUCCESS           The callback successfully handled the action.
  @retval  EFI_INVALID_PARAMETER The setup browser call this function with invalid parameters.

**/
EFI_STATUS
EFIAPI
OemCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest
  )
{
  if (Action != EFI_BROWSER_ACTION_CHANGING) {
    //
    // Do nothing for other UEFI Action. Only do call back when data is changed.
    //
    return EFI_UNSUPPORTED;
  }
  if ((Value == NULL) || (ActionRequest == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

/**
  Install Boot Manager Menu driver.

  @param ImageHandle     The image handle.
  @param SystemTable     The system table.

  @retval  EFI_SUCEESS  Install Boot manager menu success.
  @retval  Other        Return error status.

**/
EFI_STATUS
EFIAPI
OemConfigUiLibConstructor (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS                  Status;
  OEM_CONFIG_DATA             Configuration;
  VOID                        *Hob;
  VERSION_INFO                *VersionInfo;
  UINTN                       BufSize;
  BOOLEAN                     Action;
  EFI_STRING                  ConfigRequestHdr;
  EFI_TIME                    Time = {0};
  CHAR16                      TmpString[0x100] = {0};

  mOemConfigPrivate.DriverHandle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mOemConfigPrivate.DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mOemHiiVendorDevicePath,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mOemConfigPrivate.ConfigAccess,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Publish our HII data.
  //
  mOemConfigPrivate.HiiHandle = HiiAddPackages (
                                   &gOemConfigGuid,
                                   mOemConfigPrivate.DriverHandle,
                                   OemConfigVfrBin,
                                   OemConfigUiLibStrings,
                                   NULL
                                   );
  if (mOemConfigPrivate.HiiHandle == NULL) {
    DEBUG ((DEBUG_ERROR, "%a Fail to Add Oem Hii Package.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }
  //
  //BIOS Build Time Init
  //
  Hob = GetFirstGuidHob (&gVersionInfoHobGuid);
  if (Hob == NULL) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%d] Version information HOB not found!\n",
      __FILE__, __LINE__));
    return EFI_NOT_FOUND;
  }
  VersionInfo = GET_GUID_HOB_DATA (Hob);
  Time = VersionInfo->BuildTime;
  UnicodeSPrint (
    TmpString,
    sizeof (TmpString),
    L"%02d/%02d/%04d %02d:%02d",
    Time.Month,
    Time.Day,
    Time.Year,
    Time.Hour,
    Time.Minute
    );
  HiiSetString (
    mOemConfigPrivate.HiiHandle,
    STRING_TOKEN (STR_MISC_BIOS_BUILDTIME),
    TmpString,
    "en-US"
    );
  //
  // Check Oem Config Variable.
  //
  ConfigRequestHdr = HiiConstructConfigHdr (
                       &gOemConfigGuid,
                       OEM_CONFIG_NAME,
                       mOemConfigPrivate.DriverHandle
                       );
  ZeroMem (&Configuration, sizeof (OEM_CONFIG_DATA));
  BufSize = sizeof (OEM_CONFIG_DATA);
  Status = gRT->GetVariable (
                  OEM_CONFIG_NAME,
                  &gOemConfigGuid,
                  NULL,
                  &BufSize,
                  &Configuration
                  );
  if (EFI_ERROR (Status)) {
    Action = HiiSetToDefaults (ConfigRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
    if (!Action) {
      //
      //Set the default value of the DDR option
      //
      Configuration.DdrDebugLevel = 0;
      Configuration.DdrFreqLimit = 0;
      Configuration.DdrRefreshSupport = 0;
      Configuration.DdrRefreshRate = 0;
      Configuration.RankMargin = 0;
      Configuration.RankMarginMode = 2;
      Configuration.rmtPatternLength = 256;
      Configuration.perbitmargin = 0x0;
      Configuration.CaMargin = 0x0;
      Configuration.CaVrefMarginOption = 0x0;
      Configuration.NumaEn = 1;
      Configuration.HwMemTest = 0;
      Configuration.DieInterleaving = 0;
      Configuration.ChannelInterleaving = 1;
      Configuration.RankInterleaving = 2;
      Configuration.EccSupport = 0;
      //
      //Set the default value of the BMC option
      //
      Configuration.BmcWdtEnable = 0;
      Configuration.BmcWdtTimeout = 15;
      Configuration.BmcWdtAction = 1;
      Configuration.OSWdtEnable = 0;
      Configuration.OSWdtTimeout = 5;
      Configuration.OSWdtAction = 1;
      //
      //Set the default value of the Misc option
      //
      Configuration.EnableSmmu = 1;
      Configuration.EnableFdtTable = 0;
      Configuration.EnableGOP = 0;
      //
      //Set the default value of the Ras option
      //
      Configuration.EnRasSupport = 1;
      Configuration.EnPoison = 1;
      Configuration.CheckAlgorithm = 0;
      Configuration.PatrolScrub = 1;
      Configuration.PatrolScrubDuration = 24;
      Configuration.DemandScrubMode = 0;
      Configuration.CorrectErrorThreshold = 0;
      Configuration.AdvanceDeviceCorrection = 0;
      Configuration.RankSparing = 0;
      Configuration.FunnelPeriod = 0;
      Configuration.DpcFeature = 0;
      Configuration.EcrcFeature = 0;
      Configuration.CompletionTimeout = 1;
      Configuration.CompletionTimeoutValue = 0;
      Configuration.HotPlug = 1;
      Status = gRT->SetVariable (
                      OEM_CONFIG_NAME,
                      &gOemConfigGuid,
                      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                      sizeof (OEM_CONFIG_DATA),
                      &Configuration
                      );
    }
  } else {
    Action = HiiValidateSettings (ConfigRequestHdr);
    if (!Action) {
      return EFI_INVALID_PARAMETER;
    }
  }
  return EFI_SUCCESS;
}

/**
  Unloads the application and its installed protocol.

  @param  ImageHandle     Handle that identifies the image to be unloaded.
  @param  SystemTable     The system table.

  @retval EFI_SUCCESS           The image has been unloaded.
**/
EFI_STATUS
EFIAPI
OemConfigUiLibDestructor (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS                  Status;

  Status = gBS->UninstallMultipleProtocolInterfaces (
                  mOemConfigPrivate.DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mOemHiiVendorDevicePath,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mOemConfigPrivate.ConfigAccess,
                  NULL
                  );

  HiiRemovePackages (mOemConfigPrivate.HiiHandle);

  return Status;
}

