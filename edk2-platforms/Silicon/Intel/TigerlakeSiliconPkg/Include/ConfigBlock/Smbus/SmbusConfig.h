/** @file
  Smbus policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SMBUS_CONFIG_H_
#define _SMBUS_CONFIG_H_

#define SMBUS_PREMEM_CONFIG_REVISION 1
extern EFI_GUID gSmbusPreMemConfigGuid;

#pragma pack (push,1)

#define PCH_MAX_SMBUS_RESERVED_ADDRESS 128

///
/// The SMBUS_CONFIG block lists the reserved addresses for non-ARP capable devices in the platform.
///
typedef struct {
  /**
    Revision 1: Init version
  **/
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  /**
    This member describes whether or not the SMBus controller of PCH should be enabled.
    0: Disable; <b>1: Enable</b>.
  **/
  UINT32  Enable             :  1;
  UINT32  ArpEnable          :  1;      ///< Enable SMBus ARP support, <b>0: Disable</b>; 1: Enable.
  UINT32  DynamicPowerGating :  1;      ///< <b>(Test)</b> <b>Disable</b> or Enable Smbus dynamic power gating.
  ///
  /// <b>(Test)</b> SPD Write Disable, 0: leave SPD Write Disable bit; <b>1: set SPD Write Disable bit.</b>
  /// For security recommendations, SPD write disable bit must be set.
  ///
  UINT32  SpdWriteDisable    :  1;
  UINT32  SmbAlertEnable     :  1;      ///< Enable SMBus Alert pin (SMBALERT#). 0: <b>Disabled<b>, 1: Enabled.
  UINT32  RsvdBits0          : 27;      ///< Reserved bits
  UINT16  SmbusIoBase;                  ///< SMBUS Base Address (IO space). Default is <b>0xEFA0</b>.
  UINT8   Rsvd0;                        ///< Reserved bytes
  UINT8   NumRsvdSmbusAddresses;        ///< The number of elements in the RsvdSmbusAddressTable.
  /**
    Array of addresses reserved for non-ARP-capable SMBus devices.
  **/
  UINT8   RsvdSmbusAddressTable[PCH_MAX_SMBUS_RESERVED_ADDRESS];
} PCH_SMBUS_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _SMBUS_CONFIG_H_
