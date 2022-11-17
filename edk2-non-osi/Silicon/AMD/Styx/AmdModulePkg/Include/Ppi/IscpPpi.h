/* $NoKeywords */
/**
 * @file
 *
 * IscpPpi.h
 *
 * Contains Intra-SoC Communication Protocol-Protocol Interface definitions.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: FDK
 * @e sub-project: UEFI
 * @e version: $Revision: 334098 $ @e date: $Date: 2016-01-08 14:21:15 -0600 (Fri, 08 Jan 2016) $
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


#ifndef _PEI_ISCP_PPI_H_
#define _PEI_ISCP_PPI_H_

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Iscp.h>


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define PEI_ISCP_PPI_GUID {\
  0xca2c1ecd, 0xc702, 0x49b1, { 0xae, 0x24, 0x9b, 0x6f, 0xa8, 0x71, 0x3b, 0x23 } \
}


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
typedef struct _EFI_PEI_ISCP_PPI EFI_PEI_ISCP_PPI;


/// ISCP Memory Transaction
typedef
EFI_STATUS
(EFIAPI *PEI_ISCP_MEMORY_TRANSACTION)(
  IN     CONST EFI_PEI_SERVICES             **PeiServices,        ///< Pointer to the PEI Services Table
  IN OUT AMD_MEMORY_RANGE_DESCRIPTOR  *MemRangeDescriptor   ///< Pointer to Memory Range Descriptor
  );


/// ISCP Fuse Transaction
typedef
EFI_STATUS
(EFIAPI *PEI_ISCP_FUSE_TRANSACTION)(
  IN CONST EFI_PEI_SERVICES     **PeiServices,  ///< Pointer to the PEI Services Table
  IN OUT   ISCP_FUSE_INFO       *FuseInfo       ///< Pointer to the Fuse Info structure
  );


/// ISCP CPU Retrieve ID Transaction
typedef
EFI_STATUS
(EFIAPI *PEI_ISCP_CPU_RETRIEVE_ID_TRANSACTION)(
  IN CONST  EFI_PEI_SERVICES     **PeiServices,    ///< Pointer to the PEI Services Table
  IN OUT    ISCP_CPU_RESET_INFO  *CpuResetInfo     ///< Pointer to CPU Reset Info structure
  );


/// ISCP CPU Reset transaction
typedef
EFI_STATUS
(EFIAPI *PEI_ISCP_CPU_RESET_TRANSACTION)(
  IN CONST EFI_PEI_SERVICES     **PeiServices,    ///< Pointer to the PEI Services Table
  IN OUT   ISCP_CPU_RESET_INFO  *CpuResetInfo     ///< Pointer to CPU Reset Info structure
  );


/// ISCP Get Real-Time-Clock Transaction
typedef
EFI_STATUS
(EFIAPI *PEI_ISCP_GET_RTC_TRANSACTION)(
  IN CONST EFI_PEI_SERVICES           **PeiServices,   ///< Pointer to the PEI Services Table
  IN OUT   ISCP_RTC_INFO              *RtcInfo         ///< Pointer to the Real-Time-Clock
  );


/// ISCP Set Real-Time-Clock Transaction
typedef
EFI_STATUS
(EFIAPI *PEI_ISCP_SET_RTC_TRANSACTION)(
  IN CONST EFI_PEI_SERVICES           **PeiServices,  ///< Pointer to the PEI Services Table
  IN OUT   ISCP_RTC_INFO              *RtcInfo        ///< Pointer to the Real-Time-Clock
  );


/// ISCP Get MAC Address Transaction
typedef
EFI_STATUS
(EFIAPI *PEI_ISCP_GET_MAC_ADDRESS_TRANSACTION)(
  IN CONST EFI_PEI_SERVICES           **PeiServices,  ///< Pointer to the PEI Services Table
  IN OUT   ISCP_MAC_INFO              *MacInfo        ///< Pointer to the MAC Address info
  );


/// ISCP Set MAC Address Transaction
typedef
EFI_STATUS
(EFIAPI *PEI_ISCP_SET_MAC_ADDRESS_TRANSACTION)(
  IN CONST EFI_PEI_SERVICES           **PeiServices,  ///< Pointer to the PEI Services Table
  IN OUT   ISCP_MAC_INFO              *MacInfo        ///< Pointer to the MAC Address info
  );


/// ISCP Update Firmware Volume Block Transaction
typedef
EFI_STATUS
(EFIAPI *PEI_ISCP_UPDATE_FV_BLOCK_TRANSACTION)(
  IN CONST  EFI_PEI_SERVICES         **PeiServices,  ///< Pointer to the PEI Services Table
  IN CONST  UINT32                   Offset,         ///< Offset from base of FV Block
  IN OUT    UINT8                    *NvData,        ///< Pointer to the NV data being stored
  IN CONST  UINT32                   NvSize          ///< Size of NV Data being stored
  );


/// SCP Load Firmware Volume Block Transaction
typedef
EFI_STATUS
(EFIAPI *PEI_ISCP_LOAD_FV_BLOCK_TRANSACTION)(
  IN CONST  EFI_PEI_SERVICES         **PeiServices,   ///< Pointer to the PEI Services Table
  IN CONST  UINT32                   Offset,          ///< Offset from base of FV Block
  IN OUT    UINT8                    *NvData,         ///< Pointer to the NV data being stored
  IN CONST  UINT32                   NvSize           ///< Size of NV Data being stored
  );


/// ISCP Erase Firmware Volume Block Transaction
typedef
EFI_STATUS
(EFIAPI *PEI_ISCP_ERASE_FV_BLOCK_TRANSACTION)(
  IN CONST  EFI_PEI_SERVICES         **PeiServices,  ///< Pointer to the PEI Services Table
  IN CONST  UINT32                   Offset,         ///< Offset from base of FV Block
  IN CONST  UINT32                   NvSize          ///< Size of NV Data being stored
  );


/// ISCP PCIE Reset Transaction
typedef
EFI_STATUS
(EFIAPI *PEI_ISCP_PCIE_RESET_TRANSACTION)(
  IN CONST  EFI_PEI_SERVICES          **PeiServices,  ///< Pointer to the PEI Services Table
  IN OUT    ISCP_PCIE_RESET_INFO      *PcieResetInfo  ///< Pointer to PCIE Reset info structure
  );


/// ISCP Send UART Config Transaction
typedef
EFI_STATUS
(EFIAPI *PEI_ISCP_SEND_UART_CONFIG_TRANSACTION)(
  IN CONST  EFI_PEI_SERVICES          **PeiServices,  ///< Pointer to the PEI Services Table
  IN OUT    ISCP_UART_INFO            *UartInfo       ///< Pointer to UART Config info structure
  );

/// ISCP Sata1 get Transaction
typedef
EFI_STATUS
(EFIAPI *PEI_ISCP_SATA1_GET_TRANSACTION)(
  IN CONST  EFI_PEI_SERVICES          **PeiServices,  ///< Pointer to the PEI Services Table
  IN OUT    ISCP_SATA1_RESET_INFO     *Sata1ResetInfo ///< Pointer to SATA1 reset structure
  );

/// ISCP BMC Present
typedef
EFI_STATUS
(EFIAPI *PEI_ISCP_BMC_PRESENT_TRANSACTION)(
  IN CONST  EFI_PEI_SERVICES          **PeiServices,   ///< Pointer to the PEI Services Table
  OUT       ISCP_BMC_PRESENCE_INFO    *BmcPresenceInfo ///< Pointer to BMC presence structure
  );

/// This PPI provides several services in PEI to work with the underlying
/// Intra-SOC Communication Protocol capabilities of the platform.  These
/// services include the ability for PEI to send/receive Firmware Setup data,
/// retrieve memory data, retrieve fuse data, perform CPU core reset, e.g launch,
/// retrieve OEM NVRAM transactions.
struct _EFI_PEI_ISCP_PPI {
  PEI_ISCP_MEMORY_TRANSACTION           ExecuteMemoryTransaction;
  PEI_ISCP_FUSE_TRANSACTION             ExecuteFuseTransaction;
  PEI_ISCP_CPU_RETRIEVE_ID_TRANSACTION  ExecuteCpuRetrieveIdTransaction;
  PEI_ISCP_CPU_RESET_TRANSACTION        ExecuteCpuResetTransaction;
  PEI_ISCP_GET_RTC_TRANSACTION          ExecuteGetRtcTransaction;
  PEI_ISCP_SET_RTC_TRANSACTION          ExecuteSetRtcTransaction;
  PEI_ISCP_GET_MAC_ADDRESS_TRANSACTION  ExecuteGetMacAddressTransaction;
  PEI_ISCP_SET_MAC_ADDRESS_TRANSACTION  ExecuteSetMacAddressTransaction;
  PEI_ISCP_UPDATE_FV_BLOCK_TRANSACTION  ExecuteUpdateFvBlock;
  PEI_ISCP_LOAD_FV_BLOCK_TRANSACTION    ExecuteLoadNvBlock;
  PEI_ISCP_ERASE_FV_BLOCK_TRANSACTION   ExecuteEraseNvBlock;
  PEI_ISCP_PCIE_RESET_TRANSACTION       ExecutePcieResetTransaction;
  PEI_ISCP_SEND_UART_CONFIG_TRANSACTION ExecuteSendUartConfigTransaction;
  PEI_ISCP_SATA1_GET_TRANSACTION        ExecuteSata1GetTransaction;
  PEI_ISCP_BMC_PRESENT_TRANSACTION      ExecuteBmcPresentTransaction;
};

extern EFI_GUID gPeiIscpPpiGuid;

#endif // #ifndef _PEI_ISCP_PPI_H_
