/** @file
  PCI memory configuration for NXP

  Copyright 2018-2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef PCI_H
#define PCI_H

#define PciLsGen4SetPg(Dbi, PgIdx)      MmioAndThenOr32 ((Dbi) + PAB_CTRL,  \
              ~(PAB_CTRL_PAGE_SEL_MASK << PAB_CTRL_PAGE_SEL_SHIFT),         \
              ((PgIdx) & PAB_CTRL_PAGE_SEL_MASK) << PAB_CTRL_PAGE_SEL_SHIFT)

#define PCI_SEG0_NUM              0
#define PCI_SEG1_NUM              1
#define PCI_SEG2_NUM              2
#define PCI_SEG3_NUM              3
#define PCI_SEG4_NUM              4
#define PCI_SEG5_NUM              5
#define PCI_SEG0_MMIO_MEMBASE     FixedPcdGet64 (PcdPciExp1BaseAddr)
#define PCI_SEG0_DBI_BASE         0x03400000

#define PCI_LINK_DOWN             0x0
#define PCI_LINK_UP               0x1

// Segment configuration
#define PCI_SEG_BUSNUM_MIN        0x0
#define PCI_SEG_BUSNUM_MAX        0xff
#define PCI_SEG_PORTIO_MIN        0x0
#define PCI_SEG_PORTIO_MAX        0xffff
#define SEG_CFG_SIZE              0x00001000
#define ECAM_DEVICE_SIZE          SIZE_32KB
#define ECAM_BUS_SIZE             SIZE_1MB
#define ECAM_CFG_REGION_SIZE      SIZE_256MB
#define SEG_MEM_BASE              0x40000000
#define SEG_MEM_SIZE              0xC0000000
#define SEG_MEM_LIMIT             (SEG_MEM_BASE + (SEG_MEM_SIZE -1))
#define SEG_IO_BASE               0x10000000
#define SEG_MEM64_BASE            0x400000000
#define PCI_BASE_DIFF             0x800000000
#define PCI_DBI_SIZE_DIFF         0x100000
#define PCI_SEG0_PHY_CFG0_BASE    PCI_SEG0_MMIO_MEMBASE
#define PCI_SEG0_PHY_CFG1_BASE    (PCI_SEG0_PHY_CFG0_BASE + SEG_CFG_SIZE)
#define PCI_SEG0_PHY_MEM_BASE     (PCI_SEG0_MMIO_MEMBASE + SEG_MEM_BASE)
#define PCI_SEG0_PHY_MEM64_BASE   (PCI_SEG0_MMIO_MEMBASE + SEG_MEM64_BASE)
#define PCI_MMIO64_WIN_SIZE       SIZE_16GB
#define PCI_SEG0_PHY_IO_BASE      (PCI_SEG0_MMIO_MEMBASE + SEG_IO_BASE)
#define PCI_SEG0_PORTIO_MIN       0x0
#define PCI_SEG0_PORTIO_MAX       0xffff
#define PCI_SEG0_PORTIO_OFFSET    0x0
#define PCI_SEG1_PORTIO_MIN       0x0
#define PCI_SEG1_PORTIO_MAX       0xffff
#define PCI_SEG1_PORTIO_OFFSET    0x10000
#define PCI_SEG2_PORTIO_MIN       0x0
#define PCI_SEG2_PORTIO_MAX       0xffff
#define PCI_SEG2_PORTIO_OFFSET    0x20000
#define PCI_SEG3_PORTIO_MIN       0x0
#define PCI_SEG3_PORTIO_MAX       0xffff
#define PCI_SEG3_PORTIO_OFFSET    0x30000
#define PCI_SEG4_PORTIO_MIN       0x0
#define PCI_SEG4_PORTIO_MAX       0xffff
#define PCI_SEG4_PORTIO_OFFSET    0x40000
#define PCI_SEG5_PORTIO_MIN       0x0
#define PCI_SEG5_PORTIO_MAX       0xffff
#define PCI_SEG5_PORTIO_OFFSET    0x50000
#define PCI_SEG_PORTIO_LIMIT      (PCI_SEG5_PORTIO_MAX + PCI_SEG5_PORTIO_OFFSET)

// PCIe Controller configuration
#define NUM_PCIE_CONTROLLER       FixedPcdGet32 (PcdNumPciController)
#define PCI_LUT_DBG               FixedPcdGet32 (PcdPcieLutDbg)
#define PCI_LUT_BASE              FixedPcdGet32 (PcdPcieLutBase)
#define LTSSM_PCIE_L0             0x11

#define PCI_CLASS_BRIDGE_PCI      0x0604
#define PCI_CLASS_DEVICE          0x8
#define PCI_DBI_RO_WR_EN          0x8bc
#define CLASS_CODE_MASK           0xffff
#define CLASS_CODE_SHIFT          0x10

// PCIe Layerscape Controller
#define IATU_VIEWPORT_OFF                            0x900
#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0            0x904
#define IATU_REGION_CTRL_2_OFF_OUTBOUND_0            0x908
#define IATU_LWR_BASE_ADDR_OFF_OUTBOUND_0            0x90C
#define IATU_UPPER_BASE_ADDR_OFF_OUTBOUND_0          0x910
#define IATU_LIMIT_ADDR_OFF_OUTBOUND_0               0x914
#define IATU_LWR_TARGET_ADDR_OFF_OUTBOUND_0          0x918
#define IATU_UPPER_TARGET_ADDR_OFF_OUTBOUND_0        0x91C
#define IATU_VIEWPORT_OUTBOUND                       0x0
#define IATU_REGION_CTRL_2_OFF_OUTBOUND_0_REGION_EN  BIT31
#define IATU_ENABLE_CFG_SHIFT_FEATURE                BIT28

// ATU Programming
#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_MEM   0x0
#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_IO    0x2
#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_CFG0  0x4
#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_CFG1  0x5
#define IATU_REGION_INDEX0        0x0
#define IATU_REGION_INDEX1        0x1
#define SEG_CFG_BUS               0x00000000
#define SEG_MEM_BUS               0x40000000
#define SEG_IO_SIZE               0x10000
#define SEG_IO_BUS                0x0

#define CFG_SHIFT_ENABLE          (PcdGetBool (PcdPciCfgShiftEnable))
#define PCI_LS_GEN4_CTRL          (PcdGetBool (PcdPciLsGen4Ctrl))

// PCIe Layerscape Gen4 Controller
#define GPEX_CLASSCODE                          0x474
#define GPEX_CLASSCODE_SHIFT                    16
#define GPEX_CLASSCODE_MASK                     0xffff
#define PAB_AXI_PIO_CTRL(Idx)                   (0x840 + 0x10 * (Idx))
#define APIO_EN                                 0x1
#define MEM_WIN_EN                              0x1 << 1
#define IO_WIN_EN                               0x1 << 2
#define CFG_WIN_EN                              0x1 << 3
#define PAB_PEX_PIO_CTRL(Idx)                   (0x8c0 + 0x10 * (Idx))
#define PPIO_EN                                 (0x1 << 0)
#define PAB_PEX_PIO_STAT(Idx)                   (0x8c4 + 0x10 * (Idx))
#define PAB_PEX_PIO_MT_STAT(Idx)                (0x8c8 + 0x10 * (Idx))
#define PEX_AMAP_CTRL_TYPE_SHIFT                0x1
#define PEX_AMAP_CTRL_EN_SHIFT                  0x0
#define PEX_AMAP_CTRL_TYPE_MASK                 0x3
#define PEX_AMAP_CTRL_EN_MASK                   0x1
#define PAB_PEX_AMAP_CTRL(Idx)                  (0x4ba0 + 0x10 * (Idx))
#define PAB_EXT_PEX_AMAP_SIZE(Idx)              (0xbef0 + 0x04 * (Idx))
#define PAB_PEX_AMAP_AXI_WIN(Idx)               (0x4ba4 + 0x10 * (Idx))
#define PAB_EXT_PEX_AMAP_AXI_WIN(Idx)           (0xb4a0 + 0x04 * (Idx))
#define PAB_PEX_AMAP_PEX_WIN_L(Idx)             (0x4ba8 + 0x10 * (Idx))
#define PAB_PEX_AMAP_PEX_WIN_H(Idx)             (0x4bac + 0x10 * (Idx))
#define PAB_CTRL                                0x808
#define PAB_CTRL_APIO_EN                        0x1
#define PAB_CTRL_PPIO_EN                        (0x1 << 1)
#define PAB_CTRL_PAGE_SEL_SHIFT                 13
#define PAB_CTRL_PAGE_SEL_MASK                  0x3f
#define INDIRECT_ADDR_BNDRY                     0xc00
#define PAGE_IDX_SHIFT                          10
#define PAGE_ADDR_MASK                          0x3ff
#define PAB_AXI_AMAP_CTRL(Idx)                  (0xba0 + 0x10 * (Idx))
#define PAB_EXT_AXI_AMAP_SIZE(Idx)              (0xbaf0 + 0x4 * (Idx))
#define PAB_AXI_AMAP_AXI_WIN(Idx)               (0xba4 + 0x10 * (Idx))
#define PAB_EXT_AXI_AMAP_AXI_WIN(Idx)           (0x80a0 + 0x4 * (Idx))
#define PAB_AXI_AMAP_PEX_WIN_L(Idx)             (0xba8 + 0x10 * (Idx))
#define PAB_AXI_AMAP_PEX_WIN_H(Idx)             (0xbac + 0x10 * (Idx))
#define PAB_AXI_TYPE_CFG                        0x00
#define PAB_AXI_TYPE_IO                         0x01
#define PAB_AXI_TYPE_MEM                        0x02
#define AXI_AMAP_CTRL_EN                        0x1
#define AXI_AMAP_CTRL_TYPE_SHIFT                1
#define AXI_AMAP_CTRL_TYPE_MASK                 0x3
#define AXI_AMAP_CTRL_SIZE_SHIFT                10
#define AXI_AMAP_CTRL_SIZE_MASK                 0x3fffff


#define OFFSET_TO_PAGE_IDX(Off)                 ((Off >> PAGE_IDX_SHIFT) \
                                                & PAB_CTRL_PAGE_SEL_MASK)

#define OFFSET_TO_PAGE_ADDR(Off)                ((Off & PAGE_ADDR_MASK) \
                                                | INDIRECT_ADDR_BNDRY)

#define GPEX_ACK_REPLAY_TO                      0x438
#define ACK_LAT_TO_VAL_SHIFT                    0
#define ACK_LAT_TO_VAL_MASK                     0x1fff

#endif
