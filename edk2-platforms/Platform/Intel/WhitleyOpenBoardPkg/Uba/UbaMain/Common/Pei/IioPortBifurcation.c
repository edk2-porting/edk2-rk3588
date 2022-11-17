/** @file
  Install Iio Port Bifurcation Init Data.

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <PiPei.h>
#include <Upi/KtiHost.h>

#include <Guid/SetupVariable.h>
#include <Guid/PlatformInfo.h>
#include <Ppi/Smbus2.h>
#include <GpioInitData.h>

#include <Library/PeiServicesTablePointerLib.h>
#include <Library/GpioLib.h>
#include <Library/UbaIioConfigLib.h>
#include <Library/UbaPlatLib.h>
#include <Library/HobLib.h>
#include <Library/UbaIioPortBifurcationInitLib.h>
#include <Library/PchMultiPchBase.h>
#include <CpuAndRevisionDefines.h>

#include <Ppi/DynamicSiLibraryPpi.h>

#include "PeiCommonBoardInitLib.h"
#include "IioBifurcationSlotTable.h"


#define IIO_D_UBALOG(...)  { DEBUG((DEBUG_ERROR, "[IIO](UBA) " __VA_ARGS__)); }          // Important log always printed
#define IIO_D_UBADBG(...)  { DEBUG((DEBUG_INFO,  "[IIO](UBA) " __VA_ARGS__)); }          // Debug log, printed with BIOS debug jumper
#define IIO_D_UBAWARN(...) { DEBUG((DEBUG_ERROR, "[IIO](UBA) WARNING: " __VA_ARGS__)); } // Warning log, not necessary error
#define IIO_D_UBAERR(...)  { DEBUG((DEBUG_ERROR, "[IIO](UBA) ERROR: " __VA_ARGS__)); }   // Errorneous situation, probably bug or hw problem

EFI_PLATFORM_INFO*
GetPlatformInfoFromHob (
  VOID
  )
{
  EFI_HOB_GUID_TYPE               *GuidHob;

  GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  if (GuidHob == NULL) {
      ASSERT (FALSE);
      return NULL;
    }
  return GET_GUID_HOB_DATA (GuidHob);
}

/**
  Check if QAT-UPLINK override is needed

  @param[in]  Socket       Socket index
  @param[in]  Iou          IOU index
  @param[out] Bifurcation  Buffer for bifurcation needed to support QAT

  @return TRUE if custom bifurcation is necessary
 */
BOOLEAN
IsQatUplinkOverrideNeeded (
  IN  UINT8                       Socket,
  IN  UINT8                       Iou,
  IN  DYNAMIC_SI_LIBARY_PPI       *DynamicSiLibraryPpi,
  OUT UINT8                       *Bifurcation
  )
{
  EFI_PLATFORM_INFO               *PlatformInfo;
  UINT32                          Gpio;
  EFI_STATUS                      Status;

  PlatformInfo = GetPlatformInfoFromHob ();
  if (PlatformInfo == NULL) {
    return FALSE;
  }

  switch (PlatformInfo->BoardId) {
  case TypeWilsonCityRP:
  case TypeWilsonCityModular:
  case TypeWilsonCitySMT:
  case TypeWilsonCityPPV:
  case TypeWilsonPointRP:
  case TypeWilsonPointModular:
  case TypeCooperCityRP:
    if (Socket %2 == 0 && Iou == 3) {
      //
      // It is possible that special bifurcation for QAT-UPLINK if needed
      // It can be recognized by GPIO D8: 0 means that there is no UPLINK
      //
      Status = DynamicSiLibraryPpi->GpioGetInputValueByPchId (PCH_LEGACY_ID, GPIO_SKL_H_GPP_D8, &Gpio);
      if (EFI_ERROR (Status)) {
        return FALSE;
      }
      if (Gpio != 0) {
        *Bifurcation = IIO_BIFURCATE_xxx8xxx8;
        return TRUE;
      } else {
        return FALSE;
      }
    }
    break;
  }

  //
  // By default there is no QAT-UPLINK support
  //
  return FALSE;
}

/**
  Check if QAT-CABLE override is needed

  @param[in]  Socket       Socket index
  @param[in]  Iou          IOU index
  @param[out] Bifurcation  Buffer for bifurcation needed to support QAT

  @return TRUE if custom bifurcation is necessary
 */
BOOLEAN
IsQatCableOverrideNeeded (
  IN  UINT8                       Socket,
  IN  UINT8                       Iou,
  IN  DYNAMIC_SI_LIBARY_PPI       *DynamicSiLibraryPpi,
  OUT UINT8                       *Bifurcation
  )
{
  EFI_PLATFORM_INFO               *PlatformInfo;
  UINT32                          Gpio;
  EFI_STATUS                      Status;

  PlatformInfo = GetPlatformInfoFromHob ();
  if (PlatformInfo == NULL) {
    return FALSE;
  }

  switch (PlatformInfo->BoardId) {
  case TypeWilsonCityRP:
  case TypeWilsonCityModular:
  case TypeWilsonCitySMT:
  case TypeWilsonCityPPV:
  case TypeWilsonPointRP:
  case TypeWilsonPointModular:
    if (Iou == 4) {
      //
      // It is possible that special bifurcation for Oculink QAT cable is needed
      // Then cable is used to connect oculinks ports with QAT
      // To determine it if cable is needed GPIO-B3 needs to be examined
      // GPIO-B3 == 0 means that cable is present
      //
      Status = DynamicSiLibraryPpi->GpioGetInputValueByPchId (PCH_LEGACY_ID, GPIO_SKL_H_GPP_B3, &Gpio);
      if (EFI_ERROR (Status)) {
        return FALSE;
      }

      if (Gpio != 0) {
        //
        // There is no cable
        //
        return FALSE;
      }

      //
      // Cable is present to check which MEZZ IOU is used GPIO B4 needs to be examined
      // GPIO-B4 == 1: MEZZ IOU from Socket0 is used;
      // GPIO-B4 == 0: MEZZ IOU from Socket1 is used;
      //
      Status = DynamicSiLibraryPpi->GpioGetInputValueByPchId (PCH_LEGACY_ID, GPIO_SKL_H_GPP_B4, &Gpio);
      if (EFI_ERROR (Status)) {
        return FALSE;
      }

      if ((Socket == 0 && Gpio == 1) || (Socket == 1 && Gpio == 0)) {
        *Bifurcation = IIO_BIFURCATE_xxx8x4x4;
        return TRUE;
      }
    }
  } //switch: Board-Id

  //
  // By default there is no QAT-cable support
  //
  return FALSE;
}

/**
  Check if QAT override is needed.
  There are two possible QAT variants:
    - QAT uplink
    - QAT cable
  If LBG with QAT is detected (by GPIO read) custom bifurcation should be set.

  @param[in]  IioGlobalData  Pointer to IioGlobalData
 */
VOID
CheckQatBifurcationOverrides (
  IN  IIO_GLOBALS                 *IioGlobalData,
  IN  DYNAMIC_SI_LIBARY_PPI       *DynamicSiLibraryPpi
  )
{
  UINT8 Socket;
  UINT8 Iou;
  UINT8 Bifurcation;
  UINT8 StackIndex;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Iou = 0; Iou < MAX_IOU_PER_SOCKET; Iou++) {

      StackIndex = Iou+1;

      if (!DynamicSiLibraryPpi->IioIsSocketPresent (Socket)) {
        //
        // Skip sockets which are not present on the platform
        //
        continue;
      }

      if (!DynamicSiLibraryPpi->IioIsStackPresent (Socket, StackIndex)) {
        //
        // Skips stacks which are not present on the platform
        //
        continue;
      }
      //
      // Override if QAT is enabled
      //
      if (IsQatUplinkOverrideNeeded (Socket, Iou, DynamicSiLibraryPpi, &Bifurcation)) {
        if (IioGlobalData->SetupData.ConfigIOU[Socket][Iou] == IIO_BIFURCATE_AUTO) {
          IioGlobalData->SetupData.ConfigIOU[Socket][Iou] = Bifurcation;
          IIO_D_UBALOG ("[%d.%d] QAT-uplink detected; bif=%d\n", Socket, StackIndex, Bifurcation);
        }
      } else if (IsQatCableOverrideNeeded (Socket, Iou, DynamicSiLibraryPpi, &Bifurcation)) {
        if (IioGlobalData->SetupData.ConfigIOU[Socket][Iou] == IIO_BIFURCATE_AUTO) {
          IioGlobalData->SetupData.ConfigIOU[Socket][Iou] = Bifurcation;
          IIO_D_UBALOG ("[%d.%d] QAT-cable detected; bif=%d\n", Socket, StackIndex, Bifurcation);
        }
      }
    } // for each Iou
  } // for each Socket
}

/**
  Check if MRL sensor is present for given port.
  It depends on board architecture.
  Sometimes slot doesn't support it in contrast to Ext.Card.

  @param[in]  TotalPortIndex  Index of the port (across all ports on platform)
  @param[in]  ExtCardPresent  TRUE if Ext. card presence was detected

  @return TRUE if MRL sensor presence is expected
 */
BOOLEAN
IsMrlSensorPresent (
  IN  UINT16                      TotalPortIndex,
  IN  BOOLEAN                     ExtCardPresent
  )
{
  EFI_HOB_GUID_TYPE               *GuidHob;
  EFI_PLATFORM_INFO               *PlatformInfo;

  //
  // This data should be read from platform specific config (slot table IIO_BIFURCATION_DATA_ENTRY_EX)
  // but for now there is no such information
  // Let this be a temp. workaround until the update of UBA slot configs
  //

  if (ExtCardPresent) {
    //
    // MRL sensor is always present for Ext. Cards
    //
    return TRUE;
  }

  GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  if (GuidHob == NULL) {
    ASSERT (FALSE);
    return FALSE;
  }

  PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);

  switch (PlatformInfo->BoardId) {
  case TypeWilsonCityRP:
    //
    // This is version for ICX
    //
    switch (TotalPortIndex) {
    case SOCKET_1_INDEX + PORT_1A_INDEX:
    case SOCKET_1_INDEX + PORT_1B_INDEX:
    case SOCKET_1_INDEX + PORT_1C_INDEX:
    case SOCKET_1_INDEX + PORT_1D_INDEX:
      //
      // only one slot on WilsonCity supports MRL
      //
      return TRUE;
    default:
      return FALSE;
    }
    break;

  case TypeCooperCityRP:
    //
    // Let's assume that MRL is supported by all slots on CooperCity - where HP is enabled!
    //
    return TRUE;
  default:
    //
    // For the rest of platforms the situation is unknown for now
    //
    return FALSE;
  }
}

/**
  Verify if VMD is enabled and override Slot configuration
  based on the VMD settings

  @param[in]  IioGlobalData           Pointer to Iio Globals.
  @param[in]  Slot                    Slot configuration settings
  @param[in]  SlotEntries             Number of slot entries
  @param[in]  ExtCardPresent          Table with results of Ext. Card detection
**/
#define  SLOT_NUM_OFFSET_FOR_EXT_CARD_SLOT      0x50

VOID
ConfigureSlots (
  IN  OUT IIO_GLOBALS                *IioGlobalData,
  IN  IIO_SLOT_CONFIG_DATA_ENTRY_EX  *Slot,
  IN  UINT8                          SlotEntries,
  IN  DYNAMIC_SI_LIBARY_PPI          *DynamicSiLibraryPpi,
  IN  BOOLEAN                        ExtCardPresent[MAX_SOCKET][MAX_IOU_PER_SOCKET]
  )
{
  UINT8     Index;
  UINT16    TotalPortIndex;
  UINT8     IioIndex = 0;
  UINT8     Stack;
  UINT8     PortIndex = 0;
  UINT8     Iou;
  BOOLEAN   IsHpSupportedBySlot;
  BOOLEAN   IsVmdEnabledForSlot = TRUE;
  BOOLEAN   IsExtCardInSlot = TRUE;
  BOOLEAN   AreLanesAssignedToPort = TRUE;

  for (Index = 0; Index < SlotEntries; Index ++) {

    TotalPortIndex = Slot[Index].PortIndex;
    if (DynamicSiLibraryPpi->GetMaxPortNumPerSocket() == 0 || DynamicSiLibraryPpi->GetMaxPortNumPerSocket() == 0) {
      return;
    }
    IioIndex = (UINT8) (TotalPortIndex / DynamicSiLibraryPpi->GetMaxPortNumPerSocket ());
    PortIndex = (UINT8) (TotalPortIndex - (IioIndex * DynamicSiLibraryPpi->GetMaxPortNumPerSocket ()));
    Stack = IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex][PortIndex];
    Iou = Stack - 1;
    IsHpSupportedBySlot = FALSE;
    IsVmdEnabledForSlot = DynamicSiLibraryPpi->IsVMDEnabledForPort (IioIndex, PortIndex);
    IsExtCardInSlot = (Slot[Index].ExtensionCardSupport && ExtCardPresent[IioIndex][Iou]);
    AreLanesAssignedToPort = DynamicSiLibraryPpi->IioAreLanesAssignedToPort (IioGlobalData, IioIndex, PortIndex);

    if (Slot[Index].Hidden || !AreLanesAssignedToPort) {
      //
      // Hide rootports which are hidden in slots table or don't have any lanes assigned
      //
      IioGlobalData->SetupData.HidePEXPMenu[TotalPortIndex] = HIDE;
      IioGlobalData->SetupData.PEXPHIDE[TotalPortIndex] = HIDE;
    } else {
      //
      // Unhide active rootports (hided by default)
      //
      IioGlobalData->SetupData.HidePEXPMenu[TotalPortIndex] = NOT_HIDE;
      IioGlobalData->SetupData.PEXPHIDE[TotalPortIndex] = NOT_HIDE;
    }

    if (Slot[Index].SlotPowerLimitScale != PWR_SCL_MAX) {
      IioGlobalData->SetupData.SLOTSPLS[TotalPortIndex] = Slot[Index].SlotPowerLimitScale;
      IioGlobalData->SetupData.SLOTSPLV[TotalPortIndex] = Slot[Index].SlotPowerLimitValue;
    }

    if (Slot[Index].SlotNumber != NO_SLT_IMP) {
       IioGlobalData->SetupData.SLOTIMP[TotalPortIndex] = SLT_IMP;
       IioGlobalData->SetupData.SLOTPSP[TotalPortIndex] = Slot[Index].SlotNumber;
    } else if (AreLanesAssignedToPort) {
      //
      // If slot is not marked as implemented yet, but is exposed in current bifurcation
      // assume that there will be a slot
      //
      IioGlobalData->SetupData.SLOTIMP[TotalPortIndex] = 1;
      IioGlobalData->SetupData.SLOTPSP[TotalPortIndex] = SLOT_NUM_OFFSET_FOR_EXT_CARD_SLOT  + TotalPortIndex;
    }

    if (IsVmdEnabledForSlot) {
      //
      // Disable Electromechanical Interlock presence for VMD
      //
      IioGlobalData->SetupData.SLOTEIP[TotalPortIndex] = DISABLE;
    } else {
      IioGlobalData->SetupData.SLOTEIP[TotalPortIndex] = Slot[Index].InterLockPresent;
    }

    //
    // HotPlug related settings:
    //  - first check the presence of Ext. Card in the slot and apply specific setting
    //  - next apply common settings for HotPlug
    //
    if (IsExtCardInSlot && Slot[Index].ExtnCardHotPlugCapable) {
      IsHpSupportedBySlot = TRUE;
      //
      // Apply specific overrides for HP with Ext. Card
      //
      if (Slot[Index].ExtnCardHPVppPort != VPP_PORT_MAX) {
        IioGlobalData->SetupData.VppEnabled[TotalPortIndex] = TRUE;
        IioGlobalData->SetupData.VppPort[TotalPortIndex]    = Slot[Index].ExtnCardHPVppPort;
        IioGlobalData->SetupData.VppAddress[TotalPortIndex] = Slot[Index].ExtnCardHPVppAddress;
      }
    } else if (Slot[Index].HotPlugCapable) {
      //
      // HP settings when there is no Ext. Card
      //
      IsHpSupportedBySlot = TRUE;

      if (Slot[Index].VppPort != VPP_PORT_MAX) {
        IioGlobalData->SetupData.VppEnabled[TotalPortIndex] = TRUE;
        IioGlobalData->SetupData.VppPort[TotalPortIndex]    = Slot[Index].VppPort;
        IioGlobalData->SetupData.VppAddress[TotalPortIndex] = Slot[Index].VppAddress;
      }
    }

    if (IsHpSupportedBySlot) {
      //
      // There are common setting for HP
      //
      IioGlobalData->SetupData.SLOTHPCAP[TotalPortIndex] = ENABLE;
      IioGlobalData->SetupData.SLOTAIP[TotalPortIndex]   = ENABLE;   // Attention Indicator Present
      IioGlobalData->SetupData.SLOTPIP[TotalPortIndex]   = ENABLE;   // Power Indicator Present

      if (IsVmdEnabledForSlot) {
        IioGlobalData->SetupData.SLOTHPSUP[TotalPortIndex] = ENABLE; // HotPlug Surprise is always enabled for VMD
      }

      if (!IsVmdEnabledForSlot && IsMrlSensorPresent (TotalPortIndex, IsExtCardInSlot)) {
        //
        // Let's assume that if MRL sensor is present Attention button and Power Controller is also present
        //
        IioGlobalData->SetupData.SLOTMRLSP[TotalPortIndex] = ENABLE;
        IioGlobalData->SetupData.SLOTABP[TotalPortIndex]   = ENABLE;
        IioGlobalData->SetupData.SLOTPCP[TotalPortIndex]   = ENABLE;
      } else {
        IioGlobalData->SetupData.SLOTMRLSP[TotalPortIndex] = DISABLE;
        IioGlobalData->SetupData.SLOTABP[TotalPortIndex]   = DISABLE;
        IioGlobalData->SetupData.SLOTPCP[TotalPortIndex]   = DISABLE;
      }
    }
  } // for each slot entry
}

/**
  Check if lanes are reversed for given IOU - this is important when non-symetrical
  bifurcation is requested.

  @param[in]  BifurcationTable  record from bifurcation table

  @return TRUE if lanes are in reversed order
 */
BOOLEAN
IsLaneReversed (
  IN IIO_BIFURCATION_DATA_ENTRY_EX *BifurcationTable
  )
{
  EFI_HOB_GUID_TYPE               *GuidHob;
  EFI_PLATFORM_INFO               *PlatformInfo;

  //
  // This data should be read from platform specific config - but for now we don't have this information there
  // Let this be a temp. workaround until the update of biffurcation config.
  //

  GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  if (GuidHob == NULL) {
    ASSERT (FALSE);
    return FALSE;
  }

  PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);

  switch (PlatformInfo->BoardId) {
    case TypeCooperCityRP:
    //
    // For CooperCityRP lane reversed are:
    // - on even sockets: on IOU4
    // - on odd sockets: on IOU1
    //
    return ((BifurcationTable->Socket&1) == 1 && BifurcationTable->IouNumber == Iio_Iou1);
    default:
      //
      // Let's assume that WilsonCity config is a default one.
      //
      //
      // This is version for ICX (default)
      //
      return (BifurcationTable->Socket == 1 &&
                   (BifurcationTable->IouNumber == Iio_Iou0 || BifurcationTable->IouNumber == Iio_Iou4));
  }
}

/**
  Read the bifurcation info stored at I/O Expander (PCA9554) which BIOS
  can get through Smbus.
  To communicate with IO Expander first MUX PCA9545 needs to be configured.

  The bifurcation encoding is [3:0]:
  BW5_BIFURCATE_x4x4x4x4  1 0 0 0
  BW5_BIFURCATE_x4x4xxx8  1 0 0 1 Lanes to port ACD on board
  BW5_BIFURCATE_xxx8x4x4  1 0 1 0 Lanes to port ABC on board
  BW5_BIFURCATE_xxx8xxx8  1 0 1 1 Lanes to port AC on board
  No BW5                  1 1 x x

  @param IioGlobalData - Pointer to IioGlobals
  @param BroadwayTablePtr         - Pointer to BroadwayTable

  @retval  IIO_BIFURCATE_xxxxxxxx  BW communication failure
  @retval  IIO_BIFURCATE_x4x4x4x4  Requested bifurcation x4x4x4x4
  @retval  IIO_BIFURCATE_x4x4xxx8  Requested bifurcation x4x4xxx8
  @retval  IIO_BIFURCATE_xxx8x4x4  Requested bifurcation xxx8x4x4
  @retval  IIO_BIFURCATE_xxx8xxx8  Requested bifurcation xxx8xxx8
**/
UINT8
GetBw5Bifurcation (
  IN  IIO_BIFURCATION_DATA_ENTRY_EX   *BifurcationTable,
  IN  IN DYNAMIC_SI_LIBARY_PPI        *DynamicSiLibraryPpi
  )
{
  EFI_STATUS                Status;
  EFI_SMBUS_DEVICE_ADDRESS  SmbDevAddr;
  EFI_PEI_SMBUS2_PPI        *SmbPpi = NULL;
  UINT16                    SmbData = 0;
  UINT8                     SmbByteData = 0;
  UINT8                     RetryCount;
  UINTN                     SmbLen = 2;
  UINTN                     SmbByteLen = 1;

  if (BifurcationTable->ExtnCardSMBusAddress == SMB_ADDR_MAX ) {
    return IIO_BIFURCATE_xxxxxxxx;
  }

  Status = PeiServicesLocatePpi (&gEfiPeiSmbus2PpiGuid, 0, NULL, &SmbPpi);

  // Initialize Bw5Id to not present

  if (Status != EFI_SUCCESS || SmbPpi == NULL) {
    DEBUG ((DEBUG_ERROR, "[Iio] Get SMBus protocol error %x\n", Status));
    return IIO_BIFURCATE_xxxxxxxx;
  }

  for (RetryCount = 0; RetryCount < NUM_OF_RETRIES; RetryCount++) {

    //
    // Configure through the smbus MUX PCA9545 to start communicate to GPIO expander
    //
    SmbDevAddr.SmbusDeviceAddress = BifurcationTable->MuxSMBusAddress >> 1;
    SmbByteData = BifurcationTable->MuxSMBusChannel;
    Status = SmbPpi->Execute (SmbPpi, SmbDevAddr, 0, EfiSmbusSendByte, FALSE, &SmbByteLen, &SmbByteData);
    if (EFI_ERROR (Status)) {
      continue;
    }

    //
    // Read the current I/O pins configuration for Port0
    //
    SmbDevAddr.SmbusDeviceAddress = BifurcationTable->ExtnCardSMBusAddress >> 1;
    Status = SmbPpi->Execute (SmbPpi, SmbDevAddr, BW4_CONFIG_OFFSET, EfiSmbusReadWord, FALSE, &SmbLen, &SmbData);
    if (!EFI_ERROR (Status)) {
     //
     // Configure the direction of I/O pins for Port0/Port1 as Input.
     //
      SmbData = SmbData | BW5_CONFIG_REG_MASK;
      Status = SmbPpi->Execute (SmbPpi, SmbDevAddr, BW4_CONFIG_OFFSET, EfiSmbusWriteWord, FALSE, &SmbLen, &SmbData);
      if (!EFI_ERROR (Status)) {
        //
        // Read Input Port0/Port1 register to identify BW5 Id
        //
        Status = SmbPpi->Execute (SmbPpi, SmbDevAddr, BW4_DATA_OFFSET, EfiSmbusReadWord, FALSE, &SmbLen, &SmbData);
        if (!EFI_ERROR (Status)){
          //
          // Mask the Input Port0/1 register data [15:0] to get BW5 ID [3:0].
          //
          // The bifurcation encoding is [3:0]:
          // BW5_BIFURCATE_x4x4x4x4  1 0 0 0
          // BW5_BIFURCATE_x4x4xxx8  1 0 0 1 Lanes to port ACD on board
          // BW5_BIFURCATE_xxx8x4x4  1 0 1 0 Lanes to port ABC on board
          // BW5_BIFURCATE_xxx8xxx8  1 0 1 1 Lanes to port AC on board
          // No BW5                  1 1 x x
          //
          switch (SmbData & 0xF) {
            case 0x8:
              return IIO_BIFURCATE_x4x4x4x4;
              break;
            case 0x9:
              if (IsLaneReversed (BifurcationTable)) {
                return IIO_BIFURCATE_xxx8x4x4;
              } else {
                return IIO_BIFURCATE_x4x4xxx8;
              }
              break;
            case 0xA:
              if (IsLaneReversed (BifurcationTable)) {
                return IIO_BIFURCATE_x4x4xxx8;
              } else {
                return IIO_BIFURCATE_xxx8x4x4;
              }
              break;
            case 0xB:
              return IIO_BIFURCATE_xxx8xxx8;
              break;
            default:
              return IIO_BIFURCATE_xxxxxxxx;
              break;
          }
        } // Read Port0
      } // Configure Port0
    } // Read Port0 Config

    //
    // Configure through the smbus MUX PCA9545 to end communicate to GPIO expander
    //
    SmbDevAddr.SmbusDeviceAddress = BifurcationTable->MuxSMBusAddress >> 1;
    SmbByteData &= ~BifurcationTable->MuxSMBusChannel;
    Status = SmbPpi->Execute (SmbPpi, SmbDevAddr, 0, EfiSmbusSendByte, FALSE, &SmbByteLen, &SmbByteData);
    if (EFI_ERROR (Status)) {
      continue;
    }
  } //RetryCount

  return IIO_BIFURCATE_xxxxxxxx;
}

/**
  Set bifurcation base of default settings and Ext. Card presence
  (detected by BW)

  @param[in]  IioGlobalData           Pointer to Iio Globals.
  @param[in]  BifurcationTable        Bifurcation configuration settings
  @param[in]  BifurcationEntries      Number of bif. table entries
  @param[in]  ExtCardPresent          Table with results of Ext. Card detection
**/
VOID
DetectExtCards (
  IN   IIO_GLOBALS                    *IioGlobalData,
  IN   IIO_BIFURCATION_DATA_ENTRY_EX  *BifurcationTable,
  IN   UINT8                          BifurcationEntries,
  IN   DYNAMIC_SI_LIBARY_PPI          *DynamicSiLibraryPpi,
  OUT  BOOLEAN                        ExtCardPresent[MAX_SOCKET][MAX_IOU_PER_SOCKET]
  )
{
  UINT8  Index;
  UINT8  BwBifurcation;
  UINT8  Socket;
  UINT8  Iou;

  //
  // Try to communicate with Ext.Card which can request custom bifurcation on the stack
  //
  for (Index = 0; Index < BifurcationEntries; Index++){

    Socket = BifurcationTable[Index].Socket;
    Iou = BifurcationTable[Index].IouNumber;

    ExtCardPresent[Socket][Iou] = FALSE; // by default

    if (BifurcationTable[Index].ExtnCardSMBusAddress != SMB_ADDR_MAX) {
      //
      // ExtCard can be supported at this stack - try to detect it
      //
      BwBifurcation = GetBw5Bifurcation (&BifurcationTable[Index], DynamicSiLibraryPpi);
      if (BwBifurcation != IIO_BIFURCATE_xxxxxxxx) {
        //
        // Ext. Card is detected on this IOU
        //
        IIO_D_UBALOG ("[%d.%d] Ext. Card detected; bif=%d\n", Socket, Iou+1, BwBifurcation);
        ExtCardPresent[Socket][Iou] = TRUE;
      }
    }

    //
    // Set custom bifurcation requested by Ext. Card
    //
    if (IioGlobalData->SetupData.ConfigIOU[Socket][Iou] == IIO_BIFURCATE_AUTO) {
      //
      // There are no overrides for this IOU
      //
      if (ExtCardPresent[Socket][Iou]) {
        //
        // Set bifurcation requested by Ext. Card
        //
        IioGlobalData->SetupData.ConfigIOU[Socket][Iou] = BwBifurcation;
      } else {
        //
        // Set the default bifurcation for this IOU
        //
        IioGlobalData->SetupData.ConfigIOU[Socket][Iou] = BifurcationTable[Index].Bifurcation;
      }
    }

    IIO_D_UBADBG ("[%d.%d] final bifurcation = %X\n", Socket, Iou+1, IioGlobalData->SetupData.ConfigIOU[Socket][Iou]);

  } // for bifurcation table entry
}

/**
  Dump prepared configuration to log.

  @param[in]  SetupData  Pointer to SetupData structure
**/
VOID
DumpConfiguration (
  IN IIO_CONFIG                  *SetupData,
  IN DYNAMIC_SI_LIBARY_PPI       *DynamicSiLibraryPpi
  )
{
  UINT8  IioIndex;
  UINT8  StackIndex = 0;
  UINT8  Iou;
  UINT8  PortIndex = 0;
  UINT8  PortInStackIndex = 0;
  UINT16 TotalPortIndex =0;

  for (IioIndex = 0; IioIndex < MAX_SOCKET ; IioIndex++) {

    if (!DynamicSiLibraryPpi->IioIsSocketPresent (IioIndex)) {
      //
      // Skips sockets which are not present on the platform
      //
      continue;
    }
    for (StackIndex = 0; StackIndex <  DynamicSiLibraryPpi->GetMaxStackNumPerSocket (); StackIndex++) {

      if (DynamicSiLibraryPpi->IsDmiStack (StackIndex)) {
        //
        // Skip DMI stacks in dump
        //
        continue;
      }

      if (!DynamicSiLibraryPpi->IioIsStackPresent (IioIndex, StackIndex)) {
        //
        // Skips stacks which are not present on the platform
        //
        continue;
      }

      if (StackIndex >= MAX_IIO_STACK) {
        break;
      }
      Iou = StackIndex - 1;

      IIO_D_UBALOG("[%d.%d] Bifurcation = %X\n", IioIndex, StackIndex, SetupData->ConfigIOU[IioIndex][Iou]);
      IIO_D_UBALOG ("  [ IDX ], PortHide, SlotImpl, SlotNumber, HotPlug,  Vpp,  Interlock\n");

      for (PortInStackIndex = 0; PortInStackIndex < DynamicSiLibraryPpi->GetMaxPortNumPerStack (StackIndex); PortInStackIndex++) {
        PortIndex = DynamicSiLibraryPpi->GetPortIndexbyStack (StackIndex, PortInStackIndex);
        TotalPortIndex = (IioIndex * DynamicSiLibraryPpi->GetMaxPortNumPerSocket ()) + PortIndex;
        IIO_D_UBALOG ("  [%d p%02d]    %2d    |   %2d    |   %3d     |   %3d  |  0x%02x   |  %2d      \n",
                       IioIndex, PortIndex,
                       SetupData->PEXPHIDE[TotalPortIndex],
                       SetupData->SLOTIMP[TotalPortIndex],
                       SetupData->SLOTPSP[TotalPortIndex],
                       SetupData->SLOTHPCAP[TotalPortIndex],
                       SetupData->VppAddress[TotalPortIndex] | SetupData->VppPort[TotalPortIndex],
                       SetupData->SLOTEIP[TotalPortIndex]);
      } // foreach PortInStack
    } //foreach StackIndex
  } // foreach IioIndex
}

/**
  This function prepare the data for silicon initialization based on
  bifurcations and slots table

  This function is for tables in version PLATFORM_IIO_CONFIG_UPDATE_VERSION = 2

  @param[in]  IioGlobalData  IIO Global data structure
  @param[in]  IioConfigTable
 */
VOID
IioPortBifurcationInitVer2 (
  IN  IIO_GLOBALS                          *IioGlobalData,
  IN  PLATFORM_IIO_CONFIG_UPDATE_TABLE_EX  IioConfigTable,
  IN  DYNAMIC_SI_LIBARY_PPI                *DynamicSiLibraryPpi
  )
{
  IIO_BIFURCATION_DATA_ENTRY_EX   *BifurcationTableEx;
  UINT8                           BifurcationEntries;
  IIO_SLOT_CONFIG_DATA_ENTRY_EX   *SlotTableEx;
  UINT8                           SlotEntries;
  BOOLEAN                         ExtCardDetected[MAX_SOCKET][MAX_IOU_PER_SOCKET];

  //
  // Init bifurcation and slots tables
  //
  BifurcationTableEx = IioConfigTable.IioBifurcationTablePtr;
  BifurcationEntries = (UINT8) (IioConfigTable.IioBifurcationTableSize / sizeof (IIO_BIFURCATION_DATA_ENTRY_EX));
  SlotTableEx = IioConfigTable.IioSlotTablePtr;
  SlotEntries = (UINT8) (IioConfigTable.IioSlotTableSize / sizeof (IIO_SLOT_CONFIG_DATA_ENTRY_EX));

  //
  // Set the bifurcations for each IOU:
  //  - if any override is set in setup menu - stay bifurcation not changed
  //  - next check if QAT is detected (this is LBG specific) - QAT requires special bifurcation
  //  - next try to detect ext. cards with BW bifurcation
  //  - at the end set default bifurcation if no other options wer applied before

  if (DynamicSiLibraryPpi->IsCpuAndRevision (CPU_ICXSP, REV_ALL)   ||
      DynamicSiLibraryPpi->IsCpuAndRevision (CPU_CPX, REV_ALL)   ||
      DynamicSiLibraryPpi->IsCpuAndRevision (CPU_SKX, REV_ALL)   ||
      DynamicSiLibraryPpi->IsCpuAndRevision (CPU_CLX, REV_ALL)
  ) {
    //
    // This is important only for programs with LBG PCH
    //
    CheckQatBifurcationOverrides (IioGlobalData, DynamicSiLibraryPpi);
  }
  DetectExtCards (IioGlobalData, BifurcationTableEx, BifurcationEntries, DynamicSiLibraryPpi, ExtCardDetected);

  //
  // Set each rootport settings based on slots table
  //
  ConfigureSlots (IioGlobalData, SlotTableEx, SlotEntries, DynamicSiLibraryPpi, ExtCardDetected);

  //
  // Configuration is ready - dump the config to log
  //
  DumpConfiguration (&IioGlobalData->SetupData, DynamicSiLibraryPpi);
}

/**

  Program the IIO_GLOBALS data structure with OEM IIO init values for SLOTs and Bifurcation.

  @param mSB - pointer to this protocol
  @param IioUds - Pointer to the IIO UDS data structure.

  @retval EFI_SUCCESS
**/
VOID
IioPortBifurcationInitCallback (
  IN IIO_GLOBALS *IioGlobalData
  )
{
  EFI_STATUS                            Status;
  UBA_CONFIG_DATABASE_PPI               *UbaConfigPpi = NULL;
  DYNAMIC_SI_LIBARY_PPI                 *DynamicSiLibraryPpi = NULL;
  PLATFORM_IIO_CONFIG_UPDATE_TABLE_EX   IioConfigTable;
  UINTN                                 TableSize;

  //
  // Locate PLATFORM_IIO_CONFIG_UPDATE_TABLE_EX
  //
  Status = PeiServicesLocatePpi (&gUbaConfigDatabasePpiGuid, 0, NULL, &UbaConfigPpi);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return;
  }

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  TableSize = sizeof (IioConfigTable);
  Status = UbaConfigPpi->GetData (UbaConfigPpi, &gPlatformIioConfigDataGuid, &IioConfigTable, &TableSize);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return;
  }

  ASSERT (IioConfigTable.Signature == PLATFORM_IIO_CONFIG_UPDATE_SIGNATURE);

  //
  // Call the right version of bifurcation init procedure
  //
  switch (IioConfigTable.Version) {
  case PLATFORM_IIO_CONFIG_UPDATE_VERSION_2:
    IioPortBifurcationInitVer2 (IioGlobalData, IioConfigTable, DynamicSiLibraryPpi);
    break;
  case PLATFORM_IIO_CONFIG_UPDATE_VERSION:
    IioPortBifurcationInitVer1 (IioGlobalData);
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IIO_PORT_BIFURCATION_INIT_TABLE IioPortBifurcationInitTable =
  {
    IIO_PORT_BIFURCATION_INIT_SIGNATURE,
    IIO_PORT_BIFURCATION_INIT_VERSION,
    IioPortBifurcationInitCallback
  };

EFI_STATUS
InstallIioPortBifurcationInitData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
  )
{
  return UbaConfigPpi->AddData (
           UbaConfigPpi,
           &gIioPortBifurcationInitDataGuid,
           &IioPortBifurcationInitTable,
           sizeof (IioPortBifurcationInitTable)
           );
}

