/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/MmPciBaseLib.h>
#include <PchAccess.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchPmcLib.h>

/**
  Query PCH to determine the Pm Status
  NOTE:
  It's matter when did platform code use this library, since some status could be cleared by write one clear.
  Therefore this funciton is not always return the same result in one boot.
  It's suggested that platform code read this status in the beginning of post.
  For the ColdBoot case, this function only returns one case of the cold boot. Some cold boot case might
  depends on the power cycle scenario and should check with different condtion.

  @param[in] PmStatus - The Pch Pm Status to be probed

  @retval Return TRUE if Status querried is Valid or FALSE if otherwise
**/
BOOLEAN
GetPchPmStatus (
  PCH_PM_STATUS PmStatus
  )
{
  UINTN  PmcRegBase;
  UINT32 PchPwrmBase;
  UINT32 PmConA;
  UINT32 PmConB;
  UINT32 GblRst0;

  PmcRegBase = MmPciBase (
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_PMC,
                 PCI_FUNCTION_NUMBER_PCH_PMC
                 );
  PchPwrmBaseGet (&PchPwrmBase);

  PmConA     = MmioRead32 (PmcRegBase + R_PCH_PMC_GEN_PMCON_A);
  PmConB     = MmioRead32 (PmcRegBase + R_PCH_PMC_GEN_PMCON_B);
  GblRst0    = MmioRead32 (PchPwrmBase + R_PCH_PWRM_124);

  switch(PmStatus){
        case PchWarmBoot:

      if (PmConA & B_PCH_PMC_GEN_PMCON_A_MEM_SR) {
        return TRUE;
      }
      break;

    case PwrFlr:
      if (PmConB & B_PCH_PMC_GEN_PMCON_B_PWR_FLR) {
        return TRUE;
      }
      break;

    case PwrFlrSys:
      if (GblRst0 & BIT12) {
        return TRUE;
      }
      break;

    case PwrFlrPch:
      if (GblRst0 & BIT11) {
        return TRUE;
      }
      break;

        case PchColdBoot:
      ///
      /// Check following conditions for cold boot.
      ///
      if ((GblRst0 & BIT11) &&  // PCHPWR_FLR
          (GblRst0 & BIT12) &&  // SYSPWR_FLR
          (!(PmConA & B_PCH_PMC_GEN_PMCON_A_MEM_SR))) {
        return TRUE;
      }
      break;

    default:
      break;
  }

  return FALSE;
}

/**
  Funtion to check if Battery lost or CMOS cleared.

  @reval TRUE  Battery is always present.
  @reval FALSE CMOS is cleared.
**/
BOOLEAN
EFIAPI
PchIsRtcBatteryGood (
  VOID
  )
{
  UINTN    Data;
  UINTN    PmcBaseAddress;

  //
  // Check if the CMOS battery is present
  // Checks RTC_PWR_STS bit in the GEN_PMCON_3 register
  //
  PmcBaseAddress = MmPciBase (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_PMC,
                     PCI_FUNCTION_NUMBER_PCH_PMC
                     );

  Data = MmioRead32 (PmcBaseAddress + R_PCH_PMC_GEN_PMCON_B);
  if ((Data & B_PCH_PMC_GEN_PMCON_B_RTC_PWR_STS) == 0) {
    return TRUE;
  }
  return FALSE;
}

/**
  Funtion to check if DWR occurs

  @reval TRUE DWR occurs
  @reval FALSE Normal boot flow
**/
BOOLEAN
EFIAPI
PchIsDwrFlow (
  VOID
  )
{
  EFI_STATUS  Status;
  UINT32      PchPwrmBase;

  Status = PchPwrmBaseGet (&PchPwrmBase);
  ASSERT (PchPwrmBase != 0);

  if ((PchPwrmBase != 0) &&
      (MmioRead32 (PchPwrmBase + R_PCH_PWRM_HPR_CAUSE0) & B_PCH_PWRM_HPR_CAUSE0_GBL_TO_HOST)) {
    return TRUE;
  } else {
    return FALSE;
  }
}
