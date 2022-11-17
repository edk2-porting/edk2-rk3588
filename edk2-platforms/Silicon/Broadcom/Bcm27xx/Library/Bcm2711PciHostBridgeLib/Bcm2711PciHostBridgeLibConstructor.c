/** @file
 *
 * PCI Host Bridge Library instance for Bcm2711 ARM SOC
 *
 * Copyright (c) 2019, Jeremy Linton
 * Copyright (c) 2017, Linaro Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 * This module initializes the Pci as close to a standard
 * PCI root complex as possible. The general information
 * for this driver was sourced from.
 *
 * See https://github.com/raspberrypi/linux/blob/rpi-5.3.y/drivers/pci/controller/pcie-brcmstb.c
 * and https://github.com/raspberrypi/linux/blob/rpi-5.3.y/arch/arm/boot/dts/bcm2838.dtsi
 *
 **/

#include <IndustryStandard/Bcm2711.h>
#include <IndustryStandard/Pci22.h>
#include <Library/ArmLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/PciHostBridgeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <PiDxe.h>
#include <Protocol/PciHostBridgeResourceAllocation.h>

STATIC
UINT32
RdRegister (
  UINT32                Offset
  )
{
  ArmDataMemoryBarrier ();
  return MmioRead32 (PCIE_REG_BASE + Offset);
}


STATIC
VOID
RMWRegister (
  UINT32                Offset,
  UINT32                Mask,
  UINT32                In
  )
{
  EFI_PHYSICAL_ADDRESS  Addr;
  UINT32                Data;
  UINT32                Shift;

  Addr = PCIE_REG_BASE + Offset;
  Shift = 1;
  if (In) {
    while (!(Mask & Shift))
      Shift <<= 1;
    Data = (MmioRead32 (Addr) & ~Mask) | ((In * Shift) & Mask);
  } else {
    Data = MmioRead32 (Addr) & ~Mask;
  }

  MmioWrite32 (Addr, Data);

  ArmDataMemoryBarrier ();
}


STATIC
VOID
WdRegister (
  UINT32                Offset,
  UINT32                In
  )
{
  MmioWrite32 (PCIE_REG_BASE + Offset, In);
  ArmDataMemoryBarrier ();
}


EFI_STATUS
EFIAPI
Bcm2711PciHostBridgeLibConstructor (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  INTN                  Timeout;
  UINT32                Data;
  EFI_PHYSICAL_ADDRESS  TopOfPciMap;

  DEBUG ((DEBUG_VERBOSE, "PCIe RootBridge constructor\n"));

  Timeout = 100;

  // Reset controller
  RMWRegister (PCIE_RGR1_SW_INIT_1, PCIE_RGR1_SW_INIT_1_INIT_MASK, 1);
  // PERST
  RMWRegister (PCIE_RGR1_SW_INIT_1, PCIE_RGR1_SW_INIT_1_PERST_MASK, 1);

  gBS->Stall (1000);
  // take the bridge out of reset
  RMWRegister (PCIE_RGR1_SW_INIT_1, PCIE_RGR1_SW_INIT_1_INIT_MASK, 0);


  RMWRegister (PCIE_MISC_HARD_PCIE_HARD_DEBUG,
    PCIE_MISC_HARD_PCIE_HARD_DEBUG_SERDES_IDDQ_MASK, 0);
  RdRegister (PCIE_MISC_HARD_PCIE_HARD_DEBUG);
  // Wait for SerDes to be stable
  gBS->Stall (1000);

  // Read revision
  Data = RdRegister (PCIE_MISC_REVISION);
  DEBUG ((DEBUG_INFO, "RootBridge: Revision %x\n", Data & PCIE_MISC_REVISION_MAJMIN_MASK));

  RMWRegister (PCIE_MISC_MISC_CTRL, PCIE_MISC_MISC_CTRL_SCB_ACCESS_EN_MASK, 1);
  RMWRegister (PCIE_MISC_MISC_CTRL, PCIE_MISC_MISC_CTRL_CFG_READ_UR_MODE_MASK, 1);
  RMWRegister (PCIE_MISC_MISC_CTRL, PCIE_MISC_MISC_CTRL_MAX_BURST_SIZE_MASK, BURST_SIZE_128);

  //
  // "RC_BAR2" is the inbound TLP window.
  // Having non RAM regions in the window is ok (and encouraged? for PtP?)
  // so lets just map the entire address space.
  //
  // For regions > 64K then the pci->mem window size = log2(size)-15
  // which is dumped into the low bits of the offset and written to
  // the "LO" register with the high bits of the offset written into
  // the "HI" part. The Linux driver makes the point that the offset
  // must be aligned to its size aka a 1G region must start on a 1G
  // boundary. The size parms are 1GB=0xf=log2(size)-15), or 4G=0x11
  //

  DEBUG ((DEBUG_VERBOSE, "RootBridge: Program bottom 4G of ram\n"));

  // lets assume a start addr of 0, size 4G
  WdRegister (PCIE_MISC_RC_BAR2_CONFIG_LO, 0x11);   /* Size = 4G */
  WdRegister (PCIE_MISC_RC_BAR2_CONFIG_HI, 0);      /* Start at addr0 */
  RMWRegister (PCIE_MISC_MISC_CTRL, PCIE_MISC_MISC_CTRL_SCB0_SIZE_MASK, 0x11);

  // RC_BAR1 pcie->gisb disable
  WdRegister (PCIE_MISC_RC_BAR1_CONFIG_LO, 0);
  // RC_BAR3 pcie->scb disable
  WdRegister (PCIE_MISC_RC_BAR3_CONFIG_LO, 0);

  TopOfPciMap = PCIE_TOP_OF_MEM_WIN;

  DEBUG ((DEBUG_VERBOSE, "RootBridge: MMIO PCIe addr %llx\n", TopOfPciMap));

  //
  // Setup the PCI side of the MMIO window.
  //
  // All the _WIN0_ values make one think there can be more than one
  // mapping, which might mean it's possible to program a prefetchable
  // window, or a PIO window...
  //
  WdRegister (PCIE_MISC_CPU_2_PCIE_MEM_WIN0_LO, TopOfPciMap);
  WdRegister (PCIE_MISC_CPU_2_PCIE_MEM_WIN0_HI, TopOfPciMap >> 32);

  //
  // Set up the CPU MMIO addresses. The BASE_LIMIT register holds the
  // bottom part of the start and end addresses in a 16-bit field (64k)
  // aligned on a 1M boundary (aka only 12 bit active) the top 32-bits
  // are then in their own registers. Further these address ranges are
  // setup to match the Linux driver and seem less than ideal on the RPi
  //
  // The mapping should be 1:1 if possible
  //
  EFI_PHYSICAL_ADDRESS    CpuAddrStart = PCIE_CPU_MMIO_WINDOW;
  EFI_PHYSICAL_ADDRESS    CpuAddrEnd   = CpuAddrStart + PCIE_BRIDGE_MMIO_LEN;

  DEBUG ((DEBUG_VERBOSE, "RootBridge: MMIO CPU addr %llx\n", CpuAddrStart));

  RMWRegister (PCIE_MISC_CPU_2_PCIE_MEM_WIN0_BASE_LIMIT,
    PCIE_MISC_CPU_2_PCIE_MEM_WIN0_BASE_LIMIT_BASE_MASK, CpuAddrStart >> 20);
  RMWRegister (PCIE_MISC_CPU_2_PCIE_MEM_WIN0_BASE_LIMIT,
    PCIE_MISC_CPU_2_PCIE_MEM_WIN0_BASE_LIMIT_LIMIT_MASK, CpuAddrEnd >> 20);
  RMWRegister (PCIE_MISC_CPU_2_PCIE_MEM_WIN0_BASE_HI,
    PCIE_MISC_CPU_2_PCIE_MEM_WIN0_BASE_HI_BASE_MASK, CpuAddrStart >> 32);
  RMWRegister (PCIE_MISC_CPU_2_PCIE_MEM_WIN0_LIMIT_HI,
    PCIE_MISC_CPU_2_PCIE_MEM_WIN0_LIMIT_HI_LIMIT_MASK, CpuAddrEnd >> 32);

  //
  // Consider MSI setup here, not that it matters much its likely the legacy intX
  // is as fast or faster...
  //

  // Clear and mask interrupts.
  WdRegister (PCIE_INTR2_CPU_MASK_CLR, 0xffffffff);
  WdRegister (PCIE_INTR2_CPU_MASK_SET, 0xffffffff);

  // Set link cap & link ctl?
  //RMWRegister (BRCM_PCIE_CAP_REGS+PCI_LNKCAP, LNKCAP, pen);
  //RMWRegister (BRCM_PCIE_CTL_REGS+PCI_LNKCAP, LNKCAP, pen);

  // De-assert PERST
  RMWRegister (PCIE_RGR1_SW_INIT_1, PCIE_RGR1_SW_INIT_1_PERST_MASK, 0);
  DEBUG ((DEBUG_VERBOSE, "RootBridge: Reset done\n"));

  // Wait for linkup
  do {
      Data = RdRegister (PCIE_MISC_PCIE_STATUS);
      gBS->Stall (1000);
      Timeout --;
  } while (((Data & 0x30) != 0x030) && (Timeout));
  DEBUG ((DEBUG_VERBOSE, "PCIe link ready (status=%x) Timeout=%d\n", Data, Timeout));

  if ((Data & 0x80) != 0x80) {
    DEBUG ((DEBUG_ERROR, "PCIe link not in RC mode (status=%x)\n", Data));
    return EFI_UNSUPPORTED;
  }

  // Change class code of the root port
  RMWRegister(BRCM_PCIE_CLASS, PCIE_RC_CFG_PRIV1_ID_VAL3_CLASS_CODE_MASK, 0x60400);

  //
  // PCIe->SCB endian mode for BAR
  // field ENDIAN_MODE_BAR2 = little endian = 0
  //
  RMWRegister (PCIE_RC_CFG_VENDOR_VENDOR_SPECIFIC_REG1,
    PCIE_RC_CFG_VENDOR_VENDOR_SPECIFIC_REG1_ENDIAN_MODE_BAR2_MASK, 0);

  RMWRegister (PCIE_MISC_HARD_PCIE_HARD_DEBUG,
    PCIE_MISC_HARD_PCIE_HARD_DEBUG_CLKREQ_DEBUG_ENABLE_MASK, 1);

  return EFI_SUCCESS;
}
