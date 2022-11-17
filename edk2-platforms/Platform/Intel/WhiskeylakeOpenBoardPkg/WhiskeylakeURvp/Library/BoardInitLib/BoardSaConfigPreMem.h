/** @file
  PEI Boards Configurations for PreMem phase.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
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
// DQ byte mapping to CMD/CTL/CLK, from the CPU side - for WHL RVP3, WHL SDS - used by WHL/WHL MRC
//
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 mDqByteMapWhlUDdr4Rvp[2][6][2] = {
  // Channel 0:
  {
    { 0x0F, 0xF0 }, // CLK0 goes to package 0 - Bytes[3:0], CLK1 goes to package 1 - Bytes[7:4]
    { 0x00, 0xF0 }, // CmdN does not have CAA, CAB goes to Bytes[7:4]
    { 0x0F, 0xF0 }, // CmdS CAA goes to Bytes[3:0], CmdS CAB goes to Byte[7:4]
    { 0x0F, 0x00 }, // CKE CAA goes to Bytes[3:0], CKE does not have CAB
    { 0xFF, 0x00 }, // CTL (CS) goes to all bytes
    { 0xFF, 0x00 }  // CA Vref is one for all bytes
  },
  // Channel 1:
  {
    { 0x33, 0xCC }, // CLK0 goes to package 0 - Bytes[3:0], CLK1 goes to package 1 - Bytes[7:4]
    { 0x00, 0xCC }, // CmdN does not have CAA, CAB goes to Bytes[7:4]
    { 0x33, 0xCC }, // CmdS CAA goes to Bytes[3:0], CmdS CAB goes to Byte[7:4]
    { 0x33, 0x00 }, // CKE CAA goes to Bytes[3:0], CKE does not have CAB
    { 0xFF, 0x00 }, // CTL (CS) goes to all bytes
    { 0xFF, 0x00 }  // CA Vref is one for all bytes
  }
};

//
// DQS byte swizzling between CPU and DRAM
//
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 mDqsMapCpu2DramWhlUDdr4Rvp[2][8] = {
  { 0, 1, 3, 2, 4, 5, 6, 7 }, // Channel 0
  { 1, 0, 4, 5, 2, 3, 6, 7 }  // Channel 1
};

//
// DQS byte swizzling between CPU and DRAM
//
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 DqsMapCpu2DramWhlUmDvp[2][8] = {
  { 0, 3, 1, 2, 7, 5, 6, 4 }, // Channel 0
  { 0, 2, 1, 3, 6, 4, 7, 5 }  // Channel 1
};

//
// Reference RCOMP resistors on motherboard
//
GLOBAL_REMOVE_IF_UNREFERENCED const UINT16 RcompResistorCflUDdr4Interposer[SA_MRC_MAX_RCOMP] = { 121, 81, 100 };

//
// RCOMP target values for RdOdt, WrDS, WrDSCmd, WrDSCtl, WrDSClk
//
GLOBAL_REMOVE_IF_UNREFERENCED const UINT16 RcompTargetWhlUDdr4Interposer[SA_MRC_MAX_RCOMP_TARGETS] = { 100, 40, 20, 20, 26 };

#endif // _BOARD_SA_CONFIG_PRE_MEM_H_

