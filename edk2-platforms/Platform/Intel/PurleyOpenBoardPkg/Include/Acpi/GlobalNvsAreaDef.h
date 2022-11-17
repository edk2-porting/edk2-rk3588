/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _EFI_GLOBAL_NVS_AREA_H_
#define _EFI_GLOBAL_NVS_AREA_H_

//
// Global NVS Area definition
//
#pragma pack (1)

typedef struct {
  // IOAPIC Start
  UINT32 PlatformId;
  UINT32 IoApicEnable;
  UINT8  ApicIdOverrided  :1;
  UINT8  RES0           :7;          
  // IOAPIC End

  // Power Management Start
  UINT8  Rsvd_Pms_0     :1;
  UINT8  CStateEnable   :1;
  UINT8  C3Enable       :1;
  UINT8  C6Enable       :1;
  UINT8  C7Enable       :1;
  UINT8  MonitorMwaitEnable :1;
  UINT8  PStateEnable   :1;
  UINT8  EmcaEn         :1;
  UINT8  HWAllEnable    :2;
  UINT8  KBPresent      :1;
  UINT8  MousePresent   :1;
  UINT8  TStateEnable   :1;
  UINT8  TStateFineGrained: 1;
  UINT8  OSCX           :1;
  UINT8  RESX           :1;          
  // Power Management End

  // RAS Start
  UINT8  CpuChangeMask;
  UINT8  IioChangeMask;
  UINT64 IioPresentBitMask;
  UINT32 SocketBitMask;           //make sure this is at 4byte boundary
  UINT32 ProcessorApicIdBase[8];
  UINT64 ProcessorBitMask[8];
  UINT16 MemoryBoardBitMask;
  UINT16 MemoryBoardChgEvent;
  UINT32 MmCfg;
  UINT32 TsegSize;
  UINT64 MemoryBoardBase[8];
  UINT64 MemoryBoardRange[8];
  UINT32 SmiRequestParam[4];
  UINT32 SciRequestParam[4];
  UINT64 MigrationActionRegionAddress;
  UINT8  Cpu0Uuid[16];
  UINT8  Cpu1Uuid[16]; 
  UINT8  Cpu2Uuid[16]; 
  UINT8  Cpu3Uuid[16]; 
  UINT8  Cpu4Uuid[16];
  UINT8  Cpu5Uuid[16]; 
  UINT8  Cpu6Uuid[16]; 
  UINT8  Cpu7Uuid[16]; 
  UINT8  CpuSpareMask;   
  UINT8  Mem0Uuid[16];  
  UINT8  Mem1Uuid[16]; 
  UINT8  Mem2Uuid[16]; 
  UINT8  Mem3Uuid[16]; 
  UINT8  Mem4Uuid[16];  
  UINT8  Mem5Uuid[16]; 
  UINT8  Mem6Uuid[16]; 
  UINT8  Mem7Uuid[16]; 
  UINT8  Mem8Uuid[16];  
  UINT8  Mem9Uuid[16]; 
  UINT8  Mem10Uuid[16]; 
  UINT8  Mem11Uuid[16]; 
  UINT8  Mem12Uuid[16];  
  UINT8  Mem13Uuid[16]; 
  UINT8  Mem14Uuid[16]; 
  UINT8  Mem15Uuid[16]; 
  UINT16 MemSpareMask;
  UINT64 EmcaL1DirAddr;
  UINT32 ProcessorId;
  UINT8  PcieAcpiHotPlugEnable;
  // RAS End

  // VTD Start
  UINT64 DrhdAddr[3];   
  UINT64 AtsrAddr[3];   
  UINT64 RhsaAddr[3];
  // VTD End
  
  // BIOS Guard Start
  UINT8   CpuSkuNumOfBitShift;
  // BIOS Guard End
  
  // USB3 Start
  UINT8   XhciMode;
  UINT8   HostAlertVector1;
  UINT8   HostAlertVector2;
  // USB3 End
  
  // HWPM Start
  UINT8   HWPMEnable:2; //HWPM
  UINT8   AutoCstate:1; //HWPM
  UINT8   HwpInterrupt:1; //HWP Interrupt
  UINT8   RES1:4;       //reserved bits
  // HWPM End

  // PCIe Multi-Seg Start
  // for 8S support needs max 32 IIO IOxAPIC being enabled!
  UINT8   BusBase[48];		  // MAX_SOCKET * MAX_IIO_STACK. Note: hardcode due to ASL constraint. 
  UINT8   PCIe_MultiSeg_Support;    // Enable /Disable switch
  // for 8S support needs matching to MAX_SOCKET!
  UINT8   PcieSegNum[8];	  // Segment number array. Must match MAX_SOCKET. Note: hardcode due to ASL constraint.
  // PCIe Multi-seg end

  // Performance Start
  UINT8   SncAnd2Cluster;           //1=SncEn and NumCluster=2, otherwise 0     
  // Performance End

 } BIOS_ACPI_PARAM;

#pragma pack ()

#endif
