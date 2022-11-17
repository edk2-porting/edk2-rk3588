/** @file
  Policy definition of Memory Config Block

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _MEMORY_CONFIG_H_
#define _MEMORY_CONFIG_H_


#pragma pack(push, 1)

// MEMORY_CONFIG_REVISION 3 adds DDR5 PDA Enumeration training within MEMORY_CONFIGURATION
// MEMORY_CONFIG_REVISION 4 adds LPDDR4 Command Mirroring within MEMORY_CONFIGURATION
// MEMORY_CONFIG_REVISION 5 adds CpuBclkSpread option within MEMORY_CONFIGURATION
// MEMORY_CONFIG_REVISION 6 adds McParity option within MEMORY_CONFIGURATION
// MEMORY_CONFIG_REVISION 7 adds VddqVoltageOverride option within MEMORY_CONFIGURATION
// MEMORY_CONFIG_REVISION 8 adds ExtendedBankHashing option within MEMORY_CONFIGURATION
// MEMORY_CONFIG_REVISION 9 adds IbeccErrorInj option within MEMORY_CONFIGURATION
#define MEMORY_CONFIG_REVISION 9
///
/// MEMORY_CONFIG interface definitions
///
#define MRC_MAX_RCOMP_TARGETS  5
///
/// Memory SubSystem Definitions
///
#define MEM_CFG_MAX_CONTROLLERS          2
#define MEM_CFG_MAX_CHANNELS             4
#define MEM_CFG_MAX_CHANNEL_SHARE_REGS   2
#define MEM_CFG_MAX_DIMMS                2
#define MEM_CFG_MAX_RANKS_PER_DIMM       2
#define MEM_CFG_NUM_BYTES_MAPPED         2
#define MEM_CFG_MAX_SPD_SIZE             1024
#define MEM_CFG_MAX_SOCKETS              (MEM_CFG_MAX_CONTROLLERS * MEM_CFG_MAX_CHANNELS * MEM_CFG_MAX_DIMMS)
#define MEM_CFG_MAX_ROWS                 (MEM_CFG_MAX_RANKS_PER_DIMM * MEM_CFG_MAX_SOCKETS)
#ifndef MEM_MAX_SAGV_POINTS
#define MEM_MAX_SAGV_POINTS                  4
#endif
#define MEM_MAX_IBECC_REGIONS                8
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

/**
  SPD Data Buffer
**/
typedef struct {
  UINT8 SpdData[MEM_CFG_MAX_CONTROLLERS][MEM_CFG_MAX_CHANNELS][MEM_CFG_MAX_DIMMS][MEM_CFG_MAX_SPD_SIZE];  ///< SpdData
//Next Field Offset 2048
} SPD_DATA_BUFFER;

/**
  DqDqs Mapping
**/
typedef struct {
  UINT8 DqsMapCpu2Dram[MEM_CFG_MAX_CONTROLLERS][MEM_CFG_MAX_CHANNELS][MEM_CFG_NUM_BYTES_MAPPED];  ///< DqsMapCpu2Dram
  UINT8 DqMapCpu2Dram[MEM_CFG_MAX_CONTROLLERS][MEM_CFG_MAX_CHANNELS][MEM_CFG_NUM_BYTES_MAPPED][8];  ///< DqMapCpu2Dram
//Next Field Offset 16
} SA_MEMORY_DQDQS_MAPPING;

/**
  Rcomp Policies
**/
typedef struct {
  UINT16  RcompResistor;                      ///< Offset 0: Reference RCOMP resistors on motherboard ~ 100 ohms
  UINT16  RcompTarget[MRC_MAX_RCOMP_TARGETS]; ///< Offset 1: RCOMP target values for DqOdt, DqDrv, CmdDrv, CtlDrv, ClkDrv
//Next Field Offset 16
} SA_MEMORY_RCOMP;

/**
  SPD Offset Table
**/
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

typedef UINT8      (EFIAPI * SA_IO_READ_8)               (UINTN IoAddress);                                                                                                                                                                               ///< CPU I/O port 8-bit read.
typedef UINT16     (EFIAPI * SA_IO_READ_16)              (UINTN IoAddress);                                                                                                                                                                               ///< CPU I/O port 16-bit read.
typedef UINT32     (EFIAPI * SA_IO_READ_32)              (UINTN IoAddress);                                                                                                                                                                               ///< CPU I/O port 32-bit read.
typedef UINT8      (EFIAPI * SA_IO_WRITE_8)              (UINTN IoAddress, UINT8 Value);                                                                                                                                                                  ///< CPU I/O port 8-bit write.
typedef UINT16     (EFIAPI * SA_IO_WRITE_16)             (UINTN IoAddress, UINT16 Value);                                                                                                                                                                 ///< CPU I/O port 16-bit write.
typedef UINT32     (EFIAPI * SA_IO_WRITE_32)             (UINTN IoAddress, UINT32 Value);                                                                                                                                                                 ///< CPU I/O port 32-bit write.
typedef UINT8      (EFIAPI * SA_MMIO_READ_8)             (UINTN Address);                                                                                                                                                                                 ///< Memory Mapped I/O port 8-bit read.
typedef UINT16     (EFIAPI * SA_MMIO_READ_16)            (UINTN Address);                                                                                                                                                                                 ///< Memory Mapped I/O port 16-bit read.
typedef UINT32     (EFIAPI * SA_MMIO_READ_32)            (UINTN Address);                                                                                                                                                                                 ///< Memory Mapped I/O port 32-bit read.
typedef UINT64     (EFIAPI * SA_MMIO_READ_64)            (UINTN Address);                                                                                                                                                                                 ///< Memory Mapped I/O port 64-bit read.
typedef UINT8      (EFIAPI * SA_MMIO_WRITE_8)            (UINTN Address, UINT8 Value);                                                                                                                                                                    ///< Memory Mapped I/O port 8-bit write.
typedef UINT16     (EFIAPI * SA_MMIO_WRITE_16)           (UINTN Address, UINT16 Value);                                                                                                                                                                   ///< Memory Mapped I/O port 16-bit write.
typedef UINT32     (EFIAPI * SA_MMIO_WRITE_32)           (UINTN Address, UINT32 Value);                                                                                                                                                                   ///< Memory Mapped I/O port 32-bit write.
typedef UINT64     (EFIAPI * SA_MMIO_WRITE_64)           (UINTN Address, UINT64 Value);                                                                                                                                                                   ///< Memory Mapped I/O port 64-bit write.
typedef UINT8      (EFIAPI * SA_SMBUS_READ_8)            (UINTN Address, RETURN_STATUS *Status);                                                                                                                                                          ///< Smbus 8-bit read.
typedef UINT16     (EFIAPI * SA_SMBUS_READ_16)           (UINTN Address, RETURN_STATUS *Status);                                                                                                                                                          ///< Smbus 16-bit read.
typedef UINT8      (EFIAPI * SA_SMBUS_WRITE_8)           (UINTN Address, UINT8 Value, RETURN_STATUS *Status);                                                                                                                                             ///< Smbus 8-bit write.
typedef UINT16     (EFIAPI * SA_SMBUS_WRITE_16)          (UINTN Address, UINT16 Value, RETURN_STATUS *Status);                                                                                                                                            ///< Smbus 16-bit write.
typedef UINT32     (EFIAPI * SA_GET_PCI_DEVICE_ADDRESS)  (UINT8 Bus, UINT8 Device, UINT8 Function, UINT8 Offset);                                                                                                                                         ///< Get PCI device address.
typedef UINT32     (EFIAPI * SA_GET_PCIE_DEVICE_ADDRESS) (UINT8 Bus, UINT8 Device, UINT8 Function, UINT8 Offset);                                                                                                                                         ///< Get PCI express device address.
typedef VOID       (EFIAPI * SA_GET_RTC_TIME)            (UINT8 *Second, UINT8 *Minute, UINT8 *Hour, UINT8 *Day, UINT8 *Month, UINT16 *Year);                                                                                                             ///< Get the current time value.
typedef UINT64     (EFIAPI * SA_GET_CPU_TIME)            (VOID);                                                                                                                                                                                          ///< The current CPU time in milliseconds.
typedef VOID *     (EFIAPI * SA_MEMORY_COPY)             (VOID *Destination, CONST VOID *Source, UINTN NumBytes);                                                                                                                                         ///< Perform byte copy operation.
typedef VOID *     (EFIAPI * SA_MEMORY_SET_BYTE)         (VOID *Buffer, UINTN NumBytes, UINT8 Value);                                                                                                                                                     ///< Perform byte initialization operation.
typedef VOID *     (EFIAPI * SA_MEMORY_SET_WORD)         (VOID *Buffer, UINTN NumWords, UINT16 Value);                                                                                                                                                    ///< Perform word initialization operation.
typedef VOID *     (EFIAPI * SA_MEMORY_SET_DWORD)        (VOID *Buffer, UINTN NumDwords, UINT32 Value);                                                                                                                                                   ///< Perform dword initialization operation.
typedef UINT64     (EFIAPI * SA_LEFT_SHIFT_64)           (UINT64 Data, UINTN NumBits);                                                                                                                                                                    ///< Left shift the 64-bit data value by specified number of bits.
typedef UINT64     (EFIAPI * SA_RIGHT_SHIFT_64)          (UINT64 Data, UINTN NumBits);                                                                                                                                                                    ///< Right shift the 64-bit data value by specified number of bits.
typedef UINT64     (EFIAPI * SA_MULT_U64_U32)            (UINT64 Multiplicand, UINT32 Multiplier);                                                                                                                                                        ///< Multiply a 64-bit data value by a 32-bit data value.
typedef UINT64     (EFIAPI * SA_DIV_U64_U64)             (UINT64 Dividend, UINT64 Divisor, UINT64 *Remainder);                                                                                                                                            ///< Divide a 64-bit data value by a 64-bit data value.
typedef BOOLEAN    (EFIAPI * SA_GET_SPD_DATA)            (SPD_BOOT_MODE BootMode, UINT8 SpdAddress, UINT8 *Buffer, UINT8 *Ddr3Table, UINT32 Ddr3TableSize, UINT8 *Ddr4Table, UINT32 Ddr4TableSize, UINT8 *LpddrTable, UINT32 LpddrTableSize);             ///< Read the SPD data over the SMBus, at the given SmBus SPD address and copy the data to the data structure.
typedef UINT8      (EFIAPI * SA_GET_MC_ADDRESS_DECODE)   (UINT64 Address, SA_ADDRESS_DECODE *DramAddress);
typedef UINT8      (EFIAPI * SA_GET_MC_ADDRESS_ENCODE)   (SA_ADDRESS_DECODE *DramAddress, UINT64 Address);
typedef BOOLEAN    (EFIAPI * SA_GET_RANDOM_NUMBER)       (UINT32 *Rand);                                                                                                                                                                                  ///< Get the next random 32-bit number.
typedef EFI_STATUS (EFIAPI * SA_CPU_MAILBOX_READ)        (UINT32 Type, UINT32 Command, UINT32 *Value, UINT32 *Status);                                                                                                                                    ///< Perform a CPU mailbox read.
typedef EFI_STATUS (EFIAPI * SA_CPU_MAILBOX_WRITE)       (UINT32 Type, UINT32 Command, UINT32 Value, UINT32 *Status);                                                                                                                                     ///< Perform a CPU mailbox write.
typedef UINT32     (EFIAPI * SA_GET_MEMORY_VDD)          (VOID *GlobalData, UINT32 DefaultVdd);                                                                                                                                                           ///< Get the current memory voltage (VDD).
typedef UINT32     (EFIAPI * SA_SET_MEMORY_VDD)          (VOID *GlobalData, UINT32 DefaultVdd, UINT32 Value);                                                                                                                                             ///< Set the memory voltage (VDD) to the given value.
typedef UINT32     (EFIAPI * SA_CHECKPOINT)              (VOID *GlobalData, UINT32 CheckPoint, VOID *Scratch);                                                                                                                                            ///< Check point that is called at various points in the MRC.
typedef VOID       (EFIAPI * SA_DEBUG_HOOK)              (VOID *GlobalData, UINT16 DisplayDebugNumber);                                                                                                                                                   ///< Typically used to display to the I/O port 80h.
typedef UINT8      (EFIAPI * SA_CHANNEL_EXIST)           (VOID *Outputs, UINT8 Channel);                                                                                                                                                                  ///< Returns whether Channel is or is not present.
typedef INT32      (EFIAPI * SA_PRINTF)                  (VOID *Debug, UINT32 Level, char *Format, ...);                                                                                                                                                  ///< Print to output stream/device.
typedef VOID       (EFIAPI * SA_DEBUG_PRINT)             (VOID *String);                                                                                                                                                                                  ///< Output a string to the debug stream/device.
typedef UINT32     (EFIAPI * SA_CHANGE_MARGIN)           (VOID *GlobalData, UINT8 Param, INT32 Value0, INT32 Value1, UINT8 EnMultiCast, UINT8 Channel, UINT8 RankIn, UINT8 Byte, UINT8 BitIn, UINT8 UpdateMrcData, UINT8 SkipWait, UINT32 RegFileParam);  ///< Change the margin.
typedef UINT8      (EFIAPI * SA_SIGN_EXTEND)             (UINT8 Value, UINT8 OldMsb, UINT8 NewMsb);                                                                                                                                                       ///< Sign extends OldMSB to NewMSB Bits (Eg: Bit 6 to Bit 7).
typedef VOID       (EFIAPI * SA_SHIFT_PI_COMMAND_TRAIN)  (VOID *GlobalData, UINT8 Channel, UINT8 Iteration, UINT8 RankMask, UINT8 GroupMask, INT32 NewValue, UINT8 UpdateHost);                                                                           ///< Move CMD/CTL/CLK/CKE PIs during training.
typedef VOID       (EFIAPI * SA_UPDATE_VREF)             (VOID *GlobalData, UINT8 Channel, UINT8 RankMask, UINT16 DeviceMask, UINT8 VrefType, INT32 Offset, BOOLEAN UpdateMrcData, BOOLEAN PDAmode, BOOLEAN SkipWait);                                    ///< Update the Vref value and wait until it is stable.
typedef UINT8      (EFIAPI * SA_GET_RTC_CMOS)            (UINT8 Location);                                                                                                                                                                                ///< Get the current value of the specified RTC CMOS location.
typedef UINT64     (EFIAPI * SA_MSR_READ_64)             (UINT32 Location);                                                                                                                                                                               ///< Get the current value of the specified MSR location.
typedef UINT64     (EFIAPI * SA_MSR_WRITE_64)            (UINT32 Location, UINT64 Data);                                                                                                                                                                  ///< Set the current value of the specified MSR location.
typedef VOID       (EFIAPI * SA_MRC_RETURN_FROM_SMC)     (VOID *GlobalData, UINT32 MrcStatus);                                                                                                                                                            ///< Hook function after returning from MrcStartMemoryConfiguration()
typedef VOID       (EFIAPI * SA_MRC_DRAM_RESET)          (UINT32 PciEBaseAddress, UINT32 ResetValue);                                                                                                                                                     ///< Assert or deassert DRAM_RESET# pin; this is used in JEDEC Reset.
typedef VOID       (EFIAPI * SA_DELAY_NS)                (VOID *GlobalData, UINT32 DelayNs);                                                                                                                                                              ///< Delay (stall) for the given amount of nanoseconds.
typedef VOID       (EFIAPI * SA_SET_LOCK_PRMRR)          (UINT64 PrmrrBaseAddress, UINT32 PrmrrSize);


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
  SA_DELAY_NS                MrcDelayNs;            ///< Offset 176  - Delay (stall) for the given amount of nanoseconds.
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
 <b>Revision 1</b>: - Initial version.
 <b>Revision 2</b>: - Adding ChHashOverride option.
 <b>Revision 3</b>: - Adding PDA enumeration option.
 <b>Revision 4</b>: - Adding LPDDR4 Command Mirroring.
 <b>Revision 5</b>: - Adding CpuBclkSpread option.
 <b>Revision 6</b>: - Adding McParity option.
 <b>Revision 7</b>: - Adding VddqVoltageOverride option.
 <b>Revision 8</b>: - Adding ExtendedBankHashing option.
 <b>Revision 9</b>: - Adding IbeccErrorInj option
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

  UINT32  EccSupport:1;              ///< Offset 64 Bit 0  - DIMM Ecc Support option - for Desktop only: 0=Disable, <b>1=Enable</b>
  UINT32  MrcSafeConfig:1;           ///<           Bit 1  - MRC Safe Mode: <b>0=Disable</b>, 1=Enable
  UINT32  RemapEnable:1;             ///<           Bit 2  - This option is used to control whether to enable/disable memory remap above 4GB: 0=Disable, <b>1=Enable</b>.
  UINT32  ScramblerSupport:1;        ///<           Bit 3  - Memory scrambler support: 0=Disable, <b>1=Enable</b>
  UINT32  Vc1ReadMeter:1;            ///<           Bit 4  - VC1 Read Metering Enable: 0=Disable, <b>1=Enable</b>
  UINT32  ForceSingleSubchannel:1;   ///<           Bit 5  - TRUE means use SubChannel0 only (for LPDDR4): <b>0=Disable</b>, 1=Enable
  UINT32  SimicsFlag:1;              ///<           Bit 6  - Option to Enable SIMICS: 0=Disable, <b>1=Enable</b>
  UINT32  Ddr4DdpSharedClock:1;      ///<           Bit 7  - Select if CLK0 is shared between Rank0 and Rank1 in DDR4 DDP package. <b>0=Not shared</b>, 1=Shared
  UINT32  SharedZqPin:1;             ///<           Bit 8  - Select if the ZQ resistor is shared between Ranks in DDR4/LPDDR4 DRAM Packages <b>0=Not Shared</b>, 1=Shared
  UINT32  LpDqsOscEn:1;              ///<           Bit 9  - LPDDR Write DQ/DQS Retraining: 0=Disable, <b>1=Enable</b>
  UINT32  RmtPerTask:1;              ///<           Bit 10 - Rank Margin Tool Per Task. <b>0 = Disabled</b>, 1 = Enabled
  UINT32  TrainTrace:1;              ///<           Bit 11 - Trained state tracing debug. <b>0 = Disabled</b>, 1 = Enabled
  UINT32  SafeMode:1;                ///<           Bit 12 - Define if safe mode is enabled for MC/IO
  UINT32  MsHashEnable:1;            ///<           Bit 13 - Controller Hash Enable: 0=Disable, <b>1=Enable</b>
  UINT32  DisPgCloseIdleTimeout:1;   ///<           Bit 14 - Disable Page Close Idle Timeout: 0=Enable, <b>1=Disable</b>
  UINT32  Ibecc:1;                   ///<           Bit 15 - Inband ECC - for LPDDR4, LPDDR5 and DDR4 only: <b>0=Disable</b>, 1=Enable
  UINT32  IbeccParity:1;             ///<           Bit 16 - Inband ECC Parity Control - for LPDDR4, LPDDR5 and DDR4 only: <b>0=Disable</b>, 1=Enable
  UINT32  IbeccOperationMode:2;      ///<           Bits 17:18 - Inband ECC Operation Mode: 0=Functional Mode protects requests based on the address range, <b>1=Makes all requests non protected and ignore range checks</b>, 2=Makes all requests protected and ignore range checks
  UINT32  ChHashOverride:1;          ///<           Bit 19 - Select if Channel Hash setting values will be taken from input parameters or automatically taken from POR values depending on DRAM type detected.
  UINT32  McParity:1;                ///<           Bit 20 - MC Parity Control - Enable Parity for CMI/MC: <b>0=Disable</b>, 1=Enable
  UINT32  IbeccErrorInj:1;           ///<           Bit 21 - In-Band ECC Error Injection: 1=Enable, <b>0=Disable</b>
  UINT32  RsvdO64B22t31:10;          ///<           Bits 22:31 reserved
  /**
   Disables a DIMM slot in the channel even if a DIMM is present\n
   Array index represents the channel number (0 = channel 0, 1 = channel 1)\n
     <b>0x0 = DIMM 0 and DIMM 1 enabled</b>\n
     0x1 = DIMM 0 disabled, DIMM 1 enabled\n
     0x2 = DIMM 0 enabled, DIMM 1 disabled\n
     0x3 = DIMM 0 and DIMM 1 disabled (will disable the whole channel)\n
  **/
  UINT8   DisableDimmChannel[MEM_CFG_MAX_CONTROLLERS][MEM_CFG_MAX_CHANNELS]; ///< Offset 68-75
  UINT8   Ratio;                  ///< Offset 76 DDR Frequency ratio, to multiply by 133 or 100 MHz depending on RefClk. <b>0 = Auto</b>
  UINT8   ProbelessTrace;         ///< Offset 77 Probeless Trace: <b>0=Disabled</b>, <b>1=Enabled</b>
  /**
     - Channel Hash Enable.\n
    NOTE: BIT7 will interleave the channels at a 2 cache-line granularity, BIT8 at 4 and BIT9 at 8\n
    0=BIT6, <B>1=BIT7</B>, 2=BIT8, 3=BIT9
  **/
  UINT8   ChHashInterleaveBit;    ///< Offset 78 Option to select interleave Address bit. Valid values are 0 - 3 for BITS 6 - 9 (Valid values for BDW are 0-7 for BITS 6 - 13)
  UINT8   SmramMask;              ///< Offset 79 Reserved memory ranges for SMRAM
  UINT32  BClkFrequency;          ///< Offset 80 Base reference clock value, in Hertz: <b>100000000 = 100Hz</b>, 125000000=125Hz, 167000000=167Hz, 250000000=250Hz

  /// Training Algorithms 1 Offset 84
  UINT32 ECT:1;                   ///< Bit 0 - Enable/Disable Early Command Training. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 SOT:1;                   ///< Bit 1 - Enable/Disable Sense Amp Offset Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 ERDMPRTC2D:1;            ///< Bit 2 - Enable/Disable Early ReadMPR Timing Centering 2D. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 RDMPRT:1;                ///< Bit 3 - Enable/Disable Read MPR Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 RCVET:1;                 ///< Bit 4 - Enable/Disable Receive Enable Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 JWRL:1;                  ///< Bit 5 - Enable/Disable JEDEC Write Leveling Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 EWRTC2D:1;               ///< Bit 6 - Enable/Disable Early Write Time Centering 2D Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 ERDTC2D:1;               ///< Bit 7 - Enable/Disable Early Read Time Centering 2D Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 WRTC1D:1;                ///< Bit 8 - Enable/Disable 1D Write Timing Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 WRVC1D:1;                ///< Bit 9 - Enable/Disable 1D Write Voltage Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 RDTC1D:1;                ///< Bit 10 - Enable/Disable 1D Read Timing Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 DIMMODTT:1;              ///< Bit 11 - Enable/Disable DIMM ODT Training. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 DIMMRONT:1;              ///< Bit 12 - Enable/Disable DIMM RON training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 WRDSEQT:1;               ///< Bit 13 - Enable/Disable Write Drive Strength / Equalization Training 2D. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 WRSRT:1;                 ///< Bit 14 - Enable/Disable Write Slew Rate traning. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable.</b>
  UINT32 RDODTT:1;                ///< Bit 15 - Enable/Disable Read ODT Training. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 RDEQT:1;                 ///< Bit 16 - Enable/Disable Read Equalization Training. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 RDAPT:1;                 ///< Bit 17 - Enable/Disable Read Amplifier Power Training. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 WRTC2D:1;                ///< Bit 18 - Enable/Disable 2D Write Timing Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 RDTC2D:1;                ///< Bit 19 - Enable/Disable 2D Read Timing Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 WRVC2D:1;                ///< Bit 20 - Enable/Disable 2D Write Voltage Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 RDVC2D:1;                ///< Bit 21 - Enable/Disable 2D Read Voltage Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 CMDVC:1;                 ///< Bit 22 - Enable/Disable Command Vref Centering Training. Note it is not recommended to change this setting from the default value 0=Disable, <b>1=Enable</b>.
  UINT32 LCT:1;                   ///< Bit 23 - Enable/Disable Late Command Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 RTL:1;                   ///< Bit 24 - Enable/Disable Round Trip Latency function. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 TAT:1;                   ///< Bit 25 - Enable/Disable Turn Around Time function. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 RMT:1;                   ///< Bit 26 - Enable/Disable Rank Margin Tool function: <b>0=Disable</b>, 1=Enable.
  UINT32 MEMTST:1;                ///< Bit 27 - Enable/Disable Memory Test function: <b>0=Disable</b>, 1=Enable.
  UINT32 ALIASCHK:1;              ///< Bit 28 - Enable/Disable DIMM SPD Alias Check: 0=Disable, <b>1=Enable</b>
  UINT32 RCVENC1D:1;              ///< Bit 29 - Enable/Disable Receive Enable Centering Training (LPDDR Only). Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable
  UINT32 RMC:1;                   ///< Bit 30 - Enable/Disable Retrain Margin Check.  Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable
  UINT32 WRDSUDT:1;               ///< Bit 31 - Enable/Disable Write Drive Strength Up/Dn independently. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable
  /// Training Algorithms 2 Offset 88
  UINT32 DCC       : 1;           ///< Bit 0  - Enable/Disable Duty Cycle Correction: 0=Disable, 1=Enable.
  UINT32 RDVC1D    : 1;           ///< Bit 1  - Enable/Disable Read Voltage Centering 1D: 0=Disable, 1=Enable.
  UINT32 TXTCO     : 1;           ///< Bit 2  - Enable/Disable Write TCO Comp Training: 0=Disable, 1=Enable.
  UINT32 CLKTCO    : 1;           ///< Bit 3  - Enable/Disable Clock TCO Comp Training: 0=Disable, 1=Enable.
  UINT32 CMDSR     : 1;           ///< Bit 4  - Enable/Disable CMD Slew Rate Training: 0=Disable, 1=Enable.
  UINT32 CMDDSEQ   : 1;           ///< Bit 5  - Enable/Disable CMD Drive Strength and Tx Equalization: 0=Disable, 1=Enable.
  UINT32 DIMMODTCA : 1;           ///< Bit 6  - Enable/Disable Dimm ODT CA Training: 0=Disable, 1=Enable.
  UINT32 TXTCODQS  : 1;           ///< Bit 7  - Enable/Disable Write TCO Dqs Training: 0=Disable, 1=Enable.
  UINT32 CMDDRUD   : 1;           ///< Bit 8  - Enable/Disable CMD/CTL Drive Strength Up/Dn 2D: 0=Disable, 1=Enable.
  UINT32 VCCDLLBP  : 1;           ///< Bit 9  - Enable/Disable VccDLL bypass to VccIOG training: 0=Disable, 1=Enable.
  UINT32 PVTTDNLP  : 1;           ///< Bit 10 - Enable/Disable PanicVttDnLp Training: 0=Disable, 1=Enable.
  UINT32 RDVREFDC  : 1;           ///< Bit 11 - Enable/Disable Read Vref Decap Training: 0=Disable, 1=Enable.
  UINT32 VDDQT     : 1;           ///< Bit 12 - Enable/Disable Vddq Training: 0=Disable, 1=Enable.
  UINT32 RMTBIT    : 1;           ///< Bit 13 - Enable/Disable Rank Margin Tool Per Bit: 0=Disable, 1=Enable.
  UINT32 PDA       : 1;           ///< BIT 14 - Enable/Disable PDA Enumeration Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 WRITE0    : 1;           ///< BIT 15 - Write0 feature enablement
  UINT32 ReservedBits2 :16;       ///< Bits 16:31 - Reserved

  UINT32  MrcTimeMeasure:1;         ///< Offset 92 Bit 0  - Enables serial debug level to display the MRC execution times only: <b>0=Disable</b>, 1=Enable
  UINT32  MrcFastBoot:1;            ///<           Bit 1  - Enables the MRC fast boot path for faster cold boot execution: 0=Disable, <b>1=Enable</b>
  UINT32  DqPinsInterleaved:1;      ///<           Bit 2  - Interleaving mode of DQ/DQS pins which depends on board routing: <b>0=Disable</b>, 1=Enable
  UINT32  RankInterleave:1;         ///<           Bit 3  - Rank Interleave Mode: 0=Disable, <b>1=Enable</b>
  UINT32  EnhancedInterleave:1;     ///<           Bit 4  - Enhanced Interleave Mode: 0=Disable, <b>1=Enable</b>
  UINT32  WeaklockEn:1;             ///<           Bit 5  - Weak Lock Enable: 0=Disable, <b>1=Enable</b>
  UINT32  ChHashEnable:1;           ///<           Bit 6  - Channel Hash Enable: 0=Disable, <b>1=Enable</b>
  UINT32  EnablePwrDn:1;            ///<           Bit 7  - Enable Power Down control for DDR: 0=PCODE control, <b>1=BIOS control</b>
  UINT32  EnablePwrDnLpddr:1;       ///<           Bit 8  - Enable Power Down for LPDDR: 0=PCODE control, <b>1=BIOS control</b>
  UINT32  SrefCfgEna:1;             ///<           Bit 9  - Enable Self Refresh: 0=Disable, <b>1=Enable</b>
  UINT32  ThrtCkeMinDefeatLpddr:1;  ///<           Bit 10 - Throttler CKE min defeature for LPDDR: 0=Disable, <b>1=Enable</b>
  UINT32  ThrtCkeMinDefeat:1;       ///<           Bit 11 - Throttler CKE min defeature: <b>0=Disable</b>, 1=Enable
  UINT32  AutoSelfRefreshSupport:1; ///<           Bit 12 - FALSE = No auto self refresh support, <b>TRUE = auto self refresh support</b>
  UINT32  ExtTemperatureSupport:1;  ///<           Bit 13 - FALSE = No extended temperature support, <b>TRUE = extended temperature support</b>
  UINT32  MobilePlatform:1;         ///<           Bit 14 - Memory controller device id indicates: <b>TRUE if mobile</b>, FALSE if not. Note: This will be auto-detected and updated.
  UINT32  Force1Dpc:1;              ///<           Bit 15 - TRUE means force one DIMM per channel, <b>FALSE means no limit</b>
  UINT32  ForceSingleRank:1;        ///<           Bit 16 - TRUE means use Rank0 only (in each DIMM): <b>0=Disable</b>, 1=Enable
  UINT32  VttTermination:1;         ///<           Bit 17 - Vtt Termination for Data ODT: <b>0=Disable</b>, 1=Enable
  UINT32  VttCompForVsshi:1;        ///<           Bit 18 - Enable/Disable Vtt Comparator For Vsshi: <b>0=Disable</b>, 1=Enable
  UINT32  ExitOnFailure:1;          ///<           Bit 19 - MRC option for exit on failure or continue on failure: 0=Disable, <b>1=Enable</b>
  UINT32  NewFeatureEnable1:1;      ///<           Bit 20 - Generic enable knob for new feature set 1 <b>0: Disable </b>; 1: Enable
  UINT32  NewFeatureEnable2:1;      ///<           Bit 21 - Generic enable knob for new feature set 2 <b>0: Disable </b>; 1: Enable
  UINT32  RhPrevention:1;           ///<           Bit 22 - RH Prevention Enable/Disable: 0=Disable, <b>1=Enable</b>
  UINT32  RhSolution:1;             ///<           Bit 23 - Type of solution to be used for RHP - 0/1 = HardwareRhp/Refresh2x
  UINT32  RefreshPanicWm:4;         ///<           Bit 24-27 - Refresh Panic Watermark, Range 1-8, default 8.
  UINT32  RefreshHpWm:4;            ///<           Bit 28-31 - Refresh High Profile Watermark, Range 1-7, default 7.
  UINT32  VddSettleWaitTime;      ///< Offset 96 Amount of time in microseconds to wait for Vdd to settle on top of 200us required by JEDEC spec: <b>Default=0</b>
  UINT16  SrefCfgIdleTmr;         ///< Offset 100 Self Refresh idle timer: <b>512=Minimal</b>, 65535=Maximum
  UINT16  ChHashMask;             ///< Offset 102 Channel Hash Mask: 0x0001=BIT6 set(Minimal), 0x3FFF=BIT[19:6] set(Maximum), <b>0x30CE= BIT[19:18, 13:12 ,9:7] set</b>
  UINT16  DdrFreqLimit;           ///< Offset 104 Memory Frequency setting: 3=1067, 5=1333, 7=1600, 9=1867, 11=2133, 13=2400, <b>15=2667</b>
  UINT8   MaxRttWr;               ///< Offset 106 Maximum DIMM RTT_WR to use in power training: <b>0=ODT Off</b>, 1 = 120 ohms
  UINT8   ThrtCkeMinTmr;          ///< Offset 107 Throttler CKE min timer: 0=Minimal, 0xFF=Maximum, <b>0x00=Default</b>
  UINT8   ThrtCkeMinTmrLpddr;     ///< Offset 108 Throttler CKE min timer for LPDDR: 0=Minimal, 0xFF=Maximum, <b>0x00=Default</b>
  BOOLEAN PerBankRefresh;         ///< Offset 109 Enables and Disables the per bank refresh.  This only impacts memory technologies that support PBR: LPDDR3, LPDDR4.  FALSE=Disabled, <b>TRUE=Enabled</b>
  UINT8   SaGv;                   ///< Offset 110 SA GV: <b>0=Disabled</b>, 1=Point1, 2=Point2, 3=Point3, 4=Point4, 5=Enabled
  UINT8   NModeSupport;           ///< Offset 111 Memory N Mode Support - Enable user to select Auto, 1N or 2N: <b>0=AUTO</b>, 1=1N, 2=2N.
  UINT8   RefClk;                 ///< Offset 112 Selects the DDR base reference clock. 0x01 = 100MHz, <b>0x00 = 133MHz</b>
  UINT8   EnCmdRate;              ///< Offset 113 CMD Rate Enable: 0=Disable, 5=2 CMDs, <b>7=3 CMDs</b>, 9=4 CMDs, 11=5 CMDs, 13=6 CMDs, 15=7 CMDs
  UINT8   Refresh2X;              ///< Offset 114 Refresh 2x: <b>0=Disable</b>, 1=Enable for WARM or HOT, 2=Enable for HOT only
  UINT8   EpgEnable;              ///< Offset 115 Enable Energy Performance Gain.
  UINT8   UserThresholdEnable;    ///< Offset 116 Flag to manually select the DIMM CLTM Thermal Threshold, 0=Disable,  1=Enable, <b>0=Default</b>
  UINT8   UserBudgetEnable;       ///< Offset 117 Flag to manually select the Budget Registers for CLTM Memory Dimms , 0=Disable,  1=Enable, <b>0=Default</b>
  UINT8   RetrainOnFastFail;      ///< Offset 118 Restart MRC in Cold mode if SW MemTest fails during Fast flow. 0 = Disabled, <b>1 = Enabled</b>
  UINT8   PowerDownMode;          ///< Offset 119 CKE Power Down Mode: <b>0xFF=AUTO</b>, 0=No Power Down, 1= APD mode, 6=PPD-DLL Off mode
  UINT8   PwdwnIdleCounter;       ///< Offset 120 CKE Power Down Mode Idle Counter: 0=Minimal, 255=Maximum, <b>0x80=0x80 DCLK</b>
  UINT8   CmdRanksTerminated;     ///< Offset 121 LPDDR: Bitmask of ranks that have CA bus terminated. <b>0x01=Default, Rank0 is terminating and Rank1 is non-terminating</b>
  UINT16  MsHashMask;             ///< Offset 122 Controller Hash Mask: 0x0001=BIT6 set(Minimal), 0x3FFF=BIT[19:6] set(Maximum), <b>0x30CE= BIT[19:18, 13:12 ,9:7] set</b>
  UINT32  Lp5CccConfig;           ///< Offset 124 BitMask where bits [3:0] are controller 0 Channel [3:0] and [7:4] are Controller 1 Channel [3:0].  0 selects Ascending mapping and 1 selects Descending mapping.
  UINT8   RMTLoopCount;           ///< Offset 128 Indicates the Loop Count to be used for Rank Margin Tool Testing: 1=Minimal, 32=Maximum, 0=AUTO, <b>0=Default</b>
  UINT8   MsHashInterleaveBit;    ///< Offset 129 Option to select interleave Address bit. Valid values are 0 - 3 for BITS 6 - 9
  UINT8   GearRatio;              ///< Offset 130 This input control's the current gear expressed as an integer when SAGV is disabled: <b>0=Default</b>, 1, 2.
  UINT8   Ddr4OneDpc;             ///< Offset 131 DDR4 1DPC performance feature: 0 - Disabled; 1 - Enabled on DIMM0 only, 2 - Enabled on DIMM1 only; 3 - Enabled on both DIMMs. (bit [0] - DIMM0, bit [1] - DIMM1)
  UINT32  BclkRfiFreq[MEM_MAX_SAGV_POINTS]; ///< Offset 132 Bclk RFI Frequency for each SAGV point in Hz units. 98000000Hz = 98MHz <b>0 - No RFI Tuning</b>. Range is 98Mhz-100Mhz.
  UINT16  SaGvFreq[MEM_MAX_SAGV_POINTS];    ///< Offset 148 Frequency per SAGV point.  0 is Auto, otherwise holds the frequency value expressed as an integer: <b>0=Default</b>, 1067, 1333, 1600, 1800, 1867, etc.
  /**
    Offset 156 Gear ratio per SAGV point.  0 is Auto, otherwise holds the Gear ratio expressed as an integer: <b>0=Default</b>, 1, 2.
      Only valid combinations of Gear Ratio per point is:
      | point | set1 | set2 | set3
      | 0     | 1    | 2    | 2
      | 1     | 1    | 2    | 2
      | 2     | 1    | 2    | 2
      | 3     | 1    | 2    | 1
  **/
  UINT8   SaGvGear[MEM_MAX_SAGV_POINTS];                      ///< Offset 156
  UINT8   IbeccProtectedRegionEnable[MEM_MAX_IBECC_REGIONS];  ///< Offset 160 Enable use of address range for ECC Protection:  <b>0=Default</b>, 1
  UINT16  IbeccProtectedRegionBase[MEM_MAX_IBECC_REGIONS];    ///< Offset 168 Base address for address range of ECC Protection:  <b>0=Default</b>, 1
  UINT16  IbeccProtectedRegionMask[MEM_MAX_IBECC_REGIONS];    ///< Offset 184 Mask address for address range of ECC Protection:  <b>0=Default</b>, 1
  UINT32  CmdMirror;              ///< Offset 200 BitMask where bits [3:0] are controller 0 Channel [3:0] and [7:4] are Controller 1 Channel [3:0].  0 = No Command Mirror and 1 = Command Mirror.
  UINT8   CpuBclkSpread;          ///< Offset 204 CPU BCLK Spread Specturm: 0 = Disabled; <b>1 = Enabled</b>
  UINT8   ExtendedBankHashing;    ///< Offset 205 Enable EBH Extended Bank Hashing: 0=Disabled; <b>1 = Enabled</b>.
  UINT16  VddqVoltageOverride;    ///< Offset 206 VccddqVoltage override in # of 1mV
  UINT8   MarginLimitCheck;                                   ///< Offset 208 Margin limit check enable: <b>0=Disable</b>, 1=L1 only, 2=L2 only, 3=Both L1 and L2
  UINT8   RsvdO209;                                           ///< Offset 209
  UINT16  MarginLimitL2;                                      ///< Offset 210 Margin limit check L2 threshold: <b>100=Default</b>
} MEMORY_CONFIGURATION;

/// Memory Configuration
/// The contents of this structure are not CRC'd by the MRC for option change detection.
/// <b>Revision 1</b>:  - Initial version.
/// <b>Revision 2</b>:  - Added MemTestOnWarmBoot
typedef struct {
  CONFIG_BLOCK_HEADER      Header;              ///< Offset 0-23 Config Block Header
  SA_FUNCTION_CALLS        SaCall;              ///< Offset 24   Function calls into the SA.
  SA_MEMORY_FUNCTIONS      MrcCall;             ///< Offset 204  Function calls into the MRC.
  SPD_DATA_BUFFER          *SpdData;            ///< Offset 240  Memory SPD data, will be used by the MRC when SPD SmBus address is zero.
  UINT32                   Reserved0;
  SA_MEMORY_DQDQS_MAPPING  *DqDqsMap;           ///< Offset 244  LPDDR DQ bit and DQS byte swizzling between CPU and DRAM.
  SA_MEMORY_RCOMP          *RcompData;          ///< Offset 248  DDR RCOMP resistors and target values.
  UINT64                   PlatformMemorySize;  ///< Offset 252  The minimum platform memory size required to pass control into DXE
  UINT32                   CleanMemory:1;       ///< Offset 256  Ask MRC to clear memory content: <b>FALSE=Do not Clear Memory</b>; TRUE=Clear Memory
  UINT32                   ReservedBits5:31;
  /**
   Sets the serial debug message level\n
     0x00 = Disabled\n
     0x01 = Errors only\n
     0x02 = Errors and Warnings\n
     <b>0x03 = Errors, Warnings, and Info</b>\n
     0x04 = Errors, Warnings, Info, and Events\n
     0x05 = Displays Memory Init Execution Time Summary only\n
  **/
  UINT8   SerialDebugLevel;                     ///< Offset 260
  UINT8   MemTestOnWarmBoot;                    ///< Offset 261  Run Base Memory Test On WarmBoot:  0=Disabled, <b>1=Enabled</b>
  UINT8   Reserved11[2];                        ///< Offset 262 - 263  Reserved
} MEMORY_CONFIG_NO_CRC;
#pragma pack(pop)

#endif // _MEMORY_CONFIG_H_
