/** @file
* NOR flash platform library to be used in StandaloneMM context
*
* This file provides platform callbacks for the NOR flash module that executes
* in the StandaloneMM context. The third NOR flash instance of 64MB size on the
* reference design platform is assigned to be used in the StandaloneMM context.
*
* Copyright (c) 2021, ARM Ltd. All rights reserved.
*
* SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/NorFlashPlatformLib.h>
#include <PiMm.h>
#include <SgiPlatform.h>

//
// 64MB NOR flash connected to CS2 is assigned to be used in StandaloneMM
// context.
//
STATIC NOR_FLASH_DESCRIPTION mNorFlashDevices[] = {
  {
    // NOR-Flash2 assigned for secure storage.
    FixedPcdGet64 (PcdSmcCs2Base),
    FixedPcdGet64 (PcdSmcCs2Base),
    SIZE_256KB * 256,
    SIZE_256KB,
  },
};

/** Allow access to NOR flash

  On the reference design platforms, the access to NOR flash has to be
  explicitly permitted by writing to the FLASH_RWEN bit of the SYSPH_SYS_REG
  register.

  @retval  EFI_SUCCESS  Initialize required to access NOR flash is complete.

**/
EFI_STATUS
NorFlashPlatformInitialization (
  VOID
  )
{
  UINT64 SysRegFlash;

  SysRegFlash = FixedPcdGet64 (PcdSysPeriphSysRegBase) + SGI_SYSPH_SYS_REG_FLASH;
  MmioOr32 (SysRegFlash, SGI_SYSPH_SYS_REG_FLASH_RWEN);
  return EFI_SUCCESS;
}

/** Returns the list of available NOR flash devices

  For the StandaloneMM execution context, return the list of available NOR
  flash devices that are available for use.

  @param[in]   NorFlashDevices  Pointer to array of NOR flash devices.
  @param[in]   Count            Number of elements in the NOR flash devices
                                array.

  @retval  EFI_SUCCESS            Valid set of NOR flash devices is returned.
  @retval  EFI_INVALID_PARAMETER  Pointers to NOR flash devices and/or count is
                                  invalid.

**/
EFI_STATUS
NorFlashPlatformGetDevices (
  OUT NOR_FLASH_DESCRIPTION   **NorFlashDevices,
  OUT UINT32                  *Count
  )
{
  if ((NorFlashDevices == NULL) || (Count == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *NorFlashDevices = mNorFlashDevices;
  *Count = ARRAY_SIZE (mNorFlashDevices);
  return EFI_SUCCESS;
}
