/** @file
 *
 *  Copyright (c) 2022, Jared McNeill <jmcneill@invisible.ca>
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef ETHERNETPHY_H__
#define ETHERNETPHY_H__

typedef
EFI_STATUS
(EFIAPI *ETHERNET_PHY_INIT)(
  IN EFI_PHYSICAL_ADDRESS   GmacBase,
  IN UINT32                 PhyId
  );

VOID
PhyRead (
  IN  EFI_PHYSICAL_ADDRESS  GmacBase,
  IN  UINT8                 Phy,
  IN  UINT16                Reg,
  OUT UINT16                *Value
  );

VOID
PhyWrite (
  IN EFI_PHYSICAL_ADDRESS  GmacBase,
  IN UINT8                 Phy,
  IN UINT16                Reg,
  IN UINT16                Value
  );

EFI_STATUS
EFIAPI
RealtekPhyInit (
  IN EFI_PHYSICAL_ADDRESS  GmacBase,
  IN UINT32                PhyId
  );

EFI_STATUS
EFIAPI
MotorcommPhyInit (
  IN EFI_PHYSICAL_ADDRESS  GmacBase,
  IN UINT32                PhyId
  );

#endif /* ETHERNETPHY_H__ */
