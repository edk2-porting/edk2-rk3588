
#include <Library/RockchipPlatformLib.h>
#include "Soc.h"
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/SpiLib.h>
#include <Library/RK806.h>
#if 0
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
#endif

struct SPI_HANDLE gSPI;

static const struct rk8xx_reg_info rk806_buck[] = {
	/* buck 1 */
	{  500000,   6250, RK806_BUCK_ON_VSEL(1), RK806_BUCK_SLP_VSEL(1), RK806_BUCK_CONFIG(1), RK806_BUCK_VSEL_MASK, 0x00, 0xa0, 3},
	{  1500000, 25000, RK806_BUCK_ON_VSEL(1), RK806_BUCK_SLP_VSEL(1), RK806_BUCK_CONFIG(1), RK806_BUCK_VSEL_MASK, 0xa1, 0xed, 3},
	{  3400000,     0, RK806_BUCK_ON_VSEL(1), RK806_BUCK_SLP_VSEL(1), RK806_BUCK_CONFIG(1), RK806_BUCK_VSEL_MASK, 0xee, 0xff, 3},
	/* buck 2 */
	{  500000,   6250, RK806_BUCK_ON_VSEL(2), RK806_BUCK_SLP_VSEL(2), RK806_BUCK_CONFIG(2), RK806_BUCK_VSEL_MASK, 0x00, 0xa0, 3},
	{  1500000, 25000, RK806_BUCK_ON_VSEL(2), RK806_BUCK_SLP_VSEL(2), RK806_BUCK_CONFIG(2), RK806_BUCK_VSEL_MASK, 0xa1, 0xed, 3},
	{  3400000,     0, RK806_BUCK_ON_VSEL(2), RK806_BUCK_SLP_VSEL(2), RK806_BUCK_CONFIG(2), RK806_BUCK_VSEL_MASK, 0xee, 0xff, 3},
	/* buck 3 */
	{  500000,   6250, RK806_BUCK_ON_VSEL(3), RK806_BUCK_SLP_VSEL(3), RK806_BUCK_CONFIG(3), RK806_BUCK_VSEL_MASK, 0x00, 0xa0, 3},
	{  1500000, 25000, RK806_BUCK_ON_VSEL(3), RK806_BUCK_SLP_VSEL(3), RK806_BUCK_CONFIG(3), RK806_BUCK_VSEL_MASK, 0xa1, 0xed, 3},
	{  3400000,     0, RK806_BUCK_ON_VSEL(3), RK806_BUCK_SLP_VSEL(3), RK806_BUCK_CONFIG(3), RK806_BUCK_VSEL_MASK, 0xee, 0xff, 3},
	/* buck 4 */
	{  500000,   6250, RK806_BUCK_ON_VSEL(4), RK806_BUCK_SLP_VSEL(4), RK806_BUCK_CONFIG(4), RK806_BUCK_VSEL_MASK, 0x00, 0xa0, 3},
	{  1500000, 25000, RK806_BUCK_ON_VSEL(4), RK806_BUCK_SLP_VSEL(4), RK806_BUCK_CONFIG(4), RK806_BUCK_VSEL_MASK, 0xa1, 0xed, 3},
	{  3400000,     0, RK806_BUCK_ON_VSEL(4), RK806_BUCK_SLP_VSEL(4), RK806_BUCK_CONFIG(4), RK806_BUCK_VSEL_MASK, 0xee, 0xff, 3},
	/* buck 5 */
	{  500000,   6250, RK806_BUCK_ON_VSEL(5), RK806_BUCK_SLP_VSEL(5), RK806_BUCK_CONFIG(5), RK806_BUCK_VSEL_MASK, 0x00, 0xa0, 3},
	{  1500000, 25000, RK806_BUCK_ON_VSEL(5), RK806_BUCK_SLP_VSEL(5), RK806_BUCK_CONFIG(5), RK806_BUCK_VSEL_MASK, 0xa1, 0xed, 3},
	{  3400000,     0, RK806_BUCK_ON_VSEL(5), RK806_BUCK_SLP_VSEL(5), RK806_BUCK_CONFIG(5), RK806_BUCK_VSEL_MASK, 0xee, 0xff, 3},
	/* buck 6 */
	{  500000,   6250, RK806_BUCK_ON_VSEL(6), RK806_BUCK_SLP_VSEL(6), RK806_BUCK_CONFIG(6), RK806_BUCK_VSEL_MASK, 0x00, 0xa0, 3},
	{  1500000, 25000, RK806_BUCK_ON_VSEL(6), RK806_BUCK_SLP_VSEL(6), RK806_BUCK_CONFIG(6), RK806_BUCK_VSEL_MASK, 0xa1, 0xed, 3},
	{  3400000,     0, RK806_BUCK_ON_VSEL(6), RK806_BUCK_SLP_VSEL(6), RK806_BUCK_CONFIG(6), RK806_BUCK_VSEL_MASK, 0xee, 0xff, 3},
	/* buck 7 */
	{  500000,   6250, RK806_BUCK_ON_VSEL(7), RK806_BUCK_SLP_VSEL(7), RK806_BUCK_CONFIG(7), RK806_BUCK_VSEL_MASK, 0x00, 0xa0, 3},
	{  1500000, 25000, RK806_BUCK_ON_VSEL(7), RK806_BUCK_SLP_VSEL(7), RK806_BUCK_CONFIG(7), RK806_BUCK_VSEL_MASK, 0xa1, 0xed, 3},
	{  3400000,     0, RK806_BUCK_ON_VSEL(7), RK806_BUCK_SLP_VSEL(7), RK806_BUCK_CONFIG(7), RK806_BUCK_VSEL_MASK, 0xee, 0xff, 3},
	/* buck 8 */
	{  500000,   6250, RK806_BUCK_ON_VSEL(8), RK806_BUCK_SLP_VSEL(8), RK806_BUCK_CONFIG(8), RK806_BUCK_VSEL_MASK, 0x00, 0xa0, 3},
	{  1500000, 25000, RK806_BUCK_ON_VSEL(8), RK806_BUCK_SLP_VSEL(8), RK806_BUCK_CONFIG(8), RK806_BUCK_VSEL_MASK, 0xa1, 0xed, 3},
	{  3400000,     0, RK806_BUCK_ON_VSEL(8), RK806_BUCK_SLP_VSEL(8), RK806_BUCK_CONFIG(8), RK806_BUCK_VSEL_MASK, 0xee, 0xff, 3},
	/* buck 9 */
	{  500000,   6250, RK806_BUCK_ON_VSEL(9), RK806_BUCK_SLP_VSEL(9), RK806_BUCK_CONFIG(9), RK806_BUCK_VSEL_MASK, 0x00, 0xa0, 3},
	{  1500000, 25000, RK806_BUCK_ON_VSEL(9), RK806_BUCK_SLP_VSEL(9), RK806_BUCK_CONFIG(9), RK806_BUCK_VSEL_MASK, 0xa1, 0xed, 3},
	{  3400000,     0, RK806_BUCK_ON_VSEL(9), RK806_BUCK_SLP_VSEL(9), RK806_BUCK_CONFIG(9), RK806_BUCK_VSEL_MASK, 0xee, 0xff, 3},
	/* buck 10 */
	{  500000,   6250, RK806_BUCK_ON_VSEL(10), RK806_BUCK_SLP_VSEL(10), RK806_BUCK_CONFIG(10), RK806_BUCK_VSEL_MASK, 0x00, 0xa0, 3},
	{  1500000, 25000, RK806_BUCK_ON_VSEL(10), RK806_BUCK_SLP_VSEL(10), RK806_BUCK_CONFIG(10), RK806_BUCK_VSEL_MASK, 0xa1, 0xed, 3},
	{  3400000,     0, RK806_BUCK_ON_VSEL(10), RK806_BUCK_SLP_VSEL(10), RK806_BUCK_CONFIG(10), RK806_BUCK_VSEL_MASK, 0xee, 0xff, 3},
};

static const struct rk8xx_reg_info rk806_nldo[] = {
	/* nldo1 */
	{  500000, 12500, RK806_NLDO_ON_VSEL(1), RK806_NLDO_SLP_VSEL(1), NA, RK806_NLDO_VSEL_MASK, 0x00, },
	{  3400000,    0, RK806_NLDO_ON_VSEL(1), RK806_NLDO_SLP_VSEL(1), NA, RK806_NLDO_VSEL_MASK, 0xE8, },
	/* nldo2 */
	{  500000, 12500, RK806_NLDO_ON_VSEL(2), RK806_NLDO_SLP_VSEL(2), NA, RK806_NLDO_VSEL_MASK, 0x00, },
	{  3400000,    0, RK806_NLDO_ON_VSEL(2), RK806_NLDO_SLP_VSEL(2), NA, RK806_NLDO_VSEL_MASK, 0xE8, },
	/* nldo3 */
	{  500000, 12500, RK806_NLDO_ON_VSEL(3), RK806_NLDO_SLP_VSEL(3), NA, RK806_NLDO_VSEL_MASK, 0x00, },
	{  3400000,    0, RK806_NLDO_ON_VSEL(3), RK806_NLDO_SLP_VSEL(3), NA, RK806_NLDO_VSEL_MASK, 0xE8, },
	/* nldo4 */
	{  500000, 12500, RK806_NLDO_ON_VSEL(4), RK806_NLDO_SLP_VSEL(4), NA, RK806_NLDO_VSEL_MASK, 0x00, },
	{  3400000,    0, RK806_NLDO_ON_VSEL(4), RK806_NLDO_SLP_VSEL(4), NA, RK806_NLDO_VSEL_MASK, 0xE8, },
	/* nldo5 */
	{  500000, 12500, RK806_NLDO_ON_VSEL(5), RK806_NLDO_SLP_VSEL(5), NA, RK806_NLDO_VSEL_MASK, 0x00, },
	{  3400000,    0, RK806_NLDO_ON_VSEL(5), RK806_NLDO_SLP_VSEL(5), NA, RK806_NLDO_VSEL_MASK, 0xE8, },
};

static const struct rk8xx_reg_info rk806_pldo[] = {
	/* pldo1 */
	{  500000, 12500, RK806_PLDO_ON_VSEL(1), RK806_PLDO_SLP_VSEL(1), NA, RK806_PLDO_VSEL_MASK, 0x00, },
	{  3400000,    0, RK806_PLDO_ON_VSEL(1), RK806_PLDO_SLP_VSEL(1), NA, RK806_PLDO_VSEL_MASK, 0xE8, },
	/* pldo2 */
	{  500000, 12500, RK806_PLDO_ON_VSEL(2), RK806_PLDO_SLP_VSEL(2), NA, RK806_PLDO_VSEL_MASK, 0x00, },
	{  3400000,    0, RK806_PLDO_ON_VSEL(2), RK806_PLDO_SLP_VSEL(2), NA, RK806_PLDO_VSEL_MASK, 0xE8, },
	/* pldo3 */
	{  500000, 12500, RK806_PLDO_ON_VSEL(3), RK806_PLDO_SLP_VSEL(3), NA, RK806_PLDO_VSEL_MASK, 0x00, },
	{  3400000,    0, RK806_PLDO_ON_VSEL(3), RK806_PLDO_SLP_VSEL(3), NA, RK806_PLDO_VSEL_MASK, 0xE8, },
	/* pldo4 */
	{  500000, 12500, RK806_PLDO_ON_VSEL(4), RK806_PLDO_SLP_VSEL(4), NA, RK806_PLDO_VSEL_MASK, 0x00, },
	{  3400000,    0, RK806_PLDO_ON_VSEL(4), RK806_PLDO_SLP_VSEL(4), NA, RK806_PLDO_VSEL_MASK, 0xE8, },
	/* pldo5 */
	{  500000, 12500, RK806_PLDO_ON_VSEL(5), RK806_PLDO_SLP_VSEL(5), NA, RK806_PLDO_VSEL_MASK, 0x00, },
	{  3400000,    0, RK806_PLDO_ON_VSEL(5), RK806_PLDO_SLP_VSEL(5), NA, RK806_PLDO_VSEL_MASK, 0xE8, },
	/* pldo6 */
	{  500000, 12500, RK806_PLDO_ON_VSEL(6), RK806_PLDO_SLP_VSEL(6), NA, RK806_PLDO_VSEL_MASK, 0x00, },
	{  3400000,    0, RK806_PLDO_ON_VSEL(6), RK806_PLDO_SLP_VSEL(6), NA, RK806_PLDO_VSEL_MASK, 0xE8, },
};

#if 0
static void io_mem_show(const char *label, unsigned long base, unsigned int start, unsigned int end)
{
  unsigned int val, offset = start, nr = 0;

  if (label)
    DEBUG ((DEBUG_ERROR, "%a:\n", label));

  DEBUG ((DEBUG_ERROR, "%08lx:  ", base + offset));
  for (offset = start; offset <= end; offset += 0x04) {
    if (nr >= 4) {
      DEBUG ((DEBUG_ERROR, "\n%08lx:  ", base + offset));
      nr = 0;
    }
    val = MmioRead32(base + offset);
    DEBUG ((DEBUG_ERROR, "%08lx ", val));
    nr++;
  }
  DEBUG ((DEBUG_ERROR, "\n"));
}
#endif

static RETURN_STATUS _spi_read(INT32 cs_id, UINT32 reg, UINT8 *buffer, INT32 len)
{
  UINT8 txbuf[8];
  RETURN_STATUS status;

  txbuf[0] = RK806_CMD_READ;
  txbuf[1] = reg;
  txbuf[2] = RK806_REG_H;

  SPI_SetCS(&gSPI, cs_id, 1);
  status = SPI_Configure(&gSPI, txbuf, NULL, 3);
  status = SPI_PioTransfer(&gSPI);
  SPI_Stop(&gSPI);
  status = SPI_Configure(&gSPI, NULL, buffer, 1);
  status = SPI_PioTransfer(&gSPI);
  SPI_Stop(&gSPI);
  SPI_SetCS(&gSPI, cs_id, 0);

  return status;
}

static RETURN_STATUS _spi_write(INT32 cs_id, UINT32 reg, const UINT8 *buffer, INT32 len)
{
  UINT8 txbuf[4];
  RETURN_STATUS status;

  txbuf[0] = RK806_CMD_WRITE;
  txbuf[1] = reg;
  txbuf[2] = RK806_REG_H;
  txbuf[3] = *buffer;
  SPI_SetCS(&gSPI, cs_id, 1);
  status = SPI_Configure(&gSPI, txbuf, NULL, 4);
  status = SPI_PioTransfer(&gSPI);
  
  SPI_Stop(&gSPI);
  SPI_SetCS(&gSPI, cs_id, 0);

  return status;
}

static RETURN_STATUS pmic_reg_read(INT32 cs_id,
			  UINT32 reg,
			  UINT8 *buffer,
			  INT32 len)
{
  RETURN_STATUS ret;

  ret = _spi_read(cs_id, reg, buffer, len);
  if (ret)
    DEBUG ((RK806_DBG, "cs_id %d rk806 read reg(0x%x) error: %d buffer = %x\n",
           cs_id, reg, ret, buffer[0]));

  return ret;
}

static INT32 pmic_reg_write(INT32 cs_id,
			   UINT32 reg,
			   const UINT8 *buffer,
			   INT32 len)
{
  RETURN_STATUS ret;

  ret = _spi_write(cs_id, reg, buffer, len);
  if (ret)
    DEBUG ((RK806_DBG, "cs_id %d rk806 write reg(0x%x) error: %d %x\n",
           cs_id, reg, ret, buffer[0]));

  return ret;
}

static RETURN_STATUS pmic_clrsetbits(INT32 cs_id, UINT32 reg, UINT32 clr, UINT32 set)
{
	UINT8 byte;
	RETURN_STATUS ret;

	ret = pmic_reg_read(cs_id, reg, &byte, 0x01);
	if (ret < 0)
		return ret;
	byte = (ret & ~clr) | set;

	 pmic_reg_write(cs_id, reg, &byte, 1);
	ret = pmic_reg_read(cs_id, reg, &byte, 0x01);
	return ret;
}
 
static const struct rk8xx_reg_info *get_buck_reg(INT32 num, INT32 uvolt)
{
	if (uvolt < 1500000)
		return &rk806_buck[num * 3 + 0];
	else if (uvolt < 3400000)
		return &rk806_buck[num * 3 + 1];
	else
		return &rk806_buck[num * 3 + 2];
}
 
static const struct rk8xx_reg_info *get_nldo_reg(INT32 num, INT32 uvolt)
{
	if (uvolt < 3400000)
		return &rk806_nldo[num * 2];
	else
		return &rk806_nldo[num * 2 + 1];
}

static const struct rk8xx_reg_info *get_pldo_reg(INT32 num, INT32 uvolt)
{
	if (uvolt < 3400000)
		return &rk806_pldo[num * 2];
	else
		return &rk806_pldo[num * 2 + 1];
}
static RETURN_STATUS buck_set_voltage(INT32 reg_id, INT32 uvolt)
{
	INT32 buck = reg_id & 0x0f;
	const struct rk8xx_reg_info *info = get_buck_reg(buck, uvolt);
	INT32 mask = info->vsel_mask;
	INT32 cs_id = (reg_id & 0xf00) >> 8;
	INT32 val;

	if (info->vsel_reg == NA)
		return RETURN_INVALID_PARAMETER;

	if (info->step_uv == 0)	/* Fixed voltage */
		val = info->min_sel;
	else
		val = ((uvolt - info->min_uv) / info->step_uv) + info->min_sel;

	DEBUG ((RK806_DBG, "%a: volt=%d, buck=%d, reg=0x%x, mask=0x%x, val=0x%x\n",
	      __func__, uvolt, buck + 1, info->vsel_reg, mask, val));

	return pmic_clrsetbits(cs_id, info->vsel_reg, mask, val);
}

static RETURN_STATUS nldo_set_voltage(INT32 reg_id, INT32 uvolt)
{
	INT32 ldo = reg_id & 0x0f;
	const struct rk8xx_reg_info *info = get_nldo_reg(ldo, uvolt);
	INT32 mask = info->vsel_mask;
	INT32 cs_id = (reg_id & 0xf00) >> 8;
	INT32 val;

	if (info->vsel_reg == NA)
		return RETURN_INVALID_PARAMETER;

	if (info->step_uv == 0)
		val = info->min_sel;
	else
		val = ((uvolt - info->min_uv) / info->step_uv) + info->min_sel;

	DEBUG ((RK806_DBG, "%a: volt=%d, ldo=%d, reg=0x%x, mask=0x%x, val=0x%x\n",
	      __func__, uvolt, ldo + 1, info->vsel_reg, mask, val));

	return pmic_clrsetbits(cs_id, info->vsel_reg, mask, val);
}

static RETURN_STATUS pldo_set_voltage(INT32 reg_id, INT32 uvolt)
{
	INT32 ldo = reg_id & 0x0f;
	const struct rk8xx_reg_info *info = get_pldo_reg(ldo, uvolt);
	INT32 mask = info->vsel_mask;
	INT32 cs_id = (reg_id & 0xf00) >> 8;
	INT32 val;

	if (info->vsel_reg == NA)
		return RETURN_INVALID_PARAMETER;

	if (info->step_uv == 0)
		val = info->min_sel;
	else
		val = ((uvolt - info->min_uv) / info->step_uv) + info->min_sel;

	DEBUG ((RK806_DBG, "%s: volt=%d, ldo=%d, reg=0x%x, mask=0x%x, val=0x%x\n",
	      __func__, uvolt, ldo + 1, info->vsel_reg, mask, val));

	return pmic_clrsetbits(cs_id, info->vsel_reg, mask, val);
}

static RETURN_STATUS buck_set_enable(INT32 reg_id, BOOLEAN enable)
{
	INT32 cs_id = (reg_id & 0xf00) >> 8;
	INT32 buck = reg_id & 0x0f;
	UINT8 value, en_reg;
	RETURN_STATUS ret;

	en_reg = RK806_POWER_EN(buck / 4);
	if (enable)
		value = ((1 << buck % 4) | (1 << (buck % 4 + 4)));
	else
		value = ((0 << buck % 4) | (1 << (buck % 4 + 4)));

	ret = pmic_reg_write(cs_id, en_reg, &value, 1);

	return ret;
}

static RETURN_STATUS nldo_set_enable(INT32 reg_id, BOOLEAN enable)
{
	INT32 cs_id = (reg_id & 0xf00) >> 8;
	INT32 ldo = reg_id & 0x0f;
	UINT8 value, en_reg;
	RETURN_STATUS ret;

		if (ldo < 4) {
			en_reg = RK806_NLDO_EN(0);
			if (enable)
				value = ((1 << ldo % 4) | (1 << (ldo % 4 + 4)));
			else
				value = ((0 << ldo % 4) | (1 << (ldo % 4 + 4)));
			ret =  pmic_reg_write(cs_id, en_reg, &value, 1);
		} else {
			en_reg = RK806_NLDO_EN(2);
			if (enable)
				value = 0x44;
			else
				value = 0x40;
			ret = pmic_reg_write(cs_id, en_reg, &value, 1);
		}

	return ret;
}

static RETURN_STATUS pldo_set_enable(INT32 reg_id, BOOLEAN enable)
{
	INT32 cs_id = (reg_id & 0xf00) >> 8;
	INT32 pldo = reg_id & 0x0f;
	UINT8 value, en_reg;
	RETURN_STATUS ret;

	if (pldo < 3) {
		en_reg = RK806_PLDO_EN(0);
		if (enable)
			value = RK806_PLDO0_2_SET(pldo);
		else
			value = RK806_PLDO0_2_CLR(pldo);
		ret =  pmic_reg_write(cs_id, en_reg, &value, 1);
	} else if (pldo == 3) {
		en_reg = RK806_PLDO_EN(1);
		if (enable) {
			value = ((1 << 0) | (1 << 4));
		} else {
			value = (1 << 4);
		}

		ret =  pmic_reg_write(cs_id, en_reg, &value, 1);
	} else if (pldo == 4) {
		en_reg = RK806_PLDO_EN(1);
		if (enable)
			value = ((1 << 1) | (1 << 5));
		else
			value = ((0 << 1) | (1 << 5));
		ret =  pmic_reg_write(cs_id, en_reg, &value, 1);
	} else if (pldo == 5) {
		en_reg = RK806_PLDO_EN(0);
		if (enable)
			value = ((1 << 0) | (1 << 4));
		else
			value = ((0 << 0) | (1 << 4));
		ret =  pmic_reg_write(cs_id, en_reg, &value, 1);
	}

	return ret;
}

void RK806RegulatorInit(struct regulator_init_data init_data)
{
	INT32 reg_id;
	INT32 init_voltage;

	init_voltage = init_data.init_voltage_mv;
	reg_id = init_data.reg_id;

	if ((reg_id & 0xf0) == BUCK) {
		buck_set_voltage(reg_id, init_voltage);
		buck_set_enable(reg_id, 1);
	} else if ((reg_id & 0xf0) == NLDO) {
		nldo_set_voltage(reg_id, init_voltage);
		nldo_set_enable(reg_id, 1);
	} if ((reg_id & 0xf0) == PLDO) {
		pldo_set_voltage(reg_id, init_voltage);
		pldo_set_enable(reg_id, 1);
	}
}

static
RETURN_STATUS
SpiCongig(struct SPI_HANDLE *pSPI)
{
  struct SPI_CONFIG *pSPIConfig = &pSPI->config;
  DEBUG ((DEBUG_ERROR, "%a(%u): 0: %x\n", "SpiCongig", __LINE__, 0));

  /* Data width */
  pSPIConfig->nBytes = CR0_DATA_FRAME_SIZE_8BIT;

  /* CPOL */
  //pSPIConfig->clkPolarity = CR0_POLARITY_HIGH;
  pSPIConfig->clkPolarity = CR0_POLARITY_LOW;

  /* CPHA */
  //pSPIConfig->clkPhase = CR0_PHASE_2EDGE;
  pSPIConfig->clkPhase = CR0_PHASE_1EDGE;

  /* MSB or LSB */
  pSPIConfig->firstBit = CR0_FIRSTBIT_MSB;
  //pSPIConfig->firstBit = CR0_FIRSTBIT_LSB;

  /* Master or Slave */
  //pSPIConfig->opMode = CR0_OPM_SLAVE;
  pSPIConfig->opMode = CR0_OPM_MASTER;

  /* CSM cycles */
  pSPIConfig->csm = 0;

  pSPI->maxFreq = 24000000;
  pSPIConfig->speed = 2000000;

  if (pSPI->config.speed > HAL_SPI_MASTER_MAX_SCLK_OUT) {
    pSPI->config.speed = HAL_SPI_MASTER_MAX_SCLK_OUT;
  }

  return RETURN_SUCCESS;
}

RETURN_STATUS RK806Init(void)
{
  UINT32 base = FixedPcdGet32 (SpiRK806BaseAddr);

  Rk806SpiIomux();

  DEBUG ((DEBUG_ERROR, "%a(%u): base: %x\n", "RK806Init", __LINE__, base));

  SPI_Init(&gSPI, base);
  SpiCongig(&gSPI);

  return RETURN_SUCCESS;
}
