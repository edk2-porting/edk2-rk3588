/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _UNCORE_COMMON_INCLUDES_H_
#define _UNCORE_COMMON_INCLUDES_H_

#ifndef V_INTEL_VID
#define V_INTEL_VID               0x8086
#endif

#ifndef MAX_CORE
#define MAX_CORE                       28           // Maximum cores per CPU (SKX)
#define MAX_CORE_BITMASK               0xFFFFFFF      // for SKX CPU
#endif

#define MAX_PROCESSOR_CORE_RATIO               100
#define DEFAULT_PROCESSOR_CORE_RATIO           23
#define MAX_SCRAMBLE_SEED_LOW                  65535
#define MAX_SCRAMBLE_SEED_HIGH                 65535
#define ITURBO_MODE_TRADITIONAL                0
#define ITURBO_MODE_POWER_OPTIMIZED            4
#define MAX_PROCESSOR_TSEG                     5


// Define the different SKX package Sku's
#define SKX_PHY_CHOP_HCC 0x2
#define SKX_PHY_CHOP_MCC 0x1
#define SKX_PHY_CHOP_LCC 0x0

//
// Defines used for variables to be range checked before consumption.
// For MiniBIOS support, these are also defined in
// so any changes here need to be updated in these files as well.
// If you change this, please also update CPU_FAMILY_XXX in \Library\ProcMemInit\Include\CpuPciAccess.h, Library\ProcessorStartup\Ia32\ProcessorStartupPlatform.inc
#ifndef CPU_FAMILY_HSX
#define CPU_FAMILY_HSX     0x306F      // Haswell CPU
#endif
#ifndef CPU_FAMILY_SKX
#define CPU_FAMILY_SKX     0x5065      // Skylake  CPU
#endif

// SKX   REV_ID  SiliconID  SteppingID  CPUID       ChopType
// A0    0       0          0           0x50650     3
// A1    0       1          0           0x50650     3
// A2    2       2          1           0x50651     3
// B0    3       3          2           0x50652     3
// L0    4       4          2           0x50652     2
// B1    5       5          3           0x50653     3
// H0    6       6          4           0x50654     3 (xcc)
// M0    7       6          4           0x50654     2 (hcc)
// U0    8       6          4           0x50654     0 (lcc)
//
// xy_REV_SKX is the logical ID for BIOS to distinguish the Si
// A0 and A1 still keep to 0
//
#ifndef A0_REV
#define A0_REV      0x00
#endif
#ifndef A0_REV_SKX
#define A0_REV_SKX  A0_REV
#endif
#ifndef A1_REV_SKX
#define A1_REV_SKX  A0_REV
#endif
#ifndef A2_REV_SKX
#define A2_REV_SKX  0x02
#endif
#ifndef B0_REV_SKX
#define B0_REV_SKX  0x03
#endif
#ifndef L0_REV_SKX
#define L0_REV_SKX  0x04
#endif
#ifndef B1_REV_SKX
#define B1_REV_SKX  0x05
#endif
#ifndef H0_REV_SKX
#define H0_REV_SKX  0x06
#endif
#ifndef M0_REV_SKX
#define M0_REV_SKX  0x07
#endif
#ifndef U0_REV_SKX
#define U0_REV_SKX  0x08
#endif

#ifndef C0_REV_SKX
#define C0_REV_SKX  0x09
#endif

//
// Xy_CPU_STEP is from CPUID
//
#ifndef A0_CPU_STEP
#define A0_CPU_STEP  0x00
#endif
#ifndef A1_CPU_STEP
#define A1_CPU_STEP  A0_CPU_STEP
#endif
#ifndef A2_CPU_STEP
#define A2_CPU_STEP  0x01
#endif
#ifndef B0_CPU_STEP
#define B0_CPU_STEP  0x02
#endif
#ifndef L0_CPU_STEP
#define L0_CPU_STEP  0x02
#endif
#ifndef B1_CPU_STEP
#define B1_CPU_STEP  0x03
#endif
#ifndef H0_CPU_STEP
#define H0_CPU_STEP  0x04
#endif
#ifndef M0_CPU_STEP
#define M0_CPU_STEP  0x04
#endif
#ifndef U0_CPU_STEP
#define U0_CPU_STEP  0x04
#endif

#ifndef C0_CPU_STEP
#define C0_CPU_STEP  0x05
#endif

#include "MaxSocket.h"

#define MAX_THREAD                      2
#define MAX_DIE                         1
#define MAX_CPU_NUM                     (MAX_THREAD * MAX_CORE * MAX_DIE * MAX_SOCKET)

#ifndef MAX_HA
#define MAX_HA                          2
#endif


// If you change this, please also update MAX_KTI_PORTS in \Library\ProcMemInit\Platform\Include\PlatformHost.h
#ifndef MAX_KTI_PORTS
#define MAX_KTI_PORTS                 3                     // Max KTI ports supported
#endif

// If you change this, please also update MAX_IMC in Library\ProcMemInit\Include\MemHostChip.h
// If you change this, please also update MAX_IMC in Library\ProcMemInit\Platform\Include\MemDefaults.h
#ifndef MAX_IMC
#define MAX_IMC                       2                     // Maximum memory controllers per socket
#endif

// If you change this, please also update MAX_MC_CH in Library\ProcMemInit\Include\MemHostChip.h
// If you change this, please also update MAX_IMC in Library\ProcMemInit\Platform\Include\MemDefaults.h
#ifndef MAX_MC_CH
#define MAX_MC_CH                     3                     // Max number of channels per MC (3 for EP)
#endif


// If you change this, please also update MAX_CH in Library\ProcMemInit\Include\MemHostChip.h
// If you change this, please also update MAX_IMC in Library\ProcMemInit\Platform\Include\MemDefaults.h
#ifndef MAX_CH
#define MAX_CH                        ((MAX_IMC)*(MAX_MC_CH))     // Max channels per socket (worst case EP * EX combination = 16)
#endif

// If you change this, please also update MAX_DIMM in Library\ProcMemInit\Include\MemHostChip.h
#ifndef MAX_DIMM
#define MAX_DIMM                      2                       // Max DIMM per channel
#endif

// If you change this, please also update MC_MAX_NODE in Library\ProcMemInit\Include\MemHostChip.h
#ifndef MC_MAX_NODE
#define MC_MAX_NODE                   (MAX_SOCKET * MAX_IMC)  // Max number of memory nodes
#endif

// If you change this, please also update MAX_SYS_CH in Library\ProcMemInit\Include\MemHostChip.h
// If you change this, please also update MAX_IMC in Library\ProcMemInit\Platform\Include\MemDefaults.h
#ifndef MAX_SYS_CH
#define MAX_SYS_CH                    (MAX_CH * MAX_SOCKET)   // Max channels in the system
#endif
#define MAX_SYS_DIMM                  MAX_SYS_CH * MAX_DIMM

#define MAX_CRS_ENTRIES_PER_NODE      8                       // Max number of ranges allowed on a memory node
#ifndef NUMBER_PORTS_PER_SOCKET
#define NUMBER_PORTS_PER_SOCKET       21
#endif

#ifndef CB3_DEVICES_PER_SOCKET
#define CB3_DEVICES_PER_SOCKET       8
#endif

#ifndef TOTAL_CB3_DEVICES
#if MAX_SOCKET > 4
#define TOTAL_CB3_DEVICES             64   // Todo Check SKX CB3 devices (IOAT_TOTAL_FUNCS * MAX_SOCKET). Note: this covers up to 8S.
#else
#define TOTAL_CB3_DEVICES             32   // Todo Check SKX CB3 devices.
#endif
#endif

#ifndef MaxIIO
#define MaxIIO                        MAX_SOCKET
#endif

#ifndef MAX_TOTAL_PORTS
#if MAX_SOCKET > 4
#define MAX_TOTAL_PORTS               168   //NUMBER_PORTS_PER_SOCKET * MaxIIO. As now, treats setup S0-S3 = S4_S7 as optimal
#else
#define MAX_TOTAL_PORTS               84   //NUMBER_PORTS_PER_SOCKET * MaxIIO
#endif
#endif

#ifndef TOTAL_IIO_STACKS
#if MAX_SOCKET > 4
#define TOTAL_IIO_STACKS              48   // MAX_SOCKET * MAX_IIO_STACK. Not reflect architecture but only sysHost structure!
#else
#define TOTAL_IIO_STACKS              24   // MAX_SOCKET * MAX_IIO_STACK
#endif
#endif

#ifndef NUMBER_NTB_PORTS_PER_SOCKET
#define NUMBER_NTB_PORTS_PER_SOCKET   3
#endif

#ifndef MAX_NTB_PORTS

#if MAX_SOCKET > 4
#define MAX_NTB_PORTS                 24   // NUMBER_NTB_PORTS_PER_SOCKET * MAX_SOCKET
#else
#define MAX_NTB_PORTS                 12   // NUMBER_NTB_PORTS_PER_SOCKET * MAX_SOCKET
#endif
#endif

#ifndef VMD_STACK_PER_SOCKET
#define VMD_STACK_PER_SOCKET          3
#endif

#ifndef VMD_PORT_PER_STACK
#define VMD_PORT_PER_STACK            4
#endif

#ifndef VMD_PORTS_PER_SOCKET
#define VMD_PORTS_PER_SOCKET          12
#endif

#if MAX_SOCKET > 4
#ifndef MAX_VMD_PORTS
#define MAX_VMD_PORTS                 96   // VMD_PORTS_PER_SOCKET * MAX_SOCKET
#endif

#ifndef MAX_VMD_STACKS
#define MAX_VMD_STACKS                24   // VMD_STACK_PER_SOCKET * MAX_SOCKET*/
#endif
#else
#ifndef MAX_VMD_PORTS
#define MAX_VMD_PORTS                 48   // VMD_PORTS_PER_SOCKET * MAX_SOCKET
#endif

#ifndef MAX_VMD_STACKS
#define MAX_VMD_STACKS                12   // VMD_STACK_PER_SOCKET * MAX_SOCKET*/
#endif
#endif

#ifndef NUM_DEVHIDE_REGS
#define NUM_DEVHIDE_REGS             8
#endif

#ifndef MAX_DEVHIDE_REGS
#define MAX_DEVHIDE_REGS             (MAX_IIO_STACK * NUM_DEVHIDE_REGS)
#endif

#ifndef MAX_DEVHIDE_REGS_PER_SYSTEM

#if MAX_SOCKET > 4
#define MAX_DEVHIDE_REGS_PER_SYSTEM  384 //(MAX_DEVHIDE_REGS * MAX_SOCKET)
#else
#define MAX_DEVHIDE_REGS_PER_SYSTEM  192 //(MAX_DEVHIDE_REGS * MAX_SOCKET)
#endif

#endif


#if MAX_SOCKET > 4
#define MAX_TOTAL_CORE_HIDE           32   //(MAX_SOCKET * VARIABLE_FUNC3_ELEMENTS)
#else
#define MAX_TOTAL_CORE_HIDE           16   //(MAX_SOCKET * VARIABLE_FUNC3_ELEMENTS)
#endif

#define MAX_IOU_PORT_DEVICES          4

//
// Resource Ratio units used by Uncore Init PEIM.
//
// Assumption: these values must be 2^N; Otherwise the algorithm in OemProcMemInit.c
// needs to be adjusted: the "if (Alignment > KTI_SOCKET_BUS_RATIO_UNIT) {" should be
// removed when computing adjusted "Length".
//
#define KTI_SOCKET_BUS_RATIO_UNIT              0x20
#define KTI_SOCKET_IO_RATIO_UNIT               0x2000
#define KTI_SOCKET_MMIOL_RATIO_UNIT            0x4000000
//
// Maximum alignment bit allowed for Socket PCI Resources.
//
#define KTI_SOCKET_MAX_BUS_ALIGNMENT           0x8
#define KTI_SOCKET_MAX_IO_ALIGNMENT            0x10
#define KTI_SOCKET_MAX_MMIOL_ALIGNMENT         0x20

#ifndef MAX_IIO_STACK
#define IIO_CSTACK          0
#define IIO_PSTACK0         1
#define IIO_PSTACK1         2
#define IIO_PSTACK2         3
#define IIO_PSTACK3         4
#define IIO_PSTACK4         5
#define MAX_IIO_STACK       6
#endif

#ifndef UNDEFINED_RAS
#define HEDT_RAS            0x0
#define BASIC_RAS_AX        0x1   // Valid in A stepping only.
#define S1WS_RAS            0x2   // Not valid in A stepping.
#define CORE_RAS            0x3   // Not valid in A stepping .
#define STANDARD_RAS        0x4
#define FPGA_RAS            0x5   // Not valid in A stepping.
#define ADVANCED_RAS        0x6
#define UNDEFINED_RAS       0x7
#endif

//
// Defines used for variables to be range checked before consumption.
//
#define MAX_CAS_LATENCY                        32
#define MAX_TRP_LATENCY                        32
#define MAX_TRCD_LATENCY                       32
#define MAX_TRRD_LATENCY                       255
#define MAX_TWTR_LATENCY                       255
#define MAX_TRAS_LATENCY                       63
#define MAX_TRTP_LATENCY                       255
#define MAX_TWR_LATENCY                        50
#define MAX_TFAW_LATENCY                       63
#define MAX_TCWL_LATENCY                       31
#define MAX_TRC_LATENCY                        255
#define MAX_REFRESH_RATE                       32767
#define MAX_TRFC_LATENCY                       1023
#define MAX_MC_BGF_THRESHOLD                   15

//
// ACPI table information used to initialize tables.
//
#define EFI_ACPI_OEM_ID           "INTEL "
#define EFI_ACPI_OEM_TABLE_ID     0x59454C525550  // "PURLEY"
#define EFI_ACPI_OEM_REVISION     0x00000002
#define EFI_ACPI_CREATOR_ID       0x4C544E49          // "INTL"
#define EFI_ACPI_CREATOR_REVISION 0x20091013          // Oct 13 2009

#endif
