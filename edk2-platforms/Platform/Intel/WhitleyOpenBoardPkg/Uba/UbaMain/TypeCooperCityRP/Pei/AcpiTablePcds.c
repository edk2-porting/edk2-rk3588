/** @file
  ACPI table pcds update.

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiBoardInit.h"
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Guid/PlatformInfo.h>
#include <UncoreCommonIncludes.h>
#include <Cpu/CpuIds.h>

EFI_STATUS
TypeCooperCityRPPlatformUpdateAcpiTablePcds (
  VOID
  )
{
  CHAR8     AcpiNameCpx[]     = "CPXXEPRP";     // Identifies DSDT on CPX builds
  CHAR8     OemTableIdXhci[]    = "xh_nccrb";

  UINTN     Size;
  EFI_STATUS Status;

  EFI_HOB_GUID_TYPE                     *GuidHob;
  EFI_PLATFORM_INFO                     *PlatformInfo;

  GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  PlatformInfo = GET_GUID_HOB_DATA (GuidHob);
  //#
  //#ACPI items
  //#
  Size = AsciiStrSize (AcpiNameCpx);
  Status = PcdSetPtrS (PcdOemSkuAcpiName , &Size, AcpiNameCpx);
  DEBUG ((DEBUG_INFO, "PlatformUpdateAcpiTablePcds TypeCooperCityRP CPX\n"));
  ASSERT_EFI_ERROR (Status);

  Size = AsciiStrSize (OemTableIdXhci);
  Status = PcdSetPtrS (PcdOemTableIdXhci , &Size, OemTableIdXhci);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

