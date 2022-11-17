/********************************************************************************
Copyright (C) 2018 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

Glossary - abbreviations used in Marvell SampleAtReset library implementation:
AP - Application Processor hardware block (Armada 7k8k incorporates AP806)
CP - South Bridge hardware blocks (Armada 7k8k incorporates CP110)
SAR - Sample At Reset

*******************************************************************************/

#define SAR_MAX_OPTIONS           16

#define AP806_SAR_BASE            0xf06f8200
#define SAR_CLOCK_FREQUENCY_MODE_MASK  0x1f

#define CP110_SAR_BASE(_CpIndex)  (0xf2000000 + (0x2000000 * (_CpIndex)) + 0x400200)

typedef enum {
  CPU_2000_DDR_1200_RCLK_1200 = 0x0,
  CPU_2000_DDR_1050_RCLK_1050 = 0x1,
  CPU_1600_DDR_800_RCLK_800   = 0x4,
  CPU_1800_DDR_1200_RCLK_1200 = 0x6,
  CPU_1800_DDR_1050_RCLK_1050 = 0x7,
  CPU_1600_DDR_1050_RCLK_1050 = 0x0d,
  CPU_1000_DDR_650_RCLK_650   = 0x13,
  CPU_1300_DDR_800_RCLK_800   = 0x14,
  CPU_1300_DDR_650_RCLK_650   = 0x17,
  CPU_1200_DDR_800_RCLK_800   = 0x19,
  CPU_1400_DDR_800_RCLK_800   = 0x1a,
  CPU_600_DDR_800_RCLK_800    = 0x1b,
  CPU_800_DDR_800_RCLK_800    = 0x1c,
  CPU_1000_DDR_800_RCLK_800   = 0x1d,
} CLOCKING_OPTIONS;

typedef struct {
  UINT32 CpuFrequency;
  UINT32 DdrFrequency;
  UINT32 RingFrequency;
  CLOCKING_OPTIONS ClockingOption;
} PLL_FREQUENCY_DESCRIPTION;

STATIC CONST PLL_FREQUENCY_DESCRIPTION PllFrequencyTable[SAR_MAX_OPTIONS] = {
  /* CPU   DDR   Ring  [MHz] */
  {2000,  1200,  1200, CPU_2000_DDR_1200_RCLK_1200},
  {2000,  1050,  1050, CPU_2000_DDR_1050_RCLK_1050},
  {1800,  1200,  1200, CPU_1800_DDR_1200_RCLK_1200},
  {1800,  1050,  1050, CPU_1800_DDR_1050_RCLK_1050},
  {1600,  1050,  1050, CPU_1600_DDR_1050_RCLK_1050},
  {1300,  800 ,  800 , CPU_1300_DDR_800_RCLK_800},
  {1300,  650 ,  650 , CPU_1300_DDR_650_RCLK_650},
  {1600,  800 ,  800 , CPU_1600_DDR_800_RCLK_800},
  {1000,  650 ,  650 , CPU_1000_DDR_650_RCLK_650},
  {1200,  800 ,  800 , CPU_1200_DDR_800_RCLK_800},
  {1400,  800 ,  800 , CPU_1400_DDR_800_RCLK_800},
  {600 ,  800 ,  800 , CPU_600_DDR_800_RCLK_800},
  {800 ,  800 ,  800 , CPU_800_DDR_800_RCLK_800},
  {1000,  800 ,  800 , CPU_1000_DDR_800_RCLK_800}
};
