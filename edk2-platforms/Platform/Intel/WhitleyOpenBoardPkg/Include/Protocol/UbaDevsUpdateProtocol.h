/** @file

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLT_DEVS_UPDATE_H_
#define _PLT_DEVS_UPDATE_H_

// {1E22C6FA-B39E-419a-A071-D60B672B21C8}
#define EFI_PLT_DEVS_UPDATE_GUID \
  { \
   0x1e22c6fa, 0xb39e, 0x419a, { 0xa0, 0x71, 0xd6, 0xb, 0x67, 0x2b, 0x21, 0xc8 }\
  }

#define  PORTNUMMAX          8

typedef struct _EFI_PLT_DEVS_UPDATE_PROTOCOL EFI_PLT_DEVS_UPDATE_PROTOCOL;

typedef enum {
  NET_PXE_1GB               = 0,
  NET_PXE_10GB              = 1,
  NET_ISCSI                 = 2,
  NET_FCOE                  = 3,
  NET_INFINITBADN           = 4
} OPROM_FILE_TYPE;

typedef enum {
  HIDDEN                    = 0,
  ONBOARD_NIC               = 1,
  IO_MODULE_NIC             = 2,
} NIC_TYPE;

typedef enum {
  UNDEF                     = 0,
  ETHERNET                  = 1,
  INFIBAND                  = 2
} NIC_SUB_TYPE;

#define DFT_MAC_SIZE         0x20
#define DFT_GUID_SIZE        0x64

#define PXE1GBit             0x1
#define PXE10GBit            0x2
#define FcoeBit              0x4
#define iSCSIBit             0x8

#pragma pack(1)
typedef struct _PLAT_NIC_SETUP_INFO{
  NIC_TYPE             NicType;               //Onboard or IO module
  NIC_SUB_TYPE         NicSubType;            //Ethernect or Infinitband controller
  UINT8                NicIndex;              //Onboard Nic1,2,3 or IOM 1, 2,3 per setup option
  UINT8                RootPortBusNo;         //Root Bridge Bus No
  UINT8                RootPortDevNo;         //Root Bridge device No
  UINT8                RootPortFunNo;         //Root Bridge Function No
  UINT16               NicVID;                //Nic Vendor ID
  UINT16               NicDID;                //Nic Device ID
  UINT16               SubDID;                //Nic Subsystem ID
  UINT8                PortNumbers;           //Ports numbder after detection
  CHAR8                NicDescription[64];    //Nic description defined in Eps
  EFI_MAC_ADDRESS      PortMacAddress[PORTNUMMAX];
  EFI_GUID             InfinitbandGuid;
  UINT8                OpROMCapMap;
  UINT8                IsPchNIC;
}PLAT_NIC_SETUP_INFO;
#pragma pack()

typedef EFI_STATUS (*PLATFORM_HKS_GET_EMBEDED_OPTIONROM) (IN EFI_PLT_DEVS_UPDATE_PROTOCOL   *This, IN EFI_HANDLE PciHandle,OPROM_FILE_TYPE OpRomType);
typedef EFI_STATUS (*PLATFORM_HKS_DISPATCH_OPTIONROM) (IN EFI_PLT_DEVS_UPDATE_PROTOCOL   *This, IN EFI_HANDLE PciHandle);
typedef EFI_STATUS (*PLATFORM_HKS_BDS_UPDATE_MAC) (IN EFI_PLT_DEVS_UPDATE_PROTOCOL   *This);
typedef EFI_STATUS (*PLATFORM_HKS_ON_ENTER_SETUP) (IN EFI_PLT_DEVS_UPDATE_PROTOCOL   *This, OUT PLAT_NIC_SETUP_INFO **NicInfo, OUT UINT8 *NicNum);

typedef struct _EFI_PLT_DEVS_UPDATE_PROTOCOL {
  PLATFORM_HKS_GET_EMBEDED_OPTIONROM           PlatformHooksOnGettingEmbedOpRom;
  PLATFORM_HKS_DISPATCH_OPTIONROM              PlatformHooksOnDispatchOpRom;
  PLATFORM_HKS_BDS_UPDATE_MAC                  PlatformHooksBdsUpdateMac;
  PLATFORM_HKS_ON_ENTER_SETUP                  PlatformHooksGetNicInfo;
} EFI_PLT_DEVS_UPDATE_PROTOCOL;

extern EFI_GUID   gEfiPlatformDevsUpdateProtocolGuid;
extern EFI_GUID   gEfiVMDDriverProtocolGuid;
extern EFI_GUID   gEfiHfiPcieGen3ProtocolGuid;

#endif
