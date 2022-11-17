#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
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
  IN  UINT32				    Offset,
  IN  UINT32				    Value
  )
{
  UINT32 BASE = ANALOGIX_DP_REG_BASE;
  MmioWrite32(BASE + Offset,Value);	    
}

UINT32
AnalogixDpRegRead (
  IN  UINT32				    Offset
  )
{
  UINT32 Reg;
  UINT32 BASE = ANALOGIX_DP_REG_BASE;
  Reg = MmioRead32(BASE + Offset);
  return Reg;
}

VOID
AnalogixDpStopVideo(
  VOID
  )
{
  UINT32 Reg;
  Reg = AnalogixDpRegRead(ANALOGIX_DP_VIDEO_CTL_1);
  Reg &= ~VIDEO_EN;
  AnalogixDpRegWrite(ANALOGIX_DP_VIDEO_CTL_1, Reg);
}

VOID
AnalogixDpEnableVideoMute(
  BOOLEAN Enable
  )
{
  UINT32 Reg;
  Reg = AnalogixDpRegRead(ANALOGIX_DP_VIDEO_CTL_1);
  if (Enable)
  {
    Reg |= HDCP_VIDEO_MUTE;
    AnalogixDpRegWrite(ANALOGIX_DP_VIDEO_CTL_1,Reg);
  }
  else
  {
    Reg &= ~HDCP_VIDEO_MUTE;
    AnalogixDpRegWrite(ANALOGIX_DP_VIDEO_CTL_1,Reg);
  }
}

VOID
AnalogixDpLaneSwap(
  BOOLEAN Enable
  )
{
  UINT32 Reg;
  if (Enable)
	  Reg = LANE3_MAP_LOGIC_LANE_0 | LANE2_MAP_LOGIC_LANE_1 |
		LANE1_MAP_LOGIC_LANE_2 | LANE0_MAP_LOGIC_LANE_3;
  else
	  Reg = LANE3_MAP_LOGIC_LANE_3 | LANE2_MAP_LOGIC_LANE_2 |
		LANE1_MAP_LOGIC_LANE_1 | LANE0_MAP_LOGIC_LANE_0;
  AnalogixDpRegWrite(ANALOGIX_DP_LANE_MAP, Reg);
}

VOID 
AnalogixDpReset(
  VOID
  )
{
  UINT32 Reg;
  AnalogixDpStopVideo();
  AnalogixDpEnableVideoMute(0);
  Reg = MASTER_VID_FUNC_EN_N | SLAVE_VID_FUNC_EN_N |
	  AUD_FIFO_FUNC_EN_N | AUD_FUNC_EN_N |
	  HDCP_FUNC_EN_N | SW_FUNC_EN_N;
  AnalogixDpRegWrite(ANALOGIX_DP_FUNC_EN_1, Reg);
  Reg = SSC_FUNC_EN_N | AUX_FUNC_EN_N |
	  SERDES_FIFO_FUNC_EN_N |
	  LS_CLK_DOMAIN_FUNC_EN_N;
  AnalogixDpRegWrite(ANALOGIX_DP_FUNC_EN_2, Reg);
  NanoSecondDelay (30000);
  AnalogixDpLaneSwap(0);

}	

VOID
AnalogixDpSwreset (
  VOID
  )
{
  /* SW_RESET */
  AnalogixDpRegWrite(ANALOGIX_DP_TX_SW_RESET,RESET_DP_TX);
  MicroSecondDelay (10);
}

VOID
AnalogixDpInitAnalogParam(
  VOID
  )
{
/* init_analog_param */
  UINT32 Reg;
  Reg = TX_TERMINAL_CTRL_50_OHM;
  AnalogixDpRegWrite(ANALOGIX_DP_ANALOG_CTL_1, Reg);
  Reg = SEL_24M | TX_DVDD_BIT_1_0625V;
  AnalogixDpRegWrite(ANALOGIX_DP_ANALOG_CTL_2, Reg);
  Reg = REF_CLK_24M;
  AnalogixDpRegWrite(ANALOGIX_DP_PLL_REG_1, Reg);
  AnalogixDpRegWrite(ANALOGIX_DP_PLL_REG_2, 0x99);
  AnalogixDpRegWrite(ANALOGIX_DP_PLL_REG_3, 0x40);
  AnalogixDpRegWrite(ANALOGIX_DP_PLL_REG_4, 0x58);
  AnalogixDpRegWrite(ANALOGIX_DP_PLL_REG_5, 0x22);
  
  AnalogixDpRegWrite(ANALOGIX_DP_BIAS, 0x44);
  Reg = DRIVE_DVDD_BIT_1_0625V | VCO_BIT_600_MICRO;
  AnalogixDpRegWrite(ANALOGIX_DP_ANALOG_CTL_3, Reg);

  Reg = PD_RING_OSC | AUX_TERMINAL_CTRL_50_OHM |
	  TX_CUR1_2X | TX_CUR_16_MA;
  AnalogixDpRegWrite(ANALOGIX_DP_PLL_FILTER_CTL_1, Reg);
  
  Reg = CH3_AMP_400_MV | CH2_AMP_400_MV |
	  CH1_AMP_400_MV | CH0_AMP_400_MV;
  AnalogixDpRegWrite(ANALOGIX_DP_TX_AMP_TUNING_CTL, Reg);

  MicroSecondDelay (10);
}

VOID
AnalogixDpInitInterrupt(
  VOID
  )
{
  /* Set interrupt pin assertion polarity as high */
  AnalogixDpRegWrite(ANALOGIX_DP_INT_CTL,INT_POL1 | INT_POL0);
  /* Clear pending Regisers */
  AnalogixDpRegWrite(ANALOGIX_DP_COMMON_INT_STA_1, 0xff);
  AnalogixDpRegWrite(ANALOGIX_DP_COMMON_INT_STA_2, 0x4f);
  AnalogixDpRegWrite(ANALOGIX_DP_COMMON_INT_STA_3, 0xe0);
  AnalogixDpRegWrite(ANALOGIX_DP_COMMON_INT_STA_4, 0xe7);
  AnalogixDpRegWrite(ANALOGIX_DP_INT_STA_MASK, 0x63);
  /* 0:mask,1: unmask */
  AnalogixDpRegWrite(ANALOGIX_DP_COMMON_INT_MASK_1, 0x00);
  AnalogixDpRegWrite(ANALOGIX_DP_COMMON_INT_MASK_2, 0x00);
  AnalogixDpRegWrite(ANALOGIX_DP_COMMON_INT_MASK_3, 0x00);
  AnalogixDpRegWrite(ANALOGIX_DP_COMMON_INT_MASK_4, 0x00);
  AnalogixDpRegWrite(ANALOGIX_DP_INT_STA_MASK, 0x00);

  MicroSecondDelay (10);
}

VOID
AnalogixDpEnableSwFunction(
  VOID
  )
{
  UINT32 Reg;
  Reg = AnalogixDpRegRead(ANALOGIX_DP_FUNC_EN_1);
  Reg &= ~SW_FUNC_EN_N;
  AnalogixDpRegWrite(ANALOGIX_DP_FUNC_EN_1, Reg);
}

VOID
AnalogixDpConfigInterrupt(
  VOID
  )
{
  UINT32 Reg;
  /* 0: mask, 1: unmask */
  Reg = COMMON_INT_MASK_1;
  AnalogixDpRegWrite(ANALOGIX_DP_COMMON_INT_MASK_1, Reg);
  Reg = COMMON_INT_MASK_2;
  AnalogixDpRegWrite(ANALOGIX_DP_COMMON_INT_MASK_2, Reg);
  
  Reg = COMMON_INT_MASK_3;
  AnalogixDpRegWrite(ANALOGIX_DP_COMMON_INT_MASK_3, Reg);
  
  Reg = COMMON_INT_MASK_4;
  AnalogixDpRegWrite(ANALOGIX_DP_COMMON_INT_MASK_4, Reg);
  Reg = INT_STA_MASK;
  AnalogixDpRegWrite(ANALOGIX_DP_INT_STA_MASK, Reg);
}

VOID
AnalogixDpSetAnalogPowerDown(
  VOID
  )
{
  AnalogixDpRegWrite(ANALOGIX_DP_PHY_PD,0x00);
}

VOID
AnalogixDpSetPllPowerDown(
  BOOLEAN Enable
  )
{
  UINT32 Reg;
  if (Enable) {
	  Reg = AnalogixDpRegRead(ANALOGIX_DP_PLL_CTL);
	  Reg |= DP_PLL_PD;
	  AnalogixDpRegWrite(ANALOGIX_DP_PLL_CTL,Reg);
  }
  else {
	  Reg = AnalogixDpRegRead(ANALOGIX_DP_PLL_CTL);
	  Reg &= ~DP_PLL_PD;
	  AnalogixDpRegWrite(ANALOGIX_DP_PLL_CTL,Reg);
  }

}

VOID
AnalogixDpInitAnalogFunc(
  VOID
  )
{
  UINT32 Reg;
  AnalogixDpSetAnalogPowerDown();
  AnalogixDpRegWrite(ANALOGIX_DP_COMMON_INT_STA_1,PLL_LOCK_CHG);
  //power all
  AnalogixDpRegWrite(ANALOGIX_DP_PHY_PD,0x00);
  Reg = AnalogixDpRegRead(ANALOGIX_DP_PHY_PD);

  Reg = AnalogixDpRegRead(ANALOGIX_DP_DEBUG_CTL);
  Reg &= ~(F_PLL_LOCK | PLL_LOCK_CTRL);
  AnalogixDpRegWrite(ANALOGIX_DP_DEBUG_CTL,Reg);
  AnalogixDpSetPllPowerDown(0);
  Reg &=  ~(SERDES_FIFO_FUNC_EN_N | LS_CLK_DOMAIN_FUNC_EN_N
	  | AUX_FUNC_EN_N);
  AnalogixDpRegWrite(ANALOGIX_DP_FUNC_EN_2,Reg);

}

VOID
AnalogixDpClearHotplugInterrupts(
  VOID
  )
{
  UINT32 Reg;
  Reg = HOTPLUG_CHG | HPD_LOST | PLUG;
  AnalogixDpRegWrite(ANALOGIX_DP_COMMON_INT_STA_4, Reg);

  Reg = INT_HPD;
  AnalogixDpRegWrite(ANALOGIX_DP_INT_STA, Reg);
}

VOID
AnalogixDpInitHpd(
  VOID
  )
{
 UINT32 Reg;
 AnalogixDpClearHotplugInterrupts();
 Reg = AnalogixDpRegRead(ANALOGIX_DP_SYS_CTL_3);
 Reg &= ~(F_HPD | HPD_CTRL);
 AnalogixDpRegWrite(ANALOGIX_DP_SYS_CTL_3, Reg);
}

VOID
AnalogixDpResetAux(
  VOID
  )
{
 
 UINT32 Reg;
 Reg = AnalogixDpRegRead(ANALOGIX_DP_FUNC_EN_2);
 Reg |= AUX_FUNC_EN_N;
 AnalogixDpRegWrite(ANALOGIX_DP_FUNC_EN_2, Reg);
}

VOID
AnalogixDpInitAux(
  VOID
  )
{
 UINT32 Reg;
 Reg = RPLY_RECEIV | AUX_ERR;
 /* Clear inerrupts related to AUX channel */
 AnalogixDpRegWrite(ANALOGIX_DP_INT_STA, Reg);

 AnalogixDpResetAux();
 AnalogixDpRegWrite(ANALOGIX_DP_AUX_HW_RETRY_CTL,0x03);

 Reg = DEFER_CTRL_EN | DEFER_COUNT(1);
 AnalogixDpRegWrite(ANALOGIX_DP_AUX_CH_DEFER_CTL, Reg);

 Reg = AnalogixDpRegRead(ANALOGIX_DP_FUNC_EN_2);
 Reg &= ~AUX_FUNC_EN_N;
 AnalogixDpRegWrite(ANALOGIX_DP_FUNC_EN_2, Reg);
}

UINTN
AnalogixDpWriteBytesToDpcd(
  OUT struct AnalogixDpDevice *Dp,
  IN  UINTN Reg_Addr,
  IN  UINTN Count,
  IN  UINT8 Data[]
  )
{
  UINT32 Reg;
  UINTN StartOffset;
  UINTN Cur_Data_Count,Cur_Data_idx, i;
  UINTN Retval = 0;
  Reg = BUF_CLR;
  AnalogixDpRegWrite(ANALOGIX_DP_BUFFER_DATA_CTL, Reg);
  StartOffset = 0;
  while (StartOffset < Count) {
	  if ((Count - StartOffset) > 16)
		  Cur_Data_Count = 16;
	  else
		  Cur_Data_Count = Count - StartOffset;
	  /* AUX CH Request Transaction process */
	  for (i = 0; i < 3; i++) {
		  /* Select DPCD device address */
		  Reg = AUX_ADDR_7_0(Reg_Addr + StartOffset);
		  AnalogixDpRegWrite(ANALOGIX_DP_AUX_ADDR_7_0, Reg);
		  Reg = AUX_ADDR_15_8(Reg_Addr + StartOffset);
		  AnalogixDpRegWrite(ANALOGIX_DP_AUX_ADDR_15_8, Reg);
		  Reg = AUX_ADDR_19_16(Reg_Addr + StartOffset);
		  AnalogixDpRegWrite(ANALOGIX_DP_AUX_ADDR_19_16, Reg);

		  for (Cur_Data_idx = 0; Cur_Data_idx < Cur_Data_Count;
			  Cur_Data_idx++) {
			  Reg = Data[StartOffset + Cur_Data_idx];
			  AnalogixDpRegWrite(ANALOGIX_DP_BUF_DATA_0 +
					  4 * Cur_Data_idx, Reg);
			  }
		  /*
		   * Set DisplayPort transaction and write
		   * If bit 3 is 1, DisplayPort transaction.
		   * If Bit 3 is 0, I2C transaction.
		   */
		  Reg = AUX_LENGTH(Cur_Data_Count) |
			  AUX_TX_COMM_DP_TRANSACTION | AUX_TX_COMM_WRITE;
		  AnalogixDpRegWrite(ANALOGIX_DP_AUX_CH_CTL_1, Reg);
		  /* Start AUX transaction */
		  Retval = AnalogixDpStartAuxTransaction();
		  if (Retval == 0)
			  break;
	  }
	  StartOffset += Cur_Data_Count;
  }
	  return Retval;
}

UINTN
AnalogixDpReadByteFromDpcd(
  OUT  struct AnalogixDpDevice *Dp,
  IN   UINTN Reg_Addr,
  IN   UINT8 *Data
  )
{
  UINT32 Reg;
  UINTN i;
  UINTN Retval;
  for (i = 0; i < 3; i++) {
	  Reg = BUF_CLR;
	  AnalogixDpRegWrite(ANALOGIX_DP_BUFFER_DATA_CTL, Reg);
	  Reg = AUX_ADDR_7_0(Reg_Addr);
	  AnalogixDpRegWrite(ANALOGIX_DP_AUX_ADDR_7_0, Reg);
	  Reg = AUX_ADDR_15_8(Reg_Addr);
	  AnalogixDpRegWrite(ANALOGIX_DP_AUX_ADDR_15_8, Reg);
	  Reg = AUX_ADDR_19_16(Reg_Addr);
	  AnalogixDpRegWrite(ANALOGIX_DP_AUX_ADDR_19_16, Reg);
	  /*
	   * Set DisplayPort transaction and read 1 byte
	   * If bit 3 is 1, DisplayPort transaction.
	   * If Bit 3 is 0, I2C transaction.
	   */
	  Reg = AUX_TX_COMM_DP_TRANSACTION | AUX_TX_COMM_READ;
	  AnalogixDpRegWrite(ANALOGIX_DP_AUX_CH_CTL_1, Reg);
	  Retval = AnalogixDpStartAuxTransaction();
	  if (Retval == 0)
		  break;
  }
	  Reg = AnalogixDpRegRead(ANALOGIX_DP_BUF_DATA_0);
	  *Data =(UINT8)(Reg & 0xff);
	  return Retval;
}

UINTN
AnalogixDpStartAuxTransaction(
  VOID
  )
{
  UINTN Reg;
  UINTN Retval = 0;
  UINTN time_loop =0;
  Reg = AnalogixDpRegRead(ANALOGIX_DP_AUX_CH_CTL_2);
  Reg |= AUX_EN;
  AnalogixDpRegWrite(ANALOGIX_DP_AUX_CH_CTL_2,Reg);
  Reg = AnalogixDpRegRead(ANALOGIX_DP_INT_STA);
  Reg |= RPLY_RECEIV;
  AnalogixDpRegWrite(ANALOGIX_DP_INT_STA,Reg);
  Reg = AnalogixDpRegRead(ANALOGIX_DP_INT_STA);
  MicroSecondDelay (100);
  while (!(Reg & RPLY_RECEIV)){
	  time_loop++;
	  if (DP_TIMEOUT_LOOP_COUNT < time_loop) {
                  DEBUG ((EFI_D_WARN, "AUX CH command reply failed!\n"));
		  return -EFI_TIMEOUT;
	  }
	  Reg = AnalogixDpRegRead(ANALOGIX_DP_INT_STA);
  MicroSecondDelay (50);
  }
  Reg = AnalogixDpRegRead(ANALOGIX_DP_INT_STA);
  if (Reg & AUX_ERR) { 
	  DEBUG ((EFI_D_WARN,"AUX CH error happens: AUX_ERR %.08x\n", Reg ));
	  return -EREMOTEIO;
  }
  Reg = AnalogixDpRegRead(ANALOGIX_DP_AUX_CH_STA);
  MicroSecondDelay (100);	
  if ((Reg & AUX_STATUS_MASK) != 0) {
	  DEBUG ((EFI_D_WARN,"AUX CH error happens: AUX_STATUS_MASK %.08x \n", Reg));
	  return -EREMOTEIO;
  }
  return Retval;
}

UINTN
AnalogixDpReadBytesFromDpcd(
  OUT struct AnalogixDpDevice *Dp,
  IN  UINTN Reg_Addr,
  IN  UINTN Count,
  IN  UINT8 Data[]
  )
{
  UINT32 Reg;
  UINTN StartOffset;
  UINTN Cur_Data_Count,Cur_Data_idx, i;
  UINTN Retval = 0;
  Reg = BUF_CLR;
  AnalogixDpRegWrite(ANALOGIX_DP_BUFFER_DATA_CTL, Reg);
  StartOffset = 0;
  while (StartOffset < Count) {
	  if ((Count - StartOffset) > 16)
		  Cur_Data_Count = 16;
	  else
		  Cur_Data_Count = Count - StartOffset;
	  /* AUX CH Request Transaction process */
	  for (i = 0; i < 3; i++) {
		  /* Select DPCD device address */
		  Reg = AUX_ADDR_7_0(Reg_Addr + StartOffset);
		  AnalogixDpRegWrite(ANALOGIX_DP_AUX_ADDR_7_0, Reg);
		  Reg = AUX_ADDR_15_8(Reg_Addr + StartOffset);
		  AnalogixDpRegWrite(ANALOGIX_DP_AUX_ADDR_15_8, Reg);
		  Reg = AUX_ADDR_19_16(Reg_Addr + StartOffset);
		  AnalogixDpRegWrite(ANALOGIX_DP_AUX_ADDR_19_16, Reg);
		  Reg = AUX_LENGTH(Cur_Data_Count) |
			  AUX_TX_COMM_DP_TRANSACTION | AUX_TX_COMM_READ;
		  AnalogixDpRegWrite(ANALOGIX_DP_AUX_CH_CTL_1, Reg);
		  Retval = AnalogixDpStartAuxTransaction();
		  if (Retval == 0)
			  break;
	  }
	  for (Cur_Data_idx = 0; Cur_Data_idx < Cur_Data_Count;
			  Cur_Data_idx++) {
		  Reg = AnalogixDpRegRead(ANALOGIX_DP_BUF_DATA_0
				  + 4 * Cur_Data_idx);
		  Data[StartOffset + Cur_Data_idx] =
			  (CHAR8)Reg;
	  }
	  StartOffset += Cur_Data_Count;
  }
  return Retval;
}

VOID
AnalogixDpSetTrainingPattern(
  OUT struct AnalogixDpDevice *Dp,
  OUT enum PatternSet Pattern
  )
{
  UINT32 Reg;
  switch (Pattern){
	  case PRBS7:
		  Reg = SCRAMBLING_ENABLE | LINK_QUAL_PATTERN_SET_PRBS7;
		  AnalogixDpRegWrite(ANALOGIX_DP_TRAINING_PTN_SET, Reg);
		  break;
	  case D10_2:
		  Reg = SCRAMBLING_ENABLE | LINK_QUAL_PATTERN_SET_D10_2;
		  AnalogixDpRegWrite(ANALOGIX_DP_TRAINING_PTN_SET, Reg);
		  break;
	  case TRAINING_PTN1:
		  Reg = SCRAMBLING_DISABLE | SW_TRAINING_PATTERN_SET_PTN1;
		  AnalogixDpRegWrite(ANALOGIX_DP_TRAINING_PTN_SET, Reg);
		  break;
	  case TRAINING_PTN2:
		  Reg = SCRAMBLING_DISABLE | SW_TRAINING_PATTERN_SET_PTN2;
		  AnalogixDpRegWrite(ANALOGIX_DP_TRAINING_PTN_SET, Reg);
		  break;
	  case TRAINING_PTN3:
		  Reg = SCRAMBLING_DISABLE | SW_TRAINING_PATTERN_SET_PTN3;
		  AnalogixDpRegWrite(ANALOGIX_DP_TRAINING_PTN_SET, Reg);
		  break;
	  case DP_NONE:
		  Reg = SCRAMBLING_ENABLE | 
			  LINK_QUAL_PATTERN_SET_DISABLE |
			  SW_TRAINING_PATTERN_SET_NORMAL;
		  AnalogixDpRegWrite(ANALOGIX_DP_TRAINING_PTN_SET, Reg);
	  default:
		 break;
  	}
}

UINT32
AnalogixDpGetLaneLinkTraining(
  OUT struct AnalogixDpDevice *Dp,
  IN UINT8 Lane
  )
{

  return AnalogixDpRegRead(ANALOGIX_DP_LN0_LINK_TRAINING_CTL + 4 * Lane);
}

VOID 
AnalogixDpGetLinkBandwidth(
  OUT struct AnalogixDpDevice *Dp, 
  UINT32 *bwtype)
{
  UINT32 Reg;
  Reg = AnalogixDpRegRead(ANALOGIX_DP_LINK_BW_SET);
#if ARMPC
  Reg = 0x0a;
#endif
  *bwtype = Reg;
}

VOID 
AnalogixDpGetLaneCount(
  OUT struct AnalogixDpDevice *Dp,
  UINT32 *Count)
{
  UINT32 Reg;

  Reg = AnalogixDpRegRead(ANALOGIX_DP_LANE_COUNT_SET);
  *Count = Reg;
}

VOID
AnalogixDpEnableVideoMaster(
  OUT struct AnalogixDpDevice *Dp,
  BOOLEAN Enable)
{
  UINT32 Reg;
  if (Enable) {
	  Reg = AnalogixDpRegRead(ANALOGIX_DP_SOC_GENERAL_CTL);
	  Reg &= ~VIDEO_MODE_MASK;
	  Reg |= VIDEO_MASTER_MODE_EN | VIDEO_MODE_MASTER_MODE;
	  AnalogixDpRegWrite(ANALOGIX_DP_SOC_GENERAL_CTL, Reg);
  } else {
	  Reg = AnalogixDpRegRead(ANALOGIX_DP_SOC_GENERAL_CTL);
	  Reg &= ~VIDEO_MODE_MASK;
	  Reg |= VIDEO_MODE_SLAVE_MODE;
	  AnalogixDpRegWrite(ANALOGIX_DP_SOC_GENERAL_CTL, Reg);
  }
}

VOID 
AnalogixDpStartVideo(
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;
  Reg = AnalogixDpRegRead(ANALOGIX_DP_VIDEO_CTL_1);
  Reg |= VIDEO_EN;
  AnalogixDpRegWrite(ANALOGIX_DP_VIDEO_CTL_1, Reg);
}

INTN 
AnalogixDpIsVideoStreamOn(
  OUT struct AnalogixDpDevice *Dp
 )
{
  UINT32 Reg;
  Reg = AnalogixDpRegRead(ANALOGIX_DP_SYS_CTL_3);
  AnalogixDpRegWrite(ANALOGIX_DP_SYS_CTL_3, Reg);

  Reg = AnalogixDpRegRead(ANALOGIX_DP_SYS_CTL_3);
  if (!(Reg & STRM_VALID))
	  return -EINVAL;
  return 0;
}

VOID 
AnalogixDpSetVideoFormat(
  OUT struct AnalogixDpDevice *Dp
  )
{
  /* todo */
}

VOID
PrintkHDptxReg(
  VOID
  )
{
  UINT32 i , ret;
  UINT32 shift;
  shift = EDPTX_PHY_BASE;
  DEBUG ((EFI_D_WARN, "\nHDPTX_PHY_BASE\n%.8x",EDPTX_PHY_BASE));
  for (i=0;i<168;i++){
     ret = MmioRead32(shift);
     shift = shift+0x04;
     DEBUG ((EFI_D_WARN, "%.8x ",ret));
     if( (i+1) % 4 == 0){
	DEBUG ((EFI_D_WARN, "\n%.8x ",shift));
	}
  }
  shift = EDPTX_PHY_BASE+0x400;

  DEBUG ((EFI_D_WARN, "\n%.8x ",EDPTX_PHY_BASE+0x400));
  for (i=0;i<40;i++) {
     ret = MmioRead32(shift);
     shift = shift+0x04;
     DEBUG ((EFI_D_WARN, "%.8x ",ret));
     if( (i+1) % 4 == 0){
	DEBUG ((EFI_D_WARN, "\n%.8x ",shift));
	}

  }
  shift = EDPTX_PHY_BASE+0x800;

  DEBUG ((EFI_D_WARN, "\n%.8x ",EDPTX_PHY_BASE+0x800));
  for (i=0;i<40;i++){
     ret = MmioRead32(shift);
     shift = shift+0x04;
     DEBUG ((EFI_D_WARN, "%.8x ",ret));
     if( (i+1) % 4 == 0){
	DEBUG ((EFI_D_WARN, "\n%.8x ",shift));
	}
  }

  shift = EDPTX_PHY_BASE+0x0C00;
  DEBUG ((EFI_D_WARN, "\n%.8x ",EDPTX_PHY_BASE+0x0C20));
  for (i=0;i<40;i++){
     ret = MmioRead32(shift);
     shift = shift+0x04;
     DEBUG ((EFI_D_WARN, "%.8x ",ret));
     if( (i+1) % 4 == 0){
	DEBUG ((EFI_D_WARN, "\n%.8x ",shift));
	}

  }

  shift = EDPTX_PHY_BASE+0x1000;  
  DEBUG ((EFI_D_WARN, "\n%.8x ",EDPTX_PHY_BASE+0x1000));
  for (i=0;i<40;i++){
     ret = MmioRead32(shift);
     shift = shift+0x04;
     DEBUG ((EFI_D_WARN, "%.8x ",ret));
     if( (i+1) % 4 == 0){
	DEBUG ((EFI_D_WARN, "\n%.8x ",shift));
		}
  }

  shift = EDPTX_PHY_BASE+0x1400;
  DEBUG ((EFI_D_WARN, "\n%.8x ",EDPTX_PHY_BASE+0x1400));
  for (i=0;i<40;i++){
     ret = MmioRead32(shift);
     shift = shift+0x04;
     DEBUG ((EFI_D_WARN, "%.8x ",ret));
     if( (i+1) % 4 == 0){
	DEBUG ((EFI_D_WARN, "\n%.8x ",shift));
		}
  }

  shift = EDPTX_PHY_BASE+0x1800;
  DEBUG ((EFI_D_WARN, "\n%.8x ",EDPTX_PHY_BASE+0x1800));
  for (i=0;i<40;i++){
     ret = MmioRead32(shift);
     shift = shift+0x04;
     DEBUG ((EFI_D_WARN, "%.8x ",ret));
     if( (i+1) % 4 == 0){
	DEBUG ((EFI_D_WARN, "\n%.8x ",shift));
		}
  }
}

VOID
PrinteDPReg(
  VOID
)
{
  UINTN i;
  UINT32 ret;
  UINT32 shift = ANALOGIX_DP_REG_BASE;
  DEBUG ((EFI_D_WARN, "%.8x ",ANALOGIX_DP_REG_BASE));
  for (i=0;i<800;i++){
     ret = MmioRead32(shift);
     shift = shift+0x04;
     DEBUG ((EFI_D_WARN, "%.8x ",ret));
     if( (i+1) % 4 == 0){
	DEBUG ((EFI_D_WARN, "\n%.8x ",shift));
	}
  };
  DEBUG ((EFI_D_WARN, "\n FD5a8000 %.8x \n",MmioRead32(0xFD5A8000)));
  DEBUG ((EFI_D_WARN, " FD5a8034 %.8x \n",MmioRead32(0xFD5A8034)));
  DEBUG ((EFI_D_WARN, " FD5a8040 %.8x \n",MmioRead32(0xFD5A8040)));
  DEBUG ((EFI_D_WARN, " FD5a8044 %.8x \n",MmioRead32(0xFD5A8044)));
  DEBUG ((EFI_D_WARN, " FD5a8048 %.8x \n",MmioRead32(0xFD5A8048)));
  DEBUG ((EFI_D_WARN, " FD5a8050 %.8x \n",MmioRead32(0xFD5A8050)));
  DEBUG ((EFI_D_WARN, " FD5a8054 %.8x \n",MmioRead32(0xFD5A8054)));
  DEBUG ((EFI_D_WARN, " FD5a8058 %.8x \n",MmioRead32(0xFD5A8058)));
  DEBUG ((EFI_D_WARN, "VOP GRF\n"));
  DEBUG ((EFI_D_WARN, " 0xfd5a4008 %.8x \n",MmioRead32(0xfd5a4008)));
  DEBUG ((EFI_D_WARN, " 0xfd5a4010 %.8x \n",MmioRead32(0xfd5a4010)));
  DEBUG ((EFI_D_WARN, " 0xfd5a4014 %.8x \n",MmioRead32(0xfd5a4014)));
  DEBUG ((EFI_D_WARN, " 0xfd5a4018 %.8x \n",MmioRead32(0xfd5a4018)));
  DEBUG ((EFI_D_WARN, " 0xfd5a401C %.8x \n",MmioRead32(0xfd5a401C)));
  DEBUG ((EFI_D_WARN, " 0xfd5a4024 %.8x \n",MmioRead32(0xfd5a4024)));
  DEBUG ((EFI_D_WARN, " 0xfd5a4028 %.8x \n",MmioRead32(0xfd5a4028)));
  DEBUG ((EFI_D_WARN, " 0xfd5a402C %.8x \n",MmioRead32(0xfd5a402C)));
  DEBUG ((EFI_D_WARN, " 0xfd5a4030 %.8x \n",MmioRead32(0xfd5a4030)));

  DEBUG ((EFI_D_WARN, "HDPTXPHY GRF\n"));
  DEBUG ((EFI_D_WARN, "0xfd5e0000 %.8x \n",MmioRead32(0xfd5e0000)));
  DEBUG ((EFI_D_WARN, "0xfd5e0080 %.8x \n",MmioRead32(0xfd5e0080)));

}

VOID
PrintCruReg(
  VOID
)
{
  UINTN i;
  UINT32 ret;
  UINT32 shift = 0xFD7C0000;
  DEBUG ((EFI_D_WARN, "CruReg  \n"));
  for (i=0;i<1000;i++){
     ret = MmioRead32(shift);
     shift = shift+0x04;
     DEBUG ((EFI_D_WARN, "%.8x ",ret)); 
     if( (i+1) % 4 == 0){
	DEBUG ((EFI_D_WARN, "\n%.8x ",shift));
	}
  };
}

