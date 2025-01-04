/** @file
 *
 *  Copyright (c) 2024-2025, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __EXIT_BOOT_SERVICES_HOOK_H__
#define __EXIT_BOOT_SERVICES_HOOK_H__

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Protocol/ExitBootServicesOsNotify.h>

typedef struct {
  UINT32                           Signature;
  LIST_ENTRY                       Link;
  EXIT_BOOT_SERVICES_OS_HANDLER    Handler;
} EXIT_BOOT_SERVICES_OS_HANDLER_ENTRY;
#define EXIT_BOOT_SERVICES_OS_HANDLER_ENTRY_SIGNATURE  SIGNATURE_32('E', 'b', 'S', 'h')
#define EXIT_BOOT_SERVICES_OS_HANDLER_ENTRY_FROM_LINK(a)  \
  CR (a, EXIT_BOOT_SERVICES_OS_HANDLER_ENTRY, Link, EXIT_BOOT_SERVICES_OS_HANDLER_ENTRY_SIGNATURE)

typedef struct {
  UINT32                                   Signature;
  EXIT_BOOT_SERVICES_OS_NOTIFY_PROTOCOL    Notify;
  LIST_ENTRY                               Handlers;
} EXIT_BOOT_SERVICES_OS_NOTIFY_INSTANCE;
#define EXIT_BOOT_SERVICES_OS_NOTIFY_INSTANCE_SIGNATURE  SIGNATURE_32('E', 'b', 'S', 'n')
#define EXIT_BOOT_SERVICES_OS_NOTIFY_INSTANCE_FROM_THIS(a) \
  CR (a, EXIT_BOOT_SERVICES_OS_NOTIFY_INSTANCE, Notify, EXIT_BOOT_SERVICES_OS_NOTIFY_INSTANCE_SIGNATURE)

EFI_PHYSICAL_ADDRESS
FindPeImageBase (
  IN EFI_PHYSICAL_ADDRESS  Base
  );

EXIT_BOOT_SERVICES_OS_TYPE
IdentifyOsType (
  IN EFI_PHYSICAL_ADDRESS  OsLoaderAddress
  );

CHAR8 *
OsTypeToString (
  IN EXIT_BOOT_SERVICES_OS_TYPE  OsType
  );

#endif
