/** @file
  Platform CMOS Access Library.

  @copyright
  Copyright 2015 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/IoLib.h>
#include <Library/PlatformCmosAccessLib.h>
#include <Platform.h>
#include <Uefi.h>
#include <PiPei.h>
#include <Library/PchInfoLib.h>
#include <Register/PchRegsPcr.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/DynamicSiLibraryPpi.h>
#include <Library/DebugLib.h>

/**
  Return the platform CMOS entries.

  @param [out]  EnryCount Number of platform CMOS entries.

  @return Platform CMOS entries.
**/
CMOS_ENTRY *
EFIAPI
PlatformCmosGetEntry (
  OUT UINTN       *EntryCount
  )
{

  *EntryCount = 0;
  return NULL;
}


/**
  Return the NMI enable status.
**/

BOOLEAN
EFIAPI
PlatformCmosGetNmiState (
  VOID
 )
{
  volatile UINT32           Data32;
  BOOLEAN                   Nmi;
  Data32                    = 0;
  EFI_STATUS                Status = EFI_SUCCESS;
  DYNAMIC_SI_LIBARY_PPI     *DynamicSiLibraryPpi = NULL;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return FALSE;
  }

  //
  // Preserve NMI bit setting
  //

  if ((DynamicSiLibraryPpi->ReadNmiEn ())& B_PCH_IO_NMI_EN_NMI_EN) {
    Nmi = TRUE;
  }
  else
    Nmi = FALSE;

  return Nmi;
}
