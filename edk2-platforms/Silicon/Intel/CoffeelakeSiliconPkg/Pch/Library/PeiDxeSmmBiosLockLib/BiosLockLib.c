/** @file
  Bios Lock library.

  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/S3BootScriptLib.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsLpc.h>
#include <Register/PchRegsSpi.h>

/**
  Enable BIOS lock. This will set the LE (Lock Enable) and EISS (Enable In SMM.STS).
  When this is set, attempts to write the WPD (Write Protect Disable) bit in PCH
  will cause a SMI which will allow the BIOS to verify that the write is from a valid source.

  Bios should always enable LockDownConfig.BiosLock policy to set Bios Lock bit in FRC.
  If capsule udpate is enabled, it's expected to not do BiosLock by setting BiosLock policy disable
  so it can udpate BIOS region.
  After flash update, it should utilize this lib to do BiosLock for security.
**/
VOID
BiosLockEnable (
  VOID
  )
{
  UINT64  LpcBaseAddress;
  UINT64  SpiBaseAddress;

  LpcBaseAddress = PCI_SEGMENT_LIB_ADDRESS (
                     DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_LPC,
                     PCI_FUNCTION_NUMBER_PCH_LPC,
                     0
                     );
  SpiBaseAddress = PCI_SEGMENT_LIB_ADDRESS (
                     DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_SPI,
                     PCI_FUNCTION_NUMBER_PCH_SPI,
                     0
                     );

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
  PciSegmentOr8 (SpiBaseAddress + R_SPI_CFG_BC, B_SPI_CFG_BC_EISS | B_SPI_CFG_BC_LE);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint8,
    PcdGet64 (PcdPciExpressBaseAddress) + SpiBaseAddress + R_SPI_CFG_BC,
    1,
    (VOID *) (UINTN) (PcdGet64 (PcdPciExpressBaseAddress) + SpiBaseAddress + R_SPI_CFG_BC)
    );
  PciSegmentOr8 (LpcBaseAddress + R_LPC_CFG_BC, B_LPC_CFG_BC_EISS | B_LPC_CFG_BC_LE);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint8,
    PcdGet64 (PcdPciExpressBaseAddress) + LpcBaseAddress + R_LPC_CFG_BC,
    1,
    (VOID *) (UINTN) (PcdGet64 (PcdPciExpressBaseAddress) + LpcBaseAddress + R_LPC_CFG_BC)
    );
}
