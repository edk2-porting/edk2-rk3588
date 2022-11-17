/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef  _CPU_PCI_ACCESS_H_
#define  _CPU_PCI_ACCESS_H_

#include "DataTypes.h"



//
// CPU Types; this needs to be contiguous to assist in table look up
//
#define MAX_CPU_TYPES        1

//
// CPU Index for MC function look-up
//
#define MAX_CPU_INDEX        1


//
// Box Types; this needs to be contiguous to assist in table look up
//
#define  BOX_CHA_MISC        0
#define  BOX_CHA_PMA         1
#define  BOX_CHA_CMS         2
#define  BOX_CHABC           3
#define  BOX_PCU             4
#define  BOX_VCU             5
#define  BOX_M2MEM           6
#define  BOX_MC              7
#define  BOX_MCIO            8
#define  BOX_KTI             9
#define  BOX_M3KTI           10
#define  BOX_MCDDC           11
#define  BOX_M2UPCIE         12
#define  BOX_IIO_PCIE_DMI    13
#define  BOX_IIO_PCIE        14
#define  BOX_IIO_PCIE_NTB    15
#define  BOX_IIO_CB          16
#define  BOX_IIO_VTD         17
#define  BOX_IIO_RTO         18
#define  BOX_UBOX            19
#define  BOX_FPGA            20
#define  MAX_BOX_TYPES       21


//
// Maximum Number of Instances supported by each box type. Note that if the number of instances
// are same for all supported CPUs, then we will have only one #define here (i.e MAX_ALL_XXXXX)
//
#define  MAX_SKX_CHA         28

#define  MAX_SKX_M2PCIE      5

#define  MAX_ALL_CBOBC       1

#define  MAX_SKX_M3KTI       2

#define  MAX_SKX_KTIAGENT    3

#define  MAX_SKX_M2MEM       2

#define  MAX_ALL_M2PCIE      1
#define  MAX_ALL_UBOX        1
#define  MAX_ALL_IIO         4
#define  MAX_ALL_PCU         1
#define  MAX_ALL_VCU         1

#define  MAX_ALL_IIO_CB          1  // 1 instance per CB function block
#define  MAX_ALL_IIO_PCIE_DMI    1  // 0:0:0
#define  MAX_ALL_IIO_PCIE_NTB    3  // 4 instances in PCIE_NTB (0:3:0/1/2/3)
#define  MAX_ALL_IIO_RTO         21 // 4 instances per M/PSTACK + 1 Cstack
#define  MAX_ALL_IIO_RTO_DMI     4  // 4 instances in C stack
#define  MAX_ALL_IIO_RTO_VTD     6  // 6 instances in IIO_RTO block across C/P/MCP stacks
#define  MAX_ALL_IIO_RTO_VTD_DMI 1  // 1 instances in IIO_RTO block across C stack
#define  MAX_ALL_IIO_PCIE        21 // 4 instances per M/PSTACK + 1 Cstack


#define  IIO_RTO             0
#define  IIO_RTO_DMI         1
#define  IIO_RTO_GLOBAL      2
#define  IIO_RTO_GLOBAL_DMI  3
#define  IIO_RTO_VTD         4
#define  IIO_RTO_VTD_DMI     5

//
// Format of CSR register offset passed to helper functions.
// This must be kept in sync with the CSR XML parser tool that generates CSR offset definitions in the CSR header files.
//
typedef union {
  struct  {
    UINT32 offset : 12;         // bits <11:0>
    UINT32 size : 3;          // bits <14:12>
    UINT32 pseudo : 1;          // bit  <15>
    UINT32 funcblk : 8;          // bits <23:16>
    UINT32 boxtype : 8;          // bits <31:24>
  } Bits;
  UINT32 Data;
} CSR_OFFSET;


//
// Format of CSR register offset passed to helper functions.
// This must be kept in sync with the CSR XML parser tool that generates CSR offset definitions in the CSR header files.
//
#define PCI_REG_ADDR(Bus,Device,Function,Offset) \
  (((Offset) & 0xff) | (((Function) & 0x07) << 8) | (((Device) & 0x1f) << 11) | (((Bus) & 0xff) << 16))
#define PCIE_REG_ADDR(Bus,Device,Function,Offset) \
  (((Offset) & 0xfff) | (((Function) & 0x07) << 12) | (((Device) & 0x1f) << 15) | (((Bus) & 0xff) << 20))

#endif   // _CPU_PCI_ACCESS_H_
