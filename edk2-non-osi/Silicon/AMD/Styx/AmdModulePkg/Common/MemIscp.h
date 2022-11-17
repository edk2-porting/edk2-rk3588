/* $NoKeywords */
/**
 * @file
 *
 * MemIscp.h
 *
 * Contains common Memory Training ISCP-related structures and defines.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: FDK
 * @e sub-project: UEFI
 * @e version: $Revision: 317558 $ @e date: $Date: 2015-04-24 17:20:55 -0700 (Fri, 24 Apr 2015) $
 *
 */
/*****************************************************************************
*
*  Copyright 2013 - 2016 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
*
*  This program and the accompanying materials are licensed and made available
*  under the terms and conditions of the BSD License which accompanies this
*  distribution. The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR
*  IMPLIED.
*
***************************************************************************/

//#########################################################################
//#########################################################################
//#########################################################################
//        NOTE: This file shared between SCP and UEFI, make sure all     //
//              changes are reflected in both copies.                    //
//#########################################################################
//#########################################################################
//#########################################################################

#ifndef MEMISCP_H_
#define MEMISCP_H_

#ifdef __cplusplus
  extern "C" {
  #endif

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

  /// Memory Attribute enum
  typedef enum {
    MEM_AVAILABLE = 1,                        ///< Memory Available
    MEM_RESERVED,                             ///< Memory Reserved
    MEM_ACPI,                                 ///< Memory ACPI
    MEM_NVS,                                  ///< Memory NVS
    MEM_UNUSABLE                              ///< Memory Unavailable
  } MEMORY_ATTRIBUTE;

  /// Memory descriptor structure for each memory range
  typedef struct {
    UINT64            Base0;                  ///< Base address of memory range 0
    UINT64            Size0;                  ///< Size of memory range 0
    MEMORY_ATTRIBUTE  Attribute0;             ///< Attribute of memory range 0
    UINT32            Padding0;               ///< 4-byte Padding to get 8-byte alignment
    UINT64            Base1;                  ///< Base address of memory range 1
    UINT64            Size1;                  ///< Size of memory range 1
    MEMORY_ATTRIBUTE  Attribute1;             ///< Attribute of memory range 1
    UINT32            Padding1;               ///< 4-byte Padding to get 8-byte alignment
    UINT64            Base2;                  ///< Base address of memory range 2
    UINT64            Size2;                  ///< Size of memory range 2
    MEMORY_ATTRIBUTE  Attribute2;             ///< Attribute of memory range 2
    UINT32            Padding2;               ///< 4-byte Padding to get 8-byte alignment
    UINT64            Base3;                  ///< Base address of memory range 3
    UINT64            Size3;                  ///< Size of memory range 3
    MEMORY_ATTRIBUTE  Attribute3;             ///< Attribute of memory range 3
    UINT32            Padding3;               ///< 4-byte Padding to get 8-byte alignment
  } AMD_MEMORY_RANGE_DESCRIPTOR;

  /// SMBIOS Structure Header
  typedef struct {
    UINT8   Type;                             ///< TYPE
    UINT8   Length;                           ///< Length of TYPE
    UINT16  Handle;                           ///< structure handle, a unique 16-bit number in the range 0 to 0FEFFh
  } ISCP_SMBIOS_STRUCTURE_HEADER;

  /// DMI Type 16 - Physical Memory Array
  typedef struct {
    UINT16                    Location;               ///< The physical location of the Memory Array,
                                                      ///< whether on the system board or an add-in board.
    UINT16                    Use;                    ///< Identifies the function for which the array
                                                      ///< is used.
    UINT16                    MemoryErrorCorrection;  ///< The primary hardware error correction or
                                                      ///< detection method supported by this memory array.
                                                      ///< ..for memory devices in this array.
    UINT16                    NumberOfMemoryDevices;  ///< The number of slots or sockets available..
                                                      ///< ..for memory devices in this array.
  } ISCP_TYPE16_SMBIOS_INFO;

  /// DMI Type 17 offset 13h - Type Detail
  typedef struct {
    UINT16                    Reserved1:1;            ///< Reserved
    UINT16                    Other:1;                ///< Other
    UINT16                    Unknown:1;              ///< Unknown
    UINT16                    FastPaged:1;            ///< Fast-Paged
    UINT16                    StaticColumn:1;         ///< Static column
    UINT16                    PseudoStatic:1;         ///< Pseudo-static
    UINT16                    Rambus:1;               ///< RAMBUS
    UINT16                    Synchronous:1;          ///< Synchronous
    UINT16                    Cmos:1;                 ///< CMOS
    UINT16                    Edo:1;                  ///< EDO
    UINT16                    WindowDram:1;           ///< Window DRAM
    UINT16                    CacheDram:1;            ///< Cache Dram
    UINT16                    NonVolatile:1;          ///< Non-volatile
    UINT16                    Registered:1;           ///< Registered (Buffered)
    UINT16                    Unbuffered:1;           ///< Unbuffered (Unregistered)
    UINT16                    Reserved2:1;            ///< Reserved
  } SMBIOS_T17_TYPE_DETAIL;

  /// DMI Type 17 - Memory Device
  typedef struct {
    UINT16                    Handle;                  ///< The temporary handle, or instance number, associated with the structure
    UINT16                    TotalWidth;              ///< Total Width, in bits, of this memory device, including any check or error-correction bits.
    UINT16                    DataWidth;               ///< Data Width, in bits, of this memory device.
    UINT16                    MemorySize;              ///< The size of the memory device.
    UINT16                    FormFactor;              ///< The implementation form factor for this memory device.
    UINT16                    DeviceSet;               ///< Identifies when the Memory Device is one of a set of..
                                                       ///< ..memory devices that must be populated with all devices of..
                                                       ///< ..the same type and size, and the set to which this device belongs.
    CHAR8                     DeviceLocator[8];        ///< The string number of the string that identifies the physically labeled socket or board position where the memory device is located.
    CHAR8                     BankLocator[16];         ///< The string number of the string that identifies the physically labeled bank where the memory device is located.
    UINT16                    MemoryType;              ///< The type of memory used in this device.
    SMBIOS_T17_TYPE_DETAIL    TypeDetail;              ///< Additional detail on the memory device type
    UINT16                    Speed;                   ///< Identifies the speed of the device, in megahertz (MHz).
    UINT8                     ManufacturerIdCode[8];   ///< Manufacturer ID code.
    CHAR8                     SerialNumber[16];        ///< Serial Number.
    CHAR8                     PartNumber[20];          ///< Part Number.
    UINT16                    Attributes;              ///< Bits 7-4: Reserved, Bits 3-0: rank.
    UINT32                    ExtSize;                 ///< Extended Size.
    UINT16                    ConfigSpeed;             ///< Configured memory clock speed
  } ISCP_TYPE17_SMBIOS_INFO;

  /// DMI Type 19 - Memory Array Mapped Address
  typedef struct {
    UINT32                    StartingAddr;           ///< The physical address, in kilobytes,
                                                      ///< of a range of memory mapped to the
                                                      ///< specified physical memory array.
    UINT32                    EndingAddr;             ///< The physical ending address of the
                                                      ///< last kilobyte of a range of addresses
                                                      ///< mapped to the specified physical memory array.
    UINT16                    MemoryArrayHandle;      ///< The handle, or instance number, associated
                                                      ///< with the physical memory array to which this
                                                      ///< address range is mapped.
    UINT8                     PartitionWidth;         ///< Identifies the number of memory devices that
                                                      ///< form a single row of memory for the address
                                                      ///< partition defined by this structure.
    UINT64                    ExtStartingAddr;        ///< The physical address, in bytes, of a range of
                                                      ///< memory mapped to the specified Physical Memory Array.
    UINT64                    ExtEndingAddr;          ///< The physical address, in bytes, of a range of
                                                      ///< memory mapped to the specified Physical Memory Array.
  } ISCP_TYPE19_SMBIOS_INFO;

  #ifdef __cplusplus
  }
#endif


#endif /* MEMISCP_H_ */
