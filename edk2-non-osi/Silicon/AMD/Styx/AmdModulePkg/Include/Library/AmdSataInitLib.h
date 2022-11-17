/* $NoKeywords */
/**
 * @file
 *
 * AmdSataInitLib.h
 *
 * Public SATA PHY layer initilization and training routines for Serdes registers.
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

#ifndef _AMD_SATA_INIT_LIB_H_
#define _AMD_SATA_INIT_LIB_H_
#ifdef __cplusplus
  extern "C" {
  #endif

  /**
   *---------------------------------------------------------------------------------------
   *
   *  SataPhyInit
   *
   *  Description:
   *    Library call that trains the SATA PHY.
   *
   *  Control flow:
   *    1. Initialize variables
   *    2. Continue to set Serdes bits while not locked
   *
   *  Parameters:
   *    @param[in]      cmu_number           Cmu block number. Port 0,1 belongs to CMU 0
   *    @param[in]      EvenPortGen           Port Gen value for even port in given CMU
   *    @param[in]      OddPortGen            Port Gen Value for Odd Port in given CMU
   *
   *    @return         VOID
   *
   *------------------------------------------------------------------------------------
   **/
  VOID
  EFIAPI
  SataPhyInit (
    IN       UINT32 cmu_number,
    IN       UINT32 EvenPortGen,
    IN       UINT32 OddPortGen
    );

  /**
   *---------------------------------------------------------------------------------------
   *
   *  SetCwMinSata0
   *
   *  Description:
   *
   *  Parameters:
   *    @param[in]      Portnum            Port number
   *
   *    @return         VOID
   *
   *------------------------------------------------------------------------------------
   **/
  VOID
  EFIAPI
  SetCwMinSata0 (
    IN       UINT32 Portnum
    );

  /**
   *---------------------------------------------------------------------------------------
   *
   *  SetCwMinSata1
   *
   *  Description:
   *
   *  Parameters:
   *    @param[in]      Portnum            Port number
   *
   *    @return         VOID
   *
   *------------------------------------------------------------------------------------
   **/
  VOID
  EFIAPI
  SetCwMinSata1 (
    IN       UINT32 Portnum
    );

  /**
   *---------------------------------------------------------------------------------------
   *
   *  SetPrdSingleSata0
   *
   *  Description:
   *
   *  Parameters:
   *    @param[in]      Portnum            Port number
   *
   *    @return         VOID
   *
   *------------------------------------------------------------------------------------
   **/
  VOID
  EFIAPI
  SetPrdSingleSata0 (
    IN       UINT32 Portnum
    );

  /**
  *---------------------------------------------------------------------------------------
  *
  *  SetPrdSingleSata1
  *
  *  Description:
  *
  *  Parameters:
  *    @param[in]      Portnum            Port number
  *
  *    @return         VOID
  *
  *------------------------------------------------------------------------------------
  **/
  VOID
  EFIAPI
  SetPrdSingleSata1 (
    IN       UINT32 Portnum
    );

/****** DO NOT WRITE BELOW THIS LINE *******/
  #ifdef __cplusplus
  }
#endif
#endif

