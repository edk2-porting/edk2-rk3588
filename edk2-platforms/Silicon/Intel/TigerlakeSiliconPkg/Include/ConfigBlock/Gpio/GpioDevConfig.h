/** @file
  GPIO device policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _GPIO_DEV_CONFIG_H_
#define _GPIO_DEV_CONFIG_H_

extern EFI_GUID gGpioDxeConfigGuid;

#define GPIO_DXE_CONFIG_REVISION 1

#pragma pack (push,1)

/**
  This structure contains the DXE policies which are related to GPIO device.

  <b>Revision 1:</b>
  - Inital version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;          ///< Config Block Header
  /**
    If GPIO ACPI device is not used by OS it can be hidden. In such case
    no other device exposed to the system can reference GPIO device in one
    of its resources through GpioIo(..) or GpioInt(..) ACPI descriptors.
    <b>0: Disable</b>; 1: Enable
  **/
  UINT32  HideGpioAcpiDevice    :  1;
  UINT32  RsvdBits              : 31;    ///< Reserved bits

} GPIO_DXE_CONFIG;

#pragma pack (pop)

#endif // _GPIO_DEV_CONFIG_H_
