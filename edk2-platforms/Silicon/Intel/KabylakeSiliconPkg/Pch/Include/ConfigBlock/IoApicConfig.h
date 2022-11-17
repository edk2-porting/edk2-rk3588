/** @file
  IoApic policy

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _IOAPIC_CONFIG_H_
#define _IOAPIC_CONFIG_H_

#define IOAPIC_CONFIG_REVISION 1
extern EFI_GUID gIoApicConfigGuid;

#pragma pack (push,1)

/**
  The PCH_IOAPIC_CONFIG block describes the expected configuration of the PCH
  IO APIC, it's optional and PCH code would ignore it if the BdfValid bit is
  not TRUE. Bus:device:function fields will be programmed to the register
  P2SB IBDF(P2SB PCI offset R6Ch-6Dh), it's using for the following purpose:
  As the Requester ID when initiating Interrupt Messages to the processor.
  As the Completer ID when responding to the reads targeting the IOxAPI's
  Memory-Mapped I/O registers.
  This field defaults to Bus 0: Device 31: Function 0 after reset. BIOS can
  program this field to provide a unique Bus:Device:Function number for the
  internal IOxAPIC.
  The address resource range of IOAPIC must be reserved in E820 and ACPI as
  system resource.
**/
typedef struct {
  /**
    Revision 1: Init version
  **/
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  UINT32  BdfValid            :  1;     ///< Set to 1 if BDF value is valid, PCH code will not program these fields if this bit is not TRUE. <b>0: Disable</b>; 1: Enable.
  UINT32  RsvdBits0           :  7;     ///< Reserved bits
  UINT32  BusNumber           :  8;     ///< Bus/Device/Function used as Requestor / Completer ID. Default is <b>0xF0</b>.
  UINT32  DeviceNumber        :  5;     ///< Bus/Device/Function used as Requestor / Completer ID. Default is <b>0x1F</b>.
  UINT32  FunctionNumber      :  3;     ///< Bus/Device/Function used as Requestor / Completer ID. Default is <b>0x00</b>.
  UINT32  IoApicEntry24_119   :  1;     ///< 0: Disable; <b>1: Enable</b> IOAPIC Entry 24-119
  /**
    Enable 8254 Static Clock Gating during early POST time. <b>0: Disable</b>, 1: Enable
    Set 8254CGE=1 in POST time might fail to boot legacy OS which using 8254 timer.
    Make sure it won't break legacy OS boot before enabling this.
  **/
  UINT32  Early8254ClockGatingEnable :  1;
  UINT32  RsvdBits1           :  6;     ///< Reserved bits
  UINT8   IoApicId;                     ///< This member determines IOAPIC ID. Default is <b>0x02</b>.
  UINT8   ApicRangeSelect;              ///< Define address bits 19:12 for the IOxAPIC range. Default is <b>0</b>
  UINT8   Rsvd0[2];                     ///< Reserved bytes
} PCH_IOAPIC_CONFIG;

#pragma pack (pop)

#endif // _IOAPIC_CONFIG_H_
