/**
*
*  Copyright (c) 2018, Marvell International Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
#ifndef __MV_I2C_H__
#define __MV_I2C_H__

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
