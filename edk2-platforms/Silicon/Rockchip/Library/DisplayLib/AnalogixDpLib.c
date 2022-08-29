/** @file
  Rockchip eDP Driver.

  Copyright (c) 2022 Rockchip Electronics Co. Ltd.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AnalogixDpLib.h>
#include <Library/PWMLib.h>
#include <Library/DrmModes.h>
#include <Library/RockchipPlatformLib.h>
#include <Library/MediaBusFormat.h>
#include <Library/DrmModes.h>

#include <string.h>

#include <Protocol/RockchipConnectorProtocol.h>

#include <Uefi/UefiBaseType.h>

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

#define DP_LINK_BW_SET                      0x100
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

#define DP_SET_ANSI_8B10B                   (1 << 0)

#define DP_TRAIN_VOLTAGE_SWING_MASK        0x3
# define DP_TRAIN_VOLTAGE_SWING_SHIFT       0
# define DP_TRAIN_MAX_SWING_REACHED         (1 << 2)
# define DP_TRAIN_VOLTAGE_SWING_LEVEL_0     (0 << 0)
# define DP_TRAIN_VOLTAGE_SWING_LEVEL_1     (1 << 0)
# define DP_TRAIN_VOLTAGE_SWING_LEVEL_2     (2 << 0)
# define DP_TRAIN_VOLTAGE_SWING_LEVEL_3     (3 << 0)

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

#define EIO 5  /* I/O error */ 
#define ENODEV    19  /* No such device */

/* DRM-DP-HELPER */
STATIC
VOID
DrmDpLinkTrainClockRecoveryDelay (
    CONST UINT8 Dpcd[DP_RECEIVER_CAP_SIZE]
    )
{
  INT32 RdInterval = Dpcd[DP_TRAINING_AUX_RD_INTERVAL] & 
    DP_TRAINING_AUX_RD_MASK;

  if (RdInterval > 4)
    DEBUG ((EFI_D_WARN, "AUX interval %d, out of range (max 4)\n", RdInterval));

  if (RdInterval == 0 || Dpcd[DP_DPCD_REV] >= DP_DPCD_REV_14)
    NanoSecondDelay(100000);
  else
    MicroSecondDelay (RdInterval * 4);
}

STATIC
VOID
DrmDpLinkTrainChannelEqDelay (
  CONST UINT8 Dpcd[DP_RECEIVER_CAP_SIZE]
  )
{
  INT32 RdInterval = Dpcd[DP_TRAINING_AUX_RD_INTERVAL] & 
    DP_TRAINING_AUX_RD_MASK;

  if (RdInterval > 4)
    DEBUG ((EFI_D_WARN, "AUX interval %d, out of range (max 4)\n", RdInterval));

  if (RdInterval == 0)
    NanoSecondDelay(400000);
  else
    MicroSecondDelay (RdInterval * 4);
}

STATIC
INT32
DrmDpBwCodeToLinkRate (
  UINT8 LinkBw
  )
{
  switch(LinkBw) {
  default:
    DEBUG ((EFI_D_WARN, "Unknown DP link BW code %x, using 162000\n", LinkBw));
  case DP_LINK_BW_1_62:
    return 162000;
  case DP_LINK_BW_2_7:
    return 270000;
  case DP_LINK_BW_5_4:
    return 540000;
  case DP_LINK_BW_8_1:
    return 810000;
  }
}

VOID
AnalogixDpEnableScramble (
  OUT struct AnalogixDpDevice *Dp,
  BOOLEAN Enable
  )
{
  UINT8 Data;

  if (Enable) {
    AnalogixDpEnableScrambling(Dp);

    AnalogixDpReadByteFromDpcd(Dp, DP_TRAINING_PATTERN_SET,
        &Data);
    AnalogixDpWriteByteToDpcd(Dp,
        DP_TRAINING_PATTERN_SET,
        (UINT8)(Data & ~DP_LINK_SCRAMBLING_DISABLE));
  } else {
    AnalogixDpDisableScrambling(Dp);

    AnalogixDpReadByteFromDpcd(Dp, DP_TRAINING_PATTERN_SET,
        &Data);
    AnalogixDpWriteByteToDpcd(Dp,
        DP_TRAINING_PATTERN_SET,
        (UINT8)(Data | DP_LINK_SCRAMBLING_DISABLE));
  }
}

BOOLEAN
AnalogixDpGetPllLockStatus (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;
  UINTN i;

  for(i = 0; i < 20; i++) {
    Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_DEBUG_CTL);
    MicroSecondDelay (20);
	}

  if (Reg & PLL_LOCK) {
    return PLL_LOCKED;
  } else {
    DEBUG ((DEBUG_WARN, "Pll unlocked \n"));
    return PLL_UNLOCKED;
	}
}

VOID
AnalogixDpEnableRxToEnhancedMode (
  OUT struct AnalogixDpDevice *Dp,
  BOOLEAN Enable
  )
{
  UINT8 Data;

  AnalogixDpReadByteFromDpcd(Dp, DP_LANE_COUNT_SET, &Data);

  if (Enable)
    AnalogixDpWriteByteToDpcd(Dp, DP_LANE_COUNT_SET,
        DP_LANE_COUNT_ENHANCED_FRAME_EN | DPCD_LANE_COUNT_SET(Data));
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
    Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_SYS_CTL_4);
    Reg |= ENHANCED;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_SYS_CTL_4,Reg);
  } else {
    Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_SYS_CTL_4);
    Reg &= ~ENHANCED;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_SYS_CTL_4,Reg);
  }
}

VOID
AnalogixDpInitVideo (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;

  Reg = VSYNC_DET | VID_FORMAT_CHG | VID_CLK_CHG;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_COMMON_INT_STA_1, Reg);

  Reg = 0x0;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_SYS_CTL_1, Reg);

  Reg = CHA_CRI(4) | CHA_CTRL;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_SYS_CTL_2, Reg);

  Reg = 0x0;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_SYS_CTL_3, Reg);

  Reg = VID_HRES_TH(2) | VID_VRES_TH(0);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_VIDEO_CTL_8, Reg);
}

VOID
AnalogixDpResetMacro (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_PHY_TEST);
  Reg |= MACRO_RST;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_PHY_TEST, Reg);

  /* 10 us is the minimum reset time. */
  NanoSecondDelay (20000);

  Reg &= ~MACRO_RST;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_PHY_TEST, Reg);
}

STATIC
VOID
AnalogixDpGetMaxRxBandwidth (
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

STATIC
VOID
AnalogixDpGetMaxRxLaneCount (
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
AnalogixDpInitTraining (
  OUT struct AnalogixDpDevice *Dp,
  enum LinkLaneCountType MaxLane,
  INT32 MaxRate
  )
{
  UINT8 Dpcd;

  /*
   * MACRO_RST must be applied after the PLL_LOCCK to avoid
   * the DP inter pair skew issue for at least 10us
   * */
  AnalogixDpResetMacro(Dp);

  /* Initailze by reading RX's DPCD */
  AnalogixDpGetMaxRxBandwidth(Dp, &Dp->LinkTrain.LinkRate);
  AnalogixDpGetMaxRxLaneCount(Dp, &Dp->LinkTrain.LaneCount);

  if ((Dp->LinkTrain.LinkRate != DP_LINK_BW_1_62) &&
      (Dp->LinkTrain.LinkRate != DP_LINK_BW_2_7) &&
      (Dp->LinkTrain.LinkRate != DP_LINK_BW_5_4) ) {
    DEBUG ((EFI_D_WARN, "faiied to get Rx Max Link Rate  0x%2x \n",
          Dp->LinkTrain.LinkRate));
    return -ENODEV;
  }

  if (Dp->LinkTrain.LaneCount == 0) {
    DEBUG ((EFI_D_WARN, "faiied to get Rx Max Lane count\n"));
    return -ENODEV;
  }

  /* Setup TX Lane count & rate */
  if (Dp->LinkTrain.LaneCount > MaxLane)
    Dp->LinkTrain.LaneCount = MaxLane;
  if (Dp->LinkTrain.LinkRate > MaxRate)
    Dp->LinkTrain.LinkRate = MaxRate;

  AnalogixDpReadByteFromDpcd(Dp, DP_MAX_DOWNSPREAD, &Dpcd);
  Dp->LinkTrain.SSC = !!(Dpcd & DP_MAX_DOWNSPREAD_0_5);

  /* All DP analog module power up */
  AnalogixDpSetAnalogPowerDown(Dp, POWER_ALL, 0);

  return 0;
}

BOOLEAN
AnalogixDpSscSupportes (
  OUT struct AnalogixDpDevice *Dp
  )
{
  // Todo
  return Dp->LinkTrain.SSC;
}

UINTN
AnalogixDpSetLinkBandwidth (
  OUT struct AnalogixDpDevice *Dp,
  UINT32 BwType
  )
{
  UINTN i;
  UINT32 Reg;
  OUT struct PhyConfigureOptsDp OPTS_DP;
  OUT struct RockchipHdptxPhy Hdptx;
  Dp->Id = Hdptx.Id = PcdGet32(PcdEdpId);

  OPTS_DP.LANES = Dp->LinkTrain.LaneCount;
  OPTS_DP.LINKRATE = DrmDpBwCodeToLinkRate(Dp->LinkTrain.LinkRate) / 100;
  OPTS_DP.SSC = AnalogixDpSscSupportes(Dp);
  OPTS_DP.SETLANES = FALSE;
  OPTS_DP.SETRATE = TRUE;
  OPTS_DP.SETVOLTAGES = FALSE;

  RockchipHdptxPhyConfigure(&Hdptx, &OPTS_DP);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_LINK_BW_SET, BwType);

  Reg = AnalogixDpGetPllLockStatus(Dp);
  for (i = 0; i < 20; i++) {
    if (Reg != PLL_LOCKED)
      MicroSecondDelay (10);
    else
      break;
  }

  return 0;
}

UINTN
AnalogixDpSetLaneCount (
  OUT struct AnalogixDpDevice *Dp,
  UINT32 Count
  )
{
  UINTN Reg ;
  OUT struct PhyConfigureOptsDp OPTS_DP;
  OUT struct RockchipHdptxPhy Hdptx;
  Dp->Id = Hdptx.Id = PcdGet32(PcdEdpId);

  Reg = Count;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_LANE_COUNT_SET, Reg);

  OPTS_DP.LANES = Dp->LinkTrain.LaneCount;
  OPTS_DP.LINKRATE = DrmDpBwCodeToLinkRate(Dp->LinkTrain.LinkRate) / 100;
  OPTS_DP.SSC = AnalogixDpSscSupportes(Dp);
  OPTS_DP.SETLANES = TRUE;
  OPTS_DP.SETRATE = FALSE;
  OPTS_DP.SETVOLTAGES = FALSE;
  RockchipHdptxPhyConfigure(&Hdptx, &OPTS_DP);

  return 0 ;
}

STATIC
INT32
AnalogixDpLinkStart (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT8 Buf[4];
  INT32 Lane, LaneCount, Retval;

  LaneCount = Dp->LinkTrain.LaneCount;
  Dp->LinkTrain.LtState = CLOCK_RECOVERY;
  Dp->LinkTrain.EqLoop = 0;
 
  for (Lane = 0; Lane < LaneCount; Lane++)
    Dp->LinkTrain.CrLoop[Lane] = 0;

  /* Set link rate and count as you want to establish*/
  AnalogixDpSetLinkBandwidth(Dp, Dp->LinkTrain.LinkRate);
  AnalogixDpSetLaneCount(Dp, Dp->LinkTrain.LaneCount);

  /* Setup RX configuration */
  Buf[0] = Dp->LinkTrain.LinkRate;
  Buf[1] = Dp->LinkTrain.LaneCount;
  Retval = AnalogixDpWriteBytesToDpcd(Dp, DP_LINK_BW_SET, 2, Buf);
  if (Retval)
    return Retval;

  /* Spread AMP if required, Enable 8b/10b coding */
  Buf[0] = AnalogixDpSscSupportes(Dp) ? DP_SPREAD_AMP_0_5 : 0;
  Buf[1] = DP_SET_ANSI_8B10B;
  Retval = AnalogixDpWriteBytesToDpcd(Dp, DP_DOWNSPREAD_CTRL,
      2, Buf);
  if (Retval < 0)
    return Retval;

  /* Set TX voltage-swing and pre-emphasis to minimum */
  for (Lane = 0; Lane < LaneCount; Lane++)
    Dp->LinkTrain.TrainingLane[Lane] =
      DP_TRAIN_VOLTAGE_SWING_LEVEL_0 |
      DP_TRAIN_PRE_EMPH_LEVEL_0;
  AnalogixDpSetLaneLinkTraining(Dp);

  /* Set training pattern 1 */
  AnalogixDpSetTrainingPattern(Dp, TRAINING_PTN1);

  /* Set RX training pattern */
  Retval = AnalogixDpWriteByteToDpcd(Dp,
      DP_TRAINING_PATTERN_SET,
      DP_LINK_SCRAMBLING_DISABLE | DP_TRAINING_PATTERN_1);
  if (Retval)
    return Retval;

  for (Lane = 0; Lane < LaneCount; Lane++)
    Buf[Lane] = DP_TRAIN_PRE_EMPH_LEVEL_0 |
      DP_TRAIN_VOLTAGE_SWING_LEVEL_0;

  Retval = AnalogixDpWriteBytesToDpcd(Dp, DP_TRAINING_LANE0_SET,
      LaneCount, Buf);

  return Retval;
}

STATIC
UINT8
AnalogixDpGetAdjustRequestVoltage (
  UINT8 AdjustRequest[2],
  UINTN Lane)
{
  UINTN Shift = (Lane & 1) * 4;
  UINT8 LinkValue = AdjustRequest[Lane >> 1];

  return (LinkValue >> Shift) & 0x3;
}

STATIC
UINT8
AnalogixDpGetAdjustRequestPreEmphasis (
  UINT8 AdjustRequest[2],
  UINTN Lane
  )
{
  UINTN shift = (Lane & 1) * 4;
  UINT8 link_value = AdjustRequest[Lane >> 1];

  return ((link_value >> shift) & 0xc) >> 2;
}

STATIC
VOID
AnalogixDpTrainingPatternDis (
  OUT struct AnalogixDpDevice *Dp
  )
{
  AnalogixDpSetTrainingPattern(Dp, DP_NONE);

  AnalogixDpWriteByteToDpcd(Dp, DP_TRAINING_PATTERN_SET,
      DP_TRAINING_PATTERN_DISABLE);
}

STATIC
INT32
AnalogixDpIsEnhancedModeAvailable (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT8 Data;
  INT32 Retval;

  AnalogixDpReadByteFromDpcd(Dp, DP_MAX_LANE_COUNT, &Data);
  Retval = DPCD_ENHANCED_FRAME_CAP(Data);

  return Retval;
}

STATIC
VOID
AnalogixDpSetEnhancedMode (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT8 Data;

  Data = AnalogixDpIsEnhancedModeAvailable(Dp);
  AnalogixDpEnableRxToEnhancedMode(Dp, Data);
  AnalogixDpEnableEnhancedMode(Dp, Data);
}

STATIC
VOID
AnalogixDpReduceLinkRate (
  OUT struct AnalogixDpDevice *Dp
  )
{
  AnalogixDpTrainingPatternDis(Dp);
  AnalogixDpSetEnhancedMode(Dp);
  Dp->LinkTrain.LtState = FAILED;
}

STATIC
VOID
AnalogixDpGetAdjustTrainingLane (
  OUT struct AnalogixDpDevice *Dp,
  UINT8 AdjustRequest[2])
{
  UINTN Lane , LaneCount;
  UINT8 VoltageSwing, PreEmphasis, TrainingLane;
  LaneCount = Dp->LinkTrain.LaneCount;

  for (Lane = 0; Lane < LaneCount; Lane++) {
    VoltageSwing = AnalogixDpGetAdjustRequestVoltage(
        AdjustRequest, Lane
        );
    PreEmphasis = AnalogixDpGetAdjustRequestPreEmphasis(
        AdjustRequest, Lane
        );
    TrainingLane = DPCD_VOLTAGE_SWING_SET(VoltageSwing) |
      DPCD_PRE_EMPHASIS_SET(PreEmphasis);
    if (VoltageSwing == VOLTAGE_LEVEL_3)
      TrainingLane |= DP_TRAIN_MAX_SWING_REACHED;
    if (PreEmphasis == PRE_EMPHASIS_LEVEL_3)
      TrainingLane |= DP_TRAIN_MAX_PRE_EMPHASIS_REACHED;
 
    Dp->LinkTrain.TrainingLane[Lane] = TrainingLane;
  }
}

VOID
AnalogixDpSetLaneLinkTraining (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT8 Lane;
  OUT struct PhyConfigureOptsDp OPTS_DP;
  OUT struct RockchipHdptxPhy Hdptx;
  Dp->Id = Hdptx.Id = PcdGet32(PcdEdpId);

  for (Lane = 0; Lane < Dp->LinkTrain.LaneCount; Lane++) {
    UINT8 TrainingLane = Dp->LinkTrain.TrainingLane[Lane];
    UINT8 VS, PE;

    AnalogixDpRegWrite(Dp,
        ANALOGIX_DP_LN0_LINK_TRAINING_CTL + 4 * Lane,
        Dp->LinkTrain.TrainingLane[Lane]);

    VS = (TrainingLane & DP_TRAIN_VOLTAGE_SWING_MASK) >>
      DP_TRAIN_VOLTAGE_SWING_SHIFT;
    PE = (TrainingLane & DP_TRAIN_PRE_EMPHASIS_MASK) >>
      DP_TRAIN_PRE_EMPHASIS_SHIFT;
    OPTS_DP.VOLTAGE[Lane] = VS;
    OPTS_DP.PRE[Lane] = PE;
  }

  OPTS_DP.LANES = Dp->LinkTrain.LaneCount;
  OPTS_DP.LINKRATE = DrmDpBwCodeToLinkRate(Dp->LinkTrain.LinkRate) / 100;
  OPTS_DP.SETLANES = FALSE;
  OPTS_DP.SETRATE = FALSE;
  OPTS_DP.SETVOLTAGES = TRUE;
  RockchipHdptxPhyConfigure(&Hdptx, &OPTS_DP);
}

STATIC
UINT8
AnalogixDpGetLaneStatus (
  UINT8 LinkStatus[2],
  INT32 Lane
  )
{
  INT32 Shift = (Lane & 1) * 4;
  UINT8 LinkValue = LinkStatus[Lane >> 1];

  return (LinkValue >> Shift) & 0xf;
}

STATIC
INT32
AnalogixDpClockRecoveryOk (
  UINT8 LinkStatus[2],
  INT32 LaneCount
  )
{
  INT32 Lane;
  UINT8 LaneStatus;

  for (Lane = 0; Lane < LaneCount; Lane++) {
    LaneStatus = AnalogixDpGetLaneStatus(LinkStatus, Lane);
    if ((LaneStatus & DP_LANE_CR_DONE) == 0)
      return -EINVAL;
  }
  return 0;
}

BOOLEAN
AnalogixDpTps3Supported (
  OUT struct AnalogixDpDevice *Dp
  )
{
  BOOLEAN SourceTps3Supported, SinkTps3Supported;
  UINT8 Dpcd = 0;

  SourceTps3Supported =
    Dp->VideoInfo.MaxLinkRate == DP_LINK_BW_5_4;

  AnalogixDpReadByteFromDpcd(Dp, DP_MAX_LANE_COUNT, &Dpcd);
  SinkTps3Supported = Dpcd & DP_TPS3_SUPPORTED;

  return SourceTps3Supported && SinkTps3Supported;
}

STATIC
INT32
AnalogixDpProcessClockRecovery (
  OUT struct AnalogixDpDevice *Dp
  )
{
  INT32 Lane, LaneCount, Retval;
  UINT8 VoltageSwing, PreEmphasis, TrainingLane;
  UINT8 LinkStatus[2], AdjustRequest[2];
  UINT8 TrainingPattern = TRAINING_PTN2;

  DrmDpLinkTrainClockRecoveryDelay(Dp->Dpcd);

  LaneCount = Dp->LinkTrain.LaneCount;

  Retval = AnalogixDpReadBytesFromDpcd(Dp,
      DP_LANE0_1_STATUS, 2, LinkStatus);
  if (Retval)
    return Retval;

  if (AnalogixDpClockRecoveryOk(LinkStatus, LaneCount) == 0) {
    if (AnalogixDpTps3Supported(Dp))
      TrainingPattern = TRAINING_PTN3;

    /* set training pattern for EQ*/
    AnalogixDpSetTrainingPattern(Dp, TrainingPattern);

    Retval = AnalogixDpWriteByteToDpcd(Dp,
        DP_TRAINING_PATTERN_SET,
        (TrainingPattern == TRAINING_PTN3 ?
         DP_TRAINING_PATTERN_3 : DP_TRAINING_PATTERN_2));
    if (Retval)
      return Retval;

    DEBUG ((EFI_D_WARN, "Link Training Clock Recovery success\n"));
    Dp->LinkTrain.LtState = EQUALIZER_TRAINING;

    return 0;
  } else {
    Retval = AnalogixDpReadBytesFromDpcd(Dp,
        DP_ADJUST_REQUEST_LANE0_1, 2, AdjustRequest);
    if (Retval)
      return Retval;

    for (Lane = 0; Lane < LaneCount; Lane++) {
      TrainingLane = AnalogixDpGetLaneLinkTraining(
          Dp, Lane);
      VoltageSwing = AnalogixDpGetAdjustRequestVoltage(
          AdjustRequest, Lane);
      PreEmphasis = AnalogixDpGetAdjustRequestPreEmphasis(
          AdjustRequest, Lane);

      if (DPCD_VOLTAGE_SWING_GET(TrainingLane) ==
          VoltageSwing &&
        DPCD_PRE_EMPHASIS_GET(TrainingLane) ==
          PreEmphasis)
      Dp->LinkTrain.CrLoop[Lane]++;

      if (Dp->LinkTrain.CrLoop[Lane] == MAX_CR_LOOP ||
          VoltageSwing == VOLTAGE_LEVEL_3 ||
          PreEmphasis == PRE_EMPHASIS_LEVEL_3) {
        DEBUG ((EFI_D_ERROR, "CR Max reached (%d,%d)\n",
              Dp->LinkTrain.CrLoop[Lane],
              VoltageSwing, PreEmphasis));
        AnalogixDpReduceLinkRate(Dp);
        return -EIO;
      }
    }
  }

  AnalogixDpGetAdjustTrainingLane(Dp, AdjustRequest);
  AnalogixDpSetLaneLinkTraining(Dp);

  Retval = AnalogixDpWriteBytesToDpcd(Dp,
      DP_TRAINING_LANE0_SET, LaneCount,
      Dp->LinkTrain.TrainingLane);
  if (Retval)
    return Retval;
  
  return Retval;
}

STATIC
INT32
AnalogixDpChannelEqOk (
  UINT8 LinkStatus[2],
  UINT8 LinkAlign,
  INT32 LaneCount
  )
{
  INT32 Lane;
  UINT8 LaneStatus;

  if ((LinkAlign & DP_INTERLANE_ALIGN_DONE) == 0)
    return -EINVAL;

  for (Lane = 0; Lane < LaneCount; Lane++) {
    LaneStatus = AnalogixDpGetLaneStatus(LinkStatus, Lane);
    LaneStatus &= DP_CHANNEL_EQ_BITS;
    if (LaneStatus != DP_CHANNEL_EQ_BITS)
      return -EINVAL;
  }

  return 0;
}

STATIC
INT32
AnalogixDpProcessEqualizerTraining (
  OUT struct AnalogixDpDevice *Dp
  )
{
  INT32 LaneCount, Retval;
  UINT32 Reg;
  UINT8 LinkAlign;
  UINT8 LinkStatus[2], AdjustRequest[2];

  DrmDpLinkTrainChannelEqDelay(Dp->Dpcd);

  LaneCount = Dp->LinkTrain.LaneCount;

  Retval = AnalogixDpReadBytesFromDpcd(Dp,
      DP_LANE0_1_STATUS, 2, LinkStatus);
  if (Retval)
    return Retval;

  if (AnalogixDpClockRecoveryOk(LinkStatus, LaneCount)) {
    DEBUG ((EFI_D_WARN, "AnalogixDpReduceLinkRate \n"));
    AnalogixDpReduceLinkRate(Dp);
    //Todo
    //return -EIO;
  }

  Retval = AnalogixDpReadBytesFromDpcd(Dp,
      DP_ADJUST_REQUEST_LANE0_1, 2, AdjustRequest);
  if (Retval)
    return Retval;

  Retval = AnalogixDpReadByteFromDpcd(Dp,
      DP_LANE_ALIGN_STATUS_UPDATED, &LinkAlign);
  if (Retval)
    return Retval;

  /* Training pattern set to normal */
  AnalogixDpTrainingPatternDis(Dp);
  if (!AnalogixDpChannelEqOk(LinkStatus, LinkAlign, LaneCount)) {

    DEBUG ((EFI_D_WARN, "Link Training success\n"));

    AnalogixDpGetLinkBandwidth(Dp, &Reg);
    Dp->LinkTrain.LinkRate = Reg;
    AnalogixDpGetLaneCount(Dp, &Reg);
    Dp->LinkTrain.LaneCount = Reg;

    DEBUG ((EFI_D_WARN, "Final link rate = 0x%.2x, Lane count = 0x%.2x\n",
          Dp->LinkTrain.LinkRate, Dp->LinkTrain.LaneCount));

    /* Set enhanced mode if available */
    AnalogixDpSetEnhancedMode(Dp);
    Dp->LinkTrain.LtState = FINISHED;

    return 0;
  }

  /* not all locked */
  Dp->LinkTrain.EqLoop++;
  if (Dp->LinkTrain.EqLoop > MAX_EQ_LOOP) {
    DEBUG ((EFI_D_WARN, "EQ Max loop\n"));
    AnalogixDpReduceLinkRate(Dp);
    return -EIO;
  }

  Retval = AnalogixDpReadBytesFromDpcd(Dp,
      DP_ADJUST_REQUEST_LANE0_1, 2, AdjustRequest);
  if (Retval)
    return Retval;

  AnalogixDpGetAdjustTrainingLane(Dp, AdjustRequest);
  AnalogixDpSetLaneLinkTraining(Dp);

  Retval = AnalogixDpWriteBytesToDpcd(Dp, DP_TRAINING_LANE0_SET,
      LaneCount, Dp->LinkTrain.TrainingLane);

  return Retval;
}

STATIC
INT32
AnalogixDpSwLinkTraining (
  OUT struct AnalogixDpDevice *Dp
  )
{
  INT32 Retval = 0, TrainingFinished = 0;

  Dp->LinkTrain.LtState = START;

  while (!Retval && !TrainingFinished) {
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
      TrainingFinished = 1;
      break;
    case FAILED:
      return -EIO;
    }
  }

  return Retval;
}

INT32
AnalogixDpSetLinkTrain (
  OUT struct AnalogixDpDevice *Dp,
  UINT32 Count,
  UINT32 BwType
  )
{
  INT32 i, Ret;

  for (i = 0; i < 5; i++) {
    Ret = AnalogixDpInitTraining(Dp, Count, BwType);
    if (Ret < 0) {
      DEBUG ((EFI_D_WARN, "faiied to init training\n")); 
      return Ret;
    }

    Ret = AnalogixDpSwLinkTraining(Dp);
    if (!Ret)
      break;
  }

  return Ret;
}

VOID
AnalogixDpConfigVideoSlaveMode (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg;

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_FUNC_EN_1);
  Reg &= ~(MASTER_VID_FUNC_EN_N | SLAVE_VID_FUNC_EN_N);
  Reg |= MASTER_VID_FUNC_EN_N;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_FUNC_EN_1, Reg);

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_VIDEO_CTL_10);
  Reg &= ~INTERACE_SCAN_CFG;
  Reg |= (Dp->VideoInfo.Interlaced << 2);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_VIDEO_CTL_10, Reg);

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_VIDEO_CTL_10);
  Reg &= ~VSYNC_POLARITY_CFG;
  Reg |= (Dp->VideoInfo.VSyncPolarity << 1);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_VIDEO_CTL_10, Reg);

  Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_VIDEO_CTL_10);
  Reg &= ~HSYNC_POLARITY_CFG;
  Reg |= (Dp->VideoInfo.HSyncPolarity << 0);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_VIDEO_CTL_10, Reg);

  Reg = AUDIO_MODE_SPDIF_MODE | VIDEO_MODE_SLAVE_MODE;
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_SOC_GENERAL_CTL, Reg);
}

VOID
AnalogixDpSetVideoColorFormat (
  OUT struct AnalogixDpDevice *Dp
  )
{
  //Todo
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_VIDEO_CTL_2, 0x10);
  AnalogixDpRegWrite(Dp, ANALOGIX_DP_VIDEO_CTL_3, 0x00);
}

VOID
AnalogixDpSetVideoCrMn (
  OUT struct AnalogixDpDevice *Dp,
  enum ClockRecoveryMValueType Type,
  UINT32 MValue,
  UINT32 NValue
  )
{
  UINT32 Reg;

  if (Type == REGISTER_M) {
    Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_SYS_CTL_4);
    Reg |= FIX_M_VID;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_SYS_CTL_4, Reg);
    Reg = MValue & 0xff;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_M_VID_0, Reg);
    Reg = (MValue >> 8) & 0xff;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_M_VID_1, Reg);
    Reg = (MValue >> 16) & 0xff;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_M_VID_2, Reg);

    Reg = NValue & 0xff;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_N_VID_0, Reg);
    Reg = (NValue >> 8) & 0xff;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_N_VID_1, Reg);
    Reg = (NValue >> 16) & 0xff;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_N_VID_2, Reg);
  } else {
    Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_SYS_CTL_4);
    Reg &= ~FIX_M_VID;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_SYS_CTL_4, Reg);

    AnalogixDpRegWrite(Dp, ANALOGIX_DP_N_VID_0, 0x00);
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_N_VID_1, 0x80);
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_N_VID_2, 0x00);
  }
}

VOID
AnalogixDpSetVideoTimingMode (
  OUT struct AnalogixDpDevice *Dp,
  UINT32 Type
  )
{
  UINT32 Reg;

  if (Type == VIDEO_TIMING_FROM_CAPTURE) {
    Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_VIDEO_CTL_10);
    Reg &= ~FORMAT_SEL;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_VIDEO_CTL_10, Reg);
  } else {
    Reg = AnalogixDpRegRead(Dp, ANALOGIX_DP_VIDEO_CTL_10);
    Reg |= FORMAT_SEL;
    AnalogixDpRegWrite(Dp, ANALOGIX_DP_VIDEO_CTL_10, Reg);
  }
}

STATIC 
INT32
AnalogixDpConfigVideo (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT32 Reg ,Ret;
  UINT32 TimeoutLoop, DoneCount;

  AnalogixDpConfigVideoSlaveMode(Dp);
  AnalogixDpSetVideoColorFormat(Dp);

  Reg = AnalogixDpGetPllLockStatus(Dp);
  if (Reg != PLL_LOCKED ) {
    DEBUG ((EFI_D_WARN, "PLL is not locked yet. \n"));
    Ret = AnalogixDpRegRead(Dp, ANALOGIX_DP_DEBUG_CTL);
    DEBUG ((EFI_D_WARN, "PLL Dp debug ctl ret 0x%.8x\n", Ret));
  }

  for (;;) {
    TimeoutLoop++;
    if (AnalogixDpIsSlaveVideoStreamClockOn(Dp) == 0)
      break;
    if (TimeoutLoop > DP_TIMEOUT_LOOP_COUNT) {
      DEBUG ((EFI_D_WARN, "Timeout of video streamclk ok\n"));
      return -ETIMEDOUT;
    }

    NanoSecondDelay(2000);
  }

  /* Set to use the Register calculated M/N video */
  AnalogixDpSetVideoCrMn(Dp, CALCULATED_M, 0, 0);

  /* For video bist Video timing must be generated register */
  AnalogixDpSetVideoTimingMode(Dp, VIDEO_TIMING_FROM_CAPTURE);

  /* Disable video mute */
  AnalogixDpEnableVideoMute(Dp, 0);

  /* Configure video slave mode */
  AnalogixDpEnableVideoMaster(Dp, 0);

  /* Enable video input */
  AnalogixDpStartVideo(Dp);
	
  TimeoutLoop = 0;

  for (;;) {
    TimeoutLoop++;
    if (AnalogixDpIsVideoStreamOn(Dp) == 0) {
      DoneCount++;
      if (DoneCount > 10)
        break;
    } else if (DoneCount)
      DoneCount = 0;

    if (TimeoutLoop > DP_TIMEOUT_LOOP_COUNT) {
      DEBUG ((EFI_D_ERROR, "Timeout of video streamclk ok\n"));
      return -ETIMEDOUT;
    }

    NanoSecondDelay(1001000);
  }

  return 0;
}

EFI_STATUS
AnalogixDpConnectorPreInit (
  OUT ROCKCHIP_CONNECTOR_PROTOCOL          *This,
  OUT DISPLAY_STATE                        *DisplayState
  )
{
  CONNECTOR_STATE *ConnectorState = &DisplayState->ConnectorState;
  struct RockchipHdptxPhy Hdptx;
  struct AnalogixDpDevice *Dp;
  Dp = AllocateZeroPool(sizeof (*Dp));

  ConnectorState->Type = DRM_MODE_CONNECTOR_eDP;
  ConnectorState->OutputInterface = VOP_OUTPUT_IF_eDP1;
  Dp->Id = Hdptx.Id = PcdGet32(PcdEdpId);

  if (Dp->Id)
    ConnectorState->OutputInterface = VOP_OUTPUT_IF_eDP1;
  else
    ConnectorState->OutputInterface = VOP_OUTPUT_IF_eDP0;

  if (ConnectorState->OutputInterface == VOP_OUTPUT_IF_eDP0) {
    MmioWrite32(0xFD5A8000, 0xFFFF0001);
  } else {
    MmioWrite32(0xFD5A8004, 0xFFFF0001);
  }

  RockchipHdptxPhyInit(&Hdptx);
  MicroSecondDelay (10);
  EnableBacklight(TRUE);
  EnablePWM(TRUE);
  AnalogixDpReset(Dp);
  AnalogixDpSwreset(Dp);
  AnalogixDpInitAnalogParam(Dp);
  AnalogixDpInitInterrupt(Dp);
  /* SW defined function Normal operation */
  AnalogixDpEnableSwFunction(Dp);
  AnalogixDpConfigInterrupt(Dp);
  AnalogixDpInitAnalogFunc(Dp);
  AnalogixDpInitHpd(Dp);
  AnalogixDpInitAux(Dp);

  return 0;
};

STATIC
INT32
AnalogixDpLinkPowerUp (
  OUT struct AnalogixDpDevice *Dp
  )
{
  UINT8 Value;
  INTN Ret;

  if (Dp->Dpcd[DP_DPCD_REV] < 0x11)
    return 0;

  Ret = AnalogixDpReadByteFromDpcd(Dp, DP_SET_POWER, &Value);
  if (Ret < 0)
    return Ret;

  Value &= ~DP_SET_POWER_MASK;
  Value |= DP_SET_POWER_D0;

  Ret = AnalogixDpWriteByteToDpcd(Dp, DP_SET_POWER, Value);
  if (Ret < 0)
    return Ret;

  MicroSecondDelay (1);
  return 0;
};

EFI_STATUS
AnalogixDpConnectorInit (
  OUT ROCKCHIP_CONNECTOR_PROTOCOL          *This,
  OUT DISPLAY_STATE                        *DisplayState
  )
{
  CONNECTOR_STATE *ConnectorState = &DisplayState->ConnectorState;

  ConnectorState->OutputMode = ROCKCHIP_OUT_MODE_AAAA;
  ConnectorState->ColorSpace = V4L2_COLORSPACE_DEFAULT;

  return 0;
};

EFI_STATUS
AnalogixDpConnectorGetEdid (
  OUT ROCKCHIP_CONNECTOR_PROTOCOL          *This,
  OUT DISPLAY_STATE                        *DisplayState
  )
{
  //Todo
  return 0;
};

EFI_STATUS
AnalogixDpConnectorEnable (
  OUT ROCKCHIP_CONNECTOR_PROTOCOL          *This,
  OUT DISPLAY_STATE                       *DisplayState
  )
{
  CONNECTOR_STATE *ConnectorState = &DisplayState->ConnectorState;
  struct AnalogixDpDevice *Dp;
  Dp = AllocatePool(sizeof (*Dp));
  struct VideoInfo *Video = &Dp->VideoInfo;
  UINTN Ret;

  switch (ConnectorState->BPC) {
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

  Dp->VideoInfo.MaxLinkRate = DP_LINK_BW_5_4;
  Dp->VideoInfo.MaxLaneCount = LANE_COUNT4;
  Dp->LinkTrain.SSC = TRUE;
  Dp->Id = PcdGet32(PcdEdpId);

  Ret = AnalogixDpReadBytesFromDpcd(Dp, DP_DPCD_REV,
      DP_RECEIVER_CAP_SIZE, Dp->Dpcd);
  if (Ret) {
    DEBUG ((DEBUG_WARN, "Falied to read dpcd cap \n"));
    return Ret;
  }

  Ret = AnalogixDpLinkPowerUp(Dp);
  if (Ret) {
    DEBUG ((DEBUG_WARN, "Falied to link power up \n"));
    return Ret;
  }

  Ret = AnalogixDpSetLinkTrain(Dp, Dp->VideoInfo.MaxLaneCount,
      Dp->VideoInfo.MaxLinkRate);
  if (Ret) {
    DEBUG ((DEBUG_WARN, "Unable to do link train !!! \n"));
    return Ret;
  }

  AnalogixDpEnableScramble(Dp, 1);
  AnalogixDpEnableRxToEnhancedMode(Dp, 1);
  AnalogixDpEnableEnhancedMode(Dp, 1);

  AnalogixDpInitVideo(Dp);
  AnalogixDpSetVideoFormat(Dp, &ConnectorState->DisplayMode);
  AnalogixDpConfigVideo(Dp);

#if REG_DUMP
  DumpDpRegisters(Dp);
  DumpHdptxPhyRegisters(Dp);
#endif
  FreePool(Dp);
  return 0;
};

EFI_STATUS
AnalogixDpConnectorDisable (
  OUT ROCKCHIP_CONNECTOR_PROTOCOL          *This,
  OUT DISPLAY_STATE                        *DisplayState
  )
{
  //Todo
  return 0;
};

EFI_STATUS
AnalogixDpConnectorDetect (
  OUT ROCKCHIP_CONNECTOR_PROTOCOL          *This,
  OUT DISPLAY_STATE                        *DisplayState
  )
{
  //Todo
  return 0;
};

ROCKCHIP_CONNECTOR_PROTOCOL mDp = {
  NULL,
  AnalogixDpConnectorPreInit,
  AnalogixDpConnectorInit,
  NULL,
  AnalogixDpConnectorDetect,
  NULL,
  AnalogixDpConnectorGetEdid,
  NULL,
  AnalogixDpConnectorEnable,
  AnalogixDpConnectorDisable,
  NULL
};

EFI_STATUS
EFIAPI
AnalogixDpInitDp (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS    Status;
  EFI_HANDLE    Handle;

  Handle = NULL;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gRockchipConnectorProtocolGuid,
                  &mDp,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
