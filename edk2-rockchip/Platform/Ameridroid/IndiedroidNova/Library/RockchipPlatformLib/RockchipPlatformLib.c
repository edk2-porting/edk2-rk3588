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
#include <Soc.h>

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
DwEmmcDxeIoMux (
  VOID
  )
{
  /* sdmmc0 iomux */
}

VOID
EFIAPI
SdhciEmmcDxeIoMux (
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
   UINT32 id
  )
{
  switch (id) {
  case 0:
    /* gmac0 iomux */
    BUS_IOC->GPIO2A_IOMUX_SEL_H = (0xFF00UL << 16) | 0x1100;
    BUS_IOC->GPIO2B_IOMUX_SEL_L = (0xFFFFUL << 16) | 0x1111;
    BUS_IOC->GPIO2B_IOMUX_SEL_H = (0xFF00UL << 16) | 0x1100;
    BUS_IOC->GPIO2C_IOMUX_SEL_L = (0xFFFFUL << 16) | 0x1111;
    BUS_IOC->GPIO4C_IOMUX_SEL_L = (0x0F00UL << 16) | 0x0100;
    BUS_IOC->GPIO4C_IOMUX_SEL_H = (0x00FFUL << 16) | 0x0011;
    break;
  case 1:
    /* gmac1 iomux */
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
  DEBUG((EFI_D_WARN, "UsbPortPowerEnable called\n"));

  /* Set GPIO4 PA5 output high to enable USB-C VBUS */
  GpioPinWrite (4, GPIO_PIN_PA5, TRUE);
  GpioPinSetDirection (4, GPIO_PIN_PA5, GPIO_PIN_OUTPUT);
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
UsbDpPhyEnable (
  VOID
  )
{
  /* enable rx_lfps_en & usbdp_low_pwrn */
  MmioWrite32(0xfd5c8004, 0x60006000);
  MmioWrite32(0xfd5cc004, 0x60006000);
  
  /* remove rx-termination, we don't support SS yet */
  MmioWrite32 (0xfd5c800c, 0x00030001);
  MmioWrite32 (0xfd5cc00c, 0x00030001);
}

VOID
EFIAPI
Pcie30IoInit (
  VOID
  )
{
  /* Set reset and power IO to gpio output mode */
  GpioPinSetDirection (4, GPIO_PIN_PB6, GPIO_PIN_OUTPUT);
  GpioPinSetDirection (1, GPIO_PIN_PA4, GPIO_PIN_OUTPUT);
}

VOID
EFIAPI
Pcie30PowerEn (
  VOID
  )
{
  /* output high to enable power */
  GpioPinWrite (1, GPIO_PIN_PA4, TRUE);
}

VOID
EFIAPI
Pcie30PeReset (
  BOOLEAN enable
  )
{
  if(enable)
    GpioPinWrite (4, GPIO_PIN_PB6, FALSE); /* output low */
  else
    GpioPinWrite (4, GPIO_PIN_PB6, TRUE); /* output high */
}

VOID
EFIAPI
PlatformEarlyInit (
  VOID
  )
{
  // Configure various things specific to this platform
}
