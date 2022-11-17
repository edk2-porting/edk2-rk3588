/** @file

Copyright (c) 2021, American Megatrends International LLC. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/


#include <PiDxe.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>


/**
 *  This is the main function for this feature. This will handle finding and launching
 *  the Linux kernel.
 *
 *  @note   In general, this function will never return to BDS. The LINUXBOOT_ALLOW_RETURN_TO_BDS
 *          token will allow you to return to BDS if the kernel fails to launch for some reason.
 *
 *  @param  None
 *
 *  @retval None
 */

EFI_STATUS
LinuxBootStart (
  VOID
  )
{

    return EFI_SUCCESS;
}


