/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#ifndef __MDIO_DXE_H__
#define __MDIO_DXE_H__

#include <Uefi.h>

#define MVEBU_SMI_TIMEOUT      10000

/* SMI register fields */
#define  MVEBU_SMI_DATA_OFFS      0  /* Data */
#define  MVEBU_SMI_DATA_MASK      (0xffff << MVEBU_SMI_DATA_OFFS)
#define  MVEBU_SMI_DEV_ADDR_OFFS      16  /* PHY device address */
#define  MVEBU_SMI_REG_ADDR_OFFS      21  /* PHY device reg addr*/
#define  MVEBU_SMI_OPCODE_OFFS      26  /* Write/Read opcode */
#define  MVEBU_SMI_OPCODE_READ      (1 << MVEBU_SMI_OPCODE_OFFS)
#define  MVEBU_SMI_READ_VALID      (1 << 27)  /* Read Valid */
#define  MVEBU_SMI_BUSY        (1 << 28)  /* Busy */

#define  MVEBU_PHY_REG_MASK      0x1f
#define  MVEBU_PHY_ADDR_MASK      0x1f

#define MdioRegWrite32(x, y) MmioWrite32((y), (x))

#endif // __MDIO_DXE_H__
