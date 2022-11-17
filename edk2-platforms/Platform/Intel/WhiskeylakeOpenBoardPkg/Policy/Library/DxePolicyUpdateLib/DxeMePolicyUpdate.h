/** @file

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DXE_ME_POLICY_UPDATE_H_
#define _DXE_ME_POLICY_UPDATE_H_

#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Guid/EventGroup.h>
#include <IndustryStandard/Acpi10.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/MePolicy.h>
#include <Library/HobLib.h>
#include <ConfigBlock/MePeiConfig.h>

#define PLATFORM_BOOT_TABLE_PTR_TYPE   0x1001
#define PLATFORM_BOOT_RECORD_TYPE      0x1022
//
// Timeout values based on HPET
//
#define HECI_MSG_DELAY                 2000000  ///< show warning msg and stay for 2 seconds.
#define CONVERSION_MULTIPLIER          1000000  ///< msec to nanosec multiplier
#define PLATFORM_BOOT_TABLE_SIGNATURE  SIGNATURE_32 ('P', 'B', 'P', 'T')

//
// Platform Boot Performance Table Record
//

typedef struct {
  UINT16 Type;
  UINT8  Length;
  UINT8  Revision;
  UINT32 Reserved;
  UINT64 TimestampDelta1;
  UINT64 TimestampDelta2;
  UINT64 TimestampDelta3;
} PLATFORM_BOOT_TABLE_RECORD;

//
// Platform boot Performance Table
//

typedef struct {
  EFI_ACPI_COMMON_HEADER     Header;
  PLATFORM_BOOT_TABLE_RECORD PlatformBoot;
} PLATFORM_BOOT_PERFORMANCE_TABLE;

/**
  Update ME Policy while MePlatformProtocol is installed.

  @param[in] MePolicyInstance     Instance of ME Policy Protocol

**/
VOID
UpdateMePolicyFromMeSetup (
  IN ME_POLICY_PROTOCOL           *MePolicyInstance
  );

/**
  Update ME Policy if Setup variable exists.

  @param[in, out] MePolicyInstance     Instance of ME Policy Protocol

**/
VOID
UpdateMePolicyFromSetup (
  IN OUT ME_POLICY_PROTOCOL     *MePolicyInstance
  );

/**
  Functions performs HECI exchange with FW to update MePolicy settings.

  @param[in] Event         A pointer to the Event that triggered the callback.
  @param[in] Context       A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
UpdateMeSetupCallback (
  IN  EFI_EVENT                       Event,
  IN  VOID                            *Context
  );

#endif

