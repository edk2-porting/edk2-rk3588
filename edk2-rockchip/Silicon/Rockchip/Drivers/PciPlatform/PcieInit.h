/** @file
*
*  Copyright (c) 2022, Rockchip Inc.
* 
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __PCIE_INIT_H__
#define __PCIE_INIT_H__

#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/TimerLib.h>

#define RK_PCIE_DBG			0

#ifndef BIT
#define BIT(nr)         (1 << (nr))
#endif

#if __SIZEOF_POINTER__ == 8
#define BITS_PER_LONG		64
#elif __SIZEOF_POINTER__ == 4
#define BITS_PER_LONG		32
#else
#error "Unexpected __SIZEOF_POINTER__"
#endif


#define upper_32_bits(n) ((UINT32)(((n) >> 16) >> 16))
#define lower_32_bits(n) ((UINT32)(n))
#define GENMASK(h, l) \
	(((~0UL) - (1UL << (l)) + 1) & (~0UL >> (BITS_PER_LONG - 1 - (h))))



extern EFI_GUID gEfiPcieRootBridgeProtocolGuid;

struct rk_pcie {
	UINTN dbi_base;
	UINTN apb_base;
	UINTN cfg_base;
	UINT32 first_busno;
	UINT32	gen;
	UINT32	lane;
};


enum {
	PCIBIOS_SUCCESSFUL = 0x0000,
	PCIBIOS_UNSUPPORTED = -1,
	PCIBIOS_NODEV = -2,
};

#define msleep(a)		MicroSecondDelay(a);

/* Parameters for the waiting for iATU enabled routine */
#define PCIE_CLIENT_GENERAL_DEBUG	0x104
#define PCIE_CLIENT_HOT_RESET_CTRL	0x180
#define PCIE_LTSSM_ENABLE_ENHANCE	BIT(4)
#define PCIE_CLIENT_LTSSM_STATUS	0x300
#define SMLH_LINKUP			BIT(16)
#define RDLH_LINKUP			BIT(17)
#define PCIE_CLIENT_DBG_FIFO_MODE_CON	0x310
#define PCIE_CLIENT_DBG_FIFO_PTN_HIT_D0 0x320
#define PCIE_CLIENT_DBG_FIFO_PTN_HIT_D1 0x324
#define PCIE_CLIENT_DBG_FIFO_TRN_HIT_D0 0x328
#define PCIE_CLIENT_DBG_FIFO_TRN_HIT_D1 0x32c
#define PCIE_CLIENT_DBG_FIFO_STATUS	0x350
#define PCIE_CLIENT_DBG_TRANSITION_DATA	0xffff0000
#define PCIE_CLIENT_DBF_EN		0xffff0003

/* PCI DBICS registers */
#define PCIE_LINK_STATUS_REG		0x80
#define PCIE_LINK_STATUS_SPEED_OFF	16
#define PCIE_LINK_STATUS_SPEED_MASK	(0xf << PCIE_LINK_STATUS_SPEED_OFF)
#define PCIE_LINK_STATUS_WIDTH_OFF	20
#define PCIE_LINK_STATUS_WIDTH_MASK	(0xf << PCIE_LINK_STATUS_WIDTH_OFF)

#define PCIE_LINK_CAPABILITY		0x7c
#define PCIE_LINK_CTL_2			0xa0
#define TARGET_LINK_SPEED_MASK		0xf
#define LINK_SPEED_GEN_1		0x1
#define LINK_SPEED_GEN_2		0x2
#define LINK_SPEED_GEN_3		0x3

#define PCIE_MISC_CONTROL_1_OFF		0x8bc
#define PCIE_DBI_RO_WR_EN		BIT(0)

#define PCIE_LINK_WIDTH_SPEED_CONTROL	0x80c
#define PORT_LOGIC_SPEED_CHANGE		BIT(17)

/*
 * iATU Unroll-specific register definitions
 * From 4.80 core version the address translation will be made by unroll.
 * The registers are offset from atu_base
 */
#define PCIE_ATU_UNR_REGION_CTRL1	0x00
#define PCIE_ATU_UNR_REGION_CTRL2	0x04
#define PCIE_ATU_UNR_LOWER_BASE		0x08
#define PCIE_ATU_UNR_UPPER_BASE		0x0c
#define PCIE_ATU_UNR_LIMIT		0x10
#define PCIE_ATU_UNR_LOWER_TARGET	0x14
#define PCIE_ATU_UNR_UPPER_TARGET	0x18

#define PCIE_ATU_REGION_INDEX1		(0x1 << 0)
#define PCIE_ATU_REGION_INDEX0		(0x0 << 0)
#define PCIE_ATU_TYPE_MEM		(0x0 << 0)
#define PCIE_ATU_TYPE_IO		(0x2 << 0)
#define PCIE_ATU_TYPE_CFG0		(0x4 << 0)
#define PCIE_ATU_TYPE_CFG1		(0x5 << 0)
#define PCIE_ATU_ENABLE			(0x1 << 31)
#define PCIE_ATU_BAR_MODE_ENABLE	(0x1 << 30)
#define PCIE_ATU_BUS(x)			(((x) & 0xff) << 24)
#define PCIE_ATU_DEV(x)			(((x) & 0x1f) << 19)
#define PCIE_ATU_FUNC(x)		(((x) & 0x7) << 16)

/* Register address builder */
#define PCIE_GET_ATU_OUTB_UNR_REG_OFFSET(region)        \
	((0x3 << 20) | ((region) << 9))
#define PCIE_GET_ATU_INB_UNR_REG_OFFSET(region) \
	((0x3 << 20) | ((region) << 9) | (0x1 << 8))

/* Parameters for the waiting for iATU enabled routine */
#define LINK_WAIT_MAX_IATU_RETRIES	5
#define LINK_WAIT_IATU			10000

#define  PCI_BASE_ADDRESS_MEM_TYPE_64 0x04    /* 64 bit address */
#define PCI_BASE_ADDRESS_0      0x10    /* 32 bits */
#define PCI_BASE_ADDRESS_1      0x14    /* 32 bits [htype 0,1 only] */
#define PCI_INTERRUPT_LINE      0x3c    /* 8 bits */
#define PCI_PRIMARY_BUS         0x18    /* Primary bus number */
#define PCI_COMMAND             0x04    /* 16 bits */
#define PCI_COMMAND_MEMORY     0x2     /* Enable response in Memory space */
#define PCI_COMMAND_SERR		0x100	/* Enable SERR */
#define  PCI_COMMAND_MASTER     0x4     /* Enable bus mastering */
#define  PCI_COMMAND_IO         0x1     /* Enable response in I/O space */
#define PCI_CLASS_BRIDGE_PCI               0x0604
#define PCI_CLASS_DEVICE       0x0a    /* Device class */

/* 3.0 PHY Register for RK3588 */
#define PHP_GRF_PCIESEL_CON 0x100
#define RK3588_PCIE3PHY_GRF_CMN_CON0 0x0
#define RK3588_PCIE3PHY_GRF_PHY0_STATUS1 0x904
#define RK3588_PCIE3PHY_GRF_PHY1_STATUS1 0xa04
#define RK3588_SRAM_INIT_DONE(reg) (reg & BIT(0))

#define PHY_MODE_PCIE_AGGREGATION 4     /* PCIe3x4 */

#endif

