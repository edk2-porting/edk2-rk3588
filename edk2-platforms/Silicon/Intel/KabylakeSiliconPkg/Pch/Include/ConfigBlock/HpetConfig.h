/** @file
  HPET policy

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _HPET_CONFIG_H_
#define _HPET_CONFIG_H_

#define HPET_PREMEM_CONFIG_REVISION 1
extern EFI_GUID gHpetPreMemConfigGuid;

#pragma pack (push,1)

/**
  The PCH_HPET_CONFIG block passes the bus/device/function value for HPET.
  The address resource range of HPET must be reserved in E820 and ACPI as
  system resource.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  /**
    Determines if enable HPET timer. 0: Disable; <b>1: Enable</b>.
    The HPET timer address decode is always enabled.
    This policy is used to configure the HPET timer count, and also the _STA of HPET device in ACPI.
    While enabled, the HPET timer is started, else the HPET timer is halted.
  **/
  UINT32   Enable             :  1;
  UINT32   BdfValid           :  1;     ///< Whether the BDF value is valid. <b>0: Disable</b>; 1: Enable.
  UINT32   RsvdBits0          :  6;     ///< Reserved bits
  UINT32   BusNumber          :  8;     ///< Bus Number HPETn used as Requestor / Completer ID. Default is <b>0xF0</b>.
  UINT32   DeviceNumber       :  5;     ///< Device Number HPETn used as Requestor / Completer ID. Default is <b>0x1F</b>.
  UINT32   FunctionNumber     :  3;     ///< Function Number HPETn used as Requestor / Completer ID. Default is <b>0x00</b>.
  UINT32   RsvdBits1          :  8;     ///< Reserved bits
  UINT32   Base;                        ///< The HPET base address. Default is <b>0xFED00000</b>.
} PCH_HPET_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _HPET_CONFIG_H_
