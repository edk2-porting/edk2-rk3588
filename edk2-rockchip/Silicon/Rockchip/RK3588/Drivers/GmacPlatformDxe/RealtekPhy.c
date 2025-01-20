/** @file
 *
 *  Copyright (c) 2022, Jared McNeill <jmcneill@invisible.ca>
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>

#include "EthernetPhy.h"

/* Realtek RTL8211F PHY registers */
#define PAGSR      0x1F
#define LCR        0x10
#define LCR_VALUE  0x6940

STATIC
VOID
RTL8211FPhyInit (
  IN EFI_PHYSICAL_ADDRESS  GmacBase
  )
{
  PhyWrite (GmacBase, 0, PAGSR, 0xD04);
  MicroSecondDelay (10000);
  PhyWrite (GmacBase, 0, LCR, LCR_VALUE);
  MicroSecondDelay (10000);
  PhyWrite (GmacBase, 0, PAGSR, 0);
}

EFI_STATUS
EFIAPI
RealtekPhyInit (
  IN EFI_PHYSICAL_ADDRESS  GmacBase,
  IN UINT32                PhyId
  )
{
  switch (PhyId) {
    case 0x001CC916:
      DEBUG ((DEBUG_INFO, "%a: Found Realtek RTL8211F GbE PHY\n", __func__));
      RTL8211FPhyInit (GmacBase);
      break;
    case 0x001CC878:
      DEBUG ((DEBUG_INFO, "%a: Found Realtek RTL8211F-VD GbE PHY\n", __func__));
      RTL8211FPhyInit (GmacBase);
      break;
    default:
      return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}
