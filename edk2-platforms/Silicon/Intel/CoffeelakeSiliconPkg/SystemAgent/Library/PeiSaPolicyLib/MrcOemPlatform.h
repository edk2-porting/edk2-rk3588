/** @file
  This file contains functions that read the SPD data for each DIMM slot over
  the SMBus interface.
  This file is SampleCode for Intel SA PEI Policy initialization.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiSaPolicyLibrary.h"
#include "MrcInterface.h"

#define RTC_INDEX_REGISTER        (0x70)
#define RTC_TARGET_REGISTER       (0x71)

#define RTC_INDEX_MASK            (0x7F)
#define RTC_BANK_SIZE             (0x80)

#define RTC_SECONDS               (0x00)
#define RTC_MINUTES               (0x02)
#define RTC_HOURS                 (0x04)
#define RTC_DAY_OF_MONTH          (0x07)
#define RTC_MONTH                 (0x08)
#define RTC_YEAR                  (0x09)
#define CMOS_REGA                 (0x0A)
#define CMOS_REGB                 (0x0B)
#define CMOS_REGC                 (0x0C)
#define CMOS_REGD                 (0x0D)

#define RTC_UPDATE_IN_PROGRESS    (0x80)
#define RTC_HOLD                  (0x80)
#define RTC_MODE_24HOUR           (0x02)
#define RTC_CLOCK_DIVIDER         (0x20)
#define RTC_RATE_SELECT           (0x06)

#define BCD2BINARY(A)             (((((A) >> 4) & 0xF) * 10) + ((A) & 0xF))
#define CENTURY_OFFSET            (2000)

/**
  Read the SPD data over the SMBus, at the given SmBus SPD address and copy the data to the data structure.
  The SPD data locations read is controlled by the current boot mode.

  @param[in] BootMode           - The current MRC boot mode.
  @param[in] Address            - SPD SmBus address offset.
  @param[in] Buffer             - Buffer that contains the data read from the SPD.
  @param[in] SpdDdr3Table       - Indicates which SPD bytes to read.
  @param[in] SpdDdr3TableSize   - Size of SpdDdr3Table in bytes.
  @param[in] SpdDdr4Table       - Indicates which SPD bytes to read.
  @param[in] SpdDdr4TableSize   - Size of SpdDdr4Table in bytes.
  @param[in] SpdLpddrTable      - Indicates which SPD bytes to read.
  @param[in] SpdLpddrTableSize  - Size of SpdLpddrTable in bytes.

  @retval TRUE if the read is successful, otherwise FALSE on error.
**/
BOOLEAN
GetSpdData (
  IN SPD_BOOT_MODE BootMode,
  IN UINT8         Address,
  IN OUT   UINT8   *Buffer,
  IN UINT8         *SpdDdr3Table,
  IN UINT32        SpdDdr3TableSize,
  IN UINT8         *SpdDdr4Table,
  IN UINT32        SpdDdr4TableSize,
  IN UINT8         *SpdLpddrTable,
  IN UINT32        SpdLpddrTableSize
  );

/**
  Output a string to the debug stream/device.

  @param[in] String     - The string to output.
**/
VOID
SaDebugPrint (
  VOID   *String
  );

/**
  Calculate the PCI device address for the given Bus/Device/Function/Offset.

  @param[in] Bus      - PCI bus
  @param[in] Device   - PCI device
  @param[in] Function - PCI function
  @param[in] Offset   - Offset

  @retval The PCI device address.
**/
UINT32
GetPciDeviceAddress (
  IN const UINT8 Bus,
  IN const UINT8 Device,
  IN const UINT8 Function,
  IN const UINT8 Offset
  );

/**
  Calculate the PCIE device address for the given Bus/Device/Function/Offset.

  @param[in] Bus      - PCI bus
  @param[in] Device   - PCI device
  @param[in] Function - PCI function
  @param[in] Offset   - Offset

   The PCIE device address.

  @retval The PCIe device address
**/
UINT32
GetPcieDeviceAddress (
  IN const UINT8 Bus,
  IN const UINT8 Device,
  IN const UINT8 Function,
  IN const UINT8 Offset
  );

/**
  Read specific RTC/CMOS RAM

  @param[in] Location        Point to RTC/CMOS RAM offset for read

  @retval The data of specific location in RTC/CMOS RAM.
**/
UINT8
PeiRtcRead (
  IN const UINT8 Location
  );

/**
  Returns the current time, as determined by reading the Real Time Clock (RTC) on the platform.
  Since RTC time is stored in BCD, convert each value to binary.

  @param[out] Seconds       - The current second (0-59).
  @param[out] Minutes       - The current minute (0-59).
  @param[out] Hours         - The current hour (0-23).
  @param[out] DayOfMonth    - The current day of the month (1-31).
  @param[out] Month         - The current month (1-12).
  @param[out] Year          - The current year (2000-2099).
**/
VOID
GetRtcTime (
  OUT UINT8  *const Seconds,
  OUT UINT8  *const Minutes,
  OUT UINT8  *const Hours,
  OUT UINT8  *const DayOfMonth,
  OUT UINT8  *const Month,
  OUT UINT16 *const Year
  );

/**
  Gets CPU current time.

  @param[in] GlobalData - Pointer to global MRC data struct.

  @retval The current CPU time in milliseconds.
**/
UINT64
GetCpuTime (
  IN VOID     *GlobalData
  );

/**
  Sets the specified number of memory words, a word at a time, at the
  specified destination.

  @param[in, out] Dest     - Destination pointer.
  @param[in]      NumWords - The number of dwords to set.
  @param[in]      Value    - The value to set.

  @retval Pointer to the buffer.
**/
VOID *
SetMemWord (
  IN OUT VOID     *Dest,
  IN UINTN        NumWords,
  IN const UINT16 Value
  );

/**
  Sets the specified number of memory dwords, a dword at a time, at the
  specified destination.

  @param[in, out] Dest      - Destination pointer.
  @param[in]      NumDwords - The number of dwords to set.
  @param[in]      Value     - The value to set.

  @retval Pointer to the buffer.
**/
VOID *
SetMemDword (
  IN OUT VOID     *Dest,
  IN UINT32       NumDwords,
  IN const UINT32 Value
  );

/**
  Read 64 bits from the Memory Mapped I/O space.
  Use MMX instruction for atomic access, because some MC registers have side effect.

  @param[in] Address - Memory mapped I/O address.
**/
UINT64
SaMmioRead64 (
  IN  UINTN Address
  );

/**
  Write 64 bits to the Memory Mapped I/O space.
  Use MMX instruction for atomic access, because some MC registers have side effect.

  @param[in] Address - Memory mapped I/O address.
  @param[in] Value   - The value to write.
**/
UINT64
SaMmioWrite64 (
  IN UINTN Address,
  IN UINT64 Value
  );

/**
  Intel Silicon View Technology check point interface based on IO port reading

  @param CheckPoint        Check point AH value.
                           AH = 0x10:  End of MRC State
                           AH = 0x20:  End of DXE State
                           AH = 0x30:  Ready to boot before INT-19h or UEFI boot
                           AH = 0x40:  After OS booting, need a timer SMI trigger to implement (TBD)

  @param PortReading       IO port reading address used for breakpoints
**/
VOID
EFIAPI
IsvtCheckPoint (
  IN UINT32          CheckPoint,
  IN UINT32          PortReading
  );

/**
  Gets the current memory voltage (VDD).

  @param[in] GlobalData - Pointer to global MRC data struct.
  @param[in] DefaultVdd - Default Vdd for the given platform.

  @retval The current memory voltage (VDD), in millivolts. 0 means platform default.
**/
UINT32
GetMemoryVdd (
  IN VOID     *GlobalData,
  IN UINT32   DefaultVdd
  );

/**
  Sets the memory voltage (VDD) to the specified value.

  @param[in] GlobalData - Pointer to global MRC data struct.
  @param[in] DefaultVdd - Default Vdd for the given platform.
  @param[in] Voltage    - The new memory voltage to set.

  @retval The actual memory voltage (VDD), in millivolts, that is closest to what the caller passed in.
**/
UINT32
SetMemoryVdd (
  IN VOID     *GlobalData,
  IN UINT32   DefaultVdd,
  IN UINT32   Voltage
  );

/**
  Check point that is called at various points in the MRC.

  @param[in] GlobalData - MRC global data.
  @param[in] Command    - OEM command.
  @param[in] Pointer    - Command specific data.

  @retval MrcStatus value.
**/
UINT32
CheckPoint (
  VOID   *GlobalData,
  UINT32 Command,
  VOID   *Pointer
  );

/**
  Typically used to display to the I/O port 80h.

  @param[in] GlobalData         - Mrc Global Data
  @param[in] DisplayDebugNumber - the number to display on port 80.

  @retval Nothing.
**/
VOID
DebugHook (
  VOID          *GlobalData,
  UINT16        DisplayDebugNumber
  );

/**
  Hook to take any action after returning from MrcStartMemoryConfiguration()
  and prior to taking any action regarding MrcStatus.  Pre-populated with issuing
  Intel Silicon View Technology (ISVT) checkpoint 0x01.

  @param[in] GlobalData         - Mrc Global Data
  @param[in] MrcStatus          - Mrc status variable
**/
VOID
ReturnFromSmc (
  VOID          *GlobalData,
  UINT32        MrcStatus
  );

/**
  Assert or deassert DRAM_RESET# pin; this is used in JEDEC Reset.

  @param[in] PciEBaseAddress  - PCI express base address.
  @param[in] ResetValue       - desired value of DRAM_RESET#. 1 - reset deasserted, 0 - reset asserted.
**/
VOID
SaDramReset (
  IN UINT32 PciEBaseAddress,
  IN UINT32 ResetValue
  );

