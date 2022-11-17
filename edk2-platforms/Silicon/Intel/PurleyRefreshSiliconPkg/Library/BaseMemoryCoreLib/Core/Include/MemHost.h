/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _memhost_h
#define _memhost_h
#include "DataTypes.h"
#include "PlatformHost.h"
#include "SysRegs.h"
#include "MemRegs.h"
#include "MemDefaults.h"
#include "MrcCommonTypes.h"
#include "MemHostChipCommon.h"
#include "KtiSi.h"

#define MAX_DIMMTABLEINDEX       (MAX_CH * MAX_DIMM)


#define MAXFIELDVAL(bitfield)     (bitfield = 0xffffffff)

//EFI_GUID definition locations for different BDAT/BSSA cases

#include <PiPei.h>

// Debug Build code
// These should be disabled for all normal builds and only enable on demand for debugging
//#define DEBUG_TURNAROUNDS         1
#define DEBUG_PERFORMANCE_STATS   1
//#define DEBUG_RDRAND              1
//#define DEBUG_SENSEAMP            1

#ifdef DEBUG_PERFORMANCE_STATS
#define  MAX_NOZONE     20
#endif  // DEBUG_PERFORMANCE_STATS

#define PGT_TIMER_ENABLE  1
#define PGT_TIMER_DISABLE 0

//
// DDR3 frequencies 800 - 2666
// DDR4 frequencies 1333 - 4200
//
#define MAX_SUP_FREQ        28                  // 26 frequencies are supported (800, 1067, 1333, 1600, 1867, 2133, 2400, 2666, 2933,
                                                //                               3200, 3400, 3467, 3600, 3733, 3800, 4000, 4200, 4266, 4400)




///
/// External signal names
///
typedef enum {
  RAS_N, CAS_N, WE_N,
  BA0, BA1, BA2,
  A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17,
  CS0_N, CS1_N, CS2_N, CS3_N, CS4_N, CS5_N, CS6_N, CS7_N, CS8_N, CS9_N,
  CKE0, CKE1, CKE2, CKE3, CKE4, CKE5,
  ODT0, ODT1, ODT2, ODT3, ODT4, ODT5,
  PAR, ALERT_N,
  BG0, BG1, ACT_N, C0, C1, C2,
  CK0, CK1, CK2, CK3, CK4, CK5,
  FNV_GOOD_PARITY, DESELECT, PRECHARGE, GNT,   // these are actually commands as opposed to signals
  gsmCsnDelim = INT32_MAX
} GSM_CSN;

typedef struct {
   INT16 left;
   INT16 right;
} SIGNAL_EYE;

#define MAX_PHASE           2                   // MAX_PHASE

//
// Common Core dummy defines
//

#ifndef MAX_MC_CH
#define MAX_MC_CH                2               // Max channels per MC
#endif
#ifndef MAX_CLUSTERS
#define MAX_CLUSTERS             1               // Maximum number of clusters supported
#endif

#ifndef MAX_EDC
#define MAX_EDC                  1               // Maximum number of EDC supported
#endif

#define CATCHALL_TIMEOUT    100000               // 100 ms

#pragma pack(push, 1)

extern const UINT16 rankSize[MAX_TECH];          ///< Rank size in 64 MB units
extern const UINT16 rankSizeDDR4[MAX_TECH];      ///< Rank size in 64 MB units


#define ALL_DRAMS 0xFF  // Indicates to write to all DRAMs when in PDA mode
#define ALL_DATABUFFERS 0xFF // Indicates to write to all Buffers in PBA Mode

typedef enum {
INVALID_BUS,
SMBUS,
EMRS,
CPGC,
SAD,
} BUS_TYPE;

#ifdef SERIAL_DBG_MSG
#define MemDebugPrint(dbgInfo)  debugPrintMem dbgInfo
#define MspDebugPrint(dbgInfo)  debugPrintMsp dbgInfo
#define MmrcDebugPrint(dbgInfo)
#define OutputExtendedCheckpoint(dbgInfo)
#else
#define MemDebugPrint(dbgInfo)
#define MspDebugPrint(dbgInfo)
#define MmrcDebugPrint(dbgInfo)
#define OutputExtendedCheckpoint(dbgInfo) OutputCheckpoint dbgInfo
#endif


typedef enum
{
  MRC_PF_NULL,                // All policy flags turned off.
  MRC_PF_COLD         = BIT0, // Execute MRC function on cold reset.
  MRC_PF_FAST         = BIT1, // Execute MRC function on cold reset when S3 data is present.
  MRC_PF_WARM         = BIT2, // Execute MRC function on warm reset.
  MRC_PF_S3           = BIT3, // Execute MRC function on S3 exit.
  //MRC_PF_FULL_MRC     = BIT4,   // Execute MRC function when in Full MRC mode.
  //MRC_PF_MINI_MRC     = BIT5,   // Execute MRC function when in Mini-MRC mode.
  MRC_PF_ALL          = 0xF   // All policy flags turned off.
} PFSelectorType;

typedef enum
{
  MRC_MP_NULL,            // All policy flags turned off
  MRC_MP_SERIAL     = BIT0, // Execute function when in serial mode
  MRC_MP_PARALLEL   = BIT1, // Execute function when in parallel mode
  MRC_MP_LOOP       = BIT2, // Execute function for each socket when in serial mode
  MRC_MP_BOTH       = MRC_MP_SERIAL | MRC_MP_PARALLEL, // Execute function in both modes
  MRC_MP_BOTH_LOOP  = MRC_MP_SERIAL | MRC_MP_PARALLEL | MRC_MP_LOOP, // Execute function in both modes and loop
} MPSelectorType;

//
// TRR defines
//
#define PTRR_MODE   BIT0
#define TRR_MODE_A  BIT1
#define TRR_IMMUNE  BIT2
#ifdef TRR_MODE_B_SUPPORT
#define TRR_MODE_B  BIT3
#endif //TRR_MODE_B_SUPPORT

typedef struct {
  UINT8  stackPresentBitmap[MAX_SOCKET]; ///< bitmap of present stacks per socket
  UINT8  StackBus[MAX_SOCKET][MAX_IIO_STACK];  ///< Bus of each stack
  UINT8  SocketFirstBus[MAX_SOCKET];
  UINT8  Socket10nmUboxBus0[MAX_SOCKET];         //10nm CPU use only
  UINT8  SocketLastBus[MAX_SOCKET];
  UINT8  segmentSocket[MAX_SOCKET];
  UINT8  cpuType;
  UINT8  stepping;
  UINT32 socketPresentBitMap;
  UINT32 FpgaPresentBitMap;
  UINT32 mmCfgBase;
  UINT8  maxCh;
  UINT8  maxIMC;
  UINT8  numChPerMC;
  UINT8  imcEnabled[MAX_SOCKET][MAX_IMC];
  UINT8  mcId[MAX_SOCKET][MAX_CH];
  CPU_CSR_ACCESS_VAR_CHIP                 ///< Chip hook to enable CPU_CSR_ACCESS_VAR fields
} CPU_CSR_ACCESS_VAR;

#pragma pack(pop)

///
/// (MPT_MT - MemeoryPowerTraining_MarginType)param type for power training steps
///
typedef enum {
  GetMargin   = 0,
  TerMargin   = 1,
  BerMargin   = 2
} MPT_MT;

///
/// (MPT_PT - MemeoryPowerTraining_ParamType)param type for power training steps
///
typedef enum {
  PerChPerByte   = 0,
  PerRank        = 1,
  PerStrobe      = 2,
  PerCh          = 3,
  PerMC          = 4
} MPT_PT;

///
/// (MPT_P - MemeoryPowerTraining_Param)param  for power training steps
///
typedef enum {
  traindramron    = 0,
  trainmcodt      = 1,
  trainnontgtodt  = 2,
  trainrttwr      = 3,
  trainmcron      = 4,
  traintxeq       = 5,
  trainimode      = 6,
  trainctle       = 7,
  traintcocomp    = 8,
  traindramrxeq   = 9,
} MPT_P;

#define IMC0 0
#define IMC1 1

//
// PPR Status
//
#define PPR_STS_SUCCESS         0x00
#define PPR_STS_ADDR_VALID      0x01
#define PPR_STS_FAILED          0x02

#define DRAM_UNKNOWN    0xFF

#pragma pack(push, 1)
//
// -----------------------------------------------------------------------------

//
// NVRAM structures for S3 state
//

#define MAX_CMD_CSR 16
#define MAX_SIDE    2

//
// -----------------------------------------------------------------------------
//
// ddrChannelSetup STRUCT 4t    ;  Channel setup structure declaration
//
// enabled  BYTE ?   ; Channel enable switch:
//                   ; 0 = channel disabled
//                   ; 1 = channel enabled
//
// options  BYTE ?   ; Bit-mapped options:
//
// numDimmSlots BYTE ? ; Number of Dimm slots per channel
//                   ; Valid options are 1, 2 or 3
//                   ; MAX_DIMM is defined in mrcplatform.h.  This option can be no larger than MAX_DIMM.
//                   ; It overrides MAX_DIMM when it is smaller.
//
// ddrChannelSetup    ENDS
//
// -----------------------------------------------------------------------------
//

//
// -----------------------------------------------------------------------------
//
// Node bit-mapped options
//
// ddrSocketSetup STRUCT 4t    ;  Socket setup structure declaration
//
// enabled  BYTE ?   ; imc enable switch:
//                   ; 0 = imc disabled
//                   ; 1 = imc enabled
//
// options  BYTE ?   ; Bit-mapped options per socket:
//
// vrefDefaultValue BYTE ? ; Default DCP value per socket for DIMM Vref = Vddq/2
//
// vrefDcp smbDevice <>    ; Defines override of DCP SMBus device and address
//                         ; compId = DCP_ISL9072X or DCP_AD5247
//                         ; strapAddress
//                         ; busSegment
//
// ddrSocketSetup    ENDS
//
// -----------------------------------------------------------------------------
//

///
/// PPR DRAM Address
///
typedef struct {
  UINT8     dimm;
  UINT8     rank;
  UINT8     subRank;
  UINT32    dramMask;
  UINT8     bank;
  UINT32    row;
} PPR_ADDR;

// HIGH_ADDR_EN enables extention of the MMIO hole to force memory to high address region
#define HIGH_ADDR_EN        BIT0
#define CR_MIXED_SKU        BIT2  //used to enable(1)- halt on mixed sku discovery and disable(0) - warn on mixed sku discovery

//#pragma pack(pop)

///
/// Sub-boot state internal to MRC (8-15 are definable).  The 2 main boot types and paths through QPIRC/MRC - NormalBoot and S3Resume.
/// Within NormalBoot and S3Resume, the sub-boot type can be cold, warm, fast warm, fast cold, and ADR resume.  These are populated
/// at the beginning of MRC so they are not applicable for QPIRC.
///
typedef enum SubBootMode
{
  ColdBoot     = 8,   // Normal path through MRC with full mem detection, init, training, etc.
  WarmBoot     = 9,   // Warm boot path through MRC. Some functionality can be skipped for speed.
  WarmBootFast = 10,  // Fast warm boot path uses the NVRAM structure to skip as much MRC
                            // code as possible to try to get through MRC fast. Should be as close
                            //    as possible to the S3 flow.
  ColdBootFast = 11,  // Fast cold boot path uses the NVRAM structure to skip as much MRC
                            //    code as possible on a cold boot.
  AdrResume    = 12,  // ADR flow can skip most of MRC (i.e. take the S3 path) for DIMMs that
                      //    are in self-refresh.  But the DIMMs that are not in self-refresh
                      //    must go through more of MRC.
  Reserved13 = 13
} SubBootMode;

#define MAX_ADV_MT_LOG  16

#define MEM_CHIP_POLICY_DEF(x)          host->var.mem.memChipPolicy.x
#define MEM_CHIP_POLICY_VALUE(host, x)  host->var.mem.memChipPolicy.x
#define CHIP_FUNC_CALL(host, x)         x

#pragma pack(pop)

#endif // _memhost_h
