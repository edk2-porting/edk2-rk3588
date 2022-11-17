/** @file
  UBA Slot Update Library Header File.

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UBA_SLOT_UPDATE_LIB_H
#define _UBA_SLOT_UPDATE_LIB_H

#include <Base.h>
#include <Uefi.h>

#define PLATFORM_SLOT_UPDATE_SIGNATURE  SIGNATURE_32 ('P', 'S', 'L', 'T')
#define PLATFORM_SLOT_UPDATE_VERSION    01

// {BE1CC570-03FC-4a44-8068-5B6E36CAEBB2}
#define   PLATFORM_SLOT_DATA_GUID \
{ 0xbe1cc570, 0x03fc, 0x4a44, { 0x80, 0x68, 0x5b, 0x6e, 0x36, 0xca, 0xeb, 0xb2 } }

// {226763AE-972C-4e3c-80D1-73B25E8CBBA3}
#define   PLATFORM_SLOT_DATA_GUID2 \
{ 0x226763ae, 0x972c, 0x4e3c, { 0x80, 0xd1, 0x73, 0xb2, 0x5e, 0x8c, 0xbb, 0xa3 } };

// {56F676D3-24DF-4c77-A336-009DAE693598}
#define   PLATFORM_SLOT_DATA_GUID2_1 \
{ 0x56f676d3, 0x24df, 0x4c77, { 0xa3, 0x36, 0x01, 0x9d, 0xae, 0x69, 0x35, 0x98 } };


// {B93613E1-48F0-4b32-B3A8-4FEDFC7C1365}
#define   PLATFORM_SLOT_DATA_DXE_GUID \
{ 0xb93613e1, 0x48f0, 0x4b32, { 0xb3, 0xa8, 0x4f, 0xed, 0xfc, 0x7c, 0x13, 0x65 } }

// {8185B70E-9A20-4fc4-A1D6-77D54A736518}
#define   PLATFORM_SLOT_DATA_DXE_GUID2 \
{ 0x8185b70e, 0x9a20, 0x4fc4, { 0xa1, 0xd6, 0x77, 0xd5, 0x4a, 0x73, 0x65, 0x18 } };

// {A87C540B-3D69-4c3b-B7F7-6383589C21CE}
#define   PLATFORM_SLOT_DATA_DXE_GUID2_1 \
{ 0xa87c540b, 0x3d69, 0x4c3b, { 0xb7, 0xf7, 0x63, 0x83, 0x58, 0x9c, 0x21, 0xce } };

// {B4CB70B3-558D-4478-84CA-22616034EA16}
#define   PLATFORM_PCI_SLOT_IMPLEMENTED_GUID \
{ 0xb4cb70b3, 0x558d, 0x4478, { 0x84, 0xca, 0x22, 0x61, 0x60, 0x34, 0xea, 0x16 } };

typedef struct _IIO_BROADWAY_ADDRESS_DATA_ENTRY {
  UINT8 Socket;
  UINT8 IouNumber;
  UINT8 BroadwayAddress; // 0xff, no override bifurcation settings.
                         // 0-2 BW5 card can be present
} IIO_BROADWAY_ADDRESS_DATA_ENTRY;

typedef
UINT8
(*PLATFORM_GET_IOU_SETTING) (
  IN UINT8      IOU2Data
);

typedef
UINT8
(*PLATFORM_GET_IOU2_SETTING) (
  IN UINT8      SkuPersonalityType,
  IN UINT8      IOU2Data
);

typedef struct _PLATFORM_SLOT_UPDATE_TABLE {
  UINT32                            Signature;
  UINT32                            Version;
  IIO_BROADWAY_ADDRESS_DATA_ENTRY   *BroadwayTablePtr;
  PLATFORM_GET_IOU_SETTING          GetIOU2Setting;
  UINT8                             FlagValue;
} PLATFORM_SLOT_UPDATE_TABLE;

typedef struct _PLATFORM_SLOT_UPDATE_TABLE2 {
  UINT32                            Signature;
  UINT32                            Version;
  IIO_BROADWAY_ADDRESS_DATA_ENTRY   *BroadwayTablePtr;
  PLATFORM_GET_IOU_SETTING          GetIOU0Setting;
  UINT8                             FlagValue;
  PLATFORM_GET_IOU2_SETTING         GetIOU2Setting;
} PLATFORM_SLOT_UPDATE_TABLE2;

typedef struct _PLATFORM_PCH_PCI_SLOT_IMPLEMENTED_UPDATE_TABLE {
  UINT32                            Signature;
  UINT32                            Version;
  UINT8                             *SlotImplementedTableDataPtr;
} PLATFORM_PCH_PCI_SLOT_IMPLEMENTED_UPDATE_TABLE;

EFI_STATUS
PlatformGetSlotTableData (
  IN OUT IIO_BROADWAY_ADDRESS_DATA_ENTRY  **BroadwayTable,
  IN OUT UINT8                            *IOU2Setting,
  IN OUT UINT8                            *FlagValue
);

EFI_STATUS
PlatformGetSlotTableData2 (
  IN OUT IIO_BROADWAY_ADDRESS_DATA_ENTRY  **BroadwayTable,
  IN OUT UINT8                            *IOU0Setting,
  IN OUT UINT8                            *FlagValue,
  IN OUT UINT8                            *IOU2Setting,
  IN     UINT8                            SkuPersonalityType
);

EFI_STATUS
PlatformPchGetPciSlotImplementedTableData (
  IN OUT UINT8                            **SlotImplementedTable
);

STATIC  EFI_GUID gPlatformSlotDataGuid = PLATFORM_SLOT_DATA_GUID;
STATIC  EFI_GUID gPlatformSlotDataGuid2 = PLATFORM_SLOT_DATA_GUID2;
STATIC  EFI_GUID gPlatformSlotDataGuid2_1 = PLATFORM_SLOT_DATA_GUID2_1;


STATIC  EFI_GUID gPlatformSlotDataDxeGuid = PLATFORM_SLOT_DATA_DXE_GUID;
STATIC  EFI_GUID gPlatformSlotDataDxeGuid2 = PLATFORM_SLOT_DATA_DXE_GUID2;
STATIC  EFI_GUID gPlatformSlotDataDxeGuid2_1 = PLATFORM_SLOT_DATA_DXE_GUID2_1;

STATIC  EFI_GUID gPlatformPciSlotImplementedGuid = PLATFORM_PCI_SLOT_IMPLEMENTED_GUID;


#endif //_UBA_SLOT_UPDATE_LIB_H
