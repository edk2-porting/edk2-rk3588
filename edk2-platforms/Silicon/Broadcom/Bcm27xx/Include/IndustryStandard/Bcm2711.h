/** @file
 *
 *  Copyright (c) 2019, Jeremy Linton
 *  Copyright (c) 2019, Pete Batard <pete@akeo.ie>.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef BCM2711_H__
#define BCM2711_H__

#define BCM2711_SOC_REGISTERS               (FixedPcdGet64 (PcdBcm27xxRegistersAddress))
#define BCM2711_SOC_REGISTER_LENGTH         0x02000000

/* Generic PCI addresses */
#define PCIE_TOP_OF_MEM_WIN                                 (FixedPcdGet64 (PcdBcm27xxPciBusMmioAdr))
#define PCIE_CPU_MMIO_WINDOW                                (FixedPcdGet64 (PcdBcm27xxPciCpuMmioAdr))
#define PCIE_BRIDGE_MMIO_LEN                                (FixedPcdGet32 (PcdBcm27xxPciBusMmioLen))

/* PCI root bridge control registers location */
#define PCIE_REG_BASE                                       (FixedPcdGet32 (PcdBcm27xxPciRegBase))
#define PCIE_REG_LIMIT                                      0x9310

/* PCI root bridge control registers */
#define BRCM_PCIE_CAP_REGS                                  0x00ac      /* Offset to ecam like range for root port */
#define PCIE_RC_CFG_VENDOR_VENDOR_SPECIFIC_REG1             0x0188
#define BRCM_PCIE_CLASS                                     0x043c
#define PCIE_MISC_MISC_CTRL                                 0x4008
#define PCIE_MISC_CPU_2_PCIE_MEM_WIN0_LO                    0x400c
#define PCIE_MISC_CPU_2_PCIE_MEM_WIN0_HI                    0x4010
#define PCIE_MISC_RC_BAR1_CONFIG_LO                         0x402c
#define PCIE_MISC_RC_BAR2_CONFIG_LO                         0x4034
#define PCIE_MISC_RC_BAR2_CONFIG_HI                         0x4038
#define PCIE_MISC_RC_BAR3_CONFIG_LO                         0x403c
#define PCIE_MISC_PCIE_STATUS                               0x4068
#define PCIE_MISC_REVISION                                  0x406c
#define PCIE_MISC_CPU_2_PCIE_MEM_WIN0_BASE_LIMIT            0x4070
#define PCIE_MISC_CPU_2_PCIE_MEM_WIN0_BASE_HI               0x4080
#define PCIE_MISC_CPU_2_PCIE_MEM_WIN0_LIMIT_HI              0x4084
#define PCIE_MISC_HARD_PCIE_HARD_DEBUG                      0x4204

#define PCIE_INTR2_CPU_STATUS                               0x4300
#define PCIE_INTR2_CPU_SET                                  0x4304
#define PCIE_INTR2_CPU_CLR                                  0x4308
#define PCIE_INTR2_CPU_MASK_STATUS                          0x430c
#define PCIE_INTR2_CPU_MASK_SET                             0x4310
#define PCIE_INTR2_CPU_MASK_CLR                             0x4314

#define PCIE_RGR1_SW_INIT_1                                 0x9210
#define PCIE_EXT_CFG_INDEX                                  0x9000
/* A small window pointing at the ECAM of the device selected by CFG_INDEX */
#define PCIE_EXT_CFG_DATA                                   0x8000

#define PCIE_RC_CFG_VENDOR_VENDOR_SPECIFIC_REG1_ENDIAN_MODE_BAR2_MASK 0xc
#define PCIE_RC_CFG_PRIV1_ID_VAL3_CLASS_CODE_MASK           0xffffff

#define PCIE_MISC_MISC_CTRL_SCB_ACCESS_EN_MASK              0x1000
#define PCIE_MISC_MISC_CTRL_CFG_READ_UR_MODE_MASK           0x2000
#define PCIE_MISC_MISC_CTRL_MAX_BURST_SIZE_MASK             0x300000
#define PCIE_MISC_MISC_CTRL_SCB0_SIZE_MASK                  0xf8000000
#define PCIE_MISC_MISC_CTRL_SCB1_SIZE_MASK                  0x7c00000
#define PCIE_MISC_MISC_CTRL_SCB2_SIZE_MASK                  0x1f
#define PCIE_MISC_RC_BAR2_CONFIG_LO_SIZE_MASK               0x1f

#define PCIE_RGR1_SW_INIT_1_INIT_MASK                       0x2
#define PCIE_RGR1_SW_INIT_1_PERST_MASK                      0x1

#define PCIE_MISC_HARD_PCIE_HARD_DEBUG_SERDES_IDDQ_MASK     0x08000000

#define PCIE_MISC_HARD_PCIE_HARD_DEBUG_CLKREQ_DEBUG_ENABLE_MASK 0x2

#define PCIE_MISC_CPU_2_PCIE_MEM_WIN0_BASE_LIMIT_LIMIT_MASK 0xfff00000
#define PCIE_MISC_CPU_2_PCIE_MEM_WIN0_BASE_LIMIT_BASE_MASK  0xfff0
#define PCIE_MISC_CPU_2_PCIE_MEM_WIN0_BASE_HI_BASE_MASK     0xff
#define PCIE_MISC_CPU_2_PCIE_MEM_WIN0_LIMIT_HI_LIMIT_MASK   0xff
#define PCIE_MISC_CPU_2_PCIE_MEM_WIN0_MASK_BITS             0xc


#define PCIE_MISC_REVISION_MAJMIN_MASK                      0xffff

#define BURST_SIZE_128                                      0
#define BURST_SIZE_256                                      1
#define BURST_SIZE_512                                      2

#define GENET_BASE_ADDRESS         FixedPcdGet64 (PcdBcmGenetRegistersAddress)
#define GENET_LENGTH               0x00010000

#define THERM_SENSOR               0xfd5d2200

#define ID_CHIPREV                 0xfc404000

#endif /* BCM2711_H__ */
