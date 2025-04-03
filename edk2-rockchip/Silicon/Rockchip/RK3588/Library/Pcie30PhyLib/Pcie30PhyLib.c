/** @file
 *
 *  RK3588 PCIe PHY Library.
 *
 *  Copyright (c) 2023, David Gwynne <david@gwynne.id.au>
 *  Copyright (c) 2023, Jared McNeill <jmcneill@invisible.ca>
 *  Copyright (c) 2023, Molly Sophia <mollysophia379@gmail.com>
 *  Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/CruLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PcdLib.h>
#include <VarStoreData.h>

#define PHP_GRF_BASE       0xfd5b0000
#define PCIE3PHY_GRF_BASE  0xfd5b8000

/* PHP_GRF */
#define PHP_GRF_PCIESEL_CON  0x100

/* PCIE3PHY_GRF */
#define PCIE3PHY_GRF_CMN_CON0      0x0
#define PCIE3PHY_GRF_PHY0_STATUS1  0x904
#define PCIE3PHY_GRF_PHY1_STATUS1  0xa04
#define PCIE3PHY_SRAM_INIT_DONE(reg)  ((reg & BIT0) != 0)

STATIC EFI_STATUS  mInitStatus = EFI_NOT_READY;

EFI_STATUS
Pcie30PhyInit (
  VOID
  )
{
  UINT8   Mode;
  UINT32  Reg;
  UINTN   Retry;

  if (mInitStatus != EFI_NOT_READY) {
    return mInitStatus;
  }

  Mode = PcdGet8 (PcdPcie30PhyMode);

  DEBUG ((DEBUG_INFO, "PCIe30: PHY init\n"));
  DEBUG ((DEBUG_INFO, "PCIe30: PHY mode %d\n", Mode));

  /* Enable power domain */
  MmioWrite32 (0xFD8D8150, 0x1 << 23 | 0x1 << 21);  // PD_PCIE & PD_PHP

  /* Phy mode */
  Reg = Mode;
  MmioWrite32 (PCIE3PHY_GRF_BASE + PCIE3PHY_GRF_CMN_CON0, (0x7 << 16) | Reg);

  /* Set pcie1ln_sel in PHP_GRF_PCIESEL_CON */
  Reg &= 0x3;
  if (Reg) {
    MmioWrite32 (PHP_GRF_BASE + PHP_GRF_PCIESEL_CON, (0x3 << 16) | Reg);
  }

  /* Assert PHY Reset */
  MmioWrite32 (0xFD7C8A00, (0x1 << 10) | (0x1 << 26));
  MicroSecondDelay (1);

  /* Deassert PCIe PMA output clamp mode */
  MmioWrite32 (PCIE3PHY_GRF_BASE + PCIE3PHY_GRF_CMN_CON0, (0x1 << 8) | (0x1 << 24));

  /* Deassert PHY Reset */
  MmioWrite32 (0xFD7C8A00, (0x1 << 26));

  for (Retry = 500; Retry > 0; Retry--) {
    Reg = MmioRead32 (PCIE3PHY_GRF_BASE + PCIE3PHY_GRF_PHY0_STATUS1);
    if (Mode == PCIE30_PHY_MODE_AGGREGATION) {
      Reg &= MmioRead32 (PCIE3PHY_GRF_BASE + PCIE3PHY_GRF_PHY1_STATUS1);
    }

    if (PCIE3PHY_SRAM_INIT_DONE (Reg)) {
      break;
    }

    MicroSecondDelay (100);
  }

  if (Retry == 0) {
    DEBUG ((DEBUG_WARN, "PCIe30: PHY init failed\n"));
    mInitStatus = EFI_TIMEOUT;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "PCIe30: PHY init complete\n"));
  mInitStatus = EFI_SUCCESS;

Exit:
  return mInitStatus;
}
