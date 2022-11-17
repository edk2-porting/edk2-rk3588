/* $NoKeywords */
/**
 * @file
 *
 * AmdNvLib.c
 *
 * Provides library calls for NV (SPI and EEPROM) access.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: FDK
 * @e sub-project: UEFI
 * @e version: $Revision: 306428 $ @e date: $Date: 2014-10-23 14:42:26 -0500 (Thu, 23 Oct 2014) $
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

#ifndef _AMD_NV_H_
#define _AMD_NV_H_
#ifdef __cplusplus
  extern "C" {
  #endif

  #pragma pack(1)


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define  EEPROM_PAGE_SIZE        0x2000        // 8K block size
#define  EEPROM_ERASE_POLARITY   0x1           // Erase Polarity Positive (all bits ON)

/*----------------------------------------------------------------------------------------
 *                                 P R O T O T Y P E S
 *----------------------------------------------------------------------------------------
 */

  VOID
  AmdNvEepromRead (
    volatile UINT8      *Address,
    UINT8               *Data,
    UINT32              *Size
    );

  VOID
  AmdNvEepromWrite (
    volatile UINT8      *Address,
    UINT8               *Data,
    UINT32              *Size
    );

  VOID
  AmdNvEepromErase (
    volatile UINT8      *Address,
    UINT32              *Size
    );

  #pragma pack()

/****** DO NOT WRITE BELOW THIS LINE *******/
  #ifdef __cplusplus
  }
#endif
#endif

