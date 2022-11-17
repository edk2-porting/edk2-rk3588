/** @file
  Policy details for miscellaneous configuration in System Agent

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SA_MISC_PEI_PREMEM_CONFIG_H_
#define _SA_MISC_PEI_PREMEM_CONFIG_H_

#pragma pack(push, 1)

#ifndef MEM_CFG_MAX_SOCKETS
#define MEM_CFG_MAX_SOCKETS 16
#endif

#define SA_MISC_PEI_PREMEM_CONFIG_REVISION 2

/**
  This configuration block is to configure SA Miscellaneous variables during PEI Pre-Mem phase like programming
  different System Agent BARs, TsegSize, MmioSize required etc.
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Deprecate IedSize.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;               ///< Offset 0-27 Config Block Header
  /**
    Offset 28 Memory DIMMs' SPD address for reading SPD data.
    TGL Mapping
      0 - Controller 0 Channel 0 Dimm 0 - DDR4 - DDR5 - LPDDR4 - LPDDR5
      1 - Controller 0 Channel 0 Dimm 1 - DDR4
      2 - Controller 0 Channel 1 Dimm 0 -------- DDR5 - LPDDR4 - LPDDR5
      3 - Controller 0 Channel 1 Dimm 1 -------- DDR5 2DPC
      4 - Controller 0 Channel 2 Dimm 0 --------------- LPDDR4 - LPDDR5
      6 - Controller 0 Channel 3 Dimm 0 --------------- LPDDR4 - LPDDR5
      8 - Controller 1 Channel 0 Dimm 0 - DDR4 - DDR5 - LPDDR4 - LPDDR5
      9 - Controller 1 Channel 0 Dimm 1 - DDR4
     10 - Controller 1 Channel 1 Dimm 0 -------- DDR5 - LPDDR4 - LPDDR5
     11 - Controller 1 Channel 1 Dimm 1 -------- DDR5 2DPC
     12 - Controller 1 Channel 2 Dimm 0 --------------- LPDDR4 - LPDDR5
     14 - Controller 1 Channel 3 Dimm 0 --------------- LPDDR4 - LPDDR5
  **/
  UINT8   SpdAddressTable[MEM_CFG_MAX_SOCKETS];
  VOID    *S3DataPtr;                        ///< Offset 44 Memory data save pointer for S3 resume. The memory space should be allocated and filled with proper S3 resume data on a resume path
  UINT32  SmbusBar;                          ///< Offset 48 Address of System Agent SMBUS BAR: <b>0xEFA0</b>
  /**
    Offset 52 Size of TSEG in bytes. (Must be power of 2)
    <b>0x400000</b>: 4MB for Release build (When IED enabled, it will be 8MB)
    0x1000000      : 16MB for Debug build (Regardless IED enabled or disabled)
  **/
  UINT32  TsegSize;
  /**
    Offset 56
    <b>(Test)</b> Size of IED region in bytes.
    <b>0</b> : IED Disabled (no memory occupied)
    0x400000 : 4MB SMM memory occupied by IED (Part of TSEG)
    <b>Note: Enabling IED may also enlarge TsegSize together.</b>
    @deprecated
  **/
  UINT32  IedSize;
  UINT32  SkipExtGfxScan:1;                  ///< <b>(Test)</b> OFfset 60:0 :1=Skip External Gfx Device Scan; <b>0=Scan for external graphics devices</b>. Set this policy to skip External Graphics card scanning if the platform uses Internal Graphics only.
  UINT32  BdatEnable:1;                      ///< Offset 60:1 :This field enables the generation of the BIOS DATA ACPI Tables: <b>0=FALSE</b>, 1=TRUE.
  UINT32  TxtImplemented:1;                  ///< OFfset 60:2 :This field currently is used to tell MRC if it should run after TXT initializatoin completed: <b>0=Run without waiting for TXT</b>, 1=Run after TXT initialization by callback
  /**
   Offset 60:3 :
   <b>(Test)</b> Scan External Discrete Graphics Devices for Legacy Only VGA OpROMs

   When enabled, if the primary graphics device is an external discrete graphics device, Si will scan the
   graphics device for legacy only VGA OpROMs.

   This is intended to ease the implementation of a BIOS feature to automatically enable CSM if the Primary Gfx device
   only supports Legacy VBIOS (No UEFI GOP Present).  Otherwise disabling CSM won't result in no video being displayed.
   This is useful for platforms that implement PCIe slots that allow the end user to install an arbitrary Gfx device.

   This setting will only take effect if SkipExtGfxScan == 0.  It is ignored otherwise.

  - Disabled (0x0)         : Don't Scan for Legacy Only VGA OpROMs (Default)
  - <b>Enabled</b>  (0x1)  : Scan External Gfx for Legacy Only VGA OpROM
  **/
  UINT32  ScanExtGfxForLegacyOpRom:1;
  UINT32  RsvdBits0  :28;                    ///< Offset 60:4 :Reserved for future use
  UINT8   UserBd;                            ///< Offset 64 <b>0=Mobile/Mobile Halo</b>, 1=Desktop/DT Halo, 5=ULT/ULX/Mobile Halo, 7=UP Server
  UINT8   LockPTMregs;                       ///< <b>(Test)</b> Offset 65 Lock PCU Thermal Management registers: 0=FALSE, <b>1=TRUE</b>
  UINT8   BdatTestType;                      ///< Offset 66 When BdatEnable is set to TRUE, this option selects the type of data which will be populated in the BIOS Data ACPI Tables: <b>0=RMT</b>, 1=RMT Per Bit, 2=Margin 2D.
  UINT8   CridEnable;                        ///< Offset 67 For Platforms supporting Intel(R) SIPP, this policy is use control enable/disable Compatibility Revision ID (CRID) feature: <b>0=FALSE</b>, 1=TRUE
  UINT32  AcpiReservedMemorySize;            ///< Offset 68 The Size of a Reserved memory buffer allocated in previous boot for S3 resume used. Originally it is retrieved from AcpiVariableCompatibility variable.
  UINT32  OpRomScanTempMmioBar;              ///< <b>(Test)</b> Offset 72 Temporary address to MMIO map OpROMs during VGA scanning.  Used for ScanExtGfxForLegacyOpRom feature.  MUST BE 16MB ALIGNED!
  UINT32  OpRomScanTempMmioLimit;            ///< <b>(Test)</b> Offset 76 Limit address for OpROM MMIO range.  Used for ScanExtGfxForLegacyOpRom feature. (OpROMScanTempMmioLimit - OpRomScanTempMmioBar) MUST BE >= 16MB!
  UINT64  AcpiReservedMemoryBase;            ///< Offset 80 The Base address of a Reserved memory buffer allocated in previous boot for S3 resume used. Originally it is retrieved from AcpiVariableCompatibility variable.
  UINT64  SystemMemoryLength;                ///< Offset 88 Total system memory length from previous boot, this is required for S3 resume. Originally it is retrieved from AcpiVariableCompatibility variable.

  UINT8   WrcFeatureEnable;                   ///< Offset 96: Enable/Disable WRC (Write Cache) feature of IOP. When enabled, supports IO devices allocating onto the ring and into LLC.
  UINT8   Reserved1[3];                       ///< Reserved for config block alignment.


  // Since the biggest element is UINT64, this structure should be aligned with 64 bits.
  UINT8   Rsvd[4];                           ///< Reserved for config block alignment.


} SA_MISC_PEI_PREMEM_CONFIG;
#pragma pack(pop)

#endif // _SA_MISC_PEI_PREMEM_CONFIG_H_
