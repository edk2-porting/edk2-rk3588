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
#include <Library/UefiBootServicesTableLib.h>
#include "../../onboard.h"

static struct regulator_init_data rk806_init_data[] = {
  /* Master PMIC */
  RK8XX_VOLTAGE_INIT(MASTER_BUCK1, 750000),
  RK8XX_VOLTAGE_INIT(MASTER_BUCK3, 750000),
  RK8XX_VOLTAGE_INIT(MASTER_BUCK4, 750000),
  RK8XX_VOLTAGE_INIT(MASTER_BUCK5, 850000),
 // RK8XX_VOLTAGE_INIT(MASTER_BUCK6, 750000),
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
  switch (Id) {
    case 1:
      /* gmac1 iomux */
      BUS_IOC->GPIO3B_IOMUX_SEL_H = (0x0FFFUL << 16) | 0x0111;
      BUS_IOC->GPIO3A_IOMUX_SEL_L = (0xFFFFUL << 16) | 0x1111;
      BUS_IOC->GPIO3B_IOMUX_SEL_L = (0xF0FFUL << 16) | 0x1011;
      BUS_IOC->GPIO3A_IOMUX_SEL_H = (0xF0FFUL << 16) | 0x1011;
      BUS_IOC->GPIO3C_IOMUX_SEL_L = (0xFF00UL << 16) | 0x1100;

      /* phy1 reset */
      GpioPinSetDirection (0, GPIO_PIN_PD3, GPIO_PIN_OUTPUT);
      break;
    default:
      break;
  }
}

VOID
EFIAPI
GmacIoPhyReset (
  UINT32   Id,
  BOOLEAN  Enable
  )
{
  switch (Id) {
    case 1:
      /* phy1 reset */
      GpioPinWrite (0, GPIO_PIN_PD3, !Enable);
      break;
    default:
      break;
  }
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
    GpioPinSetFunction(0, GPIO_PIN_PD1, 3); //i2c0_scl_m2
    GpioPinSetFunction(0, GPIO_PIN_PD2, 3); //i2c0_sda_m2
    break;
  case 1:
    break;
  case 2:
    GpioPinSetFunction(0, GPIO_PIN_PB7, 9); //i2c2_scl_m0
    GpioPinSetFunction(0, GPIO_PIN_PC0, 9); //i2c2_sda_m0
    break;
  case 3:
    GpioPinSetFunction(1, GPIO_PIN_PC1, 9); //i2c3_scl_m0
    GpioPinSetFunction(1, GPIO_PIN_PC0, 9); //i2c3_sda_m0
    break;
  case 4:
    GpioPinSetFunction(1, GPIO_PIN_PA3, 9); //i2c4_scl_m3 
    GpioPinSetFunction(1, GPIO_PIN_PA2, 9); //i2c4_sda_m3
    break;
  case 5:
    break;
  case 6:
    GpioPinSetFunction(4, GPIO_PIN_PB1, 9); //i2c6_scl_m3
    GpioPinSetFunction(4, GPIO_PIN_PB0, 9); //i2c6_sda_m3
    break;
  case 7:
    GpioPinSetFunction(1, GPIO_PIN_PD0, 9); //i2c7_scl_m0
    GpioPinSetFunction(1, GPIO_PIN_PD1, 9); //i2c7_sda_m0
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

  /* Enable USB-C VBUS */
  GpioPinWrite (1, GPIO_PIN_PB1, TRUE);
  GpioPinSetDirection (1, GPIO_PIN_PB1, GPIO_PIN_OUTPUT);

  //
  // Power cycle vcc5v0_host as some USB 3.0 devices won't enumerate
  // during boot otherwise.
  //
  GpioPinSetDirection (1, GPIO_PIN_PB6, GPIO_PIN_OUTPUT);
  GpioPinWrite (1, GPIO_PIN_PB6, FALSE);
  gBS->Stall (1200000);
  GpioPinWrite (1, GPIO_PIN_PB6, TRUE);
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
  if(Segment == PCIE_SEGMENT_PCIE20L2) { // M.2 M Key
    /* reset */
    GpioPinSetDirection (3, GPIO_PIN_PD1, GPIO_PIN_OUTPUT);
    /* vcc3v3_pcie20 */
    GpioPinSetDirection (1, GPIO_PIN_PD7, GPIO_PIN_OUTPUT);
  }
}

VOID
EFIAPI
PciePowerEn (
  UINT32 Segment,
  BOOLEAN Enable
  )
{
  if(Segment == PCIE_SEGMENT_PCIE20L2) {
    /* vcc3v3_pcie20 */
    GpioPinWrite (1, GPIO_PIN_PD7, Enable);
  }
}

VOID
EFIAPI
PciePeReset (
  UINT32 Segment,
  BOOLEAN Enable
  )
{
  if(Segment == PCIE_SEGMENT_PCIE20L2) {
    GpioPinWrite (3, GPIO_PIN_PD1, !Enable);
  }
}

PWM_DATA pwm_data = {
  .ControllerID = PWM_CONTROLLER2,
  .ChannelID = PWM_CHANNEL3,
  .PeriodNs = 50000,
  .DutyNs = 50000,
  .Polarity = TRUE,
}; // PWM2_CH3

VOID
EFIAPI
PwmFanIoSetup (
  VOID
  )
{
  GpioPinSetFunction (3, GPIO_PIN_PD5, 0xB); // PWM11_IR_M3
  RkPwmSetConfig (&pwm_data);
  RkPwmEnable (&pwm_data);
}

VOID
EFIAPI
PwmFanSetSpeed (
  IN UINT32 Percentage
  )
{
  pwm_data.DutyNs = pwm_data.PeriodNs * Percentage / 100;
  RkPwmSetConfig (&pwm_data);
}

VOID
EFIAPI
PlatformInitLeds (
  VOID
  )
{
  /* Red off, Green for status, Blue for power */
  GpioPinWrite (3, GPIO_PIN_PB2, FALSE);
  GpioPinSetDirection (3, GPIO_PIN_PB2, GPIO_PIN_OUTPUT);
  GpioPinWrite (3, GPIO_PIN_PC0, FALSE);
  GpioPinSetDirection (3, GPIO_PIN_PC0, GPIO_PIN_OUTPUT);
  GpioPinWrite (1, GPIO_PIN_PD5, TRUE);
  GpioPinSetDirection (1, GPIO_PIN_PD5, GPIO_PIN_OUTPUT);
}

VOID
EFIAPI
PlatformSetStatusLed (
  IN BOOLEAN Enable
  )
{
  GpioPinWrite (3, GPIO_PIN_PC0, Enable);
}

VOID
EFIAPI
PlatformEarlyInit (
  VOID
  )
{
  GpioPinSetFunction(1, BOARD_CODEC_GPIO_PIN, 0); //jdet
}
