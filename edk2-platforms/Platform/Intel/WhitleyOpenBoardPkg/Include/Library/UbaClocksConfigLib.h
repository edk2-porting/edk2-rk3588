/** @file
  UBA Clocks Config Library Header File.

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UBA_CLOCKS_CONFIG_LIB_H
#define _UBA_CLOCKS_CONFIG_LIB_H

#include <Base.h>
#include <Uefi.h>
#include <PiPei.h>
#include <Uefi/UefiSpec.h>

#define PLATFORM_CLOCKS_CONFIG_SIGNATURE  SIGNATURE_32 ('P', 'C', 'L', 'K')
#define PLATFORM_CLOCKS_CONFIG_VERSION    01

// {34F1B964-49C7-4CB6-B9BD-7043B37C90BE}
#define   PLATFORM_CLOCKS_CONFIG_DATA_GUID \
{ 0x34f1b964, 0x49c7, 0x4cb6, { 0xb9, 0xbd, 0x70, 0x43, 0xb3, 0x7c, 0x90, 0xbe } }

typedef
EFI_STATUS
(*CLOCKS_CONFIG_CALLBACK) (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDescriptor,
  IN VOID                               *Ppi
);

typedef struct {
  UINT32                  Signature;
  UINT32                  Version;

  CLOCKS_CONFIG_CALLBACK     CallUpdate;

} PLATFORM_CLOCKS_CONFIG_TABLE;

EFI_STATUS
ConfigurePlatformClock (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDescriptor,
  IN VOID                               *SmbusPpi
);

STATIC  EFI_GUID gPlatformClocksConfigDataGuid = PLATFORM_CLOCKS_CONFIG_DATA_GUID;

#endif //_UBA_CLOCKS_CONFIG_LIB_H

