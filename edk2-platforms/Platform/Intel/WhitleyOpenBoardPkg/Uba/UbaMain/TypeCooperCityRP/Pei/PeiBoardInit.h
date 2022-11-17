/** @file
  PeiBoardInit.

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_BOARD_INIT_PEIM_H_
#define _PEI_BOARD_INIT_PEIM_H_

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Ppi/UbaCfgDb.h>
#include <Guid/PlatformInfo.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/GpioLib.h>
#include <GpioPinsSklH.h>
#include <Ppi/DynamicSiLibraryPpi.h>

#define PLATFORM_CAPABILITY_UNDEFINED   0
#define PLATFORM_CAPABILITY_2_SOCKET    1
#define PLATFORM_CAPABILITY_4_SOCKET    2
#define PLATFORM_CAPABILITY_8_SOCKET    3

STATIC CHAR8 *PlatformCapabilitiesStr[] = {
  "Unknown",    // PLATFORM_CAPABILITY_UNDEFINED
  "2-Socket",   // PLATFORM_CAPABILITY_2_SOCKET
  "4-Socket",   // PLATFORM_CAPABILITY_4_SOCKET
  "8-Socket"    // PLATFORM_CAPABILITY_8_SOCKET
};

//TypeCooperCityRP
EFI_STATUS
GetPlatformCapabilitiesInfo (
  IN OUT UINT8                 *PlatformCapabilities,
  IN DYNAMIC_SI_LIBARY_PPI     *DynamicSiLibraryPpi
  );

EFI_STATUS
TypeCooperCityRPPlatformUpdateUsbOcMappings (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
TypeCooperCityRPPlatformUpdateAcpiTablePcds (
  VOID
);

EFI_STATUS
TypeCooperCityRPInstallGpioData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
) ;

EFI_STATUS
TypeCooperCityRPInstallClockgenData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
TypeCooperCityRPInstallPcdData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
TypeCooperCityRPInstallSoftStrapData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
TypeCooperCityRPIioPortBifurcationInit (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
TypeCooperCityRPInstallSlotTableData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);
EFI_STATUS
TypeCooperCityRPInstallKtiEparamData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi,
  IN UINT8                      PlatformCapabilities
);

EFI_STATUS
EFIAPI
TypeCooperCityRPPchEarlyUpdate(
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

#endif // _PEI_BOARD_INIT_PEIM_H_
