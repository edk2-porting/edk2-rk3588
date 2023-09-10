/** @file
 *
 *  RK3588 GPIO Library.
 * 
 *  Copyright (c) 2021, Jared McNeill <jmcneill@invisible.ca>
 *  Copyright (c) 2023, Molly Sophia <mollysophia379@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/GpioLib.h>

#define GRF_GPIO_IOMUX_REG(Pin)         (((Pin) / 4) * 4)
#define GRF_GPIO_IOMUX_SHIFT(Pin)       (((Pin) % 4) * 4)
#define GRF_GPIO_IOMUX_MASK(Pin)        (0xFU << (GRF_GPIO_IOMUX_SHIFT(Pin) + 16))

#define GRF_GPIO_P_REG(Pin)             (((Pin) / 8) * 4)
#define GRF_GPIO_P_SHIFT(Pin)           (((Pin) % 8) * 2)
#define GRF_GPIO_P_MASK(Pin)            (0x3U << (GRF_GPIO_P_SHIFT (Pin) + 16))

#define GRF_GPIO_DS_REG(Pin)            (((Pin) / 4) * 4)
#define GRF_GPIO_DS_SHIFT(Pin)          (((Pin) % 4) * 4)
#define GRF_GPIO_DS_MASK(Pin)           (0xFFU << (GRF_GPIO_DS_SHIFT (Pin) + 16))

#define GRF_GPIO_IE_REG(Pin)            (((Pin) / 8) * 4)
#define GRF_GPIO_IE_SHIFT(Pin)          (((Pin) % 8) * 2)
#define GRF_GPIO_IE_MASK(Pin)           (0x3U << (GRF_GPIO_IE_SHIFT (Pin) + 16))

#define GPIO_SWPORT_DR(Pin)             ((Pin) < 16 ? 0x0000 : 0x0004)
#define GPIO_SWPORT_DDR(Pin)            ((Pin) < 16 ? 0x0008 : 0x000C)

#define GPIO_WRITE_MASK(Pin)            (1U << (((Pin) % 16) + 16))
#define GPIO_VALUE_MASK(Pin, Value)     ((UINT32)Value << ((Pin) % 16))

#define GPIO_EXT_PORT                   0x0070

#define GPIO_NGROUPS                    5

#define PMU1_IOC_BASE     (0xFD5F0000U)
#define PMU2_IOC_BASE     (PMU1_IOC_BASE + 0x4000)
#define BUS_IOC_BASE      (PMU1_IOC_BASE + 0x8000)
#define VCCIO1_4_IOC_BASE (PMU1_IOC_BASE + 0x9000)
#define VCCIO3_5_IOC_BASE (PMU1_IOC_BASE + 0xA000)
#define VCCIO2_IOC_BASE   (PMU1_IOC_BASE + 0xB000)
#define VCCIO6_IOC_BASE   (PMU1_IOC_BASE + 0xC000)
#define EMMC_IOC_BASE     (PMU1_IOC_BASE + 0xD000)

typedef struct {
  UINT8 Group;
  UINT8 Pin;
  EFI_PHYSICAL_ADDRESS Addr;
} GPIO_REG;

STATIC GPIO_REG mDsReg[] = {
  {0, GPIO_PIN_PA0, PMU1_IOC_BASE + 0x0010},
  {0, GPIO_PIN_PA4, PMU1_IOC_BASE + 0x0014},
  {0, GPIO_PIN_PB0, PMU1_IOC_BASE + 0x0018},
  {0, GPIO_PIN_PB4, PMU2_IOC_BASE + 0x0014},
  {0, GPIO_PIN_PC0, PMU2_IOC_BASE + 0x0018},
  {0, GPIO_PIN_PC4, PMU2_IOC_BASE + 0x001C},
  {0, GPIO_PIN_PD0, PMU2_IOC_BASE + 0x0020},
  {0, GPIO_PIN_PD4, PMU2_IOC_BASE + 0x0024},
  {1, GPIO_PIN_PA0, VCCIO1_4_IOC_BASE + 0x0020},
  {1, GPIO_PIN_PA4, VCCIO1_4_IOC_BASE + 0x0024},
  {1, GPIO_PIN_PB0, VCCIO1_4_IOC_BASE + 0x0028},
  {1, GPIO_PIN_PB4, VCCIO1_4_IOC_BASE + 0x002C},
  {1, GPIO_PIN_PC0, VCCIO1_4_IOC_BASE + 0x0030},
  {1, GPIO_PIN_PC4, VCCIO1_4_IOC_BASE + 0x0034},
  {1, GPIO_PIN_PD0, VCCIO1_4_IOC_BASE + 0x0038},
  {1, GPIO_PIN_PD4, VCCIO1_4_IOC_BASE + 0x003C},
  {2, GPIO_PIN_PA0, EMMC_IOC_BASE + 0x0040},
  {2, GPIO_PIN_PA4, VCCIO3_5_IOC_BASE + 0x0044},
  {2, GPIO_PIN_PB0, VCCIO3_5_IOC_BASE + 0x0048},
  {2, GPIO_PIN_PB4, VCCIO3_5_IOC_BASE + 0x004C},
  {2, GPIO_PIN_PC0, VCCIO3_5_IOC_BASE + 0x0050},
  {2, GPIO_PIN_PC4, VCCIO3_5_IOC_BASE + 0x0054},
  {2, GPIO_PIN_PD0, EMMC_IOC_BASE + 0x0058},
  {2, GPIO_PIN_PD4, EMMC_IOC_BASE + 0x005C},
  {3, GPIO_PIN_PA0, VCCIO3_5_IOC_BASE + 0x0060},
  {3, GPIO_PIN_PA4, VCCIO3_5_IOC_BASE + 0x0064},
  {3, GPIO_PIN_PB0, VCCIO3_5_IOC_BASE + 0x0068},
  {3, GPIO_PIN_PB4, VCCIO3_5_IOC_BASE + 0x006C},
  {3, GPIO_PIN_PC0, VCCIO3_5_IOC_BASE + 0x0070},
  {3, GPIO_PIN_PC4, VCCIO3_5_IOC_BASE + 0x0074},
  {3, GPIO_PIN_PD0, VCCIO3_5_IOC_BASE + 0x0078},
  {3, GPIO_PIN_PD4, VCCIO3_5_IOC_BASE + 0x007C},
  {4, GPIO_PIN_PA0, VCCIO6_IOC_BASE + 0x0080},
  {4, GPIO_PIN_PA4, VCCIO6_IOC_BASE + 0x0084},
  {4, GPIO_PIN_PB0, VCCIO6_IOC_BASE + 0x0088},
  {4, GPIO_PIN_PB4, VCCIO6_IOC_BASE + 0x008C},
  {4, GPIO_PIN_PC0, VCCIO6_IOC_BASE + 0x0090},
  {4, GPIO_PIN_PC2, VCCIO3_5_IOC_BASE + 0x0090},
  {4, GPIO_PIN_PC4, VCCIO3_5_IOC_BASE + 0x0094},
  {4, GPIO_PIN_PD0, VCCIO2_IOC_BASE + 0x0098},
};

STATIC GPIO_REG mPullReg[] = {
  {0, GPIO_PIN_PA0, PMU1_IOC_BASE + 0x0020},
  {0, GPIO_PIN_PB0, PMU1_IOC_BASE + 0x0024},
  {0, GPIO_PIN_PB5, PMU2_IOC_BASE + 0x0028},
  {0, GPIO_PIN_PC0, PMU2_IOC_BASE + 0x002C},
  {0, GPIO_PIN_PD0, PMU2_IOC_BASE + 0x0030},
  {1, GPIO_PIN_PA0, VCCIO1_4_IOC_BASE + 0x0110},
  {1, GPIO_PIN_PB0, VCCIO1_4_IOC_BASE + 0x0114},
  {1, GPIO_PIN_PC0, VCCIO1_4_IOC_BASE + 0x0118},
  {1, GPIO_PIN_PD0, VCCIO1_4_IOC_BASE + 0x011C},
  {2, GPIO_PIN_PA0, EMMC_IOC_BASE + 0x0120},
  {2, GPIO_PIN_PA4, VCCIO3_5_IOC_BASE + 0x0120},
  {2, GPIO_PIN_PB0, VCCIO3_5_IOC_BASE + 0x0124},
  {2, GPIO_PIN_PC0, VCCIO3_5_IOC_BASE + 0x0128},
  {2, GPIO_PIN_PD0, EMMC_IOC_BASE + 0x012C},
  {3, GPIO_PIN_PA0, VCCIO3_5_IOC_BASE + 0x0130},
  {3, GPIO_PIN_PB0, VCCIO3_5_IOC_BASE + 0x0134},
  {3, GPIO_PIN_PC0, VCCIO3_5_IOC_BASE + 0x0138},
  {3, GPIO_PIN_PD0, VCCIO3_5_IOC_BASE + 0x013C},
  {4, GPIO_PIN_PA0, VCCIO6_IOC_BASE + 0x0140},
  {4, GPIO_PIN_PB0, VCCIO6_IOC_BASE + 0x0144},
  {4, GPIO_PIN_PC0, VCCIO6_IOC_BASE + 0x0148},
  {4, GPIO_PIN_PC2, VCCIO3_5_IOC_BASE + 0x0148},
  {4, GPIO_PIN_PD0, VCCIO2_IOC_BASE + 0x014C},
};

STATIC GPIO_REG mIEReg[] = {
  {0, GPIO_PIN_PA0, PMU1_IOC_BASE + 0x0028},
  {0, GPIO_PIN_PB0, PMU1_IOC_BASE + 0x002C},
  {0, GPIO_PIN_PB5, PMU2_IOC_BASE + 0x0034},
  {0, GPIO_PIN_PC0, PMU2_IOC_BASE + 0x0038},
  {0, GPIO_PIN_PD0, PMU2_IOC_BASE + 0x003C},
  {1, GPIO_PIN_PA0, VCCIO1_4_IOC_BASE + 0x0180},
  {1, GPIO_PIN_PB0, VCCIO1_4_IOC_BASE + 0x0184},
  {1, GPIO_PIN_PC0, VCCIO1_4_IOC_BASE + 0x0188},
  {1, GPIO_PIN_PD0, VCCIO1_4_IOC_BASE + 0x018C},
  {2, GPIO_PIN_PA0, EMMC_IOC_BASE + 0x0190},
  {2, GPIO_PIN_PA4, VCCIO3_5_IOC_BASE + 0x0190},
  {2, GPIO_PIN_PB0, VCCIO3_5_IOC_BASE + 0x0194},
  {2, GPIO_PIN_PC0, VCCIO3_5_IOC_BASE + 0x0198},
  {2, GPIO_PIN_PD0, EMMC_IOC_BASE + 0x019C},
  {3, GPIO_PIN_PA0, VCCIO3_5_IOC_BASE + 0x01A0},
  {3, GPIO_PIN_PB0, VCCIO3_5_IOC_BASE + 0x01A4},
  {3, GPIO_PIN_PC0, VCCIO3_5_IOC_BASE + 0x01A8},
  {3, GPIO_PIN_PD0, VCCIO3_5_IOC_BASE + 0x01AC},
  {4, GPIO_PIN_PA0, VCCIO6_IOC_BASE + 0x01B0},
  {4, GPIO_PIN_PB0, VCCIO6_IOC_BASE + 0x01B4},
  {4, GPIO_PIN_PC0, VCCIO6_IOC_BASE + 0x01B8},
  {4, GPIO_PIN_PC2, VCCIO3_5_IOC_BASE + 0x01B8},
  {4, GPIO_PIN_PD0, VCCIO2_IOC_BASE + 0x01BC},
};

#define GPIO_BASE(n) ((n) == 0 ? 0xFD8A0000UL : (0xFEC20000UL + ((n) - 1) * 0x10000))

VOID
GpioPinSetDirection (
  IN UINT8 Group,
  IN UINT8 Pin,
  IN GPIO_PIN_DIRECTION Direction
  )
{
    DEBUG((DEBUG_INFO, "GpioPinSetDirection Group:%d Pin:%d Direction:%d\n", 
           Group, Pin, Direction));
    MmioWrite32 (GPIO_BASE (Group) + GPIO_SWPORT_DDR (Pin),
                 GPIO_WRITE_MASK (Pin) | GPIO_VALUE_MASK (Pin, Direction));
}

VOID
GpioPinWrite (
  IN UINT8 Group,
  IN UINT8 Pin,
  IN BOOLEAN Value
  )
{
    DEBUG((DEBUG_VERBOSE, "GpioPinWrite Group:%d Pin:%d Value:%d\n", 
           Group, Pin, Value));
    MmioWrite32 (GPIO_BASE (Group) + GPIO_SWPORT_DR (Pin),
                 GPIO_WRITE_MASK (Pin) | GPIO_VALUE_MASK (Pin, Value));
}

BOOLEAN
GpioPinRead (
  IN UINT8 Group,
  IN UINT8 Pin
  )
{
    CONST UINT32 Value = MmioRead32 (GPIO_BASE (Group) + GPIO_SWPORT_DR (Pin));
    return (Value & GPIO_VALUE_MASK (Pin, 1)) != 0;
}

BOOLEAN
GpioPinReadActual (
  IN UINT8 Group,
  IN UINT8 Pin
  )
{
    CONST UINT32 Value = MmioRead32 (GPIO_BASE (Group) + GPIO_EXT_PORT);
    return (Value & (1 << Pin)) != 0;
}

VOID
GpioPinSetFunction (
  IN UINT8 Group,
  IN UINT8 Pin,
  IN UINT8 Function
  )
{
  ASSERT (Group < GPIO_NGROUPS);

  EFI_PHYSICAL_ADDRESS Reg;
  UINT32 Value = GRF_GPIO_IOMUX_MASK (Pin) | ((UINT32)Function << GRF_GPIO_IOMUX_SHIFT (Pin));

  DEBUG ((DEBUG_INFO, "GpioPinSetFunction Group:%d Pin:%d Function:%d\n", Group, Pin, Function));

  if(Group == 0) {
    if(Pin >= GPIO_PIN_PB4) {
      if(Function >= 8) {
        Reg = PMU2_IOC_BASE - 0xC + GRF_GPIO_IOMUX_REG(Pin);
        Value = GRF_GPIO_IOMUX_MASK (Pin) | ((UINT32)8 << GRF_GPIO_IOMUX_SHIFT (Pin));
        MmioWrite32(Reg, Value);

        Reg = BUS_IOC_BASE + GRF_GPIO_IOMUX_REG(Pin);
        Value = GRF_GPIO_IOMUX_MASK (Pin) | ((UINT32)Function << GRF_GPIO_IOMUX_SHIFT (Pin));
        MmioWrite32(Reg, Value);
      } else {
        Reg = PMU2_IOC_BASE - 0xC + GRF_GPIO_IOMUX_REG(Pin);
        Value = GRF_GPIO_IOMUX_MASK (Pin) | ((UINT32)Function << GRF_GPIO_IOMUX_SHIFT (Pin));
        MmioWrite32(Reg, Value);

        Reg = BUS_IOC_BASE + GRF_GPIO_IOMUX_REG(Pin);
        Value = GRF_GPIO_IOMUX_MASK (Pin);
        MmioWrite32(Reg, Value);
      }
      DEBUG ((DEBUG_VERBOSE, "Reg - Value: 0x%lX = 0x%08X\n", Reg, Value));
      return;
    } else {
      Reg = PMU1_IOC_BASE + GRF_GPIO_IOMUX_REG(Pin);
    }
  } else {
    Reg = BUS_IOC_BASE + Group * 0x20 + GRF_GPIO_IOMUX_REG(Pin);
  }

  DEBUG ((DEBUG_VERBOSE, "Reg - Value: 0x%lX = 0x%08X\n", Reg, Value));
  MmioWrite32 (Reg, Value);
}

VOID
GpioPinSetPull (
  IN UINT8 Group,
  IN UINT8 Pin,
  IN GPIO_PIN_PULL Pull
  )
{
  ASSERT (Group < GPIO_NGROUPS);
  
  UINTN Idx;
  EFI_PHYSICAL_ADDRESS Reg;
  CONST UINT32 Value = GRF_GPIO_P_MASK (Pin) | ((UINT32)Pull << GRF_GPIO_P_SHIFT (Pin));

  for(Idx = ARRAY_SIZE(mPullReg) - 1; Idx >= 0; Idx--) {
    if(Group == mPullReg[Idx].Group && Pin >= mPullReg[Idx].Pin) {
      Reg = mPullReg[Idx].Addr + GRF_GPIO_P_REG(Pin - mPullReg[Idx].Pin);
    }
  }

  DEBUG ((DEBUG_INFO, "GpioPinSetPull Group:%d Pin:%d Pull:%d\n", Group, Pin, Pull));
  DEBUG ((DEBUG_VERBOSE, "Reg - Value: 0x%lX = 0x%08X\n", Reg, Value));

  MmioWrite32 (Reg, Value);
}

VOID
GpioPinSetDrive (
  IN UINT8 Group,
  IN UINT8 Pin,
  IN GPIO_PIN_DRIVE Drive
  )
{
  ASSERT (Group < GPIO_NGROUPS);
  
  UINTN Idx;
  EFI_PHYSICAL_ADDRESS Reg;
  CONST UINT32 Value = GRF_GPIO_DS_MASK (Pin) | ((UINT32)Drive << GRF_GPIO_DS_SHIFT (Pin));

  for(Idx = ARRAY_SIZE(mDsReg) - 1; Idx >= 0; Idx--) {
    if(Group == mDsReg[Idx].Group && Pin >= mDsReg[Idx].Pin) {
      Reg = mDsReg[Idx].Addr + GRF_GPIO_DS_REG(Pin - mDsReg[Idx].Pin);
    }
  }

  DEBUG ((DEBUG_INFO, "GpioPinSetDrive Group:%d Pin:%d Drive:%d\n", Group, Pin, Drive));
  DEBUG ((DEBUG_VERBOSE, "Reg - Value: 0x%lX = 0x%08X\n", Reg, Value));

  MmioWrite32 (Reg, Value);
}

VOID
GpioPinSetInput (
  IN UINT8 Group,
  IN UINT8 Pin,
  IN GPIO_PIN_INPUT_ENABLE InputEnable
  )
{
  ASSERT (Group < GPIO_NGROUPS);
  
  UINTN Idx;
  EFI_PHYSICAL_ADDRESS Reg;
  CONST UINT32 Value = GRF_GPIO_IE_MASK (Pin) | ((UINT32)InputEnable << GRF_GPIO_IE_SHIFT (Pin));

  for(Idx = ARRAY_SIZE(mIEReg) - 1; Idx >= 0; Idx--) {
    if(Group == mIEReg[Idx].Group && Pin >= mIEReg[Idx].Pin) {
      Reg = mIEReg[Idx].Addr + GRF_GPIO_IE_REG(Pin - mIEReg[Idx].Pin);
    }
  }

  DEBUG ((DEBUG_INFO, "GpioPinSetInput Group:%d Pin:%d InputEnable:%d\n", Group, Pin, InputEnable));
  DEBUG ((DEBUG_VERBOSE, "Reg - Value: 0x%lX = 0x%08X\n", Reg, Value));

  MmioWrite32 (Reg, Value);
}

VOID
GpioSetIomuxConfig (
  IN CONST GPIO_IOMUX_CONFIG *Configs,
  IN UINT32 NumConfigs
  )
{
  UINT32 Index;

  for (Index = 0; Index < NumConfigs; Index++) {
    CONST GPIO_IOMUX_CONFIG *Mux = &Configs[Index];
    DEBUG ((DEBUG_INFO, "GPIO: IOMUX for pin '%a'\n", Mux->Name));
    GpioPinSetFunction (Mux->Group, Mux->Pin, Mux->Function);
    GpioPinSetPull (Mux->Group, Mux->Pin, Mux->Pull);
    if (Mux->Drive != GPIO_PIN_DRIVE_DEFAULT) {
      GpioPinSetDrive (Mux->Group, Mux->Pin, Mux->Drive);
    }
  }
}