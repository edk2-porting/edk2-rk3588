/** @file
  MemBootHealthGuid.h

 Header for using Structured PCD in MemBootHealth

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MEM_BOOT_HEALTH_GUID_H_
#define _MEM_BOOT_HEALTH_GUID_H_

// Enable Mem boot health check feature
#define MEM_BOOT_HEALTH_ENABLE 1
// Disable Mem boot health check feature
#define MEM_BOOT_HEALTH_DISABLE 0

// Mem Boot Health check option
// Option to choose Mem Boot Health configuration type.
// 00=>Auto (Use defaults)
#define MEM_BOOT_HEALTH_CONFIG_AUTO 0
// 01=>Manual(Override defaults with setup option)
#define MEM_BOOT_HEALTH_CONFIG_MANUAL 1
// 02=>Disable (Disable feature)
#define MEM_BOOT_HEALTH_CONFIG_DISABLE 2

#define ENABLE_REBOOT_ON_CRITICAL_FAILURE 1
#define DISABLE_REBOOT_ON_CRITICAL_FAILURE 0

#pragma pack(1)
typedef struct {

  //
  // Memory Boot Health Check
  //
  UINT8                   MemBootHealthVisible;    // 0 => Hide Memory boot health check option, 1 => Enable the option in setup

  UINT8                   MemBootHealthCheck;     // 0=>Auto, 1=>Manual and 2=>Disabled Memory Boot Health Check

  UINT8                   ResetOnCriticalError;   // 0 => Dont reboot on critical error, 1 = Reboot on critical error
  //
  // Memory Boot Health check parameters
  //
  UINT8                   WarningTxDqDelayLeftEdge;
  UINT8                   WarningTxDqDelayRightEdge;
  UINT8                   WarningTxVrefLeftEdge;
  UINT8                   WarningTxVrefRightEdge;
  UINT8                   WarningRxDqsDelayLeftEdge;
  UINT8                   WarningRxDqsDelayRightEdge;
  UINT8                   WarningRxVrefLeftEdge;
  UINT8                   WarningRxVrefRightEdge;

  UINT8                   CriticalTxDqDelayLeftEdge;
  UINT8                   CriticalTxDqDelayRightEdge;
  UINT8                   CriticalTxVrefLeftEdge;
  UINT8                   CriticalTxVrefRightEdge;
  UINT8                   CriticalRxDqsDelayLeftEdge;
  UINT8                   CriticalRxDqsDelayRightEdge;
  UINT8                   CriticalRxVrefLeftEdge;
  UINT8                   CriticalRxVrefRightEdge;
} MEM_BOOT_HEALTH_CONFIG;
#pragma pack()

#define MEM_BOOT_HEALTH_GUID { 0xACD56900, 0xDEFC, 0x4127, { 0xDE, 0x12, 0x32, 0xA0, 0xD2, 0x69, 0x46, 0x2F } }

#define MEM_BOOT_HEALTH_SETUP_STR  L"MemBootHealthConfig"

extern EFI_GUID gMemBootHealthGuid;
#endif // _MEM_BOOT_HEALTH_GUID_H_