/** @file
  MemHost.h

  @copyright
  Copyright 2006 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _memhost_h
#define _memhost_h

#include <Ppi/MemoryPolicyPpi.h>
#include "DataTypes.h"
#include "PlatformHost.h"
#include "MemRegs.h"
#include "MemDefaults.h"
#include "NGNDimmPlatformCfgData.h"
#include "MrcCommonTypes.h"
#include "MemHostChipCommon.h"
#include <MemCommon.h>
#include <PiPei.h>
#include <Memory/Ddr4SpdRegisters.h>

#define RESERVED_2    2

typedef struct TrainingStepDoneStruct {
  UINT8 DramRxEq  : 1;
  UINT8 HostFlyBy : 1;
  UINT8 TxVref    : 1;
  UINT8 DqSwzDone : 1;
  UINT8 Reserved  : 4;
} TRAINING_STEP_DONE_STRUCT;

#define CADB_MUX_MAX  4

#define MAX_BITS_IN_BYTE      8                     // BITS per byte
#define MAX_BITS_IN_DWORD     32                    // BITS per dword
#define BITS_PER_NIBBLE       4
#define MAX_NIBBLES_PER_BYTE  2
#define INDEX_NIBBLE0         0
#define INDEX_NIBBLE1         1

//
// DDR4 DB has 4 DFE coeff taps
//
#define DB_DFE_TAP                   4

#pragma pack(push, 1)

///
/// Rand per channel information
//
typedef struct RankCh {
  UINT8             dimm;                             ///< DIMM this rank belongs to
  UINT8             rank;                             ///< Rank on the DIMM (0-3)
  UINT8             Roundtrip;
  UINT8             IOLatency;
} CHANNEL_RANKS;

typedef enum {
INVALID_BUS,
SMBUS,
EMRS,
CPGC,
SAD,
} BUS_TYPE;

//
// -----------------------------------------------------------------------------
///
/// Common DRAM timings
///
struct comTime {
  UINT8   nCL;
  UINT8   nWR;
  UINT8   nRCD;
  UINT8   nAL;
  UINT8   nRP;
  UINT8   nRC;
  UINT8   nWTR;
  UINT8   nWTR_L;
  UINT8   nRAS;
  UINT8   nRTP;
  UINT8   nFAW;
  UINT8   nRRD;
  UINT8   nRRD_L;
  UINT8   nWL;
  UINT16  nRFC;
  UINT32  tCL;
  UINT16  tRCD;
  UINT16  tRP;
  UINT16  tRC;
  UINT16  tRFC;
  UINT16  tRRD;
  UINT16  tRRD_L;
  UINT16  tRAS;
  UINT16  tCCD;
  UINT16  tCCD_WR;
  UINT16  tCCD_L;
  UINT16  tCCD_WR_L;
  UINT64  casSup;
  UINT8   casSupRange;
}; //struct comTime

//
// Only applicable for x16 devices where 2 strobes are within a single DRAM device
//
typedef struct _X16_MAPPING {
  UINT8   PairLogical[MAX_STROBE/2];    ///< Identifies logical strobe within the same DRAM device
  BOOLEAN IsLow[MAX_STROBE/2];          ///< TRUE:  represents the current logical strobe is connected to DQSL
                                        ///< FALSE: represents the current logical strobe is connected to DQSU
} X16_MAPPING;

//
// Packed bit field structure for storing MR1 RX EQ bits
//
typedef struct {
  UINT32  Strobe0 : 3; //bits 0...2
  UINT32  Strobe1 : 3; //bits 3...5
  UINT32  Strobe2 : 3; //bits 6...8
  UINT32  Strobe3 : 3; //bits 9...11
  UINT32  Strobe4 : 3; //bits 12..14
  UINT32  Strobe5 : 3; //bits 15..17
  UINT32  Strobe6 : 3; //bits 18..20
  UINT32  Strobe7 : 3; //bits 21..23
  UINT32  Strobe8 : 3; //bits 24..26
  UINT32  Rsvd    : 5; //bits 27..31
} Mr1RxEqStruct;

//
// NVRAM structures for S3 state
//

///
/// DIMM rank info
/// List indexed by rank number
///
typedef struct ddrRank {
  UINT8   enabled;                    ///< 0 = disabled,  1 = enabled
  UINT8   rankIndex;
  UINT8   phyRank;                    ///< Physical Rank #(3:0)
  UINT8   logicalRank;                ///< Logical Rank number (0 - 7)
  UINT8   ckIndex;                    ///< Index to the clock for this rank
  UINT8   ctlIndex;                   ///< Index to the control group for this rank
  UINT8   CKEIndex;
  UINT8   ODTIndex;
  UINT8   CSIndex;
  UINT8   devTagInfo;
  UINT16  RttWr;
  UINT16  RttPrk;
  UINT16  RttNom;
  UINT16  rankSize;                   ///< Units of 64 MB
  UINT16  remSize;                    ///< Units of 64 MB
  UINT16  NVrankSize;                   ///< Units of 64 MB

  UINT8   cmdLeft;
  UINT8   cmdRight;
  UINT8   cmdLow;
  UINT8   cmdHigh;

  UINT8   rxDQLeftSt;
  UINT8   rxDQRightSt;
  UINT8   txDQLeftSt;
  UINT8   txDQRightSt;
  UINT16  OutDrvImpCtrl;
  UINT8   DqSwzDdr5[SUB_CH][20];
  UINT8   faultyParts[MAX_STROBE];
#ifdef LRDIMM_SUPPORT
  UINT8   lrbufRxVref[MAX_STROBE];      ///< Actual value of backside RxVref
  UINT8   lrBuf_FxBC4x5x[MAX_STROBE];
  UINT8   lrBuf_FxBC8x9x[MAX_STROBE];
#endif
  UINT8   lrBuf_FxBC2x3x[MAX_STROBE];
  UINT8   lrBuf_FxBCAxBx[MAX_STROBE];
  UINT8   lrBuf_FxBCCxEx[MAX_STROBE/2];
  UINT8   lrBuf_FxBCDxFx[MAX_STROBE/2];
  UINT32  lrbufTxVref[MAX_STROBE];      ///< Actual value of backside TxVref
  //
  // MR1 per strobe Rx Eq value structure with 9 elements per structure
  // therefore you need MAX_STROBE/9 structures to store all elements
  //
  Mr1RxEqStruct Mr1RxEqSettings[MAX_STROBE/9];

} DDR_RANK_STRUCT;  //struct ddrRank

typedef enum {
  DIMM_RANK_MAP_OUT_UNKNOWN = 0,
  DIMM_RANK_MAP_OUT_MEM_DECODE,
  DIMM_RANK_MAP_OUT_POP_POR_VIOLATION,
  DIMM_RANK_MAP_OUT_RANK_DISABLED,
  DIMM_RANK_MAP_OUT_ADVMEMTEST_FAILURE,
  DIMM_RANK_MAP_OUT_MAX
} DIMM_RANK_MAP_OUT_REASON;

///
/// DIMM information stored in NVRAM
//
typedef struct dimmNvram {
  UINT8                         dimmPresent;        ///< 1 = DIMM present in this slot
  UINT8                         mailboxReady;
  UINT16                        lrbufswizzle;
  INT32                         minTCK;             ///< minimum tCK for this DIMM (SPD_MIN_TCK)
  UINT8                         ftbTCK;             ///< fine offset for tCK
  UINT32                        tCL;
  UINT16                        tRCD;
  UINT16                        tRP;
  UINT8                         ftbTRP;             ///< DDR4 fine offset for tRP
  UINT8                         ftbTRCD;            ///< DDR4 fine offset for tRCD
  UINT8                         ftbTAA;             ///< DDR4 fine offset for tAA
  UINT8                         mapOut[MAX_RANK_DIMM]; ///< 1 = User requested rank made non-Present
  UINT8                         numRanks;           ///< Number of logical ranks on this DIMM
  UINT8                         numDramRanks;       ///< Number of physical DRAM ranks on this DIMM
  UINT8                         techIndex;          ///< Index into DIMM technology table
  UINT8                         aepTechIndex;       ///< Index into NVM DIMM technology table
  UINT8                         fmcType;            ///< Far Memory Controller Type
  UINT8                         fmcRev;             ///< Far Memory Controller Rev Type
  SPD_AEP_MOD_REVISION_STRUCT   FmcModRev; ///< Far Memory Controller Module Rev and Type
  UINT8                         NvmStepping;        ///< NVM Device Stepping
  UINT8                         NvmDevDensity;      ///< NVM DEV DENSITY
  UINT8                         NvmDevType;         ///< NVM DEV TYPE
  UINT8                         SPDRawCard;         ///< Raw Card Number
  UINT8                         DimmRevType;        ///< RCD Revision
  UINT8                         SPDOtherOptFeatures;///< SDRAM Other Optional features
  UINT8                         SPDAddrMapp;        ///< Address Mapping from Edge connector to DRAM
  UINT8                         SPDRegRev;          ///< Register Revision
  UINT8                         SPDODCtl;           ///< Register Output Drive Strength for Control
  UINT8                         SPDODCk;            ///< Register Output Drive Strength for Clock
#ifdef LRDIMM_SUPPORT
  UINT16                        SPDLrbufVen;          ///< LRDIMM Data Buffer Vendor ID
  UINT8                         SPDLrbufDramVrefdqR0; ///< DRAM VrefDQ for Package Rank 0
  UINT8                         SPDLrbufDramVrefdqR1; ///< DRAM VrefDQ for Package Rank 1
  UINT8                         SPDLrbufDramVrefdqR2; ///< DRAM VrefDQ for Package Rank 2
  UINT8                         SPDLrbufDramVrefdqR3; ///< DRAM VrefDQ for Package Rank 3
  UINT8                         SPDLrbufDbVrefdq; ///< LR Data Buffer VrefDQ for DRAM Interface
  UINT8                         SPDLrbufDbDsRttLe1866; ///< LR Data Buffer MDQ Drive Strength and RTT for data rate <= 1866
  UINT8                         SPDLrbufDbDsRttGt1866Le2400; ///< LR Data Buffer MDQ Drive Strength and RTT for data rate > 1866 and <= 2400
  UINT8                         SPDLrbufDbDsRttGt2400Le3200; ///< LR Data Buffer MDQ Drive Strength and RTT for data rate > 2400 and <= 3200
  UINT8                         SPDLrbufDramDs; ///< LR Buffer DRAM Drive Strength (for data rates <1866, 1866 < data rate < 2400, and 2400 < data rate < 3200)
  UINT8                         SPDLrbufDramOdtWrNomLe1866; ///< LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate <= 1866
  UINT8                         SPDLrbufDramOdtWrNomGt1866Le2400; ///< LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate > 1866 and <= 2400
  UINT8                         SPDLrbufDramOdtWrNomGt2400Le3200; ///< LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate > 2400 and <= 3200
  UINT8                         SPDLrbufDramOdtParkLe1866; ///< LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate <= 1866
  UINT8                         SPDLrbufDramOdtParkGt1866Le2400; ///< LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate > 1866 and <= 2400
  UINT8                         SPDLrbufDramOdtParkGt2400Le3200; ///< LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate > 2400 and <= 3200
  UINT8                         rcLrFunc;
  UINT8                         lrBuf_BC00;
  UINT8                         lrBuf_BC01;
  UINT8                         lrBuf_BC02;
  UINT8                         lrBuf_BC03;
  UINT8                         lrBuf_BC04;
  UINT8                         lrBuf_BC05;
  UINT8                         lrBuf_BC1x;
  UINT8                         LrBuf_DFECoef[MAX_BITS_IN_BYTE][DB_DFE_TAP][MAX_STROBE/2];  // JEDEC F3BCCx-Fx coeffcient. 8 DQ x 4 taps x 9 DB
  UINT8                         Dfe_F2BCEX; // JEDEC DB02DFE DQ selection and global enable
#endif    // LRDIMM_SUPPORT

  UINT8                         XMPOrg;             ///< XMP organization and configuration
  UINT8                         XMPRev;
  UINT32                        XMPChecksum;

  UINT8                         x4Present;          ///< Set if this is a x4 rank
  UINT8                         keyByte;
  UINT8                         keyByte2;           ///< Logical module type (for MRC compat)
  UINT8                         DcpmmPresent;       ///< Is an NVM DIMM present?
  BOOLEAN                       NvmDimmDisable;     ///< 1 = NVMDimm present but got disabled in this slot
  UINT8                         actKeyByte2;        ///< Actual module type reported by SPD
  UINT8                         SPDModuleOrg;       ///< Logical number of DRAM ranks and device width
  UINT8                         actSPDModuleOrg;    ///< Actual number of DRAM ranks and device width
  UINT8                         dramIOWidth;        ///< The encoded value of DRAM IO width
  UINT8                         dramIOWidthSecondary;    ///< The encoded value of IO width for secondary SDRAM
  BOOLEAN                       rankMix;            ///< Used to indicate if the target DIMM is asymmetric
  UINT8                         SPDDeviceType;      ///< Primary SDRAM Device Type
  UINT8                         SPDSecondaryDeviceType; ///< Secondary SDRAM Device Type
  UINT8                         numBankAddressBits; ///< Number of bank address bits per bank group
  UINT8                         numBankAddressBitsSecondary; ///< Number of bank address bits per bank group of Secondary SDRAM
  UINT8                         numBankGroupBits;   ///< Number of bank group bits
  UINT8                         numBankGroupBitsSecondary; ///< Number of bank group bits of Secondary SDRAM
  UINT8                         sdramCapacity;      ///< Encoded SDRAM capacity based on DDR4 format
  UINT8                         sdramCapacitySecondary;      ///< Encoded SDRAM capacity based on DDR4 format for Secondary SDRAM
  UINT8                         numRowBits;         ///< Number of row address bits
  UINT8                         numRowBitsSecondary;    ///< Number of row address bits of Secondary SDRAM
  UINT8                         numColBits;         ///< Number of column address bits
  UINT8                         numColBitsSecondary;    ///< Number of column address bits of Secondary SDRAM
  UINT8                         dieCount;           ///< Number of SDRAM dies per package for symmetric DIMMs or Primary SDRAM dies per package for asymmetric DIMMs
  UINT8                         dieCountSecondary;  ///< Number of Secondary DRAM dies per package for asymmetric DIMMs
  UINT8                         cidBitMap;          ///< SubRanks per chip select per dimm of DDR4 3DS and non3ds_lrdimm
  UINT8                         lrRankMult;         ///< Rank multiplication factor
  UINT8                         SPDMemBusWidth;     ///< Width of the Primary bus and extension
  UINT8                         dimmTs;             ///< Module Thermal Sensor
  UINT8                         dimmAttrib;         ///< Module attributes
  UINT8                         mtbDiv;             ///< medium time base divisor (SPD_MTB_DIV)
  UINT8                         SPDftb;             ///< fine time base (SPD_FTB)
  UINT32                        dimmMemTech;        ///< Used to program DIMMMTR
  UINT16                        SPDRegVen;          ///< Register Vendor ID
  UINT16                        SPDMMfgId;          ///< Module Mfg Id from SPD
  UINT8                         SPDMMfLoc;          ///< Module Mfg Location from SPD
  UINT16                        SPDModDate;         ///< Module Manufacturing Date
  UINT16                        SPDDramMfgId;       ///< DRAM Mfg Id
  UINT8                         SPDDramRev;         ///< DRAM Rev Id
  UINT8                         SPDModSN[SPD_MODULE_SERIAL];               ///< Module Serial Number
  UINT8                         SPDModPartDDR4[SPD_MODULE_PART_DDR4];      ///< Module Part Number DDR4
  UINT8                         SPDOptionalFeature; ///< DDR4 optional feature
  UINT8                         AitDramPoll;
  BOOLEAN                       NvDimmNPresent;     ///< JEDEC NVDIMM-N Type Memory Present
  UINT16                        NvDimmStatus;
  // Bit            Description
  //  0             NVDIMM controller failure
  //  1             NVDIMM restore failed
  //  2             NVDIMM restore retry
  //  3             NVDIMM backup failed
  //  4             NVDIMM erase failed
  //  5             NVDIMM erase retry
  //  6             NVDIMM arm failed
  //  7             No backup energy source detected
  //  8             Backup energy source charge failed
  //  9             NVDIMM uncorrectable memory error
  // 10             NVDIMM correctable memory memory error threshold
  // 11-15          Reserved

  UINT8                         lrbufRid;           ///< LR Buffer Revision ID (from buffer not SPD)
  UINT8                         lrbufGen;           ///< LR Buffer Gen
  UINT8                         SPDIntelSN[5];      ///< Intel DIMM serial number
  struct ddrRank                rankList[MAX_RANK_DIMM];
  UINT8                         SPDmtb;             ///< DDR4 medium time base (SPD_TB_DDR4)
  UINT8                         SPDSpecRev;         ///< Revision of the SPD spec for this data
  UINT8                         SpdModuleType;      ///< SPD Byte 2 (0x002): Key Byte / Host Bus Command Protocol Type
  UINT8                         rcCache[16];        /// DDR4 RCD cache
  UINT8                         rcxCache[16];
  UINT8                         lrDimmPresent;
  UINT8                         rcClk;
  UINT8                         rcCmd;
  UINT8                         rcCtl;
  UINT16                        rcVref;
  INT16                         QxCAClkOffset;      ///< RCD QxCA Training Offset
  UINT8                         dqSwz[36];
  UINT16                        rawCap;             ///< Raw Capacity
  UINT16                        VendorID;           ///< Vendor ID for NVMDIMM Mgmt driver thru Nfit
  UINT16                        DeviceID;           ///< Device ID for NVMDIMM Mgmt driver thru Nfit
  UINT16                        RevisionID;
  UINT16                        SubsystemVendorID;
  UINT16                        SubsystemDeviceID;
  UINT16                        SubsystemRevisionID;
  UINT8                         DimmGnt2Erid;       ///< DIMM-side Grant-to-ERID (produced by SPD and consumed by FMC; distinct from MC-programmed value)
  X16_MAPPING                   LogicalX16Mapping;
  UINT8                         manufacturer[NGN_MAX_MANUFACTURER_STRLEN];  /* Manufacturer */
  UINT8                         serialNumber[NGN_MAX_SERIALNUMBER_STRLEN];  /* Serial Number */
  UINT8                         PartNumber[NGN_MAX_PARTNUMBER_STRLEN]; /* Part Number */
  UINT16                        volCap;             /* Volatile Capacity (2LM region) */
  UINT16                        nonVolCap;          /* NonVolatile Capacity (PMEM/PMEM$ region + Blk region) */
  UINT16                        perCap;             /* Persistent Capcity (PMEM/PMEM$). This size is not obtained from FNV. This is derived data. */
  UINT16                        WbCachePerCap;      /* Persistent WB cache Capcity (AD-WB) This size is not obtained from FNV. This is derived data. */
  UINT32                        volRegionDPA;       /* DPA start address of 2LM Region */
  UINT32                        perRegionDPA;       /* DPA start address of PMEM Region */
  struct firmwareRev            firmwareRevision;   /* Firmware Revision */
  UINT8                         dimmSku;            /* Dimm SKU info Bit0:MemorymodeEnabled 1: StoragemodeEnabled 2:AppDirectmodeEnabled 3:DieSparingCapable 4:SoftProgrammableSKU 5:MemorymodeEncryptionEnabled 6:AppDirectmodeEncryptionEnabled 7:StoragemodeEncrytionEnabled */
  UINT16                        InterfaceFormatCode;
  UINT16                        VendorIDIdentifyDIMM;
  UINT16                        DeviceIDIdentifyDIMM;
  UINT32                        DdrtDimmBasicTiming;
  UINT8                         Uniqueid[NGN_MAX_UID_STRLEN];  /* Unique ID */
  DIMM_RANK_MAP_OUT_REASON      MapOutReason[MAX_RANK_DIMM];
  UINT8                         VlpRdimmPresent;
  RDIMM_RDIMM_THERMAL_HEAT_SPREADER_SOLUTION_STRUCT  SpdThermalHeatSpreaderSolution; ///< Byte 132 (0x084) (Registered): RDIMM Thermal Heat Spreader Solution
  UINT8                         pad[1];             ///< padding added to eliminate: MemChipDdrio.c(5567): warning C4366: The result of the unary '&' operator may be unaligned. ie:  &((*ChannelNvList)[Channel].ddrCRClkControls)
} DIMM_NVRAM_STRUCT; //struct dimmNvram

#define MAX_CMD_CSR 16
#define MAX_SIDE    2

///
/// Channel information stored in NVRAM
///
typedef struct channelNvram {
  UINT8             enabled;            ///< 0 = channel disabled, 1 = channel enabled
  UINT8             mb3trainingfailure;
  UINT8             features;           ///< Bit mask of features to enable or disable
  UINT8             maxDimm;            ///< Number of DIMM
  UINT8             numRanks;           ///< Number of ranks on this channel
  UINT8             numQuadRanks;       ///< Number of QR DIMMs on this channel
  UINT8             timingMode;         ///< Command timing mode(1N, 2N, or 3N) for normal operation
  UINT8             trainTimingMode;    ///< Command timing mode(1N, 2N, or 3N) for training
  UINT8             ckeMask;            ///< CKE signals to assert during IOSAV mode
  UINT8             chFailed;           ///< ddr4 memory in this channel had failed (MFO)
  UINT8             ngnChFailed;        ///< NGN memory in this channel had failed (MFO)
  struct comTime    common;             ///< Common timings for this channel
  struct dimmNvram  dimmList[MAX_DIMM];
  struct RankCh     rankPerCh[MAX_RANK_CH];
  UINT32            dimmVrefControlFnv1;
  UINT32            dimmVrefControlFnv1Sa; // SA fub
  UINT32            ddrCrCmdPiCodingFnv;
  UINT32            ddrCrCmdPiCodingFnv2;
  UINT8             encodedCSMode;
  UINT8             cidBitMap;
  UINT8             txVrefSafe[MAX_RANK_CH];
  UINT8             dimmRevType;
  UINT8             lrDimmPresent;
#ifdef LRDIMM_SUPPORT
  UINT8             chOneRankTimingModeEnable;
#endif
  UINT8             lrRankMultEnabled;
  UINT8             v110NotSupported;
  UINT8             v120NotSupported;
  UINT32            rankErrCountStatus;
  UINT32            rankErrCountInfo[(MAX_RANK_CH * SUB_CH)/2];
  UINT32            rankErrThresholdInfo[(MAX_RANK_CH * SUB_CH)/2];
  UINT8             ddrtEnabled;
  UINT32            SmiSpareCtlMcMainExt;
  UINT8             spareInUse;
  UINT8             spareDimm[MAX_RANK_CH/2];
  UINT8             spareRank[MAX_RANK_CH/2];
  UINT16            spareRankSize[MAX_RANK_CH/2];
  UINT8             spareLogicalRank[MAX_SPARE_RANK];
  UINT8             sparePhysicalRank[MAX_SPARE_RANK];
  UINT32            ioLatency1;
  UINT32            tcrwp;              ///< TCRWP_MCDDC_CTL_STRUCT
  UINT32            tcothp;             ///< TCOTHP_MCDDC_CTL_STRUCT
  UINT32            tcothp2;
  UINT32            TCMr0Shadow;
  UINT32            TCMr2Shadow;
  UINT32            TCMr4Shadow;
  UINT32            TCMr5Shadow;
  INT16             normalizationFactor;
  UINT8             FmcWrCreditLimit;
  UINT8             FmcRdCreditLimit;
  UINT8             twoXRefreshSetPerCh;  /* Saves the refreshRate value for each channel */
  UINT8             tCCDAdder;
  BUS_TYPE          fnvAccessMode;
  UINT32            ddrtDimm0BasicTiming;
  UINT32            ddrtDimm1BasicTiming;
  UINT32            cadbMuxPattern[CADB_MUX_MAX];
  UINT8             EccModeMcMain[(MAX_RANK_CH * SUB_CH)];
  UINT32            Plus1Failover[(MAX_RANK_CH * SUB_CH)];
  UINT32            LinkRetryErrLimits;
  UINT32            LinkLinkFail;
  UINT32            mtCas2CasDr;                         /* CNX Change */
  UINT32            mtCas2CasDd;                         /* CNX Change */
  UINT32            mtCas2CasSr;                         /* CNX Change */
  UINT32            mtCas2CasSg;                         /* CNX Change */
  UINT32            mtCas2CasDs;                         /* CNX Change */
  UINT32            tCke;                                /* CNX Change */
  UINT32            MemoryTimingsAdj;
  UINT8             T_rrdr_org;   ///< Nvram Variable to store original tRRDR turnaround timing parameter
  UINT8             T_rrdd_org;   ///< Nvram Variable to store original tRRDD turnaround timing parameter
  UINT8             T_rrdr;   ///< Nvram Variable to store current tRRDR turnaround timing parameter
  UINT8             T_rrdd;   ///< Nvram Variable to store current tRRDD turnaround timing parameter
#ifndef DDRIO_DATA_1S
  UINT32            dataControl0;
  UINT32            dataControl1[MAX_STROBE];       ///< need to save for each strobe
  UINT32            dataControl2[MAX_STROBE];
  UINT32            dataControl4[MAX_STROBE];
  UINT32            dataControl3[MAX_STROBE];
  UINT32            dataOffsetComp[MAX_STROBE];
  UINT8             DrvStaticLegCfg[MAX_STROBE];
  UINT8             OdtSegmentEnable[MAX_STROBE];
  UINT8             DataSegmentEnable[MAX_STROBE];
  UINT8             RcompCode[MAX_STROBE];
  UINT32            LegacyRxDfeTxEcho[MAX_STROBE];   //Used to store Legacy RxDfe or TxEcho register values
  UINT32            ddrCRClkControls;
  UINT32            DdrCrCmdNControls;
  UINT32            DdrCrCmdSControls;
  UINT32            DdrCrCkeControls;
  UINT32            DdrCrCtlControls;
  UINT32            clkCsr;
  INT16             TxDqsDelay[MAX_RANK_CH][MAX_STROBE];
  UINT32            txGroup0[MAX_RANK_CH][MAX_STROBE];
  UINT32            txGroup1[MAX_RANK_CH][MAX_STROBE];
  UINT32            TxEqCoefTap2[MAX_RANK_CH][MAX_STROBE];
  UINT16            XtalkDeltaN0[MAX_RANK_CH][MAX_STROBE];
  UINT16            XtalkDeltaN1[MAX_RANK_CH][MAX_STROBE];
  UINT32            txTco[MAX_RANK_CH][MAX_STROBE];
  UINT32            rxGroup0[MAX_RANK_CH][MAX_STROBE];
  UINT32            rxGroup1[MAX_RANK_CH][MAX_STROBE];
  UINT32            RxDfeCoeff[MAX_STROBE];
  UINT32            RxDfeControl[MAX_STROBE];
  UINT32            rxOffset[MAX_RANK_CH][MAX_STROBE];
  UINT32            rxVrefCtrl[MAX_STROBE];
  UINT8             txVrefCache[MAX_RANK_CH][MAX_STROBE]; ///< Cached value of txVref (this might not be the programmed value)
  UINT8             txVref[MAX_RANK_CH][MAX_STROBE];      ///< Actual current value of txVref
  UINT32            ddrCRCmdTrainingCmdN;
  UINT32            ddrCRCmdTrainingCmdS;
  UINT32            ddrCRCtlTraining;
  UINT32            ddrCRCkeTraining;
  UINT32            ddrCRClkTraining;
  UINT32            ddrCRClkRanksUsed;
  UINT32            dataOffsetTrain[MAX_STROBE];
  UINT32            DataTrainFeedbackMultiCast;
  UINT32            ddrCRCmdControls3CmdN;
  UINT32            ddrCRCmdControls3CmdS;
  UINT32            ddrCRCmdControls3Ctl;
  UINT32            ddrCRCmdControls3Cke;
  UINT32            cmdCsr[MAX_CMD_CSR];
  UINT32            rxGroup1n[MAX_RANK_CH][MAX_STROBE];  /* CNX Change */
  UINT32            rxGroup1p[MAX_RANK_CH][MAX_STROBE];  /* CNX Change */
  UINT32            DdrCrintfDataTiming0;
  UINT32            TxDqBitClockDelta[MAX_RANK_CH][MAX_STROBE];  ///< Each unit represent 64 ticks
                                                                 ///< [7:0]   -> UIs for Bit 0
                                                                 ///< [15:8]  -> UIs for Bit 1
                                                                 ///< [23:16] -> UIs for Bit 2
                                                                 ///< [31:24] -> UIs for Bit 3
  UINT32            EnableRidUnderfillOrg;   ///< Nvram Variable to store original EnableRidUnderfill RDB entry
  UINT32            EnableRidVc2Org;         ///< Nvram Variable to store original EnableRidVc2 RDB entry
  UINT32            EnableRidVc3Org;         ///< Nvram Variable to store original EnableRidVC3 RDB entry
#endif // !DDRIO_DATA_1S
} CHANNEL_NVRAM_STRUCT, *PCHANNEL_NVRAM_STRUCT;

///
/// IMC information stored in NVRAM
///
typedef struct imcNvram {
  UINT8               enabled;          ///< 0 = imc disabled, 1 = imc enabled
  UINT32              scrubMask;        ///< Scrub mask
  UINT32              scrubMask2;       ///< Scrub mask2
  UINT8               EmcaLtCtlMcMainExt;
  UINT32              ExRasConfigHaCfg;
  UINT32              SmiSpareCtlMcMainExt;
  UINT8               AppDirectHoleSize;
  UINT8               imcNodeId;               /*  System wide socket id for imc */
  BOOLEAN             LaneReversalEn;
#ifndef DDRIO_DATA_1S
  UINT32              ddrCRCompCtl0;
  UINT32              ddrCRCompCtl3; // This is need for silicon workaround 'S1409370801'
  UINT32              dimmVrefControl1;
#endif // !DDRIO_DATA_1S
} IMC_NVRAM_STRUCT; //struct imcNvram

///
/// Socket information stored in NVRAM
///
struct socketNvram {
  UINT8                     enabled;
  INT32                     minTCK;           ///< minimum tCK for this DIMM
  UINT8                     ddrFreq;          ///< DDR Frequency of this socket
  UINT16                    ddrFreqMHz;       ///< DDR Frequency of this socket in MHz
  UINT16                    QCLKps;           ///< Qclk period in pS
  UINT8                     cmdClkTrainingDone;
  UINT8                     ddrVoltage;       ///< Voltage of this socket
  UINT8                     lrDimmPresent;
  UINT8                     DcpmmPresent;
  BOOLEAN                   x16DimmPresent;
  UINT8                     maxDimmPop;       ///< Maximum number of DIMM populated on a channel for a socket
  UINT8                     wa;               ///< Bit field for workarounds
  UINT8                     ddr4SpdPageEn;
  struct channelNvram       channelList[MAX_CH];
  struct imcNvram           imc[MAX_IMC];
  UINT64_STRUCT             procPpin;         ///< Processor PPIN number
  UINT32                    smiCtrlUboxMisc;
  UINT8                     refreshRate;
  INT8                      normalizationFactorEn;
  UINT16                    WdbCacheValidPerChannel;  ///< Channel bitmask indicating whether the WDB cache is valid
  TRAINING_STEP_DONE_STRUCT TrainingStepDone;     ///< Indicators of whether a given training step is done
  UINT8                     ddrtFreq;
  UINT8                     cmdVrefTrainingDone;
  UINT8                     mcpPresent;
  UINT32                    FaultyPartsFlag[MAX_CH]; ///< Store faulty strobe info in a channel to NVRAM
  UINT32                    CsrWriteLatency[MAX_CH]; ///< CSR Write Latency from Core-Ubox-iMC[ch] roundtrip
  UINT32                    CsrReadLatency[MAX_CH];  ///< CSR Read Latency from Core-Ubox-iMC[ch] roundtrip
}; //struct socketNvram
typedef struct socketNvram  SOCKET_NVRAM;

typedef struct memNvram {
  UINT8           DataGood;                   ///< Set to one if valid data is present in this structure
  UINT8           RASmode;                    ///< RAS mode (lockstep, mirror, sparing)
  UINT16          RASmodeEx;                  ///< Extended RAS mode (patrol scrub)
  UINT8           ratioIndex;                 ///< Index into the DDR3 ratio table
  UINT8           eccEn;                      ///< Set if ECC will be enabled
  UINT8           dimmTypePresent;            ///< Type of DIMMs populated (RDIMM,UDIMM,SODIMM)
  UINT8           DcpmmPresent;               ///< Is an DCPMM present in the system?
  UINT16          dramType;                   ///< DDR3 or DDR4 (from keybyte in SPD)
  UINT32          scrambleSeed;               ///< Data scrambling seed
  UINT32          socketBitMap;               ///< CPU present mask
  UINT8           ExtendedADDDCEn;
  struct memSetup savedSetupData;
  struct socketNvram socket[MAX_SOCKET];

  UINT8           XMPProfilesSup;
  UINT8           XMPProfilesRevision;
  struct          memTiming  profileMemTime[2];

  UINT16          Crc16;
  BOOLEAN         FmcCacheDone;               ///< Variable to indicate FMC Register caching is finished
  UINT8           threeDsModeDisabled;        ///< Variable to track if 3DS mode is enabled/disabled for CPGC
  UINT8           volMemMode;
  UINT8           CacheMemType;               ///< Only valid if volMemMode is 2LM
#ifdef DDRIO_DATA_1S
  UINT8           DdrioNvdata[DDRIO_DATA_1S * MAX_SOCKET];
#endif // DDRIO_DATA_1S
#ifdef DRAM_DATA_1S
  UINT8           DramNvdata[DRAM_DATA_1S * MAX_SOCKET];
#endif // DRAM_DATA_1S
#ifdef RCD_DATA_1S
  UINT8           RcdNvdata[RCD_DATA_1S * MAX_SOCKET];
#endif // RCD_DATA_1S
#ifdef LRDIMM_DB_DATA_1S
  UINT8           DbNvdata[LRDIMM_DB_DATA_1S * MAX_SOCKET];
#endif // LRDIMM_DB_DATA_1S

} MEM_NVRAM_STRUCT;

//
// Max number for FMC cache register and structure
//
#define  MAX_FMC_CACHE    2

struct FmcCacheSt {
  UINT8   Status;  // Cache status
  UINT32  Reg;     // Register
  UINT32  Data;    // Data
}; // struct FmcCacheSt


///
/// Rank info
///
struct rankDevice {
  UINT16  MR0;  ///< MR0 value for this rank
  UINT16  MR1;  ///< MR1 value for this rank
  UINT16  MR2;  ///< MR2 value for this rank
  UINT16  MR3;  ///< MR3 value for this rank
  UINT16  MR4;  ///< MR4 value for this rank
  UINT16  MR5;  ///< MR5 value for this rank
  UINT16  MR6[MAX_STROBE];  ///< MR6 value for this rank/dram
#ifdef LRDIMM_SUPPORT
  UINT8   CurrentLrdimmTrainingMode;
  UINT8   CurrentDramMode;
  UINT16  CurrentMpr0Pattern;
  UINT8   lrbufRxVrefCache[MAX_STROBE];
  UINT8   cachedLrBuf_FxBC2x3x[MAX_STROBE];
  UINT8   cachedLrBuf_FxBC4x5x[MAX_STROBE];
  UINT8   cachedLrBuf_FxBC8x9x[MAX_STROBE];
  UINT8   cachedLrBuf_FxBCAxBx[MAX_STROBE];
  UINT8   cachedLrBuf_FxBCCxEx[MAX_STROBE/2];
  UINT8   cachedLrBuf_FxBCDxFx[MAX_STROBE/2];
#endif
  UINT32  lrbufTxVrefCache[MAX_STROBE];
}; //struct rankDevice

///
/// DIMM info
/// List ordered by proximity to Host (far to near)
///
typedef struct dimmDevice {
  INT32               minTCK;     ///< minimum tCK for this DIMM (SPD_MIN_TCK)
#ifdef DEBUG_CODE_BLOCK
  UINT32              tCL;
  UINT16              tRCD;
  UINT16              tRP;
#endif  // DEBUG_CODE_BLOCK
  UINT16              NVmemSize;
  UINT16              memSize;    ///< Memory size for this DIMM (64MB granularity)
  UINT16              UnmappedMemSize;
  struct rankDevice   rankStruct[MAX_RANK_DIMM];
  struct FmcCacheSt   FmcCache[MAX_FMC_CACHE]; ///< FMC cache info/status
  UINT8               SPDPartitionRatio[MAX_SOCKET * MAX_IMC]; ///< NVM DIMM partitionRatios
  UINT8               CachedLrBuf_DFECoef[MAX_BITS_IN_BYTE][DB_DFE_TAP][MAX_STROBE/2];  // JEDEC F3BCCx-Fx coeffcient. 8 DQ x 4 taps x 9 DB
  BOOLEAN             FmcWdbFlushFailed; /// < 0 = WDB flush failed on previous boot, 1 = WDB flush completed w/o errors on previous boot
  BOOLEAN             EadrFlushFailed;   /// < 0 = Extended ADR flush failed on previous boot, 1 = Extended ADR flush completed w/o errors on previous boot
} DIMM_DEVICE_INFO_STRUCT; //struct dimmDevice

///
/// DDRT DIMM info
///
typedef struct {
  UINT16                NgnLogSeqNum[NGN_LOG_TYPE_NUM][NGN_LOG_LEVEL_NUM];
  UINT16                NgnMaxLogEntries[NGN_LOG_TYPE_NUM][NGN_LOG_LEVEL_NUM];
  UINT8                 NvdimmLinkFailOnPrevBoot  : 1, /* Link failure was detected in this boot */
                        NvdimmMediaErrLogged      : 1, /* Media error log was detected in this boot */
                        NvdimmTempErrLogged       : 1, /* Fatal temperature error log was detected in this boot */
                        NvdimmUnmapped            : 1, /* NVDIMM is not to be mapped per memory population POR enforcement or SKU Limit violation. */
                        NvdimmUnmappedReason      : 1, /* Reason of NVDIMM is not to be mapped 0 - population POR enforcement, 1 - SKU Limit Violation */
                        NvdimmRemapped            : 1, /* NVDIMM is not to be unmapped per memory population POR enforcement. */
                        NvdimmAdModeNotEnabled    : 1, /* Indicates whether DIMM SKU reports AD mode enabled or not */
                        Reserved                  : 1;
  UINT64_STRUCT         NgnBsr;                        /* NGN NVDIMM Boot Status Register */
} HOST_DDRT_DIMM_DEVICE_INFO_STRUCT;

typedef struct {
  UINT8 WdbLine[MRC_WDB_LINE_SIZE];
} TWdbLine;

///
/// TT channel info
///
typedef struct ddrChannel {
  UINT8                             mcId;              ///<Memory controller number
  UINT8                             numDimmSlots;      ///<Number of DIMM slots per channel
  UINT32                            memSize;           ///<Memory size for this channel (64MB granularity)
  UINT32                            ddr4RemSize;       ///<Remaining DDR4 Memory size for this channel
  UINT32                            volSize;           ///<Volatile size of the NVM dimms in this channel
  UINT32                            NonVolSize;        ///<Non Volatile size of the NVM dimms in this channel
  UINT32                            volRemSize;        ///<Remaining Volatile size of the NVM dimms in this channel
  UINT32                            perSize;           ///<Pmem size of the NVM dimms in this channel
  UINT32                            perRemSize;        ///<Remaining Pmem size of the NVM dimms in this channel
  UINT32                            WbCachePerSize;    ///<Persistent WB cache (AD-WB) size of the NVM dimms in this channel
  UINT32                            WbCachePerRemSize; ///<Remaining Persistent WB cache (AD-WB) size of the NVM dimms in this channel
  UINT32                            ctrlSize;          ///<Ctrl region size of the NVM dimms in this channel
  UINT32                            ctrlRemSize;       ///<Remaining Ctrl region size of the NVM dimms in this channel
  UINT32                            NVmemSize;         ///< NVDIMM channel memory size
  UINT32                            NVmemRemSize;
  UINT32                            remSize;           ///< Size not yet mapped in units of 64 MB
  UINT32                            DdrCacheSize;      ///< Size of DDR memory reserved for 2LM cache (64MB granularity)
  DIMM_DEVICE_INFO_STRUCT           dimmList[MAX_DIMM];
  HOST_DDRT_DIMM_DEVICE_INFO_STRUCT DdrtDimmList[MAX_DDRT_DIMM_PER_CH];
  UINT8                             CsrUnlock;              ///< Whether FMC CSRs are unlocked (accessible/writable)
  UINT16                            FisVersion[MAX_DIMM];   ///< Firmwre Interface Specification version
} DDR_CHANNEL_STRUCT; //struct ddrChannel

typedef union {
  struct {
    UINT8 TrainingPatternMode : 3; // Bits[2:0] - Training Pattern Mode
    UINT8 QcaUcaTrainingDone : 1; // Bits[3] - QCA training or UDIMM CA training are done or not
    UINT8 BitIndex : 2; // Bits[5:4] - Used to indicate which bit to test.
    UINT8 CoarseRdDqDqsTrainingDone : 1; // Bits[6] - Coarse Read Dq/Dqs training are done or not
    UINT8 Reserved : 1; // Bits[7] - Reserved for future use.
  } Bits;
  UINT8 Data;
} TRAINING_STATUS_STRUCT;

#define MAX_FAIL_RANGE 128

typedef union {
  struct {
    UINT32 row : 18;        // This field is used for < > comparisons; more significant bits are used for match comparison only
    UINT32 bankPair : 4;
    UINT32 logicalSubRank : 3;
    UINT32 logicalRank : 3;
    UINT32 upperBgMask : 2; // bit-0 = 1 means failure with upper BG bit = 0; bit-1 = 1 means failure with upper BG = 1
    UINT32 rfu: 1;
    UINT32 valid : 1;
  } Bits;
  UINT32 Data;
} ROW_ADDR;

typedef struct {
  ROW_ADDR  addr;
  UINT32 size;
  UINT32 mask[3];
} ROW_FAIL_RANGE;

///
/// socket info
///
typedef struct Socket {
  UINT8                   SocketDieCount;           ///< Number of cpu dies present in the socket
  UINT32                  NumMemSs;
  MRC_MSM                 MemSsList[MAX_MEM_SS];
  BOOLEAN                 ImcPresent[MAX_IMC];      ///< on multi-die, some iMCs might not exist
  UINT8                   imcEnabled[MAX_IMC];
  UINT8                   fatalError;
  UINT8                   majorCode;
  UINT8                   minorCode;
  UINT8                   maxRankDimm;              ///< Maximum number or ranks supported per DIMM
  UINT32                  memSize;                  ///< DDR4 memory size for this socket (64MB granularity)
  TRAINING_STATUS_STRUCT  TrainingStatus;
  struct ddrChannel       channelList[MAX_CH];
  UINT8                   socketSubBootMode;
  UINT8                   hostRefreshStatus;
  UINT8                   firstJEDECDone;           ///< Flag to indicate the first JEDEC Init has executed
  UINT64_STRUCT           procPpin;
  UINT8                   cadbMRSMode;              ///< Mode of operation (LOAD / EXECUTE / NORMAL)
  UINT8                   cadbMRSIndex[MAX_CH];
  UINT32                  NVmemSize;                ///< Memory size for this node
  UINT32                  TotalInterleavedMemSize;  ///< DDR4 memory size for this socket (64MB granularity)
  UINT32                  TotalInterleavedNVMemSize;///< Actual NVMEM interleaved.
  UINT32                  volSize;                  ///< Volatile size of the NVM dimms for this socket (64MB granularity)
  UINT32                  perSize;                  ///< Persistent size of the NVM dimms for this socket (64MB granularity)
  UINT32                  WbCachePerSize;           ///< Persistent WB cache (AD-WB) size of the NVM dimms for this socket (64MB granularity)
  BOOLEAN                 TurnaroundInitDone;       ///< Turnaround Initialization Done
  MRC_TT                  CurrentTestType;          ///< Training step currently being executed by this socket
  SOCKET_CHIP                                 ///< Chip hook to enable Socket fields
} SOCKET_INFO_STRUCT; // struct Socket

///
/// Sub-boot state internal to MRC (8-15 are definable).  The 2 main boot types and paths through KTI RC/MRC - NormalBoot and S3Resume.
/// Within NormalBoot and S3Resume, the sub-boot type can be cold, warm, fast warm, fast cold, and ADR resume.  These are populated
/// at the beginning of MRC so they are not applicable for KTI RC.
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
  NvDimmResume = 13   // NvDimm flow is similar to Adr Batterybackup, but the DIMMs need
                      //    Rx & Mx registers initialized.
} SUB_BOOT_MODE;

///
/// define the Training_Result_UP/DOWN CRs struct.
///
struct TrainingResults {
  UINT32  results[4];
};

#ifdef LRDIMM_SUPPORT
struct lrMrecTrainingResults {
  UINT8  results;
};
#endif // LRDIMM_SUPPORT

#define MEM_CHIP_POLICY_DEF(x)          Host->var.mem.memChipPolicy.x
#define MEM_CHIP_POLICY_VALUE(Host, x)  Host->var.mem.memChipPolicy.x
typedef struct {
  UINT8                     maxVrefSettings;                        // MAX_VREF_SETTINGS
  UINT8                     earlyVrefStepSize;                      // EARLY_VREF_STEP_SIZE
  INT16                     ctlPiGrp;                               // CTL_PI_GRP
  UINT8                     minIoLatency;                           // MIN_IO_LATENCY
  UINT16                    cas2DrvenMaxGap;                        // CAS2DRVEN_MAXGAP
  UINT8                     mrcRoundTripIoComp;                     // MRC_ROUND_TRIP_IO_COMPENSATION;
  UINT8                     mrcRoundTripIoCompStart;                // MRC_ROUND_TRIP_IO_COMP_START;
  UINT8                     mrcRoundTripMax;                        // MRC_ROUND_TRIP_MAX_VALUE;
  UINT32                    SrPbspCheckinCsr;                       // SR_PBSP_CHECKIN_CSR                       (BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG)           // UBOX scratchpad CSR02
  UINT32                    SrBiosSerialDebugCsr;                   // SR_BIOS_SERIAL_DEBUG_CSR(BIOSSCRATCHPAD6_UBOX_MISC_REG)                                      // UBOX scratchpad CSR6
  UINT32                    SrPostCodeCsr;                          // SR_POST_CODE_CSR         (BIOSNONSTICKYSCRATCHPAD7_UBOX_MISC_REG)                            // UBOX scratchpad CSR7
  UINT32                    SrErrorCodeCsr;                         // SR_ERROR_CODE_CSR        (BIOSNONSTICKYSCRATCHPAD8_UBOX_MISC_REG)                            // UBOX scratchpad CSR8
  UINT32                    SrMemoryDataStorageDispatchPipeCsr;     // #define SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR  (BIOSNONSTICKYSCRATCHPAD13_UBOX_MISC_REG)  // UBOX scratchpad CSR13
  UINT32                    SrMemoryDataStorageCommandPipeCsr;      // #define SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR   (BIOSNONSTICKYSCRATCHPAD14_UBOX_MISC_REG)  // UBOX scratchpad CSR14
  UINT32                    SrMemoryDataStorageDataPipeCsr;         // #define SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR      (BIOSNONSTICKYSCRATCHPAD15_UBOX_MISC_REG)  // UBOX scratchpad CSR15
  UINT32                    SrBdatStructPtrCsr;                     // SR_BDAT_STRUCT_PTR_CSR
  UINT32                    BiosStickyScratchPad0;                  // BIOSSCRATCHPAD0_UBOX_MISC_REG
  UINT8                     PerBitMarginDefault;                    // PER_BIT_MARGIN_DEFAULT
  UINT8                     RxOdtDefault;                           // RX_ODT_DEFAULT
  UINT8                     CmdTxEqDefault;                         // CMD_TX_EQ_DEFAULT
  UINT8                     RxDfeDefault;                           // RX_DFE_DEFAULT
  UINT8                     TxRiseFallSlewRateDefault;              // TX_RF_SLEW_RATE_DEFAULT
  UINT8                     RmtColdFastBootDefault;                 // RMT_COLD_FAST_BOOT_DEFAULT
  UINT8                     RxVrefTrainingMode;                     // RX_VREF_TRAINING_MODE
  UINT8                     TxVrefTrainingMode;                     // TX_VREF_TRAINING_MODE
  UINT16                    MaxPhaseInReadAdjustmentDq;             // MAX_PHASE_IN_READ_ADJUSTMENT_DQ
} MEM_CHIP_POLICY;

struct DimmDeviceTraining {
  UINT8   oneRankTimingModeLrbuf_FxBC2x3x[MAX_STROBE];
  UINT8   oneRankTimingModeLrbuf_FxBC4x5x[MAX_STROBE];
  UINT8   oneRankTimingModeLrbuf_FxBC8x9x[MAX_STROBE];
  UINT8   oneRankTimingModeLrbuf_FxBCAxBx[MAX_STROBE];
  UINT8   oneRankTimingModeLrbuf_FxBCCxEx[MAX_STROBE/2];
  UINT8   oneRankTimingModeLrbuf_FxBCDxFx[MAX_STROBE/2];
  UINT8   originalRank0Lrbuf_FxBC2x3x[MAX_RANK_DIMM][MAX_STROBE];
  UINT8   originalRank0Lrbuf_FxBC4x5x[MAX_RANK_DIMM][MAX_STROBE];
  UINT8   originalRank0Lrbuf_FxBC8x9x[MAX_RANK_DIMM][MAX_STROBE];
  UINT8   originalRank0Lrbuf_FxBCAxBx[MAX_RANK_DIMM][MAX_STROBE];
  UINT8   originalRank0Lrbuf_FxBCCxEx[MAX_RANK_DIMM][MAX_STROBE/2];
  UINT8   originalRank0Lrbuf_FxBCDxFx[MAX_RANK_DIMM][MAX_STROBE/2];
}; //struct DimmDeviceTraining

/// TT channel info
///
struct DdrChannelTraining {
  TWdbLine                  WdbLines[MRC_WDB_LINES];
  struct DimmDeviceTraining dimmList[MAX_DIMM];
}; //struct DdrChannelTraining

struct TrainingVariable {
  UINT8   rdVrefLo;
  UINT8   rdVrefHi;
  UINT8   wrVrefLo;
  UINT8   wrVrefHi;
  UINT8   cmdVrefLo;
  UINT8   cmdVrefHi;
  UINT8   DQPat;       ///< Global Variables storing the current DQPat REUT Test
  UINT8   DQPatLC;     ///< Global Variables storing the current DQPat Loopcount
  BOOLEAN EnDumRd;     ///< Enable/Disable Logic Analizer

  INT16   TxDqLeft[MAX_CH][MAX_RANK_CH][MAX_STROBE];
  INT16   TxDqRight[MAX_CH][MAX_RANK_CH][MAX_STROBE];
  INT16   TxVrefLow[MAX_CH][MAX_RANK_CH][MAX_STROBE];
  INT16   TxVrefHigh[MAX_CH][MAX_RANK_CH][MAX_STROBE];
  INT16   RxDqsLeft[MAX_CH][MAX_RANK_CH][MAX_STROBE];
  INT16   RxDqsRight[MAX_CH][MAX_RANK_CH][MAX_STROBE];
  INT16   RxVrefLow[MAX_CH][MAX_RANK_CH][MAX_STROBE];
  INT16   RxVrefHigh[MAX_CH][MAX_RANK_CH][MAX_STROBE];

  struct  DdrChannelTraining  channelList[MAX_CH];
}; //struct TrainingVariable

///
/// Indicates how SPD data should be retrieved:
///   SpdSmbus         Data should be retrieved via SMBUS
///   SpdInternal      Data should be retrieved via internal SPD array
///   SpdInternalTrace Data should be retrieved via internal SPD array, but the SMBUS transactions should still be
///                      carried out to generate register traces for debugging
///
typedef enum {
  SpdSmbus,
  SpdInternal,
  SpdInternalTrace,
  SpdMax
} SPD_SOURCE;

//
// CMI Read and Write credit configuration register defaults
//
typedef struct {
  UINT32      CmiRdCreditConfigN0;
  UINT32      CmiRdCreditConfigN1;
  UINT32      CmiWrCreditConfigN0;
  UINT32      CmiWrCreditConfigN1;
  BOOLEAN     Valid;                /// Set to TRUE when structure is updated
} CMI_CREDIT_CONFIG_DEFAULT;

///
/// TT Host info
///
typedef struct memVar {
  UINT8                   currentSocket;    ///< Current socket being initialized
  UINT8                   PostCodeMinor;
  SUB_BOOT_MODE           subBootMode;      ///< WarmBoot, WarmBootFast, etc.
  UINT8                   wipeMemory;       ///< Write 0 to all memory to clean secrets
  UINT8                   skipMemoryInit;   ///< Skip memory init based on certain conditions.
  UINT8                   ddrFreqLimit;     ///< Set to limit frequency by the user
  UINT8                   chInter;          ///< Number of ways to interleave channels (1,2,3, or 4)
  UINT8                   callingTrngOffstCfgOnce; ///<to prevent looping inside RMT
  UINT8                   earlyCmdClkExecuted;
  UINT8                   checkMappedOutRanks;
#ifdef DEBUG_CODE_BLOCK
  UINT8                   earlyCtlClkSerialDebugFlag;
#endif // DEBUG_CODE_BLOCK
  UINT32                  memSize;          ///< Total physical memory size
  UINT32                  NVmemSize;          ///< Total physical memory size
  UINT32                  TotalInterleavedMemSize;      ///< DDR4 memory size for this socket (64MB granularity)
  UINT32                  TotalInterleavedNVMemSize; /// < Actual NVMEM interleaved.
  UINT32                  QCLKPeriod;       ///< QCLK Period in pico seconds
  UINT32                  lowMemBase;       ///< Mem base in 64MB units for below 4GB mem.
  UINT32                  lowMemSize;       ///< Mem size in 64MB units for below 4GB mem.
  UINT32                  highMemBase;      ///< Mem base in 64MB units for above 4GB mem.
  UINT32                  highMemSize;      ///< Mem size in 64MB units for above 4GB mem.
  UINT32                  initialLFSRSeed;
  UINT32                  piSettingStopFlag[MAX_CH];
  UINT8                   pxcEnabled;
  struct Socket           socket[MAX_SOCKET];                        ///< Per socket structure
  struct TrainingVariable TrainingVar;       ///< Local variable for DDR training
  UINT32                  NumValidPprEntries;
  PPR_ADDR_MRC_SETUP      pprAddrSetup[MAX_PPR_ADDR_ENTRIES_SPPR];
  UINT8                   pprStatus[MAX_PPR_ADDR_ENTRIES_SPPR];
  UINT8                   softPprDone[MAX_SOCKET];
  ///
  /// Flag to skip RowTestPPR execution, so it wont return false error
  ///
  BOOLEAN                 SkipPprRowTest[MAX_SOCKET];
  //
  // Avanced Memtest Failure Range tracking
  //
  UINT8                   FailRangeInitDone[MAX_SOCKET];
  ROW_FAIL_RANGE          FailRange[MAX_CH][MAX_FAIL_RANGE];
  UINT32                  FailMax[MAX_CH];  // This is initialized to 0 prior to any memtest, indicating no failures
  INT32                   FailIndex[MAX_CH];
  UINT8                   RetryState;
  ROW_FAIL_RANGE          LastFail[MAX_CH];
  UINT8                   PprResourceAvailable[MAX_CH][MAX_DIMM * MAX_RANK_DIMM_3DS * MAX_SUBRANK_3DS][MAX_STROBE];  // Max logical ranks = 16 (2DPC, 3DS 2R-4H), each BG is bit-mapped into UINT8
  UINT64_STRUCT           AdvMemtestErrInjMask;
  UINT8                   InjectErrorMATS[MAX_CH];
  UINT8                   AmtErrInjDone;
  BOOLEAN                 IsDdrMemInitDone;    ///< Flag indicates if DDR memory init is done.

#ifdef LRDIMM_SUPPORT
  struct TrainingResults  lrTrainRes[MAX_CH][MAX_STROBE];
  struct lrMrecTrainingResults lrMrecTrainRes[MAX_CH][MAX_STROBE];
  struct TrainingResults  lrMrdTrainRes[MAX_CH][MAX_STROBE];
  struct TrainingResults  lrDwlTrainRes[MAX_CH][MAX_STROBE];
  struct lrMrecTrainingResults lrCwlTrainRes[MAX_CH][MAX_STROBE];
  struct TrainingResults  lrMwdTrainRes[MAX_CH][MAX_STROBE];
  UINT8                   InPbaWaMode;
  UINT8                   InOvrRttPrkMode;
#endif // LRDIMM_SUPPORT

  UINT8                   maxSubRank;
  UINT8                   currentSubRank;
  struct TrainingResults  trainRes[MAX_CH][MAX_STROBE];
  BOOLEAN                 PerformanceTrackerInitialized;
  UINT8                   firstPass;
  UINT8                   previousBootError;
  UINT8                   xoverModeVar;     ///< xover mode (1 = xover2:2, 0 = native 1:1)
  UINT32                  Reserved6;
  UINT8                   Reserved7[RESERVED_2];
  EFI_GUID                Reserved8[RESERVED_2];
  UINT8                   Reserved9;

  SPD_SOURCE              SpdSource;        ///< Determines SPD data source
  UINT8                   SmbMode[MAX_SOCKET][MAX_SMB_INSTANCE]; ///< Stores type of smbus mode: 0 - I2C mode, 1 - I3C mode
  UINT8                   rankInter;        ///< 1, 2, 4, or 8 way interleave
  UINT8                   RasStatePrevBoot;
  UINT32                  mccpubusno;
  UINT32                  rtDefaultValue;
  UINT8                   channelEn;
  UINT8                   runningRmt;   ///< Flag to check that the RankMarginTool is currently being running
  UINT32                  lastCheckpoint[MAX_SOCKET];
  UINT8                   notRunningFromCT; ///< PostPackageRepairMain called from the CallTable (CT)
  UINT32                  chBitMask;
  UINT8                   FmcMaxCached;     ///< Record how many FMC register been cached
  UINT32                  FmcCachedReads;   ///< Record the amount of total FMC cache read times
  MEM_CHIP_POLICY         memChipPolicy;
  BOOLEAN                 secondPass[MAX_CH][MAX_STROBE];
  UINT8                   volMemMode;       ///< 0: 1LM  1:2LM 2:Undefined
  UINT8                   CacheMemType;     ///< 0: DDR$DDRT  Only valid if volMemMode is 2LM

  UINT8                   PmemCaching;        // caching contorl for AppDirect
  UINT8                   EadrSupport;        // eADR support
  UINT8                   EadrCacheFlushMode;

  UINT8                   imcInter;           // Number of ways to interleave imc (1 or 2)
  UINT32                  mmiohBase;          // MMIOH base in 64MB granularity
  UINT8                   read2tckCL[MAX_CH];     // increases to CL or CWL based on TCLK preamble
  UINT8                   write2tckCWL[MAX_CH];
  UINT8                   memInterleaveGran1LM;
  struct Reserved168      Reserved168;
  UINT32                  optionsNgn;
  UINT8                   setSecureEraseAllDIMMs;
  UINT8                   setSecureEraseSktCh[MAX_SOCKET][MAX_CH];
  UINT8                   SetSecureEraseSktChHob[MAX_SOCKET][MAX_CH];
  BOOLEAN                 AdrStateForPmemModule[MAX_SOCKET][MAX_CH];   // ADR state for Intel PMEM Modules
  UINT8                   AppDirectMemoryHole;
  UINT8                   LsxImplementation;
  UINT32                  NvdimmSmbusMaxAccessTime;
  UINT32                  NvdimmSmbusReleaseDelay;
  MEMORY_MAP_BLOCK_DECODER_DATA BlockDecoderData;   // Block decoder data for memory map
  UINT8                   ExtendedType17;

  BOOLEAN                 DdrtReadPendingQueueTimedOut[MAX_SOCKET][MAX_CH];  ///< Each element represents whether a given channel has had a DDRT Read Pending Queue (RPQ) timeout
  BOOLEAN                 DdrtReadPendingQueueCreditLimitSet[MAX_SOCKET][MAX_CH];  ///< Each element represents whether a given channel has the DDRT Read Pending (RPQ) credit limit enabled
  BOOLEAN                 DdrtSkipRpqDrainInCpgcPolling[MAX_SOCKET];  ///< Indicates whether to drain the DDRT RPQ when polling for CPGC done
  BOOLEAN                 WrLvlDeNormalizeStatus[MAX_SOCKET][MAX_CH][MAX_DIMM];
  BOOLEAN                 EkvPresent;   ///< Set if EKV controller on system
  BOOLEAN                 BwvPresent;   ///< Set if BWV controller on system
  BOOLEAN                 FastWarmBootDowngraded; ///On slow boot when FWR is downgrated to SWR NVDIMMS need to do a catastrophic save to prevent data loss
  BOOLEAN                 DcpmmWaitMediaReady; ///< Set on Slow Warm boots to indicate that BIOS needs to poll for Media Ready on DCPMM DIMMs after CKE is asserted
  BOOLEAN                 WarmBootRequested; ///< Set on Warm boots when is not possible to detect warm boot it via subBootMode (when BIOS internally promoted warm reset to cold reset/fast cold reset)
  CMI_CREDIT_CONFIG_DEFAULT   CmiCreditConfigDefault;
  UINT16                  BiosFisRevision;
  UINT16                  MaxAveragePowerLimit;  ///< Max Power limit in mW used for averaged power ( Valid range ends at 15000mW)
  UINT16                  MinAveragePowerLimit;  ///< Min Power limit in mW used for averaged power ( Valid range starts from 10000mW)
  UINT16                  CurrAveragePowerLimit; ///< Current Power limit in mW used for average power
  UINT16                  MaxMbbPowerLimit;    ///< Max MBB power limit ( Valid range ends at 18000mW).
  UINT16                  MinMbbPowerLimit;    ///< Min MBB power limit ( Valid range starts from 15000mW).
  UINT16                  CurrMbbPowerLimit; ///< Current Power limit in mW used for MBB power
  UINT32                  MaxMbbAveragePowerTimeConstant;  ///< Max MBB Average Power Time Constant
  UINT32                  MinMbbAveragePowerTimeConstant;  ///< Min MBB Average Power Time Constant
  UINT32                  CurrMbbAveragePowerTimeConstant; ///< Current MBB Average POwer Time Constant
  BOOLEAN                 RmtMinimumMarginCheckEnable;
} MEM_VAR_STRUCT;  // struct memVar

#pragma pack(pop)

#endif // _memhost_h

