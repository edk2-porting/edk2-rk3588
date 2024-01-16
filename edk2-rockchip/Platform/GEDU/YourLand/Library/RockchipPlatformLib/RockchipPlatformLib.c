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
#include <Library/PcdLib.h>
//geduer-23-11-28 
#include <VarStoreData.h>
#include <Library/GeduSdPort.h>
#include <Library/TimerLib.h>

unsigned int gd_flags = 0;
VOID PlatformInitGPIOs();
VOID PlatformDumpGPIOs();

void GdDbgLight(int FlashingTimes, int KeepOn)
{
  GpioPinSetDirection (3, GPIO_PIN_PD3, 1/*GPIO_PIN_OUTPUT*/);
  for ( int i = 0; i< FlashingTimes; i++){
    GpioPinWrite (3, GPIO_PIN_PD3, 1);
    MicroSecondDelay(1000);
    GpioPinWrite (3, GPIO_PIN_PD3, 0);
  }  
  if(KeepOn)
    GpioPinWrite (3, GPIO_PIN_PD3, 1);

  GpioPinWrite (3, GPIO_PIN_PD4, 1);
}

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

static struct regulator_init_data rk806_master[] = {
	RK8XX_VOLTAGE_INIT(MASTER_BUCK1, 750000),
	RK8XX_VOLTAGE_INIT(MASTER_BUCK2, 750000),
	RK8XX_VOLTAGE_INIT(MASTER_BUCK3, 750000),
	RK8XX_VOLTAGE_INIT(MASTER_BUCK4, 750000),
	RK8XX_VOLTAGE_INIT(MASTER_BUCK5, 750000),
	RK8XX_VOLTAGE_INIT(MASTER_BUCK6, 750000),
	RK8XX_VOLTAGE_INIT(MASTER_BUCK7, 2000000),
	RK8XX_VOLTAGE_INIT(MASTER_BUCK8, 750000),
	RK8XX_VOLTAGE_INIT(MASTER_BUCK10, 1100000),

	RK8XX_VOLTAGE_INIT(MASTER_NLDO1, 750000),
	RK8XX_VOLTAGE_INIT(MASTER_NLDO2, 900000),
	RK8XX_VOLTAGE_INIT(MASTER_NLDO3, 750000),
	RK8XX_VOLTAGE_INIT(MASTER_NLDO4, 750000),
	RK8XX_VOLTAGE_INIT(MASTER_NLDO5, 850000),

	RK8XX_VOLTAGE_INIT(MASTER_PLDO1, 1800000),
	RK8XX_VOLTAGE_INIT(MASTER_PLDO2, 1800000),
	RK8XX_VOLTAGE_INIT(MASTER_PLDO3, 1800000),
	RK8XX_VOLTAGE_INIT(MASTER_PLDO4, 3300000),
	RK8XX_VOLTAGE_INIT(MASTER_PLDO5, 1800000),
	RK8XX_VOLTAGE_INIT(MASTER_PLDO6, 1800000),
};

static struct regulator_init_data rk806_slaver[] = {
	RK8XX_VOLTAGE_INIT(SLAVER_BUCK1, 750000),
	RK8XX_VOLTAGE_INIT(SLAVER_BUCK2, 750000),
	RK8XX_VOLTAGE_INIT(SLAVER_BUCK3, 750000),
	RK8XX_VOLTAGE_INIT(SLAVER_BUCK4, 3300000),
	RK8XX_VOLTAGE_INIT(SLAVER_BUCK5, 750000),
	RK8XX_VOLTAGE_INIT(SLAVER_BUCK6, 750000),
	RK8XX_VOLTAGE_INIT(SLAVER_BUCK7, 1800000),
	RK8XX_VOLTAGE_INIT(SLAVER_BUCK8, 750000),
	RK8XX_VOLTAGE_INIT(SLAVER_BUCK10, 850000),

	RK8XX_VOLTAGE_INIT(SLAVER_NLDO1, 750000),
	RK8XX_VOLTAGE_INIT(SLAVER_NLDO2, 850000),
	RK8XX_VOLTAGE_INIT(SLAVER_NLDO3, 850000),
	RK8XX_VOLTAGE_INIT(SLAVER_NLDO4, 1200000),
	RK8XX_VOLTAGE_INIT(SLAVER_NLDO5, 1200000),

	RK8XX_VOLTAGE_INIT(SLAVER_PLDO1, 1800000),
	RK8XX_VOLTAGE_INIT(SLAVER_PLDO2, 1800000),
	RK8XX_VOLTAGE_INIT(SLAVER_PLDO3, 1800000),
	RK8XX_VOLTAGE_INIT(SLAVER_PLDO4, 3300000),
	RK8XX_VOLTAGE_INIT(SLAVER_PLDO5, 2800000),
	RK8XX_VOLTAGE_INIT(SLAVER_PLDO6, 1800000),
};

BOOLEAN UlIsUlan()
{
  unsigned int mb = PcdGet32(PcdPlatformID);
  //unsigned int mb = 16*1024;
  BOOLEAN isUlan = (mb == 0) || (mb > (8*1024));
  DEBUG((DEBUG_INFO, "%dMB DDR. %s\n", mb, isUlan?"YourLand":"RK3588"));

  return isUlan; 
}

VOID
EFIAPI
Rk806Configure (
  VOID
  )
{
  UINTN RegCfgIndex;
  INT32 i;
  
  RK806Init();
// #ifdef TRY_POWER_BUG
  if(UlIsUlan()) {
    DEBUG((DEBUG_INFO, "Running on YourLand\n"));
    for (i = 0; i < ARRAY_SIZE(rk806_master); i++)
      RK806RegulatorInit(rk806_master[i]);
  
    GdRk806InitPowerdown(0);
  
    for (i = 0; i < ARRAY_SIZE(rk806_slaver); i++)
      RK806RegulatorInit(rk806_slaver[i]);

    GdRk806InitPowerdown(1);

  } else {
    DEBUG((DEBUG_INFO, "Seting RK806 + RK860 regulators\n"));
    for (RegCfgIndex = 0; RegCfgIndex < ARRAY_SIZE(rk806_init_data); RegCfgIndex++)
      RK806RegulatorInit(rk806_init_data[RegCfgIndex]);
  }
// #else 
// #endif    
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
  DEBUG((DEBUG_INFO, "UsbPortPowerEnable called. 1126\n"));
  // USB_HOST_PWREN_H [25]
  GpioPinSetDirection (4, GPIO_PIN_PB1, GPIO_PIN_OUTPUT);
  GpioPinWrite (4, GPIO_PIN_PB1, TRUE);

  /* vcc5v0_host_en2 */
  GpioPinSetDirection (3, GPIO_PIN_PD2, GPIO_PIN_OUTPUT);
  GpioPinWrite (3, GPIO_PIN_PD2, TRUE);

  /* vcc5v0_host_en1 consulted to HW, for future extension, not used now*/
  // GpioPinSetDirection (4, GPIO_PIN_PC3, GPIO_PIN_OUTPUT);
  // GpioPinWrite (4, GPIO_PIN_PC3, TRUE);

  /* typec5v_pwren */
  GpioPinSetDirection (3, GPIO_PIN_PD5, GPIO_PIN_OUTPUT);
  GpioPinWrite (3, GPIO_PIN_PD5, TRUE);
  
  PlatformInitGPIOs();
  PlatformDumpGPIOs();
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
  MmioWrite32(0xFEC4000c, 0x80008); /* output */
#if 0  
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
#endif  
}

VOID
EFIAPI
PciePowerEn (
  UINT32 Segment,
  BOOLEAN Enable
  )
{
  DEBUG((DEBUG_INFO, "PciePowerEn(%d, %d) called\n", Segment, Enable));
  // GpioPinWrite (2, GPIO_PIN_PC3, Enable);
  MmioWrite32(0xFEC40004, 0x80008); /* output high to enable power */
/*  
  switch(Segment) {
    case PCIE_SEGMENT_PCIE30X4:
      GpioPinWrite (2, GPIO_PIN_PB6, Enable);
      break;
    case PCIE_SEGMENT_PCIE20L0:
      GpioPinWrite (2, GPIO_PIN_PC5, Enable);
      break;
    case PCIE_SEGMENT_PCIE20L1:
    case PCIE_SEGMENT_PCIE20L2:
      // Yes, disabling one would disable the other as well. 
      GpioPinWrite (3, GPIO_PIN_PB4, !Enable);
      break;
    default:
      break;
  }
*/  
}

VOID
EFIAPI
PciePeReset (
  UINT32 Segment,
  BOOLEAN Enable
  )
{
  DEBUG((DEBUG_INFO, "PciePeReset(%d, %d) called\n", Segment, Enable));
  // GpioPinWrite (2, GPIO_PIN_PC3, Enable);

  if(Enable)
    MmioWrite32(0xFEC50000, 0x40000000); /* output low */
  else
    MmioWrite32(0xFEC50000, 0x40004000); /* output high */

#ifdef GD_TRY_PCIE_POWER_BUG  
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
#endif  
}

PWM_DATA pwm_data = {
  .ControllerID = PWM_CONTROLLER0,
  .ChannelID = PWM_CHANNEL3,
  .PeriodNs = 4000000,
  .DutyNs = 4000000,
  .Polarity = FALSE,
}; // PWM0_CH3

VOID
EFIAPI
PwmFanIoSetup (
  VOID
  )
{
  // GpioPinSetFunction (3, GPIO_PIN_PB2, 0xB); // PWM3_IR_M1
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
  /* Status indicator */
  GpioPinSetDirection (3, GPIO_PIN_PC1, GPIO_PIN_OUTPUT);
  GpioPinWrite (3, GPIO_PIN_PC1, FALSE);
  // GPIO3_PD3 charge OK of YL
  GpioPinSetDirection (3, GPIO_PIN_PD3, GPIO_PIN_OUTPUT);
  GpioPinWrite (3, GPIO_PIN_PD3, TRUE); // TEST only!!!
}

VOID
EFIAPI
PlatformSetStatusLed (
  IN BOOLEAN Enable
  )
{
  GpioPinWrite (3, GPIO_PIN_PC1, Enable);
}

VOID PlatformInitGPIOs()
{
	//gpio1
  //image
  MmioWrite32(0xFEC20000,0xFFFF0400);
  MmioWrite32(0xFEC20004,0xFFFF4010);
  MmioWrite32(0xFEC20008,0xFFFF0400);
  MmioWrite32(0xFEC2000c,0xFFFF4010);
  //gpio2
  MmioWrite32(0xFEC30000,0xFFFFe000);
  MmioWrite32(0xFEC30004,0xFFFF0000);
  MmioWrite32(0xFEC30008,0xFFFFe000);
  MmioWrite32(0xFEC3000c,0xFFFF0000);
	//gpio3
  //MmioWrite32(0xFEC40000,0xFFFF0240);
  //MmioWrite32(0xFEC40004,0xFFFF0443);
  //MmioWrite32(0xFEC40008,0xFFFF2384);
  //MmioWrite32(0xFEC4000c,0xFFFF244b);
  // according dump from yl04
  MmioWrite32(0xFEC40000,0xFFFF0240);
  MmioWrite32(0xFEC40004,0xFFFF144b);
  MmioWrite32(0xFEC40008,0xFFFF0200);
  MmioWrite32(0xFEC4000c,0xFFFF3c4b);
  //gpio4
  //Backlight
  MmioWrite32(0xFEC50000,0xFFFF2100);
  MmioWrite32(0xFEC50004,0xFFFF0018);
  MmioWrite32(0xFEC50008,0xFFFF21c0);
  MmioWrite32(0xFEC5000C,0xFFFF0018);
}
/*
**GPIO0 0x0 0x50 0x0 0x50
**GPIO1 0x0 0x0 0x0 0x0
**GPIO2 0x0 0x2C08 0x0 0x2C0A
**GPIO3 0x6A00 0x0 0x2A00 0x0
**GPIO4 0x0 0x0 0x0 0x0
*/
VOID PlatformDumpGPIOs()
{
  UINT32 i, v[4], base;
  for(i = 0; i<4; i++) {
    base = 0xFEC20000 + i*0x10000;
    v[0] = MmioRead32(base + 0);
    v[1] = MmioRead32(base + 4);
    v[2] = MmioRead32(base + 8);
    v[3] = MmioRead32(base + 0xc);
    DEBUG ((DEBUG_INFO, "**GPIO%d@%x 0x%0x 0x%0x 0x%0x 0x%0x\n", i+1, base, v[0], v[1], v[2], v[3]));
  }
}

VOID
EFIAPI
PlatformEarlyInit (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "YL Platform Early Init starts\n"));

  // Configure various things specific to this platform
  //geduer-23-11-28
  DEBUG ((DEBUG_INFO, "GDDBG get new mode %x\n", PcdGet32(PcdSDSocketMode)));
  if(PcdGet32(PcdSDSocketMode) != SERIAL_PORT_AND_JTAG){
    DEBUG ((DEBUG_INFO, "GDDBG will disable jtag\n"));
    gdsd_enable_jtag(0); 
    // we enable jtag in DebugAgentLibNdb, and disable it if user wants
  }
  // GdDbgLight(5, 0);
}

VOID
EFIAPI
EnableBacklight (
  IN BOOLEAN en
  )
{
  // moved to PlatformEarlyInit
  // PlatformInitGPIOs();

  // LCD_PWR_ON
  GpioPinSetDirection (1, GPIO_PIN_PC4, GPIO_PIN_OUTPUT);
  GpioPinWrite (1, GPIO_PIN_PC4, en);
  // LCD_HPD
  GpioPinSetDirection (1, GPIO_PIN_PC6, GPIO_PIN_OUTPUT);
  GpioPinWrite (1, GPIO_PIN_PC6, en);
  // LCD_BL_EN [29]
  GpioPinSetDirection (4, GPIO_PIN_PB5, GPIO_PIN_OUTPUT);
  GpioPinWrite (4, GPIO_PIN_PB5, en);  
  
  DEBUG ((DEBUG_INFO, "YL backlight is set to %d. rev.GD.1130.15\n", en));
};

VOID
EFIAPI
EnablePWM (
  IN BOOLEAN en
  )
{
  // LCD_BL_PWM [29]
  GpioPinSetDirection (4, GPIO_PIN_PB3, GPIO_PIN_OUTPUT);
  GpioPinWrite (4, GPIO_PIN_PB3, en);
};

