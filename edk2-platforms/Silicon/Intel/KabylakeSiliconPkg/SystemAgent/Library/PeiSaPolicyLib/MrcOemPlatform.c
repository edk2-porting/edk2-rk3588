/** @file
  This file is SampleCode for Intel SA PEI Policy initialization.

Copyright (c) 2017 - 2020 Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "MrcOemPlatform.h"
#include <Library/CpuPlatformLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmbusLib.h>
#include <PchAccess.h>
#include <Library/PeiSaPolicyLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/ConfigBlockLib.h>

#pragma pack (push, 1)
typedef union {
  struct {
    UINT32                         : 8;
    UINT32 MAX_NON_TURBO_LIM_RATIO : 8;
    UINT32                         : 16;
    UINT32                         : 32;
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} PCU_CR_PLATFORM_INFO_STRUCT;

#pragma pack (pop)

#define SA_SYSTEM_BCLK               (100)
#define PCU_CR_PLATFORM_INFO         (0xCE)
#define MRC_POST_CODE_LOW_BYTE_ADDR  (0x48)
#define MRC_POST_CODE_HIGH_BYTE_ADDR (0x49)
#define MAX_SPD_PAGE_COUNT           (2)
#define MAX_SPD_PAGE_SIZE            (256)
#define MAX_SPD_DDR3_SIZE            (MAX_SPD_PAGE_SIZE * 1)
#define MAX_SPD_DDR4_SIZE            (MAX_SPD_PAGE_SIZE * 2)
#define MAX_SPD_SIZE                 (MAX_SPD_PAGE_SIZE * MAX_SPD_PAGE_COUNT)
#define SPD_PAGE_ADDRESS_0           (0x6C)
#define SPD_PAGE_ADDRESS_1           (0x6E)
#define SPD_DDR3_XMP_OFFSET          (176)
#define SPD_DDR4_XMP_OFFSET          (384)
#define SPD_DDR3_SDRAM_TYPE_OFFSET   (0x02)
#define SPD_DDR3_SDRAM_TYPE_NUMBER   (0x0B)
#define SPD_DDR4_SDRAM_TYPE_NUMBER   (0x0C)
#define SPD_LPDDR3_SDRAM_TYPE_NUMBER (0xF1)
#define SPD_JEDEC_LPDDR3_SDRAM_TYPE_NUMBER (0x0F)

/**
  Read the SPD data over the SMBus, at the specified SPD address, starting at
  the specified starting offset and read the given amount of data.

  @param[in] SpdAddress  - SPD SMBUS address
  @param[in, out] Buffer - Buffer to store the data.
  @param[in] Start       - Starting SPD offset
  @param[in] Size        - The number of bytes of data to read and also the size of the buffer.
  @param[in, out] Page   - The final page that is being pointed to.

  @retval RETURN_SUCCESS if the read is successful, otherwise error status.
**/
static
RETURN_STATUS
DoSpdRead (
  IN     const UINT8  SpdAddress,
  IN OUT UINT8        *const Buffer,
  IN     const UINT16 Start,
  IN           UINT16 Size,
  IN OUT UINT8        *const Page
  )
{
  RETURN_STATUS EfiStatus;
  BOOLEAN       PageUpdate;
  UINT16        Count;
  UINT16        Index;

  EfiStatus = RETURN_DEVICE_ERROR;
  if ((Buffer != NULL) && (Start < MAX_SPD_SIZE) && ((Start + Size) < MAX_SPD_SIZE)) {
    Count = 0;
    PageUpdate = FALSE;
    while (Size--) {
      Index = Start + Count;
      if ((Index / MAX_SPD_PAGE_SIZE) != *Page) {
        *Page = (UINT8) (Index / MAX_SPD_PAGE_SIZE);
        PageUpdate = TRUE;
      }
      Index %= MAX_SPD_PAGE_SIZE;
      if (PageUpdate == TRUE) {
        PageUpdate = FALSE;
        SmBusWriteDataByte ((*Page == 0) ? SPD_PAGE_ADDRESS_0 : SPD_PAGE_ADDRESS_1, 0, &EfiStatus);
      }
      Buffer[Count] = SmBusReadDataByte (SpdAddress | ((UINT32) Index << 8), &EfiStatus);
      if (RETURN_SUCCESS != EfiStatus) {
        Buffer[Count] = 0;
        break;
      }
      Count++;
    }
    EfiStatus = RETURN_SUCCESS;
  }
  return (EfiStatus);
}

/**
  See if there is valid XMP SPD data.

  @param[in] Debug    - Mrc debug structure.
  @param[in, out] Spd - Mrc SPD structure.
  @param[in] XmpStart - The current offset in the SPD.

  @retval TRUE if valid, FALSE in not.
**/
static
BOOLEAN
VerifyXmp (
  IN OUT MrcSpd *const Spd,
  IN const UINT16 XmpStart
  )
{
  SPD_EXTREME_MEMORY_PROFILE_HEADER      *Header1;
  SPD_EXTREME_MEMORY_PROFILE_HEADER_2_0  *Header2;
  BOOLEAN                                 Xmp;

  Xmp = FALSE;

  switch (((UINT8 *) Spd) [2]) {
    case SPD_DDR3_SDRAM_TYPE_NUMBER:
      Header1 = &Spd->Ddr3.Xmp.Header;
      if (XmpStart == ((UINT32) (Header1) - (UINT32) Spd)) {
        Xmp = TRUE;
        if ((Header1->XmpRevision.Data & 0xFE) == 0x12) {
          return (TRUE);
        } else {
          Header1->XmpId            = 0;
          Header1->XmpOrgConf.Data  = 0;
          Header1->XmpRevision.Data = 0;
        }
      }
      break;
    case SPD_DDR4_SDRAM_TYPE_NUMBER:
      Header2 = &Spd->Ddr4.EndUser.Xmp.Header;
      if (XmpStart == ((UINT32) (Header2) - (UINT32) Spd)) {
        Xmp = TRUE;
        if ((Header2->XmpRevision.Data) == 0x20) {
          return (TRUE);
        } else {
          Header2->XmpId            = 0;
          Header2->XmpOrgConf.Data  = 0;
          Header2->XmpRevision.Data = 0;
        }
      }
      break;
    case SPD_LPDDR3_SDRAM_TYPE_NUMBER:
    case SPD_JEDEC_LPDDR3_SDRAM_TYPE_NUMBER:
      return (TRUE);
    default:
      return (FALSE);
  }
  if (!Xmp) {
    return (TRUE);
  }
  return (FALSE);
}

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
  )
{
  const SPD_OFFSET_TABLE *Tbl;
  const SPD_OFFSET_TABLE *TableSelect;
  RETURN_STATUS          Status;
  UINT32                 Byte;
  UINT32                 Stop;
  UINT8                  Page;

  Page   = (UINT8) (~0);
  Status = DoSpdRead (Address, &Buffer[SPD_DDR3_SDRAM_TYPE_OFFSET], 2, 1, &Page);
  if (RETURN_SUCCESS == Status) {
    switch (Buffer[SPD_DDR3_SDRAM_TYPE_OFFSET]) {
      case SPD_DDR3_SDRAM_TYPE_NUMBER:
      case SPD_LPDDR3_SDRAM_TYPE_NUMBER:
      default:
        TableSelect = (SPD_OFFSET_TABLE *) SpdDdr3Table;
        Stop = (SpdDdr3TableSize / sizeof (SPD_OFFSET_TABLE));
        break;
      case SPD_DDR4_SDRAM_TYPE_NUMBER:
        TableSelect = (SPD_OFFSET_TABLE *) SpdDdr4Table;
        Stop = (SpdDdr4TableSize / sizeof (SPD_OFFSET_TABLE));
        break;
      case SPD_JEDEC_LPDDR3_SDRAM_TYPE_NUMBER:
        TableSelect = (SPD_OFFSET_TABLE *) SpdLpddrTable;
        Stop = (SpdLpddrTableSize / sizeof (SPD_OFFSET_TABLE));
        break;
    }
    for (Byte = 0; (RETURN_SUCCESS == Status) && (Byte < Stop); Byte++) {
      Tbl = &TableSelect[Byte];
      if ((1 << BootMode) & Tbl->BootMode) {
        Status = DoSpdRead (Address, &Buffer[Tbl->Start], Tbl->Start, Tbl->End - Tbl->Start + 1, &Page);
        if (Status == RETURN_SUCCESS) {
          if (SpdCold == BootMode) {
            if (FALSE == VerifyXmp ((MrcSpd *) Buffer, Tbl->Start)) {
              break;
            }
          }
        } else {
          break;
        }
      }
    }
  }

  return ((RETURN_SUCCESS == Status) ? TRUE : FALSE);
}

//
// This is from MdeModulePkg\Include\Guid\StatusCodeDataTypeDebug.h
// Might need to be adjusted for a particular BIOS core
//
#ifndef EFI_STATUS_CODE_DATA_MAX_SIZE
#define EFI_STATUS_CODE_DATA_MAX_SIZE 200
#endif

/**
  Output a string to the debug stream/device.
  If there is a '%' sign in the string, convert it to '%%', so that DEBUG() macro will print it properly.

  @param[in] String     - The string to output.

  @retval Nothing.
**/
VOID
SaDebugPrint (
  VOID   *String
  )
{
  CHAR8   Str[EFI_STATUS_CODE_DATA_MAX_SIZE];
  CHAR8   *InputStr;
  CHAR8   *OutputStr;
  UINT32  i;

  InputStr = (CHAR8 *) String;
  OutputStr = Str;
  i = 0;
  while (*InputStr != 0) {
    if (i < (EFI_STATUS_CODE_DATA_MAX_SIZE - 2)) {
      *OutputStr++ = *InputStr;
      i++;
      if (*InputStr++ == '%') {
        *OutputStr++ = '%';
        i++;
      }
    }
  }
  *OutputStr = 0;  // Terminating NULL
  DEBUG ((DEBUG_ERROR, Str));
  return;
}

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
  )
{
  return (
    ((UINT32) ((Bus)      & 0xFF) << 16) |
    ((UINT32) ((Device)   & 0x1F) << 11) |
    ((UINT32) ((Function) & 0x07) << 8)  |
    ((UINT32) ((Offset)   & 0xFF) << 0)  |
    (1UL << 31));
}

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
  )
{
  return (
    ((UINT32) Bus << 20) +
    ((UINT32) Device << 15) +
    ((UINT32) Function << 12) +
    ((UINT32) Offset << 0));
}

/**
  Read specific RTC/CMOS RAM

  @param[in] Location        Point to RTC/CMOS RAM offset for read

  @retval The data of specific location in RTC/CMOS RAM.
**/
UINT8
PeiRtcRead (
  IN const UINT8 Location
  )
{
  UINT8  RtcIndexPort;
  UINT8  RtcDataPort;

  //
  // CMOS access registers (using alternative access not to handle NMI bit)
  //
  if (Location < RTC_BANK_SIZE) {
    //
    // First bank
    //
    RtcIndexPort  = R_PCH_RTC_INDEX_ALT;
    RtcDataPort   = R_PCH_RTC_TARGET_ALT;
  } else {
    //
    // Second bank
    //
    RtcIndexPort  = R_PCH_RTC_EXT_INDEX_ALT;
    RtcDataPort   = R_PCH_RTC_EXT_TARGET_ALT;
  }

  IoWrite8 (RtcIndexPort, Location & RTC_INDEX_MASK);
  return IoRead8 (RtcDataPort);
}

/**
  Returns the current time, as determined by reading the Real Time Clock (RTC) on the platform.
  Since RTC time is stored in BCD, convert each value to binary.

  @param[out] Seconds       - The current second (0-59).
  @param[out] Minutes       - The current minute (0-59).
  @param[out] Hours         - The current hour (0-23).
  @param[out] DayOfMonth    - The current day of the month (1-31).
  @param[out] Month         - The current month (1-12).
  @param[out] Year          - The current year (2000-2099).

  @retval Nothing.
**/
VOID
GetRtcTime (
  OUT UINT8  *const Seconds,
  OUT UINT8  *const Minutes,
  OUT UINT8  *const Hours,
  OUT UINT8  *const DayOfMonth,
  OUT UINT8  *const Month,
  OUT UINT16 *const Year
  )
{
  UINT32 Timeout;

  //
  // Wait until RTC "update in progress" bit goes low.
  //
  Timeout = 0x0FFFFF;
  do {
    IoWrite8 (RTC_INDEX_REGISTER, CMOS_REGA);
    if ((IoRead8 (RTC_TARGET_REGISTER) & RTC_UPDATE_IN_PROGRESS) != RTC_UPDATE_IN_PROGRESS) {
      break;
    }
  } while (--Timeout > 0);

  if (0 == Timeout) {
    IoWrite8 (RTC_INDEX_REGISTER, CMOS_REGB);
    IoWrite8 (RTC_TARGET_REGISTER, RTC_HOLD | RTC_MODE_24HOUR);

    IoWrite8 (RTC_INDEX_REGISTER, CMOS_REGA);
    IoWrite8 (RTC_TARGET_REGISTER, RTC_CLOCK_DIVIDER | RTC_RATE_SELECT);

    IoWrite8 (RTC_INDEX_REGISTER, CMOS_REGC);
    IoRead8 (RTC_TARGET_REGISTER);

    IoWrite8 (RTC_INDEX_REGISTER, CMOS_REGD);
    IoRead8 (RTC_TARGET_REGISTER);

    IoWrite8 (RTC_INDEX_REGISTER, CMOS_REGB);
    IoWrite8 (RTC_TARGET_REGISTER, RTC_MODE_24HOUR);
  }
  //
  // Read seconds
  //
  IoWrite8 (RTC_INDEX_REGISTER, RTC_SECONDS);
  *Seconds = IoRead8 (RTC_TARGET_REGISTER);

  //
  // Read minutes
  //
  IoWrite8 (RTC_INDEX_REGISTER, RTC_MINUTES);
  *Minutes = IoRead8 (RTC_TARGET_REGISTER);

  //
  // Read hours
  //
  IoWrite8 (RTC_INDEX_REGISTER, RTC_HOURS);
  *Hours = IoRead8 (RTC_TARGET_REGISTER);

  //
  // Read day of month
  //
  IoWrite8 (RTC_INDEX_REGISTER, RTC_DAY_OF_MONTH);
  *DayOfMonth = IoRead8 (RTC_TARGET_REGISTER);

  //
  // Read month
  //
  IoWrite8 (RTC_INDEX_REGISTER, RTC_MONTH);
  *Month = IoRead8 (RTC_TARGET_REGISTER);

  //
  // Read year and add current century.
  //
  IoWrite8 (RTC_INDEX_REGISTER, RTC_YEAR);
  *Year = IoRead8 (RTC_TARGET_REGISTER);

  *Seconds    = BCD2BINARY (*Seconds);
  *Minutes    = BCD2BINARY (*Minutes);
  *Hours      = BCD2BINARY (*Hours);
  *DayOfMonth = BCD2BINARY (*DayOfMonth);
  *Month      = BCD2BINARY (*Month);
  *Year       = BCD2BINARY (*Year) + CENTURY_OFFSET;
}

/**
  Gets CPU current time.

  @param[in] GlobalData - Pointer to global MRC data struct.

  @retval The current CPU time in milliseconds.
**/
UINT64
GetCpuTime (
  IN VOID         *GlobalData
  )
{
  MrcParameters               *MrcData;
  MrcInput                    *Inputs;
  PCU_CR_PLATFORM_INFO_STRUCT Msr;
  UINT32                      TimeBase;

  MrcData   = (MrcParameters *) GlobalData;
  Inputs    = &MrcData->Inputs;

  Msr.Data = AsmReadMsr64 (PCU_CR_PLATFORM_INFO);
  TimeBase = (Inputs->BClkFrequency / 1000) * Msr.Bits.MAX_NON_TURBO_LIM_RATIO; //In Millisec
  return ((TimeBase == 0) ? 0 : DivU64x32 (AsmReadTsc (), TimeBase));
}

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
  )
{
  UINT16 *Buffer;

  Buffer = (UINT16 *) Dest;
  while (0 != NumWords--) {
    *Buffer++ = Value;
  }

  return (Dest);
}

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
  )
{
  UINT32 *Buffer;

  Buffer = (UINT32 *) Dest;
  while (0 != NumDwords--) {
    *Buffer++ = Value;
  }

  return (Dest);
}


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
  )
{
  MrcParameters   *MrcData;
  UINT32          CurrentVoltage;

  MrcData = (MrcParameters *) GlobalData;
  CurrentVoltage = DefaultVdd;

  return CurrentVoltage;
}

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
  )
{
  MrcParameters   *MrcData;

  MrcData = (MrcParameters *) GlobalData;

  return Voltage;
}

/**
  This function is used by the OEM to do a dedicated task during the MRC.

  @param[in] GlobalData        - include all the MRC data
  @param[in] OemStatusCommand  - A command that indicates the task to perform.
  @param[in] Pointer           - general ptr for general use.

  @retval The status of the task.
**/
MrcStatus
CheckPoint (
  IN VOID                *GlobalData,
  IN MrcOemStatusCommand OemStatusCommand,
  IN VOID                *Pointer
  )
{
  const SA_MEMORY_FUNCTIONS    *SaCall;
  MrcParameters                *MrcData;
  MrcInput                     *Inputs;
  MrcStatus                    Status;
  SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi;
  MEMORY_CONFIG_NO_CRC         *MemConfigNoCrc;
  EFI_STATUS                   Status1;

  //
  // Locate SiPreMemPolicyPpi to do a GetConfigBlock() to access platform data
  //
  Status1 = PeiServicesLocatePpi (
              &gSiPreMemPolicyPpiGuid,
              0,
              NULL,
              (VOID **) &SiPreMemPolicyPpi
              );
  ASSERT_EFI_ERROR (Status1);

  Status1 = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemoryConfigNoCrcGuid, (VOID *) &MemConfigNoCrc);
  ASSERT_EFI_ERROR (Status1);
  MrcData             = (MrcParameters *) GlobalData;
  Inputs              = &MrcData->Inputs;
  SiPreMemPolicyPpi = (SI_PREMEM_POLICY_PPI *) Inputs->SiPreMemPolicyPpi;
  SaCall              = &MemConfigNoCrc->MrcCall;
  Status              = mrcSuccess;

  switch (OemStatusCommand) {
    case OemAfterNormalMode:
      SaCall->MrcThermalOverrides (MrcData);
      break;

    default:
      break;
  }

  return (mrcSuccess);
}

/**
  Typically used to display to the I/O port 80h.

  @param[in] GlobalData         - Mrc Global Data
  @param[in] DisplayDebugNumber - the number to display on port 80.

  @retval Nothing.
**/
VOID
DebugHook (
  IN VOID       *GlobalData,
  UINT16        DisplayDebugNumber
  )
{
  MrcParameters                *MrcData;
  MrcOutput                    *Outputs;
  MrcDebug                     *Debug;
  EFI_STATUS                   Status;
  SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi;
  SA_MISC_PEI_PREMEM_CONFIG    *MiscPeiPreMemConfig;

  MrcData = (MrcParameters *) GlobalData;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  Debug->PostCode[MRC_POST_CODE] = DisplayDebugNumber;
  IoWrite16 (0x80, DisplayDebugNumber);
  DEBUG ((DEBUG_INFO, "Post Code: %04Xh\n", DisplayDebugNumber));

  //
  // Locate SiPreMemPolicyPpi to do a GetConfigBlock() to access platform data
  //
  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  if (Status == EFI_SUCCESS) {
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSaMiscPeiPreMemConfigGuid, (VOID *) &MiscPeiPreMemConfig);
  }
  return;
}

/**
  Hook to take any action after returning from MrcStartMemoryConfiguration()
  and prior to taking any action regarding MrcStatus.  Pre-populated with issuing
  Intel Silicon View Technology (ISVT) checkpoint 0x10.

  @param[in] GlobalData         - Mrc Global Data
  @param[in] MrcStatus          - Mrc status variable
**/
void
ReturnFromSmc (
  IN VOID         *GlobalData,
  IN UINT32       MrcStatus
  )
{
  return;
}

/**
  Assert or deassert DRAM_RESET# pin; this is used in JEDEC Reset.

  @param[in] PciEBaseAddress  - PCI express base address.
  @param[in] ResetValue       - desired value of DRAM_RESET#. 1 - reset deasserted, 0 - reset asserted.
**/
VOID
SaDramReset (
  IN UINT32 PciEBaseAddress,
  IN UINT32 ResetValue
  )
{
  UINT32  MmioBase;
  UINT32  PmCfg2;

  //
  // Get the PCH PWRM Base
  //
  PchPwrmBaseGet (&MmioBase);

  //
  // Set DRAM RESET# value via PCH register (both SKL PCH-H and SKL PCH-LP)
  //
  PmCfg2 = MmioRead32 (MmioBase + R_PCH_PWRM_CFG2);
  PmCfg2 &= ~(B_PCH_PWRM_CFG2_DRAM_RESET_CTL);
  PmCfg2 |= (ResetValue << 26);
  MmioWrite32 (MmioBase + R_PCH_PWRM_CFG2, PmCfg2);

  return;
}

