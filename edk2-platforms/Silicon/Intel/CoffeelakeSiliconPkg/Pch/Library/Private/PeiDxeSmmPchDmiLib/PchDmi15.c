/** @file
  This file contains functions for PCH DMI SIP15

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Private/Library/PchDmiLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Register/PchRegsDmi.h>
#include <Register/PchRegsDmi15.h>
#include <Register/PchRegsPcr.h>

/**
  This function checks if DMI SIP15 Secured Register Lock (SRL) is set

  @retval SRL state
**/
BOOLEAN
IsPchDmi15Locked (
  VOID
  )
{
  return ((PchPcrRead32 (PID_DMI, R_PCH_DMI15_PCR_MPC) & B_PCH_DMI15_PCR_MPC_SRL) != 0);
}

/**
  Set DMI thermal throttling to recommended configuration.
  It's intended only for P-DMI SIP15.
**/
VOID
PchDmi15SetRecommendedThermalThrottling (
  VOID
  )
{
  UINT32  Data32And;
  UINT32  Data32Or;
  ///
  /// DMI recommended Thermal Sensor Target Width
  /// is the HW default configuration:
  ///  - Thermal Sensor 3 Target Width: 0 (x1)
  ///  - Thermal Sensor 2 Target Width: 1 (x2)
  ///  - Thermal Sensor 1 Target Width: 2 (x4)
  ///  - Thermal Sensor 0 Target Width: 3 (x8)
  /// Enable Thermal Sensor Autonomous Width
  ///
  Data32And = (UINT32)~(B_PCH_DMI15_PCR_UPHWAWC_TS3TW | B_PCH_DMI15_PCR_UPHWAWC_TS2TW |
                        B_PCH_DMI15_PCR_UPHWAWC_TS1TW | B_PCH_DMI15_PCR_UPHWAWC_TS0TW);
  Data32Or  = (0 << N_PCH_DMI15_PCR_UPHWAWC_TS3TW) |
              (1 << N_PCH_DMI15_PCR_UPHWAWC_TS2TW) |
              (2 << N_PCH_DMI15_PCR_UPHWAWC_TS1TW) |
              (3 << N_PCH_DMI15_PCR_UPHWAWC_TS0TW) |
              B_PCH_DMI15_PCR_UPHWAWC_TSAWEN;

  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI15_PCR_UPHWAWC, Data32And, Data32Or);
}

/**
  Set DMI thermal throttling to custom configuration.
  This function will configure Thermal Sensor 0/1/2/3 TargetWidth and set
  DMI Thermal Sensor Autonomous Width Enable.
  It's intended only for P-DMI SIP15.

  @param[in] DmiThermalThrottling        DMI Thermal Throttling structure.
**/
VOID
PchDmi15SetCustomThermalThrottling (
  IN DMI_THERMAL_THROTTLING      DmiThermalThrottling
  )
{
  UINT32  Data32And;
  UINT32  Data32Or;

  ///
  /// DMI Throttling action
  ///
  Data32And = (UINT32)~(B_PCH_DMI15_PCR_UPHWAWC_TS3TW | B_PCH_DMI15_PCR_UPHWAWC_TS2TW |
                        B_PCH_DMI15_PCR_UPHWAWC_TS1TW | B_PCH_DMI15_PCR_UPHWAWC_TS0TW);
  Data32Or  = (DmiThermalThrottling.ThermalSensor3TargetWidth << N_PCH_DMI15_PCR_UPHWAWC_TS3TW) |
              (DmiThermalThrottling.ThermalSensor2TargetWidth << N_PCH_DMI15_PCR_UPHWAWC_TS2TW) |
              (DmiThermalThrottling.ThermalSensor1TargetWidth << N_PCH_DMI15_PCR_UPHWAWC_TS1TW) |
              (DmiThermalThrottling.ThermalSensor0TargetWidth << N_PCH_DMI15_PCR_UPHWAWC_TS0TW) |
              B_PCH_DMI15_PCR_UPHWAWC_TSAWEN;

  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI15_PCR_UPHWAWC, Data32And, Data32Or);
}


/**
 Secure Register Lock data

 @param[out] SrlRegOffset        Register offset holding Secure Register Lock setting
 @param[out] SrlRegMask          Mask for Secure Register Lock setting
**/
VOID
PchDmi15SrlRegData (
  OUT UINT16  *SrlRegOffset,
  OUT UINT32  *SrlRegMask
  )
{
  *SrlRegMask = B_PCH_DMI15_PCR_MPC_SRL;
  *SrlRegOffset = R_PCH_DMI15_PCR_MPC;
}
