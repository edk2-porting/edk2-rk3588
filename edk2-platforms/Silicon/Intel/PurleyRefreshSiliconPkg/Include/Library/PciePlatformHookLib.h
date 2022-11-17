/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __PCIE_PLATFORM_HOOK_LIB_H__
#define __PCIE_PLATFORM_HOOK_LIB_H__

typedef enum {
  PcieInitStart,
  BeforeBifurcation,
  AfterBifurcation,
  BeforePortInit,
  AfterPortInit,
  PcieInitEnd
} PCIE_HOOK_EVENT;

EFI_STATUS
EFIAPI
PciePlatformHookEvent (
  IN PCIE_HOOK_EVENT    Event,
  IN VOID               *Context
  );

#endif
