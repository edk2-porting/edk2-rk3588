/* $NoKeywords */
/**
 * @file
 *
 * CoreState.h
 *
 * CPU Core State Structures and Definitions.
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

#ifndef CORESTATUS_H_
#define CORESTATUS_H_


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

/// Core State Enumeration
typedef enum {
  CPU_CORE_UNDEFINED = 0,         ///< Core is undefined
  CPU_CORE_DISABLED,              ///< Core is disabled
  CPU_CORE_POWERUP,               ///< Core/cluster is powered up
  CPU_CORE_POWERDOWN,             ///< Core/cluster is powered down
  CPU_CORE_RESET,                 ///< Core is powered but in reset
  CPU_CORE_RUN,                   ///< Core is running
  CPU_CORE_SLEEP,                 ///< Core is powered and sleeping (TBD)
} CPU_CORE_STATE;

/// SOC Core Status Structure
typedef struct {
  UINT32            ClusterId;    ///< CPU Cluster ID
  UINT32            CoreId;       ///< CPU Core ID
  CPU_CORE_STATE    Status;       ///< Core State Enumeration
  UINT64            ResetVector;  ///< CPU Core Reset Vector
} SocCoreStatus;

#endif /* CORESTATUS_H_ */
