
/** @file
  Platform Hook Library instance for the UpXtreme Board

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <GpioPinsCnlLp.h>
#include <Library/BaseLib.h>
#include <Library/GpioLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PlatformHookLib.h>
#include <PchAccess.h>
#include <Uefi/UefiBaseType.h>

#define LCR_OFFSET  (FixedPcdGet32 (PcdSerialRegisterStride) * 0x03)

STATIC GPIO_INIT_CONFIG mUartGpioTable[] = {
  {GPIO_CNL_LP_GPP_C20, {GpioPadModeNative1, GpioHostOwnGpio, GpioDirNone,  GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},//SERIALIO_UART2_RXD
  {GPIO_CNL_LP_GPP_C21, {GpioPadModeNative1, GpioHostOwnGpio, GpioDirNone,  GpioOutDefault, GpioIntDis, GpioHostDeepReset,  GpioTermNone}},//SERIALIO_UART2_TXD
};

/**
  Retrieve the I/O or MMIO base address register for the PCI UART device.

  @retval  The base address register of the UART device.

**/
STATIC
UINTN
GetSerialRegisterBase (
  VOID
  )
{
  UINT64  PciAddress;
  UINT32  BaseAddressBuffer[2];
  UINT16  Cmd16;

  PciAddress =  PCI_SEGMENT_LIB_ADDRESS (
                  DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                  DEFAULT_PCI_BUS_NUMBER_PCH,
                  PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART2,
                  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART2,
                  0
                  );

  Cmd16 = PciSegmentRead16 (PciAddress + PCI_VENDOR_ID_OFFSET);
  if (Cmd16 == 0xFFFF) {
    //
    // Device might be hidden, return the fixed serial register base address
    //
    return (UINTN) FixedPcdGet32 (PcdSerialRegisterBase);
  } else {
    if (PciSegmentRead32 (PciAddress + PCI_COMMAND_OFFSET) & EFI_PCI_COMMAND_MEMORY_SPACE) {
      BaseAddressBuffer[0] = PciSegmentRead32 (PciAddress + R_SERIAL_IO_CFG_BAR0_LOW) & B_SERIAL_IO_CFG_BAR0_LOW_BAR;
      BaseAddressBuffer[1] = 0;

      if ((PciSegmentRead32 (PciAddress + PCI_BASE_ADDRESSREG_OFFSET) & 0x6) == 0x4) {
        BaseAddressBuffer[1] = PciSegmentRead32 (PciAddress + R_SERIAL_IO_CFG_BAR0_HIGH);
      }
      return *((UINTN *) (&BaseAddressBuffer[0]));
    } else {
      return 0;
    }
  }
}

/**
  Performs platform specific initialization required for the CPU to access
  the hardware associated with a SerialPortLib instance.  This function does
  not initialize the serial port hardware itself.  Instead, it initializes
  hardware devices that are required for the CPU to access the serial port
  hardware.  This function may be called more than once.

  @retval RETURN_SUCCESS       The platform specific initialization succeeded.
  @retval RETURN_DEVICE_ERROR  The platform specific initialization could not be completed.

**/
RETURN_STATUS
EFIAPI
PlatformHookSerialPortInitialize (
  VOID
  )
{
  UINT64               PciAddress;
  UINTN                SerialRegisterBase;
  UINTN                SerialRegisterBase1;

  PciAddress =  PCI_SEGMENT_LIB_ADDRESS (
                  DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                  DEFAULT_PCI_BUS_NUMBER_PCH,
                  PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART2,
                  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART2,
                  0
                  );

  SerialRegisterBase = GetSerialRegisterBase ();

  if (SerialRegisterBase == 0 || SerialRegisterBase == (UINTN) ~0) {
    return RETURN_DEVICE_ERROR;
  }
  SerialRegisterBase1 = SerialRegisterBase + V_SERIAL_IO_CFG_BAR_SIZE;

  if (
    (MmioRead8 (SerialRegisterBase + LCR_OFFSET) & 0x3F) !=
    (FixedPcdGet8 (PcdSerialLineControl) & 0x3F)
    ) {
      GpioConfigurePads ((sizeof (mUartGpioTable) / sizeof (GPIO_INIT_CONFIG)), mUartGpioTable);

      PciSegmentWrite32 (PciAddress + R_SERIAL_IO_CFG_BAR0_LOW,  (UINT32) SerialRegisterBase);
      PciSegmentWrite32 (PciAddress + R_SERIAL_IO_CFG_BAR1_LOW,  (UINT32) SerialRegisterBase1);
      if (sizeof (UINTN) == sizeof (UINT32)) {
        PciSegmentWrite32 (PciAddress + R_SERIAL_IO_CFG_BAR0_HIGH, 0x0);
        PciSegmentWrite32 (PciAddress + R_SERIAL_IO_CFG_BAR1_HIGH, 0x0);
      } else {
        PciSegmentWrite32 (PciAddress + R_SERIAL_IO_CFG_BAR0_HIGH, (UINT32) RShiftU64 (SerialRegisterBase, 32));
        PciSegmentWrite32 (PciAddress + R_SERIAL_IO_CFG_BAR1_HIGH, (UINT32) RShiftU64 (SerialRegisterBase1, 32));
      }
      PciSegmentWrite32 (PciAddress + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE);
      PciSegmentOr32 (PciAddress + R_SERIAL_IO_CFG_D0I3MAXDEVPG, BIT18 | BIT17 | BIT16);

      //
      // Get controller out of reset
      //
      MmioOr32 (SerialRegisterBase + R_SERIAL_IO_MEM_PPR_RESETS,
        B_SERIAL_IO_MEM_PPR_RESETS_FUNC | B_SERIAL_IO_MEM_PPR_RESETS_APB | B_SERIAL_IO_MEM_PPR_RESETS_IDMA);

      //
      // Program clock dividers for UARTs
      //
      MmioWrite32 (SerialRegisterBase + R_SERIAL_IO_MEM_PPR_CLK,
          (B_SERIAL_IO_MEM_PPR_CLK_UPDATE | (V_SERIAL_IO_MEM_PPR_CLK_N_DIV << 16) |
           (V_SERIAL_IO_MEM_PPR_CLK_M_DIV << 1) | B_SERIAL_IO_MEM_PPR_CLK_EN )
          );
  }

  return RETURN_SUCCESS;
}

