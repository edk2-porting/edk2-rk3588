/** @file
  File to contain all the hardware specific stuff for the Smm Gpi dispatch protocol.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "PchSmmHelpers.h"

//
// Structure for GPI SMI is a template which needs to have
// GPI Smi bit offset and Smi Status & Enable registers updated (accordingly
// to choosen group and pad number) after adding it to SMM Callback database
//

GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC PCH_GPI_SOURCE_DESC_TEMPLATE = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        GPIO_ADDR_TYPE, {0x0}
      },
      S_PCH_GPIO_GP_SMI_EN, 0x0,
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        GPIO_ADDR_TYPE, {0x0}
      },
      S_PCH_GPIO_GP_SMI_STS, 0x0,
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_GPIO_SMI
  }
};
