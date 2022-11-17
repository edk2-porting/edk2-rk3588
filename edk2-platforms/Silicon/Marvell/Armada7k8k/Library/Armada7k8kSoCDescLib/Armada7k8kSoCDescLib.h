/**
*
*  Copyright (C) 2018, Marvell International Ltd. and its affiliates.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*  Glossary - abbreviations used in Marvell SampleAtReset library implementation:
*  AP - Application Processor hardware block (Armada 7k8k incorporates AP806)
*  CP - South Bridge hardware blocks (Armada 7k8k incorporates CP110)
**/

#ifndef __ARMADA7K8K_SOC_DESC_LIB_H__
#define __ARMADA7K8K_SOC_DESC_LIB_H__

//
// Common macros
//
#define MV_SOC_CP_BASE(Cp)               (0xF2000000 + ((Cp) * 0x2000000))
#define MV_SOC_AP806_BASE                0xF0000000
#define MV_SOC_AP806_COUNT               1

//
// Armada7k8k default North Bridge index
//
#define ARMADA7K8K_AP806_INDEX           0

//
// Platform description of AHCI controllers
//
#define MV_SOC_AHCI_BASE(Cp)             (MV_SOC_CP_BASE (Cp) + 0x540000)
#define MV_SOC_AHCI_ID(Cp)               ((Cp) % 2)

//
// Platform description of ComPhy controllers
//
#define MV_SOC_COMPHY_BASE(Cp)           (MV_SOC_CP_BASE (Cp) + 0x441000)
#define MV_SOC_HPIPE3_BASE(Cp)           (MV_SOC_CP_BASE (Cp) + 0x120000)
#define MV_SOC_COMPHY_LANE_COUNT         6
#define MV_SOC_COMPHY_MUX_BITS           4

//
// Platform description of GPIO controllers
//
#define MV_SOC_AP806_GPIO_BASE           0xF06F5040
#define MV_SOC_AP806_GPIO_PIN_COUNT      20
#define MV_SOC_GPIO_PER_CP_COUNT         2
#define MV_SOC_CP_GPIO_BASE(Gpio)        (0x440100 + ((Gpio) * 0x40))
#define MV_SOC_CP_GPIO_PIN_COUNT(Gpio)   ((Gpio) == 0 ? 32 : 31)

//
// Platform description of I2C controllers
//
#define MV_SOC_I2C_PER_AP_COUNT          1
#define MV_SOC_I2C_AP_BASE               (MV_SOC_AP806_BASE + 0x511000)
#define MV_SOC_I2C_PER_CP_COUNT          3
#define MV_SOC_I2C_BASE(I2c)             ((I2c < 2) ? \
                                          (0x701000 + (I2c) * 0x100) : \
                                          0x211000)

//
// Platform description of ICU (Interrupt Consolidation Unit) controllers
//
#define ICU_GIC_MAPPING_OFFSET           0
#define ICU_NSR_SET_SPI_BASE             0xf03f0040
#define ICU_NSR_CLEAR_SPI_BASE           0xf03f0048
#define ICU_SEI_SET_SPI_BASE             0xf03f0230
#define ICU_SEI_CLEAR_SPI_BASE           0xf03f0230
#define ICU_REI_SET_SPI_BASE             0xf03f0270
#define ICU_REI_CLEAR_SPI_BASE           0xf03f0270
#define ICU_GROUP_UNSUPPORTED            0x0

//
// Platform description of MDIO controllers
//
#define MV_SOC_MDIO_BASE(Cp)             (MV_SOC_CP_BASE (Cp) + 0x12A200)
#define MV_SOC_MDIO_ID(Cp)               (Cp)

//
// Platform description of PCIE
//
#define MV_SOC_PCIE_PER_CP_COUNT         3
#define MV_SOC_PCIE_BASE(Index)          (0x600000 + ((Index) * 0x20000))

//
// Platform description of PP2 NIC
//
#define MV_SOC_PP2_BASE(Cp)              MV_SOC_CP_BASE (Cp)
#define MV_SOC_PP2_CLK_FREQ              333333333

//
// Platform description of SDMMC controllers
//
#define MV_SOC_SDMMC_PER_CP_COUNT        1
#define MV_SOC_AP80X_SDMMC_BASE          0xF06E0000
#define MV_SOC_CP_SDMMC_BASE(Cp)         (MV_SOC_CP_BASE (Cp) + 0x780000)

//
// Platform description of UTMI PHY's
//
#define MV_SOC_UTMI_PER_CP_COUNT         2
#define MV_SOC_UTMI_ID(Utmi)             (Utmi)
#define MV_SOC_UTMI_BASE(Utmi)           (0x58000C + ((Utmi) * 0x1000))
#define MV_SOC_UTMI_PLL_BASE             0x580000
#define MV_SOC_UTMI_CFG_BASE             0x440440
#define MV_SOC_UTMI_USB_CFG_BASE         0x440420

//
// Platform description of XHCI controllers
//
#define MV_SOC_XHCI_PER_CP_COUNT         2
#define MV_SOC_XHCI_BASE(Xhci)           (0x500000 + ((Xhci) * 0x10000))

#endif
