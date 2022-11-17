/** @file

  Copyright (c) 2020 Jared McNeill. All rights reserved.
  Copyright (c) 2020 Andrey Warkentin <andrey.warkentin@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "GenericPhy.h"

#define PHY_RESET_TIMEOUT       500

/**
  Perform a PHY register read.

  @param  Phy[in]      Pointer to GENERIC_PHY_PRIVATE_DATA.
  @param  PhyAddr[in]  PHY address.
  @param  Reg[in]      PHY register.
  @param  Data[out]    Pointer to register data read.

  @retval EFI_SUCCESS       Data read successfully.
  @retval EFI_DEVICE_ERROR  Failed to read data.

**/
STATIC
EFI_STATUS
GenericPhyRead (
  IN  GENERIC_PHY_PRIVATE_DATA    *Phy,
  IN  UINT8                       PhyAddr,
  IN  UINT8                       Reg,
  OUT UINT16                      *Data
  )
{
  return Phy->Read (Phy->PrivateData, PhyAddr, Reg, Data);
}

/**
  Perform a PHY register write.

  @param  Phy[in]      Pointer to GENERIC_PHY_PRIVATE_DATA.
  @param  PhyAddr[in]  PHY address.
  @param  Reg[in]      PHY register.
  @param  Data[in]     Pointer to register data to write.

  @retval EFI_SUCCESS  Data written successfully.
  @retval EFI_DEVICE_ERROR  Failed to write data.

**/
STATIC
EFI_STATUS
GenericPhyWrite (
  IN GENERIC_PHY_PRIVATE_DATA   *Phy,
  IN UINT8                      PhyAddr,
  IN UINT8                      Reg,
  IN UINT16                     Data
  )
{
  return Phy->Write (Phy->PrivateData, PhyAddr, Reg, Data);
}

/**
  Process a PHY link speed change (e.g. with MAC layer).

  @param  Phy[in]     Pointer to GENERIC_PHY_PRIVATE_DATA.
  @param  Speed[in]   Speed setting.
  @param  Duplex[in]  Duplex setting.

**/
STATIC
VOID
GenericPhyConfigure (
  IN GENERIC_PHY_PRIVATE_DATA   *Phy,
  IN GENERIC_PHY_SPEED          Speed,
  IN GENERIC_PHY_DUPLEX         Duplex
  )
{
  Phy->Configure (Phy->PrivateData, Speed, Duplex);
}

/**
  Detect address for the first PHY seen, probing all possible addresses.

  @param  Phy[in]  Pointer to GENERIC_PHY_PRIVATE_DATA.

  @retval EFI_SUCCESS       Found a PHY and programmed Phy->PhyAddr
  @retval EFI_DEVICE_ERROR  Error reading/writing a PHY register.
  @retval EFI_NOT_FOUND     No PHY detected.

**/
STATIC
EFI_STATUS
GenericPhyDetect (
  IN GENERIC_PHY_PRIVATE_DATA   *Phy
  )
{
  EFI_STATUS  Status;
  UINT8       PhyAddr;
  UINT16      Id1, Id2;

  for (PhyAddr = 0; PhyAddr < 32; PhyAddr++) {
    Status = GenericPhyRead (Phy, PhyAddr, GENERIC_PHY_PHYIDR1, &Id1);
    if (EFI_ERROR (Status)) {
      continue;
    }
    Status = GenericPhyRead (Phy, PhyAddr, GENERIC_PHY_PHYIDR2, &Id2);
    if (EFI_ERROR (Status)) {
      continue;
    }
    if (Id1 != 0xFFFF && Id2 != 0xFFFF) {
      Phy->PhyAddr = PhyAddr;
      DEBUG ((DEBUG_INFO,
        "%a: PHY detected at address 0x%02X (PHYIDR1=0x%04X, PHYIDR2=0x%04X)\n",
        __FUNCTION__, PhyAddr, Id1, Id2));
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Start link auto-negotiation on a PHY.

  @param  Phy[in]  Pointer to GENERIC_PHY_PRIVATE_DATA.

  @retval EFI_SUCCESS       Auto-netogiation started.
  @retval EFI_DEVICE_ERROR  PHY register read/write error.

**/
STATIC
EFI_STATUS
GenericPhyAutoNegotiate (
  IN GENERIC_PHY_PRIVATE_DATA   *Phy
  )
{
  EFI_STATUS    Status;
  UINT16        Anar, Gbcr, Bmcr;

  Status = GenericPhyRead (Phy, Phy->PhyAddr, GENERIC_PHY_ANAR, &Anar);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Anar |= GENERIC_PHY_ANAR_100BASETX_FDX |
          GENERIC_PHY_ANAR_100BASETX |
          GENERIC_PHY_ANAR_10BASET_FDX |
          GENERIC_PHY_ANAR_10BASET;
  Status = GenericPhyWrite (Phy, Phy->PhyAddr, GENERIC_PHY_ANAR, Anar);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GenericPhyRead (Phy, Phy->PhyAddr, GENERIC_PHY_GBCR, &Gbcr);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Gbcr |= GENERIC_PHY_GBCR_1000BASET_FDX |
          GENERIC_PHY_GBCR_1000BASET;
  Status = GenericPhyWrite (Phy, Phy->PhyAddr, GENERIC_PHY_GBCR, Gbcr);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GenericPhyRead (Phy, Phy->PhyAddr, GENERIC_PHY_BMCR, &Bmcr);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Bmcr |= GENERIC_PHY_BMCR_ANE |
          GENERIC_PHY_BMCR_RESTART_AN;
  return GenericPhyWrite (Phy, Phy->PhyAddr, GENERIC_PHY_BMCR, Bmcr);
}

/**
  Initialize the first PHY detected, performing a reset and enabling
  auto-negotiation.

  @param  Phy[in]  Pointer to GENERIC_PHY_PRIVATE_DATA.

  @retval EFI_SUCCESS       Auto-negotiation started.
  @retval EFI_DEVICE_ERROR  PHY register read/write error.
  @retval EFI_TIMEOUT       PHY reset time-out.
  @retval EFI_NOT_FOUND     No PHY detected.

**/
EFI_STATUS
EFIAPI
GenericPhyInit (
  IN GENERIC_PHY_PRIVATE_DATA   *Phy
  )
{
  EFI_STATUS    Status;

  ASSERT (Phy->Read != NULL);
  ASSERT (Phy->Write != NULL);
  ASSERT (Phy->Configure != NULL);

  Status = GenericPhyDetect (Phy);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GenericPhyReset (Phy);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GenericPhyAutoNegotiate (Phy);
}

/**
  Perform a PHY reset.

  @param  Phy[in]  Pointer to GENERIC_PHY_PRIVATE_DATA.

  @retval EFI_SUCCESS       Auto-negotiation started.
  @retval EFI_DEVICE_ERROR  PHY register read/write error.
  @retval EFI_TIMEOUT       PHY reset time-out.

**/
EFI_STATUS
EFIAPI
GenericPhyReset (
  IN GENERIC_PHY_PRIVATE_DATA   *Phy
  )
{
  EFI_STATUS    Status;
  UINTN         Retry;
  UINT16        Data;

  // Start reset sequence
  Status = GenericPhyWrite (Phy, Phy->PhyAddr, GENERIC_PHY_BMCR,
             GENERIC_PHY_BMCR_RESET);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Wait up to 500ms for it to complete
  for (Retry = PHY_RESET_TIMEOUT; Retry > 0; Retry--) {
    Status = GenericPhyRead (Phy, Phy->PhyAddr, GENERIC_PHY_BMCR, &Data);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    if ((Data & GENERIC_PHY_BMCR_RESET) == 0) {
      break;
    }
    gBS->Stall (1000);
  }
  if (Retry == 0) {
    return EFI_TIMEOUT;
  }

  if (Phy->ResetAction != NULL) {
    Phy->ResetAction (Phy->PrivateData);
  }

  return EFI_SUCCESS;
}

/**
  Probe link status.

  @param  Phy[in]  Pointer to GENERIC_PHY_PRIVATE_DATA.

  @retval EFI_SUCCESS       Link is up and auto-negotiation is complete.
  @retval EFI_DEVICE_ERROR  PHY register read/write error,

**/
STATIC
EFI_STATUS
GenericPhyGetLinkStatus (
  IN GENERIC_PHY_PRIVATE_DATA   *Phy
  )
{
  EFI_STATUS  Status;
  UINT16      Bmsr;

  Status = GenericPhyRead (Phy, Phy->PhyAddr, GENERIC_PHY_BMSR, &Bmsr);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((Bmsr & GENERIC_PHY_BMSR_LINK_STATUS) == 0) {
   return EFI_TIMEOUT;
  }

  if ((Bmsr & GENERIC_PHY_BMSR_ANEG_COMPLETE) == 0) {
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}

/**
  Return PHY link configuration.

  @param  Phy[in]      Pointer to GENERIC_PHY_PRIVATE_DATA.
  @param  Speed[out]   Pointer to store link speed.
  @param  Duplex[out]  Pointer to store link duplex setting.

  @retval EFI_SUCCESS       Link configuration settings read.
  @retval EFI_DEVICE_ERROR  PHY register read/write error,

**/
STATIC
EFI_STATUS
GenericPhyGetConfig (
  IN  GENERIC_PHY_PRIVATE_DATA  *Phy,
  OUT GENERIC_PHY_SPEED         *Speed,
  OUT GENERIC_PHY_DUPLEX        *Duplex
  )
{
  EFI_STATUS  Status;
  UINT16      Gbcr, Gbsr, Anlpar, Anar;
  UINT16      Gb, An;

  Status = GenericPhyRead (Phy, Phy->PhyAddr, GENERIC_PHY_GBCR, &Gbcr);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = GenericPhyRead (Phy, Phy->PhyAddr, GENERIC_PHY_GBSR, &Gbsr);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = GenericPhyRead (Phy, Phy->PhyAddr, GENERIC_PHY_ANLPAR, &Anlpar);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = GenericPhyRead (Phy, Phy->PhyAddr, GENERIC_PHY_ANAR, &Anar);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Gb = (Gbsr >> 2) & Gbcr;
  An = Anlpar & Anar;

  if ((Gb & (GENERIC_PHY_GBCR_1000BASET_FDX |
             GENERIC_PHY_GBCR_1000BASET)) != 0) {
    *Speed = PHY_SPEED_1000;
    *Duplex = (Gb & GENERIC_PHY_GBCR_1000BASET_FDX) ? PHY_DUPLEX_FULL
                                                    : PHY_DUPLEX_HALF;
  } else if ((An & (GENERIC_PHY_ANAR_100BASETX_FDX |
                    GENERIC_PHY_ANAR_100BASETX)) != 0) {
    *Speed = PHY_SPEED_100;
    *Duplex = (An & GENERIC_PHY_ANAR_100BASETX_FDX) ? PHY_DUPLEX_FULL
                                                    : PHY_DUPLEX_HALF;
  } else {
    *Speed = PHY_SPEED_10;
    *Duplex = (An & GENERIC_PHY_ANAR_10BASET_FDX) ? PHY_DUPLEX_FULL
                                                  : PHY_DUPLEX_HALF;
  }

  DEBUG ((DEBUG_INFO, "%a: Link speed %d Mbps, %a-duplex\n",
    __FUNCTION__, *Speed, *Duplex == PHY_DUPLEX_FULL ? "full" : "half"));

  return EFI_SUCCESS;
}

/**
  Update link status, propagating PHY link state into the MAC layer.

  @param  Phy[in]  Pointer to GENERIC_PHY_PRIVATE_DATA.

  @retval EFI_SUCCESS       Link is up.
  @retval EFI_DEVICE_ERROR  PHY register read/write error.
  @retval EFI_NOT_READY     Link is down.

**/
EFI_STATUS
EFIAPI
GenericPhyUpdateConfig (
  IN GENERIC_PHY_PRIVATE_DATA *Phy
  )
{
  EFI_STATUS          Status;
  GENERIC_PHY_SPEED   Speed;
  GENERIC_PHY_DUPLEX  Duplex;
  BOOLEAN             LinkUp;

  Status = GenericPhyGetLinkStatus (Phy);
  LinkUp = EFI_ERROR (Status) ? FALSE : TRUE;

  if (Phy->LinkUp != LinkUp) {
    if (LinkUp) {
      DEBUG ((DEBUG_VERBOSE, "%a: Link is up\n", __FUNCTION__));

      Status = GenericPhyGetConfig (Phy, &Speed, &Duplex);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      GenericPhyConfigure (Phy, Speed, Duplex);
    } else {
      DEBUG ((DEBUG_VERBOSE, "%a: Link is down\n", __FUNCTION__));
    }
  }

  Phy->LinkUp = LinkUp;

  return LinkUp ? EFI_SUCCESS : EFI_NOT_READY;
}
