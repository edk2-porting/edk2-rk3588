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
#include <Soc.h>

void DebugPrintHex(void *buf, UINT32 width, UINT32 len)
{
  UINT32 i,j;
  UINT8 *p8 = (UINT8 *) buf;
  UINT16 *p16 = (UINT16 *) buf;
  UINT32 *p32 =(UINT32 *) buf;

  j = 0;
  for (i = 0; i < len; i++) {
    if (j == 0) {
      DebugPrint(DEBUG_ERROR, "%p + 0x%x:",buf, i * width);
    }

    if (width == 4) {
      DebugPrint(DEBUG_ERROR, "0x%08x,", p32[i]);
    } else if (width == 2) {
      DebugPrint(DEBUG_ERROR, "0x%04x,", p16[i]);
    } else {
      DebugPrint(DEBUG_ERROR, "0x%02x,", p8[i]);
    }

    if (++j >= (16/width)) {
      j = 0;
      DebugPrint(DEBUG_ERROR, "\n","");
    }
  }
  DebugPrint(DEBUG_ERROR, "\n","");
}

void
EFIAPI
DwEmmcDxeIoMux(void)
{
  /* sdmmc0 iomux */
}

void
EFIAPI
SdhciEmmcDxeIoMux(void)
{
  /* sdmmc0 iomux */
  BUS_IOC->GPIO2A_IOMUX_SEL_L = (0xFFFFUL << 16) | (0x1111); //EMMC_CMD,EMMC_CLKOUT,EMMC_DATASTROBE,EMMC_RSTN
  BUS_IOC->GPIO2D_IOMUX_SEL_L = (0xFFFFUL << 16) | (0x1111); //EMMC_D0,EMMC_D1,EMMC_D2,EMMC_D3
  BUS_IOC->GPIO2D_IOMUX_SEL_H = (0xFFFFUL << 16) | (0x1111); //EMMC_D4,EMMC_D5,EMMC_D6,EMMC_D7
}

#define NS_CRU_BASE         0xFD7C0000
#define CRU_CLKSEL_CON59    0x03EC
#define CRU_CLKSEL_CON78    0x0438

void
EFIAPI
Rk806SpiIomux(void)
{
  /* io mux */
  //BUS_IOC->GPIO1A_IOMUX_SEL_H = (0xFFFFUL << 16) | 0x8888;
  //BUS_IOC->GPIO1B_IOMUX_SEL_L = (0x000FUL << 16) | 0x0008;
  PMU1_IOC->GPIO0A_IOMUX_SEL_H = (0x0FF0UL << 16) | 0x0110;
  PMU1_IOC->GPIO0B_IOMUX_SEL_L = (0xF0FFUL << 16) | 0x1011;
  MmioWrite32(NS_CRU_BASE + CRU_CLKSEL_CON59, (0x00C0UL << 16) | 0x0080);
}

void
EFIAPI
NorFspiIomux(void)
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

void
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

void
EFIAPI
NorFspiEnableClock (
  UINT32 *CruBase
)
{
  UINTN BaseAddr = (UINTN) CruBase;

  MmioWrite32(BaseAddr + 0x087C, 0x0E000000);
}

UINT32
EFIAPI
I2cGetBase (
   UINT32 id
)
{
  UINT32 Base = 0;

  switch (id) {
  case 0:
    Base = 0xFD880000;
    break;
  case 1:
    Base = 0xFEA90000;
    /* io mux */
    //BUS_IOC->GPIO0B_IOMUX_SEL_H = (0x0FF0UL << 16) | 0x0990;
    //PMU2_IOC->GPIO0B_IOMUX_SEL_H = (0x0FF0UL << 16) | 0x0880;
    break;
  case 2:
    Base = 0xFEAA0000;
    /* io mux */
    BUS_IOC->GPIO0B_IOMUX_SEL_H = (0xF000UL << 16) | 0x9000;
    BUS_IOC->GPIO0C_IOMUX_SEL_L = (0x000FUL << 16) | 0x0009;
    PMU2_IOC->GPIO0B_IOMUX_SEL_H = (0xF000UL << 16) | 0x8000;
    PMU2_IOC->GPIO0C_IOMUX_SEL_L = (0x000FUL << 16) | 0x0008;
    break;
  case 3:
    Base = 0xFEAB0000;
    break;
  case 4:
    Base = 0xFEAC0000;
    break;
  case 5:
    Base = 0xFEAD0000;
    break;
  default:
    break;
  }

  return Base;
}

#define GPIO4_BASE         0xFEC50000
#define GPIO_SWPORT_DR_L   0x0000
#define GPIO_SWPORT_DR_H   0x0004
#define GPIO_SWPORT_DDR_L  0x0008
#define GPIO_SWPORT_DDR_H  0x000C

void
EFIAPI
UsbPortPowerEnable (void)
{
  /* enable usb host vbus supply */
  MmioWrite32(GPIO4_BASE + GPIO_SWPORT_DR_L, (0x0100UL << 16) | 0x0100);
  MmioWrite32(GPIO4_BASE + GPIO_SWPORT_DDR_L, (0x0100UL << 16) | 0x0100);
  /* enable usb otg0 vbus supply */
  MmioWrite32(GPIO4_BASE + GPIO_SWPORT_DR_H, (0x0100UL << 16) | 0x0100);
  MmioWrite32(GPIO4_BASE + GPIO_SWPORT_DDR_H, (0x0100UL << 16) | 0x0100);
}

void
EFIAPI
Usb2PhySuspend (void)
{
  MmioWrite32(0xfd5d4008, 0x20000000);
  MmioWrite32(0xfd5d8008, 0x20000000);
  MmioWrite32(0xfd5dc008, 0x20000000);
}

void
EFIAPI
Usb2PhyResume (void)
{
  MmioWrite32(0xfd5d4008, 0x20000000);
  MmioWrite32(0xfd5d8008, 0x20000000);
  MmioWrite32(0xfd5dc008, 0x20000000);
  MmioWrite32(0xfd7f0a10, 0x07000700);
  MmioWrite32(0xfd7f0a10, 0x07000000);
}

void
EFIAPI
UdPhyU3PortDisable (void)
{
  /* disable U3 port */
  MmioWrite32 (0xfd5ac01c, 0xf08d0089);
  MmioWrite32 (0xfd5ac034, 0xf08d0089);
  /* remove rx-termination */
  MmioWrite32 (0xfd5c800c, 0x00030001);
  MmioWrite32 (0xfd5cc00c, 0x00030001);
}

void
EFIAPI
Dwc3Force20ClkFor30Clk (UINT32 Address, BOOLEAN enable)
{
  UINT32 Reg;

  if (enable) {
    Reg = MmioRead32(Address);
    Reg |= (1 << 26);
    MmioWrite32((Address), Reg);
  } else {
    Reg = MmioRead32(Address);
    Reg &= ~(1 << 26);
    MmioWrite32(Address, Reg);
  }
}

void
EFIAPI
Pcie30IoInit(void)
{
    /* Set reset and power IO to gpio output mode */
    MmioWrite32(0xFD5F808C, 0xf << (8 + 16)); /* gpio4b6 to gpio mode -> reset */
    MmioWrite32(0xFEC50008, 0x40004000); /* output */ 

    MmioWrite32(0xFD5F8070, 0xf << (12 + 16)); /* gpio3c3 to gpio mode -> power */
    MmioWrite32(0xFEC4000c, 0x80008); /* output */
}

void
EFIAPI
Pcie30PowerEn(void)
{
    MmioWrite32(0xFEC40004, 0x80008); /* output high to enable power */
}

void
EFIAPI
Pcie30PeReset(BOOLEAN enable)
{
    if(enable)
        MmioWrite32(0xFEC50000, 0x40000000); /* output low */
    else
        MmioWrite32(0xFEC50000, 0x40004000); /* output high */ 
}

