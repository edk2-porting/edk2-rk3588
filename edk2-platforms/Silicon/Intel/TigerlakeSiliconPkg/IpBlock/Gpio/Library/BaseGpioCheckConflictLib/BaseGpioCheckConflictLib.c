/** @file
  Implementation of BaseGpioCheckConflictLib.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/GpioCheckConflictLib.h>
#include <Uefi/UefiMultiPhase.h>
#include <Pi/PiBootMode.h>
#include <Pi/PiHob.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/GpioPrivateLib.h>

/**
  Check Gpio PadMode conflict and report it.

  @retval     none.
**/
VOID
GpioCheckConflict (
  VOID
  )
{
  EFI_HOB_GUID_TYPE              *GpioCheckConflictHob;
  GPIO_PAD_MODE_INFO             *GpioCheckConflictHobData;
  UINT32                          HobDataSize;
  UINT32                          GpioCount;
  UINT32                          GpioIndex;
  GPIO_CONFIG                     GpioActualConfig;

  GpioCheckConflictHob = NULL;
  GpioCheckConflictHobData = NULL;

  DEBUG ((DEBUG_INFO, "GpioCheckConflict Start..\n"));

  //
  // Use Guid to find HOB.
  //
  GpioCheckConflictHob = (EFI_HOB_GUID_TYPE *) GetFirstGuidHob (&gGpioCheckConflictHobGuid);
  if (GpioCheckConflictHob == NULL) {
    DEBUG ((DEBUG_INFO, "[GPIO Conflict Check] No GPIO HOB found.\n"));
  } else {
    while (GpioCheckConflictHob != NULL) {
      //
      // Find the Data area pointer and Data size from the Hob
      //
      GpioCheckConflictHobData = (GPIO_PAD_MODE_INFO *) GET_GUID_HOB_DATA (GpioCheckConflictHob);
      HobDataSize = GET_GUID_HOB_DATA_SIZE (GpioCheckConflictHob);

      GpioCount = HobDataSize / sizeof (GPIO_PAD_MODE_INFO);
      DEBUG ((DEBUG_INFO, "[GPIO Conflict Check] Hob : GpioCount =  %d\n", GpioCount));

      //
      // Probe Gpio entries in Hob and compare which are conflicted
      //
      for (GpioIndex = 0; GpioIndex < GpioCount ; GpioIndex++) {
        GpioGetPadConfig (GpioCheckConflictHobData[GpioIndex].GpioPad, &GpioActualConfig);
        if (GpioCheckConflictHobData[GpioIndex].GpioPadMode != GpioActualConfig.PadMode) {
          DEBUG ((DEBUG_ERROR, "[GPIO Conflict Check] Identified conflict on pad %a (actual: 0x%X, expected: 0x%X)\n",
                  GpioName (GpioCheckConflictHobData[GpioIndex].GpioPad),
                  GpioActualConfig.PadMode,
                  GpioCheckConflictHobData[GpioIndex].GpioPadMode));
        }
      }
      //
      // Find next Hob and return the Hob pointer by the specific Hob Guid
      //
      GpioCheckConflictHob = GET_NEXT_HOB (GpioCheckConflictHob);
      GpioCheckConflictHob = GetNextGuidHob (&gGpioCheckConflictHobGuid, GpioCheckConflictHob);
    }

    DEBUG ((DEBUG_INFO, "GpioCheckConflict End.\n"));
  }

  return;
}

/**
  This libaray will create one Hob for each Gpio config table
  without PadMode is GpioHardwareDefault

  @param[in]  GpioDefinition    Point to Platform Gpio table
  @param[in]  GpioTableCount    Number of Gpio table entries

  @retval     none.
**/
VOID
CreateGpioCheckConflictHob (
  IN GPIO_INIT_CONFIG          *GpioDefinition,
  IN UINT32                    GpioTableCount
  )
{

  UINT32                   Index;
  UINT32                   GpioIndex;
  GPIO_PAD_MODE_INFO       *GpioCheckConflictHobData;
  UINT16                   GpioCount;

  GpioCount = 0;
  GpioIndex = 0;

  DEBUG ((DEBUG_INFO, "CreateGpioCheckConflictHob Start \n"));

  for (Index = 0; Index < GpioTableCount ; Index++) {
    if (GpioDefinition[Index].GpioConfig.PadMode == GpioHardwareDefault) {
      continue;
    } else {
      //
      // Calculate non-default GPIO number
      //
      GpioCount++;
    }
  }

  //
  // Build a HOB tagged with a GUID for identification and returns
  // the start address of GUID HOB data.
  //
  GpioCheckConflictHobData = (GPIO_PAD_MODE_INFO *) BuildGuidHob (&gGpioCheckConflictHobGuid , GpioCount * sizeof (GPIO_PAD_MODE_INFO));

  //
  // Record Non Default Gpio entries to the Hob
  //
  for (Index = 0; Index < GpioTableCount; Index++) {
    if (GpioDefinition[Index].GpioConfig.PadMode == GpioHardwareDefault) {
      continue;
    } else {
      if (GpioCheckConflictHobData != NULL) {
        GpioCheckConflictHobData[GpioIndex].GpioPad = GpioDefinition[Index].GpioPad;
        GpioCheckConflictHobData[GpioIndex].GpioPadMode = GpioDefinition[Index].GpioConfig.PadMode;
        GpioIndex++;
      }
    }
  }

  DEBUG ((DEBUG_INFO, "CreateGpioCheckConflictHob End \n"));
  return;
}
