/** @file
  UBA Hsio Ptss Table Config Library Header File.

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UBA_HSIO_PTSS_TABLE_CONFIG_LIB_H
#define _UBA_HSIO_PTSS_TABLE_CONFIG_LIB_H

#include <Base.h>
#include <Uefi.h>
#include <PiPei.h>
#include <Uefi/UefiSpec.h>
#include <Ppi/PchPolicy.h>
#include <PchSetupVariable.h>

#define PLATFORM_HSIO_PTSS_TABLE_SIGNATURE  SIGNATURE_32 ('P', 'H', 'P', 'T')
#define PLATFORM_HSIO_PTSS_TABLE_VERSION    01

// {47EA4CA7-F89A-42E6-89F0-20F4B72BA616}
#define   PLATFORM_HSIO_PTSS_TABLE_GUID \
{ 0x47ea4ca7, 0xf89a, 0x42e6, { 0x89, 0xf0, 0x20, 0xf4, 0xb7, 0x2b, 0xa6, 0x16 } }

typedef
VOID
(*HSIO_PTSS_TABLE_CONFIG_CALLBACK) (
  IN          PCH_SETUP                    *PchSetup,
  IN OUT      PCH_POLICY_PPI               *PchPolicy

);

typedef struct {
  UINT32                              Signature;
  UINT32                              Version;

  HSIO_PTSS_TABLE_CONFIG_CALLBACK     CallUpdate;

} PLATFORM_HSIO_PTSS_CONFIG_TABLE;

EFI_STATUS
InstallPlatformHsioPtssTable (
  IN          PCH_SETUP                    *PchSetup,
  IN OUT      PCH_POLICY_PPI               *PchPolicy
);

STATIC  EFI_GUID gPlatformHsioPtssTableGuid = PLATFORM_HSIO_PTSS_TABLE_GUID;

#endif //_UBA_HSIO_PTSS_TABLE_CONFIG_LIB_H

