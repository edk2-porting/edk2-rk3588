/* $NoKeywords */
/**
 * @file
 *
 * AmdIscpDxeProtocol.h
 *
 * Contains Intra-SoC Communication DXE Protocol definitions.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: FDK
 * @e sub-project: UEFI
 * @e version: $Revision: 337020 $ @e date: $Date: 2016-03-02 11:49:34 -0600 (Wed, 02 Mar 2016) $
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

#ifndef __AMD_ISCP_DXE_PROTOCOL__H_
#define __AMD_ISCP_DXE_PROTOCOL__H_

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Iscp.h>


/*----------------------------------------------------------------------------------------
 *                         G U I D    D E F I N I T I O N
 *----------------------------------------------------------------------------------------
 */
#define AMD_ISCP_DXE_PROTOCOL_GUID {\
  0x5c794c8, 0x6aef, 0x4450, 0x91, 0x78, 0xca, 0x70, 0x53, 0x75, 0xbd, 0x91 \
}

/*----------------------------------------------------------------------------------------
 *                                  E X T E R N S
 *----------------------------------------------------------------------------------------
 */
extern EFI_GUID gAmdIscpDxeProtocolGuid;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
typedef struct _AMD_ISCP_DXE_PROTOCOL AMD_ISCP_DXE_PROTOCOL;

  /// HEST Notification type
  typedef enum {
    HEST_NOTIFY_POLLED  = 0,                  ///< Polled
    HEST_NOTIFY_GPIO    = 7,                  ///< GPIO-Signal
  } HEST_NOTIFY_TYPE;

  /// Trusted Firmware Generic Error Source structure
  typedef struct {
    UINT32 IscpVersion;                       ///< Version of BERT Region structure
    UINT8  SourceGUID[16];                    ///< ACPI v6.0: Table 18-331 [Section Type]
    UINT64 ErrorStatusPhysAddr;               ///< ACPI v6.0: Table 18-329 [Error Status Address]
    UINT32 ErrorStatusLength;                 ///< ACPI v6.0: Table 18-329 [Error Status Block Length]
    HEST_NOTIFY_TYPE NotificationType;        ///< ACPI v6.0: Table 18-332 [Type]
  } ISCP_TFW_GENERIC_ERROR_SOURCE;

/// CPU Core Reset Prototype
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_CPU_CORE_RESET) (
  IN      AMD_ISCP_DXE_PROTOCOL           *This,          ///< Pointer to AMD_ISCP_DXE_PROTOCOL
  IN OUT  ISCP_CPU_RESET_INFO             *CpuResetInfo   ///< Pointer to CPU Reset Info structure
  );

typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_CPU_RETRIEVE_ID) (
  IN      AMD_ISCP_DXE_PROTOCOL           *This,          ///< Pointer to AMD_ISCP_DXE_PROTOCOL
  IN OUT  ISCP_CPU_RESET_INFO             *CpuResetInfo   ///< Pointer to CPU Reset Info structure
  );

/// ISCP call to get MAC Address
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_GET_MAC_ADDRESS) (
  IN      AMD_ISCP_DXE_PROTOCOL           *This,      ///< Pointer to AMD_ISCP_DXE_PROTOCOL
  IN OUT  ISCP_MAC_INFO                   *MacInfo    ///< Pointer to the Firmware MAC Address structure
  );


/// ISCP call to set MAC Address
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_SET_MAC_ADDRESS) (
  IN      AMD_ISCP_DXE_PROTOCOL           *This,      ///< Pointer to AMD_ISCP_DXE_PROTOCOL
  IN OUT  ISCP_MAC_INFO                   *MacInfo    ///< Pointer to the Firmware MAC Address structure
  );


/// ISCP call to get Real-Time-Clock
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_GET_RTC) (
  IN      AMD_ISCP_DXE_PROTOCOL           *This,    ///< Pointer to AMD_ISCP_DXE_PROTOCOL
  IN OUT  ISCP_RTC_INFO                   *RtcInfo  ///< Pointer to the Real-Time-Clock structure
  );


/// ISCP call to set Real-Time-Clock
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_SET_RTC) (
  IN      AMD_ISCP_DXE_PROTOCOL           *This,    ///< Pointer to AMD_ISCP_DXE_PROTOCOL
  IN OUT  ISCP_RTC_INFO                   *RtcInfo  ///< Pointer to the Real-Time-Clock structure
  );


/// Update Firmware Volume Block into SPI from local memory
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_UPDATE_FV_BLOCK_DXE) (
  IN        AMD_ISCP_DXE_PROTOCOL       *This,    ///< Pointer to AMD_ISCP_DXE_PROTOCOL
  IN CONST  UINT32                      Offset,   ///< Offset to base of Firmware Volume Block
  IN OUT    UINT8                       *NvData,  ///< Pointer to data being stored in FV Block
  IN CONST  UINT32                      NvSize    ///< Size of data being stored FV Block
  );


/// Load Firmware Volume Block from SPI into local memory
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_LOAD_FV_BLOCK_DXE) (
  IN        AMD_ISCP_DXE_PROTOCOL       *This,    ///< Pointer to AMD_ISCP_DXE_PROTOCOL
  IN CONST  UINT32                      Offset,   ///< Offset to base of Firmware Volume Block
  IN OUT    UINT8                       *NvData,  ///< Pointer to data being stored in FV Block
  IN CONST  UINT32                      NvSize    ///< Size of data being retrieved from FV Block
  );


/// Erase Firmware Volume Block Prototype
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_ERASE_FV_BLOCK_DXE) (
  IN        AMD_ISCP_DXE_PROTOCOL       *This,    ///< Pointer to AMD_ISCP_DXE_PROTOCOL
  IN CONST  UINT32                      Offset,   ///< Offset to base of Firmware Volume Block
  IN CONST  UINT32                      NvSize    ///< Size of data being erased
  );


/// Update EEPROM Block from local memory prototype
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_UPDATE_EEPROM_BLOCK_DXE) (
  IN        AMD_ISCP_DXE_PROTOCOL       *This,    ///< Pointer to AMD_ISCP_DXE_PROTOCOL
  IN CONST  UINT32                      Offset,   ///< Offset to base of EEPROM Block
  IN OUT    UINT8                       *Data,    ///< Pointer to data being stored in EEPROM Block
  IN CONST  UINT32                      Size      ///< Size of data being stored EEPROM Block
  );


/// Load EEPROM Block into local memory prototype
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_LOAD_EEPROM_BLOCK_DXE) (
  IN        AMD_ISCP_DXE_PROTOCOL       *This,    ///< Pointer to AMD_ISCP_DXE_PROTOCOL
  IN CONST  UINT32                      Offset,   ///< Offset to base of EEPROM Block
  IN OUT    UINT8                       *Data,    ///< Pointer to data being stored in EEPROM Block
  IN CONST  UINT32                      Size      ///< Size of data being retrieved from EEPROM Block
  );


/// Erase EEPROM Block prototype
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_ERASE_EEPROM_BLOCK_DXE) (
  IN        AMD_ISCP_DXE_PROTOCOL       *This,    ///< Pointer to AMD_ISCP_DXE_PROTOCOL
  IN CONST  UINT32                      Offset,   ///< Offset to base of EEPROM Block
  IN CONST  UINT32                      Size      ///< Size of data being erased
  );


/// Issue ISCP Doorbell command
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_GET_BMC_IP_ADDRESS) (
  IN      AMD_ISCP_DXE_PROTOCOL         *This,
  IN OUT  ISCP_BMC_IP_ADDRESS_INFO      *BmcIpAddressInfo
  );

/// Issue ISCP command to retrieve SMBIOS info
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_SMBIOS_INFO) (
  IN       AMD_ISCP_DXE_PROTOCOL           *This,
  IN OUT   ISCP_SMBIOS_INFO                *SmbiosInfo
  );

/// Issue ISCP command to issue SoC shutdown command
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_SOC_SHUTDOWN) (
  IN       AMD_ISCP_DXE_PROTOCOL           *This
  );

/// Issue ISCP command to issue SoC reset command
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_SOC_RESET) (
  IN       AMD_ISCP_DXE_PROTOCOL           *This
  );

/// ISCP call to set Memory Set up Nodes
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_MEM_SETUP) (
  IN      AMD_ISCP_DXE_PROTOCOL           *This,        ///< Pointer to AMD_ISCP_DXE_PROTOCOL
  IN OUT  MEM_SETUP_VAR                   *SetupInfo    ///< Pointer to the Firmware MAC Address structure
  );

/// Issue Override Command
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_OVERRIDE_CMD) (
  IN      AMD_ISCP_DXE_PROTOCOL           *This,                ///< Pointer to AMD_ISCP_DXE_PROTOCOL
  IN      ISCP_OVERRIDE_CMD_INFO          *OverrideCmdInfo      ///< Pointer to the Overrride Command structure
  );

/// Issue Sata1 get state
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_SATA1_GET) (
  IN      AMD_ISCP_DXE_PROTOCOL           *This,                ///< Pointer to AMD_ISCP_DXE_PROTOCOL
  IN OUT  ISCP_SATA1_RESET_INFO           *Sata1ResetInfo       ///< Pointer to the SATA1 reset structure
  );

/// Issue Sata1 set state
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_SATA1_SET) (
  IN      AMD_ISCP_DXE_PROTOCOL           *This,                ///< Pointer to AMD_ISCP_DXE_PROTOCOL
  IN      ISCP_SATA1_RESET_INFO           *Sata1ResetInfo       ///< Pointer to the SATA1 reset structure
  );

/// Issue BMC presence check
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_BMC_PRESENT) (
  IN       AMD_ISCP_DXE_PROTOCOL          *This,             ///< Pointer to AMD_ISCP_DXE_PROTOCOL
  OUT      ISCP_BMC_PRESENCE_INFO         *BmcPresenceInfo   ///< Pointer to BMC presence structure
  );

/// Register Boot Error Region
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_RETRIEVE_BERT_RECORD) (
  IN      AMD_ISCP_DXE_PROTOCOL           *This,
  IN OUT  ISCP_BERT_REGION_INFO           *BertRegionInfo
  );

/// Register generic hardware error soure
typedef
EFI_STATUS
(EFIAPI *AMD_EXECUTE_REGISTER_ERROR_SOURCE) (
  IN  AMD_ISCP_DXE_PROTOCOL               *This,
  IN  ISCP_TFW_GENERIC_ERROR_SOURCE       *GenericErrorSource
  );


/*----------------------------------------------------------------------------------------
 *                         P R O T O C O L    S T R U C T U R E
 *----------------------------------------------------------------------------------------
 */
/// ISCP DXE Protocol Structure
struct _AMD_ISCP_DXE_PROTOCOL {
  AMD_EXECUTE_CPU_CORE_RESET          AmdExecuteCpuCoreReset;         ///< Execute CPU Core Reset
  AMD_EXECUTE_CPU_RETRIEVE_ID         AmdExecuteCpuRetrieveId;        ///< Execute CPU Retrieve ID
  AMD_EXECUTE_GET_MAC_ADDRESS         AmdExecuteGetMacAddress;        ///< Execute Get MAC Address
  AMD_EXECUTE_SET_MAC_ADDRESS         AmdExecuteSetMacAddress;        ///< Execute Set MAC Address
  AMD_EXECUTE_GET_RTC                 AmdExecuteGetRtc;               ///< Execute Get Real-Time-Clock Time
  AMD_EXECUTE_SET_RTC                 AmdExecuteSetRtc;               ///< Execute Set Real-Time-Clock Time
  AMD_EXECUTE_UPDATE_FV_BLOCK_DXE     AmdExecuteUpdateFvBlockDxe;     ///< Execute Update FV Block Data on the SPI device
  AMD_EXECUTE_LOAD_FV_BLOCK_DXE       AmdExecuteLoadFvBlockDxe;       ///< Execute Load FV Block Data from the SPI device
  AMD_EXECUTE_ERASE_FV_BLOCK_DXE      AmdExecuteEraseFvBlockDxe;      ///< Execute Erase FV Block Data on the SPI device
  AMD_EXECUTE_UPDATE_EEPROM_BLOCK_DXE AmdExecuteUpdateEepromBlockDxe; ///< Execute Update EEPROM Data on the EEPROM device
  AMD_EXECUTE_LOAD_EEPROM_BLOCK_DXE   AmdExecuteLoadEepromBlockDxe;   ///< Execute Load EEPROM Data on the EEPROM device
  AMD_EXECUTE_ERASE_EEPROM_BLOCK_DXE  AmdExecuteEraseEepromBlockDxe;  ///< Execute Erase EEPROM Data on the EEPROM device
  AMD_EXECUTE_GET_BMC_IP_ADDRESS      AmdExecuteGetBmcIpAddress;      ///< Execute Get BMC IP Address
  AMD_EXECUTE_SMBIOS_INFO             AmdExecuteSmbiosInfoDxe;        ///< Execute SMBIOS info
  AMD_EXECUTE_SOC_SHUTDOWN            AmdExecuteSocShutdownDxe;       ///< Execute SoC Shutdown
  AMD_EXECUTE_SOC_RESET               AmdExecuteSocResetDxe;          ///< Execute SoC Reset
  AMD_EXECUTE_MEM_SETUP               AmdExecuteMemSetup;             ///< Execute Set MAC Address
  AMD_EXECUTE_OVERRIDE_CMD            AmdExecuteOverrideCmd;          ///< Execute Override Command
  AMD_EXECUTE_SATA1_GET               AmdExecuteSata1Get;             ///< Execute Sata1 get state
  AMD_EXECUTE_SATA1_SET               AmdExecuteSata1Set;             ///< Execute Sata1 set state
  AMD_EXECUTE_BMC_PRESENT             AmdExecuteBmcPresent;           ///< Execute BMC presence check
  AMD_EXECUTE_RETRIEVE_BERT_RECORD    AmdExecuteRetrieveBertRecord;   ///< Execute Retrieve Boot Error Record
  AMD_EXECUTE_REGISTER_ERROR_SOURCE   AmdExecuteRegisterErrorSource;  ///< Execute Register Generic Hardware Error Source
};

#endif //_AMD_ISCP_DXE_PROTOCOL_H_
