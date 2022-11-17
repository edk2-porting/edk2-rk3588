/** @file

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/MemoryMappedConfigurationSpaceAccessTable.h>
#include <IndustryStandard/HighPrecisionEventTimerTable.h>
#include <IndustryStandard/DmaRemappingReportingTable.h>
#include <IndustryStandard/WindowsSmmSecurityMitigationTable.h>
#include <IndustryStandard/TcpaAcpi.h>
#include <IndustryStandard/Tpm2Acpi.h>
#include <Guid/Acpi.h>

EFI_GCD_MEMORY_SPACE_DESCRIPTOR                     *mAcpiGcdMemoryMap;
EFI_GCD_IO_SPACE_DESCRIPTOR                         *mAcpiGcdIoMap;
UINTN                                               mAcpiGcdMemoryMapNumberOfDescriptors;
UINTN                                               mAcpiGcdIoMapNumberOfDescriptors;

VOID
DumpAcpiMadt (
  IN EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER  *Madt
  );

VOID
DumpAcpiMcfg (
  IN EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER  *Mcfg
  );

VOID
DumpAcpiHpet (
  IN EFI_ACPI_HIGH_PRECISION_EVENT_TIMER_TABLE_HEADER  *Hpet
  );

VOID
DumpAcpiDmar (
  IN EFI_ACPI_DMAR_HEADER  *Dmar
  );

VOID
DumpAcpiWsmt (
  IN EFI_ACPI_WSMT_TABLE  *Wsmt
  );

VOID
DumpAcpiTpm2 (
  IN EFI_TPM2_ACPI_TABLE  *Tpm2
  );

VOID
DumpAcpiTcpa (
  IN EFI_ACPI_DESCRIPTION_HEADER  *Tcpa
  );

EFI_STATUS
CheckAcpiMadt (
  IN EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER  *Madt
  );

EFI_STATUS
CheckAcpiMcfg (
  IN EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER  *Mcfg
  );

EFI_STATUS
CheckAcpiHpet (
  IN EFI_ACPI_HIGH_PRECISION_EVENT_TIMER_TABLE_HEADER  *Hpet
  );

EFI_STATUS
CheckAcpiDmar (
  IN EFI_ACPI_DMAR_HEADER  *Dmar
  );

EFI_STATUS
CheckAcpiTpm2 (
  IN EFI_TPM2_ACPI_TABLE  *Tpm2
  );

EFI_STATUS
CheckAcpiTcpa (
  IN EFI_ACPI_DESCRIPTION_HEADER  *Tcpa
  );

VOID
TestPointDumpGcd (
  OUT EFI_GCD_MEMORY_SPACE_DESCRIPTOR **GcdMemoryMap,  OPTIONAL
  OUT UINTN                           *GcdMemoryMapNumberOfDescriptors,  OPTIONAL
  OUT EFI_GCD_IO_SPACE_DESCRIPTOR     **GcdIoMap,  OPTIONAL
  OUT UINTN                           *GcdIoMapNumberOfDescriptors,  OPTIONAL
  IN  BOOLEAN                         DumpPrint
  );

BOOLEAN
IsMmioExit (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length,
  IN BOOLEAN               CheckAllocated
  )
{
  UINTN  Index;
  for (Index = 0; Index < mAcpiGcdMemoryMapNumberOfDescriptors; Index++) {
    if (mAcpiGcdMemoryMap[Index].GcdMemoryType != EfiGcdMemoryTypeMemoryMappedIo) {
      continue;
    }
    if ((BaseAddress >= mAcpiGcdMemoryMap[Index].BaseAddress) &&
        (BaseAddress + Length <= mAcpiGcdMemoryMap[Index].BaseAddress + mAcpiGcdMemoryMap[Index].Length)) {
      if (CheckAllocated) {
        if (mAcpiGcdMemoryMap[Index].ImageHandle != NULL) {
          return TRUE;
        } else {
          return FALSE;
        }
      } else {
        return TRUE;
      }
    }
  }
  return FALSE;
}

BOOLEAN
IsIoExit (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length,
  IN BOOLEAN               CheckAllocated
  )
{
  UINTN  Index;
  for (Index = 0; Index < mAcpiGcdIoMapNumberOfDescriptors; Index++) {
    if (mAcpiGcdIoMap[Index].GcdIoType != EfiGcdIoTypeIo) {
      continue;
    }
    if ((BaseAddress >= mAcpiGcdIoMap[Index].BaseAddress) &&
        (BaseAddress + Length <= mAcpiGcdIoMap[Index].BaseAddress + mAcpiGcdIoMap[Index].Length)) {
      if (CheckAllocated) {
        if (mAcpiGcdIoMap[Index].ImageHandle != NULL) {
          return TRUE;
        } else {
          return FALSE;
        }
      } else {
        return TRUE;
      }
    }
  }
  return FALSE;
}

VOID
DumpCharArray (
  IN CHAR8  *Ch,
  IN UINTN  Size
  )
{
  UINTN  Index;

  for (Index = 0; Index < Size; Index++) {
    DEBUG ((DEBUG_INFO, "%c", Ch[Index]));
  }
}

VOID
DumpAcpiTableHeader (
  IN EFI_ACPI_DESCRIPTION_HEADER  *Table
  )
{
  DEBUG ((DEBUG_INFO, "  "));
  DumpCharArray ((CHAR8 *)&Table->Signature, sizeof(Table->Signature));
  DEBUG ((DEBUG_INFO, "  : (0x%016lx) 0x%02x \'", Table, Table->Revision));
  DumpCharArray ((CHAR8 *)Table->OemId, sizeof(Table->OemId));
  DEBUG ((DEBUG_INFO, "\' \'"));
  DumpCharArray ((CHAR8 *)&Table->OemTableId, sizeof(Table->OemTableId));
  DEBUG ((DEBUG_INFO, "\' 0x%08x \'", Table->OemRevision));
  DumpCharArray ((CHAR8 *)&Table->CreatorId, sizeof(Table->CreatorId));
  DEBUG ((DEBUG_INFO, "\' 0x%08x |\n", Table->CreatorRevision));
}

VOID
DumpAcpiFadt (
  IN EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE  *Fadt
  )
{
  DumpAcpiTableHeader (&Fadt->Header);
  DEBUG ((DEBUG_INFO, "         "));
  DEBUG ((DEBUG_INFO, " SCI=0x%04x", Fadt->SciInt));
  DEBUG ((DEBUG_INFO, " SMI=0x%08x", Fadt->SmiCmd));
  DEBUG ((DEBUG_INFO, " AcpiEnable=0x%02x", Fadt->AcpiEnable));
  DEBUG ((DEBUG_INFO, " AcpiDisable=0x%02x", Fadt->AcpiDisable));
  DEBUG ((DEBUG_INFO, "\n"));
}

EFI_STATUS
CheckAcpiFadt (
  IN EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE  *Fadt
  )
{
  if (Fadt->SmiCmd != 0) {
    if (!IsIoExit (Fadt->SmiCmd, 1, TRUE)) {
      DEBUG ((DEBUG_ERROR, "FADT.SmiCmd resource (0x%x) is not reported correctly.\n", Fadt->SmiCmd));
      return EFI_NOT_STARTED;
    }
  }
  if (Fadt->Pm1aEvtBlk != 0) {
    if (!IsIoExit (Fadt->Pm1aEvtBlk, Fadt->Pm1EvtLen, TRUE)) {
      DEBUG ((DEBUG_ERROR, "FADT.Pm1aEvtBlk resource (0x%x) is not reported correctly.\n", Fadt->Pm1aEvtBlk));
      return EFI_NOT_STARTED;
    }
  }
  if (Fadt->Pm1bEvtBlk != 0) {
    if (!IsIoExit (Fadt->Pm1bEvtBlk, Fadt->Pm1EvtLen, TRUE)) {
      DEBUG ((DEBUG_ERROR, "FADT.Pm1bEvtBlk resource (0x%x) is not reported correctly.\n", Fadt->Pm1bEvtBlk));
      return EFI_NOT_STARTED;
    }
  }
  if (Fadt->Pm1aCntBlk != 0) {
    if (!IsIoExit (Fadt->Pm1aCntBlk, Fadt->Pm1CntLen, TRUE)) {
      DEBUG ((DEBUG_ERROR, "FADT.Pm1aCntBlk resource (0x%x) is not reported correctly.\n", Fadt->Pm1aCntBlk));
      return EFI_NOT_STARTED;
    }
  }
  if (Fadt->Pm1bCntBlk != 0) {
    if (!IsIoExit (Fadt->Pm1bCntBlk, Fadt->Pm1CntLen, TRUE)) {
      DEBUG ((DEBUG_ERROR, "FADT.Pm1bCntBlk resource (0x%x) is not reported correctly.\n", Fadt->Pm1bCntBlk));
      return EFI_NOT_STARTED;
    }
  }
  if (Fadt->Pm2CntBlk != 0) {
    if (!IsIoExit (Fadt->Pm2CntBlk, Fadt->Pm2CntLen, TRUE)) {
      DEBUG ((DEBUG_ERROR, "FADT.Pm2CntBlk resource (0x%x) is not reported correctly.\n", Fadt->Pm2CntBlk));
      return EFI_NOT_STARTED;
    }
  }
  if (Fadt->PmTmrBlk != 0) {
    if (!IsIoExit (Fadt->PmTmrBlk, Fadt->PmTmrLen, TRUE)) {
      DEBUG ((DEBUG_ERROR, "FADT.PmTmrBlk resource (0x%x) is not reported correctly.\n", Fadt->PmTmrBlk));
      return EFI_NOT_STARTED;
    }
  }
  if (Fadt->Gpe0Blk != 0) {
    if (!IsIoExit (Fadt->Gpe0Blk, Fadt->Gpe0BlkLen, TRUE)) {
      DEBUG ((DEBUG_ERROR, "FADT.Gpe0Blk resource (0x%x) is not reported correctly.\n", Fadt->Gpe0Blk));
      return EFI_NOT_STARTED;
    }
  }
  if (Fadt->Gpe1Blk != 0) {
    if (!IsIoExit (Fadt->Gpe1Blk, Fadt->Gpe1BlkLen, TRUE)) {
      DEBUG ((DEBUG_ERROR, "FADT.Gpe1Blk resource (0x%x) is not reported correctly.\n", Fadt->Gpe1Blk));
      return EFI_NOT_STARTED;
    }
  }
  if (Fadt->Header.Length > OFFSET_OF(EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE, ResetReg)) {
    if (Fadt->ResetReg.Address != 0) {
      switch (Fadt->ResetReg.AddressSpaceId) {
      case EFI_ACPI_5_0_SYSTEM_MEMORY:
        if (!IsMmioExit (Fadt->ResetReg.Address, Fadt->ResetReg.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.ResetReg resource (0x%x) is not reported correctly.\n", Fadt->ResetReg.Address));
          return EFI_NOT_STARTED;
        }
        break;
      case EFI_ACPI_5_0_SYSTEM_IO:
        if (!IsIoExit (Fadt->ResetReg.Address, Fadt->ResetReg.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.ResetReg resource (0x%x) is not reported correctly.\n", Fadt->ResetReg.Address));
          return EFI_NOT_STARTED;
        }
        break;
      }
    }
  }
  if (Fadt->Header.Length > OFFSET_OF(EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE, XPm1aEvtBlk)) {
    if (Fadt->XPm1aEvtBlk.Address != 0) {
      switch (Fadt->XPm1aEvtBlk.AddressSpaceId) {
      case EFI_ACPI_5_0_SYSTEM_MEMORY:
        if (!IsMmioExit (Fadt->XPm1aEvtBlk.Address, Fadt->XPm1aEvtBlk.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.XPm1aEvtBlk resource (0x%x) is not reported correctly.\n", Fadt->XPm1aEvtBlk.Address));
          return EFI_NOT_STARTED;
        }
        break;
      case EFI_ACPI_5_0_SYSTEM_IO:
        if (!IsIoExit (Fadt->XPm1aEvtBlk.Address, Fadt->XPm1aEvtBlk.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.XPm1aEvtBlk resource (0x%x) is not reported correctly.\n", Fadt->XPm1aEvtBlk.Address));
          return EFI_NOT_STARTED;
        }
        break;
      }
    }
  }
  if (Fadt->Header.Length > OFFSET_OF(EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE, XPm1bEvtBlk)) {
    if (Fadt->XPm1bEvtBlk.Address != 0) {
      switch (Fadt->XPm1bEvtBlk.AddressSpaceId) {
      case EFI_ACPI_5_0_SYSTEM_MEMORY:
        if (!IsMmioExit (Fadt->XPm1bEvtBlk.Address, Fadt->XPm1bEvtBlk.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.XPm1bEvtBlk resource (0x%x) is not reported correctly.\n", Fadt->XPm1bEvtBlk.Address));
          return EFI_NOT_STARTED;
        }
        break;
      case EFI_ACPI_5_0_SYSTEM_IO:
        if (!IsIoExit (Fadt->XPm1bEvtBlk.Address, Fadt->XPm1bEvtBlk.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.XPm1bEvtBlk resource (0x%x) is not reported correctly.\n", Fadt->XPm1bEvtBlk.Address));
          return EFI_NOT_STARTED;
        }
        break;
      }
    }
  }
  if (Fadt->Header.Length > OFFSET_OF(EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE, XPm1aCntBlk)) {
    if (Fadt->XPm1aCntBlk.Address != 0) {
      switch (Fadt->XPm1aCntBlk.AddressSpaceId) {
      case EFI_ACPI_5_0_SYSTEM_MEMORY:
        if (!IsMmioExit (Fadt->XPm1aCntBlk.Address, Fadt->XPm1aCntBlk.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.XPm1aCntBlk resource (0x%x) is not reported correctly.\n", Fadt->XPm1aCntBlk.Address));
          return EFI_NOT_STARTED;
        }
        break;
      case EFI_ACPI_5_0_SYSTEM_IO:
        if (!IsIoExit (Fadt->XPm1aCntBlk.Address, Fadt->XPm1aCntBlk.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.XPm1aCntBlk resource (0x%x) is not reported correctly.\n", Fadt->XPm1aCntBlk.Address));
          return EFI_NOT_STARTED;
        }
        break;
      }
    }
  }
  if (Fadt->Header.Length > OFFSET_OF(EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE, XPm1bCntBlk)) {
    if (Fadt->XPm1bCntBlk.Address != 0) {
      switch (Fadt->XPm1bCntBlk.AddressSpaceId) {
      case EFI_ACPI_5_0_SYSTEM_MEMORY:
        if (!IsMmioExit (Fadt->XPm1bCntBlk.Address, Fadt->XPm1bCntBlk.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.XPm1bCntBlk resource (0x%x) is not reported correctly.\n", Fadt->XPm1bCntBlk.Address));
          return EFI_NOT_STARTED;
        }
        break;
      case EFI_ACPI_5_0_SYSTEM_IO:
        if (!IsIoExit (Fadt->XPm1bCntBlk.Address, Fadt->XPm1bCntBlk.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.XPm1bCntBlk resource (0x%x) is not reported correctly.\n", Fadt->XPm1bCntBlk.Address));
          return EFI_NOT_STARTED;
        }
        break;
      }
    }
  }
  if (Fadt->Header.Length > OFFSET_OF(EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE, XPm2CntBlk)) {
    if (Fadt->XPm2CntBlk.Address != 0) {
      switch (Fadt->XPm2CntBlk.AddressSpaceId) {
      case EFI_ACPI_5_0_SYSTEM_MEMORY:
        if (!IsMmioExit (Fadt->XPm2CntBlk.Address, Fadt->XPm2CntBlk.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.XPm2CntBlk resource (0x%x) is not reported correctly.\n", Fadt->XPm2CntBlk.Address));
          return EFI_NOT_STARTED;
        }
        break;
      case EFI_ACPI_5_0_SYSTEM_IO:
        if (!IsIoExit (Fadt->XPm2CntBlk.Address, Fadt->XPm2CntBlk.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.XPm2CntBlk resource (0x%x) is not reported correctly.\n", Fadt->XPm2CntBlk.Address));
          return EFI_NOT_STARTED;
        }
        break;
      }
    }
  }
  if (Fadt->Header.Length > OFFSET_OF(EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE, XPmTmrBlk)) {
    if (Fadt->XPmTmrBlk.Address != 0) {
      switch (Fadt->XPmTmrBlk.AddressSpaceId) {
      case EFI_ACPI_5_0_SYSTEM_MEMORY:
        if (!IsMmioExit (Fadt->XPmTmrBlk.Address, Fadt->XPmTmrBlk.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.XPmTmrBlk resource (0x%x) is not reported correctly.\n", Fadt->XPmTmrBlk.Address));
          return EFI_NOT_STARTED;
        }
        break;
      case EFI_ACPI_5_0_SYSTEM_IO:
        if (!IsIoExit (Fadt->XPmTmrBlk.Address, Fadt->XPmTmrBlk.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.XPmTmrBlk resource (0x%x) is not reported correctly.\n", Fadt->XPmTmrBlk.Address));
          return EFI_NOT_STARTED;
        }
        break;
      }
    }
  }
  if (Fadt->Header.Length > OFFSET_OF(EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE, XGpe0Blk)) {
    if (Fadt->XGpe0Blk.Address != 0) {
      switch (Fadt->XGpe0Blk.AddressSpaceId) {
      case EFI_ACPI_5_0_SYSTEM_MEMORY:
        if (!IsMmioExit (Fadt->XGpe0Blk.Address, Fadt->XGpe0Blk.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.XGpe0Blk resource (0x%x) is not reported correctly.\n", Fadt->XGpe0Blk.Address));
          return EFI_NOT_STARTED;
        }
        break;
      case EFI_ACPI_5_0_SYSTEM_IO:
        if (!IsIoExit (Fadt->XGpe0Blk.Address, Fadt->XGpe0Blk.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.XGpe0Blk resource (0x%x) is not reported correctly.\n", Fadt->XGpe0Blk.Address));
          return EFI_NOT_STARTED;
        }
        break;
      }
    }
  }
  if (Fadt->Header.Length > OFFSET_OF(EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE, XGpe1Blk)) {
    if (Fadt->XGpe1Blk.Address != 0) {
      switch (Fadt->XGpe1Blk.AddressSpaceId) {
      case EFI_ACPI_5_0_SYSTEM_MEMORY:
        if (!IsMmioExit (Fadt->XGpe1Blk.Address, Fadt->XGpe1Blk.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.XGpe1Blk resource (0x%x) is not reported correctly.\n", Fadt->XGpe1Blk.Address));
          return EFI_NOT_STARTED;
        }
        break;
      case EFI_ACPI_5_0_SYSTEM_IO:
        if (!IsIoExit (Fadt->XGpe1Blk.Address, Fadt->XGpe1Blk.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.XGpe1Blk resource (0x%x) is not reported correctly.\n", Fadt->XGpe1Blk.Address));
          return EFI_NOT_STARTED;
        }
        break;
      }
    }
  }
  if (Fadt->Header.Length > OFFSET_OF(EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE, SleepControlReg)) {
    if (Fadt->SleepControlReg.Address != 0) {
      switch (Fadt->SleepControlReg.AddressSpaceId) {
      case EFI_ACPI_5_0_SYSTEM_MEMORY:
        if (!IsMmioExit (Fadt->SleepControlReg.Address, Fadt->SleepControlReg.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.SleepControlReg resource (0x%x) is not reported correctly.\n", Fadt->SleepControlReg.Address));
          return EFI_NOT_STARTED;
        }
        break;
      case EFI_ACPI_5_0_SYSTEM_IO:
        if (!IsIoExit (Fadt->SleepControlReg.Address, Fadt->SleepControlReg.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.SleepControlReg resource (0x%x) is not reported correctly.\n", Fadt->SleepControlReg.Address));
          return EFI_NOT_STARTED;
        }
        break;
      }
    }
  }
  if (Fadt->Header.Length > OFFSET_OF(EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE, SleepStatusReg)) {
    if (Fadt->SleepStatusReg.Address != 0) {
      switch (Fadt->SleepStatusReg.AddressSpaceId) {
      case EFI_ACPI_5_0_SYSTEM_MEMORY:
        if (!IsMmioExit (Fadt->SleepStatusReg.Address, Fadt->SleepStatusReg.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.SleepStatusReg resource (0x%x) is not reported correctly.\n", Fadt->SleepStatusReg.Address));
          return EFI_NOT_STARTED;
        }
        break;
      case EFI_ACPI_5_0_SYSTEM_IO:
        if (!IsIoExit (Fadt->SleepStatusReg.Address, Fadt->SleepStatusReg.RegisterBitWidth/8, TRUE)) {
          DEBUG ((DEBUG_ERROR, "FADT.SleepStatusReg resource (0x%x) is not reported correctly.\n", Fadt->SleepStatusReg.Address));
          return EFI_NOT_STARTED;
        }
        break;
      }
    }
  }
  return EFI_SUCCESS;
}

VOID
DumpAcpiFacs (
  IN EFI_ACPI_2_0_FIRMWARE_ACPI_CONTROL_STRUCTURE  *Facs
  )
{
  DEBUG ((DEBUG_INFO, "  "));
  DumpCharArray ((CHAR8 *)&Facs->Signature, sizeof(Facs->Signature));
  DEBUG ((DEBUG_INFO, "  : (0x%016lx) 0x%02x\n", Facs, Facs->Version));
  DEBUG ((DEBUG_INFO, "    "));
  DEBUG ((DEBUG_INFO, " HardwareSignature=%08x\n", Facs->HardwareSignature));
}

VOID
DumpAcpiTable (
  IN EFI_ACPI_DESCRIPTION_HEADER  *Table
  )
{
  EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE  *Fadt;

  if (Table == NULL) {
    return ;
  }

  switch (Table->Signature) {
  case EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE:
    Fadt = (EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE *)Table;
    DumpAcpiFadt (Fadt);
    if (Fadt->Header.Revision >= EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE_REVISION) {
      if (Fadt->XFirmwareCtrl != 0) {
        DumpAcpiFacs ((EFI_ACPI_2_0_FIRMWARE_ACPI_CONTROL_STRUCTURE *)(UINTN)Fadt->XFirmwareCtrl);
      } else {
        DumpAcpiFacs ((EFI_ACPI_2_0_FIRMWARE_ACPI_CONTROL_STRUCTURE *)(UINTN)Fadt->FirmwareCtrl);
      }
      if (Fadt->XDsdt != 0) {
        DumpAcpiTable ((EFI_ACPI_DESCRIPTION_HEADER *)(UINTN)Fadt->XDsdt);
      } else {
        DumpAcpiTable ((EFI_ACPI_DESCRIPTION_HEADER *)(UINTN)Fadt->Dsdt);
      }
    } else {
      DumpAcpiFacs ((EFI_ACPI_2_0_FIRMWARE_ACPI_CONTROL_STRUCTURE *)(UINTN)Fadt->FirmwareCtrl);
      DumpAcpiTable ((EFI_ACPI_DESCRIPTION_HEADER *)(UINTN)Fadt->Dsdt);
    }
    break;
  case EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_SIGNATURE:
    DumpAcpiMadt ((EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER *)Table);
    break;
  case EFI_ACPI_2_0_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_SIGNATURE:
    DumpAcpiMcfg ((EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER *)Table);
    break;
  case EFI_ACPI_3_0_HIGH_PRECISION_EVENT_TIMER_TABLE_SIGNATURE:
    DumpAcpiHpet ((EFI_ACPI_HIGH_PRECISION_EVENT_TIMER_TABLE_HEADER *)Table);
    break;
  case EFI_ACPI_4_0_DMA_REMAPPING_TABLE_SIGNATURE:
    DumpAcpiDmar ((EFI_ACPI_DMAR_HEADER *)Table);
    break;
  case EFI_ACPI_WINDOWS_SMM_SECURITY_MITIGATION_TABLE_SIGNATURE:
    DumpAcpiWsmt ((EFI_ACPI_WSMT_TABLE *)Table);
    break;
  case EFI_ACPI_5_0_TRUSTED_COMPUTING_PLATFORM_2_TABLE_SIGNATURE:
    DumpAcpiTpm2 ((EFI_TPM2_ACPI_TABLE *)Table);
    break;
  case EFI_ACPI_3_0_TRUSTED_COMPUTING_PLATFORM_ALLIANCE_CAPABILITIES_TABLE_SIGNATURE:
    DumpAcpiTcpa ((VOID *)Table);
    break;
  default:
    DumpAcpiTableHeader (Table);
    break;
  }
}

EFI_STATUS
CheckAcpiTableResource (
  IN EFI_ACPI_DESCRIPTION_HEADER  *Table
  )
{
  EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE  *Fadt;

  if (Table == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  switch (Table->Signature) {
  case EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE:
    Fadt = (EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE *)Table;
    CheckAcpiFadt (Fadt);
    if (Fadt->Header.Revision >= EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE_REVISION) {
      if (Fadt->XFirmwareCtrl != 0) {
        // CheckAcpiFacs ((EFI_ACPI_2_0_FIRMWARE_ACPI_CONTROL_STRUCTURE *)(UINTN)Fadt->XFirmwareCtrl);
      } else {
        // CheckAcpiFacs ((EFI_ACPI_2_0_FIRMWARE_ACPI_CONTROL_STRUCTURE *)(UINTN)Fadt->FirmwareCtrl);
      }
      if (Fadt->XDsdt != 0) {
        // CheckAcpiDsdt ((EFI_ACPI_DESCRIPTION_HEADER *)(UINTN)Fadt->XDsdt);
      } else {
        // CheckAcpiDsdt ((EFI_ACPI_DESCRIPTION_HEADER *)(UINTN)Fadt->Dsdt);
      }
    } else {
      // CheckAcpiFacs ((EFI_ACPI_2_0_FIRMWARE_ACPI_CONTROL_STRUCTURE *)(UINTN)Fadt->FirmwareCtrl);
      // CheckAcpiDsdt ((EFI_ACPI_DESCRIPTION_HEADER *)(UINTN)Fadt->Dsdt);
    }
    break;
  case EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_SIGNATURE:
    return CheckAcpiMadt ((EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER *)Table);
    break;
  case EFI_ACPI_2_0_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_SIGNATURE:
    return CheckAcpiMcfg ((EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER *)Table);
    break;
  case EFI_ACPI_3_0_HIGH_PRECISION_EVENT_TIMER_TABLE_SIGNATURE:
    return CheckAcpiHpet ((EFI_ACPI_HIGH_PRECISION_EVENT_TIMER_TABLE_HEADER *)Table);
    break;
  case EFI_ACPI_4_0_DMA_REMAPPING_TABLE_SIGNATURE:
    return CheckAcpiDmar ((EFI_ACPI_DMAR_HEADER *)Table);
    break;
  case EFI_ACPI_WINDOWS_SMM_SECURITY_MITIGATION_TABLE_SIGNATURE:
    // CheckAcpiWsmt ((EFI_ACPI_WSMT_TABLE *)Table);
    break;
  case EFI_ACPI_5_0_TRUSTED_COMPUTING_PLATFORM_2_TABLE_SIGNATURE:
    return CheckAcpiTpm2 ((EFI_TPM2_ACPI_TABLE *)Table);
    break;
  case EFI_ACPI_3_0_TRUSTED_COMPUTING_PLATFORM_ALLIANCE_CAPABILITIES_TABLE_SIGNATURE:
    return CheckAcpiTcpa ((VOID *)Table);
    break;
  default:
    break;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
DumpAcpiRsdt (
  IN EFI_ACPI_DESCRIPTION_HEADER  *Rsdt,
  IN UINT32                       *Signature, OPTIONAL
  OUT VOID                        **OutTable, OPTIONAL
  IN BOOLEAN                      DumpPrint,
  IN BOOLEAN                      CheckResource
  )
{
  UINTN                              Index;
  UINT32                             EntryCount;
  UINT32                             *EntryPtr;
  EFI_ACPI_DESCRIPTION_HEADER        *Table;
  EFI_STATUS                         Status;
  EFI_STATUS                         ReturnStatus;

  if (Rsdt == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (OutTable != NULL) {
    *OutTable = NULL;
  } else if ((OutTable == NULL) && (Signature != NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  ReturnStatus = EFI_SUCCESS;
  EntryCount = (Rsdt->Length - sizeof (EFI_ACPI_DESCRIPTION_HEADER)) / sizeof(UINT32);
  EntryPtr = (UINT32 *)(Rsdt + 1);
  for (Index = 0; Index < EntryCount; Index ++, EntryPtr ++) {
    Table = (EFI_ACPI_DESCRIPTION_HEADER *)((UINTN)(*EntryPtr));
    if (DumpPrint) {
      DumpAcpiTable (Table);
    }
    if (CheckResource) {
      Status = CheckAcpiTableResource (Table);
      if (EFI_ERROR(Status)) {
        ReturnStatus = Status;
      }
    }
    if (Signature != NULL && Table->Signature == *Signature) {
      *OutTable = Table;
    }
  }

  if (OutTable != NULL) {
    if (*OutTable == NULL) {
      return EFI_NOT_FOUND;
    }
  }

  return ReturnStatus;
}

EFI_STATUS
DumpAcpiXsdt (
  IN EFI_ACPI_DESCRIPTION_HEADER  *Xsdt,
  IN UINT32                       *Signature, OPTIONAL
  OUT VOID                        **OutTable, OPTIONAL
  IN BOOLEAN                      DumpPrint,
  IN BOOLEAN                      CheckResource
  )
{
  UINTN                          Index;
  UINT32                         EntryCount;
  UINT64                         EntryPtr;
  UINTN                          BasePtr;
  EFI_ACPI_DESCRIPTION_HEADER    *Table;
  EFI_STATUS                     Status;
  EFI_STATUS                     ReturnStatus;

  if (Xsdt == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (OutTable != NULL) {
    *OutTable = NULL;
  } else if ((OutTable == NULL) && (Signature != NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  ReturnStatus = EFI_SUCCESS;
  EntryCount = (Xsdt->Length - sizeof (EFI_ACPI_DESCRIPTION_HEADER)) / sizeof(UINT64);

  BasePtr = (UINTN)(Xsdt + 1);
  for (Index = 0; Index < EntryCount; Index ++) {
    CopyMem (&EntryPtr, (VOID *)(BasePtr + Index * sizeof(UINT64)), sizeof(UINT64));
    Table = (EFI_ACPI_DESCRIPTION_HEADER *)((UINTN)(EntryPtr));
    if (DumpPrint) {
      DumpAcpiTable (Table);
    }
    if (CheckResource) {
      Status = CheckAcpiTableResource (Table);
      if (EFI_ERROR(Status)) {
        ReturnStatus = Status;
      }
    }
    if (Signature != NULL && Table->Signature == *Signature) {
      *OutTable = Table;
    }
  }

  if (OutTable != NULL) {
    if (*OutTable == NULL) {
      return EFI_NOT_FOUND;
    }
  }

  return ReturnStatus;
}

EFI_STATUS
DumpAcpiRsdp (
  IN EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER  *Rsdp,
  IN UINT32                                        *Signature, OPTIONAL
  OUT VOID                                         **Table,
  IN BOOLEAN                                       DumpPrint,
  IN BOOLEAN                                       CheckResource
  )
{
  EFI_STATUS                                    Status;
  EFI_ACPI_DESCRIPTION_HEADER                   *Rsdt;
  EFI_ACPI_DESCRIPTION_HEADER                   *Xsdt;

  if (DumpPrint) {
    DumpCharArray ((CHAR8 *)&Rsdp->Signature, sizeof(Rsdp->Signature));
    DEBUG ((DEBUG_INFO, ": (0x%016lx) 0x%02x \'", Rsdp, Rsdp->Revision));
    DumpCharArray ((CHAR8 *)Rsdp->OemId, sizeof(Rsdp->OemId));
    DEBUG ((DEBUG_INFO, "\'\n"));
    DEBUG ((DEBUG_INFO, "         "));
    DEBUG ((DEBUG_INFO, " RSDT=0x%08x XSDT=0x%016lx\n", Rsdp->RsdtAddress, Rsdp->XsdtAddress));

    if (Rsdp->Revision >= EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER_REVISION) {
      DumpAcpiTable ((EFI_ACPI_DESCRIPTION_HEADER *)(UINTN)Rsdp->XsdtAddress);
    }
    DumpAcpiTable ((EFI_ACPI_DESCRIPTION_HEADER *)(UINTN)Rsdp->RsdtAddress);
  }

  //
  // Search XSDT
  //
  if (Rsdp->Revision >= EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER_REVISION) {
    Xsdt = (EFI_ACPI_DESCRIPTION_HEADER *)(UINTN) Rsdp->XsdtAddress;
    Status = DumpAcpiXsdt (Xsdt, Signature, Table, DumpPrint, CheckResource);
  } else {
    //
    // Search RSDT
    //
    Rsdt = (EFI_ACPI_DESCRIPTION_HEADER *)(UINTN) Rsdp->RsdtAddress;
    Status = DumpAcpiRsdt (Rsdt, Signature, Table, DumpPrint, CheckResource);
  }

  return Status;
}

EFI_STATUS
DumpAcpiWithGuid (
  IN EFI_GUID  *AcpiTableGuid,
  IN UINT32    *Signature, OPTIONAL
  OUT VOID     **Table,
  IN BOOLEAN   DumpPrint,
  IN BOOLEAN   CheckResource
  )
{
  VOID        *Rsdp;
  EFI_STATUS  Status;

  Status = EfiGetSystemConfigurationTable (AcpiTableGuid, &Rsdp);
  if (EFI_ERROR(Status)) {
    return EFI_NOT_FOUND;
  }

  Status = DumpAcpiRsdp (Rsdp, Signature, Table, DumpPrint, CheckResource);

  return Status;
}

EFI_STATUS
TestPointCheckAcpi (
  VOID
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "==== TestPointCheckAcpi - Enter\n"));

  DEBUG ((DEBUG_INFO, "AcpiTable :\n"));
  DEBUG ((DEBUG_INFO, "  Table         Address        Rev   OemId   OemTableId   OemRev   Creat  CreatorRev\n"));
  //
  // First dump
  //
  Status = DumpAcpiWithGuid (&gEfiAcpi20TableGuid, NULL, NULL, TRUE, FALSE);
  if (Status == EFI_NOT_FOUND) {
    Status = DumpAcpiWithGuid (&gEfiAcpi10TableGuid, NULL, NULL, TRUE, FALSE);
  }

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "No ACPI table\n"));
    TestPointLibAppendErrorString (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      TEST_POINT_BYTE4_READY_TO_BOOT_ACPI_TABLE_FUNCTIONAL_ERROR_CODE \
        TEST_POINT_READY_TO_BOOT \
        TEST_POINT_BYTE4_READY_TO_BOOT_ACPI_TABLE_FUNCTIONAL_ERROR_STRING
      );
  }

  DEBUG ((DEBUG_INFO, "==== TestPointCheckAcpi - Exit\n"));

  return Status;
}

EFI_STATUS
TestPointCheckAcpiGcdResource (
  VOID
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "==== TestPointCheckAcpiGcdResource - Enter\n"));

  //
  // Check the ACPI existence
  //
  Status = DumpAcpiWithGuid (&gEfiAcpi20TableGuid, NULL, NULL, FALSE, FALSE);
  if (Status == EFI_NOT_FOUND) {
    Status = DumpAcpiWithGuid (&gEfiAcpi10TableGuid, NULL, NULL, FALSE, FALSE);
  }

  if (!EFI_ERROR(Status)) {
    //
    // Then check resource in ACPI and GCD
    //
    TestPointDumpGcd (&mAcpiGcdMemoryMap, &mAcpiGcdMemoryMapNumberOfDescriptors, &mAcpiGcdIoMap, &mAcpiGcdIoMapNumberOfDescriptors, FALSE);

    Status = DumpAcpiWithGuid (&gEfiAcpi20TableGuid, NULL, NULL, FALSE, TRUE);
    if (Status == EFI_NOT_FOUND) {
      Status = DumpAcpiWithGuid (&gEfiAcpi10TableGuid, NULL, NULL, FALSE, TRUE);
    }
  }

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "ACPI table resource not in GCD\n"));
    TestPointLibAppendErrorString (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      TEST_POINT_BYTE4_READY_TO_BOOT_GCD_RESOURCE_FUNCTIONAL_ERROR_CODE \
        TEST_POINT_READY_TO_BOOT \
        TEST_POINT_BYTE4_READY_TO_BOOT_GCD_RESOURCE_FUNCTIONAL_ERROR_STRING
      );
  }
  DEBUG ((DEBUG_INFO, "==== TestPointCheckAcpiGcdResource - Exit\n"));

  return Status;
}

VOID *
TestPointGetAcpi (
  IN UINT32  Signature
  )
{
  EFI_STATUS  Status;
  VOID        *Table;

  Status = DumpAcpiWithGuid (&gEfiAcpi20TableGuid, &Signature, &Table, FALSE, FALSE);
  if (Status == EFI_NOT_FOUND) {
    Status = DumpAcpiWithGuid (&gEfiAcpi10TableGuid, &Signature, &Table, FALSE, FALSE);
  }

  if (EFI_ERROR(Status)) {
    return NULL;
  }

  return Table;
}

