/** @file
  Policy definition of Memory Config Block

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
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
  Default,        ///< 0, Default SPD
  UserDefined,    ///< 1, User Defined profile
  XMPProfile1,    ///< 2, XMP Profile 1
  XMPProfile2     ///< 3, XMP Profile 2
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
  UINT8  Rsvd[3];         ///< Offset 5 Reserved for future use
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
typedef VOID       (EFIAPI * SA_UPDATE_VREF_WAIT_STABLE) (VOID *GlobalData, UINT8 VrefType, UINT8 UpdateMrcData, INT32 Offset, UINT8 RankMask, UINT8 DeviceMask, BOOLEAN PDAmode, UINT8 SkipWait);
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
  SA_GET_MC_ADDRESS_DECODE   GetMcAddressDecode;    ///< Offset 0:  - Converts system address to DRAM address.
  SA_GET_MC_ADDRESS_ENCODE   GetMcAddressEncode;    ///< Offset 4:  - Converts DRAM address to system address.
  SA_CHANNEL_EXIST           MrcChannelExist;       ///< Offset 8:  - Returns whether Channel is or is not present.
  SA_PRINTF                  MrcPrintf;             ///< Offset 12: - Print to output stream/device.
  SA_CHANGE_MARGIN           MrcChangeMargin;       ///< Offset 16: - Change the margin.
  SA_SIGN_EXTEND             MrcSignExtend;         ///< Offset 20: - Sign extends OldMSB to NewMSB Bits (Eg: Bit 6 to Bit 7).
  SA_SHIFT_PI_COMMAND_TRAIN  ShiftPiCommandTrain;   ///< Offset 24: - Move CMD/CTL/CLK/CKE PIs during training.
  SA_UPDATE_VREF_WAIT_STABLE UpdateVrefWaitStable;  ///< Offset 28: - Update the Vref value and wait until it is stable.
  SA_THERMAL_OVERRIDES       MrcThermalOverrides;   ///< Offset 32: - Update memory thermal conditions.
} SA_MEMORY_FUNCTIONS;

/**
 Memory Configuration
 The contents of this structure are CRC'd by the MRC for option change detection.
 This structure is copied en mass to the MrcInput structure.
 If you add fields here, you must update the MrcInput structure.\n
  <b>Revision 1</b>:
  - Initial version.

  <b>Revision 2</b>:
  - Added CMDSR, CMDDSEQ and CMDNORM.
  - Added Ddr4DdpSharedClock and Ddr4DdpSharedZq.

  <b>Revision 3</b>:
  - Added EWRDSEQ.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;    ///< Offset 0-27 Config Block Header
  UINT8   HobBufferSize;          ///< Offset 28 Size of HOB buffer for MRC
  //
  // The following parameters are used only when SpdProfileSelected is UserDefined (CUSTOM PROFILE)
  //
  UINT8   SpdProfileSelected;     ///< Offset 29 SPD XMP profile selection - for XMP supported DIMM: <b>0=Default DIMM profile</b>, 1=Customized profile, 2=XMP profile 1, 3=XMP profile 2.
  UINT16  tCL;                    ///< Offset 30 User defined Memory Timing tCL value,   valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 31=Maximum.
  UINT16  tRCDtRP;                ///< Offset 32 User defined Memory Timing tRCD value (same as tRP), valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 63=Maximum.
  UINT16  tRAS;                   ///< Offset 34 User defined Memory Timing tRAS value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 64=Maximum.
  UINT16  tWR;                    ///< Offset 36 User defined Memory Timing tWR value,   valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, legal values: 5, 6, 7, 8, 10, 12, 14, 16, 18, 20, 24.
  UINT16  tRFC;                   ///< Offset 38 User defined Memory Timing tRFC value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 1023=Maximum.
  UINT16  tRRD;                   ///< Offset 40 User defined Memory Timing tRRD value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 15=Maximum.
  UINT16  tWTR;                   ///< Offset 42 User defined Memory Timing tWTR value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 28=Maximum.
  UINT16  tRTP;                   ///< Offset 44 User defined Memory Timing tRTP value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 15=Maximum. DDR4 legal values: 5, 6, 7, 8, 9, 10, 12
  UINT16  tFAW;                   ///< Offset 46 User defined Memory Timing tFAW value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 63=Maximum.
  UINT16  tCWL;                   ///< Offset 48 User defined Memory Timing tCWL value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 20=Maximum.
  UINT16  tREFI;                  ///< Offset 50 User defined Memory Timing tREFI value, valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 65535=Maximum.
  UINT16  VddVoltage;             ///< Offset 52 DRAM voltage (Vdd) in millivolts: <b>0=Platform Default (no override)</b>, 1200=1.2V, 1350=1.35V etc.
  UINT8   NModeSupport;           ///< Offset 54 Memory N Mode Support - Enable user to select Auto, 1N or 2N: <b>0=AUTO</b>, 1=1N, 2=2N.

  UINT8   McLock;                 ///< Offset 55 Enable/Disable memory configuration register locking: 0=Disable, <b>1=Enable</b>.
  //
  // Thermal Management
  //
  UINT32  ThermalManagement:1;        ///< Offset 56 Memory Thermal Management Support: <b>0=Disable</b>, 1=Enable.
  UINT32  PeciInjectedTemp:1;         ///<  - Enable/Disable memory temperatures to be injected to the processor via PECI: <b>0=Disable</b>, 1=Enable.
  UINT32  ExttsViaTsOnBoard:1;        ///<  - Enable/Disable routing TS-on-Board's ALERT# and THERM# to EXTTS# pins on the PCH: <b>0=Disable</b>, 1=Enable.
  UINT32  ExttsViaTsOnDimm:1;         ///<  - Enable/Disable routing TS-on-DIMM's ALERT# to EXTTS# pin on the PCH: <b>0=Disable</b>, 1=Enable.
  UINT32  VirtualTempSensor:1;        ///<  - Enable/Disable Virtual Temperature Sensor (VTS): <b>0=Disable</b>, 1=Enable.
  UINT32  RsvdBits0:27;
  //
  // Training Algorithms
  //
  UINT32 ECT:1;                       ///< Offset 60 Enable/Disable Early Command Training. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 SOT:1;                       ///<  - Enable/Disable Sense Amp Offset Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 ERDMPRTC2D:1;                ///<  - Enable/Disable Early ReadMPR Timing Centering 2D. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 RDMPRT:1;                    ///<  - Enable/Disable Read MPR Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 RCVET:1;                     ///<  - Enable/Disable Receive Enable Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 JWRL:1;                      ///<  - Enable/Disable JEDEC Write Leveling Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 EWRTC2D:1;                   ///<  - Enable/Disable Early Write Time Centering 2D Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 ERDTC2D:1;                   ///<  - Enable/Disable Early Read Time Centering 2D Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 WRTC1D:1;                    ///<  - Enable/Disable 1D Write Timing Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 WRVC1D:1;                    ///<  - Enable/Disable 1D Write Voltage Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 RDTC1D:1;                    ///<  - Enable/Disable 1D Read Timing Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 DIMMODTT:1;                  ///<  - Enable/Disable DIMM ODT Training. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 DIMMRONT:1;                  ///<  - Enable/Disable DIMM RON training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 WRDSEQT:1;                   ///<  - Enable/Disable Write Drive Strength / Equalization Training 2D. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 WRSRT:1;                     ///<  - Enable/Disable Write Slew Rate traning. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable.</b>
  UINT32 RDODTT:1;                    ///<  - Enable/Disable Read ODT Training. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 RDEQT:1;                     ///<  - Enable/Disable Read Equalization Training. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 RDAPT:1;                     ///<  - Enable/Disable Read Amplifier Power Training. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 WRTC2D:1;                    ///<  - Enable/Disable 2D Write Timing Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 RDTC2D:1;                    ///<  - Enable/Disable 2D Read Timing Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 WRVC2D:1;                    ///<  - Enable/Disable 2D Write Voltage Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 RDVC2D:1;                    ///<  - Enable/Disable 2D Read Voltage Centering Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 CMDVC:1;                     ///<  - Enable/Disable Command Vref Centering Training. Note it is not recommended to change this setting from the default value 0=Disable, <b>1=Enable</b>.
  UINT32 LCT:1;                       ///<  - Enable/Disable Late Command Training. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 RTL:1;                       ///<  - Enable/Disable Round Trip Latency function. Note it is not recommended to change this setting from the default value: 0=Disable, <b>1=Enable</b>.
  UINT32 TAT:1;                       ///<  - Enable/Disable Turn Around Time function. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable.
  UINT32 RMT:1;                       ///<  - Enable/Disable Rank Margin Tool function: <b>0=Disable</b>, 1=Enable.
  UINT32 MEMTST:1;                    ///<  - Enable/Disable Memory Test function: <b>0=Disable</b>, 1=Enable.
  UINT32 ALIASCHK:1;                  ///<  - Enable/Disable DIMM SPD Alias Check: 0=Disable, <b>1=Enable</b>
  UINT32 RCVENC1D:1;                  ///<  - Enable/Disable Receive Enable Centering Training (LPDDR Only). Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable
  UINT32 RMC:1;                       ///<  - Enable/Disable Retrain Margin Check.  Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable
  UINT32 WRDSUDT:1;                   ///<  - Enable/Disable Write Drive Strength Up/Dn independently. Note it is not recommended to change this setting from the default value: <b>0=Disable</b>, 1=Enable

  UINT32  MrcSafeConfig:1;            ///< Offset 64 MRC Safe Mode: <b>0=Disable</b>, 1=Enable
  UINT32  EccSupport:1;               ///<  - DIMM Ecc Support option - for Desktop only: 0=Disable, <b>1=Enable</b>
  UINT32  RemapEnable:1;              ///<  - This option is used to control whether to enable/disable memory remap above 4GB: 0=Disable, <b>1=Enable</b>.
  UINT32  ScramblerSupport:1;         ///<  - Memory scrambler support: 0=Disable, <b>1=Enable</b>
  UINT32  OddRatioMode:1;             ///<  - If Odd Ratio Mode is enabled, QCLK frequency has an addition of 133/100 MHz: <b>0=Disable</b>, 1=Enable
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

  UINT32  RhPrevention:1;             ///< Offset 68 RH Prevention Enable/Disable: 0=Disable, <b>1=Enable</b>
  UINT32  VttTermination:1;           ///<  - Vtt Termination for Data ODT: <b>0=Disable</b>, 1=Enable
  UINT32  VttCompForVsshi:1;          ///<  - Enable/Disable Vtt Comparator For Vsshi: <b>0=Disable</b>, 1=Enable
  UINT32  ExitOnFailure:1;            ///<  - MRC option for exit on failure or continue on failure: 0=Disable, <b>1=Enable</b>
  UINT32  Vc1ReadMeter:1;             ///<  - VC1 Read Metering Enable: 0=Disable, <b>1=Enable</b>
  UINT32  DdrThermalSensor:1;         ///<  - Ddr Thermal Sensor: 0=Disable, <b>1=Enable</b>
  UINT32  LpddrMemWriteLatencySet:1;  ///<  - LPDDR3 Write Latency Set option: 0=Set A, <b>1=Set B</b>
  UINT32  EvLoader:1;                 ///<  - Option to Enable EV Loader: <b>0=Disable</b>,1=Enable
  UINT32  EvLoaderDelay:1;            ///<  - Option to Enable EV Loader Delay: 0=Disable, <b>1=Enable</b>
  UINT32  Ddr4DdpSharedClock:1;       ///<  - Select if CLK0 is shared between Rank0 and Rank1 in DDR4 DDP package. <b>0=Not shared</b>, 1=Shared
  UINT32  Ddr4DdpSharedZq:1;          ///<  - Select if ZQ pin is shared between Rank0 and Rank1 in DDR4 DDP package. <b>0=Not shared</b>, 1=Shared
  UINT32  RsvdBits1:21;

  UINT32  BClkFrequency;              ///< Offset 72  - Base reference clock value, in Hertz: <b>100000000 = 100Hz</b>, 125000000=125Hz, 167000000=167Hz, 250000000=250Hz
  UINT16  DdrFreqLimit;               ///< Offset 76 Memory Frequency Limit: <b>0=Auto (limited by SPD/CPU capability)</b>, for valid values see MrcFrequency in MrcInterface.h
  /**
   Selects the DDR base reference clock\n
     <b>0x00 = 133MHz</b>
     0x01 = 100MHz
  **/
  UINT8   RefClk;                 ///< Offset 78
  /**
   Selects the ratio to multiply the reference clock by for the DDR frequency\n
   When RefClk is 133MHz\n
   <b>0x00 = Auto</b>, 0x03 through 0x0C are valid values, all others are invalid\n
   When RefClk is 100MHz\n
   <b>0x00 = Auto</b>, 0x06 through 0x10 are valid values, all others are invalid\n
  **/
  UINT8   Ratio;                  ///< Offset 79
  UINT8   ProbelessTrace;         ///< Offset 80 Probeless Trace: <b>0=Disabled</b>, 1=GDXC IOT/MOT, 2=HD Port. <b>Note: Enabling ProbelessTrace will also need enabling IED together.</b>
  UINT8   GdxcIotSize;            ///< Offset 81 GDXC IOT Size in 8MB granularity: 0=Minimal, 128=Maximum, <b>4=32MB</b>.
  UINT8   GdxcMotSize;            ///< Offset 82 GDXC MOT Size in 8MB granularity: 0=Minimal, 128=Maximum, <b>12=96MB</b>.
  /**
     - Channel Hash Enable.\n
    NOTE: BIT7 will interlave the channels at a 2 cacheline granularity, BIT8 at 4 and BIT9 at 8\n
    0=BIT6, <B>1=BIT7</B>, 2=BIT8, 3=BIT9
  **/
  UINT8   ChHashInterleaveBit;    ///< Offset 83
  UINT16  ChHashMask;             ///< Offset 84  - Channel Hash Mask: 0x0001=BIT6 set(Minimal), 0x3FFF=BIT[19:6] set(Maximum), <b>0x30CE= BIT[19:18, 13:12 ,9:7] set</b>
  /**
   Disables a DIMM slot in the channel even if a DIMM is present\n
   Array index represents the channel number (0 = channel 0, 1 = channel 1)\n
     <b>0x0 = DIMM 0 and DIMM 1 enabled</b>\n
     0x1 = DIMM 0 disabled, DIMM 1 enabled\n
     0x2 = DIMM 0 enabled, DIMM 1 disabled\n
     0x3 = DIMM 0 and DIMM 1 disabled (will disable the whole channel)\n
  **/
  UINT8   DisableDimmChannel[SA_MC_MAX_CHANNELS];///< Offset 86

  UINT8   RaplLim2WindX;          ///< Offset 88  - Power Limit 2 Time Window X value: 0=Minimal, 3=Maximum, <b>1=Default</b>
  UINT8   RaplLim2WindY;          ///< Offset 89  - Power Limit 2 Time Window Y value: 0=Minimal, 3=Maximum, <b>1=Default</b>
  UINT8   RaplLim1WindX;          ///< Offset 90  - Power Limit 1 Time Window X value: <b>0=Minimal</b>, 3=Maximum
  UINT8   RaplLim1WindY;          ///< Offset 91  - Power Limit 1 Time Window Y value: <b>0=Minimal</b>, 31=Maximum
  UINT16  RaplLim2Pwr;            ///< Offset 92  - Power Limit 2: 0=Minimal, 16383=Maximum, <b>222=Default</b>
  UINT16  RaplLim1Pwr;            ///< Offset 94  - Power Limit 1: <b>0=Minimal</b>, 16383=Maximum
  UINT8   WarmThresholdCh0Dimm0;  ///< Offset 96  - Warm Threshold (Channel 0, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8   WarmThresholdCh0Dimm1;  ///< Offset 97  - Warm Threshold (Channel 0, Dimm 1): 0=Minimal, <b>255=Maximum</b>
  UINT8   WarmThresholdCh1Dimm0;  ///< Offset 98  - Warm Threshold (Channel 1, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8   WarmThresholdCh1Dimm1;  ///< Offset 99  - Warm Threshold (Channel 1, Dimm 1): 0=Minimal, <b>255=Maximum</b>
  UINT8   HotThresholdCh0Dimm0;   ///< Offset 100  - Hot Threshold (Channel 0, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8   HotThresholdCh0Dimm1;   ///< Offset 101  - Hot Threshold (Channel 0, Dimm 1): 0=Minimal, <b>255=Maximum</b>
  UINT8   HotThresholdCh1Dimm0;   ///< Offset 102  - Hot Threshold (Channel 1, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8   HotThresholdCh1Dimm1;   ///< Offset 103  - Hot Threshold (Channel 1, Dimm 1): 0=Minimal, <b>255=Maximum</b>
  UINT8   WarmBudgetCh0Dimm0;     ///< Offset 104  - Warm Budget (Channel 0, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8   WarmBudgetCh0Dimm1;     ///< Offset 105  - Warm Budget (Channel 0, Dimm 1): 0=Minimal, <b>255=Maximum</b>
  UINT8   WarmBudgetCh1Dimm0;     ///< Offset 106  - Warm Budget (Channel 1, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8   WarmBudgetCh1Dimm1;     ///< Offset 107  - Warm Budget (Channel 1, Dimm 1): 0=Minimal, <b>255=Maximum</b>
  UINT8   HotBudgetCh0Dimm0;      ///< Offset 108  - Hot Budget (Channel 0, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8   HotBudgetCh0Dimm1;      ///< Offset 109  - Hot Budget (Channel 0, Dimm 1): 0=Minimal, <b>255=Maximum</b>
  UINT8   HotBudgetCh1Dimm0;      ///< Offset 110  - Hot Budget (Channel 1, Dimm 0): 0=Minimal, <b>255=Maximum</b>
  UINT8   HotBudgetCh1Dimm1;      ///< Offset 111 - Hot Budget (Channel 1, Dimm 1): 0=Minimal, <b>255=Maximum</b>
  UINT8   IdleEnergyCh0Dimm0;     ///< Offset 113 - Idle Energy (Channel 0, Dimm 0): 0=Minimal, 63=Maximum, <b>10=Default</b>
  UINT8   IdleEnergyCh0Dimm1;     ///< Offset 112 - Idle Energy (Channel 0, Dimm 1): 0=Minimal, 63=Maximum, <b>10=Default</b>
  UINT8   IdleEnergyCh1Dimm0;     ///< Offset 115  - Idle Energy (Channel 1, Dimm 0): 0=Minimal, 63=Maximum, <b>10=Default</b>
  UINT8   IdleEnergyCh1Dimm1;     ///< Offset 114  - Idle Energy (Channel 1, Dimm 1): 0=Minimal, 63=Maximum, <b>10=Default</b>
  UINT8   PdEnergyCh0Dimm0;       ///< Offset 117  - Power Down Energy (Channel 0, Dimm 0): 0=Minimal, 63=Maximum, <b>6=Default</b>
  UINT8   PdEnergyCh0Dimm1;       ///< Offset 116  - Power Down Energy (Channel 0, Dimm 1): 0=Minimal, 63=Maximum, <b>6=Default</b>
  UINT8   PdEnergyCh1Dimm0;       ///< Offset 119  - Power Down Energy (Channel 1, Dimm 0): 0=Minimal, 63=Maximum, <b>6=Default</b>
  UINT8   PdEnergyCh1Dimm1;       ///< Offset 118  - Power Down Energy (Channel 1, Dimm 1): 0=Minimal, 63=Maximum, <b>6=Default</b>
  UINT8   ActEnergyCh0Dimm0;      ///< Offset 121  - Activation Energy (Channel 0, Dimm 0): 0=Minimal, 255=Maximum, <b>172=Default</b>
  UINT8   ActEnergyCh0Dimm1;      ///< Offset 120  - Activation Energy (Channel 0, Dimm 1): 0=Minimal, 255=Maximum, <b>172=Default</b>
  UINT8   ActEnergyCh1Dimm0;      ///< Offset 123  - Activation Energy (Channel 1, Dimm 0): 0=Minimal, 255=Maximum, <b>172=Default</b>
  UINT8   ActEnergyCh1Dimm1;      ///< Offset 122  - Activation Energy (Channel 1, Dimm 1): 0=Minimal, 255=Maximum, <b>172=Default</b>
  UINT8   RdEnergyCh0Dimm0;       ///< Offset 125  - Read Energy (Channel 0, Dimm 0): 0=Minimal, 255=Maximum, <b>212=Default</b>
  UINT8   RdEnergyCh0Dimm1;       ///< Offset 124  - Read Energy (Channel 0, Dimm 1): 0=Minimal, 255=Maximum, <b>212=Default</b>
  UINT8   RdEnergyCh1Dimm0;       ///< Offset 127  - Read Energy (Channel 1, Dimm 0): 0=Minimal, 255=Maximum, <b>212=Default</b>
  UINT8   RdEnergyCh1Dimm1;       ///< Offset 126  - Read Energy (Channel 1, Dimm 1): 0=Minimal, 255=Maximum, <b>212=Default</b>
  UINT8   WrEnergyCh0Dimm0;       ///< Offset 129  - Write Energy (Channel 0, Dimm 0): 0=Minimal, 255=Maximum, <b>221=Default</b>
  UINT8   WrEnergyCh0Dimm1;       ///< Offset 128  - Write Energy (Channel 0, Dimm 1): 0=Minimal, 255=Maximum, <b>221=Default</b>
  UINT8   WrEnergyCh1Dimm0;       ///< Offset 131  - Write Energy (Channel 1, Dimm 0): 0=Minimal, 255=Maximum, <b>221=Default</b>
  UINT8   WrEnergyCh1Dimm1;       ///< Offset 130  - Write Energy (Channel 1, Dimm 1): 0=Minimal, 255=Maximum, <b>221=Default</b>

  UINT16  SrefCfgIdleTmr;         ///< Offset 132  - Self Refresh idle timer: <b>512=Minimal</b>, 65535=Maximum
  UINT8   MaxRttWr;               ///< Offset 134  - Maximum DIMM RTT_WR to use in power training: <b>0=ODT Off</b>, 1 = 120 ohms
  UINT8   ThrtCkeMinTmr;          ///< Offset 135  - Throttler CKE min timer: 0=Minimal, 0xFF=Maximum, <b>0x30=Default</b>
  UINT8   ThrtCkeMinTmrLpddr;     ///< Offset 136  - Throttler CKE min timer for LPDDR: 0=Minimal, 0xFF=Maximum, <b>0x40=Default</b>

  UINT8   EnergyScaleFact;        ///< Offset 137 - Energy Scale Factor. 0=Minimal, 7=Maximum, <b>4=Default</b>
  UINT8   RaplPwrFlCh1;           ///< Offset 138 - Power Channel 1 Floor value: <b>0=Minimal</b>, 255=Maximum
  UINT8   RaplPwrFlCh0;           ///< Offset 139 - Power Channel 0 Floor value: <b>0=Minimal</b>, 255=Maximum
  UINT8   PowerDownMode;          ///< Offset 140 - CKE Power Down Mode: <b>0xFF=AUTO</b>, 0=No Power Down, 1= APD mode, 6=PPD-DLL Off mode
  UINT8   PwdwnIdleCounter;       ///< Offset 141 - CKE Power Down Mode Idle Counter: 0=Minimal, 255=Maximum, <b>0x80=0x80 DCLK</b>
  UINT8   CkeRankMapping;         ///< Offset 142  - Bits [7:4] - Channel 1, bits [3:0] - Channel 0. <b>0xAA=Default</b> Bit [i] specifies which rank CKE[i] goes to.
  UINT8   BerEnable;              ///< Offset 143  - BER Enable and # of Addresses passed in: <b>0=Minimal</b>, 8=Maximum
  UINT64  BerAddress[4];          ///< Offset 144  - BER Address(es): <b>0=Minimal</b>, 0xFFFFFFFFFFFFFFFF=Maximum (step is 0x40)
  UINT16  PciIndex;               ///< Offset 176  - Pci index register address: <b>0xCF8=Default</b>
  UINT16  PciData;                ///< Offset 178  - Pci data register address: <b>0xCFC=Default</b>

  UINT8   StrongWkLeaker;         ///< Offset 180  - Strong Weak Leaker: 1=Minimal, <b>7=Maximum</b>
  UINT8   CaVrefConfig;           ///< Offset 181 0=VREF_CA goes to both CH_A and CH_B, 1=VREF_CA to CH_A, VREF_DQ_A to CH_B, <b>2=VREF_CA to CH_A, VREF_DQ_B to CH_B</b>
  UINT16  FreqSaGvLow;            ///< Offset 182 SA GV Low: 0 is Auto/default, otherwise holds the frequency value: <b>0=Default</b>, 1067, 1200, 1333, 1400, 1600, 1800, 1867. NOTE: must be below or equal to the SA GV High frequency.
  UINT32  Vc1ReadMeterTimeWindow; ///< Offset 184  - VC1 Read Meter Time Window: 0=Minimal, 0x1FFFF=Maximum, <b>0x320=Default</b>
  UINT16  Vc1ReadMeterThreshold;  ///< Offset 188  - VC1 Read Meter Threshold (within Time Window): 0=Minimal, 0xFFFF=Maximum, <b>0x118=Default</b>
  UINT16  Idd3n;                  ///< Offset 190 EPG Active standby current (Idd3N) in milliamps from DIMM datasheet.
  UINT16  Idd3p;                  ///< Offset 192 EPG Active power-down current (Idd3P) in milliamps from DIMM datasheet.
  UINT8   EpgEnable;              ///< Offset 194 Enable Energy Performance Gain.
  UINT8   RhSolution;             ///< Offset 195 Type of solution to be used for RHP - 0/1 = HardwareRhp/Refresh2x
  UINT8   RhActProbability;       ///< Offset 196 Activation probability for Hardware RHP
  UINT8   SaGv;                   ///< Offset 197 SA GV: <b>0=Disabled</b>, 1=FixedLow, 2=FixedHigh, 3=Enabled

  UINT8   UserThresholdEnable;    ///< Offset 198 - Flag to manually select the DIMM CLTM Thermal Threshold, 0=Disable,  1=Enable, <b>0=Default</b>
  UINT8   UserBudgetEnable;       ///< Offset 199 - Flag to manually select the Budget Regiseters for CLTM Memory Dimms , 0=Disable,  1=Enable, <b>0=Default</b>
  UINT8   TsodTcritMax;           ///< Offset 200 - TSOD Tcrit Maximum Value  to be Configure , 0=Minimal, 128=Maximum, , <b>105=Default</b>
  UINT8   TsodEventMode;          ///< Offset 201 - Flag to Enable Event Mode Interruption in TSOD Configuration Register, 0=Disable,  1=Enable, <b>1=Default</b>
  UINT8   TsodEventPolarity;      ///< Offset 202 - Event Signal Polarity in TSOD Configuration Register, 0=Low,  1=High, <b>0=Default</b>
  UINT8   TsodCriticalEventOnly;  ///< Offset 203 - Critical Trigger Only in TSOD Configuration Register,0=Disable,  1=Enable, <b>1=Default</b>
  UINT8   TsodEventOutputControl; ///< Offset 204 - Event Output Control in TSOD Configuration Register,0=Disable,  1=Enable, <b>1=Default</b>
  UINT8   TsodAlarmwindowLockBit; ///< Offset 205 - Alarm Windows Lock Bit in TSOD Configuration Register,0=Unlock,  1=Lock, <b>0=Default</b>
  UINT8   TsodCriticaltripLockBit;///< Offset 206 - Critical Trip Lock Bit in TSOD Configuration Register,0=Unlock,  1=Lock, <b>0=Default</b>
  UINT8   TsodShutdownMode;       ///< Offset 207 - Shutdown Mode TSOD Configuration Register,0=Enable,  1=Disable, <b>0=Default</b>
  UINT8   TsodThigMax;            ///< Offset 208 - Thigh Max Value In the  for CLTM Memory Dimms , 0=Disable,  1=Enable, <b>0=Default</b>
  UINT8   TsodManualEnable;       ///< Offset 209 - Flag to manually select the TSOD Register Values , 0=Disable,  1=Enable, <b>0=Default</b>

  UINT8   RetrainOnFastFail;      ///< Offset 210 - Restart MRC in Cold mode if SW MemTest fails during Fast flow. 0 = Disabled, <b>1 = Enabled</b>
  UINT8   ForceOltmOrRefresh2x;   ///< Offset 211 - Force OLTM or 2X Refresh when needed. <b>0 = Force OLTM</b>, 1 = Force 2x Refresh
  UINT8   DllBwEn0;               ///< Offset 212 - DllBwEn value for 1067
  UINT8   DllBwEn1;               ///< Offset 213 - DllBwEn value for 1333
  UINT8   DllBwEn2;               ///< Offset 214 - DllBwEn value for 1600
  UINT8   DllBwEn3;               ///< Offset 215 - DllBwEn value for 1867 and up
  UINT32  VddSettleWaitTime;      ///< Offset 216 - Amount of time in microseconds to wait for Vdd to settle on top of 200us required by JEDEC spec: <b>Default=0</b>
  UINT8   EnCmdRate;              ///< Offset 220  - CMD Rate Enable: 0=Disable, 1=1 CMD, 2=2 CMDs, <b>3=3 CMDs</b>, 4=4 CMDs, 5=5 CMDs, 6=6 CMDs, 7=7 CMDs
  UINT8   Refresh2X;              ///< Offset 221  - Refresh 2x: <b>0=Disable</b>, 1=Enable for WARM or HOT, 2=Enable for HOT only
  UINT8   SmramMask;              ///< Offset 222 Reserved memory ranges for SMRAM
  UINT8   Rsvd0;                  ///< Offset 223 - Reserved.

  //
  // Training Algorithms
  //
  UINT32  CMDSR     : 1;          ///< Offset 224 - CMD Slew Rate Training: 0=Disable, <b>1=Enable</b>.
  UINT32  CMDDSEQ   : 1;          ///<  - CMD Drive Strength and Tx Equalization: 0=Disable, <b>1=Enable</b>.
  UINT32  CMDNORM   : 1;          ///<  - CMD Normalization: 0=Disable, <b>1=Enable</b>.
  UINT32  EWRDSEQ   : 1;          ///<  - Early DQ Write Drive Strength and Equalization Training: 0=Disable, <b>1=Enable</b>.
  UINT32  RsvdBits2 :28;
} MEMORY_CONFIGURATION;

/**
 No CRC Memory Configuration
 The contents of this structure are not CRC'd by the MRC for option change detection.\n
  <b>Revision 1</b>:
  - Initial version.

  <b>Revision 2</b>:
  - No changes (updated due to a change in MEMORY_CONFIGURATION)
**/
typedef struct {
  CONFIG_BLOCK_HEADER      Header;              ///< Offset 0-27 Config Block Header
  SA_FUNCTION_CALLS        SaCall;              ///< Offset 28   - Function calls into the SA.
  SA_MEMORY_FUNCTIONS      MrcCall;             ///< Offset 204  - Function calls into the MRC.
  SPD_DATA_BUFFER          *SpdData;            ///< Offset 240  - Memory SPD data, will be used by the MRC when SPD SmBus address is zero.
  SA_MEMORY_DQ_MAPPING     *DqByteMap;          ///< Offset 244  - LPDDR3 DQ byte mapping to CMD/CTL/CLK, from the CPU side.
  SA_MEMORY_DQS_MAPPING    *DqsMap;             ///< Offset 248  - LPDDR3 DQS byte swizzling between CPU and DRAM.
  SA_MEMORY_RCOMP          *RcompData;          ///< Offset 252  - DDR RCOMP resistors and target values.
  /**
   The minimum platform memory size required to pass control into DXE.
   @note Some debug features, such as Intel Processor Trace and Intel Trace Hub,
   may require updating this policy to a larger value to accommodate those memory requests.
   @note 4GB restriction: Due to 32-bit memory address space restriction,
   increasing this value above default is not guaranteed to work, and will depend on available memory in 4GB space.
  **/
  UINT64                   PlatformMemorySize;  ///< Offset 256
  UINT32                   MmaTestContentPtr;   ///< Offset 264  - Pointer to MMA Test Content Data. Used in FSP.
  UINT32                   MmaTestContentSize;  ///< Offset 268  - Size of MMA Test Content Data. Used in FSP.
  UINT32                   MmaTestConfigPtr;    ///< Offset 272  - Pointer to MMA Test Config Data. Used in FSP.
  UINT32                   MmaTestConfigSize;   ///< Offset 276  - Size of MMA Test Config Data. Used in FSP.
  UINT32                   CleanMemory:1;       ///< Offset 280  - Ask MRC to clear memory content: <b>FALSE=Do not Clear Memory</b>; TRUE=Clear Memory
  UINT32                   RsvdBits0:31;
  /**
   Sets the serial debug message level\n
     0x00 = Disabled\n
     0x01 = Errors only\n
     0x02 = Errors and Warnings\n
     <b>0x03 = Errors, Warnings, and Info</b>\n
     0x04 = Errors, Warnings, Info, and Events\n
     0x05 = Displays Memory Init Execution Time Summary only\n
  **/
  UINT8                    SerialDebugLevel;    ///< Offset 284
  UINT8                    Rsvd0[3];            ///< Offset 285
} MEMORY_CONFIG_NO_CRC;
#pragma pack(pop)

#endif // _MEMORY_CONFIG_H_
