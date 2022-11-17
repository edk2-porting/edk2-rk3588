/** @file
  PCIE DXE policy definitions

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCIE_DXE_CONFIG_H_
#define _PCIE_DXE_CONFIG_H_

#include "CpuPcieInfo.h"

#pragma pack(push, 1)

#define PCIE_DXE_CONFIG_REVISION 2

typedef struct {
  UINT16  VendorId; ///< Offset 0 PCI Config space offset 0
  UINT16  DeviceId; ///< Offset 2 PCI Config space offset 2
/**
  Offset 4:
  SnoopLatency bit definition
  Note: All Reserved bits must be set to 0

  BIT[15]     - When set to 1b, indicates that the values in bits 9:0 are valid
                When clear values in bits 9:0 will be ignored
  BIT[14]     - Should be set to 0b
  BIT[13]     - Reserved
  BITS[12:10] - Value in bits 9:0 will be multiplied with the scale in these bits
                000b - 1 ns
                001b - 32 ns
                010b - 1024 ns
                011b - 32,768 ns
                100b - 1,048,576 ns
                101b - 33,554,432 ns
                110b - Reserved
                111b - Reserved
  BITS[9:0]   - Snoop Latency Value. The value in these bits will be multiplied with
                the scale in bits 12:10
**/
  UINT16  SnoopLatency;
/**
  Offset 6:
  NonSnoopLatency bit definition
  Note: All Reserved bits must be set to 0

  BIT[15]     - When set to 1b, indicates that the values in bits 9:0 are valid
                When clear values in bits 9:0 will be ignored
  BIT[14]     - Should be set to 0b
  BIT[13]     - Reserved
  BITS[12:10] - Value in bits 9:0 will be multiplied with the scale in these bits
                000b - 1 ns
                001b - 32 ns
                010b - 1024 ns
                011b - 32,768 ns
                100b - 1,048,576 ns
                101b - 33,554,432 ns
                110b - Reserved
                111b - Reserved
  BITS[9:0]   - Non Snoop Latency Value. The value in these bits will be multiplied with
                the scale in bits 12:10
**/
  UINT16  NonSnoopLatency;
  UINT8   RevId;    ///<   Offset 8 PCI Config space offset 8; 0xFF means all steppings
  UINT8   Rsvd0[3]; ///<   Offset 9
} PCIE_LTR_DEV_INFO;

///
/// PCIE Power Optimizer config
///
typedef struct {
  UINT16  LtrMaxSnoopLatency;   ///< Offset 0 LTR Maximum Snoop Latency: <b>0x0846=70us</b>
  UINT16  LtrMaxNoSnoopLatency; ///< Offset 2 LTR Maximum Non-Snoop Latency: <b>0x0846=70us</b>
  UINT8   ObffEnable;           ///< Offset 4 LTR enable/disable: 0=Disable, <b>1=Enable</b>
  UINT8   LtrEnable;            ///< Offset 5 LTR enable/disable: 0=Disable, <b>1=Enable</b>
  UINT8   Rsvd0[2];             ///< Offset 6 Reserved
} CPU_PCIE_PWR_OPT;


/**
  The PCI Express Configuration info includes PCI Resources Range Base and Limits and the control
  for PEG ASPM.
  The data elements should be initialized by a Platform Module.\n
  @note <b>Optional.</b> These policies will be ignored if there is no PEG port present on board.
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  -  Adding PEG RTD3 Support Setup Variable
  <b>Revision 3</b>:
  -  Adding CPU PCIE RTD3 Support Setup Variable
  -  Deprecating PEG RTD3 Support Setup Variable
  <b>Revision 4</b>:
  -  Deprecating CPU PCIE RTD3 Support Setup Variable
**/
typedef struct {
  CONFIG_BLOCK_HEADER      Header;                         ///< Offset 0-27 Config Block Header
/**
  Offset 28: This field is used to describe the ASPM control for PEG Ports\n
  0=ASPM Disabled, 1=ASPM L0s Enabled, 2=ASPM L1 Enabled, 3=ASPM L0sL1 Enabled, <b>4=ASPM AUTO</b>
**/
  UINT8                    PegAspm[SA_PEG_MAX_FUN];
/**
  Offset 32: PCIe Hot Plug Enable/Disable. It has 2 policies.
   - <b>Disabled (0x0)</b>     : No hotplug.
   - Enabled (0x1)      : Bios assist hotplug.
**/
  UINT8                    PegRootPortHPE[SA_PEG_MAX_FUN];
  CPU_PCIE_PWR_OPT          PegPwrOpt[SA_PEG_MAX_FUN];     ///< Offset 36: This field is used to describe the PCIe LTR/OBFF relevant settings
  UINT32                   PegRtd3;                       ///  Deprecated Policy
  UINT8                    CpuPcieRtd3;                   ///< Enable/Disable RTD3 Support for CPU PCIE. 0=Disable and 1=Enable (default)  // Deprecated Policy
  UINT8                    Rsvd3[3];
} PCIE_DXE_CONFIG;
#pragma pack(pop)

#endif // _PCIE_DXE_CONFIG_H_
