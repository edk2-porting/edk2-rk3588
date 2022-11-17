/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "TbtSmiHandler.h"
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Protocol/SmmVariable.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/PciSegmentLib.h>
#define MEM_PER_SLOT  (DEF_RES_MEM_PER_DEV << 4)
#define PMEM_PER_SLOT (DEF_RES_PMEM_PER_DEV << 4)
#define IO_PER_SLOT   (DEF_RES_IO_PER_DEV << 2)

GLOBAL_REMOVE_IF_UNREFERENCED UINTN                 gDeviceBaseAddress;
//
//US(X:0:0), DS(X+1:3:0),DS(X+1:4:0),DS(X+1:5:0),DS(X+1:6:0)
//
GLOBAL_REMOVE_IF_UNREFERENCED BRDG_CONFIG           HrConfigs[MAX_CFG_PORTS];

extern UINT8                      gCurrentDiscreteTbtRootPort;
extern UINT8                      gCurrentDiscreteTbtRootPortType;

BOOLEAN isLegacyDevice          = FALSE;
STATIC UINT8 TbtSegment         = 0;

STATIC
VOID
PortInfoInit (
  IN  OUT PORT_INFO *PortInfo
  )
{
  PortInfo->BusNumLimit = 4;
}

STATIC
VOID
UnsetVesc (
  IN       UINT8     Bus,
  IN       UINT8     Dev,
  IN       UINT8     Fun
  )
{
  UINT8 Dbus;
  UINT32 Data32;
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);

  //
  // Check for abcence of DS bridge
  //
  if(0xFFFF == PciSegmentRead16(gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET)) {
    return;
  }

  //
  // Unset vesc_reg2[23] bit (to have an option to access below DS)
  //
  Data32 = PciSegmentRead32 (gDeviceBaseAddress + PCI_TBT_VESC_REG2);
  Data32 &= 0xFF7FFFFF;
  PciSegmentWrite32(gDeviceBaseAddress + PCI_TBT_VESC_REG2, Data32);
  //
  // Go to Device behind DS
  //
  Dbus = PciSegmentRead8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
  DEBUG((DEBUG_INFO, "Dbus = %d\n",Dbus));
  //
  // Check if there is something behind this Downstream Port (Up or Ep)
  // If there nothing  behind Downstream Port Set vesc_reg2[23] bit -> this will flush all future MemWr
  //
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Dbus, 0x00, 0x00, 0);
  if(0xFFFF == PciSegmentRead16(gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET))
  {
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
  Data32 = PciSegmentRead32 (gDeviceBaseAddress + PCI_TBT_VESC_REG2);
  Data32 |= 0x00800000;
  PciSegmentWrite32 (gDeviceBaseAddress + PCI_TBT_VESC_REG2, Data32);
  }
}// Unset_VESC_REG2

STATIC
UINT16
MemPerSlot (
  IN    UINT16 CurrentUsage
  )
{
  if (CurrentUsage == 0) {
    return 0;
  }

  if (CurrentUsage <= 16) {
    return 16;
  }

  if (CurrentUsage <= 64) {
    return 64;
  }

  if (CurrentUsage <= 128) {
    return 128;
  }

  if (CurrentUsage <= 256) {
    return 256;
  }

  if (CurrentUsage <= 512) {
    return 512;
  }

  if (CurrentUsage <= 1024) {
    return 1024;
  }

  return CurrentUsage;
} // MemPerSlot

STATIC
UINT64
PMemPerSlot (
  IN    UINT64 CurrentUsage
  )
{
  if (CurrentUsage == 0) {
    return 0;
  }

  if (CurrentUsage <= 1024ULL) {
    return 1024ULL;
  }

  if (CurrentUsage <= 4096ULL) {
    return 4096ULL;
  }

  return CurrentUsage;
} // PMemPerSlot

STATIC
VOID
SetPhyPortResources (
  IN       UINT8      Bus,
  IN       UINT8      Dev,
  IN       UINT8      SubBus,
  IN       INT8       Depth,
  IN       PORT_INFO  *CurrentPi,
  IN  OUT  PORT_INFO  *PortInfo
  )
{
  UINT8   Cmd;
  UINT16  DeltaMem;
  UINT64  DeltaPMem;

  Cmd               = CMD_BUS_MASTER;
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, 0x00, 0);

  PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, SubBus);
  PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, Cmd);

  DeltaMem = PortInfo->MemBase - CurrentPi->MemBase;
  if (isLegacyDevice) {
    if (Depth >= 0 && (DeltaMem < MEM_PER_SLOT)) {
      PortInfo->MemBase += MEM_PER_SLOT - DeltaMem;
    }
  } else {
    if (DeltaMem < MemPerSlot (DeltaMem)) {
      PortInfo->MemBase += MemPerSlot (DeltaMem) - DeltaMem;
    }
  }

  if (PortInfo->MemBase > CurrentPi->MemBase && (PortInfo->MemBase - 0x10) <= PortInfo->MemLimit) {
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase), CurrentPi->MemBase);
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryLimit), PortInfo->MemBase - 0x10);
    Cmd |= CMD_BM_MEM;
  } else {
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase), DISBL_MEM32_REG20);
    PortInfo->MemBase = CurrentPi->MemBase;
  }

  DeltaPMem = PortInfo->PMemBase64 - CurrentPi->PMemBase64;
  if (isLegacyDevice) {
    if ((Depth >= 0) && ((UINTN)DeltaPMem < (UINTN)PMEM_PER_SLOT)) {
      PortInfo->PMemBase64 += PMEM_PER_SLOT - DeltaPMem;
    }
  } else {
    if (DeltaPMem < PMemPerSlot (DeltaPMem)) {
      PortInfo->PMemBase64 += PMemPerSlot (DeltaPMem) - DeltaPMem;
    }
  }

  if (PortInfo->PMemBase64 > CurrentPi->PMemBase64 && (PortInfo->PMemBase64 - 0x10) <= PortInfo->PMemLimit64) {
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryBase), (UINT16) (CurrentPi->PMemBase64 & 0xFFFF));
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryLimit), (UINT16) ((PortInfo->PMemBase64 - 0x10) & 0xFFFF));
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableBaseUpper32), (UINT32) (CurrentPi->PMemBase64 >> 16));
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableLimitUpper32), (UINT32) ((PortInfo->PMemBase64 - 0x10) >> 16));
    Cmd |= CMD_BM_MEM;
  } else {
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryBase), DISBL_PMEM_REG24);
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableBaseUpper32), 0);
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableLimitUpper32), 0);
    PortInfo->PMemBase64 = CurrentPi->PMemBase64;
  }

  PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, Cmd);
  PciSegmentWrite8 (gDeviceBaseAddress + PCI_CACHELINE_SIZE_OFFSET, DEF_CACHE_LINE_SIZE);
} // SetPhyPortResources

STATIC
UINT32
SaveSetGetRestoreBar (
  IN  UINTN  Bar
  )
{
  UINT32  BarReq;
  UINT32  OrigBar;

  OrigBar = PciSegmentRead32(Bar);     // Save BAR
  PciSegmentWrite32(Bar, 0xFFFFFFFF);  // Set BAR
  BarReq = PciSegmentRead32(Bar);      // Get BAR
  PciSegmentWrite32(Bar, OrigBar);     // Restore BAR

  return BarReq;
} // SaveSetGetRestoreBar

STATIC
VOID
SetIoBar (
  IN            UINTN    BAR,
  IN            UINT32   BarReq,
  IN  OUT       UINT8    *Cmd,
  IN  OUT       IO_REGS  *IoReg
  )
{
  UINT16  Alignment;
  UINT16  Size;
  UINT16  NewBase;

  Alignment = ~(BarReq & 0xFFFC);
  Size      = Alignment + 1;

  if (IoReg->Base > IoReg->Limit || !Size) {
    return ;

  }

  NewBase = BAR_ALIGN (IoReg->Base, Alignment);
  if (NewBase > IoReg->Limit || NewBase + Size - 1 > IoReg->Limit) {
    return ;

  }
  PciSegmentWrite16(BAR, NewBase);
  IoReg->Base = NewBase + Size; // Advance to new position
  *Cmd      |= CMD_BM_IO; // Set Io Space Enable
} // SetIoBar

STATIC
VOID
SetMemBar (
  IN            UINTN     BAR,
  IN            UINT32    BarReq,
  IN  OUT       UINT8     *Cmd,
  IN  OUT       MEM_REGS  *MemReg
  )
{
  UINT32  Alignment;
  UINT32  Size;
  UINT32  NewBase;

  Alignment = ~(BarReq & 0xFFFFFFF0);
  Size      = Alignment + 1;

  if (MemReg->Base > MemReg->Limit || !Size) {
    return ;

  }

  NewBase = BAR_ALIGN (MemReg->Base, Alignment);
  if (NewBase > MemReg->Limit || NewBase + Size - 1 > MemReg->Limit) {
    return ;

  }

  PciSegmentWrite32(BAR, NewBase);
  MemReg->Base = NewBase + Size; // Advance to new position
  *Cmd       |= CMD_BM_MEM; // Set Memory Space Enable
} // SetMemBar

STATIC
VOID
SetPMem64Bar (
  IN              UINTN      BAR,
  IN              BOOLEAN    IsMaxBar,
  IN              UINT32     BarReq,
  IN    OUT       UINT8      *Cmd,
  IN    OUT       PMEM_REGS  *MemReg
  )
{
  UINT32  Alignment;
  UINT32  Size;
  UINT64  NewBase;

  Alignment = ~(BarReq & 0xFFFFFFF0);
  Size      = Alignment + 1;

  if (MemReg->Base64 > MemReg->Limit64 || !Size) {
    return ;
  }

  NewBase = BAR_ALIGN (MemReg->Base64, Alignment);
  if (NewBase > MemReg->Limit64 || NewBase + Size - 1 > MemReg->Limit64) {
    return ;
  }
  PciSegmentWrite32(BAR, (UINT32)(NewBase & 0xFFFFFFFF));
  if (!IsMaxBar) {
    BAR++;
    PciSegmentWrite32(BAR, (UINT32)(NewBase >> 32));
  }
  MemReg->Base64 = NewBase + Size; // Advance to new position
  *Cmd         |= CMD_BM_MEM; // Set Memory Space Enable
} // SetPMem64Bar

STATIC
VOID
SetDevResources (
  IN       UINT8      Bus,
  IN       UINT8      Dev,
  IN       UINT8      MaxFun,  // PCI_MAX_FUNC for devices, 1 for bridge
  IN       UINT8      MaxBar,     // PCI_BAR5 for devices, PCI_BAR1 for bridge
  IN  OUT  PORT_INFO  *PortInfo
  )
{
  UINT8     Fun;
  UINT8     Reg;
  UINT32    BarReq;
  IO_REGS   Io;
  MEM_REGS  Mem;
  PMEM_REGS PMem;
  UINT8     Cmd;

  Io.Base       = PortInfo->IoBase << 8;
  Io.Limit      = (PortInfo->IoLimit << 8) | 0xFF;
  Mem.Base      = PortInfo->MemBase << 16;
  Mem.Limit     = (PortInfo->MemLimit << 16) | 0xFFFF;
  PMem.Base64   = PortInfo->PMemBase64 << 16;
  PMem.Limit64  = (PortInfo->PMemLimit64 << 16) | 0xFFFF;

  for (Fun = 0; Fun < MaxFun; ++Fun) {
    gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, CMD_BUS_MASTER);
    Cmd = PciSegmentRead8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET);
    if (0xFFFF == PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET)) {
      continue;

    }

    for (Reg = PCI_BASE_ADDRESSREG_OFFSET; Reg <= MaxBar; Reg += 4) {
      BarReq = SaveSetGetRestoreBar(gDeviceBaseAddress + Reg); // Perform BAR sizing

      if (BarReq & BIT0) {
        //
        // I/O BAR
        //
        SetIoBar (
         (gDeviceBaseAddress + Reg),
          BarReq,
          &Cmd,
          &Io
          );
        continue;
      }

      if (BarReq & BIT3) {
        //
        // P-Memory BAR
        //
        SetPMem64Bar ((gDeviceBaseAddress + Reg), MaxBar == Reg, BarReq, &Cmd, &PMem);
      } else {
        SetMemBar ((gDeviceBaseAddress + Reg), BarReq, &Cmd, &Mem);
      }

      if (BIT2 == (BarReq & (BIT2 | BIT1))) {
        //
        // Base address is 64 bits wide
        //
        Reg += 4;
        if (!(BarReq & BIT3)) {
          //
          // 64-bit memory bar
          //
          PciSegmentWrite32 (gDeviceBaseAddress + Reg, 0);
        }
      }
    }

    if (Cmd & BIT1) {
      //
      // If device uses I/O and MEM mapping use only MEM mepping
      //
      Cmd &= ~BIT0;
    }

    PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, Cmd);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_CACHELINE_SIZE_OFFSET, DEF_CACHE_LINE_SIZE);
  }
  //
  // Update PortInfo if any changes
  //
  if (Io.Base > ((UINT32) PortInfo->IoBase << 8)) {
    PortInfo->IoBase = (UINT8) (BAR_ALIGN (Io.Base, 0xFFF) >> 8);
  }

  if (Mem.Base > ((UINT32) PortInfo->MemBase << 16)) {
    PortInfo->MemBase = (UINT16) (BAR_ALIGN (Mem.Base, 0xFFFFF) >> 16);
  }

  if (PMem.Base64 > (PortInfo->PMemBase64 << 16)) {
    PortInfo->PMemBase64 = (BAR_ALIGN (PMem.Base64, 0xFFFFF) >> 16);
  }
} // SetDevResources

STATIC
VOID
InitARHRConfigs(
  IN HR_CONFIG *Hr_Config,
  IN UINT8 BusNumLimit,
  IN OUT BRDG_RES_CONFIG* HrResConf
)
{
  UINT8 i,j;

  //
  // DS port for USB device
  //
  HrConfigs[AR_DS_PORT2].DevId.Bus = HrConfigs[HR_US_PORT].DevId.Bus + 1;
  HrConfigs[AR_DS_PORT2].DevId.Dev = 2;
  HrConfigs[AR_DS_PORT2].DevId.Fun = 0;
  HrConfigs[AR_DS_PORT2].PBus = HrConfigs[AR_DS_PORT2].DevId.Bus;
  HrConfigs[AR_DS_PORT2].SBus = HrConfigs[AR_DS_PORT2].PBus + 1;
  HrConfigs[AR_DS_PORT2].SubBus = HrConfigs[AR_DS_PORT2].PBus + 1;
  //
  // CIO port
  //
  HrConfigs[AR_DS_PORT1].DevId.Bus = HrConfigs[HR_US_PORT].DevId.Bus + 1;
  HrConfigs[AR_DS_PORT1].DevId.Dev = 1;
  HrConfigs[AR_DS_PORT1].DevId.Fun = 0;
  HrConfigs[AR_DS_PORT1].PBus = HrConfigs[AR_DS_PORT1].DevId.Bus;
  HrConfigs[AR_DS_PORT1].SBus = HrConfigs[HR_DS_PORT0].SubBus + 1;
  HrConfigs[AR_DS_PORT1].SubBus = BusNumLimit;

  switch(Hr_Config->DeviceId)
  {
    //
    // HR with 1 DS and 1 USB
    //
    case AR_HR_2C:
    case AR_HR_LP:
    case AR_HR_C0_2C:
    case TR_HR_2C:
      Hr_Config->MinDSNumber = HrConfigs[AR_DS_PORT1].DevId.Dev;
      Hr_Config->MaxDSNumber = HrConfigs[AR_DS_PORT2].DevId.Dev;
      Hr_Config->BridgeLoops = 4;
      break;
    //
    // HR with 2 DS and 1 USB
    //
    case AR_HR_4C:
    case TR_HR_4C:
    case AR_HR_C0_4C:
      Hr_Config->MinDSNumber = 1;
      Hr_Config->MaxDSNumber = 4;
      Hr_Config->BridgeLoops = 6;
      for(j = 2, i = Hr_Config->MinDSNumber; j < count(HrConfigs) && i <= Hr_Config->MaxDSNumber; ++j, ++i)
      {
        HrConfigs[j].DevId.Bus = HrConfigs[HR_US_PORT].DevId.Bus + 1;
        HrConfigs[j].DevId.Dev = i;
        HrConfigs[j].DevId.Fun = 0;
        HrConfigs[j].PBus = HrConfigs[j].DevId.Bus;
        HrConfigs[j].Res.Cls = DEF_CACHE_LINE_SIZE;
      }
    break;
  }
}//InitARHRConfigs


STATIC
VOID
InitCommonHRConfigs (
  IN       HR_CONFIG        *Hr_Config,
  IN       UINT8            BusNumLimit,
  IN  OUT  BRDG_RES_CONFIG  *HrResConf
  )
{
  UINT8 i;

  UINT8 j;
  for(i = 0; i < count(HrConfigs); ++i) {
    HrConfigs[i].IsDSBridge = TRUE;
  }
  //
  // US(HRBus:0:0)
  //
  HrConfigs[HR_US_PORT].DevId.Bus   = Hr_Config->HRBus;
  HrConfigs[HR_US_PORT].DevId.Dev   = 0;
  HrConfigs[HR_US_PORT].DevId.Fun   = 0;
  HrConfigs[HR_US_PORT].Res         = *HrResConf;
  HrConfigs[HR_US_PORT].Res.IoBase  = 0xF1;
  HrConfigs[HR_US_PORT].Res.IoLimit = 0x01;
  HrConfigs[HR_US_PORT].PBus        = HrConfigs[HR_US_PORT].DevId.Bus;
  HrConfigs[HR_US_PORT].SBus        = HrConfigs[HR_US_PORT].PBus + 1;
  HrConfigs[HR_US_PORT].SubBus      = BusNumLimit;
  HrConfigs[HR_US_PORT].IsDSBridge  = FALSE;

  //
  // HIA resides here
  //
  HrConfigs[HR_DS_PORT0].DevId.Bus    = HrConfigs[HR_US_PORT].DevId.Bus + 1;
  HrConfigs[HR_DS_PORT0].DevId.Dev    = 0;
  HrConfigs[HR_DS_PORT0].DevId.Fun    = 0;
  HrConfigs[HR_DS_PORT0].Res          = NOT_IN_USE_BRIDGE;
  HrConfigs[HR_DS_PORT0].Res.MemBase  = HrResConf->MemLimit;
  HrConfigs[HR_DS_PORT0].Res.MemLimit = HrResConf->MemLimit;
  HrResConf->MemLimit                -= 0x10; //This 1 MB chunk will be used by HIA
  HrConfigs[HR_DS_PORT0].Res.Cmd      = CMD_BM_MEM;
  HrConfigs[HR_DS_PORT0].Res.Cls      = DEF_CACHE_LINE_SIZE;
  HrConfigs[HR_DS_PORT0].PBus         = HrConfigs[HR_DS_PORT0].DevId.Bus;
  HrConfigs[HR_DS_PORT0].SBus         = HrConfigs[HR_DS_PORT0].PBus + 1;
  HrConfigs[HR_DS_PORT0].SubBus       = HrConfigs[HR_DS_PORT0].PBus + 1;

  switch (Hr_Config->DeviceId) {
  //
  // Alpine Ridge
  //
  case AR_HR_2C:
  case AR_HR_C0_2C:
  case AR_HR_LP:
  case AR_HR_4C:
  case AR_HR_C0_4C:
  //
  // Titan Ridge
  //
  case TR_HR_2C:
  case TR_HR_4C:
    InitARHRConfigs(Hr_Config, BusNumLimit, HrResConf);
    break;

  default:
    //
    // DS(HRBus+2:3-6:0)
    //
    Hr_Config->MinDSNumber  = 3;
    Hr_Config->MaxDSNumber  = 6;
    Hr_Config->BridgeLoops  = count (HrConfigs);

    for (j = 2, i = Hr_Config->MinDSNumber; j < count (HrConfigs) && i <= Hr_Config->MaxDSNumber; ++j, ++i) {
      HrConfigs[j].DevId.Bus  = HrConfigs[HR_US_PORT].DevId.Bus + 1;
      HrConfigs[j].DevId.Dev  = i;
      HrConfigs[j].DevId.Fun  = 0;
      HrConfigs[j].PBus       = HrConfigs[j].DevId.Bus;
      HrConfigs[j].Res.Cls    = DEF_CACHE_LINE_SIZE;
    }
  }
} // InitCommonHRConfigs

STATIC
VOID
InitHRDSPort_Disable (
  IN       UINT8        id,
  IN  OUT  BRDG_CONFIG  *BrdgConf
  )
{
  HrConfigs[id].Res     = NOT_IN_USE_BRIDGE;
  HrConfigs[id].SBus    = BrdgConf->SBus;
  HrConfigs[id].SubBus  = BrdgConf->SBus;

  BrdgConf->SBus++;
} // InitHRDSPort_Disable

//AR only

STATIC
VOID
InitARDSPort_1Port(
  IN  OUT  BRDG_CONFIG* BrdgConf
)
{
  UINT16 MemBase    = BrdgConf->Res.MemBase & 0xFFF0;
  UINT64 PMemBase64 = BrdgConf->Res.PMemBase64 & ~0xFULL;
  UINT8  BusRange = BrdgConf->SubBus - BrdgConf->PBus - 2;

  HrConfigs[AR_DS_PORT1].Res = NOT_IN_USE_BRIDGE;
  HrConfigs[AR_DS_PORT1].Res.Cls = DEF_CACHE_LINE_SIZE;
  HrConfigs[AR_DS_PORT1].Res.Cmd = CMD_BM_MEM;
  HrConfigs[AR_DS_PORT1].Res.MemBase = MemBase;
  HrConfigs[AR_DS_PORT1].Res.MemLimit = BrdgConf->Res.MemLimit - 1;
  HrConfigs[AR_DS_PORT1].Res.PMemBase64 = PMemBase64;
  HrConfigs[AR_DS_PORT1].Res.PMemLimit64 = BrdgConf->Res.PMemLimit64;
  HrConfigs[AR_DS_PORT1].SBus = BrdgConf->SBus;
  HrConfigs[AR_DS_PORT1].SubBus = BrdgConf->SBus + BusRange;

  BrdgConf->SBus = HrConfigs[AR_DS_PORT1].SubBus + 1;

  HrConfigs[AR_DS_PORT2].Res = NOT_IN_USE_BRIDGE;
  HrConfigs[AR_DS_PORT2].Res.Cls = DEF_CACHE_LINE_SIZE;
  HrConfigs[AR_DS_PORT2].Res.Cmd = CMD_BM_MEM;
  HrConfigs[AR_DS_PORT2].Res.MemBase = BrdgConf->Res.MemLimit;
  HrConfigs[AR_DS_PORT2].Res.MemLimit = BrdgConf->Res.MemLimit;
  HrConfigs[AR_DS_PORT2].SBus = BrdgConf->SBus;
  HrConfigs[AR_DS_PORT2].SubBus = BrdgConf->SBus;

  BrdgConf->SBus = HrConfigs[AR_DS_PORT2].SubBus + 1;
}//InitARDSPort_1Port

STATIC
VOID
InitARDSPort_2Port(
  IN OUT BRDG_CONFIG* BrdgConf
)
{
  UINT16 MemBase    = BrdgConf->Res.MemBase & 0xFFF0;
  UINT64 PMemBase64 = BrdgConf->Res.PMemBase64 & ~0xFULL;
  UINT8  BusRange = BrdgConf->SubBus - BrdgConf->PBus - 3;

  // Busses are split between ports 1 and 4
  BusRange /= 2;

  HrConfigs[AR_DS_PORT1].Res = NOT_IN_USE_BRIDGE;
  HrConfigs[AR_DS_PORT1].Res.Cls = DEF_CACHE_LINE_SIZE;
  HrConfigs[AR_DS_PORT1].Res.Cmd = CMD_BM_MEM;
  HrConfigs[AR_DS_PORT1].Res.MemBase = MemBase;
  HrConfigs[AR_DS_PORT1].Res.MemLimit = MemBase + 0x17F0 - 1;
  HrConfigs[AR_DS_PORT1].Res.PMemBase64 = PMemBase64;
  HrConfigs[AR_DS_PORT1].Res.PMemLimit64 = PMemBase64 + 0x2000 - 1;
  HrConfigs[AR_DS_PORT1].SBus = BrdgConf->SBus;
  HrConfigs[AR_DS_PORT1].SubBus = BrdgConf->SBus + BusRange;

  BrdgConf->SBus = HrConfigs[AR_DS_PORT1].SubBus + 1;

  HrConfigs[AR_DS_PORT2].Res = NOT_IN_USE_BRIDGE;
  HrConfigs[AR_DS_PORT2].Res.Cls = DEF_CACHE_LINE_SIZE;
  HrConfigs[AR_DS_PORT2].Res.Cmd = CMD_BM_MEM;
  HrConfigs[AR_DS_PORT2].Res.MemBase = MemBase + 0x17F0;
  HrConfigs[AR_DS_PORT2].Res.MemLimit = MemBase + 0x1800 - 1;
  HrConfigs[AR_DS_PORT2].SBus = BrdgConf->SBus;
  HrConfigs[AR_DS_PORT2].SubBus = BrdgConf->SBus;

  BrdgConf->SBus = HrConfigs[AR_DS_PORT2].SubBus + 1;


  HrConfigs[AR_DS_PORT4].Res = NOT_IN_USE_BRIDGE;
  HrConfigs[AR_DS_PORT4].Res.Cls = DEF_CACHE_LINE_SIZE;
  HrConfigs[AR_DS_PORT4].Res.Cmd = CMD_BM_MEM;
  HrConfigs[AR_DS_PORT4].Res.MemBase = MemBase + 0x1800;
  HrConfigs[AR_DS_PORT4].Res.MemLimit = BrdgConf->Res.MemLimit;
  HrConfigs[AR_DS_PORT4].Res.PMemBase64 = PMemBase64 + 0x2000;
  HrConfigs[AR_DS_PORT4].Res.PMemLimit64 = BrdgConf->Res.PMemLimit64;
  HrConfigs[AR_DS_PORT4].SBus = BrdgConf->SBus;
  HrConfigs[AR_DS_PORT4].SubBus = BrdgConf->SubBus;

  BrdgConf->SBus = HrConfigs[AR_DS_PORT4].SubBus + 1;
}//InitARDSPort_2Port


STATIC
BOOLEAN
CheckLimits (
  IN    BOOLEAN          Is2PortDev,
  IN    BRDG_RES_CONFIG  *HrResConf,
  IN    UINT8            BusRange
  )
{
  UINT16  MemBase;
  UINT16  MemLimit;
  UINT64  PMemBase64;
  UINT64  PMemLimit64;

  MemBase     = HrResConf->MemBase & 0xFFF0;
  MemLimit    = HrResConf->MemLimit & 0xFFF0;
  PMemBase64  = HrResConf->PMemBase64 & 0xFFF0;
  PMemLimit64 = HrResConf->PMemLimit64 & 0xFFF0;
  //
  // Check memoty alignment
  //
  if (MemBase & 0x3FF) {
    DEBUG((DEBUG_INFO, "M alig\n"));
    return FALSE;
  }

  if (PMemBase64 & 0xFFF) {
    DEBUG((DEBUG_INFO, "PM alig\n"));
    return FALSE;
  }

  if (Is2PortDev) {
    //
    // Check mem size
    //
    if (MemLimit + 0x10 - MemBase < 0x2E00) {
      DEBUG((DEBUG_INFO, "M size\n"));
      return FALSE;
    }
    //
    // Check P-mem size
    //
    if (PMemLimit64 + 0x10 - PMemBase64 < 0x4A00) {
      DEBUG((DEBUG_INFO, "PM size\n"));
      return FALSE;
    }
    //
    // Check bus range
    //
    if (BusRange < 106) {
      DEBUG((DEBUG_INFO, "Bus range\n"));
      return FALSE;
    }
  } else {
    //
    // Check mem size
    //
    if (MemLimit + 0x10 - MemBase < 0x1600) {
      DEBUG((DEBUG_INFO, "M size\n"));
      return FALSE;
    }
    //
    // Check P-mem size
    //
    if (PMemLimit64 + 0x10 - PMemBase64 < 0x2200) {
      DEBUG((DEBUG_INFO, "PM size\n"));
      return FALSE;
    }
    //
    // Check bus range
    //
    if (BusRange < 56) {
      DEBUG((DEBUG_INFO, "Bus range\n"));
      return FALSE;
    }
  }

  return TRUE;
} // CheckLimits

STATIC
BOOLEAN
InitHRResConfigs (
  IN  OUT HR_CONFIG      *Hr_Config,
  IN    UINT8            BusNumLimit,
  IN  OUT BRDG_RES_CONFIG*HrResConf
  )
{
  BRDG_CONFIG  BrdgConf = { { 0 } };

  InitCommonHRConfigs (Hr_Config, BusNumLimit, HrResConf);
  BrdgConf.PBus   = Hr_Config->HRBus + 2;// Take into account busses
  BrdgConf.SBus   = Hr_Config->HRBus + 3;// for US and DS of HIA
  BrdgConf.SubBus = BusNumLimit;
  BrdgConf.Res    = *HrResConf;
  while (TRUE) {
    switch (Hr_Config->DeviceId) {
    case AR_HR_4C:
    case TR_HR_4C:
    case AR_HR_C0_4C:
      //
      // 2 Port host
      //
      if (CheckLimits (TRUE, HrResConf, BusNumLimit - Hr_Config->HRBus)) {


          InitARDSPort_2Port(&BrdgConf);
          DEBUG((DEBUG_INFO, "AR2\n"));

        return TRUE;
      } else {
       return FALSE;
      }
    // AR only
  case AR_HR_2C: // 1 port host
  case AR_HR_C0_2C:
  case AR_HR_LP:
  case TR_HR_2C:
    DEBUG((DEBUG_INFO, "AR1\n"));
    InitARDSPort_1Port(&BrdgConf);
    return TRUE;

    default:
      InitHRDSPort_Disable (HR_DS_PORT3, &BrdgConf);
      InitHRDSPort_Disable (HR_DS_PORT4, &BrdgConf);
      InitHRDSPort_Disable (HR_DS_PORT5, &BrdgConf);
      InitHRDSPort_Disable (HR_DS_PORT6, &BrdgConf);
      return FALSE;
    }
  }
} // InitHRResConfigs

STATIC
BOOLEAN
InitializeHostRouter (
  OUT  HR_CONFIG  *Hr_Config,
  IN   UINTN      RpSegment,
  IN   UINTN      RpBus,
  IN   UINTN      RpDevice,
  IN   UINTN      RpFunction
  )
{
  UINT8           BusNumLimit;
  BRDG_RES_CONFIG HrResConf = { 0 };
  UINT8           i;
  BOOLEAN         Ret;

  Ret = TRUE;

  gDeviceBaseAddress   = PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, 0);
  Hr_Config->HRBus    = PciSegmentRead8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
  gDeviceBaseAddress   = PCI_SEGMENT_LIB_ADDRESS (RpSegment, Hr_Config->HRBus, 0x00, 0x00, 0);
  Hr_Config->DeviceId = PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET);
  if (!(IsTbtHostRouter (Hr_Config->DeviceId))) {
    return FALSE;
  }
  TbtSegment = (UINT8)RpSegment;

  HrResConf.Cmd          = CMD_BM_MEM;
  HrResConf.Cls          = DEF_CACHE_LINE_SIZE;
  gDeviceBaseAddress      = PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, 0);
  HrResConf.IoBase       = PciSegmentRead8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoBase));
  HrResConf.IoLimit      = PciSegmentRead8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoLimit));
  HrResConf.MemBase      = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase));
  HrResConf.MemLimit     = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryLimit));
  HrResConf.PMemBase64   = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryBase));
  HrResConf.PMemLimit64  = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryLimit));
  HrResConf.PMemBase64  |= (UINT64)(PciSegmentRead32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableBaseUpper32))) << 16;
  HrResConf.PMemLimit64 |= (UINT64)(PciSegmentRead32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableLimitUpper32))) << 16;
  BusNumLimit = PciSegmentRead8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET);

  Ret         = InitHRResConfigs (Hr_Config, BusNumLimit, &HrResConf);

  for (i = 0; i < Hr_Config->BridgeLoops; ++i) {
    UINT8 Bus;
    UINT8 Dev;
    UINT8 Fun;
    Bus               = HrConfigs[i].DevId.Bus;
    Dev               = HrConfigs[i].DevId.Dev;
    Fun               = HrConfigs[i].DevId.Fun;
    gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment,Bus, Dev, Fun, 0);

    PciSegmentWrite8 (gDeviceBaseAddress + PCI_CACHELINE_SIZE_OFFSET, HrConfigs[i].Res.Cls);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, HrConfigs[i].PBus);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, HrConfigs[i].SBus);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, HrConfigs[i].SubBus);
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase), HrConfigs[i].Res.MemBase);
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryLimit), HrConfigs[i].Res.MemLimit);
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryBase), (UINT16) (HrConfigs[i].Res.PMemBase64 & 0xFFFF));
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryLimit), (UINT16) (HrConfigs[i].Res.PMemLimit64 & 0xFFFF));
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableBaseUpper32), (UINT32) (HrConfigs[i].Res.PMemBase64 >> 16));
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableLimitUpper32), (UINT32) (HrConfigs[i].Res.PMemLimit64 >> 16));
    PciSegmentWrite8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoBase), HrConfigs[i].Res.IoBase);
    PciSegmentWrite8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoLimit), HrConfigs[i].Res.IoLimit);
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoBaseUpper16), 0x00000000);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, HrConfigs[i].Res.Cmd);
  }
  if (Hr_Config->DeviceId == AR_HR_2C || Hr_Config->DeviceId == AR_HR_4C || Hr_Config->DeviceId == AR_HR_LP) {
    for (i = 0; i < Hr_Config->BridgeLoops; ++i) {
      if(HrConfigs[i].IsDSBridge) {
        UnsetVesc(HrConfigs[i].DevId.Bus, HrConfigs[i].DevId.Dev, HrConfigs[i].DevId.Fun);
      }
    }
  }

  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment,(Hr_Config->HRBus + 2), 0x00, 0x00, 0);
  PciSegmentWrite32 (gDeviceBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX0 * 4), HrConfigs[HR_DS_PORT0].Res.MemLimit << 16);
  PciSegmentWrite32 (gDeviceBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX1 * 4), (HrConfigs[HR_DS_PORT0].Res.MemLimit + 0x4) << 16);
  PciSegmentWrite8 (gDeviceBaseAddress + PCI_CACHELINE_SIZE_OFFSET, DEF_CACHE_LINE_SIZE);
  PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, CMD_BM_MEM);
  return Ret;
} // InitializeHostRouter
STATIC
UINT8
ConfigureSlot (
  IN       UINT8      Bus,
  IN       UINT8      MAX_DEVICE,
  IN       INT8       Depth,
  IN       BOOLEAN    ArPcie,
  IN  OUT  PORT_INFO  *PortInfo
  )
{
  UINT8      Device;
  UINT8      SBus;
  UINT8      UsedBusNumbers;
  UINT8      RetBusNum;
  PORT_INFO  CurrentSlot;

  RetBusNum = 0;

  for (Device = 0; Device < MAX_DEVICE; Device++) {
    //
    // Continue if device is absent
    //
    gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Device, 0x00, 0);
    if (0xFFFF == PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET)) {
      continue;

    }

    if (P2P_BRIDGE != PciSegmentRead16 (gDeviceBaseAddress + (PCI_CLASSCODE_OFFSET + 1))) {
      SetDevResources (
        Bus,
        Device,
        PCI_MAX_FUNC,
        PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX5 * 4),
        PortInfo
        );
      continue;
    }
    //
    // Else Bridge
    //
    CopyMem (&CurrentSlot, PortInfo, sizeof (PORT_INFO));

    ++RetBusNum; // UP Bridge
    SBus = Bus + RetBusNum; // DS Bridge

    if (SBus + 1 >= PortInfo->BusNumLimit) {
      continue;

    }

    SetDevResources (Bus, Device, 1, PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX1 * 4), PortInfo);

    //
    // Init UP Bridge to reach DS Bridge
    //
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, Bus);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, SBus);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, PortInfo->BusNumLimit);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, CMD_BM_MEM);

  if(ArPcie) {
    UnsetVesc(Bus, Device, 0x00);
  }

  UsedBusNumbers = ConfigureSlot(SBus, PCI_MAX_DEVICE + 1, -1, FALSE, PortInfo);
  RetBusNum += UsedBusNumbers;

    SetPhyPortResources (
      Bus,
      Device,
      SBus + UsedBusNumbers,
      Depth,
      &CurrentSlot,
      PortInfo
      );
  }
  //
  // for (Device = 0; Device <= PCI_MAX_DEVICE; Device++)
  //
  return RetBusNum;
} // ConfigureSlot

STATIC
VOID
SetCioPortResources (
  IN       UINT8     Bus,
  IN       UINT8     Dev,
  IN       UINT8     SBus,
  IN       UINT8     SubBus,
  IN       PORT_INFO  *portInfoBeforeChange,
  IN  OUT  PORT_INFO  *PortInfo
  )
{
  UINT8 Cmd;
  Cmd               = CMD_BUS_MASTER;

  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment,Bus, Dev, 0x00, 0);
  PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, Bus);
  PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, SBus);
  PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, SubBus);
  PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, Cmd);

  if (PortInfo->IoBase <= PortInfo->IoLimit) {
    PciSegmentWrite8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoBase), PortInfo->IoBase);
    PciSegmentWrite8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoLimit), PortInfo->IoLimit);
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoBaseUpper16), 0x00000000);
    Cmd |= CMD_BM_IO;
  } else {
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoBase), DISBL_IO_REG1C);
  }

  if (PortInfo->MemBase <= PortInfo->MemLimit) {
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase), PortInfo->MemBase);
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryLimit), PortInfo->MemLimit);
    Cmd |= CMD_BM_MEM;
  } else {
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase), DISBL_MEM32_REG20);
  }

  if (PortInfo->PMemBase64 <= PortInfo->PMemLimit64) {
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryBase), (UINT16) (PortInfo->PMemBase64 & 0xFFFF));
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryLimit), (UINT16) (PortInfo->PMemLimit64 & 0xFFFF));
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableBaseUpper32), (UINT32) (PortInfo->PMemBase64 >> 16));
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableLimitUpper32), (UINT32) (PortInfo->PMemLimit64 >> 16));
    Cmd |= CMD_BM_MEM;
  } else {
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryBase), DISBL_PMEM_REG24);
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableBaseUpper32), 0);
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableLimitUpper32), 0);
  }

  PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, Cmd);
  PciSegmentWrite8 (gDeviceBaseAddress + PCI_CACHELINE_SIZE_OFFSET, DEF_CACHE_LINE_SIZE);
} // SetCioPortResources

STATIC
VOID
SetSlotsAsUnused (
  IN       UINT8      Bus,
  IN       UINT8      MaxSlotNum,
  IN       UINT8      CioSlot,
  IN  OUT  PORT_INFO  *PortInfo
  )
{
  UINT8 Slot;
  for (Slot = MaxSlotNum; Slot > CioSlot; --Slot) {
    gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment,Bus, Slot, 0x00, 0);
    if (0xFFFF == PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET)) {
      continue;
    }

    PciSegmentWrite8 (gDeviceBaseAddress + PCI_CACHELINE_SIZE_OFFSET, DEF_CACHE_LINE_SIZE);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, Bus);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, PortInfo->BusNumLimit);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, PortInfo->BusNumLimit);
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoBase), DISBL_IO_REG1C);
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase), DISBL_MEM32_REG20);
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryBase), DISBL_PMEM_REG24);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, CMD_BUS_MASTER);
    PortInfo->BusNumLimit--;
  }
} // SetSlotsAsUnused

STATIC
UINT16
FindVendorSpecificHeader(
  IN  UINT8  Bus
)
{
  PCI_EXP_EXT_HDR   *ExtHdr;
  UINT32            ExtHdrValue;
  UINT16            ExtendedRegister;

  ExtHdr = (PCI_EXP_EXT_HDR*) &ExtHdrValue;
  ExtendedRegister  = 0x100;
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment,Bus, 0x00, 0x00, 0);
  while (ExtendedRegister) {
    ExtHdrValue = PciSegmentRead32 (gDeviceBaseAddress + ExtendedRegister);
    if (ExtHdr->CapabilityId == 0xFFFF) {
      return 0x0000; // No Vendor-Specific Extended Capability header
    }

    if (PCI_EXPRESS_EXTENDED_CAPABILITY_VENDOR_SPECIFIC_ID == ExtHdr->CapabilityId) {
      return ExtendedRegister;
    }

    ExtendedRegister = (UINT16) ExtHdr->NextCapabilityOffset;
  }
  return 0x0000; // No Vendor-Specific Extended Capability header
}

STATIC
UINT8
FindSsid_SsvidHeader (
  IN    UINT8  Bus
  )
{
  UINT8 CapHeaderId;
  UINT8 CapHeaderOffset;
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment,Bus, 0x00, 0x00, 0);
  CapHeaderOffset   = PciSegmentRead8 (gDeviceBaseAddress + PCI_CAPBILITY_POINTER_OFFSET);

  while (CapHeaderOffset != 0) {
    CapHeaderId = PciSegmentRead8 (gDeviceBaseAddress + CapHeaderOffset);

    if (CapHeaderId == PCIE_CAP_ID_SSID_SSVID) {
      return CapHeaderOffset;
    }

    CapHeaderOffset = PciSegmentRead8 (gDeviceBaseAddress + CapHeaderOffset + 1);
  }

  DEBUG((DEBUG_INFO, "SID0\n"));
  return 0;
} // FindSsid_SsvidHeader

STATIC
BOOLEAN
GetCioSlotByDevId (
  IN   UINT8  Bus,
  OUT  UINT8  *CioSlot,
  OUT  UINT8  *MaxSlotNum,
  OUT  BOOLEAN *ArPcie
  )
{
  UINT16            VSECRegister;
  BRDG_CIO_MAP_REG  BridgMap;
  UINT32            BitScanRes;
  UINT16            DevId;
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, 0x00, 0x00, 0);
  DevId             = PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET);

  //
  // Init out params in case device is not recognised
  //
  *CioSlot    = 4;
  *MaxSlotNum = 7;
  *ArPcie     = FALSE;

  switch (DevId) {
    //
    // For known device IDs
    //
    case 0x1578:
      *ArPcie = TRUE;
  }

  switch (DevId) {
  //
  // For known device IDs
  //
  case 0x1513:
  case 0x151A:
  case 0x151B:
  case 0x1547:
  case 0x1548:
    return TRUE; // Just return
  case 0x1549:
    return FALSE; // Just return
  }

  VSECRegister = FindVendorSpecificHeader(Bus);
  if (!VSECRegister) {
    return TRUE; // Just return
  }
  //
  // Go to Bridge/CIO map register
  //
  VSECRegister += 0x18;
  BridgMap.AB_REG = PciSegmentRead32(gDeviceBaseAddress + VSECRegister);
  //
  // Check for range
  //
  if (BridgMap.Bits.NumOfDSPorts < 1 || BridgMap.Bits.NumOfDSPorts > 27) {
    return TRUE;
  //
  // Not a valid register
  //
  }
  //
  // Set OUT params
  //
  *MaxSlotNum = (UINT8) BridgMap.Bits.NumOfDSPorts;

#ifdef _MSC_VER
  if(!_BitScanForward(&BitScanRes, BridgMap.Bits.CioPortMap)) { // No DS bridge which is CIO port
    return FALSE;
  }
#else
#ifdef __GNUC__
  if (BridgMap.Bits.CioPortMap == 0) {
    return FALSE;
  }
  BitScanRes = __builtin_ctz (BridgMap.Bits.CioPortMap);
#else
#error Unsupported Compiler
#endif
#endif

  *CioSlot = (UINT8)BitScanRes;
  return TRUE;
} // GetCioSlotByDevId

#define TBT_LEGACY_SUB_SYS_ID 0x11112222

STATIC
BOOLEAN
IsLegacyDevice (
  IN    UINT8  Bus
  )
{
  UINT32  Sid;
  UINT8   SidRegister;
  UINT16  DevId;

  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment,Bus, 0x00, 0x00, 0);
  DevId             = PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET);
  switch (DevId) {
  //
  // For known device IDs
  //
  case 0x1513:
  case 0x151A:
  case 0x151B:
    DEBUG((DEBUG_INFO, "Legacy "));
    DEBUG((DEBUG_INFO, "DevId = %d\n",DevId));
    return TRUE;
    //
    // Legacy device by Device Id
    //
  }

  SidRegister = FindSsid_SsvidHeader(Bus);

  if (!SidRegister) {
    return TRUE; // May be absent for legacy devices
  }
  //
  // Go to register
  //
  SidRegister += 0x4;
  Sid = PciSegmentRead32(gDeviceBaseAddress + SidRegister);
  DEBUG((DEBUG_INFO, "SID"));
  DEBUG((DEBUG_INFO, " = %d\n", Sid));

return TBT_LEGACY_SUB_SYS_ID == Sid || 0 == Sid;
} // IsLegacyDevice

STATIC
VOID
UnsetVescEp(
  IN  UINT8     Bus,
  IN  UINT8     MaxSlotNum
  )
{
  UINT8 i;

  for (i = 0; i <= MaxSlotNum; ++i)
  {
    UnsetVesc(Bus, i, 0);
  }
}// Unset_VESC_REG2_EP

STATIC
BOOLEAN
ConfigureEP (
  IN       INT8      Depth,
  IN  OUT  UINT8     *Bus,
  IN  OUT  PORT_INFO *PortInfo
  )
{
  UINT8      SBus;
  UINT8      CioSlot;
  UINT8      MaxSlotNum;
  BOOLEAN    ArPcie;
  UINT8      MaxPHYSlots;
  UINT8      UsedBusNumbers;
  UINT8      cmd;
  BOOLEAN    CioSlotPresent;
  BOOLEAN    Continue;
  PORT_INFO  PortInfoOrg;
  UINT8      CioBus;

  CioSlot     = 4;
  MaxSlotNum  = 7;
  CopyMem (&PortInfoOrg, PortInfo, sizeof (PORT_INFO));

  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, *Bus, 0x00, 0x00, 0);
  cmd               = PciSegmentRead8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET);
  // AR ONLY
  // Endpoint on CIO slot, but not a bridge device
  if (P2P_BRIDGE != PciSegmentRead16 (gDeviceBaseAddress + (PCI_CLASSCODE_OFFSET + 1))) {
    DEBUG((DEBUG_INFO, "UEP\n"));
    // Check whether EP already configured by examining CMD register
    if(cmd & CMD_BUS_MASTER) // Yes, no need to touch this EP
    {
      DEBUG((DEBUG_INFO, "BMF\n"));
      return FALSE;
    }
    // Configure it as regular PCIe device
    ConfigureSlot(*Bus, PCI_MAX_DEVICE + 1, -1, FALSE, PortInfo);

    return FALSE;
  }

  //
  // Based on Device ID assign Cio slot and max number of PHY slots to scan
  //
  CioSlotPresent  =  GetCioSlotByDevId(*Bus, &CioSlot, &MaxSlotNum, &ArPcie);
  MaxPHYSlots     = MaxSlotNum;
  //
  // Check whether EP already configured by examining CMD register
  //

  if (cmd & CMD_BUS_MASTER) {
    //
    // Yes no need to touch this EP, just move to next one in chain
    //
    CioBus = *Bus + 1;
    if(ArPcie){
      UnsetVescEp(CioBus, MaxSlotNum);
    }
    if (!CioSlotPresent) {
      //
      // Cio slot is not present in EP, just return FALSE
      //
      DEBUG((DEBUG_INFO, "BMF\n"));
      return FALSE;
    }
    //
    // Take all resources from Cio slot and return
    //
    gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment,CioBus, CioSlot, 0x00, 0);
    PortInfo->BusNumLimit   = PciSegmentRead8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET);
    PortInfo->IoBase        = PciSegmentRead8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoBase));
    PortInfo->IoLimit       = PciSegmentRead8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoLimit));
    PortInfo->MemBase       = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase));
    PortInfo->MemLimit      = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryLimit));
    PortInfo->PMemBase64    = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryBase)) & 0xFFF0;
    PortInfo->PMemLimit64   = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryLimit)) & 0xFFF0;
    PortInfo->PMemBase64   |= (UINT64)(PciSegmentRead32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableBaseUpper32))) << 16;
    PortInfo->PMemLimit64  |= (UINT64)(PciSegmentRead32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableLimitUpper32))) << 16;
    PortInfo->PMemLimit64  |= 0xF;
    //
    // Jump to next EP
    //
    *Bus = PciSegmentRead8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
    //
    // Should we continue?
    //
    gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment,*Bus, 0x00, 0x00, 0);
    Continue          = 0xFFFF != PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET);
    return Continue;
  }
  //
  // Set is legacy dvice
  //
  isLegacyDevice = IsLegacyDevice (*Bus);

  SetCioPortResources (
    *Bus,
    0, // Assign all available resources to US port of EP
    *Bus + 1,
    PortInfo->BusNumLimit,
    0,
    PortInfo
    );

  SBus = *Bus + 1;// Jump to DS port

  if (CioSlotPresent) {
    MaxPHYSlots = CioSlot;
  }

  UsedBusNumbers = ConfigureSlot(SBus, MaxPHYSlots, Depth, ArPcie, PortInfo);
  if (!CioSlotPresent) {
    return FALSE;
    //
    // Stop resource assignment on this chain
    //
  }
  //
  // Set rest of slots us unused
  //
  SetSlotsAsUnused (SBus, MaxSlotNum, CioSlot, PortInfo);

  SetCioPortResources (
    SBus,
    CioSlot,
    SBus + UsedBusNumbers + 1,
    PortInfo->BusNumLimit,
    &PortInfoOrg,
    PortInfo
    );
  *Bus = SBus + UsedBusNumbers + 1;// Go to next EP
  if(ArPcie) {
    UnsetVesc(SBus, CioSlot, 0x00);
  }
  if (*Bus > PortInfo->BusNumLimit - 2) {
    //
    // In case of bus numbers are exhausted stop enumeration
    //
    return FALSE;
  }
  //
  // Check whether we should continue on this chain
  //
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment,*Bus, 0x00, 0x00, 0);
  Continue          = 0xFFFF != PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET);
  return Continue;
} // ConfigureEP

STATIC
VOID
GetPortResources (
  IN       UINT8      Bus,
  IN       UINT8      Dev,
  IN       UINT8      Fun,
  IN  OUT  PORT_INFO  *PortInfo
  )
{
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment,Bus, Dev, Fun, 0);
  PortInfo->BusNumLimit   = PciSegmentRead8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET);
  PortInfo->IoBase        = PciSegmentRead8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoBase)) & 0xF0;
  PortInfo->IoLimit       = PciSegmentRead8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoLimit)) & 0xF0;
  PortInfo->MemBase       = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase)) & 0xFFF0;
  PortInfo->MemLimit      = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryLimit)) & 0xFFF0;
  PortInfo->PMemBase64    = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryBase)) & 0xFFF0;
  PortInfo->PMemLimit64   = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryLimit)) & 0xFFF0;
  PortInfo->PMemBase64   |= (UINT64)(PciSegmentRead32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableBaseUpper32))) << 16;
  PortInfo->PMemLimit64  |= (UINT64)(PciSegmentRead32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableLimitUpper32))) << 16;
  PortInfo->IoLimit |= 0xF;
  PortInfo->MemLimit |= 0xF;
  PortInfo->PMemLimit64 |= 0xF;
} // GetPortResources

STATIC
VOID
ConfigurePort (
  IN       UINT8      Bus,
  IN       UINT8      Dev,
  IN       UINT8      Fun,
  IN  OUT  PORT_INFO  *PortInfo
  )
{
  INT8  i;
  UINT8 USBusNum;
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment,Bus, Dev, Fun, 0);
  USBusNum          = PciSegmentRead8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, USBusNum, 0x00, 0x00, 0);
  if (0xFFFF == PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET)) {
    //
    // Nothing to do if TBT device is not connected
    //
    return ;
  }

  GetPortResources(Bus, Dev, Fun, PortInfo);// Take reserved resources from DS port
  //
  // Assign resources to EPs
  //
  for (i = 0; i < MAX_TBT_DEPTH; ++i) {
    PortInfo->ConfedEP++;
    if (!ConfigureEP (i, &USBusNum, PortInfo)) {
      return ;
    }
  }
} // ConfigurePort

VOID
ThunderboltCallback (
  IN UINT8 Type
  )
{
  PORT_INFO                     PortInfoOrg  = { 0 };
  HR_CONFIG                     HrConfig  = { 0 };
  UINT8                         i;
  UINTN                         Segment = 0;
  UINTN                         Bus = 0;
  UINTN                         Device;
  UINTN                         Function;

  DEBUG((DEBUG_INFO, "ThunderboltCallback.Entry\n"));

  DEBUG((DEBUG_INFO, "PortInfo Initialization\n"));
  PortInfoInit (&PortInfoOrg);
  if(Type == DTBT_CONTROLLER) {
    if (gCurrentDiscreteTbtRootPort == 0) {
      DEBUG((DEBUG_ERROR, "Invalid RP Input\n"));
      return;
    }
    GetDTbtRpDevFun(gCurrentDiscreteTbtRootPortType, gCurrentDiscreteTbtRootPort - 1, &Device, &Function);
    DEBUG((DEBUG_INFO, "InitializeHostRouter. \n"));
    if (!InitializeHostRouter (&HrConfig, Segment, Bus, Device, Function)) {
      return ;
    }
  //
  // Configure DS ports
  //
  for (i = HrConfig.MinDSNumber; i <= HrConfig.MaxDSNumber; ++i) {
    DEBUG((DEBUG_INFO, "ConfigurePort. \n"));
    ConfigurePort (HrConfig.HRBus + 1, i,0, &PortInfoOrg);
  }

  DEBUG((DEBUG_INFO, "EndOfThunderboltCallback.\n"));
  EndOfThunderboltCallback (Segment, Bus, Device, Function);

  }
  DEBUG((DEBUG_INFO, "ThunderboltCallback.Exit\n"));
} // ThunderboltCallback

VOID
DisablePCIDevicesAndBridges (
  IN UINT8 MinBus,
  IN UINT8 MaxBus
  )
{
  UINT8   Bus;
  UINT8   Dev;
  UINT8   Fun;
  UINT8   RegVal;
  //
  //  Disable PCI device First, and then Disable PCI Bridge
  //
  for (Bus = MaxBus; Bus > MinBus; --Bus) {
    for (Dev = 0; Dev <= PCI_MAX_DEVICE; ++Dev) {
      for (Fun = 0; Fun <= PCI_MAX_FUNC; ++Fun) {
        gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment,Bus, Dev, Fun, 0);
        if (INVALID_PCI_DEVICE == PciSegmentRead32 (gDeviceBaseAddress + PCI_VENDOR_ID_OFFSET)) {
          if (Fun == 0) {
            break;

          }

          continue;
        }

        RegVal = PciSegmentRead8 (gDeviceBaseAddress + PCI_HEADER_TYPE_OFFSET);
        if (HEADER_TYPE_DEVICE == (RegVal & 1)) {
          //
          // ********     Disable PCI Device   ********
          // BIT0  I/O Space Enabled    BIT1  Memory Space Enabled
          // BIT2  Bus Master Enabled   BIT4  Memory Write and Invalidation Enable
          //
          PciSegmentAnd8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, (UINT8)~(BIT0 | BIT1 | BIT2 | BIT4));
          PciSegmentWrite32 (gDeviceBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX0 * 4), 0);
          PciSegmentWrite32 (gDeviceBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX1 * 4), 0);
          PciSegmentWrite32 (gDeviceBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX2 * 4), 0);
          PciSegmentWrite32 (gDeviceBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX3 * 4), 0);
          PciSegmentWrite32 (gDeviceBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX4 * 4), 0);
          PciSegmentWrite32 (gDeviceBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX5 * 4), 0);
        }
      }
    }
  }
  //
  // now no more PCI dev on another side of PCI Bridge can safty disable PCI Bridge
  //
  for (Bus = MaxBus; Bus > MinBus; --Bus) {
    for (Dev = 0; Dev <= PCI_MAX_DEVICE; ++Dev) {
      for (Fun = 0; Fun <= PCI_MAX_FUNC; ++Fun) {
        gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment,Bus, Dev, Fun, 0);
        if (INVALID_PCI_DEVICE == PciSegmentRead32 (gDeviceBaseAddress + PCI_VENDOR_ID_OFFSET)) {
          if (Fun == 0) {
            break;
          }

          continue;
        }

        RegVal = PciSegmentRead8 (gDeviceBaseAddress + PCI_HEADER_TYPE_OFFSET);
        if (HEADER_TYPE_PCI_TO_PCI_BRIDGE == (RegVal & BIT0)) {
          PciSegmentAnd8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, (UINT8)~(BIT0 | BIT1 | BIT2 | BIT4));
          PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, 0);
          PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, 0);
          PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, 0);
          PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableBaseUpper32), 0);
        }
      } // for ( Fun .. )
    } // for ( Dev ... )
  } // for ( Bus ... )
} // DisablePCIDevicesAndBridges

VOID
TbtDisablePCIDevicesAndBridges (
  IN UINT8 Type
  )
{
  UINTN         Segment = 0;
  UINTN         Bus = 0;
  UINTN         Device;
  UINTN         Function;
  UINT8         MinBus;
  UINT8         MaxBus;
  UINT16        DeviceId;

  MinBus = 1;
  if(Type == DTBT_CONTROLLER) {
    //
    // for(Dev = 0; Dev < 8; ++Dev)
    // {
    // PciOr8(PCI_LIB_ADDRESS(2, Dev, 0, PCI_BRIDGE_CONTROL_REGISTER_OFFSET), 0x40);
    // gBS->Stall(2000);      // 2msec
    // PciAnd8(PCI_LIB_ADDRESS(2, Dev, 0, PCI_BRIDGE_CONTROL_REGISTER_OFFSET), 0xBF);
    // }
    // gBS->Stall(200 * 1000);        // 200 msec
    //
    if (gCurrentDiscreteTbtRootPort == 0) {
      DEBUG((DEBUG_ERROR, "Invalid RP Input\n"));
      return;
    }
    GetDTbtRpDevFun(gCurrentDiscreteTbtRootPortType, gCurrentDiscreteTbtRootPort - 1, &Device, &Function);
    gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, Device, Function, 0);
    MinBus            = PciSegmentRead8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
    MaxBus            = PciSegmentRead8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET);
    gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (Segment, MinBus, 0x00, 0x00, 0);
    DeviceId          = PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET);
    if (!(IsTbtHostRouter (DeviceId))) {
      return;
    }
    TbtSegment = (UINT8)Segment;
    MinBus++;
    //
    // @todo : Move this out when we dont have Loop for ITBT
    //
    DisablePCIDevicesAndBridges(MinBus, MaxBus);

  }
} // DisablePCIDevicesAndBridges



