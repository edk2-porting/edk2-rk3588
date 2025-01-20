/** @file
 *
 *  RK3588 PCIe PHY Library.
 *
 *  Copyright (c) 2023, David Gwynne <david@gwynne.id.au>
 *  Copyright (c) 2023, Jared McNeill <jmcneill@invisible.ca>
 *  Copyright (c) 2023, Molly Sophia <mollysophia379@gmail.com>
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

#define PCIE30_PHY_GRF  0xfd5b8000
/* PCIEPHY_GRF */
#define GRF_PCIE30_PHY_CON(n)       (PCIE30_PHY_GRF + 0x0000 + (n) * 0x4)         /* 0 .. 9 */
#define GRF_PCIE30_PHY_STATUS(n)    (PCIE30_PHY_GRF + 0x0080 + (n) * 0x4)         /* 0 .. 2 */
#define GRF_PCIE30_PHY_PRT0_CON(n)  (PCIE30_PHY_GRF + 0x0100 + (n) * 0x4)         /* 0 .. 39 */
/* CON1 and CON9 */
#define GRF_PCIE30PHY_DA_OCM_MASK  BIT15
#define GRF_PCIE30PHY_DA_OCM       BIT15
/* CON5 */
#define GRF_PCIE30PHY_LANE0_LINK_NUM_SHIFT  0
#define GRF_PCIE30PHY_LANE0_LINK_NUM_MASK   (0xfU << GRF_PCIE30PHY_LANE0_LINK_NUM_SHIFT)
/* CON6 */
#define GRF_PCIE30PHY_LANE1_LINK_NUM_SHIFT  0
#define GRF_PCIE30PHY_LANE1_LINK_NUM_MASK   (0xfU << GRF_PCIE30PHY_LANE1_LINK_NUM_SHIFT)

/* STATUS0 */
#define GRF_PCIE30PHY_SRAM_INIT_DONE  BIT14

#define SOFTRST_INDEX  27
#define SOFTRST_BIT    14

STATIC
VOID
GrfUpdateRegister (
  IN EFI_PHYSICAL_ADDRESS  Reg,
  IN UINT32                Mask,
  IN UINT32                Val
  )
{
  ASSERT ((Mask & ~0xFFFF) == 0);
  ASSERT ((Val & ~0xFFFF) == 0);
  ASSERT ((Mask & Val) == Val);

  MmioWrite32 (Reg, (Mask << 16) | Val);
}

EFI_STATUS
Pcie30PhyInit (
  VOID
  )
{
  // UINTN Retry;

  DEBUG ((DEBUG_INFO, "PCIe30: PHY init\n"));
  DEBUG ((DEBUG_INFO, "PCIe30: PHY mode %d\n", PcdGet8 (PcdPcie30PhyMode)));

  // MicroSecondDelay(100000);

  /* Disable power domain */
  MmioWrite32 (0xFD8D8150, 0x1 << 23 | 0x1 << 21);  // PD_PCIE & PD_PHP

  /* Phy mode: from pcd Pcie30PhyMode */
  MmioWrite32 (GRF_PCIE30_PHY_CON (0), (0x7 << 16) | PcdGet8 (PcdPcie30PhyMode));

  MmioWrite32 (0xFD7C8A00, (0x1 << 10) | (0x1 << 26));

  /* Deassert PCIe PMA output clamp mode */
  MmioWrite32 (GRF_PCIE30_PHY_CON (0), (0x1 << 8) | (0x1 << 24));

  /* Deassert PHY Reset */
  MmioWrite32 (0xFD7C8A00, (0x1 << 26));

  // /* Enable clocks */
  // PmuCruEnableClock (2, 13);
  // PmuCruEnableClock (2, 14);
  // CruEnableClock (33, 8);

  // /* Assert reset */
  // CruAssertSoftReset (SOFTRST_INDEX, SOFTRST_BIT);
  // gBS->Stall (1000);

  // MicroSecondDelay (1);

  // GrfUpdateRegister (GRF_PCIE30_PHY_CON (9), GRF_PCIE30PHY_DA_OCM_MASK, GRF_PCIE30PHY_DA_OCM);
  // GrfUpdateRegister (GRF_PCIE30_PHY_CON (5), GRF_PCIE30PHY_LANE0_LINK_NUM_MASK, PCIE30PHY_LANE0_LINK_NUM);
  // GrfUpdateRegister (GRF_PCIE30_PHY_CON (6), GRF_PCIE30PHY_LANE1_LINK_NUM_MASK, PCIE30PHY_LANE1_LINK_NUM);
  // GrfUpdateRegister (GRF_PCIE30_PHY_CON (1), GRF_PCIE30PHY_DA_OCM_MASK, GRF_PCIE30PHY_DA_OCM);

  // /* De-assert reset */
  // CruDeassertSoftReset (SOFTRST_INDEX, SOFTRST_BIT);

  // for (Retry = 500; Retry > 0; Retry--) {
  //     MicroSecondDelay (100);

  //     if ((MmioRead32 (GRF_PCIE30_PHY_STATUS (0)) & GRF_PCIE30PHY_SRAM_INIT_DONE) != 0) {
  //         break;
  //     }
  // }
  // if (Retry == 0) {
  //     DEBUG ((DEBUG_WARN, "PCIe30: Failed to enable PCIe 3.0 PHY\n"));
  //     return EFI_TIMEOUT;
  // }

  DEBUG ((DEBUG_INFO, "PCIe30: PHY init complete\n"));
  return EFI_SUCCESS;
}
