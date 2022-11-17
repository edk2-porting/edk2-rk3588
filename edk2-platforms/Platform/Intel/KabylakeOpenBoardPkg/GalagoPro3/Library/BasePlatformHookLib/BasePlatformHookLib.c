/** @file
  Platform Hook Library

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/PlatformHookLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/PcdLib.h>
#include <SystemAgent/Include/SaAccess.h>
#include <SioRegs.h>
#include <Library/MmPciLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Register/PchRegsLpc.h>
#include <PchAccess.h>

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

typedef struct {
  UINT8 Register;
  UINT8 Value;
} EFI_SIO_TABLE;

GLOBAL_REMOVE_IF_UNREFERENCED EFI_SIO_TABLE mSioTable[] = {
  {0x002, 0x88},     // Power On UARTs
  {0x024, COM1_BASE >> 2},
  {0x025, COM2_BASE >> 2},
  {0x028, 0x043},    // IRQ of UARTs, UART2 IRQ=3,UART1 IRQ=4,
  {0x029, 0x080},    // SIRQ_CLKRUN_EN
  {0x02A, 0x000},
  {0x02B, 0x0DE},
  {0x00A, 0x040},
  {0x00C, 0x00E},
  {0x02c, 0x002},
  {0x030, FixedPcdGet16 (PcdSioBaseAddress) >> 4},
  {0x03b, SIO_DOCKING_LPC_SWITCH_REGISTER_ADDRESS >> 8},
  {0x03c, SIO_DOCKING_LPC_SWITCH_REGISTER_ADDRESS & 0xff},
  {0x03a, 0x00A},    // LPC Docking Enabling
  {0x031, 0x01f},
  {0x032, 0x000},
  {0x033, 0x004},
  {0x038, 0x0FB},
  {0x035, 0x0FE},
  {0x036, 0x000},
  {0x037, 0x0FF},
  {0x039, 0x000},
  {0x034, 0x001},
  {0x012, FixedPcdGet16 (PcdLpcSioConfigDefaultPort) & 0xFF},           // Relocate configuration ports base address
  {0x013, (FixedPcdGet16 (PcdLpcSioConfigDefaultPort) >> 8) & 0xFF}     // to ensure SIO config address can be accessed in OS
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_SIO_TABLE mSioTableSmsc1000[] = {
  {0x002, 0x88},     // Power On UARTs
  {0x007, 0x00},
  {0x024, COM1_BASE >> 2},
  {0x025, COM2_BASE >> 2},
  {0x028, 0x043},    // IRQ of UARTs, UART2 IRQ=3,UART1 IRQ=4,
  {0x029, 0x080},    // SIRQ_CLKRUN_EN
  {0x02A, 0x000},
  {0x02B, 0x0DE},
  {0x00A, 0x040},
  {0x00C, 0x00E},
  {0x030, FixedPcdGet16 (PcdSioBaseAddress) >> 4},
  {0x03b, SIO_DOCKING_LPC_SWITCH_REGISTER_ADDRESS >> 8},
  {0x03c, SIO_DOCKING_LPC_SWITCH_REGISTER_ADDRESS & 0xff},
  {0x03a, 0x00A},    // LPC Docking Enabling
  {0x031, 0x01f},
  {0x032, 0x000},
  {0x033, 0x004},
  {0x038, 0x0FB},
  {0x035, 0x0FE},
  {0x036, 0x000},
  {0x037, 0x0FE},
  {0x039, 0x000},
  {0x034, 0x001}
};

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
// National PC8374L
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_SIO_TABLE mDesktopSioTable[] = {
  {0x007, 0x03}, // Select Com1
  {0x061, 0xF8}, // 0x3F8
  {0x060, 0x03}, // 0x3F8
  {0x070, 0x04}, // IRQ4
  {0x030, 0x01}  // Active
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

GLOBAL_REMOVE_IF_UNREFERENCED EFI_SIO_TABLE mSioIt8628TableParallelPort[] = {
  {0x007, 0x03}, // Parallel Port Logical Device Number select
  {0x030, 0x00}, // Parallel port Activate
  {0x061, 0x78}, // Parallel Port Base Address 1 MSB Register
  {0x060, 0x03}, // Parallel Port Base Address 1 LSB Register
  {0x063, 0x78}, // Parallel Port Base Address 2 MSB Register
  {0x062, 0x07}, // Parallel Port Base Address 1 LSB Register
  {0x0F0, 0x03}  // Special Configuration register
};


GLOBAL_REMOVE_IF_UNREFERENCED EFI_SIO_TABLE mSioTableWinbondX374[] = {
  {0x07, 0x03},                   // Select UART0 device
  {0x60, (COM1_BASE >> 8)},       // Set Base Address MSB
  {0x61, (COM1_BASE & 0x00FF)},   // Set Base Address LSB
  {0x70, 0x04},                   // Set to IRQ4
  {0x30, 0x01}                    // Enable it with Activation bit
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_SIO_TABLE mSioTablePilot3[] = {
  {0x07, 0x02}, // Set logical device SP Serial port Com0
  {0x61, 0xF8}, // Write Base Address LSB register 0x3F8
  {0x60, 0x03}, // Write Base Address MSB register 0x3F8
  {0x70, 0x04}, // Write IRQ1 value (IRQ 1)  keyboard
  {0x30, 0x01}  // Enable serial port with Activation bit
};

/**
  Detect if a National 393 SIO is docked. If yes, enable the docked SIO
  and its serial port, and disable the onboard serial port.

  @retval EFI_SUCCESS     Operations performed successfully.
**/
STATIC
VOID
CheckNationalSio (
  VOID
  )
{
  UINT8           Data8;

  //
  // Pc87393 access is through either (0x2e, 0x2f) or (0x4e, 0x4f).
  // We use (0x2e, 0x2f) which is determined by BADD default strapping
  //

  //
  // Read the Pc87393 signature
  //
  IoWrite8 (0x2e, 0x20);
  Data8 = IoRead8 (0x2f);

  if (Data8 == 0xea) {
    //
    // Signature matches - National PC87393 SIO is docked
    //

    //
    // Enlarge the LPC decode scope to accommodate the Docking LPC Switch
    // Register (SIO_DOCKING_LPC_SWITCH_REGISTER_ADDRESS is allocated at
    // SIO_BASE_ADDRESS + 0x10)
    //
    PchLpcGenIoRangeSet ((FixedPcdGet16 (PcdSioBaseAddress) & (UINT16)~0x7F), 0x20);

    //
    // Enable port switch
    //
    IoWrite8 (SIO_DOCKING_LPC_SWITCH_REGISTER_ADDRESS, 0x06);

    //
    // Turn on docking power
    //
    IoWrite8 (FixedPcdGet16 (PcdSioBaseAddress) + 0x0E, 0x8c);

    IoWrite8 (FixedPcdGet16 (PcdSioBaseAddress) + 0x0E, 0x9c);

    IoWrite8 (FixedPcdGet16 (PcdSioBaseAddress) + 0x0E, 0xBc);

    //
    // Enable port switch
    //
    IoWrite8 (SIO_DOCKING_LPC_SWITCH_REGISTER_ADDRESS, 0x7);

    //
    // GPIO setting
    //
    IoWrite8 (0x2e, 0x24);
    IoWrite8 (0x2f, 0x29);

    //
    // Enable chip clock
    //
    IoWrite8 (0x2e, 0x29);
    IoWrite8 (0x2f, 0x1e);


    //
    // Enable serial port
    //

    //
    // Select com1
    //
    IoWrite8 (0x2e, 0x7);
    IoWrite8 (0x2f, 0x3);

    //
    // Base address: 0x3f8
    //
    IoWrite8 (0x2e, 0x60);
    IoWrite8 (0x2f, 0x03);
    IoWrite8 (0x2e, 0x61);
    IoWrite8 (0x2f, 0xf8);

    //
    // Interrupt: 4
    //
    IoWrite8 (0x2e, 0x70);
    IoWrite8 (0x2f, 0x04);

    //
    // Enable bank selection
    //
    IoWrite8 (0x2e, 0xf0);
    IoWrite8 (0x2f, 0x82);

    //
    // Activate
    //
    IoWrite8 (0x2e, 0x30);
    IoWrite8 (0x2f, 0x01);

    //
    // Disable onboard serial port
    //
    IoWrite8 (FixedPcdGet16 (PcdLpcSioConfigDefaultPort), 0x55);

    //
    // Power Down UARTs
    //
    IoWrite8 (PcdGet16 (PcdLpcSioIndexPort), 0x2);
    IoWrite8 (PcdGet16 (PcdLpcSioDataPort), 0x00);

    //
    // Dissable COM1 decode
    //
    IoWrite8 (PcdGet16 (PcdLpcSioIndexPort), 0x24);
    IoWrite8 (PcdGet16 (PcdLpcSioDataPort), 0);

    //
    // Disable COM2 decode
    //
    IoWrite8 (PcdGet16 (PcdLpcSioIndexPort), 0x25);
    IoWrite8 (PcdGet16 (PcdLpcSioDataPort), 0);

    //
    // Disable interrupt
    //
    IoWrite8 (PcdGet16 (PcdLpcSioIndexPort), 0x28);
    IoWrite8 (PcdGet16 (PcdLpcSioDataPort), 0x0);

    IoWrite8 (FixedPcdGet16 (PcdLpcSioConfigDefaultPort), 0xAA);

    //
    // Enable floppy
    //

    //
    // Select floppy
    //
    IoWrite8 (0x2e, 0x7);
    IoWrite8 (0x2f, 0x0);

    //
    // Base address: 0x3f0
    //
    IoWrite8 (0x2e, 0x60);
    IoWrite8 (0x2f, 0x03);
    IoWrite8 (0x2e, 0x61);
    IoWrite8 (0x2f, 0xf0);

    //
    // Interrupt: 6
    //
    IoWrite8 (0x2e, 0x70);
    IoWrite8 (0x2f, 0x06);

    //
    // DMA 2
    //
    IoWrite8 (0x2e, 0x74);
    IoWrite8 (0x2f, 0x02);

    //
    // Activate
    //
    IoWrite8 (0x2e, 0x30);
    IoWrite8 (0x2f, 0x01);

  } else {

    //
    // No National pc87393 SIO is docked, turn off dock power and
    // disable port switch
    //
    // IoWrite8 (SIO_BASE_ADDRESS + 0x0E, 0xbf);
    // IoWrite8 (0x690, 0);

    //
    // If no National pc87393, just return
    //
    return;
  }
}


/**
Check whether the IT8628 SIO present on LPC. If yes, enable its serial
ports, parallel port, and port 80.

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
                  PCI_DEVICE_NUMBER_PCH_LPC,
                  PCI_FUNCTION_NUMBER_PCH_LPC
                  );

  LpcIoDecondeRangeSet = (UINT16) MmioRead16 (LpcBaseAddr + R_PCH_LPC_IOD);
  LpcIoDecoodeSet = (UINT16) MmioRead16 (LpcBaseAddr + R_PCH_LPC_IOE);
  MmioWrite16 ((LpcBaseAddr + R_PCH_LPC_IOD), (LpcIoDecondeRangeSet | ((V_PCH_LPC_IOD_COMB_2F8 << 4) | V_PCH_LPC_IOD_COMA_3F8)));
  MmioWrite16 ((LpcBaseAddr + R_PCH_LPC_IOE), (LpcIoDecoodeSet | (B_PCH_LPC_IOE_SE | B_PCH_LPC_IOE_CBE | B_PCH_LPC_IOE_CAE)));

  //
  // Enter MB PnP Mode
  //
  IoWrite8 (LPC_SIO_INDEX_DEFAULT_PORT_2, 0x87);
  IoWrite8 (LPC_SIO_INDEX_DEFAULT_PORT_2, 0x01);
  IoWrite8 (LPC_SIO_INDEX_DEFAULT_PORT_2, 0x55);
  IoWrite8 (LPC_SIO_INDEX_DEFAULT_PORT_2, 0x55);

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
  if ((ChipId0 == 0x86) && (ChipId1 == 0x28)) {
    for (Index = 0; Index < sizeof (mSioIt8628TableSerialPort) / sizeof (EFI_SIO_TABLE); Index++) {
      IoWrite8 (LPC_SIO_INDEX_DEFAULT_PORT_2, mSioIt8628TableSerialPort[Index].Register);
      IoWrite8 (LPC_SIO_DATA_DEFAULT_PORT_2, mSioIt8628TableSerialPort[Index].Value);
    }
  }

  //
  // Exit MB PnP Mode
  //
  IoWrite8 (LPC_SIO_INDEX_DEFAULT_PORT_2, 0x02);
  IoWrite8 (LPC_SIO_DATA_DEFAULT_PORT_2, 0x02);

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
  UINT16  ConfigPort;
  UINT16  IndexPort;
  UINT16  DataPort;
  UINT16  DeviceId;
  UINT8   Index;
  UINT16  AcpiBase;

  //
  // Set the ICH ACPI Base Address (Reg#40h) and ACPI Enable bit
  // in ACPI Controll (Reg#44h bit7) for PrePpiStall function use.
  //
  IndexPort = 0;
  DataPort = 0;
  Index = 0;
  AcpiBase = 0;
  PchAcpiBaseGet (&AcpiBase);
  if (AcpiBase == 0) {
    PchAcpiBaseSet (PcdGet16 (PcdAcpiBaseAddress));
  }

  //
  // Enable I/O decoding for COM1(3F8h-3FFh), COM2(2F8h-2FFh), I/O port 2Eh/2Fh, 4Eh/4Fh, 60h/64Fh and 62h/66h.
  //
  PchLpcIoDecodeRangesSet (PcdGet16 (PcdLpcIoDecodeRange));
  PchLpcIoEnableDecodingSet (PcdGet16 (PchLpcIoEnableDecoding));

  // Configure Sio IT8628
  It8628SioSerialPortInit ();

  DeviceId = MmioRead16 (MmPciBase (SA_MC_BUS, 0, 0) + R_SA_MC_DEVICE_ID);
  if (IS_SA_DEVICE_ID_MOBILE (DeviceId)) {
    //
    // if no EC, it is SV Bidwell Bar board
    //
    if ((IoRead8 (0x66) != 0xFF) && (IoRead8 (0x62) != 0xFF)) {
      //
      // Super I/O initialization for SMSC SI1007
      //
      ConfigPort = FixedPcdGet16 (PcdLpcSioConfigDefaultPort);
      DataPort   = PcdGet16 (PcdLpcSioDataDefaultPort);
      IndexPort  = PcdGet16 (PcdLpcSioIndexDefaultPort);

      //
      // 128 Byte Boundary and SIO Runtime Register Range is 0x0 to 0xF;
      //
      PchLpcGenIoRangeSet (FixedPcdGet16 (PcdSioBaseAddress) & (~0x7F), 0x10);

      //
      // Program and Enable Default Super IO Configuration Port Addresses and range
      //
      PchLpcGenIoRangeSet (FixedPcdGet16 (PcdLpcSioConfigDefaultPort) & (~0xF), 0x10);

      //
      // Enter Config Mode
      //
      IoWrite8 (ConfigPort, 0x55);

      //
      // Check for SMSC SIO1007
      //
      IoWrite8 (IndexPort, 0x0D);   // SMSC SIO1007 Device ID register is 0x0D
      if (IoRead8 (DataPort) == 0x20) {   // SMSC SIO1007 Device ID is 0x20
        //
        // Configure SIO
        //
        for (Index = 0; Index < sizeof (mSioTable) / sizeof (EFI_SIO_TABLE); Index++) {
          IoWrite8 (IndexPort, mSioTable[Index].Register);
          IoWrite8 (DataPort, mSioTable[Index].Value);
        }

        //
        // Exit Config Mode
        //
        IoWrite8 (FixedPcdGet16 (PcdLpcSioConfigDefaultPort), 0xAA);

        //
        // GPIO 15-17:IN  10-14:OUT  Enable RS232  ref: Page42 of CRB_SCH
        //
        IoWrite8 (FixedPcdGet16 (PcdSioBaseAddress) + 0x0c, 0x1f);
      }

      //
      // Check if a National Pc87393 SIO is docked
      //
      CheckNationalSio ();

      //
      // Super I/O initialization for SMSC SIO1000
      //
      ConfigPort = PcdGet16 (PcdLpcSioIndexPort);
      IndexPort  = PcdGet16 (PcdLpcSioIndexPort);
      DataPort   = PcdGet16 (PcdLpcSioDataPort);

      //
      // Enter Config Mode
      //
      IoWrite8 (ConfigPort, 0x55);

      //
      // Check for SMSC SIO1000
      //
      if (IoRead8 (ConfigPort) != 0xFF) {
        //
        // Configure SIO
        //
        for (Index = 0; Index < sizeof (mSioTableSmsc1000) / sizeof (EFI_SIO_TABLE); Index++) {
          IoWrite8 (IndexPort, mSioTableSmsc1000[Index].Register);
          IoWrite8 (DataPort, mSioTableSmsc1000[Index].Value);
        }

        //
        // Exit Config Mode
        //
        IoWrite8 (FixedPcdGet16 (PcdLpcSioConfigDefaultPort), 0xAA);
      }

      //
      // Super I/O initialization for Winbond WPCN381U
      //
      IndexPort  = LPC_SIO_INDEX_DEFAULT_PORT_2;
      DataPort   = LPC_SIO_DATA_DEFAULT_PORT_2;

      //
      // Check for Winbond WPCN381U
      //
      IoWrite8 (IndexPort, 0x20);         // Winbond WPCN381U Device ID register is 0x20
      if (IoRead8 (DataPort) == 0xF4) {   // Winbond WPCN381U Device ID is 0xF4
        //
        // Configure SIO
        //
        for (Index = 0; Index < sizeof (mSioTableWpcn381u) / sizeof (EFI_SIO_TABLE); Index++) {
          IoWrite8 (IndexPort, mSioTableWpcn381u[Index].Register);
          IoWrite8 (DataPort, mSioTableWpcn381u[Index].Value);
        }
      }
    } //EC is not exist, skip mobile board detection for SV board

    //
    //add for SV Bidwell Bar board
    //
    if (IoRead8 (COM1_BASE) == 0xFF) {
      //
      // Super I/O initialization for Winbond WPCD374 (LDC2) and 8374 (LDC)
      // Looking for LDC2 card first
      //
      IoWrite8 (LEGACY_DAUGHTER_CARD_2_SIO_INDEX_PORT, 0x55);
      if (IoRead8 (LEGACY_DAUGHTER_CARD_2_SIO_INDEX_PORT) == 0x55) {
        IndexPort = LEGACY_DAUGHTER_CARD_2_SIO_INDEX_PORT;
        DataPort  = LEGACY_DAUGHTER_CARD_2_SIO_DATA_PORT;
      } else {
        IndexPort = LEGACY_DAUGHTER_CARD_SIO_INDEX_PORT;
        DataPort  = LEGACY_DAUGHTER_CARD_SIO_DATA_PORT;
      }

      IoWrite8 (IndexPort, 0x20);         // Winbond x374 Device ID register is 0x20
      if (IoRead8 (DataPort) == 0xF1) {   // Winbond x374 Device ID is 0xF1
        for (Index = 0; Index < sizeof (mSioTableWinbondX374) / sizeof (EFI_SIO_TABLE); Index++) {
          IoWrite8 (IndexPort, mSioTableWinbondX374[Index].Register);
          IoWrite8 (DataPort, mSioTableWinbondX374[Index].Value);
        }
      }
    }// end of Bidwell Bar SIO initialization
  } else if (IS_SA_DEVICE_ID_DESKTOP (DeviceId) ||  IS_SA_DEVICE_ID_SERVER (DeviceId)) {
    //
    // If we are in debug mode, we will allow serial status codes
    //

    //
    // National PC8374 SIO & Winbond WPCD374 (LDC2)
    //
    IndexPort  = LEGACY_DAUGHTER_CARD_2_SIO_INDEX_PORT;

    IoWrite8 (IndexPort, 0x55);
    if (IoRead8 (IndexPort) == 0x55) {
      IndexPort = LEGACY_DAUGHTER_CARD_2_SIO_INDEX_PORT;
      DataPort  = LEGACY_DAUGHTER_CARD_2_SIO_DATA_PORT;
    } else {
      IndexPort = LEGACY_DAUGHTER_CARD_SIO_INDEX_PORT;
      DataPort  = LEGACY_DAUGHTER_CARD_SIO_DATA_PORT;
    }

    //
    // Configure SIO
    //
    IoWrite8 (IndexPort, 0x20);         // Winbond x374 Device ID register is 0x20
    if (IoRead8 (DataPort) == 0xF1) {   // Winbond x374 Device ID is 0xF1
      for (Index = 0; Index < sizeof (mDesktopSioTable) / sizeof (EFI_SIO_TABLE); Index++) {
        IoWrite8 (IndexPort, mDesktopSioTable[Index].Register);
        //PrePpiStall (200);
        IoWrite8 (DataPort, mDesktopSioTable[Index].Value);
        //PrePpiStall (200);
      }
      return RETURN_SUCCESS;
    }
    //
    // Configure Pilot3 SIO
    //
    IoWrite8 (PILOTIII_SIO_INDEX_PORT, PILOTIII_UNLOCK); //Enter config mode.
    IoWrite8 (PILOTIII_SIO_INDEX_PORT, PILOTIII_CHIP_ID_REG);     // Pilot3 SIO Device ID register is 0x20.
    if (IoRead8 (PILOTIII_SIO_DATA_PORT) == PILOTIII_CHIP_ID) {    // Pilot3 SIO Device ID register is 0x03.
      //
      // Configure SIO
      //
      for (Index = 0; Index < sizeof (mSioTablePilot3) / sizeof (EFI_SIO_TABLE); Index++) {
        IoWrite8 (PILOTIII_SIO_INDEX_PORT, mSioTablePilot3[Index].Register);
        IoWrite8 (PILOTIII_SIO_DATA_PORT, mSioTablePilot3[Index].Value);
      }
    }
    IoWrite8 (PILOTIII_SIO_INDEX_PORT , PILOTIII_LOCK); //Exit config mode.
  }


  return RETURN_SUCCESS;
}
