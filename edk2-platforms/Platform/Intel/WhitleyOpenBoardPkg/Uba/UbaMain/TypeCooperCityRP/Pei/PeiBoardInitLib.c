/** @file

 @copyright
  Copyright 2018 - 2021 Intel Corporation.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/


/**
  The constructor function for Board Init Libray.

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval  EFI_SUCCESS            Table initialization successfully.
  @retval  EFI_OUT_OF_RESOURCES   No enough memory to initialize table.
**/

#include "PeiBoardInit.h"
#include <UncoreCommonIncludes.h>
#include <Library/PchMultiPchBase.h>
#include <Ppi/DynamicSiLibraryPpi.h>


/**

  Identifies platform capabilites if the platform is a 2-socket standalone
  modular board, or a 4-socket modular boards, or 8-socket modular boards.

  @param[in,out] PlatformCapabilities    Holds the information about platform being
                                         a standalone 2S, or 4S/8S modular boards.

  @retval EFI_SUCCESS  Platform Capabilities is updated.

**/
EFI_STATUS
GetPlatformCapabilitiesInfo (
  IN OUT UINT8                 *PlatformCapabilities,
  IN DYNAMIC_SI_LIBARY_PPI     *DynamicSiLibraryPpi
  )
{
  GPIO_CONFIG          PadConfig;
  UINT32               GpioState;
  EFI_STATUS           Status = EFI_SUCCESS;

  *PlatformCapabilities = PLATFORM_CAPABILITY_UNDEFINED;

  PadConfig.PadMode          = GpioPadModeGpio;
  PadConfig.HostSoftPadOwn   = GpioHostOwnGpio;
  PadConfig.Direction        = GpioDirIn;
  PadConfig.OutputState      = GpioOutDefault;
  PadConfig.InterruptConfig  = GpioIntDefault;
  PadConfig.PowerConfig      = GpioResetDefault;
  PadConfig.ElectricalConfig = GpioTermDefault;
  PadConfig.LockConfig       = GpioLockDefault;
  PadConfig.OtherSettings    = GpioRxRaw1Default;

  //
  // GPP_D6 is standalone signal. GPP_D7 is 4S/8S signal.
  //   GPP_D7     |  GPP_D6
  //  =======================
  //   Don't care |    0   (2-socket)
  //       1      |    1   (4-socket)
  //       0      |    1   (8-socket)
  //

  if (DynamicSiLibraryPpi->IsSimicsEnvironment ()) {
    //
    // Workaround until GPIO pins new definition are implemented in Simics
    //
    *PlatformCapabilities = PLATFORM_CAPABILITY_8_SOCKET;
    return EFI_SUCCESS;
  }

  Status = DynamicSiLibraryPpi->GpioSetPadConfigByPchId (PCH_LEGACY_ID, GPIO_SKL_H_GPP_D6, &PadConfig);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = DynamicSiLibraryPpi->GpioGetInputValueByPchId (PCH_LEGACY_ID, GPIO_SKL_H_GPP_D6, &GpioState);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "GPP_D6:%d\n", GpioState));

  if (GpioState == 0) {
    *PlatformCapabilities = PLATFORM_CAPABILITY_2_SOCKET;
    return Status;
  }

  Status = DynamicSiLibraryPpi->GpioSetPadConfigByPchId (PCH_LEGACY_ID, GPIO_SKL_H_GPP_D7, &PadConfig);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = DynamicSiLibraryPpi->GpioGetInputValueByPchId (PCH_LEGACY_ID, GPIO_SKL_H_GPP_D7, &GpioState);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "GPP_D7:%d\n", GpioState));

  *PlatformCapabilities = (GpioState == 1) ? PLATFORM_CAPABILITY_4_SOCKET : PLATFORM_CAPABILITY_8_SOCKET;

  return Status;
}


EFI_STATUS
EFIAPI
TypeCooperCityRPPeiBoardInitLibConstructor (
  IN EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS                      Status = EFI_SUCCESS;
  UBA_CONFIG_DATABASE_PPI         *UbaConfigPpi;
  DYNAMIC_SI_LIBARY_PPI           *DynamicSiLibraryPpi = NULL;
  EFI_HOB_GUID_TYPE               *GuidHob;
  EFI_PLATFORM_INFO               *PlatformInfo;
  UINT8                           SocketIndex;
  CONST CHAR8                     *Str;

  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  PlatformInfo  = GET_GUID_HOB_DATA(GuidHob);

  if (PlatformInfo->BoardId == TypeCooperCityRP) {

    DEBUG ((DEBUG_INFO, "PEI UBA init BoardId 0x%X: CooperCityRP\n", PlatformInfo->BoardId));

    PlatformInfo->MaxNumOfPchs = 4;
    ASSERT (PlatformInfo->MaxNumOfPchs <= PCH_MAX);

    Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }

    Status = UbaConfigPpi->InitSku (
                       UbaConfigPpi,
                       PlatformInfo->BoardId,
                       NULL,
                       NULL
                       );
    ASSERT_EFI_ERROR (Status);

    Status = TypeCooperCityRPInstallGpioData (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = TypeCooperCityRPInstallPcdData (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = TypeCooperCityRPInstallSoftStrapData (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = TypeCooperCityRPPchEarlyUpdate (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = TypeCooperCityRPPlatformUpdateUsbOcMappings (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = TypeCooperCityRPIioPortBifurcationInit (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = TypeCooperCityRPInstallSlotTableData (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    //
    // Set default memory type connector to DimmConnectorSmt
    //
    (*PeiServices)->SetMem (&PlatformInfo->MemoryConnectorType, sizeof (PlatformInfo->MemoryConnectorType), DimmConnectorSmt);

    //
    // Initialize InterposerType to InterposerUnknown
    //
    for (SocketIndex = 0; SocketIndex < MAX_SOCKET; ++SocketIndex) {
      PlatformInfo->InterposerType[SocketIndex] = InterposerUnknown;
    }

    GetPlatformCapabilitiesInfo (&PlatformInfo->PlatformCapabilities, DynamicSiLibraryPpi);

    Str = (PlatformInfo->PlatformCapabilities <= PLATFORM_CAPABILITY_8_SOCKET) ? \
          PlatformCapabilitiesStr[PlatformInfo->PlatformCapabilities] : PlatformCapabilitiesStr[PLATFORM_CAPABILITY_UNDEFINED];
    DEBUG ((DEBUG_INFO, "PlatformCapabilities = %a\n", Str));

    Status = TypeCooperCityRPInstallKtiEparamData (UbaConfigPpi, PlatformInfo->PlatformCapabilities);
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }

  return Status;
}
