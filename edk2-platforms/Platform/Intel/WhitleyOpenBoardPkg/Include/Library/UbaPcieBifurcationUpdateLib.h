/** @file
  PCH PCIe Bifurcation Update Library Header File.

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UBA_PCIE_BIFURCATION_UPDATE_LIB_H
#define _UBA_PCIE_BIFURCATION_UPDATE_LIB_H

#include <Base.h>
#include <Uefi.h>
#include <Library/PchPcieRpLib.h>

#define PLATFORM_PCH_PCIE_BIFURCATION_UPDATE_SIGNATURE  SIGNATURE_32 ('P', 'P', 'C', 'I')
#define PLATFORM_PCH_PCIE_BIFURCATION_UPDATE_VERSION    01

typedef struct {
  UINT8   PortIndex;
  UINT16  SlotNumber;         // 0xff if slot not implemented , Slot number if slot implemented
  BOOLEAN InterLockPresent;   // Yes / No
  UINT8   SlotPowerLimitScale;
  UINT8   SlotPowerLimitValue;
  BOOLEAN HotPlugCapable;     // Yes / No
  UINT8   VppPort;            // 0xff if Vpp not enabled
  UINT8   VppAddress;
  BOOLEAN PcieSSDCapable;     // Yes / No
  UINT8   PcieSSDVppPort;     // 0xff if Vpp not enabled
  UINT8   PcieSSDVppAddress;  // 0xff if Vpp not enabled
  BOOLEAN Hidden;             // deprecate this as it should be purely based on bifurcation

  BOOLEAN CommonClock;         // Yes / No - whether the both side of the link are in same clock domain or not
  BOOLEAN UplinkPortConnected; // Yes / No - indicate the PCIe RP is connected to Uplink port of another chip
  BOOLEAN RetimerConnected;    // Yes / No - BIOS would have overhead to bifurcate the retimers explicitly
  UINT8   RetimerSMBusAddress; // SNBus address to read the retimer status and bifurcate if required

  BOOLEAN ExtensionCardSupport;   // Yes / No, any PCIe Port extension card which are supported in board thro' SMBus address (typically BW5)
  UINT8   ExtnCardSMBusPort;      //SMBus Port for the PCIe extension card - use to dynamically determine PCIe bifurcation
  UINT8   ExtnCardSMBusAddress;   //SNBus address for the PCIe extension card - use to dynamically determine PCIe bifurcation
  BOOLEAN ExtnCardRetimerSupport; //yes - retimer on this PCIe extension card (BW5), or No
  UINT8   ExtnCardRetimerSMBusAddress; // SNBus address to read the retimer status and bifurcate if required
  BOOLEAN ExtnCardHotPlugCapable; // yes / No, independent of board, indicates slot in which this PCIe extn. Card is mounted
  UINT8   ExtnCardHPVppPort;      // 0xff if VPP not enabled
  UINT8   ExtnCardHPVppAddress;   // 0xff if VPP not enabled

  UINT8   RetimerConnectCount;    // number of Retimers (1 or 2) intercepted between the PCIe port and the slot device. Retimer may appear  mutually exclusive.
} PCH_SLOT_CONFIG_DATA_ENTRY_EX;

// {187576ac-fec1-41bf-91f6-7d1ace7f2bee}
#define   PLATFORM_UBA_PCIE_BIFURCATION_GUID \
{ 0x187576ac, 0xfec1, 0x41bf, { 0x91, 0xf6, 0x7d, 0x1a, 0xce, 0x7f, 0x2b, 0xee } }

typedef
EFI_STATUS
(*PCIE_BIFURCATION_UPDATE_CALLBACK) (
  IN OUT   PCIE_BIFURCATION_CONFIG         **PchPcieBifurcationConfig,
  IN OUT   PCH_SLOT_CONFIG_DATA_ENTRY_EX   **PchSlotConfig
);

typedef struct _PLATFORM_PCH_PCIE_BIFURCATION_UPDATE_TABLE{
  UINT32                               Signature;
  UINT32                               Version;
  PCIE_BIFURCATION_UPDATE_CALLBACK     CallPcieBifurcationUpdate;
} PLATFORM_PCH_PCIE_BIFURCATION_UPDATE_TABLE;

#define ENABLE            1
#define DISABLE           0
#define NO_SLT_IMP        0xFF
#define SLT_IMP           1
#define HIDE              1
#define NOT_HIDE          0
#define VPP_PORT_0        0
#define VPP_PORT_1        1
#define VPP_PORT_MAX      0xFF
#define VPP_ADDR_MAX      0xFF
#define PWR_VAL_MAX       0xFF
#define PWR_SCL_MAX       0xFF
#define SMB_ADDR_MAX      0xFF
#define NO_BIF_INPUT      0xFF
#define PORT_0_INDEX      0
#define PORT_1_INDEX      1
#define PORT_2_INDEX      2
#define PORT_3_INDEX      3
#define PORT_4_INDEX      4
#define PORT_5_INDEX      5
#define PORT_6_INDEX      6
#define PORT_7_INDEX      7
#define PORT_8_INDEX      8
#define PORT_9_INDEX      9
#define PORT_10_INDEX     10
#define PORT_11_INDEX     11
#define PORT_12_INDEX     12
#define PORT_13_INDEX     13
#define PORT_14_INDEX     14
#define PORT_15_INDEX     15
#define PORT_16_INDEX     16 // Added dummy ports(16-27) TODO_FHF
#define PORT_17_INDEX     17
#define PORT_18_INDEX     18
#define PORT_19_INDEX     19
#define PORT_20_INDEX     20
#define PORT_21_INDEX     21
#define PORT_22_INDEX     22
#define PORT_23_INDEX     23
#define PORT_24_INDEX     24
#define PORT_25_INDEX     25
#define PORT_26_INDEX     26
#define PORT_27_INDEX     27

//-----------------------------------------------------------------------------------
// PCIE port index for SKX
//------------------------------------------------------------------------------------
#define SOCKET_0_INDEX           0
#define SOCKET_1_INDEX           21
#define SOCKET_2_INDEX           42
#define SOCKET_3_INDEX           63
#define SOCKET_4_INDEX           84
#define SOCKET_5_INDEX           105
#define SOCKET_6_INDEX           126
#define SOCKET_7_INDEX           147

EFI_STATUS
PlatformGetPchPcieBifurcationConfig (
  IN OUT   PCIE_BIFURCATION_CONFIG         **PchPcieBifurcationConfig,
  IN OUT   PCH_SLOT_CONFIG_DATA_ENTRY_EX   **PchSlotConfig
);
STATIC  EFI_GUID gPlatformUbaPcieBifurcationGuid = PLATFORM_UBA_PCIE_BIFURCATION_GUID;

#endif //_UBA_PCIE_BIFURCATION_UPDATE_LIB_H
