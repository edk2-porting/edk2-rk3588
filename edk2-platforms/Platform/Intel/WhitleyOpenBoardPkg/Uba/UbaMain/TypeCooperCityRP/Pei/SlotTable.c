/** @file
  Slot Table Update.

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiBoardInit.h"
#include <Library/UbaSlotUpdateLib.h>
#include <IioPlatformData.h>

#define PCI_DEVICE_ON_BOARD_TRUE 0
#define PCI_DEVICE_ON_BOARD_FALSE 1

typedef enum {
  Iio_Socket0 = 0,
  Iio_Socket1,
  Iio_Socket2,
  Iio_Socket3,
  Iio_Socket4,
  Iio_Socket5,
  Iio_Socket6,
  Iio_Socket7
} IIO_SOCKETS;

typedef enum {
  Iio_Iou0 =0,
  Iio_Iou1,
  Iio_Iou2,
  Iio_Mcp0,
  Iio_Mcp1,
  Iio_IouMax
} IIO_IOUS;

typedef enum {
  Bw5_Addr_0 = 0,
  Bw5_Addr_1,
  Bw5_Addr_2,
  Bw5_Addr_3,
  Bw5_Addr_Max
} BW5_ADDRESS;

static UINT8 TypeCooperCityRPPchPciSlotImpementedTableData[] = {
    PCI_DEVICE_ON_BOARD_FALSE,  // Root Port 0
    PCI_DEVICE_ON_BOARD_FALSE,  // Root Port 1
    PCI_DEVICE_ON_BOARD_FALSE,  // Root Port 2
    PCI_DEVICE_ON_BOARD_FALSE,  // Root Port 3
    PCI_DEVICE_ON_BOARD_TRUE,   // Root Port 4
    PCI_DEVICE_ON_BOARD_TRUE,   // Root Port 5
    PCI_DEVICE_ON_BOARD_FALSE,  // Root Port 6
    PCI_DEVICE_ON_BOARD_FALSE,  // Root Port 7
    PCI_DEVICE_ON_BOARD_FALSE,  // Root Port 8
    PCI_DEVICE_ON_BOARD_FALSE,  // Root Port 9
    PCI_DEVICE_ON_BOARD_FALSE,  // Root Port 10
    PCI_DEVICE_ON_BOARD_FALSE,  // Root Port 11
    PCI_DEVICE_ON_BOARD_FALSE,  // Root Port 12
    PCI_DEVICE_ON_BOARD_FALSE,  // Root Port 13
    PCI_DEVICE_ON_BOARD_FALSE,  // Root Port 14
    PCI_DEVICE_ON_BOARD_FALSE,  // Root Port 15
    PCI_DEVICE_ON_BOARD_FALSE,  // Root Port 16
    PCI_DEVICE_ON_BOARD_FALSE,  // Root Port 17
    PCI_DEVICE_ON_BOARD_FALSE,  // Root Port 18
    PCI_DEVICE_ON_BOARD_FALSE   // Root Port 19
};

UINT8
GetTypeCooperCityRPIOU0Setting (
  UINT8  IOU0Data
)
{
  //
  // Change bifurcation of Port1C-1D as x4x4 when QATGpio enabled.
  //
  IOU0Data = IIO_BIFURCATE_x4x4xxx8;
  return IOU0Data;
}

UINT8
GetTypeCooperCityRPIOU2Setting (
  UINT8  SkuPersonalityType,
  UINT8  IOU2Data
)
{
  return IOU2Data;
}

PLATFORM_SLOT_UPDATE_TABLE  TypeCooperCityRPSlotTable =
{
  PLATFORM_SLOT_UPDATE_SIGNATURE,
  PLATFORM_SLOT_UPDATE_VERSION,

  NULL,
  GetTypeCooperCityRPIOU0Setting,
  1
};

PLATFORM_SLOT_UPDATE_TABLE2  TypeCooperCityRPSlotTable2 =
{
  PLATFORM_SLOT_UPDATE_SIGNATURE,
  PLATFORM_SLOT_UPDATE_VERSION,

  NULL,
  GetTypeCooperCityRPIOU0Setting,
  1,
  GetTypeCooperCityRPIOU2Setting
};

PLATFORM_PCH_PCI_SLOT_IMPLEMENTED_UPDATE_TABLE TypeCooperCityRPPchPciSlotImplementedTable = {
  PLATFORM_SLOT_UPDATE_SIGNATURE,
  PLATFORM_SLOT_UPDATE_VERSION,

  TypeCooperCityRPPchPciSlotImpementedTableData
};

/**
  Entry point function for the PEIM

  @param FileHandle      Handle of the file being invoked.
  @param PeiServices     Describes the list of possible PEI Services.

  @return EFI_SUCCESS    If we installed our PPI

**/
EFI_STATUS
TypeCooperCityRPInstallSlotTableData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
)
{
  EFI_STATUS                         Status;

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformSlotDataGuid,
                                 &TypeCooperCityRPSlotTable,
                                 sizeof(TypeCooperCityRPSlotTable)
                                 );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformSlotDataGuid2,
                                 &TypeCooperCityRPSlotTable2,
                                 sizeof(TypeCooperCityRPSlotTable2)
                                 );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformPciSlotImplementedGuid,
                                 &TypeCooperCityRPPchPciSlotImplementedTable,
                                 sizeof(TypeCooperCityRPPchPciSlotImplementedTable)
                                 );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return Status;
}
