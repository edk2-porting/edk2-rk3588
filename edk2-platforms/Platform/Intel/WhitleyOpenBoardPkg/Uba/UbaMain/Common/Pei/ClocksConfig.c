/** @file
  Install Platform Clocks Config Data.

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiCommonBoardInitLib.h"
#include <Library/PlatformClocksLib.h>
#include <Library/UbaClocksConfigLib.h>
#include <Library/SpsPeiLib.h>
#include <Library/PchInfoLib.h>
#include <Ppi/PchPolicy.h>
#include <Ppi/DynamicSiLibraryPpi.h>

//
// Table providing details on clocks supported by this library
//
// It is critical that this table be properly constructed.
// The table entries must line up with the clock generatory types
//
CLOCK_GENERATOR_DETAILS mSupportedClockGeneratorTable[] = {
{
    ClockGeneratorCk410,
    CK410_GENERATOR_ID,
    CK410_GENERATOR_SPREAD_SPECTRUM_BYTE,
    CK410_GENERATOR_SPREAD_SPECTRUM_BIT
  },
{
    ClockGeneratorCk420,
    CK420_GENERATOR_ID,
    CK420_GENERATOR_SPREAD_SPECTRUM_BYTE,
    CK420_GENERATOR_SPREAD_SPECTRUM_BIT
  },
{
    ClockGeneratorCk440,
    CK440_GENERATOR_ID,
    CK440_GENERATOR_SPREAD_SPECTRUM_BYTE,
    CK440_GENERATOR_SPREAD_SPECTRUM_BIT
  },
{
    ClockGeneratorCk505,
    CK505_GENERATOR_ID,
    CK505_GENERATOR_SPREAD_SPECTRUM_BYTE,
    CK505_GENERATOR_SPREAD_SPECTRUM_BIT
  }
};

/**

  Configure the clock generator and enable Spread Spectrum if applicable.

  @param None

  @retval EFI_SUCCESS      The function completed successfully.
  @retval EFI_UNSUPPORTED  Clock generator configuration is not supported

**/
EFI_STATUS
PlatformClocksConfigCallback (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDescriptor,
  IN VOID                               *Ppi
  )
{
  EFI_STATUS                            Status;
  CLOCKING_MODES                        ClockingMode;
  UINT8                                 *ConfigurationTable;
  UINT8                                 ConfigurationTablePlatformSRP[] = CLOCK_GENERATOR_SETTINGS_PLATFORMSRP;
  UINT8                                 ConfigurationTableCK505[] = CLOCK_GENERATOR_SETTINGS_CK505;
  UINTN                                 Length;
  CLOCK_GENERATOR_TYPE                  ClockType;
  BOOLEAN                               SecondarySmbus = FALSE;
  BOOLEAN                               EnableSpreadSpectrum;
  PCH_POLICY_PPI                        *PchPolicyPpi;
  UINT8                                 ClockGeneratorAddress = 0;
  DYNAMIC_SI_LIBARY_PPI                 *DynamicSiLibraryPpi = NULL;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "ConfigurePlatformClocks. Can not read gDynamicSiLibraryPpiGuid\n"));
    return Status;
  }

  ClockGeneratorAddress = PcdGet8 (PcdOemSkuClockGeneratorAddress);

  ClockingMode = InternalAlternate;
  Status = EFI_SUCCESS;
  if (EFI_ERROR (Status)) {
    if (Status != EFI_UNSUPPORTED) {
      DEBUG ((DEBUG_ERROR, "ConfigurePlatformClocks. Can't read clock mode! EFI_STATUS = %r\n", Status));
    }
    return Status;
  }

  if (External == ClockingMode)
  {
    DEBUG ((DEBUG_INFO, "ConfigurePlatformClocks. Clock Mode: External\n"));

    //
    // If the clocking mode is external and CPX is present, then no further configuration of the
    // clock is supported at this time.
    //
    if (DynamicSiLibraryPpi->IsCpuAndRevision (CPU_CPX, REV_ALL)) {
      return EFI_UNSUPPORTED;
    }

    if (DynamicSiLibraryPpi->IsSimicsEnvironment()) {
      //
      // Simics implements CK505 model
      //
      ConfigurationTable = ConfigurationTableCK505;
      Length = sizeof (ConfigurationTableCK505);
      ClockType = ClockGeneratorCk505;
    }
    else {
      //
      // SRP/DVP configuration
      //
      ConfigurationTable = ConfigurationTablePlatformSRP;
      Length = sizeof (ConfigurationTablePlatformSRP);
      ClockType = ClockGeneratorCk420;
    }
    Status = (*PeiServices)->LocatePpi (PeiServices,
                                        &gPchPlatformPolicyPpiGuid,
                                        0,
                                        NULL,
                                        (VOID **)&PchPolicyPpi
                                       );
    ASSERT_EFI_ERROR (Status);
    EnableSpreadSpectrum = (BOOLEAN) PchPolicyPpi->PchConfig.EnableClockSpreadSpec;
    if (1 == EnableSpreadSpectrum) {
      ConfigurationTable[mSupportedClockGeneratorTable[ClockType].SpreadSpectrumByteOffset] |= mSupportedClockGeneratorTable[ClockType].SpreadSpectrumBitOffset;
    } else {
      ConfigurationTable[mSupportedClockGeneratorTable[ClockType].SpreadSpectrumByteOffset] &= ~(mSupportedClockGeneratorTable[ClockType].SpreadSpectrumBitOffset);
    }
    Status = ConfigureClockGenerator (PeiServices,
                                      ClockType,
                                      ClockGeneratorAddress,
                                      Length,
                                      ConfigurationTable,
                                      EnableSpreadSpectrum,
                                      &mSupportedClockGeneratorTable[ClockType],
                                      SecondarySmbus
                                     );
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

PLATFORM_CLOCKS_CONFIG_TABLE    PlatformClocksConfigTable =
{
  PLATFORM_CLOCKS_CONFIG_SIGNATURE,
  PLATFORM_CLOCKS_CONFIG_VERSION,
  PlatformClocksConfigCallback
};

EFI_STATUS
InstallPlatformClocksConfigData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
)
{
  EFI_STATUS                            Status = EFI_SUCCESS;

   Status = UbaConfigPpi->AddData (
                                  UbaConfigPpi,
                                  &gPlatformClocksConfigDataGuid,
                                  &PlatformClocksConfigTable,
                                  sizeof(PlatformClocksConfigTable)
                                  );

  return Status;
}

