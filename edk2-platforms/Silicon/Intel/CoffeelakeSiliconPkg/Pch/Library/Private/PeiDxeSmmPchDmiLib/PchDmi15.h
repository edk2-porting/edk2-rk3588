/** @file
  Internal header file for PCH DMI library for SIP15

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __PCH_DMI_15_H__
#define __PCH_DMI_15_H__

#include <Private/Library/PchDmiLib.h>
#include <Private/Library/PeiPchDmiLib.h>

/**
  This function checks if DMI SIP15 Secured Register Lock (SRL) is set

  @retval SRL state
**/
BOOLEAN
IsPchDmi15Locked (
  VOID
  );

/**
  Set DMI thermal throttling to recommended configuration.
  It's intended only for P-DMI SIP15.
**/
VOID
PchDmi15SetRecommendedThermalThrottling (
  VOID
  );

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
  );

/**
  Enable PCIe Relaxed Order for DMI SIP15
**/
VOID
PchDmi15EnablePcieRelaxedOrder (
  VOID
  );

/**
  This function will switch SAI value to be driven to IOSF Primary Fabric
  for cycles with Core BDF from HOSTIA_BOOT_SAI to HOSTIA_POSTBOOT_SAI.
  To be used when PCH is paired with CFL CPU.
**/
VOID
PchDmi15EnablePostBootSai (
  VOID
  );

/**
  This function will do necessary configuration after platform
  should have switched to POSTBOOT_SAI. It needs to be called even if
  POSTBOOT_SAI was not set.
**/
VOID
PchDmi15ConfigAfterPostBootSai (
  VOID
  );

/**
 Secure Register Lock data

 @param[out] SrlRegOffset        Register offset holding Secure Register Lock setting
 @param[out] SrlRegMask          Mask for Secure Register Lock setting
**/
VOID
PchDmi15SrlRegData (
  OUT UINT16  *SrlRegOffset,
  OUT UINT32  *SrlRegMask
  );

/**
  Get PCH DMI SIP15 Virtual Channel Control and Status registers

  @param[in]  Vc                   The virtual channel number for programing
  @param[out] DmiVcCtlAddress      DMI Virtual Channel Control register address
  @param[out] DmiVcStsAddress      DMI Virtual Channel Status register address
**/
VOID
PchDmi15VcRegs (
  IN   PCH_DMI_VC_TYPE  Vc,
  OUT  UINT16           *DmiVcCtlAddress,
  OUT  UINT16           *DmiVcStsAddress
  );

/**
  The function sets the Target Link Speed to GEN 3 in P-DMI SIP15.

  @param[in] TargetLinkSpeed        Target Link Speed
                                    2: GEN2
                                    3: GEN3
**/
VOID
PchDmi15SetTargetLinkSpeed (
  IN  UINT8                 TargetLinkSpeed
  );

#endif
