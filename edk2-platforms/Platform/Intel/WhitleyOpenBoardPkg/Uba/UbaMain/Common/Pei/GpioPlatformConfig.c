/** @file

  @copyright
  Copyright 2013 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiCommonBoardInitLib.h"
#include <GpioPinsSklH.h>
#include <Library/GpioLib.h>
#include <Library/UbaGpioPlatformConfig.h>
#include <Library/PchInfoLib.h>
#include <Ppi/DynamicSiLibraryPpi.h>

STATIC PLATFORM_GPIO_CONFIG_TABLE mGpioPlatformConfig = {

  PLATFORM_GPIO_CONFIG_SIGNATURE,
  PLATFORM_GPIO_CONFIG_VERSION,

  //MFG pad
  {
    GPIO_SKL_H_GPP_C9, //To be verified with the Board schematics
    {GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioPlatformReset, GpioTermNone}
  },

  GPIO_SKL_H_GPP_A18,

  //Recovery jumper pad
  GPIO_SKL_H_GPP_B14,

  //FM ADR Trigger
  GPIO_SKL_H_GPP_E3,

  //ADR enable GPIO output
  GPIO_SKL_H_GPP_D4,

  // Force to S1 config mode pad
  GPIO_SKL_H_GPP_A17,

  //
  // Used by PC Platforms
  //
  GPIO_SKL_H_GPP_B3,
  //
  // Used by PC platforms. This is the first GPIO pad of the pad series to indicate Board ID
  //
  GPIO_SKL_H_GPP_G12,

  //
  // WHEA SCI generation pad
  //
  GPIO_SKL_H_GPP_A12,

  //
  // Used to generate CPU HP SMI
  //
  GPIO_SKL_H_GPP_E6,

  //
  // FPGA error indicator
  //
  GPIO_SKL_H_GPP_E4,

  //
  // FPGA error indicator
  //
  GPIO_SKL_H_GPP_E5,

  // Flash Security
  UNUSED_GPIO,
};

STATIC PLATFORM_GPIO_CONFIG_TABLE mGpioMiniPchPlatformConfig = {

  PLATFORM_GPIO_CONFIG_SIGNATURE,
  PLATFORM_GPIO_CONFIG_VERSION,

  //MFG pad
  {
    UNUSED_GPIO, //To be verified with the Board schematics
    {0, 0, 0, 0, 0, 0, 0}
  },

  UNUSED_GPIO,

  //Recovery jumper pad
  UNUSED_GPIO,

  //FM ADR Trigger
  UNUSED_GPIO,

  //ADR enable GPIO output
  UNUSED_GPIO,

  // Force to S1 config mode pad
  UNUSED_GPIO,

  //
  // Used by PC Platforms
  //
  UNUSED_GPIO,
  //
  // Used by PC platforms. This is the first GPIO pad of the pad series to indicate Board ID
  //
  UNUSED_GPIO,

  //
  // WHEA SCI generation pad
  //
  UNUSED_GPIO,

  //
  // Used to generate CPU HP SMI
  //
  UNUSED_GPIO,

  //
  // FPGA error indicator
  //
  UNUSED_GPIO,

  //
  // FPGA error indicator
  //
  UNUSED_GPIO,

};


EFI_STATUS
InstallGpioPlatformData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
)
{
  EFI_STATUS                  Status;
  DYNAMIC_SI_LIBARY_PPI       *DynamicSiLibraryPpi = NULL;


  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  if (DynamicSiLibraryPpi->GetPchSeries () == PchMini) {
    return UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformGpioPlatformConfigDataGuid,
                                 &mGpioMiniPchPlatformConfig,
                                 sizeof(mGpioMiniPchPlatformConfig)
                                 );
  }

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformGpioPlatformConfigDataGuid,
                                 &mGpioPlatformConfig,
                                 sizeof(mGpioPlatformConfig)
                                 );
  ASSERT_EFI_ERROR (Status);

  return Status;
}


