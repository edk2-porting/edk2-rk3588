/** @file
  Pch common library for PCH INIT PEI/DXE/SMM modules

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/MmPciLib.h>
#include <PchAccess.h>
#include <PchPolicyCommon.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchP2sbLib.h>
#include <Library/PchSbiAccessLib.h>
#include <Library/PchInfoLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/TimerLib.h>

extern CONST PCH_PCIE_CONTROLLER_INFO mPchPcieControllerInfo[];
extern CONST UINT32 mPchPcieControllerInfoSize;

#define PORT_PLS_TIMEOUT 100 ///< 100 * 10 us = 1ms timeout for USB3 PortSC PLS polling

/**
  This function returns PID according to PCIe controller index

  @param[in] ControllerIndex     PCIe controller index

  @retval PCH_SBI_PID    Returns PID for SBI Access
**/
PCH_SBI_PID
PchGetPcieControllerSbiPid (
  IN  UINT32  ControllerIndex
  )
{
  ASSERT (ControllerIndex < mPchPcieControllerInfoSize);
  return mPchPcieControllerInfo[ControllerIndex].Pid;
}

/**
  This function returns PID according to Root Port Number

  @param[in] RpIndex     Root Port Index (0-based)

  @retval PCH_SBI_PID    Returns PID for SBI Access
**/
PCH_SBI_PID
GetRpSbiPid (
  IN  UINTN  RpIndex
  )
{
  return PchGetPcieControllerSbiPid ((UINT32) (RpIndex / PCH_PCIE_CONTROLLER_PORTS));
}

/**
  Calculate root port device number based on physical port index.

  @param[in]  RpIndex              Root port index (0-based).

  @retval     Root port device number.
**/
UINT32
PchGetPcieRpDevice (
  IN  UINT32   RpIndex
  )
{
  UINTN ControllerIndex;
  ControllerIndex = RpIndex / PCH_PCIE_CONTROLLER_PORTS;
  ASSERT (ControllerIndex < mPchPcieControllerInfoSize);
  return mPchPcieControllerInfo[ControllerIndex].DevNum;
}

/**
  This function reads Pci Config register via SBI Access

  @param[in]  RpIndex             Root Port Index (0-based)
  @param[in]  Offset              Offset of Config register
  @param[out] *Data32             Value of Config register

  @retval EFI_SUCCESS             SBI Read successful.
**/
EFI_STATUS
PchSbiRpPciRead32 (
  IN    UINT32  RpIndex,
  IN    UINT32  Offset,
  OUT   UINT32  *Data32
  )
{
  EFI_STATUS    Status;
  UINT32        RpDevice;
  UINT8         Response;
  UINT16        Fid;


  RpDevice = PchGetPcieRpDevice (RpIndex);
  Fid = (UINT16) ((RpDevice << 3) | (RpIndex % 4 ));
  Status = PchSbiExecutionEx (
             GetRpSbiPid (RpIndex),
             Offset,
             PciConfigRead,
             FALSE,
             0xF,
             0,
             Fid,
             Data32,
             &Response
             );
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR,"Sideband Read Failed\n"));
    ASSERT (FALSE);
  }
  return Status;
}

/**
  This function And then Or Pci Config register via SBI Access

  @param[in]  RpIndex             Root Port Index (0-based)
  @param[in]  Offset              Offset of Config register
  @param[in]  Data32And           Value of Config register to be And-ed
  @param[in]  Data32AOr           Value of Config register to be Or-ed

  @retval EFI_SUCCESS             SBI Read and Write successful.
**/
EFI_STATUS
PchSbiRpPciAndThenOr32 (
  IN  UINT32  RpIndex,
  IN  UINT32  Offset,
  IN  UINT32  Data32And,
  IN  UINT32  Data32Or
  )
{
  EFI_STATUS  Status;
  UINT32      RpDevice;
  UINT32      Data32;
  UINT8       Response;
  UINT16      Fid;

  RpDevice = PchGetPcieRpDevice (RpIndex);
  Status = PchSbiRpPciRead32 (RpIndex, Offset, &Data32);
  if (Status == EFI_SUCCESS) {
    Data32 &= Data32And;
    Data32 |= Data32Or;
    Fid = (UINT16) ((RpDevice << 3) | (RpIndex % 4 ));
    Status = PchSbiExecutionEx (
               GetRpSbiPid (RpIndex),
               Offset,
               PciConfigWrite,
               FALSE,
               0xF,
               0,
               Fid,
               &Data32,
               &Response
               );
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR,"Sideband Write Failed\n"));
      ASSERT (FALSE);
    }
  } else {
    ASSERT (FALSE);
  }
  return Status;
}

/**
  Check if RST PCIe Storage Remapping is enabled based on policy

  @param[in] RstPcieStorageRemap    The PCH PCIe Storage remapping

  @retval TRUE                      RST PCIe Storage Remapping is enabled
  @retval FALSE                     RST PCIe Storage Remapping is disabled
**/
BOOLEAN
IsRstPcieStorageRemapEnabled (
  IN  CONST PCH_RST_PCIE_STORAGE_CONFIG   *RstPcieStorageRemap
  )
{
  BOOLEAN                      RstPcieStorageRemapEnabled;
  UINTN                        Index;

  RstPcieStorageRemapEnabled  = FALSE;

  DEBUG ((DEBUG_INFO, "IsRstPcieStorageRemapEnabled Started\n"));

  for (Index = 0; Index < PCH_MAX_RST_PCIE_STORAGE_CR; Index++) {
    if (RstPcieStorageRemap[Index].Enable == 1) {
      RstPcieStorageRemapEnabled = TRUE;
    }
  }

  DEBUG ((DEBUG_INFO, "IsRstPcieStorageRemapEnabled Ended\n"));
  return RstPcieStorageRemapEnabled;
}

/**
  Disable the RST remap address decoding range while RST is disabled.

  @param[in] AhciBar                ABAR address
**/
VOID
DisableRstRemapDecoding (
  UINT32                            AhciBar
  )
{
  UINTN                             PciSataRegBase;
  UINT32                            OrgAhciBar;
  UINT8                             OrgCmd;

  if (AhciBar == 0) {
    DEBUG ((DEBUG_ERROR, "DisableRstRemapDecoding: Invalid AHCI BAR address.\n"));
    ASSERT (FALSE);
    return;
  }

  PciSataRegBase  = MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_SATA, PCI_FUNCTION_NUMBER_PCH_SATA);
  OrgCmd          = MmioRead8 (PciSataRegBase + PCI_COMMAND_OFFSET);
  OrgAhciBar      = MmioRead32 (PciSataRegBase + R_PCH_SATA_AHCI_BAR);

  if (AhciBar != OrgAhciBar) {
    MmioWrite8 (PciSataRegBase + PCI_COMMAND_OFFSET, 0);
    MmioWrite32 (PciSataRegBase + R_PCH_SATA_AHCI_BAR, AhciBar);
  }
  MmioWrite8 (PciSataRegBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // Program "NVM Remap Memory BAR Enable",  AHCIBar offset A4h [0] to ['0b']
  // Also set NRMO AHCIBar offset A4h [27:16] to 0x00F, and MSL AHCIBar offset A4h [12:1] to 0.
  //
  DEBUG ((DEBUG_INFO, "DisableRstRemapDecoding: Program AHCIBar offset A4h [27:16, 12:1, 0] to [0x00F, 0, 0] to disable remap decoding\n"));
  MmioWrite32 (AhciBar + R_PCH_SATA_VS_CAP, (0x00F << N_PCH_SATA_VS_CAP_NRMO));

  if (AhciBar != OrgAhciBar) {
    MmioWrite8 (PciSataRegBase + PCI_COMMAND_OFFSET, 0);
    MmioWrite32 (PciSataRegBase + R_PCH_SATA_AHCI_BAR, OrgAhciBar);
  }
  MmioWrite8 (PciSataRegBase + PCI_COMMAND_OFFSET, OrgCmd);
}

/**
  This function lock down the P2sb SBI before going into OS.

  @param[in] SbiUnlock
**/
VOID
ConfigureP2sbSbiLock (
  IN  CONST BOOLEAN             SbiUnlock
  )
{
  UINTN                                 P2sbBase;
  BOOLEAN                               P2sbOrgStatus;

  if (SbiUnlock == FALSE) {
    P2sbBase = MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_P2SB, PCI_FUNCTION_NUMBER_PCH_P2SB);
    PchRevealP2sb (P2sbBase, &P2sbOrgStatus);

    ///
    /// Set P2SB PCI Offset 0xE0[31] to 1 to lock down SBI interface.
    /// @note: SbiLock is write once. Please make sure it's not touched earlier.
    ///
    MmioOr8 (P2sbBase + R_PCH_P2SB_E0 + 3, BIT7);

    if (!P2sbOrgStatus) {
      PchHideP2sb (P2sbBase);
    }
  }
}

/**
  Bios will remove the host accessing right to Sideband register range
  prior to any 3rd party code execution.

  1) Set EPMASK5 Offset C4 (bits 29, 28, 27, 26, 17, 16 (PCH-LP only), 10, 1) to disable Sideband access for PSF and MIPI controller
  2) Set EPMASK7 Offset CC (bits 6, 5) to disable Sideband access for XHCI controller
  3) Set the "Endpoint Mask Lock!", P2SB PCI offset E2h bit[1] to 1.

**/
VOID
RemoveSidebandAccess (
  VOID
  )
{
  UINTN                                 P2sbBase;
  BOOLEAN                               P2sbOrgStatus;

  P2sbBase = MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_P2SB, PCI_FUNCTION_NUMBER_PCH_P2SB);
  PchRevealP2sb (P2sbBase, &P2sbOrgStatus);

  //
  // Set EPMASK for ports B1, B0, AA, A1 (bits 17, 16 (PCH-LP only), 10, 1) to disable Sideband access for MIPI controller
  // Set EPMASK for ports BD, BC, BB, BA (bits 29, 28, 27, 26) to disable Sideband access for PCI Root Bridge
  //
  if (GetPchSeries () == PchLp) {
    MmioOr32 (P2sbBase + R_PCH_P2SB_EPMASK5, BIT29 | BIT28 | BIT27 | BIT26 | BIT17 | BIT16 | BIT10 | BIT1);
  } else {
    MmioOr32 (P2sbBase + R_PCH_P2SB_EPMASK5, BIT29 | BIT28 | BIT27 | BIT26 | BIT17         | BIT10 | BIT1);
  }

  //
  // Set EPMASK for ports E6, E5 (bits 6, 5) to disable Sideband access for XHCI controller
  //
  MmioOr32(P2sbBase + R_PCH_P2SB_EPMASK7, BIT6 | BIT5);

  DEBUG((DEBUG_INFO, "Lock the EPMASK registers\n"));

  MmioOr8 (P2sbBase + R_PCH_P2SB_E0 + 2, BIT1);

  if (!P2sbOrgStatus) {
    PchHideP2sb (P2sbBase);
  }
}

/**
  Configure PMC static function disable lock
**/
VOID
ConfigurePmcStaticFunctionDisableLock (
  VOID
  )
{
  UINT32                                PchPwrmBase;
  PchPwrmBaseGet (&PchPwrmBase);

  MmioOr32 (PchPwrmBase + R_PCH_PWRM_ST_PG_FDIS_PMC_1, B_PCH_PWRM_ST_PG_FDIS_PMC_1_ST_FDIS_LK);
}

/**
  Print registers value

  @param[in] PrintMmioBase       Mmio base address
  @param[in] PrintSize           Number of registers
  @param[in] OffsetFromBase      Offset from mmio base address

  @retval None
**/
VOID
PrintRegisters (
  IN  UINTN        PrintMmioBase,
  IN  UINT32       PrintSize,
  IN  UINT32       OffsetFromBase
  )
{
  UINT32  Offset;
  DEBUG ((DEBUG_VERBOSE, "       00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F"));
  for (Offset = 0; Offset < PrintSize; Offset++) {
    if ((Offset % 16) == 0) {
      DEBUG ((DEBUG_VERBOSE, "\n %04X: ", (Offset + OffsetFromBase) & 0xFFF0));
    }
    DEBUG ((DEBUG_VERBOSE, "%02X ", MmioRead8 (PrintMmioBase + Offset)));
  }
  DEBUG ((DEBUG_VERBOSE, "\n"));
}

VOID
PrintPchPciConfigSpace (
  VOID
  )
{
  DEBUG ((DEBUG_VERBOSE, "[ 0 - 00 - 0 ] Host Bridge\n"));
  PrintRegisters (MmPciBase ( 0,0,0 ), 256, 0);
  DEBUG ((DEBUG_VERBOSE, "[ 0 - 01 - 0 ] PCI Bridge (0-1) x16@1 (x16)\n"));
  PrintRegisters (MmPciBase ( 0,1,0 ), 256, 0);
  DEBUG ((DEBUG_VERBOSE, "[ 0 - 02 - 0 ] IGD\n"));
  PrintRegisters (MmPciBase ( 0,2,0 ), 256, 0);
  DEBUG ((DEBUG_VERBOSE, "[ 0 - 03 - 0 ] High Def Audio\n"));
  PrintRegisters (MmPciBase (00, 03, 00), 256, 0);
  DEBUG ((DEBUG_VERBOSE, "[ 0 - 20(0x14) - 0 ] XHCI Controller\n"));
  PrintRegisters (MmPciBase (00, 20, 00), 256, 0);
  DEBUG ((DEBUG_VERBOSE, "[ 0 - 22(0x16) - 0 ] ME\n"));
  PrintRegisters (MmPciBase (00, 22, 00), 256, 0);
  DEBUG ((DEBUG_VERBOSE, "[ 0 - 22(0x16) - 2 ] ME IDER\n"));
  PrintRegisters (MmPciBase (00, 22, 02), 256, 0);
  DEBUG ((DEBUG_VERBOSE, "[ 0 - 22(0x16) - 3 ] ME SOL\n"));
  PrintRegisters (MmPciBase (00, 22, 03), 256, 0);
  DEBUG ((DEBUG_VERBOSE, "[ 0 - 25(0x19) - 0 ] LAN\n"));
  PrintRegisters (MmPciBase (00, 25, 00), 256, 0);
  DEBUG ((DEBUG_VERBOSE, "[ 0 - 26(0x1A) - 0 ] EHCI Controller\n"));
  PrintRegisters (MmPciBase (00, 26, 00), 256, 0);
  DEBUG ((DEBUG_VERBOSE, "[ 0 - 27(0x1B) - 0 ] High Def Audio\n"));
  PrintRegisters (MmPciBase (00, 27, 00), 256, 0);
  DEBUG ((DEBUG_VERBOSE, "[ 0 - 28(0x1C) - 0 ] PCI Bridge (0-2) x0@1 (x1)\n"));
  PrintRegisters (MmPciBase (00, 28, 00), 256, 0);
  DEBUG ((DEBUG_VERBOSE, "PCI Bridge: AECH\n"));
  PrintRegisters (MmPciBase (00, 28, 00) + 0x0100, 0x040,0x0100);
  DEBUG ((DEBUG_VERBOSE, "PCI Bridge: DFT\n"));
  PrintRegisters (MmPciBase (00, 28, 00) + 0x0310, 0x030,0x0310);
  DEBUG ((DEBUG_VERBOSE, "PCI Bridge: VC0\n"));
  PrintRegisters (MmPciBase (00, 28, 00) + 0x0340, 0x020,0x0340);
  DEBUG ((DEBUG_VERBOSE, "PCI Bridge: Port Configuration Extension\n"));
  PrintRegisters (MmPciBase (00, 28, 00) + 0x0400, 0x020,0x0400);
  DEBUG ((DEBUG_VERBOSE, "[ 0 - 28(0x1C) - 6 ] PCI Bridge (0-3:10) x0@1 (x1)\n"));
  PrintRegisters (MmPciBase (00, 28, 06), 256, 0);
  DEBUG ((DEBUG_VERBOSE, "[ 0 - 29(0x1D) - 0 ] EHCI Controller\n"));
  PrintRegisters (MmPciBase (00, 29, 00), 256, 0);
  DEBUG ((DEBUG_VERBOSE, "[ 0 - 31(0x1F) - 0 ] ISA Bridge\n"));
  PrintRegisters (MmPciBase (00, 31, 00), 256, 0);
  DEBUG ((DEBUG_VERBOSE, "[ 0 - 31(0x1F) - 2 ] SATA\n"));
  PrintRegisters (MmPciBase (00, 31, 02), 256, 0);
  DEBUG ((DEBUG_VERBOSE, "[ 0 - 31(0x1F) - 4 ] SMBus Controller\n"));
  PrintRegisters (MmPciBase (00, 31, 04), 256, 0);

  DEBUG ((DEBUG_VERBOSE, "\n\n"));
}

/**
  Program Xhci Port Disable Override

  @param[in] XhciMmioBase         xHCI controller MBAR0 address
  @param[in] Usb2DisabledPorts    Disabled USB2 ports where each port has its disabling bit
  @param[in] Usb3DisabledPorts    Disabled USB3 ports where each port has its disabling bit

  @retval TRUE if platform reset is needed, otherwise FALSE is returned
**/
BOOLEAN
UsbPdoProgramming (
  IN  UINTN   XhciMmioBase,
  IN  UINT32  Usb2DisabledPorts,
  IN  UINT32  Usb3DisabledPorts
  )
{
  UINT32    Index;
  UINT32    XhciUsb2Pdo;
  UINT32    XhciUsb3Pdo;
  UINT32    XhciUsb2PdoRd;
  UINT32    XhciUsb3PdoRd;
  UINT32    XhciUsb3PortScStartingOffset;
  UINT32    XhciUsb3PortScOffset;
  UINT32    XhciPlsTimeout;

  DEBUG ((DEBUG_INFO, "UsbPdoProgramming () start\n"));
  DEBUG ((DEBUG_INFO, "xHCI: Usb2DisabledPorts = 0x%x\n", Usb2DisabledPorts));
  DEBUG ((DEBUG_INFO, "xHCI: Usb3DisabledPorts = 0x%x\n", Usb3DisabledPorts));

  //
  // XHCI PDO for HS
  //
  if (GetPchSeries () == PchLp) {
    XhciUsb2Pdo = MmioRead32 (XhciMmioBase + R_PCH_XHCI_USB2PDO) & B_PCH_XHCI_LP_USB2PDO_MASK;
  } else {
    XhciUsb2Pdo = MmioRead32 (XhciMmioBase + R_PCH_XHCI_USB2PDO) & B_PCH_XHCI_H_USB2PDO_MASK;
  }
  XhciUsb2Pdo |= Usb2DisabledPorts;

  //
  // XHCI PDO for SS
  //
  if (GetPchSeries () == PchLp) {
    XhciUsb3Pdo = MmioRead32 (XhciMmioBase + R_PCH_XHCI_USB3PDO) & B_PCH_XHCI_LP_USB3PDO_MASK;
    XhciUsb3PortScStartingOffset = R_PCH_LP_XHCI_PORTSC01USB3;
  } else {
    XhciUsb3Pdo = MmioRead32 (XhciMmioBase + R_PCH_XHCI_USB3PDO) & B_PCH_XHCI_H_USB3PDO_MASK;
    XhciUsb3PortScStartingOffset = R_PCH_H_XHCI_PORTSC01USB3;
  }

  for (Index = 0; Index < GetPchXhciMaxUsb3PortNum (); Index++) {
    // Check if port should be disabled
    if ((Usb3DisabledPorts & (BIT0 << Index)) != 0) {
      // Check if PDO bit has not been set yet
      if (((XhciUsb3Pdo & (UINT32)(BIT0 << Index)) == 0)) {
        // Calculate port PortSC register offset in xHCI MMIO space
        XhciUsb3PortScOffset = (XhciUsb3PortScStartingOffset + (Index * 0x10));

        // Disable port before setting PDO bit for it
        MmioOr32 (XhciMmioBase + XhciUsb3PortScOffset, B_PCH_XHCI_PORTSCXUSB3_PED);

        // Poll for PLS (PortSC[5:8]) to reach 0x4 before setting PDO for the port
        for (XhciPlsTimeout = 0; XhciPlsTimeout < PORT_PLS_TIMEOUT; XhciPlsTimeout++) {
          if ((MmioRead32 (XhciMmioBase + XhciUsb3PortScOffset) & B_PCH_XHCI_PORTSCXUSB3_PLS) == V_PCH_XHCI_PORTSCXUSB3_PLS_DISABLED) {
            break;
          }
          MicroSecondDelay (10);
        }

        if (XhciPlsTimeout == PORT_PLS_TIMEOUT) {
          DEBUG ((DEBUG_ERROR, "[xHCI PDO] PLS Polling for Disabled has timed out for port %d\n", Index + 1));
        }

        // A '1' in a bit position prevents the corresponding USB3 port from reporting a Device Connection to the XHC
        XhciUsb3Pdo |= (UINT32) (B_PCH_XHCI_USB3PDO_DIS_PORT0 << Index);

        // Put port back to RxDetect state
        // Need to write LWS and PLS regs with same MMIO operation otherwise PLS write will be ignored
        MmioOr32 (XhciMmioBase + XhciUsb3PortScOffset, (B_PCH_XHCI_PORTSCXUSB3_LWS | V_PCH_XHCI_PORTSCXUSB3_PLS_RXDETECT));
      }
    }
  }

  //
  // USB2PDO and USB3PDO are Write-Once registers and bits in them are in the SUS Well.
  //
  MmioWrite32 (XhciMmioBase + R_PCH_XHCI_USB2PDO, XhciUsb2Pdo);
  MmioWrite32 (XhciMmioBase + R_PCH_XHCI_USB3PDO, XhciUsb3Pdo);

  XhciUsb2PdoRd = MmioRead32 (XhciMmioBase + R_PCH_XHCI_USB2PDO);
  XhciUsb3PdoRd = MmioRead32 (XhciMmioBase + R_PCH_XHCI_USB3PDO);

  //
  // If USB2PDO and USB3PDO are not updated successfully perform Warm Reset to unlock RWO bits.
  //
  if ((XhciUsb2Pdo != XhciUsb2PdoRd) || (XhciUsb3Pdo != XhciUsb3PdoRd)) {
    DEBUG ((DEBUG_ERROR, "UsbPdoProgramming: PDO register locked. Performing Warm Reset\n"));
    return TRUE;
  }

  DEBUG ((DEBUG_INFO, "UsbPdoProgramming () end\n"));
  return FALSE;
}

