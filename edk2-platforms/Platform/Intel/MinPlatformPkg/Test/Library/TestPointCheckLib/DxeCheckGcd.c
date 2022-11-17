/** @file

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeServicesTableLib.h>

EFI_STATUS
TestPointCheckAcpiGcdResource (
  VOID
  );

CHAR8 *mGcdMemoryTypeShortName[] = {
  " NE  ",
  " RSVD",
  " SYS ",
  " MMIO",
  " PERS",
  " RELI",
};

CHAR8 mUnknownStr[11];

CHAR8 *
ShortNameOfGcdMemoryType(
  IN UINT32 Type
  )
{
  if (Type < sizeof(mGcdMemoryTypeShortName) / sizeof(mGcdMemoryTypeShortName[0])) {
    return mGcdMemoryTypeShortName[Type];
  } else {
    return " ???? ";
  }
}

CHAR8 *mGcdIoTypeShortName[] = {
  " NE  ",
  " RSVD",
  " SYS ",
};

CHAR8 *
ShortNameOfGcdIoType(
  IN UINT32 Type
  )
{
  if (Type < sizeof(mGcdIoTypeShortName) / sizeof(mGcdIoTypeShortName[0])) {
    return mGcdIoTypeShortName[Type];
  } else {
    return " ???? ";
  }
}

VOID
PrintBitMask (
  IN UINT64 Bit,
  IN UINT64 Capabilities,
  IN UINT64 Attributes
  )
{
  if ((Capabilities & Bit) != 0) {
    if ((Attributes & Bit) != 0) {
      DEBUG ((DEBUG_INFO, "1"));
    } else {
      DEBUG ((DEBUG_INFO, "0"));
    }
  } else {
    DEBUG ((DEBUG_INFO, "-"));
  }
}

VOID
PrintMemoryDescriptorHeader (
  VOID
  )
{
  if (sizeof(UINT64) == sizeof(UINTN)) {
    DEBUG ((DEBUG_INFO, "                                              U                                      \n"));
    DEBUG ((DEBUG_INFO, "                                       RRMNXRWCWWWU                                  \n"));
    DEBUG ((DEBUG_INFO, "Base Address     End Address      Type TORVPPPEBTCC Image            Device          \n"));
    DEBUG ((DEBUG_INFO, "================ ================ ==== ============ ================ ================\n"));
  } else {
    DEBUG ((DEBUG_INFO, "                                              U                      \n"));
    DEBUG ((DEBUG_INFO, "                                       RRMNXRWCWWWU                  \n"));
    DEBUG ((DEBUG_INFO, "Base Address     End Address      Type TORVPPPEBTCC Image    Device  \n"));
    DEBUG ((DEBUG_INFO, "================ ================ ==== ============ ======== ========\n"));
  }
}

VOID
PrintMemoryDescriptor (
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR  *MemoryDescriptor
  )
{
  DEBUG ((DEBUG_INFO,
    "%016lx-%016lx",
    MemoryDescriptor->BaseAddress,
    MemoryDescriptor->BaseAddress + MemoryDescriptor->Length - 1
    ));

  DEBUG ((DEBUG_INFO, ShortNameOfGcdMemoryType(MemoryDescriptor->GcdMemoryType)));
  DEBUG ((DEBUG_INFO, " "));

  if (MemoryDescriptor->GcdMemoryType != EfiGcdMemoryTypeNonExistent) {
    PrintBitMask (EFI_MEMORY_RUNTIME, MemoryDescriptor->Capabilities, MemoryDescriptor->Attributes);
    PrintBitMask (EFI_MEMORY_RO, MemoryDescriptor->Capabilities, MemoryDescriptor->Attributes);
    PrintBitMask (EFI_MEMORY_MORE_RELIABLE, MemoryDescriptor->Capabilities, MemoryDescriptor->Attributes);
    PrintBitMask (EFI_MEMORY_NV, MemoryDescriptor->Capabilities, MemoryDescriptor->Attributes);
    PrintBitMask (EFI_MEMORY_XP, MemoryDescriptor->Capabilities, MemoryDescriptor->Attributes);
    PrintBitMask (EFI_MEMORY_RP, MemoryDescriptor->Capabilities, MemoryDescriptor->Attributes);
    PrintBitMask (EFI_MEMORY_WP, MemoryDescriptor->Capabilities, MemoryDescriptor->Attributes);
    PrintBitMask (EFI_MEMORY_UCE, MemoryDescriptor->Capabilities, MemoryDescriptor->Attributes);
    PrintBitMask (EFI_MEMORY_WB, MemoryDescriptor->Capabilities, MemoryDescriptor->Attributes);
    PrintBitMask (EFI_MEMORY_WT, MemoryDescriptor->Capabilities, MemoryDescriptor->Attributes);
    PrintBitMask (EFI_MEMORY_WC, MemoryDescriptor->Capabilities, MemoryDescriptor->Attributes);
    PrintBitMask (EFI_MEMORY_UC, MemoryDescriptor->Capabilities, MemoryDescriptor->Attributes);
  } else {
    DEBUG ((DEBUG_INFO, "            "));
  }

  if (sizeof(UINT64) == sizeof(UINTN)) {
    if (MemoryDescriptor->ImageHandle != NULL) {
      DEBUG ((DEBUG_INFO, " %016lx", (UINT64)(UINTN)MemoryDescriptor->ImageHandle));
      if (MemoryDescriptor->DeviceHandle != NULL) {
        DEBUG ((DEBUG_INFO, " %016lx", (UINT64)(UINTN)MemoryDescriptor->ImageHandle));
      }
    }
  } else {
    if (MemoryDescriptor->ImageHandle != NULL) {
      DEBUG ((DEBUG_INFO, " %08x", MemoryDescriptor->ImageHandle));
      if (MemoryDescriptor->DeviceHandle != NULL) {
        DEBUG ((DEBUG_INFO, " %08x", MemoryDescriptor->ImageHandle));
      }
    }
  }

  DEBUG ((DEBUG_INFO, "\n"));
}

VOID
PrintIoDescriptorHeader (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "Base Address     End Address      Type Image    Device  \n"));
  DEBUG ((DEBUG_INFO, "================ ================ ==== ======== ========\n"));
}

VOID
PrintIoDescriptor (
  EFI_GCD_IO_SPACE_DESCRIPTOR  *IoDescriptor
  )
{
  DEBUG ((DEBUG_INFO,
    "%016lx-%016lx",
    IoDescriptor->BaseAddress,
    IoDescriptor->BaseAddress + IoDescriptor->Length - 1
    ));

  DEBUG ((DEBUG_INFO, ShortNameOfGcdIoType (IoDescriptor->GcdIoType)));

  if (IoDescriptor->ImageHandle != NULL) {
    DEBUG ((DEBUG_INFO, " %08x", IoDescriptor->ImageHandle));
    if (IoDescriptor->DeviceHandle != NULL) {
      DEBUG ((DEBUG_INFO, " %08x", IoDescriptor->ImageHandle));
    }
  }

  DEBUG ((DEBUG_INFO, "\n"));
}

VOID
TestPointDumpGcd (
  OUT EFI_GCD_MEMORY_SPACE_DESCRIPTOR **GcdMemoryMap,  OPTIONAL
  OUT UINTN                           *GcdMemoryMapNumberOfDescriptors,  OPTIONAL
  OUT EFI_GCD_IO_SPACE_DESCRIPTOR     **GcdIoMap,  OPTIONAL
  OUT UINTN                           *GcdIoMapNumberOfDescriptors,  OPTIONAL
  IN  BOOLEAN                         DumpPrint
  )
{
  EFI_STATUS                      Status;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR *MemoryMap;
  EFI_GCD_IO_SPACE_DESCRIPTOR     *IoMap;
  UINTN                           NumberOfDescriptors;
  UINTN                           Index;

  if (GcdMemoryMap != NULL) {
    *GcdMemoryMap = NULL;
    *GcdMemoryMapNumberOfDescriptors = 0;
  }
  if (GcdIoMap != NULL) {
    *GcdIoMap = NULL;
    *GcdIoMapNumberOfDescriptors = 0;
  }
  
  if (DumpPrint) {
    DEBUG ((DEBUG_INFO, "==== TestPointDumpGcd - Enter\n"));
    DEBUG ((DEBUG_INFO, "GCD MEM:\n"));
  }
  NumberOfDescriptors = 0;
  MemoryMap           = NULL;
  Status = gDS->GetMemorySpaceMap (
                  &NumberOfDescriptors,
                  &MemoryMap
                  );
  if (!EFI_ERROR (Status)) {
    if (DumpPrint) {
      PrintMemoryDescriptorHeader ();
      for (Index = 0; Index < NumberOfDescriptors; Index++) {
        PrintMemoryDescriptor (&MemoryMap[Index]);
      }
    }
    if (GcdMemoryMap != NULL) {
      *GcdMemoryMap = AllocateCopyPool (NumberOfDescriptors * sizeof(EFI_GCD_MEMORY_SPACE_DESCRIPTOR), MemoryMap);
      *GcdMemoryMapNumberOfDescriptors = NumberOfDescriptors;
    }
  }

  if (DumpPrint) {
    DEBUG ((DEBUG_INFO, "GCD IO:\n"));
  }
  NumberOfDescriptors = 0;
  IoMap               = NULL;
  Status = gDS->GetIoSpaceMap (
                  &NumberOfDescriptors,
                  &IoMap
                  );
  if (!EFI_ERROR (Status)) {
    if (DumpPrint) {
      PrintIoDescriptorHeader ();
      for (Index = 0; Index < NumberOfDescriptors; Index++) {
        PrintIoDescriptor (&IoMap[Index]);
      }
    }
    if (GcdMemoryMap != NULL) {
      if (GcdIoMap != NULL) {
        *GcdIoMap = AllocateCopyPool (NumberOfDescriptors * sizeof(EFI_GCD_IO_SPACE_DESCRIPTOR), IoMap);
      }
      *GcdIoMapNumberOfDescriptors = NumberOfDescriptors;
    }
  }

  if (DumpPrint) {
    DEBUG ((DEBUG_INFO, "==== TestPointDumpGcd - Exit\n"));
  }
  return ;
}
