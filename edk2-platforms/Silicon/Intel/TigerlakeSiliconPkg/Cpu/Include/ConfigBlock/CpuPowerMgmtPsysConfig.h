/** @file
  CPU Power Management Psys(Platform) Config Block.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CPU_POWER_MGMT_PSYS_CONFIG_H_
#define _CPU_POWER_MGMT_PSYS_CONFIG_H_

#define CPU_POWER_MGMT_PSYS_CONFIG_REVISION 1

extern EFI_GUID gCpuPowerMgmtPsysConfigGuid;

#pragma pack (push,1)

/**
  CPU Power Management Psys(Platform) Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  UINT32 PsysPowerLimit1       : 1;               ///< MSR 0x65C[15]: PL1 Enable activates the PL1 value to limit average platform power
  UINT32 PsysPowerLimit1Time   : 8;               ///< MSR 0x65C[23:17]: PL1 timewindow in seconds.
  UINT32 PsysPowerLimit2       : 1;               ///< MSR 0x65C[47]: PL2 Enable activates the PL2 value to limit average platform power
  UINT32 RsvdBits              : 22;              ///< Reserved for future use.
  UINT16 PsysPowerLimit1Power;                    ///< MSR 0x65C[14:0]: Platform PL1 power. Units are based on POWER_MGMT_CONFIG.CustomPowerUnit.
  UINT16 PsysPowerLimit2Power;                    ///< MSR 0x65C[46:32]]: Platform PL2 power. Units are based on POWER_MGMT_CONFIG.CustomPowerUnit.
  UINT16 PsysPmax;                                ///< PCODE MMIO Mailbox: Platform Power Pmax. <b>0 - Auto</b> Specified in 1/8 Watt increments. 0-1024 Watts. Value of 800 = 100W.
  UINT8  Rsvd[2];                                 ///< Reserved for future use and config block alignment
} CPU_POWER_MGMT_PSYS_CONFIG;

#pragma pack (pop)

#endif // _CPU_POWER_MGMT_PSYS_CONFIG_H_
