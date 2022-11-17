/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*  Based on the files under Platform/ARM/JunoPkg/AcpiTables/
*
**/

#ifndef _PPTT_H_
#define _PPTT_H_

#include <PlatformArch.h>
#include <IndustryStandard/Acpi.h>
#include <Library/ArmLib/ArmLibPrivate.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/AcpiTable.h>

#define PPTT_VENDOR_ID             SIGNATURE_32('H', 'I', 'S', 'I')

#define EFI_ACPI_MAX_NUM_TABLES    20

#define MAX_SCL_PER_SOCKET         MAX_DIE
#define MAX_SCL                    (MAX_SOCKET * MAX_SCL_PER_SOCKET)
#define MAX_CLUSTER_PER_SCL        8

#define PPTT_TABLE_MAX_LEN         0x6000
#define PPTT_SOCKET_COMPONENT_NO   0x1
#define PPTT_CACHE_NO              0x4

typedef union {
  struct {
    UINT32    InD           :1;
    UINT32    Level         :3;
    UINT32    Reserved      :28;
  } Bits;
  UINT32 Data;
} CSSELR_DATA;

typedef union {
  struct {
    UINT32    LineSize           :3;
    UINT32    Associativity      :10;
    UINT32    NumSets            :15;
    UINT32    Wa                 :1;
    UINT32    Ra                 :1;
    UINT32    Wb                 :1;
    UINT32    Wt                 :1;
  } Bits;
  UINT32 Data;
} CCSIDR_DATA;

#endif    // _PPTT_H_

