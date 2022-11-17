/*++

Copyright (c) 2006 - 2009, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2015, Hisilicon Limited. All rights reserved.<BR>
Copyright (c) 2015, Linaro Limited. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

  MiscIpmiDeviceInformationData.c

Abstract:

  This file provide OEM to define Smbios Type38 Data

Based on files under Nt32Pkg/MiscSubClassPlatformDxe/
**/

#include "SmbiosMisc.h"

MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE38, MiscIpmiDeviceInformation) =
{
  {                                                       // Header
   EFI_SMBIOS_TYPE_IPMI_DEVICE_INFORMATION,                 // Type;
   0,                                                       // Length;
   0                                                        // Handle;
  },
  IPMIDeviceInfoInterfaceTypeUnknown,                         // InterfaceType
  0x00,                                                   // Ipmi Specification Revision
  0,                                                      // I2CSlaveAddress
  0xFF,                                                   // NvStorageDeviceAddress
  0x88,                                                   // BaseAddress
  0,                                                      // BaseAddressModifier/InterruptInfo
  0                                                       // InterruptNumber
};

