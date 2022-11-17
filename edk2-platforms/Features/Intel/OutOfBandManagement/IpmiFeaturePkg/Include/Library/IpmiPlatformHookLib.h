/** @file
  This library abstract the platform specific hook for IPMI.

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _IPMI_PLATFORM_UPDATE_LIB_H_
#define _IPMI_PLATFORM_UPDATE_LIB_H_

#include <Uefi.h>
#include <IndustryStandard/Acpi.h>
#include <Protocol/Smbios.h>

EFI_STATUS
EFIAPI
PlatformIpmiIoRangeSet(
  UINT16 IpmiIoBase
  );

#endif

