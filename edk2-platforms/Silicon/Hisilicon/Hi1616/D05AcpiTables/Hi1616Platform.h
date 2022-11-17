/** @file
*
*  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
*  Copyright (c) 2015-2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015-2016, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*  Based on the files under ArmPlatformPkg/ArmJunoPkg/AcpiTables/
*
**/


#ifndef _HI1610_PLATFORM_H_
#define _HI1610_PLATFORM_H_

#include <IndustryStandard/Acpi.h>
#include <PlatformArch.h>

#define HI1616_WATCHDOG_COUNT  2
#define HI1616_GIC_STRUCTURE_COUNT  64

#define HI1616_MPID_TA_BASE  0x10000
#define HI1616_MPID_TB_BASE  0x30000
#define HI1616_MPID_TA_2_BASE  0x50000
#define HI1616_MPID_TB_2_BASE  0x70000

// Differs from Juno, we have another affinity level beyond cluster and core
#define PLATFORM_GET_MPID_TA(ClusterId, CoreId)   (HI1616_MPID_TA_BASE | ((ClusterId) << 8) | (CoreId))
#define PLATFORM_GET_MPID_TB(ClusterId, CoreId)   (HI1616_MPID_TB_BASE | ((ClusterId) << 8) | (CoreId))
#define PLATFORM_GET_MPID_TA_2(ClusterId, CoreId)   (HI1616_MPID_TA_2_BASE | ((ClusterId) << 8) | (CoreId))
#define PLATFORM_GET_MPID_TB_2(ClusterId, CoreId)   (HI1616_MPID_TB_2_BASE | ((ClusterId) << 8) | (CoreId))

//
// Multiple APIC Description Table
//
#pragma pack (1)

typedef struct {
  EFI_ACPI_6_1_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER   Header;
  EFI_ACPI_6_1_GIC_STRUCTURE                            GicInterfaces[HI1616_GIC_STRUCTURE_COUNT];
  EFI_ACPI_6_1_GIC_DISTRIBUTOR_STRUCTURE                GicDistributor;
  EFI_ACPI_6_1_GIC_ITS_STRUCTURE                        GicITS[8];
} EFI_ACPI_6_1_MULTIPLE_APIC_DESCRIPTION_TABLE;

#pragma pack ()

#endif
