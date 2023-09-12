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
#include <Soc.h>

static struct regulator_init_data rk806_init_data[] = {
  /* Master PMIC */
  RK8XX_VOLTAGE_INIT(MASTER_BUCK1, 750000),
  RK8XX_VOLTAGE_INIT(MASTER_BUCK3, 750000),
  RK8XX_VOLTAGE_INIT(MASTER_BUCK4, 750000),
  RK8XX_VOLTAGE_INIT(MASTER_BUCK5, 850000),
  //RK8XX_VOLTAGE_INIT(MASTER_BUCK6, 750000),
  RK8XX_VOLTAGE_INIT(MASTER_BUCK7, 2000000),
  RK8XX_VOLTAGE_INIT(MASTER_BUCK8, 3300000),
  RK8XX_VOLTAGE_INIT(MASTER_BUCK10, 1800000),

  RK8XX_VOLTAGE_INIT(MASTER_NLDO1, 750000),
  RK8XX_VOLTAGE_INIT(MASTER_NLDO2, 850000),
  RK8XX_VOLTAGE_INIT(MASTER_NLDO3, 750000),
  RK8XX_VOLTAGE_INIT(MASTER_NLDO4, 850000),
  RK8XX_VOLTAGE_INIT(MASTER_NLDO5, 750000),

  RK8XX_VOLTAGE_INIT(MASTER_PLDO1, 1800000),
  RK8XX_VOLTAGE_INIT(MASTER_PLDO2, 1800000),
  RK8XX_VOLTAGE_INIT(MASTER_PLDO3, 1200000),
  RK8XX_VOLTAGE_INIT(MASTER_PLDO4, 3300000),
  RK8XX_VOLTAGE_INIT(MASTER_PLDO5, 3300000),
  RK8XX_VOLTAGE_INIT(MASTER_PLDO6, 1800000),

  /* No dual PMICs on this platform */
};

VOID
EFIAPI
SdmmcIoMux (
  VOID
  )
{
  /* sdmmc0 iomux (microSD socket) */
  BUS_IOC->GPIO4D_IOMUX_SEL_L = (0xFFFFUL << 16) | (0x1111); //SDMMC_D0,SDMMC_D1,SDMMC_D2,SDMMC_D3
  BUS_IOC->GPIO4D_IOMUX_SEL_H = (0x00FFUL << 16) | (0x0011); //SDMMC_CLK,SDMMC_CMD
  PMU1_IOC->GPIO0A_IOMUX_SEL_H = (0x000FUL << 16) | (0x0001); //SDMMC_DET
}

VOID
EFIAPI
SdhciEmmcIoMux (
  VOID
  )
{
  /* sdmmc0 iomux */
  BUS_IOC->GPIO2A_IOMUX_SEL_L = (0xFFFFUL << 16) | (0x1111); //EMMC_CMD,EMMC_CLKOUT,EMMC_DATASTROBE,EMMC_RSTN
  BUS_IOC->GPIO2D_IOMUX_SEL_L = (0xFFFFUL << 16) | (0x1111); //EMMC_D0,EMMC_D1,EMMC_D2,EMMC_D3
  BUS_IOC->GPIO2D_IOMUX_SEL_H = (0xFFFFUL << 16) | (0x1111); //EMMC_D4,EMMC_D5,EMMC_D6,EMMC_D7
}

#define NS_CRU_BASE         0xFD7C0000
#define CRU_CLKSEL_CON59    0x03EC
#define CRU_CLKSEL_CON78    0x0438

VOID
EFIAPI
Rk806SpiIomux (
  VOID
  )
{
  /* io mux */
  //BUS_IOC->GPIO1A_IOMUX_SEL_H = (0xFFFFUL << 16) | 0x8888;
  //BUS_IOC->GPIO1B_IOMUX_SEL_L = (0x000FUL << 16) | 0x0008;
  PMU1_IOC->GPIO0A_IOMUX_SEL_H = (0x0FF0UL << 16) | 0x0110;
  PMU1_IOC->GPIO0B_IOMUX_SEL_L = (0xF0FFUL << 16) | 0x1011;
  MmioWrite32(NS_CRU_BASE + CRU_CLKSEL_CON59, (0x00C0UL << 16) | 0x0080);
}

VOID
EFIAPI
Rk806Configure (
  VOID
  )
{
  UINTN RegCfgIndex;

  RK806Init();

  for (RegCfgIndex = 0; RegCfgIndex < ARRAY_SIZE(rk806_init_data); RegCfgIndex++)
    RK806RegulatorInit(rk806_init_data[RegCfgIndex]);
}

VOID
EFIAPI
SetCPULittleVoltage (
  IN UINT32 Microvolts
  )
{
  struct regulator_init_data Rk806CpuLittleSupply =
        RK8XX_VOLTAGE_INIT(MASTER_BUCK2, Microvolts);

  RK806RegulatorInit(Rk806CpuLittleSupply);
}

VOID
EFIAPI
NorFspiIomux (
  VOID
  )
{
  /* io mux */
  MmioWrite32(NS_CRU_BASE + CRU_CLKSEL_CON78,
             (((0x3 << 12) | (0x3f << 6)) << 16) | (0x0 << 12) | (0x3f << 6));
#define FSPI_M1
#if defined(FSPI_M0)
   /*FSPI M0*/
  BUS_IOC->GPIO2A_IOMUX_SEL_L = ((0xF << 0) << 16) | (2 << 0); //FSPI_CLK_M0
  BUS_IOC->GPIO2D_IOMUX_SEL_L = (0xFFFFUL << 16) | (0x2222); //FSPI_D0_M0,FSPI_D1_M0,FSPI_D2_M0,FSPI_D3_M0
  BUS_IOC->GPIO2D_IOMUX_SEL_H = ((0xF << 8) << 16) | (0x2 << 8); //FSPI_CS0N_M0
#elif defined(FSPI_M1)
  /*FSPI M1*/
  BUS_IOC->GPIO2A_IOMUX_SEL_H = (0xFF00UL << 16) | (0x3300); //FSPI_D0_M1,FSPI_D1_M1
  BUS_IOC->GPIO2B_IOMUX_SEL_L = (0xF0FFUL << 16) | (0x3033); //FSPI_D2_M1,FSPI_D3_M1,FSPI_CLK_M1
  BUS_IOC->GPIO2B_IOMUX_SEL_H = (0xF << 16) | (0x3); //FSPI_CS0N_M1
#else
  /*FSPI M2*/
  BUS_IOC->GPIO3A_IOMUX_SEL_L = (0xFFFFUL << 16) | (0x5555); //[FSPI_D0_M2-FSPI_D3_M2]
  BUS_IOC->GPIO3A_IOMUX_SEL_H = (0xF0UL << 16) | (0x50); //FSPI_CLK_M2
  BUS_IOC->GPIO3C_IOMUX_SEL_H = (0xF << 16) | (0x2); //FSPI_CS0_M2
#endif
}

VOID
EFIAPI
GmacIomux (
  IN UINT32 Id
  )
{
  /* No GMAC here */
}

VOID
EFIAPI
NorFspiEnableClock (
  UINT32 *CruBase
  )
{
  UINTN BaseAddr = (UINTN) CruBase;

  MmioWrite32(BaseAddr + 0x087C, 0x0E000000);
}

VOID
EFIAPI
I2cIomux (
   UINT32 id
  )
{
  switch (id) {
  case 0:
    /* io mux M2 */
    PMU2_IOC->GPIO0D_IOMUX_SEL_L = (0x0F00UL << 16) | 0x0300;
    PMU2_IOC->GPIO0D_IOMUX_SEL_L = (0x00F0UL << 16) | 0x0030;
    break;
  case 1:
    /* io mux */
    //BUS_IOC->GPIO0B_IOMUX_SEL_H = (0x0FF0UL << 16) | 0x0990;
    //PMU2_IOC->GPIO0B_IOMUX_SEL_H = (0x0FF0UL << 16) | 0x0880;
    break;
  case 2:
    /* io mux */
    BUS_IOC->GPIO0B_IOMUX_SEL_H = (0xF000UL << 16) | 0x9000;
    BUS_IOC->GPIO0C_IOMUX_SEL_L = (0x000FUL << 16) | 0x0009;
    PMU2_IOC->GPIO0B_IOMUX_SEL_H = (0xF000UL << 16) | 0x8000;
    PMU2_IOC->GPIO0C_IOMUX_SEL_L = (0x000FUL << 16) | 0x0008;
    break;
  case 3:
    break;
  case 4:
    break;
  case 5:
    break;
  case 6:
    /* io mux M0 */
    BUS_IOC->GPIO0C_IOMUX_SEL_H = (0xF000UL << 16) | 0x9000;
    BUS_IOC->GPIO0D_IOMUX_SEL_L = (0x000FUL << 16) | 0x0009;
    PMU2_IOC->GPIO0C_IOMUX_SEL_H = (0xF000UL << 16) | 0x8000;
    PMU2_IOC->GPIO0D_IOMUX_SEL_L = (0x000FUL << 16) | 0x0008;
    break;
  default:
    break;
  }
}

VOID
EFIAPI
UsbPortPowerEnable (
  VOID
  )
{
  DEBUG((DEBUG_INFO, "UsbPortPowerEnable called\n"));

  /* vcc5v0_host_en */
  GpioPinWrite (3, GPIO_PIN_PB7, TRUE);
  GpioPinSetDirection (3, GPIO_PIN_PB7, GPIO_PIN_OUTPUT);

  /* typec5v_pwren */
  GpioPinWrite (4, GPIO_PIN_PB0, TRUE);
  GpioPinSetDirection (4, GPIO_PIN_PB0, GPIO_PIN_OUTPUT);
}

VOID
EFIAPI
Usb2PhyResume (
  VOID
  )
{
  MmioWrite32(0xfd5d0008, 0x20000000);
  MmioWrite32(0xfd5d4008, 0x20000000);
  MmioWrite32(0xfd5d8008, 0x20000000);
  MmioWrite32(0xfd5dc008, 0x20000000);
  MmioWrite32(0xfd7f0a10, 0x07000700);
  MmioWrite32(0xfd7f0a10, 0x07000000);
}

VOID
EFIAPI
PcieIoInit (
  UINT32 Segment
  )
{
  /* Set reset and power IO to gpio output mode */
  switch(Segment) {
    case PCIE_SEGMENT_PCIE30X4: // M.2 M Key
      /* reset */
      GpioPinSetDirection (4, GPIO_PIN_PB6, GPIO_PIN_OUTPUT);
      /* vcc3v3_pcie30 */
      GpioPinSetDirection (2, GPIO_PIN_PB6, GPIO_PIN_OUTPUT);
      break;
    case PCIE_SEGMENT_PCIE20L0: // M.2 A+E Key
      /* reset */
      GpioPinSetDirection (4, GPIO_PIN_PA5, GPIO_PIN_OUTPUT);
      /* vcc3v3_pcie2x1l0 */
      GpioPinSetDirection (2, GPIO_PIN_PC5, GPIO_PIN_OUTPUT);
      break;
    case PCIE_SEGMENT_PCIE20L1: // RTL8125B
      /* reset */
      GpioPinSetDirection (3, GPIO_PIN_PB3, GPIO_PIN_OUTPUT);
      break;
    case PCIE_SEGMENT_PCIE20L2: // RTL8125B
      /* reset */
      GpioPinSetDirection (4, GPIO_PIN_PA2, GPIO_PIN_OUTPUT);
      break;
    default:
      break;
  }
  if (Segment == PCIE_SEGMENT_PCIE20L1 || Segment == PCIE_SEGMENT_PCIE20L2) {
    /* vcc3v3_pcie_eth */
    GpioPinSetDirection (3, GPIO_PIN_PB4, GPIO_PIN_OUTPUT);
  }
}

VOID
EFIAPI
PciePowerEn (
  UINT32 Segment,
  BOOLEAN Enable
  )
{
  switch(Segment) {
    case PCIE_SEGMENT_PCIE30X4:
      GpioPinWrite (2, GPIO_PIN_PB6, Enable);
      break;
    case PCIE_SEGMENT_PCIE20L0:
      GpioPinWrite (2, GPIO_PIN_PC5, Enable);
      break;
    case PCIE_SEGMENT_PCIE20L1:
    case PCIE_SEGMENT_PCIE20L2:
      /* Yes, disabling one would disable the other as well. */
      GpioPinWrite (3, GPIO_PIN_PB4, !Enable);
      break;
    default:
      break;
  }
}

VOID
EFIAPI
PciePeReset (
  UINT32 Segment,
  BOOLEAN Enable
  )
{
  switch(Segment) {
    case PCIE_SEGMENT_PCIE30X4:
      GpioPinWrite (4, GPIO_PIN_PB6, !Enable);
      break;
    case PCIE_SEGMENT_PCIE20L0:
      GpioPinWrite (4, GPIO_PIN_PA5, !Enable);
      break;
    case PCIE_SEGMENT_PCIE20L1:
      GpioPinWrite (3, GPIO_PIN_PB3, !Enable);
      break;
    case PCIE_SEGMENT_PCIE20L2:
      GpioPinWrite (4, GPIO_PIN_PA2, !Enable);
      break;
    default:
      break;
  }
}

VOID
EFIAPI
PwmFanIoSetup(
  VOID
)
{
}

VOID
EFIAPI
PwmFanSetSpeed(
  UINT32 Percentage
)
{
}

VOID
EFIAPI
PlatformInitLeds (
  VOID
  )
{
  /* Status indicator */
  GpioPinWrite (3, GPIO_PIN_PA6, FALSE);
  GpioPinSetDirection (3, GPIO_PIN_PA6, GPIO_PIN_OUTPUT);
}

VOID
EFIAPI
PlatformSetStatusLed (
  IN BOOLEAN Enable
  )
{
  GpioPinWrite (3, GPIO_PIN_PA6, Enable);
}

VOID
EFIAPI
PlatformEarlyInit (
  VOID
  )
{
  // Configure various things specific to this platform
}
