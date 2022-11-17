/** @file
  CPU PID Config Block.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CPU_PID_TEST_CONFIG_H_
#define _CPU_PID_TEST_CONFIG_H_

#define CPU_PID_TEST_CONFIG_REVISION 1

extern EFI_GUID gCpuPidTestConfigGuid;

#pragma pack (push,1)

/**
  PID Tuning Configuration Structure.
  Domain is mapped to Kp = 0, Ki = 1, Kd = 2.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  UINT16  Ratl[3];                                ///< RATL setting, in 1/256 units. Range is 0 - 65280
  UINT16  VrTdcVr0[3];                            ///< VR Thermal Design Current for VR0. In 1/256 units. Range is 0 - 65280
  UINT16  VrTdcVr1[3];                            ///< VR Thermal Design Current for VR1. In 1/256 units. Range is 0 - 65280
  UINT16  VrTdcVr2[3];                            ///< VR Thermal Design Current for VR2. In 1/256 units. Range is 0 - 65280
  UINT16  VrTdcVr3[3];                            ///< VR Thermal Design Current for VR3. In 1/256 units. Range is 0 - 65280
  UINT16  PbmPsysPl1Msr[3];                       ///< Power Budget Management Psys PL1 MSR. In 1/256 units. Range is 0 - 65280
  UINT16  PbmPsysPl1MmioPcs[3];                   ///< Power Budget Management Psys PL1 MMIO/PCS. In 1/256 units. Range is 0 - 65280
  UINT16  PbmPsysPl2Msr[3];                       ///< Power Budget Management Psys PL2 MSR. In 1/256 units. Range is 0 - 65280
  UINT16  PbmPsysPl2MmioPcs[3];                   ///< Power Budget Management Psys PL2 MMIO/PCS. In 1/256 units. Range is 0 - 65280
  UINT16  PbmPkgPl1Msr[3];                        ///< Power Budget Management Package PL1 MSR. In 1/256 units. Range is 0 - 65280
  UINT16  PbmPkgPl1MmioPcs[3];                    ///< Power Budget Management Package PL1 MMIO/PCS. In 1/256 units. Range is 0 - 65280
  UINT16  PbmPkgPl2Msr[3];                        ///< Power Budget Management Package PL2 MSR. In 1/256 units. Range is 0 - 65280
  UINT16  PbmPkgPl2MmioPcs[3];                    ///< Power Budget Management Package PL2 MMIO/PCS. In 1/256 units. Range is 0 - 65280
  UINT16  DdrPl1Msr[3];                           ///< DDR PL1 MSR. In 1/256 units. Range is 0 - 65280
  UINT16  DdrPl1MmioPcs[3];                       ///< DDR PL1 MMIO/PCS. In 1/256 units. Range is 0 - 65280
  UINT16  DdrPl2Msr[3];                           ///< DDR PL2 MSR. In 1/256 units. Range is 0 - 65280
  UINT16  DdrPl2MmioPcs[3];                       ///< DDR PL2 MMIO/PCS. In 1/256 units. Range is 0 - 65280
  /**
  Enable or Disable PID Tuning programming flow.
  If disabled, all other policies in this config block are ignored.
  **/
  UINT8   PidTuning;
  UINT8   Rsvd;                                   ///< Reserved for DWORD alignment.
} CPU_PID_TEST_CONFIG;

#pragma pack (pop)

#endif // _CPU_PID_TEST_CONFIG_H_
