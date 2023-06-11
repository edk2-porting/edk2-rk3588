/** @file
*
*  Copyright (c) 2021, Rockchip Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
#include <Library/DebugLib.h>
#include <Soc.h>

void DebugPrintHex(void *buf, UINT32 width, UINT32 len)
{
	UINT32 i,j;
	UINT8 *p8 = (UINT8 *) buf;
	UINT16 *p16 = (UINT16 *) buf;
	UINT32 *p32 =(UINT32 *) buf;

	j = 0;
	for (i = 0; i < len; i++) {
		if (j == 0)
			DebugPrint(DEBUG_ERROR, "%p + 0x%x:",buf, i * width);

		if (width == 4)
			DebugPrint(DEBUG_ERROR, "0x%08x,", p32[i]);
		else if (width == 2)
			DebugPrint(DEBUG_ERROR, "0x%04x,", p16[i]);
		else
			DebugPrint(DEBUG_ERROR, "0x%02x,", p8[i]);

		if (++j >= (16/width)) {
			j = 0;
			DebugPrint(DEBUG_ERROR, "\n","");
		}
	}
	DebugPrint(DEBUG_ERROR, "\n","");
}

void DwEmmcDxeIoMux(void)
{
  /* sdmmc0 iomux */
  GRF->GPIO1D_IOMUX_H = (0x7770UL << 16) | (0x111 << 4);
  GRF->GPIO2A_IOMUX_L = (0x777UL << 16) | (0x111 << 0);
  GRF->GPIO0A_IOMUX_H = (0x77 << 16) | (0x11 << 0);
  GRF->SDMMC_DET_COUNTER = 2;
}

