/* $NoKeywords */
/**
 * @file
 *
 * SocConfiguration.h
 *
 * Contains SoC Fuse Data structure definitions.
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

//#########################################################################
//#########################################################################
//#########################################################################
//        NOTE: This file shared between SCP and UEFI, make sure all     //
//              changes are reflected in both copies.                    //
//#########################################################################
//#########################################################################
//#########################################################################

#ifndef __SOC_CONFIGURATION_H_
#define __SOC_CONFIGURATION_H_

#ifdef __cplusplus
  extern "C" {
  #endif


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
  #include "ProcessorBind.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define SOC_BRAND_NAME_SIZE     (48)

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
  /// SOC Security Modes Enumeration
  typedef enum {
    SOC_SECURITY_MODE_BLANK     = 0,        ///< Security Mode Blank
    SOC_SECURITY_MODE_NOT_SECURE,           ///< Security Mode Non-secure
    SOC_SECURITY_MODE_SECURE,               ///< Security Mode Secure
    SOC_SECURITY_MODE_UNDEFINED,            ///< Security Mode Undefined
  } SOC_SECURITY_MODES;

  /// SOC Configuration, i.e. fusing structure
  typedef struct {
    UINT64   SerialNumber;                   ///< SOC Serial Number
    SOC_SECURITY_MODES  SecurityState;       ///< Indicates what security mode the SOC is in.
    INT32    CpuMap;                         ///< Map of CPU cores in SOC.
    INT32    CpuDefaultAClock;               ///< Default fused core frequency
    INT32    CpuClusterCount;                ///< Number of CPU clusters in SOC.
    INT32    CpuCoreCount;                   ///< Number of CPU cores in SOC.
    INT32    CpuClusterBoot;                 ///< Primary cluster used for boot.
    INT32    CpuCoreBoot;                    ///< Primary core used for boot.
    INT32    CcpEnabled;                     ///< Indicates CCP enabled state.  Zero if disabled; otherwise, enabled.
    INT32    PcieEnabled;                    ///< Indicates PCIe enabled state.  Zero if disabled; otherwise, enabled.
    INT32    SataEnabled;                    ///< Indicates SATA enabled state.  Zero if disabled; otherwise, enabled.
    INT32    XgeEnabled;                     ///< Indicates 10 gigabit Ethernet port enabled state.  Zero if disabled; otherwise, enabled.
    UINT32   BrandId;                        ///< Brand ID
    UINT32   ConfigurationId;                ///< Configuration ID
    UINT32   CpuIdModel;                     ///< CPU ID - Model
    UINT32   CpuIdExtModel;                  ///< CPU ID - Extended Model
    UINT32   CpuIdStepping;                  ///< CPU ID - Stepping
    UINT32   FixedErrata;                    ///< Fixed Errata
    UINT32   InternalRevision;               ///< Internal Revision
    UINT32   ManufacturingSpecifiedId;       ///< Manufacturing Specified Field
    CHAR8    BrandName[SOC_BRAND_NAME_SIZE]; ///< Null appended at end
  } SocConfiguration;

  #ifdef __cplusplus
  }
#endif

#endif // __SOC_CONFIGURATION_H__
