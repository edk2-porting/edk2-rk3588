/** @file
  Platform Hook Library instances

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/PlatformHookLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcdLib.h>
#include <Library/MmPciLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsLpc.h>
#include <Library/SaPlatformLib.h>
#include <Library/PchPciBdfLib.h>

#define COM1_BASE                                 0x3f8
#define COM2_BASE                                 0x2f8

#define SIO_DOCKING_LPC_SWITCH_REGISTER_ADDRESS   0x0690

#define LPC_SIO_INDEX_DEFAULT_PORT_2              0x2E
#define LPC_SIO_DATA_DEFAULT_PORT_2               0x2F
#define LPC_SIO_GPIO_REGISTER_ADDRESS_2           0x0A20

#define LEGACY_DAUGHTER_CARD_SIO_INDEX_PORT       0x2E
#define LEGACY_DAUGHTER_CARD_SIO_DATA_PORT        0x2F
#define LEGACY_DAUGHTER_CARD_2_SIO_INDEX_PORT     0x4E
#define LEGACY_DAUGHTER_CARD_2_SIO_DATA_PORT      0x4F

#define IT8628_ENTER_CONFIG_WRITE_SEQ_0           0x87
#define IT8628_ENTER_CONFIG_WRITE_SEQ_1           0x01
#define IT8628_ENTER_CONFIG_WRITE_SEQ_2           0x55
#define IT8628_ENTER_CONFIG_WRITE_SEQ_3           0x55
#define IT8628_EXIT_CONFIG                        0x2
#define IT8628_CHIPID_BYTE1                       0x86
#define IT8628_CHIPID_BYTE2                       0x28

typedef struct {
  UINT8 Register;
  UINT8 Value;
} EFI_SIO_TABLE;


GLOBAL_REMOVE_IF_UNREFERENCED EFI_SIO_TABLE mSioTableWpcn381u[] = {
  {0x29, 0x0A0},                  // Enable super I/O clock and set to 48MHz
  {0x22, 0x003},                  //
  {0x07, 0x003},                  // Select UART0 device
  {0x60, (COM1_BASE >> 8)},       // Set Base Address MSB
  {0x61, (COM1_BASE & 0x00FF)},   // Set Base Address LSB
  {0x70, 0x004},                  // Set to IRQ4
  {0x30, 0x001},                  // Enable it with Activation bit
  {0x07, 0x002},                  // Select UART1 device
  {0x60, (COM2_BASE >> 8)},       // Set Base Address MSB
  {0x61, (COM2_BASE & 0x00FF)},   // Set Base Address LSB
  {0x70, 0x003},                  // Set to IRQ3
  {0x30, 0x001},                  // Enable it with Activation bit
  {0x07, 0x007},                  // Select GPIO device
  {0x60, (LPC_SIO_GPIO_REGISTER_ADDRESS_2 >> 8)},      // Set Base Address MSB
  {0x61, (LPC_SIO_GPIO_REGISTER_ADDRESS_2 & 0x00FF)},  // Set Base Address LSB
  {0x30, 0x001},                  // Enable it with Activation bit
  {0x21, 0x001},                  // Global Device Enable
  {0x26, 0x000}                   // Fast Enable UART 0 & 1 as their enable & activation bit
};

//
// IT8628
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_SIO_TABLE mSioIt8628TableSerialPort[] = {
  {0x023, 0x09}, // Clock Selection register
  {0x007, 0x01}, // Com1 Logical Device Number select
  {0x061, 0xF8}, // Serial Port 1 Base Address MSB Register
  {0x060, 0x03}, // Serial Port 1 Base Address LSB Register
  {0x070, 0x04}, // Serial Port 1 Interrupt Level Select
  {0x030, 0x01}, // Serial Port 1 Activate
  {0x007, 0x02}, // Com1 Logical Device Number select
  {0x061, 0xF8}, // Serial Port 2 Base Address MSB Register
  {0x060, 0x02}, // Serial Port 2 Base Address MSB Register
  {0x070, 0x03}, // Serial Port 2 Interrupt Level Select
  {0x030, 0x01}  // Serial Port 2 Activate

};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_SIO_TABLE mSioTableWinbond_x374[] = {
  {0x07, 0x03},                   // Select UART0 device
  {0x60, (COM1_BASE >> 8)},       // Set Base Address MSB
  {0x61, (COM1_BASE & 0x00FF)},   // Set Base Address LSB
  {0x70, 0x04},                   // Set to IRQ4
  {0x30, 0x01}                    // Enable it with Activation bit
};

/**
Check whether the IT8628 SIO present on LPC. If yes, enable its serial ports

@retval EFI_SUCCESS     Operations performed successfully.
**/
STATIC
VOID
It8628SioSerialPortInit (
  VOID
  )
{
  UINT8   ChipId0              = 0;
  UINT8   ChipId1              = 0;
  UINT16  LpcIoDecondeRangeSet = 0;
  UINT16  LpcIoDecoodeSet      = 0;
  UINT8   Index;
  UINTN   LpcBaseAddr;



  //
  // Enable I/O decoding for COM1 (3F8h-3FFh), COM2(2F8h-2FFh), I/O port 2Eh/2Fh.
  //
  LpcBaseAddr = MmPciBase (
                  DEFAULT_PCI_BUS_NUMBER_PCH,
                  LpcDevNumber (),
                  LpcFuncNumber ()
                  );

  LpcIoDecondeRangeSet = (UINT16) MmioRead16 (LpcBaseAddr + R_LPC_CFG_IOD);
  LpcIoDecoodeSet = (UINT16) MmioRead16 (LpcBaseAddr + R_LPC_CFG_IOE);
  MmioWrite16 ((LpcBaseAddr + R_LPC_CFG_IOD), (LpcIoDecondeRangeSet | ((V_LPC_CFG_IOD_COMB_2F8 << 4) | V_LPC_CFG_IOD_COMA_3F8)));
  MmioWrite16 ((LpcBaseAddr + R_LPC_CFG_IOE), (LpcIoDecoodeSet | (B_LPC_CFG_IOE_SE | B_LPC_CFG_IOE_CBE | B_LPC_CFG_IOE_CAE|B_LPC_CFG_IOE_KE)));


  //
  // Enter MB PnP Mode
  //
  IoWrite8 (LPC_SIO_INDEX_DEFAULT_PORT_2, IT8628_ENTER_CONFIG_WRITE_SEQ_0);
  IoWrite8 (LPC_SIO_INDEX_DEFAULT_PORT_2, IT8628_ENTER_CONFIG_WRITE_SEQ_1);
  IoWrite8 (LPC_SIO_INDEX_DEFAULT_PORT_2, IT8628_ENTER_CONFIG_WRITE_SEQ_2);
  IoWrite8 (LPC_SIO_INDEX_DEFAULT_PORT_2, IT8628_ENTER_CONFIG_WRITE_SEQ_3);

  //
  // Read Chip Id of SIO IT8628 (registers 0x20 and 0x21)
  //
  IoWrite8 (LPC_SIO_INDEX_DEFAULT_PORT_2, 0x20);
  ChipId0 = IoRead8 (LPC_SIO_DATA_DEFAULT_PORT_2);

  IoWrite8 (LPC_SIO_INDEX_DEFAULT_PORT_2, 0x21);
  ChipId1 = IoRead8 (LPC_SIO_DATA_DEFAULT_PORT_2);

  //
  // Enable Serial Port 1, Port 2
  //
  if ((ChipId0 == IT8628_CHIPID_BYTE1) && (ChipId1 == IT8628_CHIPID_BYTE2)) {
    for (Index = 0; Index < sizeof (mSioIt8628TableSerialPort) / sizeof (EFI_SIO_TABLE); Index++) {
      IoWrite8 (LPC_SIO_INDEX_DEFAULT_PORT_2, mSioIt8628TableSerialPort[Index].Register);
      IoWrite8 (LPC_SIO_DATA_DEFAULT_PORT_2, mSioIt8628TableSerialPort[Index].Value);
    }
  }

  //
  // Exit MB PnP Mode
  //
  IoWrite8 (LPC_SIO_INDEX_DEFAULT_PORT_2, IT8628_EXIT_CONFIG);
  IoWrite8 (LPC_SIO_DATA_DEFAULT_PORT_2, IT8628_EXIT_CONFIG);

  return;
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
  //
  // Enable I/O decoding for COM1(3F8h-3FFh), COM2(2F8h-2FFh), I/O port 2Eh/2Fh, 4Eh/4Fh, 60h/64Fh and 62h/66h.
  //
  PchLpcIoDecodeRangesSet (PcdGet16 (PcdLpcIoDecodeRange));
  PchLpcIoEnableDecodingSet (PcdGet16 (PchLpcIoEnableDecoding));

  // Configure Sio IT8628
  It8628SioSerialPortInit ();

  return RETURN_SUCCESS;
}
