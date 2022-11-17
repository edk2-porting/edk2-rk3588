/** @file
  PCH HSIO Library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <PchAccess.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchHsioLib.h>

/**
  The function returns the Port Id and lane owner for the specified lane

  @param[in]  PhyMode             Phymode that needs to be checked
  @param[out] PortId              Common Lane End Point ID
  @param[out] LaneOwner           Lane Owner

  @retval EFI_SUCCESS             Read success
  @retval EFI_INVALID_PARAMETER   Invalid lane number
**/
EFI_STATUS
EFIAPI
PchGetLaneInfo (
  IN  UINT32                            LaneNum,
  OUT UINT8                             *PortId,
  OUT UINT8                             *LaneOwner
  )
{
  PCH_SERIES      PchSeries;
  PCH_GENERATION  PchGeneration;
  UINT32 Los1;
  UINT32 Los2;
  UINT32 Los3;
  UINT32 Los4;

  Los1 = 0;
  Los2 = 0;
  Los3 = 0;
  Los4 = 0;

  PchSeries = GetPchSeries ();
  PchGeneration = GetPchGeneration ();

  if (((LaneNum > 15) && (PchSeries == PchLp)) ||
      ((LaneNum > 29) && (PchSeries == PchH) && (PchGeneration == SklPch)) ||
      (LaneNum > 33) ) {
    return EFI_INVALID_PARAMETER;
  } else if (LaneNum < 8) {
    PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS1_REG_BASE, &Los1);
  } else if (LaneNum < 16) {
    PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS2_REG_BASE, &Los2);
  } else if (LaneNum < 24) {
    PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS3_REG_BASE, &Los3);
  } else {
    PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS4_REG_BASE, &Los4);
  }

  if (PchSeries == PchLp) {
    switch (LaneNum) {
      case 0:
        *LaneOwner = (UINT8) ((Los1 & B_PCH_PCR_FIA_L0O));
        *PortId = PID_MODPHY0;
        break;
      case 1:
        *LaneOwner = (UINT8) ((Los1 & B_PCH_PCR_FIA_L1O) >>4);
        *PortId = PID_MODPHY0;
        break;
      case 2:
        *LaneOwner = (UINT8) ((Los1 & B_PCH_PCR_FIA_L2O) >>8);
        *PortId = PID_MODPHY0;
        break;
      case 3:
        *LaneOwner = (UINT8) ((Los1 & B_PCH_PCR_FIA_L3O) >>12);
        *PortId = PID_MODPHY0;
        break;
      case 4:
        *LaneOwner = (UINT8) ((Los1 & B_PCH_PCR_FIA_L4O) >>16);
        *PortId = PID_MODPHY0;
        break;
      case 5:
        *LaneOwner = (UINT8) ((Los1 & B_PCH_PCR_FIA_L5O) >>20);
        *PortId = PID_MODPHY0;
        break;
      case 6:
        *LaneOwner = (UINT8) ((Los1 & B_PCH_PCR_FIA_L6O) >>24);
        *PortId = PID_MODPHY1;
        break;
      case 7:
        *LaneOwner = (UINT8) ((Los1 & B_PCH_PCR_FIA_L7O) >>28);
        *PortId = PID_MODPHY1;
        break;
      case 8:
        *LaneOwner = (UINT8) (Los2 & B_PCH_PCR_FIA_L8O);
        *PortId = PID_MODPHY1;
        break;
      case 9:
        *LaneOwner = (UINT8) ((Los2 & B_PCH_PCR_FIA_L9O) >>4);
        *PortId = PID_MODPHY1;
        break;
      case 10:
        *LaneOwner = (UINT8) ((Los2 & B_PCH_PCR_FIA_L10O) >>8);
        *PortId = PID_MODPHY2;
        break;
      case 11:
        *LaneOwner = (UINT8) ((Los2 & B_PCH_PCR_FIA_L11O) >>12);
        *PortId = PID_MODPHY2;
        break;
      case 12:
        *LaneOwner = (UINT8) ((Los2 & B_PCH_PCR_FIA_L12O) >>16);
        *PortId = PID_MODPHY2;
        break;
      case 13:
        *LaneOwner = (UINT8) ((Los2 & B_PCH_PCR_FIA_L13O) >>20);
        *PortId = PID_MODPHY2;
        break;
      case 14:
        *LaneOwner = (UINT8) ((Los2 & B_PCH_PCR_FIA_L14O) >>24);
        *PortId = PID_MODPHY2;
        break;
      case 15:
        *LaneOwner = (UINT8) ((Los2 & B_PCH_PCR_FIA_L15O) >>28);
        *PortId = PID_MODPHY2;
        break;
    }
  } else {
    switch (LaneNum) {
      case 0:
        *LaneOwner = (UINT8) ((Los1 & B_PCH_PCR_FIA_L0O));
        *PortId = PID_MODPHY0;
        break;
      case 1:
        *LaneOwner = (UINT8) ((Los1 & B_PCH_PCR_FIA_L1O) >>4);
        *PortId = PID_MODPHY0;
        break;
      case 2:
        *LaneOwner = (UINT8) ((Los1 & B_PCH_PCR_FIA_L2O) >>8);
        *PortId = PID_MODPHY0;
        break;
      case 3:
        *LaneOwner = (UINT8) ((Los1 & B_PCH_PCR_FIA_L3O) >>12);
        *PortId = PID_MODPHY0;
        break;
      case 4:
        *LaneOwner = (UINT8) ((Los1 & B_PCH_PCR_FIA_L4O) >>16);
        *PortId = PID_MODPHY0;
        break;
      case 5:
        *LaneOwner = (UINT8) ((Los1 & B_PCH_PCR_FIA_L5O) >>20);
        *PortId = PID_MODPHY0;
        break;
      case 6:
        *LaneOwner = (UINT8) ((Los1 & B_PCH_PCR_FIA_L6O) >>24);
        *PortId = PID_MODPHY1;
        break;
      case 7:
        *LaneOwner = (UINT8) ((Los1 & B_PCH_PCR_FIA_L7O) >>28);
        *PortId = PID_MODPHY1;
        break;
      case 8:
        *LaneOwner = (UINT8) (Los2 & B_PCH_PCR_FIA_L8O);
        *PortId = PID_MODPHY1;
        break;
      case 9:
        *LaneOwner = (UINT8) ((Los2 & B_PCH_PCR_FIA_L9O) >>4);
        *PortId = PID_MODPHY1;
        break;
      case 10:
        *LaneOwner = (UINT8) ((Los2 & B_PCH_PCR_FIA_L10O) >>8);
        *PortId = PID_MODPHY1;
        break;
      case 11:
        *LaneOwner = (UINT8) ((Los2 & B_PCH_PCR_FIA_L11O) >>12);
        *PortId = PID_MODPHY1;
        break;
      case 12:
        *LaneOwner = (UINT8) ((Los2 & B_PCH_PCR_FIA_L12O) >>16);
        *PortId = PID_MODPHY1;
        break;
      case 13:
        *LaneOwner = (UINT8) ((Los2 & B_PCH_PCR_FIA_L13O) >>20);
        *PortId = PID_MODPHY1;
        break;
      case 14:
        *LaneOwner = (UINT8) ((Los2 & B_PCH_PCR_FIA_L14O) >>24);
        *PortId = PID_MODPHY2;
        break;
      case 15:
        *LaneOwner = (UINT8) ((Los2 & B_PCH_PCR_FIA_L15O) >>28);
        *PortId = PID_MODPHY2;
        break;
      case 16:
        *LaneOwner = (UINT8) (Los3 & B_PCH_PCR_FIA_L16O);
        *PortId = PID_MODPHY2;
        break;
      case 17:
        *LaneOwner = (UINT8) ((Los3 & B_PCH_PCR_FIA_L17O) >>4);
        *PortId = PID_MODPHY2;
        break;
      case 18:
        *LaneOwner = (UINT8) ((Los3 & B_PCH_PCR_FIA_L18O) >>8);
        *PortId = PID_MODPHY2;
        break;
      case 19:
        *LaneOwner = (UINT8) ((Los3 & B_PCH_PCR_FIA_L19O) >>12);
        *PortId = PID_MODPHY2;
        break;
      case 20:
        *LaneOwner = (UINT8) ((Los3 & B_PCH_PCR_FIA_L20O) >>16);
        *PortId = PID_MODPHY2;
        break;
      case 21:
        *LaneOwner = (UINT8) ((Los3 & B_PCH_PCR_FIA_L21O) >>20);
        *PortId = PID_MODPHY2;
        break;
      case 22:
        *LaneOwner = (UINT8) ((Los3 & B_PCH_PCR_FIA_L22O) >>24);
        *PortId = PID_MODPHY3;
        break;
      case 23:
        *LaneOwner = (UINT8) ((Los3 & B_PCH_PCR_FIA_L23O) >>28);
        *PortId = PID_MODPHY3;
        break;
      case 24:
        *LaneOwner = (UINT8) (Los4 & B_PCH_PCR_FIA_L24O);
        *PortId = PID_MODPHY3;
        break;
      case 25:
        *LaneOwner = (UINT8) ((Los4 & B_PCH_PCR_FIA_L25O) >>4);
        *PortId = PID_MODPHY3;
        break;
      case 26:
        *LaneOwner = (UINT8) ((Los4 & B_PCH_PCR_FIA_L26O) >>8);
        *PortId = PID_MODPHY3;
        break;
      case 27:
        *LaneOwner = (UINT8) ((Los4 & B_PCH_PCR_FIA_L27O) >>12);
        *PortId = PID_MODPHY3;
        break;
      case 28:
        *LaneOwner = (UINT8) ((Los4 & B_PCH_PCR_FIA_L28O) >>16);
        *PortId = PID_MODPHY3;
        break;
      case 29:
        *LaneOwner = (UINT8) ((Los4 & B_PCH_PCR_FIA_L29O) >>20);
        *PortId = PID_MODPHY3;
        break;
      case 30:
      case 31:
      case 32:
      case 33:
        //
        // Lanes 30-33 don't have ownership registers and are always configured to PCIE
        //
        *LaneOwner = V_PCH_PCR_FIA_LANE_OWN_PCIEDMI;
        *PortId = PID_MODPHY4;
          break;
    }
  }
  return EFI_SUCCESS;
}

/**
  Determine the lane number of a specified port

  @param[out] LaneNum                   GBE Lane Number

  @retval EFI_SUCCESS                   Lane number valid.
  @retval EFI_UNSUPPORTED               Incorrect input device port
**/
EFI_STATUS
PchGetGbeLaneNum (
  UINT8               *LaneNum
  )
{
  PCH_SERIES  PchSeries;
  UINT32 Los1;
  UINT32 Los2;
  UINT32 Los3;

  PchSeries = GetPchSeries ();
  PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS2_REG_BASE, &Los2);

  if (PchSeries == PchLp) {
    PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS1_REG_BASE, &Los1);
    if (((Los1 & B_PCH_PCR_FIA_L6O) >> 24) == V_PCH_PCR_FIA_LANE_OWN_GBE) {
      *LaneNum = 6;
      return EFI_SUCCESS;
    } else if (((Los1 & B_PCH_PCR_FIA_L7O) >> 28) == V_PCH_PCR_FIA_LANE_OWN_GBE) {
      *LaneNum = 7;
      return EFI_SUCCESS;
    } else if ((Los2 & B_PCH_PCR_FIA_L8O) == V_PCH_PCR_FIA_LANE_OWN_GBE) {
      *LaneNum = 8;
      return EFI_SUCCESS;
    } else if (((Los2 & B_PCH_PCR_FIA_L12O) >> 16) == V_PCH_PCR_FIA_LANE_OWN_GBE) {
      *LaneNum = 12;
      return EFI_SUCCESS;
    } else if (((Los2 & B_PCH_PCR_FIA_L13O) >> 20) == V_PCH_PCR_FIA_LANE_OWN_GBE) {
      *LaneNum = 13;
      return EFI_SUCCESS;
    }
  } else {
    PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS3_REG_BASE, &Los3);
    if (((Los2 & B_PCH_PCR_FIA_L9O) >> 4) == V_PCH_PCR_FIA_LANE_OWN_GBE) {
      *LaneNum = 9;
      return EFI_SUCCESS;
    } else if (((Los2 & B_PCH_PCR_FIA_L10O) >> 8) == V_PCH_PCR_FIA_LANE_OWN_GBE) {
      *LaneNum = 10;
      return EFI_SUCCESS;
    } else if (((Los3 & B_PCH_PCR_FIA_L18O) >> 8) == V_PCH_PCR_FIA_LANE_OWN_GBE) {
      *LaneNum = 18;
      return EFI_SUCCESS;
    } else if (((Los3 & B_PCH_PCR_FIA_L21O) >> 20) == V_PCH_PCR_FIA_LANE_OWN_GBE) {
      *LaneNum = 21;
      return EFI_SUCCESS;
    } else if (((Los3 & B_PCH_PCR_FIA_L22O) >> 24) == V_PCH_PCR_FIA_LANE_OWN_GBE) {
      *LaneNum = 22;
      return EFI_SUCCESS;
    }
  }
  return EFI_UNSUPPORTED;
}

/**
  Determine the lane number of a specified port

  @param[in]  Usb3LaneIndex             USB3 Lane Index
  @param[out] LaneNum                   Lane Number

  @retval EFI_SUCCESS                   Lane number valid.
  @retval EFI_UNSUPPORTED               Incorrect input device port
**/
EFI_STATUS
PchGetUsb3LaneNum (
  UINT32              Usb3LaneIndex,
  UINT8               *LaneNum
  )
{
  PCH_SERIES  PchSeries;
  UINT32 Los1;
  UINT32 Los2;
  UINT32 Los3;
  UINT32 Los4;

  PchSeries = GetPchSeries ();
  PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS1_REG_BASE, &Los1);
  PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS2_REG_BASE, &Los2);

  if (PchSeries == PchLp) {
    switch (Usb3LaneIndex) {
      case 0:
        if ((Los1 & B_PCH_PCR_FIA_L0O) == V_PCH_PCR_FIA_LANE_OWN_USB3) {
          *LaneNum = 0;
          return EFI_SUCCESS;
        }
        break;
      case 1:
        if (((Los1 & B_PCH_PCR_FIA_L1O) >> 4) == V_PCH_PCR_FIA_LANE_OWN_USB3) {
          *LaneNum = 1;
          return EFI_SUCCESS;
        }
        break;
      case 2:
        if (((Los1 & B_PCH_PCR_FIA_L2O) >> 8) == V_PCH_PCR_FIA_LANE_OWN_USB3) {
          *LaneNum = 2;
          return EFI_SUCCESS;
        }
        break;
      case 3:
        if (((Los1 & B_PCH_PCR_FIA_L3O) >> 12) == V_PCH_PCR_FIA_LANE_OWN_USB3) {
          *LaneNum = 3;
          return EFI_SUCCESS;
        }
        break;
      case 4:
        if (((Los1 & B_PCH_PCR_FIA_L4O) >> 16) == V_PCH_PCR_FIA_LANE_OWN_USB3) {
          *LaneNum = 4;
          return EFI_SUCCESS;
        }
        break;
      case 5:
        if (((Los1 & B_PCH_PCR_FIA_L5O) >> 20) == V_PCH_PCR_FIA_LANE_OWN_USB3) {
          *LaneNum = 5;
          return EFI_SUCCESS;
        }
        break;
      default:
        DEBUG ((DEBUG_ERROR, "Unsupported USB3 Lane Index"));
        ASSERT (FALSE);
        return EFI_UNSUPPORTED;
        break;
    }
  } else {
    PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS3_REG_BASE, &Los3);
    PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS4_REG_BASE, &Los4);
    switch (Usb3LaneIndex) {
      case 0:
        if ((Los1 & B_PCH_PCR_FIA_L0O) == V_PCH_PCR_FIA_LANE_OWN_USB3) {
          *LaneNum = 0;
          return EFI_SUCCESS;
        }
        break;
      case 1:
        if (((Los1 & B_PCH_PCR_FIA_L1O) >> 4) == V_PCH_PCR_FIA_LANE_OWN_USB3) {
          *LaneNum = 1;
          return EFI_SUCCESS;
        }
        break;
      case 2:
        if (((Los1 & B_PCH_PCR_FIA_L2O) >> 8) == V_PCH_PCR_FIA_LANE_OWN_USB3) {
          *LaneNum = 2;
          return EFI_SUCCESS;
        }
        break;
      case 3:
        if (((Los1 & B_PCH_PCR_FIA_L3O) >> 12) == V_PCH_PCR_FIA_LANE_OWN_USB3) {
          *LaneNum = 3;
          return EFI_SUCCESS;
        }
        break;
      case 4:
        if (((Los1 & B_PCH_PCR_FIA_L4O) >> 16) == V_PCH_PCR_FIA_LANE_OWN_USB3) {
          *LaneNum = 4;
          return EFI_SUCCESS;
        }
        break;
      case 5:
        if (((Los1 & B_PCH_PCR_FIA_L5O) >> 20) == V_PCH_PCR_FIA_LANE_OWN_USB3) {
          *LaneNum = 5;
          return EFI_SUCCESS;
        }
        break;
      case 6:
        if (((Los1 & B_PCH_PCR_FIA_L6O) >> 24) == V_PCH_PCR_FIA_LANE_OWN_USB3) {
          *LaneNum = 6;
          return EFI_SUCCESS;
        }
        break;
      case 7:
        if (((Los1 & B_PCH_PCR_FIA_L7O) >> 28) == V_PCH_PCR_FIA_LANE_OWN_USB3) {
          *LaneNum = 7;
          return EFI_SUCCESS;
        }
        break;
      case 8:
        if ((Los2 & B_PCH_PCR_FIA_L8O) == V_PCH_PCR_FIA_LANE_OWN_USB3) {
          *LaneNum = 8;
          return EFI_SUCCESS;
        }
        break;
      case 9:
        if (((Los2 & B_PCH_PCR_FIA_L9O) >> 4) == V_PCH_PCR_FIA_LANE_OWN_USB3) {
          *LaneNum = 9;
          return EFI_SUCCESS;
        }
        break;
      default:
        DEBUG ((DEBUG_ERROR, "Unsupported USB3 Lane Index"));
        ASSERT (FALSE);
        return EFI_UNSUPPORTED;
        break;
    }
  }
  return EFI_UNSUPPORTED;
}

/**
  Determine the lane number of a specified port

  @param[in]  SataLaneIndex             Sata Lane Index
  @param[out] LaneNum                   Lane Number

  @retval EFI_SUCCESS                   Lane number valid.
  @retval EFI_UNSUPPORTED               Incorrect input device port
**/
EFI_STATUS
PchGetSataLaneNum (
  UINT32              SataLaneIndex,
  UINT8               *LaneNum
  )
{
  PCH_SERIES  PchSeries;
  UINT32 Los1;
  UINT32 Los2;
  UINT32 Los3;
  UINT32 Los4;

  PchSeries = GetPchSeries ();
  PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS1_REG_BASE, &Los1);
  PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS2_REG_BASE, &Los2);

  if (PchSeries == PchLp) {
    switch (SataLaneIndex) {
      case 0:
        if (((Los2 & B_PCH_PCR_FIA_L10O) >> 8) == V_PCH_PCR_FIA_LANE_OWN_SATA) {
          *LaneNum = 10;
          return EFI_SUCCESS;
        }
        break;
      case 1:
        if (((Los2 & B_PCH_PCR_FIA_L11O) >> 12) == V_PCH_PCR_FIA_LANE_OWN_SATA) {
          *LaneNum = 11;
          return EFI_SUCCESS;
        } else if (((Los2 & B_PCH_PCR_FIA_L14O) >> 24) == V_PCH_PCR_FIA_LANE_OWN_SATA) {
          *LaneNum = 14;
          return EFI_SUCCESS;
        }
        break;
      case 2:
        if (((Los2 & B_PCH_PCR_FIA_L15O) >> 28) == V_PCH_PCR_FIA_LANE_OWN_SATA) {
          *LaneNum = 15;
          return EFI_SUCCESS;
        }
        break;
      default:
        DEBUG ((DEBUG_ERROR, "Unsupported SATA Lane Index"));
        ASSERT (FALSE);
        return EFI_UNSUPPORTED;
        break;
    }
  } else {
    PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS3_REG_BASE, &Los3);
    PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS4_REG_BASE, &Los4);
    switch (SataLaneIndex) {
      case 0:
        if (((Los3 & B_PCH_PCR_FIA_L18O) >> 8) == V_PCH_PCR_FIA_LANE_OWN_SATA) {
          *LaneNum = 18;
          return EFI_SUCCESS;
        } else if (((Los3 & B_PCH_PCR_FIA_L22O) >> 24) == V_PCH_PCR_FIA_LANE_OWN_SATA) {
          *LaneNum = 22;
          return EFI_SUCCESS;
        }
        break;
      case 1:
        if (((Los3 & B_PCH_PCR_FIA_L19O) >> 12) == V_PCH_PCR_FIA_LANE_OWN_SATA) {
          *LaneNum = 19;
          return EFI_SUCCESS;
        } else if (((Los3 & B_PCH_PCR_FIA_L23O) >> 28) == V_PCH_PCR_FIA_LANE_OWN_SATA) {
          *LaneNum = 23;
          return EFI_SUCCESS;
        }
        break;
      case 2:
        if ((Los4 & B_PCH_PCR_FIA_L24O) == V_PCH_PCR_FIA_LANE_OWN_SATA) {
          *LaneNum = 24;
          return EFI_SUCCESS;
        }
        break;
      case 3:
        if (((Los4 & B_PCH_PCR_FIA_L25O) >> 4) == V_PCH_PCR_FIA_LANE_OWN_SATA) {
          *LaneNum = 25;
          return EFI_SUCCESS;
        }
        break;
      case 4:
        if (((Los4 & B_PCH_PCR_FIA_L26O) >> 8) == V_PCH_PCR_FIA_LANE_OWN_SATA) {
          *LaneNum = 26;
          return EFI_SUCCESS;
        }
        break;
      case 5:
        if (((Los4 & B_PCH_PCR_FIA_L27O) >> 12) == V_PCH_PCR_FIA_LANE_OWN_SATA) {
          *LaneNum = 27;
          return EFI_SUCCESS;
        }
        break;
      case 6:
        if (((Los4 & B_PCH_PCR_FIA_L28O) >> 16) == V_PCH_PCR_FIA_LANE_OWN_SATA) {
          *LaneNum = 28;
          return EFI_SUCCESS;
        }
        break;
      case 7:
        if (((Los4 & B_PCH_PCR_FIA_L29O) >> 20) == V_PCH_PCR_FIA_LANE_OWN_SATA) {
          *LaneNum = 29;
          return EFI_SUCCESS;
        }
        break;
      default:
        DEBUG ((DEBUG_ERROR, "Unsupported SATA Lane Index"));
        ASSERT (FALSE);
        return EFI_UNSUPPORTED;
        break;
    }
  }
  return EFI_UNSUPPORTED;
}

/**
  Determine the lane number of a specified port

  @param[in]  PcieLaneIndex             PCIE Root Port Lane Index
  @param[out] LaneNum                   Lane Number

  @retval EFI_SUCCESS                   Lane number valid.
  @retval EFI_UNSUPPORTED               Incorrect input device port
**/
EFI_STATUS
PchGetPcieLaneNum (
  UINT32              PcieLaneIndex,
  UINT8               *LaneNum
  )
{
  PCH_SERIES  PchSeries;
  UINT32 Los1;
  UINT32 Los2;
  UINT32 Los3;
  UINT32 Los4;

  PchSeries = GetPchSeries ();
  PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS1_REG_BASE, &Los1);
  PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS2_REG_BASE, &Los2);

  if (PchSeries == PchLp) {
    switch (PcieLaneIndex) {
      case 0:
        if (((Los1 & B_PCH_PCR_FIA_L4O) >> 16) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 4;
          return EFI_SUCCESS;
        }
        break;
      case 1:
        if (((Los1 & B_PCH_PCR_FIA_L5O) >> 20) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 5;
          return EFI_SUCCESS;
        }
        break;
      case 2:
        if (((Los1 & B_PCH_PCR_FIA_L6O) >> 24) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 6;
          return EFI_SUCCESS;
        }
        break;
      case 3:
        if (((Los1 & B_PCH_PCR_FIA_L7O) >> 28) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 7;
          return EFI_SUCCESS;
        }
        break;
      case 4:
        if ((Los2 & B_PCH_PCR_FIA_L8O) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 8;
          return EFI_SUCCESS;
        }
        break;
      case 5:
        if (((Los2 & B_PCH_PCR_FIA_L9O) >> 4) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 9;
          return EFI_SUCCESS;
        }
        break;
      case 6:
        if (((Los2 & B_PCH_PCR_FIA_L10O) >> 8) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 10;
          return EFI_SUCCESS;
        }
        break;
      case 7:
        if (((Los2 & B_PCH_PCR_FIA_L11O) >> 12) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 11;
          return EFI_SUCCESS;
        }
        break;
      case 8:
        if (((Los2 & B_PCH_PCR_FIA_L12O) >> 16) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 12;
          return EFI_SUCCESS;
        }
        break;
      case 9:
        if (((Los2 & B_PCH_PCR_FIA_L13O) >> 20) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 13;
          return EFI_SUCCESS;
        }
        break;
      case 10:
        if (((Los2 & B_PCH_PCR_FIA_L14O) >> 24) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 14;
          return EFI_SUCCESS;
        }
        break;
      case 11:
        if (((Los2 & B_PCH_PCR_FIA_L15O) >> 28) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 15;
          return EFI_SUCCESS;
        }
        break;
      default:
        DEBUG ((DEBUG_ERROR, "Unsupported PCIE Root Port Lane Index"));
        ASSERT (FALSE);
        return EFI_UNSUPPORTED;
        break;
    }
  } else {
    PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS3_REG_BASE, &Los3);
    PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_LOS4_REG_BASE, &Los4);
    switch (PcieLaneIndex) {
      case 0:
        if (((Los1 & B_PCH_PCR_FIA_L6O) >> 24) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 6;
          return EFI_SUCCESS;
        }
        break;
      case 1:
        if (((Los1 & B_PCH_PCR_FIA_L7O) >> 28) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 7;
          return EFI_SUCCESS;
        }
        break;
      case 2:
        if ((Los2 & B_PCH_PCR_FIA_L8O) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 8;
          return EFI_SUCCESS;
        }
        break;
      case 3:
        if (((Los2 & B_PCH_PCR_FIA_L9O) >> 4) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 9;
          return EFI_SUCCESS;
        }
        break;
      case 4:
        if (((Los2 & B_PCH_PCR_FIA_L10O) >> 8) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 10;
          return EFI_SUCCESS;
        }
        break;
      case 5:
        if (((Los2 & B_PCH_PCR_FIA_L11O) >> 12) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 11;
          return EFI_SUCCESS;
        }
        break;
      case 6:
        if (((Los2 & B_PCH_PCR_FIA_L12O) >> 16) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 12;
          return EFI_SUCCESS;
        }
        break;
      case 7:
        if (((Los2 & B_PCH_PCR_FIA_L13O) >> 20) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 13;
          return EFI_SUCCESS;
        }
        break;
      case 8:
        if (((Los3 & B_PCH_PCR_FIA_L18O) >> 8) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 18;
          return EFI_SUCCESS;
        }
        break;
      case 9:
        if (((Los3 & B_PCH_PCR_FIA_L19O) >> 12) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 19;
          return EFI_SUCCESS;
        }
        break;
      case 10:
        if (((Los3 & B_PCH_PCR_FIA_L20O) >> 16) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 20;
          return EFI_SUCCESS;
        }
        break;
      case 11:
        if (((Los3 & B_PCH_PCR_FIA_L21O) >> 20) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 21;
          return EFI_SUCCESS;
        }
        break;
      case 12:
        if (((Los3 & B_PCH_PCR_FIA_L22O) >> 24) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 22;
          return EFI_SUCCESS;
        }
        break;
      case 13:
        if (((Los3 & B_PCH_PCR_FIA_L23O) >> 28) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 23;
          return EFI_SUCCESS;
        }
        break;
      case 14:
        if ((Los4 & B_PCH_PCR_FIA_L24O) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 24;
          return EFI_SUCCESS;
        }
        break;
      case 15:
        if (((Los4 & B_PCH_PCR_FIA_L25O) >> 4) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 25;
          return EFI_SUCCESS;
        }
        break;
      case 16:
        if (((Los4 & B_PCH_PCR_FIA_L26O) >> 8) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 26;
          return EFI_SUCCESS;
        }
        break;
      case 17:
        if (((Los4 & B_PCH_PCR_FIA_L27O) >> 12) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 27;
          return EFI_SUCCESS;
        }
        break;
      case 18:
        if (((Los4 & B_PCH_PCR_FIA_L28O) >> 16) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 28;
          return EFI_SUCCESS;
        }
        break;
      case 19:
        if (((Los4 & B_PCH_PCR_FIA_L29O) >> 20) == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) {
          *LaneNum = 29;
          return EFI_SUCCESS;
        }
        break;
      case 20:
        if (GetPchGeneration () == KblPch) { // there's no Los register for 4 last PCIe rootports on KBL
          *LaneNum = 30;
          return EFI_SUCCESS;
        } else {
          return EFI_UNSUPPORTED;
        }
      case 21:
        if (GetPchGeneration () == KblPch) { // there's no Los register for 4 last PCIe rootports on KBL
          *LaneNum = 31;
          return EFI_SUCCESS;
        } else {
          return EFI_UNSUPPORTED;
        }
      case 22:
        if (GetPchGeneration () == KblPch) { // there's no Los register for 4 last PCIe rootports on KBL
          *LaneNum = 32;
          return EFI_SUCCESS;
        } else {
          return EFI_UNSUPPORTED;
        }
      case 23:
        if (GetPchGeneration () == KblPch) { // there's no Los register for 4 last PCIe rootports on KBL
          *LaneNum = 33;
          return EFI_SUCCESS;
        } else {
          return EFI_UNSUPPORTED;
        }
      default:
        DEBUG ((DEBUG_ERROR, "Unsupported PCIE Root Port Lane Index"));
        ASSERT (FALSE);
        return EFI_UNSUPPORTED;
        break;
    }
  }
  return EFI_UNSUPPORTED;
}

/**
  Get HSIO lane representation needed to perform any operation on the lane.

  @param[in]  LaneIndex  Number of the HSIO lane
  @param[out] HsioLane   HSIO lane representation
**/
VOID
HsioGetLane (
  IN   UINT8       LaneIndex,
  OUT  HSIO_LANE   *HsioLane
  )
{
  // SPT-LP
  // iolane  0 -  5 : 0xEA - 000, 200, 400, 600, 800, a00
  // iolane  6 -  9 : 0xE9 - 000, 200, 400, 600
  // iolane 10 - 15 : 0xA9 - 000, 200, 400, 600, 800, a00
  // SPT-H
  // iolane  0 -  5 : 0xEA - 000, 200, 400, 600, 800, a00
  // iolane  6 - 13 : 0xE9 - 000, 200, 400, 600, 800, a00, c00, e00
  // iolane 14 - 21 : 0xA9 - 000, 200, 400, 600, 800, a00, c00, e00
  // iolane 22 - 29 : 0xA8 - 000, 200, 400, 600, 800, a00, c00, e00
  // KBL-H: as for SKL-H plus the following lanes
  // iolane 30 - 33 : 0xB0 - 000, 200, 400, 600

  static UINT8 IoLanesLp[] = { 0, 6, 10, 16 };
  static UINT8 IoLanesH[] = { 0, 6, 14, 22, 30, 34 };
  static UINT8 Pids[] = { PID_MODPHY0, PID_MODPHY1, PID_MODPHY2, PID_MODPHY3, PID_MODPHY4 };

  UINT8* IoLanes;
  UINT8  PidMax;
  UINT32 Index;

  ASSERT (HsioLane != NULL);

  if (GetPchSeries () == PchLp) {
    IoLanes = IoLanesLp;
    PidMax = 3;
  } else {
    IoLanes = IoLanesH;
    if (GetPchGeneration () == KblPch) {
      PidMax = 5;
    } else {
      PidMax = 4;
    }
  }
  ASSERT (LaneIndex < IoLanes[PidMax]);

  for (Index = 0; Index < PidMax; ++Index) {
    if (LaneIndex < IoLanes[Index + 1]) {
      HsioLane->Index = LaneIndex;
      HsioLane->Pid = Pids[Index];
      HsioLane->Base = (LaneIndex - IoLanes[Index]) * 0x200;
      return;
    }
  }
  ASSERT (FALSE);
}

