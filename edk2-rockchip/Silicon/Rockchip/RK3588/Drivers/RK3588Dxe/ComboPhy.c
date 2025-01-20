/** @file
 *
 *  Copyright (c) 2021, Rockchip Limited. All rights reserved.
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/CruLib.h>
#include <VarStoreData.h>

#include "RK3588DxeFormSetGuid.h"
#include "ComboPhy.h"

#define COMBO_PIPE_PHY0  0xFEE00000
#define COMBO_PIPE_PHY1  0xFEE10000
#define COMBO_PIPE_PHY2  0xFEE20000

#define PHP_GRF_BASE         0xFD5B0000
#define PHP_GRF_PCIESEL_CON  0x100

#define PIPE_PHY0_GRF  0xFD5BC000
#define PIPE_PHY1_GRF  0xFD5C0000
#define PIPE_PHY2_GRF  0xFD5C4000

static UINTN  ComPhyReg[3][2] = {
  { COMBO_PIPE_PHY0, PIPE_PHY0_GRF },
  { COMBO_PIPE_PHY1, PIPE_PHY1_GRF },
  { COMBO_PIPE_PHY2, PIPE_PHY2_GRF },
};

STATIC
VOID
EFIAPI
InitComPhyConfig (
  UINTN   PhyBaseAddr,
  UINTN   PhpBaseAddr,
  UINT32  PhyMode
  )
{
  UINT32  Val;

  DEBUG ((DEBUG_INIT, "%a reg=%x %x mode = %d\n", __func__, PhyBaseAddr, PhpBaseAddr, PhyMode));

  switch (PhyMode) {
    case COMBO_PHY_MODE_PCIE:
      MmioWrite32 (PhpBaseAddr + 0x0, 0xFFFF1000);
      MmioWrite32 (PhpBaseAddr + 0x4, 0xFFFF4000);
      MmioWrite32 (PhpBaseAddr + 0x8, 0xFFFF0101);
      MmioWrite32 (PhpBaseAddr + 0xc, 0xFFFF0200);

      /* gate_tx_pck_sel length select work for L1SS */
      MmioWrite32 (PhyBaseAddr + 0x74, 0xc0);

      /* PLL KVCO tuning fine */
      Val  = MmioRead32 (PhyBaseAddr + (0x20 << 2));
      Val &= ~(0x7 << 2);
      Val |= 0x4 << 2;
      MmioWrite32 (PhyBaseAddr + (0x20 << 2), Val);

      /* Set up rx_trim: PLL LPF C1 85pf R1 1.25kohm */
      MmioWrite32 (PhyBaseAddr + (0x1b << 2), 0x4c);

      /* Set up su_trim: T3 */
      MmioWrite32 (PhyBaseAddr + (0xa << 2), 0xb0);
      MmioWrite32 (PhyBaseAddr + (0xb << 2), 0x47);
      MmioWrite32 (PhyBaseAddr + (0xd << 2), 0x57);

      if (PhyBaseAddr == COMBO_PIPE_PHY0) {
        MmioWrite32 (PHP_GRF_BASE + PHP_GRF_PCIESEL_CON, BIT1 << 16);
      }

      if (PhyBaseAddr == COMBO_PIPE_PHY1) {
        MmioWrite32 (PHP_GRF_BASE + PHP_GRF_PCIESEL_CON, BIT0 << 16);
      }

      break;

    case COMBO_PHY_MODE_SATA:
      MmioWrite32 (PhyBaseAddr + 0x38, 0x41);
      MmioWrite32 (PhyBaseAddr + 0x18, 0x8F);
      MmioWrite32 (PhyBaseAddr + 0x7C, 0x50);
      MmioWrite32 (PhyBaseAddr + 0x24, 0x07);

      MmioWrite32 (PhpBaseAddr + 0x0, 0xFFFF0129);
      MmioWrite32 (PhpBaseAddr + 0x4, 0xFFFF4000);
      MmioWrite32 (PhpBaseAddr + 0x8, 0xFFFF80c1);
      MmioWrite32 (PhpBaseAddr + 0xc, 0xFFFF4007);

      /* Should we tune the rest of the parameters too? */

      break;

    case COMBO_PHY_MODE_USB3:
      /* Set SSC downward spread spectrum */
      Val  = MmioRead32 (PhyBaseAddr + (0x1f << 2));
      Val &= ~(0x3 << 4);
      Val |= 0x01 << 4;
      MmioWrite32 (PhyBaseAddr + 0x7c, Val);

      /* Enable adaptive CTLE for USB3.0 Rx */
      Val  = MmioRead32 (PhyBaseAddr + (0x0e << 2));
      Val &= ~(0x1 << 0);
      Val |= 0x01;
      MmioWrite32 (PhyBaseAddr + (0x0e << 2), Val);

      /* Set PLL KVCO fine tuning signals */
      Val  = MmioRead32 (PhyBaseAddr + (0x20 << 2));
      Val &= ~(0x7 << 2);
      Val |= 0x2 << 2;
      MmioWrite32 (PhyBaseAddr + (0x20 << 2), Val);

      /* Set PLL LPF R1 to su_trim[10:7]=1001 */
      MmioWrite32 (PhyBaseAddr + (0xb << 2), 0x4);

      /* Set PLL input clock divider 1/2 */
      Val  = MmioRead32 (PhyBaseAddr + (0x5 << 2));
      Val &= ~(0x3 << 6);
      Val |= 0x1 << 6;
      MmioWrite32 (PhyBaseAddr + (0x5 << 2), Val);

      /* Set PLL loop divider */
      MmioWrite32 (PhyBaseAddr + (0x11 << 2), 0x32);

      /* Set PLL KVCO to min and set PLL charge pump current to max */
      MmioWrite32 (PhyBaseAddr + (0xa << 2), 0xf0);

      /* Set Rx squelch input filler bandwidth */
      MmioWrite32 (PhyBaseAddr + (0x14 << 2), 0x0d);

      /* Set txcomp_sel[15] to 1b'0 */
      MmioWrite32 (PhpBaseAddr + 0x8, 0x80000000);
      /* Set txelec_sel[12] to 1b'0 */
      MmioWrite32 (PhpBaseAddr + 0x8, 0x10000000);
      /* Set usb_mode_set[5:0]=6b'000100 */
      MmioWrite32 (PhpBaseAddr + 0x0, 0x003F0004);

      /* phy_clk_sel to 100MHz */
      MmioWrite32 (PhpBaseAddr + 0x4, 0x60004000);
      break;

    default:
      break;
  }
}

VOID
EFIAPI
ApplyComboPhyVariables (
  VOID
  )
{
  UINTN  Index;

  UINT32  ComPhyMode[] = {
    PcdGet32 (PcdComboPhy0Mode),
    PcdGet32 (PcdComboPhy1Mode),
    PcdGet32 (PcdComboPhy2Mode)
  };

  /* config phy clock to 100Mhz */
  HAL_CRU_ClkSetFreq (PLL_PPLL, 100 * 1000000);
  HAL_CRU_ClkSetFreq (CLK_REF_PIPE_PHY0, 100 * 1000000);
  HAL_CRU_ClkSetFreq (CLK_REF_PIPE_PHY1, 100 * 1000000);
  HAL_CRU_ClkSetFreq (CLK_REF_PIPE_PHY2, 100 * 1000000);

  /* Initialize echo combo phy */
  for (Index = 0; Index < ARRAY_SIZE (ComPhyMode); Index++) {
    InitComPhyConfig (ComPhyReg[Index][0], ComPhyReg[Index][1], ComPhyMode[Index]);
  }

  MmioWrite32 (PHP_GRF_BASE + 0x0, 0x07E00440);
  MmioWrite32 (PHP_GRF_BASE + 0x4, 0x00070002);

  /* reset deassert */
  MmioWrite32 (0xfd7c0000 + 0x0b34, 0x01c00000);
}

VOID
EFIAPI
SetupComboPhyVariables (
  VOID
  )
{
  UINTN       Size;
  UINT32      Var32;
  EFI_STATUS  Status;

  Size = sizeof (UINT32);

  Status = gRT->GetVariable (
                  L"ComboPhy0Mode",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status) || !FixedPcdGetBool (PcdComboPhy0Switchable)) {
    Status = PcdSet32S (PcdComboPhy0Mode, FixedPcdGet32 (PcdComboPhy0ModeDefault));
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (UINT32);
  Status = gRT->GetVariable (
                  L"ComboPhy1Mode",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status) || !FixedPcdGetBool (PcdComboPhy1Switchable)) {
    Status = PcdSet32S (PcdComboPhy1Mode, FixedPcdGet32 (PcdComboPhy1ModeDefault));
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (UINT32);
  Status = gRT->GetVariable (
                  L"ComboPhy2Mode",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status) || !FixedPcdGetBool (PcdComboPhy2Switchable)) {
    Status = PcdSet32S (PcdComboPhy2Mode, FixedPcdGet32 (PcdComboPhy2ModeDefault));
    ASSERT_EFI_ERROR (Status);
  }
}
