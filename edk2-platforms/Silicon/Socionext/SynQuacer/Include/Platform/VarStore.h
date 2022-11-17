/** @file

  Copyright (c) 2017 - 2018, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __VARSTORE_H__
#define __VARSTORE_H__

#define SYNQUACER_PLATFORM_VARIABLE_NAME  L"SynQuacerPlatformSettings"

#define EMMC_DISABLED                     0x0
#define EMMC_ENABLED                      0x1

#define PCIE_MAX_SPEED_UNLIMITED          0x0
#define PCIE_MAX_SPEED_GEN1               0x1

#define ACPIPREF_DT                       0x0
#define ACPIPREF_ACPI                     0x1

typedef struct {
  UINT8         EnableEmmc;
  UINT8         PcieSlot0MaxSpeed;
  UINT8         PcieSlot1MaxSpeed;
  UINT8         PcieSlot2MaxSpeed;
  UINT8         AcpiPref;
  UINT8         Reserved[3];
} SYNQUACER_PLATFORM_VARSTORE_DATA;

#endif
