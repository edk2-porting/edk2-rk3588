/** @file

  Copyright 2017, 2020 NXP
  Copyright 2021-2023, Jared McNeill <jmcneill@invisible.ca>
  Copyright 2023, Molly Sophia <mollysophia379@gmail.com>
  Copyright (c) 2023-2025, Mario Bălănică <mariobalanica02@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/GpioLib.h>
#include <Library/CruLib.h>
#include <Library/GpioLib.h>
#include <Library/Rk3588Pcie.h>
#include <Library/RockchipPlatformLib.h>
#include <Library/Pcie30PhyLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <IndustryStandard/Pci.h>
#include <VarStoreData.h>

#include "PciHostBridgeInit.h"

/* APB Registers */
#define PCIE_CLIENT_GENERAL_CON         0x0000
#define  DEVICE_TYPE_SHIFT              4
#define  DEVICE_TYPE_MASK               (0xFU << DEVICE_TYPE_SHIFT)
#define  DEVICE_TYPE_RC                 (4 << DEVICE_TYPE_SHIFT)
#define  LINK_REQ_RST_GRT               BIT3
#define  LTSSM_ENABLE                   BIT2
#define PCIE_CLIENT_INTR_MASK_LEGACY    0x001C
#define PCIE_CLIENT_POWER_CON           0x002C
#define  APP_CLK_REQ_N                  BIT0
#define  CLK_REQ_N_BYPASS               BIT12
#define  CLK_REQ_N_CON                  BIT13
#define PCIE_CLIENT_GENERAL_DEBUG_INIT  0x0104
#define PCIE_CLIENT_HOT_RESET_CTRL      0x0180
#define  APP_LSSTM_ENABLE_ENHANCE       BIT4
#define PCIE_CLIENT_LTSSM_STATUS        0x0300
#define  RDLH_LINK_UP                   BIT17
#define  SMLH_LINK_UP                   BIT16
#define  SMLH_LTSSM_STATE_MASK          0x3f
#define  SMLH_LTSSM_STATE_LINK_UP       0x11

/* DBI Registers */
#define PCI_COMMAND               0x0004
#define PCI_DEVICE_CLASS          0x000A
#define PCI_BAR0                  0x0010
#define PCI_BAR1                  0x0014
#define PCIE_LINK_CAPABILITY      0x007C
#define PCIE_LINK_STATUS          0x0080
#define  LINK_STATUS_WIDTH_SHIFT  20
#define  LINK_STATUS_WIDTH_MASK   (0xFU << LINK_STATUS_WIDTH_SHIFT)
#define  LINK_STATUS_SPEED_SHIFT  16
#define  LINK_STATUS_SPEED_MASK   (0xFU << LINK_STATUS_SPEED_SHIFT)
#define PCIE_LINK_CTL_2           0x00A0
#define PL_PORT_LINK_CTRL_OFF     0x0710
#define  LINK_CAPABLE_SHIFT       16
#define  LINK_CAPABLE_MASK        (0x3FU << LINK_CAPABLE_SHIFT)
#define  FAST_LINK_MODE           BIT7
#define  DLL_LINK_EN              BIT5
#define PL_GEN2_CTRL_OFF          0x080C
#define  DIRECT_SPEED_CHANGE      BIT17
#define  NUM_OF_LANES_SHIFT       8
#define  NUM_OF_LANES_MASK        (0x1FU << NUM_OF_LANES_SHIFT)
#define PL_MISC_CONTROL_1_OFF     0x08BC
#define  DBI_RO_WR_EN             BIT0

#define PCIE_TYPE0_HDR_DBI2_OFFSET  0x100000

/* ATU Registers */
#define ATU_CAP_BASE  0x300000
#define IATU_REGION_CTRL_OUTBOUND(n)  (ATU_CAP_BASE + ((n) << 9))
#define IATU_REGION_CTRL_INBOUND(n)   (ATU_CAP_BASE + ((n) << 9) + 0x100)
#define IATU_REGION_CTRL_1_OFF      0x000
#define  IATU_TYPE_MEM              0
#define  IATU_TYPE_IO               2
#define  IATU_TYPE_CFG0             4
#define  IATU_TYPE_CFG1             5
#define IATU_REGION_CTRL_2_OFF      0x004
#define  IATU_ENABLE                BIT31
#define  IATU_CFG_SHIFT_MODE        BIT28
#define IATU_LWR_BASE_ADDR_OFF      0x008
#define IATU_UPPER_BASE_ADDR_OFF    0x00C
#define IATU_LIMIT_ADDR_OFF         0x010
#define IATU_LWR_TARGET_ADDR_OFF    0x014
#define IATU_UPPER_TARGET_ADDR_OFF  0x018

BOOLEAN
IsPcieNumEnabled (
  UINTN  PcieNum
  )
{
  BOOLEAN  Enabled = FALSE;

  /*
   * According to TRM Part 1 6.19.2 PCIe3PHY_GRF_CMN_CON0,
   * if pcie l4(PCIE30X4) bifurcation enabled (phymode & 0b01), pcie3phy lane 1 is connected to 1l0
   * if pcie l2(PCIE30X2) bifurcation enabled (phymode & 0b10), pcie3phy lane 3 is connected to 1l1
  */
  switch (PcieNum) {
    case PCIE_SEGMENT_PCIE30X4:
      Enabled = (PcdGet32 (PcdPcie30State) == PCIE30_STATE_ENABLED);
      break;

    case PCIE_SEGMENT_PCIE30X2:
      Enabled = (PcdGet32 (PcdPcie30State) == PCIE30_STATE_ENABLED &&
                 FixedPcdGetBool (PcdPcie30x2Supported) &&
                 PcdGet8 (PcdPcie30PhyMode) != PCIE30_PHY_MODE_AGGREGATION);
      break;

    case PCIE_SEGMENT_PCIE20L0:
      Enabled = (PcdGet32 (PcdComboPhy1Mode) == COMBO_PHY_MODE_PCIE);
      break;

    case PCIE_SEGMENT_PCIE20L1:
      Enabled = (PcdGet32 (PcdComboPhy2Mode) == COMBO_PHY_MODE_PCIE);
      break;

    case PCIE_SEGMENT_PCIE20L2:
      Enabled = (PcdGet32 (PcdComboPhy0Mode) == COMBO_PHY_MODE_PCIE);
      break;

    default:
      break;
  }

  return Enabled;
}

STATIC CONST GPIO_IOMUX_CONFIG  mPcie30x1_0_IomuxConfigs[][3] = {
  {
    { "pcie30x1_0_clkreqnm0", 0, GPIO_PIN_PC0, 12, GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
    { "pcie30x1_0_wakenm0",   0, GPIO_PIN_PC4, 12, GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
    { "pcie30x1_0_perstnm0",  0, GPIO_PIN_PC5, 12, GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
  },
  {
    { "pcie30x1_0_clkreqnm1", 4, GPIO_PIN_PA3, 4,  GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
    { "pcie30x1_0_wakenm1",   4, GPIO_PIN_PA4, 4,  GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
    { "pcie30x1_0_perstnm1",  4, GPIO_PIN_PA5, 4,  GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
  },
  {
    { "pcie30x1_0_clkreqnm2", 1, GPIO_PIN_PB5, 4,  GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
    { "pcie30x1_0_wakenm2",   1, GPIO_PIN_PB3, 4,  GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
    { "pcie30x1_0_perstnm2",  1, GPIO_PIN_PB4, 4,  GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
  },
};

STATIC CONST GPIO_IOMUX_CONFIG  mPcie30x1_1_IomuxConfigs[][3] = {
  {
    { "pcie30x1_1_clkreqnm0", 0, GPIO_PIN_PB5, 12, GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
    { "pcie30x1_1_wakenm0",   0, GPIO_PIN_PB6, 12, GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
    { "pcie30x1_1_perstnm0",  0, GPIO_PIN_PB7, 12, GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
  },
  {
    { "pcie30x1_1_clkreqnm1", 4, GPIO_PIN_PA0, 4,  GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
    { "pcie30x1_1_wakenm1",   4, GPIO_PIN_PA1, 4,  GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
    { "pcie30x1_1_perstnm1",  4, GPIO_PIN_PA2, 4,  GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
  },
  {
    { "pcie30x1_1_clkreqnm2", 1, GPIO_PIN_PA0, 4,  GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
    { "pcie30x1_1_wakenm2",   1, GPIO_PIN_PA1, 4,  GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
    { "pcie30x1_1_perstnm2",  1, GPIO_PIN_PA7, 4,  GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
  },
};

STATIC CONST GPIO_IOMUX_CONFIG  mPcie20x1_2_IomuxConfigs[][3] = {
  {
    { "pcie20x1_2_clkreqnm0", 3, GPIO_PIN_PC7, 4, GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
    { "pcie20x1_2_wakenm0",   3, GPIO_PIN_PD0, 4, GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
    { "pcie20x1_2_perstnm0",  3, GPIO_PIN_PD1, 4, GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
  },
  {
    { "pcie20x1_2_clkreqnm1", 4, GPIO_PIN_PB7, 4, GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
    { "pcie20x1_2_wakenm1",   4, GPIO_PIN_PC0, 4, GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
    { "pcie20x1_2_perstnm1",  4, GPIO_PIN_PC1, 4, GPIO_PIN_PULL_NONE, GPIO_PIN_DRIVE_DEFAULT },
  },
};

STATIC BOOLEAN  PcieSupportClkreq[NUM_PCIE_CONTROLLER] = {
  FALSE, FALSE, FALSE, FALSE, FALSE
};

VOID
PciePinmuxInit (
  UINTN  PcieNum,
  UINTN  MuxNum
  )
{
  ASSERT (PcieNum < NUM_PCIE_CONTROLLER);
  switch (PcieNum) {
    case PCIE_SEGMENT_PCIE20L0:
      ASSERT (MuxNum < 3);
      GpioSetIomuxConfig (&mPcie30x1_0_IomuxConfigs[MuxNum][0], 3);
      break;
    case PCIE_SEGMENT_PCIE20L1:
      ASSERT (MuxNum < 3);
      GpioSetIomuxConfig (&mPcie30x1_1_IomuxConfigs[MuxNum][0], 3);
      break;
    case PCIE_SEGMENT_PCIE20L2:
      ASSERT (MuxNum < 2);
      GpioSetIomuxConfig (&mPcie20x1_2_IomuxConfigs[MuxNum][0], 3);
      break;

    default:
      return;
  }

  PcieSupportClkreq[PcieNum] = TRUE;
}

STATIC
VOID
PciSetupClocks (
  IN UINTN  Segment
  )
{
  MmioWrite32 (0xFD7C8800, (0x1 << 17)|(0x1 << 25)); // pclk_phptop_cru_en
  MmioWrite32 (0xFD7C0880, 0xffff0000);              // CRU_GATE_CON32
  switch (Segment) {
    case PCIE_SEGMENT_PCIE30X4:
      MmioWrite32 (0xFD7C8A00, (0x1 << 24)|(0x1 << 26));             // PHPTOPCRU_SOFTRST_CON00
      MmioWrite32 (0xFD7C8800, (0x1 << 24));                         // PHPTOPCRU_GATE_CON00
      MmioWrite32 (0xFD7C0A80, (0x1 << 29));                         // CRU_SOFTRST_CON32:resetn_pcie_4l_power_up
      MmioWrite32 (0xFD7C089c, (0x1 << 16));                         // CRU_GATE_CON39:clk_pcie_4l_pipe_en
      MmioWrite32 (0xFD7C0888, (0x1 << 17));                         // CRU_GATE_CON34:clk_pcie_4l_aux_en
      MmioWrite32 (0xFD7C0884, (0x1 << 28)|(0x1 << 23)|(0x1 << 18)); // CRU_GATE_CON33:pclk_pcie_4l_en,aclk_pcie_4l_slv_en,aclk_pcie_4l_mstr_en
      break;
    case PCIE_SEGMENT_PCIE30X2:
      MmioWrite32 (0xFD7C0A80, (0x1 << 30));                         // CRU_SOFTRST_CON32:resetn_pcie_2l_power_up
      MmioWrite32 (0xFD7C089c, (0x1 << 17));                         // CRU_GATE_CON39:clk_pcie_2l_pipe_en
      MmioWrite32 (0xFD7C0888, (0x1 << 18));                         // CRU_GATE_CON34:clk_pcie_2l_aux_en
      MmioWrite32 (0xFD7C0884, (0x1 << 29)|(0x1 << 24)|(0x1 << 19)); // CRU_GATE_CON33:pclk_pcie_2l_en,aclk_pcie_2l_slv_en,aclk_pcie_2l_mstr_en
      break;
    case PCIE_SEGMENT_PCIE20L2:                          // phy0
      MmioWrite32 (0xFD7C8A00, (0x1 << 21)|(0x1 << 18)); // PHPTOPCRU_SOFTRST_CON00
      MmioWrite32 (0xFD7C8800, (0x1 << 21)|(0x1 << 18)); // PHPTOPCRU_GATE_CON00
      MmioWrite32 (0xFD7C0A84, (0x1 << 17));             // CRU_SOFTRST_CON33:resetn_pcie_1l2_power_up
      MmioWrite32 (0xFD7C0898, (0x1 << 29));             // CRU_GATE_CON38:clk_pcie_1l2_pipe_en
      MmioWrite32 (0xFD7C0888, (0x1 << 21)|(0x1 << 16)); // CRU_GATE_CON34:clk_pcie_1l2_aux_en,pclk_pcie_1l2_en
      MmioWrite32 (0xFD7C0884, (0x1 << 27)|(0x1 << 22)); // CRU_GATE_CON33:aclk_pcie_1l2_slv_en,aclk_pcie_1l2_mstr_en
      break;
    case PCIE_SEGMENT_PCIE20L0:                                      // phy1
      MmioWrite32 (0xFD7C8A00, (0x1 << 22)|(0x1 << 19));             // PHPTOPCRU_SOFTRST_CON00
      MmioWrite32 (0xFD7C8800, (0x1 << 22)|(0x1 << 19));             // PHPTOPCRU_GATE_CON00
      MmioWrite32 (0xFD7C0A80, (0x1 << 31));                         // CRU_SOFTRST_CON32:resetn_pcie_1l0_power_up
      MmioWrite32 (0xFD7C0898, (0x1 << 30));                         // CRU_GATE_CON38:clk_pcie_1l0_pipe_en
      MmioWrite32 (0xFD7C0888, (0x1 << 19));                         // CRU_GATE_CON34:clk_pcie_1l0_aux_en
      MmioWrite32 (0xFD7C0884, (0x1 << 30)|(0x1 << 25)|(0x1 << 20)); // CRU_GATE_CON33:pclk_pcie_1l0_en,aclk_pcie_1l0_slv_en,aclk_pcie_1l0_mstr_en
      break;
    case PCIE_SEGMENT_PCIE20L1:                                      // phy2
      MmioWrite32 (0xFD7C8A00, (0x1 << 23)|(0x1 << 20));             // PHPTOPCRU_SOFTRST_CON00
      MmioWrite32 (0xFD7C8800, (0x1 << 23)|(0x1 << 20));             // PHPTOPCRU_GATE_CON00
      MmioWrite32 (0xFD7C0A84, (0x1 << 16));                         // CRU_SOFTRST_CON33:resetn_pcie_1l1_power_up
      MmioWrite32 (0xFD7C0898, (0x1 << 31));                         // CRU_GATE_CON38:clk_pcie_1l1_pipe_en
      MmioWrite32 (0xFD7C0888, (0x1 << 20));                         // CRU_GATE_CON34:clk_pcie_1l1_aux_en
      MmioWrite32 (0xFD7C0884, (0x1 << 31)|(0x1 << 26)|(0x1 << 21)); // CRU_GATE_CON33:pclk_pcie_1l1_en,aclk_pcie_1l1_slv_en,aclk_pcie_1l1_mstr_en
      break;
    default:
      break;
  }
}

STATIC
VOID
PciSetRcMode (
  IN UINTN                 Segment,
  IN EFI_PHYSICAL_ADDRESS  ApbBase
  )
{
  MmioWrite32 (ApbBase + PCIE_CLIENT_INTR_MASK_LEGACY, 0xFFFF0000);
  MmioWrite32 (
    ApbBase + PCIE_CLIENT_HOT_RESET_CTRL,
    (APP_LSSTM_ENABLE_ENHANCE << 16) | APP_LSSTM_ENABLE_ENHANCE
    );

  if (PcieSupportClkreq[Segment]) {
    MmioWrite32 (ApbBase + PCIE_CLIENT_POWER_CON, (APP_CLK_REQ_N << 16) | APP_CLK_REQ_N);
  } else {
    MmioWrite32 (ApbBase + PCIE_CLIENT_POWER_CON, APP_CLK_REQ_N << 16);
    MmioWrite32 (ApbBase + PCIE_CLIENT_POWER_CON, ((CLK_REQ_N_BYPASS|CLK_REQ_N_CON) << 16) | CLK_REQ_N_BYPASS);
    // Maybe also disable PCIE L1SS capabilities here?
  }

  MmioWrite32 (
    ApbBase + PCIE_CLIENT_GENERAL_CON,
    (DEVICE_TYPE_MASK << 16) | DEVICE_TYPE_RC
    );
}

STATIC
VOID
PciSetupBars (
  IN EFI_PHYSICAL_ADDRESS  DbiBase
  )
{
  MmioWrite16 (DbiBase + PCI_DEVICE_CLASS, (PCI_CLASS_BRIDGE << 8) | PCI_CLASS_BRIDGE_P2P);

  // Disable BAR0 + BAR1 of root port as they're invalid.
  MmioWrite32 (DbiBase + PCIE_TYPE0_HDR_DBI2_OFFSET + PCI_BAR0, 0x0);
  MmioWrite32 (DbiBase + PCIE_TYPE0_HDR_DBI2_OFFSET + PCI_BAR1, 0x0);
}

STATIC
VOID
PciDirectSpeedChange (
  IN EFI_PHYSICAL_ADDRESS  DbiBase
  )
{
  DEBUG ((DEBUG_INIT, "PCIe: SetupBars: Speed change\n"));
  /* Initiate a speed change to Gen2 or Gen3 after the link is initialized as Gen1 speed. */
  MmioOr32 (DbiBase + PL_GEN2_CTRL_OFF, DIRECT_SPEED_CHANGE);
}

STATIC
VOID
PciSetupLinkSpeed (
  IN EFI_PHYSICAL_ADDRESS  DbiBase,
  IN UINT32                Speed,
  IN UINT32                NumLanes
  )
{
  /* Select target link speed */
  MmioAndThenOr32 (DbiBase + PCIE_LINK_CTL_2, ~0xFU, Speed);
  MmioAndThenOr32 (DbiBase + PCIE_LINK_CAPABILITY, ~0xFU, Speed);

  /* Disable fast link mode, select number of lanes, and enable link initialization */
  MmioAndThenOr32 (
    DbiBase + PL_PORT_LINK_CTRL_OFF,
    ~(LINK_CAPABLE_MASK | FAST_LINK_MODE),
    DLL_LINK_EN | (((NumLanes * 2) - 1) << LINK_CAPABLE_SHIFT)
    );

  /* Select link width */
  MmioAndThenOr32 (
    DbiBase + PL_GEN2_CTRL_OFF,
    ~NUM_OF_LANES_MASK,
    NumLanes << NUM_OF_LANES_SHIFT
    );
}

STATIC
VOID
PciGetLinkSpeedWidth (
  IN EFI_PHYSICAL_ADDRESS  DbiBase,
  OUT UINT32               *Speed,
  OUT UINT32               *Width
  )
{
  UINT32  Val;

  Val    = MmioRead32 (DbiBase + PCIE_LINK_STATUS);
  *Speed = (Val & LINK_STATUS_SPEED_MASK) >> LINK_STATUS_SPEED_SHIFT;
  *Width = (Val & LINK_STATUS_WIDTH_MASK) >> LINK_STATUS_WIDTH_SHIFT;
}

STATIC
VOID
PciPrintLinkSpeedWidth (
  IN UINT32  Speed,
  IN UINT32  Width
  )
{
  char  LinkSpeedBuf[6];

  switch (Speed) {
    case 0:
      AsciiStrCpyS (LinkSpeedBuf, sizeof (LinkSpeedBuf) - 1, "1.25");
      break;
    case 1:
      AsciiStrCpyS (LinkSpeedBuf, sizeof (LinkSpeedBuf) - 1, "2.5");
      break;
    case 2:
      AsciiStrCpyS (LinkSpeedBuf, sizeof (LinkSpeedBuf) - 1, "5.0");
      break;
    case 3:
      AsciiStrCpyS (LinkSpeedBuf, sizeof (LinkSpeedBuf) - 1, "8.0");
      break;
    case 4:
      AsciiStrCpyS (LinkSpeedBuf, sizeof (LinkSpeedBuf) - 1, "16.0");
      break;
    default:
      AsciiSPrint (
        LinkSpeedBuf,
        sizeof (LinkSpeedBuf),
        "%u.%u",
        (Speed * 25) / 10,
        (Speed * 25) % 10
        );
      break;
  }

  DEBUG ((DEBUG_INIT, "PCIe: Link up (x%u, %a GT/s)\n", Width, LinkSpeedBuf));
}

STATIC
VOID
PciEnableLtssm (
  IN EFI_PHYSICAL_ADDRESS  ApbBase,
  IN BOOLEAN               Enable
  )
{
  UINT32  Val;

  Val  = (LINK_REQ_RST_GRT | LTSSM_ENABLE) << 16;
  Val |= LINK_REQ_RST_GRT;
  if (Enable) {
    Val |= LTSSM_ENABLE;
  }

  MmioWrite32 (ApbBase + PCIE_CLIENT_GENERAL_CON, Val);
}

STATIC
BOOLEAN
PciIsLinkUp (
  IN EFI_PHYSICAL_ADDRESS  ApbBase
  )
{
  STATIC UINT32  LastVal = 0xFFFFFFFF;
  UINT32         Val;

  Val = MmioRead32 (ApbBase + PCIE_CLIENT_LTSSM_STATUS);
  if (Val != LastVal) {
    DEBUG ((DEBUG_INIT, "PCIe: PciIsLinkUp(): LTSSM_STATUS=0x%08X\n", Val));
    LastVal = Val;
  }

  if ((Val & RDLH_LINK_UP) == 0) {
    return FALSE;
  }

  if ((Val & SMLH_LINK_UP) == 0) {
    return FALSE;
  }

  return (Val & SMLH_LTSSM_STATE_MASK) == SMLH_LTSSM_STATE_LINK_UP;
}

STATIC
VOID
PciSetupAtu (
  IN EFI_PHYSICAL_ADDRESS  DbiBase,
  IN UINT32                Index,
  IN UINT32                Type,
  IN UINT64                CpuBase,
  IN UINT64                BusBase,
  IN UINT64                Length
  )
{
  UINT32  Ctrl2Off = IATU_ENABLE;

  if ((Type == IATU_TYPE_CFG0) || (Type == IATU_TYPE_CFG1)) {
    Ctrl2Off |= IATU_CFG_SHIFT_MODE;
  }

  MmioWrite32 (
    DbiBase + IATU_REGION_CTRL_OUTBOUND (Index) + IATU_LWR_BASE_ADDR_OFF,
    (UINT32)CpuBase
    );
  MmioWrite32 (
    DbiBase + IATU_REGION_CTRL_OUTBOUND (Index) + IATU_UPPER_BASE_ADDR_OFF,
    (UINT32)(CpuBase >> 32)
    );
  MmioWrite32 (
    DbiBase + IATU_REGION_CTRL_OUTBOUND (Index) + IATU_LIMIT_ADDR_OFF,
    (UINT32)(CpuBase + Length - 1)
    );
  MmioWrite32 (
    DbiBase + IATU_REGION_CTRL_OUTBOUND (Index) + IATU_LWR_TARGET_ADDR_OFF,
    (UINT32)BusBase
    );
  MmioWrite32 (
    DbiBase + IATU_REGION_CTRL_OUTBOUND (Index) + IATU_UPPER_TARGET_ADDR_OFF,
    (UINT32)(BusBase >> 32)
    );
  MmioWrite32 (
    DbiBase + IATU_REGION_CTRL_OUTBOUND (Index) + IATU_REGION_CTRL_1_OFF,
    Type
    );
  MmioWrite32 (
    DbiBase + IATU_REGION_CTRL_OUTBOUND (Index) + IATU_REGION_CTRL_2_OFF,
    Ctrl2Off
    );

  gBS->Stall (10000);
}

STATIC
VOID
PciValidateCfg0 (
  IN  UINT32                Segment,
  IN  EFI_PHYSICAL_ADDRESS  Cfg0Base
  )
{
  EFI_STATUS  Status;

  //
  // Check if the downstream device appears mirrored (due to 64 KB iATU granularity)
  // and needs to have config acesses shifted for single device ECAM mode in ACPI.
  //
  // Warning:
  // Bus hang ups have been observed when doing CFG0 cycles to 01:01.0 (+0x8000)
  // for some devices that have working filtering and aren't mirrored (e.g. VIA VL805).
  //
  // Checking 01:01.0 before 01:00.0 and then never again seems fine, at least for UEFI.
  // In single-device ECAM mode, an OS is going to scan the entire affected bus again, so
  // there's still a risk of locking up the system, but we can't do anything about it.
  //
  if (  (MmioRead32 (Cfg0Base + 0x8000) == 0xffffffff)
     && (MmioRead32 (Cfg0Base) != 0xffffffff))
  {
    Status = PcdSet32S (
               PcdPcieEcamCompliantSegmentsMask,
               PcdGet32 (PcdPcieEcamCompliantSegmentsMask) | (1 << Segment)
               );
    ASSERT_EFI_ERROR (Status);

    DEBUG ((DEBUG_INFO, "PCIe: Working CFG0 TLP filtering for connected device!\n"));
  }
}

#define NUM_SEGMENTS  5
#define NUM_MODES     5

STATIC struct {
  UINT32    Speed;
  UINT32    Width;
} LinkSpeedWidthMap[NUM_MODES][NUM_SEGMENTS] = {
  /* NANBNB */      {
    { 3, 2 },
    { 3, 2 },
    { 2, 1 },
    { 2, 1 },
    { 2, 1 },
  },
  /* NANBBI */      {
    { 3, 1 },
    { 3, 2 },
    { 3, 1 },
    { 2, 1 },
    { 2, 1 },
  },
  /* NABINB */      {
    { 3, 2 },
    { 3, 1 },
    { 2, 1 },
    { 3, 1 },
    { 2, 1 },
  },
  /* NABIBI */      {
    { 3, 1 },
    { 3, 1 },
    { 3, 1 },
    { 3, 1 },
    { 2, 1 },
  },
  /* AGGREGATION */ {
    { 3, 4 },
    { 0, 0 },
    { 2, 1 },
    { 2, 1 },
    { 2, 1 },
  },
};

EFI_STATUS
InitializePciHost (
  UINT32  Segment
  )
{
  EFI_PHYSICAL_ADDRESS  ApbBase = PCIE_APB_BASE (Segment);
  EFI_PHYSICAL_ADDRESS  DbiBase = PCIE_DBI_BASE (Segment);
  EFI_PHYSICAL_ADDRESS  CfgBase;
  EFI_PHYSICAL_ADDRESS  CfgSize;
  EFI_STATUS            Status;
  UINTN                 Retry;
  UINT32                LinkSpeed;
  UINT32                LinkWidth;
  UINT8                 Pcie30PhyMode;

  Pcie30PhyMode = PcdGet8 (PcdPcie30PhyMode);
  if (Pcie30PhyMode >= NUM_MODES) {
    /* If one modified this to some strange value, this will make all things about to work */
    DEBUG ((DEBUG_WARN, "PCIe: Invalid PCIe 3.0 PHY mode %u, use NANBNB(x2x2)\n", Pcie30PhyMode));
    Pcie30PhyMode = PCIE30_PHY_MODE_NANBNB;
  }

  if (Segment >= NUM_SEGMENTS) {
    DEBUG ((DEBUG_WARN, "PCIe: Invalid segment %u\n", Segment));
    return EFI_INVALID_PARAMETER;
  }

  LinkSpeed = LinkSpeedWidthMap[Pcie30PhyMode][Segment].Speed;
  LinkWidth = LinkSpeedWidthMap[Pcie30PhyMode][Segment].Width;
  if ((LinkSpeed == 0) || (LinkWidth == 0)) {
    /* should never here */
    DEBUG ((DEBUG_WARN, "PCIe: Segment %u not enabled\n", Segment));
    return EFI_UNSUPPORTED;
  }

  /* Log settings */
  DEBUG ((DEBUG_INIT, "\nPCIe: Segment %u\n", Segment));
  DEBUG ((DEBUG_INIT, "PCIe: ApbBase 0x%lx\n", ApbBase));
  DEBUG ((DEBUG_INIT, "PCIe: DbiBase 0x%lx\n", DbiBase));
  DEBUG ((DEBUG_INIT, "PCIe: NumLanes %u\n", LinkWidth));
  DEBUG ((DEBUG_INIT, "PCIe: LinkSpeed %u\n", LinkSpeed));

  PcieIoInit (Segment);
  PciePowerEn (Segment, TRUE);
  gBS->Stall (100000);

  if ((Segment == PCIE_SEGMENT_PCIE30X4) || (Segment == PCIE_SEGMENT_PCIE30X2)) {
    /* Configure PCIe 3.0 PHY */
    Status = Pcie30PhyInit ();
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  /* Combo PHY for PCIe 2.0 is configured earlier by RK3588Dxe */

  DEBUG ((DEBUG_INIT, "PCIe: Setup clocks\n"));
  PciSetupClocks (Segment);

  DEBUG ((DEBUG_INIT, "PCIe: Switching to RC mode\n"));
  PciSetRcMode (Segment, ApbBase);

  /* Allow writing RO registers through the DBI */
  DEBUG ((DEBUG_INIT, "PCIe: Enabling DBI access\n"));
  MmioOr32 (DbiBase + PL_MISC_CONTROL_1_OFF, DBI_RO_WR_EN);

  DEBUG ((DEBUG_INIT, "PCIe: Setup BARs\n"));
  PciSetupBars (DbiBase);

  DEBUG ((DEBUG_INIT, "PCIe: Setup iATU\n"));
  CfgBase = PCIE_CFG_BASE (Segment) + PCIE_BUS_BASE_OFFSET (Segment);
  CfgSize = PCIE_BUS_COUNT * SIZE_1MB;

  PciSetupAtu (
    DbiBase,
    0,
    IATU_TYPE_CFG0,
    CfgBase + SIZE_1MB,   // Bus 1
    SIZE_1MB,
    SIZE_64KB             // Should be 32KB but granule is 64KB (see PciValidateCfg0())
    );
  PciSetupAtu (
    DbiBase,
    1,
    IATU_TYPE_CFG1,
    CfgBase + SIZE_2MB,   // Bus 2 and above
    SIZE_2MB,
    CfgSize - SIZE_2MB
    );
  PciSetupAtu (
    DbiBase,
    2,
    IATU_TYPE_IO,
    PCIE_IO_BASE (Segment),
    PCIE_IO_BUS_BASE,
    PCIE_IO_SIZE
    );
  PciSetupAtu (
    DbiBase,
    3,
    IATU_TYPE_MEM,
    PCIE_MEM32_BASE (Segment),
    PCIE_MEM32_BUS_BASE,
    PCIE_MEM32_SIZE
    );

  DEBUG ((DEBUG_INIT, "PCIe: Set link speed\n"));
  PciSetupLinkSpeed (DbiBase, LinkSpeed, LinkWidth);
  PciDirectSpeedChange (DbiBase);

  /* Disallow writing RO registers through the DBI */
  MmioAnd32 (DbiBase + PL_MISC_CONTROL_1_OFF, ~DBI_RO_WR_EN);

  DEBUG ((DEBUG_INIT, "PCIe: Assert reset\n"));
  PciePeReset (Segment, TRUE);

  DEBUG ((DEBUG_INIT, "PCIe: Start LTSSM\n"));

  PciEnableLtssm (ApbBase, TRUE);

  gBS->Stall (100000);
  DEBUG ((DEBUG_INIT, "PCIe: Deassert reset\n"));
  PciePeReset (Segment, FALSE);

  /* Wait for link up */
  DEBUG ((DEBUG_INIT, "PCIe: Waiting for link up...\n"));
  for (Retry = 10; Retry != 0; Retry--) {
    if (PciIsLinkUp (ApbBase)) {
      break;
    }

    gBS->Stall (100000);
  }

  if (Retry == 0) {
    DEBUG ((DEBUG_WARN, "PCIe: Link up timeout!\n"));
    return EFI_TIMEOUT;
  }

  PciGetLinkSpeedWidth (DbiBase, &LinkSpeed, &LinkWidth);
  PciPrintLinkSpeedWidth (LinkSpeed, LinkWidth);

  PciValidateCfg0 (Segment, CfgBase + SIZE_1MB);

  return EFI_SUCCESS;
}
