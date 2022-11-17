/** @file
*  Header defining the Base addresses, sizes, flags etc for Erratas
*
*  Copyright 2020 NXP
*
   SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef ERRATUM_H__
#define ERRATUM_H__

#define USB_TXVREFTUNE        0x9
#define USB_SQRXTUNE          0xFC7FFFFF
#define USB_PCSTXSWINGFULL    0x47
#define USB_PHY_RX_EQ_VAL_1   0x0000
#define USB_PHY_RX_EQ_VAL_2   0x8000
#define USB_PHY_RX_EQ_VAL_3   0x8003
#define USB_PHY_RX_EQ_VAL_4   0x800b

#define USB_PHY_RX_OVRD_IN_HI 0x200c

#endif
