/** @file
  GUID used for Memory Map Data entries in the HOB list.

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MEMORY_MAP_DATA_H_
#define _MEMORY_MAP_DATA_H_

#include "SysHost.h"
#include "PartialMirrorGuid.h"

#define RESERVED_2    2
#define RESERVED_4    4

//
//  System Memory Map HOB information
//

#pragma pack(1)
struct RankDevice {
  UINT8   enabled;                // 0 = disabled,  1 = enabled
  UINT8   logicalRank;            // Logical Rank number (0 - 7)
  UINT16  rankSize;               // Units of 64 MB
};

struct PersisentDpaMap
{
    UINT32 perRegionDPAOffset;
    UINT32 SPALimit;
};

typedef struct DimmDevice {
  UINT8   Present;
  BOOLEAN Enabled;
  UINT8   DcpmmPresent;     // 1 - This is a DCPMM
  UINT8   X4Present;
  UINT8   NumRanks;
  UINT8   keyByte;
  UINT8   actKeyByte2;        // Actual module type reported by SPD
  UINT8   actSPDModuleOrg;    // Actual number of DRAM ranks and device width
  UINT8   dimmTs;             // Thermal sensor data.
  UINT16  VolCap;             // Volatile capacity (AEP DIMM only)
  UINT16  nonVolCap;          // Non-volatile capacity (AEP DIMM only)
  UINT16  DimmSize;
  UINT32  NVmemSize;
  UINT16  SPDMMfgId;          // Module Mfg Id from SPD
  UINT16  VendorID;
  UINT16  DeviceID;
  UINT16  RevisionID;
  UINT32  perRegionDPA;       // DPA of PMEM that Nfit needs
  struct  PersisentDpaMap perDPAMap[MAX_UNIQUE_NGN_DIMM_INTERLEAVE]; // DPA map
  UINT8   serialNumber[NGN_MAX_SERIALNUMBER_STRLEN];  // Serial Number
  UINT8   PartNumber[NGN_MAX_PARTNUMBER_STRLEN];    // Part Number
  UINT8   FirmwareVersionStr[NGN_FW_VER_LEN]; // Used to update the SMBIOS TYPE 17
  struct  firmwareRev FirmwareVersion; // Firmware revision
  struct  RankDevice  rankList[MAX_RANK_DIMM];
  UINT16  InterfaceFormatCode;
  UINT16  SubsystemVendorID;
  UINT16  SubsystemDeviceID;
  UINT16  SubsystemRevisionID;
  UINT16  FisVersion;         // Firmware Interface Specification version
  UINT8   DimmSku;            // Dimm SKU info
  UINT8   manufacturingLocation;  // Manufacturing location for the NVDIMM
  UINT16  manufacturingDate;      // Date the NVDIMM was manufactured
  INT32   commonTck;
  UINT8   EnergyType;     // 0: 12V aux power; 1: dedicated backup energy source; 2: no backup energy source
  BOOLEAN NvDimmNPresent;     ///< JEDEC NVDIMM-N Type Memory Present
  UINT16  SPDRegVen;          ///< Register Vendor ID in SPD
  UINT8   CidBitMap;          // SubRankPer CS for DIMM device
} MEMMAP_DIMM_DEVICE_INFO_STRUCT;

struct ChannelDevice {
  UINT8                               Enabled;                            // 0 = channel disabled, 1 = channel enabled
  UINT8                               Features;                           // Bit mask of features to enable or disable
  UINT8                               MaxDimm;                            // Number of DIMM
  UINT8                               NumRanks;                           // Number of ranks on this channel
  UINT8                               chFailed;
  UINT8                               ngnChFailed;
  UINT8                               SpareLogicalRank[MAX_SPARE_RANK];   // Logical rank, selected as Spare
  UINT8                               SparePhysicalRank[MAX_SPARE_RANK];  // Physical rank, selected as spare
  UINT16                              SpareRankSize[MAX_SPARE_RANK];      // spare rank size
  UINT8                               EnabledLogicalRanks; // Bitmap of Logical ranks that are enabled
  MEMMAP_DIMM_DEVICE_INFO_STRUCT      DimmInfo[MAX_DIMM];
};

struct memcontroller {
  UINT32    MemSize;
};

typedef struct socket {
  UINT8      SocketEnabled;
  UINT16     IioStackBitmap;
  BOOLEAN    Reserved[RESERVED_4];
  UINT8      imcEnabled[MAX_IMC];
  UINT8      SadIntList[MAX_DRAM_CLUSTERS * MAX_SAD_RULES][MC_MAX_NODE]; // SAD interleave list
  UINT8      SktSkuValid;                            // Whether Socket SKU value is valid from PCU
  UINT32     SktSkuLimit;                            // SKU limit value from PCU
  UINT32     SktTotMemMapSPA;                        // Total memory mapped to SPA
  UINT32     SktPmemMapSpa;                          // Total persistent memory mapped to SPA
  UINT32     SktMemSize2LM;                          // Total memory excluded from Limit
  SAD_TABLE  SAD[MAX_DRAM_CLUSTERS * MAX_SAD_RULES];                     // SAD table
  struct     memcontroller imc[MAX_IMC];
  struct     ChannelDevice ChannelInfo[MAX_CH];
} MEMMAP_SOCKET;

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
  //
  // Total Clusters. In SNC2 mode there are 2 clusters and SNC4 mode has 4 clusters.
  // All2All/Quad/Hemi modes can be considered as having only one cluster (i.e SNC1).
  //
  UINT8    TotalClusters;

  MEMORY_MAP_BLOCK_DECODER_DATA BlockDecoderData; // block decoder data structure
  UINT32   lowMemBase;                            // Mem base in 64MB units for below 4GB mem.
  UINT32   lowMemSize;                            // Mem size in 64MB units for below 4GB mem.
  UINT32   highMemBase;                           // Mem base in 64MB units for above 4GB mem.
  UINT32   highMemSize;                           // Mem size in 64MB units for above 4GB mem.
  UINT32   memSize;                               // Total physical memory size
  UINT16   memFreq;                               // Mem Frequency
  UINT8    memMode;                               // 0 - Independent, 1 - Lockstep
  UINT8    volMemMode;                            // 0 - 1LM, 1 - 2LM
  UINT8    CacheMemType;                          // 0 - DDR$DDRT
  UINT8    DdrtIntlvGranularity;                  // 1 - 256B, 2 - 4KB
  UINT16   DramType;
  UINT8    SmbMode[MAX_SOCKET][MAX_SMB_INSTANCE]; // Stores type of smbus mode: 0 - I2C mode, 1 - I3C mode
  UINT8    DdrVoltage;
  UINT8    DcpmmPresent;                        // If at least one DCPMM Present (used by Nfit), then this should get set
  BOOLEAN  EkvPresent;                            // Set if EKV controller on system
  BOOLEAN  BwvPresent;                            // Set if BWV controller on system
  UINT8    XMPProfilesSup;
  UINT16   Reserved1[MAX_SOCKET];
  UINT32   Reserved2;
  UINT32   Reserved3;
  UINT16   Reserved4;
  UINT16   Reserved5;
  UINT8    SystemRasType;
  UINT8    RasModesEnabled;                       // RAS modes that are enabled
  UINT16   ExRasModesEnabled;                     // Extended RAS modes that are enabled
  UINT8    sncEnabled;                            // 0 - SNC disabled for this configuration, 1 - SNC enabled for this configuration
  UINT8    NumOfCluster;
  UINT8    NumChPerMC;
  UINT8    numberEntries;                         // Number of Memory Map Elements
  SYSTEM_MEMORY_MAP_ELEMENT Element[(MAX_SOCKET * MAX_DRAM_CLUSTERS * MAX_SAD_RULES) + MAX_FPGA_REMOTE_SAD_RULES];
  struct   memSetup MemSetup;
  MEM_RESERVED_1    Reserved142;
  MEMMAP_SOCKET Socket[MAX_SOCKET];
  struct   memTiming  profileMemTime[2];

  UINT32    Reserved6;
  UINT8     Reserved7[RESERVED_2];
  EFI_GUID  Reserved8[RESERVED_2];
  UINT8     Reserved9;
  RASMEMORYINFO  RasMeminfo;
  UINT8   LatchSystemShutdownState;
  BOOLEAN IsWpqFlushSupported;
  UINT8   EadrSupport;
  UINT8   EadrCacheFlushMode;
  UINT8   SetSecureEraseSktChHob[MAX_SOCKET][MAX_CH];  //MAX_CH * MAX_SOCKET * MAX_DCPMM_CH
  HOST_DDRT_DIMM_DEVICE_INFO_STRUCT HostDdrtDimmInfo[MAX_SOCKET][MAX_CH];
  UINT32  DdrCacheSize[MAX_SOCKET][MAX_CH];            // Size of DDR memory reserved for 2LM cache (64MB granularity)
  BOOLEAN AdrStateForPmemModule[MAX_SOCKET][MAX_CH];   // ADR state for Intel PMEM Modules
  UINT16  BiosFisVersion;                              // Firmware Interface Specification version currently supported by BIOS
  UINT16  MaxAveragePowerLimit;                        // Max Power limit in mW used for averaged power ( Valid range ends at 15000mW)
  UINT16  MinAveragePowerLimit;                        // Min Power limit in mW used for averaged power ( Valid range starts from 10000mW)
  UINT16  CurrAveragePowerLimit;                       // Current Power limit in mW used for average power
  UINT16  MaxMbbPowerLimit;                            // Max MBB power limit ( Valid range ends at 18000mW).
  UINT16  MinMbbPowerLimit;                            // Min MBB power limit ( Valid range starts from 15000mW).
  UINT16  CurrMbbPowerLimit;                           // Current Power limit in mW used for MBB power
  UINT32  MaxMbbAveragePowerTimeConstant;              // Max MBB Average Power Time Constant
  UINT32  MinMbbAveragePowerTimeConstant;              // Min MBB Average Power Time Constant
  UINT32  CurrMbbAveragePowerTimeConstant;             // Current MBB Average Power Time Constant
  UINT32  MmiohBase;                                   // MMIOH base in 64MB granularity
  UINT8   MaxSadRules;                                 // Maximum SAD entries supported by silicon (24 for 14nm silicon, 16 for 10nm silicon)
  UINT8   NumberofChaDramClusters;                     // Number of CHA DRAM decoder clusters
  UINT8   VirtualNumaEnable;                           // Enable or Disable Virtual NUMA
  UINT8   VirtualNumOfCluster;                         // Number of Virtual NUMA nodes in each physical NUMA node (Socket or SNC cluster)
  BOOLEAN NumaEnable;                                  // Information if NUMA is enabled or not
} SYSTEM_MEMORY_MAP_HOB;

#pragma pack()

#endif // _MEMORY_MAP_DATA_H_
