/* $NoKeywords */
/**
 * @file
 *
 * MemSetup.h
 *
 * Contains common MemSetup-related structures and defines.
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

#ifndef MEMSETUP_H_
#define MEMSETUP_H_

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

  /// Memory Set up Structure for customer visibility
  typedef struct {
    UINT8  MemoryClockSpeed;                   ///< Memory clock speed
    UINT8  DDR3RttWr;                          ///< DDR3 Rtt_Wr
    UINT8  DDR3RttNom;                         ///< DDR3 Rtt_Nom
    UINT8  DDR4RttWr;                          ///< DDR4 Rtt_Wr
    UINT8  DDR4RttNom;                         ///< DDR4 Rtt_Nom
    UINT8  AddCtrlDriveStrength;               ///< Address/Control Drive Strength
    UINT8  ClockDriveStrengt;                  ///< Clock Drive Strength
    UINT8  DataDMDriveStrength;                ///< Data/DM Drive Strength
    UINT8  DQSDriveStrength;                   ///< DQS Drive Strength
    UINT8  PowerdownEnable;                    ///< Power down Enable
    UINT16 PowerdownIdleClocks;                ///< Power down Idle Clocks
    UINT8  LongCountMask;                      ///< Long Count Mask
    UINT8  ECCEnable;                          ///< ECC Enable/Disable
    UINT16  tref;                              ///< tref
    UINT16  tselseldq;                         ///< tsel_sel_dq
    UINT16  tselseldqs;                        ///< tsel_sel_dqs
    UINT16  trainingProgress;                  ///< training progress
    UINT16  trainingRestore;                   ///< restore training results
  } MEM_SETUP_VAR;

  #ifdef __cplusplus
  }
#endif


#endif /* MEMSETUP_H_ */
