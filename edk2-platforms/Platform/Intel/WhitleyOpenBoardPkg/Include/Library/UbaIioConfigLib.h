/** @file
  UBA IIO Config Library Header File.

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UBA_IIO_CONFIG_LIB_H
#define _UBA_IIO_CONFIG_LIB_H

#include <Base.h>
#include <Uefi.h>

#include <IioPlatformData.h>

#define PLATFORM_IIO_CONFIG_UPDATE_SIGNATURE  SIGNATURE_32 ('P', 'I', 'I', 'O')
#define PLATFORM_IIO_CONFIG_UPDATE_VERSION      01
#define PLATFORM_IIO_CONFIG_UPDATE_VERSION_2    02
#define PLATFORM_IIO_CONFIG_UPDATE_VERSION_3    03
#define PLATFORM_IIO_CONFIG_UPDATE_VERSION_UNSUPPORTED   0x20

// {EB35ED63-EACA-4e29-9516-7EDF1F818837}
#define   PLATFORM_IIO_CONFIG_DATA_GUID \
{ 0xeb35ed63, 0xeaca, 0x4e29, { 0x95, 0x16, 0x7e, 0xdf, 0x1f, 0x81, 0x88, 0x37 } }

// {3093F83B-5934-473e-8523-24BF297EE684}
#define   PLATFORM_IIO_CONFIG_DATA_GUID_1 \
{ 0x3093f83b, 0x5934, 0x473e, { 0x85, 0x23, 0x24, 0xbf, 0x29, 0x7e, 0xe6, 0x84 } }

// {1C5267A4-634B-4bf2-BFF8-9A1164E6D198}
#define   PLATFORM_IIO_CONFIG_DATA_GUID_2 \
{ 0x1c5267a4, 0x634b, 0x4bf2, { 0xbf, 0xf8, 0x9a, 0x11, 0x64, 0xe6, 0xd1, 0x98 } }

// {1E486CCA-048E-4702-B28C-0B677201683A}
#define   PLATFORM_IIO_CONFIG_DATA_GUID_3 \
{ 0x1e486cca, 0x48e, 0x4702, { 0xb2, 0x8c, 0xb, 0x67, 0x72, 0x1, 0x68, 0x3a } }

// {6FE6C559-4F35-4111-98E1-332A251512F3}
#define   PLATFORM_IIO_CONFIG_DATA_DXE_GUID \
{ 0x6fe6c559, 0x4f35, 0x4111, { 0x98, 0xe1, 0x33, 0x2a, 0x25, 0x15, 0x12, 0xf3 } }

// {0F722F2A-650F-448a-ABB7-04EECD75BB30}
#define   PLATFORM_IIO_CONFIG_DATA_DXE_GUID_1 \
{ 0xf722f2a, 0x650f, 0x448a, { 0xab, 0xb7, 0x4, 0xee, 0xcd, 0x75, 0xbb, 0x30 } }

// {EBD11A00-8C5C-4f71-BB9E-5394032B01F4}
#define   PLATFORM_IIO_CONFIG_DATA_DXE_GUID_2 \
{ 0xebd11a00, 0x8c5c, 0x4f71, { 0xbb, 0x9e, 0x53, 0x94, 0x3, 0x2b, 0x1, 0xf4 } }

// {123BD082-3201-465c-B139-0CB8C77208F8}
#define   PLATFORM_IIO_CONFIG_DATA_DXE_GUID_3 \
{ 0x123bd082, 0x3201, 0x465c, { 0xb1, 0x39, 0xc, 0xb8, 0xc7, 0x72, 0x8, 0xf8 } }

#define ENABLE            1
#define DISABLE           0
#define NO_SLT_IMP        0xFF
#define SLT_IMP           1
#define HIDE              1
#define NOT_HIDE          0
#define VPP_PORT_MAX      0xFF
#define VPP_ADDR_MAX      0xFF
#define PWR_VAL_MAX       0xFF
#define PWR_SCL_MAX       0xFF
#define SMB_ADDR_MAX      0xFF
#define SMB_DATA_MAX      0xFF
#define NO_BIF_INPUT      0xFF
#define NOT_EXIST         0xFF

typedef
EFI_STATUS
(*IIO_VAR_UPDATE_CALLBACK) (
  IN  IIO_GLOBALS             *IioGlobalData
);

typedef struct _PLATFORM_IIO_BIFURCATION_ENTRY {
  UINT8 Socket;
  UINT8 IouNumber;
  UINT8 Bifurcation;
} IIO_BIFURCATION_DATA_ENTRY;

typedef struct _PLATFORM_IIO_BIFURCATION_ENTRY_EX {
  UINT8 Socket;
  UINT8 IouNumber;
  UINT8 Bifurcation;
  UINT8 ExtnCardSMBusPort;      //SMBus Port for the PCIe extension card - use to dynamically determine PCIe bifurcation
  UINT8 ExtnCardSMBusAddress;   //SMBus address for the PCIe extension card - use to dynamically determine PCIe bifurcation
  UINT8 MuxSMBusAddress;        // SMBus address for the Mux - used to communicate to different group of devices
  UINT8 MuxSMBusChannel;        // SMBus channel for the Mux - used to select the channel that will be used to communicate to the different group of devices.
} IIO_BIFURCATION_DATA_ENTRY_EX;

typedef struct _PLATFORM_IIO_SLOT_ENTRY {
  UINT8   PortIndex;
  UINT16  SlotNumber;     // 0xff if slot not implemented , Slot number if slot implemented
  BOOLEAN InterLockPresent;
  UINT8   SlotPowerLimitScale;
  UINT8   SlotPowerLimitValue;
  BOOLEAN HotPlugCapable;
  UINT8   VppPort;        // 0xff if Vpp not enabled
  UINT8   VppAddress;
  BOOLEAN PcieSSDCapable;
  UINT8   PcieSSDVppPort; // 0xff if Vpp not enabled
  UINT8   PcieSSDVppAddress;
  BOOLEAN Hidden;
} IIO_SLOT_CONFIG_DATA_ENTRY;

typedef struct _PLATFORM_IIO_SLOT_ENTRY_EX {
  UINT8   PortIndex;          //( {1,2,3,4} = {1A,,1B,,1C,1D} ), ( {5,6,7,8} = {2A,2B,2C,2D} ), ( {9,10,11,12} = {3A,3B,3C,3D}),...
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
  BOOLEAN SRIS;                // Yes / No - in case of distinct clocking whether the separate ref. clk supports SSC or not
  BOOLEAN UplinkPortConnected; // Yes / No - indicate the PCIe RP is connected to Uplink port of another chip
  BOOLEAN RetimerConnected;    // Yes / No - BIOS would have overhead to bifurcate the retimers explicitly
  UINT8   RetimerSMBusAddress; // SMBus address to read the retimer status and bifurcate if required
  UINT8   RetimerSMBusChannel; // SMBus Mux channel to read the retimer status and bifurcate if required
  UINT8   RetimerWidth;        // Retimer width to determine adjacent Retimers for the same PCIe slot
  UINT8   MuxSMBusAddress;     // SMBus address for the Mux - used to communicate to different group of devices
  UINT8   MuxSMBusChannel;     // SMBus channel for the Mux - used to select the channel that will be used to communicate to the different group of devices.

  BOOLEAN ExtensionCardSupport;   // Yes / No, any PCIe Port extension card which are supported in board thro' SMBus address (typically BW5)
  UINT8   ExtnCardSMBusPort;      //SMBus Port for the PCIe extension card - use to dynamically determine PCIe bifurcation
  UINT8   ExtnCardSMBusAddress;   //SMBus address for the PCIe extension card - use to dynamically determine PCIe bifurcation
  BOOLEAN ExtnCardRetimerSupport; //yes - retimer on this PCIe extension card (BW5), or No
  UINT8   ExtnCardRetimerSMBusAddress; // SMBus address to read the retimer status and bifurcate if required
  UINT8   ExtnCardRetimerWidth;   // use to determine adjacent Retimers to the x16 PCIe slot on which this Riser is mounted
  BOOLEAN ExtnCardHotPlugCapable; // yes / No, independent of board, indicates slot in which this PCIe extn. Card is mounted
  UINT8   ExtnCardHPVppPort;      // 0xff if VPP not enabled
  UINT8   ExtnCardHPVppAddress;   // 0xff if VPP not enabled

  UINT8   RetimerConnectCount;    // number of Retimers (1 or 2) intercepted between the PCIe port and the slot device. Retimer may appear  mutually exclusive.
} IIO_SLOT_CONFIG_DATA_ENTRY_EX;


typedef struct _PLATFORM_IIO_CONFIG_UPDATE_TABLE {
  UINT32                            Signature;
  UINT32                            Version;
  IIO_BIFURCATION_DATA_ENTRY        *IioBifurcationTablePtr;
  UINTN                             IioBifurcationTableSize;
  IIO_VAR_UPDATE_CALLBACK           CallUpdate;
  IIO_SLOT_CONFIG_DATA_ENTRY        *IioSlotTablePtr;
  UINTN                             IioSlotTableSize;
} PLATFORM_IIO_CONFIG_UPDATE_TABLE;

typedef struct _PLATFORM_IIO_CONFIG_UPDATE_TABLE_EX {
  UINT32                            Signature;
  UINT32                            Version;
  IIO_BIFURCATION_DATA_ENTRY_EX     *IioBifurcationTablePtr;
  UINTN                             IioBifurcationTableSize;
  IIO_VAR_UPDATE_CALLBACK           CallUpdate;
  IIO_SLOT_CONFIG_DATA_ENTRY_EX     *IioSlotTablePtr;
  UINTN                             IioSlotTableSize;
} PLATFORM_IIO_CONFIG_UPDATE_TABLE_EX;

typedef  struct {
  UINT8 MuxSmbAddress;
  UINT8 MuxSmbChannel;
  UINT8 SmbAddress0;
  UINT8 SmbAddress1;
  UINT8 RegOffset;
  UINT8 RegValue;
} MUX_SMB_ADDR;

typedef struct _PLATFORM_IIO_CONFIG_UPDATE_TABLE_3 {
  UINT32                            Signature;
  UINT32                            Version;
  IIO_BIFURCATION_DATA_ENTRY_EX     *IioBifurcationTablePtr;
  UINTN                             IioBifurcationTableSize;
  IIO_VAR_UPDATE_CALLBACK           CallUpdate;
  IIO_SLOT_CONFIG_DATA_ENTRY_EX     *IioSlotTablePtr;
  UINTN                             IioSlotTableSize;
  MUX_SMB_ADDR                      *RetimerInitTablePtr;
} PLATFORM_IIO_CONFIG_UPDATE_TABLE_3;

typedef struct {
  UINT8     PortHide[8];
} PCIE_PORT_HIDE_TABLE;

EFI_STATUS
PlatformIioConfigInit (
  IN OUT IIO_BIFURCATION_DATA_ENTRY    **BifurcationTable,
  IN OUT UINT8                         *BifurcationEntries,
  IN OUT IIO_SLOT_CONFIG_DATA_ENTRY    **SlotTable,
  IN OUT UINT8                         *SlotEntries
);

EFI_STATUS
PlatformIioConfigInit2 (
  IN     UINT8                         SkuPersonalityType,
  IN OUT IIO_BIFURCATION_DATA_ENTRY    **BifurcationTable,
  IN OUT UINT8                         *BifurcationEntries,
  IN OUT IIO_SLOT_CONFIG_DATA_ENTRY    **SlotTable,
  IN OUT UINT8                         *SlotEntries
);

EFI_STATUS
PlatformUpdateIioConfig (
  IN  IIO_GLOBALS             *IioGlobalData
);

EFI_STATUS
PlatformUpdateIioConfig_EX (
  IN  IIO_GLOBALS             *IioGlobalData
);

STATIC  EFI_GUID gPlatformIioConfigDataGuid = PLATFORM_IIO_CONFIG_DATA_GUID;
STATIC  EFI_GUID gPlatformIioConfigDataGuid_1 = PLATFORM_IIO_CONFIG_DATA_GUID_1;
STATIC  EFI_GUID gPlatformIioConfigDataGuid_2 = PLATFORM_IIO_CONFIG_DATA_GUID_2;
STATIC  EFI_GUID gPlatformIioConfigDataGuid_3 = PLATFORM_IIO_CONFIG_DATA_GUID_3;

STATIC  EFI_GUID gPlatformIioConfigDataDxeGuid = PLATFORM_IIO_CONFIG_DATA_DXE_GUID;
STATIC  EFI_GUID gPlatformIioConfigDataDxeGuid_1 = PLATFORM_IIO_CONFIG_DATA_DXE_GUID_1;
STATIC  EFI_GUID gPlatformIioConfigDataDxeGuid_2 = PLATFORM_IIO_CONFIG_DATA_DXE_GUID_2;
STATIC  EFI_GUID gPlatformIioConfigDataDxeGuid_3 = PLATFORM_IIO_CONFIG_DATA_DXE_GUID_3;

#endif //_UBA_IIO_CONFIG_LIB_H
