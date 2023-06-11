/** @file
  Rockchip eDP Driver.

  Copyright (c) 2022 Rockchip Electronics Co. Ltd.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/DebugLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AnalogixDpLib.h>

#define COMMON_INT_MASK_1       0
#define COMMON_INT_MASK_2       0
#define COMMON_INT_MASK_3       0
#define COMMON_INT_MASK_4       (HOTPLUG_CHG | HPD_LOST | PLUG)
#define INT_STA_MASK            INT_HPD

VOID
AnalogixDpRegWrite (
  OUT struct  AnalogixDpDevice *Dp,
  IN  UINT32  Offset,
  IN  UINT32  Value
  )
{
  UINT32 BASE;

  if (!Dp->Id)
    BASE = ANALOGIX_DP0_REG_BASE;
  else
    BASE = ANALOGIX_DP1_REG_BASE;

  MmioWrite32(BASE + Offset, Value);
}

UINT32
AnalogixDpRegRead (
  OUT struct  AnalogixDpDevice *Dp,
  IN  UINT32  Offset
  )
{
  UINT32 Reg;
  UINT32 BASE;

  if (!Dp->Id)
    BASE = ANALOGIX_DP0_REG_BASE;
  else
    BASE = ANALOGIX_DP1_REG_BASE;

  Reg = MmioRead32(BASE + Offset);
  return Reg;
}

VOID
AnalogixDpStopVideo (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_VIDEO_CTL_1);
  Reg &= ~VIDEO_EN;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_VIDEO_CTL_1, Reg);
}

VOID
AnalogixDpSetLaneMap (
  OUT struct AnalogixDpDevice *Dp
  )
{
  //Todo
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_LANE_MAP, 0xE4);
}


VOID
AnalogixDpEnableVideoMute (
  OUT struct AnalogixDpDevice *Dp,
  BOOLEAN Enable
  )
{
  UINT32 Reg;

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_VIDEO_CTL_1);
  if (Enable) {
    Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_VIDEO_CTL_1);
    Reg |= HDCP_VIDEO_MUTE;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_VIDEO_CTL_1,Reg);
  } else {
    Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_VIDEO_CTL_1);
    Reg &= ~HDCP_VIDEO_MUTE;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_VIDEO_CTL_1,Reg);
  }
}

VOID
AnalogixDpLaneSwap (
  OUT struct AnalogixDpDevice *Dp,
  BOOLEAN  Enable
  )
{
  //Todo
  UINT32 Reg;

  if (Enable)
    Reg = LANE3_MAP_LOGIC_LANE_0 | LANE2_MAP_LOGIC_LANE_1 |
      LANE1_MAP_LOGIC_LANE_2 | LANE0_MAP_LOGIC_LANE_3;
  else
    Reg = LANE3_MAP_LOGIC_LANE_3 | LANE2_MAP_LOGIC_LANE_2 |
      LANE1_MAP_LOGIC_LANE_1 | LANE0_MAP_LOGIC_LANE_0;

  AnalogixDpRegWrite(Dp, ANALOGIX_DP_LANE_MAP, Reg);
}

VOID
AnalogixDpReset (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;

  AnalogixDpStopVideo(Dp);
  AnalogixDpEnableVideoMute(Dp, 0);

  Reg = MASTER_VID_FUNC_EN_N | SLAVE_VID_FUNC_EN_N |
    AUD_FIFO_FUNC_EN_N | AUD_FUNC_EN_N |
    HDCP_FUNC_EN_N | SW_FUNC_EN_N;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_FUNC_EN_1, Reg);

  Reg = SSC_FUNC_EN_N | AUX_FUNC_EN_N |
    SERDES_FIFO_FUNC_EN_N |
    LS_CLK_DOMAIN_FUNC_EN_N;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_FUNC_EN_2, Reg);

  NanoSecondDelay (30000);

  AnalogixDpSetLaneMap(Dp);

  AnalogixDpRegWrite(Dp, ANALOGIX_DP_SYS_CTL_1, 0x0);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_SYS_CTL_2, 0x40);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_SYS_CTL_3, 0x0);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_SYS_CTL_4, 0x0);

  AnalogixDpRegWrite(Dp, ANALOGIX_DP_PKT_SEND_CTL, 0x0);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_HDCP_CTL, 0x0);

  AnalogixDpRegWrite(Dp, ANALOGIX_DP_HPD_DEGLITCH_L, 0x5e);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_HPD_DEGLITCH_H, 0x1a);

  AnalogixDpRegWrite(Dp, ANALOGIX_DP_LINK_DEBUG_CTL, 0x10);

  AnalogixDpRegWrite(Dp, ANALOGIX_DP_PHY_TEST, 0x0);

  AnalogixDpRegWrite(Dp, ANALOGIX_DP_VIDEO_FIFO_THRD, 0x0);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUDIO_MARGIN, 0x20);

  AnalogixDpRegWrite(Dp, ANALOGIX_DP_M_VID_GEN_FILTER_TH, 0x4);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_M_AUD_GEN_FILTER_TH, 0x2);

  AnalogixDpRegWrite(Dp, ANALOGIX_DP_SOC_GENERAL_CTL, 0x00000101);
}

VOID
AnalogixDpSwreset (
  OUT struct AnalogixDpDevice *Dp
  )
{
  /* SW_RESET */
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_TX_SW_RESET,RESET_DP_TX);
}

VOID
AnalogixDpInitAnalogParam (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;

  Reg = TX_TERMINAL_CTRL_50_OHM;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_ANALOG_CTL_1, Reg);

  Reg = SEL_24M | TX_DVDD_BIT_1_0625V;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_ANALOG_CTL_2, Reg);
  
  Reg = REF_CLK_24M;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_PLL_REG_1, Reg);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_PLL_REG_2, 0x99);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_PLL_REG_3, 0x40);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_PLL_REG_4, 0x58);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_PLL_REG_5, 0x22);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_BIAS, 0x44);

  Reg = DRIVE_DVDD_BIT_1_0625V | VCO_BIT_600_MICRO;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_ANALOG_CTL_3, Reg);

  Reg = PD_RING_OSC | AUX_TERMINAL_CTRL_50_OHM |
    TX_CUR1_2X | TX_CUR_16_MA;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_PLL_FILTER_CTL_1, Reg);

  Reg = CH3_AMP_400_MV | CH2_AMP_400_MV |
    CH1_AMP_400_MV | CH0_AMP_400_MV;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_TX_AMP_TUNING_CTL, Reg);

  MicroSecondDelay (10);
}

VOID
AnalogixDpInitInterrupt (
  OUT struct AnalogixDpDevice *Dp
  )
{
  /* Set interrupt pin assertion polarity as high */
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_INT_CTL,INT_POL1 | INT_POL0);

  /* Clear pending Regisers */
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_COMMON_INT_STA_1, 0xff);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_COMMON_INT_STA_2, 0x4f);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_COMMON_INT_STA_3, 0xe0);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_COMMON_INT_STA_4, 0xe7);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_INT_STA_MASK, 0x63);

  /* 0:mask,1: unmask */
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_COMMON_INT_MASK_1, 0x00);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_COMMON_INT_MASK_2, 0x00);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_COMMON_INT_MASK_3, 0x00);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_COMMON_INT_MASK_4, 0x00);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_INT_STA_MASK, 0x00);
}

VOID
AnalogixDpEnableSwFunction (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_FUNC_EN_1);
  Reg &= ~SW_FUNC_EN_N;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_FUNC_EN_1, Reg);
}

VOID
AnalogixDpConfigInterrupt (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;

  /* 0: mask, 1: unmask */
  Reg = COMMON_INT_MASK_1;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_COMMON_INT_MASK_1, Reg);
  Reg = COMMON_INT_MASK_2;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_COMMON_INT_MASK_2, Reg);

  Reg = COMMON_INT_MASK_3;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_COMMON_INT_MASK_3, Reg);

  Reg = COMMON_INT_MASK_4;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_COMMON_INT_MASK_4, Reg);
  Reg = INT_STA_MASK;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_INT_STA_MASK, Reg);

}

VOID
AnalogixDpSetAnalogPowerDown (
  OUT struct AnalogixDpDevice *Dp,
  enum AnalogPowerBlock Block,
  BOOLEAN Enable
  )
{
  UINT32 Reg;
  UINT32 PhyPdAddr = ANALOGIX_DP_PD;
  //Todo
  switch (Block) {
  case AUX_BLOCK:
    if (Enable) {
      Reg = AnalogixDpRegRead(Dp, PhyPdAddr);
      Reg |= AUX_PD;
      AnalogixDpRegWrite(Dp, PhyPdAddr, Reg);
    } else {
      Reg = AnalogixDpRegRead(Dp, PhyPdAddr);
      Reg &= ~AUX_PD;
      AnalogixDpRegWrite(Dp, PhyPdAddr, Reg);
    }
    break;
  case CH0_BLOCK:
    if (Enable) {
      Reg = AnalogixDpRegRead(Dp, PhyPdAddr);
      Reg |= CH0_PD;
      AnalogixDpRegWrite(Dp, PhyPdAddr, Reg);
    } else {
      Reg = AnalogixDpRegRead(Dp, PhyPdAddr);
      Reg &= ~CH0_PD;
      AnalogixDpRegWrite(Dp, PhyPdAddr, Reg);
    }
    break;
  case CH1_BLOCK:
    if (Enable) {
      Reg = AnalogixDpRegRead(Dp, PhyPdAddr);
      Reg |= CH1_PD;
      AnalogixDpRegWrite(Dp, PhyPdAddr, Reg);
    } else {
      Reg = AnalogixDpRegRead(Dp, PhyPdAddr);
      Reg &= ~CH1_PD;
      AnalogixDpRegWrite(Dp, PhyPdAddr, Reg);
    }
    break;
  case CH2_BLOCK:
    if (Enable) {
      Reg = AnalogixDpRegRead(Dp, PhyPdAddr);
      Reg |= CH2_PD;
      AnalogixDpRegWrite(Dp, PhyPdAddr, Reg);
    } else {
      Reg = AnalogixDpRegRead(Dp, PhyPdAddr);
      Reg &= ~CH2_PD;
      AnalogixDpRegWrite(Dp, PhyPdAddr, Reg);
    }
    break;
  case CH3_BLOCK:
    if (Enable) {
      Reg = AnalogixDpRegRead(Dp, PhyPdAddr);
      Reg |= CH3_PD;
      AnalogixDpRegWrite(Dp, PhyPdAddr, Reg);
    } else {
      Reg = AnalogixDpRegRead(Dp, PhyPdAddr);
      Reg &= ~CH3_PD;
      AnalogixDpRegWrite(Dp, PhyPdAddr, Reg);
    }
    break;
  case ANALOG_TOTAL:
    if (Enable) {
      Reg = AnalogixDpRegRead(Dp, PhyPdAddr);
      Reg |= DP_PHY_PD;
      AnalogixDpRegWrite(Dp, PhyPdAddr, Reg);
    } else {
      Reg = AnalogixDpRegRead(Dp, PhyPdAddr);
      Reg &= ~DP_PHY_PD;
      AnalogixDpRegWrite(Dp, PhyPdAddr, Reg);
    }
    break;
  case POWER_ALL:
    if (Enable) {
      Reg = DP_PHY_PD | AUX_PD | CH3_PD | CH2_PD |
        CH1_PD | CH0_PD;
      AnalogixDpRegWrite(Dp, PhyPdAddr, Reg);
    } else {
      AnalogixDpRegWrite(Dp, PhyPdAddr, 0x00);
    }
    break;
  default:
    break;
  }
}

VOID
AnalogixDpSetPllPowerDown (
  OUT struct AnalogixDpDevice *Dp,
  BOOLEAN Enable
  )
{
  UINT32 Reg;

  if (Enable) {
    Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_PLL_CTL);
    Reg |= DP_PLL_PD;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_PLL_CTL,Reg);
  } else {
    Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_PLL_CTL);
    Reg &= ~DP_PLL_PD;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_PLL_CTL,Reg);
  }
}

VOID
AnalogixDpInitAnalogFunc (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;

  AnalogixDpSetAnalogPowerDown(Dp, POWER_ALL, 0);

  Reg = PLL_LOCK_CHG;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_COMMON_INT_STA_1, Reg);

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_DEBUG_CTL);
  Reg &= ~(F_PLL_LOCK | PLL_LOCK_CTRL);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_DEBUG_CTL, Reg);

  /* Power up PLL */
  AnalogixDpSetPllPowerDown(Dp, 0);
	
  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_FUNC_EN_2);
  Reg &=  ~(SERDES_FIFO_FUNC_EN_N | LS_CLK_DOMAIN_FUNC_EN_N
      | AUX_FUNC_EN_N);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_FUNC_EN_2, Reg);
}

VOID
AnalogixDpClearHotplugInterrupts (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;

  //TODO
  Reg = HOTPLUG_CHG | HPD_LOST | PLUG;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_COMMON_INT_STA_4, Reg);

  Reg = INT_HPD;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_INT_STA, Reg);
}

VOID
AnalogixDpInitHpd (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;

  AnalogixDpClearHotplugInterrupts(Dp);

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_SYS_CTL_3);
  Reg &= ~(F_HPD | HPD_CTRL);

  AnalogixDpRegWrite(Dp, ANALOGIX_DP_SYS_CTL_3, Reg);
}

VOID
AnalogixDpResetAux (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;

  /* Disable AUX channel module */
  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_FUNC_EN_2);
  Reg |= AUX_FUNC_EN_N;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_FUNC_EN_2, Reg);
}

VOID
AnalogixDpInitAux (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;

  /* Clear inerrupts related to AUX channel */
  Reg = RPLY_RECEIV | AUX_ERR;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_INT_STA, Reg);

  AnalogixDpResetAux(Dp);

  Reg = AUX_BIT_PERIOD_EXPECTED_DELAY(0) |
  AUX_HW_RETRY_COUNT_SEL(3) |
  AUX_HW_RETRY_INTERVAL_600_MICROSECONDS;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_HW_RETRY_CTL, Reg);

  /* Receive AUX Channel DEFER commands equal to DEFFER_COUNT*64 */
  Reg = DEFER_CTRL_EN | DEFER_COUNT(1);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_CH_DEFER_CTL, Reg);

  /* Enable AUX channel module */
  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_FUNC_EN_2);
  Reg &= ~AUX_FUNC_EN_N;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_FUNC_EN_2, Reg);
}

INT32
AnalogixDpWriteByteToDpcd (
  OUT struct AnalogixDpDevice *Dp,
  IN UINT32 RegAddr,
  IN UINT8 Data
  )
{
  UINT32 Reg;
  UINTN i;
  UINTN Retval;

  for (i = 0; i < 3; i++) {
    /* Clear AUX CH data buffer */
    Reg = BUF_CLR;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_BUFFER_DATA_CTL, Reg);

    /* Select DPCD device address */
    Reg = AUX_ADDR_7_0(RegAddr);
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_ADDR_7_0, Reg);
    Reg = AUX_ADDR_15_8(RegAddr);
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_ADDR_15_8, Reg);
    Reg = AUX_ADDR_19_16(RegAddr);
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_ADDR_19_16, Reg);

    /* write Data buffer */
    Reg = (UINT32)Data;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_BUF_DATA_0, Reg);

    /*
     *  Set DisplayPort transaction and write 1 byte
     *  If bit 3 is 1, DisplayPort transaction.
     *  If Bit 3 is 0, I2C transaction.
     */
    Reg = AUX_TX_COMM_DP_TRANSACTION | AUX_TX_COMM_WRITE;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_CH_CTL_1, Reg);

    Retval = AnalogixDpStartAuxTransaction(Dp);
    if (Retval == 0)
      break;
  }

  return Retval;
}

INT32
AnalogixDpWriteBytesToDpcd (
  OUT struct AnalogixDpDevice *Dp,
  IN  UINT32 RegAddr,
  IN  UINT32 Count,
  IN  UINT8 Data[]
  )
{
  UINT32 Reg;
  UINT32 StartOffset;
  UINT32 CurDataCount, CurDataIdx;
  INT32 Retval = 0;
  INT32 i;

  /* Clear AUX CH data buffer */
  Reg = BUF_CLR;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_BUFFER_DATA_CTL, Reg);
  
  StartOffset = 0;
  while (StartOffset < Count) {
    /* Buffer size of AUX CH is 16 * 4bytes */
    if ((Count - StartOffset) > 16)
      CurDataCount = 16;
    else
      CurDataCount = Count - StartOffset;

    for (i = 0; i < 3; i++) {
      /* Select DPCD device address */
      Reg = AUX_ADDR_7_0(RegAddr + StartOffset);
      AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_ADDR_7_0, Reg);
      Reg = AUX_ADDR_15_8(RegAddr + StartOffset);
      AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_ADDR_15_8, Reg);
      Reg = AUX_ADDR_19_16(RegAddr + StartOffset);
      AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_ADDR_19_16, Reg);

      for (CurDataIdx = 0; CurDataIdx < CurDataCount;
          CurDataIdx++) {
        Reg = Data[StartOffset + CurDataIdx];
        AnalogixDpRegWrite(Dp, ANALOGIX_DP_BUF_DATA_0 +
            4 * CurDataIdx, Reg);
      }

      /*
       * Set DisplayPort transaction and write
       * If bit 3 is 1, DisplayPort transaction.
       * If Bit 3 is 0, I2C transaction.
       */
      Reg = AUX_LENGTH(CurDataCount) |
        AUX_TX_COMM_DP_TRANSACTION | AUX_TX_COMM_WRITE;
      AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_CH_CTL_1, Reg);

      /* Start AUX transaction */
      Retval = AnalogixDpStartAuxTransaction(Dp);
      if (Retval == 0)
        break;
    }
    StartOffset += CurDataCount;
  }
    return Retval;
}

INT32
AnalogixDpReadByteFromDpcd (
  OUT  struct AnalogixDpDevice *Dp,
  IN   UINT32 RegAddr,
  IN   UINT8 *Data
  )
{
  UINT32 Reg;
  INT32 i;
  INT32 Retval;

  for (i = 0; i < 3; i++) {
    /* Clear AUX CH data buffer */
    Reg = BUF_CLR;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_BUFFER_DATA_CTL, Reg);

    /* Select DPCD device address */
    Reg = AUX_ADDR_7_0(RegAddr);
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_ADDR_7_0, Reg);
    Reg = AUX_ADDR_15_8(RegAddr);
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_ADDR_15_8, Reg);
    Reg = AUX_ADDR_19_16(RegAddr);
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_ADDR_19_16, Reg);

    /*
     * Set DisplayPort transaction and read 1 byte
     * If bit 3 is 1, DisplayPort transaction.
     * If Bit 3 is 0, I2C transaction.
     */
    Reg = AUX_TX_COMM_DP_TRANSACTION | AUX_TX_COMM_READ;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_CH_CTL_1, Reg);

    /* Start AUX transaction */
    Retval = AnalogixDpStartAuxTransaction(Dp);
    if (Retval == 0)
      break;
  }

  /* Read data buffer */
  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_BUF_DATA_0);
  *Data = (UINT8)(Reg & 0xff);

  return Retval;
}

INT32
AnalogixDpStartAuxTransaction (
  OUT struct AnalogixDpDevice *Dp
  )
{
  INT32 Reg;
  INT32 Retval = 0;
  INT32 TimeOutLoop =0;

  /* Enable AUX CH operation */
  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_AUX_CH_CTL_2);
  Reg |= AUX_EN;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_CH_CTL_2, Reg);

  /* Is AUX CH command reply received? */
  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_INT_STA);

  while (!(Reg & RPLY_RECEIV)) {
    TimeOutLoop++;
    if (DP_TIMEOUT_LOOP_COUNT < TimeOutLoop) {
      DEBUG ((EFI_D_WARN, "AUX CH command reply failed!\n"));
      return -ETIMEDOUT;
    }

    Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_INT_STA);
    NanoSecondDelay (11000);
  }

  /* Clear interrupt source for AUX CH command reply */
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_INT_STA, Reg);

  /* Clear interrupt source for AUX CH access error */
  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_INT_STA);
  if (Reg & AUX_ERR) {
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_INT_STA,AUX_ERR);
    return -EREMOTEIO;
  }

  /* Check AUX CH error access status */
  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_AUX_CH_STA);
  if ((Reg & AUX_STATUS_MASK) != 0) {
    DEBUG ((EFI_D_ERROR,"AUX CH error happens: %d \n",
          Reg & AUX_STATUS_MASK));
    return -EREMOTEIO;
  }

  return Retval;
}

INT32
AnalogixDpReadBytesFromDpcd (
  OUT struct AnalogixDpDevice *Dp,
  IN  UINT32 RegAddr,
  IN  UINT32 Count,
  IN  UINT8 Data[]
  )
{
  UINT32 Reg;
  UINT32 StartOffset;
  UINT32 CurDataCount, CurDataIdx;
  INT32 i;
  INT32 Retval = 0;

  /* Clear AUX CH data buffer*/
  Reg = BUF_CLR;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_BUFFER_DATA_CTL, Reg);

  StartOffset = 0;
  while (StartOffset < Count) {
    /* Buffer size of AUX CH is 16 * 4 bytes */
    if ((Count - StartOffset) > 16)
      CurDataCount = 16;
    else
      CurDataCount = Count - StartOffset;

    /* AUX CH Request Transaction process */
    for (i = 0; i < 3; i++) {
      /* Select DPCD device address */
      Reg = AUX_ADDR_7_0(RegAddr + StartOffset);
      AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_ADDR_7_0, Reg);
      Reg = AUX_ADDR_15_8(RegAddr + StartOffset);
      AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_ADDR_15_8, Reg);
      Reg = AUX_ADDR_19_16(RegAddr + StartOffset);
      AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_ADDR_19_16, Reg);

      /* 
       * Set DisplayPort transaction and read
       * If bit 3 is 1,DisplayPort transaction
       * If bit 3 is 0,I2C transaction
       */
      Reg = AUX_LENGTH(CurDataCount) |
        AUX_TX_COMM_DP_TRANSACTION | AUX_TX_COMM_READ;
      AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_CH_CTL_1, Reg);

      /* Start AUX transaction */
      Retval = AnalogixDpStartAuxTransaction(Dp);
      if (Retval == 0)
        break;
    }

    for (CurDataIdx = 0; CurDataIdx < CurDataCount;
        CurDataIdx++) {
      Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_BUF_DATA_0
          + 4 * CurDataIdx);
      Data[StartOffset + CurDataIdx] =
        (UINT8)Reg;
    }

    StartOffset += CurDataCount;
  }

  return Retval;
}

INTN
AnalogixDpSelectI2cDevice (
  OUT struct AnalogixDpDevice *Dp,
  UINTN DeviceAddr,
  UINTN RegAddr
  )
{
  UINT32 Reg;
  INTN Retval;

  // Todo
  /* Set EDID device address */
  Reg = DeviceAddr;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_ADDR_7_0, Reg);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_ADDR_15_8, 0x0);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_ADDR_19_16, 0x0);

  /* Set offset from base address of EDID device */
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_BUF_DATA_0, RegAddr);

  /*
   * Set I2C transaction and write address
   * If bit 3 is 1, DisplayPort transaction.
   * If Bit 3 is 0, I2C transaction.
   */
  Reg = AUX_TX_COMM_I2C_TRANSACTION | AUX_TX_COMM_MOT |
    AUX_TX_COMM_WRITE;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_CH_CTL_1, Reg);

  /* Start AUX transaction */
  Retval = AnalogixDpStartAuxTransaction(Dp);
  if (Retval < 0)
    return Retval;

  return 0;
}

INTN
AnalogixDpReadByteFromI2c (
  struct AnalogixDpDevice *Dp,
  UINTN DeviceAddr,
  UINTN RegAddr,
  UINTN *DATA
  )
{
  UINT32 Reg;
  INTN i;
  INTN Retval;

  // Todo 
  for (i = 0; i < 3; i++) {
    /* Clear AUX CH data buffer */
    Reg = BUF_CLR;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_BUFFER_DATA_CTL, Reg);

    /* Select EDID device */
    Retval = AnalogixDpSelectI2cDevice(Dp, DeviceAddr,
        RegAddr);
    if (Retval != 0)
      continue;

    /*
     * Set I2C transaction and read data
     * If bit 3 is 1, DisplayPort transaction.
     * If Bit 3 is 0, I2C transaction.
     */
    Reg = AUX_TX_COMM_I2C_TRANSACTION |
      AUX_TX_COMM_READ;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_CH_CTL_1, Reg);

    /* Start AUX transaction */
    Retval = AnalogixDpStartAuxTransaction(Dp);
    if (Retval == 0)
      break;
  }

  /* Read data */
  if (Retval == 0)
    *DATA = AnalogixDpRegRead(Dp, ANALOGIX_DP_BUF_DATA_0);

  return Retval;
}

INTN 
AnalogixDpReadBytesFromI2c (
  OUT struct AnalogixDpDevice *Dp,
  UINTN DeviceAddr,
  UINTN RegAddr,
  UINTN Count,
  UINT8 Edid[])
{
  UINT32 Reg;
  UINTN i, j;
  UINTN CurDataIdx;
  UINTN Defer = 0;
  INT32 Retval = 0;

  // Todo
  for (i = 0; i < Count; i += 16) {
    for (j = 0; j < 3; j++) {
      /* Clear AUX CH data buffer */
      Reg = BUF_CLR;
      AnalogixDpRegWrite(Dp, ANALOGIX_DP_BUFFER_DATA_CTL, Reg);

      /* Set normal AUX CH command */
      Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_AUX_CH_CTL_2);
      Reg &= ~ADDR_ONLY;
      AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_CH_CTL_2, Reg);

      /*
       * If Rx sends defer, Tx sends only reads
       * request without sending address
       */
      if (!Defer)
        Retval = AnalogixDpSelectI2cDevice(Dp,
            DeviceAddr, RegAddr + i);
      else
        Defer = 0;

      if (Retval == 0) {
        /*
         * Set I2C transaction and write data
         * If bit 3 is 1, DisplayPort transaction.
         * If Bit 3 is 0, I2C transaction.
         */
      Reg = AUX_LENGTH(16) |
        AUX_TX_COMM_I2C_TRANSACTION |
        AUX_TX_COMM_READ;
      AnalogixDpRegWrite(Dp, ANALOGIX_DP_AUX_CH_CTL_1,
          Reg);

      /* Start AUX transaction */
      Retval = AnalogixDpStartAuxTransaction(Dp);
      if (Retval == 0)
        break;
      }
      /* Check if Rx sends defer */
      Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_AUX_RX_COMM);
      if (Reg == AUX_RX_COMM_AUX_DEFER ||
          Reg == AUX_RX_COMM_I2C_DEFER)
        Defer = 1;
    }

    for (CurDataIdx = 0; CurDataIdx < 16; CurDataIdx++) {
      Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_BUF_DATA_0
          + 4 * CurDataIdx);
      Edid[i + CurDataIdx] = (UINT8)Reg;
    }
  }

  return Retval;
}

VOID
AnalogixDpSetTrainingPattern (
  OUT struct AnalogixDpDevice *Dp,
  OUT enum PatternSet Pattern
  )
{
  UINT32 Reg;

  switch (Pattern) {
  case PRBS7:
    Reg = SCRAMBLING_ENABLE | LINK_QUAL_PATTERN_SET_PRBS7;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_TRAINING_PTN_SET, Reg);
    break;
  case D10_2:
    Reg = SCRAMBLING_ENABLE | LINK_QUAL_PATTERN_SET_D10_2;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_TRAINING_PTN_SET, Reg);
    break;
  case TRAINING_PTN1:
    Reg = SCRAMBLING_DISABLE | SW_TRAINING_PATTERN_SET_PTN1;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_TRAINING_PTN_SET, Reg);
    break;
  case TRAINING_PTN2:
    Reg = SCRAMBLING_DISABLE | SW_TRAINING_PATTERN_SET_PTN2;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_TRAINING_PTN_SET, Reg);
    break;
  case TRAINING_PTN3:
    Reg = SCRAMBLING_DISABLE | SW_TRAINING_PATTERN_SET_PTN3;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_TRAINING_PTN_SET, Reg);
    break;
  case DP_NONE:
    Reg = SCRAMBLING_ENABLE |
      LINK_QUAL_PATTERN_SET_DISABLE |
      SW_TRAINING_PATTERN_SET_NORMAL;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_TRAINING_PTN_SET, Reg);
  default:
    break;
  }
}

UINT32
AnalogixDpGetLaneLinkTraining (
  OUT struct AnalogixDpDevice *Dp,
  IN UINT8 Lane
  )
{
  return AnalogixDpRegRead(Dp,
      ANALOGIX_DP_LN0_LINK_TRAINING_CTL + 4 * Lane);
}

VOID
AnalogixDpGetLinkBandwidth (
  OUT struct AnalogixDpDevice *Dp,
  UINT32 *BwType)
{
  UINT32 Reg;

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_LINK_BW_SET);
  *BwType = Reg;
}

VOID 
AnalogixDpGetLaneCount (
  OUT struct AnalogixDpDevice *Dp,
  UINT32 *Count
  )
{
  UINT32 Reg;

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_LANE_COUNT_SET);
  *Count = Reg;
}

INT32 
AnalogixDpIsSlaveVideoStreamClockOn (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_SYS_CTL_1);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_SYS_CTL_1, Reg);

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_SYS_CTL_1);

  if (!(Reg & DET_STA))
    return -EINVAL;

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_SYS_CTL_2);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_SYS_CTL_2, Reg);

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_SYS_CTL_2);

  if (Reg & CHA_STA)
    return -EINVAL;

  return 0;
}

VOID
AnalogixDpEnableVideoMaster (
  OUT struct AnalogixDpDevice *Dp,
  BOOLEAN Enable
  )
{
  UINT32 Reg;

  if (Enable) {
    Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_SOC_GENERAL_CTL);
    Reg &= ~VIDEO_MODE_MASK;
    Reg |= VIDEO_MASTER_MODE_EN | VIDEO_MODE_MASTER_MODE;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_SOC_GENERAL_CTL, Reg);
  } else {
    Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_SOC_GENERAL_CTL);
    Reg &= ~VIDEO_MODE_MASK;
    Reg |= VIDEO_MODE_SLAVE_MODE;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_SOC_GENERAL_CTL, Reg);
  }
}

VOID
AnalogixDpStartVideo (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_VIDEO_CTL_1);
  Reg |= VIDEO_EN;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_VIDEO_CTL_1, Reg);
}

INTN
AnalogixDpIsVideoStreamOn (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_SYS_CTL_3);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_SYS_CTL_3, Reg);

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_SYS_CTL_3);
  if (!(Reg & STRM_VALID))
    return -EINVAL;

  return 0;
}

VOID
AnalogixDpEnableScrambling (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_TRAINING_PTN_SET);
  Reg &= ~SCRAMBLING_DISABLE;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_TRAINING_PTN_SET, Reg);
}

VOID
AnalogixDpDisableScrambling (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_TRAINING_PTN_SET);
  Reg |= ~SCRAMBLING_DISABLE;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_TRAINING_PTN_SET, Reg);
}

VOID 
AnalogixDpSetVideoFormat (
  OUT struct AnalogixDpDevice *Dp,
  OUT CONST DRM_DISPLAY_MODE *Mode
  )
{
  UINT32 Hsw, Hfp, Hbp, Vsw, Vfp, Vbp;

  Dp->VideoInfo.Interlaced = !!(Mode->Flags & DRM_MODE_FLAG_INTERLACE);
  Dp->VideoInfo.VSyncPolarity = TRUE;
  Dp->VideoInfo.HSyncPolarity = TRUE;

  Hsw = Mode->HSyncEnd - Mode->HSyncStart;
  Hfp = Mode->HSyncStart - Mode->HDisplay;
  Hbp = Mode->HTotal - Mode->HSyncEnd;
  Vsw = Mode->VSyncEnd - Mode->VSyncStart;
  Vfp = Mode->VSyncStart - Mode->VDisplay;
  Vbp = Mode->VTotal - Mode->VSyncEnd;

  /* Set Video Format Parameters */
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_TOTAL_LINE_CFG_L,
      TOTAL_LINE_CFG_L(Mode->VTotal));
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_TOTAL_LINE_CFG_H,
      TOTAL_LINE_CFG_H(Mode->VTotal >> 8));
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_ACTIVE_LINE_CFG_L,
      ACTIVE_LINE_CFG_L(Mode->VDisplay));
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_ACTIVE_LINE_CFG_H,
      ACTIVE_LINE_CFG_H(Mode->VDisplay >> 8));
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_V_F_PORCH_CFG,
      V_F_PORCH_CFG(Vfp));
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_V_SYNC_WIDTH_CFG,
      V_SYNC_WIDTH_CFG(Vsw));
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_V_B_PORCH_CFG,
      V_B_PORCH_CFG(Vbp));
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_TOTAL_PIXEL_CFG_L,
      TOTAL_PIXEL_CFG_L(Mode->HTotal));
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_TOTAL_PIXEL_CFG_H,
      TOTAL_PIXEL_CFG_H(Mode->HTotal >> 8));
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_ACTIVE_PIXEL_CFG_L,
      ACTIVE_PIXEL_CFG_L(Mode->HDisplay));
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_ACTIVE_PIXEL_CFG_H,
      ACTIVE_PIXEL_CFG_H(Mode->HDisplay >> 8));
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_H_F_PORCH_CFG_L,
      H_F_PORCH_CFG_L(Hfp));
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_H_F_PORCH_CFG_H,
      H_F_PORCH_CFG_H(Hfp >> 8));
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_H_SYNC_CFG_L,
      H_SYNC_CFG_L(Hsw));
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_H_SYNC_CFG_H,
      H_SYNC_CFG_H(Hsw >> 8));
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_H_B_PORCH_CFG_L,
      H_B_PORCH_CFG_L(Hbp));
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_H_B_PORCH_CFG_H,
      H_B_PORCH_CFG_H(Hbp >> 8));
}

VOID
DumpHdptxPhyRegisters (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 i, Ret;
  UINT32 Shift, BASE;

  if (!Dp->Id)
    Shift = BASE = EDP0TX_PHY_BASE;
  else
    Shift = BASE = EDP1TX_PHY_BASE;

  DEBUG ((EFI_D_WARN, "\nHDPTX_PHY_BASE\n%.8x ", BASE));
  for (i = 0; i < 168; i++) {
    Ret = MmioRead32(Shift);
    Shift = Shift + 0x04;
    DEBUG ((EFI_D_WARN, "%.8x ", Ret));
    if((i+1) % 4 == 0) {
      DEBUG ((EFI_D_WARN, "\n%.8x ", Shift));
    }
  }

  Shift = BASE+0x400;
  DEBUG ((EFI_D_WARN, "\n%.8x ", Shift));
  for (i = 0; i < 40; i++) {
    Ret = MmioRead32(Shift);
    Shift = Shift + 0x04;
    DEBUG ((EFI_D_WARN, "%.8x ", Ret));
    if( (i+1) % 4 == 0) {
      DEBUG ((EFI_D_WARN, "\n%.8x ", Shift));
    }
  }

  Shift = BASE + 0x800;
  DEBUG ((EFI_D_WARN, "\n%.8x ", Shift));
  for (i = 0; i < 40; i++) {
    Ret = MmioRead32(Shift);
    Shift = Shift + 0x04;
    DEBUG ((EFI_D_WARN, "%.8x ", Ret));
    if((i+1) % 4 == 0) {
      DEBUG ((EFI_D_WARN, "\n%.8x ", Shift));
    }
  }

  Shift = BASE + 0x0C00;
  DEBUG ((EFI_D_WARN, "\n%.8x ", Shift));
  for (i = 0; i < 40; i++) {
    Ret = MmioRead32(Shift);
    Shift = Shift + 0x04;
    DEBUG ((EFI_D_WARN, "%.8x ", Ret));
    if((i+1) % 4 == 0) {
      DEBUG ((EFI_D_WARN, "\n%.8x ", Shift));
    }
  }

  Shift = BASE + 0x1000;
  DEBUG ((EFI_D_WARN, "\n%.8x ", Shift));
  for ( i = 0; i < 40; i++) {
    Ret = MmioRead32(Shift);
    Shift = Shift + 0x04;
    DEBUG ((EFI_D_WARN, "%.8x ", Ret));
    if( (i+1) % 4 == 0) {
      DEBUG ((EFI_D_WARN, "\n%.8x ", Shift));
    }
  }

  Shift = BASE + 0x1400;
  DEBUG ((EFI_D_WARN, "\n%.8x ", Shift));
  for (i = 0; i < 40; i++) {
    Ret = MmioRead32(Shift);
    Shift = Shift + 0x04;
    DEBUG ((EFI_D_WARN, "%.8x ", Ret));
    if( (i+1) % 4 == 0) {
      DEBUG ((EFI_D_WARN, "\n%.8x ", Shift));
    }
  }

  Shift = BASE + 0x1800;
  DEBUG ((EFI_D_WARN, "\n%.8x ", Shift));
  for (i = 0; i < 40; i++) {
    Ret = MmioRead32(Shift);
    Shift = Shift + 0x04;
    DEBUG ((EFI_D_WARN, "%.8x ", Ret));
    if((i+1) % 4 == 0) {
      DEBUG ((EFI_D_WARN, "\n%.8x ", Shift));
    }
  }
}

VOID
DumpDpRegisters (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINTN i;
  UINT32 Ret;
  UINT32 Shift;

  if (!Dp->Id)
    Shift = ANALOGIX_DP0_REG_BASE;
  else
    Shift = ANALOGIX_DP1_REG_BASE;

  DEBUG ((EFI_D_WARN, "\n%.8x ", Shift));
  for (i = 0; i < 800; i++) {
    Ret = MmioRead32(Shift);
    Shift = Shift + 0x04;
    DEBUG ((EFI_D_WARN, "%.8x ", Ret));
    if( (i+1) % 4 == 0)
      DEBUG ((EFI_D_WARN, "\n%.8x ", Shift)); 
  }
}
