/** @file
  This file contains definitions of PCIe controller information

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CPU_PCIE_INFO_H_
#define _CPU_PCIE_INFO_H_

#define PCIE_HWEQ_COEFFS_MAX    5

//
// Device 1 Memory Mapped IO Register Offset Equates
//
#define SA_PEG_DEV_NUM     0x01
#define SA_PEG0_DEV_NUM    SA_PEG_DEV_NUM
#define SA_PEG3_DEV_NUM    0x06

//
// SA PCI Express* Port configuration
//

#define CPU_PCIE_MAX_ROOT_PORTS            4

#define SA_PEG_MAX_FUN           0x04
#define SA_PEG_MAX_LANE          0x14
#define SA_PEG_MAX_FUN_GEN3      0x03
#define SA_PEG_MAX_LANE_GEN3     0x10
#define SA_PEG_MAX_BUNDLE_GEN3   0x08

#endif
