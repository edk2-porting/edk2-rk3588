/** @file

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//
// Module specific Includes
//
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/GpioLib.h>
#include <TbtBoardInfo.h>
#include <Protocol/TbtNvsArea.h>
#include <PchAccess.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/IoLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Protocol/SmmSxDispatch2.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Uefi/UefiSpec.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Guid/HobList.h>
#include "TbtSmiHandler.h"
#include <PcieRegs.h>
#include <Protocol/SaPolicy.h>
#include <Protocol/DxeTbtPolicy.h>
#include <Library/PchPmcLib.h>
#define P2P_BRIDGE                    (((PCI_CLASS_BRIDGE) << 8) | (PCI_CLASS_BRIDGE_P2P))

#define CMD_BM_MEM_IO                 (CMD_BUS_MASTER | BIT1 | BIT0)

#define DISBL_IO_REG1C                0x01F1
#define DISBL_MEM32_REG20             0x0000FFF0
#define DISBL_PMEM_REG24              0x0001FFF1

#define DOCK_BUSSES                   8

#define PCI_CAPABILITY_ID_PCIEXP      0x10
#define PCI_CAPBILITY_POINTER_OFFSET  0x34

#define LTR_MAX_SNOOP_LATENCY_VALUE             0x0846    ///< Intel recommended maximum value for Snoop Latency  can we put like this ?
#define LTR_MAX_NON_SNOOP_LATENCY_VALUE         0x0846    ///< Intel recommended maximum value for Non-Snoop Latency can we put like this ?


GLOBAL_REMOVE_IF_UNREFERENCED TBT_NVS_AREA                *mTbtNvsAreaPtr;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                       gCurrentDiscreteTbtRootPort;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                       gCurrentDiscreteTbtRootPortType;
GLOBAL_REMOVE_IF_UNREFERENCED UINT16                      TbtLtrMaxSnoopLatency;
GLOBAL_REMOVE_IF_UNREFERENCED UINT16                      TbtLtrMaxNoSnoopLatency;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                       gDTbtPcieRstSupport;
GLOBAL_REMOVE_IF_UNREFERENCED TBT_INFO_HOB                *gTbtInfoHob = NULL;
STATIC UINTN                                              mPciExpressBaseAddress;
STATIC UINT8                TbtSegment        = 0;
VOID
GpioWrite (
  IN  UINT32         GpioNumber,
  IN  BOOLEAN        Value
  )
{
  GpioSetOutputValue (GpioNumber, (UINT32)Value);
}

/**
  Search and return the offset of desired Pci Express Capability ID
  CAPID list:
    0x0001 = Advanced Error Reporting Capability
    0x0002 = Virtual Channel Capability
    0x0003 = Device Serial Number Capability
    0x0004 = Power Budgeting Capability

  @param[in] Bus                  Pci Bus Number
  @param[in] Device               Pci Device Number
  @param[in] Function             Pci Function Number
  @param[in] CapId                Extended CAPID to search for

  @retval 0                       CAPID not found
  @retval Other                   CAPID found, Offset of desired CAPID
**/
UINT16
PcieFindExtendedCapId (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT16  CapId
  )
{
  UINT16  CapHeaderOffset;
  UINT16  CapHeaderId;
  UINT64  DeviceBase;

  DeviceBase = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Device, Function, 0);

  ///
  /// Start to search at Offset 0x100
  /// Get Capability Header, A pointer value of 00h is used to indicate the last capability in the list.
  ///
  CapHeaderId     = 0;
  CapHeaderOffset = 0x100;
  while (CapHeaderOffset != 0 && CapHeaderId != 0xFFFF) {
    CapHeaderId = PciSegmentRead16 (DeviceBase + CapHeaderOffset);
    if (CapHeaderId == CapId) {
      return CapHeaderOffset;
    }
    ///
    /// Each capability must be DWORD aligned.
    /// The bottom two bits of all pointers are reserved and must be implemented as 00b
    /// although software must mask them to allow for future uses of these bits.
    ///
    CapHeaderOffset = (PciSegmentRead16 (DeviceBase + CapHeaderOffset + 2) >> 4) & ((UINT16) ~(BIT0 | BIT1));
  }

  return 0;
}

/**
  Find the Offset to a given Capabilities ID
  CAPID list:
    0x01 = PCI Power Management Interface
    0x04 = Slot Identification
    0x05 = MSI Capability
    0x10 = PCI Express Capability

  @param[in] Bus                  Pci Bus Number
  @param[in] Device               Pci Device Number
  @param[in] Function             Pci Function Number
  @param[in] CapId                CAPID to search for

  @retval 0                       CAPID not found
  @retval Other                   CAPID found, Offset of desired CAPID
**/
UINT8
PcieFindCapId (
  IN UINT8   Segment,
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT8   CapId
  )
{
  UINT8   CapHeaderOffset;
  UINT8   CapHeaderId;
  UINT64  DeviceBase;

  DeviceBase = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, Device, Function, 0);

  if ((PciSegmentRead8 (DeviceBase + PCI_PRIMARY_STATUS_OFFSET) & EFI_PCI_STATUS_CAPABILITY) == 0x00) {
    ///
    /// Function has no capability pointer
    ///
    return 0;
  }

  ///
  /// Check the header layout to determine the Offset of Capabilities Pointer Register
  ///
  if ((PciSegmentRead8 (DeviceBase + PCI_HEADER_TYPE_OFFSET) & HEADER_LAYOUT_CODE) == (HEADER_TYPE_CARDBUS_BRIDGE)) {
    ///
    /// If CardBus bridge, start at Offset 0x14
    ///
    CapHeaderOffset = 0x14;
  } else {
    ///
    /// Otherwise, start at Offset 0x34
    ///
    CapHeaderOffset = 0x34;
  }
  ///
  /// Get Capability Header, A pointer value of 00h is used to indicate the last capability in the list.
  ///
  CapHeaderId     = 0;
  CapHeaderOffset = PciSegmentRead8 (DeviceBase + CapHeaderOffset) & ((UINT8) ~(BIT0 | BIT1));
  while (CapHeaderOffset != 0 && CapHeaderId != 0xFF) {
    CapHeaderId = PciSegmentRead8 (DeviceBase + CapHeaderOffset);
    if (CapHeaderId == CapId) {
      return CapHeaderOffset;
    }
    ///
    /// Each capability must be DWORD aligned.
    /// The bottom two bits of all pointers (including the initial pointer at 34h) are reserved
    /// and must be implemented as 00b although software must mask them to allow for future uses of these bits.
    ///
    CapHeaderOffset = PciSegmentRead8 (DeviceBase + CapHeaderOffset + 1) & ((UINT8) ~(BIT0 | BIT1));
  }

  return 0;
}
/**
  This function configures the L1 Substates.
  It can be used for Rootport and endpoint devices.

  @param[in] DownstreamPort               Indicates if the device about to be programmed is a downstream port
  @param[in] DeviceBase                   Device PCI configuration base address
  @param[in] L1SubstateExtCapOffset       Pointer to L1 Substate Capability Structure
  @param[in] PortL1SubstateCapSupport     L1 Substate capability setting
  @param[in] PortCommonModeRestoreTime    Common Mode Restore Time
  @param[in] PortTpowerOnValue            Tpower_on Power On Wait Time
  @param[in] PortTpowerOnScale            Tpower-on Scale

  @retval none
**/
VOID
ConfigureL1s (
  IN UINTN                              DeviceBase,
  IN UINT16                             L1SubstateExtCapOffset,
  IN UINT32                             PortL1SubstateCapSupport,
  IN UINT32                             PortCommonModeRestoreTime,
  IN UINT32                             PortTpowerOnValue,
  IN UINT32                             PortTpowerOnScale,
  IN UINT16                             MaxLevel
  )
{

  PciSegmentAndThenOr32 (
    DeviceBase + L1SubstateExtCapOffset + R_PCIE_EX_L1SCTL1_OFFSET,
    (UINT32) ~(0xFF00),
    (UINT32) PortCommonModeRestoreTime << 8
    );

  PciSegmentAnd32(DeviceBase + L1SubstateExtCapOffset + R_PCIE_EX_L1SCTL2_OFFSET, 0xFFFFFF04);

  PciSegmentOr32(DeviceBase + L1SubstateExtCapOffset + R_PCIE_EX_L1SCTL2_OFFSET,(UINT32) ((PortTpowerOnValue << N_PCIE_EX_L1SCTL2_POWT) | PortTpowerOnScale));

  PciSegmentAndThenOr32 (
    DeviceBase + L1SubstateExtCapOffset + R_PCIE_EX_L1SCTL1_OFFSET,
    (UINT32) ~(0xE3FF0000),
    (UINT32) (BIT30 | BIT23 | BIT21)
    );

}

VOID
RootportL1sSupport (
  IN UINT8   Bus,
  IN UINT8   Dev,
  IN UINT8   Fun,
  IN UINT16  RootL1SubstateExtCapOffset,
  IN UINT16  MaxL1Level
  )
{
  UINTN       ComponentABaseAddress;
  UINTN       ComponentBBaseAddress;
  UINT8       SecBus;
  UINT32      PortL1SubstateCapSupport;
  UINT32      PortCommonModeRestoreTime;
  UINT32      PortTpowerOnValue;
  UINT32      PortTpowerOnScale;
  UINT16      ComponentBL1SubstateExtCapOffset;
  UINT32      ComponentBL1Substates;
  UINT32      ComponentBCommonModeRestoreTime;
  UINT32      ComponentBTpowerOnValue;
  UINT32      ComponentBTpowerOnScale;
  UINT32      Data32;

  PortL1SubstateCapSupport  = 0;
  PortCommonModeRestoreTime = 0;
  PortTpowerOnValue = 0;
  PortTpowerOnScale = 0;
  Data32 = 0;

  ComponentABaseAddress  = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
  if (RootL1SubstateExtCapOffset != 0) {
    Data32 = PciSegmentRead32 (ComponentABaseAddress + RootL1SubstateExtCapOffset + R_PCIE_EX_L1SCAP_OFFSET);
    PortL1SubstateCapSupport  = (Data32) & 0x0F;
    PortCommonModeRestoreTime = (Data32 >> 8) & 0xFF;
    PortTpowerOnScale         = (Data32 >> 16) & 0x3;
    PortTpowerOnValue         = (Data32 >> 19) & 0x1F;
  } else {
    MaxL1Level                = 0; // If L1 Substates from Root Port side is disable, then Disable from Device side also.
  }

  SecBus                = PciSegmentRead8 (ComponentABaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
  ComponentBBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, SecBus, 0, 0, 0);

  if (PciSegmentRead16 (ComponentBBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
    ComponentBL1SubstateExtCapOffset = PcieFindExtendedCapId (
                                  SecBus,
                                  0,
                                  0,
                                  V_PCIE_EX_L1S_CID
                                  );
    if (ComponentBL1SubstateExtCapOffset != 0) {
      ComponentBL1Substates = PciSegmentRead32 (ComponentBBaseAddress + ComponentBL1SubstateExtCapOffset + R_PCIE_EX_L1SCAP_OFFSET);
      ComponentBCommonModeRestoreTime = (ComponentBL1Substates >> 8) & 0xFF;
      ComponentBTpowerOnScale         = (ComponentBL1Substates >> 16) & 0x3;
      ComponentBTpowerOnValue         = (ComponentBL1Substates >> 19) & 0x1F;

      if (MaxL1Level == 3) {
        if (Data32 >= ComponentBL1Substates) {
          if (~(Data32 | BIT2)) {
            MaxL1Level = 1;
          }
        }
        else {
          if (~(ComponentBL1Substates | BIT2)) {
          MaxL1Level = 1;
        }
      }
    }

      if (MaxL1Level == 3) {
        ConfigureL1s (
          ComponentABaseAddress,
          RootL1SubstateExtCapOffset,
          PortL1SubstateCapSupport,
          ComponentBCommonModeRestoreTime,
          ComponentBTpowerOnValue,
          ComponentBTpowerOnScale,
          MaxL1Level
          );

      ConfigureL1s (
          ComponentBBaseAddress,
          ComponentBL1SubstateExtCapOffset,
          ComponentBL1Substates,
          PortCommonModeRestoreTime,
          PortTpowerOnValue,
          PortTpowerOnScale,
          MaxL1Level
          );
      }

      if (MaxL1Level == 1) {
        PciSegmentOr32 (
          ComponentABaseAddress + RootL1SubstateExtCapOffset + R_PCIE_EX_L1SCTL1_OFFSET,
          (UINT32) (BIT3 | BIT1)
          );

        PciSegmentOr32 (
          ComponentBBaseAddress + ComponentBL1SubstateExtCapOffset + R_PCIE_EX_L1SCTL1_OFFSET,
          (UINT32) (BIT3 | BIT1)
          );
      }
      else {
        if (RootL1SubstateExtCapOffset != 0) {
          PciSegmentOr32 (
            ComponentABaseAddress + RootL1SubstateExtCapOffset + R_PCIE_EX_L1SCTL1_OFFSET,
            (UINT32) (BIT3 | BIT1)
            );

          PciSegmentOr32 (
            ComponentABaseAddress + RootL1SubstateExtCapOffset + R_PCIE_EX_L1SCTL1_OFFSET,
            (UINT32) (BIT2 | BIT0)
            );
        }
        if (ComponentBL1SubstateExtCapOffset != 0) {
          PciSegmentOr32 (
            ComponentBBaseAddress + ComponentBL1SubstateExtCapOffset + R_PCIE_EX_L1SCTL1_OFFSET,
            (UINT32) (BIT3 | BIT1)
           );

          PciSegmentOr32 (
            ComponentBBaseAddress + ComponentBL1SubstateExtCapOffset + R_PCIE_EX_L1SCTL1_OFFSET,
            (UINT32) (BIT2 | BIT0)
            );
        }
      }
    }
  }
}

VOID
MultiFunctionDeviceAspm (
  IN UINT8   Bus,
  IN UINT8   Dev
  )
{
  UINT16  LowerAspm;
  UINT16  AspmVal;
  UINT8   Fun;
  UINT64  DeviceBaseAddress;
  UINT8   CapHeaderOffset;

  LowerAspm = 3; // L0s and L1 Supported
  for (Fun = 0; Fun <= PCI_MAX_FUNC; ++Fun) {
    //
    // Check for Device availability
    //
    DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
    if (PciSegmentRead16 (DeviceBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
      // Device not present
      continue;
    }

    CapHeaderOffset = PcieFindCapId (TbtSegment, Bus, Dev, Fun, 0x10);

    AspmVal = (PciSegmentRead16 (DeviceBaseAddress + CapHeaderOffset + 0x00C) >> 10) & 3;
    if (LowerAspm > AspmVal) {
      LowerAspm = AspmVal;
    }
  } //Fun

  for (Fun = 0; Fun <= PCI_MAX_FUNC; ++Fun) {
    //
    // Check for Device availability
    //
    DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
    if (PciSegmentRead16 (DeviceBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
      //
      // Device not present
      //
      continue;
    }

    CapHeaderOffset = PcieFindCapId (TbtSegment, Bus, Dev, Fun, 0x10);

    PciSegmentAndThenOr16 (DeviceBaseAddress + CapHeaderOffset + 0x10, 0xFFFC, LowerAspm);
  } //Fun
}

UINT16
LimitAspmLevel (
  IN UINT16  SelectedAspm,
  IN UINT16  MaxAspmLevel
  )
{
  SelectedAspm = SelectedAspm & MaxAspmLevel;

  return SelectedAspm;
}

UINT16
FindOptimalAspm (
  IN UINT16   ComponentAaspm,
  IN UINT16   ComponentBaspm
  )
{
  UINT16  SelectedAspm;

  SelectedAspm = ComponentAaspm & ComponentBaspm;

  return SelectedAspm;
}

UINT16
FindComponentBaspm (
  IN UINT8   Bus,
  IN UINT8   MaxBus
  )
{
  UINT8   BusNo;
  UINT8   DevNo;
  UINT8   FunNo;
  UINT64  DevBaseAddress;
  UINT8   RegVal;
  UINT8   SecBusNo;
  UINT16  SelectedAspm; // No ASPM Support
  UINT8   CapHeaderOffset_B;
  BOOLEAN AspmFound;

  SelectedAspm  = 0;
  AspmFound     = FALSE;

  for (BusNo = MaxBus; (BusNo != 0xFF) && (!AspmFound); --BusNo) {
    for (DevNo = 0; (DevNo <= PCI_MAX_DEVICE) && (!AspmFound); ++DevNo) {
      for (FunNo = 0; (FunNo <= PCI_MAX_FUNC) && (!AspmFound); ++FunNo) {
        //
        // Check for Device availability
        //
        DevBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, BusNo, DevNo, FunNo, 0);
        if (PciSegmentRead16 (DevBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
          //
          // Device not present
          //
          continue;
        }

        RegVal = PciSegmentRead8 (DevBaseAddress + PCI_HEADER_TYPE_OFFSET);
        if ((RegVal & (BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6)) != 0x01) {
          //
          // Not a PCI-to-PCI bridges device
          //
          continue;
        }

        SecBusNo = PciSegmentRead8 (DevBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);

        if (SecBusNo == Bus) {
          //
          // This is the Rootbridge for the given 'Bus' device
          //
          CapHeaderOffset_B = PcieFindCapId (TbtSegment, BusNo, DevNo, FunNo, 0x10);
          SelectedAspm      = (PciSegmentRead16 (DevBaseAddress + CapHeaderOffset_B + 0x00C) >> 10) & 3;
          AspmFound         = TRUE;
        }
      } //FunNo
    } //DevNo
  } //BusNo

  return (SelectedAspm);
}

VOID
NoAspmSupport (
  IN UINT8   Bus,
  IN UINT8   Dev,
  IN UINT8   Fun,
  IN UINT8   CapHeaderOffset
  )
{
  UINT64 DeviceBaseAddress;

  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
  PciSegmentAndThenOr16 (DeviceBaseAddress + CapHeaderOffset + 0x10, 0xFFFC, 0x00);
}

VOID
EndpointAspmSupport (
  IN UINT8   Bus,
  IN UINT8   Dev,
  IN UINT8   Fun,
  IN UINT8   CapHeaderOffset,
  IN UINT8   MaxBus,
  IN UINT16  MaxAspmLevel
  )
{
  UINT64  DeviceBaseAddress;
  UINT16  ComponentAaspm;
  UINT16  ComponentBaspm;
  UINT16  SelectedAspm;

  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
  ComponentAaspm    = (PciSegmentRead16 (DeviceBaseAddress + CapHeaderOffset + 0x00C) >> 10) & 3;
  ComponentBaspm    = FindComponentBaspm (Bus, MaxBus);
  SelectedAspm      = FindOptimalAspm (ComponentAaspm, ComponentBaspm);
  SelectedAspm      = LimitAspmLevel (SelectedAspm, MaxAspmLevel);
  PciSegmentAndThenOr16 (DeviceBaseAddress + CapHeaderOffset + 0x10, 0xFFFC, SelectedAspm);
}

VOID
UpstreamAspmSupport (
  IN UINT8   Bus,
  IN UINT8   Dev,
  IN UINT8   Fun,
  IN UINT8   CapHeaderOffset,
  IN UINT8   MaxBus,
  IN UINT16  MaxAspmLevel
  )
{
  UINT64  DeviceBaseAddress;
  UINT16  ComponentAaspm;
  UINT16  ComponentBaspm;
  UINT16  SelectedAspm;

  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
  ComponentAaspm    = (PciSegmentRead16 (DeviceBaseAddress + CapHeaderOffset + 0x00C) >> 10) & 3;
  ComponentBaspm    = FindComponentBaspm (Bus, MaxBus);
  SelectedAspm      = FindOptimalAspm (ComponentAaspm, ComponentBaspm);
  SelectedAspm      = LimitAspmLevel (SelectedAspm, MaxAspmLevel);
  PciSegmentAndThenOr16 (DeviceBaseAddress + CapHeaderOffset + 0x10, 0xFFFC, SelectedAspm);
}

VOID
DownstreamAspmSupport (
  IN UINT8   Bus,
  IN UINT8   Dev,
  IN UINT8   Fun,
  IN UINT8   CapHeaderOffset,
  IN UINT16  MaxAspmLevel
  )
{
  UINT64  ComponentABaseAddress;
  UINT64  ComponentBBaseAddress;
  UINT16  ComponentAaspm;
  UINT16  ComponentBaspm;
  UINT16  SelectedAspm;
  UINT8   SecBus;
  UINT8   CapHeaderOffset_B;

  ComponentABaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
  ComponentAaspm        = (PciSegmentRead16 (ComponentABaseAddress + CapHeaderOffset + 0x00C) >> 10) & 3;

  SecBus                = PciSegmentRead8 (ComponentABaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
  ComponentBBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, SecBus, 0, 0, 0);
  ComponentBaspm        = 0; // No ASPM Support
  if (PciSegmentRead16 (ComponentBBaseAddress + PCI_DEVICE_ID_OFFSET) != 0xFFFF) {
    CapHeaderOffset_B = PcieFindCapId (TbtSegment, SecBus, 0, 0, 0x10);
    ComponentBaspm    = (PciSegmentRead16 (ComponentBBaseAddress + CapHeaderOffset_B + 0x00C) >> 10) & 3;
  }

  SelectedAspm = FindOptimalAspm (ComponentAaspm, ComponentBaspm);
  SelectedAspm = LimitAspmLevel (SelectedAspm, MaxAspmLevel);
  PciSegmentAndThenOr16 (ComponentABaseAddress + CapHeaderOffset + 0x10, 0xFFFC, SelectedAspm);
}

VOID
RootportAspmSupport (
  IN UINT8   Bus,
  IN UINT8   Dev,
  IN UINT8   Fun,
  IN UINT8   CapHeaderOffset,
  IN UINT16  MaxAspmLevel
  )
{
  UINT64  ComponentABaseAddress;
  UINT64  ComponentBBaseAddress;
  UINT16  ComponentAaspm;
  UINT16  ComponentBaspm;
  UINT16  SelectedAspm;
  UINT8   SecBus;
  UINT8   CapHeaderOffset_B;

  ComponentABaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
  ComponentAaspm        = (PciSegmentRead16 (ComponentABaseAddress + CapHeaderOffset + 0x00C) >> 10) & 3;

  SecBus                = PciSegmentRead8 (ComponentABaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
  ComponentBBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, SecBus, 0, 0, 0);
  ComponentBaspm        = 0; // No ASPM Support
  if (PciSegmentRead16 (ComponentBBaseAddress + PCI_DEVICE_ID_OFFSET) != 0xFFFF) {
    CapHeaderOffset_B = PcieFindCapId (TbtSegment, SecBus, 0, 0, 0x10);
    ComponentBaspm    = (PciSegmentRead16 (ComponentBBaseAddress + CapHeaderOffset_B + 0x00C) >> 10) & 3;
  }

  SelectedAspm = FindOptimalAspm (ComponentAaspm, ComponentBaspm);
  SelectedAspm = LimitAspmLevel (SelectedAspm, MaxAspmLevel);
  PciSegmentAndThenOr16 (ComponentABaseAddress + CapHeaderOffset + 0x10, 0xFFFC, SelectedAspm);
}

VOID
ThunderboltEnableAspmWithoutLtr (
  IN   UINT16     MaxAspmLevel,
  IN   UINTN      RpSegment,
  IN   UINTN      RpBus,
  IN   UINTN      RpDevice,
  IN   UINTN      RpFunction
  )
{
  UINT8   Bus;
  UINT8   Dev;
  UINT8   Fun;
  UINT8   RootBus;
  UINT8   RootDev;
  UINT8   RootFun;
  UINT8   MinBus;
  UINT8   MaxBus;
  UINT16  DeviceId;
  UINT64  DeviceBaseAddress;
  UINT8   RegVal;
  UINT8   CapHeaderOffset;
  UINT16  DevicePortType;

  MinBus  = 0;
  MaxBus  = 0;

  MinBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
  MaxBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET));
  DeviceId  = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, MinBus, 0x00, 0x00, PCI_DEVICE_ID_OFFSET));
  if (!(IsTbtHostRouter (DeviceId))) {
    return;
  }

  TbtSegment = (UINT8)RpSegment;

  RootBus = (UINT8)RpBus;
  RootDev = (UINT8)RpDevice;
  RootFun = (UINT8)RpFunction;

  //
  //  Enumerate all the bridges and devices which are available on TBT host controller
  //
  for (Bus = MinBus; Bus <= MaxBus; ++Bus) {
    for (Dev = 0; Dev <= PCI_MAX_DEVICE; ++Dev) {
      //
      // Check for Device availability
      //
      DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, 0, 0);
      if (PciSegmentRead16 (DeviceBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
        //
        // Device not present
        //
        continue;
      }

      RegVal = PciSegmentRead8 (DeviceBaseAddress + PCI_HEADER_TYPE_OFFSET);
      if ((RegVal & BIT7) == 0) {
        //
        // Not a multi-function device
        //
        continue;
      }

      MultiFunctionDeviceAspm(Bus, Dev);
    } //Dev
  } //Bus


  for (Bus = MinBus; Bus <= MaxBus; ++Bus) {
    for (Dev = 0; Dev <= PCI_MAX_DEVICE; ++Dev) {
      for (Fun = 0; Fun <= PCI_MAX_FUNC; ++Fun) {
        //
        // Check for Device availability
        //
        DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
        if (PciSegmentRead16 (DeviceBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
          //
          // Device not present
          //
          continue;
        }

        CapHeaderOffset = PcieFindCapId (TbtSegment, Bus, Dev, Fun, 0x10);
        DevicePortType  = (PciSegmentRead16 (DeviceBaseAddress + CapHeaderOffset + 0x002) >> 4) & 0xF;
        if(PciSegmentRead8 (DeviceBaseAddress + PCI_CLASSCODE_OFFSET) == PCI_CLASS_SERIAL) {
          MaxAspmLevel = (UINT16) 0x1;
        }

        switch (DevicePortType) {
        case 0:
          //
          // PCI Express Endpoint
          //
          EndpointAspmSupport (Bus, Dev, Fun, CapHeaderOffset, MaxBus, MaxAspmLevel);
          break;

        case 1:
          //
          // Legacy PCI Express Endpoint
          //
          EndpointAspmSupport (Bus, Dev, Fun, CapHeaderOffset, MaxBus, MaxAspmLevel);
          break;

        case 4:
          //
          // Root Port of PCI Express Root Complex
          //
          RootportAspmSupport (Bus, Dev, Fun, CapHeaderOffset, MaxAspmLevel);
          break;

        case 5:
          //
          // Upstream Port of PCI Express Switch
          //
          UpstreamAspmSupport (Bus, Dev, Fun, CapHeaderOffset, MaxBus, MaxAspmLevel);
          break;

        case 6:
          //
          // Downstream Port of PCI Express Switch
          //
          DownstreamAspmSupport (Bus, Dev, Fun, CapHeaderOffset, MaxAspmLevel);
          break;

        case 7:
          //
          // PCI Express to PCI/PCI-X Bridge
          //
          NoAspmSupport (Bus, Dev, Fun, CapHeaderOffset);
          break;

        case 8:
          //
          // PCI/PCI-X to PCI Express Bridge
          //
          NoAspmSupport (Bus, Dev, Fun, CapHeaderOffset);
          break;

        case 9:
          //
          // Root Complex Integrated Endpoint
          //
          EndpointAspmSupport (Bus, Dev, Fun, CapHeaderOffset, MaxBus, MaxAspmLevel);
          break;

        case 10:
          //
          // Root Complex Event Collector
          //
          EndpointAspmSupport (Bus, Dev, Fun, CapHeaderOffset, MaxBus, MaxAspmLevel);
          break;

        default:
          break;
        }
        //
        // switch(DevicePortType)
        //
      }
      //
      // Fun
      //
    }
    //
    // Dev
    //
  }
  //
  // Bus
  //
  CapHeaderOffset = PcieFindCapId (TbtSegment, RootBus, RootDev, RootFun, 0x10);
  RootportAspmSupport (RootBus, RootDev, RootFun, CapHeaderOffset, MaxAspmLevel);
}



VOID
ThunderboltEnableL1Sub (
  IN   UINT16     MaxL1Level,
  IN   UINTN      RpSegment,
  IN   UINTN      RpBus,
  IN   UINTN      RpDevice,
  IN   UINTN      RpFunction
  )
{
  UINT16  CapHeaderOffsetExtd;

  RpBus   = 0;

  CapHeaderOffsetExtd = PcieFindExtendedCapId ((UINT8) RpBus, (UINT8) RpDevice, (UINT8) RpFunction, V_PCIE_EX_L1S_CID);
  RootportL1sSupport ((UINT8) RpBus, (UINT8) RpDevice, (UINT8) RpFunction, CapHeaderOffsetExtd, MaxL1Level);
}

VOID
ThunderboltDisableAspmWithoutLtr (
  IN   UINTN      RpSegment,
  IN   UINTN      RpBus,
  IN   UINTN      RpDevice,
  IN   UINTN      RpFunction
  )
{
  UINT8   Bus;
  UINT8   Dev;
  UINT8   Fun;
  UINT8   RootBus;
  UINT8   RootDev;
  UINT8   RootFun;
  UINT8   MinBus;
  UINT8   MaxBus;
  UINT16  DeviceId;
  UINT64  DeviceBaseAddress;
  UINT8   CapHeaderOffset;

  MinBus  = 0;
  MaxBus  = 0;

  MinBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
  MaxBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET));
  DeviceId  = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, MinBus, 0x00, 0x00, PCI_DEVICE_ID_OFFSET));
  if (!(IsTbtHostRouter (DeviceId))) {
    return;
  }

  TbtSegment = (UINT8)RpSegment;
  RootBus = (UINT8)RpBus;
  RootDev = (UINT8)RpDevice;
  RootFun = (UINT8)RpFunction;

  //
  //  Enumerate all the bridges and devices which are available on TBT host controller
  //
  for (Bus = MinBus; Bus <= MaxBus; ++Bus) {
    for (Dev = 0; Dev <= PCI_MAX_DEVICE; ++Dev) {
      for (Fun = 0; Fun <= PCI_MAX_FUNC; ++Fun) {
        //
        // Check for Device availability
        //
        DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
        if (PciSegmentRead16 (DeviceBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
          //
          // Device not present
          //
          continue;
        }

        CapHeaderOffset = PcieFindCapId (TbtSegment, Bus, Dev, Fun, 0x10);
        PciSegmentAndThenOr16 (DeviceBaseAddress + CapHeaderOffset + 0x10, 0xFFFC, 0x00);
      } //Fun
    } //Dev
  } //Bus

  CapHeaderOffset = PcieFindCapId (TbtSegment, RootBus, RootDev, RootFun, 0x10);
  NoAspmSupport(RootBus, RootDev, RootFun, CapHeaderOffset);
}

VOID
TbtProgramClkReq (
  IN        UINT8  Bus,
  IN        UINT8  Device,
  IN        UINT8  Function,
  IN        UINT8  ClkReqSetup
  )
{
  UINT64  DeviceBaseAddress;
  UINT8   CapHeaderOffset;
  UINT16  Data16;

  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Device, Function, 0);
  CapHeaderOffset   = PcieFindCapId (TbtSegment, Bus, Device, Function, 0x10);

  //
  // Check if CLKREQ# is supported
  //
  if ((PciSegmentRead32 (DeviceBaseAddress + CapHeaderOffset + 0x0C) & BIT18) != 0) {
    Data16 = PciSegmentRead16 (DeviceBaseAddress + CapHeaderOffset + 0x010);

    if (ClkReqSetup) {
      Data16 = Data16 | BIT8; // Enable Clock Power Management
    } else {
      Data16 =  Data16 & (UINT16)(~BIT8); // Disable Clock Power Management
    }

    PciSegmentWrite16 (DeviceBaseAddress + CapHeaderOffset + 0x010, Data16);
  }
}
VOID
TbtProgramPtm(
   IN        UINT8  Bus,
   IN        UINT8  Device,
   IN        UINT8  Function,
   IN        UINT8  PtmSetup,
   IN        BOOLEAN IsRoot
)
{
   UINT64  DeviceBaseAddress;
   UINT16  CapHeaderOffset;
   UINT16  PtmControlRegister;
   UINT16  PtmCapabilityRegister;

   DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS(TbtSegment, Bus, Device, Function, 0);
   CapHeaderOffset = PcieFindExtendedCapId(Bus, Device, Function, 0x001F /*V_PCIE_EX_PTM_CID*/);
   if(CapHeaderOffset != 0) {
      PtmCapabilityRegister = PciSegmentRead16(DeviceBaseAddress + CapHeaderOffset + 0x04);
     //
     // Check if PTM Requester/ Responder capability for the EP/Down stream etc
     //
     if ((PtmCapabilityRegister & (BIT1 | BIT0)) != 0) {
        PtmControlRegister = PciSegmentRead16(DeviceBaseAddress + CapHeaderOffset + 0x08);

        if (PtmSetup) {
           PtmControlRegister = PtmControlRegister | BIT0; // Enable PTM
           if(IsRoot) {
             PtmControlRegister = PtmControlRegister | BIT1; // Enable PTM
           }
           PtmControlRegister = PtmControlRegister | (PtmCapabilityRegister & 0xFF00); // Programm Local Clock Granularity
        } else {
           PtmControlRegister = PtmControlRegister & (UINT16)(~(BIT0 | BIT1)); // Disable Clock Power Management
        }

        PciSegmentWrite16(DeviceBaseAddress + CapHeaderOffset + 0x08, PtmControlRegister);
     }
   }
}

VOID
ConfigureTbtPm (
  IN   UINTN      RpSegment,
  IN   UINTN      RpBus,
  IN   UINTN      RpDevice,
  IN   UINTN      RpFunction,
  IN   UINT8      Configuration    // 1- Clk Request , 2- PTM ,
  )
{
  UINT8   Bus;
  UINT8   Dev;
  UINT8   Fun;
  UINT8   MinBus;
  UINT8   MaxBus;
  UINT16  DeviceId;
  UINT64  DeviceBaseAddress;

  MinBus  = 0;
  MaxBus  = 0;

  if ((Configuration != 1) && (Configuration != 2)) {
    return;
  }
  MinBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
  MaxBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET));
  DeviceId  = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, MinBus, 0x00, 0x00, PCI_DEVICE_ID_OFFSET));
  if (!(IsTbtHostRouter (DeviceId))) {
    return;
  }

  TbtSegment = (UINT8)RpSegment;
  //
  //  Enumerate all the bridges and devices which are available on TBT host controller
  //
  for (Bus = MaxBus; Bus >= MinBus; --Bus) {
    for (Dev = 0; Dev <= PCI_MAX_DEVICE; ++Dev) {
      for (Fun = 0; Fun <= PCI_MAX_FUNC; ++Fun) {
        //
        // Check for Device availability
        //
        DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
        if (PciSegmentRead16 (DeviceBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
          if (Fun == 0) {
            //
            // IF Fun is zero, stop enumerating other functions of the particular bridge
            //
            break;
          }
          //
          // otherwise, just skip checking for CLKREQ support
          //
          continue;
        }
        switch (Configuration) {
          case 1:
            TbtProgramClkReq (Bus, Dev, Fun, (UINT8) mTbtNvsAreaPtr->TbtSetClkReq);
            break;
          case 2:
            TbtProgramPtm (Bus, Dev, Fun, (UINT8) mTbtNvsAreaPtr->TbtPtm, FALSE);
            TbtProgramPtm((UINT8) RpBus, (UINT8) RpDevice, (UINT8) RpFunction, (UINT8) mTbtNvsAreaPtr->TbtPtm, TRUE);
            break;
          default:
            break;
        }
      } //Fun
    } // Dev
  } // Bus
}

/**
  1) Check LTR support in device capabilities 2 register (bit 11).
  2) If supported enable LTR in device control 2 register (bit 10).

**/
VOID
TbtProgramLtr (
  IN        UINT8  Bus,
  IN        UINT8  Device,
  IN        UINT8  Function,
  IN        UINT8  LtrSetup
  )
{
  UINT64  DeviceBaseAddress;
  UINT8   CapHeaderOffset;
  UINT16  Data16;

  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Device, Function, 0);
  CapHeaderOffset   = PcieFindCapId (TbtSegment, Bus, Device, Function, 0x10);

  //
  // Check if LTR# is supported
  //
  if ((PciSegmentRead32 (DeviceBaseAddress + CapHeaderOffset + 0x24) & BIT11) != 0) {
    Data16 = PciSegmentRead16 (DeviceBaseAddress + CapHeaderOffset + 0x028);

    if (LtrSetup) {
      Data16 = Data16 | BIT10; // LTR Mechanism Enable
    } else {
      Data16 =  Data16 & (UINT16)(~BIT10); // LTR Mechanism Disable
    }

    PciSegmentWrite16 (DeviceBaseAddress + CapHeaderOffset + 0x028, Data16);
  }
}

VOID
ConfigureLtr (
  IN   UINTN      RpSegment,
  IN   UINTN      RpBus,
  IN   UINTN      RpDevice,
  IN   UINTN      RpFunction
  )
{
  UINT8   Bus;
  UINT8   Dev;
  UINT8   Fun;
  UINT8   MinBus;
  UINT8   MaxBus;
  UINT16  DeviceId;
  UINT64  DeviceBaseAddress;

  MinBus  = 0;
  MaxBus  = 0;

  MinBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
  MaxBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET));
  DeviceId  = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, MinBus, 0x00, 0x00, PCI_DEVICE_ID_OFFSET));
  if (!(IsTbtHostRouter (DeviceId))) {
    return;
  }

  TbtSegment = (UINT8)RpSegment;
  //
  //  Enumerate all the bridges and devices which are available on TBT host controller
  //
  for (Bus = MinBus; Bus <= MaxBus; ++Bus) {
    for (Dev = 0; Dev <= PCI_MAX_DEVICE; ++Dev) {
      for (Fun = 0; Fun <= PCI_MAX_FUNC; ++Fun) {
        //
        // Check for Device availability
        //
        DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
        if (PciSegmentRead16 (DeviceBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
          if (Fun == 0) {
            //
            // IF Fun is zero, stop enumerating other functions of the particular bridge
            //
            break;
          }
          //
          // otherwise, just skip checking for LTR support
          //
          continue;
        }

        TbtProgramLtr (Bus, Dev, Fun, (UINT8) mTbtNvsAreaPtr->TbtLtr);

      } //Fun
    } // Dev
  } // Bus
  TbtProgramLtr ((UINT8) RpBus, (UINT8) RpDevice, (UINT8) RpFunction, (UINT8) mTbtNvsAreaPtr->TbtLtr);
}

/*
  US ports and endpoints which declare support must also have the LTR capability structure (cap ID 18h).
  In this structure you need to enter the max snoop latency and max non-snoop latency in accordance with the format specified in the PCIe spec.
  The latency value itself is platform specific so you'll need to get it from the platform architect or whatever.
*/
VOID
ThunderboltGetLatencyLtr (
  VOID
  )
{
  PCH_SERIES       PchSeries;

  PchSeries = GetPchSeries ();

  if(gCurrentDiscreteTbtRootPortType == DTBT_TYPE_PEG) {
  // PEG selector
  TbtLtrMaxSnoopLatency = LTR_MAX_SNOOP_LATENCY_VALUE;
  TbtLtrMaxNoSnoopLatency = LTR_MAX_NON_SNOOP_LATENCY_VALUE;
  } else if (gCurrentDiscreteTbtRootPortType == DTBT_TYPE_PCH) {
  // PCH selector

    if (PchSeries == PchLp) {
      TbtLtrMaxSnoopLatency = 0x1003;
      TbtLtrMaxNoSnoopLatency = 0x1003;
    }
    if (PchSeries == PchH) {
      TbtLtrMaxSnoopLatency = 0x0846;
      TbtLtrMaxNoSnoopLatency = 0x0846;
    }
  }
}

VOID
SetLatencyLtr (
  IN UINT8   Bus,
  IN UINT8   Dev,
  IN UINT8   Fun,
  IN UINT16  CapHeaderOffsetExtd,
  IN UINT16  LtrMaxSnoopLatency,
  IN UINT16  LtrMaxNoSnoopLatency
  )
{
  UINT64 DeviceBaseAddress;
  if(CapHeaderOffsetExtd == 0) {
    return;
  }
  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
  PciSegmentWrite16 (DeviceBaseAddress + CapHeaderOffsetExtd + 0x004, LtrMaxSnoopLatency);
  PciSegmentWrite16 (DeviceBaseAddress + CapHeaderOffsetExtd + 0x006, LtrMaxNoSnoopLatency);
}

VOID
ThunderboltSetLatencyLtr (
  IN   UINTN      RpSegment,
  IN   UINTN      RpBus,
  IN   UINTN      RpDevice,
  IN   UINTN      RpFunction
  )
{
  UINT8   Bus;
  UINT8   Dev;
  UINT8   Fun;
  UINT8   MinBus;
  UINT8   MaxBus;
  UINT16  DeviceId;
  UINT64  DeviceBaseAddress;
  UINT8   CapHeaderOffsetStd;
  UINT16  CapHeaderOffsetExtd;
  UINT16  DevicePortType;

  MinBus  = 0;
  MaxBus  = 0;

  MinBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
  MaxBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET));
  DeviceId  = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, MinBus, 0x00, 0x00, PCI_DEVICE_ID_OFFSET));
  if (!(IsTbtHostRouter (DeviceId))) {
    return;
  }

  TbtSegment = (UINT8)RpSegment;

  for (Bus = MinBus; Bus <= MaxBus; ++Bus) {
    for (Dev = 0; Dev <= PCI_MAX_DEVICE; ++Dev) {
      for (Fun = 0; Fun <= PCI_MAX_FUNC; ++Fun) {
        //
        // Check for Device availability
        //
        DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
        if (PciSegmentRead16 (DeviceBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
          //
          // Device not present
          //
          continue;
        }

        CapHeaderOffsetStd = PcieFindCapId (TbtSegment, Bus, Dev, Fun, 0x10);
        DevicePortType  = (PciSegmentRead16 (DeviceBaseAddress + CapHeaderOffsetStd + 0x002) >> 4) & 0xF;

        CapHeaderOffsetExtd = PcieFindExtendedCapId (Bus, Dev, Fun, 0x0018);

        switch (DevicePortType) {
        case 0:
          //
          // PCI Express Endpoint
          //
          SetLatencyLtr (Bus, Dev, Fun, CapHeaderOffsetExtd, TbtLtrMaxSnoopLatency, TbtLtrMaxNoSnoopLatency);
          break;

        case 1:
          //
          // Legacy PCI Express Endpoint
          //
          SetLatencyLtr (Bus, Dev, Fun, CapHeaderOffsetExtd, TbtLtrMaxSnoopLatency, TbtLtrMaxNoSnoopLatency);
          break;

        case 4:
          //
          // Root Port of PCI Express Root Complex
          //
          // Do-nothing
          break;

        case 5:
          //
          // Upstream Port of PCI Express Switch
          //
          SetLatencyLtr (Bus, Dev, Fun, CapHeaderOffsetExtd, TbtLtrMaxSnoopLatency, TbtLtrMaxNoSnoopLatency);
          break;

        case 6:
          //
          // Downstream Port of PCI Express Switch
          //
          // Do-nothing
          break;

        case 7:
          //
          // PCI Express to PCI/PCI-X Bridge
          //
          // Do-nothing
          break;

        case 8:
          //
          // PCI/PCI-X to PCI Express Bridge
          //
          // Do-nothing
          break;

        case 9:
          //
          // Root Complex Integrated Endpoint
          //
          // Do-nothing
          break;

        case 10:
          //
          // Root Complex Event Collector
          //
          // Do-nothing
          break;

        default:
          break;
        }
        //
        // switch(DevicePortType)
        //
      }
      //
      // Fun
      //
    }
    //
    // Dev
    //
  }
  //
  // Bus
  //
}

static
VOID
Stall (
  UINTN     Usec
  )
{
  UINTN   Index;
  UINT32  Data32;
  UINT32  PrevData;
  UINTN   Counter;

  Counter = (UINTN) ((Usec * 10) / 3);
  //
  // Call WaitForTick for Counter + 1 ticks to try to guarantee Counter tick
  // periods, thus attempting to ensure Microseconds of stall time.
  //
  if (Counter != 0) {

    PrevData = IoRead32 (PcdGet16 (PcdAcpiBaseAddress) + R_PCH_ACPI_PM1_TMR);
    for (Index = 0; Index < Counter;) {
      Data32 = IoRead32 (PcdGet16 (PcdAcpiBaseAddress) + R_PCH_ACPI_PM1_TMR);
      if (Data32 < PrevData) {
        //
        // Reset if there is a overlap
        //
        PrevData = Data32;
        continue;
      }

      Index += (Data32 - PrevData);
      PrevData = Data32;
    }
  }

  return ;
}
/**
  Called during Sx entry, initates TbtSetPcie2TbtCommand HandShake to set GO2SX_NO_WAKE
  for Tbt devices if WakeupSupport is not present.

  @param[in] DispatchHandle         - The unique handle assigned to this handler by SmiHandlerRegister().
  @param[in] DispatchContext        - Points to an optional handler context which was specified when the
                                      handler was registered.
  @param[in, out] CommBuffer        - A pointer to a collection of data in memory that will
                                      be conveyed from a non-SMM environment into an SMM environment.
  @param[in, out] CommBufferSize    - The size of the CommBuffer.

  @retval EFI_SUCCESS               - The interrupt was handled successfully.
**/
EFI_STATUS
EFIAPI
SxDTbtEntryCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer OPTIONAL,
  IN  UINTN                         *CommBufferSize OPTIONAL
  )
{
  UINT16          DeviceId;
  UINT8           CableConnected;
  UINT8           RootportSelected;
  UINT8           HoustRouteBus;
  volatile UINT32 *PowerState;
  UINT32          PowerStatePrev;
  BOOLEAN         SecSubBusAssigned;
  UINT64          DeviceBaseAddress;
  UINT8           CapHeaderOffset;
  UINTN           RpDev;
  UINTN           RpFunc;
  EFI_STATUS      Status;
  UINT32          Timeout;
  UINT32          RegisterValue;
  UINT64          Tbt2Pcie;
  UINTN           Index;
  UINT32          TbtCioPlugEventGpioNo;
  UINT32          TbtFrcPwrGpioNo;
  UINT8           TbtFrcPwrGpioLevel;
  UINT32          TbtPcieRstGpioNo;
  UINT8           TbtPcieRstGpioLevel;
  EFI_SMM_SX_REGISTER_CONTEXT   *EntryDispatchContext;

  CableConnected    = 0;
  HoustRouteBus     = 3;
  SecSubBusAssigned = FALSE;
  Timeout = 600;
  RootportSelected      = 0;
  TbtCioPlugEventGpioNo = 0;
  TbtFrcPwrGpioNo       = 0;
  TbtFrcPwrGpioLevel    = 0;
  TbtPcieRstGpioNo      = 0;
  TbtPcieRstGpioLevel   = 0;
  Index = 0;

  EntryDispatchContext = (EFI_SMM_SX_REGISTER_CONTEXT*) DispatchContext;

//  CableConnected = GetTbtHostRouterStatus ();
  //SaveTbtHostRouterStatus (CableConnected & 0xF0);
  //
  // Get the Power State and Save
  //
  if (((mTbtNvsAreaPtr->DTbtControllerEn0 == 0) && (Index == 0)))  {

  RootportSelected      = mTbtNvsAreaPtr->RootportSelected0;
  TbtCioPlugEventGpioNo = mTbtNvsAreaPtr->TbtCioPlugEventGpioNo0;
  TbtFrcPwrGpioNo       = mTbtNvsAreaPtr->TbtFrcPwrGpioNo0;
  TbtFrcPwrGpioLevel    = mTbtNvsAreaPtr->TbtFrcPwrGpioLevel0;
  TbtPcieRstGpioNo      = mTbtNvsAreaPtr->TbtPcieRstGpioNo0;
  TbtPcieRstGpioLevel   = mTbtNvsAreaPtr->TbtPcieRstGpioLevel0;
  }

  Status = GetDTbtRpDevFun (gCurrentDiscreteTbtRootPortType, RootportSelected - 1, &RpDev, &RpFunc);
  ASSERT_EFI_ERROR (Status);
  CapHeaderOffset = PcieFindCapId (TbtSegment, 0x00, (UINT8)RpDev, (UINT8)RpFunc, 0x01);
  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, 0x00, (UINT32)RpDev, (UINT32)RpFunc, 0);
  PowerState        = &*((volatile UINT32 *) (mPciExpressBaseAddress + DeviceBaseAddress + CapHeaderOffset + 4)); //PMCSR
  PowerStatePrev    = *PowerState;
  *PowerState &= 0xFFFFFFFC;

  HoustRouteBus = PciSegmentRead8 (DeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
  //
  // Check the Subordinate bus .If it is Zero ,assign temporary bus to
  // find the device presence .
  //
  if (HoustRouteBus == 0) {
    PciSegmentWrite8 (DeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, 0xF0);
    PciSegmentWrite8 (DeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, 0xF0);
    HoustRouteBus     = 0xF0;
    SecSubBusAssigned = TRUE;
  }
  //
  // Clear Interrupt capability of TBT CIO Plug Event Pin to make sure no SCI is getting generated,
  // This GPIO will be reprogrammed while resuming as part of Platform GPIO Programming.
  //
  GpioSetPadInterruptConfig (TbtCioPlugEventGpioNo, GpioIntDis);
  //
  // Read the TBT Host router DeviceID
  //
  DeviceId = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (TbtSegment, HoustRouteBus, 0, 0, PCI_DEVICE_ID_OFFSET));

  //
  // Check For HostRouter Presence
  //
  if (IsTbtHostRouter (DeviceId)) {
    //    CableConnected = GetTbtHostRouterStatus ();
    if (!((CableConnected & (DTBT_SAVE_STATE_OFFSET << Index)) == (DTBT_SAVE_STATE_OFFSET << Index))) {
      CableConnected = CableConnected | (DTBT_SAVE_STATE_OFFSET << Index);
   //     SaveTbtHostRouterStatus (CableConnected);
    }
  }

  //
  // Check value of Tbt2Pcie reg, if Tbt is not present, bios needs to apply force power prior to sending mailbox command
  //
  GET_TBT2PCIE_REGISTER_ADDRESS(TbtSegment, HoustRouteBus, 0x00, 0x00, Tbt2Pcie)
  RegisterValue = PciSegmentRead32 (Tbt2Pcie);
  if (0xFFFFFFFF == RegisterValue) {

    GpioWrite (TbtFrcPwrGpioNo,TbtFrcPwrGpioLevel);

    while (Timeout -- > 0) {
      RegisterValue = PciSegmentRead32 (Tbt2Pcie);
      if (0xFFFFFFFF != RegisterValue) {
        break;
      }
      Stall(1* (UINTN)1000);
    }
    //
    // Before entering Sx state BIOS should execute GO2SX/NO_WAKE mailbox command for AIC.
    // However BIOS shall not execute go2sx mailbox command on S5/reboot cycle.
    //

    if( (EntryDispatchContext->Type == SxS3) || (EntryDispatchContext->Type == SxS4))
    {
      if(!mTbtNvsAreaPtr->TbtWakeupSupport) {
        //Wake Disabled, GO2SX_NO_WAKE Command
        TbtSetPcie2TbtCommand (PCIE2TBT_GO2SX_NO_WAKE, HoustRouteBus, 0, 0, TBT_5S_TIMEOUT);
      } else {
        //Wake Enabled, GO2SX Command
        TbtSetPcie2TbtCommand (PCIE2TBT_GO2SX, HoustRouteBus, 0, 0, TBT_5S_TIMEOUT);
      }
    }
    if (mTbtNvsAreaPtr->TbtFrcPwrEn == 0) {
      GpioWrite (TbtFrcPwrGpioNo,!(TbtFrcPwrGpioLevel));
    }
  } else {
    //
    // Before entering Sx state BIOS should execute GO2SX/NO_WAKE mailbox command for AIC.
    // However BIOS shall not execute go2sx mailbox command on S5/reboot cycle.
    //
    if( (EntryDispatchContext->Type == SxS3) || (EntryDispatchContext->Type == SxS4))
    {
      if(!mTbtNvsAreaPtr->TbtWakeupSupport) {
        //Wake Disabled, GO2SX_NO_WAKE Command
        TbtSetPcie2TbtCommand (PCIE2TBT_GO2SX_NO_WAKE, HoustRouteBus, 0, 0, TBT_5S_TIMEOUT);
      } else {
        //Wake Enabled, GO2SX Command
        TbtSetPcie2TbtCommand (PCIE2TBT_GO2SX, HoustRouteBus, 0, 0, TBT_5S_TIMEOUT);
      }
    }
  }
  *PowerState = PowerStatePrev;
  //
  // Restore the bus number in case we assigned temporarily
  //
  if (SecSubBusAssigned) {
    PciSegmentWrite8 (DeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, 0x00);
    PciSegmentWrite8 (DeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, 0x00);
  }
  if (gDTbtPcieRstSupport) {
    GpioWrite (TbtPcieRstGpioNo,TbtPcieRstGpioLevel);
  }
  return EFI_SUCCESS;
}

VOID
ThunderboltSwSmiCallback (
  IN UINT8 Type
  )
{
  UINT8 ThunderboltSmiFunction;

  DEBUG ((DEBUG_INFO, "ThunderboltSwSmiCallback Entry\n"));
  ThunderboltSmiFunction = mTbtNvsAreaPtr->ThunderboltSmiFunction;
  DEBUG ((DEBUG_INFO, "ThunderboltSwSmiCallback. ThunderboltSmiFunction=%d\n", ThunderboltSmiFunction));
  if (Type == DTBT_CONTROLLER) {
    gCurrentDiscreteTbtRootPort     = mTbtNvsAreaPtr->CurrentDiscreteTbtRootPort;
    gCurrentDiscreteTbtRootPortType = mTbtNvsAreaPtr->CurrentDiscreteTbtRootPortType;
  }

  switch (ThunderboltSmiFunction) {
  case 21:
    ThunderboltCallback (Type);
    break;

  case 22:
    TbtDisablePCIDevicesAndBridges (Type);
    break;

  case 23:
    ConfigureTbtAspm (Type, (UINT16) 0x02);
    break;

  case 24:
    ConfigureTbtAspm (Type, (UINT16) 0x01);
    break;

  default:
    break;
  }
  DEBUG ((DEBUG_INFO, "ThunderboltSwSmiCallback Exit.\n"));
}
STATIC
EFI_STATUS
EFIAPI
DiscreteThunderboltSwSmiCallback (
  IN EFI_HANDLE                     DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer OPTIONAL,
  IN  UINTN                         *CommBufferSize OPTIONAL
  )
{
  ThunderboltSwSmiCallback(DTBT_CONTROLLER);
  return EFI_SUCCESS;
}
EFI_STATUS
TbtRegisterHandlers (
  IN BOOLEAN Type
  )
{
  EFI_STATUS                    Status;
  UINTN                         SmiInputValue;
  EFI_SMM_HANDLER_ENTRY_POINT2   SxHandler;
  EFI_SMM_HANDLER_ENTRY_POINT2   SwHandler;
  EFI_SMM_SX_DISPATCH2_PROTOCOL *SxDispatchProtocol;
  EFI_SMM_SW_DISPATCH2_PROTOCOL *SwDispatch;
  EFI_SMM_SX_REGISTER_CONTEXT   EntryDispatchContext;
  EFI_SMM_SW_REGISTER_CONTEXT   SwContext;
  EFI_HANDLE                    SwDispatchHandle;
  EFI_HANDLE                    S3DispatchHandle;
  EFI_HANDLE                    S4DispatchHandle;
  EFI_HANDLE                    S5DispatchHandle;

  Status = EFI_UNSUPPORTED;

  if(Type == DTBT_CONTROLLER) {
    SxHandler = SxDTbtEntryCallback;
    SwHandler = DiscreteThunderboltSwSmiCallback;
    SmiInputValue = PcdGet8 (PcdSwSmiDTbtEnumerate);
    gDTbtPcieRstSupport = gTbtInfoHob->DTbtCommonConfig.PcieRstSupport;
    Status = EFI_SUCCESS;
  }
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SwDispatchHandle        = NULL;
  S3DispatchHandle        = NULL;
  S4DispatchHandle        = NULL;
  S5DispatchHandle        = NULL;

   Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmSxDispatch2ProtocolGuid,
                    NULL,
                    (VOID **) &SxDispatchProtocol
                    );
  ASSERT_EFI_ERROR (Status);
  //
  // Register S3 entry phase call back function
  //
  EntryDispatchContext.Type   = SxS3;
  EntryDispatchContext.Phase  = SxEntry;
  Status = SxDispatchProtocol->Register (
                                SxDispatchProtocol,
                                SxHandler,
                                &EntryDispatchContext,
                                &S3DispatchHandle
                                );
  ASSERT_EFI_ERROR (Status);
  //
  // Register S4 entry phase call back function
  //
  EntryDispatchContext.Type   = SxS4;
  EntryDispatchContext.Phase  = SxEntry;
  Status = SxDispatchProtocol->Register (
                                SxDispatchProtocol,
                                SxHandler,
                                &EntryDispatchContext,
                                &S4DispatchHandle
                                );
  ASSERT_EFI_ERROR (Status);
  //
  // Register S5 entry phase call back function
  //
  EntryDispatchContext.Type   = SxS5;
  EntryDispatchContext.Phase  = SxEntry;
  Status = SxDispatchProtocol->Register (
                                SxDispatchProtocol,
                                SxHandler,
                                &EntryDispatchContext,
                                &S5DispatchHandle
                                );
  ASSERT_EFI_ERROR (Status);
  //
  // Locate the SMM SW dispatch protocol
  //
  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmSwDispatch2ProtocolGuid,
                    NULL,
                    (VOID **) &SwDispatch
                    );

  ASSERT_EFI_ERROR (Status);
  //
  // Register SWSMI handler
  //
  SwContext.SwSmiInputValue = SmiInputValue;
  Status = SwDispatch->Register (
                        SwDispatch,
                        SwHandler,
                        &SwContext,
                        &SwDispatchHandle
                        );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
EFI_STATUS
InSmmFunction (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                    Status;

  Status = EFI_SUCCESS;

  Status = TbtRegisterHandlers(DTBT_CONTROLLER);
  return Status;
}

EFI_STATUS
EFIAPI
TbtSmmEntryPoint (
  IN EFI_HANDLE               ImageHandle,
  IN EFI_SYSTEM_TABLE         *SystemTable
  )
{
  TBT_NVS_AREA_PROTOCOL         *TbtNvsAreaProtocol;
  EFI_STATUS                    Status;

  DEBUG ((DEBUG_INFO, "TbtSmmEntryPoint\n"));

  mPciExpressBaseAddress = PcdGet64 (PcdPciExpressBaseAddress);
  //
  // Locate Tbt shared data area
  //
  Status = gBS->LocateProtocol (&gTbtNvsAreaProtocolGuid, NULL, (VOID **) &TbtNvsAreaProtocol);
  ASSERT_EFI_ERROR (Status);
  mTbtNvsAreaPtr = TbtNvsAreaProtocol->Area;

  //
  // Get TBT INFO HOB
  //
  gTbtInfoHob = (TBT_INFO_HOB *) GetFirstGuidHob (&gTbtInfoHobGuid);
  if (gTbtInfoHob == NULL) {
    return EFI_NOT_FOUND;
  }

  return InSmmFunction (ImageHandle, SystemTable);
}

VOID
EndOfThunderboltCallback (
  IN   UINTN      RpSegment,
  IN   UINTN      RpBus,
  IN   UINTN      RpDevice,
  IN   UINTN      RpFunction
  )
{
  if(mTbtNvsAreaPtr->TbtL1SubStates != 0) {
    ThunderboltEnableL1Sub (mTbtNvsAreaPtr->TbtL1SubStates, RpSegment, RpBus, RpDevice, RpFunction);
  }
  ConfigureTbtPm(RpSegment, RpBus, RpDevice, RpFunction, 1);
  if (!mTbtNvsAreaPtr->TbtAspm) { //Aspm disable case
    ThunderboltDisableAspmWithoutLtr (RpSegment, RpBus, RpDevice, RpFunction);
  } else { //Aspm enable case
    ThunderboltEnableAspmWithoutLtr ((UINT16)mTbtNvsAreaPtr->TbtAspm, RpSegment, RpBus, RpDevice, RpFunction);
  }

  if (mTbtNvsAreaPtr->TbtLtr) {
    ThunderboltGetLatencyLtr ();
    ThunderboltSetLatencyLtr (RpSegment, RpBus, RpDevice, RpFunction);
  }
  ConfigureLtr (RpSegment, RpBus, RpDevice, RpFunction);
  ConfigureTbtPm(RpSegment, RpBus, RpDevice, RpFunction, 2);
} // EndOfThunderboltCallback

VOID
ConfigureTbtAspm (
  IN UINT8        Type,
  IN UINT16       Aspm
  )
{
  UINTN                         RpSegment = 0;
  UINTN                         RpBus = 0;
  UINTN                         RpDevice;
  UINTN                         RpFunction;

  if(Type == DTBT_CONTROLLER) {
    if (gCurrentDiscreteTbtRootPort == 0) {
      return;
    }
    GetDTbtRpDevFun(DTBT_CONTROLLER, gCurrentDiscreteTbtRootPort - 1, &RpDevice, &RpFunction);

    ConfigureTbtPm (RpSegment, RpBus, RpDevice, RpFunction, 1);
    if (!mTbtNvsAreaPtr->TbtAspm) { //Aspm disable case
      ThunderboltDisableAspmWithoutLtr (RpSegment, RpBus, RpDevice, RpFunction);
    } else { //Aspm enable case
      ThunderboltEnableAspmWithoutLtr ((UINT16) Aspm, RpSegment, RpBus, RpDevice, RpFunction);
    }

  if (mTbtNvsAreaPtr->TbtLtr) {
      ThunderboltGetLatencyLtr ();
      ThunderboltSetLatencyLtr (RpSegment, RpBus, RpDevice, RpFunction);
    }
    ConfigureLtr (RpSegment, RpBus, RpDevice, RpFunction);
  } // EndOfThunderboltCallback
}

