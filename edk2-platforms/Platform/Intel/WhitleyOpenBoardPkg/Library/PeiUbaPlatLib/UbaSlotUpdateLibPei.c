/** @file
  UbaSlotUpdateLib implementation.

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Uefi/UefiSpec.h>
#include <Ppi/UbaCfgDb.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/DebugLib.h>
#include <Library/UbaSlotUpdateLib.h>

EFI_STATUS
PlatformGetSlotTableData (
  IN OUT IIO_BROADWAY_ADDRESS_DATA_ENTRY  **BroadwayTable,
  IN OUT UINT8                            *IOU2Setting,
  IN OUT UINT8                            *FlagValue
)
{
  EFI_STATUS                        Status;
  UBA_CONFIG_DATABASE_PPI           *UbaConfigPpi = NULL;
  PLATFORM_SLOT_UPDATE_TABLE        IioSlotTable;
  UINTN                             TableSize;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  TableSize = sizeof(IioSlotTable);
  Status = UbaConfigPpi->GetData (
                                 UbaConfigPpi,
                                 &gPlatformSlotDataGuid,
                                 &IioSlotTable,
                                 &TableSize
                                 );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (IioSlotTable.Signature == PLATFORM_SLOT_UPDATE_SIGNATURE);
  ASSERT (IioSlotTable.Version == PLATFORM_SLOT_UPDATE_VERSION);

  *BroadwayTable = IioSlotTable.BroadwayTablePtr;
  *IOU2Setting   = IioSlotTable.GetIOU2Setting (*IOU2Setting);
  *FlagValue      = IioSlotTable.FlagValue;

  return Status;
}

EFI_STATUS
PlatformGetSlotTableData2 (
  IN OUT IIO_BROADWAY_ADDRESS_DATA_ENTRY  **BroadwayTable,
  IN OUT UINT8                            *IOU0Setting,
  IN OUT UINT8                            *FlagValue,
  IN OUT UINT8                            *IOU2Setting,
  IN     UINT8                            SkuPersonalityType
)
{
  EFI_STATUS                        Status;
  UBA_CONFIG_DATABASE_PPI           *UbaConfigPpi = NULL;
  PLATFORM_SLOT_UPDATE_TABLE2       IioSlotTable;
  UINTN                             TableSize;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  TableSize = sizeof(IioSlotTable);
  if ((SkuPersonalityType == 1) || (SkuPersonalityType == 3)) {
    Status = UbaConfigPpi->GetData (
                                 UbaConfigPpi,
                                 &gPlatformSlotDataGuid2_1,
                                 &IioSlotTable,
                                 &TableSize
                                 );
  } else {
    Status = UbaConfigPpi->GetData (
                                 UbaConfigPpi,
                                 &gPlatformSlotDataGuid2,
                                 &IioSlotTable,
                                 &TableSize
                                 );
  }
  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (IioSlotTable.Signature == PLATFORM_SLOT_UPDATE_SIGNATURE);
  ASSERT (IioSlotTable.Version == PLATFORM_SLOT_UPDATE_VERSION);

  *BroadwayTable = IioSlotTable.BroadwayTablePtr;
  *IOU0Setting   = IioSlotTable.GetIOU0Setting (*IOU0Setting);
  *FlagValue     = IioSlotTable.FlagValue;
  *IOU2Setting   = IioSlotTable.GetIOU2Setting (SkuPersonalityType, *IOU2Setting);

  return Status;
}

EFI_STATUS
PlatformPchGetPciSlotImplementedTableData (
  IN OUT UINT8                     **SlotImplementedTable
)
{
  EFI_STATUS                                      Status;
  UBA_CONFIG_DATABASE_PPI                         *UbaConfigPpi = NULL;
  PLATFORM_PCH_PCI_SLOT_IMPLEMENTED_UPDATE_TABLE  SITable;
  UINTN                                           TableSize;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              (VOID **)&UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  TableSize = sizeof(SITable);
  Status = UbaConfigPpi->GetData (
                                 UbaConfigPpi,
                                 &gPlatformPciSlotImplementedGuid,
                                 &SITable,
                                 &TableSize
                                 );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (SITable.Signature == PLATFORM_SLOT_UPDATE_SIGNATURE);
  ASSERT (SITable.Version == PLATFORM_SLOT_UPDATE_VERSION);

  *SlotImplementedTable = SITable.SlotImplementedTableDataPtr;

  return Status;
}
