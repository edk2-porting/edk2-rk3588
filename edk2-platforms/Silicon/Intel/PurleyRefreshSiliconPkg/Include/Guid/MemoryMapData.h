/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _MEMORY_MAP_GUID_H_
#define _MEMORY_MAP_GUID_H_

#include "SysHost.h"
#include "UncoreCommonIncludes.h"
#include "PartialMirrorGuid.h"

// {F8870015-6994-4b98-95A2-BD56DA91C07F}
#define EFI_MEMORY_MAP_GUID \
  { \
    0xf8870015,0x6994,0x4b98,0x95,0xa2,0xbd,0x56,0xda,0x91,0xc0,0x7f \
  }

extern EFI_GUID gEfiMemoryMapGuid;
extern CHAR16   EfiMemoryMapVariable[];

//
//  System Memory Map HOB information
//

#pragma pack(1)


typedef struct SystemMemoryMapElement {
  UINT16   Type;           // Type of this memory element; Bit0: 1LM  Bit1: 2LM  Bit2: PMEM  Bit3: PMEM-cache  Bit4: BLK Window  Bit5: CSR/Mailbox/Ctrl region
  UINT8    NodeId;         // Node ID of the HA Owning the memory
  UINT8    SocketId;       // Socket Id of socket that has his memory - ONLY IN NUMA
  UINT8    SktInterBitmap; // Socket interleave bitmap, if more that on socket then ImcInterBitmap and ChInterBitmap are identical in all sockets
  UINT8    ImcInterBitmap; // IMC interleave bitmap for this memory
  UINT8    ChInterBitmap[MAX_IMC];//Bit map to denote which channels are interleaved per IMC eg: 111b - Ch 2,1 & 0 are interleaved; 011b denotes Ch1 & 0 are interleaved
  UINT32   BaseAddress;    // Base Address of the element in 64MB chunks
  UINT32   ElementSize;    // Size of this memory element in 64MB chunks
} SYSTEM_MEMORY_MAP_ELEMENT;

typedef struct SystemMemoryMapHob {
  UINT32   lowMemBase;     // Mem base in 64MB units for below 4GB mem.
  UINT32   lowMemSize;     // Mem size in 64MB units for below 4GB mem.
  UINT32   highMemBase;    // Mem base in 64MB units for above 4GB mem.
  UINT32   highMemSize;    // Mem size in 64MB units for above 4GB mem.
  UINT32   memSize;      // Total physical memory size
  UINT16   memFreq;      // Mem Frequency
  UINT8    memMode;      // 0 - Independent, 1 - Lockstep
  UINT8    volMemMode;   // 0 - 1LM, 1 - 2LM
  UINT8    DimmType;
  UINT16   DramType;
  UINT8    DdrVoltage;
  UINT8    AepDimmPresent; // If at least one Aep Dimm Present (used by Nfit), then this should get set
  UINT8    SADNum;
  UINT8    XMPProfilesSup;
  UINT8    cpuType;
  UINT8    cpuStepping;
  UINT8    SystemRasType;
  UINT8    RasModesEnabled; // RAS modes that are enabled
  UINT8    ExRasModesEnabled; // Extended RAS modes that are enabled
  UINT8    RasModesSupported; //RAS modes that are supported by current memory population.
  UINT8    sncEnabled;      // 0 - SNC disabled for this configuration, 1 - SNC enabled for this configuration
  UINT8    NumOfCluster;
  UINT8    NumChPerMC;
  UINT8    numberEntries;   // Number of Memory Map Elements
  UINT8    maxIMC;
  UINT8    maxCh;
  struct   SystemMemoryMapElement Element[MAX_SOCKET * SAD_RULES];
} SYSTEM_MEMORY_MAP_HOB;

#pragma pack()

#endif
