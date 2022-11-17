/** @file

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __SCRATCHPAD_LIST__
#define __SCRATCHPAD_LIST__

//
// Sticky BIOS Scratchpad 7.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Reserved for Intel use.
//

#define BIOS_SCRATCHPAD7_COMPLETION_DELAY_RESET_COUNT_SIZE  2

typedef union {
  struct {
    UINT32 Available : 4;                   // Bits[3:0] - These bits are not reserved.
    UINT32 EnteredColdResetFlow : 1;        // Bits[4:4] - Entered cold reset flow.
    UINT32 CompletedColdResetFlow : 1;      // Bits[5:5] - Completed cold reset flow.
    UINT32 CompletionDelayResetCount : BIOS_SCRATCHPAD7_COMPLETION_DELAY_RESET_COUNT_SIZE;
                                            // Bits[7:6] - The number of resets requested because the calculated
                                            // completion delay was out of bounds. Only the socket 0 instance of this is
                                            // used. This bitfield is used with workaround S1409301288.
    UINT32 Available1 : 7;                  // Bits[14:8] - These bits are not reserved.
    UINT32 CompletedWarmResetWA : 1;        // Bits[15:15] - Indicates if the Warm reset WA was done in sec.
    UINT32 SbspSocketId : 4;                // Bits[19:16] - SBSP socket id
    UINT32 PrefetchFailRecovery : 1;        // Bits[20:20] - Prefetch failure/recovery.
    UINT32 UmaBasedClusteringDowngrade : 2; // Bits[22:21] - Indicate UMA based clusting downgrade
                                            // 0:default; 1: Quad-> Hemi 2: Quad-> Disable 3: Hemi-> Disable
    UINT32 MarginTestfailure : 1;           // Bits[23:23] - This bit is set when Margin Test Fails
    UINT32 DcuModeSelect : 1;               // Bits [24:24] - DCU_MODE select 0/1: 32KB 8-way no-ECC (hardware default) /
                                            // 16KB 4-way with ECC.
    UINT32 DwrBiosStall : 1;                // Bits[25:25] - BIOS Stall if enter DWR.
    UINT32 InDwr : 1;                       // Bits[26:26] - In DWR.
    UINT32 FailMemChkFastColdBoot : 1;      // Bits[27:27] - Bit set when setup option "DEBUG INTERFACE" is enabled
    UINT32 BistFrbEventLastBoot : 1;        // Bits[28:28] - BIST/FRB event occured during the last boot.
    UINT32 RemoteSocketReleased : 1;        // Bits[29:29] - Remote socket released in LT enabled system.
    UINT32 SncFailRecovery : 1;             // Bits[30:30] - Snc failure/recovery.
    UINT32 AepDimmPresent : 1;              // Bits[31:31] - AEP Dimm Present
  } Bits;
  UINT32 Data;
} BIOS_SCRATCHPAD7_STRUCT;

#endif // #ifndef __SCRATCHPAD_LIST__
