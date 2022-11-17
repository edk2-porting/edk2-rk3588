/** @file
  PCI Host Bridge Library instance for Socionext SynQuacer ARM SOC

  Copyright (c) 2017, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>
#include <IndustryStandard/Pci22.h>
#include <Library/ArmLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/PciHostBridgeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Platform/Pcie.h>
#include <Platform/VarStore.h>
#include <Protocol/PciHostBridgeResourceAllocation.h>

#define IATU_VIEWPORT_OFF                                   0x900
#define IATU_VIEWPORT_INBOUND                               BIT31
#define IATU_VIEWPORT_OUTBOUND                              0
#define IATU_VIEWPORT_REGION_INDEX(Idx)                     ((Idx) & 7)

#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0                   0x904
#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_MEM          0x0
#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_IO           0x2
#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_CFG0         0x4
#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_CFG1         0x5
#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TH                BIT12

#define IATU_REGION_CTRL_2_OFF_OUTBOUND_0                   0x908
#define IATU_REGION_CTRL_2_OFF_OUTBOUND_0_REGION_EN         BIT31
#define IATU_REGION_CTRL_2_OFF_OUTBOUND_0_CFG_SHIFT_MODE    BIT28
#define IATU_REGION_CTRL_2_OFF_OUTBOUND_0_MSG_CODE_32BIT    0xF
#define IATU_REGION_CTRL_2_OFF_OUTBOUND_0_MSG_CODE_64BIT    0xFF

#define IATU_LWR_BASE_ADDR_OFF_OUTBOUND_0                   0x90C
#define IATU_UPPER_BASE_ADDR_OFF_OUTBOUND_0                 0x910
#define IATU_LIMIT_ADDR_OFF_OUTBOUND_0                      0x914
#define IATU_LWR_TARGET_ADDR_OFF_OUTBOUND_0                 0x918
#define IATU_UPPER_TARGET_ADDR_OFF_OUTBOUND_0               0x91C

#define CORE_CONTROL                  0x000
#define APP_LTSSM_ENABLE              BIT4
#define DEVICE_TYPE                   (BIT3 | BIT2 | BIT1 | BIT0)

#define AXI_CLK_STOP                  0x004
#define DBI_ACLK_STOP                 BIT8
#define SLV_ACLK_STOP                 BIT4
#define MSTR_ACLK_STOP                BIT0
#define DBI_CSYSREQ_REG               BIT9
#define SLV_CSYSREQ_REG               BIT5
#define MSTR_CSYSREQ_REG              BIT1

#define RESET_CONTROL_1               0x00C
#define PERST_N_O_REG                 BIT5
#define PERST_N_I_REG                 BIT4
#define BUTTON_RST_N_REG              BIT1
#define PWUP_RST_N_REG                BIT0

#define RESET_CONTROL_2               0x010

#define RESET_SELECT_1                0x014
#define SQU_RST_SEL                   BIT29
#define PHY_RST_SEL                   BIT28
#define PWR_RST_SEL                   BIT24
#define STI_RST_SEL                   BIT20
#define N_STI_RST_SEL                 BIT16
#define CORE_RST_SEL                  BIT12
#define PERST_SEL                     BIT4
#define BUTTON_RST_SEL                BIT1
#define PWUP_RST_SEL                  BIT0

#define RESET_SELECT_2                0x018
#define DBI_ARST_SEL                  BIT8
#define SLV_ARST_SEL                  BIT4
#define MSTR_ARST_SEL                 BIT0

#define EM_CONTROL                    0x030
#define PRE_DET_STT_REG               BIT4

#define EM_SELECT                     0x034
#define PRE_DET_STT_SEL               BIT4

#define PM_CONTROL_2                  0x050
#define SYS_AUX_PWR_DET               BIT8

#define PHY_CONFIG_COM_6              0x114
#define PIPE_PORT_SEL                 (BIT1 | BIT0)

#define LINK_MONITOR                  0x210
#define SMLH_LINK_UP                  BIT0

#define LINK_CAPABILITIES_REG         0x07C
#define PCIE_CAP_MAX_LINK_WIDTH       (BIT7 | BIT6 | BIT5 | BIT4)
#define PCIE_CAP_MAX_LINK_SPEED       (BIT3 | BIT2 | BIT1 | BIT0)

#define LINK_CONTROL_LINK_STATUS_REG  0x080
#define PCIE_CAP_NEGO_LINK_WIDTH      (BIT23 | BIT22 | BIT21 | BIT20)
#define PCIE_CAP_LINK_SPEED           (BIT19 | BIT18 | BIT17 | BIT16)

#define TYPE1_CLASS_CODE_REV_ID_REG   0x008
#define BASE_CLASS_CODE               0xFF000000
#define BASE_CLASS_CODE_VALUE         0x06
#define SUBCLASS_CODE                 0x00FF0000
#define SUBCLASS_CODE_VALUE           0x04
#define PROGRAM_INTERFACE             0x0000FF00
#define PROGRAM_INTERFACE_VALUE       0x00

#define GEN2_CONTROL_OFF              0x80c
#define DIRECT_SPEED_CHANGE           BIT17

#define MISC_CONTROL_1_OFF            0x8BC
#define DBI_RO_WR_EN                  BIT0

extern PCI_ROOT_BRIDGE                mPciRootBridges[];

STATIC
VOID
ConfigureWindow (
  IN  EFI_PHYSICAL_ADDRESS    DbiBase,
  IN  UINTN                   Index,
  IN  UINT64                  CpuBase,
  IN  UINT64                  PciBase,
  IN  UINT64                  Size,
  IN  UINTN                   Type,
  IN  UINTN                   EnableFlags
  )
{
  ArmDataMemoryBarrier ();

  MmioWrite32 (DbiBase + IATU_VIEWPORT_OFF,
               IATU_VIEWPORT_OUTBOUND | IATU_VIEWPORT_REGION_INDEX (Index));

  ArmDataMemoryBarrier ();

  MmioWrite32 (DbiBase + IATU_LWR_BASE_ADDR_OFF_OUTBOUND_0,
               (UINT32)(CpuBase & 0xFFFFFFFF));
  MmioWrite32 (DbiBase + IATU_UPPER_BASE_ADDR_OFF_OUTBOUND_0,
               (UINT32)(CpuBase >> 32));
  MmioWrite32 (DbiBase + IATU_LIMIT_ADDR_OFF_OUTBOUND_0,
               (UINT32)(CpuBase + Size - 1));
  MmioWrite32 (DbiBase + IATU_LWR_TARGET_ADDR_OFF_OUTBOUND_0,
               (UINT32)(PciBase & 0xFFFFFFFF));
  MmioWrite32 (DbiBase + IATU_UPPER_TARGET_ADDR_OFF_OUTBOUND_0,
               (UINT32)(PciBase >> 32));
  MmioWrite32 (DbiBase + IATU_REGION_CTRL_1_OFF_OUTBOUND_0,
               Type);
  MmioWrite32 (DbiBase + IATU_REGION_CTRL_2_OFF_OUTBOUND_0,
               IATU_REGION_CTRL_2_OFF_OUTBOUND_0_REGION_EN | EnableFlags);
}

STATIC
VOID
SnPcieSetData (
  EFI_PHYSICAL_ADDRESS  Base,
  UINT32                Offset,
  UINT32                Mask,
  UINT32                In
  )
{
  UINT32 Data;
  UINT32 Shift;

  Shift = 1;
  if (In) {
    while (!(Mask & Shift))
      Shift <<= 1;
    Data = (MmioRead32 (Base + Offset) & ~Mask) | (In * Shift);
  } else {
    Data = MmioRead32 (Base + Offset) & ~Mask;
  }

  MmioWrite32 (Base + Offset, Data);

  ArmDataMemoryBarrier ();
}

STATIC
UINT32
SnPcieReadData (
  EFI_PHYSICAL_ADDRESS  Base,
  UINT32                Offset,
  UINT32                Mask
  )
{
  UINT32 Shift;

  Shift = 0;
  while (!(Mask & 1)) {
    Mask >>= 1;
    Shift++;
  }

  ArmDataMemoryBarrier ();

  return (MmioRead32 (Base + Offset) >> Shift) & Mask;
}

STATIC
VOID
SnDbiRoWrEn (
  IN  EFI_PHYSICAL_ADDRESS    DbiBase,
  IN  INTN                    MaxLinkWidth,
  IN  INTN                    MaxLinkSpeed
  )
{
  SnPcieSetData (DbiBase, MISC_CONTROL_1_OFF, DBI_RO_WR_EN, 1);

  SnPcieSetData (DbiBase, LINK_CAPABILITIES_REG, PCIE_CAP_MAX_LINK_WIDTH, MaxLinkWidth);
  SnPcieSetData (DbiBase, LINK_CAPABILITIES_REG, PCIE_CAP_MAX_LINK_SPEED, MaxLinkSpeed);

  SnPcieSetData (DbiBase, TYPE1_CLASS_CODE_REV_ID_REG, BASE_CLASS_CODE, BASE_CLASS_CODE_VALUE);
  SnPcieSetData (DbiBase, TYPE1_CLASS_CODE_REV_ID_REG, SUBCLASS_CODE, SUBCLASS_CODE_VALUE);
  SnPcieSetData (DbiBase, TYPE1_CLASS_CODE_REV_ID_REG, PROGRAM_INTERFACE, PROGRAM_INTERFACE_VALUE);

  SnPcieSetData (DbiBase, MISC_CONTROL_1_OFF, DBI_RO_WR_EN, 0);
}

STATIC
VOID
PciInitControllerPre (
  IN  EFI_PHYSICAL_ADDRESS    ExsBase
  )
{
  SnPcieSetData (ExsBase, EM_SELECT, PRE_DET_STT_SEL, 0);
  SnPcieSetData (ExsBase, EM_CONTROL, PRE_DET_STT_REG, 0);
  SnPcieSetData (ExsBase, EM_CONTROL, PRE_DET_STT_REG, 1);

  // 1: Assert all PHY / LINK resets
  SnPcieSetData (ExsBase, RESET_SELECT_1 , PERST_SEL     , 0);
  SnPcieSetData (ExsBase, RESET_CONTROL_1, PERST_N_I_REG , 0);
  SnPcieSetData (ExsBase, RESET_CONTROL_1, PERST_N_O_REG , 0);

  // Device Reset(PERST#) is effective afrer Set device_type (RC)
  SnPcieSetData (ExsBase, RESET_SELECT_1 , PWUP_RST_SEL  , 0);
  SnPcieSetData (ExsBase, RESET_CONTROL_1, PWUP_RST_N_REG, 0);
  SnPcieSetData (ExsBase, RESET_SELECT_1 , BUTTON_RST_SEL  , 0);
  SnPcieSetData (ExsBase, RESET_CONTROL_1, BUTTON_RST_N_REG, 0);
  SnPcieSetData (ExsBase, RESET_SELECT_1 , PWR_RST_SEL     , 1);
  SnPcieSetData (ExsBase, RESET_SELECT_2 , MSTR_ARST_SEL   , 1);
  SnPcieSetData (ExsBase, RESET_SELECT_2 , SLV_ARST_SEL    , 1);
  SnPcieSetData (ExsBase, RESET_SELECT_2 , DBI_ARST_SEL    , 1);
  SnPcieSetData (ExsBase, RESET_SELECT_1 , CORE_RST_SEL    , 1);
  SnPcieSetData (ExsBase, RESET_SELECT_1 , STI_RST_SEL     , 1);
  SnPcieSetData (ExsBase, RESET_SELECT_1 , N_STI_RST_SEL   , 1);
  SnPcieSetData (ExsBase, RESET_SELECT_1 , SQU_RST_SEL     , 1);
  SnPcieSetData (ExsBase, RESET_SELECT_1 , PHY_RST_SEL     , 1);

  // 2: Set P<n>_app_ltssm_enable='0' for reprogramming before linkup.
  SnPcieSetData (ExsBase, CORE_CONTROL, APP_LTSSM_ENABLE, 0);

  // 3: Set device_type (RC)
  SnPcieSetData (ExsBase, CORE_CONTROL, DEVICE_TYPE, 4);
}

STATIC
VOID
PciInitControllerPost (
  IN  EFI_PHYSICAL_ADDRESS    ExsBase,
  IN  EFI_PHYSICAL_ADDRESS    DbiBase,
  IN  EFI_PHYSICAL_ADDRESS    ConfigBase,
  IN  EFI_PHYSICAL_ADDRESS    IoMemBase,
  IN  CONST PCI_ROOT_BRIDGE   *RootBridge,
  IN  BOOLEAN                 EnableGen2Speed
  )
{
  // 4: Set Bifurcation  1=disable  4=able
  // 5: Supply Reference (It has executed)
  // 6: Wait for 10usec (Reference Clocks is stable)
  // 7 De assert PERST# */
  SnPcieSetData (ExsBase, RESET_CONTROL_1, PERST_N_I_REG, 1);
  SnPcieSetData (ExsBase, RESET_CONTROL_1, PERST_N_O_REG, 1);

  // 8 Assert SYS_AUX_PWR_DET
  SnPcieSetData(ExsBase, PM_CONTROL_2, SYS_AUX_PWR_DET, 1);

  // 9 Supply following clocks
  SnPcieSetData (ExsBase, AXI_CLK_STOP, MSTR_CSYSREQ_REG, 1);
  SnPcieSetData (ExsBase, AXI_CLK_STOP, MSTR_ACLK_STOP, 0);
  SnPcieSetData (ExsBase, AXI_CLK_STOP, SLV_CSYSREQ_REG, 1);
  SnPcieSetData (ExsBase, AXI_CLK_STOP, SLV_ACLK_STOP, 0);
  SnPcieSetData (ExsBase, AXI_CLK_STOP, DBI_CSYSREQ_REG, 1);
  SnPcieSetData (ExsBase, AXI_CLK_STOP, DBI_ACLK_STOP, 0);

  // 10 De assert PHY reset
  // 11 De assert LINK's PMC reset
  SnPcieSetData (ExsBase, RESET_CONTROL_1, PWUP_RST_N_REG, 1);
  SnPcieSetData (ExsBase, RESET_CONTROL_1, BUTTON_RST_N_REG, 1);
  // 12 PHY auto
  // 13 Wrapper auto
  // 14-17 PHY auto
  // 18 Wrapper auto
  // 19 Update registers through DBI AXI Slave interface
  SnDbiRoWrEn (DbiBase, 4 /* lanes */, /* Gen */ 2);

  //
  // ECAM shift mode uses bits [27:12] of the untranslated address as
  // B/D/F identifiers. This only works as expected if the base of the
  // region is aligned to 256 MB, or the effective bus numbers will be
  // out of sync with the bus base and limit values we chose.
  //
  ASSERT ((ConfigBase % SIZE_256MB) == RootBridge->Bus.Base * SIZE_1MB);

  MmioOr32 (DbiBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_IO_SPACE |
                                          EFI_PCI_COMMAND_MEMORY_SPACE |
                                          EFI_PCI_COMMAND_BUS_MASTER);

  if (EnableGen2Speed) {
    // Force link speed change to Gen2 at link up
    MmioOr32 (DbiBase + GEN2_CONTROL_OFF, DIRECT_SPEED_CHANGE);
  }

  // Region 0: MMIO32 range
  ConfigureWindow (DbiBase, 0,
    RootBridge->Mem.Base - RootBridge->Mem.Translation,
    RootBridge->Mem.Base,
    RootBridge->Mem.Limit - RootBridge->Mem.Base + 1,
    IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_MEM |
    IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TH,
    IATU_REGION_CTRL_2_OFF_OUTBOUND_0_MSG_CODE_32BIT);

  // Region 1: Type 0 config space
  ConfigureWindow (DbiBase, 1,
    ConfigBase + RootBridge->Bus.Base * SIZE_1MB,
    0x0,
    SIZE_64KB,
    IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_CFG0,
    IATU_REGION_CTRL_2_OFF_OUTBOUND_0_CFG_SHIFT_MODE);

  // Region 2: Type 1 config space
  ConfigureWindow (DbiBase, 2,
    ConfigBase + RootBridge->Bus.Base * SIZE_1MB + SIZE_64KB,
    0x0,
    (RootBridge->Bus.Limit - RootBridge->Bus.Base + 1) * SIZE_1MB - SIZE_64KB,
    IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_CFG1,
    IATU_REGION_CTRL_2_OFF_OUTBOUND_0_CFG_SHIFT_MODE);

  // Region 3: port I/O range
  ConfigureWindow (DbiBase, 3,
    IoMemBase,
    RootBridge->Io.Base,
    RootBridge->Io.Limit - RootBridge->Io.Base + 1,
    IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_IO,
    0);

  // Region 4: MMIO64 range
  if (RootBridge->AllocationAttributes & EFI_PCI_HOST_BRIDGE_MEM64_DECODE) {
    ConfigureWindow (DbiBase, 4,
      RootBridge->MemAbove4G.Base,
      RootBridge->MemAbove4G.Base,
      RootBridge->MemAbove4G.Limit - RootBridge->MemAbove4G.Base + 1,
      IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_MEM |
      IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TH,
      IATU_REGION_CTRL_2_OFF_OUTBOUND_0_MSG_CODE_32BIT);
  }

  // enable link
  if (SnPcieReadData (ExsBase, CORE_CONTROL, APP_LTSSM_ENABLE) == 0) {
    SnPcieSetData (ExsBase, CORE_CONTROL, APP_LTSSM_ENABLE, 1);
  }
}

STATIC CONST struct {
  EFI_PHYSICAL_ADDRESS      DbiBase;
  EFI_PHYSICAL_ADDRESS      ExsBase;
  EFI_PHYSICAL_ADDRESS      ConfigBase;
  EFI_PHYSICAL_ADDRESS      IoMemBase;
} mBaseAddresses [] = {
  {
    SYNQUACER_PCI_SEG0_DBI_BASE,
    SYNQUACER_PCI_SEG0_EXS_BASE,
    SYNQUACER_PCI_SEG0_CONFIG_BASE,
    SYNQUACER_PCI_SEG0_PORTIO_MEMBASE
  },
  {
    SYNQUACER_PCI_SEG1_DBI_BASE,
    SYNQUACER_PCI_SEG1_EXS_BASE,
    SYNQUACER_PCI_SEG1_CONFIG_BASE,
    SYNQUACER_PCI_SEG1_PORTIO_MEMBASE
  },
};


EFI_STATUS
EFIAPI
SynQuacerPciHostBridgeLibConstructor (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  UINTN                             Idx;
  UINT64                            SettingsVal;
  SYNQUACER_PLATFORM_VARSTORE_DATA  *Settings;
  UINT8                             MaxSpeed;

  SettingsVal = PcdGet64 (PcdPlatformSettings);
  Settings = (SYNQUACER_PLATFORM_VARSTORE_DATA *)&SettingsVal;

  for (Idx = 0; Idx < ARRAY_SIZE (mBaseAddresses); Idx++) {
    if (PcdGet8 (PcdPcieEnableMask) & (1 << Idx)) {
      PciInitControllerPre (mBaseAddresses[Idx].ExsBase);
    }
  }

  //
  // The PCIe spec requires that PERST# is asserted for at least 100 ms after
  // the power and clocks have become stable. So let's give a bit or margin,
  // and stall for 150 ms between asserting PERST# on both controllers and
  // de-asserting it again.
  //
  gBS->Stall (150 * 1000);

  for (Idx = 0; Idx < ARRAY_SIZE (mBaseAddresses); Idx++) {
    //
    // Check whether this root port is described by any of our 'slot'
    // definitions, and get the maximum speed if this is the case.
    //
    switch (SYNQUACER_PCI_LOCATION (Idx, 0, 0)) {
    case SYNQUACER_PCI_SLOT0_LOCATION:
      MaxSpeed = Settings->PcieSlot0MaxSpeed;
      break;
    case SYNQUACER_PCI_SLOT1_LOCATION:
      MaxSpeed = Settings->PcieSlot1MaxSpeed;
      break;
    case SYNQUACER_PCI_SLOT2_LOCATION:
      MaxSpeed = Settings->PcieSlot2MaxSpeed;
      break;
    default:
      MaxSpeed = PCIE_MAX_SPEED_UNLIMITED;
    }

    if (PcdGet8 (PcdPcieEnableMask) & (1 << Idx)) {
      PciInitControllerPost (mBaseAddresses[Idx].ExsBase,
                             mBaseAddresses[Idx].DbiBase,
                             mBaseAddresses[Idx].ConfigBase,
                             mBaseAddresses[Idx].IoMemBase,
                             &mPciRootBridges[Idx],
                             (MaxSpeed != PCIE_MAX_SPEED_GEN1));
    }
  }

  return EFI_SUCCESS;
}
