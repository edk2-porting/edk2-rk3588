/********************************************************************************
Copyright (C) 2018 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#ifndef __COMPHY_SIP_SVC_H__
#define __COMPHY_SIP_SVC_H__

#include <IndustryStandard/MvSmc.h>

/*
 * All values in this file are defined externally and used
 * for the SerDes configuration via SiP services.
 */

/* Helper macros for passing ComPhy parameters to the EL3 */
#define COMPHY_CALLED_FROM_FW       (1 << 21)
#define COMPHY_FW_MODE_FORMAT(mode) (mode << 12)
#define COMPHY_FW_FORMAT(mode, idx, speeds) \
                                    ((mode << 12) | (idx << 8) | (speeds << 2))
#define COMPHY_FW_PCIE_FORMAT(pcie_width, mode, speeds) \
                              (COMPHY_CALLED_FROM_FW | ((pcie_width) << 18) | \
                               COMPHY_FW_FORMAT(mode, 0, speeds))

/* Comphy speed */
#define COMPHY_SPEED_1_25G          0
#define COMPHY_SPEED_2_5G           1
#define COMPHY_SPEED_3_125G         2
#define COMPHY_SPEED_5G             3
#define COMPHY_SPEED_5_15625G       4
#define COMPHY_SPEED_6G             5
#define COMPHY_SPEED_10_3125G       6
#define COMPHY_SPEED_MAX            7
#define COMPHY_SPEED_INVALID        0xff
/* The  default speed for IO with fixed known speed */
#define COMPHY_SPEED_DEFAULT        0x3F

/* Comphy mode */
#define COMPHY_SATA_MODE            0x1
#define COMPHY_SGMII_MODE           0x2   /* SGMII 1G */
#define COMPHY_HS_SGMII_MODE        0x3   /* SGMII 2.5G */
#define COMPHY_USB3H_MODE           0x4
#define COMPHY_USB3D_MODE           0x5
#define COMPHY_PCIE_MODE            0x6
#define COMPHY_RXAUI_MODE           0x7
#define COMPHY_XFI_MODE             0x8
#define COMPHY_SFI_MODE             0x9
#define COMPHY_USB3_MODE            0xa
#define COMPHY_AP_MODE              0xb

/* Comphy unit index */
#define COMPHY_UNIT_ID0             0
#define COMPHY_UNIT_ID1             1
#define COMPHY_UNIT_ID2             2
#define COMPHY_UNIT_ID3             3

#endif //__COMPHY_SIP_SVC_H__
