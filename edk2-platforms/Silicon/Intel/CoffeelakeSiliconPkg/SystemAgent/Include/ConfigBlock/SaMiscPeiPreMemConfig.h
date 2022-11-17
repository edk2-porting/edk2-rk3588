/** @file
  Policy details for miscellaneous configuration in System Agent

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SA_MISC_PEI_PREMEM_CONFIG_H_
#define _SA_MISC_PEI_PREMEM_CONFIG_H_

#pragma pack(push, 1)

#ifndef SA_MC_MAX_SOCKETS
#define SA_MC_MAX_SOCKETS 4
#endif

#define SA_MISC_PEI_PREMEM_CONFIG_REVISION 3

/**
  This configuration block is to configure SA Miscellaneous variables during PEI Pre-Mem phase like programming
  different System Agent BARs, TsegSize, IedSize, MmioSize required etc.
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - add BdatTestType, default is RMT
  <b>Revision 3</b>:
  - Remove SgSubSystemId.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;               ///< Offset 0-27 Config Block Header
  UINT8   SpdAddressTable[SA_MC_MAX_SOCKETS];///< Offset 28 Memory DIMMs' SPD address for reading SPD data. <b>example: SpdAddressTable[0]=0xA2(C0D0), SpdAddressTable[1]=0xA0(C0D1), SpdAddressTable[2]=0xA2(C1D0), SpdAddressTable[3]=0xA0(C1D1)</b>
  VOID    *S3DataPtr;                        ///< Offset 32 Memory data save pointer for S3 resume. The memory space should be allocated and filled with proper S3 resume data on a resume path
  UINT32  MchBar;                            ///< Offset 36 Address of System Agent MCHBAR: <b>0xFED10000</b>
  UINT32  DmiBar;                            ///< Offset 40 Address of System Agent DMIBAR: <b>0xFED18000</b>
  UINT32  EpBar;                             ///< Offset 44 Address of System Agent EPBAR: <b>0xFED19000</b>
  UINT32  SmbusBar;                          ///< Offset 48 Address of System Agent SMBUS BAR: <b>0xEFA0</b>
  UINT32  GdxcBar;                           ///< Offset 52 Address of System Agent GDXCBAR: <b>0xFED84000</b>
  /**
    Offset 56 Size of TSEG in bytes. (Must be power of 2)
    <b>0x400000</b>: 4MB for Release build (When IED enabled, it will be 8MB)
    0x1000000      : 16MB for Debug build (Regardless IED enabled or disabled)
  **/
  UINT32  TsegSize;
  UINT32  EdramBar;                          ///< Offset 60 Address of System Agent EDRAMBAR: <b>0xFED80000</b>
  /**
    Offset 64
    <b>(Test)</b> Size of IED region in bytes.
    <b>0</b> : IED Disabled (no memory occupied)
    0x400000 : 4MB SMM memory occupied by IED (Part of TSEG)
    <b>Note: Enabling IED may also enlarge TsegSize together.</b>
  **/
  UINT32  IedSize;
  UINT8   UserBd;                            ///< Offset 68 <b>0=Mobile/Mobile Halo</b>, 1=Desktop/DT Halo, 5=ULT/ULX/Mobile Halo, 7=UP Server
  UINT8   SgMode;                            ///< Offset 69 SgMode: <b>0=Disabled</b>, 1=SG Muxed, 2=SG Muxless, 3=PEG
  UINT16  SgDelayAfterPwrEn;                 ///< Offset 70 Dgpu Delay after Power enable using Setup option: 0=Minimal, 1000=Maximum, <b>300=300 microseconds</b>
  UINT16  SgDelayAfterHoldReset;             ///< Offset 72 Dgpu Delay after Hold Reset using Setup option: 0=Minimal, 1000=Maximum, <b>100=100 microseconds</b>
  UINT32  SkipExtGfxScan:1;                  ///< <b>(Test)</b> OFfset 74:0 :1=Skip External Gfx Device Scan; <b>0=Scan for external graphics devices</b>. Set this policy to skip External Graphics card scanning if the platform uses Internal Graphics only.
  UINT32  BdatEnable:1;                      ///< Offset 74:1 :This field enables the generation of the BIOS DATA ACPI Tables: <b>0=FALSE</b>, 1=TRUE.
  UINT32  TxtImplemented:1;                  ///< OFfset 74:2 :This field currently is used to tell MRC if it should run after TXT initializatoin completed: <b>0=Run without waiting for TXT</b>, 1=Run after TXT initialization by callback
  /**
   Offset 74:3 :
   <b>(Test)</b> Scan External Discrete Graphics Devices for Legacy Only VGA OpROMs

   When enabled, if the primary graphics device is an external discrete graphics device, Si will scan the
   graphics device for legacy only VGA OpROMs.  If the primary graphics device only implements legacy VBIOS, then the
   LegacyOnlyVgaOpRomDetected field in the SA_DATA_HOB will be set to 1.

   This is intended to ease the implementation of a BIOS feature to automatically enable CSM if the Primary Gfx device
   only supports Legacy VBIOS (No UEFI GOP Present).  Otherwise disabling CSM won't result in no video being displayed.
   This is useful for platforms that implement PCIe slots that allow the end user to install an arbitrary Gfx device.

   This setting will only take effect if SkipExtGfxScan == 0.  It is ignored otherwise.

  - Disabled (0x0)         : Don't Scan for Legacy Only VGA OpROMs (Default)
  - <b>Enabled</b>  (0x1)  : Scan External Gfx for Legacy Only VGA OpROM
  **/
  UINT32  ScanExtGfxForLegacyOpRom:1;
  UINT32  RsvdBits0  :28;                    ///< OFfset 74:4 :Reserved for future use
  UINT8   LockPTMregs;                       ///< <b>(Test)</b> Offset 78 Lock PCU Thermal Management registers: 0=FALSE, <b>1=TRUE</b>
  UINT8   BdatTestType;                      ///< Offset 79 When BdatEnable is set to TRUE, this option selects the type of data which will be populated in the BIOS Data ACPI Tables: <b>0=RMT</b>, 1=RMT Per Bit, 2=Margin 2D.
  UINT8   Rsvd1[4];                          ///< Offset 80 Reserved for future use
  /**
    Offset 84 :
    Size of reserved MMIO space for PCI devices\n
    <b>0=AUTO</b>, 512=512MB, 768=768MB, 1024=1024MB, 1280=1280MB, 1536=1536MB, 1792=1792MB,
    2048=2048MB, 2304=2304MB, 2560=2560MB, 2816=2816MB, 3072=3072MB\n
    When AUTO mode selected, the MMIO size will be calculated by required MMIO size from PCIe devices detected.
  **/
  UINT16  MmioSize;
  INT16   MmioSizeAdjustment;                ///< Offset 86 Increase (given positive value) or Decrease (given negative value) the Reserved MMIO size when Dynamic Tolud/AUTO mode enabled (in MBs): <b>0=no adjustment</b>
  UINT64  AcpiReservedMemoryBase;            ///< Offset 88 The Base address of a Reserved memory buffer allocated in previous boot for S3 resume used. Originally it is retrieved from AcpiVariableCompatibility variable.
  UINT64  SystemMemoryLength;                ///< Offset 96 Total system memory length from previous boot, this is required for S3 resume. Originally it is retrieved from AcpiVariableCompatibility variable.
  UINT32  AcpiReservedMemorySize;            ///< Offset 104 The Size of a Reserved memory buffer allocated in previous boot for S3 resume used. Originally it is retrieved from AcpiVariableCompatibility variable.
  UINT32  OpRomScanTempMmioBar;              ///< <b>(Test)</b> Offset 108 Temporary address to MMIO map OpROMs during VGA scanning.  Used for ScanExtGfxForLegacyOpRom feature.  MUST BE 16MB ALIGNED!
  UINT32  OpRomScanTempMmioLimit;            ///< <b>(Test)</b> Offset 112 Limit address for OpROM MMIO range.  Used for ScanExtGfxForLegacyOpRom feature. (OpROMScanTempMmioLimit - OpRomScanTempMmioBar) MUST BE >= 16MB!

  // Since the biggest element is UINT64, this structure should be aligned with 64 bits.
  UINT8   Rsvd[4];                           ///< Reserved for config block alignment.
} SA_MISC_PEI_PREMEM_CONFIG;
#pragma pack(pop)

#endif // _SA_MISC_PEI_PREMEM_CONFIG_H_
