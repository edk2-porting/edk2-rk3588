/** @file
  Register names for CPU registers

  <b>Conventions</b>
  - Definitions beginning with "MSR_" are MSRs
  - Definitions beginning with "R_" are registers
  - Definitions beginning with "B_" are bits within registers
  - Definitions beginning with "V_" are meaningful values of bits within the registers
  - Definitions beginning with "S_" are register sizes
  - Definitions beginning with "N_" are the bit position

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _CPU_REGS_H_
#define _CPU_REGS_H_

///
/// Arch-specific MSR defines in SDM, but not defined for SKL
/// @{

#define MSR_PLATFORM_INFO                                             0x000000CE
#define N_PLATFORM_INFO_MIN_RATIO                                     40
#define B_PLATFORM_INFO_RATIO_MASK                                    0xFF
#define N_PLATFORM_INFO_MAX_RATIO                                     8
#define B_MSR_PLATFORM_INFO_BIOSGUARD_AVAIL                           BIT35
#define N_MSR_PLATFORM_INFO_CONFIG_TDP_NUM_LEVELS_OFFSET              33
#define V_CONFIG_TDP_NUM_LEVELS_MASK                                  (BIT34 | BIT33)
#define B_PLATFORM_INFO_TDC_TDP_LIMIT                                 BIT29
#define N_PLATFORM_INFO_RATIO_LIMIT                                   28
#define B_PLATFORM_INFO_RATIO_LIMIT                                   BIT28
#define B_PLATFORM_INFO_SAMPLE_PART                                   BIT27
#define B_PLATFORM_INFO_SMM_SAVE_CONTROL                              BIT16
#define N_PLATFORM_INFO_PROG_TCC_ACTIVATION_OFFSET                    30
#define B_PLATFORM_INFO_PROG_TCC_ACTIVATION_OFFSET                    BIT30
#define B_PLATFORM_INFO_TIMED_MWAIT_SUPPORTED                         BIT37
#define B_PLATFORM_INFO_EDRAM_EN                                      BIT57

//
// MSR_BROADWELL_PKG_CST_CONFIG_CONTROL: related defines
//
#define B_TIMED_MWAIT_ENABLE                                          BIT31 ///< @todo Remove when bitfield definition is available.
#define V_CSTATE_LIMIT_C1                                             0x01
#define V_CSTATE_LIMIT_C3                                             0x02
#define V_CSTATE_LIMIT_C6                                             0x03
#define V_CSTATE_LIMIT_C7                                             0x04
#define V_CSTATE_LIMIT_C7S                                            0x05
#define V_CSTATE_LIMIT_C8                                             0x06
#define V_CSTATE_LIMIT_C9                                             0x07
#define V_CSTATE_LIMIT_C10                                            0x08

#define MSR_PMG_IO_CAPTURE_BASE                                       0x000000E4
#define B_MSR_PMG_CST_RANGE                                           (BIT18 | BIT17 | BIT16)
#define V_IO_CAPT_LVL2                                                (0x0 << 16)   ///< C3
#define V_IO_CAPT_LVL3                                                (0x1 << 16)   ///< C6
#define V_IO_CAPT_LVL4                                                (0x2 << 16)   ///< C7
#define V_IO_CAPT_LVL5                                                (0x3 << 16)   ///< C8
#define V_IO_CAPT_LVL6                                                (0x4 << 16)   ///< C9
#define V_IO_CAPT_LVL7                                                (0x5 << 16)   ///< C10
#define V_IO_CAPT_LVL2_BASE_ADDR_MASK                                 0xFFFF

#define MSR_TEMPERATURE_TARGET                                        0x000001A2
#define B_MSR_TEMPERATURE_TARGET_TCC_OFFSET_LOCK                      BIT31
#define N_MSR_TEMPERATURE_TARGET_TCC_OFFSET_LIMIT                     24
#define V_MSR_TEMPERATURE_TARGET_TCC_ACTIVATION_OFFSET_MASK           0x3F
#define N_MSR_TEMPERATURE_TARGET_TCC_ACTIVATION_TEMPERATURE_OFFSET    (16)
#define B_MSR_TEMPERATURE_TARGET_TCC_ACTIVATION_TEMPERATURE_MASK      (0xFF << 16)
#define N_MSR_TEMPERATURE_TARGET_FAN_TEMP_TARGET_OFFSET               8
#define B_MSR_TEMPERATURE_TARGET_FAN_TEMP_TARGET_OFFSET               (0xFF << 8)
#define B_MSR_TEMPERATURE_TARGET_TCC_OFFSET_TIME_WINDOW               (0x7F)
#define B_MSR_TEMPERATURE_TARGET_TCC_OFFSET_MASK                      0xFF
#define B_MSR_TEMPERATURE_TARGET_TCC_OFFSET_CLAMP_BIT                 BIT7


#define MSR_TURBO_RATIO_LIMIT                                         0x000001AD
#define N_MSR_TURBO_RATIO_LIMIT_1C                                    0
#define B_MSR_TURBO_RATIO_LIMIT_1C                                    (0xFFULL << 0)
#define N_MSR_TURBO_RATIO_LIMIT_2C                                    8
#define B_MSR_TURBO_RATIO_LIMIT_2C                                    (0xFFULL << 8)
#define N_MSR_TURBO_RATIO_LIMIT_3C                                    16
#define B_MSR_TURBO_RATIO_LIMIT_3C                                    (0xFFULL << 16)
#define N_MSR_TURBO_RATIO_LIMIT_4C                                    24
#define B_MSR_TURBO_RATIO_LIMIT_4C                                    (0xFFULL << 24)
#define N_MSR_TURBO_RATIO_LIMIT_5C                                    32
#define B_MSR_TURBO_RATIO_LIMIT_5C                                    (0xFFULL << 32)
#define N_MSR_TURBO_RATIO_LIMIT_6C                                    40
#define B_MSR_TURBO_RATIO_LIMIT_6C                                    (0xFFULL << 40)
#define N_MSR_TURBO_RATIO_LIMIT_7C                                    48
#define B_MSR_TURBO_RATIO_LIMIT_7C                                    (0xFFULL << 48)
#define N_MSR_TURBO_RATIO_LIMIT_8C                                    56
#define B_MSR_TURBO_RATIO_LIMIT_8C                                    (0xFFULL << 56)

#define MSR_IA32_FEATURE_CONFIG                                       0x0000013C
#define B_IA32_FEATURE_CONFIG_AES_DIS                                 BIT1
#define B_IA32_FEATURE_CONFIG_LOCK                                    BIT0


//
// MSRs for SMM State Save Register
//
#define MSR_SMM_MCA_CAP                                               0x0000017D
#define B_TARGETED_SMI                                                BIT56
#define N_TARGETED_SMI                                                56
#define B_SMM_CPU_SVRSTR                                              BIT57
#define N_SMM_CPU_SVRSTR                                              57
#define B_SMM_CODE_ACCESS_CHK                                         BIT58
#define N_SMM_CODE_ACCESS_CHK                                         58
#define B_LONG_FLOW_INDICATION                                        BIT59
#define N_LONG_FLOW_INDICATION                                        59
#define MSR_SMM_FEATURE_CONTROL                                       0x000004E0
#define B_SMM_FEATURE_CONTROL_LOCK                                    BIT0
#define B_SMM_CPU_SAVE_EN                                             BIT1
#define B_SMM_CODE_CHK_EN                                             BIT2

/// @}


///
/// Bit defines for MSRs defined in UefiCpuPkg/Include/Register/ArchitecturalMsr.h.
/// @{

//
// Number of fixed MTRRs
//
#define V_FIXED_MTRR_NUMBER                                           11


//
// Number of variable MTRRs
//
#define V_MAXIMUM_VARIABLE_MTRR_NUMBER                                10

//
// Bit defines for MSR_IA32_MTRR_DEF_TYPE
//
#define B_CACHE_MTRR_VALID                                            BIT11
#define B_CACHE_FIXED_MTRR_VALID                                      BIT10

//
// Bit defines for MSR_IA32_DEBUG_INTERFACE
//
#define B_DEBUG_INTERFACE_ENABLE                                      BIT0
#define B_DEBUG_INTERFACE_LOCK                                        BIT30
#define B_DEBUG_INTERFACE_DEBUG_STATUS                                BIT31

/// @}

///
/// Other defines
///

//
// Local APIC defines
//
#define APIC_REGISTER_LOCAL_ID_OFFSET         0x00000020
#define APIC_REGISTER_APIC_VERSION_OFFSET     0x00000030
#define APIC_REGISTER_SPURIOUS_VECTOR_OFFSET  0x000000F0
#define APIC_REGISTER_ICR_LOW_OFFSET          0x00000300
#define APIC_REGISTER_ICR_HIGH_OFFSET         0x00000310
#define APIC_REGISTER_LINT0_VECTOR_OFFSET     0x00000350
#define APIC_REGISTER_LINT1_VECTOR_OFFSET     0x00000360

#define BROADCAST_MODE_SPECIFY_CPU            0x00
#define BROADCAST_MODE_ALL_INCLUDING_SELF     0x01
#define BROADCAST_MODE_ALL_EXCLUDING_SELF     0x02

#ifndef DELIVERY_MODE_FIXED
#define DELIVERY_MODE_FIXED           0x0
#endif
#ifndef DELIVERY_MODE_LOWEST_PRIORITY
#define DELIVERY_MODE_LOWEST_PRIORITY 0x1
#endif
#ifndef DELIVERY_MODE_SMI
#define DELIVERY_MODE_SMI             0x2
#endif
#ifndef DELIVERY_MODE_REMOTE_READ
#define DELIVERY_MODE_REMOTE_READ     0x3
#endif
#ifndef DELIVERY_MODE_NMI
#define DELIVERY_MODE_NMI             0x4
#endif
#ifndef DELIVERY_MODE_INIT
#define DELIVERY_MODE_INIT            0x5
#endif
#ifndef DELIVERY_MODE_SIPI
#define DELIVERY_MODE_SIPI            0x6
#endif
#ifndef DELIVERY_MODE_MAX
#define DELIVERY_MODE_MAX             0x7
#endif

#ifndef TRIGGER_MODE_EDGE
#define TRIGGER_MODE_EDGE             0x00
#endif
#ifndef TRIGGER_MODE_LEVEL
#define TRIGGER_MODE_LEVEL            0x01
#endif

#ifndef CPU_FEATURE_DISABLE
#define CPU_FEATURE_DISABLE  0
#endif
#ifndef CPU_FEATURE_ENABLE
#define CPU_FEATURE_ENABLE   1
#endif

#define CACHE_UNCACHEABLE               0
#define CACHE_WRITECOMBINING            1
#define CACHE_WRITETHROUGH              4
#define CACHE_WRITEPROTECTED            5
#define CACHE_WRITEBACK                 6


//
// Processor Definitions
//
#define CPUID_FULL_STEPPING                      0x0000000F
#define CPUID_FULL_FAMILY_MODEL                  0x0FFF0FF0
#define CPUID_FULL_FAMILY_MODEL_STEPPING         0x0FFF0FFF
#define CPUID_FULL_FAMILY_MODEL_SKYLAKE_ULT_ULX  0x000406E0
#define CPUID_FULL_FAMILY_MODEL_SKYLAKE_DT_HALO  0x000506E0
#define CPUID_FULL_FAMILY_MODEL_KABYLAKE_ULT_ULX 0x000806E0
#define CPUID_FULL_FAMILY_MODEL_KABYLAKE_DT_HALO 0x000906E0

#ifndef STALL_ONE_MICRO_SECOND
#define STALL_ONE_MICRO_SECOND 1
#endif
#ifndef STALL_ONE_MILLI_SECOND
#define STALL_ONE_MILLI_SECOND 1000
#endif

#define BITS(x) (1 << (x))

/**
Notes :
  1.  Bit position always starts at 0.
  2.  Following macros are applicable only for Word aligned integers.
**/
#define BIT(Pos, Value)               (1 << (Pos) & (Value))
#define BITRANGE(From, Width, Value)  (((Value) >> (From)) & ((1 << (Width)) - 1))

///
/// Enums for CPU Family IDs
///
typedef enum {
  EnumCpuSklUltUlx    = CPUID_FULL_FAMILY_MODEL_SKYLAKE_ULT_ULX,
  EnumCpuSklDtHalo    = CPUID_FULL_FAMILY_MODEL_SKYLAKE_DT_HALO,
  EnumCpuKblUltUlxG0  = EnumCpuSklUltUlx,
  EnumCpuKblDtHaloA0  = EnumCpuSklDtHalo,
  EnumCpuKblUltUlx    = CPUID_FULL_FAMILY_MODEL_KABYLAKE_ULT_ULX,
  EnumCpuKblDtHalo    = CPUID_FULL_FAMILY_MODEL_KABYLAKE_DT_HALO,
  EnumCpuMax          = CPUID_FULL_FAMILY_MODEL
} CPU_FAMILY;

///
/// Enums for CPU Stepping IDs
///
typedef enum {
  ///
  /// Skylake ULX/ULT Steppings
  ///
  EnumSklB0         = 1,
  EnumSklJ0         = 2,
  EnumSklC0         = 2,
  EnumSklK0         = 3,
  EnumSklD0         = 3,
  EnumSklMaxUltUlxStep = EnumSklD0,

  ///
  /// Kabylake ULX/ULT Steppings
  ///
  EnumKblG0         = 8,
  EnumKblH0         = 9,
  EnumKblJ0         = 9,
  EnumKblY0         = 0xA,
  EnumKblMaxUltUlxStep = EnumKblY0,

  ///
  /// Skylake DT/Halo Steppings
  ///
  EnumSklP0         = 0,
  EnumSklQ0         = 1,
  EnumSklM0         = 2,
  EnumSklR0         = 3,
  EnumSklS0         = 3,
  EnumSklN0         = 3,
  EnumSklMaxDtHaloStep = EnumSklN0,

  ///
  /// Kabylake DT/Halo Steppings
  ///
  EnumKblA0         = 8,
  EnumKblB0         = 9,
  EnumKblS0         = 9,
  EnumKblM0         = 9,
  EnumKblN0         = 0xA,
  EnumKblMaxDtHaloStep = EnumKblN0,

  ///
  /// Max Stepping
  ///
  EnumCpuSteppingMax  = CPUID_FULL_STEPPING
} CPU_STEPPING;

///
/// Enums for CPU SKU IDs
///
typedef enum {
  EnumCpuUlt        = 0,
  EnumCpuTrad,
  EnumCpuUlx,
  EnumCpuHalo,
  EnumCpuUnknown
} CPU_SKU;

///
/// Enums for CPU Generation
///
typedef enum {
  EnumSklCpu  = 0,
  EnumKblCpu,
  EnumCpuUnknownGeneration
} CPU_GENERATION;
#endif
