/** @file
*
*  Copyright (c) 2017, Rockchip Inc. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/ArmLib.h>
#include <Library/BaseLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>

#include <Library/I2CLib.h>

/* i2c timerout */
#define I2C_TIMEOUT_US		100000  // 100000us = 100ms
#define I2C_RETRY_COUNT		3

/* i2c error return code */
#define I2C_OK			0
#define I2C_ERROR_TIMEOUT	-1
#define I2C_ERROR_NOACK		-2
#define I2C_ERROR_IO		-3

/* rk i2c fifo max transfer bytes */
#define RK_I2C_FIFO_SIZE	32

/* rk i2c device register size */
#define RK_I2C_REGISTER_SIZE	3

#define RK_CEIL(x, y) \
	({ unsigned long __x = (x), __y = (y); (__x + __y - 1) / __y; })

#define I2C_ADAP_SEL_BIT(nr)	((nr) + 11)
#define I2C_ADAP_SEL_MASK(nr)	((nr) + 27)

/* Control register */
#define I2C_CON			0x000
#define I2C_CON_EN		(1 << 0)
#define I2C_CON_MOD(mod)	((mod) << 1)
#define I2C_MODE_TX		0x00
#define I2C_MODE_TRX		0x01
#define I2C_MODE_RX		0x02
#define I2C_MODE_RRX		0x03
#define I2C_CON_MASK		(3 << 1)

#define I2C_CON_START		(1 << 3)
#define I2C_CON_STOP		(1 << 4)
#define I2C_CON_LASTACK		(1 << 5)
#define I2C_CON_ACTACK		(1 << 6)

/* Clock dividor register */
#define I2C_CLKDIV		0x004
#define I2C_CLKDIV_VAL(divl, divh)	(((divl) & 0xffff) | (((divh) << 16) & 0xffff0000))

/* the slave address accessed  for master rx mode */
#define I2C_MRXADDR		0x008
#define I2C_MRXADDR_SET(vld, addr)	(((vld) << 24) | (addr))

/* the slave register address accessed  for master rx mode */
#define I2C_MRXRADDR		0x00c
#define I2C_MRXRADDR_SET(vld, raddr)	(((vld) << 24) | (raddr))

/* master tx count */
#define I2C_MTXCNT		0x010

/* master rx count */
#define I2C_MRXCNT		0x014

/* interrupt enable register */
#define I2C_IEN			0x018
#define I2C_BTFIEN		(1 << 0)
#define I2C_BRFIEN		(1 << 1)
#define I2C_MBTFIEN		(1 << 2)
#define I2C_MBRFIEN		(1 << 3)
#define I2C_STARTIEN		(1 << 4)
#define I2C_STOPIEN		(1 << 5)
#define I2C_NAKRCVIEN		(1 << 6)

/* interrupt pending register */
#define I2C_IPD                 0x01c
#define I2C_BTFIPD              (1 << 0)
#define I2C_BRFIPD              (1 << 1)
#define I2C_MBTFIPD             (1 << 2)
#define I2C_MBRFIPD             (1 << 3)
#define I2C_STARTIPD            (1 << 4)
#define I2C_STOPIPD             (1 << 5)
#define I2C_NAKRCVIPD           (1 << 6)
#define I2C_IPD_ALL_CLEAN       0x7f

/* finished count */
#define I2C_FCNT                0x020

/* I2C tx data register */
#define I2C_TXDATA_BASE         0X100

/* I2C rx data register */
#define I2C_RXDATA_BASE         0x200

/* register io */
#define I2CRegReadl(addr)			MmioRead32(addr)
#define I2CRegWritel(val, addr)		MmioWrite32(addr, val)

/* Get pll rate by id */
#define FRAC_MODE	0
#define MHZ				(1000*1000)
static UINT32
RkClkPllGetRate (
  enum RkPllsId pll_id
  )
{
  UINT32 Con, PllCon0, PllCon1, PllCon2, Dsmp;

  if (pll_id == PPLL_ID) {
    Con = PmuCruReadl(PMUCRU_PLL_CON(0, 3)) & PLL_MODE_MSK;
    PllCon0 = PmuCruReadl(PMUCRU_PLL_CON(0, 0));
    PllCon1 = PmuCruReadl(PMUCRU_PLL_CON(0, 1));
    PllCon2 = PmuCruReadl(PMUCRU_PLL_CON(0, 2));
    Dsmp = PLL_GET_DSMPD(PmuCruReadl(PMUCRU_PLL_CON(0, 3)));
  } else {
    Con = CruReadl(CRU_PLL_CON(pll_id, 3)) & PLL_MODE_MSK;
    PllCon0 = CruReadl(CRU_PLL_CON(pll_id, 0));
    PllCon1 = CruReadl(CRU_PLL_CON(pll_id, 1));
    PllCon2 = CruReadl(CRU_PLL_CON(pll_id, 2));
    Dsmp = PLL_GET_DSMPD(CruReadl(CRU_PLL_CON(pll_id, 3)));
  }

  if (Con == PLL_MODE_SLOW) { /* slow mode */
    return 24 * MHZ;
  } else if (Con == PLL_MODE_NORM) { /* normal mode */
    UINT32 Rate = 0, FracRate64 = 0;

    /* integer mode */
    Rate = (UINT32)(24) * PLL_GET_FBDIV(PllCon0);
    Rate = Rate / PLL_GET_REFDIV(PllCon1);
    if (FRAC_MODE == Dsmp) {
    /* fractional mode */
      FracRate64 = (UINT32)(24) * PLL_GET_FRAC(PllCon2);
      FracRate64 = FracRate64 / PLL_GET_REFDIV(PllCon1);
      Rate += FracRate64 >> 24;
    }
    Rate = Rate / PLL_GET_POSTDIV1(PllCon1);
    Rate = Rate / PLL_GET_POSTDIV2(PllCon1);
    return Rate * MHZ;
  } else { /* deep slow mode */
    return 32768;
  }
}

static UINT32
RkClkGetI2CClk (
  UINT32 BusId
  )
{
  UINT32 Con = 0;
  UINT32 Div = 1;
  UINT32 PmuPll;

  PmuPll = RkClkPllGetRate(PPLL_ID);
  if (BusId == 0) {
    Con = PmuCruReadl(PMUCRU_CLKSELS_CON(2));
    Div = ((Con >> 0) & 0x7F) + 1;
  } else if (BusId == 4) {
    Con = PmuCruReadl(PMUCRU_CLKSELS_CON(3));
    Div = ((Con >> 0) & 0x7F) + 1;
  } else if (BusId == 8) {
    Con = PmuCruReadl(PMUCRU_CLKSELS_CON(2));
    Div = ((Con >> 8) & 0x7F) + 1;
  }

  return (UINT32)(PmuPll / Div);
}

static void
RkI2CGetDiv (
  UINT32 Div,
  UINT32 *Divh,
  UINT32 *Divl
  )
{
  if (Div % 2 == 0) {
    *Divh = Div / 2;
    *Divl = Div / 2;
  } else {
    *Divh = RK_CEIL(Div, 2);
    *Divl = Div / 2;
  }
}

EFI_STATUS
RkI2CSetClk (
  UINT32 BusId,
  UINT32 SclRate
  )
{
  UINT32 I2CRate;
  UINT32 Div, Divl, Divh;
  struct RkI2CInfo *I2CInfo = (struct RkI2CInfo *)RkI2CGetBase(BusId);
	
  if (I2CInfo->Speed == SclRate) {
    return EFI_SUCCESS;
  }
  /* First get i2c rate from pclk */
  I2CRate = RkClkGetI2CClk(BusId);

  Div = RK_CEIL(I2CRate, SclRate * 8) - 2;
  if (Div < 0) {
    Divh = Divl = 0;
  } else {
    RkI2CGetDiv(Div, &Divh, &Divl);
  }
  I2CRegWritel(I2C_CLKDIV_VAL(Divl, Divh), I2CInfo->Regs + I2C_CLKDIV);

  I2CInfo->Speed = SclRate;

  DEBUG ((EFI_D_VERBOSE, "RkI2CSetClk: i2c rate = %d, scl rate = %d\n", I2CRate, SclRate));
  DEBUG ((EFI_D_VERBOSE, "set i2c clk div = %d, divh = %d, divl = %d\n", Div, Divh, Divl));
  DEBUG ((EFI_D_VERBOSE, "set clk(I2C_CLKDIV: 0x%08x)\n", I2CRegReadl(I2CInfo->Regs + I2C_CLKDIV)));

  return EFI_SUCCESS;
}

EFI_STATUS
RkI2CIomux (
  enum RkI2CBusID BusId
  )
{
  if (BusId == I2C_CH0) {
    MmioWrite32(RK3399_PMU_GRF_BASE + PMU_GRF_GPIO1B_IOMUX, (3 << 30) | (2 << 14));
    MmioWrite32(RK3399_PMU_GRF_BASE + PMU_GRF_GPIO1C_IOMUX, (3 << 16) | (2 << 0));
  } else {
	  DEBUG ((EFI_D_ERROR, "I2C iomux error, PLS check i2c config!\n"));	  
	  return EFI_LOAD_ERROR;
  }
  
  return EFI_SUCCESS;
}

EFI_STATUS
RkI2CInit (
  UINT32 BusId,
  UINT32 Speed
  )
{
  EFI_STATUS	Status = EFI_SUCCESS;

  Status = RkI2CIomux(BusId);
  if(EFI_ERROR (Status)){
    goto EXIT;
  }
  
  Status = RkI2CSetClk(BusId, Speed);
  if(EFI_ERROR (Status)){
    goto EXIT;
  }

EXIT:
  return Status;
}

EFI_STATUS
RkI2CSendStartBit (
  struct RkI2CInfo *I2CInfo
  )
{
  UINT32 TimeOut = I2C_TIMEOUT_US;

  DEBUG ((EFI_D_VERBOSE, "I2c Send Start bit.\n"));
  I2CRegWritel(I2C_IPD_ALL_CLEAN, I2CInfo->Regs + I2C_IPD);

  I2CRegWritel(I2C_CON_EN | I2C_CON_START, I2CInfo->Regs + I2C_CON);
  I2CRegWritel(I2C_STARTIEN, I2CInfo->Regs + I2C_IEN);

  while (TimeOut--) {
    if (I2CRegReadl(I2CInfo->Regs + I2C_IPD) & I2C_STARTIPD) {
      I2CRegWritel(I2C_STARTIPD, I2CInfo->Regs + I2C_IPD);
      break;
    }
    MicroSecondDelay(1);
  }
  if (TimeOut <= 0) {
    DEBUG ((EFI_D_ERROR, "I2C Send Start Bit Timeout\n"));
    return EFI_LOAD_ERROR;
  }

return EFI_SUCCESS;
}

EFI_STATUS
RkI2CSendStopBit (
  struct RkI2CInfo *I2CInfo
  )
{
  int TimeOut = I2C_TIMEOUT_US;

  DEBUG ((EFI_D_VERBOSE, "I2c Send Stop bit.\n"));
  I2CRegWritel(I2C_IPD_ALL_CLEAN, I2CInfo->Regs + I2C_IPD);

  I2CRegWritel(I2C_CON_EN | I2C_CON_STOP, I2CInfo->Regs + I2C_CON);
  I2CRegWritel(I2C_CON_STOP, I2CInfo->Regs + I2C_IEN);

  while (TimeOut--) {
    if (I2CRegReadl(I2CInfo->Regs + I2C_IPD) & I2C_STOPIPD) {
      I2CRegWritel(I2C_STOPIPD, I2CInfo->Regs + I2C_IPD);
      break;
    }
    MicroSecondDelay(1);
  }

  if (TimeOut <= 0) {
    DEBUG ((EFI_D_ERROR, "I2C Send Stop Bit Timeout\n"));
    return EFI_LOAD_ERROR;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
RkI2CDisable (
  struct RkI2CInfo *i2c
  )
{
  I2CRegWritel(0, i2c->Regs + I2C_CON);
  
  return EFI_SUCCESS;
}

EFI_STATUS
RkI2CWrite (
  struct RkI2CInfo *I2CInfo,
  UINT8 Chip,
  UINT32 Reg,
  UINT32 RLen,
  UINT8 *Buf,
  UINT32 BLen
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  int TimeOut = I2C_TIMEOUT_US;
  UINT8 *PBuf = Buf;
  UINT32 BytesRemainLen = BLen + RLen + 1;
  UINT32 BytesTranferedLen = 0;
  UINT32 WordsTranferedLen = 0;
  UINT32 TxData;
  UINT32 i, j;

  DEBUG ((EFI_D_VERBOSE, "RkI2CWrite: chip = %d, reg = %d, r_len = %d, b_len = %d\n", Chip, Reg, RLen, BLen));

  Status = RkI2CSendStartBit(I2CInfo);
  if(EFI_ERROR (Status)){
    goto EXIT;
  }

  while (BytesRemainLen) {
    if (BytesRemainLen > RK_I2C_FIFO_SIZE) {
      BytesTranferedLen = 32;
    } else {
      BytesTranferedLen = BytesRemainLen;
    }
    WordsTranferedLen = RK_CEIL(BytesTranferedLen, 4);

    for (i = 0; i < WordsTranferedLen; i++) {
      TxData = 0;
      for (j = 0; j < 4; j++) {
        if ((i * 4 + j) == BytesTranferedLen) {
          break;
        }

        if (i == 0 && j == 0) {
          TxData |= (Chip << 1);
        } else if (i == 0 && j <= RLen) {
          TxData |= (Reg & (0xff << ((j - 1) * 8))) << 8;
        } else {
          TxData |= (*PBuf++)<<(j * 8);
        }
        I2CRegWritel(TxData, I2CInfo->Regs + I2C_TXDATA_BASE + i * 4);
      }
      DEBUG ((EFI_D_VERBOSE, "I2c Write TXDATA[%d] = 0x%x\n", i, TxData));
    }

    I2CRegWritel(I2C_CON_EN | I2C_CON_MOD(I2C_MODE_TX), I2CInfo->Regs + I2C_CON);
    I2CRegWritel(BytesTranferedLen, I2CInfo->Regs + I2C_MTXCNT);
    I2CRegWritel(I2C_MBTFIEN | I2C_NAKRCVIEN, I2CInfo->Regs + I2C_IEN);

    TimeOut = I2C_TIMEOUT_US;
    while (TimeOut--) {
      if (I2CRegReadl(I2CInfo->Regs + I2C_IPD) & I2C_NAKRCVIPD) {
        I2CRegWritel(I2C_NAKRCVIPD, I2CInfo->Regs + I2C_IPD);
      }
      if (I2CRegReadl(I2CInfo->Regs + I2C_IPD) & I2C_MBTFIPD) {
        I2CRegWritel(I2C_MBTFIPD, I2CInfo->Regs + I2C_IPD);
        break;
      }
      MicroSecondDelay(1);
    }

    if (TimeOut <= 0) {
      DEBUG ((EFI_D_ERROR, "I2C Write Data Timeout\n"));
      Status = EFI_LOAD_ERROR;
      goto EXIT;

    }

    BytesRemainLen -= BytesTranferedLen;
    DEBUG ((EFI_D_VERBOSE, "I2C Write bytes_remain_len %d\n", BytesRemainLen));
  }

EXIT:
  // Send stop bit
  RkI2CSendStopBit(I2CInfo);
  // Disable Controller
  RkI2CDisable(I2CInfo);

  return Status;
}

EFI_STATUS
RkI2CRead (
  struct RkI2CInfo *I2CInfo,
  UINT8 Chip,
  UINT32 Reg,
  UINT32 RLen,
  UINT8 *Buf,
  UINT32 BLen
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  int TimeOut = I2C_TIMEOUT_US;
  UINT8 *PBuf = Buf;
  UINT32 BytesRemainLen = BLen;
  UINT32 BytesTranferedLen = 0;
  UINT32 WordsTranferedLen = 0;
  UINT32 Con = 0;
  UINT32 RxData;
  UINT32 i, j;

  DEBUG ((EFI_D_VERBOSE, "RkI2CRead: Chip = %d, Reg = %d, RLen = %d, BLen = %d\n", Chip, Reg, RLen, BLen));
  Status = RkI2CSendStartBit(I2CInfo);
  if(EFI_ERROR (Status)){
    goto EXIT;
  }

  I2CRegWritel(I2C_MRXADDR_SET(1, Chip << 1 | 1), I2CInfo->Regs + I2C_MRXADDR);
  if (RLen == 0) {
    I2CRegWritel(0, I2CInfo->Regs + I2C_MRXRADDR);
  } else if (RLen < 4) {
    I2CRegWritel(I2C_MRXRADDR_SET(RLen, Reg), I2CInfo->Regs + I2C_MRXRADDR);
  } else {
    DEBUG ((EFI_D_ERROR, "I2C Read: addr len %d not supported\n", RLen));
    Status = EFI_LOAD_ERROR;
    goto EXIT;
  }
  
  while (BytesRemainLen) {
    if (BytesRemainLen > RK_I2C_FIFO_SIZE) {
      Con = I2C_CON_EN | I2C_CON_MOD(I2C_MODE_TRX);
      BytesTranferedLen = 32;
    } else {
      Con = I2C_CON_EN | I2C_CON_MOD(I2C_MODE_TRX) | I2C_CON_LASTACK;
      BytesTranferedLen = BytesRemainLen;
    }
    WordsTranferedLen = RK_CEIL(BytesTranferedLen, 4);

    I2CRegWritel(Con, I2CInfo->Regs + I2C_CON);
    I2CRegWritel(BytesTranferedLen, I2CInfo->Regs + I2C_MRXCNT);
    I2CRegWritel(I2C_MBRFIEN | I2C_NAKRCVIEN, I2CInfo->Regs + I2C_IEN);

    TimeOut = I2C_TIMEOUT_US;
    while (TimeOut--) {
      if (I2CRegReadl(I2CInfo->Regs + I2C_IPD) & I2C_NAKRCVIPD) {
        I2CRegWritel(I2C_NAKRCVIPD, I2CInfo->Regs + I2C_IPD);
      }
      if (I2CRegReadl(I2CInfo->Regs + I2C_IPD) & I2C_MBRFIPD) {
        I2CRegWritel(I2C_MBRFIPD, I2CInfo->Regs + I2C_IPD);
        break;
      }
      MicroSecondDelay(1);
    }

    if (TimeOut <= 0) {
      DEBUG ((EFI_D_ERROR, "I2C Read Data Timeout\n"));
      Status = EFI_LOAD_ERROR;
      goto EXIT;
    }

    for (i = 0; i < WordsTranferedLen; i++) {
      RxData = I2CRegReadl(I2CInfo->Regs + I2C_RXDATA_BASE + i * 4);
      DEBUG ((EFI_D_VERBOSE, "I2c Read RXDATA[%d] = 0x%x\n", i, RxData));
      for (j = 0; j < 4; j++) {
        if ((i * 4 + j) == BytesTranferedLen) {
          break;
        }
        *PBuf++ = (RxData >> (j * 8)) & 0xff;
      }
    }
    BytesRemainLen -= BytesTranferedLen;
    DEBUG ((EFI_D_VERBOSE, "I2C Read bytes_remain_len %d\n", BytesRemainLen));
  }

EXIT:
  // Send stop bit
  RkI2CSendStopBit(I2CInfo);
  // Disable Controller
  RkI2CDisable(I2CInfo);

  return Status;
}

EFI_STATUS
EFIAPI
I2CWrite (
  UINT32 BusId,
  UINT8 Chip,
  UINT32 Addr,
  UINT32 Alen,
  UINT8 *Buf,
  UINT32 Len
  )
{
  EFI_STATUS		  Status = EFI_SUCCESS;
  struct RkI2CInfo *I2CInfo = (struct RkI2CInfo *)RkI2CGetBase(BusId);

  if (I2CInfo == NULL) {
    return -1;
  }
  if ((Buf == NULL) && (Len != 0)) {
    DEBUG ((EFI_D_ERROR, "I2CWrite: buf == NULL\n"));
    return -2;
  }

  Status = RkI2CWrite(I2CInfo, Chip, Addr, Alen, Buf, Len);
  if(EFI_ERROR (Status)){
    goto EXIT;
  }

EXIT:
  return Status;
}

EFI_STATUS
EFIAPI
I2CRead (
  UINT32 BusId,
  UINT8 Chip,
  UINT32 Addr,
  UINT32 Alen,
  UINT8 *Buf,
  UINT32 Len)
{
  EFI_STATUS		Status = EFI_SUCCESS;
  struct RkI2CInfo *I2CInfo = (struct RkI2CInfo *)RkI2CGetBase(BusId);

  if (I2CInfo == NULL) {
    return EFI_LOAD_ERROR;
  }
  if ((Buf == NULL) && (Len != 0)) {
    DEBUG ((EFI_D_ERROR, "I2CRead: buf == NULL\n"));
    return EFI_LOAD_ERROR;
  }

  Status = RkI2CRead(I2CInfo, Chip, Addr, Alen, Buf, Len);
  if(EFI_ERROR (Status)){
    goto EXIT;
  }

EXIT:
  return Status;
}

EFI_STATUS
EFIAPI
I2CInit(
  UINT32 BusID,
  UINT32 speed
)
{
  EFI_STATUS	  Status = EFI_SUCCESS;

  DEBUG ((EFI_D_VERBOSE, "I2CInit\n"));
    
  Status = RkI2CInit(BusID, speed);
  if(EFI_ERROR (Status)){
    goto EXIT;
  }

  Status = RkI2cLibRuntimeSetup(BusID);
  if(EFI_ERROR (Status)){
    goto EXIT;
  }

EXIT:
  return Status;
}

