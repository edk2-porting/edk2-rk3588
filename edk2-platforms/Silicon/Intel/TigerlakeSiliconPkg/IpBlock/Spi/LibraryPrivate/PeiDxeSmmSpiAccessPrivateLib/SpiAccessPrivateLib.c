/** @file
  SPI library for abstraction of SPI HW registers accesses

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/SpiAccessLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/DebugLib.h>
#include <Library/S3BootScriptLib.h>
#include <Register/SpiRegs.h>
#include <Register/PchRegs.h>
#include <Register/PchPcrRegs.h>
#include <Library/PchPciBdfLib.h>

/**
  Disable EISS (Enable InSMM.STS)
**/
VOID
SpiDisableEiss (
  VOID
  )
{
  UINT64  SpiBaseAddress;
  SpiBaseAddress = SpiPciCfgBase ();

  ASSERT ((PciSegmentRead8 (SpiBaseAddress + R_SPI_CFG_BC) & B_SPI_CFG_BC_LE) == 0);

  PciSegmentAnd8 (SpiBaseAddress + R_SPI_CFG_BC, (UINT8) ~(B_SPI_CFG_BC_EISS));
}

/**
  Configure BiosLockEnable bit and BiosInterfaceLock bit according to policy setting.

  @param[in] BiosLockEnable     Policy for BiosLockEnable bit programming
  @param[in] BiosInterfaceLock  Policy for BiosInterfaceLock bit programming

**/
VOID
SpiBiosLockEnableAndBiosInterfaceLockWithS3BootScript (
  IN BOOLEAN  BiosLockEnable,
  IN BOOLEAN  BiosInterfaceLock
  )
{
  UINT64                   SpiBaseAddress;
  UINT8                    SpiData8;

  if (!BiosLockEnable && !BiosInterfaceLock) {
    return;
  }

  SpiBaseAddress = SpiPciCfgBase ();

  ///
  /// PCH BIOS Spec Flash Security Recommendation
  ///
  /// BIOS needs to enable the BIOS Lock Enable (BLE) feature of the PCH by setting
  /// SPI/eSPI/LPC PCI offset DCh[1] = 1b.
  /// When this bit is set, attempts to write the Write Protect Disable (WPD) bit
  /// in PCH will cause a SMI which will allow the BIOS to verify that the write is
  /// from a valid source.
  /// Remember that BIOS needs to set SPI/LPC/eSPI PCI Offset DC [0] = 0b to enable
  /// BIOS region protection before exiting the SMI handler.
  /// Also, TCO_EN bit needs to be set (SMI_EN Register, ABASE + 30h[13] = 1b) to keep
  /// BLE feature enabled after booting to the OS.
  /// Intel requires that BIOS enables the Lock Enable (LE) feature of the PCH to
  /// ensure SMM protection of flash.
  /// RC installs a default SMI handler that clears WPD.
  /// There could be additional SMI handler to log such attempt if desired.
  ///
  /// BIOS needs to enable the "Enable in SMM.STS" (EISS) feature of the PCH by setting
  /// SPI PCI offset DCh[5] = 1b for SPI or setting eSPI PCI offset DCh[5] = 1b for eSPI.
  /// When this bit is set, the BIOS region is not writable until SMM sets the InSMM.STS bit,
  /// to ensure BIOS can only be modified from SMM. Please refer to CPU BWG for more details
  /// on InSMM.STS bit.
  /// Intel requires that BIOS enables the Lock Enable (LE) feature of the PCH to ensure
  /// SMM protection of flash.
  /// SPI PCI offset DCh[1] = 1b for SPI or setting eSPI PCI offset DCh[1] = 1b for eSPI.
  /// When this bit is set, EISS is locked down.
  ///
  SpiData8 = 0;
  if (BiosLockEnable) {
    SpiData8 |= B_SPI_CFG_BC_EISS | B_SPI_CFG_BC_LE;
  }
  ///
  /// BIOS also needs to set the BIOS Interface Lock Down bit in multiple locations
  /// (PCR[DMI] + 274Ch[0], LPC/eSPI PCI offset DCh[7] and SPI PCI offset DCh[7]).
  /// (done in PchInit/Dxe/PchInit.c by PchDmiSetBiosLockDownWithS3BootScript ()) for PCR[DMI] 274Ch)
  /// Setting these bits will prevent writes to the Top Swap bit (under their respective locations)
  /// and the Boot BIOS Straps. Enabling this bit will mitigate malicious software
  /// attempts to replace the system BIOS option ROM with its own code.
  ///
  if (BiosInterfaceLock) {
    SpiData8 |= B_SPI_CFG_BC_BILD;
  }

  PciSegmentOr8 (SpiBaseAddress + R_SPI_CFG_BC, SpiData8);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint8,
    PcdGet64 (PcdPciExpressBaseAddress) + SpiBaseAddress + R_SPI_CFG_BC,
    1,
    (VOID *) (UINTN) (PcdGet64 (PcdPciExpressBaseAddress) + SpiBaseAddress + R_SPI_CFG_BC)
    );
  //
  // Reads back for posted write to take effect
  //
  SpiData8 = PciSegmentRead8 (SpiBaseAddress + R_SPI_CFG_BC);
  S3BootScriptSaveMemPoll  (
    S3BootScriptWidthUint8,
    PcdGet64 (PcdPciExpressBaseAddress) + SpiBaseAddress + R_SPI_CFG_BC,
    &SpiData8,
    &SpiData8,
    1,
    1
    );
}

/**
  Clears BIOS Write Protect Disable bit
**/
VOID
SpiClearBiosWriteProtectDisable (
  VOID
  )
{
  //
  // Disable BIOSWE bit to protect BIOS
  //
  PciSegmentAnd8 (SpiPciCfgBase () + R_SPI_CFG_BC, (UINT8) ~B_SPI_CFG_BC_WPD);
}
