/* $NoKeywords */
/**
 * @file
 *
 * PostCode.h
 *
 * Contains Where's-The-Fimrware (WTF) POST code definitions.
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

#ifndef __POSTCODE__H_
#define __POSTCODE__H_

#ifdef __cplusplus
  extern "C" {
  #endif

// AMD UEFI WTF POST Codes
#define POST_ENTER_SEC_CORE             0x200       ///< Enter SEC Phase Core
#define POST_INIT_GIG_SEC               0x201       ///< Initialize GIC in Secure Mode
#define POST_EXIT_SEC_CORE              0x202       ///< Exit SEC Phase Core
#define POST_INIT_GIC_NON_SEC           0x203       ///< Initalize GIC in Non-Secure Mode
#define POST_INIT_FV                    0x204       ///< Initialize Firmware Volume
#define POST_RE_INIT_FV                 0x205       ///< Re-Initialize Firmware Volume
#define POST_PROCESS_FV_FILE            0x206       ///< Process Firmware Volume File
#define POST_PROCESS_FV_FILE_SECT       0x207       ///< Process Firmware Volume Sections
#define POST_CSP_LIB_INIT_PEI           0x208       ///< Initialize CSP Library PEI Phase
#define POST_BSP_CORE_MAIN_PRE_PEI      0x209       ///< BSP Core Main Pre-PEI
#define POST_AP_CORE_MAIN_PRE_PEI       0x20A       ///< AP Core Main Pre-PEI
#define POST_ENTER_PEI_CORE             0x20B       ///< Enter PEI Core
#define POST_INIT_GIC_PEI               0x20C       ///< Initialize PEI GIC
#define POST_UART_INIT                  0x20D       ///< Initialize UART
#define POST_UART_INIT_PORT             0x20E       ///< Initiaize the UART port attributes
#define POST_PEI_ISCP_INIT              0x20F       ///< Initialize PEI ISCP
#define POST_EXIT_PEI_CORE              0x210       ///< Exit PEI Core
#define POST_PRE_PI_MAIN                0x211       ///< Enter Pre-DXE Main
#define POST_BSP_CORE_MAIN_PRE_PI       0x212       ///< Enter BSP Pre-DXE Core
#define POST_AP_CORE_MAIN_PRE_PI        0x213       ///< Enter AP Core Main Pre-DXE
#define POST_DXE_MAIN_UEFI_DECOMP       0x214       ///< Decompress DXE
#define POST_ENTER_DXE_CORE             0x215       ///< Enter DXE Core
#define POST_DXE_CORE_MEM_ADD_SPACE     0x216       ///< Add Memory Space in DXE Core
#define POST_DXE_CORE_MEM_FREE_SPACE    0x217       ///< Free Memory Space in DXE Core
#define POST_INIT_GIC_DXE               0x218       ///< Initialize GIC in DXE phase
#define POST_INIT_ISCP_DXE              0x219       ///< Initialize ISCP DXE
#define POST_EXIT_DXE_CORE              0x21A       ///< Exit DXE Core
#define POST_EXIT_BOOT_SERV             0x21B       ///< Exit Boot Services
#define POST_FINAL                      0x3FF       ///< Final POST code


// AMD UEFI WTF Error Codes
#define ERROR_ISCP_TIMEOUT              0x250       ///< ISCP Timeout (no response from SCP)
#define ERROR_PXE_DHCP_FAIL             0x251       ///< PXE DHCP Fail
#define ERROR_PXE_DHCP_PASS             0x252       ///< PXE DHCP Pass
#define ERROR_PCIE_TRAIN_ERROR          0x261       ///< GIONB PCIE training error
#define ERROR_PCIE_SPEED_ERROR          0x262       ///< GIONB PCIE data rate error
#define ERROR_PCIE_PLL_ERROR            0x263       ///< GIONB PCIE PLL error
#define ERROR_NO_HDD_DETECTED           0x2CF       ///< No HDD Detected from SATA ports

/****** DO NOT WRITE BELOW THIS LINE *******/
  #ifdef __cplusplus
  }
#endif
#endif
