/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <IndustryStandard/Acpi.h>

VOID
DumpCharArray (
  IN CHAR8  *Ch,
  IN UINTN  Size
  );

VOID
DumpAcpiTableHeader (
  IN EFI_ACPI_DESCRIPTION_HEADER  *Table
  );

BOOLEAN
IsMmioExit (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length,
  IN BOOLEAN               CheckAllocated
  );

typedef struct {
  UINT8                 Type;
  UINT8                 Length;
} APIC_STRUCT_HEADER;

CHAR8 *mMadtTypeString[] = {
  "APIC      ",
  "IO_APIC   ",
  "INT_SRC_OR",
  "NNI_SRC   ",
  "APIC_NMI  ",
  "APIC_OR   ",
  "IO_SAPIC  ",
  "SAPIC     ",
  "PL_INT_SRC",
  "X2APIC    ",
  "X2APIC_NMI",
};

STATIC CHAR8 mUnknownStr[11];

CHAR8 *
ShortNameOfMadtType(
  IN UINT8 Type
  )
{
  if (Type < sizeof(mMadtTypeString) / sizeof(mMadtTypeString[0])) {
    return mMadtTypeString[Type];
  } else {
    AsciiSPrint(mUnknownStr, sizeof(mUnknownStr), "[%02x]      ", Type);
    return mUnknownStr;
  }
}

VOID
DumpAcpiMadt (
  IN EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER  *Madt
  )
{
  APIC_STRUCT_HEADER                                    *ApicStructHeader;
  INTN                                                  MadtLen;
  EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC_STRUCTURE           *ProcessorLocalApic;
  EFI_ACPI_4_0_IO_APIC_STRUCTURE                        *IOApic;
  EFI_ACPI_4_0_INTERRUPT_SOURCE_OVERRIDE_STRUCTURE      *InterruptSourceOverride;
  EFI_ACPI_4_0_NON_MASKABLE_INTERRUPT_SOURCE_STRUCTURE  *NonMaskableInterruptSource;
  EFI_ACPI_4_0_LOCAL_APIC_NMI_STRUCTURE                 *LocalApicNMI;
  EFI_ACPI_4_0_LOCAL_APIC_ADDRESS_OVERRIDE_STRUCTURE    *LocalApicAddressOverride;
  EFI_ACPI_4_0_IO_SAPIC_STRUCTURE                       *IOSapic;
  EFI_ACPI_4_0_PROCESSOR_LOCAL_SAPIC_STRUCTURE          *ProcessorLocalSapic;
  EFI_ACPI_4_0_PLATFORM_INTERRUPT_SOURCES_STRUCTURE     *PlatformInterruptSource;
  EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC_STRUCTURE         *ProcessorLocalX2Apic;
  EFI_ACPI_4_0_LOCAL_X2APIC_NMI_STRUCTURE               *LocalX2ApicNmi;

  DumpAcpiTableHeader (&Madt->Header);
  DEBUG ((DEBUG_INFO, "         "));
  DEBUG ((DEBUG_INFO, " LocalApicAddress=0x%08x\n", Madt->LocalApicAddress));

  //
  // Sub table
  //
  MadtLen  = Madt->Header.Length - sizeof(EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER);
  ApicStructHeader = (APIC_STRUCT_HEADER *)(Madt + 1);
  while (MadtLen > 0) {
    switch (ApicStructHeader->Type) {
    case EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC:
      ProcessorLocalApic = (EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC_STRUCTURE *)ApicStructHeader;
      DEBUG ((DEBUG_INFO, "         "));
      DEBUG ((DEBUG_INFO, " "));
      DEBUG ((DEBUG_INFO, ShortNameOfMadtType(ApicStructHeader->Type)));
      DEBUG ((DEBUG_INFO, ": [0x%02x]", ApicStructHeader->Type));
      DEBUG ((DEBUG_INFO, " ID=0x%02x", ProcessorLocalApic->AcpiProcessorId));
      DEBUG ((DEBUG_INFO, " ApicId=0x%02x", ProcessorLocalApic->ApicId));
      if ((ProcessorLocalApic->Flags & EFI_ACPI_4_0_LOCAL_APIC_ENABLED) != 0) {
        DEBUG ((DEBUG_INFO, " (Enabled)"));
      }
      DEBUG ((DEBUG_INFO, "\n"));
      break;
    case EFI_ACPI_4_0_IO_APIC:
      IOApic = (EFI_ACPI_4_0_IO_APIC_STRUCTURE *)ApicStructHeader;
      DEBUG ((DEBUG_INFO, "         "));
      DEBUG ((DEBUG_INFO, " "));
      DEBUG ((DEBUG_INFO, ShortNameOfMadtType(ApicStructHeader->Type)));
      DEBUG ((DEBUG_INFO, ": [0x%02x]", ApicStructHeader->Type));
      DEBUG ((DEBUG_INFO, " IoApicId=0x%02x", IOApic->IoApicId));
      DEBUG ((DEBUG_INFO, " Address=0x%08x", IOApic->IoApicAddress));
      DEBUG ((DEBUG_INFO, " InterruptBase=0x%08x", IOApic->GlobalSystemInterruptBase));
      DEBUG ((DEBUG_INFO, "\n"));
      break;
    case EFI_ACPI_4_0_INTERRUPT_SOURCE_OVERRIDE:
      InterruptSourceOverride = (EFI_ACPI_4_0_INTERRUPT_SOURCE_OVERRIDE_STRUCTURE *)ApicStructHeader;
      DEBUG ((DEBUG_INFO, "         "));
      DEBUG ((DEBUG_INFO, " "));
      DEBUG ((DEBUG_INFO, ShortNameOfMadtType(ApicStructHeader->Type)));
      DEBUG ((DEBUG_INFO, ": [0x%02x]", ApicStructHeader->Type));
      DEBUG ((DEBUG_INFO, " Bus=0x%02x", InterruptSourceOverride->Bus));
      DEBUG ((DEBUG_INFO, " Source=0x%02x", InterruptSourceOverride->Source));
      DEBUG ((DEBUG_INFO, " Interrupt=0x%08x", InterruptSourceOverride->GlobalSystemInterrupt));
      DEBUG ((DEBUG_INFO, " Flags=0x%04x", InterruptSourceOverride->Flags));
      DEBUG ((DEBUG_INFO, "\n"));
      break;
    case EFI_ACPI_4_0_NON_MASKABLE_INTERRUPT_SOURCE:
      NonMaskableInterruptSource = (EFI_ACPI_4_0_NON_MASKABLE_INTERRUPT_SOURCE_STRUCTURE *)ApicStructHeader;
      DEBUG ((DEBUG_INFO, "         "));
      DEBUG ((DEBUG_INFO, " "));
      DEBUG ((DEBUG_INFO, ShortNameOfMadtType(ApicStructHeader->Type)));
      DEBUG ((DEBUG_INFO, ": [0x%02x]", ApicStructHeader->Type));
      DEBUG ((DEBUG_INFO, " Interrupt=0x%08x", NonMaskableInterruptSource->GlobalSystemInterrupt));
      DEBUG ((DEBUG_INFO, " Flags=0x%04x", NonMaskableInterruptSource->Flags));
      DEBUG ((DEBUG_INFO, "\n"));
      break;
    case EFI_ACPI_4_0_LOCAL_APIC_NMI:
      LocalApicNMI = (EFI_ACPI_4_0_LOCAL_APIC_NMI_STRUCTURE *)ApicStructHeader;
      DEBUG ((DEBUG_INFO, "         "));
      DEBUG ((DEBUG_INFO, " "));
      DEBUG ((DEBUG_INFO, ShortNameOfMadtType(ApicStructHeader->Type)));
      DEBUG ((DEBUG_INFO, ": [0x%02x]", ApicStructHeader->Type));
      DEBUG ((DEBUG_INFO, " ID=0x%02x", LocalApicNMI->AcpiProcessorId));
      DEBUG ((DEBUG_INFO, " Lint=0x%02x", LocalApicNMI->LocalApicLint));
      DEBUG ((DEBUG_INFO, " Flags=0x%04x", LocalApicNMI->Flags));
      DEBUG ((DEBUG_INFO, "\n"));
      break;
    case EFI_ACPI_4_0_LOCAL_APIC_ADDRESS_OVERRIDE:
      LocalApicAddressOverride = (EFI_ACPI_4_0_LOCAL_APIC_ADDRESS_OVERRIDE_STRUCTURE *)ApicStructHeader;
      DEBUG ((DEBUG_INFO, "         "));
      DEBUG ((DEBUG_INFO, " "));
      DEBUG ((DEBUG_INFO, ShortNameOfMadtType(ApicStructHeader->Type)));
      DEBUG ((DEBUG_INFO, ": [0x%02x]", ApicStructHeader->Type));
      DEBUG ((DEBUG_INFO, " LocalApicAddress=0x%016lx", LocalApicAddressOverride->LocalApicAddress));
      DEBUG ((DEBUG_INFO, "\n"));
      break;
    case EFI_ACPI_4_0_IO_SAPIC:
      IOSapic = (EFI_ACPI_4_0_IO_SAPIC_STRUCTURE *)ApicStructHeader;
      DEBUG ((DEBUG_INFO, "         "));
      DEBUG ((DEBUG_INFO, " "));
      DEBUG ((DEBUG_INFO, ShortNameOfMadtType(ApicStructHeader->Type)));
      DEBUG ((DEBUG_INFO, ": [0x%02x]", ApicStructHeader->Type));
      DEBUG ((DEBUG_INFO, " IoApicId=0x%02x", IOSapic->IoApicId));
      DEBUG ((DEBUG_INFO, " InterruptBase=0x%08x", IOSapic->GlobalSystemInterruptBase));
      DEBUG ((DEBUG_INFO, " IoSapicAddress=0x%016lx", IOSapic->IoSapicAddress));
      DEBUG ((DEBUG_INFO, "\n"));
      break;
    case EFI_ACPI_4_0_LOCAL_SAPIC:
      ProcessorLocalSapic = (EFI_ACPI_4_0_PROCESSOR_LOCAL_SAPIC_STRUCTURE *)ApicStructHeader;
      DEBUG ((DEBUG_INFO, "         "));
      DEBUG ((DEBUG_INFO, " "));
      DEBUG ((DEBUG_INFO, ShortNameOfMadtType(ApicStructHeader->Type)));
      DEBUG ((DEBUG_INFO, ": [0x%02x]", ApicStructHeader->Type));
      DEBUG ((DEBUG_INFO, " ID=0x%02x", ProcessorLocalSapic->AcpiProcessorId));
      DEBUG ((DEBUG_INFO, " LocalSapicId=0x%02x", ProcessorLocalSapic->LocalSapicId));
      DEBUG ((DEBUG_INFO, " LocalSapicEid=0x%02x", ProcessorLocalSapic->LocalSapicEid));
      DEBUG ((DEBUG_INFO, " UID=0x%08x", ProcessorLocalSapic->ACPIProcessorUIDValue));
      if ((ProcessorLocalSapic->Flags & EFI_ACPI_5_0_LOCAL_APIC_ENABLED) != 0) {
        DEBUG ((DEBUG_INFO, " (Enabled)"));
      }
      DEBUG ((DEBUG_INFO, "\n"));
      break;
    case EFI_ACPI_4_0_PLATFORM_INTERRUPT_SOURCES:
      PlatformInterruptSource = (EFI_ACPI_4_0_PLATFORM_INTERRUPT_SOURCES_STRUCTURE *)ApicStructHeader;
      DEBUG ((DEBUG_INFO, "         "));
      DEBUG ((DEBUG_INFO, " "));
      DEBUG ((DEBUG_INFO, ShortNameOfMadtType(ApicStructHeader->Type)));
      DEBUG ((DEBUG_INFO, ": [0x%02x]", ApicStructHeader->Type));
      DEBUG ((DEBUG_INFO, " Type=0x%02x", PlatformInterruptSource->InterruptType));
      DEBUG ((DEBUG_INFO, " ID=0x%02x", PlatformInterruptSource->ProcessorId));
      DEBUG ((DEBUG_INFO, " EID=0x%02x", PlatformInterruptSource->ProcessorEid));
      DEBUG ((DEBUG_INFO, " IoSapicVector=0x%02x", PlatformInterruptSource->IoSapicVector));
      DEBUG ((DEBUG_INFO, " Interrupt=0x%08x", PlatformInterruptSource->GlobalSystemInterrupt));
      DEBUG ((DEBUG_INFO, " SourceFlags=0x%08x", PlatformInterruptSource->PlatformInterruptSourceFlags));
      DEBUG ((DEBUG_INFO, " Flags=0x%04x", PlatformInterruptSource->Flags));
      DEBUG ((DEBUG_INFO, "\n"));
      break;
    case EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC:
      ProcessorLocalX2Apic = (EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC_STRUCTURE *)ApicStructHeader;
      DEBUG ((DEBUG_INFO, "         "));
      DEBUG ((DEBUG_INFO, " "));
      DEBUG ((DEBUG_INFO, ShortNameOfMadtType(ApicStructHeader->Type)));
      DEBUG ((DEBUG_INFO, ": [0x%02x]", ApicStructHeader->Type));
      DEBUG ((DEBUG_INFO, " X2ApicId=0x%08x", ProcessorLocalX2Apic->X2ApicId));
      DEBUG ((DEBUG_INFO, " UID=0x%08x", ProcessorLocalX2Apic->AcpiProcessorUid));
      if ((ProcessorLocalX2Apic->Flags & EFI_ACPI_5_0_LOCAL_APIC_ENABLED) != 0) {
        DEBUG ((DEBUG_INFO, " (Enabled)"));
      }
      DEBUG ((DEBUG_INFO, "\n"));
      break;
    case EFI_ACPI_4_0_LOCAL_X2APIC_NMI:
      LocalX2ApicNmi = (EFI_ACPI_4_0_LOCAL_X2APIC_NMI_STRUCTURE *)ApicStructHeader;
      DEBUG ((DEBUG_INFO, "         "));
      DEBUG ((DEBUG_INFO, " "));
      DEBUG ((DEBUG_INFO, ShortNameOfMadtType(ApicStructHeader->Type)));
      DEBUG ((DEBUG_INFO, ": [0x%02x]", ApicStructHeader->Type));
      DEBUG ((DEBUG_INFO, " UID=0x%08x", LocalX2ApicNmi->AcpiProcessorUid));
      DEBUG ((DEBUG_INFO, " Lint=0x%02x", LocalX2ApicNmi->LocalX2ApicLint));
      DEBUG ((DEBUG_INFO, " Flags=0x%04x", LocalX2ApicNmi->Flags));
      DEBUG ((DEBUG_INFO, "\n"));
      break;
    default:
      DEBUG ((DEBUG_INFO, "         "));
      DEBUG ((DEBUG_INFO, " "));
      DEBUG ((DEBUG_INFO, ShortNameOfMadtType(ApicStructHeader->Type)));
      DEBUG ((DEBUG_INFO, "\n"));
      break;
    }
    ApicStructHeader = (APIC_STRUCT_HEADER *)((UINT8 *)ApicStructHeader + ApicStructHeader->Length);
    MadtLen         -= ApicStructHeader->Length;
  }
}

EFI_STATUS
CheckAcpiMadt (
  IN EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER  *Madt
  )
{

  APIC_STRUCT_HEADER                                    *ApicStructHeader;
  INTN                                                  MadtLen;
  EFI_ACPI_4_0_IO_APIC_STRUCTURE                        *IOApic;
  EFI_ACPI_4_0_LOCAL_APIC_ADDRESS_OVERRIDE_STRUCTURE    *LocalApicAddressOverride;
  EFI_ACPI_4_0_IO_SAPIC_STRUCTURE                       *IOSapic;
#if 0
  EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC_STRUCTURE           *ProcessorLocalApic;
  EFI_ACPI_4_0_INTERRUPT_SOURCE_OVERRIDE_STRUCTURE      *InterruptSourceOverride;
  EFI_ACPI_4_0_NON_MASKABLE_INTERRUPT_SOURCE_STRUCTURE  *NonMaskableInterruptSource;
  EFI_ACPI_4_0_LOCAL_APIC_NMI_STRUCTURE                 *LocalApicNMI;
  EFI_ACPI_4_0_PROCESSOR_LOCAL_SAPIC_STRUCTURE          *ProcessorLocalSapic;
  EFI_ACPI_4_0_PLATFORM_INTERRUPT_SOURCES_STRUCTURE     *PlatformInterruptSource;
  EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC_STRUCTURE         *ProcessorLocalX2Apic;
  EFI_ACPI_4_0_LOCAL_X2APIC_NMI_STRUCTURE               *LocalX2ApicNmi;
#endif

  if (!IsMmioExit (Madt->LocalApicAddress, SIZE_4KB, TRUE)) {
    DEBUG ((DEBUG_ERROR, "MADT resource (0x%x) is not reported correctly.\n", Madt->LocalApicAddress));
    return EFI_NOT_STARTED;
  }

  //
  // Sub table
  //
  MadtLen  = Madt->Header.Length - sizeof(EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER);
  ApicStructHeader = (APIC_STRUCT_HEADER *)(Madt + 1);
  while (MadtLen > 0) {
    switch (ApicStructHeader->Type) {
    case EFI_ACPI_4_0_IO_APIC:
      IOApic = (EFI_ACPI_4_0_IO_APIC_STRUCTURE *)ApicStructHeader;
      if (!IsMmioExit (IOApic->IoApicAddress, SIZE_4KB, TRUE)) {
        DEBUG ((DEBUG_ERROR, "MADT.IOAPIC resource (0x%x) is not reported correctly.\n", IOApic->IoApicAddress));
        return EFI_NOT_STARTED;
      }
      break;
    case EFI_ACPI_4_0_LOCAL_APIC_ADDRESS_OVERRIDE:
      LocalApicAddressOverride = (EFI_ACPI_4_0_LOCAL_APIC_ADDRESS_OVERRIDE_STRUCTURE *)ApicStructHeader;
      if (!IsMmioExit (LocalApicAddressOverride->LocalApicAddress, SIZE_4KB, TRUE)) {
        DEBUG ((DEBUG_ERROR, "MADT.LocalApicOverride resource (0x%x) is not reported correctly.\n", LocalApicAddressOverride->LocalApicAddress));
        return EFI_NOT_STARTED;
      }
      break;
    case EFI_ACPI_4_0_IO_SAPIC:
      IOSapic = (EFI_ACPI_4_0_IO_SAPIC_STRUCTURE *)ApicStructHeader;
      if (!IsMmioExit (IOSapic->IoSapicAddress, SIZE_4KB, TRUE)) {
        DEBUG ((DEBUG_ERROR, "MADT.IOSAPIC resource (0x%x) is not reported correctly.\n", IOSapic->IoSapicAddress));
        return EFI_NOT_STARTED;
      }
      break;
#if 0
    case EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC:
      ProcessorLocalApic = (EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC_STRUCTURE *)ApicStructHeader;
      break;
    case EFI_ACPI_4_0_INTERRUPT_SOURCE_OVERRIDE:
      InterruptSourceOverride = (EFI_ACPI_4_0_INTERRUPT_SOURCE_OVERRIDE_STRUCTURE *)ApicStructHeader;
      break;
    case EFI_ACPI_4_0_NON_MASKABLE_INTERRUPT_SOURCE:
      NonMaskableInterruptSource = (EFI_ACPI_4_0_NON_MASKABLE_INTERRUPT_SOURCE_STRUCTURE *)ApicStructHeader;
      break;
    case EFI_ACPI_4_0_LOCAL_APIC_NMI:
      LocalApicNMI = (EFI_ACPI_4_0_LOCAL_APIC_NMI_STRUCTURE *)ApicStructHeader;
      break;
    case EFI_ACPI_4_0_LOCAL_SAPIC:
      ProcessorLocalSapic = (EFI_ACPI_4_0_PROCESSOR_LOCAL_SAPIC_STRUCTURE *)ApicStructHeader;
      break;
    case EFI_ACPI_4_0_PLATFORM_INTERRUPT_SOURCES:
      PlatformInterruptSource = (EFI_ACPI_4_0_PLATFORM_INTERRUPT_SOURCES_STRUCTURE *)ApicStructHeader;
      break;
    case EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC:
      ProcessorLocalX2Apic = (EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC_STRUCTURE *)ApicStructHeader;
      break;
    case EFI_ACPI_4_0_LOCAL_X2APIC_NMI:
      LocalX2ApicNmi = (EFI_ACPI_4_0_LOCAL_X2APIC_NMI_STRUCTURE *)ApicStructHeader;
      break;
#endif
    default:
      break;
    }
    ApicStructHeader = (APIC_STRUCT_HEADER *)((UINT8 *)ApicStructHeader + ApicStructHeader->Length);
    MadtLen         -= ApicStructHeader->Length;
  }
  return EFI_SUCCESS;
}