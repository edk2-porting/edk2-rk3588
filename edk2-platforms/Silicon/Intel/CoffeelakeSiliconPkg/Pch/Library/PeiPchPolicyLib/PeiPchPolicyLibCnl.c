/** @file
  This file is PeiPchPolicy library Cannon Lake specific.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiPchPolicyLibrary.h"
#include <Library/PchPcieRpLib.h>
#include <Library/CpuPlatformLib.h>

/**
  mDevIntConfig[] table contains data on INTx and IRQ for each device.
  IRQ value for devices which use ITSS INTx->PIRQx mapping need to be set in a way
  that for each multifunctional Dxx:Fy same interrupt pins must map to the same IRQ.
  Those IRQ values will be used to update ITSS.PIRx register.
  In APIC relationship between PIRQs and IRQs is:
  PIRQA -> IRQ16
  PIRQB -> IRQ17
  PIRQC -> IRQ18
  PIRQD -> IRQ19
  PIRQE -> IRQ20
  PIRQF -> IRQ21
  PIRQG -> IRQ22
  PIRQH -> IRQ23

  Devices which use INTx->PIRQy mapping are: cAVS(in PCI mode), SMBus, GbE, TraceHub, PCIe,
  SATA, HECI, IDE-R, KT Redirection, xHCI, Thermal Subsystem, Camera IO Host Controller

  PCI Express Root Ports mapping should be programmed only with values as in below table (D27/28/29)
  otherwise _PRT methods in ACPI for RootPorts would require additional patching as
  PCIe Endpoint Device Interrupt is further subjected to INTx to PIRQy Mapping

  Configured IRQ values are not used if an OS chooses to be in PIC instead of APIC mode
**/
GLOBAL_REMOVE_IF_UNREFERENCED PCH_DEVICE_INTERRUPT_CONFIG mDevIntConfig[] = {
//  {31, 0, PchNoInt, 0}, // LPC/eSPI Interface, doesn't use interrupts
//  {31, 1, PchNoInt, 0}, // P2SB, doesn't use interrupts
//  {31, 2, PchNoInt, 0}, // PMC , doesn't use interrupts
  {31, 3, PchIntA, 16}, // cAVS(Audio, Voice, Speach), INTA is default, programmed in PciCfgSpace 3Dh
  {31, 4, PchIntA, 16}, // SMBus Controller, no default value, programmed in PciCfgSpace 3Dh
//  {31, 5, PchNoInt, 0}, // SPI , doesn't use interrupts
  {31, 7, PchIntA, 16}, // TraceHub, INTA is default, RO register
  {30, 0, PchIntA, 20}, // SerialIo: UART #0, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[7]
  {30, 1, PchIntB, 21}, // SerialIo: UART #1, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[8]
  {30, 2, PchIntC, 22}, // SerialIo: SPI #0, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[10]
  {30, 3, PchIntD, 23}, // SerialIo: SPI #1, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[11]
  {28, 0, PchIntA, 16}, // PCI Express Port 1, INT is default, programmed in PciCfgSpace + FCh
  {28, 1, PchIntB, 17}, // PCI Express Port 2, INT is default, programmed in PciCfgSpace + FCh
  {28, 2, PchIntC, 18}, // PCI Express Port 3, INT is default, programmed in PciCfgSpace + FCh
  {28, 3, PchIntD, 19}, // PCI Express Port 4, INT is default, programmed in PciCfgSpace + FCh
  {28, 4, PchIntA, 16}, // PCI Express Port 5, INT is default, programmed in PciCfgSpace + FCh
  {28, 5, PchIntB, 17}, // PCI Express Port 6, INT is default, programmed in PciCfgSpace + FCh
  {28, 6, PchIntC, 18}, // PCI Express Port 7, INT is default, programmed in PciCfgSpace + FCh
  {28, 7, PchIntD, 19}, // PCI Express Port 8, INT is default, programmed in PciCfgSpace + FCh
  {25, 2, PchIntC, 34}, // SerialIo UART Controller #2, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[9]
//  {24, 0, 0, 0}, // Reserved (used by RST PCIe Storage Cycle Router)
  {23, 0, PchIntA, 16}, // SATA Controller, INTA is default, programmed in PciCfgSpace + 3Dh
  {22, 0, PchIntA, 16}, // CSME: HECI #1
  {22, 1, PchIntB, 17}, // CSME: HECI #2
  {22, 4, PchIntA, 16}, // CSME: HECI #3
//  {22, 7, PchNoInt, 0}, // CSME: WLAN
  {21, 0, PchIntA, 16}, // SerialIo I2C Controller #0, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[1]
  {21, 1, PchIntB, 17}, // SerialIo I2C Controller #1, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[2]
  {21, 2, PchIntC, 18}, // SerialIo I2C Controller #2, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[3]
  {21, 3, PchIntD, 19}, // SerialIo I2C Controller #3, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[4]
  {20, 0, PchIntA, 16}, // USB 3.0 xHCI Controller, no default value, programmed in PciCfgSpace 3Dh
  {20, 1, PchIntB, 17}, // USB Device Controller (OTG)
  //{20, 2, PchNoInt, 0}, // Shared SRAM, no interrupts
  {20, 3, PchIntA, 16}, // CNVi WiFir
//  {20, 4, 0, 0}, // TraceHub Phantom (ACPI) Function
  {20, 5, PchIntD, 19}, // SCS: SDCard
//  {18, 0, PchNoInt, 0}, // CSME: KVMcc,  doesn't use interrupts
//  {18, 1, PchNoInt, 0}, // CSME: Clink,  doesn't use interrupts
//  {18, 2, PchNoInt, 0}, // CSME: PMT,  doesn't use interrupts
//  {18, 3, 0, 0}, // CSME: CSE UMA
//  {18, 4, 0, 0}  // CSME: fTPM DMA
  {18, 5, PchIntA, 16}  // SCS: UFS
};

//
// mCnlPchLpOnlyDevIntConfig[] table contains data on INTx and IRQ for devices that exist on PCH-LP
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_DEVICE_INTERRUPT_CONFIG mPchLpOnlyDevIntConfig[] = {
  {31, 6, PchIntA, 16}, // GbE Controller, INTA is default, programmed in PciCfgSpace 3Dh
  {29, 0, PchIntA, 16}, // PCI Express Port 9, INT is default, programmed in PciCfgSpace + FCh
  {29, 1, PchIntB, 17}, // PCI Express Port 10, INT is default, programmed in PciCfgSpace + FCh
  {29, 2, PchIntC, 18}, // PCI Express Port 11, INT is default, programmed in PciCfgSpace + FCh
  {29, 3, PchIntD, 19}, // PCI Express Port 12, INT is default, programmed in PciCfgSpace + FCh
  {29, 4, PchIntA, 16}, // PCI Express Port 13, INT is default, programmed in PciCfgSpace + FCh
  {29, 5, PchIntB, 17}, // PCI Express Port 14, INT is default, programmed in PciCfgSpace + FCh
  {29, 6, PchIntC, 18}, // PCI Express Port 15, INT is default, programmed in PciCfgSpace + FCh
  {29, 7, PchIntD, 19}, // PCI Express Port 16, INT is default, programmed in PciCfgSpace + FCh
  {26, 0, PchIntA, 16}, // SCS: eMMC
  {25, 0, PchIntA, 32}, // SerialIo I2C Controller #4, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[5]
  {25, 1, PchIntB, 33}, // SerialIo I2C Controller #5, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[6]
  {22, 2, PchIntC, 18}, // CSME: IDE-Redirection (IDE-R)
  {22, 3, PchIntD, 19}, // CSME: Keyboard and Text (KT) Redirection
  {19, 0, PchIntA, 20}, // Integrated Sensor Hub
  {18, 0, PchIntA, 16}, // Thermal Subsystem
  {18, 6, PchIntB, 24}  // SerialIo: SPI #2, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[12]
};

//
// mPchHOnlyDevIntConfig[] table contains data on INTx and IRQ for devices that exist on PCH-H
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_DEVICE_INTERRUPT_CONFIG mPchHOnlyDevIntConfig[] = {
  {31, 6, PchIntA, 16}, // GbE Controller, INTA is default, programmed in PciCfgSpace 3Dh
  {29, 0, PchIntA, 16}, // PCI Express Port 9, INT is default, programmed in PciCfgSpace + FCh
  {29, 1, PchIntB, 17}, // PCI Express Port 10, INT is default, programmed in PciCfgSpace + FCh
  {29, 2, PchIntC, 18}, // PCI Express Port 11, INT is default, programmed in PciCfgSpace + FCh
  {29, 3, PchIntD, 19}, // PCI Express Port 12, INT is default, programmed in PciCfgSpace + FCh
  {29, 4, PchIntA, 16}, // PCI Express Port 13, INT is default, programmed in PciCfgSpace + FCh
  {29, 5, PchIntB, 17}, // PCI Express Port 14, INT is default, programmed in PciCfgSpace + FCh
  {29, 6, PchIntC, 18}, // PCI Express Port 15, INT is default, programmed in PciCfgSpace + FCh
  {29, 7, PchIntD, 19}, // PCI Express Port 16, INT is default, programmed in PciCfgSpace + FCh
  {27, 0, PchIntA, 16}, // PCI Express Port 17, INT is default, programmed in PciCfgSpace + FCh
  {27, 1, PchIntB, 17}, // PCI Express Port 18, INT is default, programmed in PciCfgSpace + FCh
  {27, 2, PchIntC, 18}, // PCI Express Port 19, INT is default, programmed in PciCfgSpace + FCh
  {27, 3, PchIntD, 19}, // PCI Express Port 20, INT is default, programmed in PciCfgSpace + FCh
  {27, 4, PchIntA, 16}, // PCI Express Port 21
  {27, 5, PchIntB, 17}, // PCI Express Port 22
  {27, 6, PchIntC, 18}, // PCI Express Port 23
  {27, 7, PchIntD, 19}, // PCI Express Port 24
  {22, 2, PchIntC, 18}, // CSME: IDE-Redirection (IDE-R)
  {22, 3, PchIntD, 19}, // CSME: Keyboard and Text (KT) Redirection
  {19, 0, PchIntA, 20}, // Integrated Sensor Hub
  {18, 0, PchIntA, 16}, // Thermal Subsystem
  {18, 6, PchIntB, 24}  // SerialIo: SPI #2, INTA is default, programmed in PCR[SERIALIO] + PCICFGCTRL[12]
};

/**
  Adds interrupt configuration for device

  @param[in/out] InterruptConfig         Pointer to interrupt config
**/
VOID
LoadDeviceInterruptConfig (
  IN OUT  PCH_INTERRUPT_CONFIG  *InterruptConfig
  )
{
  UINT8                 IntConfigTableEntries;

  IntConfigTableEntries = ARRAY_SIZE (mDevIntConfig);
  ASSERT (IntConfigTableEntries <= PCH_MAX_DEVICE_INTERRUPT_CONFIG);
  InterruptConfig->NumOfDevIntConfig = IntConfigTableEntries;
  CopyMem (
    InterruptConfig->DevIntConfig,
    mDevIntConfig,
    sizeof (mDevIntConfig)
    );

  if (IsPchLp ()) {
    CopyMem (
      &(InterruptConfig->DevIntConfig[InterruptConfig->NumOfDevIntConfig]),
      mPchLpOnlyDevIntConfig,
      sizeof (mPchLpOnlyDevIntConfig)
      );
    InterruptConfig->NumOfDevIntConfig += ARRAY_SIZE (mPchLpOnlyDevIntConfig);
  } else if (IsPchH ()) {
    CopyMem (
      &(InterruptConfig->DevIntConfig[InterruptConfig->NumOfDevIntConfig]),
      mPchHOnlyDevIntConfig,
      sizeof (mPchHOnlyDevIntConfig)
      );
    InterruptConfig->NumOfDevIntConfig += ARRAY_SIZE (mPchHOnlyDevIntConfig);
  }
}
