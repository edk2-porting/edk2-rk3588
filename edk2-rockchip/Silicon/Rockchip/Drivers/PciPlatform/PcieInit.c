/** @file
*
*  Copyright (c) 2022, Rockchips Inc. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include "PcieInit.h"
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/OemMiscLib.h>
#include <Library/PlatformPciLib.h>
#include <Library/IoLib.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <Library/MemoryAllocationLib.h>
#include <stdio.h>
#include <Library/PcdLib.h>
#include <Library/RockchipPlatformLib.h>

extern VOID PcieRegWrite(UINT32 Port, UINTN Offset, UINT32 Value);
extern EFI_STATUS PciePortReset(UINT32 HostBridgeNum, UINT32 Port);


static UINTN rk_pcie_read(UINTN addr, UINTN size, UINT32 *val)
{
	if ((UINTN)addr & (size - 1)) {
		*val = 0;
		return PCIBIOS_UNSUPPORTED;
	}

	if (size == 4) {
		*val = MmioRead32(addr);
	} else if (size == 2) {
		*val = MmioRead16(addr);
	} else if (size == 1) {
		*val = MmioRead8(addr);
	} else {
		*val = 0;
		return PCIBIOS_NODEV;
	}

	return PCIBIOS_SUCCESSFUL;
}

static UINTN rk_pcie_write(UINTN addr, UINTN size, UINT32 val)
{
	if ((UINTN)addr & (size - 1))
		return PCIBIOS_UNSUPPORTED;

	if (size == 4)
		MmioWrite32(addr, val);
	else if (size == 2)
		MmioWrite16(addr, val);
	else if (size == 1)
		MmioWrite8(addr, val);
	else
		return PCIBIOS_NODEV;

	return PCIBIOS_SUCCESSFUL;
}

static UINTN __rk_pcie_read_apb(struct rk_pcie *rk_pcie, UINTN base,
			      UINT32 reg, UINTN size)
{
	int ret;
	UINT32 val;

	ret = rk_pcie_read(base + reg, size, &val);
	if (ret)
		DEBUG((EFI_D_ERROR, "Read APB address failed\n"));

	return val;
}

static void __rk_pcie_write_apb(struct rk_pcie *rk_pcie, UINTN base,
				UINT32 reg, UINTN size, UINT32 val)
{
	int ret;

	ret = rk_pcie_write(base + reg, size, val);
	if (ret)
		DEBUG((EFI_D_ERROR, "Write APB address failed\n"));
}

static inline UINT32 rk_pcie_readl_apb(struct rk_pcie *rk_pcie, UINT32 reg)
{
	return __rk_pcie_read_apb(rk_pcie, rk_pcie->apb_base, reg, 0x4);
}

static inline void rk_pcie_writel_apb(struct rk_pcie *rk_pcie, UINT32 reg,
				      UINT32 val)
{
	__rk_pcie_write_apb(rk_pcie, rk_pcie->apb_base, reg, 0x4, val);
}

static int rk_pcie_get_link_speed(struct rk_pcie *rk_pcie)
{
	return (MmioRead32(rk_pcie->dbi_base + PCIE_LINK_STATUS_REG) &
		PCIE_LINK_STATUS_SPEED_MASK) >> PCIE_LINK_STATUS_SPEED_OFF;
}

static int rk_pcie_get_link_width(struct rk_pcie *rk_pcie)
{
	return (MmioRead32(rk_pcie->dbi_base + PCIE_LINK_STATUS_REG) &
		PCIE_LINK_STATUS_WIDTH_MASK) >> PCIE_LINK_STATUS_WIDTH_OFF;
}

static inline void rk_pcie_dbi_write_enable(struct rk_pcie *rk_pcie, BOOLEAN en)
{
	UINT32 val;

	val = MmioRead32(rk_pcie->dbi_base + PCIE_MISC_CONTROL_1_OFF);

	if (en)
		val |= PCIE_DBI_RO_WR_EN;
	else
		val &= ~PCIE_DBI_RO_WR_EN;
	MmioWrite32(rk_pcie->dbi_base + PCIE_MISC_CONTROL_1_OFF, val);
}


static void rk_pcie_setup_host(struct rk_pcie *rk_pcie)
{

	UINT32 val;

	rk_pcie_dbi_write_enable(rk_pcie, TRUE);	

	/* setup RC BARs */
	MmioWrite32(rk_pcie->dbi_base + PCI_BASE_ADDRESS_0, PCI_BASE_ADDRESS_MEM_TYPE_64);
	MmioWrite32(rk_pcie->dbi_base + PCI_BASE_ADDRESS_1, 0x0);

	/* setup interrupt pins */
	val = MmioRead32(rk_pcie->dbi_base + PCI_INTERRUPT_LINE);
	val &= 0xffff00ff;
	val |= 0x00000100;
	//val |= 0x00000000;
	MmioWrite32(rk_pcie->dbi_base + PCI_INTERRUPT_LINE,val);

	/* setup bus numbers */
	val = MmioRead32(rk_pcie->dbi_base + PCI_PRIMARY_BUS);
	val &= 0xff000000;
	val |= 0x00ff0100;
	MmioWrite32(rk_pcie->dbi_base + PCI_PRIMARY_BUS, val);

	/* setup command register */
	val = MmioRead32(rk_pcie->dbi_base + PCI_COMMAND);
	val &= 0xffff0000;
	val |= PCI_COMMAND_IO | PCI_COMMAND_MEMORY |
		PCI_COMMAND_MASTER | PCI_COMMAND_SERR;
	MmioWrite32(rk_pcie->dbi_base + PCI_COMMAND, val);

	/* program correct class for RC */
	MmioWrite16(rk_pcie->dbi_base + PCI_CLASS_DEVICE, PCI_CLASS_BRIDGE_PCI);

	/* Better disable write permission right after the update */
	val = MmioRead32(rk_pcie->dbi_base + PCIE_LINK_WIDTH_SPEED_CONTROL);
	val |= PORT_LOGIC_SPEED_CHANGE;
	MmioWrite32(rk_pcie->dbi_base + PCIE_LINK_WIDTH_SPEED_CONTROL, val);

	rk_pcie_dbi_write_enable(rk_pcie, FALSE);
}

static void rk_pcie_configure(struct rk_pcie *pci, UINT32 cap_speed)
{
	UINT32 val;

	rk_pcie_dbi_write_enable(pci, TRUE);

	val = MmioRead32(pci->dbi_base + PCIE_LINK_CAPABILITY);
	val &= ~TARGET_LINK_SPEED_MASK;
	val |= cap_speed;
	MmioWrite32(pci->dbi_base + PCIE_LINK_CAPABILITY, val);

	val = MmioRead32(pci->dbi_base + PCIE_LINK_CTL_2);
	val &= ~TARGET_LINK_SPEED_MASK;
	val |= cap_speed;
	MmioWrite32(pci->dbi_base + PCIE_LINK_CTL_2, val);

	rk_pcie_dbi_write_enable(pci, FALSE);
}

static void rk_pcie_enable_debug(struct rk_pcie *rk_pcie)
{
#if RK_PCIE_DBG
	rk_pcie_writel_apb(rk_pcie, PCIE_CLIENT_DBG_FIFO_PTN_HIT_D0, PCIE_CLIENT_DBG_TRANSITION_DATA);
	rk_pcie_writel_apb(rk_pcie, PCIE_CLIENT_DBG_FIFO_PTN_HIT_D1, PCIE_CLIENT_DBG_TRANSITION_DATA);
	rk_pcie_writel_apb(rk_pcie, PCIE_CLIENT_DBG_FIFO_TRN_HIT_D0, PCIE_CLIENT_DBG_TRANSITION_DATA);
	rk_pcie_writel_apb(rk_pcie, PCIE_CLIENT_DBG_FIFO_TRN_HIT_D1, PCIE_CLIENT_DBG_TRANSITION_DATA);
	rk_pcie_writel_apb(rk_pcie, PCIE_CLIENT_DBG_FIFO_MODE_CON, PCIE_CLIENT_DBF_EN);
#endif
}
						 
static void rk_pcie_debug_dump(struct rk_pcie *rk_pcie)
{
#if RK_PCIE_DBG
	UINT32 loop;

	DEBUG((EFI_D_ERROR, "ltssm = 0x%x\n", rk_pcie_readl_apb(rk_pcie, PCIE_CLIENT_LTSSM_STATUS)));
	
	for (loop = 0; loop < 64; loop++)
		DEBUG((EFI_D_ERROR, "fifo_status = 0x%x\n", rk_pcie_readl_apb(rk_pcie, PCIE_CLIENT_DBG_FIFO_STATUS)));

#endif
}
						 
static inline void rk_pcie_link_status_clear(struct rk_pcie *rk_pcie)
{
	rk_pcie_writel_apb(rk_pcie, PCIE_CLIENT_GENERAL_DEBUG, 0x0);
}
						 
static inline void rk_pcie_disable_ltssm(struct rk_pcie *rk_pcie)
{
	rk_pcie_writel_apb(rk_pcie, 0x0, 0xc0008);
}
						 
static inline void rk_pcie_enable_ltssm(struct rk_pcie *rk_pcie)
{
	rk_pcie_writel_apb(rk_pcie, 0x0, 0xc000c);
}
						 
static int is_link_up(struct rk_pcie *priv)
{
	UINT32 val;
						 
	val = rk_pcie_readl_apb(priv, PCIE_CLIENT_LTSSM_STATUS);
	if ((val & (RDLH_LINKUP | SMLH_LINKUP)) == 0x30000)
		return 1;
						 
	return 0;
}
						 
static int rk_pcie_link_up(struct rk_pcie *priv, UINT32 cap_speed)
{
	int retries;

	
	if (is_link_up(priv)) {
		DEBUG((EFI_D_ERROR, "PCI Link already up before configuration!\n"));
		return 1;
	}
					 
	/* DW pre link configurations */
	rk_pcie_configure(priv, cap_speed);
			 
	/* Release the device */
	Pcie30PeReset(FALSE);

	rk_pcie_disable_ltssm(priv);

	rk_pcie_link_status_clear(priv);

	rk_pcie_enable_debug(priv);

	/* Enable LTSSM */
	rk_pcie_enable_ltssm(priv);

	for (retries = 0; retries < 500; retries++) {
		if (is_link_up(priv)) {
			DEBUG((EFI_D_ERROR, "PCIe Link up, LTSSM is 0x%x\n", rk_pcie_readl_apb(priv, PCIE_CLIENT_LTSSM_STATUS)));
			rk_pcie_debug_dump(priv);
			/* Link maybe in Gen switch recovery but we need to wait more 1s */
			msleep(1000);
			return 0;
		}

		DEBUG((EFI_D_ERROR, "PCIe Linking... LTSSM is 0x%x\n", rk_pcie_readl_apb(priv, PCIE_CLIENT_LTSSM_STATUS)));
		rk_pcie_debug_dump(priv);
		msleep(1000);
	}

	DEBUG((EFI_D_ERROR, "PCIe-%d Link Fail\n", 0));

	return -22;
}
						 
static int rockchip_pcie_init_port(struct rk_pcie *priv)
{
	int ret;
	UINT32 val;


	/* RK3588 EVB1 PCIe3L4*/
	Pcie30IoInit();
	
	/* Rest the device */
	Pcie30PeReset(TRUE);
	
	/* Set power and maybe external ref clk input */
	Pcie30PowerEn();
	msleep(100000);

	//DEBUG((EFI_D_ERROR, "0xFD5F8070 = 0x%x, 0xFEC4000c = 0x%x, 0xFEC50004 = 0x%x\n", 0));

	/* Disable power domain */
	MmioWrite32(0xFD8D8150, 0x1 << 23 | 0x1 << 21); //PD_PCIE & PD_PHP
					 
	/* FixMe init 3.0 PHY */
	/* Phy mode: Aggregation NBNB */
	MmioWrite32(0xfd5b8000 + RK3588_PCIE3PHY_GRF_CMN_CON0, (0x7 << 16) | PHY_MODE_PCIE_AGGREGATION);

	/* 开控制器和phy时钟,撤销复位        */
	MmioWrite32(0xFD7C0A80, 0xffff0000);  //CRU_SOFTRST_CON32
	MmioWrite32(0xFD7C0A84, 0xffff0000);  //CRU_SOFTRST_CON33
	MmioWrite32(0xFD7C0A88, 0xffff0000);  //CRU_SOFTRST_CON34
	MmioWrite32(0xFD7C0880, 0xffff0000);  //CRU_GATE_CON32
	MmioWrite32(0xFD7C0884, 0xffff0000);  //CRU_GATE_CON33
	MmioWrite32(0xFD7C0888, 0xffff0000);  //CRU_GATE_CON34
	MmioWrite32(0xFD7C0898, 0xffff0000);  //CRU_GATE_CON38
	MmioWrite32(0xFD7C089c, 0xffff0000);  //CRU_GATE_CON39
	
	MmioWrite32(0xFD7C8A00, (0x1 << 24));  //PHPTOPCRU_SOFTRST_CON00
	MmioWrite32(0xFD7C8800, 0xffff0000);  //PHPTOPCRU_GATE_CON00

	/* PHY复位 */
	MmioWrite32(0xFD7C8A00, (0x1 << 10) | (0x1 << 26)); 

	msleep(10);
	
	/* Deassert PCIe PMA output clamp mode */
	MmioWrite32(0xfd5b8000 + RK3588_PCIE3PHY_GRF_CMN_CON0, (0x1 << 8) | (0x1 << 24));

	/* 撤销PHY复位 */
	MmioWrite32(0xFD7C8A00, (0x1 << 26)); 
	
	/* 配置PHY:  3.0 PHY不用配置 */
						 
	/* LTSSM EN ctrl mode */
	val = rk_pcie_readl_apb(priv, PCIE_CLIENT_HOT_RESET_CTRL);
	val |= PCIE_LTSSM_ENABLE_ENHANCE | (PCIE_LTSSM_ENABLE_ENHANCE << 16);
	rk_pcie_writel_apb(priv, PCIE_CLIENT_HOT_RESET_CTRL, val);

	/* Set RC mode */
	rk_pcie_dbi_write_enable(priv, TRUE);	
	rk_pcie_writel_apb(priv, 0x0, 0xf00000);
	MmioWrite32(0xf50002ec, 0xfffff0);
	MmioWrite32(0xf50002f0, 0x13c0); //BAR0 resize 512G
	//io -4 0xa400008bc 0x77f41


	rk_pcie_writel_apb(priv, 0x0, 0xf00040);

	rk_pcie_setup_host(priv);
			 
	ret = rk_pcie_link_up(priv, priv->gen);
	if (ret < 0)
		return ret;

	DEBUG((EFI_D_ERROR, "PCIe Init sucessfully (Gen%d-x%d, Bus%d)\n",
			rk_pcie_get_link_speed(priv),rk_pcie_get_link_width(priv)));
	rk_pcie_dbi_write_enable(priv, TRUE);
	msleep(10000);

	return 0;
}


EFI_STATUS
PcieInitEntry (
  IN EFI_HANDLE                 ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable
  )
{
    EFI_STATUS         Status = EFI_SUCCESS;
	struct rk_pcie *priv;
	int ret;


	priv  = (struct rk_pcie *)AllocatePool (sizeof (struct rk_pcie));
	if (priv == NULL) {
		DEBUG((EFI_D_ERROR, "Failed to allocate priv memory!\n"));
  		return EFI_OUT_OF_RESOURCES;
	}


	memset(priv, 0, sizeof(struct rk_pcie));

	/* 先调试3L4，配置Gen 3 */
	priv->gen = 3;
	priv->lane = 4;
	priv->dbi_base = PcdGet64(PcdPcieRootPort3x4DbiBaseAddress);
	priv->apb_base = PcdGet64(PcdPcieRootPort3x4ApbBaseAddress);
	priv->cfg_base = PcdGet64(PcdPcieRootPort3x4CfgBaseAddress);

	DEBUG((EFI_D_ERROR, "dbi_base = 0x%lx   apb_base = 0x%lx  cfg_base = 0x%lx\n",
		priv->dbi_base, priv->apb_base, priv->cfg_base));	

	ret = rockchip_pcie_init_port(priv);
	if (ret < 0)
		return EFI_NO_MEDIA;

    return Status;
}
