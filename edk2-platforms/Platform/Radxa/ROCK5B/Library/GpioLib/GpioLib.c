/** @file
 *
 *  RK3566/RK3568 GPIO Library.
 * 
 *  Copyright (c) 2021, Jared McNeill <jmcneill@invisible.ca>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/GpioLib.h>

#if 0
#define GRF_GPIO_IOMUX_REG(Pin)         (((Pin) / 4) * 4)
#define GRF_GPIO_IOMUX_SHIFT(Pin)       (((Pin) % 4) * 4)
#define GRF_GPIO_IOMUX_MASK(Pin)        (0xFU << (GRF_GPIO_IOMUX_SHIFT(Pin) + 16))

#define GRF_GPIO_P_REG(Pin)             (((Pin) / 8) * 4)
#define GRF_GPIO_P_SHIFT(Pin)           (((Pin) % 8) * 2)
#define GRF_GPIO_P_MASK(Pin)            (0x3U << (GRF_GPIO_P_SHIFT (Pin) + 16))

#define GRF_GPIO_DS_REG(Pin)            (((Pin) / 2) * 4)
#define GRF_GPIO_DS_SHIFT(Pin)          (((Pin) % 2) * 8)
#define GRF_GPIO_DS_MASK(Pin)           (0xFFU << (GRF_GPIO_DS_SHIFT (Pin) + 16))

#define GRF_GPIO_IE_REG(Pin)            (((Pin) / 8) * 4)
#define GRF_GPIO_IE_SHIFT(Pin)          (((Pin) % 8) * 2)
#define GRF_GPIO_IE_MASK(Pin)           (0x3U << (GRF_GPIO_IE_SHIFT (Pin) + 16))
#endif

#define GPIO_SWPORT_DR(Pin)             ((Pin) < 16 ? 0x0000 : 0x0004)
#define GPIO_SWPORT_DDR(Pin)            ((Pin) < 16 ? 0x0008 : 0x000C)

#define GPIO_WRITE_MASK(Pin)            (1U << (((Pin) % 16) + 16))
#define GPIO_VALUE_MASK(Pin, Value)     ((UINT32)Value << ((Pin) % 16))

#define GPIO_NGROUPS                    5

#if 0
typedef struct {
  EFI_PHYSICAL_ADDRESS  IOMUX;
  EFI_PHYSICAL_ADDRESS  P;
  EFI_PHYSICAL_ADDRESS  IE;
  EFI_PHYSICAL_ADDRESS  DS;
} GPIO_PINMUX_REGS;

STATIC GPIO_PINMUX_REGS mPinmuxReg[GPIO_NGROUPS] = {
#define REG_BASE(Base, IomuxOff, POff, IeOff, DsOff)   { .IOMUX = (Base) + (IomuxOff), .P = (Base) + (POff), .IE = (Base) + (IeOff), .DS = (Base) + (DsOff) }
  [0] = REG_BASE(PMU_GRF, 0x0000, 0x0020, 0x0030, 0x0070),
  [1] = REG_BASE(SYS_GRF, 0x0000, 0x0080, 0x00C0, 0x0200),
  [2] = REG_BASE(SYS_GRF, 0x0020, 0x0090, 0x00D0, 0x0240),
  [3] = REG_BASE(SYS_GRF, 0x0040, 0x00A0, 0x00E0, 0x0280),
  [4] = REG_BASE(SYS_GRF, 0x0060, 0x00B0, 0x00F0, 0x02C0),
#undef REG_BASE
};
#endif

#define GPIO_BASE(n)        ((n) == 0 ? 0xFD8A0000UL : (0xFEC20000UL + ((n) - 1) * 0x10000))

VOID
GpioPinSetDirection (
  IN UINT8 Group,
  IN UINT8 Pin,
  IN GPIO_PIN_DIRECTION Direction
  )
{
    DEBUG((EFI_D_WARN, "GpioPinSetDirection Group:%d Pin:%d\n", Group, Pin));
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
    DEBUG((EFI_D_WARN, "GpioPinWrite Group:%d Pin:%d\n", Group, Pin));
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

#if 0
VOID
GpioPinSetFunction (
  IN UINT8 Group,
  IN UINT8 Pin,
  IN UINT8 Function
  )
{
  ASSERT (Group < GPIO_NGROUPS);

  CONST EFI_PHYSICAL_ADDRESS Reg = mPinmuxReg[Group].IOMUX + GRF_GPIO_IOMUX_REG (Pin);
  CONST UINT32 Value = GRF_GPIO_IOMUX_MASK (Pin) | ((UINT32)Function << GRF_GPIO_IOMUX_SHIFT (Pin));

  DEBUG ((DEBUG_INFO, "GPIO: SetFunction %u %02X %04X      0x%lX = 0x%08X\n", Group, Pin, Function, Reg, Value));

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

  if (Group == 0 && Pin >= GPIO_PIN_PD3 && Pin <= GPIO_PIN_PD6 && Pull == GPIO_PIN_PULL_UP) {
    Pull = 3;
  }

  CONST EFI_PHYSICAL_ADDRESS Reg = mPinmuxReg[Group].P + GRF_GPIO_P_REG (Pin);
  CONST UINT32 Value = GRF_GPIO_P_MASK (Pin) | ((UINT32)Pull << GRF_GPIO_P_SHIFT (Pin));

  DEBUG ((DEBUG_INFO, "GPIO: SetPull     %u %02X %04X      0x%lX = 0x%08X\n", Group, Pin, Pull, Reg, Value));

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
  ASSERT (Drive != GPIO_PIN_DRIVE_DEFAULT);

  CONST EFI_PHYSICAL_ADDRESS Reg = mPinmuxReg[Group].DS + GRF_GPIO_DS_REG (Pin);
  CONST UINT32 Value = GRF_GPIO_DS_MASK (Pin) | ((UINT32)Drive << GRF_GPIO_DS_SHIFT (Pin));

  DEBUG ((DEBUG_INFO, "GPIO: SetDrive    %u %02X %04X      0x%lX = 0x%08X\n", Group, Pin, Drive, Reg, Value));

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
  ASSERT (InputEnable != GPIO_PIN_INPUT_DEFAULT);

  CONST EFI_PHYSICAL_ADDRESS Reg = mPinmuxReg[Group].IE + GRF_GPIO_IE_REG (Pin);
  CONST UINT32 Value = GRF_GPIO_IE_MASK (Pin) | ((UINT32)InputEnable << GRF_GPIO_IE_SHIFT (Pin));

  DEBUG ((DEBUG_INFO, "GPIO: SetInput    %u %02X %04X      0x%lX = 0x%08X\n", Group, Pin, InputEnable, Reg, Value));

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
#endif
