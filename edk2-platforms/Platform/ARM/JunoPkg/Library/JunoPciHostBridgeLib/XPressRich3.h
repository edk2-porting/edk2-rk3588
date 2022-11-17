/** @file
*  Header containing the Xpress-RICH3 PCIe Root Complex specific values
*
*  Copyright (c) 2011-2015, ARM Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __XPRESS_RICH3_H__
#define __XPRESS_RICH3_H__

#include <Protocol/CpuIo2.h>
#include <Library/PcdLib.h>

#define PCI_ECAM_BASE       FixedPcdGet64 (PcdPciConfigurationSpaceBaseAddress)
#define PCI_ECAM_SIZE       FixedPcdGet64 (PcdPciConfigurationSpaceSize)
#define PCI_IO_BASE         FixedPcdGet64 (PcdPciIoTranslation)
#define PCI_IO_SIZE         FixedPcdGet64 (PcdPciIoSize)
#define PCI_MEM32_BASE      FixedPcdGet64 (PcdPciMmio32Base)
#define PCI_MEM32_SIZE      FixedPcdGet64 (PcdPciMmio32Size)
#define PCI_MEM64_BASE      FixedPcdGet64 (PcdPciMmio64Base)
#define PCI_MEM64_SIZE      FixedPcdGet64 (PcdPciMmio64Size)

#define PCI_TRACE(txt)  DEBUG((EFI_D_VERBOSE, "ARM_PCI: " txt "\n"))

#define PCIE_ROOTPORT_WRITE32(Add, Val) { UINT32 Value = (UINT32)(Val); CpuIo->Mem.Write (CpuIo,EfiCpuIoWidthUint32,(UINT64)(PcdGet64 (PcdPcieRootPortBaseAddress)+(Add)),1,&Value); }
#define PCIE_ROOTPORT_READ32(Add, Val) { CpuIo->Mem.Read (CpuIo,EfiCpuIoWidthUint32,(UINT64)(PcdGet64 (PcdPcieRootPortBaseAddress)+(Add)),1,&Val); }

#define PCIE_CONTROL_WRITE32(Add, Val) { UINT32 Value = (UINT32)(Val); CpuIo->Mem.Write (CpuIo,EfiCpuIoWidthUint32,(UINT64)(PcdGet64 (PcdPcieControlBaseAddress)+(Add)),1,&Value); }
#define PCIE_CONTROL_READ32(Add, Val) { CpuIo->Mem.Read (CpuIo,EfiCpuIoWidthUint32,(UINT64)(PcdGet64 (PcdPcieControlBaseAddress)+(Add)),1,&Val); }

/*
 * Bridge Internal Registers
 */

// PCIe Available Credit Settings
#define PCIE_VC_CRED                            0x090
// PCIe PCI Standard Configuration Identification Settings registers
#define PCIE_PCI_IDS                            0x098
#define PCIE_PCI_IDS_CLASSCODE_OFFSET           0x4
// PCIe Specific 2 Capabilities Settings
#define PCIE_PEX_SPC2                           0x0d8
// PCIe Windows Settings register
#define PCIE_BAR_WIN                            0x0FC
// Local Processor Interrupt Mask
#define PCIE_IMASK_LOCAL                        0x180

#define PCIE_BAR_WIN_SUPPORT_IO                 BIT0
#define PCIE_BAR_WIN_SUPPORT_IO32               BIT1
#define PCIE_BAR_WIN_SUPPORT_MEM                BIT2
#define PCIE_BAR_WIN_SUPPORT_MEM64              BIT3

#define PCIE_INT_MSI                            BIT28
#define PCIE_INT_A                              BIT24
#define PCIE_INT_B                              BIT25
#define PCIE_INT_C                              BIT26
#define PCIE_INT_D                              BIT27
#define PCIE_INT_INTx                           (PCIE_INT_A | PCIE_INT_B |\
                                                 PCIE_INT_C | PCIE_INT_D)

/*
 * PCIe Control Registers
 */
#define PCIE_CONTROL_RST_CTL     0x1004
#define PCIE_CONTROL_RST_STS     0x1008

/*
 * PCI Express Address Translation registers
 * All are offsets from PcdPcieControlBaseAddress
 */
#define VEXPRESS_ATR_PCIE_WIN0    0x600
#define VEXPRESS_ATR_AXI4_SLV0    0x800
#define VEXPRESS_ATR_AXI4_SLV1    0x820

#define PCI_ATR_ENTRY_SIZE           0x20
#define PCI_ATR_SRC_ADDR_LOW_SIZE    0
#define PCI_ATR_SRC_ADDR_HI          0x4
#define PCI_ATR_TRSL_ADDR_LOW        0x8
#define PCI_ATR_TRSL_ADDR_HI         0xc
#define PCI_ATR_TRSL_PARAM           0x10

#define PCI_ATR_TRSLID_AXIDEVICE     0x420004
#define PCI_ATR_TRSLID_AXIMEMORY     0x4e0004
#define PCI_ATR_TRSLID_PCIE_CONF     0x000001
#define PCI_ATR_TRSLID_PCIE_IO       0x020000
#define PCI_ATR_TRSLID_PCIE_MEMORY   0x000000

#define PCIE_CONTROL_RST_CTL_RC_REL        (1 << 1)
#define PCIE_CONTROL_RST_CTL_PHY_REL       (1 << 0)
#define PCIE_CONTROL_RST_CTL_RCPHY_REL     (PCIE_CONTROL_RST_CTL_RC_REL | PCIE_CONTROL_RST_CTL_PHY_REL)

#define PCIE_CONTROL_RST_STS_RC_ST         (1 << 2)
#define PCIE_CONTROL_RST_STS_PHY_ST        (1 << 1)
#define PCIE_CONTROL_RST_STS_PLL_ST        (1 << 0)
#define PCIE_CONTROL_RST_STS_RCPHYPLL_OUT  (PCIE_CONTROL_RST_STS_RC_ST | PCIE_CONTROL_RST_STS_PHY_ST | PCIE_CONTROL_RST_STS_PLL_ST)

#define VEXPRESS_BASIC_STATUS       0x18
#define LINK_UP                     0xff

#endif
