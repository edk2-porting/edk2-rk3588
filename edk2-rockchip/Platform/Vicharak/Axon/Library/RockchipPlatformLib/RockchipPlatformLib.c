/** @file
*
*  Copyright (c) 2021, Rockchip Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/GpioLib.h>
#include <Library/RK806.h>
#include <Library/Rk3588Pcie.h>
#include <Library/PWMLib.h>
#include <Soc.h>
#include <VarStoreData.h>

static struct regulator_init_data rk806_init_data[] = {
	/* Master PMIC */
	// vdd_gpu_s0
	RK8XX_VOLTAGE_INIT(MASTER_BUCK1, 950000),
	// vdd_cpu_lit_s0 (Dynamically set from CpuPerformance)
	// RK8XX_VOLTAGE_INIT(MASTER_BUCK2, 950000),
	// vdd_log_s0
	RK8XX_VOLTAGE_INIT(MASTER_BUCK3, 750000),
	// vdd_vdenc_s0
	RK8XX_VOLTAGE_INIT(MASTER_BUCK4, 750000),
	// vdd_ddr_s0
	RK8XX_VOLTAGE_INIT(MASTER_BUCK5, 900000),
	// vcc_2v0_pldo_s3
	RK8XX_VOLTAGE_INIT(MASTER_BUCK7, 2000000),
	// vcc_3v3_s3
	RK8XX_VOLTAGE_INIT(MASTER_BUCK8, 3300000),
	// vcc_1v8_s3
	RK8XX_VOLTAGE_INIT(MASTER_BUCK10, 1800000),

	// avcc_1v8_s0
	RK8XX_VOLTAGE_INIT(MASTER_PLDO1, 1800000),
	// vcc_1v8_s0
	RK8XX_VOLTAGE_INIT(MASTER_PLDO2, 1800000),
	// avdd_1v2_s0
	RK8XX_VOLTAGE_INIT(MASTER_PLDO3, 1200000),
	// vcc_3v3_s0
	RK8XX_VOLTAGE_INIT(MASTER_PLDO4, 3300000),
	// vccio_sd_s0
	RK8XX_VOLTAGE_INIT(MASTER_PLDO5, 3300000),
	// pldo6_s3
	RK8XX_VOLTAGE_INIT(MASTER_PLDO6, 1800000),

	// vdd_0v75_s3
	RK8XX_VOLTAGE_INIT(MASTER_NLDO1, 750000),
	// vdd_ddr_pll_s0
	RK8XX_VOLTAGE_INIT(MASTER_NLDO2, 850000),
	// avdd_0v75_s0
	RK8XX_VOLTAGE_INIT(MASTER_NLDO3, 837500),
	// vdd_0v85_s0
	RK8XX_VOLTAGE_INIT(MASTER_NLDO4, 850000),
	// vdd_0v75_s0
	RK8XX_VOLTAGE_INIT(MASTER_NLDO5, 750000),
	/* No dual PMICs on this platform */
};

VOID EFIAPI SdmmcIoMux(VOID)
{
	/* sdmmc0 iomux (microSD socket) */
	BUS_IOC->GPIO4D_IOMUX_SEL_L =
		// write_enable{0,1,2,3}
		(0xFFFFUL << 16) |
		// SDMMC_D0, SDMMC_D1, SDMMC_D2, SDMMC_D3
		(0x1111);

	BUS_IOC->GPIO4D_IOMUX_SEL_H =
		// write_enable
		(0x00FFUL << 16) |
		// SDMMC_CLK, SDMMC_CMD
		(0x0011);

	PMU1_IOC->GPIO0A_IOMUX_SEL_H =
		// write_enable
		(0x000FUL << 16) |
		// SDMMC_DET
		(0x0001);
}

VOID EFIAPI SdhciEmmcIoMux(VOID)
{
	/* sdhci0 iomux (eMMC socket) */
	BUS_IOC->GPIO2A_IOMUX_SEL_L =
		// write_enable
		(0xFFFFUL << 16) |
		// EMMC_CMD, EMMC_CLKOUT, EMMC_DATASTROBE, EMMC_RSTN
		(0x1111);

	BUS_IOC->GPIO2D_IOMUX_SEL_L =
		// write_enable{0,1,2,3}
		(0xFFFFUL << 16) |
		// EMMC_D0, EMMC_D1, EMMC_D2, EMMC_D3
		(0x1111);

	BUS_IOC->GPIO2D_IOMUX_SEL_H =
		// write_enable{4,5,6,7}
		(0xFFFFUL << 16) |
		// EMMC_D4, EMMC_D5, EMMC_D6, EMMC_D7
		(0x1111);
}

#define NS_CRU_BASE 0xFD7C0000
#define CRU_CLKSEL_CON59 0x03EC
#define CRU_CLKSEL_CON78 0x0438

VOID EFIAPI Rk806SpiIomux(VOID)
{
	/* io mux */
	PMU1_IOC->GPIO0A_IOMUX_SEL_H =
		// write_enable
		(0x0FF0UL << 16) |
		// SPI2_MOSI_M2, SPI2_CLK_M2
		(0x0110);

	PMU1_IOC->GPIO0B_IOMUX_SEL_L =
		// write_enable
		(0xF0FFUL << 16) |
		// SPI2_MISO_M2, SPI2_CS0_M2, SPI2_CS1_M2
		(0x1011);

	MmioWrite32(NS_CRU_BASE + CRU_CLKSEL_CON59,
		    (0x00C0UL << 16) | (0x0080));
}

VOID EFIAPI Rk806Configure(VOID)
{
	UINTN RegCfgIndex;

	RK806Init();

	for (RegCfgIndex = 0; RegCfgIndex < ARRAY_SIZE(rk806_init_data);
	     RegCfgIndex++)
		RK806RegulatorInit(rk806_init_data[RegCfgIndex]);
}

VOID EFIAPI SetCPULittleVoltage(IN UINT32 Microvolts)
{
	struct regulator_init_data Rk806CpuLittleSupply =
		RK8XX_VOLTAGE_INIT(MASTER_BUCK2, Microvolts);

	RK806RegulatorInit(Rk806CpuLittleSupply);
}

VOID EFIAPI NorFspiIomux(VOID)
{
	/* No NOR flash here */
}

VOID EFIAPI NorFspiEnableClock(UINT32 *CruBase)
{
}

VOID EFIAPI GmacIomux(IN UINT32 Id)
{
	switch (Id) {
	case 1:
		/* gmac1 iomux */
		BUS_IOC->GPIO3A_IOMUX_SEL_L =
			// write_enable
			(0xFFFFUL << 16) |
			// GMAC1_RXD3, GMAC1_RXD2, GMAC1_TXD3, GMAC1_TXD2
			(0x1111);

		BUS_IOC->GPIO3A_IOMUX_SEL_H =
			// write_enable
			(0xF0FFUL << 16) |
			// GMAC1_RXD0, GMAC1_RXCLK, GMAC1_TXCLK
			(0x1011);

		BUS_IOC->GPIO3B_IOMUX_SEL_L =
			(0xF0FFUL << 16) |
			// GMAC1_TXD0, GMAC1_RXDV_CRS, GMAC1_RXD1
			(0x1011);

		BUS_IOC->GPIO3B_IOMUX_SEL_H =
			// write_enable
			(0x0FFFUL << 16) |
			// GMAC1_PTP_REF_CLK, GMAC1_MCLKINOUT, GMAC1_TXEN, GMAC1_TXD1
			(0x0111);

		BUS_IOC->GPIO3C_IOMUX_SEL_L =
			// write_enable
			(0xFF00UL << 16) |
			// GMAC1_MDIO, GMAC1_MDC
			(0x1100);

		/* phy1 reset */
		GpioPinSetDirection(3, GPIO_PIN_PB7, GPIO_PIN_OUTPUT);
		break;
	default:
		break;
	}
}

VOID EFIAPI GmacIoPhyReset(UINT32 Id, BOOLEAN Enable)
{
	switch (Id) {
	case 1:
		/* phy1 reset */
		GpioPinWrite(3, GPIO_PIN_PB7, !Enable);
		break;
	default:
		break;
	}
}

VOID EFIAPI I2cIomux(UINT32 id)
{
	switch (id) {
	case 0:
		/* io mux M2 */
		PMU2_IOC->GPIO0D_IOMUX_SEL_L =
			// write_enable
			(0x0FF0UL << 16) |
			// I2C0_SDA_M2, I2C0_SCL_M2
			(0x0330);
		break;
	case 1:
		/* io mux M2 */
		BUS_IOC->GPIO0D_IOMUX_SEL_H =
			// write_enable
			(0x00FFUL << 16) |
			// I2C1_SDA_M2, I2C1_SCL_M2
			(0x0099);
		PMU2_IOC->GPIO0D_IOMUX_SEL_H =
			// write_enable
			(0x00FFUL << 16) |
			// I2C1_SDA_M2, I2C1_SCL_M2
			(0x0088);
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	default:
		break;
	}
}

VOID EFIAPI UsbPortPowerEnable(VOID)
{
	DEBUG((DEBUG_INFO, "UsbPortPowerEnable called\n"));

	/* vbus5v0_typec0 */
	// Need to implement PCA9554 IO Expander support
	/* vbus5v0_typec1 */
	// Need to implement PCA9554 IO Expander support
}

#define USB2PHY0_GRF_BASE 0xFD5D0000
#define PMU0_GRF_SOC_CON2 0x00000008
#define USB2PHY1_GRF_BASE 0xFD5D4000
#define USB2PHY2_GRF_BASE 0xFD5D8000
#define USB2PHY3_GRF_BASE 0xFD5DC000
#define PMU1_CRU_BASE 0xFD7F0000
#define PMU1CRU_SOFTRST_CON04 0x00000A10

VOID EFIAPI Usb2PhyResume(VOID)
{
	// Set pmu0_mem_cfg_hdsprf to RA
	MmioWrite32(USB2PHY0_GRF_BASE + PMU0_GRF_SOC_CON2,
		    (0x2000UL << 16) | 0x0000);
	MmioWrite32(USB2PHY1_GRF_BASE + PMU0_GRF_SOC_CON2,
		    (0x2000UL << 16) | 0x0000);
	MmioWrite32(USB2PHY2_GRF_BASE + PMU0_GRF_SOC_CON2,
		    (0x2000UL << 16) | 0x0000);
	MmioWrite32(USB2PHY3_GRF_BASE + PMU0_GRF_SOC_CON2,
		    (0x2000UL << 16) | 0x0000);

	MmioWrite32(PMU1_CRU_BASE + PMU1CRU_SOFTRST_CON04, 0x07000700);
	MmioWrite32(PMU1_CRU_BASE + PMU1CRU_SOFTRST_CON04, 0x07000000);
}

VOID EFIAPI PcieIoInit(UINT32 Segment)
{
	/* Set reset and power IO to gpio output mode */
	switch (Segment) {
	case PCIE_SEGMENT_PCIE30X4: // U.2
		GpioPinSetDirection(4, GPIO_PIN_PB6, GPIO_PIN_OUTPUT);
		GpioPinSetDirection(0, GPIO_PIN_PB2, GPIO_PIN_OUTPUT);
		break;
	case PCIE_SEGMENT_PCIE20L0:
		GpioPinSetDirection(4, GPIO_PIN_PA5, GPIO_PIN_OUTPUT);
		break;
	case PCIE_SEGMENT_PCIE20L1:
		GpioPinSetDirection(4, GPIO_PIN_PA2, GPIO_PIN_OUTPUT);
		break;
	}
}

VOID EFIAPI PciePowerEn(UINT32 Segment, BOOLEAN Enable)
{
	switch (Segment) {
	case PCIE_SEGMENT_PCIE30X4:
		GpioPinWrite(0, GPIO_PIN_PB2, Enable);
		break;
	}
}

VOID EFIAPI PciePeReset(UINT32 Segment, BOOLEAN Enable)
{
	switch (Segment) {
	case PCIE_SEGMENT_PCIE30X4:
		GpioPinWrite(4, GPIO_PIN_PB6, !Enable);
		break;
	case PCIE_SEGMENT_PCIE20L0:
		GpioPinWrite(4, GPIO_PIN_PA5, !Enable);
		break;
	case PCIE_SEGMENT_PCIE20L1:
		GpioPinWrite(4, GPIO_PIN_PA2, !Enable);
		break;
	}
}

VOID
EFIAPI
HdmiTxIomux (
  IN UINT32  Id
  )
{
  switch (Id) {
    case 0:
      GpioPinSetFunction (4, GPIO_PIN_PC1, 5); // hdmim0_tx0_cec
      GpioPinSetPull (4, GPIO_PIN_PC1, GPIO_PIN_PULL_NONE);
      GpioPinSetFunction (1, GPIO_PIN_PA5, 5); // hdmim0_tx0_hpd
      GpioPinSetPull (1, GPIO_PIN_PA5, GPIO_PIN_PULL_NONE);
      GpioPinSetFunction (4, GPIO_PIN_PB7, 5); // hdmim0_tx0_scl
      GpioPinSetPull (4, GPIO_PIN_PB7, GPIO_PIN_PULL_NONE);
      GpioPinSetFunction (4, GPIO_PIN_PC0, 5); // hdmim0_tx0_sda
      GpioPinSetPull (4, GPIO_PIN_PC0, GPIO_PIN_PULL_NONE);
      break;
    case 1:
      GpioPinSetFunction (2, GPIO_PIN_PC4, 4); // hdmim0_tx1_cec
      GpioPinSetPull (2, GPIO_PIN_PC4, GPIO_PIN_PULL_NONE);
      GpioPinSetFunction (1, GPIO_PIN_PA6, 5); // hdmim0_tx1_hpd
      GpioPinSetPull (1, GPIO_PIN_PA6, GPIO_PIN_PULL_NONE);
      GpioPinSetFunction (3, GPIO_PIN_PC6, 5); // hdmim1_tx1_scl
      GpioPinSetPull (3, GPIO_PIN_PC6, GPIO_PIN_PULL_NONE);
      GpioPinSetFunction (3, GPIO_PIN_PC5, 5); // hdmim1_tx1_sda
      GpioPinSetPull (3, GPIO_PIN_PC5, GPIO_PIN_PULL_NONE);
      break;
  }
}

VOID
EFIAPI
PwmFanIoSetup (
  VOID
  )
{
	// TODO:
}

VOID
EFIAPI
PwmFanSetSpeed (
  IN UINT32  Percentage
  )
{
	// TODO:
}

VOID
EFIAPI
PlatformInitLeds (
  VOID
  )
{
  /* Status indicator */
	// TODO:
}

VOID
EFIAPI
PlatformSetStatusLed (
  IN BOOLEAN  Enable
  )
{
	// TODO:
}

CONST EFI_GUID *
EFIAPI
PlatformGetDtbFileGuid (
  IN UINT32  CompatMode
  )
{
	STATIC CONST EFI_GUID  VendorDtbFileGuid = {
    // DeviceTree/Vendor.inf
    0xd58b4028, 0x43d8, 0x4e97, { 0x87, 0xd4, 0x4e, 0x37, 0x16, 0x13, 0x65, 0x80 }
  };

  switch (CompatMode) {
    case FDT_COMPAT_MODE_VENDOR:
      return &VendorDtbFileGuid;
    case FDT_COMPAT_MODE_MAINLINE:
      return NULL;
  }

  return NULL;
}

VOID
EFIAPI
PlatformEarlyInit (
  VOID
  )
{
  // Configure various things specific to this platform
}
