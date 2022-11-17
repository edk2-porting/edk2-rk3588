/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __RK_806_H
#define __RK_806_H

#include <Library/RockchipPlatformLib.h>
#include "Soc.h"

/* Not used or exisit register and configure */
#define NA			-1
#define BIT(n)			(1 << (n))
#define RK806_DBG		DEBUG_ERROR
/* rk806 buck*/
#define RK806_BUCK_ON_VSEL(n)		(0x1a + n - 1)
#define RK806_BUCK_SLP_VSEL(n)		(0x24 + n - 1)
#define RK806_BUCK_CONFIG(n)		(0x10 + n - 1)
#define RK806_BUCK_VSEL_MASK		0xff

/* RK806 LDO */
#define RK806_NLDO_ON_VSEL(n)		(0x43 + n - 1)
#define RK806_NLDO_SLP_VSEL(n)		(0x48 + n - 1)
#define RK806_NLDO_VSEL_MASK		0xff
#define RK806_PLDO_ON_VSEL(n)		(0x4e + n - 1)
#define RK806_PLDO_SLP_VSEL(n)		(0x54 + n - 1)
#define RK806_PLDO_VSEL_MASK		0xff

/* RK806 ENABLE */
#define RK806_POWER_EN(n)		(0x00 + n)
#define RK806_NLDO_EN(n)		(0x03 + n)
#define RK806_PLDO_EN(n)		(0x04 + n)
#define RK806_RAMP_RATE_MASK1		0xc0
#define RK806_RAMP_RATE_REG1(n)		(0x10 + n)
#define RK806_RAMP_RATE_REG1_8		0xeb
#define RK806_RAMP_RATE_REG9_10		0xea

#define RK806_RAMP_RATE_4LSB_PER_1CLK	0x00/* LDO 100mV/uS buck 50mV/us */
#define RK806_RAMP_RATE_2LSB_PER_1CLK	0x01/* LDO 50mV/uS buck 25mV/us */
#define RK806_RAMP_RATE_1LSB_PER_1CLK	0x02/* LDO 25mV/uS buck 12.5mV/us */
#define RK806_RAMP_RATE_1LSB_PER_2CLK	0x03/* LDO 12.5mV/uS buck 6.25mV/us */

#define RK806_RAMP_RATE_1LSB_PER_4CLK	0x04/* LDO 6.28/2mV/uS buck 3.125mV/us */
#define RK806_RAMP_RATE_1LSB_PER_8CLK	0x05/* LDO 3.12mV/uS buck 1.56mV/us */
#define RK806_RAMP_RATE_1LSB_PER_13CLK	0x06/* LDO 1.9mV/uS buck 961mV/us */
#define RK806_RAMP_RATE_1LSB_PER_32CLK	0x07/* LDO 0.78mV/uS buck 0.39mV/us */

#define RK806_PLDO0_2_MSK(pldo)		(BIT(pldo + 5))
#define RK806_PLDO0_2_SET(pldo)		(BIT(pldo + 1) | RK806_PLDO0_2_MSK(pldo))
#define RK806_PLDO0_2_CLR(pldo)		RK806_PLDO0_2_MSK(pldo)

#define RK806_CHIP_NAME			0x5A
#define RK806_CHIP_VER			0x5B

#define RK806_CMD_READ			0
#define RK806_CMD_WRITE			BIT(7)
#define RK806_CMD_CRC_EN		BIT(6)
#define RK806_CMD_CRC_DIS		0
#define RK806_CMD_LEN_MSK		0x0f
#define RK806_REG_H			0x00

#define RK806_SYS_CFG1			0x5f
#define RK806_SYS_CFG3			0x72
#define RK806_PWRON_KEY			0x76
#define RK806_INT_STS0			0x77
#define RK806_INT_MSK0			0x78
#define RK806_INT_STS1			0x79
#define RK806_INT_MSK1			0x7A
#define RK806_GPIO_INT_CONFIG		0x7B
#define RK806_IRQ_PWRON_FALL_MSK	BIT(0)
#define RK806_IRQ_PWRON_RISE_MSK	BIT(1)
#define RK806_DEV_OFF			BIT(0)
#define RK806_RST_MODE1			0x01
#define RK806_RST_MODE2			0x02
#define VERSION_AB			0x01

struct regulator_init_data {
	const char *supply_regulator;        /* or NULL for system supply */
	INT32 reg_id;
	INT32 init_voltage_mv;
};

struct rk8xx_reg_info {
	UINT32 min_uv;
	UINT32 step_uv;
	UINT8 vsel_reg;
	UINT8 vsel_sleep_reg;
	UINT8 config_reg;
	UINT8 vsel_mask;
	UINT8 min_sel;
	/* only for buck now */
	UINT8 max_sel;
	UINT8 range_num;
};

#define RK8XX_DESC_COM(_name, _reg_info, _ops) \
{						\
	.reg_info		= (_reg_info),	\
	.name	= (_reg_id),			\
	.ops		= _ops,			\
}

#define RK8XX_VOLTAGE_INIT(_id, _voltage) \
{		\
	.reg_id = (_id),\
	.init_voltage_mv	= (_voltage),\
}

/******************************************
8 -12: MASTER, SLAVE
4 -7: BUCK, NLDO, PLDO
0 -3:  num
******************************************/
#define MASTER	(0x0 << 8)
#define SLAVER	(0x1 << 8)
#define BUCK		(0x0 << 4)
#define NLDO		(0x1 << 4)
#define PLDO		(0x2 << 4)

enum master_num {
	MASTER_BUCK1 = (MASTER | BUCK | 0),
	MASTER_BUCK2 = (MASTER | BUCK | 1),
	MASTER_BUCK3 = (MASTER | BUCK | 2),
	MASTER_BUCK4 = (MASTER | BUCK | 3),
	MASTER_BUCK5 = (MASTER | BUCK | 4),
	MASTER_BUCK6 = (MASTER | BUCK | 5),
	MASTER_BUCK7 = (MASTER | BUCK | 6),
	MASTER_BUCK8 = (MASTER | BUCK | 7),
	MASTER_BUCK9 = (MASTER | BUCK | 8),
	MASTER_BUCK10 = (MASTER | BUCK | 9),

	MASTER_NLDO1 = (MASTER | NLDO | 0),
	MASTER_NLDO2 = (MASTER | NLDO | 1),
	MASTER_NLDO3 = (MASTER | NLDO | 2),
	MASTER_NLDO4 = (MASTER | NLDO | 3),
	MASTER_NLDO5 = (MASTER | NLDO | 4),

	MASTER_PLDO1 = (MASTER | PLDO | 0),
	MASTER_PLDO2 = (MASTER | PLDO | 1),
	MASTER_PLDO3 = (MASTER | PLDO | 2),
	MASTER_PLDO4 = (MASTER | PLDO | 3),
	MASTER_PLDO5 = (MASTER | PLDO | 4),
	MASTER_PLDO6 = (MASTER | PLDO | 5),
};

enum slaver_num {
	SLAVER_BUCK1 = (SLAVER | BUCK | 0),
	SLAVER_BUCK2 = (SLAVER | BUCK | 1),
	SLAVER_BUCK3 = (SLAVER | BUCK | 2),
	SLAVER_BUCK4 = (SLAVER | BUCK | 3),
	SLAVER_BUCK5 = (SLAVER | BUCK | 4),
	SLAVER_BUCK6 = (SLAVER | BUCK | 5),
	SLAVER_BUCK7 = (SLAVER | BUCK | 6),
	SLAVER_BUCK8 = (SLAVER | BUCK | 7),
	SLAVER_BUCK9 = (SLAVER | BUCK | 8),
	SLAVER_BUCK10 = (SLAVER | BUCK | 9),

	SLAVER_NLDO1 = (SLAVER | NLDO | 0),
	SLAVER_NLDO2 = (SLAVER | NLDO | 1),
	SLAVER_NLDO3 = (SLAVER | NLDO | 2),
	SLAVER_NLDO4 = (SLAVER | NLDO | 3),
	SLAVER_NLDO5 = (SLAVER | NLDO | 4),
            
	SLAVER_PLDO1 = (SLAVER | PLDO | 0),
	SLAVER_PLDO2 = (SLAVER | PLDO | 1),
	SLAVER_PLDO3 = (SLAVER | PLDO | 2),
	SLAVER_PLDO4 = (SLAVER | PLDO | 3),
	SLAVER_PLDO5 = (SLAVER | PLDO | 4),
	SLAVER_PLDO6 = (SLAVER | PLDO | 5),
};

extern RETURN_STATUS RK806Init(void);
extern void RK806RegulatorInit(struct regulator_init_data init_data);

#endif