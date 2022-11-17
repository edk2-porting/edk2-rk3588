/** @file
  Header file defining MEMORY_POLICY_PPI, which is for platform code to set platform
  specific configurations of memory reference code.

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MEMORY_POLICY_PPI_H_
#define _MEMORY_POLICY_PPI_H_

#include <MemHostChipCommon.h>
#include <PlatformHost.h>
#include <Memory/ProcSmbChipCommon.h>
#include <Guid/MemBootHealthGuid.h>
#include <PlatformInfoTypes.h>


///
///  Number of group of BIOS-to-Pcode Mailbox command.
//
#define MAX_B2P_MAILBOX_GROUPS        32

#pragma pack(push, 1)

///
/// Memory Timings Settings.
//
struct memTiming {

  ///
  /// @brief
  /// Column Latency.
  /// @details
  /// Column Latency (CL) time is the number of clock cycles needed to access a
  /// certain column of data in RAM. It's also known as CAS (column address
  /// strobe) time.
  //
  UINT8   nCL;

  ///
  /// @brief
  /// Row Precharge.
  /// @details
  /// RP (row precharge) time is the number of clock cycles needed to terminate
  /// access to an open row of memory, and open access to the next row.
  //
  UINT8   nRP;

  ///
  /// @brief
  /// RAS to CAS Delay.
  /// @details
  /// RAS to CAS Delay (RCD) is the number of clock cycles delay required
  /// between an active command row address strobe (RAS) and a CAS. It is
  /// the time required between the memory controller asserting a row address,
  /// and then asserting a column address during the subsequent read or write
  /// command. RCD stands for row address to column address delay time.
  //
  UINT8   nRCD;

  ///
  /// @brief
  /// Row to Row Delay.
  /// @details
  /// Active to Active Delay, Row to Row Delay or RAS to RAS Delay. The amount of
  /// cycles that taken to activate the next bank of memory.
  //
  UINT8   nRRD;
  UINT8   nRRD_L;

  ///
  /// @brief
  /// Write to Read Delay.
  /// @details
  /// Write to Read Delay. The amount of cycles required between a valid write
  /// command and the next read command.
  //
  UINT8   nWTR;

  ///
  /// @brief
  /// Row Active Strobe.
  /// @details
  /// Row Active Strobe (RAS) time is the minimum number of clock cycles needed
  /// to access a certain row of data in RAM between the data request and the
  /// precharge command. It's known as active to precharge delay.
  //
  UINT8   nRAS;

  ///
  /// @brief
  /// Read To Precharge delay.
  /// @details
  /// The number of clocks between a read command to a row pre-charge command.
  //
  UINT8   nRTP;

  ///
  /// @brief
  /// Write Recovery time.
  /// @details
  /// The amount of cycles that are required between a valid
  /// write operation and precharge, to make sure that data is written properly.
  ///
  UINT8   nWR;

  ///
  /// @brief
  /// Four Activate Window.
  /// @details
  /// Four Activate Window, which specifies the time window in wich four activates
  /// are allowed on the same rank.
  //
  UINT8   nFAW;

  ///
  /// CAS (WRITE) latency (CWL).
  //
  UINT8   nCWL;

  ///
  /// @brief
  /// Row Cycle.
  /// @details
  /// Row Cycle time, the minimum time in cycles taken for a row to complete a full
  /// cycle, which typically can be calculated by nRC = nRAS + nRP.
  //
  UINT8   nRC;

  ///
  /// @brief
  /// Command Rate.
  /// @details
  /// Command Rate / Command per Clock (1T/2T) is the delay between a memory chip
  /// is selected and the first active command can be issued.
  //
  UINT8   nCMDRate;

  ///
  /// The limit of DDR frequency ratio, based on base clock frequency.
  //
  UINT8   ddrFreqLimit;

  ///
  /// Vdd for DRAM core.
  //
  UINT16  vdd;

  ///
  /// XMP Memory Controller Voltage Level.
  //
  UINT8   ucVolt;

  ///
  /// Bits map to indicate if a CAS in a CAS list is supported.
  //
  UINT64  casSup;

  ///
  /// Refresh Interval.
  //
  UINT16  tREFI;

  ///
  /// @brief
  /// Refresh to Activate Delay.
  /// @details
  /// Refresh to Activate Delay or Refresh Cycle Time. The number of clocks from
  /// a Refresh command to the first Activate command.
  //
  UINT16  nRFC;

  ///
  /// Frequency of DDR.
  //
  UINT16  ddrFreq;
};

typedef struct ReservedS193 {

  UINT8    ReservedS69;

  UINT8    ReservedS70;

  UINT8    ReservedS68;

  UINT8    ReservedS67;

  UINT8    ReservedS71;

  UINT8    ReservedS74[MAX_SOCKET * MAX_IMC];

  UINT8    ReservedS194[MAX_SOCKET];

  UINT8    ReservedS77;

  UINT8    ReservedS48;

  UINT32   ReservedS195;

  UINT8    ReservedS196;

  UINT8    ReservedS107;

  UINT8    ReservedS105;

  UINT8    ReservedS75;

  UINT8    ReservedS108;

  UINT8    ReservedS73;

  UINT8    ReservedS197;


  UINT8    ReservedS49;

  UINT8    ReservedS50;

  UINT8    ReservedS51;

  UINT8    ReservedS52;

  UINT16   ReservedS53;

  UINT16   ReservedS54;

  UINT16   ReservedS55;

  UINT16   ReservedS56;

  UINT16   ReservedS57;

  UINT8    ReservedS58;

  UINT16   ReservedS59;

  UINT8    ReservedS60;

  UINT16   ReservedS61;

  UINT8    ReservedS62;

  UINT8    ReservedS63;

  UINT16   ReservedS64;

  UINT16   ReservedS65;

  UINT8    ReservedS66;

  UINT8   ReservedS198 : 1,
          ReservedS199 : 1,
          ReservedS200 : 1,
          ReservedS119 : 1,
          ReservedS201 : 4;
  UINT8   ReservedS120;

  BOOLEAN               ReservedS128;           // Posted CSR access method is available when this is TRUE

  BOOLEAN               ReservedS130;

  UINT16                ReservedS131;

  BOOLEAN               ReservedS111;

  BOOLEAN               ReservedS202;

  UINT8                 ReservedS203;

  BOOLEAN               ReservedS204;

  BOOLEAN               ReservedS205;

  BOOLEAN               ReservedS206;

  UINT8                 ReservedS121;

  UINT8                 ReservedS122;

  UINT8                 ReservedS123;

  UINT8                 ReservedS124;

  UINT8                 ReservedS125;

  UINT8                 ReservedS126;

  UINT8                 ReservedS127;

  BOOLEAN               ReservedS137;

  UINT8                 SenseAmpCalibHwSwOption;

  BOOLEAN               ReservedS129;

  UINT8   ReservedS207    :4,
          ReservedS208 :1,
          ReservedS209 :1,
          ReservedS210           :2;

  UINT8   ReservedS143;

  UINT8   ReservedS144;

  UINT8   ReservedS145;

  UINT8   ReservedS211    :4,
          ReservedS212 :1,
          ReservedS213           :3;

  UINT8   ReservedS148;


  UINT8   ReservedS149;

} RESERVED_S193;

///
/// DIMM enable/disable information
//
struct ddrDimmSetup {

  ///
  /// Setting for each DIMM to be mapped out.
  //
  UINT8             mapOut[MAX_RANK_DIMM];
};

///
/// Channel setup structure declaration
//
struct ddrChannelSetup {
  UINT8               enabled;            /// Channel enable switch.
  UINT8               numDimmSlots;       /// Number of DIMM slots per channel.
  UINT8               batterybacked;      /// ADR Battery backed or not.
  UINT8               rankmask;           /// Rank mask. 0 = disable; 1 = enable.
  struct ddrDimmSetup dimmList[MAX_DIMM]; /// DIMM enable/disable information.
};

///
/// PPR DRAM Address.
//
typedef struct {
  UINT8     dimm;
  UINT8     rank;
  UINT8     subRank;
  UINT32    nibbleMask;
  UINT8     bank;
  UINT32    row;
} PPR_ADDR;

///
/// PPR Address, buffer to hold DRAM Address that need to be repaired.<BR>
//
typedef struct {
  UINT8     pprAddrStatus;
  UINT8     socket;
  UINT8     mc;
  UINT8     ch;
  PPR_ADDR  pprAddr;
} PPR_ADDR_MRC_SETUP;


///
/// Socket setup structure declaration.
//
struct ddrSocketSetup {

  ///
  /// iMC enable/disable switch.
  //
  UINT8                   enabled;

  ///
  /// Bit-mapped options per socket.
  //
  UINT8                   options;

  ///
  /// Platform configuration for each channel.
  //
  struct ddrChannelSetup  ddrCh[MAX_CH];

  ///
  /// Enable/Disable memory controller.
  //
  UINT8                   imcEnabled[MAX_IMC];
};

///
/// Define AdvMemTest Rank List item
///  The input format is defined as follows:
///  Rank number in bits[3:0]
///  DIMM number in bits[7:4]
///  Channel number in the MC in bits[11:8]
///  MC number in bits[15:12]
///  Socket number in bits [19:16]
///  bits [31:20] are reserved
///  For example:
///  To test MC 0, CH 1, DIMM 0, RANK 0 on Socket 0, you need to enter a value of 0x100
///  To test MC 1, CH 0, DIMM 0, RANK 0 on Socket 0, you need to enter a value of 0x1000
//
typedef union {
  struct {
    UINT32   Rank: 4;
    UINT32   Dimm: 4;
    UINT32   Channel: 4;
    UINT32   Mc: 4;
    UINT32   Socket: 4;
    UINT32   rsvd: 12;
  } Bits;
  UINT32  Data;
} AdvMemTestRankData;

///
/// Host memory setup structure declaration.
//
struct memSetup {

  ///
  /// @brief
  /// Flags for enabling (1)/disabling(0) MRC features.<BR>
  /// @details
  /// TEMPHIGH_EN                   BIT0, enables support for 95 degree DIMMs.<BR>
  /// ATTEMPT_FAST_BOOT_COLD        BIT1.<BR>
  /// PDWN_SR_CKE_MODE              BIT2, enables CKE to be tri-stated during
  ///                                     register clock off power down self-refresh.<BR>
  /// OPP_SELF_REF_EN               BIT3, enables the opportunistic self refresh mechanism.<BR>
  /// MDLL_SHUT_DOWN_EN             BIT4, enables MDLL shutdown.<BR>
  /// PAGE_POLICY                   BIT5, Clear for open page, set for closed page. Open page
  ///                                     has better performance and power usage in general.
  ///                                     Close page may benefit some applications with poor <BR>
  ///                                     locality.<BR>
  /// ALLOW2XREF_EN                 BIT6, enables 2X refresh if needed for extended operating
  ///                                     temperature range (95degrees) If TEMPHIGH_EN is also
  ///                                     set, setting this bit will result in 2X refresh timing
  ///                                     for the IMC refresh control register. <BR>
  /// MULTI_THREAD_MRC_EN           BIT7, enables multithreaded MRC. This reduces boot time for
  ///                                     systems with multiple processor sockets.<BR>
  /// ADAPTIVE_PAGE_EN              BIT8, enables adaptive page mode. The memory controller will
  ///                                     dynamically determine how long to keep pages open<BR>
  ///                                     to improve performance.<BR>
  /// CMD_CLK_TRAINING_EN           BIT9, enables command to clock training step.<BR>
  /// SCRAMBLE_EN                   BIT10, set to enable data scrambling. This should always be
  ///                                      enabled except for debug purposes.<BR>
  /// SCRAMBLE_EN_DDRT              BIT11, set to enable data scrambling. This should always be
  ///                                      enabled except for debug purposes.<BR>
  /// DISPLAY_EYE_EN                BIT12,<BR>
  /// DDR_RESET_LOOP                BIT13, enables infinite channel reset loop without retries
  ///                                      for gathering of margin data.<BR>
  /// NUMA_AWARE                    BIT14, enables configuring memory interleaving appropriately
  ///                                      for NUMA aware OS.<BR>
  /// DISABLE_WMM_OPP_READ          BIT15, disables issuing read commands opportunistically during WMM.<BR>
  /// RMT_COLD_FAST_BOOT            BIT16. <BR>
  /// ECC_CHECK_EN                  BIT17, enables ECC checking.<BR>
  /// ECC_MIX_EN                    BIT18, enables ECC in a system with mixed ECC and non-ECC memory in a
  ///                                      channel by disabling ECC when this configuration is detected.<BR>
  /// DISABLE_ECC_SUPPORT           BIT19, disables ECC check.<BR>
  /// CA_PARITY_EN                  BIT20, <BR>
  /// PER_NIBBLE_EYE_EN             BIT22.
  /// RAS_TO_INDP_EN                BIT23, switches from lockstep or mirror mode to independenct channel
  ///                                      mode when memory is present on channel 2 and this is enabled.<BR>
  /// MARGIN_RANKS_EN               BIT25, enables the rank margin tool.<BR>
  /// MEM_OVERRIDE_EN               BIT26, enables use of inputMemTime inputs as hard overrides.<BR>
  /// DRAMDLL_OFF_PD_EN             BIT27, <BR>
  /// MEMORY_TEST_EN                BIT28, enables execution of MemTest if on cold boot<BR>
  /// MEMORY_TEST_COLD_FAST_BOOT_EN BIT29, enables the memory test when going through a cold fast boot
  ///                                      path<BR>
  /// ATTEMPT_FAST_BOOT             BIT30, attempts to take a fast boot path if the NVRAM structure is good
  ///                                      and the memory config hasn't changed.  For example, on a warm boot,
  ///                                      this will take the "fast warm" path through MRC which attempts
  ///                                      to make it as close as possible to the S3 path.<BR>
  /// SW_MEMORY_TEST_EN             BIT31.  <BR>
  //
  UINT32                options;

  ///
  /// @brief
  /// Flags for enabling (1)/disabling(0) MRC features.<BR>
  /// @details
  ///   PD_CRC_CHECK              BIT0<BR>
  ///   SET_MEM_TESTED_EN         BIT1<BR>
  ///   AVAILABLE                 BIT2<BR>
  ///   TURNAROUND_OPT_EN_DDRT    BIT3<BR>
  ///   PDA_EN                    BIT5<BR>
  ///   TURNAROUND_OPT_EN         BIT6<BR>
  ///   AVAILABLE                 BIT7<BR>
  ///   ALLOW_CORRECTABLE_ERROR   BIT8<BR>
  ///   ALLOW_CORRECTABLE_MEM_TEST_ERROR   BIT9<BR>
  ///   AVAILABLE                 BIT10<BR>
  ///   AVAILABLE                 BIT11<BR>
  ///   AVAILABLE                 BIT12<BR>
  ///   PER_BIT_MARGINS           BIT13<BR>
  ///   DUTY_CYCLE_EN             BIT14<BR>
  ///   LRDIMM_BACKSIDE_VREF_EN   BIT15<BR>
  ///   AVAILABLE                 BIT16<BR>
  ///   DRAM_RX_EQ_EN             BIT17<BR>
  ///   AVAILABLE                 BIT18<BR>
  ///   AVAILABLE                 BIT19<BR>
  ///   AVAILABLE                 BIT20<BR>
  ///   OPTIONS_EXT_RESERVED1     BIT21<BR>
  ///   AVAILABLE                 BIT22<BR>
  ///   WR_CRC                    BIT23<BR>
  ///   OPTIONS_EXT_RESERVED2     BIT24<BR>
  ///   AVAILABLE                 BIT25<BR>
  ///   AVAILABLE                 BIT26<BR>
  ///   AVAILABLE                 BIT27<BR>
  ///   AVAILABLE                 BIT28<BR>
  ///   DIMM_ISOLATION_EN         BIT29<BR>
  ///   AVAILABLE                 BIT30<BR>
  //
  UINT32                optionsExt;

  ///
  /// @brief
  /// NGN Flags.
  /// @details
  ///  NGN_CMD_TIME               BIT1<BR>
  ///  NGN_DEBUG_LOCK             BIT6<BR>
  ///  NGN_ARS_ON_BOOT            BIT7<BR>
  ///  NGN_ARS_PUBLISH            BIT9<BR>
  ///  NGN_ECC_EXIT_CORR          BIT10<BR>
  ///  NGN_ECC_CORR               BIT11<BR>
  ///  NGN_ECC_WR_CHK             BIT12<BR>
  ///  NGN_ECC_RD_CHK             BIT13<BR>
  //
  UINT32                optionsNgn;


  ///
  /// @brief
  /// PDA behavior for x16 devices.<BR>
  /// @details
  ///  0 - will disable PDA operation when a x16 device is detected.<BR>
  ///  1 - will not modify PDA Mode.<BR>
  //
  UINT8                 PdaModeX16;

  ///
  /// @brief
  /// IMC BCLK frequency. <BR>
  /// @details
  /// 0 - Auto, MRC code determine the value.<BR>
  /// 1 - 100MHz.<BR>
  /// 2 - 133MHz.<BR>
  //
  UINT8                 imcBclk;

  ///
  /// @brief
  /// Enforce memory POR configurations.<BR>
  /// @details
  ///   0 (ENFORCE_POR_EN)      - Enforce memory POR.<BR>
  ///   1 (ENFORCE_STRETCH_EN)  - Enforce memory frequency stretch goal.<BR>
  ///   2 (ENFORCE_POR_DIS)     - Do not enforce POR configurations.<BR>
  //
  UINT8                 enforcePOR;

  ///
  /// @brief
  /// DDR Frequency Limit.
  /// @details
  /// Forces a DDR frequency slower than the common tCK detected via SPD.<BR>
  /// A DDR frequency faster than the common frequency is a config error.<BR>
  /// Options are 0=AUTO, 1=DDR_800, 3=DDR_1066, 5=DDR_1333, 7=DDR_1600, <BR>
  /// 9=DDR_1866, 11=DDR_2133, 13=DDR2400.<BR>
  //
  UINT8                 ddrFreqLimit;

  ///
  /// @brief
  /// Channels interleave setting.
  /// @details
  /// Valid options are 1, 2, or 3 way interleave. Other values defaults
  /// to 3 ways interleave.<BR>
  //
  UINT8                 chInter;

  ///
  /// @brief
  /// DIMM types.<BR>
  /// @details
  /// 0=RDIMM, 1=UDIMM, 2 = RDIMMandUDIMM or SODIMM, 9=LRDIMM; 10=QRDIMM, 11=NVMDIMM.<BR>
  //
  UINT8                 dimmTypeSupport;

  ///
  /// @brief
  /// CKE Power managment mode.<BR>
  /// @details
  ///   0 = Disabled.<BR>
  ///   1 = APD Enabled, PPD Disabled.<BR>
  ///   2 = APD Disabled, PPDF Enabled.<BR>
  ///   3 = APD Disabled, PPDS Enabled.<BR>
  ///   4 = APD Enabled, PPDF Enabled.<BR>
  ///   5 = APD Enabled, PPDS Enabled.<BR>
  //
  UINT8                 ckeThrottling;

  ///
  /// @brief
  /// Open Loop Thermal Throttling.
  /// @details
  /// (value/100) * 255 / max number of dimms per channel = DIMM_TEMP_THRT_LMT THRT_HI.<BR>
  //
  UINT8                 olttPeakBWLIMITPercent;

  ///
  /// @brief
  /// Bitmapped field for Thermal Throttling Modes.
  /// @details
  /// Defined in mem.thermalThrottlingOptions section.<BR>
  //
  UINT16                thermalThrottlingOptions;

  ///
  /// @brief
  /// Option to manualy enter Temperature refresh value.
  /// @details
  /// Select Manual to use value from HalfxRefreshValue, TwoxRefreshValue and
  /// FourxRefreshValue. Auto for default value in MRC code.<BR>
  ///   0 = Auto.<BR>
  ///   1 = Manual option select.<BR>
  //
  UINT8                 TempRefreshOption;

  ///
  /// Half X temperature refresh value.
  //
  UINT8                 HalfxRefreshValue;

  ///
  /// Two X temperature refresh value.
  //
  UINT8                 TwoxRefreshValue;

  ///
  /// Four X temperature refresh value.
  //
  UINT8                 FourxRefreshValue;

  //
  // Receive Enable Average Feature
  //
  BOOLEAN               RecEnDelayAverage;

  ///
  /// @brief
  /// Thermal Throttling O/P bits - (High | Mid | Low).<BR>
  /// @details
  /// 0= Memhot output disabled,<BR>
  /// 1 = Memhot on High,<BR>
  /// 2 = Memhot on High|Mid, <BR>
  /// 3 = Memhot on High|Mid|Low.<BR>
  //
  UINT8                 MemHotOuputAssertThreshold;

  ///
  /// @brief
  /// Enable/Disable the initialization of THRTMID on TEMPLO.<BR>
  /// @details
  /// 0 = THRTMID on TEMPLO disabled,<BR>
  /// 1 = THRTMID on TEMPLO enabled.<BR>
  //
  UINT8                 ThrottlingMidOnTempLo;

  ///
  /// @brief
  /// Enable/Disable DRAM RAPL.<BR>
  /// @details
  /// 0 - disable.<BR>
  /// 1 - enable.<BR>
  //
  UINT8                 DramRaplEnable;

  ///
  /// Multipler of BW_LIMIT_TF when DRAM RAPL is enabled.
  //
  UINT8                 dramraplbwlimittf;

  ///
  /// @brief
  /// Notify PCU to enable/disable DRAM PM of memory controller.<BR>
  /// @details
  /// 0 - Disable.<BR>
  /// 1 - Enable.<BR>
  //
  UINT8                 CmsEnableDramPm;

  ///
  /// @brief
  /// DRAM RAPL Refresh Base.
  /// @details
  /// Allows custom tuning of Power scaling by Refresh rate in units of 0.1x
  /// when DRAM RAPL is enabled.<BR>
  //
  UINT8                 dramraplRefreshBase;

  ///
  /// Enable disable per Bit DeSkew Training.
  //
  UINT8                 perBitDeSkew;

  UINT8                   ReservedS214;
  UINT8                   ReservedS215;
  UINT8                   ReservedS86;
  UINT8                   ReservedS216;
  UINT8                   ReservedS217;
  UINT8                   ReservedS218;
  UINT8                   ReservedS219;
  UINT8                   ReservedS220;
  UINT8                   ReservedS221;
  UINT8                   ReservedS222;
  UINT8                   ReservedS223;
  UINT8                   ReservedS224;
  UINT8                   ReservedS225;
  UINT8                   ReservedS226;
  UINT8                   ReservedS227;
  UINT8                   ReservedS228;
  UINT8                   ReservedS229;
  UINT8                   ReservedS230;
  UINT8                   ReservedS231;
  UINT8                   ReservedS232;
  UINT8                   ReservedS233;

  ///
  /// NVDIMM Factory Reset Clear
  //
  UINT8                   FactoryResetClear;

  ///
  /// Enable Backside RMT.
  //
  UINT8                 enableBacksideRMT;

  ///
  /// Enable Backside CMD RMT.
  //
  UINT8                 enableBacksideCMDRMT;

  ///
  /// Enable NVMDIMM BCOM margining support.
  //
  UINT8                 enableNgnBcomMargining;

  ///
  /// @brief
  /// Training Result Offset function enable or disable.
  /// @details
  /// It controls whether to enable the function to offset the final training results or not.<BR>
  ///   Enable - Enables training results to be offset.  <BR>
  ///   Disable - Disables this feature; current default is Enable disable<BR>

  //
  UINT8                 trainingResultOffsetFunctionEnable;

  ///
  /// Platform value to offset the final memory training result of TxDq.
  //
  INT16                 offsetTxDq;

  ///
  /// Platform value to offset the final memory training result of RxDq.
  //
  INT16                 offsetRxDq;

  ///
  /// Platform value to offset the final memory training result of TxVref.
  //
  INT16                 offsetTxVref;

  ///
  /// Platform value to offset the final memory training result of RxVref.
  //
  INT16                 offsetRxVref;

  ///
  /// Platform value to offset the final memory training result of CmdAll.
  //
  INT16                 offsetCmdAll;

  ///
  /// Platform value to offset the final memory training result of CmdVref.
  //
  INT16                 offsetCmdVref;

  ///
  /// Platform value to offset the final memory training result of CtlAll.
  //
  INT16                 offsetCtlAll;

  ///
  /// Platform value to offset the final memory training result of RecvEn.
  //
  INT16                 OffsetRecEn;

  ///
  /// Rank Margin Test: patten length.
  //
  UINT32                rmtPatternLength;

  ///
  /// Rank Margin Test: patten length extension.
  //
  UINT32                rmtPatternLengthExt;

  ///
  /// Memory RAS: Specifies the number of hours it takes for patrol scrub to scrub all system memory
  //
  UINT32                patrolScrubDuration;

  ///
  /// Enable/Disable Memory RAS die sparing.
  //
  UINT8                 DieSparing;

  ///
  /// Memory RAS: Address Range Scrubbing
  //
  UINT8                 NgnAddressRangeScrub;

  ///
  /// Number of MemTests loops to execute for legacy MemTest (type 8 and 10), that
  /// provides the ability of inverting the data pattern in every odd pass for detecting
  /// opposite polarity faults
  //
  UINT16                memTestLoops;

  ///
  /// CPGC MemTest step bit fields to enable different advanced MemTest options
  //
  UINT32                AdvMemTestOptions;

  ///
  /// Enable/Disable PPR repair during Advanced Memtest
  //
  UINT8                 AdvMemTestPpr;

  ///
  /// Retry the Advanced Memtest step after a PPR repair occurs
  /// This option is useful for testing that the PPR repair was successful, but it adds some latency
  //
  UINT8                 AdvMemTestRetry;

  ///
  /// Reset row fail list after executing each Advanced MemTest option
  /// This option is useful for testing multiple options.
  //
  UINT8                 AdvMemTestResetList;

  ///
  /// Set Test Conditions for Advanced Memtest algorithms
  /// ADV_MEM_TEST_COND_DISABLE - Do not modify test conditions during Advanced Memtest
  /// ADV_MEM_TEST_COND_AUTO    - Modify test conditions automatically based on Advanced Memtest algorithm
  /// ADV_MEM_TEST_COND_MANUAL  - Modify test conditions manually based on AdvMemTestCond input options
  //
  UINT8                 AdvMemTestCondition;

  ///
  /// Manually set Vdd level when AdvMemTestCondition = ADV_MEM_TEST_COND_MANUAL
  /// Specify Vdd in units of mV
  //
  UINT16                AdvMemTestCondVdd;

  ///
  /// Manually set host Write Recovery time when AdvMemTestCondition = ADV_MEM_TEST_COND_MANUAL
  /// Specify host tWR value in units of tCK. This timing is only applicable in Open Page mode.
  //
  UINT8                 AdvMemTestCondTwr;

  ///
  /// Manually set host tREFI time when AdvMemTestCondition = ADV_MEM_TEST_COND_MANUAL
  /// Specify host tREFI in units of usec. 7800 = 1x refresh rate; 15600 = 0.5x refresh rate
  //
  UINT16                AdvMemTestCondTrefi;

  ///
  /// Manually set Pause time without refresh when AdvMemTestCondition = ADV_MEM_TEST_COND_MANUAL
  /// Specify the Pause time in units of msec. It is applied between write and read steps to test data retention.
  //

  UINT32                AdvMemTestCondPause;
  ///
  /// Indicate the number of Ranks that will be tested in the system. A value of 0 will test all Ranks
  ///
  //
  UINT8                   AdvMemTestRankListNumEntries;

  ///
  /// The list of Rank addresses in the sysem that will execute AdvMemTest
  //
  AdvMemTestRankData       AdvMemTestRankList[ADV_MT_LIST_LIMIT];

  UINT8                 Reserved0;
  UINT8                 Reserved1;
  UINT8                 Reserved2;
  UINT32                Reserved3;
  UINT32                Reserved4;
  UINT8                 Reserved5;
  UINT8                 Reserved6;
  ///
  /// Low 16 bits of the data scrambling seed.
  //
  UINT16                scrambleSeedLow;

  ///
  /// High 16 bits of the data scrambling seed
  //
  UINT16                scrambleSeedHigh;

  ///
  /// @brief
  /// ADR: Enable/Disable Async DRAM Refresh(ADR) feature<BR>
  /// @details
  ///   0 - Disable.<BR>
  ///   1 - Enable.<BR>
  //
  UINT8                 ADREn;

  ///
  /// @brief
  /// ADR: Enable/Dsiable Legacy ADR Async DRAM Refresh(ADR) feature<BR>
  /// @details
  ///   0 - Disable.<BR>
  ///   1 - Enable.<BR>
  //
  UINT8                 LegacyADRModeEn;

  ///
  /// @brief
  /// ADR: Minimum memory size assigned as system memory when only JEDEC NVDIMMs are present<BR>
  /// @details
  ///   2 - 2GB.<BR>
  ///   4 - 4GB.<BR>
  ///   6 - 6GB.<BR>
  ///   8 - 8GB.<BR>
  //
  UINT8                 MinNormalMemSize;

  ///
  /// @brief
  /// ADR: Data Save Mode for ADR. <BR>
  /// @details
  ///   0=Disabled, <BR>
  ///   1=Batterybacked, <BR>
  ///   2=NVDIMM.<BR>
  //
  UINT8                 ADRDataSaveMode;

  ///
  /// @brief
  /// ADR: Use the PCH_PM_STS register as ADR recovery indicator.<BR>
  /// @details
  ///   0 - Disable.<BR>
  ///   1 - Enable.<BR>
  //
  UINT8                 check_pm_sts;

  ///
  /// @brief
  /// ADR: Use the PlatformDetectADR OEM hook function as ADR recovery indicator.<BR>
  /// @details
  ///   0 - Disable.<BR>
  ///   1 - Enable.<BR>
  //
  UINT8                 check_platform_detect;

  ///
  /// @brief
  /// Memory RAS: Normal operation duration within sparing interval.
  //
  UINT16                normOppIntvl;

  ///
  /// @brief
  /// SM Bus Clock Frequency- see SMB_CLOCK_FREQUENCY.<BR>
  /// @details
  ///   0 - SMB_CLK_100K.<BR>
  ///   1 - SMB_CLK_400K. <BR>
  ///   2 - SMB_CLK_700K.<BR>
  ///   3 - SMB_CLK_1M.<BR>
  //
  SMB_CLOCK_FREQUENCY   SpdSmbSpeed;

  ///
  /// @brief
  /// Enable(1)/Disable(0) SPD data Print.<BR>
  /// @details
  ///   0 - Disable.<BR>
  ///   1 - Enable.<BR>
  //
  UINT8                 SpdPrintEn;

  ///
  /// @brief
  /// Pirnt length of SPD data.<BR>
  /// @details
  ///   0 - AUTO(512 for DDR4, 1024 for DDR5).<BR>
  ///   256.<BR>
  ///   512.<BR>
  ///
  UINT16                SpdPrintLength;

  ///
  /// Socket setup configuration.
  //
  struct ddrSocketSetup socket[MAX_SOCKET];

  ///
  /// Memory timing settings.
  //
  struct memTiming      inputMemTime;

  UINT32                XMPChecksum[MAX_SOCKET][MAX_CH][MAX_DIMM];

  UINT32                Reserved7;

  UINT32                Reserved8;

  UINT32                Reserved9;

  ///
  /// @brief
  /// Custom tuning multiplier of Refresh rate from 2.0x to 4.0x in units of 0.1x
  //
  UINT8                 customRefreshRate;

  ///
  /// @brief
  /// Enable Mirror on entire memory for TAD0.<BR>
  /// @details
  ///   0 - Disable.<BR>
  ///   1 - Enable.<BR>
  //
  UINT8                 partialmirrorsad0;

  ///
  /// Size of each partial mirror to be created in order.
  //
  UINT16                partialmirrorsize[MAX_PARTIAL_MIRROR];

  ///
  /// @brief
  /// Imitate behavior of UEFI based Address Range Mirror with setup option.
  /// @details
  /// It controls whether to enable partial mirror in 1LM and 2LM or not.
  //
  UINT8                 partialMirrorUEFI;

  ///
  /// @brief
  /// Numerator of the mirror ratio.<BR>
  /// @details
  /// Given the Numerator (N) and Denominator (D) returned by this function, and<BR>
  /// the total memory size (T), the mirror size (M) should be computed as follows:<BR>
  ///    M = (T * N) / D<BR>
  /// MirroredAmountAbove4GB is the amount of available memory above 4GB that needs to be mirrored
  /// measured in basis point (hundredths of percent e.g. 12.75% = 1275).
  /// In a multi-socket system, platform is required to distribute the mirrored memory ranges such that the
  /// amount mirrored is approximately  proportional to the amount of memory on each NUMA node. E.g. on
  /// a two node machine with 64GB on node 0 and 32GB on node 1, a request for 12GB of mirrored memory
  /// should be allocated with 8GB of mirror on node 0 and 4GB on node 1.<BR>
  UINT32                partialmirrorpercent;

  ///
  /// @brief
  /// Partial mirror status.<BR>
  /// @details
  ///   MIRROR_STATUS_SUCCESS                    0<BR>
  ///   MIRROR_STATUS_MIRROR_INCAPABLE           1<BR>
  ///   MIRROR_STATUS_VERSION_MISMATCH           2<BR>
  ///   MIRROR_STATUS_INVALID_REQUEST            3<BR>
  ///   MIRROR_STATUS_UNSUPPORTED_CONFIG         4<BR>
  ///   MIRROR_STATUS_OEM_SPECIFIC_CONFIGURATION 5<BR>
  //
  UINT8                 partialmirrorsts;

  ///
  /// Immediate failover enable or disable when mirror scrub reads a uncorrected error.
  //
  UINT8                 ImmediateFailoverAction;

  ///
  /// Number of times to loop through RMT to test the DLL Reset.
  //
  UINT8                 dllResetTestLoops;

  ///
  /// @brief
  /// Flags to enable(1)/disable(0) memory training steps in MRC flow.
  /// The following are bit to MRC training step map.<BR>
  /// @details
  ///   MF_X_OVER_EN        BIT0;<BR>
  ///   MF_SENSE_AMP_EN     BIT1;<BR>
  ///   MF_E_CMDCLK_EN      BIT2;<BR>
  ///   MF_REC_EN_EN        BIT3;<BR>
  ///   MF_RD_DQS_EN        BIT4;<BR>
  ///   MF_WR_LVL_EN        BIT5;<BR>
  ///   MF_WR_FLYBY_EN      BIT6;<BR>
  ///   MF_WR_DQ_EN         BIT7;<BR>
  ///   MF_CMDCLK_EN        BIT8;<BR>
  ///   MF_RD_ADV_EN        BIT9;<BR>
  ///   MF_WR_ADV_EN        BIT10;<BR>
  ///   MF_RD_VREF_EN       BIT11;<BR>
  ///   MF_WR_VREF_EN       BIT12;<BR>
  ///   MF_RT_OPT_EN        BIT13;<BR>
  ///   MF_RX_DESKEW_EN     BIT14;<BR>
  ///   MF_TX_DESKEW_EN     BIT14;<BR>
  ///   MF_TX_EQ_EN         BIT15;<BR>
  ///   MF_IMODE_EN         BIT16;<BR>
  ///   MF_EARLY_RID_EN     BIT17;<BR>
  ///   MF_DQ_SWIZ_EN       BIT18;<BR>
  ///   MF_LRBUF_RD_EN      BIT19;<BR>
  ///   MF_LRBUF_WR_EN      BIT20;<BR>
  ///   MF_RANK_MARGIN_EN   BIT21;<BR>
  ///   MF_E_WR_VREF_EN     BIT22;<BR>
  ///   MF_E_RD_VREF_EN     BIT23;<BR>
  ///   MF_L_RD_VREF_EN     BIT24;<BR>
  ///   MF_MEMINIT_EN       BIT25;<BR>
  ///   MF_NORMAL_MODE_EN   BIT27;<BR>
  ///   MF_CMD_VREF_EN      BIT28;<BR>
  ///   MF_L_WR_VREF_EN     BIT29;<BR>
  ///   MF_MEMTEST_EN       BIT30;<BR>
  ///   MF_E_CTLCLK_EN      BIT31.<BR>
  //
  UINT32                memFlows;

  ///
  /// @brief
  /// Extension of flags to enable(1)/disable(0) memory training steps in MRC flow.<BR>
  /// @details
  ///   MF_EXT_RX_CTLE_EN         BIT0<BR>
  ///   MF_EXT_PXC_EN             BIT1<BR>
  ///   MF_EXT_CMD_NORM_EN        BIT2<BR>
  ///   MF_EXT_LRDIMM_BKSIDE_EN   BIT3<BR>
  ///   MF_EXT_CHECK_POR          BIT6<BR>
  ///   MF_EXT_MMRC_RUN           BIT7<BR>
  ///   MF_EXT_THROTTLING_EARLY   BIT8<BR>
  ///   MF_EXT_THROTTLING         BIT9<BR>
  ///   MF_EXT_POST_TRAINING      BIT10<BR>
  ///   MF_EXT_E_CONFIG           BIT11<BR>
  ///   MF_EXT_L_CONFIG           BIT12<BR>
  ///   MF_EXT_MCODT_EN           BIT14<BR>
  ///   MF_EXT_MCRON_EN           BIT15<BR>
  ///   MF_EXT_DIMMRON_EN         BIT16<BR>
  ///   MF_EXT_CACLK_BACKSIDE_EN  BIT17<BR>
  ///   MF_DQ_SWIZ_X16_EN         BIT18<BR>
  ///   MF_EXT_TCO_COMP_EN        BIT19<BR>
  ///   MF_EXT_TX_SLEW_RATE_EN    BIT20<BR>
  ///   MF_EXT_INIT_MEM_EN        BIT21<BR>
  ///   MF_EXT_CMD_TX_EQ_EN       BIT22<BR>
  ///   MF_EXT_RCOMP_STAT_LEG     BIT23<BR>
  ///   MF_EXT_DDJC_EN            BIT24<BR>
  ///   MF_EXT_RX_DFE_EN          BIT25<BR>
  ///   MF_EXT_CSCLK_EN           BIT26<BR>
  ///   MF_EXT_CSCLK_BACKSIDE_EN  BIT27<BR>
  ///   MF_EXT_CACLK_EN           BIT28<BR>
  ///   MF_X_OVER_HWFSM_EN        BIT29<BR>
  ///   MF_EXT_INIT_CMI_EN        BIT30<BR>
  ///   MF_EXT_QxCA_CLK_EN        BIT31<BR>
  //
  UINT32                memFlowsExt;

  //
  // Addtional storage for memory flows
  //

  UINT32                memFlowsExt2;
  UINT32                memFlowsExt3;

  ///
  /// @brief
  /// Write Preamble timing.<BR>
  /// @details
  ///   0 = PREAMBLE_1TCLK;<BR>
  ///   1 = PREAMBLE_2TCLK;<BR>
  ///   2 = PREAMBLE_3TCLK;<BR>
  ///   3 = PREAMBLE_4TCLK.<BR>
  //
  UINT8                 writePreamble;

  ///
  /// @brief
  /// Read Preamble timing.<BR>
  /// @details
  ///   0 = PREAMBLE_1TCLK;<BR>
  ///   1 = PREAMBLE_2TCLK;<BR>
  ///   2 = PREAMBLE_3TCLK;<BR>
  ///   3 = PREAMBLE_4TCLK.<BR>
  //
  UINT8                 readPreamble;

  ///
  /// Enable extended range for DRAM RAPL.
  //
  UINT8                 DramRaplExtendedRange;

  ///
  /// @brief
  /// Memory RAS: Threshold value for logging Correctable Errors(CE).
  /// @details
  /// Threshold of 10 logs 10th CE, "All" logs every CE, and "None"
  /// means no CE logging. All and None are not valid with Rank Sparing.
  //
  UINT16                spareErrTh;

  ///
  /// Memory RAS: Enable/Disable New 48B SDDC.<BR>
  //
  UINT8                 NsddcEn;


  ///
  /// Memory RAS: Enable/Disable enhanced sddc.<BR>
  //
  UINT8                 EsddcEn;

  ///
  /// Disable - Turns ON Column Correction feature. Enable - Turns OFF Column Correction feature
  //
  UINT8                 ColumnCorrectionDisable;

  ///
  /// Memory RAS: Enable/Disable leaky bucket time window based interface.<BR>
  //
  UINT8                 leakyBktTimeWindow;

  ///
  /// Leaky bucket low mask position.
  //
  UINT8                 leakyBktLo;

  ///
  /// Leaky bucket high mask position.
  //
  UINT8                 leakyBktHi;

  ///
  /// Leaky bucket time window based interface Hour(0 - 3744).
  //
  UINT16                leakyBktHour;

  ///
  /// Leaky bucket time window based interface Minute" (0 - 60).
  //
  UINT8                 leakyBktMinute;

  ///
  /// Number of spare ranks  per channel.
  //
  UINT8                 spareRanks;

  ///
  /// Controls if NVDIMMs are interleaved together or not.
  //
  UINT8                 interNVDIMMS;

  ///
  /// Control if BIOS will perform NVDIMM Restore operation.
  //
  UINT8                 restoreNVDIMMS;

  ///
  /// Control if BIOS will perform NVDIMM erase & ARM operations.
  //
  UINT8                 eraseArmNVDIMMS;

  ///
  /// Cmd setup percent offset for late cmd traning result. The possible
  /// values are from 0 to 100.
  //
  UINT8                 cmdSetupPercentOffset;

  ///
  /// @brief
  /// Memory RAS. Power-up DDR4 Post Package Repair (PPR) type.<BR>
  /// @details
  /// 0 - PPR disabled.<BR>
  /// 1 - PPR type hard.<BR>
  /// 2 - PPR type soft.<BR>
  //
  UINT8                 pprType;

  ///
  /// @brief
  /// PPR Address.
  /// @details
  /// Buffer to hold DRAM Address that need to be repaired by PPR (Post Package Repair).<BR>
  /// Platform Sample Implementation: <BR>
  /// RAS code uses pprAddrSetup to cause MRC to launch PPR (Post Package Repair) on
  /// a subsequent boot. RAS code passes failed DRAM information into pprAddrSetup
  /// via the UEFI variable PPR_ADDR_VARIABLE.
  //
  PPR_ADDR_MRC_SETUP    pprAddrSetup[MAX_PPR_ADDR_ENTRIES];

  ///
  /// IMC interleave setting (within a socket). Valid options are 1 or 2 way interleave.
  //
 UINT8                 imcInter;

  ///
  /// Enable/Disable support for JEDEC RCD v2.0+ One Rank Timing Mode.
  //
  UINT8                 oneRankTimingModeEn;

  struct ReservedS193    ReservedS193;

  ///
  /// @brief
  /// Volatile Memory Mode
  /// @details
  ///   0 - 1LM;<BR>
  ///   1 - 2LM;<BR>
  //
  UINT8                 volMemMode;

  ///
  /// For 2LM, the caching type. Only valid if volMemMode is 2LM
  ///   0 - DDR caching DDRT.
  //
  UINT8                 CacheMemType;

  ///
  /// @brief
  /// Size of channel DDR to use as 2LM cache.
  /// @details
  /// Size of channel DDR to use as 2LM cache when Volatile Memory Mode
  /// under Crystal Ridge is 1LM+2LM.
  //
  UINT8                 DdrCacheSize;

  ///
  /// Caching contorl for AppDirect.
  //
  UINT8                 PmemCaching;

  ///
  /// eADR support.
  //
  UINT8                 EadrSupport;
  UINT8                 EadrCacheFlushMode;

  ///
  /// Enable or disable fADR support.
  //
  UINT8                 FadrSupport;

  ///
  /// Memory interleave mode for 1LM.
  //
  UINT8                 memInterleaveGran1LM;

  ///
  /// Enable or disable biased 2-way near memory cache.
  //
  UINT8                 EnableTwoWayNmCache;

  ///
  /// A 10-bit mask to control the bias counter ratio.
  //
  UINT16                NonPreferredWayMask;

  ///
  /// Reads are issued to the non-preferred or preferred way first.
  //
  UINT8                 PreferredReadFirst;

  ///
  /// Enable or disable boot-time fast zero memory support.
  //
  UINT8                 FastZeroMemSupport;

  ///
  /// Enable/Disable DDRT memory power saving.
  //
  UINT8                 DdrtMemPwrSave;

  ///
  /// @brief
  /// Memory RAS: Patrol Scrub Address Mode.<BR>
  /// @details
  /// Selects the address mode between System Physical Address (or) Reverse Address.<BR>
  ///   0 - PATROL_SCRUB_REVERSE_ADDR,<BR>
  ///   1 - PATROL_SCRUB_SPA, <BR>
  //
  UINT8                 patrolScrubAddrMode;

  ///
  /// @brief
  /// Self Refresh control programming.
  /// @details
  /// Memory power managment feature:<BR>
  /// Select manual or auto programming Self Refresh controls at Load Line point
  /// 0/1/2/3 registers.<BR>
  ///  0 - auto - MRC determines the value;<BR>
  ///  1 - manual - use value from user Setup.<BR>
  //
  UINT8                 SrefProgramming;

  ///
  /// @brief
  /// Opportunistic self-refresh setting.
  /// @details
  /// Memory power managment feature:<BR>
  /// opportunistic self-refresh setting in Self Refresh controls at Load Line point
  /// 0/1/2/3 registers.<BR>
  /// 0 - disable;<BR>
  /// 1 - enable.<BR>
  //
  UINT8                 OppSrefEn;

  ///
  /// @brief
  /// Master DLLs (MDLL) setting.
  /// @details
  /// Memory power managment feature:<BR>
  /// Master DLLs (MDLL) setting in Self Refresh controls at Load Line point 0/1/2/3 registers.<BR>
  /// When 0 - Master DLLs (MDLL) cannot be turned off in Self Refresh.<BR>
  /// When 1 - Master DLLs (MDLL) can be turned off in Self Refresh.<BR>
  //
  UINT8                 MdllOffEn;

  ///
  /// @brief
  /// Enables or disables Self Refresh in PkgC flow.<BR>
  /// @details
  /// Memory power managment feature.<BR>
  ///   0 - Didable.<BR>
  ///   1 - Enable.<BR>
  //
  UINT8                 PkgcSrefEn;

  ///
  /// @brief
  /// Configures CK behavior during self-refresh.<BR>
  /// @details
  ///   0    - CK is driven during self refresh.<BR>
  ///   2    - CK is pulled low during self refresh.<BR>
  //
  UINT8                 CkMode;

  ///
  /// @brief
  /// CKE Registers Programming Mode.
  /// @details
  /// Select manual or auto programming registers Control for CKE (DRAM powerdown modes).
  /// at Load Line point 0/1/23.<BR>
  ///  0 - auto - MRC determines the value.<BR>
  ///  1 - manual - use value from user Setup.<BR>
  //
  UINT8                 CkeProgramming;

  ///
  /// @brief
  /// CKE Idle Timer.
  /// @details
  /// Set the number of rank idle cycles that causes  CKE power-down entrance.
  /// The number of idle cycles (in DCLKs) are based from command CS assertion.
  /// It is important to program this parameter to be greater than roundtrip
  /// latency parameter in order to avoid the CKE de-assertion sooner than data return.
  //
  UINT8                 CkeIdleTimer;

  ///
  /// @brief
  /// CKE Active Power Down Mode for DDR4 DIMMs.<BR>
  /// @details
  ///   0 = APD is disabled.<BR>
  ///   1 = APD is enabled.<BR>
  //
  UINT8                 ApdEn;

  ///
  /// @brief
  /// CKE Precharge Power Down (PPD).<BR>
  /// @details
  ///  0 = PPD is disabled.<BR>
  ///  1 = PPD is enabled.<BR>
  //
  UINT8                 PpdEn;

  ///
  /// @brief
  /// CKE Active Power Down Mode for DDR-T DIMMs.<BR>
  /// @details
  ///   0 = APD is disabled.<BR>
  ///   1 = APD is enabled.<BR>
  //
  UINT8                 DdrtCkeEn;

  ///
  /// @brief
  /// Turn off DDRIO data DLL in CKE Power Down or OppSR low power mode.<BR>
  /// @details
  ///   0 = Do not turn off data DLL.<BR>
  ///   1 = Turn off data DLL.<BR>
  //
  UINT8                DataDllOff;

  ///
  /// @brief
  /// RAS: Enable/Disable Extended ADDDC sparing.<BR>
  /// @details
  ///  0 = Disabled.<BR>
  ///  1 = Enabled.<BR>
  //
  UINT8                 ExtendedADDDCEn;

  ///
  /// @brief
  /// DDRT Defeature Enable/Disable BLOCK GNT2CMD1CYC.<BR>
  /// @details
  ///  0 = Disabled.<BR>
  ///  1 = Enabled.<BR>
  //
  UINT8                 Blockgnt2cmd1cyc;

  ///
  /// @brief
  /// Enable/Disable NVMDIMM OPPRD.<BR>
  /// @details
  ///  0 = DDRT RPQ Reads will not be scheduled in DDR4 mode DDRT Underfill Reads
  ///      will not be scheduled in DDR4 mode.<BR>
  ///  1 = DDRT RPQ Reads will be scheduled in DDR4 mode. GNTs continue to be blocked
  ///      in DDR4 mode. This should be set for DDRT 2N mode.DDRT Underfill Reads will
  ///      be scheduled in DDR4 mode. GNTs continue to be blocked in DDR4 mode This
  ///      bit should be set for DDRT 2N mod.<BR>
  //
  UINT8                 Disddrtopprd;

  UINT16                Reserved10;

  ///
  /// @brief
  /// NGNVM DIMM Secure Erase Unit, Erases the persistent memory region of the selected DIMMs".<BR>
  /// @details
  ///   0 - Erase DIMMs according to setting of setSecureEraseSktCh.<BR>
  //    1 - enable Erase All DIMMs, Erases the persistent memory region of all NVMDIMMs in the system".<BR>
  UINT8                 setSecureEraseAllDIMMs;

  ///
  /// @brief
  /// Enable/Disable secure erase of persistent memory region of NVMDIMM.<BR>
  /// @details
  ///   0 = Disable erasing the persistent memory region of NVMDIMM in <Channel 0, Memory controller 0, Socket 0.<BR>
  ///   1 = Enable erasing the persistent memory region of NVMDIMM in Channel 0, Memory controller 0, Socket 0.<BR>
  //
  UINT8                 setSecureEraseSktCh[MAX_SOCKET][MAX_CH];

  ///
  /// @brief
  /// Select Crystal Ridge FastGo QoS Configuration Profiles.<BR>
  /// @details
  ///   CR_FASTGO_DEFAULT                       0;<BR>
  ///   CR_FASTGO_DISABLE                       1;<BR>
  ///   CR_FASTGO_DISABLE_MLC_SQ_THRESHOLD_5    2;<BR>
  ///   CR_FASTGO_DISABLE_MLC_SQ_THRESHOLD_6    3;<BR>
  ///   CR_FASTGO_DISABLE_MLC_SQ_THRESHOLD_8    4;<BR>
  ///   CR_FASTGO_DISABLE_MLC_SQ_THRESHOLD_10   5;<BR>
  ///   CR_FASTGO_AUTOMATIC                     6;<BR>
  ///   CR_FASTGO_LAST_OPTION                   CR_FASTGO_AUTOMATIC;<BR>
  ///   CR_FASTGO_KNOB_DEFAULT                  CR_FASTGO_AUTOMATIC.<BR>
  //
  UINT8                 FastGoConfig;

  ///
  /// @brief
  /// Non-Volatile Memory DIMM baseline performance settings depending on the workload behavior.<BR>
  /// @details
  ///   0 = BW Optimized.<BR>
  ///   1 = Latency Optimized.<BR>
  //
  UINT8                 NvmdimmPerfConfig;

  ///
  /// @brief
  /// Memory topology of each channel per socket.<BR>
  /// @details
  ///   0 = DaisyChainTopology.<BR>
  ///   1 = InvSlotsDaisyChainTopology.<BR>
  ///   2 = TTopology.<BR>
  //
  EFI_MEMORY_TOPOLOGY_TYPE MemoryTopology[MAX_SOCKET][MAX_CH];

  ///
  /// @brief
  /// Memory connector type of each channel per socket.<BR>
  /// @details
  ///   0 = DimmConnectorPth.<BR>
  ///   1 = DimmConnectorSmt.<BR>
  ///   2 = DimmConnectorMemoryDown.<BR>
  //
  EFI_MEMORY_DIMM_CONNECTOR_TYPE MemoryConnectorType[MAX_SOCKET][MAX_CH];

  ///
  /// @brief
  /// Enable/Disable the App Direct Memory Hole.<BR>
  /// @details
  ///   0 = disable.<BR>
  ///   1 = enable.<BR>
  //
  UINT8                 AppDirectMemoryHole;

  ///
  /// @brief
  /// Enable/disable Latch System Shutdown (LSS) of all enabled NVDIMMs.<BR>
  /// @details
  /// LSS is supposed to be done by the persistent memory driver in OS using ACPI DSM function,
  /// before any write to persistent memory is done. BIOS knob is implemented to enable Latch LSS
  /// for operating systems that would not call DSM. Enabling latch twice is not a problem so the BIOS
  /// action does not colide with OSes that use DSM to enable latch.<BR>
  ///   0 = Disable.<BR>
  ///   1 = Enable.<BR>
  //
  UINT8                 LatchSystemShutdownState;

  ///
  /// @brief
  /// Select snoopy mode for 2LM.
  /// @details
  /// Set to 0 to Enables new 2LM specific feature to
  /// avoid directory updates to far-memory from non-NUMA optimized workloads.<BR>
  ///   0 = Enable eliminating directory in far memory.<BR>
  ///   1 = Disable eliminating directory in far memory.<BR>
  //
  UINT8                 EliminateDirectoryInFarMemory;

  ///
  /// @brief
  /// Power Cycle Policy on NVM Surprise Clock Stop.
  /// @details
  /// Enable/Disable power cycle policy when NVMDIMM receive surprise clock stop.
  //
  UINT8                 NvmdimmPowerCyclePolicy;

  ///
  /// @brief
  /// NV DIMM Energy Policy Management.<BR>
  /// @details
  ///   1 = Setting Energy Policy to Device Managed.<BR>
  ///   2 = Setting Energy Policy to Host Managed.<BR>
  //
  UINT8                 NvDimmEnergyPolicy;

  ///
  /// @brief
  /// Option to force Rx DFE enabled or disabled.<BR>
  /// @details
  ///  0 = Disable Rx DFE.<BR>
  ///  1 = Enable Rx DFE.<BR>
  ///  2 = Auto. MRC code detemines if enable or disable.<BR>
  //
  UINT8                 RxDfeEn;

  ///
  /// @brief
  /// Enable/Disable TX Rise Fall Slew Rate Training. <BR>
  /// @details
  ///   0 = Dsiable.<BR>
  ///   1 = Enable.<BR>
  ///   2 = AUTO, will enable if DDR Freq >= 2933.<BR>
  //
  UINT8                 TxRiseFallSlewRate;

  ///
  /// @brief
  /// Forces PXC (Phase-based Crosstalk Cancellation) initialization.
  /// @details
  /// Forces PXC (Phase-based Crosstalk Cancellation) initialization even if
  /// PXC training is not enabled.<BR>
  ///   0 = Disable.<BR>
  ///   1 = Enable.<BR>
  //
  UINT8                 ForcePxcInit;

  ///
  /// @brief
  /// CMI Initialize Option.<BR>
  /// @details
  ///   0 = Initialize with desired credit.<BR>
  ///   1 = Inialize with default(Reset Value)credit.<BR>
  //
  UINT8                 CmiInitOption;

  ///
  /// @brief
  /// Snoopy mode for AD.
  /// @details
  /// Snoopy mode for AD: Disable/Enable new AD specific feature to avoid directory
  /// updates to DDRT memory from non-NUMA optimized workloads.<BR>
  ///   0 = Disable.<BR>
  ///   1 = Enable.<BR>
  //
  UINT8                 DisableDirForAppDirect;

  ///
  /// @brief
  /// Enable/Disable Crystal Ridge MediaStatus Exception.<BR>
  /// @details
  /// 0 = Disable.<BR>
  /// 1 = Enable.<BR>
  //
  UINT8                 NvmMediaStatusException;

  ///
  /// @brief
  /// Select Crystal Ridge QoS tuning recipes.<BR>
  /// @details
  ///   0 = Enables tuning recipe 1 for CR QoS knobs <BR>
  ///       (recommended for 2-2-2 memory configuration in AD);<BR>
  ///   1 = Enables tuning recipe 2 for CR QoS knobs<BR>
  ///       (recommended for other memory configuration in AD);<BR>
  ///   2 = Enables tuning recipe 3 for CR QoS knobs <BR>
  ///       (recommended for 1 DIMM per channel config);<BR>
  ///   3 = Disable CR QoS feature.<BR>
  //
  UINT8                 NvmQos;

  ///
  /// @brief
  /// Disable/Enable using extended Type 17 SMBIOS Structures.<BR>
  /// @details
  ///   0 = Disable.<BR>
  ///   1 = Enable .<BR>
  //
  UINT8                 ExtendedType17;

  ///
  /// @brief
  /// Gen 2 Intel Optane DC Persistent Memory (DCPMM)  Average Power Limit (in mW)".
  /// @details
  /// Valid range for power limit starts from 10000mW and must be a multiple of 250mW."
  //
  UINT16                DcpmmAveragePowerLimit;

  ///
  /// @brief
  /// Gen 2 DCPMM Average Power Time Constant for Turbo Mode support (in mSec).
  /// @details
  /// This value is used as a base time window for power usage measurements.
  //
  UINT8                 DcpmmAveragePowerTimeConstant;


  ///
  /// @brief
  /// Gen 2 DCPMM Average Power Time Constant for Memory Bandwidth Boost Feature support(in mSec).
  /// @details
  /// This value is used as a base time window for power usage measurements.
  //
  UINT32                 DcpmmMbbAveragePowerTimeConstant;

  ///
  /// @brief
  /// Gen 2 DCPMM Turbo Mode/Memory Bandwidth Boost Feature Enable.<BR>
  /// @details
  ///   0 = Disable.<BR>
  ///   1 = Enable.<BR>
  //
  UINT8                 DcpmmMbbFeature;

  ///
  /// @brief
  /// DCPPM Power limit in mW  for Turbo Mode/Memory Bandwidth Boost Feature.
  /// @details
  /// DCPPM Power limit in mW used for limiting the Turbo Mode/Memory Bandwidth Boost power consumption (Valid
  /// range starts from 15000mW).
  //
  UINT16                DcpmmMbbMaxPowerLimit;

  ///
  /// @brief
  /// Select LSx (LSI/LSR/LSW) ACPI method implementation.<BR>
  /// @details
  ///   0 = Software SMI.<BR>
  ///   1 = ASL.<BR>
  //
  UINT8                 LsxImplementation;

  ///
  /// @brief
  /// Set Smbus maximum access time<BR>
  /// @details
  /// Maximum amount of time (ms) UEFI mgmt driver is allowed to use the SMBus.<BR>
  //
  UINT32                NvdimmSmbusMaxAccessTime;

  ///
  /// @brief
  /// Set Smbus release delay.<BR>
  /// @details
  /// Delay time (ms) before releasing after UEFI mgmt driver requests SMBus release.<BR>
  //
  UINT32                NvdimmSmbusReleaseDelay;

  ///
  /// @brief
  /// Controls Mailbox structures in the NFIT
  /// @details
  ///   0 - Publish Mailbox structures in the NFIT
  ///   1 - Do not publish Mailbox structures in the NFIT
  ///
  UINT8                 NfitPublishMailboxStructsDisable;

  ///
  /// @brief
  /// Enforce memory population POR configurations.<BR>
  /// @details
  ///   0 (ENFORCE_POPULATION_POR_DIS)                - Do not enforce memory population POR.<BR>
  ///   1 (ENFORCE_POPULATION_POR_ENFORCE_SUPPORTED)  - Enforce supported memory populations.<BR>
  ///   2 (ENFORCE_POPULATION_POR_ENFORCE_VALIDATED)  - Enforce validated memory populations.<BR>
  //
  UINT8                 EnforcePopulationPor;

  ///
  /// Configure Stagger Host Refresh feature
  ///
  UINT8                 TrefiPerChannel;
  UINT8                 TrefiNumofRank;
  UINT16                TrefiDelay;

  //
  // DFE Path Finding
  //
  UINT8       EnableTapSweep;

  INT8        DfeGainBias;

  INT8        Tap1Start;
  INT8        Tap1End;
  UINT8       Tap1Size;

  INT8        Tap2Start;
  INT8        Tap2End;
  UINT8       Tap2Size;

  INT8        Tap3Start;
  INT8        Tap3End;
  UINT8       Tap3Size;

  INT8        Tap4Start;
  INT8        Tap4End;
  UINT8       Tap4Size;

  ///
  /// @brief
  /// Training Comp Options Values.
  /// @details
  /// Options for issuing a Comp. cycle (RCOMP) at specific points in training.<BR>
  ///   0 - One RCOMP cycle only on PHY Init (MMRC Init);<BR>
  ///   1 - One RCOMP cycle after every JEDEC Init;<BR>
  ///   2 - One RCOMP cycle right before every training step;<BR>
  //
  UINT8       TrainingCompOptions;

  ///
  /// @brief
  /// Periodic Rcomp Control.
  /// @details
  /// Enable/Disable memory periodic Rcomp with PCU.<BR>
  ///   0 - Disable;<BR>
  ///   1 - Enable;<BR>
  ///   2 - Auto;<BR>
  //
  UINT8   PeriodicRcomp;

  ///
  /// @brief
  /// Periodic Rcomp Interval.
  /// @details
  /// Interval of periodic Rcomp controlled by PCU.<BR>
  //
  UINT8   PeriodicRcompInterval;

  ///
  /// @brief
  /// Use SMBUS for early MRW commands
  /// @details
  /// Option to require all MRW commands to be sent over SMBUS until QCA training is complete<BR>
  //
  BOOLEAN   UseSmbusForMrwEarly;

  ///
  /// @brief
  /// Enable/Disable AEP DIMM Not Supported Exception.<BR>
  /// @details
  /// 0 = Disable.<BR>
  /// 1 = Enable.<BR>
  //
  UINT8   AepNotSupportedException;

  //
  // Memory Boot Health Check
  //
  MEM_BOOT_HEALTH_CONFIG     MemBootHealthConfig;

  /// @brief
  /// Select between Panic/High Watermark of Auto or High or Low.<BR>
  /// @details
  /// 0 = Auto
  /// 1 = High<BR>
  /// 2 = Low<BR>
  UINT8   PanicWm;

  /// @brief
  /// Enable/Disable LRDIMM DB DFE.<BR>
  /// @details
  /// 0 - Disable;<BR>
  /// 1 - Pmem Only;<BR>
  /// 2 - All LRDIMM;<BR>
  UINT8   DataBufferDfe;

  ///
  /// @brief
  /// Enable/Disable Virtual NUMA.<BR>
  /// @details
  /// 0 - disable.<BR>
  /// 1 - enable.<BR>
  //
  UINT8                 VirtualNumaEnable;

  ///
  /// @brief
  /// Smart Test Key pattern.<BR>
  /// @details
  /// Option to enter the confidential key to be used<BR>
  //
  UINT32                smartTestKey;

  ///
  /// Enable RMT minimum margin check
  //
  BOOLEAN               RmtMinimumMarginCheckEnable;
}; // memSetup

///
/// Common Platform Settings of MRC.
///
struct commonSetup {
  ///
  /// @brief
  /// Flags for common platform settings.<BR>
  /// @details
  ///   PROMOTE_WARN_EN      BIT0  Enables warnings to be treated as fatal error.<BR>
  ///   PROMOTE_MRC_WARN_EN  BIT1  Enables MRC warnings to be treated as fatal error.<BR>
  ///   HALT_ON_ERROR_EN     BIT2  Enables errors to loop forever.<BR>
  ///   HALT_ON_ERROR_AUTO   BIT3  Auto reset with Maximum Serial port debug<BR>
  ///                              message level when fatal error is encountered.<BR>
  //
  UINT32  options;

  ///
  /// @brief
  /// MRC debug feature. It indicates if debug jumper is set. <BR>
  /// @details
  ///   0 - Debug jumper is not set.<BR>
  ///   1 - Debug jumper is set.<BR>
  //
  UINT8   debugJumper;

  ///
  /// @brief
  /// Specifies what level of debug messages will be sent to serial port.<BR>
  /// @details
  /// Available options are a bitfield where: <BR>
  ///   SDBG_MIN        BIT0;<BR>
  ///   SDBG_MAX        BIT1;<BR>
  ///   SDBG_TRACE      BIT2;<BR>
  ///   SDBG_MEM_TRAIN  BIT3 + SDBG_MAX;<BR>
  ///   SDBG_CPGC       BIT5;<BR>
  ///   SDBG_MINMAX     SDBG_MIN + SDBG_MAX.<BR>
  //
  UINT32  serialDebugMsgLvl;

  ///
  /// MRC debug feature: Enable/Disable serial port buffer.<BR>
  //
  UINT8   serialBufEnable;

  ///
  /// MRC debug feature: Enable/Disable serial port pipe.<BR>
  //
  UINT8   serialPipeEnable;

  ///
  /// MRC debug feature: Enable/Disable serial pipe compress.<BR>
  //
  UINT8   serialPipeCompress;

  ///
  /// @brief
  /// Maximum addressable memory supported by the platform.
  /// @details
  /// Skylake Processor supports up to 46-bit addressing. This input should be the
  /// total number of addressable bytes in 256MB units. (0x40000 for 46-bit
  /// and 0x1000 for 40-bit).
  //
  UINT32  maxAddrMem;

  ///
  /// User configurable IO port for post code which is traditionally located at 0x80.
  //
  UINT16  debugPort;

  ///
  /// 32-bit pointer to an optional OEM NVRAM image to be copied into the host NVRAM structure.
  ///
  UINT32  nvramPtr;

  ///
  /// 32-bit pointer to an optional OEM provided Host structure.
  //
  UINT32  sysHostBufferPtr;

  ///
  /// @brief
  /// Disable/Enable DDRT Transcator.<BR>
  /// @details
  ///   0 - Disable;<BR>
  ///   1 - Enable;<BR>
  //
  UINT8   ddrtXactor;

  UINT8   ReservedS3;

  UINT8   ReservedS7[MAX_B2P_MAILBOX_GROUPS];

  ///
  /// @brief
  /// Socktet configuration supported by platform
  /// @details
  ///   0 - SOCKET_UNDEFINED
  ///   1 - SOCKET_4S
  ///   2 - SOCKET_HEDT  High End Desktop
  ///   3 - SOCKET_1S
  ///   4 - SOCKET_1SWS  1 Socket Work Station
  ///   5 - SOCKET_8S
  ///   6 - SOCKET_2S
  //
  UINT8   SocketConfig;
};

///
/// Platform Setting for MRC.
//
typedef struct sysSetup {

  ///
  /// Memory technology related settings for MRC.
  //
  struct  memSetup     mem;

  ///
  /// Common platform settings  not related to memory techology.
  //
  struct  commonSetup  common;

  ///
  /// @brief
  /// WFR Uncore GV Rate Reduction.<BR>
  /// @details
  ///   AUTO: Enable if WFR socket is detected in system.<BR>
  ///   Enabled: Always enables WFR Uncore GV Rate Reduction.<BR>
  //
  UINT8   WFRWAEnable;

  ///
  /// Enable/Disable Pmax through BIOS to Pcode Mailbox.
  //
  UINT8   PmaxDisable;

  UINT8 Pci64BitResourceAllocation; // TODO - This is IIO related, they need to stop relying on MRC structures


  ///
  /// Whether of not we should recover from ADR.
  //

  UINT32 AdrEvent;

} SYS_SETUP;

#pragma pack(pop)

///
/// Revison of MEMORY_POLICY_PPI.
//
#define MEMORY_POLICY_PPI_REVISION 0x00000001

///
/// Memory Policy PPI Definition.
//
typedef struct _MEMORY_POLICY_PPI {

  ///
  /// Revision of this PPI.
  //
  UINT32    Revision;

  ///
  /// This data structure contanis all platform level configuration for MRC.
  //
  SYS_SETUP *SysSetup;
} MEMORY_POLICY_PPI;

extern EFI_GUID gMemoryPolicyPpiGuid;

#endif
