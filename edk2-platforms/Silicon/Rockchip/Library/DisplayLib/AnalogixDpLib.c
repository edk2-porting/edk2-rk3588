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

# define DP_MAX_LINK_RATE                    0x001
# define DP_MAX_LANE_COUNT                   0x002

#define DP_LANE0_1_STATUS                   0x202
#define DP_LANE2_3_STATUS                   0x203
# define DP_LANE_CR_DONE                    (1 << 0)
# define DP_LANE_CHANNEL_EQ_DONE            (1 << 1)
# define DP_LANE_SYMBOL_LOCKED              (1 << 2)

#define DP_TRAINING_LANE0_SET               0x103
#define DP_TRAINING_LANE1_SET               0x104
#define DP_TRAINING_LANE2_SET               0x105
#define DP_TRAINING_LANE3_SET               0x106

# define DP_LINK_BW_1_62                    0x06
# define DP_LINK_BW_2_7                     0x0a
# define DP_LINK_BW_5_4                     0x14    /* 1.2 */
# define DP_LINK_BW_8_1                     0x1e    /* 1.4 */

#define DP_ADJUST_REQUEST_LANE0_1           0x206
#define DP_ADJUST_REQUEST_LANE2_3           0x207

#define DP_DOWNSPREAD_CTRL                  0x107
# define DP_SPREAD_AMP_0_5                  (1 << 4)
#define DP_MAX_DOWNSPREAD                   0x003
# define DP_MAX_DOWNSPREAD_0_5              (1 << 0)

#define DP_SET_ANSI_8B10B                  (1 << 0)

#define DP_TRAIN_VOLTAGE_SWING_MASK        0x3
# define DP_TRAIN_VOLTAGE_SWING_SHIFT       0
# define DP_TRAIN_MAX_SWING_REACHED         (1 << 2)
# define DP_TRAIN_VOLTAGE_SWING_LEVEL_0 (0 << 0)
# define DP_TRAIN_VOLTAGE_SWING_LEVEL_1 (1 << 0)
# define DP_TRAIN_VOLTAGE_SWING_LEVEL_2 (2 << 0)
# define DP_TRAIN_VOLTAGE_SWING_LEVEL_3 (3 << 0)

#define DP_TRAIN_PRE_EMPHASIS_MASK         (3 << 3)
# define DP_TRAIN_PRE_EMPH_LEVEL_0              (0 << 3)
# define DP_TRAIN_PRE_EMPH_LEVEL_1              (1 << 3)
# define DP_TRAIN_PRE_EMPH_LEVEL_2              (2 << 3)
# define DP_TRAIN_PRE_EMPH_LEVEL_3              (3 << 3)

#define DP_TRAINING_PATTERN_SET             0x102
# define DP_TRAINING_PATTERN_DISABLE        0
# define DP_TRAINING_PATTERN_1              1
# define DP_TRAINING_PATTERN_2              2
# define DP_TRAINING_PATTERN_3              3       /* 1.2 */
# define DP_TRAINING_PATTERN_4              7       /* 1.4 */
# define DP_TRAINING_PATTERN_MASK           0x3
# define DP_TRAINING_PATTERN_MASK_1_4       0xf
# define DP_TRAIN_MAX_PRE_EMPHASIS_REACHED  (1 << 5)

#define DP_LINK_SCRAMBLING_DISABLE         (1 << 5)

#define DP_LANE_COUNT_SET                   0x101
# define DP_LANE_COUNT_MASK                 0x0f
# define DP_LANE_COUNT_ENHANCED_FRAME_EN    (1 << 7)

#define DP_INTERLANE_ALIGN_DONE             (1 << 0)
#define DP_DOWNSTREAM_PORT_STATUS_CHANGED   (1 << 6)
#define DP_LINK_STATUS_UPDATED              (1 << 7)
#define DP_CHANNEL_EQ_BITS (DP_LANE_CR_DONE | DP_LANE_CHANNEL_EQ_DONE | DP_LANE_SYMBOL_LOCKED)
#define DP_LANE_ALIGN_STATUS_UPDATED        0x204

VOID
AnalogixDpEnableScramble (
  VOID
  )
{
  UINT32 Reg;
  //power down
  MmioWrite32(0xFDEC012C,0x00);

  Reg = AnalogixDpRegRead(ANALOGIX_DP_TRAINING_PTN_SET);
  Reg &= ~SCRAMBLING_DISABLE;
  AnalogixDpRegWrite(ANALOGIX_DP_TRAINING_PTN_SET,Reg);//Enable_scrambling
}

BOOLEAN AnalogixDpGetPllLockStatus (
  VOID
  )
{
  UINT32 Reg;
  UINTN i;
  for (i=0;i<20;i++)
  {
	Reg = AnalogixDpRegRead(ANALOGIX_DP_DEBUG_CTL);
  	MicroSecondDelay (20);
  }
  if (Reg & PLL_LOCK)
	  return PLL_LOCKED;
  else 
	  return PLL_UNLOCKED;
}

VOID AnalogixDpInitDp(
  VOID
  )
{
  EnablePwm();
  AnalogixDpReset();
  AnalogixDpSwreset();
  AnalogixDpInitAnalogParam();
  AnalogixDpInitInterrupt();
  /* SW defined function Normal operation */
  AnalogixDpEnableSwFunction();
  AnalogixDpConfigInterrupt();
  AnalogixDpInitAnalogFunc();
  AnalogixDpInitHpd();
  AnalogixDpInitAux();

}

UINTN
AnalogixDpWriteByteToDpcd(
  OUT struct AnalogixDpDevice *Dp,
  IN UINTN Reg_Addr,
  IN UINT8 Data
  ){
  /* buffer have Data
   * AUX CH buffer
   * */
  UINT32 Reg;
  UINT32 i;
  UINTN Retval;
	  for (i = 0; i < 3; i++) {
  		  Reg = BUF_CLR;
  		  AnalogixDpRegWrite(ANALOGIX_DP_BUFFER_DATA_CTL, Reg);
		  Reg = AUX_ADDR_7_0(Reg_Addr);
		  AnalogixDpRegWrite(ANALOGIX_DP_AUX_ADDR_7_0, Reg);
		  Reg = AUX_ADDR_15_8(Reg_Addr);
		  AnalogixDpRegWrite(ANALOGIX_DP_AUX_ADDR_15_8, Reg);
		  Reg = AUX_ADDR_19_16(Reg_Addr);
		  AnalogixDpRegWrite(ANALOGIX_DP_AUX_ADDR_19_16,Reg);
		  
		  /* write Data buffer */
		  Reg = (UINTN)Data;
		  AnalogixDpRegWrite(ANALOGIX_DP_BUF_DATA_0,Reg);
		  #if ARMPC
		  AnalogixDpRegWrite(ANALOGIX_DP_BUF_DATA_0,0x02);
		  #else
		  AnalogixDpRegWrite(ANALOGIX_DP_BUF_DATA_0,0x84);
		  #endif
		  Reg = AUX_TX_COMM_DP_TRANSACTION | AUX_TX_COMM_WRITE;
		  AnalogixDpRegWrite(ANALOGIX_DP_AUX_CH_CTL_1, Reg);

		  Retval = AnalogixDpStartAuxTransaction();
		  if (Retval == 0 ){
			  break;
		  }
	  }
  return Retval;
}

VOID EnablePwm(
  VOID
  )
{
#if RK3588S_EVB1
  //rk3588s-evb1 gpio1_a5
  MmioWrite32(0xFD5F8024,0xFFFF0000);
  MmioWrite32(0xFEC20000,0xFFFF0220);
  MmioWrite32(0xFEC20004,0xFFFF0000);
  MmioWrite32(0xFEC20008,0xFFFF0222);
  MmioWrite32(0xFEC2000C,0xFFFF0000);
  //Register set pwm12
  MmioWrite32(0xFD5F808C,0xFFFF00B0);
  MmioWrite32(0xFEBF0000,0x00000000);
  MmioWrite32(0xFEBF0004,0x00000258);
  MmioWrite32(0xFEBF0008,0x000001d4);
  MmioWrite32(0xFEBF000C,0x0000000b);
#elif ARMPC
  //ARMPC gpio1_c4
  MmioWrite32(0xFD5F8024,0xFFFF2505);
  MmioWrite32(0xFEC20000,0xFFFF0000);
  MmioWrite32(0xFEC20004,0xFFFF4010);
  MmioWrite32(0xFEC20008,0xFFFF2400);
  MmioWrite32(0xFEC2000C,0xFFFF4010);
  //ARMPC gpio4 b5
  MmioWrite32(0xFEC50000,0xFFFF2180);
  MmioWrite32(0xFEC50004,0xFFFF0018);
  MmioWrite32(0xFEC50008,0xFFFF21c0);
  MmioWrite32(0xFEC5000C,0xFFFF000c);
  DEBUG ((EFI_D_WARN, " armpc pwm  Enable end \n"));
#else
  /* Enable lcd_pwren gpio*/
  MmioWrite32(0xFEC20004,0xFFFF0010);
  MmioWrite32(0xFEC2000C,0xFFFF0010);
  /* Enable pwm15 */
  //PWMEnable(0xFEBF0030);
  DEBUG ((EFI_D_WARN, " pwm  Enable end \n"));
#endif
}

VOID
AnalogixDpEnableRxToEnhancedMode (
  OUT struct AnalogixDpDevice *Dp,
  BOOLEAN Enable
  )
{
  UINT8 Data;
  AnalogixDpReadByteFromDpcd(Dp,DP_LANE_COUNT_SET,&Data);//0x102
  if (Enable)
	  AnalogixDpWriteByteToDpcd(Dp,DP_LANE_COUNT_SET,DP_LINK_SCRAMBLING_DISABLE |
			  DP_TRAINING_PATTERN_1);//0x102
  else
	  AnalogixDpWriteByteToDpcd(Dp, DP_LANE_COUNT_SET,
			  DPCD_LANE_COUNT_SET(Data));
}

VOID
AnalogixDpEnableEnhancedMode (
  OUT struct AnalogixDpDevice *Dp,
  BOOLEAN Enable
 )
{
  UINT32 Reg;
  if (Enable) {
	  Reg = AnalogixDpRegRead(ANALOGIX_DP_SYS_CTL_4);
	  Reg |= ENHANCED;
	  AnalogixDpRegWrite(ANALOGIX_DP_SYS_CTL_4,Reg);
	}
  else {
	  Reg = AnalogixDpRegRead(ANALOGIX_DP_SYS_CTL_4);
	  Reg &= ~ENHANCED;
	  AnalogixDpRegWrite(ANALOGIX_DP_SYS_CTL_4,Reg);
  }
}

VOID
AnalogixDpInitVideo (
  VOID
  )
{
  UINT32 Reg;
  Reg = VSYNC_DET | VID_FORMAT_CHG | VID_CLK_CHG;
  AnalogixDpRegWrite(ANALOGIX_DP_COMMON_INT_STA_1,Reg);
  AnalogixDpRegWrite(ANALOGIX_DP_SYS_CTL_1,0x07);
  AnalogixDpRegWrite(ANALOGIX_DP_SYS_CTL_2,0x41);
  Reg = VID_HRES_TH(2) | VID_VRES_TH(0);
  AnalogixDpRegWrite(ANALOGIX_DP_SYS_CTL_3,Reg);
  AnalogixDpRegWrite(ANALOGIX_DP_VIDEO_CTL_8,0x20);
}

VOID
AnalogixDpResetMacro(
  VOID
  )
{
  AnalogixDpRegWrite(ANALOGIX_DP_PHY_TEST,0x20);
  NanoSecondDelay (20000);//at least 10us
  AnalogixDpRegWrite(ANALOGIX_DP_PHY_TEST,0x00);
}

STATIC VOID
AnalogixDpGetMaxRxBandwidth(
  OUT struct AnalogixDpDevice *Dp, 
  IN  UINT8  *Bandwidth
  )
{
  UINT8 Data;
  /*
   * For DP rev.1.1, Maximum link rate of Main Link Lanes
   * 0x06 = 1.62 Gbps, 0x0a = 2.7 Gbps
   * For DP rev.1.2, Maximum link rate of Main Link Lanes
   * 0x06 = 1.62 Gbps, 0x0a = 2.7 Gbps, 0x14 = 5.4Gbps
   */
  AnalogixDpReadByteFromDpcd(Dp, DP_MAX_LINK_RATE, &Data);
  *Bandwidth = Data;
}

STATIC VOID
AnalogixDpGetMaxRxLaneCount(
  OUT struct AnalogixDpDevice *Dp, 
  IN UINT8 *LaneCount
  )
{
  UINT8 Data;
  /*
   * For DP rev.1.1, Maximum number of Main Link Lanes
   * 0x01 = 1 Lane, 0x02 = 2 Lanes, 0x04 = 4 Lanes
   */
  AnalogixDpReadByteFromDpcd(Dp, DP_MAX_LANE_COUNT, &Data);
  *LaneCount = DPCD_MAX_LANE_COUNT(Data);
}

UINTN
AnalogixDpInitTraining(
  OUT struct AnalogixDpDevice *Dp,
  enum LinkLaneCountType MaxLane,
  INTN max_rate
  )
{
  OUT struct VideoInfo *Video = &Dp->VideoInfo;
  AnalogixDpResetMacro();
  Dp->LinkTrain.Link_Rate = DP_LINK_BW_1_62;
  Video->ColorDepth = COLOR_8;
  Dp->LinkTrain.LaneCount = 4;
  AnalogixDpGetMaxRxBandwidth(Dp,&Dp->LinkTrain.Link_Rate);
  AnalogixDpGetMaxRxLaneCount(Dp, &Dp->LinkTrain.LaneCount);
  Dp->LinkTrain.Link_Rate = DP_LINK_BW_5_4;
  
  if ((Dp->LinkTrain.Link_Rate != DP_LINK_BW_1_62) &&
      (Dp->LinkTrain.Link_Rate != DP_LINK_BW_2_7) &&
      (Dp->LinkTrain.Link_Rate != DP_LINK_BW_5_4) )
  {
	DEBUG ((EFI_D_WARN, "faiied to get Rx Max Link Rate  0x%.08x \n",Dp->LinkTrain.Link_Rate));
  }
  if (Dp->LinkTrain.LaneCount == 0) {
	DEBUG ((EFI_D_WARN, "faiied to get Rx Max Lane count\n"));
  }
  UINT8 Dpcd;
  /* Setup TX Lane count & rate */
  if (Dp->LinkTrain.LaneCount > MaxLane)
	  Dp->LinkTrain.LaneCount = MaxLane;
  if (Dp->LinkTrain.Link_Rate > max_rate)
	  Dp->LinkTrain.Link_Rate = max_rate;

  AnalogixDpReadByteFromDpcd(Dp, DP_MAX_DOWNSPREAD, &Dpcd);
  Dp->LinkTrain.SSC = !!(Dpcd & DP_MAX_DOWNSPREAD_0_5);
  AnalogixDpSetAnalogPowerDown();
  return 0;
}

UINTN
AnalogixDpSetLinkBandwidth(
  UINT32 bwType
  )
{
  UINTN i;
  UINT32 Reg;
  AnalogixDpRegWrite(ANALOGIX_DP_LINK_BW_SET,bwType);
  Reg = AnalogixDpGetPllLockStatus();
  for (i=0;i<20;i++){
	  if (Reg != PLL_LOCKED)
  	      MicroSecondDelay (10);
	  else
	      break;
  }
  return 0;
}

UINTN
AnalogixDpSetLaneCount(
  UINT32 count
  )
{
  UINTN Reg ;
  Reg = count;
  #if ARMPC
  AnalogixDpRegWrite(ANALOGIX_DP_LANE_COUNT_SET,0x02);
  #else
  AnalogixDpRegWrite(ANALOGIX_DP_LANE_COUNT_SET,Reg);
  #endif
  return 0 ;
}

UINTN
AnalogixDpLinkStart(
  OUT struct AnalogixDpDevice *Dp
  )
{
 UINT8 buf[4];
 UINTN Lane, LaneCount, Retval;
 LaneCount = Dp->LinkTrain.LaneCount;
 Dp->LinkTrain.LtState = CLOCK_RECOVERY;
 Dp->LinkTrain.Eq_Loop = 0;
 for (Lane = 0; Lane < LaneCount; Lane++)
	 Dp->LinkTrain.Cr_Loop[Lane] = 0;
 //Set link rate and count as you want to establish
 AnalogixDpSetLinkBandwidth(Dp->LinkTrain.Link_Rate);
 AnalogixDpSetLaneCount(Dp->LinkTrain.LaneCount);
 // Setup RX configuration
 buf[0] = Dp->LinkTrain.Link_Rate;
 buf[1] = Dp->LinkTrain.LaneCount;
 Retval = AnalogixDpWriteBytesToDpcd(Dp,0x100, 2, buf); 
 if (Retval)
	 return Retval;
 /* Spread AMP if required, Enable 8b/10b coding */
 buf[0] = DP_SPREAD_AMP_0_5;
 buf[1] = DP_SET_ANSI_8B10B;
 Retval = AnalogixDpWriteBytesToDpcd(Dp, DP_DOWNSPREAD_CTRL,
		 2,buf);
 if (Retval<0)
	 return Retval;
 /* Set TX voltage-swing and pre-emphasis to minimum */
 for (Lane = 0; Lane < LaneCount; Lane++)
	 Dp->LinkTrain.TrainingLane[Lane] =
		 DP_TRAIN_VOLTAGE_SWING_LEVEL_0 |
		 DP_TRAIN_PRE_EMPH_LEVEL_0;
 RockchipHdpPhySetRate();
 AnalogixDpSetTrainingPattern(Dp,TRAINING_PTN1);
 /* Set RX training pattern */
 Retval = AnalogixDpWriteByteToDpcd(Dp,
		 DP_TRAINING_PATTERN_SET,
		 DP_LINK_SCRAMBLING_DISABLE | DP_TRAINING_PATTERN_1);
 if (Retval)
	 return Retval;
 for (Lane = 0; Lane < LaneCount; Lane++)
	 buf[Lane] = DP_TRAIN_PRE_EMPH_LEVEL_0 |
		 DP_TRAIN_VOLTAGE_SWING_LEVEL_0;
 Retval = AnalogixDpWriteBytesToDpcd(Dp, DP_TRAINING_LANE0_SET,
		 LaneCount, buf);
 return Retval;
}

STATIC UINT8
AnalogixDpGetAdjustRequestVoltage(
  UINT8 adjust_request[2], UINTN Lane)
{
  UINTN shift = (Lane & 1) * 4;
  UINT8 link_value = adjust_request[Lane >> 1];
  return (link_value >> shift) & 0x3;
}

STATIC UINT8
AnalogixDpGetAdjustRequestPreEmphasis(
  UINT8 adjust_request[2], UINTN Lane)
{
  UINTN shift = (Lane & 1) * 4;
  UINT8 link_value = adjust_request[Lane >> 1];
  return ((link_value >> shift) & 0xc) >> 2;
}

STATIC VOID
AnalogixDpTrainingPatternDis(
  OUT struct AnalogixDpDevice *Dp
  )
{
  AnalogixDpSetTrainingPattern(Dp, DP_NONE);
  AnalogixDpWriteByteToDpcd(Dp, DP_TRAINING_PATTERN_SET,
			DP_TRAINING_PATTERN_DISABLE);
}

STATIC UINTN
AnalogixDpIsEnhancedModeAvailable(
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT8 Data;
  UINTN Retval;
  AnalogixDpReadByteFromDpcd(Dp, DP_MAX_LANE_COUNT, &Data);
  Retval = DPCD_ENHANCED_FRAME_CAP(Data);

  return Retval;
}

STATIC VOID
AnalogixDpSetEnhancedMode(
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT8 Data;
  Data = AnalogixDpIsEnhancedModeAvailable(Dp);
  AnalogixDpEnableRxToEnhancedMode(Dp, Data);
  AnalogixDpEnableEnhancedMode(Dp,Data);
}

STATIC VOID
AnalogixDpReduceLinkRate(
  OUT struct AnalogixDpDevice *Dp
  )
{
  AnalogixDpTrainingPatternDis(Dp);
  AnalogixDpSetEnhancedMode(Dp);
  Dp->LinkTrain.LtState = FAILED;
}

STATIC VOID
AnalogixDpGetAdjustTrainingLane(
  OUT struct AnalogixDpDevice *Dp,
  UINT8 adjust_request[2])
{
  UINTN Lane , LaneCount;
  UINT8 voltage_swing, pre_emphasis, TrainingLane;
  LaneCount = Dp->LinkTrain.LaneCount;
  for (Lane = 0; Lane < LaneCount; Lane++) {
	  voltage_swing = AnalogixDpGetAdjustRequestVoltage(
			  adjust_request, Lane
			  );
	  pre_emphasis = AnalogixDpGetAdjustRequestPreEmphasis(
			  adjust_request, Lane
			  );
	  TrainingLane = DPCD_VOLTAGE_SWING_SET(voltage_swing) |
		  DPCD_PRE_EMPHASIS_SET(pre_emphasis);
	  if (voltage_swing == VOLTAGE_LEVEL_3)
		  TrainingLane |= DP_TRAIN_MAX_SWING_REACHED;
	  if (pre_emphasis == PRE_EMPHASIS_LEVEL_3)
		  TrainingLane |= DP_TRAIN_MAX_PRE_EMPHASIS_REACHED;

	  Dp->LinkTrain.TrainingLane[Lane] = TrainingLane;
  }
}

VOID
AnalogixDpSetLaneLinkTraining(
  VOID
)
{
  //RockchipHdpPhySetRate();
}

STATIC UINT8
AnalogixDpGetLaneStatus(
  UINT8 link_status[2],
  UINTN Lane
  )
{
  UINTN shift = (Lane & 1) * 4;
  UINT8 link_value = link_status[Lane >> 1];
  return (link_value >> shift) & 0xf;
}

STATIC UINTN
AnalogixDpClockRecoveryOk(
  UINT8 link_status[2],
  UINTN LaneCount
  )
{
  UINTN Lane;
  UINT8 Lane_status;
  for (Lane = 0; Lane < LaneCount; Lane++) {
	  Lane_status = AnalogixDpGetLaneStatus(link_status, Lane);
	  if ((Lane_status & DP_LANE_CR_DONE) == 0)
		  return -1;
  }
  return 0;
}

STATIC UINTN
AnalogixDpProcessClockRecovery(
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINTN Lane, LaneCount, Retval;
  UINT8 voltage_swing, pre_emphasis, TrainingLane;
  UINT8 link_status[2], adjust_request[2];
  UINT8 Training_Pattern = TRAINING_PTN2;
  NanoSecondDelay (100000);

  LaneCount = Dp->LinkTrain.LaneCount;
  Retval = AnalogixDpReadBytesFromDpcd(Dp,
		  DP_LANE0_1_STATUS, 2, link_status);
  if (Retval)
	  return Retval;
  Retval =  AnalogixDpReadBytesFromDpcd(Dp,
		  DP_ADJUST_REQUEST_LANE0_1, 2, adjust_request);
  if (Retval)
	  return Retval;
  if (AnalogixDpClockRecoveryOk(link_status, LaneCount) == 0) {
	  AnalogixDpSetTrainingPattern(Dp, Training_Pattern);
	  Retval = AnalogixDpWriteByteToDpcd(Dp,
			  DP_TRAINING_PATTERN_SET,
			  (Training_Pattern == TRAINING_PTN3 ?
			   DP_TRAINING_PATTERN_3 : DP_TRAINING_PATTERN_2));
		  if (Retval)
			  return Retval;
		  DEBUG ((EFI_D_WARN, "Link Training Clock Recovery success\n"));
		  Dp->LinkTrain.LtState = EQUALIZER_TRAINING;
  }
  else{
	  for (Lane = 0; Lane < LaneCount; Lane++) {
		  TrainingLane = AnalogixDpGetLaneLinkTraining(
				  Dp, Lane);
		  voltage_swing = AnalogixDpGetAdjustRequestVoltage(
				  adjust_request, Lane);
		  pre_emphasis = AnalogixDpGetAdjustRequestPreEmphasis(
				  adjust_request, Lane);
		  if (DPCD_VOLTAGE_SWING_GET(TrainingLane) ==
				  voltage_swing &&
				  DPCD_PRE_EMPHASIS_GET(TrainingLane) ==
				  pre_emphasis){
			  Dp->LinkTrain.Cr_Loop[Lane]++;
		  }
		  if (Dp->LinkTrain.Cr_Loop[Lane] == MAX_CR_LOOP ||
				  voltage_swing == VOLTAGE_LEVEL_3 ||
				  pre_emphasis == PRE_EMPHASIS_LEVEL_3) {
		          DEBUG ((EFI_D_WARN, "CR Max reached (%d,%d)\n",
						  Dp->LinkTrain.Cr_Loop[Lane],
						  voltage_swing, pre_emphasis));
			  AnalogixDpReduceLinkRate(Dp);
			  return -1;
		  }
	  }
  }
  AnalogixDpGetAdjustTrainingLane(Dp, adjust_request);
  AnalogixDpSetLaneLinkTraining();
  Retval = AnalogixDpWriteBytesToDpcd(Dp,
		  DP_TRAINING_LANE0_SET, LaneCount,
		  Dp->LinkTrain.TrainingLane);
  if (Retval)
	  return Retval;
  return Retval;

}

STATIC UINTN
AnalogixDpChannelEqOk(
  UINT8 link_status[2],
  UINT8 link_align,
  UINTN LaneCount
  )
{
  UINTN Lane;
  UINT8 Lane_status;
  if ((link_align & DP_INTERLANE_ALIGN_DONE) == 0)
	  return -1;
  for (Lane = 0; Lane < LaneCount; Lane++) {
	  Lane_status = AnalogixDpGetLaneStatus(link_status, Lane);
	  Lane_status &= DP_CHANNEL_EQ_BITS;
  	  if (Lane_status != DP_CHANNEL_EQ_BITS)
		   return -1;
  }
  return 0;
}

STATIC UINTN
AnalogixDpProcessEqualizerTraining(
  OUT struct AnalogixDpDevice *Dp)
{
  UINTN LaneCount, Retval;
  UINT32 Reg;
  UINT8 link_align;
  UINT8 link_status[2], adjust_request[2];
  NanoSecondDelay (401000);
  LaneCount = Dp->LinkTrain.LaneCount;
  Retval = AnalogixDpReadBytesFromDpcd(Dp,
		  DP_LANE0_1_STATUS, 2, link_status);
  if (Retval)
	  return Retval;
  if (AnalogixDpClockRecoveryOk(link_status, LaneCount)) {
	  AnalogixDpReduceLinkRate(Dp);
	  return -1;
  }
  Retval = AnalogixDpReadBytesFromDpcd(Dp,
		  DP_ADJUST_REQUEST_LANE0_1, 2, adjust_request);
  if (Retval)
	  return Retval;
  Retval = AnalogixDpReadByteFromDpcd(Dp,
		  DP_LANE_ALIGN_STATUS_UPDATED, &link_align);
  if (Retval)
	  return Retval;
  AnalogixDpGetAdjustTrainingLane(Dp, adjust_request);
  if (!AnalogixDpChannelEqOk(link_status, link_align, LaneCount)) {
	AnalogixDpTrainingPatternDis(Dp);
        DEBUG ((EFI_D_WARN, "Link Training success\n"));
	AnalogixDpGetLinkBandwidth(Dp, &Reg);
	Dp->LinkTrain.Link_Rate = Reg;
	AnalogixDpGetLaneCount(Dp, &Reg);
        DEBUG ((EFI_D_WARN, "final link rate = 0x%.2x, Lane count = 0x%.2x\n",
				Dp->LinkTrain.Link_Rate, Dp->LinkTrain.LaneCount));
	AnalogixDpSetEnhancedMode(Dp);
	Dp->LinkTrain.LtState = FINISHED;

	return 0;
  }
  /* not all locked */
  Dp->LinkTrain.Eq_Loop++;
  if (Dp->LinkTrain.Eq_Loop > MAX_EQ_LOOP) {
	  AnalogixDpReduceLinkRate(Dp);
	  return -1;
  }
  AnalogixDpSetLaneLinkTraining();
  Retval = AnalogixDpWriteBytesToDpcd(Dp, DP_TRAINING_LANE0_SET,
		  LaneCount, Dp->LinkTrain.TrainingLane);

  return 0;
}

STATIC UINTN
AnalogixDpSwLinkTraining(
  OUT struct AnalogixDpDevice *Dp
  )
{
  int Retval = 0, training_finished = 0;
  Dp->LinkTrain.LtState = START;

  while (!Retval && !training_finished) {
	  switch (Dp->LinkTrain.LtState) {
		  case START:
			  Retval = AnalogixDpLinkStart(Dp);
			  if (Retval)
		              DEBUG ((EFI_D_WARN, "LT link start failed!\n"));
			  break;
		  case CLOCK_RECOVERY:
			  Retval = AnalogixDpProcessClockRecovery(Dp);
			  if (Retval)
		              DEBUG ((EFI_D_WARN, "LT CR failed!\n"));
		  case EQUALIZER_TRAINING:
			  Retval = AnalogixDpProcessEqualizerTraining(Dp);
			  if (Retval)
		              DEBUG ((EFI_D_WARN, "LT EQ failed!\n"));
		  case FINISHED:
			  training_finished = 1;
			  break;
		  case FAILED:
			  return -1;
	  }
  }
	  return Retval;
}

UINTN
AnalogixDpSetLinkTrain(
  OUT struct AnalogixDpDevice *Dp,
  UINT32 count,
  UINT32 bwType
  )
{
  int i, ret;
  DEBUG ((DEBUG_WARN, "[DRIVER]enter %a\n", __func__));
  for (i = 0; i < 5; i++) { 
	  ret = AnalogixDpInitTraining(Dp,count, bwType);
	  if (ret < 0) { 
		DEBUG ((EFI_D_WARN, "faiied to init training\n")); 
		return ret;
	  }
	  ret = AnalogixDpSwLinkTraining(Dp);
	  if (!ret) 
		  break;
  }
  return ret;

}

VOID
AnalogixDpConfigVideoSlaveMode(
  VOID
  )
{

  MmioWrite32(0xFDEC0018,0x00);
  MmioWrite32(0xFDEC06E0,0x10);
  MmioWrite32(0xFDEC0600,0x04);
  MmioWrite32(0xFDEC0604,0x41);
  MmioWrite32(0xFDEC060C,0x08);
  MmioWrite32(0xFDEC0024,0x10);
  MmioWrite32(0xFDEC0044,0x08);
  MmioWrite32(0xFDEC0028,0x00);
  MmioWrite32(0xFDEC0800,0x00);
  MmioWrite32(0xFDEC0020,0x80);
  MmioWrite32(0xFDEC0608,0x48);
  MmioWrite32(0xFDEC0600,0x04);

  AnalogixDpRegWrite(ANALOGIX_DP_FUNC_EN_1,0x9c);
  AnalogixDpRegWrite(ANALOGIX_DP_SOC_GENERAL_CTL,0x0101);
}

VOID
AnalogixDpSetVideoColorFormat(
  VOID
  )
{
  DEBUG ((DEBUG_WARN, "[DRIVER]enter %a\n", __func__));
  AnalogixDpRegWrite(ANALOGIX_DP_VIDEO_CTL_2,0x10);
  AnalogixDpRegWrite(ANALOGIX_DP_VIDEO_CTL_3,0x80);

}

VOID
AnalogixDpSetVideoCrMn(
  struct AnalogixDpDevice *Dp,
  enum ClockRecoveryMValueType Type,
  UINT32 m_value,
  UINT32 n_value
  )
{
  UINT32 Reg;
  DEBUG ((DEBUG_WARN, "[DRIVER]enter %a\n", __func__));
  if (Type == REGISTER_M) {
	  Reg = AnalogixDpRegRead(ANALOGIX_DP_SYS_CTL_4);
	  Reg |= FIX_M_VID;
	  AnalogixDpRegWrite(ANALOGIX_DP_SYS_CTL_4, Reg);
	  Reg = m_value & 0xff;
	  AnalogixDpRegWrite(ANALOGIX_DP_M_VID_0, Reg);
	  Reg = (m_value >> 8) & 0xff;
	  AnalogixDpRegWrite(ANALOGIX_DP_M_VID_1, Reg);
	  Reg = (m_value >> 16) & 0xff;
	  AnalogixDpRegWrite(ANALOGIX_DP_M_VID_2, Reg);

	  Reg = n_value & 0xff;
	  AnalogixDpRegWrite(ANALOGIX_DP_N_VID_0, Reg);
	  Reg = (n_value >> 8) & 0xff;
	  AnalogixDpRegWrite(ANALOGIX_DP_N_VID_1, Reg);
	  Reg = (n_value >> 16) & 0xff;
	  AnalogixDpRegWrite(ANALOGIX_DP_N_VID_2, Reg);
  } else {
  	Reg = AnalogixDpRegRead(ANALOGIX_DP_SYS_CTL_4);
  	Reg &= ~FIX_M_VID;
  	AnalogixDpRegWrite(ANALOGIX_DP_SYS_CTL_4,Reg);
  	AnalogixDpRegWrite(ANALOGIX_DP_N_VID_0,0x00);
  	AnalogixDpRegWrite(ANALOGIX_DP_N_VID_1,0x80);
  	AnalogixDpRegWrite(ANALOGIX_DP_N_VID_2,0x00);
  }
}

VOID
AnalogixDpSetVideoTimingMode(
  VOID
  )
{
  UINT32 Reg ;
  DEBUG ((DEBUG_WARN, "[DRIVER]enter %a\n", __func__));
  Reg = AnalogixDpRegRead(ANALOGIX_DP_VIDEO_CTL_10);
  Reg &= ~FORMAT_SEL;
  AnalogixDpRegWrite(ANALOGIX_DP_VIDEO_CTL_10,Reg);
}

UINTN
AnalogixDpConfigVideo(
  struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg ,ret;
  DEBUG ((DEBUG_WARN, "[DRIVER]enter %a\n", __func__));
  AnalogixDpConfigVideoSlaveMode();
  AnalogixDpSetVideoColorFormat();
  Reg = AnalogixDpGetPllLockStatus();
  if (Reg != PLL_LOCKED ) {
  DEBUG ((EFI_D_WARN, "\n  -czz- PLL is not locked yet. \n"));
  ret = AnalogixDpRegRead(ANALOGIX_DP_DEBUG_CTL);
  DEBUG ((EFI_D_WARN, "  -czz- PLL Dp debug ctl ret 0x%.8x\n",ret));
    }
  /* AnalogixDp is slave Video stream clock on */
  AnalogixDpRegWrite(ANALOGIX_DP_SYS_CTL_1,0x04);
  Reg = AnalogixDpRegRead(ANALOGIX_DP_SYS_CTL_1);
  MicroSecondDelay (200);
  AnalogixDpRegWrite(ANALOGIX_DP_SYS_CTL_1,0x04);
  MicroSecondDelay (200);
  UINTN i;
  if (!(Reg & DET_STA)){
	  for(i=0;i<10;i++){
  		MicroSecondDelay (200);
  		AnalogixDpRegWrite(ANALOGIX_DP_SYS_CTL_1,0x04);
  		Reg = AnalogixDpRegRead(ANALOGIX_DP_SYS_CTL_1);

	  }
	  DEBUG ((EFI_D_WARN, "  not lock  %.08x \n",Reg));
  }
  AnalogixDpRegWrite(ANALOGIX_DP_SYS_CTL_2,0x41);

  AnalogixDpSetVideoCrMn(Dp, CALCULATED_M, 0, 0);
  AnalogixDpSetVideoTimingMode();
  AnalogixDpEnableVideoMute(0);
  AnalogixDpEnableVideoMaster(Dp,0);
  AnalogixDpStartVideo(Dp);
  INTN TimeoutLoop, DoneCount= 0; 
  for (;;) {
	  TimeoutLoop++;
	  if (AnalogixDpIsVideoStreamOn(Dp) == 0) { 
		DoneCount++;
		if (DoneCount > 10)
			break;
	  } else if (DoneCount){
		  DoneCount = 0;
	  }
	  if (TimeoutLoop > DP_TIMEOUT_LOOP_COUNT){
		 DEBUG ((DEBUG_WARN, "Timeout of Video streamclk ok\n")); 
		 return -EFI_TIMEOUT;
	  }
  MicroSecondDelay (100);

  }
  
#if ARMPC  /* todo */
  AnalogixDpRegWrite(0x90,0x65);
  AnalogixDpRegWrite(0x94,0x04);
  AnalogixDpRegWrite(0x98,0x38);
  AnalogixDpRegWrite(0x9C,0x04);
  AnalogixDpRegWrite(0xA0,0x03);
  AnalogixDpRegWrite(0xA4,0x05);
  AnalogixDpRegWrite(0xA8,0x25);
#endif
  return 0;
}

INT32
AnalogixDpConnectorPreInit (
  OUT DISPLAY_STATE                        *DisplayState
  )
{
  CONNECTOR_STATE *ConnectorState = &DisplayState->ConnectorState;
  ConnectorState->Type = DRM_MODE_CONNECTOR_eDP;
  return 0;
};

INT32
AnalogixDpConnectorInit (
  OUT DISPLAY_STATE                        *DisplayState
  )
{
  CONNECTOR_STATE *ConnectorState = &DisplayState->ConnectorState;

  ConnectorState->OutputInterface = VOP_OUTPUT_IF_eDP0;
  ConnectorState->OutputMode = ROCKCHIP_OUT_MODE_AAAA;
  ConnectorState->ColorSpace = V4L2_COLORSPACE_DEFAULT;

  MmioWrite32(0xFD5A4008,0xFFFF0001); //eDp mode
  MmioWrite32(0xFD5A4010,0xFFFF1410);
  MmioWrite32(0xFD5A4014,0xFFFF8010);
  MmioWrite32(0xFD5A4018,0xFFFF0010);
  MmioWrite32(0xFD5A401C,0xFFFF8010);
  MmioWrite32(0xFD5A4024,0xFFFF0008);
  MmioWrite32(0xFD5A4028,0xFFFF0010);
  MmioWrite32(0xFD5A402C,0xFFFF0011);
  MmioWrite32(0xFD5A4030,0xFF00FF00);
  MmioWrite32(0xFD5A8000,0xFFFF0001);
  MmioWrite32(0xFD5A8034,0xF0902040);
  MmioWrite32(0xFD5A8040,0xFFF40000);
  MmioWrite32(0xFD5A8044,0xF5000000);
  MmioWrite32(0xFD5A8048,0xF4000000);
  MmioWrite32(0xFD5A8050,0xFFFF1410);
  MmioWrite32(0xFD5A8054,0xFFFF0010);
  MmioWrite32(0xFD5A8058,0xFFFF0011);
  /* phy start up */
  MmioWrite32(0xFD7F0A0C,0xFFFF1000);
  MmioWrite32(0xFD7F0A10,0xFFFF0003);
  MmioWrite32(EDPTX_PHY_BASE+0x27C,0xC0);
  MmioWrite32(EDPTX_PHY_BASE+0x27C,0xCC);
  MmioWrite32(EDPTX_PHY_BASE+0x280,0x30);
  MmioWrite32(EDPTX_PHY_BASE+0x27C,0x03);
  MmioWrite64(HDPTXPHY0_GRF_BASE,0xFFFF0060);
  MmioWrite32(0xFD7F0A0C,0xFFFF0000);
  MmioWrite64(HDPTXPHY0_GRF_BASE,0xFFFF00E0);
  MmioWrite32(0xFD7F0A10,0xFFFF0002);
  MmioWrite32(EDPTX_PHY_BASE+0x27C,0x00);
  MmioWrite32(EDPTX_PHY_BASE+0x280,0x00);
  MicroSecondDelay (10);
  RockchipHdptxPhyInit();
  AnalogixDpInitDp();
  RockchipHdptxPhySetVoltage();
  return 0;
};

INT32
AnalogixDpConnectorGetEdid (
  OUT DISPLAY_STATE                        *DisplayState
  )
{

  return 0;
};

INT32
AnalogixDpConnectorEnable (
  OUT DISPLAY_STATE                        *DisplayState,
  OUT struct AnalogixDpDevice              *Dp
  )
{
  CONNECTOR_STATE *ConnectorState = &DisplayState->ConnectorState;
  CRTC_STATE *CrtcState = &DisplayState->CrtcState;
  struct VideoInfo *Video = &Dp->VideoInfo;
  Dp = AllocatePool(sizeof (*Dp));
  Video = AllocatePool(sizeof (*Video));
  CrtcState = AllocatePool(sizeof(*CrtcState));
  UINTN ret;
  switch (ConnectorState->BPC)
  {
  case 12:
	  Video->ColorDepth = COLOR_12;
	  break;
  case 10:
	  Video->ColorDepth = COLOR_10;
	  break;
  case 6:
	  Video->ColorDepth = COLOR_6;
	  break;
  case 8:
	  Video->ColorDepth = COLOR_8;
	  break;
  default:
	  Video->ColorDepth = COLOR_8;
	  break;
  }
  RockchipHdpPhySetRate();
#if ARMPC
  ret = AnalogixDpSetLinkTrain(Dp,LANE_COUNT4,
		  DP_LINK_BW_2_7);
  Video->MaxLinkRate = DP_LINK_BW_2_7; 

#else
  ret = AnalogixDpSetLinkTrain(Dp,LANE_COUNT4,
		  DP_LINK_BW_1_62);
  Video->MaxLinkRate = DP_LINK_BW_1_62; 
#endif
  if (ret){
	  DEBUG ((DEBUG_WARN, "-czz- unable to do link train \n", __func__));
	  return ret;
  }

  Video->MaxLaneCount = 4;
  AnalogixDpEnableScramble();
  AnalogixDpEnableRxToEnhancedMode(Dp,1);
  AnalogixDpEnableEnhancedMode(Dp,1);
  AnalogixDpInitVideo();
  AnalogixDpSetVideoFormat(Dp);
  AnalogixDpConfigVideo(Dp);
  #if REG_DUMP
  PrintCruReg();
  PrinteDPReg();
  PrintkHDptxReg();
  #endif
  return 0;
};

INT32
AnalogixDpConnectorDisable (
  OUT DISPLAY_STATE                        *DisplayState
  )
{

  return 0;
};

INT32
AnalogixDpConnectorDetect (
  OUT DISPLAY_STATE                        *DisplayState
  )
{

  return 0;
};

