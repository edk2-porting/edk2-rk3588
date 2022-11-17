/** @file
  PCI Host Bridge Library instance for Marvell 70x0/80x0

  Copyright (c) 2017, Linaro Ltd. All rights reserved.<BR>
  Copyright (c) 2019 Marvell International Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>

#include <IndustryStandard/Pci22.h>

#include <Library/ArmLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MvGpioLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/BoardDesc.h>

#include "PciHostBridgeLibConstructor.h"

/**
  This function configures PCIE controllers IATU windows.

  @param [in] PcieDbiAddress  PCIE controller base address.
  @param [in] Index           IATU window index.
  @param [in] CpuBase         Address from the CPU perspective.
  @param [in] PciBase         Target PCIE address.
  @param [in] Size            IATU window size.
  @param [in] Type            IATU window type.
  @param [in] EnableFlags     Extra configuration flags.

  @retval none

**/
STATIC
VOID
ConfigureWindow (
  IN EFI_PHYSICAL_ADDRESS PcieDbiAddress,
  IN UINTN                Index,
  IN UINT64               CpuBase,
  IN UINT64               PciBase,
  IN UINT64               Size,
  IN UINTN                Type,
  IN UINTN                EnableFlags
  )
{
  ArmDataMemoryBarrier ();

  MmioWrite32 (PcieDbiAddress + IATU_VIEWPORT_OFF,
    IATU_VIEWPORT_OUTBOUND | IATU_VIEWPORT_REGION_INDEX (Index));

  ArmDataMemoryBarrier ();

  MmioWrite32 (PcieDbiAddress + IATU_LWR_BASE_ADDR_OFF_OUTBOUND_0,
    (UINT32)(CpuBase & MAX_UINT32));
  MmioWrite32 (PcieDbiAddress + IATU_UPPER_BASE_ADDR_OFF_OUTBOUND_0,
    (UINT32)(CpuBase >> 32));
  MmioWrite32 (PcieDbiAddress + IATU_LIMIT_ADDR_OFF_OUTBOUND_0,
    (UINT32)(CpuBase + Size - 1));
  MmioWrite32 (PcieDbiAddress + IATU_LWR_TARGET_ADDR_OFF_OUTBOUND_0,
    (UINT32)(PciBase & MAX_UINT32));
  MmioWrite32 (PcieDbiAddress + IATU_UPPER_TARGET_ADDR_OFF_OUTBOUND_0,
    (UINT32)(PciBase >> 32));
  MmioWrite32 (PcieDbiAddress + IATU_REGION_CTRL_1_OFF_OUTBOUND_0,
    Type);
  MmioWrite32 (PcieDbiAddress + IATU_REGION_CTRL_2_OFF_OUTBOUND_0,
    IATU_REGION_CTRL_2_OFF_OUTBOUND_0_REGION_EN | EnableFlags);
}

/**
  Perform PCIE slot reset using external GPIO pin.

  @param [in] PcieDbiAddress  PCIE controller base address.

  @retval none

**/
STATIC
VOID
WaitForLink (
  IN EFI_PHYSICAL_ADDRESS PcieDbiAddress
  )
{
  UINT32 Mask;
  UINT32 Status;
  UINT32 Timeout;

  if (!(MmioRead32 (PcieDbiAddress + PCIE_PM_STATUS) & PCIE_PM_LTSSM_STAT_MASK)) {
    DEBUG ((DEBUG_INIT, "%a: no PCIE device detected\n", __FUNCTION__));
    return;
  }

  /* Wait for the link to establish itself. */
  DEBUG ((DEBUG_INIT, "%a: waiting for PCIE link\n", __FUNCTION__));

  Mask = PCIE_GLOBAL_STATUS_RDLH_LINK_UP | PCIE_GLOBAL_STATUS_PHY_LINK_UP;
  Timeout = PCIE_LINK_UP_TIMEOUT_US / 10;
  do {
    Status = MmioRead32 (PcieDbiAddress + PCIE_GLOBAL_STATUS_REG);
    if ((Status & Mask) == Mask) {
      DEBUG ((DEBUG_ERROR, "pcie@0x%x link UP\n", PcieDbiAddress));
      break;
    }
    /* Wait 10us between each link status check. */
    gBS->Stall (10);
  } while (Timeout--);
}

/**
  Perform PCIE slot reset using external GPIO pin.

  @param [in] *PcieResetGpio  GPIO pin description.

  @retval EFI_SUCEESS         PCIE slot reset succeeded.
  @retval Other               Return error status.

**/
STATIC
EFI_STATUS
ResetPcieSlot (
  IN MV_GPIO_PIN CONST *PcieResetGpio
  )
{
  EMBEDDED_GPIO_MODE   Mode;
  EMBEDDED_GPIO_PIN    GpioPin;
  EMBEDDED_GPIO       *GpioProtocol;
  EFI_STATUS           Status;

  /* Get GPIO protocol. */
  Status = MvGpioGetProtocol (PcieResetGpio->ControllerType, &GpioProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Unable to find GPIO protocol\n", __FUNCTION__));
    return Status;
  }

  GpioPin = GPIO (PcieResetGpio->ControllerId, PcieResetGpio->PinNumber),

  /* Activate reset. */
  Mode = PcieResetGpio->ActiveHigh ? GPIO_MODE_OUTPUT_1 : GPIO_MODE_OUTPUT_0;
  Status = GpioProtocol->Set (GpioProtocol, GpioPin, Mode);

  /*
   * According to the PCIE specification, the reset signal must be active
   * for minimum 100ms. To be on a safe side, use 150ms delay.
   */
  MemoryFence ();
  gBS->Stall (150 * 1000);

  /* Dectivate reset. */
  Mode = PcieResetGpio->ActiveHigh ? GPIO_MODE_OUTPUT_0 : GPIO_MODE_OUTPUT_1;
  Status = GpioProtocol->Set (GpioProtocol, GpioPin, Mode);

  /*
   * The controller cannot be configured (e.g. the clocks have to establish)
   * during 20ms period after the reset is deactivated.
   */
  MemoryFence ();
  gBS->Stall (20 * 1000);

  return EFI_SUCCESS;
}

/**
  Obtain resources and perform a low-level PCIE controllers
  configuration.

  @param [in]  ImageHandle  The image handle.
  @param [in] *SystemTable  The system table.

  @retval EFI_SUCEESS       PCIE configuration successful.
  @retval Other             Return error status.

**/
EFI_STATUS
EFIAPI
Armada7k8kPciHostBridgeLibConstructor (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  MV_BOARD_PCIE_DESCRIPTION CONST *BoardPcieDescription;
  MARVELL_BOARD_DESC_PROTOCOL     *BoardDescriptionProtocol;
  MV_PCIE_CONTROLLER CONST        *PcieController;
  EFI_PHYSICAL_ADDRESS             PcieDbiAddress;
  EFI_STATUS                       Status;
  UINTN                            Index;

  /* Obtain list of available controllers */
  Status = gBS->LocateProtocol (&gMarvellBoardDescProtocolGuid,
                  NULL,
                  (VOID **)&BoardDescriptionProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Cannot locate BoardDesc protocol\n",
      __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Status = BoardDescriptionProtocol->PcieDescriptionGet (
                                       BoardDescriptionProtocol,
                                       &BoardPcieDescription);
  if (Status == EFI_NOT_FOUND) {
    /* No controllers used on the platform, exit silently */
    return EFI_SUCCESS;
  } else if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Cannot get Pcie board desc from BoardDesc protocol\n",
      __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  for (Index = 0; Index < BoardPcieDescription->PcieControllerCount; Index++) {

    PcieController = &(BoardPcieDescription->PcieControllers[Index]);

    ASSERT (PcieController->PcieBusMin == 0);

    if (PcieController->HaveResetGpio == TRUE) {
      /* Reset PCIE slot */
      Status = ResetPcieSlot (&PcieController->PcieResetGpio);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR,
          "%a: Cannot reset Pcie Slot\n",
          __FUNCTION__));
        return EFI_DEVICE_ERROR;
      }
    }

    /* Low level PCIE controller configuration */
    PcieDbiAddress = PcieController->PcieDbiAddress;

    MmioAndThenOr32 (PcieDbiAddress + PORT_LINK_CTRL_OFF,
      ~PORT_LINK_CTRL_OFF_LINK_CAPABLE_MASK,
      PORT_LINK_CTRL_OFF_LINK_CAPABLE_x4);

    MmioAndThenOr32 (PcieDbiAddress + GEN2_CTRL_OFF,
      ~GEN2_CTRL_OFF_NUM_OF_LANES_MASK,
      GEN2_CTRL_OFF_NUM_OF_LANES(4) | GEN2_CTRL_OFF_DIRECT_SPEED_CHANGE);

    MmioAndThenOr32 (PcieDbiAddress + PCIE_GLOBAL_CTRL_OFFSET,
      ~(PCIE_GLOBAL_CTRL_DEVICE_TYPE_MASK | PCIE_GLOBAL_APP_LTSSM_EN),
      PCIE_GLOBAL_CTRL_DEVICE_TYPE_RC);

    MmioWrite32 (PcieDbiAddress + PCIE_ARCACHE_TRC_REG,
      ARCACHE_DEFAULT_VALUE);

    MmioWrite32 (PcieDbiAddress + PCIE_AWCACHE_TRC_REG,
      AWCACHE_DEFAULT_VALUE);

    MmioAndThenOr32 (PcieDbiAddress + PCIE_ARUSER_REG,
      ~AX_USER_DOMAIN_MASK,
      AX_USER_DOMAIN_OUTER_SHAREABLE);

    MmioAndThenOr32 (PcieDbiAddress + PCIE_AWUSER_REG,
      ~AX_USER_DOMAIN_MASK,
      AX_USER_DOMAIN_OUTER_SHAREABLE);

    MmioAndThenOr32 (PcieDbiAddress + PCIE_LINK_CTL_2,
      ~TARGET_LINK_SPEED_MASK,
      LINK_SPEED_GEN_3);

    MmioAndThenOr32 (PcieDbiAddress + PCIE_LINK_CAPABILITY,
      ~TARGET_LINK_SPEED_MASK,
      LINK_SPEED_GEN_3);

    MmioOr32 (PcieDbiAddress + PCIE_GEN3_EQU_CTRL,
      GEN3_EQU_EVAL_2MS_DISABLE);

    MmioOr32 (PcieDbiAddress + PCIE_GLOBAL_CTRL_OFFSET,
      PCIE_GLOBAL_APP_LTSSM_EN);

    /* Region 0: MMIO32 range */
    ConfigureWindow (PcieDbiAddress,
      PcieController->PcieMmio32WinBase - PcieController->PcieMmio32Translation,
      PcieController->PcieMmio32WinBase,
      PcieController->PcieMmio32WinBase,
      PcieController->PcieMmio32WinSize,
      IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_MEM,
      0);

    /* Region 1: Type 0 config space */
    ConfigureWindow (PcieDbiAddress,
      1,
      PcieController->ConfigSpaceAddress,
      0x0,
      SIZE_64KB,
      IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_CFG0,
      IATU_REGION_CTRL_2_OFF_OUTBOUND_0_CFG_SHIFT_MODE);

    /* Region 2: Type 1 config space */
    ConfigureWindow (PcieDbiAddress,
      2,
      PcieController->ConfigSpaceAddress + SIZE_64KB,
      0x0,
      PcieController->PcieBusMax * SIZE_1MB,
      IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_CFG1,
      IATU_REGION_CTRL_2_OFF_OUTBOUND_0_CFG_SHIFT_MODE);

    /* Region 3: port I/O range */
    ConfigureWindow (PcieDbiAddress,
      3,
      PcieController->PcieIoTranslation,
      PcieController->PcieIoWinBase,
      PcieController->PcieIoWinSize,
      IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_IO,
      0);

    /* Region 4: MMIO64 range */
    ConfigureWindow (PcieDbiAddress,
      4,
      PcieController->PcieMmio64WinBase,
      PcieController->PcieMmio64WinBase,
      PcieController->PcieMmio64WinSize,
      IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_MEM,
      0);

    MmioOr32 (PcieDbiAddress + PCIE_GLOBAL_INT_MASK1_REG,
      PCIE_INT_A_ASSERT_MASK |
      PCIE_INT_B_ASSERT_MASK |
      PCIE_INT_C_ASSERT_MASK |
      PCIE_INT_D_ASSERT_MASK);

    WaitForLink (PcieDbiAddress);

    /* Enable the RC */
    MmioOr32 (PcieDbiAddress + PCI_COMMAND_OFFSET,
      EFI_PCI_COMMAND_IO_SPACE |
      EFI_PCI_COMMAND_MEMORY_SPACE |
      EFI_PCI_COMMAND_BUS_MASTER);
  }

  return EFI_SUCCESS;
}
