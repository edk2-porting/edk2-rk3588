/** @file
  PEI Boards Configurations for PreMem phase.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _BOARD_SA_CONFIG_PRE_MEM_H_
#define _BOARD_SA_CONFIG_PRE_MEM_H_

#include <ConfigBlock.h>
#include <ConfigBlock/MemoryConfig.h>               // for MRC Configuration
#include <ConfigBlock/SwitchableGraphicsConfig.h>   // for PCIE RTD3 GPIO
#include <GpioPinsCnlLp.h>                          // for GPIO definition
#include <GpioPinsCnlH.h>
#include <SaAccess.h>                               // for Root Port number
#include <PchAccess.h>                              // for Root Port number

//
// The following section contains board-specific CMD/CTL/CLK and DQ/DQS mapping, needed for LPDDR3/LPDDR4
//

//
// DQByteMap[0] - ClkDQByteMap:
//   If clock is per rank, program to [0xFF, 0xFF]
//   If clock is shared by 2 ranks, program to [0xFF, 0] or [0, 0xFF]
//   If clock is shared by 2 ranks but does not go to all bytes,
//           Entry[i] defines which DQ bytes Group i services
// DQByteMap[1] - CmdNDQByteMap: Entry[0] is CmdN/CAA and Entry[1] is CmdN/CAB
// DQByteMap[2] - CmdSDQByteMap: Entry[0] is CmdS/CAA and Entry[1] is CmdS/CAB
// DQByteMap[3] - CkeDQByteMap : Entry[0] is CKE /CAA and Entry[1] is CKE /CAB
//                For DDR, DQByteMap[3:1] = [0xFF, 0]
// DQByteMap[4] - CtlDQByteMap : Always program to [0xFF, 0] since we have 1 CTL / rank
//                               Variable only exists to make the code easier to use
// DQByteMap[5] - CmdVDQByteMap: Always program to [0xFF, 0] since we have 1 CA Vref
//                               Variable only exists to make the code easier to use
//
//
// DQ byte mapping to CMD/CTL/CLK, from the CPU side - for BoardIdCometLakeULpddr3Rvp
//
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 DqByteMapCmlULpddr3Rvp[2][6][2] = {
  // Channel 0:
  {
    { 0xF0, 0x0F },
    { 0x00, 0x0F },
    { 0xF0, 0x0F },
    { 0xF0, 0x00 },
    { 0xFF, 0x00 },
    { 0xFF, 0x00 }
  },
  // Channel 1:
  {
    { 0x0F, 0xF0 },
    { 0x00, 0xF0 },
    { 0x0F, 0xF0 },
    { 0x0F, 0x00 },
    { 0xFF, 0x00 },
    { 0xFF, 0x00 }
    }
};

//
// DQS byte swizzling between CPU and DRAM - for CML-U LPDDR3 RVP
//
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 DqsMapCpu2DramCmlULpddr3Rvp[2][8] = {
  { 5, 6, 7, 4, 1, 0, 2, 3 }, // Channel 0
  { 2, 3, 1, 0, 6, 4, 5, 7 }  // Channel 1
};


//
// Reference RCOMP resistors on motherboard - for CML-U LPDDR3 RVP
//
const UINT16 RcompResistorCmlULpKc[SA_MRC_MAX_RCOMP] = { 200, 81, 162 };

//
// RCOMP target values for RdOdt, WrDS, WrDSCmd, WrDSCtl, WrDSClk - for CML-U LPDDR3 RVP
//
const UINT16 RcompTargetCmlULpKc[SA_MRC_MAX_RCOMP_TARGETS] = { 100, 40, 40, 23, 40 };

#endif // _BOARD_SA_CONFIG_PRE_MEM_H_

