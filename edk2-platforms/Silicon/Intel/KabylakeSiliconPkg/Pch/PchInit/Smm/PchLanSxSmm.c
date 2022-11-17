/** @file
  PCH LAN Sx handler implementation.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/TimerLib.h>
#include "PchInitSmm.h"

//
// Maximum loop time for GbE status check
//
#define GBE_MAX_LOOP_TIME  4000

/**
  Checks if Lan is Enabled or Disabled

  @retval  BOOLEAN    TRUE if device is enabled, FALSE otherwise.
**/
BOOLEAN
IsGbeEnabled (
  VOID
  )
{
  UINTN  GbePciBase;

  GbePciBase = MmPciBase (
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_LAN,
                 PCI_FUNCTION_NUMBER_PCH_LAN
                 );

  if (MmioRead32 (GbePciBase) != 0xFFFFFFFF) {
    return TRUE;
  }

  return FALSE;
}



/**
  Test for MDIO operation complete.

  @param [in]  GbeBar   GbE MMIO space

  @retval EFI_SUCCESS
  @retval EFI_TIMEOUT
**/
EFI_STATUS
GbeMdiWaitReady (
  UINT32          GbeBar
  )
{
  UINT32  Count;

  for (Count = 0; Count < GBE_MAX_LOOP_TIME; ++Count) {
    if (MmioRead32 (GbeBar + R_PCH_LAN_CSR_MDIC) & B_PCH_LAN_CSR_MDIC_RB) {
      return EFI_SUCCESS;
    }
    MicroSecondDelay (50);
  }
  return EFI_TIMEOUT;
}

/**
  Acquire MDIO software semaphore.

  1. Ensure that MBARA offset F00h [5] = 1b
  2. Poll MBARA offset F00h [5] up to 200ms

  @param [in] GbeBar   GbE MMIO space

  @retval EFI_SUCCESS
  @retval EFI_TIMEOUT
**/
EFI_STATUS
GbeAcquireMdio (
  IN      UINT32  GbeBar
  )
{
  UINT32  ExtCnfCtrl;
  UINT32  Count;

  MmioOr32 (GbeBar + R_PCH_LAN_CSR_EXTCNF_CTRL, B_PCH_LAN_CSR_EXTCNF_CTRL_SWFLAG);
  for (Count = 0; Count < GBE_MAX_LOOP_TIME; ++Count) {
    ExtCnfCtrl = MmioRead32 (GbeBar + R_PCH_LAN_CSR_EXTCNF_CTRL);
    if (ExtCnfCtrl & B_PCH_LAN_CSR_EXTCNF_CTRL_SWFLAG) {
      return EFI_SUCCESS;
    }
    MicroSecondDelay (50);
  }
  ASSERT (FALSE);
  return EFI_TIMEOUT;
}


/**
  Release MDIO software semaphore by clearing MBARA offset F00h [5]

  @param [in]  GbeBar   GbE MMIO space
**/
VOID
GbeReleaseMdio (
  IN      UINT32  GbeBar
  )
{
  ASSERT (MmioRead32 (GbeBar + R_PCH_LAN_CSR_EXTCNF_CTRL) & B_PCH_LAN_CSR_EXTCNF_CTRL_SWFLAG);
  MmioAnd32 (GbeBar + R_PCH_LAN_CSR_EXTCNF_CTRL, (UINT32) ~B_PCH_LAN_CSR_EXTCNF_CTRL_SWFLAG);
  ASSERT ((MmioRead32 (GbeBar + R_PCH_LAN_CSR_EXTCNF_CTRL) & B_PCH_LAN_CSR_EXTCNF_CTRL_SWFLAG) == 0);
}

/**
  Perform MDI write.

  @param [in]  GbeBar   GbE MMIO space
  @param [in]  Data     Value to write in lower 16bits, upper 16bit may contain opcode.

  @return EFI_STATUS
**/
EFI_STATUS
GbeMdiWrite (
  IN      UINT32  GbeBar,
  IN      UINT32  Data
  )
{
  MmioWrite32 (GbeBar + R_PCH_LAN_CSR_MDIC, Data | 0x04000000);
  return GbeMdiWaitReady (GbeBar);
}

/**
  Perform MDI read.

  @param [in]  GbeBar   GbE MMIO space
  @param [in]  Command  MDI command
  @param [out] Data     Value read

  @return EFI_STATUS
**/
EFI_STATUS
GbeMdiRead (
  IN      UINT32  GbeBar,
  IN      UINT32  Command,
  OUT     UINT16  *Data
  )
{
  EFI_STATUS  Status;
  MmioWrite32 (GbeBar + R_PCH_LAN_CSR_MDIC, Command | 0x08000000);
  Status = GbeMdiWaitReady (GbeBar);
  *Data = (UINT16) MmioRead32 (GbeBar + R_PCH_LAN_CSR_MDIC);
  return Status;
}

/**
  Configure WOL during Sx entry.

  @param [in]  GbeBar   GbE MMIO space
**/
VOID
GbeWolWorkaround (
  IN      UINT32  GbeBar
  )
{
  UINT32      RAL0;
  UINT32      RAH0;
  EFI_STATUS  Status;
  UINT16      Data16;

  //
  // System BIOS performs read from MBARA offset 5400h [31:0] and MBARA offset 5404h [31:0]
  //
  RAL0 = MmioRead32 (GbeBar + R_PCH_LAN_CSR_RAL);
  RAH0 = MmioRead32 (GbeBar + R_PCH_LAN_CSR_RAH);

  //
  // Set MBARA offset 20h = 0x043f6400
  //
  Status = GbeMdiWrite (GbeBar, 0x043f6400);

  //
  // If timeout is reached, force MAC to SMB and try again
  // Assert if failed on second attempt
  //
  if (Status == EFI_TIMEOUT) {
    //
    // Change Extended Device Control Register BIT 11 to 1 which
    // forces the interface between the MAC and the Phy to be on SMBus.
    // Cleared on the assertion of PCI reset.
    //
    MmioOr32 (GbeBar + R_PCH_LAN_CSR_CTRL_EXT, B_PCH_LAN_CSR_CTRL_EXT_FORCE_SMB);
    Status = GbeMdiWrite (GbeBar, 0x043f6400);
  }
  if (EFI_ERROR (Status)) return;

  //
  // Wait 4 mSec
  //
  MicroSecondDelay (4000);

  //
  // Set MBARA offset 20h = 0x4310010
  //
  Status = GbeMdiWrite (GbeBar, 0x04310010);
  if (EFI_ERROR (Status)) return;

  //
  // Set MBARA offset 20h = 0x4320000 or with
  // the least significant word of MBARA offset 5400
  //
  Status = GbeMdiWrite (GbeBar, (0x04320000 | (RAL0 & 0xFFFF)));
  if (EFI_ERROR (Status)) return;

  //
  // Set MBARA offset 20h = 0x4310011
  //
  Status = GbeMdiWrite (GbeBar, 0x04310011);
  if (EFI_ERROR (Status)) return;

  //
  // Set MBARA offset 20h = 0x4320000 or with
  // the most significant word of MBARA offset 5400
  //
  Status = GbeMdiWrite (GbeBar, (0x04320000 | (RAL0 >> 16)));
  if (EFI_ERROR (Status)) return;

  //
  // Set MBARA offset 20h = 0x4310012
  //
  Status = GbeMdiWrite (GbeBar, 0x04310012);
  if (EFI_ERROR (Status)) return;

  //
  // Set MBARA offset 20h = 0x4320000 or with
  // the least significant word of MBARA offset 5404
  //
  Status = GbeMdiWrite (GbeBar, (0x04320000 | (RAH0 & B_PCH_LAN_CSR_RAH_RAH)));
  if (EFI_ERROR (Status)) return;

  //
  // Set MBARA offset 20h = 0x4310013
  //
  Status = GbeMdiWrite (GbeBar, 0x04310013);
  if (EFI_ERROR (Status)) return;

  //
  // Set MBARA offset 20h = 0x4328000
  //
  Status = GbeMdiWrite (GbeBar, 0x04328000);
  if (EFI_ERROR (Status)) return;

  //
  // Set MBARA offset 20h = 0x4310001
  //
  Status = GbeMdiWrite (GbeBar, 0x04310001);
  if (EFI_ERROR (Status)) return;

  //
  // Set MBARA offset 20h = 0x8320000
  //
  Status = GbeMdiRead (GbeBar, 0x08320000, &Data16);
  if (EFI_ERROR (Status)) return;

  //
  // Set MBARA offset 20h = 0x4320000 OR TEMP[15:0] OR 1
  //
  Status = GbeMdiWrite (GbeBar, 0x04320000 | Data16 | BIT0);
  if (EFI_ERROR (Status)) return;

  //
  // Set MBARA offset 20h = 0x43f6460
  //
  Status = GbeMdiWrite (GbeBar, 0x043f6460);
  if (EFI_ERROR (Status)) return;

  //
  // Wait 4 mSec
  //
  MicroSecondDelay (4000);

  //
  // Set MBARA offset 20h = 0x4310042
  //
  Status = GbeMdiWrite (GbeBar, 0x04310042);
  if (EFI_ERROR (Status)) return;

  //
  // Set MBARA offset 20h = 0x43F6020
  //
  Status = GbeMdiWrite (GbeBar, 0x043F6020);
  if (EFI_ERROR (Status)) return;

  //
  // Wait 4 mSec
  //
  MicroSecondDelay (4000);

  //
  // Set MBARA offset 20h = 0x8310000
  // TEMP[15:0] = MBARA + 20[15:0]
  //
  Status = GbeMdiRead (GbeBar, 0x08310000, &Data16);
  if (EFI_ERROR (Status)) return;

  //
  // Set MBARA + 20h = 4310000h or with the TEMP[15:0] OR 10h
  //
  GbeMdiWrite (GbeBar, 0x04310000 | Data16 | BIT4);
}

/**
  Additional Internal GbE Controller special cases WOL Support.

  System BIOS is required perform additional steps upon S0 to S3,4,5 transition
  when ME is off and GbE device in D0. This is needed to enable LAN wake
  in particular when platform is shut-down from EFI.
**/
VOID
GbeSxWorkaround (
  VOID
  )
{
  UINTN       LanRegBase;
  UINT32      GbeBar;
  EFI_STATUS  Status;

  LanRegBase = MmPciBase (
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_LAN,
                 PCI_FUNCTION_NUMBER_PCH_LAN
                 );

  if (MmioRead16 (LanRegBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return;
  }

  //
  // Check if GbE device is in D0
  //
  if ((MmioRead16 (LanRegBase + R_PCH_LAN_PMCS) & B_PCH_LAN_PMCS_PS) != V_PCH_LAN_PMCS_PS0) {
    return;
  }

  ASSERT (mResvMmioSize >= (1 << N_PCH_LAN_MBARA_ALIGN));
  GbeBar = (UINT32) mResvMmioBaseAddr;
  if (GbeBar == 0) {
    ASSERT (FALSE);
    return;
  }

  //
  // Enable MMIO decode using reserved range.
  //
  MmioAnd16 (LanRegBase + PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);
  MmioWrite32 (LanRegBase + R_PCH_LAN_MBARA, GbeBar);
  MmioOr16 (LanRegBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // If MBARA offset 5800h [0] = 1b then proceed with the w/a
  //
  if (MmioRead32 (GbeBar + R_PCH_LAN_CSR_WUC) & B_PCH_LAN_CSR_WUC_APME) {
    Status = GbeAcquireMdio (GbeBar);
    ASSERT_EFI_ERROR (Status);
    if (!EFI_ERROR (Status)) {
      GbeWolWorkaround (GbeBar);
      GbeReleaseMdio (GbeBar);
    }
  }

  //
  // Disable MMIO decode.
  //
  MmioAnd16 (LanRegBase + PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);
  MmioWrite32 (LanRegBase + R_PCH_LAN_MBARA, 0);
}

/**
  Enable platform wake from LAN when in DeepSx if platform supports it.
  Called upon Sx entry.
**/
VOID
GbeConfigureDeepSxWake (
  VOID
  )
{
  UINT32      PchPwrmBase;

  PchPwrmBaseGet (&PchPwrmBase);

  if ((MmioRead32 (PchPwrmBase + R_PCH_PWRM_DSX_CFG) & (UINT32) B_PCH_PWRM_DSX_CFG_LAN_WAKE_EN) != 0) {
    IoOr32 ((UINTN) (mAcpiBaseAddr + R_PCH_ACPI_GPE0_EN_127_96), (UINT32) B_PCH_ACPI_GPE0_EN_127_96_LAN_WAKE);
  }
}

/**
  GbE Sx entry handler
**/
VOID
PchLanSxCallback (
  VOID
  )
{
  if (IsGbeEnabled ()) {
    GbeSxWorkaround ();
    GbeConfigureDeepSxWake ();

  }
}
