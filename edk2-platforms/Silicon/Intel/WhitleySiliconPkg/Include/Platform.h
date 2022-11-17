/** @file
  Platform specific information

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <BackCompatible.h>
#include <Uefi.h>
#include <Library/PcdLib.h>

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

//
// Assigning default ID and base addresses here, these definitions are used by ACPI tables
//

#define PCH_IOAPIC                      (UINT64)BIT0
#define PCH_IOAPIC_ID                   0x08
#define PCH_IOAPIC_ADDRESS              0xFEC00000 // This must get range from Legacy IIO
#define PCH_INTERRUPT_BASE              0

#define PC00_IOAPIC                     (UINT64)BIT1  //Because PCH_IOAPIC gets the first bit, these bit values will be 1+PC number.
#define PC00_IOAPIC_ID                  0x09
#define PC00_INTERRUPT_BASE             24

#define PC01_IOAPIC                     (UINT64)BIT2
#define PC01_IOAPIC_ID                  0x0A
#define PC01_INTERRUPT_BASE             32

#define PC02_IOAPIC                     (UINT64)BIT3
#define PC02_IOAPIC_ID                  0x0B
#define PC02_INTERRUPT_BASE             40

#define PC03_IOAPIC                     (UINT64)BIT4
#define PC03_IOAPIC_ID                  0x0C
#define PC03_INTERRUPT_BASE             48

#define PC04_IOAPIC                     (UINT64)BIT5
#define PC04_IOAPIC_ID                  0x0D
#define PC04_INTERRUPT_BASE             56

#define PC05_IOAPIC                     (UINT64)BIT6
#define PC05_IOAPIC_ID                  0x0E
#define PC05_INTERRUPT_BASE             64

#define PC06_IOAPIC                     (UINT64)BIT7
#define PC06_IOAPIC_ID                  0x0F
#define PC06_INTERRUPT_BASE             72

#define PC07_IOAPIC                     (UINT64)BIT8
#define PC07_IOAPIC_ID                  0x10
#define PC07_INTERRUPT_BASE             80

#define PC08_IOAPIC                     (UINT64)BIT9
#define PC08_IOAPIC_ID                  0x11
#define PC08_INTERRUPT_BASE             88

#define PC09_IOAPIC                     (UINT64)BIT10
#define PC09_IOAPIC_ID                  0x12
#define PC09_INTERRUPT_BASE             96

#define PC10_IOAPIC                     (UINT64)BIT11
#define PC10_IOAPIC_ID                  0x13
#define PC10_INTERRUPT_BASE             104

#define PC11_IOAPIC                     (UINT64)BIT12
#define PC11_IOAPIC_ID                  0x14
#define PC11_INTERRUPT_BASE             112

#define PC12_IOAPIC                     (UINT64)BIT13
#define PC12_IOAPIC_ID                  0x15
#define PC12_INTERRUPT_BASE             120

#define PC13_IOAPIC                     (UINT64)BIT14
#define PC13_IOAPIC_ID                  0x16
#define PC13_INTERRUPT_BASE             128

#define PC14_IOAPIC                     (UINT64)BIT15
#define PC14_IOAPIC_ID                  0x17
#define PC14_INTERRUPT_BASE             136

#define PC15_IOAPIC                     (UINT64)BIT16
#define PC15_IOAPIC_ID                  0x18
#define PC15_INTERRUPT_BASE             144

#define PC16_IOAPIC                     (UINT64)BIT17
#define PC16_IOAPIC_ID                  0x19
#define PC16_INTERRUPT_BASE             152

#define PC17_IOAPIC                     (UINT64)BIT18
#define PC17_IOAPIC_ID                  0x1A
#define PC17_INTERRUPT_BASE             160

#define PC18_IOAPIC                     (UINT64)BIT19
#define PC18_IOAPIC_ID                  0x1B
#define PC18_INTERRUPT_BASE             168

#define PC19_IOAPIC                     (UINT64)BIT20
#define PC19_IOAPIC_ID                  0x1C
#define PC19_INTERRUPT_BASE             176

#define PC20_IOAPIC                     (UINT64)BIT21
#define PC20_IOAPIC_ID                  0x1D
#define PC20_INTERRUPT_BASE             184

#define PC21_IOAPIC                     (UINT64)BIT22
#define PC21_IOAPIC_ID                  0x1E
#define PC21_INTERRUPT_BASE             192

#define PC22_IOAPIC                     (UINT64)BIT23
#define PC22_IOAPIC_ID                  0x1F
#define PC22_INTERRUPT_BASE             200

#define PC23_IOAPIC                     (UINT64)BIT24
#define PC23_IOAPIC_ID                  0x20
#define PC23_INTERRUPT_BASE             208

#define PC24_IOAPIC                     (UINT64)BIT25
#define PC24_IOAPIC_ID                  0x21
#define PC24_INTERRUPT_BASE             216

#define PC25_IOAPIC                     (UINT64)BIT26
#define PC25_IOAPIC_ID                  0x22
#define PC25_INTERRUPT_BASE             224

#define PC26_IOAPIC                     (UINT64)BIT27
#define PC26_IOAPIC_ID                  0x23
#define PC26_INTERRUPT_BASE             232

#define PC27_IOAPIC                     (UINT64)BIT28
#define PC27_IOAPIC_ID                  0x24
#define PC27_INTERRUPT_BASE             240

#define PC28_IOAPIC                     (UINT64)BIT29
#define PC28_IOAPIC_ID                  0x25
#define PC28_INTERRUPT_BASE             248

#define PC29_IOAPIC                     (UINT64)BIT30
#define PC29_IOAPIC_ID                  0x26
#define PC29_INTERRUPT_BASE             256

#define PC30_IOAPIC                     (UINT64)BIT31
#define PC30_IOAPIC_ID                  0x27
#define PC30_INTERRUPT_BASE             264

#define PC31_IOAPIC                     (UINT64)BIT32
#define PC31_IOAPIC_ID                  0x28
#define PC31_INTERRUPT_BASE             272

#define PC32_IOAPIC                     (UINT64)BIT33
#define PC32_IOAPIC_ID                  0x29
#define PC32_INTERRUPT_BASE             280

#define PC33_IOAPIC                     (UINT64)BIT34
#define PC33_IOAPIC_ID                  0x2A
#define PC33_INTERRUPT_BASE             288

#define PC34_IOAPIC                     (UINT64)BIT35
#define PC34_IOAPIC_ID                  0x2B
#define PC34_INTERRUPT_BASE             296

#define PC35_IOAPIC                     (UINT64)BIT36
#define PC35_IOAPIC_ID                  0x2C
#define PC35_INTERRUPT_BASE             304

#define PC36_IOAPIC                     (UINT64)BIT37
#define PC36_IOAPIC_ID                  0x2D
#define PC36_INTERRUPT_BASE             312

#define PC37_IOAPIC                     (UINT64)BIT38
#define PC37_IOAPIC_ID                  0x2E
#define PC37_INTERRUPT_BASE             320

#define PC38_IOAPIC                     (UINT64)BIT39
#define PC38_IOAPIC_ID                  0x2F
#define PC38_INTERRUPT_BASE             328

#define PC39_IOAPIC                     (UINT64)BIT40
#define PC39_IOAPIC_ID                  0x30
#define PC39_INTERRUPT_BASE             336

#define PC40_IOAPIC                     (UINT64)BIT41
#define PC40_IOAPIC_ID                  0x31
#define PC40_INTERRUPT_BASE             344

#define PC41_IOAPIC                     (UINT64)BIT42
#define PC41_IOAPIC_ID                  0x32
#define PC41_INTERRUPT_BASE             352

#define PC42_IOAPIC                     (UINT64)BIT43
#define PC42_IOAPIC_ID                  0x33
#define PC42_INTERRUPT_BASE             360

#define PC43_IOAPIC                     (UINT64)BIT44
#define PC43_IOAPIC_ID                  0x34
#define PC43_INTERRUPT_BASE             368

#define PC44_IOAPIC                     (UINT64)BIT45
#define PC44_IOAPIC_ID                  0x35
#define PC44_INTERRUPT_BASE             376

#define PC45_IOAPIC                     (UINT64)BIT46
#define PC45_IOAPIC_ID                  0x36
#define PC45_INTERRUPT_BASE             384

#define PC46_IOAPIC                     (UINT64)BIT47
#define PC46_IOAPIC_ID                  0x37
#define PC46_INTERRUPT_BASE             392

#define PC47_IOAPIC                     (UINT64)BIT48
#define PC47_IOAPIC_ID                  0x38
#define PC47_INTERRUPT_BASE             400

//
// Define platform base
// Note: All the PCH devices must get Legacy IO resources within first 16KB
// since KTI RC allcoates range 0-16KB for the legacy IIO.
//
#define PCH_ACPI_BASE_ADDRESS           PcdGet16 (PcdAcpiBaseAddress)      // ACPI Power Management I/O Register Base Address

#define PCH_TCO_BASE_ADDRESS            PcdGet16 (PcdTcoBaseAddress)

#define SIO_GPIO_BASE_ADDRESS           0x0800

//
// SMBUS Data
//
#define PCH_SMBUS_BASE_ADDRESS          0x0780

//
// CMOS usage
//

// Second bank
//
#define CMOS_PLATFORM_ID_LO             0x18            // Second bank CMOS location of Platform ID
#define CMOS_PLATFORM_ID_HI             0x19            //

#define PCI_BUS_NUMBER_PCH_HPET                       0x0
#define PCI_DEVICE_NUMBER_PCH_HPET                    0x1F

#define PCI_FUNCTION_NUMBER_PCH_HPET0                 0x00

#define PCI_BUS_NUMBER_PCH_IOAPIC                     0x00
#define PCI_DEVICE_NUMBER_PCH_IOAPIC                  0x1E

#define PCI_FUNCTION_NUMBER_PCH_IOAPIC                0x0

//
// AHCI port offset values
//
#define EFI_AHCI_PORT_START                           0x0100
#define EFI_AHCI_PORT_REG_WIDTH                       0x0080
#define EFI_AHCI_PORT_CLB                             0x0000
#define EFI_AHCI_PORT_CLBU                            0x0004
#define EFI_AHCI_PORT_FB                              0x0008
#define EFI_AHCI_PORT_FBU                             0x000C
#define EFI_AHCI_PORT_IS                              0x0010
#define EFI_AHCI_PORT_IE                              0x0014
#define EFI_AHCI_PORT_CMD                             0x0018

#endif
