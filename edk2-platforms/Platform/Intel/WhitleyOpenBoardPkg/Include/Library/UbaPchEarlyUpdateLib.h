/** @file

  @copyright
  Copyright 2008 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_PCH_EARLY_UPDATE_LIB_H
#define _PLATFORM_PCH_EARLY_UPDATE_LIB_H


#include <Base.h>
#include <PiPei.h>

#include <Guid/SetupVariable.h>

#define PLATFORM_PCH_EARLY_UPDATE_SIGNATURE  SIGNATURE_32 ('P', 'P', 'C', 'H')
#define PLATFORM_PCH_EARLY_UPDATE_VERSION    01


// {1763F1D2-6A47-43f8-8279-3765A6929060}
#define   PLATFORM_PCH_EARLY_CONFIG_DATA_GUID \
{ 0x1763f1d2, 0x6a47, 0x43f8, { 0x82, 0x79, 0x37, 0x65, 0xa6, 0x92, 0x90, 0x60 } }


typedef
EFI_STATUS
(*PLATFORM_PCH_LAN_CONFIG) (
  IN SYSTEM_CONFIGURATION         *SystemConfig
);

typedef
EFI_STATUS
(*PLATFORM_EARLY_INIT_HOOK) (
  IN SYSTEM_CONFIGURATION         *SystemConfig
);

typedef struct
{
  UINT32                          Signature;
  UINT32                          Version;

  PLATFORM_PCH_LAN_CONFIG         ConfigLan;
  PLATFORM_EARLY_INIT_HOOK        InitLateHook;

} PLATFORM_PCH_EARLY_UPDATE_TABLE;


EFI_STATUS
PlatformPchLanConfig (
  IN SYSTEM_CONFIGURATION         *SystemConfig
);

EFI_STATUS
PlatformInitLateHook (
  IN SYSTEM_CONFIGURATION         *SystemConfig
);


STATIC  EFI_GUID gPlatformPchEarlyConfigDataGuid = PLATFORM_PCH_EARLY_CONFIG_DATA_GUID;

#endif //_PLATFORM_PCH_EARLY_UPDATE_LIB_H
