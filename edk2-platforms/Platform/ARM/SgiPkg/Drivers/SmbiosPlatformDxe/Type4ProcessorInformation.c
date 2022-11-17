/** @file
  SMBIOS Type 4 (Processor information) table for ARM RD platforms.

  This file installs SMBIOS Type 4 (Processor information) Table for Arm's
  Reference Design platforms. It includes information about manufacture,
  family, processor id, maximum operating frequency, and other information
  related to the processor.

  Copyright (c) 2021, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Specification Reference:
    - SMBIOS Reference Specification 3.4.0, Chapter 7.5
**/

#include <Library/ArmLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Protocol/Smbios.h>

#include "SgiPlatform.h"
#include "SmbiosPlatformDxe.h"

#define NEOVERSE_E1_THREADS_PER_CORE    2

#define SOCKET_TYPE_BASE        3
#define SOCKET_TYPE_NUM         1
#define PROCESSOR_VERSION_BASE  (SOCKET_TYPE_BASE + SOCKET_TYPE_NUM)
#define PROCESSOR_VERSION_NUM   9
#define SERIAL_NUMBER_BASE      (PROCESSOR_VERSION_BASE + PROCESSOR_VERSION_NUM)
#define TYPE4_STRINGS                                   \
  "0x000\0"                     /* Part Number */       \
  "ARM LTD\0"                   /* manufactuer */       \
  "Other\0"                     /* socket type */       \
  "Unknown\0"                   /* Processor Version */ \
  "Cortex-A75\0"                                        \
  "Neoverse-N1\0"                                       \
  "Neoverse-N1\0"                                       \
  "Neoverse-E1\0"                                       \
  "Neoverse-V1\0"                                       \
  "Neoverse-V1\0"                                       \
  "Neoverse-N2\0"                                       \
  "Neoverse-N2\0"                                       \
  "000-0\0"                     /* Serial number */     \
  "783-3\0"                                             \
  "786-1\0"                                             \
  "786-1\0"                                             \
  "786-2\0"                                             \
  "78A-1\0"                                             \
  "78A-2\0"                                             \
  "7B7-1\0"                                             \
  "7B6-1\0"

typedef enum {
  PartNumber = 1,
  ManufacturerName,
  SocketTypeBase = SOCKET_TYPE_BASE,
  ProcessorVersionBase = PROCESSOR_VERSION_BASE,
  SerialNumberBase = SERIAL_NUMBER_BASE
} TYPE4_STRING_ELEMENTS;

/* SMBIOS Type4 structure */
#pragma pack(1)
typedef struct {
  SMBIOS_TABLE_TYPE4  Base;
  CHAR8               Strings[sizeof (TYPE4_STRINGS)];
} ARM_RD_SMBIOS_TYPE4;
#pragma pack()

/* Processor information */
STATIC ARM_RD_SMBIOS_TYPE4 mArmRdSmbiosType4 = {
  {
    {
      // SMBIOS header
      EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION, // Type 4
      sizeof (SMBIOS_TABLE_TYPE4),           // Length
      SMBIOS_HANDLE_CLUSTER1,                // handle number
    },
    SocketTypeBase,          // Socket type
    CentralProcessor,        // Processor type
    ProcessorFamilyIndicatorFamily2,
                             // Use Processor Family 2 field
    ManufacturerName,        // Manufacturer string number
    {{0}, {0}},              // Processor id, update dynamically
    ProcessorVersionBase,    // Processor version, update dynamically
    {0, 0, 0, 0, 0, 1},      // Non legacy mode for processor voltage
    0,                       // External clock frequency unknown
    2600,                    // Max speed in MHz
    2600,                    // Current speed in MHz
    (                        // Status
     (1 << 6) |              // CPU Socket Populated
     (1 << 0)                // CPU Enabled
    ),
    ProcessorUpgradeOther,   // Processor Upgrade
    SMBIOS_HANDLE_L1I_CACHE, // L1 Cache handle
    SMBIOS_HANDLE_L2_CACHE,  // L2 Cache handle
    SMBIOS_HANDLE_L3_CACHE,  // L3 Cache handle
    0,                       // Processor serial number not set
    0,                       // Processor asset tag not set
    PartNumber,              // Part number, update dynamically
    0,                       // Core count, update dynamically
    0,                       // Enabled core count, update dynamically
    0,                       // Thread per socket count
    (                        // Processor characteristics
      (1 << 2) |             // 64-bit Capable
      (1 << 3) |             // Multi-Core
      (1 << 5) |             // Execute Protection
      (1 << 6) |             // Enhanced Virtualization
      (1 << 7)               // Power/Performance Control
    ),
    ProcessorFamilyARM       // Processor Family 2
    },
  TYPE4_STRINGS
};

/**
  Update the part-number string.

  Get the part number from ProcessorId and update TYPE4_STRINGS

  @param  ProcessorId    The processor Id read from MIDR register
**/
STATIC
VOID
UpdatePartNumber (
  IN     UINT64 ProcessorId
  )
{
  CHAR8  PartNumber[6] = {0};
  UINT16 PartNum;

  PartNum = (UINT16)((ProcessorId >> 4) & 0xFFF);

  /* Convert 3 digit hexadecimal partnumber to ASCII and update TYPE4_STRINGS */
  AsciiSPrint (PartNumber, sizeof (PartNumber), "0x%03x", PartNum);
  CopyMem (&mArmRdSmbiosType4.Strings[0], PartNumber, sizeof (PartNumber));
}

/**
  Install SMBIOS Processor information Table

  Install the SMBIOS Processor information (type 4) table for Arm's Reference
  Design platforms.

  @param[in]  Smbios   SMBIOS protocol.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_NOT_FOUND         Unknown product id.
  @retval EFI_OUT_OF_RESOURCES  Record was not added.
  @retval EFI_ALREADY_STARTED   The SmbiosHandle passed is already in use.
**/
EFI_STATUS
InstallType4ProcessorInformation (
  IN     EFI_SMBIOS_PROTOCOL    *Smbios
  )
{
  EFI_STATUS Status;
  EFI_SMBIOS_HANDLE SmbiosHandle;
  UINT8  ProductId;
  UINT32 CoreCount;
  UINT64 *ProcessorId = (UINT64 *)&(mArmRdSmbiosType4.Base.ProcessorId);

  SmbiosHandle = ((EFI_SMBIOS_TABLE_HEADER *)&mArmRdSmbiosType4)->Handle;
  CoreCount = (FixedPcdGet32 (PcdCoreCount) * FixedPcdGet32 (PcdClusterCount));
  ProductId = SgiGetProductId ();

  /* Set the core count and processor speed for all platforms */
  switch (ProductId) {
  case Sgi575:
  case RdN1Edge:
  case RdV1:
    mArmRdSmbiosType4.Base.CoreCount = CoreCount;
    mArmRdSmbiosType4.Base.EnabledCoreCount = CoreCount;
    mArmRdSmbiosType4.Base.ThreadCount = CoreCount;
    break;
  case RdN2:
  case RdN2Cfg1:
    mArmRdSmbiosType4.Base.CoreCount = CoreCount;
    mArmRdSmbiosType4.Base.EnabledCoreCount = CoreCount;
    mArmRdSmbiosType4.Base.ThreadCount = CoreCount;
    mArmRdSmbiosType4.Base.MaxSpeed = 3200;      // Frequency in MHz
    mArmRdSmbiosType4.Base.CurrentSpeed = 3200;  // Frequency in MHz
    break;
  case RdN1EdgeX2:
  case RdV1Mc:
    mArmRdSmbiosType4.Base.CoreCount = CoreCount * FixedPcdGet32 (PcdChipCount);
    mArmRdSmbiosType4.Base.EnabledCoreCount = CoreCount * FixedPcdGet32 (PcdChipCount);
    mArmRdSmbiosType4.Base.ThreadCount = CoreCount * FixedPcdGet32 (PcdChipCount);
    break;
  case RdE1Edge:
    mArmRdSmbiosType4.Base.CoreCount = CoreCount / NEOVERSE_E1_THREADS_PER_CORE;
    mArmRdSmbiosType4.Base.EnabledCoreCount = CoreCount / NEOVERSE_E1_THREADS_PER_CORE;
    mArmRdSmbiosType4.Base.ThreadCount = CoreCount;
    mArmRdSmbiosType4.Base.MaxSpeed = 2300;      // Frequency in MHz
    mArmRdSmbiosType4.Base.CurrentSpeed = 2300;  // Frequency in MHz
    break;
  }

  mArmRdSmbiosType4.Base.ProcessorVersion = ProcessorVersionBase + ProductId;
  mArmRdSmbiosType4.Base.SerialNumber = SerialNumberBase + ProductId;

  /* Update processor-id and part number */
  *ProcessorId = ArmReadMidr ();
  UpdatePartNumber (*ProcessorId);

  /* Install type 4 table */
  Status = Smbios->Add (
                     Smbios,
                     NULL,
                     &SmbiosHandle,
                     (EFI_SMBIOS_TABLE_HEADER *)&mArmRdSmbiosType4
                     );
  if (Status != EFI_SUCCESS) {
    DEBUG ((
      DEBUG_ERROR,
      "SMBIOS: Failed to install Type4 SMBIOS table.\n"
      ));
  }

  return Status;
}
