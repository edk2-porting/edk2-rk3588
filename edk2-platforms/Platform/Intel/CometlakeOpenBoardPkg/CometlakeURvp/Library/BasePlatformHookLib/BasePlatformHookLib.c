/** @file
  Platform Hook Library instances


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/PlatformHookLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcdLib.h>
#include <SystemAgent/Include/SaAccess.h>
#include <SioRegs.h>
#include <Library/PchCycleDecodingLib.h>
#include <Register/PchRegsLpc.h>
#include <PchAccess.h>

#define LPC_SIO_INDEX_DEFAULT_PORT_2              0x2E
#define LPC_SIO_DATA_DEFAULT_PORT_2               0x2F

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

/**
  Check whether the IT8628 SIO present on LPC. If yes, enable its serial ports
**/
STATIC
VOID
It8628SioSerialPortInit (
  VOID
  )
{
  UINT8   ChipId0;
  UINT8   ChipId1;
  UINT16  LpcIoDecondeRangeSet;
  UINT16  LpcIoDecoodeSet;
  UINT8   Index;
  UINT64  LpcBaseAddr;

  ChipId0              = 0;
  ChipId1              = 0;
  LpcIoDecondeRangeSet = 0;
  LpcIoDecoodeSet      = 0;

  //
  // Enable I/O decoding for COM1 (3F8h-3FFh), COM2(2F8h-2FFh), I/O port 2Eh/2Fh.
  //
  LpcBaseAddr = PCI_SEGMENT_LIB_ADDRESS (
                  DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                  DEFAULT_PCI_BUS_NUMBER_PCH,
                  PCI_DEVICE_NUMBER_PCH_LPC,
                  PCI_FUNCTION_NUMBER_PCH_LPC,
                  0
                  );

  LpcIoDecondeRangeSet = (UINT16) PciSegmentRead16 (LpcBaseAddr + R_LPC_CFG_IOD);
  LpcIoDecoodeSet = (UINT16) PciSegmentRead16 (LpcBaseAddr + R_LPC_CFG_IOE);
  PciSegmentWrite16 ((LpcBaseAddr + R_LPC_CFG_IOD), (LpcIoDecondeRangeSet | ((V_LPC_CFG_IOD_COMB_2F8 << 4) | V_LPC_CFG_IOD_COMA_3F8)));
  PciSegmentWrite16 ((LpcBaseAddr + R_LPC_CFG_IOE), (LpcIoDecoodeSet | (B_LPC_CFG_IOE_SE | B_LPC_CFG_IOE_CBE | B_LPC_CFG_IOE_CAE|B_LPC_CFG_IOE_KE)));

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

