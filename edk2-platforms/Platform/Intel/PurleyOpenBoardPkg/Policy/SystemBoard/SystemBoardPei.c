/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SystemBoardPei.h"

#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include <IioBifurcationSlotTable.h>

extern IIO_BIFURCATION_ENTRY   mIioBifurcationTable[];
extern UINT8 mIioBifurcationTableEntries;
extern IIO_SLOT_CONFIG_ENTRY   mIioSlotTable[];
extern UINT8 mIioSlotTableEntries;

//
// System board PPI structure
//
static SYSTEM_BOARD_PPI                mSystemBoardPpi = {
  SystemIioPortBifurcationInit,        // Set IIO Bifurcation ports configuration
  GetUplinkPortInformation,
};

static EFI_PEI_PPI_DESCRIPTOR       mSystemBoardPpiDesc = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gEfiPeiSystemBoardPpiGuid,
  &mSystemBoardPpi
};

/**

  GetUplinkPortInformation - Get uplink port information

  @param IioIndex - socket ID.

  @retval PortIndex for uplink port

**/
UINT8
EFIAPI
GetUplinkPortInformation (
    IN UINT8 IioIndex
)
{
  UINT8 UplinkPortIndex;

  UplinkPortIndex =  GetUplinkPortInformationCommon(IioIndex);
  return UplinkPortIndex;
}

/**

  SystemIioPortBifurcationInit - Program the UDS data structure with OEM IIO init values
  for SLOTs and Bifurcation.

  @param mSB - pointer to this protocol
  @param IioUds - Pointer to the IIO UDS datastructure.

  @retval EFI_SUCCESS

**/
VOID
InternalSystemIioPortBifurcationInitCommon (
  IN OUT IIO_GLOBALS                   *IioGlobalData,
  IN OUT IIO_BIFURCATION_ENTRY         **BifurcationTable,
  IN OUT UINT8                         *BifurcationEntries,
  IN OUT IIO_SLOT_CONFIG_ENTRY         **SlotTable,
  IN OUT UINT8                         *SlotEntries
)
{

  UINT8                         PortIndex;//, iio;
  
  /// This function outline:
  //// 1 Based on platform apply the default bifurcation and slot configuration.
  //// 2 Apply dynamic overrides based on GPIO and other configurations.
  //// 3 Hide unused ports due bifurcation.

  for (PortIndex = 0; PortIndex < MAX_SOCKET*NUMBER_PORTS_PER_SOCKET; PortIndex++) {
    IioGlobalData->SetupData.PEXPHIDE[PortIndex] = 0;
    IioGlobalData->SetupData.HidePEXPMenu[PortIndex] = 0;
  }

  *BifurcationEntries = 0;
  *SlotEntries = 0;

  // Purley Intel boards are not Multi-PCH
  IioGlobalData->IioVar.IioVData.MultiPch = 0;

  *BifurcationTable   = (IIO_BIFURCATION_ENTRY *)(UINTN)PcdGet64 (PcdIioBifurcationTable);
  *BifurcationEntries = PcdGet8 (PcdIioBifurcationTableEntries);
  *SlotTable          = (IIO_SLOT_CONFIG_ENTRY *)(UINTN)PcdGet64 (PcdIioSlotTable);
  *SlotEntries        = PcdGet8 (PcdIioSlotTableEntries);
}

/**

  SystemIioPortBifurcationInit - Program the IIO_GLOBALS data structure with OEM IIO init values
  for SLOTs and Bifurcation.

  @param mSB - pointer to this protocol
  @param IioUds - Pointer to the IIO UDS datastructure.

  @retval EFI_SUCCESS

**/
VOID
SystemIioPortBifurcationInit (
    IN IIO_GLOBALS *IioGlobalData
)
{

  UINT8                         IioIndex;
  IIO_BIFURCATION_ENTRY         *BifurcationTable = NULL;
  UINT8                         BifurcationEntries;
  IIO_SLOT_CONFIG_ENTRY         *SlotTable = NULL;
  UINT8                         SlotEntries;

  // This function outline:
  // 1. Based on platform apply the default bifurcation and slot configuration.
  // 2. Apply dynamic overrides based on GPIO and other configurations.
  // 3. Hide unused ports due bifurcation.

  SystemIioPortBifurcationInitCommon(IioGlobalData, &BifurcationTable, &BifurcationEntries, &SlotTable, &SlotEntries);
  /// Set the default bifurcations for this platform.
  SetBifurcations(IioGlobalData, BifurcationTable, BifurcationEntries);
  ConfigSlots(IioGlobalData, SlotTable, SlotEntries);
  OverrideConfigSlots(IioGlobalData, SlotTable, SlotEntries);

  // All overrides have been applied now.
  // Hide root ports whose lanes are assigned preceding ports.
  for (IioIndex = Iio_Socket0; IioIndex < MaxIIO; IioIndex++) {
    if (IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
      SystemHideIioPortsCommon(IioGlobalData, IioIndex);
    }
  }
}


/**

  This function dump raw data.

  @param  Data  raw data
  @param  Size  raw data size

**/
VOID
InternalDumpData (
  IN UINT8  *Data,
  IN UINTN  Size
  )
{
  UINTN  Index;
  for (Index = 0; Index < Size; Index++) {
    DEBUG ((EFI_D_INFO, "%02x", (UINTN)Data[Index]));
  }
}

/**

  This function dump raw data with colume format.

  @param  Data  raw data
  @param  Size  raw data size

**/
VOID
InternalDumpHex (
  IN UINT8  *Data,
  IN UINTN  Size
  )
{
  UINTN   Index;
  UINTN   Count;
  UINTN   Left;

#define COLUME_SIZE  (16 * 2)

  Count = Size / COLUME_SIZE;
  Left  = Size % COLUME_SIZE;
  for (Index = 0; Index < Count; Index++) {
    DEBUG ((EFI_D_INFO, "%04x: ", Index * COLUME_SIZE));
    InternalDumpData (Data + Index * COLUME_SIZE, COLUME_SIZE);
    DEBUG ((EFI_D_INFO, "\n"));
  }

  if (Left != 0) {
    DEBUG ((EFI_D_INFO, "%04x: ", Index * COLUME_SIZE));
    InternalDumpData (Data + Index * COLUME_SIZE, Left);
    DEBUG ((EFI_D_INFO, "\n"));
  }
}

VOID
DumpConfig (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "PcdSetupData - 0x%x\n", PcdGetSize (PcdSetupData)));
  InternalDumpHex (PcdGetPtr (PcdSetupData), PcdGetSize (PcdSetupData));
  DEBUG ((DEBUG_INFO, "PcdPchRcConfigurationData - 0x%x\n", PcdGetSize (PcdPchRcConfigurationData)));
  InternalDumpHex (PcdGetPtr (PcdPchRcConfigurationData), PcdGetSize (PcdPchRcConfigurationData));
  DEBUG ((DEBUG_INFO, "PcdSocketIioConfigData - 0x%x\n", PcdGetSize (PcdSocketIioConfigData)));
  InternalDumpHex (PcdGetPtr (PcdSocketIioConfigData), PcdGetSize (PcdSocketIioConfigData));
  DEBUG ((DEBUG_INFO, "PcdSocketCommonRcConfigData - 0x%x\n", PcdGetSize (PcdSocketCommonRcConfigData)));
  InternalDumpHex (PcdGetPtr (PcdSocketCommonRcConfigData), PcdGetSize (PcdSocketCommonRcConfigData));
  DEBUG ((DEBUG_INFO, "PcdSocketMpLinkConfigData - 0x%x\n", PcdGetSize (PcdSocketMpLinkConfigData)));
  InternalDumpHex (PcdGetPtr (PcdSocketMpLinkConfigData), PcdGetSize (PcdSocketMpLinkConfigData));
  DEBUG ((DEBUG_INFO, "PcdSocketMemoryConfigData - 0x%x\n", PcdGetSize (PcdSocketMemoryConfigData)));
  InternalDumpHex (PcdGetPtr (PcdSocketMemoryConfigData), PcdGetSize (PcdSocketMemoryConfigData));
  DEBUG ((DEBUG_INFO, "PcdSocketPowerManagementConfigData - 0x%x\n", PcdGetSize (PcdSocketPowerManagementConfigData)));
  InternalDumpHex (PcdGetPtr (PcdSocketPowerManagementConfigData), PcdGetSize (PcdSocketPowerManagementConfigData));
  DEBUG ((DEBUG_INFO, "PcdSocketProcessorCoreConfigData - 0x%x\n", PcdGetSize (PcdSocketProcessorCoreConfigData)));
  InternalDumpHex (PcdGetPtr (PcdSocketProcessorCoreConfigData), PcdGetSize (PcdSocketProcessorCoreConfigData));
}

//
// PEI entry point - SystemBoardPpi entry point
//
/**

  PEI system board PPI intialization main entry point. This will setup up a PPI that will handle providing system board level
  configuration for the platform.

  @param FileHandle         Pointer to the PEIM FFS file header.
  @param PeiServices       General purpose services available to every PEIM.

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval Otherwise         System board initialization failed.
**/
EFI_STATUS
EFIAPI
SystemBoardPeiEntry (
  IN EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{
  EFI_STATUS Status;

  DEBUG ((EFI_D_ERROR, "--> SystemBoard PEI BoardDetection\n"));

  //DumpConfig ();

  //
  // Initialize system board information PPI
  //
  Status = PeiServicesInstallPpi(&mSystemBoardPpiDesc);
  ASSERT_EFI_ERROR (Status);
  return Status;
}
