/** @file
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/BaseLib.h>
#include <Library/CruLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/SaradcLib.h>
#include <Library/TimerLib.h>

#define SARADC_BASE  0xfec10000

#define SARADC_CLOCK_RATE     1000000
#define SARADC_CHANNEL_COUNT  8
#define SARADC_RESOLUTION     12
#define SARADC_DATA_MASK      (1 << SARADC_RESOLUTION) - 1

#define SARADC_TIMEOUT_US  100000

#define SARADC_CONV_CON         0x0000
#define  SARADC_SINGLE_PD_MODE  BIT5
#define  SARADC_START_ADC       BIT4
#define SARADC_T_PD_SOC         0x0004
#define SARADC_T_AS_SOC         0x0008
#define SARADC_T_DAS_SOC        0x000C
#define SARADC_T_SEL_SOC        0x0010
#define SARADC_HIGH_COMP_BASE   0x0014
#define SARADC_LOW_COMP_BASE    0x0054
#define SARADC_DEBOUNCE         0x0094
#define SARADC_HT_INT_EN        0x0098
#define SARADC_LT_INT_EN        0x009C
#define SARADC_MT_INT_EN        0x0100
#define SARADC_END_INT_EN       0x0104
#define SARADC_ST_CON           0x0108
#define SARADC_STATUS           0x010C
#define SARADC_END_INT_ST       0x0110
#define SARADC_HT_INT_ST        0x0114
#define SARADC_LT_INT_ST        0x0118
#define SARADC_MT_INT_ST        0x011C
#define SARADC_DATA_BASE        0x0120
#define SARADC_AUTO_CH_EN       0x0160

#define SARADC_END_INT  BIT0

#define WRITE_ENABLE_SHIFT  16

STATIC
VOID
SaradcReset (
  VOID
  )
{
  HAL_CRU_RstAssert (RESET_SRST_P_SARADC);
  MicroSecondDelay (10);
  HAL_CRU_RstDeassert (RESET_SRST_P_SARADC);
}

STATIC
VOID
SaradcStartChannel (
  IN UINT32  Channel
  )
{
  UINT32  Value;

  // Changing channels without a reset leads to glitches...
  SaradcReset ();

  MmioWrite32 (SARADC_BASE + SARADC_T_PD_SOC, 0x20);
  MmioWrite32 (SARADC_BASE + SARADC_T_DAS_SOC, 0xc);

  Value = SARADC_END_INT;
  MmioWrite32 (SARADC_BASE + SARADC_END_INT_EN, (Value << WRITE_ENABLE_SHIFT) | Value);

  Value = SARADC_START_ADC | SARADC_SINGLE_PD_MODE | Channel;
  MmioWrite32 (SARADC_BASE + SARADC_CONV_CON, (Value << WRITE_ENABLE_SHIFT) | Value);
}

RETURN_STATUS
SaradcReadChannel (
  IN  UINT32  Channel,
  OUT UINT32  *Data
  )
{
  UINTN  Retry;

  if ((Channel >= SARADC_CHANNEL_COUNT) || (Data == NULL)) {
    ASSERT (FALSE);
    return RETURN_INVALID_PARAMETER;
  }

  SaradcStartChannel (Channel);

  for (Retry = SARADC_TIMEOUT_US; Retry > 0; Retry--) {
    if (MmioRead32 (SARADC_BASE + SARADC_END_INT_ST) & SARADC_END_INT) {
      break;
    }

    MicroSecondDelay (1);
  }

  if (Retry == 0) {
    return RETURN_TIMEOUT;
  }

  MmioWrite32 (SARADC_BASE + SARADC_END_INT_ST, SARADC_END_INT);

  *Data  = MmioRead32 (SARADC_BASE + SARADC_DATA_BASE + (Channel * sizeof (UINT32)));
  *Data &= SARADC_DATA_MASK;

  return RETURN_SUCCESS;
}

RETURN_STATUS
EFIAPI
SaradcLibConstructor (
  VOID
  )
{
  if (HAL_CRU_ClkGetFreq (CLK_SARADC) != SARADC_CLOCK_RATE) {
    HAL_CRU_ClkSetFreq (CLK_SARADC, SARADC_CLOCK_RATE);
  }

  return RETURN_SUCCESS;
}
