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
IsPcieNumEnabled (
  UINTN  PcieNum
  );

VOID
PciePinmuxInit (
  UINTN  PcieNum,
  UINTN  MuxNum
  );

//
// PCIe 3x4 is the first PCIe controller in memory, the others
// immediately follow it in the order of the segments below.
//

#define NUM_PCIE_CONTROLLER  5

/*
 * All pcie controllers supports PCIe 3.0
 * Here we name them using their device tree name in the linux kernel source
 */
#define PCIE_SEGMENT_PCIE30X4  0
#define PCIE_SEGMENT_PCIE30X2  1
#define PCIE_SEGMENT_PCIE20L0  2
#define PCIE_SEGMENT_PCIE20L1  3
#define PCIE_SEGMENT_PCIE20L2  4

#define PCIE_3X4_APB_BASE  0xfe150000
#define PCIE_3X4_DBI_BASE  0xa40000000ULL
#define PCIE_3X4_CFG_BASE  0x900000000ULL
#define PCIE_3X4_MEM_BASE  0xf0000000

#define PCIE_APB_SIZE  SIZE_64KB
#define PCIE_DBI_SIZE  SIZE_4MB
#define PCIE_CFG_SIZE  SIZE_1GB
#define PCIE_MEM_SIZE  SIZE_16MB

#define PCIE_MEM64_OFFSET  0x10000000ULL

#define PCIE_APB_BASE(Segment)  (PCIE_3X4_APB_BASE + (Segment * PCIE_APB_SIZE))
#define PCIE_DBI_BASE(Segment)  (PCIE_3X4_DBI_BASE + (Segment * 1ULL * PCIE_DBI_SIZE))
#define PCIE_CFG_BASE(Segment)  (PCIE_3X4_CFG_BASE + (Segment * 1ULL * PCIE_CFG_SIZE))

#define PCIE_MEM_BASE(Segment)    (PCIE_3X4_MEM_BASE + (Segment * PCIE_MEM_SIZE))
#define PCIE_MEM64_BASE(Segment)  (PCIE_CFG_BASE(Segment) + PCIE_MEM64_OFFSET)

#define PCIE_IO_BASE  0x0000
#define PCIE_IO_SIZE  SIZE_64KB
#define PCIE_IO_XLATE(Segment)  (PCIE_CFG_BASE(Segment) + PCIE_CFG_SIZE - PCIE_IO_SIZE)

#define PCIE_MEM64_SIZE  (PCIE_CFG_SIZE - PCIE_IO_SIZE - PCIE_MEM64_OFFSET)

#define PCIE_BUS_LIMIT  252 // limited by CFG1 iATU window size

#endif
