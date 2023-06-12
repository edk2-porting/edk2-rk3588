/** @file
*
*  Copyright (c) 2023, Molly Sophia <mollysophia379@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef RK3588PCIE_H
#define RK3588PCIE_H

BOOLEAN
IsPcieNumEnabled(
  UINTN PcieNum
  );

VOID
PciePinmuxInit(
  UINTN PcieNum,
  UINTN MuxNum
  );

#define NUM_PCIE_CONTROLLER 5

#define PCIE_SEGMENT_PCIE30X4 0
#define PCIE_SEGMENT_PCIE30X2 1
#define PCIE_SEGMENT_PCIE20L0 2
#define PCIE_SEGMENT_PCIE20L1 3
#define PCIE_SEGMENT_PCIE20L2 4

#define PCIE_SEG0_DBI_BASE PcdGet64(PcdPcieRootPort3x4DbiBaseAddress)
#define PCIE_SEG0_APB_BASE PcdGet64(PcdPcieRootPort3x4ApbBaseAddress)
#define PCIE_SEG0_CFG_BASE PcdGet64(PcdPcieRootPort3x4CfgBaseAddress)
#define PCIE_SEG0_MEM64_BASE 0x900000000 //PcdGet64(PcdPcieRootPort3x4MemBaseAddress64)

#define PCIE_DBI_SIZE SIZE_4MB
#define PCIE_APB_SIZE SIZE_64KB
#define PCIE_CFG_SIZE PcdGet64(PcdPcieRootPort3x4CfgSize)
#define PCIE_IO_SIZE PcdGet32(PcdPcieRootPort3x4IoSize)
#define PCIE_MEM_SIZE PcdGet32(PcdPcieRootPort3x4MemSize)
#define PCIE_MEM64_SIZE 0x40000000

#define PCIE_IO_OFFSET PCIE_CFG_SIZE
#define PCIE_MEM_OFFSET 0x200000
#define PCIE_CFG_BASE_DIFF SIZE_16MB

#endif