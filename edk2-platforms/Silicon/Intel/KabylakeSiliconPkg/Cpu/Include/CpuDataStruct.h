/** @file
  This file declares various data structures used in CPU reference code.

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _CPU_DATA_STRUCT_H
#define _CPU_DATA_STRUCT_H

//
// The reason for changing the state of the processor Only applies to Disabling processors.
// In future, we can add add/remove support
//
#define CPU_CAUSE_NOT_DISABLED      0x0000
#define CPU_CAUSE_INTERNAL_ERROR    0x0001
#define CPU_CAUSE_THERMAL_ERROR     0x0002
#define CPU_CAUSE_SELFTEST_FAILURE  0x0004
#define CPU_CAUSE_PREBOOT_TIMEOUT   0x0008
#define CPU_CAUSE_FAILED_TO_START   0x0010
#define CPU_CAUSE_CONFIG_ERROR      0x0020
#define CPU_CAUSE_USER_SELECTION    0x0080
#define CPU_CAUSE_BY_ASSOCIATION    0x0100
#define CPU_CAUSE_UNSPECIFIED       0x8000

#define MAX_MICROCODE_PATCH_SIZE    0x20000

typedef UINT32 CPU_STATE_CHANGE_CAUSE;

///
/// The data structure is used retrieve data required for MP initialization during S3 resume.
///
typedef struct {
  BOOLEAN              APState;                ///< Indicates whether the newstate of the AP is enabled or disabled.
  BOOLEAN              S3BootPath;             ///< TRUE: S3 Boot path. FALSE: Regular boot path.
  EFI_PHYSICAL_ADDRESS WakeUpBuffer;           ///< Buffer location used during AP Initialization
  EFI_PHYSICAL_ADDRESS GdtrProfile;            ///< Address of Global Descriptor Table
  EFI_PHYSICAL_ADDRESS IdtrProfile;            ///< address of Interrupt Descriptor Table
  EFI_PHYSICAL_ADDRESS CpuPrivateData;         ///< Address of CPU_PRIVATE_DATA structure
  EFI_PHYSICAL_ADDRESS StackAddress;           ///< Address of APs's stacks
  EFI_PHYSICAL_ADDRESS SmramBase;              ///< @deprecated Address of SMRAM base as seen by software executing on the processors
  EFI_PHYSICAL_ADDRESS SmmStartImageBase;      ///< @deprecated Address of SMM Start Image Base
  UINT32               SmmStartImageSize;      ///< @deprecated Size of SMM Start Image
  UINT32               NumberOfCpus;           ///< Number of processors
} ACPI_CPU_DATA;

///
/// CPU information stored in SMRAM during DXE.
///
typedef struct {
  //
  // Guid as Signature.
  //
  EFI_GUID             HeaderGuid;
  EFI_PHYSICAL_ADDRESS AcpiCpuPointer;
  ACPI_CPU_DATA        AcpiCpuData;

  //
  // It points the data defined below.
  //
  EFI_PHYSICAL_ADDRESS GdtrProfileOffset;
  EFI_PHYSICAL_ADDRESS GdtOffset;
  EFI_PHYSICAL_ADDRESS IdtrProfileOffset;
  EFI_PHYSICAL_ADDRESS IdtOffset;
  EFI_PHYSICAL_ADDRESS CpuPrivateDataOffset;
  EFI_PHYSICAL_ADDRESS S3BspMtrrTableOffset;
  ///
  /// We need put all the data buffer here as well.
  /// These data will be copied to original location in S3.
  ///
  //
  // DataBuffer size
  //
  UINT32 GdtrProfileSize;
  UINT32 GdtSize;
  UINT32 IdtrProfileSize;
  UINT32 IdtSize;
  UINT32 CpuPrivateDataSize;
  UINT32 S3BspMtrrTableSize;
} SMRAM_CPU_DATA;

///
/// Structure to hold the return value of AsmCpuid instruction
///
typedef struct {
  UINT32 RegEax; ///< Value of EAX.
  UINT32 RegEbx; ///< Value of EBX.
  UINT32 RegEcx; ///< Value of ECX.
  UINT32 RegEdx; ///< Value of EDX.
} EFI_CPUID_REGISTER;

///
/// Structure to describe microcode header
///
typedef struct {
  UINT32 HeaderVersion;  ///< Version number of the update header.
  UINT32 UpdateRevision; ///< Unique version number for the update.
  UINT32 Date;           ///< Date of the update creation.
  UINT32 ProcessorId;    ///< Signature of the processor that requires this update.
  UINT32 Checksum;       ///< Checksum of update data and header.
  UINT32 LoaderRevision; ///< Version number of the microcode loader program.
  UINT32 ProcessorFlags; ///< Lower 4 bits denoting platform type information.
  UINT32 DataSize;       ///< Size of encoded data in bytes.
  UINT32 TotalSize;      ///< Total size of microcode update in bytes.
  UINT8  Reserved[12];   ///< Reserved bits.
} CPU_MICROCODE_HEADER;

///
/// Structure to describe the extended signature table header of the microcode update
///
typedef struct {
  UINT32 ExtendedSignatureCount; ///< Number of extended signature structures.
  UINT32 ExtendedTableChecksum;  ///< Checksum of update extended processor signature table.
  UINT8  Reserved[12];           ///< Reserved bits.
} CPU_MICROCODE_EXTENDED_TABLE_HEADER;

///
/// Structure to describe the data of the extended table of the microcode update
///
typedef struct {
  UINT32 ProcessorSignature; ///< Extended signature of the processor that requires this update
  UINT32 ProcessorFlag;      ///< Lower 4 bits denoting platform type information
  UINT32 ProcessorChecksum;  ///< checksum of each of the extended update
} CPU_MICROCODE_EXTENDED_TABLE;

#pragma pack(1)
///
/// MSR_REGISTER definition as a Union of QWORDS, DWORDS and BYTES
///
typedef union _MSR_REGISTER {
  UINT64  Qword;       ///< MSR value in 64 bit QWORD.

  ///
  /// MSR value represented in two DWORDS
  ///
  struct {
    UINT32  Low;       ///< Lower DWORD of the 64 bit MSR value.
    UINT32  High;      ///< Higher DWORD of the 64 bit MSR value.
  } Dwords;

  ///
  /// MSR value represented in eight bytes.
  ///
  struct {
    UINT8 FirstByte;   ///< First byte of the 64 bit MSR value.
    UINT8 SecondByte;  ///< Second byte of the 64 bit MSR value.
    UINT8 ThirdByte;   ///< Third byte of the 64 bit MSR value.
    UINT8 FouthByte;   ///< Fourth byte of the 64 bit MSR value.
    UINT8 FifthByte;   ///< Fifth byte of the 64 bit MSR value.
    UINT8 SixthByte;   ///< Sixth byte of the 64 bit MSR value.
    UINT8 SeventhByte; ///< Seventh byte of the 64 bit MSR value.
    UINT8 EighthByte;  ///< Eigth byte of the 64 bit MSR value.
  } Bytes;
} MSR_REGISTER;

///
/// Store BIST data for BSP.
///
typedef struct {
  UINT32 ApicId;    ///< APIC ID
  UINT32 Health;    ///< BIST result
} BIST_HOB_DATA;

///
/// Processor trace buffer size selection.
///
typedef enum {
  Enum4K    = 0,
  Enum8K,
  Enum16K,
  Enum32K,
  Enum64K,
  Enum128K,
  Enum256K,
  Enum512K,
  Enum1M,
  Enum2M,
  Enum4M,
  Enum8M,
  Enum16M,
  Enum32M,
  Enum64M,
  Enum128M,
  EnumProcTraceMemDisable
} PROC_TRACE_MEM_SIZE;

#pragma pack()

#endif
