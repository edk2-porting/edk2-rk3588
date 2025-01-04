/** @file
 *
 *  Copyright (c) 2024-2025, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __EXIT_BOOT_SERVICES_OS_NOTIFY_H__
#define __EXIT_BOOT_SERVICES_OS_NOTIFY_H__

#define EXIT_BOOT_SERVICES_OS_NOTIFY_PROTOCOL_GUID \
  { 0x8c254127, 0x3fd5, 0x4b75, { 0xbb, 0x13, 0x22, 0xe8, 0xb5, 0x3e, 0x64, 0x03 } }

typedef struct _EXIT_BOOT_SERVICES_OS_NOTIFY_PROTOCOL EXIT_BOOT_SERVICES_OS_NOTIFY_PROTOCOL;

typedef enum {
  ExitBootServicesOsUnknown = 0,
  ExitBootServicesOsWindows,
  ExitBootServicesOsLinux,
  ExitBootServicesOsMax
} EXIT_BOOT_SERVICES_OS_TYPE;

typedef struct {
  EXIT_BOOT_SERVICES_OS_TYPE    OsType;
  EFI_PHYSICAL_ADDRESS          OsLoaderAddress;
  EFI_PHYSICAL_ADDRESS          ReturnAddress;
} EXIT_BOOT_SERVICES_OS_CONTEXT;

typedef
VOID
(EFIAPI *EXIT_BOOT_SERVICES_OS_HANDLER)(
  IN EXIT_BOOT_SERVICES_OS_CONTEXT     *Context
  );

typedef
EFI_STATUS
(EFIAPI *EXIT_BOOT_SERVICES_OS_NOTIFY_REGISTER)(
  IN EXIT_BOOT_SERVICES_OS_NOTIFY_PROTOCOL  *This,
  IN EXIT_BOOT_SERVICES_OS_HANDLER          Handler
  );

typedef
EFI_STATUS
(EFIAPI *EXIT_BOOT_SERVICES_OS_NOTIFY_UNREGISTER)(
  IN EXIT_BOOT_SERVICES_OS_NOTIFY_PROTOCOL  *This,
  IN EXIT_BOOT_SERVICES_OS_HANDLER          Handler
  );

struct _EXIT_BOOT_SERVICES_OS_NOTIFY_PROTOCOL {
  EXIT_BOOT_SERVICES_OS_NOTIFY_REGISTER      RegisterHandler;
  EXIT_BOOT_SERVICES_OS_NOTIFY_UNREGISTER    UnregisterHandler;
};

extern EFI_GUID  gExitBootServicesOsNotifyProtocolGuid;

#endif
