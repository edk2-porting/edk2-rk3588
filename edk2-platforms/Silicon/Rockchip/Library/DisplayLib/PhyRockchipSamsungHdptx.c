
/** @file

  Copyright (c) 2017, Linaro, Ltd. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AnalogixDpLib.h>
#include <Library/PWMLib.h>
#include <Library/DrmModes.h>


VOID
RockchipHdptxPhyInit(
  VOID
  )
{
  /* start up sequence for phy */
  MmioWrite32(0xFD7F0A0C,0xFFFF8000);

  MmioWrite32(EDPTX_PHY_BASE+0x0C78,0x00);

#if RK3588S_EVB1
  MmioWrite32(EDPTX_PHY_BASE+0x1478,0x00);
#elif ARMPC
  MmioWrite32(EDPTX_PHY_BASE+0x1078,0x00);
#else
  MmioWrite32(EDPTX_PHY_BASE+0x1078,0x04);
#endif
  MmioWrite32(EDPTX_PHY_BASE+0x1878,0x00);
  MmioWrite32(HDPTXPHY0_GRF_BASE,0xFFFF0001);
  /* rockchip_hDptx_phy_power_on */
  MmioWrite32(EDPTX_PHY_BASE+0x0800,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x0818,0x02);

  /* rockchip_hDptx_phy_Dp_pll_init  */
  MmioWrite32(EDPTX_PHY_BASE+0x0020,0x80);
  MmioWrite32(EDPTX_PHY_BASE+0x00F4,0xC0);
  MmioWrite32(EDPTX_PHY_BASE+0x0138,0x34);
  MmioWrite32(EDPTX_PHY_BASE+0x0144,0x87);
  MmioWrite32(0xFD5A402C,0xFFFF0011);  
  MmioWrite32(EDPTX_PHY_BASE+0x0148,0x71);
  MmioWrite32(EDPTX_PHY_BASE+0x014C,0x71);
  MmioWrite32(EDPTX_PHY_BASE+0x0154,0x87);
  MmioWrite32(EDPTX_PHY_BASE+0x0158,0x71);
  MmioWrite32(EDPTX_PHY_BASE+0x015C,0x71);
  MmioWrite32(EDPTX_PHY_BASE+0x0164,0x11);
  MmioWrite32(EDPTX_PHY_BASE+0x0168,0x31);
  MmioWrite32(EDPTX_PHY_BASE+0x016C,0x03);
  MmioWrite32(EDPTX_PHY_BASE+0x0178,0x7F);
  MmioWrite32(EDPTX_PHY_BASE+0x017C,0x31);
  MmioWrite32(EDPTX_PHY_BASE+0x0180,0x21);
  MmioWrite32(EDPTX_PHY_BASE+0x0184,0x27);
  MmioWrite32(EDPTX_PHY_BASE+0x0188,0x27);
  MmioWrite32(EDPTX_PHY_BASE+0x0194,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x0198,0x0D);
  MmioWrite32(EDPTX_PHY_BASE+0x019C,0x0D);
  MmioWrite32(EDPTX_PHY_BASE+0x01A4,0x02);
  MmioWrite32(EDPTX_PHY_BASE+0x01A8,0x09);
  MmioWrite32(EDPTX_PHY_BASE+0x01B0,0x03);
  MmioWrite32(EDPTX_PHY_BASE+0x01B4,0x07);
  MmioWrite32(EDPTX_PHY_BASE+0x01B8,0x07);
  MmioWrite32(EDPTX_PHY_BASE+0x01C0,0x08);
  MmioWrite32(EDPTX_PHY_BASE+0x01C4,0x18);
  MmioWrite32(EDPTX_PHY_BASE+0x01C8,0x18);
  MmioWrite32(EDPTX_PHY_BASE+0x01D0,0x0F);
  MmioWrite32(EDPTX_PHY_BASE+0x01DC,0x08);
  MmioWrite32(EDPTX_PHY_BASE+0x0118,0xEE);
  MmioWrite32(EDPTX_PHY_BASE+0x011C,0x24);
  MmioWrite32(EDPTX_PHY_BASE+0x0204,0x01);
  MmioWrite32(EDPTX_PHY_BASE+0x025C,0x02);
  MmioWrite32(EDPTX_PHY_BASE+0x021C,0x04);
  MmioWrite32(EDPTX_PHY_BASE+0x0204,0x01);
  MmioWrite32(EDPTX_PHY_BASE+0x0264,0x84);
  MmioWrite32(EDPTX_PHY_BASE+0x0208,0x04);
  MmioWrite32(EDPTX_PHY_BASE+0x00F0,0x80);
  MmioWrite32(EDPTX_PHY_BASE+0x020C,0x24);
  MmioWrite32(EDPTX_PHY_BASE+0x0214,0x03);
  MmioWrite32(EDPTX_PHY_BASE+0x0210,0x20);
  MmioWrite32(EDPTX_PHY_BASE+0x0268,0x01);
  MmioWrite32(EDPTX_PHY_BASE+0x026C,0x10);

  MmioWrite32(0xFDEC03C8,0xC8);
  /* rockchip_hDptx_phy_Dp_aux_init  */
  MmioWrite32(EDPTX_PHY_BASE+0x044C,0x13);
  MmioWrite32(EDPTX_PHY_BASE+0x0450,0x12);
  MmioWrite32(EDPTX_PHY_BASE+0x0454,0x12);
  MmioWrite32(EDPTX_PHY_BASE+0x0458,0x20);
  MmioWrite32(EDPTX_PHY_BASE+0x045C,0x04);
  MmioWrite32(EDPTX_PHY_BASE+0x0460,0x0A);
  MmioWrite32(EDPTX_PHY_BASE+0x0468,0x03);
#if ARMPC
  MmioWrite32(EDPTX_PHY_BASE+0x046C,0x13);
#else
  MmioWrite32(EDPTX_PHY_BASE+0x046C,0x03);
#endif
  MmioWrite32(EDPTX_PHY_BASE+0x0470,0x04);
  MmioWrite32(EDPTX_PHY_BASE+0x0474,0x67);
  MmioWrite32(EDPTX_PHY_BASE+0x0478,0x6a);
  MmioWrite32(EDPTX_PHY_BASE+0x047C,0x16);
  MmioWrite32(EDPTX_PHY_BASE+0x0434,0x10);
  MmioWrite32(EDPTX_PHY_BASE+0x0440,0x03);
  MmioWrite32(EDPTX_PHY_BASE+0x043C,0xC0);
  MmioWrite32(EDPTX_PHY_BASE+0x0408,0x33);
  MmioWrite32(EDPTX_PHY_BASE+0x040C,0x1B);
  MmioWrite32(EDPTX_PHY_BASE+0x047C,0x16);
  MmioWrite32(EDPTX_PHY_BASE+0x0410,0x33);
  MmioWrite32(EDPTX_PHY_BASE+0x0480,0x80);
  MmioWrite32(EDPTX_PHY_BASE+0x040C,0x1B);//double
  MmioWrite32(EDPTX_PHY_BASE+0x0410,0x33);
  MmioWrite32(EDPTX_PHY_BASE+0x0408,0x33);
  MmioWrite32(EDPTX_PHY_BASE+0x043C,0xC0);
  MmioWrite32(EDPTX_PHY_BASE+0x0410,0x33);//double 
  
  /* BGR_EN BIAS_EN */
  NanoSecondDelay (10000);
  MmioWrite32(HDPTXPHY0_GRF_BASE,0x10000);
  NanoSecondDelay (100000);
  MmioWrite32(HDPTXPHY0_GRF_BASE,0xFFFF0060);
  NanoSecondDelay (10000);

  MmioWrite32(EDPTX_PHY_BASE+0x040C,0x1B);
  MmioWrite32(EDPTX_PHY_BASE+0x0410,0x33);
  MmioWrite32(EDPTX_PHY_BASE+0x0408,0x33);
  MmioWrite32(EDPTX_PHY_BASE+0x043C,0xC0);
  MmioWrite32(EDPTX_PHY_BASE+0x0410,0x33);
  MmioWrite32(0xFED60028,0x83);
  MmioWrite32(0xFED60190,0x07);
}

VOID
RockchipHdpPhySetRate(
  VOID
  )
{
  DEBUG ((DEBUG_WARN, "[DRIVER]enter %a\n", __func__));
  /* RockchipHdpPhySetRate */
  MmioWrite32(HDPTXPHY0_GRF_BASE,0xFFFF0061);
  NanoSecondDelay (10000);
#if ARMPC
  MmioWrite32(EDPTX_PHY_BASE+0x081C,0x03);
  MmioWrite32(EDPTX_PHY_BASE+0x0254,0x01);
  NanoSecondDelay (10000);
#else
  MmioWrite32(EDPTX_PHY_BASE+0x081C,0x0F);
  MmioWrite32(EDPTX_PHY_BASE+0x0254,0x00);
  NanoSecondDelay (10000);
#endif
  MmioWrite32(EDPTX_PHY_BASE+0x01D0,0x0F);
  MmioWrite32(EDPTX_PHY_BASE+0x01D4,0x0E);
  MmioWrite32(EDPTX_PHY_BASE+0x01D8,0x68);
  NanoSecondDelay (10000);
#if ARMPC
  MmioWrite32(EDPTX_PHY_BASE+0x100C,0x26);

#else
  MmioWrite32(EDPTX_PHY_BASE+0x100C,0x23);

#endif
  MmioWrite32(EDPTX_PHY_BASE+0x1010,0x11);
  MmioWrite32(EDPTX_PHY_BASE+0x1014,0x44);
  MmioWrite32(EDPTX_PHY_BASE+0x101C,0x60);
  MmioWrite32(EDPTX_PHY_BASE+0x1040,0x03);
#if ARMPC
  MmioWrite32(EDPTX_PHY_BASE+0x1044,0x04);
#else
  MmioWrite32(EDPTX_PHY_BASE+0x1044,0x08);

#endif
  MmioWrite32(EDPTX_PHY_BASE+0x1058,0x02);
  MmioWrite32(EDPTX_PHY_BASE+0x106C,0x01);
#if ARMPC
  MmioWrite32(EDPTX_PHY_BASE+0x140C,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x1410,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x1414,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x1418,0x98);
  MmioWrite32(EDPTX_PHY_BASE+0x141C,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x1440,0x02);
  MmioWrite32(EDPTX_PHY_BASE+0x146C,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x180C,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x1818,0x98);
  MmioWrite32(EDPTX_PHY_BASE+0x181C,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x1840,0x02);
  MmioWrite32(EDPTX_PHY_BASE+0x1858,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x1854,0x21);
  MmioWrite32(EDPTX_PHY_BASE+0x186C,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x101C,0x60);
  MmioWrite32(EDPTX_PHY_BASE+0x141C,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x181C,0x00);
#else
  MmioWrite32(EDPTX_PHY_BASE+0x140C,0x23);
  MmioWrite32(EDPTX_PHY_BASE+0x1410,0x11);
  MmioWrite32(EDPTX_PHY_BASE+0x1414,0x44);
  MmioWrite32(EDPTX_PHY_BASE+0x141C,0x38);
  MmioWrite32(EDPTX_PHY_BASE+0x1440,0x03);
  MmioWrite32(EDPTX_PHY_BASE+0x146C,0x01);
  MmioWrite32(EDPTX_PHY_BASE+0x180C,0x23);
  MmioWrite32(EDPTX_PHY_BASE+0x181C,0x60);
  MmioWrite32(EDPTX_PHY_BASE+0x1840,0x03);
  MmioWrite32(EDPTX_PHY_BASE+0x1858,0x02);
  MmioWrite32(EDPTX_PHY_BASE+0x186C,0x01);
  MmioWrite32(EDPTX_PHY_BASE+0x101C,0x38);
  MmioWrite32(EDPTX_PHY_BASE+0x141C,0x38);
  MmioWrite32(EDPTX_PHY_BASE+0x181C,0x38);

#endif  
  MmioWrite32(0xFED60028,0x83);
  MmioWrite32(0xFED60190,0xffff0007);
  MmioWrite32(0xFED6046C,0x13);
  MmioWrite32(0xFED60C10,0x11);
  MmioWrite32(0xFED60C14,0x44);
  MmioWrite32(0xFED60C40,0x03);
  MmioWrite32(0xFED60C58,0x02);
#if ARMPC
  MmioWrite32(0xFED61028,0x10);
  MmioWrite32(0xFED61458,0x00);
  MmioWrite32(0xFED6145C,0x00);
  MmioWrite32(0xFED61810,0x00);
  MmioWrite32(0xFED61814,0x00);
#else
  MmioWrite32(0xFED61028,0x17);
  MmioWrite32(0xFED61458,0x02);
  MmioWrite32(0xFED6145C,0x00);
  MmioWrite32(0xFED61810,0x11);
  MmioWrite32(0xFED61814,0x44);

#endif
  MmioWrite32(HDPTXPHY0_GRF_BASE,0xFFFF00E1);
}

VOID
RockchipHdptxPhySetVoltage (
  VOID
  )
{
  DEBUG ((DEBUG_WARN, "[DRIVER]enter %a\n", __func__));

  /* RockchipHdptxPhySetVoltage */
#if ARMPC
  MmioWrite32(EDPTX_PHY_BASE+0x0C28,0x10);
  MmioWrite32(EDPTX_PHY_BASE+0x0C30,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x0C44,0x04);
  NanoSecondDelay (10000);
  MmioWrite32(EDPTX_PHY_BASE+0x1828,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x1830,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x1844,0x00);
  NanoSecondDelay (10000);
  MmioWrite32(EDPTX_PHY_BASE+0x1028,0x10);
  MmioWrite32(EDPTX_PHY_BASE+0x1030,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x1044,0x04);
  NanoSecondDelay (10000);
  MmioWrite32(EDPTX_PHY_BASE+0x1428,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x1430,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x1444,0x00);
  NanoSecondDelay (10000);
  MmioWrite32(EDPTX_PHY_BASE+0x0c0c,0x26);
  MmioWrite32(EDPTX_PHY_BASE+0x0c1c,0x60);
  MmioWrite32(EDPTX_PHY_BASE+0x0c2c,0x70);
  MmioWrite32(EDPTX_PHY_BASE+0x0c34,0x70);
  MmioWrite32(EDPTX_PHY_BASE+0x0c6c,0x01);
  MmioWrite32(EDPTX_PHY_BASE+0x102c,0x70);
  MmioWrite32(EDPTX_PHY_BASE+0x1034,0x70);
#else
  MmioWrite32(EDPTX_PHY_BASE+0x0C28,0x17);
  MmioWrite32(EDPTX_PHY_BASE+0x0C30,0x07);
  MmioWrite32(EDPTX_PHY_BASE+0x0C44,0x08);
  NanoSecondDelay (10000);
  MmioWrite32(EDPTX_PHY_BASE+0x1828,0x17);
  MmioWrite32(EDPTX_PHY_BASE+0x1844,0x08);
  NanoSecondDelay (10000);
  MmioWrite32(EDPTX_PHY_BASE+0x1028,0x17);
  MmioWrite32(EDPTX_PHY_BASE+0x1030,0x07);
  MmioWrite32(EDPTX_PHY_BASE+0x1044,0x08);
  NanoSecondDelay (10000);
  MmioWrite32(EDPTX_PHY_BASE+0x1428,0x17);
  MmioWrite32(EDPTX_PHY_BASE+0x1430,0x07);
  MmioWrite32(EDPTX_PHY_BASE+0x1444,0x08);
  NanoSecondDelay (10000);
#endif
}

