/** @file
*
*  Copyright (c) 2021, Rockchip Inc. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#ifndef _RK_I2C_H__
#define _RK_I2C_H__

/*
 * I2C_FLAG_NORESTART is not part of PI spec, it allows to continue
 * transmission without repeated start operation.
 */
#define I2C_FLAG_NORESTART             0x00000002

/*
 * Helper macros for maintaining multiple I2C buses
 * and devices defined via EFI_I2C_DEVICE.
 */
#define I2C_DEVICE_ADDRESS(Index)      ((Index) & MAX_UINT16)
#define I2C_DEVICE_BUS(Index)          ((Index) >> 16)
#define I2C_DEVICE_INDEX(Bus, Address) (((Address) & MAX_UINT16) | (Bus) << 16)

#endif
