/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _CPU_PPM_LIB_H_
#define _CPU_PPM_LIB_H_


#include <PiDxe.h>
#include <Protocol/CpuCsrAccess.h>
#include <Library/TimerLib.h>
#include "SysHost.h"
#include "UncoreCommonIncludes.h"

#define NUM_CST_LAT_MSR     3

//
// Value definition for CpuPCPSCtrl
//
#define PCD_CPU_PCPS_SINGLEPCTL         0x00000004
#define PCD_CPU_PCPS_SPD                0x00000008
#define PCD_CPU_PCPS_PSTATEDOMAIN       0x00000010

// Bits configuration for PcdAdvPwrMgtFlags
#define PCD_CPU_PKG_CST_ENTRY_VAL_CTL   0x00000001
#define PCD_CPU_SAPM_CTL_VAL_CTL        0x00000002
#define PCD_CPU_SKIP_PKG_CST_ENTRY      0x00000004
#define PCD_CPU_SW_LTR_OVRD_CTL         0x00000008
#define PCD_CPU_PRI_PLN_CURR_CFG_CTL    0x00000010
#define PCD_CPU_CURRENT_CONFIG          0x00000020
#define PCU_CPU_EFFICIENT_BOOT          0x00000040
#define CPU_MSR_LOCK                    0x00000080
#define MPLL_OFF_ENA_AUTO               0x00000100
#define DYNAMIC_L1_DISABLE              0x00000200
#define SPT_PCH_WORKAROUND              0x00000400
#define TURBO_LIMIT_CSR_LOCK            0x00000800
#define VCCSA_VCCIO_DISABLE             0x00001000


// PCU_CR_PMAX_CONFIG bit definition
#define PCU_CR_PMAX_CFG_OFFSET_SHIFT    0
#define PCU_CR_PMAX_CFG_LOCK_SHIFT      31
#define PCU_CR_PMAX_CFG_OFFSET          (0x1f << PCU_CR_PMAX_CFG_OFFSET_SHIFT)    // Bits 4:0
#define USER_PMAX_USE_OFFSET_TABLE      BIT5
#define USER_PMAX_NEGATIVE_BIT          BIT4
#define USER_PMAX_VALUE_BIT_MASK        0x0F                                      // Bits 3:0
#define PCU_CR_PMAX_CFG_LOCK            (0x1 << PCU_CR_PMAX_CFG_LOCK_SHIFT)       // Bit 31

// DYNAMIC_PERF_POWER_CTL bit definition
#define EEP_L_OVERRIDE_SHIFT                        26 // Bit 29:26
#define EEP_L_OVERRIDE_ENABLE_SHIFT                 25 // Bit 25
#define I_TURBO_OVERRIDE_ENABLE_SHIFT               24 // Bit 24
#define CST_DEMOTION_OVERRIDE_ENABLE_SHIFT          23 // Bit 23
#define TURBO_DEMOTION_OVERRIDE_ENABLE_SHIFT        22 // Bit 22
#define UNCORE_PERF_PLIMIT_OVERRIDE_ENABLE_SHIFT    20 // Bit 20
#define EET_OVERRIDE_ENABLE_SHIFT                   18 // Bit 18
#define IO_BW_PLIMIT_OVERRIDE_ENABLE_SHIFT          15 // Bit 15
#define IMC_APM_OVERRIDE_ENABLE_SHIFT               10 // Bit 10
#define IOM_APM_OVERRIDE_ENABLE_SHIFT               5  // Bit 5
#define QPI_APM_OVERRIDE_ENABLE_SHIFT               0  // Bit 0
#define EEP_L_OVERRIDE                             (0xf << EEP_L_OVERRIDE_SHIFT)                   // Bit 29:26
#define EEP_L_OVERRIDE_ENABLE                      (1 << EEP_L_OVERRIDE_ENABLE_SHIFT)              // Bit 25
#define I_TURBO_OVERRIDE_ENABLE                    (1 << I_TURBO_OVERRIDE_ENABLE_SHIFT)            // Bit 24
#define CST_DEMOTION_OVERRIDE_ENABLE               (1 << CST_DEMOTION_OVERRIDE_ENABLE_SHIFT)       // Bit 23
#define TURBO_DEMOTION_OVERRIDE_ENABLE             (1 << TURBO_DEMOTION_OVERRIDE_ENABLE_SHIFT)     // Bit 22
#define UNOCRE_PERF_PLIMIT_OVERRIDE_ENABLE         (1 << UNCORE_PERF_PLIMIT_OVERRIDE_ENABLE_SHIFT) // Bit 20
#define EET_OVERRIDE_ENABLE                        (1 << EET_OVERRIDE_ENABLE_SHIFT)                // Bit 18
#define IO_BW_PLIMIT_OVERRIDE_ENABLE               (1 << IO_BW_PLIMIT_OVERRIDE_ENABLE_SHIFT)       // Bit 15
#define IMC_APM_OVERRIDE_ENABLE                    (1 << IMC_APM_OVERRIDE_ENABLE_SHIFT)            // Bit 10
#define IOM_APM_OVERRIDE_ENABLE                    (1 << IOM_APM_OVERRIDE_ENABLE_SHIFT)            // Bit 5
#define QPI_APM_OVERRIDE_ENABLE                    (1 << QPI_APM_OVERRIDE_ENABLE_SHIFT)            // Bit 0
#define DYNAMIC_PERF_POWER_CTL_MASK  (0x3C000000 + UNOCRE_PERF_PLIMIT_OVERRIDE_ENABLE + EET_OVERRIDE_ENABLE + IO_BW_PLIMIT_OVERRIDE_ENABLE + IMC_APM_OVERRIDE_ENABLE + IOM_APM_OVERRIDE_ENABLE + QPI_APM_OVERRIDE_ENABLE)

// CSR_PCIE_ILTR_OVRD  (CSR 1:10:1:78)
// SW_LTR_OVRD (MSR 0xa02) -- not used
//CSR_PCIE_ILTR_OVRD bit definition
#define SNOOP_LATENCY_VLD_SHIFT                  31                        // Bits 31
#define FORCE_SNOOP_OVRD_SHIFT                   30                        // Bits 30
#define SNOOP_LATENCY_MUL_SHIFT                  26                        // Bits 28:26
#define SNOOP_LATENCY_Value_SHIFT                16                        // Bits 25:16
#define NON_SNOOP_LATENCY_VLD_SHIFT              15                        // Bits 15
#define FORCE_NON_SNOOP_OVRD_SHIFT               14                        // Bits 14
#define NON_SNOOP_LATENCY_MUL_SHIFT              10                        // Bits 12:10
#define NON_SNOOP_LATENCY_Value_SHIFT            0                        // Bits 9:0
#define SNOOP_LATENCY_VLD_MASK                   (1 << SNOOP_LATENCY_VLD_SHIFT)        // Bits 31
#define FORCE_SNOOP_OVRD_MASK                    (1 << FORCE_SNOOP_OVRD_SHIFT)         // Bits 30
#define SNOOP_LATENCY_MUL_MASK                   (0x7 << SNOOP_LATENCY_MUL_SHIFT)      // Bits 28:26
#define SNOOP_LATENCY_Value_MASK                 (0x3FF << SNOOP_LATENCY_Value_SHIFT)  // Bits 25:16
#define NON_SNOOP_LATENCY_VLD_MASK               (1 << NON_SNOOP_LATENCY_VLD_SHIFT)    // Bits 15
#define FORCE_NON_SNOOP_OVRD_MASK                (1 << FORCE_NON_SNOOP_OVRD_SHIFT)     // Bits 14
#define NON_SNOOP_LATENCY_MUL_MASK               (0x7 << NON_SNOOP_LATENCY_MUL_SHIFT)  // Bits 12:10
#define NON_SNOOP_LATENCY_Value_MASK  (0x3FF << NON_SNOOP_LATENCY_Value_SHIFT)       // Bits 9:0
#define SW_LTR_OVRD_CTL_MASK   (SNOOP_LATENCY_VLD_MASK + FORCE_SNOOP_OVRD_MASK + SNOOP_LATENCY_MUL_MASK + SNOOP_LATENCY_Value_MASK + \
                               NON_SNOOP_LATENCY_VLD_MASK + FORCE_NON_SNOOP_OVRD_MASK + NON_SNOOP_LATENCY_MUL_MASK + NON_SNOOP_LATENCY_Value_MASK)

//CSR_PKG_CST_ENTRY_CRITERIA_MASK bit definition
#define DRAM_IN_SR_SHIFT                         28
#define QPI_2_IN_L1_SHIFT                        27
#define QPI_1_IN_L1_SHIFT                        26
#define QPI_0_IN_L1_SHIFT                        25
#define QPI_2_IN_L0S_SHIFT                       24
#define QPI_1_IN_L0S_SHIFT                       23
#define QPI_0_IN_L0S_SHIFT                       22
#define PCIE_IN_L1_SHIFT                         11
#define PCIE_IN_L0S_SHIFT                        0
#define DRAM_IN_SR                              (1 << DRAM_IN_SR_SHIFT)
#define QPI_2_IN_L1                             (1 << QPI_2_IN_L1_SHIFT)
#define QPI_1_IN_L1                             (1 << QPI_1_IN_L1_SHIFT)
#define QPI_0_IN_L1                             (1 << QPI_0_IN_L1_SHIFT)
#define QPI_2_IN_L0S                            (1 << QPI_2_IN_L0S_SHIFT)
#define QPI_1_IN_L0S                            (1 << QPI_1_IN_L0S_SHIFT)
#define QPI_0_IN_L0S                            (1 << QPI_0_IN_L0S_SHIFT)
#define PCIE_IN_L1                              (1 << PCIE_IN_L1_SHIFT)
#define PCIE_IN_L0S                             (1 << PCIE_IN_L0S_SHIFT)
#define PCIE_IN_LX_MASK                         0x7FF          // Bit[10:0]
#define MASK_PCIE_BITS                          0xFFC00000    // clear bits 21:0
// For SKX
#define KTI_2_IN_L1_SHIFT                       2
#define KTI_1_IN_L1_SHIFT                       1
#define KTI_0_IN_L1_SHIFT                       0
#define KTI_2_IN_L1                            (1 << KTI_2_IN_L1_SHIFT)
#define KTI_1_IN_L1                            (1 << KTI_1_IN_L1_SHIFT)
#define KTI_0_IN_L1                            (1 << KTI_0_IN_L1_SHIFT)
#define MASK_PCIE_IN_L1_BITS                   0xFF000000    // clear bits 23:0
#define SET_KTI_INPKGCENTRY                    (KTI_0_IN_L1 + KTI_1_IN_L1 + KTI_2_IN_L1)
#define SET_PCIE_INPKGCENTRY                   0xFFFFFF      // set bits 23:0
#define SET_PCIEx_MASK                         0xF
#define SET_DMI_MASK                           0x1


// CSR Perf PLIMIT bit definition for HSX <= B Stepping
#define I_TURBO_WAIT_PERIOD_SHIFT              19                                       // Bits 31:19
#define PERF_P_LIMIT_THRESHOLD_SHIFT           13                                       // Bits 18:13
#define I_TURBO_EN_SHIFT                       12                                       // Bit 12
#define PERF_P_LIMIT_CLIP_SHIFT                6                                        // Bits 11:6
#define DISABLE_PERF_P_INPUT_SHIFT             5                                        // Bit 5
#define RESOLUTION_MODE_SHIFT                  1                                        // Bits 2:1
#define REPERF_P_LIMIT_EN_SHIFT                0                                        // Bit 0
#define I_TURBO_WAIT_PERIOD                   (0x1fff << I_TURBO_WAIT_PERIOD_SHIFT)     // Bits 31:19
#define PERF_P_LIMIT_THRESHOLD                (0x3f << PERF_P_LIMIT_THRESHOLD_SHIFT)    // Bits 18:13
#define I_TURBO_EN                            (1 << I_TURBO_EN_SHIFT)                   // Bit 12
#define PERF_P_LIMIT_CLIP                     (0x3f << PERF_P_LIMIT_CLIP_SHIFT)         // Bits 11:6
#define DISABLE_PERF_P_INPUT                  (1 << DISABLE_PERF_P_INPUT_SHIFT)         // Bit 5
#define RESOLUTION_MODE                       (3 << RESOLUTION_MODE_SHIFT)              // Bits 2:1
#define REPERF_P_LIMIT_EN                     (1 << REPERF_P_LIMIT_EN_SHIFT)            // Bit 0

// CSR Perf PLIMIT bit definition for HSX >= C Stepping & SKX
#define PERF_PLIMIT_DIFFERENTIAL_SHIFT        15                                     // Bits 17:15
#define PERF_PLIMIT_DIFFERENTIAL              (7 << PERF_PLIMIT_DIFFERENTIAL_SHIFT)  // Bits 17:15
#define PERF_PLIMIT_CLIP_SHIFT                7                                      // Bits 11:7
#define PERF_PLIMIT_CLIP                      (0x1f << PERF_P_LIMIT_CLIP_SHIFT)      // Bits 11:7
#define PERF_PLIMIT_THRESHOLD_SHIFT           1                                      // Bits 5:1
#define PERF_PLIMIT_THRESHOLD                 (0x1f << PERF_P_LIMIT_THRESHOLD_SHIFT) // Bits 5:1
#define REPERF_PLIMIT_EN_SHIFT                0                                      // Bit 0
#define REPERF_PLIMIT_EN                      (1 << REPERF_P_LIMIT_EN_SHIFT)         // Bit 0
#define PERF_P_LIMIT_CTRL_MASK  (PERF_PLIMIT_THRESHOLD + PERF_PLIMIT_CLIP + PERF_PLIMIT_DIFFERENTIAL + REPERF_PLIMIT_EN)

//CSR SAPMCTLbit definition
#define SAPMCTL_LOCK_SHIFT                      31                          // Bit 31 for IVT/HSX/SKX
#define SETVID_DECAY_DISABLE_SHIFT              30                          // Bit 30 for IVT/KSX/SKX
#define QPI_L0S_PLL_SEN_ENABLE_SHIFT            29                          // Bit 29//Only for IVT
#define QPI_L0_PLL_SEN_ENABLE_SHIFT             28                          // Bit 28//Only for IVT
#define IIO_L0S_PLL_SEN_ENABLE_SHIFT            27                          // Bit 27//Only for IVT
#define IIO_L0_PLL_SEN_ENABLE_SHIFT             26                          // Bit 26//Only for IVT
#define QPI2_L0S_PLL_SEN_ENABLE_SHIFT           25                          // Bit 25//Only for IVT
#define QPI2_L0_PLL_SEN_ENABLE_SHIFT            24                          // Bit 24//Only for IVT
#define QPI2_PKGC_CLOCK_GATE_DISABLE_SHIFT      18                          // Bit 18//IVT/HSX
#define QPI01_PKGC_CLOCK_GATE_DISABLE_SHIFT     17                          // Bit 17//IVT/HSX
#define IIO_PKGC_CLOCK_GATE_DISABLE_SHIFT       16                          // Bit 16//IVT/HSX
#define MDLL_ON_DE_SHIFT                        15                          // Bit 15//IVT/HSX
#define MPLL_ON_DE_SHIFT                        14                          // Bit 14//IVT/HSX
#define SACG_MPLL_SHIFT                         13                          // Bit 13//Only for IVT
#define NSWAKE_SREXIT_SHIFT                     12                          // Bit 12//IVT/HSX
#define SACG_SREXIT_SHIFT                       11                          // Bit 11//Only for IVT
#define MDLL_OFF_SEN_SHIFT                      10                          // Bit 10//Only for IVT
#define MPLL_OFF_SEN_SHIFT                      9                           // Bit 9//Only for IVT
#define SACG_SEN_SHIFT                          8                           // Bit 8//Only for IVT
#define FORCE_PPLL_OFF_SHIFT                    4                           // Bit 4 //IVT/HSX
#define QPLL_OFF_ENA_SHIFT                      3                           // Bit 3//Only for IVT
#define PPLL_OFF_ENA_SHIFT                      2                           // Bit 2//IVT/HSX
#define MPLL_OFF_ENA_SHIFT                      1                           // Bit 1//IVT/HSX
#define SACG_ENA_SHIFT                          0                           // Bit 0//Only for IVT
#define SAPMCTL_LOCK                           (1 << SAPMCTL_LOCK_SHIFT)                    // Bit 31
#define SETVID_DECAY_DISABLE                   (1 << SETVID_DECAY_DISABLE_SHIFT)            // Bit 30
#define QPI_L0S_PLL_SEN_ENABLE                 (1 << QPI_L0S_PLL_SEN_ENABLE_SHIFT)          // Bit 29
#define QPI_L0_PLL_SEN_ENABLE                  (1 << QPI_L0_PLL_SEN_ENABLE_SHIFT)           // Bit 28
#define IIO_L0S_PLL_SEN_ENABLE                 (1 << IIO_L0S_PLL_SEN_ENABLE_SHIFT)          // Bit 27
#define IIO_L0_PLL_SEN_ENABLE                  (1 << IIO_L0_PLL_SEN_ENABLE_SHIFT)           // Bit 26
#define QPI2_L0S_PLL_SEN_ENABLE                (1 << QPI2_L0S_PLL_SEN_ENABLE_SHIFT)         // Bit 25
#define QPI2_L0_PLL_SEN_ENABLE                 (1 << QPI2_L0_PLL_SEN_ENABLE_SHIFT)          // Bit 24
#define QPI2_PKGC_CLOCK_GATE_DISABLE           (1 << QPI2_PKGC_CLOCK_GATE_DISABLE_SHIFT)    // Bit 18//IVT
#define QPI01_PKGC_CLOCK_GATE_DISABLE          (1 << QPI01_PKGC_CLOCK_GATE_DISABLE_SHIFT)   // Bit 17//IVT
#define IIO_PKGC_CLOCK_GATE_DISABLE            (1 << IIO_PKGC_CLOCK_GATE_DISABLE_SHIFT)     // Bit 16//IVT
#define MDLL_ON_DE                             (1 << MDLL_ON_DE_SHIFT)       // Bit 15
#define MPLL_ON_DE                             (1 << MPLL_ON_DE_SHIFT)       // Bit 14
#define SACG_MPLL                              (1 << SACG_MPLL_SHIFT)        // Bit 13
#define NSWAKE_SREXIT                          (1 << NSWAKE_SREXIT_SHIFT)    // Bit 12
#define SACG_SREXIT                            (1 << SACG_SREXIT_SHIFT)      // Bit 11
#define MDLL_OFF_SEN                           (1 << MDLL_OFF_SEN_SHIFT)     // Bit 10
#define MPLL_OFF_SEN                           (1 << MPLL_OFF_SEN_SHIFT)     // Bit 9
#define SACG_SEN                               (1 << SACG_SEN_SHIFT)         // Bit 8
#define FORCE_PPLL_OFF                         (1 << FORCE_PPLL_OFF_SHIFT)   // Bit 4 //IVT
#define QPLL_OFF_ENA                           (1 << QPLL_OFF_ENA_SHIFT)     // Bit 3
#define PPLL_OFF_ENA                           (1 << PPLL_OFF_ENA_SHIFT)     // Bit 2
#define MPLL_OFF_ENA                           (1 << MPLL_OFF_ENA_SHIFT)     // Bit 1
#define SACG_ENA                               (1 << SACG_ENA_SHIFT)         // Bit 0

//CSR SAPMCTLbit definition for SKX
#define MC1_PKGC_DIG_VOLTAGE_REDUCTION_DISABLE_SHIFT  27        // Bit 27, SKX
#define MC0_PKGC_DIG_VOLTAGE_REDUCTION_DISABLE_SHIFT  26        // Bit 26, SKX
#define MC1_PKGC_IO_VOLTAGE_REDUCTION_DISABLE_SHIFT   25        // Bit 25, SKX
#define MC0_PKGC_IO_VOLTAGE_REDUCTION_DISABLE_SHIFT   24        // Bit 24, SKX
#define MEM_PLL_OFF_EN_SHIFT                    22              // Bit 22,23, SKX
#define KTI_PLL_OFF_EN_SHIFT                    19              // Bit 19,20, SKX
#define IIO_PLL_OFF_EN_SHIFT                    16              // Bit 16,17,18, SKX
#define MC1_PKGC_CLK_GATE_DISABLE_SHIFT          7              // Bit 7, SKX
#define MC0_PKGC_CLK_GATE_DISABLE_SHIFT          6              // Bit 6, SKX
#define KTI23_PKGC_CLK_GATE_DISABLE_SHIFT        4              // Bit 4, SKX
#define KTI01_PKGC_CLK_GATE_DISABLE_SHIFT        3              // Bit 3, SKX
#define IIO012_PKGC_CLK_GATE_DISABLE_SHIFT       0              // Bit 0,1,2, SKX
#define MC1_PKGC_DIG_VOLTAGE_REDUCTION_DISABLE   (1 << MC1_PKGC_DIG_VOLTAGE_REDUCTION_DISABLE_SHIFT)   // Bit 27, SKX
#define MC0_PKGC_DIG_VOLTAGE_REDUCTION_DISABLE   (1 << MC0_PKGC_DIG_VOLTAGE_REDUCTION_DISABLE_SHIFT)   // Bit 26, SKX
#define MC1_PKGC_IO_VOLTAGE_REDUCTION_DISABLE    (1 << MC1_PKGC_IO_VOLTAGE_REDUCTION_DISABLE_SHIFT)    // Bit 25, SKX
#define MC0_PKGC_IO_VOLTAGE_REDUCTION_DISABLE    (1 << MC0_PKGC_IO_VOLTAGE_REDUCTION_DISABLE_SHIFT)    // Bit 24, SKX
#define MEM_PLL_OFF_EN                           (3 << MEM_PLL_OFF_EN_SHIFT)                           // Bit 22,23, SKX
#define KTI_PLL_OFF_EN                           (3 << KTI_PLL_OFF_EN_SHIFT)                           // Bit 19,20, SKX
#define IIO_PLL_OFF_EN                           (7 << IIO_PLL_OFF_EN_SHIFT)                           // Bit 16,17,18, SKX
#define MC1_PKGC_CLK_GATE_DISABLE                (1 << MC1_PKGC_CLK_GATE_DISABLE_SHIFT)                // Bit 7, SKX
#define MC0_PKGC_CLK_GATE_DISABLE                (1 << MC0_PKGC_CLK_GATE_DISABLE_SHIFT)                // Bit 6, SKX
#define KTI23_PKGC_CLK_GATE_DISABLE              (1 << KTI23_PKGC_CLK_GATE_DISABLE_SHIFT)              // Bit 4, SKX
#define KTI01_PKGC_CLK_GATE_DISABLE              (1 << KTI01_PKGC_CLK_GATE_DISABLE_SHIFT)              // Bit 3, SKX
#define IIO012_PKGC_CLK_GATE_DISABLE             (7 << IIO012_PKGC_CLK_GATE_DISABLE_SHIFT)             // Bit 0,1,2, SKX
#define SAPMCTL_MASK  (IIO012_PKGC_CLK_GATE_DISABLE + KTI01_PKGC_CLK_GATE_DISABLE + KTI23_PKGC_CLK_GATE_DISABLE + MC0_PKGC_CLK_GATE_DISABLE \
        + MC1_PKGC_CLK_GATE_DISABLE + IIO_PLL_OFF_EN + KTI_PLL_OFF_EN + MEM_PLL_OFF_EN + SETVID_DECAY_DISABLE + SAPMCTL_LOCK \
        + MC1_PKGC_IO_VOLTAGE_REDUCTION_DISABLE + MC0_PKGC_IO_VOLTAGE_REDUCTION_DISABLE + MC1_PKGC_DIG_VOLTAGE_REDUCTION_DISABLE + MC0_PKGC_DIG_VOLTAGE_REDUCTION_DISABLE)

//Config TDP
#define CONFIG_TDP_LEVEL                         (3 << CONFIG_TDP_LEVEL_SHIFT)
#define CONFIG_TDP_LEVEL_SHIFT                   1    // Bit [2:1]
#define CONFIG_TDP_SHIFT                         0    // Bit 0

// MSR 0x1FC
#define MSR_POWER_CTL                           0x1FC
#define PCH_NEG_DISABLE                         (1 << 30)
#define PCH_NEG_DISABLE_SHIFT                   30
#define LTR_SW_DISABLE                          (1 << 29)  //LTR_IIO_DISABLE
#define LTR_SW_DISABLE_SHIFT                    29
#define PROCHOT_LOCK                            (1 << 27)
#define PROCHOT_LOCK_SHIFT                      27
#define PROCHOT_RESPONSE                        (1 << 26)
#define PROCHOT_RESPONSE_SHIFT                  26
#define PWR_PERF_TUNING_CFG_MODE                (1 << 25)
#define PWR_PERF_TUNING_CFG_MODE_SHIFT          25
#define PWR_PERF_TUNING_ENABLE_DYN_SWITCHING    (1 << 24)
#define PWR_PERF_TUNING_ENABLE_DYN_SHIFT        24
#define PWR_PERF_TUNING_DISABLE_EEP_CTRL        (1 << 23)
#define PWR_PERF_TUNING_DISABLE_EEP_SHIFT       23
#define PWR_PERF_TUNING_DISABLE_SAPM_CTRL       (1 << 22)
#define PWR_PERF_TUNING_DISABLE_SAPM_SHIFT      22
#define DIS_PROCHOT_OUT                         (1 << 21)
#define DIS_PROCHOT_OUT_SHIFT                   21
#define EE_TURBO_DISABLE                        (1 << 19)
#define EE_TURBO_DISABLE_SHIFT                  19
#define ENERGY_EFFICIENT_PSTATE_ENABLE          (1 << 18)
#define ENERGY_EFFICIENT_PSTATE_ENABLE_SHIFT    18
#define PHOLD_SR_DISABLE                        (1 << 17)
#define PHOLD_SR_DISABLE_SHIFT                  17
#define PHOLD_CST_PREVENTION_INIT               (1 << 16)
#define PHOLD_CST_PREVENTION_INIT_SHIFT         16
#define FAST_BRK_INT_EN                         (1 << 4)
#define FAST_BRK_INT_EN_SHIFT                   4
#define FAST_BRK_SNP_EN                         (1 << 3)
#define FAST_BRK_SNP_EN_SHIFT                   3
#define SAPM_IMC_C2_POLICY_EN                   (1 << 2)
#define SAPM_IMC_C2_POLICY_SHIFT                2
#define C1E_ENABLE                              (1 << 1)
#define C1E_ENABLE_SHIFT                        1
#define ENABLE_BIDIR_PROCHOT_EN                 (1 << 0)
#define ENABLE_BIDIR_PROCHOT_EN_SHIFT           0
#define POWER_CTL_MASK                          (PCH_NEG_DISABLE + LTR_SW_DISABLE + PWR_PERF_TUNING_CFG_MODE + \
        PWR_PERF_TUNING_ENABLE_DYN_SWITCHING + PWR_PERF_TUNING_DISABLE_EEP_CTRL + \
        PWR_PERF_TUNING_DISABLE_SAPM_CTRL + DIS_PROCHOT_OUT + ENABLE_BIDIR_PROCHOT_EN + C1E_ENABLE)

// PRIMARY_PLANE_CURRENT_CONFIG_CONTROL  0x601
#define PSI3_CODE_SHIFT               27  // (Bits 61:59 actully) we operate on a 32 bit register
#define PSI3_THSHLD_SHIFT             20  // (Bits 58:52 actully) we operate on a 32 bit register
#define PSI2_CODE_SHIFT               17  // (Bits 51:49 actully) we operate on a 32 bit register
#define PSI2_THSHLD_SHIFT             10  // (Bits 48:42 actully) we operate on a 32 bit register
#define PSI1_CODE_SHIFT               7   // (Bits 41:39 actully) we operate on a 32 bit register
#define PSI1_THSHLD_SHIFT             0   // (Bits 38:32 actully) we operate on a 32 bit register
#define PPCCC_LOCK_SHIFT              31
#define CURRENT_LIMIT_SHIFT           0
#define PSI3_CODE                     (0x7 << PSI3_CODE_SHIFT)  // (Bits 61:59 actully) we operate on a 32 bit register
#define PSI3_THSHLD                   (0x7f << PSI3_THSHLD_SHIFT)  // (Bits 58:52 actully) we operate on a 32 bit register
#define PSI2_CODE                     (0x7 << PSI2_CODE_SHIFT)  // (Bits 51:49 actully) we operate on a 32 bit register
#define PSI2_THSHLD                   (0x7f << PSI2_THSHLD_SHIFT)  // (Bits 48:42 actully) we operate on a 32 bit register
#define PSI1_CODE                     (0x7 << PSI1_CODE_SHIFT)  // (Bits 41:39 actully) we operate on a 32 bit register
#define PSI1_THSHLD                   (0x7f << PSI1_THSHLD_SHIFT)  // (Bits 38:32 actully) we operate on a 32 bit register
#define PPCCC_LOCK                    (1 << PPCCC_LOCK_SHIFT)
#define CURRENT_LIMIT                 (0x1fff << CURRENT_LIMIT_SHIFT)

#define B_PCPS_DISABLE                (1 << 25) // Bit 25

// MSR_C_STATE_LATENCY_CONTROL_0       0x60a, 60b, 60c
#define VALID_SHIFT                    15
#define MULTIPLIER_SHIFT               10
#define VALUE_SHIFT                    0

// MSR_TURBO_POWER_LIMIT  0x610
// CSR_TURBO_POWER_LIMIT
#define POWER_LIMIT_ENABLE_SHIFT    15
#define POWER_LIMIT_ENABLE          (1 << POWER_LIMIT_ENABLE_SHIFT)   // Used as Bit 15 and Bit 47
#define PKG_CLMP_LIM_SHIFT          16
#define PKG_CLMP_LIM                (1 <<  PKG_CLMP_LIM_SHIFT)        // used as Bit 16 and Bit48
#define POWER_LIMIT_MASK            (0x7FFF)                          // Bits 14:0 and 46:32
#define POWER_LIMIT_1_TIME_SHIFT    17
#define POWER_LIMIT_1_TIME_MASK     (0xFE0000)                        // Bits 23:17
#define POWER_LIMIT_LOCK_SHIFT      31
#define POWER_LIMIT_LOCK            (1 << POWER_LIMIT_LOCK_SHIFT)     // Bit 63

//  MSR_ENERGY_PERF_BIAS_CONFIG             0xA01
#define AVG_TIME_Window                     (0xff << AVG_TIME_Window_SHIFT)           // Bits 31:24
#define PO_TOTAL_TIME_THSHLD_LOW            (0x3f << PO_TOTAL_TIME_THSHLD_LOW_SHIFT)  // Bits 23:18
#define PO_TOTAL_TIME_THSHLD_HIGH           (0x3f << PO_TOTAL_TIME_THSHLD_HIGH_SHIFT) // Bis 17:12
#define ALT_ENERGY_PERF_BIAS                (0xf << ALT_ENERGY_PERF_BIAS_SHIFT)       // Bits 6:3
#define WORKLD_CONFIG                       (0x7 << WORKLD_CONFIG_SHIFT)              // Bits 2:0
#define AVG_TIME_Window_SHIFT               24                                        // Bits 31:24
#define PO_TOTAL_TIME_THSHLD_LOW_SHIFT      18                                        // Bits 23:18
#define PO_TOTAL_TIME_THSHLD_HIGH_SHIFT     12                                        // Bis 17:12
#define ALT_ENERGY_PERF_BIAS_SHIFT          3                                         // Bits 6:3
#define WORKLD_CONFIG_SHIFT                 0                                         // Bits 2:0
#define ENERGY_PERF_BIAS_CTRL_MASK   (AVG_TIME_Window + PO_TOTAL_TIME_THSHLD_LOW + PO_TOTAL_TIME_THSHLD_HIGH +  WORKLD_CONFIG + ALT_ENERGY_PERF_BIAS)

//
// Cross over Mode
//
#define XOVER_MODE_2TO2                     1
#define XOVER_MODE_1TO1                     2

// HWPM features
#define HWP_ENABLE                          0x01
#define ACC_ENABLE                          0x02

// SPT workarounds
#define SPT_WA_ENABLE                       0x03

// ratio in Performance Control MSR (MSR_IA32_PERF_CTL)
#define B_IA32_PERF_CTRLP_STATE_TARGET  (0x7F << 8)

#pragma pack(1)

/**************************
 Processor Power Management Data structs
***************************/

typedef struct _PPM_FROM_PPMINFO_HOB {
  UINT8       NumberOfSockets;                      // # of populated sockets in the system
  UINT8       SocketNumber;                         // which socket
  UINT32      SocketPresentBitMap;                  // bitmap for present CPU packages/nodes
  UINT8       IioBusNumber[MAX_SOCKET];             // Bus# for IIO, indexed by CPU Socket/Node ID
  UINT8       UncoreBusNumber[MAX_SOCKET];          // Bus# for Uncore, indexed by CPU Socket/Node ID
  UINT32      mmCfgBase;
  UINT8       DdrXoverMode;                         // DDR 2.2 Mode
  UINT32      OutKtiPerLinkL1En[MAX_SOCKET];        // output kti link enabled status for PM
  UINT32      OutPciePerLinkL1En[MAX_SOCKET];       // output PCIe (IIO) link enabled status for PM
  UINT8       KtiPortCnt;                           // num KTI ports residing on each Socket
  UINT8       ProcessorPowerUnit[MAX_SOCKET];       //
  UINT8       ProcessorTimeUnit[MAX_SOCKET];        //
  UINT16      PackageTdp[MAX_SOCKET];               // Package TDP
  UINT32      CapId4;                               //CapId CSR value
  UINT32      CpuType;                              // CpuType
  UINT8       CpuStepping;                          // CpuStepping
  UINT32      mNumOfBitShift;                       // # Bits to shift right APIC ID to get next level APIC ID
  UINTN       NumberOfProcessors;                   // number of active threads
  BOOLEAN     EistCap;                              // EIST Capability
  UINT8       Bios_Reset_Cpl_Phase;
  UINT8        HwpmSupport;                         //HWPM support flag
}PPM_FROM_PPMINFO_HOB;

typedef struct {
  UINT8           Major;         // Major Vesion
  UINT8           Minor;         // Minor Vesion
  UINT8           Rev;           // Release Version
  UINT8           Build;         //
} PPM_VERSION;

typedef union _MSR_REGISTER {
  UINT64  Qword;

  struct _DWORDS {
    UINT32  Low;
    UINT32  High;
  } Dwords;

  struct _BYTES {
    UINT8 FirstByte;
    UINT8 SecondByte;
    UINT8 ThirdByte;
    UINT8 FouthByte;
    UINT8 FifthByte;
    UINT8 SixthByte;
    UINT8 SeventhByte;
    UINT8 EighthByte;
  } Bytes;

} MSR_REGISTER;

typedef struct {
  BOOLEAN   C1e;

  UINT32          PkgCstEntryCriteriaMaskKti[MAX_SOCKET];
  UINT32          PkgCstEntryCriteriaMaskPcie[MAX_SOCKET];
  MSR_REGISTER    LatencyCtrl[NUM_CST_LAT_MSR];

} CSTATE_STRUCT;

typedef struct {

  BOOLEAN     EistEnabled;            // option to enable GV3
  UINT8       ConfigTdpLevel;         // Config TDP Level
  UINT16      CurrentPackageTdp;      // Package TDP
  UINT8       PcpsCtrl;

} PSTATE_STRUCT;

typedef struct {
  BOOLEAN   Enable;
  UINT32    Voltage;
  UINT16    RatioLimit[MAX_CORE];
} XE_STRUCT;

typedef struct {
  UINT8    RatioLimitRatio[8];
  UINT8    RatioLimitRatioMask[8];
  UINT8    RatioLimitCores[8];
  UINT8    RatioLimitCoresMask[8];
} TURBO_RATIO_LIMIT_RATIO_CORES;

typedef struct {
  UINT8   HWPMEnable;
  UINT8   HWPMNative;
  UINT8   HWPMOOB;
  UINT8   HWPMInterrupt;
  UINT8   AutoCState;
  UINT8   EPPEnable;
  UINT8   EPPProfile;
  UINT8   APSrocketing;
  UINT8   Scalability;
  UINT8   ProcessorRaplPrioritization;
  UINT8   OutofBandAlternateEPB;
  UINT8   PbfEnabled;
  UINT8   ConfigurePbf;
  UINT64  PbfHighPriCoreMap[MAX_SOCKET];    // PBF High Priority Cores Bitmap
  UINT8   PbfP1HighRatio[MAX_SOCKET];       // PBF P1_High Ratio
  UINT8   PbfP1LowRatio[MAX_SOCKET];        // PBF P1_Low Ratio
} HWPM_STRUCT;

typedef struct {

  UINT8           FastRaplDutyCycle;
  UINT8           FuseTjMaxOffset;

  UINT8           OverclockingLock;
  UINT8           AvxIccpLevel;
  UINT32          AdvPwrMgtCtlFlags;

  MSR_REGISTER    PowerCtl;
  MSR_REGISTER    TurboPowerLimit;
  MSR_REGISTER    PP0CurrentCfg;
  MSR_REGISTER    PerfBiasConfig;

  UINT32          ProchotRatio;
  UINT32          PmaxConfig;
  UINT32          SapmCtl[MAX_SOCKET];
  UINT32          PerPLimitCtl;
  UINT32          C2C3TT;
  UINT32          DynamicPerPowerCtl;
  UINT32          PcieIltrOvrd;

  CSTATE_STRUCT   Cst;
  PSTATE_STRUCT   Pst;
  XE_STRUCT       Xe;
  HWPM_STRUCT     Hwpm;
  TURBO_RATIO_LIMIT_RATIO_CORES TurboRatioLimitRatioCores;

  UINT8           TCCActivationOffset;
  UINT8           IsOppSrefEn;

} EFI_PPM_STRUCT;


typedef struct {

  PPM_VERSION                   Version;

  EFI_CPU_CSR_ACCESS_PROTOCOL   *CpuCsrAccess;

  PPM_FROM_PPMINFO_HOB          *Info;

  EFI_PPM_STRUCT                *Setup;

  UINTN                         ProcessorNumber;

} EFI_CPU_PM_STRUCT;


#pragma pack()

VOID
PStateTransition (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
InitializeCpuPPMLib (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
PpmSetBiosInitDone (
   EFI_CPU_PM_STRUCT   *ppm
  );

VOID
PpmSetCsrLockBit (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
PpmSetMsrLockBit (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
ProgramCSRTurboPowerLimit (
  EFI_CPU_PM_STRUCT *ppm
  );

VOID
ProgramCsrDynamicPerfPowerCtl (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
ProgramCsrSapmCtl (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
ProgramCsrSwLtrOvrd (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
ProgramCsrPkgCstEntryCriteriaMask(
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
ProgramCsrResponseRatioCfg(
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
EFIAPI
SetupPCIEPkgCstEntryCriteria (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
ProgramCsrPerfPlimitControl (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
ProgramCsrPmaxConfig (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
ProgramMsrPowerCtl (
  EFI_CPU_PM_STRUCT   *ppm,
  UINTN               ProcessorNumber
  );

VOID
ProgramMsrTurboPowerLimit (
  EFI_CPU_PM_STRUCT   *ppm,
  UINTN               ProcessorNumber
  );

VOID
ProgramEnergyPerfBiasConfigMsr (
  EFI_CPU_PM_STRUCT   *ppm,
  UINTN               ProcessorNumber
  );

VOID
ProgMsrPriPlaneCurtCfgCtrL (
  EFI_CPU_PM_STRUCT   *ppm,
  UINTN               ProcessorNumber
  );

VOID
ProgramMsrTurboRatioLimit (
  EFI_CPU_PM_STRUCT *ppm,
  UINTN             ProcessorNumber
  );

VOID
ProgramMsrTemperatureTarget (
  EFI_CPU_PM_STRUCT *ppm,
  UINTN             ProcessorNumber
  );

VOID
GetMsrTemperatureTarget (
  EFI_CPU_PM_STRUCT *ppm
  );

VOID
ProgramMsrMiscPwrMgmt (
  EFI_CPU_PM_STRUCT *ppm,
  UINTN             ProcessorNumber
  );

VOID
Program_Bios_Reset_Cpl (
  EFI_CPU_PM_STRUCT *ppm
  );

VOID
ProgramB2PFastRaplDutyCycle (
  EFI_CPU_PM_STRUCT *ppm
  );

UINT8
EFIAPI
GetHwpmSupport (
  EFI_CPU_PM_STRUCT *ppm
  );

VOID
HWPMInterfaceReg (
  EFI_CPU_PM_STRUCT   *ppm,
  UINTN               ProcessorNumber
  );

VOID
EnableAutonomousCStateControl (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
EFIAPI
EnableHwpLvtThermalInterrupt(
  EFI_CPU_PM_STRUCT   *ppm,
  UINTN               ProcessorNumber
  );

VOID
EFIAPI
EnableHwpFeatures(
  EFI_CPU_PM_STRUCT   *ppm,
  UINTN               ProcessorNumber
  );

VOID
ProgramB2PPcuMiscConfig (
  EFI_CPU_PM_STRUCT *ppm
  );

VOID
ProgramB2PHWPMMiscConfig (
  EFI_CPU_PM_STRUCT *ppm
  );

VOID
ProgramMsrCLatency (
  EFI_CPU_PM_STRUCT *ppm,
  UINTN             ProcessorNumber
  );

VOID
ProgramB2PDynamicL1 (
  EFI_CPU_PM_STRUCT *ppm
  );

VOID
PpmSetMsrCstConfigCtrlLockBit (
  EFI_CPU_PM_STRUCT *ppm
  );

VOID
ProgramB2PForceUncoreAndMeshRatio (
  EFI_CPU_PM_STRUCT *ppm
  );

VOID
ProgramB2PMiscWorkaroundEnable (
  EFI_CPU_PM_STRUCT *ppm
  );

#endif
