/* $NoKeywords */
/**
 * @file
 *
 * IscpConfig.h
 *
 * Contains Intra-SoC Communication Protocol configuration definitions.
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
#ifndef ISCP_CONFIG_H_
#define ISCP_CONFIG_H_

#ifdef __cplusplus
  extern "C" {
  #endif


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
  #include <ProcessorBind.h>  // Included just so this file can be built into both the RTOS
                              // and UEFI without needing separate copies for both build
                              // environments.

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

// Door Bell Flag Register
#define ISCP_DRAM_BUFFER_ADDR_REG_LO    (0xE0000008UL)
#define ISCP_DRAM_BUFFER_ADDR_REG_HI    (0xE000000CUL)
#define ISCP_BUFFER_SIZE                (0x1000)
#define DOORBELL_OFFSET_NS              (0x100)
#define DOORBELL_BIT_NS                 (UINT32)              (1 << 7)    // Door Bell bit = [GPIO_1 (Line 14)]
#define DOORBELL_BIT_SEC                (UINT32)              (1 << 7)    // Door Bell bit = [GPIO_1 (Line 15)]

  #ifdef __cplusplus
  }
#endif


#endif /* ISCP_CONFIG_H_ */
