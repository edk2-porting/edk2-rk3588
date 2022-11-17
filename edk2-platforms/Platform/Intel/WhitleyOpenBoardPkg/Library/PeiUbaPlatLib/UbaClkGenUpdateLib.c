/** @file
  UbaClkGenUpdateLib implementation.

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Uefi/UefiSpec.h>

#include <Ppi/UbaCfgDb.h>
#include <Ppi/Smbus2.h>

#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/DebugLib.h>

#include <Library/UbaClkGenUpdateLib.h>

#define CLOCK_GENERATOR_ADDRESS     0xD2

EFI_STATUS
PlatformUpdateClockgen (
  IN  BOOLEAN                           EnableSpreadSpectrum
)
{
  EFI_STATUS                            Status;

  UBA_CONFIG_DATABASE_PPI               *UbaConfigPpi = NULL;
  PLATFORM_CLOCKGEN_UPDATE_TABLE        ClockgenTable;
  UINTN                                 TableSize = 0;

  EFI_SMBUS_DEVICE_ADDRESS              SlaveAddress;
  UINT8                                 Buffer[PLATFORM_NUMBER_OF_CLOCKGEN_DATA];
  UINTN                                 Length = 0;
  EFI_SMBUS_DEVICE_COMMAND              Command;
  EFI_PEI_SMBUS2_PPI                    *SmbusPpi = NULL;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = PeiServicesLocatePpi (
              &gEfiPeiSmbus2PpiGuid,
              0,
              NULL,
              &SmbusPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Read the clock generator
  //
  SlaveAddress.SmbusDeviceAddress = CLOCK_GENERATOR_ADDRESS >> 1;
  Length      = sizeof (Buffer);
  Command     = 0;
  Status = SmbusPpi->Execute (
                      SmbusPpi,
                      SlaveAddress,
                      Command,
                      EfiSmbusReadBlock,
                      FALSE,
                      &Length,
                      Buffer
                      );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  do {

    TableSize = sizeof(ClockgenTable);
    Status = UbaConfigPpi->GetData (
                                  UbaConfigPpi,
                                  &gPlatformClockgenConfigDataGuid,
                                  &ClockgenTable,
                                  &TableSize
                                  );

    if (EFI_ERROR(Status)) {
      return Status;
    }

    ASSERT (ClockgenTable.Signature == PLATFORM_CLOCKGEN_UPDATE_SIGNATURE);
    ASSERT (ClockgenTable.Version == PLATFORM_CLOCKGEN_UPDATE_VERSION);

    if (ClockgenTable.Id != PLATFORM_CLOCKGEN_NO_ID) {
      if (ClockgenTable.Id != Buffer[ClockgenTable.IdOffset]) {
        continue;
      }
    }

    if (EnableSpreadSpectrum) {
      ClockgenTable.Data[ClockgenTable.SpreadSpectrumByteOffset] = ClockgenTable.SpreadSpectrumValue;
    }

    //
    // Program clock generator
    //
    Command = 0;
    Length  = ClockgenTable.DataLength;

    Status = SmbusPpi->Execute (
                        SmbusPpi,
                        SlaveAddress,
                        Command,
                        EfiSmbusWriteBlock,
                        FALSE,
                        &Length,              //&ConfigurationTableLength,
                        &ClockgenTable.Data   //ConfigurationTable
                        );
    if (EFI_ERROR(Status)) {
      return Status;
    } else {
      break;
    }
  }while (!EFI_ERROR(Status));

  return Status;
}


