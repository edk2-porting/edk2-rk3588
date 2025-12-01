/** @file
 *
 *  Copyright (c) 2023-2025, Mario Bălănică <mariobalanica02@gmail.com>
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
// Hardware mapping
//
// PCIe 3x4 is the first PCIe controller in memory, immediately followed
// by the others in the order of the segments below.
//
#define NUM_PCIE_CONTROLLER  5

#define PCIE_SEGMENT_PCIE30X4  0
#define PCIE_SEGMENT_PCIE30X2  1
#define PCIE_SEGMENT_PCIE20L0  2
#define PCIE_SEGMENT_PCIE20L1  3
#define PCIE_SEGMENT_PCIE20L2  4

#define PCIE_3X4_APB_BASE     0xfe150000
#define PCIE_3X4_DBI_BASE     0xa40000000ULL
#define PCIE_3X4_MMIO32_BASE  0xf0000000
#define PCIE_3X4_MMIO64_BASE  0x900000000ULL

#define PCIE_APB_SIZE     SIZE_64KB
#define PCIE_DBI_SIZE     SIZE_4MB
#define PCIE_MMIO32_SIZE  SIZE_16MB
#define PCIE_MMIO64_SIZE  SIZE_1GB

#define PCIE_APB_BASE(Segment)     (PCIE_3X4_APB_BASE + (Segment * PCIE_APB_SIZE))
#define PCIE_DBI_BASE(Segment)     (PCIE_3X4_DBI_BASE + (Segment * 1ULL * PCIE_DBI_SIZE))
#define PCIE_MMIO32_BASE(Segment)  (PCIE_3X4_MMIO32_BASE + (Segment * PCIE_MMIO32_SIZE))
#define PCIE_MMIO64_BASE(Segment)  (PCIE_3X4_MMIO64_BASE + (Segment * 1ULL * PCIE_MMIO64_SIZE))

//
// Software mapping
//
// Since the MMIO32 space is absurdly small, we only use it for I/O.
// ECAM and MEM32 go at the top of the MMIO64 space. This preserves
// the MEM64 alignment and allows EDK2 to allocate up to a single
// large 512 MB BAR + remaining space until the ECAM base.
//
#define PCIE_MEM32_SIZE           SIZE_128MB
#define PCIE_MEM32_BASE(Segment)  (PCIE_MMIO64_BASE (Segment) + PCIE_MMIO64_SIZE - PCIE_MEM32_SIZE)
//
// Likely not an issue here, but let's play it safe and ensure the bus
// address does not overlap inbound system RAM, since I've seen DMA
// corruption on a different IP due to this.
//
#define PCIE_MEM32_BUS_BASE              PCIE_MMIO32_BASE (0)
#define PCIE_MEM32_TRANSLATION(Segment)  (PCIE_MEM32_BASE (Segment) - PCIE_MEM32_BUS_BASE)

// ECAM must start on a 256 MB boundary (28-bit B/D/F addressing).
#define PCIE_CFG_SIZE           (SIZE_256MB - PCIE_MEM32_SIZE)
#define PCIE_CFG_BASE(Segment)  (PCIE_MMIO64_BASE (Segment) + PCIE_MMIO64_SIZE - SIZE_256MB)

#define PCIE_MEM64_SIZE           (PCIE_MMIO64_SIZE - PCIE_MEM32_SIZE - PCIE_CFG_SIZE)
#define PCIE_MEM64_BASE(Segment)  PCIE_MMIO64_BASE (Segment)

#define PCIE_IO_SIZE                  SIZE_64KB
#define PCIE_IO_BASE(Segment)         (PCIE_MMIO32_BASE (Segment) + SIZE_1MB)
#define PCIE_IO_BUS_BASE              0x0000
#define PCIE_IO_TRANSLATION(Segment)  (PCIE_IO_BASE (Segment) - PCIE_IO_BUS_BASE)

//
// All RCs share a single SMMU and two ITS blocks. To prevent overlapping
// Requester IDs, we need to space segments by <PCIE_BUS_COUNT> bus numbers.
// It doesn't seem possible to encode the segment number instead.
//
#define PCIE_BUS_COUNT                 (PCIE_CFG_SIZE / SIZE_1MB / NUM_PCIE_CONTROLLER)
#define PCIE_BUS_BASE(Segment)         (Segment * PCIE_BUS_COUNT)
#define PCIE_BUS_LIMIT(Segment)        (PCIE_BUS_BASE (Segment) + PCIE_BUS_COUNT - 1)
#define PCIE_BUS_BASE_OFFSET(Segment)  (PCIE_BUS_BASE (Segment) * SIZE_1MB)

#endif
