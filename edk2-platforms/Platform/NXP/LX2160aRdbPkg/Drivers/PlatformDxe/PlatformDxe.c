/** @file
*
*  Copyright 2020 NXP
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/SocLib.h>
#include <Soc.h>

/**
  Set PCDs for PCIe controller based on SoC version.
  LX2160-Rev1 and LX2160-Rev2 has different PCIe controller. This function
  check the SoC version and set PCDs for PCIe controller which will be
  used by PciHostBridgeLib and PciSegmentLib for controller initialization.

  @return EFI_SUCCESS            PCDs were set successfully
  @return EFI_INVALID_PARAMETER  Invalid major version number
**/
EFI_STATUS
SetPciControllerPcdOptions (
  VOID
  )
{
  UINT32 Svr;

  Svr = SocGetSvr ();
  if (SVR_SOC_VER(Svr) == SVR_LX2160A) {
    switch (SVR_MAJOR(Svr)) {
    case 0x1:
      //
      // LX2160-Rev1 and LX2160-Rev2 has different PCIe controllers.
      // Set PcdPciLsGen4Ctrl to TRUE for LX2160-Rev1, which will be used
      // by PciHostBridgeLib and PciSegmentLib to differentiate both controllers
      // and perform controller specific initialization.
      //
      PcdSetBoolS (PcdPciLsGen4Ctrl, TRUE);
      break;
    case 0x2:
      //
      // PCIe controller in LX2160-Rev2 supports two methods for config
      // transactions.
      // 1 - Default (Non ECAM compliant): PCIe controller
      // requires target BDF to be written to bit[31:16] of type0/type1
      // outbound window.
      // 2 - CFG SHIFT: PCIe controller shifts BDF from bits[27:12] to
      // bits[31:16] and supports Enhanced Configuration Address Mapping (ECAM)
      // mechanism.
      //
      // Set PcdPciCfgShiftEnable to TRUE for LX2160-Rev2, which will be used by
      // PciHostBridgeLib and PciSegmentLib to enable CFG SHIFT feature on
      // PCIe controller and program the iATU windows accordingly.
      //
      PcdSetBoolS (PcdPciCfgShiftEnable, TRUE);

      //
      // PCIe controller in LX2160-Rev2 is not ECAM-compliant for bus0.
      // Set PcdPciHideRootPort for LX2160-Rev2, which will be used by
      // PciHostBridgeLib and PciSegmentLib to program iATU windows accordingly.
      //
      PcdSetBoolS (PcdPciHideRootPort, TRUE);
      break;
    default:
      DEBUG ((DEBUG_ERROR, "%a: Invalid SoC Version 0x%x \n", __FUNCTION__,
              SVR_MAJOR(Svr)));
      return EFI_INVALID_PARAMETER;
    }
  }

  return EFI_SUCCESS;
}

/**
  The entry point for PlatformDxe driver. This driver
  intends to perform platform specific initialization.

  @param[in] ImageHandle     The image handle of the driver.
  @param[in] SystemTable     The system table.

  @retval EFI_SUCCESS         Driver initialization success.

**/
EFI_STATUS
EFIAPI
PlatformDxeEntryPoint (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS Status;

  // Set PCDs for PCIe controller
  Status = SetPciControllerPcdOptions ();

  return Status;
}
