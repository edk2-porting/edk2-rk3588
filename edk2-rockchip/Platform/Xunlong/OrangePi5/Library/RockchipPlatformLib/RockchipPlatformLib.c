/** @file
*
*  Copyright (c) 2021, Rockchip Limited. All rights reserved.
*  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
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
  RK8XX_VOLTAGE_INIT(MASTER_BUCK1, 850000),
  RK8XX_VOLTAGE_INIT(MASTER_BUCK3, 750000),
  RK8XX_VOLTAGE_INIT(MASTER_BUCK4, 750000),
  RK8XX_VOLTAGE_INIT(MASTER_BUCK5, 850000),
  /* This is not configured in the OrangePi5's Linux device tree
  RK8XX_VOLTAGE_INIT(MASTER_BUCK6, 1100000), */
  RK8XX_VOLTAGE_INIT(MASTER_BUCK7, 2000000),
  RK8XX_VOLTAGE_INIT(MASTER_BUCK8, 3300000),
  RK8XX_VOLTAGE_INIT(MASTER_BUCK10, 1800000),

  RK8XX_VOLTAGE_INIT(MASTER_NLDO1, 750000),
  RK8XX_VOLTAGE_INIT(MASTER_NLDO2, 850000),
  /* The OPi is officially configured for the 837500 voltage, but is still marked as avdd_0v75_s0 in the schematic and Linux device tree. rockchip says this voltage is set to improve HDMI stability. */
  RK8XX_VOLTAGE_INIT(MASTER_NLDO3, 837500),
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
  /* Do not override, set by earlier boot stages. */
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
  /* Do not override, set by earlier boot stages. */
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
      GpioPinSetDirection (3, GPIO_PIN_PB2, GPIO_PIN_OUTPUT);
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
      GpioPinWrite (3, GPIO_PIN_PB2, !Enable);
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
    break;
  case 4:
    GpioPinSetFunction(3, GPIO_PIN_PA6, 9); //i2c4_scl_m0
    GpioPinSetFunction(3, GPIO_PIN_PA5, 9); //i2c4_sda_m0
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
  /* Set GPIO3 PC0 (TYPEC_EN) output high to power Type-C/USB2.0 ports */
  GpioPinWrite (3, GPIO_PIN_PC0, TRUE);
  GpioPinSetDirection (3, GPIO_PIN_PC0, GPIO_PIN_OUTPUT);

  // DEBUG((DEBUG_INFO, "Trying to enable green led\n"));
  // GpioPinWrite (1, GPIO_PIN_PA2, TRUE);
  // GpioPinSetDirection (1, GPIO_PIN_PA2, GPIO_PIN_OUTPUT);
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
  /* Set reset to gpio output mode */
  if(Segment == PCIE_SEGMENT_PCIE20L2) { // M.2 M Key
    GpioPinSetDirection (3, GPIO_PIN_PD1, GPIO_PIN_OUTPUT);
  }
}

VOID
EFIAPI
PciePowerEn (
  UINT32 Segment,
  BOOLEAN Enable
  )
{
  /* nothing to power on */
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
  .ControllerID = PWM_CONTROLLER3,
  .ChannelID = PWM_CHANNEL2,
  .PeriodNs = 4000000,
  .DutyNs = 4000000,
  .Polarity = FALSE,
}; // PWM3_CH2

VOID
EFIAPI
PwmFanIoSetup (
  VOID
  )
{
  GpioPinSetFunction (4, GPIO_PIN_PB2, 0xB); // PWM14_M1
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
  GpioPinWrite (1, GPIO_PIN_PA2, FALSE);
  GpioPinSetDirection (1, GPIO_PIN_PA2, GPIO_PIN_OUTPUT);
}

VOID
EFIAPI
PlatformSetStatusLed (
  IN BOOLEAN Enable
  )
{
  GpioPinWrite (1, GPIO_PIN_PA2, Enable);
}

CONST EFI_GUID *
EFIAPI
PlatformGetDtbFileGuid (
  IN UINT32 CompatMode
  )
{
  STATIC CONST EFI_GUID VendorDtbFileGuid = {         // DeviceTree/Vendor.inf
    0xd58b4028, 0x43d8, 0x4e97, { 0x87, 0xd4, 0x4e, 0x37, 0x16, 0x13, 0x65, 0x80 }
  };
  STATIC CONST EFI_GUID MainlineDtbFileGuid = {       // DeviceTree/Mainline.inf
    0x84492e97, 0xa10f, 0x49a7, { 0x85, 0xe9, 0x02, 0x5d, 0x19, 0x66, 0xb3, 0x43 }
  };

  switch (CompatMode) {
    case FDT_COMPAT_MODE_VENDOR:
      return &VendorDtbFileGuid;
    case FDT_COMPAT_MODE_MAINLINE:
      return &MainlineDtbFileGuid;
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

  GpioPinSetFunction(1, GPIO_PIN_PD5, 0); //jdet
}
