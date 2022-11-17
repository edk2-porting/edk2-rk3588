/* $NoKeywords */
/**
 * @file
 *
 * Wtf_Reg.h
 *
 * Contains Where's-The-Firmware (WTF) definitions and Macros.
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

#ifndef __WTF_REG__H_
#define __WTF_REG__H_

#ifdef __cplusplus
  extern "C" {
  #endif

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define WTF_STATUS_REG                  0xE0000000  // "Where's The Firmware" Register

#define WTF_STATUS_REG_SIZE             32
#define WTF_STATUS_ERROR_SIZE           12
#define WTF_STATUS_POST_SIZE            10
#define WTF_STATUS_FW_INDICATOR_SIZE    2
#define WTF_STATUS_BT_CHKSUMFAIL_SIZE   1

#define WTF_STATUS_ERROR_SHIFT          0
#define WTF_STATUS_POST_SHIFT           12
#define WTF_STATUS_FW_INDICATOR_SHIFT   22
#define WTF_STATUS_BT_CHKSUMFAIL_SHIFT  31

#define WTF_STATUS_ERROR_MASK           0x00000FFF
#define WTF_STATUS_POST_MASK            0x003FF000
#define WTF_STATUS_FW_INDICATOR_MASK    0x00C00000
#define WTF_STATUS_BT_CHKSUMFAIL_MASK   0x80000000

#define WTF_STATUS_MASK \
        (WTF_STATUS_ERROR_MASK | \
        WTF_STATUS_POST_MASK | \
        WTF_STATUS_FW_INDICATOR_MASK | \
        WTF_STATUS_BT_CHKSUMFAIL_MASK)

#define WTF_STATUS_DEFAULT              0x00000000
#define WTF_STATUS_FW_INDICATOR_UEFI    0x2

#define WTF_STATUS_GET_ERROR(wtf_status) \
        ((wtf_status & WTF_STATUS_ERROR_MASK) >> WTF_STATUS_ERROR_SHIFT)
#define WTF_STATUS_GET_POST(wtf_status) \
        ((wtf_status & WTF_STATUS_POST_MASK) >> WTF_STATUS_POST_SHIFT)
#define WTF_STATUS_GET_FW_INDICATOR(wtf_status) \
        ((wtf_status & WTF_STATUS_FW_INDICATOR_MASK) >> WTF_STATUS_FW_INDICATOR_SHIFT)
#define WTF_STATUS_GET_BT_CHKSUMFAIL(wtf_status) \
        ((wtf_status & WTF_STATUS_BT_CHECKSUMFAIL_MASK) >> WTF_STATUS_BT_CHECKSUMFAIL_SHIFT)

#define WTF_STATUS_SET_ERROR(error) { \
        UINT32 wtf_status_reg; \
        wtf_status_reg = MmioRead32 (WTF_STATUS_REG); \
        wtf_status_reg = (wtf_status_reg & ~WTF_STATUS_ERROR_MASK) | (error << WTF_STATUS_ERROR_SHIFT); \
        wtf_status_reg = (wtf_status_reg & ~WTF_STATUS_FW_INDICATOR_MASK) | (WTF_STATUS_FW_INDICATOR_UEFI << WTF_STATUS_FW_INDICATOR_SHIFT); \
        MmioWrite32 (WTF_STATUS_REG, wtf_status_reg); \
}

#define WTF_STATUS_SET_POST(post) { \
        UINT32 wtf_status_reg; \
        wtf_status_reg = MmioRead32 (WTF_STATUS_REG); \
        wtf_status_reg = (wtf_status_reg & ~WTF_STATUS_POST_MASK) | (post << WTF_STATUS_POST_SHIFT); \
        wtf_status_reg = (wtf_status_reg & ~WTF_STATUS_FW_INDICATOR_MASK) | (WTF_STATUS_FW_INDICATOR_UEFI << WTF_STATUS_FW_INDICATOR_SHIFT); \
        MmioWrite32 (WTF_STATUS_REG, wtf_status_reg); \
}

#define WTF_STATUS_SET_FW_INDICATOR(fwindicator) { \
        UINT32 wtf_status_reg; \
        wtf_status_reg = MmioRead32 (WTF_STATUS_REG); \
        wtf_status_reg = (wtf_status_reg & ~WTF_STATUS_FW_INDICATOR_MASK) | (fwindicator << WTF_STATUS_FW_INDICATOR_SHIFT); \
        MmioWrite32 (WTF_STATUS_REG, wtf_status_reg); \
}

#define WTF_STATUS_SET_BT_CHKSUMFAIL(btchksmfail) { \
        UINT32 wtf_status_reg; \
        wtf_status_reg = MmioRead32 (WTF_STATUS_REG); \
        wtf_status_reg = (wtf_status_reg & ~WTF_STATUS_BT_CHKSUMFAIL_MASK) | (btchksmfail << WTF_STATUS_BT_CHKSUMFAIL_SHIFT); \
        MmioWrite32 (WTF_STATUS_REG, wtf_status_reg); \
}

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
/// WTF Status Structure
typedef
struct _WTF_STATUS_T {
  UINT64 error                   : WTF_STATUS_ERROR_SIZE;          ///< WTF Status Error Size
  UINT64 post                    : WTF_STATUS_POST_SIZE;           ///< WTF Status Post Size
  UINT64 fwindicator             : WTF_STATUS_FW_INDICATOR_SIZE;   ///< WTF Status Firmware Indicator Size
  UINT64 reserved                : 7;                              ///< Reserved
  UINT64 btchksmfail             : WTF_STATUS_BT_CHKSUMFAIL_SIZE;  ///< WTF Status Bit Checksum Fail Size
} WTF_STATUS_T;

/// WTF Status Union
typedef
union {
  UINT32 val : 32;   ///< Value
  WTF_STATUS_T f;    ///< WTF Status Structure
} WTF_STATUS_U;


/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif
