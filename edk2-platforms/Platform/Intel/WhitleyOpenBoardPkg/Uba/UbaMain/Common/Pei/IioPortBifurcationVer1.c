/** @file
  Install Iio Port Bifurcation Init Data.
  This is depreciated version of the code used for PLATFORM_IIO_CONFIG_UPDATE_VERSION == 1

  @copyright
  Copyright 2020 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiCommonBoardInitLib.h"
#include <PiPei.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Guid/SetupVariable.h>
#include <Guid/PlatformInfo.h>
#include <Ppi/Smbus2.h>
#include <GpioInitData.h>
#include <Upi/KtiHost.h>
#include "IioBifurcationSlotTable.h"
#include <Library/GpioLib.h>
#include <Library/UbaPlatLib.h>
#include <Library/HobLib.h>
#include <Library/UbaIioPortBifurcationInitLib.h>
#include <Library/PchMultiPchBase.h>
#include <CpuAndRevisionDefines.h>
#include <Ppi/DynamicSiLibraryPpi.h>

/**
  Function returns the board ID from running HW

  @return BoardId
**/
UINT8
EFIAPI
SystemBoardIdValue (
  VOID
  )
{
  EFI_HOB_GUID_TYPE       *GuidHob;
  EFI_PLATFORM_INFO       *mPlatformInfo;

  GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return TypePlatformUnknown;
  }
  mPlatformInfo = GET_GUID_HOB_DATA (GuidHob);
  return mPlatformInfo->BoardId;
}


/**
  DVP/Neon City platform support BW5 bifurcation card in socket0 and socket1
  Lightning Ridge platform support BW5 bifurcation card in socket1, socket2 and socket3
  The bifurcation info stored at I/O Expander (PCA9555) which BIOS
  can get through Smbus read.

  PCA9555 SMBus slave Address: 0x4C

----------------------------------
  Neon
----------------------------------
    data0 = BW5 socket0 iio2
    data1 = BW5 socket1 iio0
    data2 = BW5 socket1 iio3
----------------------------------
  Lighting Ridge
----------------------------------
    data1 = BW5 socket1 iio2
    data2 = BW5 socket2 iio1
    data3 = BW5 socket3 iio3

  The bifurcation encoding is [2:0]:
  BW5_BIFURCATE_x4x4x4x4  x 0 0 0
  BW5_BIFURCATE_xxx8x4x4  x 0 0 1
  BW5_BIFURCATE_x4x4xxx8  x 0 1 0
  BW5_BIFURCATE_xxx8xxx8  x 0 1 1
  No BW5                  x 1 1 1

  @param IioGlobalData - Pointer to IioGlobals
  @param Bw5Id         - Pointer to BW5ID

  @retval None

**/
VOID
GetBw5Id (
  IN IIO_GLOBALS *IioGlobalData,
  IN  OUT BW5_BIFURCATION_DATA_STRUCT        *Bw5Id
  )
{
  CONST EFI_PEI_SERVICES    **PeiServices;
  EFI_STATUS                Status;
  EFI_SMBUS_DEVICE_ADDRESS  SmbusDeviceAddress;
  EFI_SMBUS_DEVICE_COMMAND  SmbusCommand;
  EFI_PEI_SMBUS2_PPI        *Smbus = NULL;
  UINT16                    SmbusData = 0;
  UINT8                     RetryCount;
  UINTN                     SmbusLength = 2;
  UINT8                     Index;

  PeiServices = GetPeiServicesTablePointer ();

  Status = (**PeiServices).LocatePpi (
                              PeiServices,
                              &gEfiPeiSmbus2PpiGuid,
                              0,
                              NULL,
                              &Smbus
                              );

  // Initialize Bw5Id to not present
  for (Index = 0; Index < BW5_CARDS_PRESENT; Index++){
    Bw5Id[Index].Data =  BW5_CARD_NOT_PRESENT;
  }

  if (Status != EFI_SUCCESS || Smbus == NULL) {
    DEBUG ((EFI_D_INFO, "!!!!Get SMBus protocol error %x\n", Status));
  } else {

    // Read Socket 0 HP Controller
    SmbusDeviceAddress.SmbusDeviceAddress = (BW5_SMBUS_ADDRESS >> 1);

    for (RetryCount = 0; RetryCount < NUM_OF_RETRIES; RetryCount++) {
      //
      // Read the current I/O pins Config for Port0
      //
      SmbusCommand = PCA9555_COMMAND_CONFIG_PORT0_REG;
      Status = Smbus->Execute (
                             Smbus,
                             SmbusDeviceAddress,
                             SmbusCommand,
                             EfiSmbusReadWord,
                             FALSE,
                             &SmbusLength,
                             &SmbusData
                             );
       if (!EFI_ERROR(Status)) {
        //
        // Configure the direction of I/O pins for Port0/Port1 as Input.
        //
        SmbusData = SmbusData | BW5_CONFIG_REG_MASK;
        Status = Smbus->Execute (
                             Smbus,
                             SmbusDeviceAddress,
                             SmbusCommand,
                             EfiSmbusWriteWord,
                             FALSE,
                             &SmbusLength,
                             &SmbusData
                             );
         if (!EFI_ERROR(Status)) {
           //
           // Read Input Port0/Port1 register to identify BW5 Id
           //
           SmbusCommand = PCA9555_COMMAND_INPUT_PORT0_REG;
           Status = Smbus->Execute( Smbus,
                               SmbusDeviceAddress,
                               SmbusCommand,
                               EfiSmbusReadWord,
                               FALSE,
                               &SmbusLength,
                               &SmbusData );
           if (!EFI_ERROR(Status)){
             DEBUG ((EFI_D_INFO, "SmbusData Port0/1 %x\n", SmbusData));
             //
             // Mask the Input Port0/1 register data [15:0] to get BW5 ID.
             //
             Bw5Id[0].Data = (SmbusData & BW5_0_ID_MASK);
             Bw5Id[1].Data = (SmbusData & BW5_1_ID_MASK) >> 4;
             Bw5Id[2].Data = (SmbusData & BW5_2_ID_MASK) >> 8;
             Bw5Id[3].Data = (SmbusData & BW5_3_ID_MASK) >> 12;
             break; // Break Loop if read was successfully.
          } // Read Port0
        } // Configure Port0
      } // Read Port0 Config
    } //RetryCount
  } // (Status != EFI_SUCCESS || Smbus == NULL)

  return;
}

EFI_STATUS
InternalPlatformGetSlotTableData2 (
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
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
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
InternalPlatformIioConfigInit2 (
  IN     UINT8                            SkuPersonalityType,
  IN OUT IIO_BIFURCATION_DATA_ENTRY       **BifurcationTable,
  IN OUT UINT8                            *BifurcationEntries,
  IN OUT IIO_SLOT_CONFIG_DATA_ENTRY       **SlotTable,
  IN OUT UINT8                            *SlotEntries
)
{
  EFI_STATUS                        Status = EFI_SUCCESS;
  UBA_CONFIG_DATABASE_PPI           *UbaConfigPpi = NULL;
  PLATFORM_IIO_CONFIG_UPDATE_TABLE  IioConfigTable;
  UINTN                             TableSize;

  Status = PeiServicesLocatePpi (
    &gUbaConfigDatabasePpiGuid,
    0,
    NULL,
    &UbaConfigPpi
    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  TableSize = sizeof(IioConfigTable);
  if (SkuPersonalityType == 1) {
    Status = UbaConfigPpi->GetData (
      UbaConfigPpi,
      &gPlatformIioConfigDataGuid_1,
      &IioConfigTable,
      &TableSize
      );
  } else if (SkuPersonalityType == 2) {
    Status = UbaConfigPpi->GetData (
      UbaConfigPpi,
      &gPlatformIioConfigDataGuid_2,
      &IioConfigTable,
      &TableSize
      );
  } else if (SkuPersonalityType == 3) {
    Status = UbaConfigPpi->GetData (
      UbaConfigPpi,
      &gPlatformIioConfigDataGuid_3,
      &IioConfigTable,
      &TableSize
      );
  } else {
    Status = UbaConfigPpi->GetData (
      UbaConfigPpi,
      &gPlatformIioConfigDataGuid,
      &IioConfigTable,
      &TableSize
      );
  }
  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (IioConfigTable.Signature == PLATFORM_IIO_CONFIG_UPDATE_SIGNATURE);
  ASSERT (IioConfigTable.Version == PLATFORM_IIO_CONFIG_UPDATE_VERSION);

  *BifurcationTable = IioConfigTable.IioBifurcationTablePtr;
  *BifurcationEntries = (UINT8) (IioConfigTable.IioBifurcationTableSize / sizeof(IIO_BIFURCATION_DATA_ENTRY));

  *SlotTable = IioConfigTable.IioSlotTablePtr;
  *SlotEntries = (UINT8) (IioConfigTable.IioSlotTableSize / sizeof(IIO_SLOT_CONFIG_DATA_ENTRY));

  return Status;
}

VOID
EnableHotPlug_SKX (
    IN OUT IIO_GLOBALS *IioGlobalData,
    IN UINT8 Port,
    IN UINT8 VppPort,
    IN UINT8 VppAddress,
    IN UINT8 PortOwnership
  )
{
  IioGlobalData->SetupData.SLOTHPCAP[Port]= ENABLE;
  IioGlobalData->SetupData.SLOTAIP[Port]  = ENABLE;  // Attention Indicator Present
  IioGlobalData->SetupData.SLOTPIP[Port]  = ENABLE;  // Power Indicator Present
  IioGlobalData->SetupData.SLOTMRLSP[Port]= ENABLE; // MRL Sensor Present
  IioGlobalData->SetupData.SLOTABP[Port]  = ENABLE; // Attention Button Present
  IioGlobalData->SetupData.SLOTPCP[Port]  = ENABLE; // Power Controlled Present

 if (PortOwnership ==  PCIEAIC_OCL_OWNERSHIP){
    IioGlobalData->SetupData.SLOTAIP[Port]  = DISABLE;  // Attention Indicator Present
    IioGlobalData->SetupData.SLOTPIP[Port]  = DISABLE;  // Power Indicator Present
  }
  if (PortOwnership ==  VMD_OWNERSHIP){
    IioGlobalData->SetupData.SLOTABP[Port]  = DISABLE;
    IioGlobalData->SetupData.SLOTPCP[Port]  = DISABLE;
    IioGlobalData->SetupData.SLOTMRLSP[Port]= DISABLE;
  }
  //
  // Set SLTCAP settings based on VMD/PCIe SSD Ownership
  //
  if ((PortOwnership == PCIEAIC_OCL_OWNERSHIP) ||
      (PortOwnership == VMD_OWNERSHIP)){
    IioGlobalData->SetupData.SLOTHPSUP[Port]= ENABLE;   // HotPlug Surprise
  }

  if (VppPort!= VPP_PORT_MAX) {
    IioGlobalData->SetupData.VppEnabled[Port]= TRUE;
    IioGlobalData->SetupData.VppPort[Port]= VppPort;
    IioGlobalData->SetupData.VppAddress[Port] = VppAddress;
  } else {
      DEBUG((EFI_D_ERROR, "PCIE HOT Plug. Missing VPP values on slot table\n"));
  }
}

/**
    Auto determine which PCIe Root port to be hidden if its
    lanes are assigned to its preceding root port...use the
    Setup option variable of ConfigIOU to determine which ports
    are to be hidden on each IOU for corresponding IIO

    @param IOUx                  - IOUx Index
    @param IioIndex              - Index to Iio
    @param IioGlobalData           Pointer to Iio Globals.
**/
VOID
CalculatePEXPHideFromIouBif_SKX (
  IN     UINT8        Iou,
  IN     UINT8        IioIndex,
  IN OUT IIO_GLOBALS *IioGlobalData
  )
{
  UINT8 *PXPHide, *HidePEXPMenu;
  UINT8 CurrentIOUConfigValue;
  UINT8 PXPOffset;
  UINT8 MaxPortNumberPerSocket;

  if (IioIndex >= MaxIIO || Iou >= NELEMENTS (IioGlobalData->SetupData.ConfigIOU[IioIndex])) {

    DEBUG ((DEBUG_ERROR, "[IIO] ERROR: %a: IIO instance %d or IOU %d out of range", __FUNCTION__, IioIndex, Iou));
    ASSERT (FALSE);
    return;
  }
  PXPHide = IioGlobalData->SetupData.PEXPHIDE;
  HidePEXPMenu = IioGlobalData->SetupData.HidePEXPMenu;
  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];

  PXPOffset = IioIndex * MaxPortNumberPerSocket;

  CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][Iou];
  switch (Iou) {
  case Iio_Iou0:
    PXPOffset += PORT_1A_INDEX;
    break;
  case Iio_Iou1:
    PXPOffset += PORT_2A_INDEX;
    break;
  case Iio_Iou2:
    PXPOffset += PORT_3A_INDEX;
    break;
  case Iio_Iou3:
    PXPOffset += PORT_4A_INDEX;
    break;
  case Iio_Iou4:
    PXPOffset += PORT_5A_INDEX;
    break;
  default:
    break;
  }
  switch (CurrentIOUConfigValue) {
    case IIO_BIFURCATE_xxxxxxxx:
      PXPHide[PXPOffset + Iio_PortA] = HIDE;            // hide A
      PXPHide[PXPOffset + Iio_PortB] = HIDE;            // hide B
      PXPHide[PXPOffset + Iio_PortC] = HIDE;            // hide C
      PXPHide[PXPOffset + Iio_PortD] = HIDE;            // hide D
      HidePEXPMenu[PXPOffset + Iio_PortA] = HIDE;       // hide the Setup menu for A
      HidePEXPMenu[PXPOffset + Iio_PortB] = HIDE;       // hide the Setup menu for B
      HidePEXPMenu[PXPOffset + Iio_PortC] = HIDE;       // hide the Setup menu for C
      HidePEXPMenu[PXPOffset + Iio_PortD] = HIDE;       // hide the Setup menu for D
      break;
    case IIO_BIFURCATE_x4x4xxx8:
      PXPHide[PXPOffset + Iio_PortA] = NOT_HIDE;        // show A
      PXPHide[PXPOffset + Iio_PortB] = HIDE;            // hide B
      PXPHide[PXPOffset + Iio_PortC] = NOT_HIDE;        // show C
      PXPHide[PXPOffset + Iio_PortD] = NOT_HIDE;        // show D
      HidePEXPMenu[PXPOffset + Iio_PortA] = NOT_HIDE;   // show the Setup menu for B
      HidePEXPMenu[PXPOffset + Iio_PortB] = HIDE;       // hide the Setup menu for B
      HidePEXPMenu[PXPOffset + Iio_PortC] = NOT_HIDE;   // show the Setup menu for D
      HidePEXPMenu[PXPOffset + Iio_PortD] = NOT_HIDE;   // show the Setup menu for B
      break;
    case IIO_BIFURCATE_xxx8x4x4:
      PXPHide[PXPOffset + Iio_PortA] = NOT_HIDE;        // show A
      PXPHide[PXPOffset + Iio_PortB] = NOT_HIDE;        // show B
      PXPHide[PXPOffset + Iio_PortC] = NOT_HIDE;        // show C
      PXPHide[PXPOffset + Iio_PortD] = HIDE;            // hide port D
      HidePEXPMenu[PXPOffset + Iio_PortA] = NOT_HIDE;   // show the Setup menu for A
      HidePEXPMenu[PXPOffset + Iio_PortB] = NOT_HIDE;   // show the Setup menu for B
      HidePEXPMenu[PXPOffset + Iio_PortC] = NOT_HIDE;   // show the Setup menu for C
      HidePEXPMenu[PXPOffset + Iio_PortD] = HIDE;       // hide the Setup menu for D
      break;
    case IIO_BIFURCATE_xxx8xxx8:
      PXPHide[PXPOffset + Iio_PortA] = NOT_HIDE;        // show A
      PXPHide[PXPOffset + Iio_PortB] = HIDE;            // hide B
      PXPHide[PXPOffset + Iio_PortC] = NOT_HIDE;        // show C
      PXPHide[PXPOffset + Iio_PortD] = HIDE;            // hide D
      HidePEXPMenu[PXPOffset + Iio_PortA] = NOT_HIDE;   // show the Setup menu for A
      HidePEXPMenu[PXPOffset + Iio_PortB] = HIDE;       // hide the Setup menu for B
      HidePEXPMenu[PXPOffset + Iio_PortC] = NOT_HIDE;   // show the Setup menu for C
      HidePEXPMenu[PXPOffset + Iio_PortD] = HIDE;       // hide the Setup menu for D
      break;
    case IIO_BIFURCATE_xxxxxx16:
      PXPHide[PXPOffset + Iio_PortA] = NOT_HIDE;        // show A
      PXPHide[PXPOffset + Iio_PortB] = HIDE;            // hide B
      PXPHide[PXPOffset + Iio_PortC] = HIDE;            // hide C
      PXPHide[PXPOffset + Iio_PortD] = HIDE;            // hide D
      HidePEXPMenu[PXPOffset + Iio_PortA] = NOT_HIDE;   // show the Setup menu for A
      HidePEXPMenu[PXPOffset + Iio_PortB] = HIDE;       // hide the Setup menu for B
      HidePEXPMenu[PXPOffset + Iio_PortC] = HIDE;       // hide the Setup menu for C
      HidePEXPMenu[PXPOffset + Iio_PortD] = HIDE;       // hide the Setup menu for D
      break;
    default:
      PXPHide[PXPOffset + Iio_PortA] = NOT_HIDE;        // show A
      PXPHide[PXPOffset + Iio_PortB] = NOT_HIDE;        // show B
      PXPHide[PXPOffset + Iio_PortC] = NOT_HIDE;        // show C
      PXPHide[PXPOffset + Iio_PortD] = NOT_HIDE;        // show port D
      HidePEXPMenu[PXPOffset + Iio_PortA] = NOT_HIDE;   // show the Setup menu for A
      HidePEXPMenu[PXPOffset + Iio_PortB] = NOT_HIDE;   // show the Setup menu for B
      HidePEXPMenu[PXPOffset + Iio_PortC] = NOT_HIDE;   // show the Setup menu for C
      HidePEXPMenu[PXPOffset + Iio_PortD] = NOT_HIDE;   // show the Setup menu for D
     break;
  }
  //
  // Change PEXPHIDE setting to hide all PCIe port of a IOU if IIO_BIFURCATE_xxxxxxxx is set.
  // And set ConfigIOU/ConfigMCPx to default bifucation control value
  // Bifurcation_Control[2:0] in IOU Bifurcation Control (PCIE_IOU_BIF_CTRL) register should be 000b ~ 100b.
  //
  if (CurrentIOUConfigValue == IIO_BIFURCATE_xxxxxxxx) {

    IioGlobalData->SetupData.ConfigIOU[IioIndex][Iou] = IIO_BIFURCATE_x4x4x4x4;
  }
}

VOID
DumpPort_SKX(
    IIO_GLOBALS *IioGlobalData,
    UINT8        Port,
    UINT8        NumberOfPorts
)
{
  UINT8 Index;
  DEBUG((EFI_D_INFO, "IDX, Port Hide, Slot Impl, Slot Number, HotPlug, PcieSSD, VppPort, VppAddress, Interlock\n"));
  for (Index = Port; Index < (Port + NumberOfPorts); Index++ ) {
  DEBUG((EFI_D_INFO, "%3d|   %2d    |    %2d    |   %3d      |   %3d  |  %3d  |  0x%02x  |  0x%02x     |  %2d      \n", \
                       Index, \
                       IioGlobalData->SetupData.PEXPHIDE[Index],  \
                       IioGlobalData->SetupData.SLOTIMP[Index],   \
                       IioGlobalData->SetupData.SLOTPSP[Index],   \
                       IioGlobalData->SetupData.SLOTHPCAP[Index], \
                       IioGlobalData->IioVar.IioOutData.PciePortOwnership[Index],   \
                       IioGlobalData->SetupData.VppPort[Index],   \
                       IioGlobalData->SetupData.VppAddress[Index],\
                       IioGlobalData->SetupData.SLOTEIP[Index]));
  }
}


/// Dump iio configuration. Dump the current IIO configuration to the serial
/// log.
VOID
DumpIioConfiguration_SKX(
    IN UINT8 iio,
    IN IIO_GLOBALS *IioGlobalData
)
{
  UINT8 Iou;
  UINT8 PortIndex;
  UINT8 MaxPortNumberPerSocket;
  UINT8 Bifurcation;
  UINT8 IouPorts;

  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[iio];
  PortIndex = iio * MaxPortNumberPerSocket;
  /// First dump the socket number;
  DEBUG((EFI_D_INFO, "Socket number: %d \n", iio));

  /// Dump DMI configuration:
  if ((iio == 0) && (PortIndex == 0)){
      DEBUG((EFI_D_INFO, "PORT 0: DMI Port\n"));
  } else {
      DEBUG((EFI_D_INFO, "PORT 0: DMI Port working as PCIE\n"));
      DumpPort_SKX(IioGlobalData, PortIndex, 1);
  }
  IouPorts=4;
  /// Dump IOU bifurcations:
  for (Iou = Iio_Iou0; Iou< Iio_IouMax; Iou ++) {
      /// Reset port index.
      PortIndex = iio * MaxPortNumberPerSocket;
      // Get the bifurcation
      switch (Iou) {
        case Iio_Iou0:
          Bifurcation = IioGlobalData->SetupData.ConfigIOU[iio][0];
          PortIndex += PORT_1A_INDEX;
          DEBUG((EFI_D_INFO, "IUO0: Root Port 1, Bifurcation: %d\n", Bifurcation));
          break;
        case Iio_Iou1:
          Bifurcation = IioGlobalData->SetupData.ConfigIOU[iio][1];
          PortIndex += PORT_2A_INDEX;
          DEBUG((EFI_D_INFO, "IUO1: Root Port 2, Bifurcation: %d\n", Bifurcation));
          break;
        case Iio_Iou2:
          Bifurcation = IioGlobalData->SetupData.ConfigIOU[iio][2];
          PortIndex += PORT_3A_INDEX;
          DEBUG((EFI_D_INFO, "IUO2: Root Port 3, Bifurcation: %d\n", Bifurcation));
          break;
        case Iio_Iou3:
          Bifurcation = IioGlobalData->SetupData.ConfigIOU[iio][3];
          PortIndex += PORT_4A_INDEX;
          DEBUG((EFI_D_INFO, "IOU3, Bifurcation: %d\n", Bifurcation));
          break;
        case Iio_Iou4:
          Bifurcation = IioGlobalData->SetupData.ConfigIOU[iio][4];
          PortIndex += PORT_5A_INDEX;
          DEBUG((EFI_D_INFO, "IOU4, Bifurcation: %d\n", Bifurcation));
          break;
        default:
          DEBUG((EFI_D_INFO, "Iou no detected = %d",Iou));
          break;
        }
      DumpPort_SKX(IioGlobalData, PortIndex, IouPorts);
  }
}


VOID
SystemHideIioPortsCommon_SKX(
    IIO_GLOBALS *IioGlobalData,
    UINT8       IioIndex
  )
{
    CalculatePEXPHideFromIouBif_SKX(Iio_Iou0, IioIndex, IioGlobalData);
    CalculatePEXPHideFromIouBif_SKX(Iio_Iou1, IioIndex, IioGlobalData);
    CalculatePEXPHideFromIouBif_SKX(Iio_Iou2, IioIndex, IioGlobalData);
    CalculatePEXPHideFromIouBif_SKX(Iio_Iou3, IioIndex, IioGlobalData);
    CalculatePEXPHideFromIouBif_SKX(Iio_Iou4, IioIndex, IioGlobalData);
    DumpIioConfiguration_SKX(IioIndex, IioGlobalData);
}

/**
    Verify if and Slot should be implemented based on IOUX bifurcation settings.

    @param IioGlobalData           Pointer to Iio Globals.
    @param Port                  - Port Index

    @retval TRUE/FALSE         to determine if an slot should be implemented or not
                               based on the IOUX bifurcation settings in case user want to do an
                               override and VMD is enabled.

**/
BOOLEAN
SlotImplemented_SKX (
    IN OUT IIO_GLOBALS        *IioGlobalData,
    IN UINT8                  Port
  )
{
  UINT8 IioIndex;
  UINT8 MaxPortNumberPerSocket;
  UINT8 PortIndex;
  UINT8 Stack;
  BOOLEAN  SlotImp = FALSE;

  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[0];
  IioIndex = Port/MaxPortNumberPerSocket;
  PortIndex = (Port - (MaxPortNumberPerSocket * IioIndex));
  Stack = IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex][PortIndex];
  DEBUG ((DEBUG_INFO, "SlotImplemented_SKX:IioIndex = %x, Stack = %x, Port = %x, PortIndex =%x\n", IioIndex, Stack, Port, PortIndex));

  switch (Stack) {
    case IIO_PSTACK0:
      if (IioGlobalData->SetupData.ConfigIOU[IioIndex][0] == IIO_BIFURCATE_x4x4x4x4) {
         SlotImp = TRUE;
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][0] == IIO_BIFURCATE_x4x4xxx8){
        if ((PortIndex == PORT_1D_INDEX) || (PortIndex == PORT_1C_INDEX) || (PortIndex == PORT_1A_INDEX)) {
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][0] == IIO_BIFURCATE_xxx8x4x4) {
        if ((PortIndex == PORT_1C_INDEX) || (PortIndex == PORT_1B_INDEX) || (PortIndex == PORT_1A_INDEX)) {
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][0] == IIO_BIFURCATE_xxx8xxx8) {
        if ((PortIndex == PORT_1C_INDEX) || (PortIndex == PORT_1A_INDEX)){
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][0] == IIO_BIFURCATE_xxxxxx16) {
        if (PortIndex == PORT_1A_INDEX) {
          SlotImp = TRUE;
        }
      }
      break;
    case IIO_PSTACK1:
      if (IioGlobalData->SetupData.ConfigIOU[IioIndex][1] == IIO_BIFURCATE_x4x4x4x4) {
         SlotImp = TRUE;
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][1] == IIO_BIFURCATE_x4x4xxx8) {
        if ((PortIndex == PORT_2D_INDEX) || (PortIndex == PORT_2C_INDEX) || (PortIndex == PORT_2A_INDEX)) {
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][1] == IIO_BIFURCATE_xxx8x4x4){
        if ((PortIndex == PORT_2C_INDEX) || (PortIndex == PORT_2B_INDEX) || (PortIndex == PORT_2A_INDEX)) {
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][1] == IIO_BIFURCATE_xxx8xxx8) {
        if ((PortIndex == PORT_2C_INDEX) || (PortIndex == PORT_2A_INDEX)) {
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][1] == IIO_BIFURCATE_xxxxxx16) {
        if (PortIndex == PORT_2A_INDEX) {
          SlotImp = TRUE;
        }
      }
      break;
    case IIO_PSTACK2:
      if (IioGlobalData->SetupData.ConfigIOU[IioIndex][2] == IIO_BIFURCATE_x4x4x4x4){
         SlotImp = TRUE;
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][2] == IIO_BIFURCATE_x4x4xxx8) {
        if ((PortIndex == PORT_3D_INDEX) || (PortIndex == PORT_3C_INDEX) || (PortIndex == PORT_3A_INDEX)) {
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][2] == IIO_BIFURCATE_xxx8x4x4) {
        if ((PortIndex == PORT_3C_INDEX) || (PortIndex == PORT_3B_INDEX) || (PortIndex == PORT_3A_INDEX)) {
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][2] == IIO_BIFURCATE_xxx8xxx8) {
        if ((PortIndex == PORT_3C_INDEX) || (PortIndex == PORT_3A_INDEX)){
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][2] == IIO_BIFURCATE_xxxxxx16) {
        if (PortIndex == PORT_3A_INDEX) {
          SlotImp = TRUE;
        }
      }
      break;
    case IIO_PSTACK3:
      if (IioGlobalData->SetupData.ConfigIOU[IioIndex][3] == IIO_BIFURCATE_x4x4x4x4){
         SlotImp = TRUE;
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][3] == IIO_BIFURCATE_x4x4xxx8) {
        if ((PortIndex == PORT_4D_INDEX) || (PortIndex == PORT_4C_INDEX) || (PortIndex == PORT_4A_INDEX)) {
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][3] == IIO_BIFURCATE_xxx8x4x4) {
        if ((PortIndex == PORT_4C_INDEX) || (PortIndex == PORT_4B_INDEX) || (PortIndex == PORT_4A_INDEX)) {
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][3] == IIO_BIFURCATE_xxx8xxx8) {
        if ((PortIndex == PORT_4C_INDEX) || (PortIndex == PORT_4A_INDEX)){
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][3] == IIO_BIFURCATE_xxxxxx16) {
        if (PortIndex == PORT_4A_INDEX) {
          SlotImp = TRUE;
        }
      }
      break;
    case IIO_PSTACK4:
      if (IioGlobalData->SetupData.ConfigIOU[IioIndex][4] == IIO_BIFURCATE_x4x4x4x4){
         SlotImp = TRUE;
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][4] == IIO_BIFURCATE_x4x4xxx8) {
        if ((PortIndex == PORT_5D_INDEX) || (PortIndex == PORT_5C_INDEX) || (PortIndex == PORT_5A_INDEX)) {
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][4] == IIO_BIFURCATE_xxx8x4x4) {
        if ((PortIndex == PORT_5C_INDEX) || (PortIndex == PORT_5B_INDEX) || (PortIndex == PORT_5A_INDEX)) {
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][4] == IIO_BIFURCATE_xxx8xxx8) {
        if ((PortIndex == PORT_5C_INDEX) || (PortIndex == PORT_5A_INDEX)){
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU[IioIndex][4] == IIO_BIFURCATE_xxxxxx16) {
        if (PortIndex == PORT_5A_INDEX) {
          SlotImp = TRUE;
        }
      }
      break;
    default:
      DEBUG ((EFI_D_INFO, "default case.\n"));  //Auto added. Please review.
      break;
  }
  DEBUG ((DEBUG_INFO, "SlotImplemented_SKX:  = %x\n", SlotImp));
  return SlotImp;
}


VOID
ConfigSlots_SKX (
    IN OUT IIO_GLOBALS        *IioGlobalData,
    IN IIO_SLOT_CONFIG_DATA_ENTRY  *Slot,
    IN UINT8                  SlotEntries
  )
{
  UINT8 Index;
  UINT8 Port;

  for (Index =0; Index < SlotEntries; Index ++) {
    Port=Slot[Index].PortIndex;
    if (Slot[Index].Hidden != NOT_HIDE) {
      IioGlobalData->SetupData.HidePEXPMenu[Port] = HIDE;
      IioGlobalData->SetupData.PEXPHIDE[Port]= HIDE;
    }
    /// Check if slot is assigned.
    if (Slot[Index].SlotNumber!= NO_SLT_IMP){
       IioGlobalData->SetupData.SLOTIMP[Port]= SLT_IMP;
       IioGlobalData->SetupData.SLOTPSP[Port]=Slot[Index].SlotNumber;
       IioGlobalData->SetupData.SLOTEIP[Port]=Slot[Index].InterLockPresent;
       if (Slot[Index].SlotPowerLimitScale!= PWR_SCL_MAX) {
         IioGlobalData->SetupData.SLOTSPLS[Port] = Slot[Index].SlotPowerLimitScale;
         IioGlobalData->SetupData.SLOTSPLV[Port] = Slot[Index].SlotPowerLimitValue;
       }
       if (Slot[Index].HotPlugCapable != DISABLE) {
         EnableHotPlug_SKX(IioGlobalData, Port, Slot[Index].VppPort, Slot[Index].VppAddress, REGULAR_PCIE_OWNERSHIP);
       }
    }
  }
}


EFI_STATUS
PlatformUpdateIioConfig (
  IN  IIO_GLOBALS             *IioGlobalData
)
{
  EFI_STATUS                        Status;
  UBA_CONFIG_DATABASE_PPI           *UbaConfigPpi = NULL;
  PLATFORM_IIO_CONFIG_UPDATE_TABLE  IioConfigTable;
  UINTN                             TableSize;

  Status = PeiServicesLocatePpi (
    &gUbaConfigDatabasePpiGuid,
    0,
    NULL,
    &UbaConfigPpi
    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  TableSize = sizeof(IioConfigTable);
  Status = UbaConfigPpi->GetData (
    UbaConfigPpi,
    &gPlatformIioConfigDataGuid,
    &IioConfigTable,
    &TableSize
    );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (IioConfigTable.Signature == PLATFORM_IIO_CONFIG_UPDATE_SIGNATURE);
  ASSERT (IioConfigTable.Version == PLATFORM_IIO_CONFIG_UPDATE_VERSION);

  Status = IioConfigTable.CallUpdate (IioGlobalData);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return Status;
}

VOID
OverrideDefaultBifSlots_SKX (
    IN IIO_GLOBALS *IioGlobalData,
    IN UINT8       BoardId,
    IN BOOLEAN     *AutoBifEnable
)
{
  EFI_STATUS                      Status;
  UINT32                          QATGpio;
  PCIE_RISER_ID                   RightRiser;
  PCIE_RISER_ID                   LeftRiser;
  UINT32                          RiserBit;
  UINT8                           Index;
  BW5_BIFURCATION_DATA_STRUCT     Bw5id[4]= {{{0,0,0,0,0}}, {{0,0,0,0,0,}}, {{0,0,0,0,0}}, {{0,0,0,0,0}}}; // Default, no BW card.
  IIO_BROADWAY_ADDRESS_ENTRY      *BroadwayTable;
  IIO_BROADWAY_ADDRESS_DATA_ENTRY *BroadwayTableTemp;
  UINT8                           IOU0Setting;
  UINT8                           IOU2Setting;
  UINT8                           FlagValue = 0;
  UINT8                           SkuPersonality = 0;
  DYNAMIC_SI_LIBARY_PPI           *DynamicSiLibraryPpi = NULL;

  BroadwayTable = NULL;
  IOU0Setting = IioGlobalData->SetupData.ConfigIOU[Iio_Socket0][0];
  IOU2Setting = IioGlobalData->SetupData.ConfigIOU[Iio_Socket0][2];

  //
  // Specific platform overrides.
  //
  // Changes because GPIO (QAT, Riser Cards, etc).
  // Read QAT and riser card GPIOs.
  //
  // Purley platforms need to read the QAT bit
  //
  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  Status = DynamicSiLibraryPpi->GpioGetInputValueByPchId (PCH_LEGACY_ID, GPIO_SKL_H_GPP_B3, &QATGpio);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "Get GPIO_SKL_H_GPP_B3 Failed\n"));
    return;
  }
  DEBUG ((EFI_D_INFO, "QAT GPIO: %d\n", QATGpio));

  if ((IioGlobalData->IioVar.IioVData.SkuPersonality[0] == TYPE_FPGA) &&\
      (IioGlobalData->IioVar.IioVData.SkuPersonality[1] == TYPE_FPGA)) {
    SkuPersonality = 1;
  } else if ((IioGlobalData->IioVar.IioVData.SkuPersonality[0] == TYPE_FPGA) &&\
      (IioGlobalData->IioVar.IioVData.SkuPersonality[1] != TYPE_FPGA)) {
    SkuPersonality = 2;
  } else if ((IioGlobalData->IioVar.IioVData.SkuPersonality[0] != TYPE_FPGA) &&\
      (IioGlobalData->IioVar.IioVData.SkuPersonality[1] == TYPE_FPGA)) {
    SkuPersonality = 3;
  } else {
    SkuPersonality = 0;
  }
  DEBUG((DEBUG_INFO, "SKU Personality Type: %d\n", SkuPersonality));

  BroadwayTableTemp = (IIO_BROADWAY_ADDRESS_DATA_ENTRY *) BroadwayTable;
  InternalPlatformGetSlotTableData2 (&BroadwayTableTemp, &IOU0Setting, &FlagValue, &IOU2Setting, SkuPersonality);
  BroadwayTable    = (IIO_BROADWAY_ADDRESS_ENTRY *)BroadwayTableTemp; // if no platform definition, BroadwayTable will be NULL

  if (AutoBifEnable[(Iio_Socket0 * Iio_IouMax) + Iio_Iou2] == TRUE) {
    IioGlobalData->SetupData.ConfigIOU[Iio_Socket0][2] = IOU2Setting;
  }

  if (FlagValue == 1) {
    //
    // Right riser
    //
    Status = DynamicSiLibraryPpi->GpioGetInputValueByPchId (PCH_LEGACY_ID, GPIO_SKL_H_GPP_B4, &RiserBit);  // PresentSignal
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "Get GPIO_SKL_H_GPP_B4 Failed\n"));
      return;
    }
    RightRiser.Bits.PresentSignal = (UINT8) RiserBit;

    Status = DynamicSiLibraryPpi->GpioGetInputValueByPchId (PCH_LEGACY_ID, GPIO_SKL_H_GPP_C15, &RiserBit);  // HotPlugConf
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "Get GPIO_SKL_H_GPP_C15 Failed\n"));
      return;
    }
    RightRiser.Bits.HPConf = (UINT8) RiserBit;

    Status = DynamicSiLibraryPpi->GpioGetInputValueByPchId (PCH_LEGACY_ID, GPIO_SKL_H_GPP_C16, &RiserBit);  // WingConf
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "Get GPIO_SKL_H_GPP_C16 Failed\n"));
      return;
    }
    RightRiser.Bits.WingConf = (UINT8) RiserBit;

    Status = DynamicSiLibraryPpi->GpioGetInputValueByPchId (PCH_LEGACY_ID, GPIO_SKL_H_GPP_C17, &RiserBit);  // Slot9En
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "Get GPIO_SKL_H_GPP_C17 Failed\n"));
      return;
    }
    RightRiser.Bits.Slot9En = (UINT8) RiserBit;

    DEBUG ((EFI_D_INFO, "GPIO Right riser information: PresentSignal=%x, HotPlugConf=%x, WingConf=%x, Slot9En=%x\n",
            RightRiser.Bits.PresentSignal, RightRiser.Bits.HPConf, RightRiser.Bits.WingConf, RightRiser.Bits.Slot9En));

    //
    // Left riser
    //
    Status = DynamicSiLibraryPpi->GpioGetInputValueByPchId (PCH_LEGACY_ID, GPIO_SKL_H_GPP_B5, &RiserBit);  // PresentSignal
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "Get GPIO_SKL_H_GPP_B5 Failed\n"));
      return;
    }
    LeftRiser.Bits.PresentSignal = (UINT8) RiserBit;

    Status = DynamicSiLibraryPpi->GpioGetInputValueByPchId (PCH_LEGACY_ID, GPIO_SKL_H_GPP_C18, &RiserBit);  // HotPlugConf
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "Get GPIO_SKL_H_GPP_C18 Failed\n"));
      return;
    }
    LeftRiser.Bits.HPConf = (UINT8) RiserBit;

    Status = DynamicSiLibraryPpi->GpioGetInputValueByPchId (PCH_LEGACY_ID, GPIO_SKL_H_GPP_C19, &RiserBit);  // WingConf
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "Get GPIO_SKL_H_GPP_C19 Failed\n"));
      return;
    }
    LeftRiser.Bits.WingConf = (UINT8) RiserBit;

    Status = DynamicSiLibraryPpi->GpioGetInputValueByPchId (PCH_LEGACY_ID, GPIO_SKL_H_GPP_B21, &RiserBit);  // Slot9En
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "Get GPIO_SKL_H_GPP_B21 Failed\n"));
      return;
    }
    LeftRiser.Bits.Slot9En = (UINT8) RiserBit;

    DEBUG ((EFI_D_INFO, "GPIO Left riser information: PresentSignal=%x, HotPlugConf=%x, WingConf=%x, Slot9En=%x\n",
            LeftRiser.Bits.PresentSignal, LeftRiser.Bits.HPConf, LeftRiser.Bits.WingConf, LeftRiser.Bits.Slot9En));
  }

  if (QATGpio == QAT_ENABLED) {
    // So Configuration of IUO0 is:
    //  1A-1B - QAT xxx8
    //  1C    - SSD x4
    //  1D    - SSD x4
    if (AutoBifEnable[(Iio_Socket0 * Iio_IouMax) + Iio_Iou0] == TRUE) {
      IioGlobalData->SetupData.ConfigIOU[Iio_Socket0][0] = IOU0Setting;
    }
  }

  if (FlagValue == 1) {
    if (QATGpio != QAT_ENABLED) {
      if ((RightRiser.Bits.Slot9En == RISER_SLOT9_DISABLE) &&
          (LeftRiser.Bits.Slot9En == RISER_SLOT9_DISABLE)) {
        //
        // SLOT 9 is disabled. SSDs are present.
        // Change configuration to x4x4x4x4.
        //
        if (AutoBifEnable[(Iio_Socket0 * Iio_IouMax) + Iio_Iou0] == TRUE) {
          IioGlobalData->SetupData.ConfigIOU[Iio_Socket0][0]  = IIO_BIFURCATE_x4x4x4x4;
          IioGlobalData->IioVar.IioOutData.PciePortOwnership[SOCKET_0_INDEX + PORT_1A_INDEX] = PCIEAIC_OCL_OWNERSHIP;
          IioGlobalData->IioVar.IioOutData.PciePortOwnership[SOCKET_0_INDEX + PORT_1B_INDEX] = PCIEAIC_OCL_OWNERSHIP;
          IioGlobalData->IioVar.IioOutData.PciePortOwnership[SOCKET_0_INDEX + PORT_1C_INDEX] = PCIEAIC_OCL_OWNERSHIP;
          IioGlobalData->IioVar.IioOutData.PciePortOwnership[SOCKET_0_INDEX + PORT_1D_INDEX] = PCIEAIC_OCL_OWNERSHIP;
        }
      } else if (RightRiser.Bits.PresentSignal == RISER_PRESENT) {
        //
        // Slot 9 is enabled. Keep the xxxxxx16 configuration (default) and
        // enable slot 9. Slot 9 supports HP.
        //
        IioGlobalData->SetupData.SLOTIMP[SOCKET_0_INDEX + PORT_1A_INDEX] = 1;
        IioGlobalData->SetupData.SLOTPSP[SOCKET_0_INDEX + PORT_1A_INDEX] = 9;
      } // End of RISER_PRESENT
    } // End of QAT_ENABLED

    if (RightRiser.Bits.PresentSignal == RISER_PRESENT) {
      IioGlobalData->SetupData.SLOTIMP[SOCKET_0_INDEX + PORT_3A_INDEX] = 1;
#if MAX_SOCKET > 2
      IioGlobalData->SetupData.SLOTIMP[SOCKET_3_INDEX + PORT_2A_INDEX] = 1;
      IioGlobalData->SetupData.SLOTIMP[SOCKET_3_INDEX + PORT_3A_INDEX] = 1;
#endif // MAX_SOCKET > 2
      if (RightRiser.Bits.WingConf == RISER_WINGED_IN) {
        IioGlobalData->SetupData.SLOTPSP[SOCKET_0_INDEX + PORT_3A_INDEX] = 1;
#if MAX_SOCKET > 2
        IioGlobalData->SetupData.SLOTPSP[SOCKET_3_INDEX + PORT_2A_INDEX] = 4;
        IioGlobalData->SetupData.SLOTPSP[SOCKET_3_INDEX + PORT_3A_INDEX] = 2;
#endif // MAX_SOCKET > 2
      } else {  // RISER_WINGED_OUT
        IioGlobalData->SetupData.SLOTPSP[SOCKET_0_INDEX + PORT_3A_INDEX] = 2;
#if MAX_SOCKET > 2
        IioGlobalData->SetupData.SLOTPSP[SOCKET_3_INDEX + PORT_2A_INDEX] = 3;
        IioGlobalData->SetupData.SLOTPSP[SOCKET_3_INDEX + PORT_3A_INDEX] = 1;
#endif // MAX_SOCKET > 2
        if (RightRiser.Bits.HPConf == RISER_HP_EN) {
          //
          // PCIe Hot Plug is supported on Winged-out riser only
          //
          EnableHotPlug_SKX(IioGlobalData, SOCKET_0_INDEX + PORT_3A_INDEX, VPP_PORT_0, 0x40, REGULAR_PCIE_OWNERSHIP);
#if MAX_SOCKET > 2
          EnableHotPlug_SKX(IioGlobalData, SOCKET_3_INDEX + PORT_2A_INDEX, VPP_PORT_1, 0x40, REGULAR_PCIE_OWNERSHIP);
          EnableHotPlug_SKX(IioGlobalData, SOCKET_3_INDEX + PORT_3A_INDEX, VPP_PORT_0, 0x40, REGULAR_PCIE_OWNERSHIP);
#endif // MAX_SOCKET > 2
        } // End of RISER_HP_EN
      } // End of RISER_WINGED_IN
    } // End of RISER_PRESENT

    if (LeftRiser.Bits.PresentSignal == RISER_PRESENT) {
      IioGlobalData->SetupData.SLOTIMP[SOCKET_1_INDEX + PORT_1A_INDEX] = 1;
      IioGlobalData->SetupData.SLOTIMP[SOCKET_1_INDEX + PORT_2A_INDEX] = 1;
      IioGlobalData->SetupData.SLOTIMP[SOCKET_2_INDEX + PORT_1A_INDEX] = 1;
      IioGlobalData->SetupData.SLOTIMP[SOCKET_2_INDEX + PORT_3A_INDEX] = 1;
      if (LeftRiser.Bits.WingConf == RISER_WINGED_IN) {
        IioGlobalData->SetupData.SLOTPSP[SOCKET_1_INDEX + PORT_1A_INDEX] = 7;
        IioGlobalData->SetupData.SLOTPSP[SOCKET_1_INDEX + PORT_2A_INDEX] = 5;
        IioGlobalData->SetupData.SLOTPSP[SOCKET_2_INDEX + PORT_1A_INDEX] = 6;
        IioGlobalData->SetupData.SLOTPSP[SOCKET_2_INDEX + PORT_3A_INDEX] = 8;
      } else {  // RISER_WINGED_OUT
        IioGlobalData->SetupData.SLOTPSP[SOCKET_1_INDEX + PORT_1A_INDEX] = 5;
        IioGlobalData->SetupData.SLOTPSP[SOCKET_1_INDEX + PORT_2A_INDEX] = 7;
        IioGlobalData->SetupData.SLOTPSP[SOCKET_2_INDEX + PORT_1A_INDEX] = 8;
        IioGlobalData->SetupData.SLOTPSP[SOCKET_2_INDEX + PORT_3A_INDEX] = 6;
        if (LeftRiser.Bits.HPConf == RISER_HP_EN) {
          //
          // PCIe Hot Plug is supported on Winged-out riser only
          //
          EnableHotPlug_SKX(IioGlobalData, SOCKET_1_INDEX + PORT_1A_INDEX, VPP_PORT_0, 0x42, REGULAR_PCIE_OWNERSHIP);
          EnableHotPlug_SKX(IioGlobalData, SOCKET_1_INDEX + PORT_2A_INDEX, VPP_PORT_1, 0x42, REGULAR_PCIE_OWNERSHIP);
          EnableHotPlug_SKX(IioGlobalData, SOCKET_2_INDEX + PORT_1A_INDEX, VPP_PORT_1, 0x42, REGULAR_PCIE_OWNERSHIP);
          EnableHotPlug_SKX(IioGlobalData, SOCKET_2_INDEX + PORT_3A_INDEX, VPP_PORT_0, 0x42, REGULAR_PCIE_OWNERSHIP);
        } // End of RISER_HP_EN
      } // End of RISER_WINGED_IN
    } // End of RISER_PRESENT
  } // End of FlagValue == 1

  /// Broadway overrides.
  if (BroadwayTable != NULL) {
    GetBw5Id (IioGlobalData, Bw5id);
    DEBUG ((EFI_D_INFO,"Broadway Config: 0x%02x, 0x%02x, 0x%02x, 0x%02x\n", Bw5id[Bw5_Addr_0].Data, Bw5id[Bw5_Addr_1].Data, Bw5id[Bw5_Addr_2].Data, Bw5id[Bw5_Addr_3].Data));
    for (Index = 0; Index < 3; Index ++) {
      //
      // Check if BW5 is present before override IOUx Bifurcation
      //
      if (BroadwayTable->BroadwayAddress == Bw5_Addr_Max) {
        break;
      }
      if (Bw5id[BroadwayTable->BroadwayAddress].Bits.BifBits != BW5_CARD_NOT_PRESENT){
        if (AutoBifEnable[(BroadwayTable->Socket * Iio_IouMax) + BroadwayTable->IouNumber] == TRUE) {
          switch (BroadwayTable->IouNumber) {
            case Iio_Iou0:
              IioGlobalData->SetupData.ConfigIOU[BroadwayTable->Socket][0] = Bw5id[BroadwayTable->BroadwayAddress].Bits.BifBits;
              DEBUG ((DEBUG_ERROR,"IioGlobalData->SetupData.ConfigIOU[%x][0] = %x\n",BroadwayTable->Socket, IioGlobalData->SetupData.ConfigIOU[BroadwayTable->Socket][0]));
              break;
            case Iio_Iou1:
              IioGlobalData->SetupData.ConfigIOU[BroadwayTable->Socket][1] = Bw5id[BroadwayTable->BroadwayAddress].Bits.BifBits;
              DEBUG ((DEBUG_ERROR,"IioGlobalData->SetupData.ConfigIOU[%x][1] = %x\n",BroadwayTable->Socket, IioGlobalData->SetupData.ConfigIOU[BroadwayTable->Socket][1]));
              break;
            case Iio_Iou2:
              IioGlobalData->SetupData.ConfigIOU[BroadwayTable->Socket][2] = Bw5id[BroadwayTable->BroadwayAddress].Bits.BifBits;
              DEBUG ((DEBUG_ERROR,"IioGlobalData->SetupData.ConfigIOU[%x][2] = %x\n",BroadwayTable->Socket,IioGlobalData->SetupData.ConfigIOU[BroadwayTable->Socket][2]));
              break;
            default:
             break;
          } // BroadwayTable->IouNumber
        } // AutoBifEnable == TRUE
     } // No BW5 present
      BroadwayTable ++;
    } // for Index
  } // BroadwayTable != NULL

  if (DynamicSiLibraryPpi->IsCpuAndRevision (CPU_SKX, REV_ALL) ||
      DynamicSiLibraryPpi->IsCpuAndRevision (CPU_CLX, REV_ALL) ||
      DynamicSiLibraryPpi->IsCpuAndRevision (CPU_CPX, REV_ALL)
      ) {
    PlatformUpdateIioConfig (IioGlobalData);
  }
}


/**
    Verify if VMD is enabled and override Slot configuration
    based on the VMD settings

    @param IioGlobalData           Pointer to Iio Globals.
    @param Slot                  - Slot configuration settings
    @param SlotEntries           - Number of slot entries

    @retval None

**/
VOID
OverrideConfigSlots_SKX (
    IN OUT IIO_GLOBALS        *IioGlobalData,
    IN IIO_SLOT_CONFIG_DATA_ENTRY  *Slot,
    IN UINT8                  SlotEntries
  )
{
  UINT8 Index;
  UINT8 Port;
  UINT8 MaxPortNumberPerSocket;
  UINT8 IioIndex;
  UINT8 Stack;
  UINT8 PortIndex;

  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[0];
  for (Index =0; Index < SlotEntries; Index ++) {
    Port = Slot[Index].PortIndex;
    //
    // Check if Slot is capable of PcieSSD Solution and override the SLOT Config values
    //
    if (Slot[Index].PcieSSDCapable) {
      IioIndex = Port/MaxPortNumberPerSocket;
      PortIndex = (Port - (MaxPortNumberPerSocket * IioIndex));
      Stack = IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex][PortIndex];
      DEBUG ((DEBUG_INFO, "Stack = %x, Port = %x, PortIndex = %x\n", Stack, Port, PortIndex));

      //
      // check if VMD will own Pcie Root Port
      //
      if (IioGlobalData->SetupData.VMDEnabled[IioIndex][Stack]) {
        if (IioGlobalData->SetupData.VMDPortEnable[IioIndex][PortIndex]) {
          IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port] = VMD_OWNERSHIP;
        }
      } else {

        DEBUG ((DEBUG_INFO, "IioGlobalData->SetupData.PcieAICEnabled[%x] = %x\n", Stack, IioGlobalData->SetupData.PcieAICEnabled[(IioIndex * MAX_STACKS_PER_SOCKET) + Stack]));
        //
        // Check if Pcie AIC Card will be present on Pcie Root Port
        //
        if (IioGlobalData->SetupData.PcieAICEnabled[(IioIndex * MAX_STACKS_PER_SOCKET) + Stack]) {
          //
          // Force to have this port enabled by default for hot-plug.
          //
          IioGlobalData->SetupData.IioPcieConfig.PciePortEnable[(IioIndex * MaxPortNumberPerSocket) + Port] = ENABLE;
          IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port] = PCIEAIC_OCL_OWNERSHIP;
          DEBUG ((DEBUG_ERROR, "Port = %x, PciePortEnable = %x\n", Port, IioGlobalData->SetupData.IioPcieConfig.PciePortEnable[(IioIndex * MaxPortNumberPerSocket) + Port]));
        }
      } // No _VMD Ownership

      DEBUG ((DEBUG_INFO, "PciePortOwnerShip[%x] = %x\n",Port, IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port]));

      // if PcieSSDSupport required do slot override settings accordingly
      if ((IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port] != REGULAR_PCIE_OWNERSHIP) &&
          (SlotImplemented_SKX(IioGlobalData, Port) == TRUE)) {
        IioGlobalData->SetupData.SLOTIMP[Port] = SLT_IMP;
        IioGlobalData->SetupData.SLOTPSP[Port] = 0x50  + Port; // Just program a value for PCIEACI_OCL/VMD
        IioGlobalData->SetupData.SLOTEIP[Port] = DISABLE;

        if (Slot[Index].SlotPowerLimitScale != PWR_SCL_MAX) {
          IioGlobalData->SetupData.SLOTSPLS[Port] = Slot[Index].SlotPowerLimitScale;
          IioGlobalData->SetupData.SLOTSPLV[Port] = Slot[Index].SlotPowerLimitValue;
        }
        DEBUG ((DEBUG_INFO,"Slot[Index].PcieSSDVppPort = %x\n", Slot[Index].PcieSSDVppPort));
        // Enable hot-plug if slot/port supports it
        if (Slot[Index].PcieSSDVppPort != VPP_PORT_MAX) {
        DEBUG ((DEBUG_INFO, "IioGlobalData->SetupData.VMDHotPlugEnable[%d][%x] = %x\n", IioIndex, Stack, IioGlobalData->SetupData.VMDHotPlugEnable[IioIndex][Stack]));
        DEBUG ((DEBUG_INFO, "IioGlobalData->SetupData.PcieAICHotPlugEnable[%x] = %x\n",Stack,IioGlobalData->SetupData.PcieAICHotPlugEnable[(IioIndex * MAX_STACKS_PER_SOCKET) + Stack]));
        // Check if hot-plug is enabled for VMD or PCIeAIC case.
          if (((IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port] == VMD_OWNERSHIP) &&
               (IioGlobalData->SetupData.VMDHotPlugEnable[IioIndex][Stack])) ||
              ((IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port] == PCIEAIC_OCL_OWNERSHIP) &&
               (IioGlobalData->SetupData.PcieAICHotPlugEnable[(IioIndex * MAX_STACKS_PER_SOCKET) + Stack]))) {
            EnableHotPlug_SKX(IioGlobalData, Port, Slot[Index].PcieSSDVppPort, Slot[Index].PcieSSDVppAddress, IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port]);
            DEBUG((DEBUG_INFO,"Enable HotPlug Done\n"));
          }
        }
        //
        // Unhide the port in order to get configured and it will be hide later for VMDLateSetup if MD own the Pcie Root Port
        //
        IioGlobalData->SetupData.PEXPHIDE[Port] = NOT_HIDE;
      }// PcieSSDSupport
    }// PcieSSDCapable
  }// Per Slot
}

VOID
SetBifurcations_SKX(
    IN OUT IIO_GLOBALS *IioGlobalData,
    IN     IIO_BIFURCATION_DATA_ENTRY *BifurcationTable,
    IN     UINT8                  BifurcationEntries,
    IN     BOOLEAN                *AutoBifEnable
)
{
  UINT8 Socket;
  UINT8 Iou;
  UINT8 Index;

  for (Index = 0; Index < BifurcationEntries ; Index++) {
    Socket = BifurcationTable[Index].Socket;
    Iou = BifurcationTable[Index].IouNumber;

    if (AutoBifEnable[(Socket * Iio_IouMax) + Iou] == TRUE) {
      switch (Iou) {
        case Iio_Iou0:
          IioGlobalData->SetupData.ConfigIOU[Socket][0] = BifurcationTable[Index].Bifurcation;
          break;
        case Iio_Iou1:
          IioGlobalData->SetupData.ConfigIOU[Socket][1] = BifurcationTable[Index].Bifurcation;
          break;
        case Iio_Iou2:
          IioGlobalData->SetupData.ConfigIOU[Socket][2] = BifurcationTable[Index].Bifurcation;
          break;
        case Iio_Iou3:
          IioGlobalData->SetupData.ConfigIOU[Socket][3] = BifurcationTable[Index].Bifurcation;
          break;
        case Iio_Iou4:
          IioGlobalData->SetupData.ConfigIOU[Socket][4] = BifurcationTable[Index].Bifurcation;
          break;
        default:
          DEBUG ((EFI_D_ERROR, "Invalid bifurcation table: Bad Iou (%d)", Iou));
          ASSERT(Iou);
          break;
      }
    }
  }
}


/**

  SystemIioPortBifurcationInit - Program the UDS data structure with OEM IIO init values
  for SLOTs and Bifurcation.

  @param mSB - pointer to this protocol
  @param IioUds - Pointer to the IIO UDS datastructure.

  @retval EFI_SUCCESS

**/
VOID
SystemIioPortBifurcationInitCommon_SKX (
    UINT8 BoardId,
    IIO_GLOBALS *IioGlobalData,
    IIO_BIFURCATION_DATA_ENTRY    **BifurcationTable,
    UINT8                         *BifurcationEntries,
    IIO_SLOT_CONFIG_DATA_ENTRY    **SlotTable,
    UINT8                         *SlotEntries
)
{

  UINT8                         PortIndex;//, iio;
  IIO_BIFURCATION_DATA_ENTRY    **BifurcationTableTemp;
  IIO_SLOT_CONFIG_DATA_ENTRY    **SlotTableTemp;
  UINT8                         SkuPersonalityType = 0;
  UINT8                         MaxPortNumberPerSocket;

  /// This function outline:
  //// 1 Based on platform apply the default bifurcation and slot configuration.
  //// 2 Apply dynamic overrides based on GPIO and other configurations.
  //// 3 Hide unused ports due bifurcation.

  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[0];
  for (PortIndex = 0; PortIndex < MAX_SOCKET*MaxPortNumberPerSocket; PortIndex++) {
    IioGlobalData->SetupData.PEXPHIDE[PortIndex] = 0;
    IioGlobalData->SetupData.HidePEXPMenu[PortIndex] = 0;
  }

  *BifurcationEntries = 0;
  *SlotEntries = 0;

  BifurcationTableTemp = (IIO_BIFURCATION_DATA_ENTRY **) BifurcationTable;
  SlotTableTemp        = (IIO_SLOT_CONFIG_DATA_ENTRY **) SlotTable;

  if ((IioGlobalData->IioVar.IioVData.SkuPersonality[0] == TYPE_FPGA) &&\
      (IioGlobalData->IioVar.IioVData.SkuPersonality[1] == TYPE_FPGA)) {
    SkuPersonalityType = 1;
  } else if ((IioGlobalData->IioVar.IioVData.SkuPersonality[0] == TYPE_FPGA) &&\
      (IioGlobalData->IioVar.IioVData.SkuPersonality[1] != TYPE_FPGA)) {
    SkuPersonalityType = 2;
  } else if ((IioGlobalData->IioVar.IioVData.SkuPersonality[0] != TYPE_FPGA) &&\
      (IioGlobalData->IioVar.IioVData.SkuPersonality[1] == TYPE_FPGA)) {
    SkuPersonalityType = 3;
  } else {
    SkuPersonalityType = 0;
  }

  InternalPlatformIioConfigInit2 (SkuPersonalityType, BifurcationTableTemp, BifurcationEntries, SlotTableTemp, SlotEntries);

  BifurcationTable = (IIO_BIFURCATION_DATA_ENTRY **) BifurcationTableTemp;
  SlotTable        = (IIO_SLOT_CONFIG_DATA_ENTRY **) SlotTableTemp;

}


/**
  This function prepare the data for silicon initialization based on
  bifuraction and slots table

  This function is for tables in version PLATFORM_IIO_CONFIG_UPDATE_VERSION = 1
 */
VOID
IioPortBifurcationInitVer1 (
  IN IIO_GLOBALS   *IioGlobalData
  )
{
  UINT8                         IioIndex;
  IIO_BIFURCATION_DATA_ENTRY    *BifurcationTable = NULL;
  UINT8                         BifurcationEntries;
  IIO_SLOT_CONFIG_DATA_ENTRY    *SlotTable = NULL;
  UINT8                         SlotEntries;
  BOOLEAN                       AutoBifEnable[MaxIIO * Iio_IouMax];
  UINT8                         BoardId;

  BoardId = SystemBoardIdValue();

  SetMem ((VOID *)AutoBifEnable, MaxIIO * Iio_IouMax, FALSE);

  for (IioIndex = Iio_Socket0; IioIndex < MaxIIO; IioIndex++) {
    if (IioGlobalData->SetupData.ConfigIOU[IioIndex][0] == IIO_BIFURCATE_AUTO) {
      AutoBifEnable[(IioIndex * Iio_IouMax) + Iio_Iou0] = TRUE;
    }

    if (IioGlobalData->SetupData.ConfigIOU[IioIndex][1] == IIO_BIFURCATE_AUTO) {
      AutoBifEnable[(IioIndex * Iio_IouMax) + Iio_Iou1] = TRUE;
    }

    if (IioGlobalData->SetupData.ConfigIOU[IioIndex][2] == IIO_BIFURCATE_AUTO) {
      AutoBifEnable[(IioIndex * Iio_IouMax) + Iio_Iou2] = TRUE;
    }

    if (IioGlobalData->SetupData.ConfigIOU[IioIndex][3] == IIO_BIFURCATE_AUTO) {
      AutoBifEnable[(IioIndex * Iio_IouMax) + Iio_Iou3] = TRUE;
    }

    if (IioGlobalData->SetupData.ConfigIOU[IioIndex][4] == IIO_BIFURCATE_AUTO) {
      AutoBifEnable[(IioIndex * Iio_IouMax) + Iio_Iou4] = TRUE;
    }
  }

  // This function outline:
  // 1. Based on platform apply the default bifurcation and slot configuration.
  // 2. Apply dynamic overrides based on GPIO and other configurations.
  // 3. Hide unused ports due bifurcation.

  // Set the default bifurcations for this platform.
  SystemIioPortBifurcationInitCommon_SKX (BoardId, IioGlobalData, &BifurcationTable, &BifurcationEntries, &SlotTable, &SlotEntries);
  SetBifurcations_SKX (IioGlobalData, BifurcationTable, BifurcationEntries, AutoBifEnable);
  ConfigSlots_SKX (IioGlobalData, SlotTable, SlotEntries);
  if (BoardId <= TypePlatformMax) {
    OverrideDefaultBifSlots_SKX (IioGlobalData, BoardId, AutoBifEnable);
  }
  OverrideConfigSlots_SKX (IioGlobalData, SlotTable, SlotEntries);


  // All overrides have been applied now.
  // Hide root ports whose lanes are assigned preceding ports.
  for (IioIndex = Iio_Socket0; IioIndex < MaxIIO; IioIndex++) {
    if (IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
      SystemHideIioPortsCommon_SKX (IioGlobalData, IioIndex);
    }
  }
}


