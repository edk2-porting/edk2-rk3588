/* $NoKeywords */
/**
 * @file
 *
 * Iscp.h
 *
 * Contains common ISCP-related structures and defines.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: FDK
 * @e sub-project: UEFI
 * @e version: $Revision: 338015 $ @e date: $Date: 2016-04-04 10:40:16 -0500 (Mon, 04 Apr 2016) $
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

#ifndef ISCP_H_
#define ISCP_H_

#ifdef __cplusplus
  extern "C" {
  #endif

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
  #include "SocConfiguration.h"
  #include "IscpConfig.h"
  #include "CoreState.h"
  #include "MemSetup.h"
  #include "MemIscp.h"
  #include "UartLineSettings.h"
  #include "CpuIscp.h"
  #include "NetworkAddress.h"

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

// *** NOTE: This controls the size of a queue in SRAM. This is the
// maximum number of elements that will fit, without changing the
// overall SRAM layout.
#define ISCP_ECC_EVENT_QUEUE_SIZE    8

  /// Types of ECC errors
  typedef enum _ECC_FAIL_TYPE {
    ECC_FAIL_NO_ERROR = 0,            ///< ECC No Error
    ECC_FAIL_CORRECTABLE,             ///< ECC Multiple Correctable Error
    ECC_FAIL_CORRECTABLE_MULTIPLE,    ///< ECC Correctable Multiple Error
    ECC_FAIL_UNCORRECTABLE,           ///< ECC Correctable Error
    ECC_FAIL_UNCORRECTABLE_MULTIPLE,  ///< ECC Uncorrectable Multiple Error
    ECC_FAIL_PARITY,                  ///< ECC Parity Error
    ECC_FAIL_END                      ///< End of ECC Fail Types
  } ECC_FAIL_TYPE;

  /// ISCP ECC error events
  typedef struct _ISCP_ECC_EVENT_DETAILS {
    UINT64 Address;            ///< Address
    UINT64 PhysicalAddress;    ///< DRAM Physical Address
    UINT64 Data;               ///< Data
    UINT32 Channel;            ///< DRAM Channel
    UINT32 SourceId;           ///< Scource ID
    UINT32 Syndrome;           ///< ECC Syndrome
    UINT32 Type;               ///< Restricted to ECC_FAIL_TYPE values
    UINT32 Module;             ///< DRAM Module
    UINT32 Bank;               ///< DRAM Bank
    UINT32 Row;                ///< DRAM Row
    UINT32 Column;             ///< DRAM Column
  } ISCP_ECC_EVENT_DETAILS;

  /// ISCP Block Transfer Memory Buffer
  typedef struct {
    UINT64  BuffAddress;                      ///< 64-Bit Communication Buffer Address
    UINT64  BufferSize;                       ///< 64-Bit Communication Buffer Size
  } BLOCK_TRANSFER_BUFFER;

  /// ISCP Data Window
  typedef struct {
    union {
      UINT8   szData[248];                       ///< 8-bit ISCP data array
      BLOCK_TRANSFER_BUFFER BlockTransferBuffer; ///< ISCP Memory block Transfer Buffer structure
    } Data;
  } DATA_WINDOW;

  /// ISCP Communication Block. This structure must fit within the 4K SRAM area.
  typedef struct {
    UINT32  Signature;                        ///< Command Signature
    UINT8   BlockLength;                      ///< Block Length of the entire message
    UINT8   RequestCode;                      ///< Request Code - Operation Requested by the recipient
    UINT8   ResponseCode;                     ///< Response Code - Response Code from recipient
    UINT8   DataLength;                       ///< Data Length - Length in bytes of data
                                              ///< being transmitted, zero if MEMORY_BUFFER is used
    DATA_WINDOW   DataWin;                    ///< Data Window Union (This completes the 256 byte header)
    UINT8   ExtraPayload[3072];               ///< Reserved for large payloads (A maximum of 3K)
    ISCP_ECC_EVENT_DETAILS FatalEccEvent;     ///< Only one fatal ECC error event needed (56 bytes)
    ISCP_ECC_EVENT_DETAILS EccEventList[ISCP_ECC_EVENT_QUEUE_SIZE];   ///< List of ECC error events (448 bytes, which nearly finishes the 4K area)
    UINT8   HeadIndex;                        ///< Index of first ECC event, when head == tail queue is empty
    UINT8   TailIndex;                        ///< Index of empty queue entry, to be filled next.
    UINT8   Overflow;                         ///< Indicates a queue overflow, saturates at 0xFF
  } ISCP_COMM_BLOCK __attribute__ ((__aligned__ (64)));

  /// Memory info HOB structure
  typedef struct {
    UINT32  Version;                          ///< Version of HOB structure
    UINT32  NumberOfDescriptor;               ///< Number of memory range descriptor
    AMD_MEMORY_RANGE_DESCRIPTOR  Ranges;      ///< Memory ranges
  } ISCP_MEMORY_INFO;

  /// SMBIOS Memory Buffer structure
  typedef struct {
    ISCP_TYPE16_SMBIOS_INFO      T16;         ///< SMBIOS Type 16 Record Data
    ISCP_TYPE17_SMBIOS_INFO      T17[2][2];   ///< SMBIOS Type 17 Record Data
    ISCP_TYPE19_SMBIOS_INFO      T19;         ///< SMBIOS Type 19 Record Data
  } AMD_SMBIOS_MEM_BUFFER;

  /// SMBIOS CPU Buffer structure
  typedef struct {
    ISCP_TYPE4_SMBIOS_INFO       T4[1];       ///< SMBIOS Type 4 Record Data
    ISCP_TYPE7_SMBIOS_INFO       T7L1[1];     ///< SMBIOS Type 7 Level 1 Cache Record Data
    ISCP_TYPE7_SMBIOS_INFO       T7L2[1];     ///< SMBIOS Type 7 Level 2 Cache Record Data
    ISCP_TYPE7_SMBIOS_INFO       T7L3[1];     ///< SMBIOS Type 7 Level 3 Cache Record Data
  } AMD_SMBIOS_CPU_BUFFER;

  /// SMBIOS Buffer structure
  typedef struct {
    AMD_SMBIOS_MEM_BUFFER        SmbiosMemBuffer;  ///< SMBIOS Memory Buffer
    AMD_SMBIOS_CPU_BUFFER        SmbiosCpuBuffer;  ///< SMBIOS CPU Buffer
  } ISCP_SMBIOS_INFO;

  /// NV Data structure
  typedef struct {
    UINT32  Version;                          ///< Version of NV data structure
    UINT32  FvOffset;                         ///< Offset from the base of the UEFI image
    UINT32  FvSize;                           ///< Firmware Volume Data Size to be written, read, or erased
    UINT8   FvData[64*1024];                  ///< Firmware Volume Data block
  } ISCP_OEM_NV_INFO;

  /// Firmware Fuse Buffer structure
  typedef struct {
    UINT32              Version;              ///< Version of Fuse Info Buffer structure
    SocConfiguration    SocConfiguration;     ///< Fuse Structure to be passed to UEFI
  } ISCP_FUSE_INFO;

  /// Firmware CPU Reset Buffer structure
  typedef struct {
    UINT32            Version;                ///< Version of CPU reset Buffer structure
    UINT32            CoreNum;                ///< The core number we want data for, e.g. 0,1,2,..
    SocCoreStatus     CoreStatus;             ///< Core Status Structure
  } ISCP_CPU_RESET_INFO;

  /// Firmware MAC Address structure
  typedef struct {
    UINT32    Version;                        ///< Version of MAC address Info Buffer structure
    UINT8     MacAddress0[6];                 ///< MAC Address 0 10Gb Ethernet port 0
    UINT8     MacAddress1[6];                 ///< MAC Address 1 10Gb Ethernet port 1
    UINT8     MacAddress2[6];                 ///< MAC Address 2 1Gb Ethernet
  } ISCP_MAC_INFO;

  /// ISCP RTC Time structure (Based on subset of EFI_TIME structure)
  typedef struct {
    UINT32      Version;                      ///< Version of RTC Info Buffer structure
    UINT16      Year;                         ///< Year: 2000 - 20XX
    UINT8       Month;                        ///< Month:  1 - 12
    UINT8       Day;                          ///< Day:  1 - 31
    UINT8       Hour;                         ///< Hour: 0 - 23
    UINT8       Minute;                       ///< Minute: 0 - 59
    UINT8       Second;                       ///< Second: 0 - 59
    UINT8       Pad;                          ///< Padding to made structure 32-bit aligned
  } ISCP_RTC_INFO;

  /// ISCP PCIE Reset structure
  typedef struct {
    UINT32      Version;                      ///< Version of PCIE reset Buffer structure
    UINT8       ResetSeq;                     ///< Sequence of Reset
    UINT16      SVID;                         ///< VRM value / Voltage
  } ISCP_PCIE_RESET_INFO;

  /// ISCP Ready To Boot structure
  typedef struct {
    UINT32         Version;                   ///< Version of Ready To Boot
    UINT8          ReadyToBoot;               ///< Signal Ready To Boot Event
  } ISCP_READY_TO_BOOT_INFO;

  /// ISCP BMC IP Address structure
  typedef struct {
    UINT32         Version;                   ///< Version of BMC IP Address
    ISCP_BMC_IPV4_ADDRESS  Ipv4Address;       ///< BMC IPv4 Address Structure
    ISCP_BMC_IPV6_ADDRESS  Ipv6Address;       ///< BMC IPv6 Address Structure
  } ISCP_BMC_IP_ADDRESS_INFO;

  /// EEPROM info structure
  typedef struct {
    UINT32  Version;                          ///< Version of EEPROM Info structure
    UINT32  EepromOffset;                     ///< EEPROM Offset from the base of the UEFI image
    UINT32  EepromSize;                       ///< EEPROM Data Size to be written, read, or erased
    UINT32  EepromArea;                       ///< EEPROM Area to be affected by read, write,erase commands
    UINT8   EepromData[64*1024];              ///< EEPROm Data block [64K]
  } ISCP_EEPROM_INFO;

  /// UART info structure. The legal values for these fields are in UartLineSettings.h and are
  /// shared between the SCP and UEFI.
  typedef struct {
    UINT32  Version;                          ///< Version of UART Info structure
    UART_LINE_SETTINGS  A57UartConfig;        ///< A57 UART Config
  } ISCP_UART_INFO;

  /// Override Command structure
  typedef struct {
    UINT32         Version;                   ///< Version of Override Command structure
    UINT8          Command;                   ///< Override command
  } ISCP_OVERRIDE_CMD_INFO;

  /// SATA1 reset structure
  typedef struct {
    UINT32         Version;                   ///< Version of SATA en/disable structure
    UINT8          State;                     ///< Enable/Disable state
  } ISCP_SATA1_RESET_INFO;

  /// BMC presence structure
  typedef struct {
    UINT32         Version;                   ///< Version of BMC presence structure
    UINT8          BmcPresent;                ///< BMC presence
  } ISCP_BMC_PRESENCE_INFO;

  /// BERT Region structure
  typedef struct {
    UINT32 IscpVersion;                       ///< Version of BERT Region structure
    UINT64 RegionPhysAddr;                    ///< ACPI v6.0: Table 18-319 [Boot Error Region]
    UINT32 RegionLength;                      ///< ACPI v6.0: Table 18-319 [Boot Error Region Length]
  } ISCP_BERT_REGION_INFO;

  /// SCP Doorbell Record structure
  typedef struct {
        UINT32 IscpVersion;                       ///< Version of Doorbell Info structure
        UINT32 ToggleRateMilliSec;                ///< Doorbell Toggle Rate
  } ISCP_SCP_DOORBELL_INFO;

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define ISCP_TIMEOUT                (1000000)

// Request Codes
#define ISCP_TRANSACTION_SUCCESS    (0x00)

#define ISCP_REQ_MEMORY             (0x03)
#define ISCP_RETRIEVE_SETUP         (0x04)
#define ISCP_STORE_SETUP            (0x05)
#define ISCP_FUSE_BLOB              (0x07)
#define ISCP_CPU_RETRIEVE_ID        (0x09)
#define ISCP_CPU_RESET              (0x0A)
#define ISCP_REQ_OEM_NV             (0x0B)
#define ISCP_STORE_OEM_NV           (0x0C)
#define ISCP_ERASE_OEM_NV           (0x0D)
#define ISCP_GET_MAC_ADDRESS        (0x0E)
#define ISCP_SET_MAC_ADDRESS        (0x0F)
#define ISCP_REQ_RTC                (0x10)
#define ISCP_SET_RTC                (0x11)
#define ISCP_GET_SMBIOS             (0x12)
#define ISCP_RESET_PCIE             (0x13)
#define ISCP_READY_TO_BOOT          (0x14)
#define ISCP_GET_BMC_IP             (0x15)
#define ISCP_RETRIEVE_VERSION       (0x16)
#define ISCP_STORE_EEPROM           (0x17)
#define ISCP_REQ_EEPROM             (0x18)
#define ISCP_ERASE_EEPROM           (0x19)
#define ISCP_MEM_SETUP              (0x1A)
#define ISCP_SEND_UART_CONFIG       (0x1C)
#define ISCP_OVERRIDE_CMD           (0x1D)
#define ISCP_SATA1_GET              (0x1E)
#define ISCP_SATA1_SET              (0x1F)
#define ISCP_BMC_PRESENT            (0x20)
#define ISCP_RETRIEVE_BERT_RECORD   (0x21)
#define ISCP_SUBMIT_BERT_RECORD     (0x22)
#define ISCP_POWER_OFF              (0xAA)
#define ISCP_SYSTEM_RESET           (0xBB)

// Response Codes
#define ISCP_TRANSACTION_SUCCESS    (0x00)
#define ISCP_UNSUCCESSFUL           (0x01)
#define ISCP_INVALID                (0x02)
#define ISCP_SIGNATURE_NOT_FOUND    (0x03)
#define ISCP_NOT_SUPPORTED          (0x04)
#define ISCP_INVALID_BLOCK_LENGTH   (0x05)
#define ISCP_INVALID_REQUEST_CODE   (0x06)
#define ISCP_INVALID_DATA_LENGTH    (0x07)
#define ISCP_NV_WRITE_FAIL          (0x0A)
#define ISCP_NV_READ_FAIL           (0x0B)
#define ISCP_NV_ERASE_FAIL          (0x0C)
#define ISCP_SETUP_READ_FAIL        (0x0D)
#define ISCP_SETUP_WRITE_FAIL       (0x0E)
#define ISCP_EE_WRITE_FAIL          (0x0F)
#define ISCP_EE_READ_FAIL           (0x10)
#define ISCP_EE_ERASE_FAIL          (0x11)
#define ISCP_SMBIOS_FAIL            (0x12)
#define ISCP_INVALID_RESPONSE_CODE  (0xFF)

// ISCP Signatures
#define BOOT_CORE_SIG               (0x524F4342)   //"BCOR" spelled backwards - Boot Core
#define BERT_SIG                    (0x54524542)   //"BERT" spelled backwards - BERT Error Block Buffer Address
#define BMC_PRESENT_SIG             (0x50434D42)   //"BMCP" spelled backwards - BMC Present
#define BMC_IP_ADDR_SIG             (0x50494D42)   //"BMIP" spelled backwards - BMC IP Address
#define CPU_MP_SIG                  (0x4D555043)   //"CPUM" spelled backwards - CPU Reset
#define DOORBELL_SIG                (0x4C454244)   //"DBEL" spelled backwards - Doorbell
#define EEPROM_SIG                  (0x52504545)   //"EEPR" spelled backwards - EEPROM
#define FUSE_BLOB_SIG               (0x45535546)   //"FUSE" spelled backwards - Fuse blob
#define HOBS_SIG                    (0x53424F48)   //"HOBS" spelled backwards - Memory HOBs buffer
#define GET_MAC_ADDR_SIG            (0x4143414D)   //"MACA" spelled backwards - Get MAC Address
#define OEM_NV_SIG                  (0x564E454F)   //"OENV" spelled backwards - OEM NV Storage save and retrieval actions
#define OVERRIDE_CMD_SIG            (0x4452564F)   //"OVRD" spelled backwards - Override Command
#define PCIE_SIG                    (0x45494350)   //"PCIE" spelled backwards - PCIE Reset
#define READY2BOOT_SIG              (0x54425452)   //"RTBT" spelled backwards - Ready-To-Boot
#define RTC_SIG                     (0x4B435452)   //"RTCK" spelled backwards - Real-Time-Clock
#define SATA1_GET_SIG               (0x47544153)   //"SATG" spelled backwards - SATA 1 get state
#define SATA1_SET_SIG               (0x53544153)   //"SATS" spelled backwards - SATA 1 set state
#define SETUP_SIG                   (0x55544553)   //"SETU" spelled backwards - BIOS Setup
#define SHUTDOWN_SIG                (0x4E444853)   //"SHDN" spelled backwards - System Shutdown
#define SET_MAC_ADDR_SIG            (0x43414D53)   //"SMAC" spelled backwards - Set MAC Address
#define SMBIOS_SIG                  (0x534D4253)   //"SMBS" spelled backwards - SMBIOS
#define UART_SIG                    (0x54524155)   //"UART" spelled backwards - UART Config


#define ISCP_BERT_REGION_INFO_VERSION               (0x00000001ul)    ///< Ver: 00.00.00.01

#ifdef THESE_SHOULD_BE_USED_ON_BOTH_SIDES
#define ISCP_BMC_PRESENT_INFO_VERSION               (0x00000001ul)    ///< Ver: 00.00.00.01
#endif

#define ISCP_BMC_IP_ADDR_INFO_VERSION               (0x00000001ul)    ///< Ver: 00.00.00.01
#define ISCP_CPU_RESET_INFO_VERSION                 (0x00000001ul)    ///< Ver: 00.00.00.01

#ifdef THESE_SHOULD_BE_USED_ON_BOTH_SIDES
#define ISCP_DOORBELL_INFO_VERSION                  (0x00000001ul)    ///< Ver: 00.00.00.01
#endif

#define ISCP_EEPROM_VERSION                         (0x00000001ul)    ///< Ver: 00.00.00.01
#define ISCP_FUSE_INFO_VERSION                      (0x00000001ul)    ///< Ver: 00.00.00.01
#define ISCP_MEMORY_INFO_VERSION                    (0x00000001ul)    ///< Ver: 00.00.00.01
#define ISCP_MAC_INFO_VERSION                       (0x00000002ul)    ///< Ver: 00.00.00.02
#define ISCP_OEM_NV_VERSION                         (0x00000001ul)    ///< Ver: 00.00.00.01

#ifdef THESE_SHOULD_BE_USED_ON_BOTH_SIDES
#define ISCP_OVERRIDE_CMD_INFO_VERSION              (0x00000001ul)    ///< Ver: 00.00.00.01
#endif

#define ISCP_PCIE_RESET_INFO_VERSION                (0x00000001ul)    ///< Ver: 00.00.00.01

#ifdef THESE_SHOULD_BE_USED_ON_BOTH_SIDES
#define ISCP_READY2BOOT_INFO_VERSION                (0x00000001ul)    ///< Ver: 00.00.00.01
#endif

#define ISCP_RTC_INFO_VERSION                       (0x00000001ul)    ///< Ver: 00.00.00.01

#ifdef THESE_SHOULD_BE_USED_ON_BOTH_SIDES
#define ISCP_SATA1_RESET_INFO_VERSION               (0x00000001ul)    ///< Ver: 00.00.00.01
#endif

#define ISCP_UART_CONFIG_INFO_VERSION               (0x00000001ul)    ///< Ver: 00.00.00.01

#define ISCP_COMM_BLK_MAX_SIZE                      (0x100)           ///< Max length of ISCP communication block, 256 bytes
#define MAX_NUMBER_OF_EXTENDED_MEMORY_DESCRIPTOR    (2)
#define MAX_SIZEOF_AMD_MEMORY_INFO_HOB_BUFFER       (sizeof (ISCP_MEM_HOB) +  \
          (MAX_NUMBER_OF_EXTENDED_MEMORY_DESCRIPTOR * sizeof (AMD_MEMORY_RANGE_DESCRIPTOR)))
#define MAX_SIZEOF_AMD_SETUP_BUFFER                 (sizeof (ISCP_SETUP_INFO))
#define MAX_SIZEOF_AMD_SMBIOS_BUFFER                (sizeof (AMD_ISCP_SMBIOS_INFO))

#define FOREVER                         for (;;)
#define USE_DRAM_BUFFER                 (0x00)
#define ISCP_BLOCK_LENGTH               (0x08)

  #ifdef __cplusplus
  }
#endif

#endif /* ISCP_H_ */
