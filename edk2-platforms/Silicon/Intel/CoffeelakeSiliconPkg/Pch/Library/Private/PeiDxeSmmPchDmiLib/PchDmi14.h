/** @file
  Internal header file for PCH DMI library for SIP14

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __PCH_DMI_14_H__
#define __PCH_DMI_14_H__

#include <Private/Library/PchDmiLib.h>
#include <Private/Library/PeiPchDmiLib.h>

/**
  This function checks if DMI SIP14 Secured Register Lock (SRL) is set

  @retval SRL state
**/
BOOLEAN
IsPchDmi14Locked (
  VOID
  );

/**
  Enable PCIe Relaxed Order for DMI SIP14
**/
VOID
PchDmi14EnablePcieRelaxedOrder (
  VOID
  );

/**
  This function will switch SAI value to be driven to IOSF Primary Fabric
  for cycles with Core BDF from HOSTIA_BOOT_SAI to HOSTIA_POSTBOOT_SAI.
  To be used when PCH is paired with CFL CPU.
**/
VOID
PchDmi14EnablePostBootSai (
  VOID
  );

/**
 Secure Register Lock data

 @param[out] SrlRegOffset        Register offset holding Secure Register Lock setting
 @param[out] SrlRegMask          Mask for Secure Register Lock setting
**/
VOID
PchDmi14SrlRegData (
  OUT UINT16  *SrlRegOffset,
  OUT UINT32  *SrlRegMask
  );

/**
  Get PCH DMI SIP14 Virtual Channel Control and Status registers

  @param[in]  Vc                   The virtual channel number for programing
  @param[out] DmiVcCtlAddress      DMI Virtual Channel Control register address
  @param[out] DmiVcStsAddress      DMI Virtual Channel Status register address
**/
VOID
PchDmi14VcRegs (
  IN   PCH_DMI_VC_TYPE  Vc,
  OUT  UINT16           *DmiVcCtlAddress,
  OUT  UINT16           *DmiVcStsAddress
  );


#endif
