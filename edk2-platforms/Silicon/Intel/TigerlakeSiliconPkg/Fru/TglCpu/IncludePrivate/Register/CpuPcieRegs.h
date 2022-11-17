/** @file
  This file contains definitions of  PCIe Configuration

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPU_PCIE_REGS_H_
#define _CPU_PCIE_REGS_H_

#define  R_PCIE_LCAP                              0x4C
#define  R_PCIE_LCTL                              0x50
#define  R_PCIE_LSTS                              0x52
#define  R_PCIE_SLCAP                             0x54
#define  R_PCIE_SLSTS                             0x5A
#define  R_PCIE_LCTL2                             0x70
#define  R_PCIE_MPC                               0xD8
#define  B_PCIE_MPC_HPME                          BIT1
#define  R_PCIE_PGTHRES                           0x5C0
#define  B_PCIE_PGTHRES_L1PGLTREN                 BIT0
#define  R_PCIE_LCTL3                             0xA34
#define  B_PCIE_LCTL3_PE                          BIT0

#endif
