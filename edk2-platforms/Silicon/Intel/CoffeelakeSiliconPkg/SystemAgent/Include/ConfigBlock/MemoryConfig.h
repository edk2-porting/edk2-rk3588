/** @file
  Policy definition of Memory Config Block

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MEMORY_CONFIG_H_
#define _MEMORY_CONFIG_H_

#include <SaRegs.h>

#pragma pack(push, 1)

#define SA_MRC_ITERATION_MAX      (6)
#define SA_MRC_MAX_RCOMP          (3)
#define SA_MRC_MAX_RCOMP_TARGETS  (5)

#define MEMORY_CONFIG_REVISION 3

///
/// SMRAM Memory Range
///
#define PEI_MR_SMRAM_ABSEG_MASK     0x01
#define PEI_MR_SMRAM_HSEG_MASK      0x02

///
/// SA SPD profile selections.
///
typedef enum {
  Default,             ///< 0, Default SPD
  UserDefined,         ///< 1, User Defined profile
  XMPProfile1,         ///< 2, XMP Profile 1
  XMPProfile2,         ///< 3, XMP Profile 2
  XMPProfileMax = 0xFF ///< Ensures SA_SPD is UINT8
} SA_SPD;

///
/// Define the boot modes used by the SPD read function.
///
typedef enum {
  SpdCold,       ///< Cold boot
  SpdWarm,       ///< Warm boot
  SpdS3,         ///< S3 resume
  SpdFast,       ///< Fast boot
  SpdBootModeMax ///< Delimiter
} SPD_BOOT_MODE;

typedef struct {
  UINT8 SpdData[SA_MC_MAX_CHANNELS][SA_MC_MAX_SLOTS][SA_MC_MAX_SPD_SIZE];
//Next Field Offset 2048
} SPD_DATA_BUFFER;

typedef struct {
  UINT8 DqByteMap[SA_MC_MAX_CHANNELS][SA_MRC_ITERATION_MAX][2];
//Next Field Offset 24
} SA_MEMORY_DQ_MAPPING;

typedef struct {
  UINT8 DqsMapCpu2Dram[SA_MC_MAX_CHANNELS][SA_MC_MAX_BYTES_NO_ECC];
//Next Field Offset 16
} SA_MEMORY_DQS_MAPPING;

typedef struct {
  UINT16  RcompResistor[SA_MRC_MAX_RCOMP];       ///< Offset 0: Reference RCOMP resistors on motherboard
  UINT16  RcompTarget[SA_MRC_MAX_RCOMP_TARGETS]; ///< Offset 6: RCOMP target values for DqOdt, DqDrv, CmdDrv, CtlDrv, ClkDrv
//Next Field Offset 16
} SA_MEMORY_RCOMP;

typedef struct {
  UINT16 Start;           ///< Offset 0
  UINT16 End;             ///< Offset 2
  UINT8  BootMode;        ///< Offset 4
  UINT8  Reserved3[3];    ///< Offset 5 Reserved for future use
} SPD_OFFSET_TABLE;

///
/// SA memory address decode.
///
typedef struct
{
  UINT8  Controller; ///< Offset 0 Zero based Controller number
  UINT8  Channel;    ///< Offset 1 Zero based Channel number
  UINT8  Dimm;       ///< Offset 2 Zero based DIMM number
  UINT8  Rank;       ///< Offset 3 Zero based Rank number
  UINT8  BankGroup;  ///< Offset 4 Zero based Bank Group number
  UINT8  Bank;       ///< Offset 5 Zero based Bank number
  UINT16 Cas;        ///< Offset 6 Zero based CAS number
  UINT32 Ras;        ///< Offset 8 Zero based RAS number
} SA_ADDRESS_DECODE;

typedef UINT8      (EFIAPI * SA_IO_READ_8)               (UINTN IoAddress);
typedef UINT16     (EFIAPI * SA_IO_READ_16)              (UINTN IoAddress);
typedef UINT32     (EFIAPI * SA_IO_READ_32)              (UINTN IoAddress);
typedef UINT8      (EFIAPI * SA_IO_WRITE_8)              (UINTN IoAddress, UINT8 Value);
typedef UINT16     (EFIAPI * SA_IO_WRITE_16)             (UINTN IoAddress, UINT16 Value);
typedef UINT32     (EFIAPI * SA_IO_WRITE_32)             (UINTN IoAddress, UINT32 Value);
typedef UINT8      (EFIAPI * SA_MMIO_READ_8)             (UINTN Address);
typedef UINT16     (EFIAPI * SA_MMIO_READ_16)            (UINTN Address);
typedef UINT32     (EFIAPI * SA_MMIO_READ_32)            (UINTN Address);
typedef UINT64     (EFIAPI * SA_MMIO_READ_64)            (UINTN Address);
typedef UINT8      (EFIAPI * SA_MMIO_WRITE_8)            (UINTN Address, UINT8 Value);
typedef UINT16     (EFIAPI * SA_MMIO_WRITE_16)           (UINTN Address, UINT16 Value);
typedef UINT32     (EFIAPI * SA_MMIO_WRITE_32)           (UINTN Address, UINT32 Value);
typedef UINT64     (EFIAPI * SA_MMIO_WRITE_64)           (UINTN Address, UINT64 Value);
typedef UINT8      (EFIAPI * SA_SMBUS_READ_8)            (UINTN Address, RETURN_STATUS *Status);
typedef UINT16     (EFIAPI * SA_SMBUS_READ_16)           (UINTN Address, RETURN_STATUS *Status);
typedef UINT8      (EFIAPI * SA_SMBUS_WRITE_8)           (UINTN Address, UINT8 Value, RETURN_STATUS *Status);
typedef UINT16     (EFIAPI * SA_SMBUS_WRITE_16)          (UINTN Address, UINT16 Value, RETURN_STATUS *Status);
typedef UINT32     (EFIAPI * SA_GET_PCI_DEVICE_ADDRESS)  (UINT8 Bus, UINT8 Device, UINT8 Function, UINT8 Offset);
typedef UINT32     (EFIAPI * SA_GET_PCIE_DEVICE_ADDRESS) (UINT8 Bus, UINT8 Device, UINT8 Function, UINT8 Offset);
typedef VOID       (EFIAPI * SA_GET_RTC_TIME)            (UINT8 *Second, UINT8 *Minute, UINT8 *Hour, UINT8 *Day, UINT8 *Month, UINT16 *Year);
typedef UINT64     (EFIAPI * SA_GET_CPU_TIME)            (VOID *GlobalData);
typedef VOID *     (EFIAPI * SA_MEMORY_COPY)             (VOID *Destination, CONST VOID *Source, UINTN NumBytes);
typedef VOID *     (EFIAPI * SA_MEMORY_SET_BYTE)         (VOID *Buffer, UINTN NumBytes, UINT8 Value);
typedef VOID *     (EFIAPI * SA_MEMORY_SET_WORD)         (VOID *Buffer, UINTN NumWords, UINT16 Value);
typedef VOID *     (EFIAPI * SA_MEMORY_SET_DWORD)        (VOID *Buffer, UINTN NumDwords, UINT32 Value);
typedef UINT64     (EFIAPI * SA_LEFT_SHIFT_64)           (UINT64 Data, UINTN NumBits);
typedef UINT64     (EFIAPI * SA_RIGHT_SHIFT_64)          (UINT64 Data, UINTN NumBits);
typedef UINT64     (EFIAPI * SA_MULT_U64_U32)            (UINT64 Multiplicand, UINT32 Multiplier);
typedef UINT64     (EFIAPI * SA_DIV_U64_U64)             (UINT64 Dividend, UINT64 Divisor, UINT64 *Remainder);
typedef BOOLEAN    (EFIAPI * SA_GET_SPD_DATA)            (SPD_BOOT_MODE BootMode, UINT8 SpdAddress, UINT8 *Buffer, UINT8 *Ddr3Table, UINT32 Ddr3TableSize, UINT8 *Ddr4Table, UINT32 Ddr4TableSize, UINT8 *LpddrTable, UINT32 LpddrTableSize);
typedef UINT8      (EFIAPI * SA_GET_MC_ADDRESS_DECODE)   (UINT64 Address, SA_ADDRESS_DECODE *DramAddress);
typedef UINT8      (EFIAPI * SA_GET_MC_ADDRESS_ENCODE)   (SA_ADDRESS_DECODE *DramAddress, UINT64 Address);
typedef BOOLEAN    (EFIAPI * SA_GET_RANDOM_NUMBER)       (UINT32 *Rand);
typedef EFI_STATUS (EFIAPI * SA_CPU_MAILBOX_READ)        (UINT32 Type, UINT32 Command, UINT32 *Value, UINT32 *Status);
typedef EFI_STATUS (EFIAPI * SA_CPU_MAILBOX_WRITE)       (UINT32 Type, UINT32 Command, UINT32 Value, UINT32 *Status);
typedef UINT32     (EFIAPI * SA_GET_MEMORY_VDD)          (VOID *GlobalData, UINT32 DefaultVdd);
typedef UINT32     (EFIAPI * SA_SET_MEMORY_VDD)          (VOID *GlobalData, UINT32 DefaultVdd, UINT32 Value);
typedef UINT32     (EFIAPI * SA_CHECKPOINT)              (VOID *GlobalData, UINT32 CheckPoint, VOID *Scratch);
typedef VOID       (EFIAPI * SA_DEBUG_HOOK)              (VOID *GlobalData, UINT16 DisplayDebugNumber);
typedef UINT8      (EFIAPI * SA_CHANNEL_EXIST)           (VOID *Outputs, UINT8 Channel);
typedef INT32      (EFIAPI * SA_PRINTF)                  (VOID *Debug, UINT32 Level, char *Format, ...);
typedef VOID       (EFIAPI * SA_DEBUG_PRINT)             (VOID *String);
typedef UINT32     (EFIAPI * SA_CHANGE_MARGIN)           (VOID *GlobalData, UINT8 Param, INT32 Value0, INT32 Value1, UINT8 EnMultiCast, UINT8 Channel, UINT8 RankIn, UINT8 Byte, UINT8 BitIn, UINT8 UpdateMrcData, UINT8 SkipWait, UINT32 RegFileParam);
typedef UINT8      (EFIAPI * SA_SIGN_EXTEND)             (UINT8 Value, UINT8 OldMsb, UINT8 NewMsb);
typedef VOID       (EFIAPI * SA_SHIFT_PI_COMMAND_TRAIN)  (VOID *GlobalData, UINT8 Channel, UINT8 Iteration, UINT8 RankMask, UINT8 GroupMask, INT32 NewValue, UINT8 UpdateHost);
typedef VOID       (EFIAPI * SA_UPDATE_VREF)             (VOID *GlobalData, UINT8 Channel, UINT8 RankMask, UINT16 DeviceMask, UINT8 VrefType, INT32 Offset, BOOLEAN UpdateMrcData, BOOLEAN PDAmode, BOOLEAN SkipWait);
typedef UINT8      (EFIAPI * SA_GET_RTC_CMOS)            (UINT8 Location);
typedef UINT64     (EFIAPI * SA_MSR_READ_64)             (UINT32 Location);
typedef UINT64     (EFIAPI * SA_MSR_WRITE_64)            (UINT32 Location, UINT64 Data);
typedef UINT32     (EFIAPI * SA_THERMAL_OVERRIDES)       (VOID *GlobalData);
typedef VOID       (EFIAPI * SA_MRC_RETURN_FROM_SMC)     (VOID *GlobalData, UINT32 MrcStatus);
typedef VOID       (EFIAPI * SA_MRC_DRAM_RESET)          (UINT32 PciEBaseAddress, UINT32 ResetValue);
typedef VOID       (EFIAPI * SA_SET_LOCK_PRMRR)          (UINT32 PrmrrBaseAddress, UINT32 PrmrrSize);


///
/// Function calls into the SA.
///
typedef struct {
  SA_IO_READ_8               IoRead8;               ///< Offset 0:   - CPU I/O port 8-bit read.
  SA_IO_READ_16              IoRead16;              ///< Offset 4:   - CPU I/O port 16-bit read.
  SA_IO_READ_32              IoRead32;              ///< Offset 8:   - CPU I/O port 32-bit read.
  SA_IO_WRITE_8              IoWrite8;              ///< Offset 12:  - CPU I/O port 8-bit write.
  SA_IO_WRITE_16             IoWrite16;             ///< Offset 16:  - CPU I/O port 16-bit write.
  SA_IO_WRITE_32             IoWrite32;             ///< Offset 20:  - CPU I/O port 32-bit write.
  SA_MMIO_READ_8             MmioRead8;             ///< Offset 24:  - Memory Mapped I/O port 8-bit read.
  SA_MMIO_READ_16            MmioRead16;            ///< Offset 28:  - Memory Mapped I/O port 16-bit read.
  SA_MMIO_READ_32            MmioRead32;            ///< Offset 32:  - Memory Mapped I/O port 32-bit read.
  SA_MMIO_READ_64            MmioRead64;            ///< Offset 36:  - Memory Mapped I/O port 64-bit read.
  SA_MMIO_WRITE_8            MmioWrite8;            ///< Offset 40:  - Memory Mapped I/O port 8-bit write.
  SA_MMIO_WRITE_16           MmioWrite16;           ///< Offset 44:  - Memory Mapped I/O port 16-bit write.
  SA_MMIO_WRITE_32           MmioWrite32;           ///< Offset 48:  - Memory Mapped I/O port 32-bit write.
  SA_MMIO_WRITE_64           MmioWrite64;           ///< Offset 52:  - Memory Mapped I/O port 64-bit write.
  SA_SMBUS_READ_8            SmbusRead8;            ///< Offset 56:  - Smbus 8-bit read.
  SA_SMBUS_READ_16           SmbusRead16;           ///< Offset 60:  - Smbus 16-bit read.
  SA_SMBUS_WRITE_8           SmbusWrite8;           ///< Offset 64:  - Smbus 8-bit write.
  SA_SMBUS_WRITE_16          SmbusWrite16;          ///< Offset 68:  - Smbus 16-bit write.
  SA_GET_PCI_DEVICE_ADDRESS  GetPciDeviceAddress;   ///< Offset 72:  - Get PCI device address.
  SA_GET_PCIE_DEVICE_ADDRESS GetPcieDeviceAddress;  ///< Offset 76:  - Get PCI express device address.
  SA_GET_RTC_TIME            GetRtcTime;            ///< Offset 80:  - Get the current time value.
  SA_GET_CPU_TIME            GetCpuTime;            ///< Offset 84:  - The current CPU time in milliseconds.
  SA_MEMORY_COPY             CopyMem;               ///< Offset 88:  - Perform byte copy operation.
  SA_MEMORY_SET_BYTE         SetMem;                ///< Offset 92:  - Perform byte initialization operation.
  SA_MEMORY_SET_WORD         SetMemWord;            ///< Offset 96:  - Perform word initialization operation.
  SA_MEMORY_SET_DWORD        SetMemDword;           ///< Offset 100: - Perform dword initialization operation.
  SA_LEFT_SHIFT_64           LeftShift64;           ///< Offset 104: - Left shift the 64-bit data value by specified number of bits.
  SA_RIGHT_SHIFT_64          RightShift64;          ///< Offset 108: - Right shift the 64-bit data value by specified number of bits.
  SA_MULT_U64_U32            MultU64x32;            ///< Offset 112: - Multiply a 64-bit data value by a 32-bit data value.
  SA_DIV_U64_U64             DivU64x64;             ///< Offset 116: - Divide a 64-bit data value by a 64-bit data value.
  SA_GET_SPD_DATA            GetSpdData;            ///< Offset 120: - Read the SPD data over the SMBus, at the given SmBus SPD address and copy the data to the data structure.
  SA_GET_RANDOM_NUMBER       GetRandomNumber;       ///< Offset 124: - Get the next random 32-bit number.
  SA_CPU_MAILBOX_READ        CpuMailboxRead;        ///< Offset 128: - Perform a CPU mailbox read.
  SA_CPU_MAILBOX_WRITE       CpuMailboxWrite;       ///< Offset 132: - Perform a CPU mailbox write.
  SA_GET_MEMORY_VDD          GetMemoryVdd;          ///< Offset 136: - Get the current memory voltage (VDD).
  SA_SET_MEMORY_VDD          SetMemoryVdd;          ///< Offset 140: - Set the memory voltage (VDD) to the given value.
  SA_CHECKPOINT              CheckPoint;            ///< Offset 144: - Check point that is called at various points in the MRC.
  SA_DEBUG_HOOK              DebugHook;             ///< Offset 148: - Typically used to display to the I/O port 80h.
  SA_DEBUG_PRINT             DebugPrint;            ///< Offset 152: - Output a string to the debug stream/device.
  SA_GET_RTC_CMOS            GetRtcCmos;            ///< Offset 156: - Get the current value of the specified RTC CMOS location.
  SA_MSR_READ_64             ReadMsr64;             ///< Offset 160: - Get the current value of the specified MSR location.
  SA_MSR_WRITE_64            WriteMsr64;            ///< Offset 164  - Set the current value of the specified MSR location.
  SA_MRC_RETURN_FROM_SMC     MrcReturnFromSmc;      ///< Offset 168  - Hook function after returning from MrcStartMemoryConfiguration()
  SA_MRC_DRAM_RESET          MrcDramReset;          ///< Offset 172  - Assert or deassert DRAM_RESET# pin; this is used in JEDEC Reset.
} SA_FUNCTION_CALLS;

///
/// Function calls into the MRC.
///
typedef struct {
  SA_CHANNEL_EXIST           MrcChannelExist;       ///< Offset 0:  - Returns whether Channel is or is not present.
  SA_PRINTF                  MrcPrintf;             ///< Offset 4:  - Print to output stream/device.
  SA_CHANGE_MARGIN           MrcChangeMargin;       ///< Offset 8:  - Change the margin.
  SA_SIGN_EXTEND             MrcSignExtend;         ///< Offset 12: - Sign extends OldMSB to NewMSB Bits (Eg: Bit 6 to Bit 7).
  SA_SHIFT_PI_COMMAND_TRAIN  ShiftPiCommandTrain;   ///< Offset 16: - Move CMD/CTL/CLK/CKE PIs during training.
  SA_UPDATE_VREF             MrcUpdateVref;         ///< Offset 20: - Update the Vref value and wait until it is stable.
} SA_MEMORY_FUNCTIONS;

/**
 Memory Configuration
 The contents of this structure are CRC'd by the MRC for option change detection.
 This structure is copied en mass to the MrcInput structure. If you add fields here, you must update the MrcInput structure.
 <b>Revision 1</b>:
 - Initial version.
 <b>Revision 2</b>:
 - Removed GearType.
 - Added Lp4DqsOscEn, RMTLoopCount, EnBER, DualDimmPerChannelBoardType.
 <b>Revision 3</b>:
 - Removed EvLoader, EvLoaderDelay.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;    ///< Offset 0-27 Config Block Header
  UINT16  Size;                   ///< Offset 28 The size of this structure, in bytes. Must be the first entry in this structure.
  UINT8   HobBufferSize;          ///< Offset 30 Size of HOB buffer for MRC

  UINT8   SpdProfileSelected;     ///< Offset 31 SPD XMP profile selection - for XMP supported DIMM: <b>0=Default DIMM profile</b>, 1=Customized profile, 2=XMP profile 1, 3=XMP profile 2.

  // The following parameters are used only when SpdProfileSelected is UserDefined (CUSTOM PROFILE)
  UINT16  tCL;                    ///< Offset 32 User defined Memory Timing tCL value,   valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 31=Maximum.
  UINT16  tRCDtRP;                ///< Offset 34 User defined Memory Timing tRCD value (same as tRP), valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 63=Maximum
  UINT16  tRAS;                   ///< Offset 36 User defined Memory Timing tRAS value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 64=Maximum.
  UINT16  tWR;                    ///< Offset 38 User defined Memory Timing tWR value,   valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, legal values: 5, 6, 7, 8, 10, 12, 14, 16, 18, 20, 24.
  UINT16  tRFC;                   ///< Offset 40 User defined Memory Timing tRFC value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 1023=Maximum.
  UINT16  tRRD;                   ///< Offset 42 User defined Memory Timing tRRD value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 15=Maximum.
  UINT16  tWTR;                   ///< Offset 44 User defined Memory Timing tWTR value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 28=Maximum.
  UINT16  tRTP;                   ///< Offset 46 User defined Memory Timing tRTP value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 15=Maximum. DDR4 legal values: 5, 6, 7, 8, 9, 10, 12
  UINT16  tFAW;                   ///< Offset 48 User defined Memory Timing tFAW value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 63=Maximum.
  UINT16  tCWL;                   ///< Offset 50 User defined Memory Timing tCWL value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 20=Maximum.
  UINT16  tREFI;                  ///< Offset 52 User defined Memory Timing tREFI value, valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 65535=Maximum.
  UINT16  PciIndex;               ///< Offset 54 Pci index register address: <b>0xCF8=Default</b>
  UINT16  PciData;                ///< Offset 56 Pci data register address: <b>0xCFC=Default</b>
  UINT16  VddVoltage;             ///< Offset 58 DRAM voltage (Vdd) in millivolts: <b>0=Platform Default (no override)</b>, 1200=1.2V, 1350=1.35V etc.
  UINT16  Idd3n;                  ///< Offset 60 EPG Active standby current (Idd3N) in milliamps from DIMM datasheet.
  UINT16  Idd3p;                  ///< Offset 62 EPG Active power-down current (Idd3P) in milliamps from DIMM datasheet.

  UINT32  EccSupport:1;                   ///< Offset 64 DIMM Ecc Support option - for Desktop only: 0=Disable, <b>1=Enable</b>
  UINT32  MrcSafeConfig:1;                ///<  - MRC Safe Mode: <b>0=Disable</b>, 1=Enable
  UINT32  RemapEnable:1;                  ///<  - This option is used to control whether to enable/disable memory remap above 4GB: 0=Disable, <b>1=Enable</b>.
  UINT32  ScramblerSupport:1;             ///<  - Memory scrambler support: 0=Disable, <b>1=Enable</b>
  UINT32  Vc1ReadMeter:1;                 ///<  - VC1 Read Metering Enable: 0=Disable, <b>1=Enable</b>
  UINT32  DdrThermalSensor:1;             ///<  - Ddr Thermal Sensor: 0=Disable, <b>1=Enable</b>
  UINT32  LpddrMemWriteLatencySet:1;      ///<  - LPDDR3 Write Latency Set option: 0=Set A, <b>1=Set B</b>
  UINT32  Off64Bits7to8Rsvd:2;            ///<  - Bit 7-8 Reserved
  UINT32  SimicsFlag:1;                   ///<  - Option to Enable SIMICS: 0=Disable, <b>1=Enable</b>
  UINT32  Ddr4DdpSharedClock:1;           ///<  - Select if CLK0 is shared between Rank0 and Rank1 in DDR4 DDP package. <b>0=Not shared</b>, 1=Shared
  UINT32  SharedZqPin:1;                  ///<  - Select if ZQ pin is shared between Rank ranks.  For CFL, this option only works for DDR4.  the option works for LPDDR4 and DDR4. <b>0=Not shared</b>, 1=Shared
  // Thermal Management
  UINT32  ThermalManagement:1;            ///<  - <CFL> Memory Thermal Management Support: <b>0=Disable</b>, 1=Enable.
  UINT32  PeciInjectedTemp:1;             ///<  - <CFL> Enable/Disable memory temperatures to be injected to the processor via PECI: <b>0=Disable</b>, 1=Enable.
  UINT32  ExttsViaTsOnBoard:1;            ///<  - <CFL> Enable/Disable routing TS-on-Board's ALERT# and THERM# to EXTTS# pins on the PCH: <b>0=Disable</b>, 1=Enable.
  UINT32  ExttsViaTsOnDimm:1;             ///<  - <CFL> Enable/Disable routing TS-on-DIMM's ALERT# to EXTTS# pin on the PCH: <b>0=Disable</b>, 1=Enable.
  UINT32  VirtualTempSensor:1;            ///<  - <CFL> Enable/Disable Virtual Temperature Sensor (VTS): <b>0=Disable</b>, 1=Enable.
  UINT32  Lp4DqsOscEn :1;                 ///<  - <CNL> LpDdrDqDqsReTraining - DqDqsReTraining Enable: 0=Disable, <b>1=Enable</b>
  UINT32  DualDimmPerChannelBoardType:1;  ///<  - <CFL> DualDimmPerChannelBoardType - Option to indicate if the Memory Design for the board includes two DIMMs per channel: <b>0=Single DIMM Design</b>, 1=Dual DIMM Design
  UINT32  ReservedBits1:13;
  /**
   Disables a DIMM slot in the channel even if a DIMM is present\n
   Array index represents the channel number (0 = channel 0, 1 = channel 1)\n
     <b>0x0 = DIMM 0 and DIMM 1 enabled</b>\n
     0x1 = DIMM 0 disabled, DIMM 1 enabled\n
     0x2 = DIMM 0 enabled, DIMM 1 disabled\n
     0x3 = DIMM 0 and DIMM 1 disabled (will disable the whole channel)\n
  **/
  UINT8   DisableDimmChannel[SA_MC_MAX_CHANNELS]; ///< Offset 68
  /**
   Selects the ratio to multiply the reference clock by for the DDR frequency\n
   When RefClk is 133MHz\n
   <b>0x00 = Auto</b>, 0x03 through 0x0C are valid values, all others are invalid\n
   When RefClk is 100MHz\n
   <b>0x00 = Auto</b>, 0x06 through 0x10 are valid values, all others are invalid\n
  **/
  UINT8   Ratio;                  ///< Offset 70
  UINT8   ProbelessTrace;         ///< Offset 71 Probeless Trace: <b>0=Disabled</b>, <b>1=Enabled</b>
  UINT32  BClkFrequency;          ///< Offset 72 Base reference clock value, in Hertz: <b>100000000 = 100Hz</b>, 125000000=125Hz, 167000000=167Hz, 250000000=250Hz
  /**
     - Channel Hash Enable.\n
    NOTE: BIT7 will interleave the channels at a 2 cache-line granularity, BIT8 at 4 and BIT9 at 8\n
    0=BIT6, <B>1=BIT7</B>, 2=BIT8, 3=BIT9
  **/
  UINT8   ChHashInterleaveBit;    ///< Offset 76 Option to select interleave Address bit. Valid values are 0 - 3 for BITS 6 - 9 (Valid values for BDW are 0-7 for BITS 6 - 13)
  UINT8   EnergyScaleFact;        ///< Offset 77 Energy Scale Factor. 0=Minimal, 7=Maximum, <b>4=Default</b>
  BOOLEAN PerBankRefresh;         ///< <CNL> Offset 78 Enables and Disables the per bank refresh.  This only impacts memory technologies that support PBR: LPDDR3, LPDDR4.  FALSE=Disabled, <b>TRUE=Enabled</b>
  UINT8   McLock;                 ///< <CFL> Offset 79 Enable/Disable memory configuration register locking: 0=Disable, <b>1=Enable</b>.
  // Training Algorithms 1
  UINT32 ECT:1;                   ///< Offset 80 Enable/Disable Early Command Training. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 SOT:1;                   ///<  - Enable/Disable Sense Amp Offset Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 ERDMPRTC2D:1;            ///<  - Enable/Disable Early ReadMPR Timing Centering 2D. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 RDMPRT:1;                ///<  - Enable/Disable Read MPR Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 RCVET:1;                 ///<  - Enable/Disable Receive Enable Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 JWRL:1;                  ///<  - Enable/Disable JEDEC Write Leveling Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 EWRTC2D:1;               ///<  - Enable/Disable Early Write Time Centering 2D Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 ERDTC2D:1;               ///<  - Enable/Disable Early Read Time Centering 2D Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 WRTC1D:1;                ///<  - Enable/Disable 1D Write Timing Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 WRVC1D:1;                ///<  - Enable/Disable 1D Write Voltage Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 RDTC1D:1;                ///<  - Enable/Disable 1D Read Timing Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 DIMMODTT:1;              ///<  - Enable/Disable DIMM ODT Training. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 DIMMRONT:1;              ///<  - Enable/Disable DIMM RON training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 WRDSEQT:1;               ///<  - Enable/Disable Write Drive Strength / Equalization Training 2D. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 WRSRT:1;                 ///<  - Enable/Disable Write Slew Rate traning. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable.</b>
  UINT32 RDODTT:1;                ///<  - Enable/Disable Read ODT Training. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 RDEQT:1;                 ///<  - Enable/Disable Read Equalization Training. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 RDAPT:1;                 ///<  - Enable/Disable Read Amplifier Power Training. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 WRTC2D:1;                ///<  - Enable/Disable 2D Write Timing Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 RDTC2D:1;                ///<  - Enable/Disable 2D Read Timing Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 WRVC2D:1;                ///<  - Enable/Disable 2D Write Voltage Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 RDVC2D:1;                ///<  - Enable/Disable 2D Read Voltage Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 CMDVC:1;                 ///<  - Enable/Disable Command Vref Centering Training. Note it is not recommended to change this setting from the default value 0=Disable, <b>1=Enable</b>.
  UINT32 LCT:1;                   ///<  - Enable/Disable Late Command Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 RTL:1;                   ///<  - Enable/Disable Round Trip Latency function. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 TAT:1;                   ///<  - Enable/Disable Turn Around Time function. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 RMT:1;                   ///<  - Enable/Disable Rank Margin Tool function: <b>0=Disable</b>, 1=Enable.
  UINT32 MEMTST:1;                ///<  - Enable/Disable Memory Test function: <b>0=Disable</b>, 1=Enable.
  UINT32 ALIASCHK:1;              ///<  - Enable/Disable DIMM SPD Alias Check: 0=Disable, <b>1=Enable</b>
  UINT32 RCVENC1D:1;              ///<  - Enable/Disable Receive Enable Centering Training (LPDDR Only). Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable
  UINT32 RMC:1;                   ///<  - Enable/Disable Retrain Margin Check.  Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable
  UINT32 WRDSUDT:1;               ///<  - Enable/Disable Write Drive Strength Up/Dn independently. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable
  // Training Algorithms 2
  UINT32  CMDSR    : 1;           ///< <CFL> Offset 84 - Enable/Disable CMD Slew Rate Training: 0=Disable, <b>1=Enable</b>.
  UINT32  CMDDSEQ  : 1;           ///< <CFL>  - Enable/Disable CMD Drive Strength and Tx Equalization: 0=Disable, <b>1=Enable</b>.
  UINT32  CMDNORM  : 1;           ///< <CFL>  - Enable/Disable CMD Normalization: 0=Disable, <b>1=Enable</b>.
  UINT32  EWRDSEQ  : 1;           ///< <CFL>  - Enable/Disable Early DQ Write Drive Strength and Equalization Training: 0=Disable, <b>1=Enable</b>.
  UINT32  RDVC1D   : 1;           ///< <CNL>  - Enable/Disable Read Voltage Centering 1D
  UINT32  TXTCO    : 1;           ///< <CNL>  - Enable/Disable Write TCO Comp Training
  UINT32  CLKTCO   : 1;           ///< <CNL>  - Enable/Disable Clock TCO Comp Training
  UINT32  ReservedBits2 :25;

  UINT32  OddRatioMode:1;             ///< Offset 88 If Odd Ratio Mode is enabled, QCLK frequency has an addition of 133/100 MHz: <b>0=Disable</b>, 1=Enable
  UINT32  MrcTimeMeasure:1;           ///<  - Enables serial debug level to display the MRC execution times only: <b>0=Disable</b>, 1=Enable
  UINT32  MrcFastBoot:1;              ///<  - Enables the MRC fast boot path for faster cold boot execution: 0=Disable, <b>1=Enable</b>
  UINT32  DqPinsInterleaved:1;        ///<  - Interleaving mode of DQ/DQS pins for HSW_ULT which depends on board routing: <b>0=Disable</b>, 1=Enable
  UINT32  RankInterleave:1;           ///<  - Rank Interleave Mode: 0=Disable, <b>1=Enable</b>
  UINT32  EnhancedInterleave:1;       ///<  - Enhanced Interleave Mode: 0=Disable, <b>1=Enable</b>
  UINT32  WeaklockEn:1;               ///<  - Weak Lock Enable: 0=Disable, <b>1=Enable</b>
  UINT32  CmdTriStateDis:1;           ///<  - CMD Tri-State Support: <b>0=Enable</b>, 1=Disable. Note: This should be set to 1 (Disable) if Command RTT is not present on the platform.
  UINT32  MemoryTrace:1;              ///<  - Memory Trace to second DDR channel using Stacked Mode: <b>0=Disable</b>, 1=Enable
  UINT32  ChHashEnable:1;             ///<  - Channel Hash Enable: 0=Disable, <b>1=Enable</b>
  UINT32  EnableExtts:1;              ///<  - Enable Extts: <b>0=Disable</b>, 1=Enable
  UINT32  EnableCltm:1;               ///<  - Enable Closed Loop Thermal Management: <b>0=Disable</b>, 1=Enable
  UINT32  EnableOltm:1;               ///<  - Enable Open Loop Thermal Management: <b>0=Disable</b>, 1=Enable
  UINT32  EnablePwrDn:1;              ///<  - Enable Power Down control for DDR: 0=PCODE control, <b>1=BIOS control</b>
  UINT32  EnablePwrDnLpddr:1;         ///<  - Enable Power Down for LPDDR: 0=PCODE control, <b>1=BIOS control</b>
  UINT32  LockPTMregs:1;              ///<  - Lock PCU Thermal Management registers: 0=Disable, <b>1=Enable</b>
  UINT32  UserPowerWeightsEn:1;       ///<  - Allows user to explicitly set power weight, scale factor, and channel power floor values: <b>0=Disable</b>, 1=Enable
  UINT32  RaplLim2Lock:1;             ///<  - Lock DDR_RAPL_LIMIT register: <b>0=Disable</b>, 1=Enable
  UINT32  RaplLim2Ena:1;              ///<  - Enable Power Limit 2: <b>0=Disable</b>, 1=Enable
  UINT32  RaplLim1Ena:1;              ///<  - Enable Power Limit 1: <b>0=Disable</b>, 1=Enable
  UINT32  SrefCfgEna:1;               ///<  - Enable Self Refresh: 0=Disable, <b>1=Enable</b>
  UINT32  ThrtCkeMinDefeatLpddr:1;    ///<  - Throttler CKE min defeature for LPDDR: 0=Disable, <b>1=Enable</b>
  UINT32  ThrtCkeMinDefeat:1;         ///<  - Throttler CKE min defeature: <b>0=Disable</b>, 1=Enable
  UINT32  AutoSelfRefreshSupport:1;   ///<  - FALSE = No auto self refresh support, <b>TRUE = auto self refresh support</b>
  UINT32  ExtTemperatureSupport:1;    ///<  - FALSE = No extended temperature support, <b>TRUE = extended temperature support</b>
  UINT32  MobilePlatform:1;           ///<  - Memory controller device id indicates: <b>TRUE if mobile</b>, FALSE if not. Note: This will be auto-detected and updated.
  UINT32  Force1Dpc:1;                ///<  - TRUE means force one DIMM per channel, <b>FALSE means no limit</b>
  UINT32  ForceSingleRank:1;          ///<  - TRUE means use Rank0 only (in each DIMM): <b>0=Disable</b>, 1=Enable
  UINT32  RhPrevention:1;             ///<  - RH Prevention Enable/Disable: 0=Disable, <b>1=Enable</b>
  UINT32  VttTermination:1;           ///<  - Vtt Termination for Data ODT: <b>0=Disable</b>, 1=Enable
  UINT32  VttCompForVsshi:1;          ///<  - Enable/Disable Vtt Comparator For Vsshi: <b>0=Disable</b>, 1=Enable
  UINT32  ExitOnFailure:1;            ///<  - MRC option for exit on failure or continue on failure: 0=Disable, <b>1=Enable</b>

  UINT32  VddSettleWaitTime;      ///< Offset 92 Amount of time in microseconds to wait for Vdd to settle on top of 200us required by JEDEC spec: <b>Default=0</b>
  UINT16  FreqSaGvLow;            ///< Offset 96 SA GV: 0 is Auto/default, otherwise holds the frequency value: <b>0=Default</b>, 1067, 1200, 1333, 1400, 1600, 1800, 1867.
  UINT16  SrefCfgIdleTmr;         ///< Offset 98 Self Refresh idle timer: <b>512=Minimal</b>, 65535=Maximum
  UINT8   RhActProbability;       ///< Offset 100 Activation probability for Hardware RHP
  UINT8   SmramMask;              ///< Offset 101 Reserved memory ranges for SMRAM
  UINT16  Vc1ReadMeterThreshold;  ///< <CFL> Offset 102 VC1 Read Meter Threshold (within Time Window): 0=Minimal, 0xFFFF=Maximum, <b>0x118=Default</b>
  UINT32  Vc1ReadMeterTimeWindow; ///< <CFL> Offset 104 VC1 Read Meter Time Window: 0=Minimal, 0x1FFFF=Maximum, <b>0x320=Default</b>
  UINT64  BerAddress[4];          ///< Offset 108 - 139 BER Address(es): <b>0=Minimal</b>, 0xFFFFFFFFFFFFFFFF=Maximum (step is 0x40)

  UINT16  ChHashMask;             ///< Offset 140 Channel Hash Mask: 0x0001=BIT6 set(Minimal), 0x3FFF=BIT[19:6] set(Maximum), <b>0x30CE= BIT[19:18, 13:12 ,9:7] set</b>
  UINT16  DdrFreqLimit;           ///< Offset 142 Memory Frequency setting: 3=1067, 5=1333, 7=1600, 9=1867, 11=2133, 13=2400, <b>15=2667</b>
  UINT8   RaplLim2WindX;          ///< Offset 144 Power Limit 2 Time Window X value: 0=Minimal, 3=Maximum, <b>1=Default</b>
  UINT8   RaplLim2WindY;          ///< Offset 145 Power Limit 2 Time Window Y value: 0=Minimal, 3=Maximum, <b>1=Default</b>
  UINT8   RaplLim1WindX;          ///< Offset 146 Power Limit 1 Time Window X value: <b>0=Minimal</b>, 3=Maximum
  UINT8   RaplLim1WindY;          ///< Offset 147 Power Limit 1 Time Window Y value: <b>0=Minimal</b>, 31=Maximum
  UINT16  RaplLim2Pwr;            ///< Offset 148  Power Limit 2: 0=Minimal, 16383=Maximum, <b>222=Default</b>
  UINT16  RaplLim1Pwr;            ///< Offset 150  Power Limit 1: <b>0=Minimal</b>, 16383=Maximum
  UINT8   WarmThresholdCh0Dimm0;  ///< Offset 152 Warm Threshold (Channel 0, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8   WarmThresholdCh0Dimm1;  ///< Offset 153 Warm Threshold (Channel 0, Dimm 1): 0=Minimal, <b>255=Maximum</b>
  UINT8   WarmThresholdCh1Dimm0;  ///< Offset 154 Warm Threshold (Channel 1, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8   WarmThresholdCh1Dimm1;  ///< Offset 155 Warm Threshold (Channel 1, Dimm 1): 0=Minimal, <b>255=Maximum</b>
  UINT8   HotThresholdCh0Dimm0;   ///< Offset 156 Hot Threshold (Channel 0, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8   HotThresholdCh0Dimm1;   ///< Offset 157 Hot Threshold (Channel 0, Dimm 1): 0=Minimal, <b>255=Maximum</b>
  UINT8   HotThresholdCh1Dimm0;   ///< Offset 158 Hot Threshold (Channel 1, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8   HotThresholdCh1Dimm1;   ///< Offset 159 Hot Threshold (Channel 1, Dimm 1): 0=Minimal, <b>255=Maximum</b>
  UINT8   WarmBudgetCh0Dimm0;     ///< Offset 160 Warm Budget (Channel 0, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8   WarmBudgetCh0Dimm1;     ///< Offset 161 Warm Budget (Channel 0, Dimm 1): 0=Minimal, <b>255=Maximum</b>
  UINT8   WarmBudgetCh1Dimm0;     ///< Offset 162 Warm Budget (Channel 1, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8   WarmBudgetCh1Dimm1;     ///< Offset 163 Warm Budget (Channel 1, Dimm 1): 0=Minimal, <b>255=Maximum</b>
  UINT8   HotBudgetCh0Dimm0;      ///< Offset 164 Hot Budget (Channel 0, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8   HotBudgetCh0Dimm1;      ///< Offset 165 Hot Budget (Channel 0, Dimm 1): 0=Minimal, <b>255=Maximum</b>
  UINT8   HotBudgetCh1Dimm0;      ///< Offset 166 Hot Budget (Channel 1, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8   HotBudgetCh1Dimm1;      ///< Offset 167 Hot Budget (Channel 1, Dimm 1): 0=Minimal, <b>255=Maximum</b>
  UINT8   IdleEnergyCh0Dimm0;     ///< Offset 168 Idle Energy (Channel 0, Dimm 0): 0=Minimal, 63=Maximum, <b>10=Default</b>
  UINT8   IdleEnergyCh0Dimm1;     ///< Offset 169 Idle Energy (Channel 0, Dimm 1): 0=Minimal, 63=Maximum, <b>10=Default</b>
  UINT8   IdleEnergyCh1Dimm0;     ///< Offset 170 Idle Energy (Channel 1, Dimm 0): 0=Minimal, 63=Maximum, <b>10=Default</b>
  UINT8   IdleEnergyCh1Dimm1;     ///< Offset 171 Idle Energy (Channel 1, Dimm 1): 0=Minimal, 63=Maximum, <b>10=Default</b>
  UINT8   PdEnergyCh0Dimm0;       ///< Offset 172 Power Down Energy (Channel 0, Dimm 0): 0=Minimal, 63=Maximum, <b>6=Default</b>
  UINT8   PdEnergyCh0Dimm1;       ///< Offset 173 Power Down Energy (Channel 0, Dimm 1): 0=Minimal, 63=Maximum, <b>6=Default</b>
  UINT8   PdEnergyCh1Dimm0;       ///< Offset 174 Power Down Energy (Channel 1, Dimm 0): 0=Minimal, 63=Maximum, <b>6=Default</b>
  UINT8   PdEnergyCh1Dimm1;       ///< Offset 175 Power Down Energy (Channel 1, Dimm 1): 0=Minimal, 63=Maximum, <b>6=Default</b>
  UINT8   ActEnergyCh0Dimm0;      ///< Offset 176 Activation Energy (Channel 0, Dimm 0): 0=Minimal, 255=Maximum, <b>172=Default</b>
  UINT8   ActEnergyCh0Dimm1;      ///< Offset 177 Activation Energy (Channel 0, Dimm 1): 0=Minimal, 255=Maximum, <b>172=Default</b>
  UINT8   ActEnergyCh1Dimm0;      ///< Offset 178 Activation Energy (Channel 1, Dimm 0): 0=Minimal, 255=Maximum, <b>172=Default</b>
  UINT8   ActEnergyCh1Dimm1;      ///< Offset 179 Activation Energy (Channel 1, Dimm 1): 0=Minimal, 255=Maximum, <b>172=Default</b>
  UINT8   RdEnergyCh0Dimm0;       ///< Offset 180 Read Energy (Channel 0, Dimm 0): 0=Minimal, 255=Maximum, <b>212=Default</b>
  UINT8   RdEnergyCh0Dimm1;       ///< Offset 181 Read Energy (Channel 0, Dimm 1): 0=Minimal, 255=Maximum, <b>212=Default</b>
  UINT8   RdEnergyCh1Dimm0;       ///< Offset 182 Read Energy (Channel 1, Dimm 0): 0=Minimal, 255=Maximum, <b>212=Default</b>
  UINT8   RdEnergyCh1Dimm1;       ///< Offset 183 Read Energy (Channel 1, Dimm 1): 0=Minimal, 255=Maximum, <b>212=Default</b>
  UINT8   WrEnergyCh0Dimm0;       ///< Offset 184 Write Energy (Channel 0, Dimm 0): 0=Minimal, 255=Maximum, <b>221=Default</b>
  UINT8   WrEnergyCh0Dimm1;       ///< Offset 185 Write Energy (Channel 0, Dimm 1): 0=Minimal, 255=Maximum, <b>221=Default</b>
  UINT8   WrEnergyCh1Dimm0;       ///< Offset 186 Write Energy (Channel 1, Dimm 0): 0=Minimal, 255=Maximum, <b>221=Default</b>
  UINT8   WrEnergyCh1Dimm1;       ///< Offset 187 Write Energy (Channel 1, Dimm 1): 0=Minimal, 255=Maximum, <b>221=Default</b>


  UINT8   MaxRttWr;               ///< Offset 188 Maximum DIMM RTT_WR to use in power training: <b>0=ODT Off</b>, 1 = 120 ohms
  UINT8   ThrtCkeMinTmr;          ///< Offset 189 Throttler CKE min timer: 0=Minimal, 0xFF=Maximum, <b>0x30=Default</b>
  UINT8   ThrtCkeMinTmrLpddr;     ///< Offset 190 Throttler CKE min timer for LPDDR: 0=Minimal, 0xFF=Maximum, <b>0x40=Default</b>
  UINT8   BerEnable;              ///< Offset 191 BER Enable and # of Addresses passed in: <b>0=Minimal</b>, 8=Maximum
  UINT8   CkeRankMapping;         ///< Offset 192 Bits [7:4] - Channel 1, bits [3:0] - Channel 0. <b>0xAA=Default</b> Bit [i] specifies which rank CKE[i] goes to.
  UINT8   StrongWkLeaker;         ///< Offset 193 Strong Weak Leaker: 1=Minimal, <b>7=Maximum</b>
  UINT8   CaVrefConfig;           ///< Offset 194 0=VREF_CA goes to both CH_A and CH_B, 1=VREF_CA to CH_A, VREF_DQ_A to CH_B, <b>2=VREF_CA to CH_A, VREF_DQ_B to CH_B</b>
  UINT8   SaGv;                   ///< Offset 195 SA GV: 0=Disabled; 1=FixedLow; CFL: 2=FixedHigh, CNL: 2=FixedMid; <b>CFL: 3=Enabled</b> CNL: 3=FixedHigh; <b>CNL: 4=Enabled</b>
  UINT8   RaplPwrFlCh1;           ///< Offset 196 Power Channel 1 Floor value: <b>0=Minimal</b>, 255=Maximum
  UINT8   RaplPwrFlCh0;           ///< Offset 197 Power Channel 0 Floor value: <b>0=Minimal</b>, 255=Maximum
  UINT8   NModeSupport;           ///< Offset 198 Memory N Mode Support - Enable user to select Auto, 1N or 2N: <b>0=AUTO</b>, 1=1N, 2=2N.
  UINT8   RefClk;                 ///< Offset 199 Selects the DDR base reference clock. 0x01 = 100MHz, <b>0x00 = 133MHz</b>
  UINT8   EnCmdRate;              ///< Offset 200 CMD Rate Enable: 0=Disable, 1=1 CMD, 2=2 CMDs, <b>3=3 CMDs</b>, 4=4 CMDs, 5=5 CMDs, 6=6 CMDs, 7=7 CMDs
  UINT8   Refresh2X;              ///< Offset 201 Refresh 2x: <b>0=Disable</b>, 1=Enable for WARM or HOT, 2=Enable for HOT only
  UINT8   EpgEnable;              ///< Offset 202 Enable Energy Performance Gain.
  UINT8   RhSolution;             ///< Offset 203 Type of solution to be used for RHP - 0/1 = HardwareRhp/Refresh2x
  UINT8   UserThresholdEnable;    ///< Offset 204 Flag to manually select the DIMM CLTM Thermal Threshold, 0=Disable,  1=Enable, <b>0=Default</b>
  UINT8   UserBudgetEnable;       ///< Offset 205 Flag to manually select the Budget Registers for CLTM Memory Dimms , 0=Disable,  1=Enable, <b>0=Default</b>
  UINT8   TsodTcritMax;           ///< Offset 206 TSOD Tcrit Maximum Value  to be Configure , 0=Minimal, 128=Maximum, , <b>105=Default</b>

  UINT8   TsodEventMode;          ///< Offset 207 Flag to Enable Event Mode Interruption in TSOD Configuration Register, 0=Disable,  1=Enable, <b>1=Default</b>
  UINT8   TsodEventPolarity;      ///< Offset 208 Event Signal Polarity in TSOD Configuration Register, 0=Low,  1=High, <b>0=Default</b>
  UINT8   TsodCriticalEventOnly;  ///< Offset 209 Critical Trigger Only in TSOD Configuration Register,0=Disable,  1=Enable, <b>1=Default</b>
  UINT8   TsodEventOutputControl; ///< Offset 210 Event Output Control in TSOD Configuration Register,0=Disable,  1=Enable, <b>1=Default</b>
  UINT8   TsodAlarmwindowLockBit; ///< Offset 211 Alarm Windows Lock Bit in TSOD Configuration Register,0=Unlock,  1=Lock, <b>0=Default</b>
  UINT8   TsodCriticaltripLockBit;///< Offset 212 Critical Trip Lock Bit in TSOD Configuration Register,0=Unlock,  1=Lock, <b>0=Default</b>
  UINT8   TsodShutdownMode;       ///< Offset 213 Shutdown Mode TSOD Configuration Register,0=Enable,  1=Disable, <b>0=Default</b>
  UINT8   TsodThigMax;            ///< Offset 214 Thigh Max Value In the  for CLTM Memory Dimms , 0=Disable,  1=Enable, <b>0=Default</b>
  UINT8   TsodManualEnable;       ///< Offset 215 Flag to manually select the TSOD Register Values , 0=Disable,  1=Enable, <b>0=Default</b>
  UINT8   DllBwEn0;               ///< Offset 216 DllBwEn value for 1067
  UINT8   DllBwEn1;               ///< Offset 217 DllBwEn value for 1333
  UINT8   DllBwEn2;               ///< Offset 218 DllBwEn value for 1600
  UINT8   DllBwEn3;               ///< Offset 219 DllBwEn value for 1867 and up
  UINT8   RetrainOnFastFail;      ///< Offset 220 Restart MRC in Cold mode if SW MemTest fails during Fast flow. 0 = Disabled, <b>1 = Enabled</b>
  UINT8   ForceOltmOrRefresh2x;   ///< Offset 221 Force OLTM or 2X Refresh when needed. <b>0 = Force OLTM</b>, 1 = Force 2x Refresh
  UINT8   PowerDownMode;          ///< Offset 222 CKE Power Down Mode: <b>0xFF=AUTO</b>, 0=No Power Down, 1= APD mode, 6=PPD-DLL Off mode
  UINT8   PwdwnIdleCounter;       ///< Offset 223 CKE Power Down Mode Idle Counter: 0=Minimal, 255=Maximum, <b>0x80=0x80 DCLK</b>
  UINT8   IsvtIoPort;             ///< Offset 224 ISVT IO Port Address: 0=Minimal, 0xFF=Maximum, <b>0x99=Default</b>
  UINT8   CmdRanksTerminated;     ///< <CNL> Offset 225 LPDDR4: Bitmask of ranks that have CA bus terminated. <b>0x01=Default, Rank0 is terminating and Rank1 is non-terminating</b>
  UINT8   GdxcEnable;             ///< <CFL> Offset 226 GDXC  MOT enable
  UINT8   GdxcIotSize;            ///< <CFL> Offset 227 IOT size in multiples of 8MEG
  UINT8   GdxcMotSize;            ///< <CFL> Offset 228 MOT size in multiples of 8MEG
  UINT8   RMTLoopCount;           ///< Offset 229 Indicates the Loop Count to be used for Rank Margin Tool Testing: 1=Minimal, 32=Maximum, 0=AUTO, <b>0=Default</b>
  UINT16  FreqSaGvMid;            ///< Offset 230 SA GV: 0 is Auto/default, otherwise holds the frequency value expressed as an integer: <b>0=Default</b>, 1600, 1800, 1867, 2000, 2133, etc.

  UINT32  RmtPerTask:1;                 ///< Offset 232 Bit 0: Rank Margin Tool Per Task. <b>0 = Disabled</b>, 1 = Enabled
  UINT32  TrainTrace:1;                 ///< Offset 232 Bit 1: Trained state tracing debug. <b>0 = Disabled</b>, 1 = Enabled
  UINT32  SafeMode:1;                   ///< Offset 232 Bit 2: Define if safe mode is enabled for MC/IO
  UINT32  EnBER:1;                      ///< Offset 232 Bit 3: Define if EnBER is enabled for Rank Margin Tool
  UINT32  Ddr4MixedUDimm2DpcLimit:1;    ///< Offset 232 Bit 4: Enable/Disable 2667 Frequency Limitation for DDR4 U-DIMM Mixed Dimm 2DPC population. 0 = Disabled, <b>1 = Enabled</b>
  UINT32  FastBootRmt:1;                ///< Offset 232 Bit 5: Enable/Disable RMT on FastBoot. <b>0 = Disabled</b>, 1 = Enabled
  UINT32  MrcTrainOnWarm:1;             ///< Offset 232 Bit 6: Force MRC training on warm boot : <b>0 = Disabled</b>, 1 = Enabled
  UINT32  LongFlyByModeEnabled:1;       ///< Offset 232 Bit 7: Long FlyBy Mode Enabled : <b>0 = Disabled</b>, 1 = Enabled
  UINT32  Off232RsvdBits:24;            ///< Offset 232 Bit 8-31: Reserved

  // TurnAround Timing
  UINT8   tRd2RdSG;               ///< Offset 236 - Read-to-Read   Same Group      Turn Around Timing: <b>0=AUTO</b>, 4=Minimal, 64=Maximum.
  UINT8   tRd2RdDG;               ///< Offset 237 - Read-to-Read   Different Group Turn Around Timing: <b>0=AUTO</b>, 4=Minimal, 64=Maximum. Same Group and Different Group Timings must be the same for Non-DDR4 memory.
  UINT8   tRd2RdDR;               ///< Offset 238 - Read-to-Read   Different Rank  Turn Around Timing: <b>0=AUTO</b>, 4=Minimal, 64=Maximum.
  UINT8   tRd2RdDD;               ///< Offset 239 - Read-to-Read   Different DIMM  Turn Around Timing: <b>0=AUTO</b>, 4=Minimal, 64=Maximum.
  UINT8   tRd2WrSG;               ///< Offset 240 - Read-to-Write  Same Group      Turn Around Timing: <b>0=AUTO</b>, 4=Minimal, 64=Maximum.
  UINT8   tRd2WrDG;               ///< Offset 241 - Read-to-Write  Different Group Turn Around Timing: <b>0=AUTO</b>, 4=Minimal, 64=Maximum. Same Group and Different Group Timings must be the same for Non-DDR4 memory.
  UINT8   tRd2WrDR;               ///< Offset 242 - Read-to-Write  Different Rank  Turn Around Timing: <b>0=AUTO</b>, 4=Minimal, 64=Maximum.
  UINT8   tRd2WrDD;               ///< Offset 243 - Read-to-Write  Different DIMM  Turn Around Timing: <b>0=AUTO</b>, 4=Minimal, 64=Maximum.
  UINT8   tWr2RdSG;               ///< Offset 244 - Write-to-Read  Same Group      Turn Around Timing: <b>0=AUTO</b>, 4=Minimal, 86=Maximum.
  UINT8   tWr2RdDG;               ///< Offset 245 - Write-to-Read  Different Group Turn Around Timing: <b>0=AUTO</b>, 4=Minimal, 64=Maximum. Same Group and Different Group Timings must be the same for Non-DDR4 memory.
  UINT8   tWr2RdDR;               ///< Offset 246 - Write-to-Read  Different Rank  Turn Around Timing: <b>0=AUTO</b>, 4=Minimal, 64=Maximum.
  UINT8   tWr2RdDD;               ///< Offset 247 - Write-to-Read  Different DIMM  Turn Around Timing: <b>0=AUTO</b>, 4=Minimal, 64=Maximum.
  UINT8   tWr2WrSG;               ///< Offset 248 - Write-to-Write Same Group      Turn Around Timing: <b>0=AUTO</b>, 4=Minimal, 64=Maximum.
  UINT8   tWr2WrDG;               ///< Offset 249 - Write-to-Write Different Group Turn Around Timing: <b>0=AUTO</b>, 4=Minimal, 64=Maximum. Same Group and Different Group Timings must be the same for Non-DDR4 memory.
  UINT8   tWr2WrDR;               ///< Offset 250 - Write-to-Write Different Rank  Turn Around Timing: <b>0=AUTO</b>, 4=Minimal, 64=Maximum.
  UINT8   tWr2WrDD;               ///< Offset 251 - Write-to-Write Different DIMM  Turn Around Timing: <b>0=AUTO</b>, 4=Minimal, 64=Maximum.
  UINT16  tRRD_L;                 ///< <CFL> Offset 252 - User defined DDR4 Memory Timing tRRD_L value,  valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, 15=Maximum.
  UINT16  tRRD_S;                 ///< <CFL> Offset 254 - User defined DDR4 Memory Timing tRRD_S value,  valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, 15=Maximum.
  UINT16  tWTR_L;                 ///< <CFL> Offset 266 - User defined DDR4 Memory Timing tWTR_L value,  valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, 28=Maximum.
  UINT16  tWTR_S;                 ///< <CFL> Offset 268 - User defined DDR4 Memory Timing tWTR_S value,  valid when MemoryProfile is CUSTOM_PROFILE: <b>0=AUTO</b>, 28=Maximum.

} MEMORY_CONFIGURATION;

/// Memory Configuration
/// The contents of this structure are not CRC'd by the MRC for option change detection.
typedef struct {
  CONFIG_BLOCK_HEADER      Header;              ///< Offset 0-23 Config Block Header
  SA_FUNCTION_CALLS        SaCall;              ///< Offset 24   Function calls into the SA.
  SA_MEMORY_FUNCTIONS      MrcCall;             ///< Offset 200  Function calls into the MRC.
  SPD_DATA_BUFFER          *SpdData;            ///< Offset 236  Memory SPD data, will be used by the MRC when SPD SmBus address is zero.
  SA_MEMORY_DQ_MAPPING     *DqByteMap;          ///< Offset 240  LPDDR3 DQ byte mapping to CMD/CTL/CLK, from the CPU side.
  SA_MEMORY_DQS_MAPPING    *DqsMap;             ///< Offset 244  LPDDR3 DQS byte swizzling between CPU and DRAM.
  SA_MEMORY_RCOMP          *RcompData;          ///< Offset 248  DDR RCOMP resistors and target values.
  UINT64                   PlatformMemorySize;  ///< Offset 252  The minimum platform memory size required to pass control into DXE
  UINT32                   CleanMemory:1;       ///< Offset 260  Ask MRC to clear memory content: <b>FALSE=Do not Clear Memory</b>; TRUE=Clear Memory
  UINT32                   MemTestOnWarmBoot:1; ///< Offset 260  Run Base Memory Test On WarmBoot:  0=Disabled, <b>1=Enabled</b>
  UINT32                   ReservedBits5:30;
  /**
   Sets the serial debug message level\n
     0x00 = Disabled\n
     0x01 = Errors only\n
     0x02 = Errors and Warnings\n
     <b>0x03 = Errors, Warnings, and Info</b>\n
     0x04 = Errors, Warnings, Info, and Events\n
     0x05 = Displays Memory Init Execution Time Summary only\n
  **/
  UINT8   SerialDebugLevel;                     ///< Offset 264
  UINT8   Reserved11[3];                        ///< Offset 265  Reserved
} MEMORY_CONFIG_NO_CRC;
#pragma pack(pop)

#endif // _MEMORY_CONFIG_H_
