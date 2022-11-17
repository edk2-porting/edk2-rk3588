/** @file
  Gbe Sx handler implementation.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/TimerLib.h>
#include "PchInitSmm.h"
#include <Library/PmcPrivateLib.h>
#include <Library/GbeMdiLib.h>
#include <Library/GbeLib.h>
#include <Register/PchRegs.h>
#include <Register/GbeRegs.h>
#include <Library/PchPciBdfLib.h>


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
  UINT16      WUC;
  EFI_STATUS  Status;
  UINT16      Data16;

  //
  // 1. Set page to 769 Port Control Registers
  // 2. Wait 4 mSec
  //
  Status = GbeMdiSetPage (GbeBar, PHY_MDI_PAGE_769_PORT_CONTROL_REGISTERS);
  if (EFI_ERROR (Status)) return;

  //
  // 3. Set registry to 17 Port General Configuration
  // 4. Copy all settings from Port General Configuration
  //
  Status = GbeMdiRead (GbeBar, B_PHY_MDI_PHY_ADDRESS_01, MDI_REG_SHIFT (R_PHY_MDI_PAGE_769_REGISETER_17_PGC), &Data16);
  if (EFI_ERROR (Status)) return;

  //
  // 5. Modify BIT 4 and BIT 2 to disable host wake up and set MACPD
  //
  Status = GbeMdiWrite (GbeBar, B_PHY_MDI_PHY_ADDRESS_01, MDI_REG_SHIFT (R_PHY_MDI_PAGE_769_REGISETER_17_PGC), (Data16 | B_PHY_MDI_PAGE_769_REGISETER_17_PGC_MACPD_ENABLE) & (~B_PHY_MDI_PAGE_769_REGISETER_17_PGC_HOST_WAKE_UP));
  if (EFI_ERROR (Status)) return;

  //
  // 6. Read Receive Address Low and Receive Address High from MMIO
  //
  RAL0 = MmioRead32 (GbeBar + R_GBE_MEM_CSR_RAL);
  RAH0 = MmioRead32 (GbeBar + R_GBE_MEM_CSR_RAH);

  //
  // 7. Set page to 800 Wake Up Registers
  // 8. Wait 4 mSec
  //
  Status = GbeMdiSetPage (GbeBar, PHY_MDI_PAGE_800_WAKE_UP_REGISTERS);
  if (EFI_ERROR (Status)) return;

  //
  // 9. Set registry to 16 Receive Address Low 1/2
  //
  Status = GbeMdiSetRegister (GbeBar, R_PHY_MDI_PAGE_800_REGISETER_16_RAL0);
  if (EFI_ERROR (Status)) return;

  //
  // 10. Program first 16 bits [0:15] out of 48 in Receive Address Low 1/2
  //
  Status = GbeMdiWrite (GbeBar, B_PHY_MDI_PHY_ADDRESS_01, R_PHY_MDI_PHY_REG_DATA_READ_WRITE, (RAL0 & 0xFFFF));
  if (EFI_ERROR (Status)) return;

  //
  // 11. Set registry to 17 Receive Address Low 2/2
  //
  Status = GbeMdiSetRegister (GbeBar, R_PHY_MDI_PAGE_800_REGISETER_17_RAL1);
  if (EFI_ERROR (Status)) return;

  //
  // 12. Program second 16 bits [16:31] out of 48 in Receive Address Low 2/2
  //
  Status = GbeMdiWrite (GbeBar, B_PHY_MDI_PHY_ADDRESS_01, R_PHY_MDI_PHY_REG_DATA_READ_WRITE, (RAL0 >> 16));
  if (EFI_ERROR (Status)) return;

  //
  // 13. Set registry to 18 Receive Address High 1/2
  //
  Status = GbeMdiSetRegister (GbeBar, R_PHY_MDI_PAGE_800_REGISETER_18_RAH0);
  if (EFI_ERROR (Status)) return;

  //
  // 14. Program last 16 bits [32:47] out of 48
  //
  Status = GbeMdiWrite (GbeBar, B_PHY_MDI_PHY_ADDRESS_01, R_PHY_MDI_PHY_REG_DATA_READ_WRITE, (RAH0 & B_GBE_MEM_CSR_RAH_RAH));
  if (EFI_ERROR (Status)) return;

  //
  // 15. Set registry to 19 Receive Address High 2/2
  //
  Status = GbeMdiSetRegister (GbeBar, R_PHY_MDI_PAGE_800_REGISETER_19_RAH1);
  if (EFI_ERROR (Status)) return;

  //
  // 16. Set Address Valid
  //
  Status = GbeMdiWrite (GbeBar, B_PHY_MDI_PHY_ADDRESS_01, R_PHY_MDI_PHY_REG_DATA_READ_WRITE, B_PHY_MDI_PAGE_800_REGISETER_19_RAH1_ADDRESS_VALID);
  if (EFI_ERROR (Status)) return;

  //
  // 17. Set Wake Up Control Register 1
  //
  Status = GbeMdiSetRegister (GbeBar, R_PHY_MDI_PAGE_800_REGISETER_1_WUC);
  if (EFI_ERROR (Status)) return;

  //
  // 18. Copy WakeUp Control from MAC MMIO
  //
  WUC = (UINT16) MmioRead32 (GbeBar + R_GBE_MEM_CSR_WUC);

  //
  // 19. Store WakeUp Contorl into LCD
  // Modify APME bit to enable APM wake up
  //
  Status = GbeMdiWrite (GbeBar, B_PHY_MDI_PHY_ADDRESS_01, R_PHY_MDI_PHY_REG_DATA_READ_WRITE, (WUC & 0xFFFF));
  if (EFI_ERROR (Status)) return;

  //
  // 20. Set page to 803 Host Wol Packet
  // 21. Wait 4 mSec
  //
  Status = GbeMdiSetPage (GbeBar, PHY_MDI_PAGE_803_HOST_WOL_PACKET);
  if (EFI_ERROR (Status)) return;

  //
  // 22. Set registry to 66 Host WoL Packet Clear
  //
  Status = GbeMdiSetRegister (GbeBar, R_PHY_MDI_PAGE_803_REGISETER_66_HWPC);
  if (EFI_ERROR (Status)) return;

  //
  // 23. Clear WOL Packet
  //
  Status = GbeMdiWrite (GbeBar, B_PHY_MDI_PHY_ADDRESS_01, R_PHY_MDI_PHY_REG_DATA_READ_WRITE, 0);
  if (EFI_ERROR (Status)) return;
  //
  // 24. Set page to 769 Port Control Registers
  // 25. Wait 4 mSec
  //
  Status = GbeMdiSetPage (GbeBar, PHY_MDI_PAGE_769_PORT_CONTROL_REGISTERS);
  if (EFI_ERROR (Status)) return;

  //
  // 26. Set registry to 17 Port General Configuration
  //
  Status = GbeMdiSetRegister (GbeBar, R_PHY_MDI_PAGE_769_REGISETER_17_PGC);
  if (EFI_ERROR (Status)) return;

  //
  // 27. Copy all settings from Port General Configuration
  //
  Status = GbeMdiRead (GbeBar, B_PHY_MDI_PHY_ADDRESS_01, MDI_REG_SHIFT (R_PHY_MDI_PAGE_769_REGISETER_17_PGC), &Data16);
  if (EFI_ERROR (Status)) return;

  //
  // 28. Modify BIT 4 and BIT 2 to enable host wake up and clear MACPD
  //
  Status = GbeMdiWrite (GbeBar, B_PHY_MDI_PHY_ADDRESS_01, MDI_REG_SHIFT (R_PHY_MDI_PAGE_769_REGISETER_17_PGC), (Data16 | B_PHY_MDI_PAGE_769_REGISETER_17_PGC_HOST_WAKE_UP) & (~B_PHY_MDI_PAGE_769_REGISETER_17_PGC_MACPD_ENABLE));
  if (EFI_ERROR (Status)) return;
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
  UINT64      LanRegBase;
  UINT32      GbeBar;
  EFI_STATUS  Status;

  LanRegBase = GbePciCfgBase ();

  if (PciSegmentRead16 (LanRegBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return;
  }

  //
  // Check if GbE device is in D0
  //
  if ((PciSegmentRead16 (LanRegBase + R_GBE_CFG_PMCS) & B_GBE_CFG_PMCS_PS) != V_GBE_CFG_PMCS_PS0) {
    return;
  }

  ASSERT (mResvMmioSize >= (1 << N_GBE_CFG_MBARA_ALIGN));
  GbeBar = (UINT32) mResvMmioBaseAddr;
  if (GbeBar == 0) {
    ASSERT (FALSE);
    return;
  }

  //
  // Enable MMIO decode using reserved range.
  //
  PciSegmentAnd16 (LanRegBase + PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);
  PciSegmentWrite32 (LanRegBase + R_GBE_CFG_MBARA, GbeBar);
  PciSegmentOr16 (LanRegBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // If MBARA offset 5800h [0] = 1b then proceed with the w/a
  //
  if (MmioRead32 (GbeBar + R_GBE_MEM_CSR_WUC) & B_GBE_MEM_CSR_WUC_APME) {
    Status = GbeMdiAcquireMdio (GbeBar);
    ASSERT_EFI_ERROR (Status);
    if (!EFI_ERROR (Status)) {
      GbeWolWorkaround (GbeBar);
      GbeMdiReleaseMdio (GbeBar);
    }
  }

  //
  // Disable MMIO decode.
  //
  PciSegmentAnd16 (LanRegBase + PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);
  PciSegmentWrite32 (LanRegBase + R_GBE_CFG_MBARA, 0);
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
  if (PmcIsLanDeepSxWakeEnabled ()) {
    IoOr32 ((UINTN) (mAcpiBaseAddr + R_ACPI_IO_GPE0_EN_127_96), (UINT32) B_ACPI_IO_GPE0_EN_127_96_LAN_WAKE);
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
